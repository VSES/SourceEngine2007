// SMDSkeleton.cpp: implementation of the SMDSkeleton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMDNode.h"
#include "SMDSkeleton.h"
#include "SIBCMatrix.h"
#include <xsi_kinematicstate.h>
#include <xsi_application.h>
#include <xsi_project.h>
#include <xsi_property.h>

#define EPSILON	0.000001f

SI_Int	CompareProc( const void *arg1, const void *arg2 );
extern int SMDType;
extern bool ExportSelectedOnly;
extern bool ZeroRootTransforms;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SMDSkeleton::SMDSkeleton()
{

}

SMDSkeleton::~SMDSkeleton()
{
	for (int s=0;s<m_lKeyList.GetUsed();s++)
	{
	//	SMDKey* l_pKey = m_lKeyList[s];

	//	delete l_pKey;
	}

	m_lKeyList.DisposeData();

}

void SMDSkeleton::AddKey ( int in_iFrame, 
						int in_szBoneId,
						CSIBCVector3D in_vPosition,
						CSIBCVector3D in_vRotation )
{
	//
	// Check if the key already exists
	//

	for (int k=0;k<m_lKeyList.GetUsed();k++)
	{
		if (( m_lKeyList[k]->m_iFrame == in_iFrame ) && 
			(m_lKeyList[k]->m_iBoneID == in_szBoneId ))
		{
			return ;
		}


	}

	SMDKey *l_pKey = new SMDKey;

	l_pKey->m_iFrame = in_iFrame;
	l_pKey->m_iBoneID = in_szBoneId;
	l_pKey->m_vPosition = in_vPosition;
	l_pKey->m_vRotation = in_vRotation;

	m_lKeyList.Extend(1);
	m_lKeyList[m_lKeyList.GetUsed()-1] = l_pKey;

}

void SMDSkeleton::Write ( FILE* l_fptr, float in_fStart )
{
	XSI::Application app;
	XSI::Property playControl = app.GetActiveProject().GetProperties().GetItem(L"Play Control");

//	float startframe = playControl.GetParameterValue(L"in");


	Sort( in_fStart );

	if ( m_lKeyList.GetUsed() )
	{
		fprintf ( l_fptr, "skeleton\n" );

		int current_frame = -9999;

		for (int s=0;s<m_lKeyList.GetUsed();s++)
		{

			SMDKey* l_pKey = m_lKeyList[s];

			if (( SMDType == 0) && (l_pKey->m_iFrame != (int)0 ))
				continue;

			if ( current_frame != l_pKey->m_iFrame )
			{
				fprintf ( l_fptr, "time %d\n" , l_pKey->m_iFrame);
				current_frame = l_pKey->m_iFrame;

			}

			CSIBCMatrix4x4 l_oMatrix;
			l_oMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), 
										l_pKey->m_vRotation * 0.01745329251994329547f, 
										l_pKey->m_vPosition );
			
			//
			// Check if the bone is a parent bone
			//

			SMDNode*	l_pNode = m_pList->GetByID ( l_pKey->m_iBoneID );

			SI_Int l_iParentID = l_pNode->GetParentID ();

			SMDNode*	l_pParentNode = m_pList->GetByID ( l_iParentID );
			

			if ( l_pParentNode == NULL )	// this is a parent bone
			//if ( 0 )	// this is a parent bone
			{

				CSIBCMatrix4x4 l_mMatrix;
				l_mMatrix.SetIdentity();
				l_mMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), CSIBCVector3D((const float)1.5707963267948965923, 0.0f, 0.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f) );
			//	l_mMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f) );

				CSIBCMatrix4x4 l_result = l_mMatrix.Multiply ( l_oMatrix );

				CSIBCVector3D	s,r,t;

				l_result.GetTransforms (s,r,t);
				//l_oMatrix.GetTransforms (s,r,t);

				if ( fabs(t.GetX()) < EPSILON)
					t.m_fX =0;
				if ( fabs(t.GetY()) < EPSILON)
					t.m_fY =0;
				if ( fabs(t.GetZ()) < EPSILON)
					t.m_fZ =0;
				
				if ( fabs(r.GetX()) < EPSILON)
					r.m_fX =0;
				if ( fabs(r.GetY()) < EPSILON)
					r.m_fY =0;
				if ( fabs(r.GetZ()) < EPSILON)
					r.m_fZ =0;
				fprintf ( l_fptr, "%d %f %f %f %f %f %f\n" , l_pKey->m_iBoneID, t.GetX(),
																			t.GetY(),
																			t.GetZ(),
																			r.GetX(), /** 0.01745329251994329547f,*/
																			r.GetY(), /** 0.01745329251994329547f,*/
																			r.GetZ() /*** 0.01745329251994329547f*/);

				l_pNode->SetMatrix ( l_result );
				//l_pNode->SetMatrix ( l_oMatrix );
				

			} else {

				if ( fabs(l_pKey->m_vPosition.GetX()) < EPSILON)
					l_pKey->m_vPosition.m_fX =0;
				if ( fabs(l_pKey->m_vPosition.GetY()) < EPSILON)
					l_pKey->m_vPosition.m_fY =0;
				if ( fabs(l_pKey->m_vPosition.GetZ()) < EPSILON)
					l_pKey->m_vPosition.m_fZ =0;
				
				if ( fabs(l_pKey->m_vRotation.GetX()) < EPSILON)
					l_pKey->m_vRotation.m_fX =0;
				if ( fabs(l_pKey->m_vRotation.GetY()) < EPSILON)
					l_pKey->m_vRotation.m_fY =0;
				if ( fabs(l_pKey->m_vRotation.GetZ()) < EPSILON)
					l_pKey->m_vRotation.m_fZ =0;

				fprintf ( l_fptr, "%d %f %f %f %f %f %f\n" , l_pKey->m_iBoneID, l_pKey->m_vPosition.GetX(),
																			l_pKey->m_vPosition.GetY(),
																			l_pKey->m_vPosition.GetZ(),
																			l_pKey->m_vRotation.GetX() * 0.01745329f,
																			l_pKey->m_vRotation.GetY() * 0.01745329f,
																			l_pKey->m_vRotation.GetZ() * 0.01745329f);

				CSIBCMatrix4x4 l_mMatrix;
				l_mMatrix.SetIdentity();
				l_mMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f) , l_pKey->m_vRotation * 0.01745329251994329547f , l_pKey->m_vPosition);
				l_pNode->SetMatrix ( l_mMatrix );

			}
			

		}

		fprintf ( l_fptr, "end\n" );

	}


}

void SMDSkeleton::Sort ( float in_fBase )
{
	//
	// Sort the bone list by frame
	//

	CSIBCArray<SMDKey*>	l_lSubKeyList;
	CSIBCArray<SMDKey*>	l_lFinalKeyList;

	int l_iCurrentFrame = (int)(in_fBase);
	
	while (1)
	{
		for (int c=0;c<m_lKeyList.GetUsed();c++)
		{

			if ( m_lKeyList[c]->m_iFrame == l_iCurrentFrame )
			{
				l_lSubKeyList.Extend(1);
				l_lSubKeyList[l_lSubKeyList.GetUsed()-1] = m_lKeyList[c];
			}
		}

		if ( l_lSubKeyList.GetUsed() )
		{
			qsort( l_lSubKeyList.ArrayPtr(), l_lSubKeyList.GetUsed(), sizeof( SMDKey* ), CompareProc );

			for (int e=0;e<l_lSubKeyList.GetUsed();e++)
			{
				l_lFinalKeyList.Extend (1);
				l_lFinalKeyList[l_lFinalKeyList.GetUsed()-1] = l_lSubKeyList[e];
			}

			l_lSubKeyList.DisposeData();

			l_iCurrentFrame++;	
		} else {
			break;
		}

	}

	for (int c=0;c<l_lFinalKeyList.GetUsed();c++)
	{
		m_lKeyList[c] = l_lFinalKeyList[c];
	}

	l_lFinalKeyList.DisposeData();


}

SI_Int	CompareProc( const void *arg1, const void *arg2 )
{
	SMDKey* pItem1 = ((SMDKey**)arg1)[0];
	SMDKey* pItem2 = ((SMDKey**)arg2)[0];

	if ( pItem1->m_iBoneID > pItem2->m_iBoneID )
		return 1;

	if ( pItem1->m_iBoneID < pItem2->m_iBoneID )
		return -1;

	return 0;


}

void	SMDSkeleton::ConvertToModelSMD ()
{
	//
	// Remove all keys
	//

	for (int s=0;s<m_lKeyList.GetUsed();s++)
	{
		SMDKey* l_pKey = m_lKeyList[s];

		delete l_pKey;
	}

	m_lKeyList.DisposeData();

	//
	// Create a single key for the root node
	//

	for (int n=0;n<m_pList->GetNbNodes();n++)
	{
		SMDNode* l_pNode = m_pList->GetByIndex ( n );

		if ( !l_pNode->GetModel().IsValid() )
		{
			AddKey ( 0, l_pNode->GetID () , CSIBCVector3D(0.0f, 0.0f, 0.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f) );
		
		} else {

			XSI::X3DObject	l_pModel = l_pNode->GetModel();
			XSI::MATH::CTransformation l_pTrans = l_pModel.GetKinematics().GetGlobal().GetTransform();
			l_pTrans.PutRotationOrder (XSI::MATH::CRotation::siXYZ);
			
			double rx,ry,rz;
			double x,y,z;
			l_pTrans.GetRotationFromXYZAnglesValues ( rx,ry,rz );
			l_pTrans.GetTranslationValues(x,y,z);
			rx *= 57.29577951308232286465;
			ry *= 57.29577951308232286465;
			rz *= 57.29577951308232286465;

			// did we want to zero out the root nodes?
			if ((ExportSelectedOnly)&&(ZeroRootTransforms))
			{
				//is this a root node?
				if ( l_pNode->GetParentID()==-1 )
				{
					rx=ry=rz=x=y=z=0;
				}
			}

			AddKey ( 0, 
					l_pNode->GetID () , 
					CSIBCVector3D((float)x,(float)y,(float)z), 
					CSIBCVector3D((float)rx,(float)ry,(float)rz) );

		}

	}


}

void	SMDSkeleton::CreateDummyKeys ( int in_iNbKeys )
{
/*
	//
	// Remove all keys
	//

	for (int s=0;s<m_lKeyList.GetUsed();s++)
	{
		SMDKey* l_pKey = m_lKeyList[s];

		delete l_pKey;
	}

	m_lKeyList.DisposeData();

	//
	// Create a single key for the root node
	//

	for (int n=0;n<m_pList->GetNbNodes();n++)
	{
		SMDNode* l_pNode = m_pList->GetByIndex ( n );

		if ( !l_pNode->GetModel().IsValid() )
		{
			for (int k=0;k<in_iNbKeys;k++)
			{
				AddKey ( k, l_pNode->GetID () , CSIBCVector3D(0.0f, 0.0f, 0.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f) );
			}
		
		} else {

			XSI::X3DObject	l_pModel = l_pNode->GetModel();
			l_pModel->Transform()->ComputeGlobalMatrix();

			CSIBCMatrix4x4 l_pGlobalMatrix = l_pModel->Transform()->GetGlobalMatrix();

			CSIBCVector3D	l_vScale;
			CSIBCVector3D	l_vRotation;
			CSIBCVector3D	l_vTranslation;

			l_pGlobalMatrix.GetTransforms( l_vScale, 
											l_vRotation, 
											l_vTranslation);

			for (int k=0;k<in_iNbKeys;k++)
			{
				AddKey ( k, l_pNode->GetID () , l_vTranslation, l_vRotation );
			}

		}

	}
*/

}