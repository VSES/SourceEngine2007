// VMFTemplate.cpp: implementation of the VMFTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VMFTemplate.h"
#include "VMFTypes.h"
#include "VMFMaps.h"
#include <Mesh.h>
#include <BaseShape.h>
#include <Model.h>
#include <Visibility.h>
#include <GlobalMaterial.h>
#include <Transform.h>
#include <MaterialLibrary.h>
#include <Material.h>
#include <XSIMaterial.h>
#include <Scene.h>
#include <PolygonList.h>
#include <CustomPSet.h>
#include <VariantParameter.h>
#include <Shape.h>
#include <Shape_35.h>
#include <Texture2D.h>
#include "VMFUtils.h"
#include <SIBCMatrix.h>
#include <UVCoordArray.h>
#include <XSIShader.h>
#include <ShaderConnectionPoint.h>
#include <SIILTGAFileDriver.h>


#include "C3DVectorCompressor.h"

#include <xsi_ogltexture.h>
#include <xsi_application.h>
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
#include <xsi_trianglevertex.h>
#include <xsi_vector3.h>
#include <xsi_vertexcolor.h>
#include <xsi_uv.h>
#include <xsi_longarray.h>
#include <xsi_polygonmesh.h>
#include <xsi_clusterproperty.h>
#include <xsi_triangle.h>
#include <xsi_point.h>
#include <xsi_vertex.h>
#include <xsi_polygonnode.h>
#include <xsi_imageclip2.h>

void AddParameter ( CSLXSIShader *l_pShader, SI_Char* in_szName, SI_Char* in_szValue );
void AddParameter ( CSLXSIShader *in_pShader, SI_Char* in_szName, SI_Bool in_bValue );
void AddParameter ( CSLXSIShader *in_pShader, SI_Char* in_szName, SI_Long in_lValue );
void AddParameter ( CSLXSIShader *in_pShader, SI_Char* in_szName, SI_Float in_fValue );

extern int	g_pGlobalMdlCounter;
extern int g_count;
extern int		g_iImportMaterials;

int		g_c_template = 0;
int		last_p = 0;

CSIBCArray< TGAMapEntry >	VMFTemplate::m_gTGAMap;
CSIBCArray<MatGroup>		VMFTemplate::m_pGroups;
int							VMFTemplate::m_iTemplateCount = 0;
bool cancelpressed = false;

CSIBCArray<CSIBCVector3D>	VMFTemplate::l_Vertices;
CSIBCArray<CSIBCVector3D>	VMFTemplate::l_Normals;
CSIBCArray<CSIBCVector2D>	VMFTemplate::l_UVs;

void DSW2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch = -1 );
void DSW2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch )
{
	if ( out_sz != NULL && 0 != in_cch )
	{
		out_sz[0] = '\0' ;

		if ( in_wcs != NULL )
		{
			// Right now, we can't really test the return value because wcstombs() is
			// mapped to strcpy() on the Irix o32 build. When this build is not done
			// anymore, the != 0 test should be updated.

			size_t l_iLen = 0;
			l_iLen = ::wcstombs( out_sz, in_wcs, ( in_cch < 0 ) ? ::wcslen(in_wcs) + 1 : (size_t) in_cch ) ;
			assert( (int)l_iLen != -1 );

        		if ( in_cch > 0 ) { out_sz[ in_cch - 1 ] = '\0'; }
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VMFTemplate::VMFTemplate()
{
	m_pParent = NULL;
	m_pMeshModel = NULL;
	cancelpressed = false;
	g_c_template = 0;
	last_p = 0;
	m_iPolygonCount = 0;

	l_Vertices.Reserve ( 2048 );
	l_Normals.Reserve ( 2048 );
	l_UVs.Reserve ( 2048 );
}

VMFTemplate::~VMFTemplate()
{
	m_pPropertyList.DisposeData();
	m_pValueList.DisposeData();

	//for (int c=0;c<m_pChildren.GetUsed();c++)
	//{
	//	VMFTemplate* l_pChild = m_pChildren[c];

	//	delete l_pChild;
	//}

	for (int d=0;d<dispinfo_convert.GetUsed();d++)
	{
		delete dispinfo_convert[d];
	}

	dispinfo_convert.DisposeData();

	m_pChildren.DisposeData();

	m_gTGAMap.DisposeData();

}

void	VMFTemplate::AddProperty ( CSIBCString& in_szName, CSIBCString& in_szValue )
{
	m_pPropertyList.Extend(1);
	m_pPropertyList[m_pPropertyList.GetUsed()-1] = in_szName;

	m_pValueList.Extend(1);
	m_pValueList[m_pValueList.GetUsed()-1] = in_szValue;


}

void	VMFTemplate::GetProperty ( SI_Int in_iIdx, CSIBCString& out_szName, CSIBCString& out_szValue )
{
	out_szName = m_pPropertyList[in_iIdx];
	out_szValue = m_pValueList[in_iIdx];

}

int		VMFTemplate::GetNbProperty ()
{
	return m_pPropertyList.GetUsed();
}

	
void	VMFTemplate::AddChild ( VMFTemplate* in_pChild, VMFTemplate* in_pParent )
{
	m_pChildren.Extend(1);
	m_pChildren[m_pChildren.GetUsed()-1] = in_pChild;

	in_pChild->SetParent ( in_pParent );

}

VMFTemplate* VMFTemplate::GetChild ( SI_Int in_iIdx )
{
	return m_pChildren[in_iIdx];
}

int			VMFTemplate::GetNbChild ()
{
	return m_pChildren.GetUsed();
}

void		VMFTemplate::Read ( SI_Char* seps )
{
	char *token = strtok (NULL, seps);

	char seps_no_space[] = "\t\n\r";

	if ( !strcmp ( m_szName.GetText(),"offset_normals") )
	{
		int a = 10;
	}

	while ( token[0] != '}' )
	{ 
		if ( token[0] == 34 )
		{
			//
			// new property
			//

			char* value = strtok (NULL, seps_no_space);

			char l_fvalue[1024];
			char l_fname[1024];

			int l=strlen(value);

			memset ( l_fvalue, 0, 1024);
			memset ( l_fname, 0, 1024);

			strcpy ( l_fvalue, value );
			strcpy ( l_fname, token );

			l_fvalue[strlen(l_fvalue) -1 ] = 0;
			l_fname[strlen(l_fname) -1 ] = 0;

			AddProperty( CSIBCString ( l_fname+1 ), CSIBCString ( l_fvalue+1 ) );
			
		}

		if (( token[0] != 34 ) && ( token[0] != '{' ) && ( token[0] != '}' ))
		{
			//
			// Found a child
			//

			VMFTemplate*	l_pNewTemplate = new VMFTemplate;
			
			AddChild ( l_pNewTemplate, this );

			l_pNewTemplate->SetName ( token );
			l_pNewTemplate->Read ( seps );
		}

		token = strtok (NULL, seps);
	}
}

CSLModel*	VMFTemplate::Convert( XSI::CComAPIHandler progressBar, XSI::CValue retval, XSI::Model in_pModel, CSLModel *in_pParent, int init )
{
	if (cancelpressed)
		return NULL;

	m_iTemplateCount++;
	g_c_template++;


	if ( m_iTemplateCount > 20 )
		m_iTemplateCount = 0;

	VMFMap	l_pCallback = GetVMFMap ( GetName() );

	CSLModel *l_pNewModel = l_pCallback (this, in_pParent, in_pModel );

	for (int c=0;c<GetNbChild();c++)
	{
		GetChild (c)->Convert ( progressBar, retval, in_pModel, l_pNewModel );
		delete GetChild (c);
	}

	if ( !_SI_STRCMP ( GetName(), "solid" ) )
	{
		bool disp = false;
		if ( m_pBrush.Convert( &disp) == SI_SUCCESS )
		{
	
			if ( !disp )
			{
				CreateGeometry( in_pModel, l_pNewModel, disp);
			}
			CreateDispinfo( in_pModel, l_pNewModel);
			
			//GetMeshModel()->ParentModel()->ClearChildren();
		}
		
	}

	XSI::CValue cancelp;
	
	//if ( !m_iTemplateCount )
	//	cancelp = progressBar.GetProperty( L"CancelPressed" );

	if ( cancelp )
	{
		cancelpressed = true;

	} else {

		float pp = ((float)g_c_template / (float)g_count) * 100.0f;
		if ( ((int)pp) != last_p )
		{
			progressBar.Call( L"Increment", retval );
			progressBar.PutProperty( L"Visible", true );
			last_p = (int)pp;
		}
	}

	//if ( () == true )
	//{
	//	cancelpressed = true;
	//}

	return in_pParent;
}

void VMFTemplate::CreateGeometry( XSI::Model in_pRootModel,CSLModel *in_pModel, bool disp)
{

//	CSLMesh *l_pMesh = (CSLMesh *) GetMeshModel()->Primitive();
	//CSLBaseShape *l_pShape = l_pMesh->Shape();
	//CSIBCString	l_szMaterialName;
	//
	// Count the number of vertices in each face
	//
	int vertexcount = 0;
	int f;
	int p;

	for (f=0;f<m_pBrush.GetNbFace ();f++)
	{
		face_t *l_Face = m_pBrush.GetFace (f);

		if ( l_Face->w )
		{
		 	vertexcount += l_Face->w->numpoints;
		}

	}


	l_Vertices.DisposeData();
	l_Normals.DisposeData();

	l_Vertices.Reserve ( 2048 );
	l_Normals.Reserve ( 2048 );

	//l_Vertices.Reset();
	//l_Normals.Reset();
	//l_UVs.Reset();

	// Add some position coordinates
	//CSLBaseShape::CSLVector3DArray *l_Vertices = l_pShape->GetVertexList();
	
	// Add some normal coordinates.
	//CSLBaseShape::CSLVector3DArray &l_Normals = *l_pShape->CreateNormals();
	

	C3DVectorCompressor	l_CompressVertex;
	C3DVectorCompressor	l_CompressNormal;
//	C3DVectorCompressor	l_CompressUV;

	//l_CompressUV.SetCompress ( FALSE );
	l_CompressNormal.SetCompress ( FALSE );

	int debugcount = 0;

	for (f=0;f<m_pBrush.GetNbFace ();f++)
	{
		face_t *l_Face = m_pBrush.GetFace (f);

		if ( !l_Face->w )
			continue;
	
		for (p=0;p<l_Face->w->numpoints;p++)
		{

			l_CompressVertex.AddVertex (l_Face->w->points[p][0], l_Face->w->points[p][1], l_Face->w->points[p][2]);
			l_CompressNormal.AddVertex ( l_Face->plane.normal[0], l_Face->plane.normal[1], l_Face->plane.normal[2] );
/*
			float u = (DotProduct(l_Face->uaxis,l_Face->w->points[p]) / l_Face->uscale) + l_Face->ushift;
			float v = (DotProduct(l_Face->vaxis,l_Face->w->points[p]) / l_Face->vscale) + l_Face->vshift;

			float w,h;

			GetImageDimensions ( l_Face->texturefile, w, h );

			u = u / w;
			v = v / h;
		
			v = 1.0f - v;

			l_CompressUV.AddVertex ( u,v,0.0f );
*/
			l_Face->w->uvidx[p] = debugcount;
		
			debugcount++;
		
		}


	}

	//
	// Resize the arrays 
	//

	l_Vertices.Resize(l_CompressVertex.GetCount());
	l_Normals.Resize(l_CompressNormal.GetCount());

	//
	// Copy each array
	//

	// positions
	for (f=0;f<l_CompressVertex.GetCount();f++)
	{
		CSIBCVector3D	l_vtemp;

		l_CompressVertex.GetVertex (f,&l_vtemp);

		l_Vertices[f] =  l_vtemp;
	}

	// normals
	for (f=0;f<l_CompressNormal.GetCount();f++)
	{
		CSIBCVector3D	l_vtemp;

		l_CompressNormal.GetVertex (f,&l_vtemp);

		l_Normals[f] =  l_vtemp;
	}
	
	
	// Add some uv coordinates.

#ifdef DOTXSI35
	for (int ci2=0;ci2<m_pBrush.GetClusterCount();ci2++)
	{
		CSLUVCoordArray* l_p35UVArray = ((CSLShape_35*)l_pShape)->AddUVCoordArray();
		CSIBCString l_szTpName = "Texture_Projection";

		l_szTpName.Concat ( ci2 );

		l_p35UVArray->SetTextureProjection ( l_szTpName.GetText() );
		CSLBaseShape::CSLVector2DArray &l_UVs = *l_p35UVArray->GetUVCoordList();

		l_UVs.Resize(l_CompressUV.GetCount ());

		for (f=0;f<l_CompressUV.GetCount();f++)
		{
			CSIBCVector3D	l_vtemp;

			l_CompressUV.GetVertex (f,&l_vtemp);

			l_UVs[f].Set ( l_vtemp.m_fX, l_vtemp.m_fY );
		}
	}
#else
	//CSLBaseShape::CSLVector2DArray &l_UVs = *((CSLShape*)l_pShape)->CreateUVCoords();

	/*
	l_UVs.Resize(l_CompressUV.GetCount ());

	// UVs
	for (f=0;f<l_CompressUV.GetCount();f++)
	{
		CSIBCVector3D	l_vtemp;

		l_CompressUV.GetVertex (f,&l_vtemp);

		l_UVs[f].Set ( l_vtemp.m_fX, l_vtemp.m_fY );
	}
	*/
#endif


	///////////////////////////////////////////////////////////////////////////
	//
	// EXPORT THE POLYLISTS
	//
	///////////////////////////////////////////////////////////////////////////

	for (int ci=0;ci<m_pBrush.GetClusterCount();ci++)
	{
		BrushCluster*	l_pCluster = m_pBrush.GetCluster ( ci );
		int vindex = 0;
		
		//
		// Need to count how many vertex index we will use
		//
		int vindexcount = 0;
		int nbface =l_pCluster->m_pFaceList.GetUsed(); 

		for (f=0;f<nbface;f++)
		{
			face_t *l_Face = l_pCluster->m_pFaceList[f];
			if ( !l_Face->w )
				continue;
			
			vindexcount += l_Face->w->numpoints;
		}

		//add a polygon list
		//CSLPolygonList* l_Poly = l_pMesh->AddPolygonList();
		//CSIBCString fullpath;
		//fullpath.Concat ( l_pCluster->m_pFaceList[0]->texturefile );
		//l_szMaterialName = fullpath;


		//l_Poly->SetMaterial ( GetMaterialFromTextureFile ( GetMeshModel()->Scene(), fullpath.GetText() ) );

		//if ( !GetMeshModel()->GlobalMaterial() )
		//{
		//	GetMeshModel()->AddGlobalMaterial();
		//}

		//GetMeshModel()->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_NODE);
		//GetMeshModel()->GlobalMaterial()->SetMaterial(l_Poly->GetMaterial() );

		//CSIBCString l_ClusterMatName = l_Poly->GetMaterial()->Name();

		

		//l_Poly->GetPolygonVertexCountList()->Resize( nbface );

		//l_Poly->GetVertexIndices()->Resize( vindexcount );
		//SI_Int *l_pnDataArray = l_Poly->GetVertexIndices()->ArrayPtr();

		//add normals
		//CSLPolygonList::CSLIntArray* l_pNormalArray = l_Poly->CreateNormalIndices();
		//l_pNormalArray->Resize(vindexcount);

		//add UV
#ifdef DOTXSI35
		for (int ci3=0;ci3<m_pBrush.GetClusterCount();ci3++)
		{
			CSLPolygonList::CSLIntArray* l_pUVArray = l_Poly->AddUVArray();
			l_pUVArray->Resize(vindexcount);
			SI_Int *l_uvDataArray = l_pUVArray->ArrayPtr();

			vindex = 0;
					
			for (f=0;f<nbface;f++)
			{
				face_t *l_Face = l_pCluster->m_pFaceList[f];

				if ( !l_Face->w )
					continue;

				int rindex = vindex + l_Face->w->numpoints - 1;

				for (p=0;p<l_Face->w->numpoints;p++)
				{
					l_uvDataArray[rindex] = l_Face->w->uvidx [p];
					vindex++;
					rindex--;

				}

			}
		}

		vindex = 0;
			

		
#else
		//CSLPolygonList::CSLIntArray* l_pUVArray = l_Poly->AddUVArray();
		//l_pUVArray->Resize(vindexcount);
		//SI_Int *l_uvDataArray = l_pUVArray->ArrayPtr();

#endif


		//SI_Int *l_normalDataArray = l_pNormalArray->ArrayPtr();
		
		for (f=0;f<nbface;f++)
		{
			face_t *l_Face = l_pCluster->m_pFaceList[f];
			if ( !l_Face->w )
				continue;
			
			//(*l_Poly->GetPolygonVertexCountList() )[f] = l_Face->w->numpoints;
			polygonData.Add ( l_Face->w->numpoints );

			int rindex = vindex + l_Face->w->numpoints - 1;

			int	*inverseindices = new int[l_Face->w->numpoints];

			for (p=0;p<l_Face->w->numpoints;p++)
			{

				int iindex = l_CompressVertex.GetIndex (l_Face->w->points[p][0], l_Face->w->points[p][1], l_Face->w->points[p][2]);
			//	int nindex = l_CompressNormal.GetIndex (l_Face->plane.normal[0], l_Face->plane.normal[1], l_Face->plane.normal[2]);
				int nindex = l_Face->w->uvidx [p];
				
			//	l_pnDataArray[rindex] = iindex;
			//	l_normalDataArray[vindex] = nindex;
				
				inverseindices[p] = iindex;

				//polygonData.Add (iindex);

			/*	(*l_Vertices)[rindex].Set(l_Face->w->points[p][0], l_Face->w->points[p][1], l_Face->w->points[p][2]);
				
				l_Normals[rindex].Set (l_Face->plane.normal[0], l_Face->plane.normal[1], l_Face->plane.normal[2]);
				*/
				float u = (DotProduct(l_Face->uaxis,l_Face->w->points[p]) / l_Face->uscale) + l_Face->ushift;
				float v = (DotProduct(l_Face->vaxis,l_Face->w->points[p]) / l_Face->vscale) + l_Face->vshift;
			
				float w,h;

				GetImageDimensions ( l_Face->texturefile, w, h );

				u = u / w;
				v = v / h;
			
				v = 1.0f - v;

				m_pUVs.Extend(1);
				m_pUVs[m_pUVs.GetUsed()-1].u = u;
				m_pUVs[m_pUVs.GetUsed()-1].v = v;

#ifndef DOTXSI35
				//int uindex = l_CompressUV.GetIndex(u,v,0.0f);

//				l_uvDataArray[rindex] = l_Face->w->uvidx [p];
			//	l_UVs[rindex].Set ( u,v );
#endif
				vindex++;
				rindex--;

			}
			
			for (int iv=l_Face->w->numpoints-1;iv>=0;iv--)
			{
				polygonData.Add (inverseindices[iv]);
			}

			delete [] inverseindices;

			m_iPolygonCount++;

		}
	}


	//
	// XSI SDK TEST
	//

	if ( disp )
		return;

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
	DSA2W(&l_wszGeomName,"Geom");
	
	//st = in_pRootModel.AddGeometry(L"Cube", L"MeshSurface", l_wszGeomName, l_x3dCube);

	//XSI::CValueArray margs(3);
	//margs[0] = l_x3dCube.GetRef();
	//app.ExecuteCommand(L"FreezeObj",margs, outArg);

	//XSI::PolygonMesh polyMesh( l_x3dCube.GetActivePrimitive().GetGeometry() );

	XSI::MATH::CVector3Array	l_vertices;
	int gg;// positions
	for (gg=0;gg<l_CompressVertex.GetCount();gg++)
	{
		CSIBCVector3D	l_vtemp;

		l_CompressVertex.GetVertex (gg,&l_vtemp);

		XSI::MATH::CVector3	l_vec;
		l_vec.PutX ( l_vtemp.m_fX );
		l_vec.PutY ( l_vtemp.m_fY );
		l_vec.PutZ ( l_vtemp.m_fZ );

		l_vertices.Add ( l_vec );
	}

//	polyMesh.Set ( l_vertices, polygonData );

	
	in_pRootModel.AddPolygonMesh ( l_vertices, polygonData , L"Mesh", l_x3dCube );
	
	
	//AddMeshToGroup ( l_x3dCube, l_szMaterialName.GetText() );

	if ( g_iImportMaterials )
	{
		args[0] = XSI::CValue( XSI::CString(L"") );
		args[1] = XSI::CValue(false);
		args[0] = XSI::CValue(l_x3dCube.GetRef());
		args[1] = XSI::CValue((long)XSI::siTxtUV);
		args[2] = XSI::CValue((long)XSI::siTxtDefaultSpherical);
		args[3] = XSI::CValue(XSI::CString(L"Texture_Support"));

		app.ExecuteCommand( L"CreateTextureSupport", args, outArg );

		//
		// freeze the mesh
		//

		XSI::CValueArray	moreargs(1);
		XSI::CValueArray	moreoutargs(3);
		moreargs[0] = l_x3dCube.GetRef();
		app.ExecuteCommand(L"FreezeObj",moreargs, outArg);
	}


	/*
	// Now add a material to it.
	st = l_x3dCube.AddMaterial(L"Phong", true, L"CubeMat", l_matMaterial);
		
	XSI::OGLTexture		l_oglTexture(l_matMaterial.GetOGLTexture());

	XSI::CString		l_szFullNameDefaultOut	= l_oglTexture.GetFullName();
	int			l_nHeightDefaultOut		= l_oglTexture.GetHeight();
	int			l_nWidthDefaultOut		= l_oglTexture.GetWidth();

	// Now actually add a texture, so we can test it.
	args[0] = XSI::CValue( XSI::CString(L"Image") );
	args[1] = XSI::CValue(l_x3dCube.GetRef());
	args[2] = XSI::CValue((short)1);
	args[3] = XSI::CValue(false);
	st = app.ExecuteCommand( L"BlendInPresets", args, outArg );
	
	// Now it should have noIcon.pic attached to it.
	XSI::CString		l_szFullNameOut		= l_oglTexture.GetFullName();
	int			l_nHeightOut		= l_oglTexture.GetHeight();
	int			l_nWidthOut			= l_oglTexture.GetWidth();*/

}

CSLBaseMaterial*	VMFTemplate::GetMaterialFromTextureFile ( CSLScene* in_pScene, char *in_szFilename )
{
	CSLMaterialLibrary* in_pMatLib = in_pScene->GetMaterialLibrary();

	for (int m=0;m<in_pMatLib->GetMaterialCount();m++)
	{
		CSLBaseMaterial* l_pCMat = in_pMatLib->GetMaterialList()[m];

		if ( l_pCMat->Type() == CSLTemplate::XSI_MATERIAL )
		{
			CSLXSIMaterial	*l_pXSIMat = (CSLXSIMaterial *)l_pCMat;


			CSLXSIShader* l_pShader = l_pXSIMat->FindShader ( "OGLCom" );

			if ( l_pShader!=NULL )
			{
				for (int cp=0;cp<l_pShader->GetConnectionPointCount();cp++)
				{
					CSLShaderConnectionPoint*	l_pCnxPoint = l_pShader->GetConnectionPointList()[cp];

					char *l_szImageName = l_pCnxPoint->GetImage();

					if ( l_szImageName != NULL )
					{
						if ( !_SI_STRCMP(in_szFilename, l_szImageName ) )
						{
							return l_pXSIMat;
						}

					}
				}
			}


/*
			CSLTexture2D* l_pTexture = ((CSLMaterial*)l_pCMat)->Texture2D();

			if ( l_pTexture != NULL )
			{
				if ( !_SI_STRCMP(l_pTexture->GetImageFileName(), in_szFilename ) )
				{
					return l_pCMat;
				}
			}
			*/

		}
	}
	
	//
	// Load the TGA to get it's dimensions
	//

	CSIILTGAFileDriver	l_pTGA;
	CSIBCPixMap			l_pPixMap;

	int w = 512;
	int h = 512;

	if ( l_pTGA.Load ( CSIBCString ( in_szFilename ), l_pPixMap ) == SI_SUCCESS )
	{
		w = l_pPixMap.GetWidth();
		h = l_pPixMap.GetHeight();
	}
		
	//
	// create a new material
	//


#ifdef DOTXSI35
	char matname[256];
	sprintf(matname,"Material_%d",in_pMatLib->GetMaterialCount());

	CSLXSIMaterial *out_pMat = in_pMatLib->AddXSIMaterial();

	out_pMat->SetName(matname);

	// add shader to XSIMaterial
	CSLXSIShader *l_pShader = out_pMat->AddShader();
	l_pShader->SetName("OGLCom");
	l_pShader->SetOutputType(CSLXSIShader::SI_OT_UNKNOWN);
	l_pShader->SetProgID("Softimage.OGLCom.1");

	// add parameter to Shader
	AddParameter ( l_pShader, "Name","OGLCom" );
	AddParameter ( l_pShader, "normal",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "uv",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "vertexcolor",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "blending",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "srcblendingfunction",(SI_Long)1 );
	AddParameter ( l_pShader, "dstblendingfunction",(SI_Long)0 );
	AddParameter ( l_pShader, "cullingmode",(SI_Long)0 );
	AddParameter ( l_pShader, "depthtest",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "Enable_Ambient",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "Ambient.red",(SI_Float)0.3f );
	AddParameter ( l_pShader, "Ambient.green",(SI_Float)0.3f );
	AddParameter ( l_pShader, "Ambient.blue",(SI_Float)0.3f );
	AddParameter ( l_pShader, "Ambient.alpha",(SI_Float)1.0f );
	AddParameter ( l_pShader, "Enable_Diffuse",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "Diffuse.red",(SI_Float)0.5f );
	AddParameter ( l_pShader, "Diffuse.green",(SI_Float)0.5f );
	AddParameter ( l_pShader, "Diffuse.blue",(SI_Float)0.5f );
	AddParameter ( l_pShader, "Diffuse.alpha",(SI_Float)1.0f );
	AddParameter ( l_pShader, "Enable_Specular",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "Specular.red",(SI_Float)0.7f );
	AddParameter ( l_pShader, "Specular.green",(SI_Float)0.7f );
	AddParameter ( l_pShader, "Specular.blue",(SI_Float)0.7f );
	AddParameter ( l_pShader, "Specular.alpha",(SI_Float)1.0f );
	AddParameter ( l_pShader, "Enable_Emissive",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "Emissive.red",(SI_Float)0.0f );
	AddParameter ( l_pShader, "Emissive.green",(SI_Float)0.0f );
	AddParameter ( l_pShader, "Emissive.blue",(SI_Float)0.0f );
	AddParameter ( l_pShader, "Emissive.alpha",(SI_Float)1.0f );

	AddParameter ( l_pShader, "Enable_Shininess",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "Shininess",(SI_Float)10.0f );
	AddParameter ( l_pShader, "Enable_Lighting",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "Texture_1_Enable",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "tspace_id1","Texture_Projection" );
	
	AddParameter ( l_pShader, "Modulation1",(SI_Long)0 );

	AddParameter ( l_pShader, "Refmap1",(SI_Bool)SI_FALSE );
	AddParameter ( l_pShader, "WrapS1",(SI_Bool)SI_TRUE );
	AddParameter ( l_pShader, "WrapT1",(SI_Bool)SI_TRUE );

	AddParameter ( l_pShader, "magfilter1",(SI_Long)1 );
	AddParameter ( l_pShader, "minfilter1",(SI_Long)1 );

	CSLShaderConnectionPoint *l_pConnectionPoint = l_pShader->AddConnectionPoint();
	l_pConnectionPoint->SetName("previous");
	
	CSLShaderConnectionPoint *l_pConnectionPoint2 = l_pShader->AddConnectionPoint();
	l_pConnectionPoint2->SetName("Texture_1");
	l_pConnectionPoint2->SetImage(in_szFilename);

	// Add connection under material
	CSLConnectionPoint *l_pMaterialConnection;

	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("surface");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("volume");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("environment");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("contour");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("displacement");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("shadow");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("Photon");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("PhotonVolume");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("normal");
	
	l_pMaterialConnection = out_pMat->AddConnectionPoint();
	l_pMaterialConnection->SetName("RealTime");
	l_pMaterialConnection->SetShader(l_pShader);
	
	return out_pMat;

#else

	CSLMaterial *l_pMaterial = in_pScene->GetMaterialLibrary()->AddMaterial ();
	char matname[256];
	sprintf(matname,"Material_%d",in_pMatLib->GetMaterialCount());
	l_pMaterial->SetName(matname);
	l_pMaterial->SetShadingModel ( CSLMaterial::PHONG );
	l_pMaterial->SetAmbientColor ( CSIBCColorf ( 0.3f, 0.3f, 0.3f, 1.0f ) );
	l_pMaterial->SetDiffuseColor ( CSIBCColorf ( 0.7f, 0.7f, 0.7f, 1.0f ) );
	l_pMaterial->SetSpecularColor( CSIBCColorf ( 1.0f, 1.0f, 1.0f, 1.0f ) );
	l_pMaterial->SetSpecularDecay ( 50.0f );
	CSLTexture2D* l_pTexture = l_pMaterial->CreateTexture2D();
	l_pTexture->SetImageFileName ( in_szFilename );
	l_pTexture->SetMappingType ( CSLTexture2D::SI_UV_MAP );
	l_pTexture->SetImageWidth ( w );
	l_pTexture->SetImageHeight ( h );
	l_pTexture->SetCropUMax ( w - 1 );
	l_pTexture->SetCropVMax ( h - 1 );
	l_pTexture->SetURepeat (1);
	l_pTexture->SetVRepeat (1);
	l_pTexture->SetUScale (1);
	l_pTexture->SetVScale (1);

	CSIBCMatrix4x4	l_pMatrix;
	l_pMatrix.SetIdentity();

	l_pTexture->SetProjectionMatrix ( l_pMatrix );

	l_pTexture->SetBlendingType ( CSLTexture2D::SI_RGB_MODULATION );
	l_pTexture->SetBlending(1.0f);
	l_pTexture->SetAmbient(0.75f);
	l_pTexture->SetDiffuse(1.0f);

	return l_pMaterial;
#endif

}

void AddParameter ( CSLXSIShader *in_pShader, SI_Char* in_szName, SI_Char* in_szValue )
{
	// add parameter to Shader
	CSLVariantParameter *l_pNewParameter = in_pShader->AddParameter();

	l_pNewParameter->SetName(in_szName);
	SI_TinyVariant l_Variant =
	{
		SI_VT_PCHAR,
		SI_TT_NONE,
		1
	};

	l_Variant.p_cVal = _SI_STRDUP(in_szValue);
	l_pNewParameter->SetValue(&l_Variant);
	
}

void AddParameter ( CSLXSIShader *in_pShader, SI_Char* in_szName, SI_Bool in_bValue )
{
	
	// add parameter to Shader
	CSLVariantParameter *l_pNewParameter = in_pShader->AddParameter();

	l_pNewParameter->SetName(in_szName);
	SI_TinyVariant l_Variant =
	{
		SI_VT_BOOL,
		SI_TT_NONE,
		1,
		in_bValue
	};
	
	l_Variant.boolVal = in_bValue;

	l_pNewParameter->SetValue(&l_Variant);
	
	
}


void AddParameter ( CSLXSIShader *in_pShader, SI_Char* in_szName, SI_Long in_lValue )
{
	// add parameter to Shader
	CSLVariantParameter *l_pNewParameter = in_pShader->AddParameter();

	l_pNewParameter->SetName(in_szName);
	SI_TinyVariant l_Variant =
	{
		SI_VT_INT,
		SI_TT_NONE,
		1,
		in_lValue
	};
	
	l_Variant.nVal = in_lValue;

	l_pNewParameter->SetValue(&l_Variant);
	

}

void AddParameter ( CSLXSIShader *in_pShader, SI_Char* in_szName, SI_Float in_fValue )
{
	// add parameter to Shader
	CSLVariantParameter *l_pNewParameter = in_pShader->AddParameter();

	l_pNewParameter->SetName(in_szName);
	SI_TinyVariant l_Variant =
	{
		SI_VT_FLOAT,
		SI_TT_NONE,
		1,
		in_fValue
	};
	
	l_Variant.fVal = in_fValue;

	l_pNewParameter->SetValue(&l_Variant);
	

}

void	VMFTemplate::CreateDispinfo( XSI::Model in_pRootModel, CSLModel *in_pModel)
{
	
	for (int f=0;f<m_pBrush.GetNbFace ();f++)
	{
		face_t *l_Face = m_pBrush.GetFace (f);

		if (( l_Face->dispinfo != NULL ) && ( l_Face->w != NULL ))
		{
			dispinfo_t * l_pdisp = l_Face->dispinfo;

			CSLModel* l_pModel = GetMeshModel();

//			CSLModel* l_pDispInfoModel = l_pModel->AddModel();

			SI_Char	l_szModelName[256];

			sprintf ( l_szModelName, "dispinfo_%d",++g_pGlobalMdlCounter );

			//l_pDispInfoModel->SetName(l_szModelName);
			//l_pDispInfoModel->Visibility()->SetVisibility(SI_TRUE);
			//l_pDispInfoModel->Transform()->SetScale(CSIBCVector3D(1.0, 1.0, 1.0) );
			//l_pDispInfoModel->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_INHERITED);
			//l_pDispInfoModel->GlobalMaterial()->SetMaterial(l_pModel->Scene()->GetMaterialLibrary()->GetMaterialList()[0] );

			
			//CSLModel *l_pMeshModel = l_pModel->AddMesh();


				
			//sprintf ( l_szMeshName, "%s_Dispinfo_%d",l_pModel->GetName(),++g_pGlobalMdlCounter );

			//l_pMeshModel->SetName(l_szMeshName);
			//l_pMeshModel->Visibility()->SetVisibility(SI_TRUE);
			//l_pMeshModel->Transform()->SetScale(CSIBCVector3D(1.0, 1.0, 1.0) );
			
			//CSLMesh *l_pMesh2 = (CSLMesh *) l_pMeshModel->Primitive();
			//CSLBaseShape *l_pShape2 = l_pMesh2->Shape();

			//
			// Create the geometry for the dispinfo
			//

			//CSLMesh *l_pMesh = (CSLMesh *) GetMeshModel()->Primitive();
			//CSLBaseShape *l_pShape = l_pMesh->Shape();
			//CSLPolygonList* l_Poly = l_pMesh->PolygonLists()[0];

			//CSLMaterial* l_pMat = (CSLMaterial*)l_Poly->GetMaterial();

			//if ( !l_pMeshModel->GlobalMaterial() )
			//{
			//	l_pMeshModel->AddGlobalMaterial();
			//}

			//l_pMeshModel->GlobalMaterial()->SetPropagationType(CSLGlobalMaterial::SI_NODE);
			//l_pMeshModel->GlobalMaterial()->SetMaterial(l_pMat);
			
			//if ( l_Poly != NULL )
			{
				CDispInfoConverter* l_pdispi = new CDispInfoConverter;
				//dispinfo_convert.Extend(1);
				//dispinfo_convert[dispinfo_convert.GetUsed()-1] = l_pdispi;

				l_pdispi->Convert ( l_Face, l_pdisp, 256, 256 );
				XSI::CRef l_cref = l_pdispi->CreateGeometry( in_pRootModel);

				if ( g_iImportMaterials )
				{
					l_pdispi->AssignUVs();
					AddMeshToGroup ( l_cref, l_Face->texturefile );
				}
				delete l_pdispi;


				//dispinfo_convert.CreateGeometry( l_pMat, l_pShape,l_pMesh,offset );
			}


		}
	}

}

void VMFTemplate::GetImageDimensions ( char * in_szFilename, float& w, float& h )
{
	//
	// First look in our map to see if the image has already been loaded
	//

	TGAMapEntry	*l_pEntry = NULL;

	for (int v=0;v<m_gTGAMap.GetUsed();v++)
	{

		if ( !strcmp ( m_gTGAMap[v].m_szFilename.GetText(), in_szFilename ) )
		{
			l_pEntry = &m_gTGAMap[v];
			break;
		}

	}

	if ( l_pEntry == NULL )
	{
		m_gTGAMap.Extend(1);
		l_pEntry = &m_gTGAMap[m_gTGAMap.GetUsed()-1];

		l_pEntry->m_szFilename = in_szFilename;

		
		CSIILTGAFileDriver	l_pTGA;
		CSIBCPixMap			l_pPixMap;

		l_pEntry->w = 512.0f;
		l_pEntry->h = 512.0f;

		if ( l_pTGA.Load ( CSIBCString ( in_szFilename ), l_pPixMap ) == SI_SUCCESS )
		{
			l_pEntry->w = (float)l_pPixMap.GetWidth();
			l_pEntry->h = (float)l_pPixMap.GetHeight();
		}

	}
	
	w = l_pEntry->w;
	h = l_pEntry->h;
}

void VMFTemplate::AddMeshToGroup ( XSI::CRef in_cRef, char *in_szMaterial )
{
	//
	// find the group
	//

	for (int c=0;c<m_pGroups.GetUsed();c++)
	{

		if ( m_pGroups[c].m_szMaterialName == in_szMaterial )
		{
			m_pGroups[c].m_Objects.Add ( in_cRef );
			return ;
		}
	}

	m_pGroups.Extend(1);
	m_pGroups[m_pGroups.GetUsed()-1].m_szMaterialName = in_szMaterial;
	m_pGroups[m_pGroups.GetUsed()-1].m_Objects.Add ( in_cRef );

}

void VMFTemplate::CreateGroups ( XSI::Model in_pRoot )
{
	XSI::CStatus st;

	XSI::CComAPIHandler xsiuitoolkit;
	xsiuitoolkit.CreateInstance( L"XSI.UIToolkit");
	XSI::CValue retval = xsiuitoolkit.GetProperty( L"ProgressBar" );
	XSI::CComAPIHandler progressbar( retval ); 	

	// set min/max
	progressbar.PutProperty( L"Minimum", (long)1 );
	progressbar.PutProperty( L"Maximum", (long)m_pGroups.GetUsed() );
	progressbar.PutProperty( L"Value", (long)1 );
	progressbar.PutProperty( L"Caption", XSI::CValue(L"Assinging materials...") );

	retval = progressbar.GetProperty( L"Value" );
	
	// show the progress bar
	progressbar.PutProperty( L"Visible", true );

	for (int c=0;c<m_pGroups.GetUsed();c++)
	{
		
		XSI::Group daGroup;

		in_pRoot.AddGroup(m_pGroups[c].m_Objects, L"MaterialGroup",false, daGroup);
		
		// Default parameters.
		XSI::CString			l_szFullNameDefault(L"");
		int				l_nHeightDefault	= 0;
		int				l_nWidthDefault		= 0;
		
		// Changed parameters (by attaching noIcon.pic).
		LPWSTR l_wszMatName;
		DSA2W(&l_wszMatName,m_pGroups[c].m_szMaterialName.GetText());

		char fname[MAX_PATH];
		_splitpath ( m_pGroups[c].m_szMaterialName.GetText(), NULL, NULL, fname, NULL );

		XSI::CString			l_szFullName( l_wszMatName );

		int				l_nHeight			= 256;
		int				l_nWidth			= 256;
		
		
		// Now add a material to it.
		XSI::Material		l_matMaterial;
		daGroup.AddMaterial(L"Phong", true, L"CubeMat", l_matMaterial);
		
		XSI::OGLTexture		l_oglTexture(l_matMaterial.GetOGLTexture());
		
		XSI::CString		l_szFullNameDefaultOut	= l_oglTexture.GetFullName();
		int			l_nHeightDefaultOut		= l_oglTexture.GetHeight();
		int			l_nWidthDefaultOut		= l_oglTexture.GetWidth();
		
		XSI::CValueArray	args(4);
		XSI::CValue			outArg;
		args[0] = XSI::CValue( XSI::CString(L"Image") );
		args[1] = XSI::CValue(daGroup.GetRef());
		args[2] = XSI::CValue((short)1);
		args[3] = XSI::CValue(false);
		
		XSI::Application	app;
		app.ExecuteCommand( L"BlendInPresets", args, outArg );

		progressbar.Call( L"Increment", retval );
		progressbar.PutProperty( L"Caption", XSI::CValue(L"Creating material groups...") );
		progressbar.PutProperty( L"Visible", true );

		XSI::CString		l_szMaterialName	= l_matMaterial.GetFullName(); 
		XSI::CString		l_szWrapu = l_szMaterialName + L".wrapu";
		XSI::CString		l_szWrapv = l_szMaterialName + L".wrapv";

		SetXSIValue ( l_szWrapu, 10497 );
		SetXSIValue ( l_szWrapv, 10497 );

		//
		// create the texture and connect
		//

		XSI::CValueArray	clipargs(3);
		XSI::ImageClip2 l_pClip;

		LPWSTR l_wszClipName;
		DSA2W(&l_wszClipName,fname);

		clipargs[0] = XSI::CValue( XSI::CString(l_wszMatName) );
		clipargs[1] = XSI::CValue( XSI::CString(l_wszClipName) );
		clipargs[2] = XSI::CValue(l_pClip.GetRef());
		app.ExecuteCommand( L"SICreateImageClip", clipargs, outArg );

		XSI::CString		l_szImageNode = l_szMaterialName + L".CubeMat.ambient_blend.Image.tex";
		XSI::CString		l_szFullclipname = L"Clips." +  XSI::CString(l_wszClipName);

		XSI::CValueArray	clipargs2(2);
		clipargs2[0] = XSI::CValue( XSI::CString(l_szFullclipname) );
		clipargs2[1] = XSI::CValue( XSI::CString(l_szImageNode) );
		
		app.ExecuteCommand( L"SIConnectShaderToCnxPoint", clipargs2, outArg );

	}

	progressbar.PutProperty( L"Visible", false );
	m_pGroups.DisposeData();
}

void VMFTemplate::GetUVFromPosition ( CSIBCVector3D in_vPosition, float& u, float& v)
{
	int f;

	u = v = 0.0f;

	for (f=0;f<m_pBrush.GetNbFace ();f++)
	{
		face_t *l_Face = m_pBrush.GetFace (f);

		if ( !l_Face->w )
			continue;
	
		for (int p=0;p<l_Face->w->numpoints;p++)
		{

			if (( in_vPosition.m_fX == l_Face->w->points[p][0]) &&
				( in_vPosition.m_fY == l_Face->w->points[p][1]) &&
				( in_vPosition.m_fZ == l_Face->w->points[p][2]))
			{
				u = (DotProduct(l_Face->uaxis,l_Face->w->points[p]) / l_Face->uscale) + l_Face->ushift;
				v = (DotProduct(l_Face->vaxis,l_Face->w->points[p]) / l_Face->vscale) + l_Face->vshift;

				float w,h;

				GetImageDimensions ( l_Face->texturefile, w, h );

				u = u / w;
				v = v / h;
			
				v = 1.0f - v;

			}
		}

	}

	
}

void	VMFTemplate::AssignUVs()
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
		
		XSI::CClusterPropertyElementArray clusterPropertyElements = UVWProp.GetElements();
		XSI::CDoubleArray elementArray = clusterPropertyElements.GetArray();

		XSI::CVertexRefArray l_Vertices = mesh.GetVertices ();

		long totalUvCount = elementArray.GetCount ();
		int cc=0;
		int uvc = 0;
		for (int c=0;c<m_iPolygonCount;c++)
		{
			long l_iNumVertex = polygonData[cc];

			cc++;
			for (int i=0;i<l_iNumVertex;i++)
			{
				long l_iID = polygonData[cc];
				cc++;

				/*
				elementArray[ l_iID * 3 ] = m_pUVs[uvc].u;

				elementArray[ (l_iID * 3) + 1] = m_pUVs[uvc].v;
				elementArray[ (l_iID * 3) + 2] = 0.0f;
				*/

				elementArray[ uvc * 3 ] = m_pUVs[uvc].u;

				elementArray[ (uvc * 3) + 1] = m_pUVs[uvc].v;
				elementArray[ (uvc * 3) + 2] = 0.0f;

				uvc++;
			}
				


		}

		/*
		for (int tri=0;tri<l_Vertices.GetCount();tri++)
		{
			XSI::Vertex l_vertex = l_Vertices.GetItem(tri);
			XSI::MATH::CVector3 vec = l_vertex.GetPosition ();
			XSI::CPolygonNodeRefArray l_nodes = l_vertex.GetNodes();

			float lu,lv;
			GetUVFromPosition ( CSIBCVector3D ( vec.GetX(), vec.GetY(), vec.GetZ() ), lu, lv );

			for (int n=0;n<l_nodes.GetCount();n++)
			{
				XSI::PolygonNode l_polynode = l_nodes.GetItem (n);
				long node_index = l_polynode.GetIndex ();

				elementArray[ node_index * 3 ] = lu;
				elementArray[ (node_index * 3) + 1] = lv;
				elementArray[ (node_index * 3) + 2] = 0.0f;

			}

		}*/

		clusterPropertyElements.PutArray(elementArray);
	}

	//
	// assign uvs for it's disp converter
	//

	for (int d=0;d<dispinfo_convert.GetUsed();d++)
	{
		dispinfo_convert[d]->AssignUVs ();
	}

	for (int c=0;c<GetNbChild();c++)
	{
		GetChild (c)->AssignUVs();
	}
}

void VMFTemplate::SetXSIValue ( XSI::CString in_szString , long in_lValue )
{
	XSI::CValueArray	args(2);
	XSI::CValue			outArg;
	args[0] = XSI::CValue( in_szString );
	args[1] = XSI::CValue(in_lValue);
		
	XSI::Application	app;
	app.ExecuteCommand( L"SetValue", args, outArg );
}
