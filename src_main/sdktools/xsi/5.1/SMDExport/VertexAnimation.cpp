// VertexAnimation.cpp: implementation of the CVertexAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VertexAnimation.h"
#include "BaseShape.h"
#include "Model.h"
#include "Transform.h"
#include "TriangleList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShapeAnimationFrame::CShapeAnimationFrame()
{
}

CShapeAnimationFrame::~CShapeAnimationFrame()
{
	m_pVertexList.DisposeData();
	m_pNormalList.DisposeData();
	m_pIndexList.DisposeData();

}

void CShapeAnimationFrame::ComputeDelta  ( CSLMesh*	in_pMesh, CSLBaseShape*	l_pShape1, CSLBaseShape*	l_pShape2 )
{
	CSIBCArray<ExplodedVertex>	l_ExplodedShape1;
	CSIBCArray<ExplodedVertex>	l_ExplodedShape2;

	ExplodeMesh	( in_pMesh, l_pShape1, l_pShape2, &l_ExplodedShape1, &l_ExplodedShape2 );


	if ( l_pShape1 == NULL )
	{
		//
		// First shape, just grab all vertices and normals
		//


		for (long v=0;v<l_ExplodedShape2.GetUsed();v++)
		{

			m_pVertexList.Extend(1);
			m_pVertexList[m_pVertexList.GetUsed()-1] = l_ExplodedShape2[v].m_vPosition;

			m_pNormalList.Extend(1);
			m_pNormalList[m_pNormalList.GetUsed()-1] = l_ExplodedShape2[v].m_vNormal;

			m_pIndexList.Extend(1);
			m_pIndexList[m_pIndexList.GetUsed()-1] = v;

		}

		
	} else {

		for (long v=0;v<l_ExplodedShape2.GetUsed();v++)
		{
			if (( l_ExplodedShape1[v].m_vPosition != l_ExplodedShape2[v].m_vPosition ) || ( l_ExplodedShape1[v].m_vNormal != l_ExplodedShape2[v].m_vNormal ))
			{
				//
				// either the vertex position is different, or the normal..
				// lets save it
				//

				m_pVertexList.Extend(1);
				m_pVertexList[m_pVertexList.GetUsed()-1] = l_ExplodedShape2[v].m_vPosition;

				m_pNormalList.Extend(1);
				m_pNormalList[m_pNormalList.GetUsed()-1] = l_ExplodedShape2[v].m_vNormal;

				m_pIndexList.Extend(1);
				m_pIndexList[m_pIndexList.GetUsed()-1] = v;

			}

		}


	}


}

SI_Error	CShapeAnimationFrame::Write ( FILE* l_fptr, CSIBCMatrix4x4 in_mGMatrix )
{

	if ( m_pVertexList.GetUsed() )
	{
		fprintf ( l_fptr, "time %d\n", ((int)m_fTime));

		for (int v=0;v<m_pVertexList.GetUsed();v++)
		{
			CSIBCVector3D	l_vTransformed;
			CSIBCVector3D	l_vNTransformed;

			in_mGMatrix.Multiply ( l_vTransformed, m_pVertexList[v] );
			in_mGMatrix.Multiply ( l_vNTransformed, m_pNormalList[v] );

			l_vNTransformed = l_vNTransformed.Normalize();

			float vx, vy, vz, inv_mod;
			vx = l_vNTransformed.m_fX;
			vy = l_vNTransformed.m_fY;
			vz = l_vNTransformed.m_fZ;

			inv_mod = (1.0f / sqrtf(vx * vx + vy * vy + vz * vz));

			l_vNTransformed.m_fX = vx * inv_mod;
			l_vNTransformed.m_fY = vy * inv_mod;
			l_vNTransformed.m_fZ = vz * inv_mod;

			if (( l_vTransformed.m_fX - -1.659382f ) < 0.00001f)
			{
				int a = 10;

			}

			fprintf ( l_fptr, "%d %f %f %f %f %f %f\n", m_pIndexList[v],
														l_vTransformed.m_fX,
														l_vTransformed.m_fY,
														l_vTransformed.m_fZ,
														l_vNTransformed.m_fX,
														l_vNTransformed.m_fY,
														l_vNTransformed.m_fZ);

		}
	}
		
	return SI_SUCCESS;
}

void	CShapeAnimationFrame::ExplodeMesh	( CSLMesh*	in_pMesh, CSLBaseShape* in_pLastFrame, CSLBaseShape* in_pFrame, CSIBCArray<ExplodedVertex>* in_pE1, CSIBCArray<ExplodedVertex>* in_pE2 )
{


	SLIndexedVector3D *l_pVertices1	= NULL;
	SLIndexedVector3D *l_pNormals1	= NULL;

	if ( in_pLastFrame != NULL )
	{
		l_pVertices1	= in_pLastFrame->GetIndexedVertexListPtr();
		l_pNormals1	= in_pLastFrame->GetIndexedNormalListPtr();
	}

	SLIndexedVector3D *l_pVertices2	= in_pFrame->GetIndexedVertexListPtr();
	SLIndexedVector3D *l_pNormals2	= in_pFrame->GetIndexedNormalListPtr();


	for (int t=0;t<in_pMesh->GetTriangleListCount();t++)
	{
		CSLTriangleList* l_pTriList = in_pMesh->TriangleLists()[t];

		SI_Int* l_pVertexList = l_pTriList->GetVertexIndicesPtr();
		SI_Int* l_pNormalList = l_pTriList->GetNormalIndicesPtr();

		int vii = 0;

		for (int v=0;v<l_pTriList->GetTriangleCount()*3;v++)
		{
			CSIBCVector3D	v0 ;
			CSIBCVector3D	n0 ;

			if ( in_pLastFrame != NULL )
			{
				v0 = l_pVertices1[l_pVertexList[v]].m_Vector3D;
				n0 = l_pNormals1[l_pNormalList[v]].m_Vector3D;
			}

			CSIBCVector3D	v1 = l_pVertices2[l_pVertexList[v]].m_Vector3D;
			CSIBCVector3D	n1 = l_pNormals2[l_pNormalList[v]].m_Vector3D;

			in_pE1->Extend(1);
			(*in_pE1)[in_pE1->GetUsed()-1].m_vPosition = v0;
			(*in_pE1)[in_pE1->GetUsed()-1].m_vNormal = n0;

			in_pE2->Extend(1);
			(*in_pE2)[in_pE2->GetUsed()-1].m_vPosition = v1;
			(*in_pE2)[in_pE2->GetUsed()-1].m_vNormal = n1;


		}
	}
	

}

CVertexAnimation::CVertexAnimation() :
m_pModel ( NULL )
{

}

CVertexAnimation::~CVertexAnimation()
{

	for (int v=0;v<m_pFrames.GetUsed();v++)
	{
		CShapeAnimationFrame* l_pFrame = m_pFrames[v];

		delete l_pFrame;

	}

	m_pFrames.DisposeData();

}

void	CVertexAnimation::AddAnimationFrame( float in_fTime, CSLMesh*	in_pMesh, CSLBaseShape*	l_pShape1, CSLBaseShape*	l_pShape2 )
{

	CShapeAnimationFrame* l_pNewFrame = new CShapeAnimationFrame;

	m_pFrames.Extend(1);
	m_pFrames[m_pFrames.GetUsed()-1] = l_pNewFrame;

	l_pNewFrame->SetTime ( in_fTime );
	l_pNewFrame->ComputeDelta  ( in_pMesh, l_pShape1, l_pShape2 );


}


SI_Error	CVertexAnimation::Write ( FILE* l_fptr, SMDNodeList* in_pNodeList)
{
	//
	// Get the global matrix for the model so that we can transform the vertices into world space
	//

	if ( !m_pModel )
	{
		return SI_ERROR;
	}

	SMDNode*	l_pNode = in_pNodeList->GetByName ( m_pModel->GetName() );

	fprintf ( l_fptr, "vertexanimation\n" );

	for (int f=0;f<m_pFrames.GetUsed();f++)
	{
		m_pFrames[f]->Write ( l_fptr, l_pNode->GetMatrix() );

	}

	fprintf ( l_fptr, "end\n" );

	return SI_SUCCESS;
}



