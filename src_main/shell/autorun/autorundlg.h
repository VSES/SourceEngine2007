//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// AutorunDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutorunDlg dialog

class CAutorunDlg : public CDialog
{
// Construction
public:
	~CAutorunDlg();
	CAutorunDlg(CWnd* pParent = NULL);	// standard constructor
	
private:
	CRect rcDib;
	HDIB m_hButtons;       // HDIB holding all of the buttons
	int m_nButtons;        // # of buttons
	CSize m_szButtonSize;  // Size of each button in HDIB
	bool m_bQuitting;	
public:
	
// Dialog Data
	//{{AFX_DATA(CAutorunDlg)
	enum { IDD = IDD_AUTORUN_DIALOG };
	CODBlendBtn	m_btnDirectX;
	CODBlendBtn	m_btnReinstall;
	CODBlendBtn	m_btnReinstDX;
	CODBlendBtn	m_btnExit;
	CODBlendBtn	m_btnWeb;
	CODBlendBtn	m_btnReadme;
	CODBlendBtn	m_btnPlay;
	CODBlendBtn	m_btnOrder;
	CODBlendBtn	m_btnInstall;
	CStatic	m_holdFrame;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutorunDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnOK();
	bool OrderButtonVisible();
	bool InstallButtonVisible();
	bool ReinstallButtonVisible();
	bool PlayButtonVisible();
	bool InstallDXButtonVisible();
	bool ReinstallDXButtonVisible();
	void InitButtonFallBackText();
	bool RepositionButtons();
	void InitLocalResource();
	
	
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAutorunDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInstall();
	afx_msg void OnWeb();
	afx_msg void OnReadme();
	afx_msg void OnPlay();
	afx_msg void OnOrder();
	afx_msg void OnExit();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDirectX();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

