// DispInfoConverter.cpp: implementation of the CDispInfoConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <sicppsdk.h>
#include <xsi_application.h>
#include <xsi_ogltexture.h>
#include <xsi_model.h>
#include <xsi_material.h>
#include <xsi_parameter.h>
#include <xsi_shader.h>
#include <xsi_group.h>
#include <xsi_application.h>
#include <xsi_model.h>
#include <xsi_geometry.h>
#include <xsi_primitive.h>
#include <xsi_polygonmesh.h>
#include <xsi_polygonface.h>
#include <xsi_vector3.h>
#include <xsi_point.h>
#include <xsi_vertex.h>
#include <xsi_polygonnode.h>
#include <xsi_clusterproperty.h>
#include <xsi_triangle.h>

#include "DispInfoConverter.h"
#include <SIBCVector.h>
#include "VMFUtils.h"
#include <Material.h>
#include <PolygonList.h>
#include <Shape.h>
#include <UVCoordArray.h>
#include <Model.h>
#include <GlobalMaterial.h>
#include "VMFTypes.h"


extern int		g_iImportMaterials;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDispInfoConverter::CDispInfoConverter() :
m_pUVs ( 0 ),
m_pNodes(0)
{

}

CDispInfoConverter::~CDispInfoConverter()
{
	if ( m_pUVs != NULL )
		delete [] m_pUVs;

	if ( m_pNodes != NULL )
		delete [] m_pNodes;

	delete [] m_pIndices;
}

void CDispInfoConverter::Convert ( face_t * l_pFace, dispinfo_t* in_pDisp,int texturewidth, int textureheight )
{

	int power = (int)pow (2,(int)in_pDisp->power) +1;

	vec3_t	l_view;
	l_view[0] = 0.0f;
	l_view[1] = 0.0f;
	l_view[2] = 1.0f;

	float d = DotProduct(l_pFace->plane.normal,l_view);

	CSIBCVector3D*	l_pSouthVectors = new CSIBCVector3D [ power ];
	CSIBCVector3D*	l_pEastVectors = new CSIBCVector3D [ power ];
	CSIBCVector3D*	l_pNorthVectors = new CSIBCVector3D [ power ];
	CSIBCVector3D*	l_pWestVectors = new CSIBCVector3D [ power ];

	m_iVertexCount = power * power;

	m_pNodes = new CSIBCVector3D [ power * power ];

	
	float step;
	CSIBCVector3D	l_vAxis;
	step = 1.0f / (power - 1);
	int s;

	//
	// Find the start vertex
	//

	int vertex_winding_indices[4];
	vertex_winding_indices[0] = 0;
	vertex_winding_indices[1] = 1;
	vertex_winding_indices[2] = 2;
	vertex_winding_indices[3] = 3;

	
	for (int ff=0;ff<l_pFace->w->numpoints;ff++)
	{

		float diff1 = fabs( l_pFace->w->points[ff][0] - in_pDisp->startposition[0] );
		float diff2 = fabs( l_pFace->w->points[ff][1] - in_pDisp->startposition[1] );
		float diff3 = fabs( l_pFace->w->points[ff][2] - in_pDisp->startposition[2] );


		if (( diff1 < 0.001f ) && ( diff2 < 0.001f ) && ( diff3 < 0.001f ))
		{
			vertex_winding_indices[0] = ff;
			vertex_winding_indices[1] = ff +1;
			vertex_winding_indices[2] = ff +2;
			vertex_winding_indices[3] = ff +3;

			break;

		}
	}
	
	for (int vv=1;vv<4;vv++)
	{
		if ( vertex_winding_indices[vv] > 3 )
		{
			vertex_winding_indices[vv] -= 4;
		}
	}


	//
	// do the south vector
	//

	l_vAxis = (CSIBCVector3D ( l_pFace->w->points[vertex_winding_indices[3]][0],
								l_pFace->w->points[vertex_winding_indices[3]][1],
								l_pFace->w->points[vertex_winding_indices[3]][2] ) -
				CSIBCVector3D ( l_pFace->w->points[vertex_winding_indices[0]][0],
								l_pFace->w->points[vertex_winding_indices[0]][1],
								l_pFace->w->points[vertex_winding_indices[0]][2] )) * step;


	for (s=0;s<power;s++)
	{
		l_pSouthVectors[s] =  ( l_vAxis * (float)s ) + CSIBCVector3D ( (float)l_pFace->w->points[vertex_winding_indices[0]][0],(float)l_pFace->w->points[vertex_winding_indices[0]][1],(float)l_pFace->w->points[vertex_winding_indices[0]][2] );
	}

	//
	// do the East vector
	//

	l_vAxis = (CSIBCVector3D ( l_pFace->w->points[vertex_winding_indices[2]][0],
								l_pFace->w->points[vertex_winding_indices[2]][1],
								l_pFace->w->points[vertex_winding_indices[2]][2] ) -
				CSIBCVector3D ( l_pFace->w->points[vertex_winding_indices[3]][0],
								l_pFace->w->points[vertex_winding_indices[3]][1],
								l_pFace->w->points[vertex_winding_indices[3]][2] )) * step;


	for (s=0;s<power;s++)
	{
		l_pEastVectors[s] =  ( l_vAxis * (float)s ) + CSIBCVector3D ( (float)l_pFace->w->points[vertex_winding_indices[3]][0],(float)l_pFace->w->points[vertex_winding_indices[3]][1],(float)l_pFace->w->points[vertex_winding_indices[3]][2] );
	}

	//
	// do the North vector
	//

	l_vAxis = (CSIBCVector3D ( l_pFace->w->points[vertex_winding_indices[2]][0],
								l_pFace->w->points[vertex_winding_indices[2]][1],
								l_pFace->w->points[vertex_winding_indices[2]][2] ) -
				CSIBCVector3D ( l_pFace->w->points[vertex_winding_indices[1]][0],
								l_pFace->w->points[vertex_winding_indices[1]][1],
								l_pFace->w->points[vertex_winding_indices[1]][2] )) * step;


	for (s=0;s<power;s++)
	{
		l_pNorthVectors[s] =  ( l_vAxis * (float)s ) + CSIBCVector3D ( (float)l_pFace->w->points[vertex_winding_indices[1]][0],(float)l_pFace->w->points[vertex_winding_indices[1]][1],(float)l_pFace->w->points[vertex_winding_indices[1]][2] );
	}
	
	//
	// do the West vector
	//

	l_vAxis = (CSIBCVector3D ( l_pFace->w->points[vertex_winding_indices[1]][0],
								l_pFace->w->points[vertex_winding_indices[1]][1],
								l_pFace->w->points[vertex_winding_indices[1]][2] ) -
				CSIBCVector3D ( l_pFace->w->points[vertex_winding_indices[0]][0],
								l_pFace->w->points[vertex_winding_indices[0]][1],
								l_pFace->w->points[vertex_winding_indices[0]][2] )) * step;


	for (s=0;s<power;s++)
	{
		l_pWestVectors[s] =  ( l_vAxis * (float)s ) + CSIBCVector3D ( (float)l_pFace->w->points[vertex_winding_indices[0]][0],(float)l_pFace->w->points[vertex_winding_indices[0]][1],(float)l_pFace->w->points[vertex_winding_indices[0]][2] );
	}

	//
	// Plot all the vertex making up the grid
	//

	int pindex = 0;

	for (s=0;s<power;s++)
	{
		l_vAxis = (l_pEastVectors[s] - l_pWestVectors[s] ) * step;

		for (int i=0;i<power;i++)
		{
			m_pNodes[pindex] = ( l_vAxis * (float)i ) + l_pWestVectors[s];
			pindex++;

		}

	}

	//
	// Transform the vertices according to the dispinfo 
	//

//	disp_point[i] = base_point[i] + 
  //                            ( elevation * base_face_normal) +
    //                          ( distances[i] * normals[i] ) +
      //                        ( offsets[i] * offset_normals[i] )  



	CSIBCVector3D	BaseFaceNormal = CSIBCVector3D(l_pFace->plane.normal[0], l_pFace->plane.normal[1], l_pFace->plane.normal[2]);

	for (int v=0;v<power * power;v++)
	{
		CSIBCVector3D result = (BaseFaceNormal * in_pDisp->elevation);
		
		float distance = in_pDisp->distances [v];

		CSIBCVector3D normal = CSIBCVector3D ( in_pDisp->normals [v][0], 
													in_pDisp->normals [v][1], 
														in_pDisp->normals [v][2] );

		CSIBCVector3D offset = CSIBCVector3D ( in_pDisp->offsets [v][0], 
													in_pDisp->offsets [v][1], 
														in_pDisp->offsets [v][2] );

		CSIBCMatrix4x4	l_pOffsetMatrix;
		l_pOffsetMatrix.SetTranslation (  CSIBCVector3D ( in_pDisp->offset_normals [v][0], 
													in_pDisp->offset_normals [v][1], 
													in_pDisp->offset_normals [v][2] ) );

		l_pOffsetMatrix.Multiply( offset, offset );

		CSIBCVector3D offset_normal = CSIBCVector3D ( in_pDisp->offset_normals [v][0], 
													in_pDisp->offset_normals [v][1], 
													in_pDisp->offset_normals [v][2] );


		normal = normal * distance;

		//offset.m_fX *= offset_normal.m_fX;
		//offset.m_fY *= offset_normal.m_fY;
		//offset.m_fZ *= offset_normal.m_fZ;

		m_pNodes[v] = m_pNodes[v] + result + normal + offset;

		//result = result + ( distance * normal ) + ( offset * offset_normal);


	}

	int l_iNumIndex = ( power - 1 ) * ( power - 1);
	m_iNumPoly = l_iNumIndex;

	

	m_pIndices = new int [ l_iNumIndex * 4];

	int findex = 0;
	int polystart = 0;
	int numpoly = 0;
	for (int p=0;p<l_iNumIndex;p++)
	{
		m_pIndices[findex] = polystart;
		m_pIndices[findex+1] = polystart+1;
		m_pIndices[findex+2] = polystart+power+1;
		m_pIndices[findex+3] = polystart+power;
		findex+=4;
		numpoly++;

		if ( numpoly == (power - 1) )
		{
			polystart+=2;
			numpoly = 0;
		} else {
			polystart++;
		}
	}

	//
	// assign UVs
	//

	m_pUVs = new CSIBCVector2D [ power * power ];

	for (int b=0;b<power * power;b++)
	{
		vec3_t	vec;
		vec[0] = m_pNodes[b].m_fX;
		vec[1] = m_pNodes[b].m_fY;
		vec[2] = m_pNodes[b].m_fZ;


		float u = (DotProduct(l_pFace->uaxis,vec) / l_pFace->uscale) + l_pFace->ushift;
		float v = (DotProduct(l_pFace->vaxis,vec) / l_pFace->vscale) + l_pFace->vshift;

		u = u / texturewidth;
		v = v / textureheight;
		v = 1.0f - v;

		m_pUVs[b].Set ( u,v );
	}

	delete [] l_pSouthVectors;
	delete [] l_pEastVectors;
	delete [] l_pWestVectors;
	delete [] l_pNorthVectors;

}

void	CDispInfoConverter::Merge ( CDispInfoConverter* in_pDisp )
{

}

XSI::CRef CDispInfoConverter::CreateGeometry ( XSI::Model in_pRootModel)
{

//	l_pTheMesh->ParentModel()->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_INHERITED);
	//l_pTheMesh->ParentModel()->GlobalMaterial()->SetMaterial(in_pMat );

	//l_pTheMesh->ParentModel()->ParentModel()->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_INHERITED);
	//l_pTheMesh->ParentModel()->ParentModel()->GlobalMaterial()->SetMaterial(in_pMat );



	// Add some position coordinates
	CSIBCArray<CSIBCVector3D> l_Vertices;
	CSIBCArray<CSIBCVector3D> l_Normals;
	CSIBCArray<CSIBCVector2D> l_UVs;
	
//	CSLBaseShape::CSLVector3DArray *l_Vertices = in_pBaseShape->GetVertexList();
	l_Vertices.Resize(m_iVertexCount);
	int vindex = 0;

	// Add some normal coordinates.
	//CSLBaseShape::CSLVector3DArray &l_Normals = *in_pBaseShape->CreateNormals();
	l_Normals.Resize(m_iVertexCount);

	// Add some uv coordinates.

	//CSLUVCoordArray* l_p35UVArray = ((CSLShape_35*)in_pBaseShape)->AddUVCoordArray();
	//l_p35UVArray->SetTextureProjection ( "Texture_Projection" );

	//CSLBaseShape::CSLVector2DArray &l_UVs = *((CSLShape*)in_pBaseShape)->CreateUVCoords();
	l_UVs.Resize(m_iVertexCount);

	for (int v=0;v<m_iVertexCount;v++)
	{
		l_Vertices[v] = m_pNodes[v];
		l_Normals[v] = CSIBCVector3D ( 0.0f, 1.0f, 0.0f );
		l_UVs[v] = m_pUVs[v];
	}


	
	//add a polygon list
	//CSLPolygonList* l_Poly = l_pTheMesh->AddPolygonList();
	
	//l_Poly->SetMaterial ( in_pMat );
	
	//l_Poly->GetPolygonVertexCountList()->Resize( m_iNumPoly );
	int f;
	//for (f=0;f<m_iNumPoly;f++)
	//{
	//	(*l_Poly->GetPolygonVertexCountList() )[f] = 4;
	//}

	//l_Poly->GetVertexIndices()->Resize( m_iNumPoly * 4 );

	//SI_Int *l_pnDataArray = l_Poly->GetVertexIndices()->ArrayPtr();

	//add normals
	//CSLPolygonList::CSLIntArray* l_pNormalArray = l_Poly->CreateNormalIndices();
	//l_pNormalArray->Resize(m_iNumPoly * 4);

	//add UV
	//CSLPolygonList::CSLIntArray* l_pUVArray = l_Poly->AddUVArray();
	//l_pUVArray->Resize(m_iNumPoly * 4);

	//SI_Int *l_normalDataArray = l_pNormalArray->ArrayPtr();
	//SI_Int *l_uvDataArray = l_pUVArray->ArrayPtr();
	
	for (f=0;f<m_iNumPoly*4;f++)
	{
		//l_pnDataArray[f] = m_pIndices[f];
		//l_normalDataArray[f] = m_pIndices[f];
		//l_uvDataArray[f] = m_pIndices[f];

		if ( !(f % 4) )
		{
			polygonData.Add ( 4 );
		}

		polygonData.Add ( m_pIndices[f] );

		m_pDispUV.Extend(1);
		dispUV*	l_pUV = &m_pDispUV[m_pDispUV.GetUsed()-1];

		l_pUV->u = m_pUVs[m_pIndices[f]].m_fX;
		l_pUV->v = m_pUVs[m_pIndices[f]].m_fY;
			
	}


	//
	// XSI SDK TEST
	//

	XSI::Application	app;
	XSI::CValueArray	args(4);
	XSI::CValue			outArg;
	XSI::CStatus		st;

	// Default parameters.
	XSI::CString			l_szFullNameDefault(L"");
	int				l_nHeightDefault	= 0;
	int				l_nWidthDefault		= 0;

	// Changed parameters (by attaching noIcon.pic).
	XSI::CString			l_szFullName(L"noIcon.pic");	//substring!
	int				l_nHeight			= 256;
	int				l_nWidth			= 256;

	// Create some geometry.
	LPWSTR l_wszGeomName;
	DSA2W(&l_wszGeomName,"DispInfo");
	
	//st = in_pRootModel.AddGeometry(L"Cube", L"MeshSurface", l_wszGeomName, l_x3dCube);

//	XSI::CValueArray margs(3);
//	margs[0] = l_x3dCube.GetRef();
//	app.ExecuteCommand(L"FreezeObj",margs, outArg);

//	XSI::PolygonMesh polyMesh( l_x3dCube.GetActivePrimitive().GetGeometry() );

	XSI::MATH::CVector3Array	l_vertices;
	int gg;// positions
	for (gg=0;gg<m_iVertexCount;gg++)
	{
		CSIBCVector3D	l_vtemp;

		l_vtemp = m_pNodes[gg];

		//l_CompressVertex.GetVertex (gg,&l_vtemp);

		XSI::MATH::CVector3	l_vec;
		l_vec.PutX ( l_vtemp.m_fX );
		l_vec.PutY ( l_vtemp.m_fY );
		l_vec.PutZ ( l_vtemp.m_fZ );

		l_vertices.Add ( l_vec );
	}

//	polyMesh.Set ( l_vertices, polygonData );
	
	in_pRootModel.AddPolygonMesh ( l_vertices, polygonData , L"DispInfo", l_x3dCube );


	if ( g_iImportMaterials )
	{
		args[0] = XSI::CValue( XSI::CString(L"") );
		args[1] = XSI::CValue(false);
		args[0] = XSI::CValue(l_x3dCube.GetRef());
		args[1] = XSI::CValue((long)XSI::siTxtUV);
		args[2] = XSI::CValue((long)XSI::siTxtDefaultSpherical);
		args[3] = XSI::CValue(XSI::CString(L"Texture_Support"));

		app.ExecuteCommand( L"CreateTextureSupport", args, outArg );

		XSI::CValueArray	moreargs(1);
		XSI::CValueArray	moreoutargs(3);
		moreargs[0] = l_x3dCube.GetRef();
		app.ExecuteCommand(L"FreezeObj",moreargs, outArg);
	}

	l_Vertices.DisposeData();
	l_Normals.DisposeData();
	l_UVs.DisposeData();

	return l_x3dCube.GetRef ();

}

void CDispInfoConverter::AssignUVs()
{
		//
	// uv
	//

	if ( l_x3dCube.IsValid () )
	{
		XSI::Geometry geom( l_x3dCube.GetActivePrimitive().GetGeometry() );
		XSI::PolygonMesh mesh(l_x3dCube.GetActivePrimitive().GetGeometry());

		XSI::CPointRefArray Geompoints = geom.GetPoints();
		
				
		XSI::CTriangleRefArray triangles(geom.GetTriangles());
		XSI::ClusterProperty UVWProp(l_x3dCube.GetMaterial().GetCurrentUV());
		
		if ( UVWProp.IsValid() )
		{

		XSI::CClusterPropertyElementArray clusterPropertyElements = UVWProp.GetElements();
			XSI::CDoubleArray elementArray = clusterPropertyElements.GetArray();

			XSI::CVertexRefArray l_Vertices = mesh.GetVertices ();

			long totalUvCount = elementArray.GetCount ();
			int cc=0;
			int uvc = 0;
			for (int c=0;c<m_iNumPoly;c++)
			{
				long l_iNumVertex = polygonData[cc];

				cc++;
				for (int i=0;i<l_iNumVertex;i++)
				{
					long l_iID = polygonData[cc];
					cc++;

					elementArray[ uvc * 3 ] = m_pDispUV[uvc].u;

					elementArray[ (uvc * 3) + 1] = m_pDispUV[uvc].v;
					elementArray[ (uvc * 3) + 2] = 0.0f;

					uvc++;
				}
					


			}

			clusterPropertyElements.PutArray(elementArray);
		}
	}

	

}
