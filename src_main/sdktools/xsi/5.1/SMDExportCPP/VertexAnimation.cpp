// VertexAnimation.cpp: implementation of the CVertexAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VertexAnimation.h"
#include <xsi_application.h>
#include <xsi_property.h>
#include <xsi_project.h>
#include <xsi_polygonmesh.h>
#include <xsi_polygonface.h>
#include <xsi_vertex.h>
#include <xsi_triangle.h>
#include <xsi_trianglevertex.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSIBCArray<ExplodedVertex> CShapeAnimationFrame::m_FirstFrameCache;

CShapeAnimationFrame::CShapeAnimationFrame()
{
}

CShapeAnimationFrame::~CShapeAnimationFrame()
{
	m_pVertexList.DisposeData();
	m_pNormalList.DisposeData();
	m_pIndexList.DisposeData();

}

void CShapeAnimationFrame::ComputeDelta  ( XSI::Primitive in_Primitive, float t1, float t2, bool firstFrame )
{
	CSIBCArray<ExplodedVertex>	l_ExplodedShape1;
	CSIBCArray<ExplodedVertex>	l_ExplodedShape2;

	ExplodeMesh	( in_Primitive, t1, t2, &l_ExplodedShape1, &l_ExplodedShape2, firstFrame );

	if ( firstFrame )
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

//			if (( l_vTransformed.m_fX - -1.659382f ) < 0.00001f)
//			{
//				int a = 10;
//
//			}

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

void	CShapeAnimationFrame::ExplodeMesh	( XSI::Primitive in_Primitive, float t1, float t2, CSIBCArray<ExplodedVertex>* in_pE1, CSIBCArray<ExplodedVertex>* in_pE2, bool firstFrame )
{

	XSI::CPointRefArray  l_pVertices1;
//	XSI::Application app;
//	XSI::Property playControl = app.GetActiveProject().GetProperties().GetItem(L"Play Control");

	XSI::Geometry geo = in_Primitive.GetGeometry( t1 );
	XSI::PolygonMesh mesh (geo);
	XSI::CTriangleRefArray tris = mesh.GetTriangles();

	if ( !firstFrame )
	{
		assert ( m_FirstFrameCache.GetUsed() );
		/*
		for (long f=0;f<tris.GetCount();f++)
		{
			XSI::Triangle thisFace = (XSI::Triangle)tris[f];
			XSI::CTriangleVertexRefArray vertexList = (XSI::CTriangleVertexRefArray) thisFace.GetPoints();

			for (long t=0;t<vertexList.GetCount();t++)
			{
				XSI::TriangleVertex vert = (XSI::TriangleVertex)vertexList[t];

				int vii = 0;
				
				CSIBCVector3D	v0 ;
				CSIBCVector3D	n0 ;
				
				bool valid = true;
				XSI::MATH::CVector3 pos( vert.GetPosition() );
				XSI::MATH::CVector3 nrm( vert.GetNormal());
					
				v0 = CSIBCVector3D ( pos.GetX(), pos.GetY(), pos.GetZ() );
				n0 = CSIBCVector3D ( nrm.GetX(), nrm.GetY(), nrm.GetZ() );
				
				in_pE1->Extend(1);
				(*in_pE1)[in_pE1->GetUsed()-1].m_vPosition = v0;
				(*in_pE1)[in_pE1->GetUsed()-1].m_vNormal = n0;
			}
		}*/
		for (long u=0;u<m_FirstFrameCache.GetUsed();u++)
		{
			in_pE1->Extend(1);
			(*in_pE1)[in_pE1->GetUsed()-1].m_vPosition = m_FirstFrameCache[u].m_vPosition ;
			(*in_pE1)[in_pE1->GetUsed()-1].m_vNormal = m_FirstFrameCache[u].m_vNormal;
		}


		
	} else {

		for (long f=0;f<tris.GetCount();f++)
		{
			XSI::Triangle thisFace = (XSI::Triangle)tris[f];
			XSI::CTriangleVertexRefArray vertexList = (XSI::CTriangleVertexRefArray) thisFace.GetPoints();

			for (long t=0;t<vertexList.GetCount();t++)
			{
//				int vii = 0;

				CSIBCVector3D	v0 ;
				CSIBCVector3D	n0 ;

				in_pE1->Extend(1);
				(*in_pE1)[in_pE1->GetUsed()-1].m_vPosition = v0;
				(*in_pE1)[in_pE1->GetUsed()-1].m_vNormal = n0;
			}
		}

	}

//	playControl.PutParameterValue(L"Current", t2);
	XSI::Geometry geo2 = in_Primitive.GetGeometry( t2 );
	XSI::PolygonMesh mesh2 (geo2);
	XSI::CTriangleRefArray tris2 = mesh2.GetTriangles();

	for (long f=0;f<tris2.GetCount();f++)
	{
		XSI::Triangle thisFace = (XSI::Triangle)tris2[f];
		XSI::CTriangleVertexRefArray vertexList = (XSI::CTriangleVertexRefArray) thisFace.GetPoints();

		for (long t=0;t<vertexList.GetCount();t++)
		{
			XSI::TriangleVertex vert = (XSI::TriangleVertex)vertexList[t];
//			int vii = 0;

//			bool valid = true;
			
			XSI::MATH::CVector3 pos( vert.GetPosition() );
			XSI::MATH::CVector3 nrm( vert.GetNormal());

			CSIBCVector3D	v1;
			CSIBCVector3D	n1;
			v1 = CSIBCVector3D ( (float)pos.GetX(), (float)pos.GetY(), (float)pos.GetZ() );
			n1 = CSIBCVector3D ( (float)nrm.GetX(), (float)nrm.GetY(), (float)nrm.GetZ() );

			in_pE2->Extend(1);
			(*in_pE2)[in_pE2->GetUsed()-1].m_vPosition = v1;
			(*in_pE2)[in_pE2->GetUsed()-1].m_vNormal = n1;



		}
	}

	if ( firstFrame )
	{
		m_FirstFrameCache.DisposeData ();

		for (long u=0;u<in_pE2->GetUsed();u++)
		{
			m_FirstFrameCache.Extend (1);
			m_FirstFrameCache[m_FirstFrameCache.GetUsed()-1].m_vNormal = (*in_pE2)[u].m_vNormal;
			m_FirstFrameCache[m_FirstFrameCache.GetUsed()-1].m_vPosition = (*in_pE2)[u].m_vPosition;
		}
	}

}

CVertexAnimation::CVertexAnimation()
{
	m_fFirstFrame = 0;

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

void	CVertexAnimation::AddAnimationFrame( float in_fTime, XSI::Primitive in_pPrimitive, float t1, float t2, bool firstFrame )
{

	if ( firstFrame )
	{
		// remember the first frame's time
		m_fFirstFrame = t1;
	}

	CShapeAnimationFrame* l_pNewFrame = new CShapeAnimationFrame;

	m_pFrames.Extend(1);
	m_pFrames[m_pFrames.GetUsed()-1] = l_pNewFrame;

	l_pNewFrame->SetTime ( in_fTime );
	l_pNewFrame->ComputeDelta  ( in_pPrimitive, m_fFirstFrame, t2, firstFrame);


}


SI_Error	CVertexAnimation::Write ( FILE* l_fptr, SMDNodeList* in_pNodeList)
{
	//
	// Get the global matrix for the model so that we can transform the vertices into world space
	//

	if ( !m_pModel.IsValid())
	{
		return SI_ERROR;
	}

	SMDNode*	l_pNode = in_pNodeList->GetByName ( m_pModel.GetFullName() );

	fprintf ( l_fptr, "vertexanimation\n" );

	for (int f=0;f<m_pFrames.GetUsed();f++)
	{
		m_pFrames[f]->Write ( l_fptr, l_pNode->GetMatrix() );

	}

	fprintf ( l_fptr, "end\n" );

	return SI_SUCCESS;
}



