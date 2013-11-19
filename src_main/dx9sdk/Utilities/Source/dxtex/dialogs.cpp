// Dialogs.cpp : implementation file
//

#include "stdafx.h"
#include "dxtex.h"
#include "formats.h"
#include "dialogs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CNewTextureDlg dialog


CNewTextureDlg::CNewTextureDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CNewTextureDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CNewTextureDlg)
    m_iTexType = 0;
    m_dwWidth = 256;
    m_dwHeight = 256;
    m_dwDepth = 8;
    m_iFmt = 0;
    m_strFmtDesc = _T("");
    m_numMips = 1;
    //}}AFX_DATA_INIT

    // Default max texture size of we can't get
    // the info from the device caps.
    m_dwMaxWidth = 1024;
    m_dwMaxHeight = 1024;
}


void CNewTextureDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewTextureDlg)
    DDX_Radio(pDX, IDC_TEXTURE, m_iTexType);
    DDX_Text(pDX, IDC_WIDTH, m_dwWidth);
    DDV_MinMaxInt(pDX, m_dwWidth, 1, m_dwMaxWidth);
    DDX_Text(pDX, IDC_HEIGHT, m_dwHeight);
    DDV_MinMaxInt(pDX, m_dwHeight, 1, m_dwMaxHeight);
    DDX_Text(pDX, IDC_DEPTH, m_dwDepth);
    DDV_MinMaxInt(pDX, m_dwDepth, 2, 1024);
    DDX_CBIndex(pDX, IDC_D3DFMT_COMBO, m_iFmt );
    DDX_Text(pDX, IDC_FMTDESC, m_strFmtDesc);
    DDX_Text(pDX, IDC_MIPCOUNT, m_numMips);
    DDV_MinMaxInt(pDX, m_numMips, 1, 20);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewTextureDlg, CDialog)
    //{{AFX_MSG_MAP(CNewTextureDlg)
    ON_BN_CLICKED(IDC_TEXTURE, OnChangeTextureType)
    ON_BN_CLICKED(IDC_VOLUMETEXTURE, OnChangeTextureType)
    ON_BN_CLICKED(IDC_CUBEMAP, OnChangeTextureType)
    ON_CBN_SELCHANGE(IDC_D3DFMT_COMBO, OnChangeFormat)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewTextureDlg message handlers

BOOL CNewTextureDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_D3DFMT_COMBO);
    TCHAR strFmtLabel[MAX_PATH] = {0}; 
    
    for( int i = 0; i < fmtInfoArraySize; i++ )
    {
        // Construct the texture label
        StringCchPrintf( strFmtLabel, MAX_PATH, TEXT("%s %i-bit: %s"),
                    fmtInfoArray[i].strType,
                    fmtInfoArray[i].nBitsPerTexel,
                    fmtInfoArray[i].strName );

        // Add the label to the combo box
        pComboBox->AddString(strFmtLabel);
        
        // Set default selection to A8R8G8B8
        if( fmtInfoArray[i].fmt == D3DFMT_A8R8G8B8 )
            pComboBox->SetCurSel( i );
    }

    OnChangeTextureType();
    OnChangeFormat();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CNewTextureDlg::OnChangeTextureType() 
{
    UpdateData(TRUE);

    if (m_iTexType == 2)
    {
        // Volume Tex
        GetDlgItem(IDC_VOLUMEDEPTHLABEL)->EnableWindow(TRUE);
        GetDlgItem(IDC_DEPTH)->EnableWindow(TRUE);
    }
    else
    {
        // Normal or Cube Tex
        GetDlgItem(IDC_VOLUMEDEPTHLABEL)->EnableWindow(FALSE);
        GetDlgItem(IDC_DEPTH)->EnableWindow(FALSE);
    }
    UpdateData(FALSE);
    OnChangeFormat();
}



void CNewTextureDlg::OnChangeFormat() 
{
    UpdateData(TRUE);
    m_fmt = fmtInfoArray[m_iFmt].fmt;
    m_strFmtDesc.LoadString(fmtInfoArray[m_iFmt].idsFmtDesc);
    UpdateData(FALSE);
}

void CNewTextureDlg::OnOK() 
{
    UpdateData(TRUE);

    // TODO: Need to do lots of validation of width/height/depth/mipcount here

    if (m_iTexType != 2)
        m_dwDepth = 0;

    m_fmt = fmtInfoArray[m_iFmt].fmt;

    CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CCubeMapDlg dialog


CCubeMapDlg::CCubeMapDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CCubeMapDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCubeMapDlg)
    m_iFace = 0;
    //}}AFX_DATA_INIT
}


void CCubeMapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCubeMapDlg)
    DDX_Radio(pDX, IDC_POSX, m_iFace);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCubeMapDlg, CDialog)
    //{{AFX_MSG_MAP(CCubeMapDlg)
        // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVolumeMapDlg dialog


CVolumeMapDlg::CVolumeMapDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVolumeMapDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CVolumeMapDlg)
    m_powLayers = 0;
    //}}AFX_DATA_INIT
}


void CVolumeMapDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVolumeMapDlg)
    DDX_Radio(pDX, IDC_RADIO2, m_powLayers);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolumeMapDlg, CDialog)
    //{{AFX_MSG_MAP(CVolumeMapDlg)
        // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeMapDlg message handlers


/////////////////////////////////////////////////////////////////////////////
// CChangeFmtDlg dialog


CChangeFmtDlg::CChangeFmtDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CChangeFmtDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CChangeFmtDlg)
    m_iFmt = -1;
    m_strFmtDesc = _T("");
    //}}AFX_DATA_INIT
}


void CChangeFmtDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CChangeFmtDlg)
    DDX_CBIndex(pDX, IDC_D3DFMT_COMBO, m_iFmt );
    DDX_Text(pDX, IDC_FMTDESC, m_strFmtDesc);
    //}}AFX_DATA_MAP

    GetDlgItem( IDOK )->EnableWindow( m_iFmt != m_iFmtInitial );
}


BEGIN_MESSAGE_MAP(CChangeFmtDlg, CDialog)
    //{{AFX_MSG_MAP(CChangeFmtDlg)
    ON_CBN_SELCHANGE(IDC_D3DFMT_COMBO, OnChangeFmt)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeFmtDlg message handlers

BOOL CChangeFmtDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_D3DFMT_COMBO);
    TCHAR strFmtLabel[MAX_PATH] = {0}; 
        
    for( int i = 0; i < fmtInfoArraySize; i++ )
    {
        // Create the texture label
        StringCchPrintf( strFmtLabel, MAX_PATH, TEXT("%s %i-bit: %s"),
                    fmtInfoArray[i].strType,
                    fmtInfoArray[i].nBitsPerTexel,
                    fmtInfoArray[i].strName );

        // Add the label to the combo box
        pComboBox->AddString( strFmtLabel );

        // If the newly-added texture matches the current image format,
        // store this index
        if( m_fmt == fmtInfoArray[i].fmt )
            m_iFmt = i;
    }

    m_iFmtInitial = m_iFmt;

    UpdateFmtDesc();    
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CChangeFmtDlg::OnChangeFmt() 
{
    UpdateData(TRUE);
    UpdateFmtDesc();
}

void CChangeFmtDlg::UpdateFmtDesc()
{
    m_fmt = fmtInfoArray[m_iFmt].fmt;
    m_strFmtDesc.LoadString(fmtInfoArray[m_iFmt].idsFmtDesc);
    UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CResizeDialog dialog


CResizeDialog::CResizeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CResizeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResizeDialog)
	m_newHeight = 0;
	m_newWidth = 0;
	m_oldHeight = _T("");
	m_oldWidth = _T("");
	//}}AFX_DATA_INIT
}


void CResizeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResizeDialog)
	DDX_Text(pDX, IDC_NEWHEIGHT, m_newHeight);
	DDV_MinMaxUInt(pDX, m_newHeight, 1, 4096);
	DDX_Text(pDX, IDC_NEWWIDTH, m_newWidth);
	DDV_MinMaxUInt(pDX, m_newWidth, 1, 4096);
	DDX_Text(pDX, IDC_OLDHEIGHT, m_oldHeight);
	DDX_Text(pDX, IDC_OLDWIDTH, m_oldWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResizeDialog, CDialog)
	//{{AFX_MSG_MAP(CResizeDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
