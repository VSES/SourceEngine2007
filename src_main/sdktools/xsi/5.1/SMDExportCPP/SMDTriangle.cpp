// SMDTriangle.cpp: implementation of the SMDTriangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMDTriangle.h"

#include <comdef.h>					// _variant_t
#include <xsisdk.h>
#include <xsi_application.h>
#include <xsi_primitive.h>
#include <xsi_geometry.h>
#include <xsi_point.h>
#include <xsi_polygonmesh.h>
#include <xsi_cluster.h>
#include <xsi_polygonnode.h>
#include <xsi_triangle.h>
#include <xsi_material.h>
#include <xsi_ogltexture.h>
#include <xsi_kinematicstate.h>
#include <xsi_trianglevertex.h>
#include <xsi_polygonface.h>

#include <xsi_application.h>
#include <xsi_uitoolkit.h>
#include <xsi_progressbar.h>

extern void	XSILogMessage ( char *in_szMessage, XSI::siSeverityType in_eSeverity );
extern int SMDType;
extern int RemoveCharacterRigRoot;


void W2AHelper2( LPSTR out_sz, LPCWSTR in_wcs,int in_cch = -1 );

typedef struct tagMaterialList
{
	char	texture[256];
	CSIBCArray<int>	polyIndices;

} MateriaList;

SMDVertex::SMDVertex() :
m_iIndex(-1)
{}

SMDVertex::~SMDVertex()
{
	m_lWeights.DisposeData();
}

void SMDVertex::AddWeight ( SI_Int in_iBoneID, SI_Float in_fWeight )
{
	
	m_lWeights.Extend(1);
	m_lWeights[m_lWeights.GetUsed()-1].m_iBoneID = in_iBoneID;
	m_lWeights[m_lWeights.GetUsed()-1].m_fWeight = in_fWeight;

}

SMDWeight*	SMDVertex::GetWeight ( SI_Int in_iVertexID )
{
	if (( in_iVertexID > (m_lWeights.GetUsed() -1) ) || ( in_iVertexID < 0 ))
	{
		return NULL;
	}

	return &m_lWeights[in_iVertexID];

}

SMDEnvelope::SMDEnvelope()
{
}

SMDEnvelope::~SMDEnvelope()
{
	for (int m=0;m<m_pVertexList.GetUsed();m++)
	{
		SMDVertex*	l_pEnv = m_pVertexList[m];

		delete l_pEnv;
	}

	m_pVertexList.DisposeData();
}

void SMDEnvelope::SetModel ( XSI::X3DObject in_pModel )
{
	m_pModel = in_pModel;

	//
	// Create the vertex list
	//

	XSI::Primitive l_pPrim = in_pModel.GetActivePrimitive();

	if ( !l_pPrim.IsValid() )
		return;
	
	XSI::Geometry l_pGeo = l_pPrim.GetGeometry();
	XSI::CPointRefArray l_pPoints = l_pGeo.GetPoints();
	XSI::PolygonMesh l_pPolyMesh = l_pPrim;

	SI_Long	l_lVertexCount = l_pPoints.GetCount();

	for (int v=0;v<l_lVertexCount;v++)
	{
		SMDVertex* l_pVertex = new SMDVertex;

		l_pVertex->SetIndex (v);
		m_pVertexList.Extend(1);
		m_pVertexList[m_pVertexList.GetUsed()-1] = l_pVertex;

	}



/*
	XSI::CPointRefArray pointRefArray(l_pPolyMesh.GetPoints());
	XSI::MATH::CVector3Array  positionArray(pointRefArray.GetPositionArray());
	XSI::CPolygonNodeRefArray nodeRefArray(l_pPolyMesh.GetNodes());
	XSI::MATH::CVector3Array  normalArray(nodeRefArray.GetNormalArray());

	//Take care of the UV's
	XSI::CRefArray		clusterRefArray;
	l_pPolyMesh.GetClusters().Filter(XSI::siSampledPointCluster,XSI::CStringArray(),L"",clusterRefArray);

	XSI::Cluster			samplePointClusterUV;
	XSI::CRefArray       uvClusterPropertiesRefArray;
	
	int i;
	for(i=0;i < clusterRefArray.GetCount(); i++)
	{
		XSI::Cluster cluster(clusterRefArray[i]);		
		if(cluster.GetProperties().Filter(XSI::siClsUVSpaceTxtType,XSI::CStringArray(), L"",uvClusterPropertiesRefArray) == XSI::CStatus::OK)
		{
			samplePointClusterUV = cluster;			
			break;
		}
	}

	for(i=0;i < uvClusterPropertiesRefArray.GetCount(); i++)
	{
		XSI::ClusterProperty uvProp(uvClusterPropertiesRefArray[i]);
		//Set the elements
		XSI::CClusterPropertyElementArray uvElementArray = uvProp.GetElements();

		XSI::CDoubleArray uvValueArray = uvElementArray.GetArray();
		long lnbUV= (long)(uvValueArray.GetCount() / 3);
			
		for(long t = 0; t < lnbUV; ++t)
		{
			m_pVertexList.Extend(1);
			SMDVertex* l_pVertex = m_pVertexList[m_pVertexList.GetUsed()-1];

			l_pVertex->m_vPosition = CSIBCVector3D ( positionArray[t].GetX(), positionArray[t].GetY(), positionArray[t].GetZ() );
			l_pVertex->m_vNormal = CSIBCVector3D ( normalArray[t].GetX(), normalArray[t].GetY(), normalArray[t].GetZ() );
			l_pVertex->m_vUV = CSIBCVector2D ( uvValueArray[t*3],uvValueArray[i*3 + 1]);
		}
	}

	//SI_Long	l_lVertexCount = l_pShape->GetVertexCount();

	//for (SI_Long i=0;i<l_lVertexCount;i++)
	//{

	//	m_pVertexList.Extend(1);
	//	SMDVertex* l_pVertex = &m_pVertexList[m_pVertexList.GetUsed()-1];

	//	l_pVertex->m_vPosition = l_pPosition[i];
	//	l_pVertex->m_vNormal = l_pNormal[i];
	//	l_pVertex->m_vUV = l_pUV[i];

	//}
	*/
}

void	SMDEnvelope::AddWeight ( SI_Int in_iVertexID, SI_Int in_iBoneID, SI_Float in_fWeight )
{

	if (( in_iVertexID < 0 ) || ( in_iVertexID > m_pVertexList.GetUsed() -1 ))
	{
		return;
	}

	m_pVertexList[in_iVertexID]->AddWeight ( in_iBoneID, in_fWeight );

}

SI_Error SMDEnvelope::Write ( FILE* l_fptr, int rigid, SMDNodeList* in_pNodeList )
{

//	CSIBCVector3D*	l_pPosition = NULL;
//	CSIBCVector3D*	l_pNormal = NULL;
//	CSIBCVector2D*	l_pUV = NULL;
	
	XSI::Primitive l_pPrim = m_pModel.GetActivePrimitive();
	
	if ( !l_pPrim.IsValid() )
		return SI_ERR_ERROR_MSG;

	XSI::Application app;

	XSI::UIToolkit kit = app.GetUIToolkit();
	XSI::ProgressBar	m_pBar = kit.GetProgressBar();
	
	m_pBar.PutMaximum( 100 );
	m_pBar.PutMinimum( 1 );
	m_pBar.PutStep( 1 );
	m_pBar.PutValue( 1 );
	m_pBar.PutCaption( L"Writing vertex data..." );
	m_pBar.PutStatusText( L"" );
	m_pBar.PutVisible( true );

	//
	// Get default texture name
	//

	char l_szDefaultTextureName[MAX_PATH];
	XSI::OGLTexture l_pDefaultTexture = m_pModel.GetMaterial().GetOGLTexture();

	if ( !l_pDefaultTexture.IsValid() )
	{
		XSILogMessage ( "Material on enveloped mesh has no texture!", XSI::siErrorMsg );
		sprintf ( l_szDefaultTextureName, "default.tga" );
	} else {

		W2AHelper2 ( l_szDefaultTextureName, l_pDefaultTexture.GetFullName().GetWideString() );
		
		char l_szTextureFile[MAX_PATH];
		char l_szTextureExt[MAX_PATH];
		
		_splitpath ( l_szDefaultTextureName, NULL, NULL, l_szTextureFile, l_szTextureExt );
		sprintf ( l_szDefaultTextureName, "%s%s",  l_szTextureFile, l_szTextureExt );
	}

	XSI::Geometry l_pGeo = l_pPrim.GetGeometry();
	XSI::CPointRefArray l_pPoints = l_pGeo.GetPoints();
	XSI::PolygonMesh l_pPolyMesh = l_pGeo;

	XSI::CPointRefArray pointRefArray(l_pPolyMesh.GetPoints());
	XSI::MATH::CVector3Array  positionArray(pointRefArray.GetPositionArray());
	XSI::CPolygonNodeRefArray nodeRefArray(l_pPolyMesh.GetNodes());
	XSI::MATH::CVector3Array  normalArray(nodeRefArray.GetNormalArray());

	//Take care of the UV's
	XSI::CRefArray		clusterRefArray;
	l_pPolyMesh.GetClusters().Filter(XSI::siSampledPointCluster,XSI::CStringArray(),L"",clusterRefArray);

	XSI::Cluster			samplePointClusterUV;
	XSI::CRefArray       uvClusterPropertiesRefArray;
	
	int i;
	for(i=0;i < clusterRefArray.GetCount(); i++)
	{
		XSI::Cluster cluster(clusterRefArray[i]);		
		if(cluster.GetProperties().Filter(XSI::siClsUVSpaceTxtType,XSI::CStringArray(), L"",uvClusterPropertiesRefArray) == XSI::CStatus::OK)
		{
			samplePointClusterUV = cluster;			
			break;
		}
	}

	XSI::ClusterProperty uvProp(uvClusterPropertiesRefArray[0]);
	XSI::CClusterPropertyElementArray uvElementArray = uvProp.GetElements();
	XSI::CDoubleArray uvValueArray = uvElementArray.GetArray();
//	long lnbUV= (long)(uvValueArray.GetCount() / 3);

	//
	// Make sure that UVs are present
	//

	if ( !samplePointClusterUV.IsValid() )
	{
		XSILogMessage ( "Invalid .SMD: Enveloped mesh doesn't have any UVs.", XSI::siErrorMsg );

		return SI_ERR_ERROR_MSG;
	}

	m_pBar.PutCaption( L"Analyzing clusters..." );
	m_pBar.PutStatusText( L"" );
	m_pBar.PutVisible( true );


	SMDNode* in_pNode = in_pNodeList->GetByName ( m_pModel.GetFullName() );
	CSIBCArray < CSIBCVector3D >	l_pVertexNormals;
	ComputeVertexNormals ( l_pVertexNormals, l_pGeo );

	XSI::CRefArray		allClusters;
	l_pPolyMesh.GetClusters().Filter(L"poly",XSI::CStringArray(),L"",allClusters);

	CSIBCArray<MateriaList>	matList;
	
	for (int c=0;c<allClusters.GetCount();c++)
	{
		XSI::Cluster Thecluster = allClusters[c];
		XSI::Material l_pMat = Thecluster.GetMaterial();
		XSI::OGLTexture l_pTexture = l_pMat.GetOGLTexture();
		matList.Extend(1);

		if ( !l_pTexture.IsValid() )
		{
			char mess[1024];
			sprintf ( mess, "Cluster #%d has no texture! Bypassing.", c);
			XSILogMessage ( mess, XSI::siErrorMsg );
			continue;
		}

		W2AHelper2 ( matList[matList.GetUsed()-1].texture, l_pTexture.GetFullName().GetWideString() );

		char l_szTextureFile[MAX_PATH];
		char l_szTextureExt[MAX_PATH];

		_splitpath ( matList[matList.GetUsed()-1].texture, NULL, NULL, l_szTextureFile, l_szTextureExt );
		sprintf ( matList[matList.GetUsed()-1].texture, "%s%s",  l_szTextureFile, l_szTextureExt );


		XSI::CClusterElementArray clusterElementArray = Thecluster.GetElements();
		XSI::CLongArray values(clusterElementArray.GetArray());
		long countPolyIndices = values.GetCount();
		matList[matList.GetUsed()-1].polyIndices.Extend(countPolyIndices);
		for (int v=0;v<countPolyIndices;v++)
		{
			matList[matList.GetUsed()-1].polyIndices[v] = values[v];
		}
	}

	XSI::CTriangleRefArray tris = l_pPolyMesh.GetTriangles();

	
	m_pBar.PutCaption( L"Processing geometry..." );
	m_pBar.PutStatusText( L"" );
	m_pBar.PutVisible( true );

	long progress_value = 0;
	long last_progress_value = 0;

		int vindex = 0;
		int vii = 0;

		for (int v=0;v<tris.GetCount();v++)
		{
			progress_value = (long)(((float)v / (float)tris.GetCount()) * 100.0f);

			if ( progress_value != last_progress_value )
			{
				last_progress_value = progress_value;
				m_pBar.PutValue ( progress_value );
				
				if ( m_pBar.IsCancelPressed() )
				{
					if ( MessageBox ( NULL, "Cancelling the export will create a corrupted SMD file.\n\n Are you sure?", "Cancel Export", MB_YESNO|MB_ICONWARNING ) == IDYES )
					{
						break;
					} else {

						m_pBar.PutVisible( true );
					}

				}

			}
			
			XSI::Triangle tri = tris[v];

			char* textureName = l_szDefaultTextureName;

			long polyI = tri.GetPolygonIndex();

			for (int p=0;p<matList.GetUsed();p++)
			{
				bool found = false;

				for (int h=0;h<matList[p].polyIndices.GetUsed();h++)
				{
					if ( matList[p].polyIndices[h] == polyI )
					{
						textureName = matList[p].texture;
						found = true;
						break;
					}

				}

				if ( found )
					break;
			}


			for (int i=0;i<3;i++)
			{
				//
				// Build a vertex
				//

				CSIBCVector3D	l_vPosition = CSIBCVector3D( (float)tri.GetPositionArray()[i].GetX(), 
																(float)tri.GetPositionArray()[i].GetY(), 
																(float)tri.GetPositionArray()[i].GetZ() );

				XSI::CTriangleVertexRefArray vRef = tri.GetPoints();
				XSI::TriangleVertex l_vTriangleVertex = vRef[i];
				

				CSIBCVector3D	l_vNormal = CSIBCVector3D ( (float)l_vTriangleVertex.GetNormal().GetX(),
															(float)l_vTriangleVertex.GetNormal().GetY(),
															(float)l_vTriangleVertex.GetNormal().GetZ() );

				//if ( SMDType == 1 )
				//{
				//	l_vNormal	= l_pVertexNormals [ l_pVertexList[vii] ];
				//} 

				if (( SMDType == 0 ) || ( SMDType == 1 ))
				{
					CSIBCMatrix4x4 l_pResult = in_pNode->GetMatrix();

					l_pResult.Multiply ( l_vPosition, l_vPosition );
					l_pResult.Multiply ( l_vNormal, l_vNormal );
					l_vNormal = l_vNormal.Normalize();
					
				}

				XSI::CTriangleVertexRefArray l_TriRef = tri.GetPoints();
				XSI::CUVArray l_vuvArray = l_TriRef.GetUVArray();
				XSI::CUV l_uv = l_vuvArray[i];
//				int c = l_vuvArray.GetCount();
				long vertexIndex = tri.GetIndexArray()[i];

				CSIBCVector2D	l_vUV;
				l_vUV.m_fX = (float)l_uv.u;
				l_vUV.m_fY = (float)l_uv.v;

				//
				// Build weight list
				//

				SMDVertex*	l_pWeights = m_pVertexList[vertexIndex];

				// not sure the line above works correclty, if not, uncomment
				// the code below
				//for (int w=0;w<m_pVertexList.GetUsed();w++)
				//{
				//	if ( m_pVertexList[w]->GetIndex() == vertexIndex )
				//	{
				//		l_pWeights = m_pVertexList[w];
				//		break;
				//		
				//	}

				//}

				vii++;
				
				//
				// Now output
				//

			
			//	XSI::OGLTexture l_pTexture = useMat.GetOGLTexture();

				if ( vindex == 0 )
				{
					fprintf ( l_fptr, "%s\n", textureName );
				}

				if ( !rigid )
				{
					CSIBCString l_szWeight;
					l_szWeight.Concat ( l_pWeights->GetNumWeights() );
					l_szWeight.Concat (" ");
					for (int f=0;f<l_pWeights->GetNumWeights();f++)
					{
						l_szWeight.Concat ( l_pWeights->GetWeight(f)->m_iBoneID );
						l_szWeight.Concat (" ");
						l_szWeight.Concat ( l_pWeights->GetWeight(f)->m_fWeight );
						l_szWeight.Concat (" ");
					}

					fprintf ( l_fptr, 
								"0 %f %f %f %f %f %f %f %f %s\n",
								l_vPosition.m_fX,
								l_vPosition.m_fY,
								l_vPosition.m_fZ,
								l_vNormal.m_fX,
								l_vNormal.m_fY,
								l_vNormal.m_fZ,
								l_vUV.m_fX,
								l_vUV.m_fY,
								l_szWeight.GetText());
				} else {

					fprintf ( l_fptr, 
								"%d %f %f %f %f %f %f %f %f \n",
								l_pWeights->GetWeight(0)->m_iBoneID,
								l_vPosition.m_fX,
								l_vPosition.m_fY,
								l_vPosition.m_fZ,
								l_vNormal.m_fX,
								l_vNormal.m_fY,
								l_vNormal.m_fZ,
								l_vUV.m_fX,
								l_vUV.m_fY
								);
				}



				vindex++;

				if ( vindex == 3 )
					vindex = 0;
			}

		}

		
	

	return SI_SUCCESS;
}

void	SMDEnvelope::ConvertToModelSMD ()
{
	//
	// Get rid of vertex assignements
	//

	int l_iNbVertex = m_pVertexList.GetUsed();

	for (int m=0;m<m_pVertexList.GetUsed();m++)
	{
		SMDVertex*	l_pEnv = m_pVertexList[m];

		delete l_pEnv;
	}

	m_pVertexList.DisposeData();

	//
	// Rebuild and make every vertex point to the root bone with a weight of 100%
	for (int v=0;v<l_iNbVertex;v++)
	{
		SMDVertex* l_pVertex = new SMDVertex;

		l_pVertex->SetIndex (v);
		l_pVertex->AddWeight ( 0, 1.0f );

		m_pVertexList.Extend(1);
		m_pVertexList[m_pVertexList.GetUsed()-1] = l_pVertex;

	}
	
}

void SMDEnvelope::ComputeVertexNormals ( CSIBCArray < CSIBCVector3D >& in_pVertexNormals, XSI::Geometry in_pGeo)
{

	XSI::PolygonMesh mesh( in_pGeo);
	XSI::CTriangleRefArray tris = mesh.GetTriangles();

	XSI::CPointRefArray l_pPoints = in_pGeo.GetPoints();
	in_pVertexNormals.Extend ( l_pPoints.GetCount() );

	//
	// Initialize all normals to zero
	//

	for (int z=0;z<l_pPoints.GetCount();z++)
	{
		in_pVertexNormals[z] = CSIBCVector3D ( 0.0f,0.0f, 0.0f);

	}

	for (int t=0;t<tris.GetCount();t++)
	{
		XSI::Triangle tri = tris[t];
	
		int vii = 0;

		CSIBCVector3D	v0 = CSIBCVector3D( (float)tri.GetPositionArray()[0].GetX(), 
											(float)tri.GetPositionArray()[0].GetY(), 
											(float)tri.GetPositionArray()[0].GetZ() );

		
		CSIBCVector3D	v1 = CSIBCVector3D( (float)tri.GetPositionArray()[1].GetX(), 
											(float)tri.GetPositionArray()[1].GetY(), 
											(float)tri.GetPositionArray()[1].GetZ() );

		
		CSIBCVector3D	v2 = CSIBCVector3D( (float)tri.GetPositionArray()[2].GetX(), 
											(float)tri.GetPositionArray()[2].GetY(), 
											(float)tri.GetPositionArray()[2].GetZ() );


		CSIBCVector3D A = (v1-v0);
		CSIBCVector3D B = (v2-v1);
		
		CSIBCVector3D Normal = A.CrossProduct ( B );
		
		in_pVertexNormals [ tri.GetIndexArray()[0] ] = in_pVertexNormals [ tri.GetIndexArray()[0] ] + Normal;
		in_pVertexNormals [ tri.GetIndexArray()[1] ] = in_pVertexNormals [ tri.GetIndexArray()[1] ] + Normal;
		in_pVertexNormals [ tri.GetIndexArray()[2] ] = in_pVertexNormals [ tri.GetIndexArray()[2] ] + Normal;
		
		vii += 3;

	}

	//
	// Now normalize the list of vectors to get averaged normal
	//

	for (long n=0;n<l_pPoints.GetCount();n++)
	{
		in_pVertexNormals[n] = in_pVertexNormals[n].Normalize();
	}

}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SMDTriangle::SMDTriangle()
{
	rigid = 0;
}

SMDTriangle::~SMDTriangle()
{
	for (int m=0;m<m_lEnvelopeList.GetUsed();m++)
	{
		SMDEnvelope*	l_pEnv = m_lEnvelopeList[m];

		delete l_pEnv;
	}

	m_lEnvelopeList.DisposeData();
	
}

SI_Error	SMDTriangle::AddEnvelope (  XSI::X3DObject in_pModel )
{
	//
	// make sure this model isnt already in our list
	//

	for (int m=0;m<m_lEnvelopeList.GetUsed();m++)
	{
		SMDEnvelope*	l_pEnv = m_lEnvelopeList[m];
		if ( l_pEnv->GetModel () == in_pModel )
		{
			return SI_ERR_ERROR_MSG;
		}

	}

	SMDEnvelope*	l_pNewEnvelope = new SMDEnvelope;
	l_pNewEnvelope->SetModel ( in_pModel );

	m_lEnvelopeList.Extend(1);
	m_lEnvelopeList[m_lEnvelopeList.GetUsed()-1] = l_pNewEnvelope;

	return SI_SUCCESS;

}

SI_Error	SMDTriangle::AddWeight	( XSI::X3DObject in_pModel, 
										SI_Int in_iBoneID, 
										SI_Int in_iVertexID, 
										SI_Float in_fWeight)
{

	if ( in_fWeight < 0.00001f )
		return SI_SUCCESS;

	in_fWeight /= 100.0f;

	for (int m=0;m<m_lEnvelopeList.GetUsed();m++)
	{
		SMDEnvelope*	l_pEnv = m_lEnvelopeList[m];
		if ( l_pEnv->GetModel () == in_pModel )
		{
			l_pEnv->AddWeight ( in_iVertexID, in_iBoneID, in_fWeight );
		}

	}
	return SI_SUCCESS;
}

SI_Error	SMDTriangle::Write ( FILE* l_fptr )
{
	fprintf ( l_fptr, "triangles\n" );
	for (int m=0;m<m_lEnvelopeList.GetUsed();m++)
	{
		m_lEnvelopeList[m]->Write ( l_fptr, rigid, m_pNodeList );
	}
	fprintf ( l_fptr, "end\n" );
	return SI_SUCCESS;

}

void		SMDTriangle::ConvertToModelSMD ()
{
	for (int m=0;m<m_lEnvelopeList.GetUsed();m++)
	{
		m_lEnvelopeList[m]->ConvertToModelSMD();
	}
}



void W2AHelper2( LPSTR out_sz, LPCWSTR in_wcs, int in_cch )
{
	if ( out_sz != NULL && 0 != in_cch )
	{
		out_sz[0] = '\0' ;

		if ( in_wcs != NULL )
		{
			size_t l_iLen = 0;
			l_iLen = ::wcstombs( out_sz, in_wcs, ( in_cch < 0 ) ? ::wcslen(in_wcs) + 1 : (size_t) in_cch ) ;

			if ( in_cch > 0 ) { out_sz[ in_cch - 1 ] = '\0'; }
		}
	}
}