// SMDNode.cpp: implementation of the SMDNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMDNode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SMDNode::SMDNode() :
m_iNodeID ( -1 ),
m_iNodeParentID ( -1 ),
m_pModel ( NULL ),
m_mHierarchyCompensation ( NULL )
{

}

SMDNode::~SMDNode()
{

}

SI_Error SMDNode::Write( FILE *l_fptr )
{
	fprintf ( l_fptr, "%d %c%s%c %d\n",GetID(), 34, GetName(), 34, GetParentID() );

	return SI_SUCCESS;
}

void	SMDNode::SetHierarchicalCompensation( CSLTransform* in_pTransform )
{
	m_mHierarchyCompensation = in_pTransform;

}

void	SMDNode::SetHierarchicalCompensation2( CSLTransform* in_pTransform )
{
	m_mHierarchyCompensation2 = in_pTransform;

}

SMDNodeList::SMDNodeList()
{}

SMDNodeList::~SMDNodeList()
{
	for (int s=0;s<m_pNodeList.GetUsed();s++)
	{
		SMDNode* l_pNode = m_pNodeList[s];

		delete l_pNode;
	}

	m_pNodeList.DisposeData();

}

void SMDNodeList::AddNode ( char *in_szName, int in_iParentID, CSLModel * in_pModel, CSLTransform*	in_pHierarchyCompensation ,CSLTransform*	in_pHierarchyCompensation2 )
{

	for (int n=0;n<m_pNodeList.GetUsed();n++)
	{
		if ( !strcmp ( m_pNodeList[n]->GetName(), in_szName ) )
		{
			return;
		}
	}

	SMDNode	*l_pNewNode = new SMDNode;

	l_pNewNode->SetName ( in_szName );
	l_pNewNode->SetParentID ( in_iParentID );
	l_pNewNode->SetModel ( in_pModel );
	l_pNewNode->SetHierarchicalCompensation ( in_pHierarchyCompensation );
	l_pNewNode->SetHierarchicalCompensation2 ( in_pHierarchyCompensation2 );
	

	m_pNodeList.Extend(1);
	m_pNodeList[m_pNodeList.GetUsed()-1] = l_pNewNode;

	l_pNewNode->SetID ( m_pNodeList.GetUsed()-1 );

}

void SMDNodeList::AddNode ( char *in_szName, char *in_szParentName, CSLTransform*	l_pHierarchyCompensation, CSLTransform*	in_pHierarchyCompensation2 )
{
	SI_Int		l_iParentID;

	SMDNode*	l_pParent = GetByName ( in_szParentName );
	if ( l_pParent != NULL )
	{
		l_iParentID = l_pParent->GetID ();
	} else {
		l_iParentID = -1;
	}
	
	AddNode ( in_szName, l_iParentID, NULL, l_pHierarchyCompensation, in_pHierarchyCompensation2 );
}

void SMDNodeList::Write   ( FILE* in_fptr )
{
	if ( m_pNodeList.GetUsed() )
	{
		fprintf ( in_fptr, "nodes\n" );

		for (int s=0;s<m_pNodeList.GetUsed();s++)
		{
			SMDNode* l_pNode = m_pNodeList[s];

			l_pNode->Write ( in_fptr );

		}
		
		fprintf ( in_fptr, "end\n" );
	}

}

SMDNode* SMDNodeList::GetByName ( SI_Char* in_szName )
{
	for (int s=0;s<m_pNodeList.GetUsed();s++)
	{
		SMDNode* l_pNode = m_pNodeList[s];

		if ( !_SI_STRCMP ( l_pNode->GetName(),in_szName) )
		{
			return l_pNode;
		}
	}

	return NULL;

}

SMDNode*	SMDNodeList::GetByID ( SI_Int in_iIndex )
{
	for (int s=0;s<m_pNodeList.GetUsed();s++)
	{
		SMDNode* l_pNode = m_pNodeList[s];

		if (l_pNode->GetID() == in_iIndex )
		{
			return l_pNode;
		}
	}

	return NULL;
}

SMDNode*	SMDNodeList::GetByIndex ( SI_Int in_iIndex )
{

	return m_pNodeList[in_iIndex];
}



void	SMDNodeList::ConvertToModelSMD ()
{
	//
	// Remove all nodes
	//

	for (int s=0;s<m_pNodeList.GetUsed();s++)
	{
		SMDNode* l_pNode = m_pNodeList[s];

		delete l_pNode;
	}

	m_pNodeList.DisposeData();

	//
	// Create single root node
	//

	AddNode ( "root", -1, NULL );

}
