// SMDNode.h: interface for the SMDNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMDNODE_H__95D421AF_C21B_4D59_B143_C51502B4E507__INCLUDED_)
#define AFX_SMDNODE_H__95D421AF_C21B_4D59_B143_C51502B4E507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SIBCArray.h>

#include "SMDKey.h"

class SMDNode  
{
public:
	SMDNode();
	virtual ~SMDNode();

	void AddChild ( SMDNode* in_pNode );
	SMDNode* GetChild ( int n ) { return m_pChildren[n]; };

	void	AddKey ( SMDKey* in_pKey );
	SMDKey* GetKey( int n ) { return m_pKeys[n]; };

	char					m_szName[MAX_PATH];
	int						m_iID;

public:

	SMDNode*				m_pParent;
	CSIBCArray<SMDNode*>	m_pChildren;
	CSIBCArray<SMDKey*>		m_pKeys;
	int						m_iParentID;
	XSI::X3DObject			m_x3d;
	double					m_fOldX;
	double					m_fOldY;
	double					m_fOldZ;
	double					m_fOldRX;
	double					m_fOldRY;
	double					m_fOldRZ;
	

};

#endif // !defined(AFX_SMDNODE_H__95D421AF_C21B_4D59_B143_C51502B4E507__INCLUDED_)
