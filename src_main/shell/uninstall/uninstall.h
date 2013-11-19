//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// UnInst.h : main header file for the UNINST DLL
//

#if !defined(AFX_UNINST_H__BC4C6C68_3963_11D2_BECF_006097078A65__INCLUDED_)
#define AFX_UNINST_H__BC4C6C68_3963_11D2_BECF_006097078A65__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUninstallApp
// See UnInst.cpp for the implementation of this class
//

class CUninstallApp : public CWinApp
{
public:
	void DoPostUninstallStuff();
	void DelValveKeysIfEmpty();	//deletes from both HKEY_LOCAL_MACHINE and HKEY_CURRENT_USER
	void DeleteSelf();
	void DelURLValue();

	CUninstallApp();	

	void DelHLKeyContentsInCurrentUser();
	void DelHLKeyContentsInLocalMachine();
	void RecursiveRegDelKey(HKEY);
	CString m_szSimpleGameName;
	CString m_szGameName;
	void DoPreUninstallStuff();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUninstallApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CUninstallApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

extern CUninstallApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNINST_H__BC4C6C68_3963_11D2_BECF_006097078A65__INCLUDED_)
