// SMDTriangle.cpp: implementation of the SMDTriangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMDTriangle.h"
#include "Primitive.h"
#include "Mesh.h"
#include "BaseShape.h"
#include "Shape.h"
#include "Shape_35.h"
#include "UVCoordArray.h"
#include "TriangleList.h"
#include "Material.h"
#include "Texture2d.h"

#include <comdef.h>					// _variant_t
#include <xsisdk.h>
#include <xsi_application.h>
#include <xsiconv.h>


extern void	XSILogMessage ( wchar_t *in_szMessage, XSI::siSeverityType in_eSeverity );
extern int SMDType;
extern int RemoveCharacterRigRoot;

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

SMDEnvelope::SMDEnvelope() :
m_pModel(NULL)
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

void SMDEnvelope::SetModel ( CSLModel *in_pModel )
{
	m_pModel = in_pModel;

	//
	// Create the vertex list
	//

	CSLPrimitive*	l_pPrimitive = m_pModel->Primitive();

	if ( l_pPrimitive->Type() != CSLTemplate::SI_MESH )
	{
		return;
	}

	CSLMesh*		l_pMesh = (CSLMesh*)l_pPrimitive;

	CSLBaseShape*	l_pShape = l_pMesh->Shape();

	SI_Long	l_lVertexCount = l_pShape->GetVertexCount();

	for (int v=0;v<l_lVertexCount;v++)
	{
		SMDVertex* l_pVertex = new SMDVertex;

		l_pVertex->SetIndex (v);
		m_pVertexList.Extend(1);
		m_pVertexList[m_pVertexList.GetUsed()-1] = l_pVertex;

	}



	/*
	CSIBCVector3D*	l_pPosition = NULL;
	CSIBCVector3D*	l_pNormal = NULL;
	CSIBCVector2D*	l_pUV = NULL;

	m_pModel = in_pModel;

	//
	// <Make sure this primitive is a mesh (and not a camera, NURBS surface etc.)
	//

	CSLPrimitive*	l_pPrimitive = m_pModel->Primitive();

	if ( l_pPrimitive->Type() != CSLTemplate::SI_MESH )
	{
		return;
	}

	CSLMesh*		l_pMesh = (CSLMesh*)l_pPrimitive;

	CSLBaseShape*	l_pShape = l_pMesh->Shape();

	//
	// Get normal and vertex lists
	//

	l_pNormal = l_pShape->GetNormalListPtr();
	l_pPosition = l_pShape->GetVertexListPtr();

	if ( l_pShape->Type() == CSLTemplate::SI_SHAPE )
	{
		CSLShape*	l_p30Shape = (CSLShape*)l_pShape;

		l_pUV = l_p30Shape->GetUVCoordListPtr();

	} else {

		if ( l_pShape->Type() == CSLTemplate::SI_SHAPE35 )
		{
			CSLShape_35*	l_p35Shape = (CSLShape_35*)l_pShape;

			if ( l_p35Shape->GetUVCoordArrayCount() )
			{
				CSLUVCoordArray* l_pUVArray = l_p35Shape->UVCoordArrays()[0];

				if ( l_pUVArray != NULL )
				{
					l_pUV = l_pUVArray->GetUVCoordListPtr();
				}

			}

		}
	}

	if (( l_pUV == NULL ) || ( l_pNormal == NULL ) || ( l_pPosition == NULL ))
	{
		return;
	}

	//
	// Make sure that UVs are present
	//

	SI_Long	l_lVertexCount = l_pShape->GetVertexCount();

	for (SI_Long i=0;i<l_lVertexCount;i++)
	{

		m_pVertexList.Extend(1);
		SMDVertex* l_pVertex = &m_pVertexList[m_pVertexList.GetUsed()-1];

		l_pVertex->m_vPosition = l_pPosition[i];
		l_pVertex->m_vNormal = l_pNormal[i];
		l_pVertex->m_vUV = l_pUV[i];

	}
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

	CSIBCVector3D*	l_pPosition = NULL;
	CSIBCVector3D*	l_pNormal = NULL;
	CSIBCVector2D*	l_pUV = NULL;

	//
	// <Make sure this primitive is a mesh (and not a camera, NURBS surface etc.)
	//

	CSLPrimitive*	l_pPrimitive = m_pModel->Primitive();

	if ( l_pPrimitive->Type() != CSLTemplate::SI_MESH )
	{
		XSILogMessage ( L"Invalid .SMD: Envelope is not a polymesh", XSI::siErrorMsg );
		return SI_ERR_ERROR_MSG;
	}

	CSLMesh*		l_pMesh = (CSLMesh*)l_pPrimitive;

	CSLBaseShape*	l_pShape = l_pMesh->Shape();

	//
	// Get normal and vertex lists
	//

	l_pNormal = l_pShape->GetNormalListPtr();
	l_pPosition = l_pShape->GetVertexListPtr();

	if ( l_pShape->Type() == CSLTemplate::SI_SHAPE )
	{
		CSLShape*	l_p30Shape = (CSLShape*)l_pShape;

		l_pUV = l_p30Shape->GetUVCoordListPtr();

	} else {

		if ( l_pShape->Type() == CSLTemplate::SI_SHAPE35 )
		{
			CSLShape_35*	l_p35Shape = (CSLShape_35*)l_pShape;

			if ( l_p35Shape->GetUVCoordArrayCount() )
			{
				CSLUVCoordArray* l_pUVArray = l_p35Shape->UVCoordArrays()[0];

				if ( l_pUVArray != NULL )
				{
					l_pUV = l_pUVArray->GetUVCoordListPtr();
				}

			}

		}
	}

	if (( l_pUV == NULL ) || ( l_pNormal == NULL ) || ( l_pPosition == NULL ))
	{
		XSILogMessage ( L"Invalid .SMD: Enveloped mesh doesn't have any UVs.", XSI::siErrorMsg );

		return SI_ERR_ERROR_MSG;
	}

	//
	// Make sure that UVs are present
	//

	SI_Long	l_lVertexCount = l_pShape->GetVertexCount();

	SMDNode* in_pNode = in_pNodeList->GetByName ( m_pModel->GetName() );

	CSIBCArray < CSIBCVector3D >	l_pVertexNormals;

	ComputeVertexNormals ( l_pVertexNormals, l_pMesh );
	
	for (int t=0;t<l_pMesh->GetTriangleListCount();t++)
	{
		CSLTriangleList* l_pTriList = l_pMesh->TriangleLists()[t];

		SI_Int* l_pVertexList = l_pTriList->GetVertexIndicesPtr();
		SI_Int* l_pNormalList = l_pTriList->GetNormalIndicesPtr();
		SI_Int* l_pUVList = l_pTriList->GetUVIndicesPtr( 0 );
		
		CSLMaterial* l_pMaterial = (CSLMaterial*)l_pTriList->GetMaterial();

		CSLTexture2D* l_pTexture = l_pMaterial->Texture2D();

		if ( l_pTexture == NULL )
		{
			wchar_t mess[1024];
			swprintf ( mess, L"Cluster %s uses Material %s which has no texture.", XA2W(l_pTriList->GetName()) , XA2W(l_pMaterial->GetName()) );
			XSILogMessage ( mess, XSI::siErrorMsg );
			swprintf ( mess, L"Object %s might not be complete in resulting SMD file.", XA2W(l_pMesh->GetName()));
			XSILogMessage ( mess, XSI::siWarningMsg );
			continue;
		}

		int vindex = 0;
		int vii = 0;

		CSIBCMatrix4x4	l_mMatrix;

		if ( RemoveCharacterRigRoot )
		{
			l_mMatrix = l_pMesh->ParentModel()->Transform()->GetGlobalMatrix();

			CSIBCVector3D s,r,t;
			l_mMatrix.GetTransforms ( s,r,t );
		//	r.m_fZ = -1.5707963267948965923;

			r = CSIBCVector3D(0.0f, 0.0f, 0.0f);

			l_mMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), r, CSIBCVector3D(0.0f, 0.0f, 0.0f) );
		} else {
			l_mMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f) );
		}


		for (int v=0;v<l_pTriList->GetTriangleCount();v++)
		{
			/*
					Point3 v0 = l_pMesh->verts[ l_pMesh->faces[i].v[0] ];
		Point3 v1 = l_pMesh->verts[ l_pMesh->faces[i].v[1] ];
		Point3 v2 = l_pMesh->verts[ l_pMesh->faces[i].v[2] ];

		Point3 normal = (v1-v0)^(v2-v1);
		normal = Normalize ( normal );
		*/

			for (int i=0;i<3;i++)
			{
				//
				// Build a vertex
				//

				CSIBCVector3D	l_vPosition = l_pPosition[l_pVertexList[vii]];

				CSIBCVector3D	l_vNormal;

				if ( SMDType == 1 )
				{
					l_vNormal	= l_pVertexNormals [ l_pVertexList[vii] ];
				} else {
					l_vNormal	= l_pNormal[l_pNormalList[vii]];
				}

				CSIBCVector2D	l_vUV		= l_pUV[l_pUVList[vii]];

				if (( SMDType == 0 ) || ( SMDType == 1 ))
				{
					CSIBCMatrix4x4 l_pResult = in_pNode->GetMatrix().Multiply ( l_mMatrix );

					l_pResult.Multiply ( l_vPosition, l_vPosition );
					l_pResult.Multiply ( l_vNormal, l_vNormal );
					l_vNormal = l_vNormal.Normalize();
					
				}


				//
				// Build weight list
				//

				SMDVertex*	l_pWeights = NULL;

				for (int w=0;w<m_pVertexList.GetUsed();w++)
				{
					if ( m_pVertexList[w]->GetIndex() == l_pVertexList[vii] )
					{
						l_pWeights = m_pVertexList[w];
						break;
						
					}

				}

				vii++;
				
				//
				// Now output
				//


				if ( vindex == 0 )
				{
					fprintf ( l_fptr, "%s\n", l_pTexture->GetImageFileName() );
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

void SMDEnvelope::ComputeVertexNormals ( CSIBCArray < CSIBCVector3D >& in_pVertexNormals, CSLMesh* in_pMesh )
{
	CSLBaseShape*	l_pShape = in_pMesh->Shape();
	SI_Long	l_lVertexCount = l_pShape->GetVertexCount();

	in_pVertexNormals.Extend ( l_lVertexCount );

	//
	// Initialize all normals to zero
	//

	for (int z=0;z<l_lVertexCount;z++)
	{
		in_pVertexNormals[z] = CSIBCVector3D ( 0.0f,0.0f, 0.0f);

	}

	CSIBCVector3D*	l_pPosition = NULL;
	
	l_pPosition = l_pShape->GetVertexListPtr();

	for (int t=0;t<in_pMesh->GetTriangleListCount();t++)
	{
		CSLTriangleList* l_pTriList = in_pMesh->TriangleLists()[t];

		SI_Int* l_pVertexList = l_pTriList->GetVertexIndicesPtr();
		
		int vii = 0;

		for (int v=0;v<l_pTriList->GetTriangleCount();v++)
		{
			CSIBCVector3D	v0 = l_pPosition[l_pVertexList[vii]];
			CSIBCVector3D	v1 = l_pPosition[l_pVertexList[vii+1]];
			CSIBCVector3D	v2 = l_pPosition[l_pVertexList[vii+2]];
			
			CSIBCVector3D A = (v1-v0);
			CSIBCVector3D B = (v2-v1);

			CSIBCVector3D Normal = A.CrossProduct ( B );
		
			in_pVertexNormals [ l_pVertexList[vii] ] = in_pVertexNormals [ l_pVertexList[vii] ] + Normal;
			in_pVertexNormals [ l_pVertexList[vii+1] ] = in_pVertexNormals [ l_pVertexList[vii+1] ] + Normal;
			in_pVertexNormals [ l_pVertexList[vii+2] ] = in_pVertexNormals [ l_pVertexList[vii+2] ] + Normal;

			vii += 3;

		}

	}

	//
	// Now normalize the list of vectors to get averaged normal
	//

	for (long n=0;n<l_lVertexCount;n++)
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

SI_Error	SMDTriangle::AddEnvelope ( CSLModel *in_pModel )
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

SI_Error	SMDTriangle::AddWeight	( CSLModel *in_pModel, 
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
