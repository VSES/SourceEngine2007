// Dialogs.h : header file
//
#if !defined(AFX_DIALOGS_H__14A2C924_FB41_4BB7_92E4_DBA7CAF1FA06__INCLUDED_)
#define AFX_DIALOGS_H__14A2C924_FB41_4BB7_92E4_DBA7CAF1FA06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNewTextureDlg dialog

class CNewTextureDlg : public CDialog
{
// Construction
public:
    CNewTextureDlg(CWnd* pParent = NULL);   // standard constructor

    void SetMaxTextureSize( int dwWidth, int dwHeight ) { m_dwMaxWidth = dwWidth; m_dwMaxHeight = dwHeight; }

// Dialog Data
    //{{AFX_DATA(CNewTextureDlg)
    enum { IDD = IDD_NEWTEXTURE };
    int     m_iTexType;
    int     m_dwWidth;
    int     m_dwHeight;
    int     m_dwDepth;
    int     m_iFmt;
    CString m_strFmtDesc;
    int     m_numMips;
    D3DFORMAT m_fmt;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNewTextureDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CNewTextureDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnChangeTextureType();
    afx_msg void OnChangeFormat();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    int m_dwMaxWidth;
    int m_dwMaxHeight;
};


/////////////////////////////////////////////////////////////////////////////
// CCubeMapDlg dialog

class CCubeMapDlg : public CDialog
{
// Construction
public:
    CCubeMapDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CCubeMapDlg)
    enum { IDD = IDD_CUBEMAP };
    INT     m_iFace;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCubeMapDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CCubeMapDlg)
        // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CVolumeMapDlg dialog

class CVolumeMapDlg : public CDialog
{
// Construction
public:
    CVolumeMapDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CVolumeMapDlg)
    enum { IDD = IDD_VOLUMEMAP };
    int     m_powLayers;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CVolumeMapDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CVolumeMapDlg)
        // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CChangeFmtDlg dialog

class CChangeFmtDlg : public CDialog
{
// Construction
public:
    CChangeFmtDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CChangeFmtDlg)
    enum { IDD = IDD_CHANGEFORMAT };
    int     m_iFmt;
    CString m_strFmtDesc;
    //}}AFX_DATA
    BOOL m_bVolume;
    D3DFORMAT m_fmt;
    int     m_iFmtInitial;

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChangeFmtDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CChangeFmtDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnChangeFmt();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void UpdateFmtDesc();
};

/////////////////////////////////////////////////////////////////////////////
// CResizeDialog dialog

class CResizeDialog : public CDialog
{
// Construction
public:
	CResizeDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResizeDialog)
	enum { IDD = IDD_RESIZE };
	UINT	m_newHeight;
	UINT	m_newWidth;
	CString	m_oldHeight;
	CString	m_oldWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResizeDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGS_H__14A2C924_FB41_4BB7_92E4_DBA7CAF1FA06__INCLUDED_)
