#include "MainHUD.h"
#include "ViewHUD.h"
#include "EffectHUD.h"
#include "App.h"
#include "StateBlock.h"

//--------------------------------------------------------------------------------------
// Control IDs
//--------------------------------------------------------------------------------------
#define MAINDLG_STATIC                  -1
//#define MAINDLG_OK                      1
//#define MAINDLG_CANCEL                  2
#define MAINDLG_VIEW                    3
#define MAINDLG_EFFECTS                 4
#define MAINDLG_HELP                    5
#define MAINDLG_REFRESH                 6
#define MAINDLG_OPEN                    7
#define MAINDLG_DXCHANGE                8


DECLARE_SINGLETON_CPP( MainHUD );

void MainHUD::CreateControls()
{
    RECT rect;
    int buttonHeight = 21;

    HWND hwnd = DXUTGetHWND();
    GetClientRect(hwnd, &rect);

    m_Dialog.SetLocation( 0, rect.bottom - buttonHeight );
    m_Dialog.SetSize( rect.right, buttonHeight );

    //m_Dialog.SetBackgroundColors( HudManager::BackgroundColor );
    //m_Dialog.SetLocation( (int)( rect.right*0.1f ), (int)( rect.bottom*0.1f )  );
    //m_Dialog.SetSize( (int)( rect.right*0.8f ), (int)( rect.bottom*0.8f ) );
    //m_Dialog.SetCaptionHeight( 31 );
    //m_Dialog.SetCaptionText(L"Test");
    //m_Dialog.EnableCaption( true );


    //bottom row
    int buttonTop = 0;//m_Dialog.GetHeight()-buttonHeight;

    int iButton = 0;
    int buttonWidth = m_Dialog.GetWidth()/6;
    m_Dialog.AddButton( MAINDLG_OPEN, L"Open", buttonWidth*iButton++, buttonTop, buttonWidth, buttonHeight, 0, true);//default control
    m_Dialog.AddButton( MAINDLG_VIEW, L"View", buttonWidth*iButton++, buttonTop, buttonWidth, buttonHeight );
    m_Dialog.AddButton( MAINDLG_EFFECTS, L"Effect Options", buttonWidth*iButton++, buttonTop, buttonWidth, buttonHeight );
    m_Dialog.AddButton( MAINDLG_REFRESH, L"Refresh", buttonWidth*iButton++,buttonTop, buttonWidth, buttonHeight );
    m_Dialog.AddButton( MAINDLG_DXCHANGE, L"", buttonWidth*iButton++,buttonTop, buttonWidth, buttonHeight );
    m_Dialog.AddButton( MAINDLG_HELP, L"Help", buttonWidth*iButton++, buttonTop, buttonWidth, buttonHeight );


}

void MainHUD::Refresh()
{
    CDXUTDialog::SetRefreshTime( (float) DXUTGetTime() );

    CDXUTButton* pButton = m_Dialog.GetButton(MAINDLG_OPEN);
    if(pButton)
    {
        if(STATE_GET(MergeMode) )
            pButton->SetText( L"Merge" );
        else
            pButton->SetText( L"Open" );
    }

    pButton = m_Dialog.GetButton(MAINDLG_REFRESH);
    if(pButton)
        pButton->SetEnabled( STATE_GET(ManualRefreshMode) );

    pButton = m_Dialog.GetButton(MAINDLG_DXCHANGE);
    if(pButton)
    {
        pButton->SetEnabled( STATE_GET( AllowD3D10 ) );

        if( STATE_GET(Version) == DXUT_D3D10_DEVICE )
            pButton->SetText( L"Direct3D9 Mode" );
        else if( STATE_GET(Version) == DXUT_D3D9_DEVICE )
            pButton->SetText( L"Direct3D10 Mode" );
    }

    
}

void MainHUD::OnEvent( UINT nEvent, int nControlID, CDXUTControl* pControl )
{
    switch( nControlID )
    {
        case MAINDLG_OPEN:   
            {
                DXVGetApp().ShowDdsOrFxOrXDialog(true, true, false); 
                Refresh();
            }
            break;
        case MAINDLG_REFRESH:                       
            STATE_SET( ManualRefreshNeeded, true );
            break;
        case MAINDLG_VIEW:                        
            SetActive( false );  
            DXVGetViewHUD().SetActive( true );  
            break;
        case MAINDLG_EFFECTS:                       
            SetActive( false );  
            DXVGetEffectHUD().SetActive( true );  
            break;
        case MAINDLG_DXCHANGE:  
            STATE_SET(Reset, DXVRESET_SWITCHDEVICE);
            break;
        case MAINDLG_HELP:                          
            DXVGetApp().Help(); 
            break;
    };
}



