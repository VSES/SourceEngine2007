//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

// Autorun.h : main header file for the AUTORUN application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <afxmt.h>

#include "resource.h"		// main symbols


#include "..\\netgame\\ODBlendBtn.h"
//#define LAUNCHER_GFX "gfx/autorun/"
#define HL_LAUNCHER_FONT "Arial"


#define STR_BACKGROUND_DIB_NAME "autorun_background"

#ifdef _OEM
	#define REG_HALFLIFE_KEY "SOFTWARE\\Valve\\Half-Life Day One"
	#define REG_AUTORUN_KEY REG_HALFLIFE_KEY "\\Autorun"
#else
	#define REG_HALFLIFE_KEY "SOFTWARE\\Valve\\Half-Life"
	#define REG_AUTORUN_KEY REG_HALFLIFE_KEY "\\Autorun"
#endif


#define REG_INSTALLED_VALUE "Installed"
#define REG_INSTALLPATH_VALUE "InstallPath"

#define REG_DIRECTX_KEY "SOFTWARE\\Microsoft\\DirectX"
#define REG_DX_VERSION_VALUE "Version"

#define CURR_DX_VERSION_MAJOR 4
#define CURR_DX_VERSION_MINOR 6
#define CURR_DX_VERSION_REV   0
#define CURR_DX_VERSION_BUILD 318

/////////////////////////////////////////////////////////////////////////////
// CAutorunApp:
// See Autorun.cpp for the implementation of this class
//
class CAutorunApp : public CWinApp
{
public:
	~CAutorunApp();
	CAutorunApp();

	
	void NewMerge(CRect& r, CDC *pSrcDC, CDC *pDestDC, CBitmap *pSrcBitmap, CBitmap *pDestBitmap,
		int nPermutation  = 1,
		int nAdjust = 0);
	
	void CopyFromBackground(CDC *pDC, CRect& rcDC, CRect& rcArea);
	void DrawBackground(CDC *pDC, CRect& r, CRect *srcRect = NULL);

	char szInstallPath[256];
	void CheckRegistry();
	const bool bButtonsLoaded;
// Data
public:
	int ExitInstance();
	bool bSetupExeExists;
	void CheckDirectories();
	DWORD Platform;
	bool bNTServicePack3;
	DWORD OSVersion;
	bool bHLInstalled;

	HDIB hDib;

	CString strGameName;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutorunApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAutorunApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	
private:
	CMutex* m_pMutex;
	void CheckOS();
};

extern CAutorunApp *g_pApp;
/////////////////////////////////////////////////////////////////////////////

