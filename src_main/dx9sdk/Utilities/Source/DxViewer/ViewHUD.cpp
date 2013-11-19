#include "windows.h"
#pragma warning(disable: 4995)
#include "shlwapi.h"
#pragma warning(default: 4995)

#include "ViewHUD.h"
#include "MainHUD.h"
#include "App.h"
#include "StateBlock.h"

//--------------------------------------------------------------------------------------
// Control IDs
//--------------------------------------------------------------------------------------
#define VIEWDLG_STATIC                  -1
#define VIEWDLG_OK                      1
#define VIEWDLG_CANCEL                  2
#define VIEWDLG_ADJACENCIES             3
#define VIEWDLG_BINORMALS               4
#define VIEWDLG_BOUNDS                  5
#define VIEWDLG_CREASES                 6
#define VIEWDLG_EDGES                   7
#define VIEWDLG_NORMALS                 8
#define VIEWDLG_POINTS                  9
#define VIEWDLG_TANGENTS                10
#define VIEWDLG_STATS                   11
#define VIEWDLG_REFRESH                 12
#define VIEWDLG_FILL                    13
#define VIEWDLG_ENVIRONMENT             14
#define VIEWDLG_ENVIRONMENT_LOAD        15


DECLARE_SINGLETON_CPP( ViewHUD );

void ViewHUD::CreateControls()
{
    HRESULT hr = S_OK;

    RECT rect;
    int buttonHeight = 21;

    HWND hwnd = DXUTGetHWND();
    GetClientRect(hwnd, &rect);

    m_Dialog.SetBackgroundColors( HudManager::BackgroundColor );
    m_Dialog.SetLocation( (int)( rect.right*0.1f ), (int)( rect.bottom*0.1f )  );
    m_Dialog.SetSize( (int)( rect.right*0.8f ), (int)( rect.bottom*0.8f ) );
    m_Dialog.SetCaptionHeight( 31 );
    m_Dialog.SetCaptionText(L"View Options");
    m_Dialog.EnableCaption( true );

    m_Dialog.SetFont( 0, L"Arial", 15, FW_NORMAL );
    m_Dialog.SetFont( 1, L"Arial", 28, FW_BOLD );


    int stdHeight =  __min( 32, (m_Dialog.GetHeight() - m_Dialog.GetCaptionHeight()) / 6 );
    int smallHeight = stdHeight/2;

    int labelLeft = 0;
    int labelWidth = (int)( m_Dialog.GetWidth()*0.2f );

    int ctrl1Left = labelLeft + labelWidth;
    int ctrl1Width = (int)( (m_Dialog.GetWidth()-labelWidth)*0.5f );

    int ctrl2Left = ctrl1Left + ctrl1Width;
    int ctrl2Width = ctrl1Width; 



    m_Dialog.AddStatic( VIEWDLG_STATIC, L"Environment", labelLeft, stdHeight*0, labelWidth, stdHeight );
    m_Dialog.AddEditBox(VIEWDLG_ENVIRONMENT, L"" , ctrl1Left, stdHeight*0, ctrl1Width+ctrl2Width-stdHeight, stdHeight, true); //default control
    m_Dialog.AddButton( VIEWDLG_ENVIRONMENT_LOAD, L"...", ctrl2Left+ctrl2Width-stdHeight, stdHeight*0, stdHeight, stdHeight );


    CDXUTComboBox* pFillBox= NULL;
    m_Dialog.AddStatic( VIEWDLG_STATIC, L"Fill Mode", labelLeft, stdHeight*1, labelWidth, stdHeight );
    m_Dialog.AddComboBox( VIEWDLG_FILL, ctrl1Left, stdHeight*1, ctrl1Width+ctrl2Width, stdHeight, 0, false, &pFillBox );
    if(pFillBox)
    {
        pFillBox->AddItem(L"Auto" ,ULongToPtr(DXVFILLMODE_AUTO));
        pFillBox->AddItem(L"Wireframe" ,ULongToPtr(DXVFILLMODE_WIRE));
        pFillBox->AddItem(L"Solid" ,ULongToPtr(DXVFILLMODE_SOLID));
    }

    int smallStart = stdHeight*3;

    m_Dialog.AddCheckBox( VIEWDLG_STATS, L"Statistics", ctrl1Left, stdHeight*2+(smallHeight/2), ctrl2Width, smallHeight );

    m_Dialog.AddCheckBox( VIEWDLG_ADJACENCIES, L"Adjacency", ctrl1Left, smallStart+smallHeight*0, ctrl1Width, smallHeight);
    m_Dialog.AddCheckBox( VIEWDLG_BINORMALS, L"Binormal", ctrl1Left, smallStart+smallHeight*1, ctrl1Width, smallHeight );
    m_Dialog.AddCheckBox( VIEWDLG_BOUNDS, L"Bounds", ctrl1Left, smallStart+smallHeight*2, ctrl1Width, smallHeight );
    m_Dialog.AddCheckBox( VIEWDLG_CREASES, L"Creases", ctrl1Left, smallStart+smallHeight*3, ctrl1Width, smallHeight );
    
    m_Dialog.AddCheckBox( VIEWDLG_REFRESH, L"Manual Refresh", ctrl2Left, stdHeight*2+(smallHeight/2), ctrl2Width, smallHeight );

    m_Dialog.AddCheckBox( VIEWDLG_EDGES, L"Edges", ctrl2Left, smallStart+smallHeight*0, ctrl2Width, smallHeight );
    m_Dialog.AddCheckBox( VIEWDLG_NORMALS, L"Normals", ctrl2Left, smallStart+smallHeight*1, ctrl2Width, smallHeight );
    m_Dialog.AddCheckBox( VIEWDLG_POINTS, L"Points", ctrl2Left, smallStart+smallHeight*2, ctrl2Width, smallHeight );
    m_Dialog.AddCheckBox( VIEWDLG_TANGENTS, L"Tangents", ctrl2Left, smallStart+smallHeight*3, ctrl2Width, smallHeight );

    m_Dialog.AddButton( VIEWDLG_OK, L"Close", 0, m_Dialog.GetHeight()-m_Dialog.GetCaptionHeight()-stdHeight, m_Dialog.GetWidth(), stdHeight );

}

void ViewHUD::Refresh()
{
    CDXUTDialog::SetRefreshTime( (float) DXUTGetTime() );

    
    CDXUTButton* pButton = NULL;
    CDXUTComboBox* pComboBox= NULL;
    CDXUTEditBox* pEditBox = NULL;
    CDXUTCheckBox* pCheckBox = NULL;

    pEditBox = m_Dialog.GetEditBox(VIEWDLG_ENVIRONMENT);
    if(pEditBox)
    {
        pEditBox->SetText( PathFindFileName(  STATE_GET( Environment ).c_str() ) ); 
        pEditBox->SetEnabled(false);
    }

    pComboBox = m_Dialog.GetComboBox(VIEWDLG_FILL);
    if(pComboBox)
        pComboBox->SetSelectedByData(  ULongToPtr( STATE_GET( FillMode ) ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_STATS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowStatistics ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_ADJACENCIES);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowAdjacencies ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_BINORMALS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowBinormals ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_BOUNDS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowBounds ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_CREASES);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowCreases ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_REFRESH);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ManualRefreshMode ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_EDGES);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowEdges ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_NORMALS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowNormals ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_POINTS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowPoints ) );

    pCheckBox = m_Dialog.GetCheckBox(VIEWDLG_TANGENTS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ShowTangents ) );

}

void ViewHUD::OnEvent( UINT nEvent, int nControlID, CDXUTControl* pControl )
{
    switch( nControlID )
    {
        case VIEWDLG_OK:
            SetActive(false);
            DXVGetMainHUD().SetActive(true);
            break;
        case VIEWDLG_ADJACENCIES:
            STATE_SET( ShowAdjacencies , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break;   
        case VIEWDLG_BINORMALS:
            STATE_SET( ShowBinormals , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break;  
        case VIEWDLG_BOUNDS:
            STATE_SET( ShowBounds , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break; 
        case VIEWDLG_CREASES:
            STATE_SET( ShowCreases , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break;
        case VIEWDLG_EDGES:
            STATE_SET( ShowEdges , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break;
        case VIEWDLG_NORMALS:
            STATE_SET( ShowNormals , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break; 
        case VIEWDLG_POINTS:
            STATE_SET( ShowPoints , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break; 
        case VIEWDLG_TANGENTS:
            STATE_SET( ShowTangents , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break; 
        case VIEWDLG_STATS:
            STATE_SET( ShowStatistics , ((CDXUTCheckBox*)pControl)->GetChecked() );
            break;               
        case VIEWDLG_REFRESH:
            STATE_SET( ManualRefreshMode , ((CDXUTCheckBox*)pControl)->GetChecked() );
            STATE_SET( ManualRefreshNeeded , ((CDXUTCheckBox*)pControl)->GetChecked() );
            DXVGetMainHUD().Refresh();
            break;   
        case VIEWDLG_FILL:
            STATE_SET( FillMode , (DXVFILLMODE) PtrToUlong( ((CDXUTComboBox*)pControl)->GetSelectedData() ) );
            break;        
        case VIEWDLG_ENVIRONMENT_LOAD:
            DXVGetApp().ShowEnvironmentDialog();
            Refresh();
            break;
    }
}
