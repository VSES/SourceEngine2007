//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// AutorunDlg.cpp : implementation file//

#include "stdafx.h"
#include "..\\netgame\\dibapi.h"
#include "Autorun.h"
#include "AutorunDlg.h"

//#include "..\\LocalResource\\resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE		   
static char THIS_FILE[] = __FILE__;
#endif

#define STR_BUTTONS_DIB_NAME "autorun_buttons"
#define MENU_TL_X 265
#define MENU_TL_Y 136
#define AVAILABLE_SPACE_FOR_BUTTONS_Y 226

#define NUM_BUTTONS_IN_BMP 7

#define NUM_TOTAL_BUTTONS 6


#define PLAY_BTN_INDEX 0
#define INSTALL_BTN_INDEX 1
#define REINSTALL_BTN_INDEX 2
#define WEB_BTN_INDEX 3
#define ORDER_BTN_INDEX 4
#define README_BTN_INDEX 5
#define EXIT_BTN_INDEX 6

/////////////////////////////////////////////////////////////////////////////
// CAutorunDlg dialog

CAutorunDlg::CAutorunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutorunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutorunDlg)
		// NOTE: the ClassWizard will add member initialization here

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bQuitting=false;

	char szFileName[256];

//	m_hLocalRes=NULL;	//initialized to NULL

	sprintf(szFileName, "%s.bmp",STR_BUTTONS_DIB_NAME);
	
	m_hButtons = ReadDIBFile(szFileName);
	if (m_hButtons)
	{

		LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
		lpDIBHdr  = (LPSTR) ::GlobalLock((HGLOBAL) m_hButtons);

		int sW, sH;

		sW = DIBWidth(lpDIBHdr);
		sH = DIBHeight(lpDIBHdr);


		::GlobalUnlock((HGLOBAL) m_hButtons);

		m_szButtonSize = CSize(sW,sH/(NUM_BUTTONS_IN_BMP*3));

		m_nButtons = sH / m_szButtonSize.cy;
		m_nButtons /= 3; // Three images per button



		if (sH % m_szButtonSize.cy)
		{
			AfxMessageBox("Button Graphic Size Mismatch");
		}
	}

	// Set button indices into the button bitmap image.
	if (m_hButtons)
	{
		m_btnPlay.SetDIBData(m_szButtonSize, PLAY_BTN_INDEX, m_hButtons);
		m_btnInstall.SetDIBData(m_szButtonSize, INSTALL_BTN_INDEX, m_hButtons);
		m_btnReinstall.SetDIBData(m_szButtonSize, REINSTALL_BTN_INDEX, m_hButtons);
		m_btnWeb.SetDIBData(m_szButtonSize, WEB_BTN_INDEX, m_hButtons);
		m_btnOrder.SetDIBData(m_szButtonSize, ORDER_BTN_INDEX, m_hButtons);
		m_btnReadme.SetDIBData(m_szButtonSize, README_BTN_INDEX, m_hButtons);
		m_btnExit.SetDIBData(m_szButtonSize,EXIT_BTN_INDEX, m_hButtons);
	}	
}

CAutorunDlg::~CAutorunDlg()
{
//	if (m_hLocalRes)
		//FreeLibrary(m_hLocalRes);
}

void CAutorunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutorunDlg)
	DDX_Control(pDX, IDC_REINSTALL, m_btnReinstall);
	DDX_Control(pDX, IDC_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_WEB, m_btnWeb);
	DDX_Control(pDX, IDC_README, m_btnReadme);
	DDX_Control(pDX, IDC_PLAY, m_btnPlay);
	DDX_Control(pDX, IDC_ORDER, m_btnOrder);
	DDX_Control(pDX, IDC_INSTALL, m_btnInstall);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAutorunDlg, CDialog)
	//{{AFX_MSG_MAP(CAutorunDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSTALL, OnInstall)
	ON_BN_CLICKED(IDC_WEB, OnWeb)
	ON_BN_CLICKED(IDC_README, OnReadme)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_ORDER, OnOrder)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_REINSTALL, OnInstall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutorunDlg message handlers

BOOL CAutorunDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	InitLocalResource();
	InitButtonFallBackText();

	//determine if we're going to show the DirectX button here


	if (!RepositionButtons())
		return FALSE;
	
	SetWindowText(g_pApp->strGameName);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutorunDlg::OnPaint() 
{
	if (m_bQuitting)
		return;

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();

		if (g_pApp)
		{
			CPaintDC dc(this);
			CRect r;
			GetClientRect(r);
			g_pApp->DrawBackground(&dc, r, &r);
		}
		//PaintDIB(dc.GetSafeHdc(), rcFrame, hDib, rcDib, NULL);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutorunDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CAutorunDlg::OnWeb() 
{
	CString WebSite;
	WebSite.LoadString(IDS_AUTORUN_URL_HL);

	ShellExecute (
		::GetFocus (),
		"open",
		WebSite,
        NULL,
		NULL,
		0 );	
}

void CAutorunDlg::OnReadme() 
{
	CString ReadmeTxt;
	ReadmeTxt.LoadString(IDS_AUTORUN_README_TXT_NAME);
	ShellExecute (
		::GetFocus(),
		"open",
		ReadmeTxt,
        NULL,
		NULL,
		SW_SHOWNORMAL );	
}

void CAutorunDlg::OnInstall() 
{
	
	CAutorunApp *pApp = (CAutorunApp *)AfxGetApp();
	if (!pApp)
		return;

	CString SetupExe;
	SetupExe.LoadString(IDS_AUTORUN_SETUP_EXE_NAME);

	ShellExecute (::GetFocus (),"open",SetupExe,NULL,NULL,0 );
	OnOK();
}


void CAutorunDlg::OnPlay() 
{
	CString GameExePath(g_pApp->szInstallPath);
	CString GameExeName;
	GameExeName.LoadString(IDS_AUTORUN_GAME_EXE_NAME);
	
	ShellExecute (::GetFocus (),"open",GameExePath+"\\"+GameExeName,NULL,g_pApp->szInstallPath,SW_SHOWNORMAL );
	OnOK();
}

void CAutorunDlg::OnOrder() 
{
	CString OrderUrl;

	OrderUrl.LoadString(IDS_AUTORUN_URL_ORDER);
	ShellExecute (
		::GetFocus (),
		"open",
		OrderUrl,
        NULL,
		NULL,
		0 );		
}

void CAutorunDlg::InitLocalResource()
{
	//may use this later...

	//m_hLocalRes=LoadLibrary("LocalResource.dll");
	
}

bool CAutorunDlg::RepositionButtons()
{
	CPoint ptTopLeft;
	CRect rcButton;
	CRect rcClient;
	CRect rcDIB;

	char *lpDib;
	int w, h;

	lpDib = (char *)::GlobalLock((HGLOBAL) g_pApp->hDib);
	if (!lpDib)
	{
		AfxMessageBox("Can't find background");
		return false;
	}
	w = DIBWidth( lpDib );
	h = DIBHeight( lpDib );
	::GlobalUnlock((HGLOBAL) g_pApp->hDib);

	MoveWindow(0,0,w,h, TRUE);

	GetClientRect(&rcClient);

	int buttonH = 30;
	int buttonStep = buttonH + 2;
	int buttonW;

	ptTopLeft.x = MENU_TL_X;
	ptTopLeft.y = MENU_TL_Y;
	
	buttonW = m_szButtonSize.cx;
	
	int nButtons;

	if (m_nButtons != NUM_BUTTONS_IN_BMP)
	{
		char temp[200];
		sprintf(temp,"Error, unexpected number of buttons loaded from button bitmap (expected %li, loaded %li)\n",NUM_BUTTONS_IN_BMP,m_nButtons);
		AfxMessageBox(temp);
		return false;
	}

	nButtons=NUM_TOTAL_BUTTONS;
	if (!PlayButtonVisible())
		nButtons--;
	
	if (!OrderButtonVisible())
		nButtons--;
		
	int PanelSizeY = nButtons*buttonStep;
	int AvailSizeY = AVAILABLE_SPACE_FOR_BUTTONS_Y;

	int YHeader=(AvailSizeY-PanelSizeY)/2;

	ptTopLeft.y+=YHeader;
	
	//Reinstall and Install buttons are mutually exclusive, but
	//the ButtonVisible functions take care of that. (a Reinstall ButtonVisible 
	//will never return true if the corresponding Install ButtonVisible returns
	//true)
	
	//position play button
	if (PlayButtonVisible())
	{
		
		rcButton = CRect(
			ptTopLeft.x, ptTopLeft.y,
			ptTopLeft.x + buttonW, ptTopLeft.y + buttonH
		);
		ptTopLeft.y += buttonStep;
		m_btnPlay.MoveWindow(rcButton, TRUE);
		m_btnPlay.ShowWindow(SW_SHOW);		
	}
	else
		m_btnPlay.ShowWindow(SW_HIDE);
	
	//position install button
	if (InstallButtonVisible())
	{
		rcButton = CRect(
			ptTopLeft.x, ptTopLeft.y,
			ptTopLeft.x + buttonW, ptTopLeft.y + buttonH
			);
		ptTopLeft.y += buttonStep;
		m_btnInstall.MoveWindow(rcButton, TRUE);
		m_btnInstall.ShowWindow(SW_SHOW);
	}
	else
		m_btnInstall.ShowWindow(SW_HIDE);
	
	//position reinstall button
	if (ReinstallButtonVisible())
	{
		rcButton = CRect(
			ptTopLeft.x, ptTopLeft.y,
			ptTopLeft.x + buttonW, ptTopLeft.y + buttonH
		);
		ptTopLeft.y += buttonStep;
		m_btnReinstall.MoveWindow(rcButton, TRUE);
		m_btnReinstall.ShowWindow(SW_SHOW);
	}
	else
		m_btnReinstall.ShowWindow(SW_HIDE);

	//position Web button
	rcButton = CRect(
		ptTopLeft.x, ptTopLeft.y,
		ptTopLeft.x + buttonW, ptTopLeft.y + buttonH
	);
	ptTopLeft.y += buttonStep;
	m_btnWeb.MoveWindow(rcButton, TRUE);

	//position Order button
	if (OrderButtonVisible())
	{
		rcButton = CRect(
			ptTopLeft.x, ptTopLeft.y,
			ptTopLeft.x + buttonW, ptTopLeft.y + buttonH
		);
		ptTopLeft.y += buttonStep;
		m_btnOrder.MoveWindow(rcButton, TRUE);
		m_btnOrder.ShowWindow(SW_SHOW);
	}
	else
		m_btnOrder.ShowWindow(SW_HIDE);

	//position readme button
	rcButton = CRect(
		ptTopLeft.x, ptTopLeft.y,
		ptTopLeft.x + buttonW, ptTopLeft.y + buttonH
	);
	ptTopLeft.y += buttonStep;
	m_btnReadme.MoveWindow(rcButton, TRUE);
	
	
	//position exit button
	rcButton = CRect(
		ptTopLeft.x, ptTopLeft.y,
		ptTopLeft.x + buttonW, ptTopLeft.y + buttonH
	);
	ptTopLeft.y += buttonStep;
	m_btnExit.MoveWindow(rcButton, TRUE);
	
	return true;	
	
}

void CAutorunDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	OnOK();

}

BOOL CAutorunDlg::OnEraseBkgnd(CDC* pDC) 
{
	CPaintDC paintDc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	BOOL bResult;
	CDC dc;
	bResult = dc.CreateCompatibleDC(&paintDc);
	if (!bResult)
		return FALSE;

	CBitmap bm;
	int w, h;
	w = rcClient.Width();
	h = rcClient.Height();
	bm.CreateCompatibleBitmap(&paintDc, w, h);
	CBitmap *oldBm;
	oldBm = dc.SelectObject(&bm);

	g_pApp->DrawBackground(&dc, rcClient, &rcClient);

	paintDc.BitBlt(rcClient.left,rcClient.top,
		rcClient.Width(),rcClient.Height(),
		&dc,0,0,SRCCOPY);

	dc.SelectObject(oldBm);
	dc.DeleteDC();

	return TRUE;
}


void CAutorunDlg::InitButtonFallBackText()
{
	CString strTemp;

	strTemp.LoadString(IDS_AUTORUN_PLAY_HL);
	m_btnPlay.SetWindowText(strTemp);

	strTemp.LoadString(IDS_AUTORUN_INSTALL_HL);
	m_btnInstall.SetWindowText(strTemp);
	
	strTemp.LoadString(IDS_AUTORUN_REINSTALL_HL);
	m_btnReinstall.SetWindowText(strTemp);

	strTemp.LoadString(IDS_AUTORUN_HL_WEB_SITE);
	m_btnWeb.SetWindowText(strTemp);

	strTemp.LoadString(IDS_AUTORUN_ORDER_HL);
	m_btnOrder.SetWindowText(strTemp);
	
	strTemp.LoadString(IDS_AUTORUN_VIEW_README);
	m_btnReadme.SetWindowText(strTemp);
		
	strTemp.LoadString(IDS_AUTORUN_EXIT);
	m_btnExit.SetWindowText(strTemp);

}



//don't bother making these inline because they don't get called that often.
//Autorun isn't exactly a target for speed optimizing anyway...

bool CAutorunDlg::PlayButtonVisible()
{
	return g_pApp->bHLInstalled;
}

bool CAutorunDlg::ReinstallButtonVisible()
{
	return g_pApp->bHLInstalled && g_pApp->bSetupExeExists;
}

bool CAutorunDlg::InstallButtonVisible()
{
	return !g_pApp->bHLInstalled && g_pApp->bSetupExeExists;
}

bool CAutorunDlg::OrderButtonVisible()
{
#ifdef _OEM
	return true;
#else
	return false;
#endif
}

void CAutorunDlg::OnOK()
{
	m_bQuitting=true;
	CDialog::OnOK();
}
