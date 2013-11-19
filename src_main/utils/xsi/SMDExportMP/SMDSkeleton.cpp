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


SI_Int	CompareProc( const void *arg1, const void *arg2 );
extern int SMDType;

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

	float startframe = playControl.GetParameterValue(L"in");


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

				// this is where the model is rotated to eliminate the need for the "$upaxis Y" command in the .qc files.
				// uncomment the line below to put this back to the original exporter implementation
//				l_mMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), CSIBCVector3D((const float)1.5707963267948965923, 0.0f, 0.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f) );
				l_mMatrix.SetTransforms ( CSIBCVector3D(1.0f, 1.0f, 1.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f), CSIBCVector3D(0.0f, 0.0f, 0.0f) );

				CSIBCMatrix4x4 l_result = l_mMatrix.Multiply ( l_oMatrix );

				CSIBCVector3D	s,r,t;

				l_result.GetTransforms (s,r,t);
				//l_oMatrix.GetTransforms (s,r,t);

				fprintf ( l_fptr, "%d %f %f %f %f %f %f\n" , l_pKey->m_iBoneID, t.GetX(),
																			t.GetY(),
																			t.GetZ(),
																			r.GetX(), /** 0.01745329251994329547f,*/
																			r.GetY(), /** 0.01745329251994329547f,*/
																			r.GetZ() /*** 0.01745329251994329547f*/);

				l_pNode->SetMatrix ( l_result );
				//l_pNode->SetMatrix ( l_oMatrix );
				

			} else {

				fprintf ( l_fptr, "%d %f %f %f %f %f %f\n" , l_pKey->m_iBoneID, l_pKey->m_vPosition.GetX(),
																			l_pKey->m_vPosition.GetY(),
																			l_pKey->m_vPosition.GetZ(),
																			l_pKey->m_vRotation.GetX() * 0.01745329251994329547f,
																			l_pKey->m_vRotation.GetY() * 0.01745329251994329547f,
																			l_pKey->m_vRotation.GetZ() * 0.01745329251994329547f);

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
			
			double rx,ry,rz;
			double x,y,z;
			l_pTrans.GetRotationFromXYZAnglesValues ( rx,ry,rz );
			l_pTrans.GetTranslationValues(x,y,z);
			rx *= 57.29577951308232286465;
			ry *= 57.29577951308232286465;
			rz *= 57.29577951308232286465;

			AddKey ( 0, 
					l_pNode->GetID () , 
					CSIBCVector3D((float)x,(float)y,(float)z), 
					CSIBCVector3D((float)rx,(float)ry,(float)rz) );

		}

	}


}

void	SMDSkeleton::CreateDummyKeys ( int in_iNbKeys )
{

}