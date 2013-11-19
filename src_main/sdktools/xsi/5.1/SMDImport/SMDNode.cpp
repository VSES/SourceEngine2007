// SMDNode.cpp: implementation of the SMDNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMDNode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SMDNode::SMDNode()
{
	m_pParent = NULL;

}

SMDNode::~SMDNode()
{
	for (int k=0;k<m_pKeys.GetUsed();k++)
	{
		delete m_pKeys[k];
	}
}


void SMDNode::AddChild ( SMDNode* in_pNode )
{
	assert ( in_pNode != NULL );

	m_pChildren.Extend(1);
	m_pChildren[m_pChildren.GetUsed()-1] = in_pNode;
}

void	SMDNode::AddKey ( SMDKey* in_pKey )
{
	assert ( in_pKey != NULL );

	m_pKeys.Extend(1);
	m_pKeys[m_pKeys.GetUsed()-1] = in_pKey;
}

