#include "windows.h"
#include <malloc.h>

#include "EffectHUD.h"
#include "MainHUD.h"
#include "App.h"
#include "StateBlock.h"
#include "Render.h"
//--------------------------------------------------------------------------------------
// Control IDs
//--------------------------------------------------------------------------------------
#define EFFECTSDLG_STATIC                  -1
#define EFFECTSDLG_OK                      1
#define EFFECTSDLG_CANCEL                  2
#define EFFECTSDLG_TECHNIQUE               3
#define EFFECTSDLG_PASSMODE                4
#define EFFECTSDLG_PASS                 5
#define EFFECTSDLG_DEFINES                 6
#define EFFECTSDLG_DEBUGPS                 7
#define EFFECTSDLG_DEBUGVS                 8
#define EFFECTSDLG_OPTIMIZE                9
#define EFFECTSDLG_VALIDATE                10
#define EFFECTSDLG_PRESHADE                11
#define EFFECTSDLG_PARTIALS                12
#define EFFECTSDLG_DYNAFLOWGOOD            13
#define EFFECTSDLG_DYNAFLOWBAD             14
#define EFFECTSDLG_INCLUDES                15
#define EFFECTSDLG_TECHNIQUEMODE           16
#define EFFECTSDLG_DEFINES_STATICTEXT      17
#define EFFECTSDLG_DYNAFLOW_GROUP          1

DECLARE_SINGLETON_CPP( EffectHUD );

void EffectHUD::BuildShaderDefines(std::wstring& output)
{
    DefineList Defines = STATE_GET( Defines );

    std::string builder;

    int i = 0;
    for (DefineList::const_iterator itr = Defines.begin(); itr != Defines.end(); itr++, i++)
    {
        if (i)
        {
            builder += ";";
        }

        builder += itr->name;

        if (itr->value.length() > 0)
        {
            builder += "=";
            builder += itr->value;
        }
    }

    WCHAR* tmp = (WCHAR*) alloca( sizeof(WCHAR) * (builder.length() + 1) );
    StringCchPrintf( tmp, (builder.length() + 1), L"%S", builder.c_str() );
    output = tmp;
}

void EffectHUD::CreateControls()
{
    HRESULT hr = S_OK;

    RECT rect;
    int buttonHeight = 21;

    HWND hwnd = DXUTGetHWND();
    GetClientRect(hwnd, &rect);

    m_Dialog.SetBackgroundColors( HudManager::BackgroundColor );
    m_Dialog.SetLocation( (int)( rect.right*0.1f ), (int)( rect.bottom*0.1f )  );
    m_Dialog.SetSize( (int)( rect.right*0.85f ), (int)( rect.bottom*0.85f ) );
    m_Dialog.SetCaptionHeight( 31 );
    m_Dialog.SetCaptionText(L"Effect Options");
    m_Dialog.EnableCaption( true );

    m_Dialog.SetFont( 0, L"Arial", 15, FW_NORMAL );
    m_Dialog.SetFont( 1, L"Arial", 28, FW_BOLD );



    int stdHeight =  __min( 32, (m_Dialog.GetHeight()-m_Dialog.GetCaptionHeight()) / 8 );
    int smallHeight = stdHeight/2;

    int labelLeft = 0;
    int labelWidth = (int)( m_Dialog.GetWidth()*0.2f );

    int ctrl1Left = labelLeft + labelWidth;
    int ctrl1Width = (int)( (m_Dialog.GetWidth()-labelWidth)*0.5f );

    int ctrl2Left = ctrl1Left + ctrl1Width;
    int ctrl2Width = ctrl1Width; 

    int verticalIndex = 0;

    CDXUTComboBox* pTechModeBox= NULL;
    m_Dialog.AddStatic( EFFECTSDLG_STATIC, L"Technique Mode", labelLeft, stdHeight*verticalIndex, labelWidth, stdHeight );
    m_Dialog.AddComboBox( EFFECTSDLG_TECHNIQUEMODE, ctrl1Left, stdHeight*verticalIndex, ctrl1Width+ctrl2Width, stdHeight, 0, true, &pTechModeBox );//default control
    if(pTechModeBox)
    {
        pTechModeBox->AddItem(L"First Valid" , ULongToPtr(DXVTECHNIQUEMODE_FIRSTVALID));
        pTechModeBox->AddItem(L"Selected" , ULongToPtr(DXVTECHNIQUEMODE_SELECTED));
    }
    verticalIndex++;

    m_Dialog.AddStatic( EFFECTSDLG_STATIC, L"Technique", labelLeft, stdHeight*verticalIndex, labelWidth, stdHeight );
    m_Dialog.AddComboBox( EFFECTSDLG_TECHNIQUE, ctrl1Left, stdHeight*verticalIndex, ctrl1Width+ctrl2Width, stdHeight, 0, false );
    verticalIndex++;

    CDXUTComboBox* pPassModeBox= NULL;
    m_Dialog.AddStatic( EFFECTSDLG_STATIC, L"Pass Mode", labelLeft, stdHeight*verticalIndex, labelWidth, stdHeight );
    m_Dialog.AddComboBox( EFFECTSDLG_PASSMODE, ctrl1Left, stdHeight*verticalIndex, ctrl1Width+ctrl2Width, stdHeight, 0, false, &pPassModeBox );
    if(pPassModeBox)
    {
        pPassModeBox->AddItem(L"All" ,ULongToPtr(0));
        pPassModeBox->AddItem(L"Up To" ,ULongToPtr(1));
    }
    verticalIndex++;

    m_Dialog.AddStatic( EFFECTSDLG_STATIC, L"Pass Index", labelLeft, stdHeight*verticalIndex, labelWidth, stdHeight );
    m_Dialog.AddComboBox( EFFECTSDLG_PASS, ctrl1Left, stdHeight*verticalIndex, ctrl1Width+ctrl2Width, stdHeight, 0, false, NULL );
    verticalIndex++;

     m_Dialog.AddStatic( EFFECTSDLG_DEFINES_STATICTEXT, L"Defines", labelLeft, stdHeight*verticalIndex, labelWidth, stdHeight );
    m_Dialog.AddEditBox( EFFECTSDLG_DEFINES, L"", ctrl1Left, stdHeight*verticalIndex, ctrl1Width+ctrl2Width-stdHeight, stdHeight);
    verticalIndex++;
    
    m_Dialog.AddStatic( EFFECTSDLG_STATIC, L"Includes", labelLeft, stdHeight*verticalIndex, labelWidth, stdHeight );
    m_Dialog.AddEditBox( EFFECTSDLG_INCLUDES, L"", ctrl1Left, stdHeight*verticalIndex, ctrl1Width+ctrl2Width-stdHeight, stdHeight);
    verticalIndex++;

    int smallStart = stdHeight*verticalIndex;
    verticalIndex = 0;

    m_Dialog.AddCheckBox( EFFECTSDLG_DEBUGPS, L"Debug Pixel Shaders", ctrl1Left, smallStart+smallHeight*verticalIndex, ctrl1Width, smallHeight);
    m_Dialog.AddCheckBox( EFFECTSDLG_PRESHADE, L"Allow Preshaders", ctrl2Left, smallStart+smallHeight*verticalIndex, ctrl2Width, smallHeight );
    
    verticalIndex++;
    
    m_Dialog.AddCheckBox( EFFECTSDLG_DEBUGVS, L"Debug Vertex Shaders", ctrl1Left, smallStart+smallHeight*verticalIndex, ctrl1Width, smallHeight);
    m_Dialog.AddCheckBox( EFFECTSDLG_PARTIALS, L"Allow Partial Precision", ctrl2Left, smallStart+smallHeight*verticalIndex, ctrl2Width, smallHeight );

    verticalIndex++;
    
    m_Dialog.AddCheckBox( EFFECTSDLG_OPTIMIZE, L"Optimize Shaders", ctrl1Left, smallStart+smallHeight*verticalIndex, ctrl1Width, smallHeight );
    m_Dialog.AddRadioButton( EFFECTSDLG_DYNAFLOWGOOD, EFFECTSDLG_DYNAFLOW_GROUP, L"Prefer Dynamic Flow", ctrl2Left, smallStart+smallHeight*verticalIndex, ctrl2Width, smallHeight, false, 0, false);

    verticalIndex++;

     m_Dialog.AddCheckBox( EFFECTSDLG_VALIDATE, L"Validate Shaders", ctrl1Left, smallStart+smallHeight*verticalIndex, ctrl1Width, smallHeight );   
    m_Dialog.AddRadioButton( EFFECTSDLG_DYNAFLOWBAD, EFFECTSDLG_DYNAFLOW_GROUP, L"Avoid Dynamic Flow", ctrl2Left, smallStart+smallHeight*verticalIndex, ctrl2Width, smallHeight, false, 0, false);
    
    m_Dialog.AddButton( EFFECTSDLG_OK, L"Close", 0, m_Dialog.GetHeight()-m_Dialog.GetCaptionHeight()-stdHeight, m_Dialog.GetWidth(), stdHeight );
}

void EffectHUD::Refresh()
{
    CDXUTDialog::SetRefreshTime( (float) DXUTGetTime() );

    CDXUTStatic* pStatic= NULL;
    CDXUTComboBox* pComboBox= NULL;
    CDXUTCheckBox* pCheckBox = NULL;
    CDXUTEditBox* pEditBox = NULL;
    CDXUTRadioButton* pRadio = NULL;

    if( STATE_GET( FileFX ).empty() )
    {
        m_Dialog.SetControlEnabled(EFFECTSDLG_TECHNIQUEMODE, false);
        m_Dialog.SetControlEnabled(EFFECTSDLG_TECHNIQUE, false);
        m_Dialog.SetControlEnabled(EFFECTSDLG_PASSMODE, false);
        m_Dialog.SetControlEnabled(EFFECTSDLG_PASS, false);

        pEditBox = m_Dialog.GetEditBox(EFFECTSDLG_DEFINES);
        if(pEditBox) 
        {
            pEditBox->SetEnabled( false );
        }

        pEditBox = m_Dialog.GetEditBox(EFFECTSDLG_INCLUDES);
        if(pEditBox) 
        {
            pEditBox->SetEnabled( false );
        }
    }
    else
    {

        pComboBox = m_Dialog.GetComboBox(EFFECTSDLG_TECHNIQUEMODE);
        if(pComboBox)
        {
            pComboBox->SetEnabled( true );
            pComboBox->SetSelectedByData(  ULongToPtr( STATE_GET( TechniqueMode ) ) );

            bool enabled = ( STATE_GET( TechniqueMode ) == DXVTECHNIQUEMODE_SELECTED );
            if(!enabled)
            {
                pComboBox = m_Dialog.GetComboBox(EFFECTSDLG_TECHNIQUE);
                if (pComboBox)
                    pComboBox->RemoveAllItems();

                pComboBox = m_Dialog.GetComboBox(EFFECTSDLG_PASSMODE);
                if (pComboBox)
                    pComboBox->SetSelectedByData(  ULongToPtr( STATE_GET( PassMode ) ) );
                
                pComboBox = m_Dialog.GetComboBox(EFFECTSDLG_PASS);
                if (pComboBox)
                    pComboBox->RemoveAllItems();

                m_Dialog.SetControlEnabled(EFFECTSDLG_TECHNIQUE, false);
                m_Dialog.SetControlEnabled(EFFECTSDLG_PASSMODE, false);
                m_Dialog.SetControlEnabled(EFFECTSDLG_PASS, false);
            }
            else
            {
                std::vector<TechniqueInfo>& TechniqueList = STATE_GET( Render )->GetEffectInfo( );

                pComboBox = m_Dialog.GetComboBox(EFFECTSDLG_TECHNIQUE);
                if(pComboBox)
                {
                    pComboBox->SetEnabled( true );

                    int index = 0;
                    pComboBox->RemoveAllItems();

                    for( std::vector<TechniqueInfo>::const_iterator itr = TechniqueList.begin();
                        itr != TechniqueList.end();
                        itr++, index++)
                    {
                        pComboBox->AddItem( itr->Name.c_str(), UlongToPtr( index ) );
                    }

                    pComboBox->SetSelectedByData(  ULongToPtr( STATE_GET( TechniqueSelected ) ) );
                }

                pComboBox = m_Dialog.GetComboBox(EFFECTSDLG_PASSMODE);
                if(pComboBox)
                {
                    pComboBox->SetEnabled( true );
                    pComboBox->SetSelectedByData(  ULongToPtr( STATE_GET( PassMode ) ) );


                    enabled = ( STATE_GET( PassMode ) == DXVTECHNIQUEMODE_SELECTED );
                    if(!enabled)
                    {
                        m_Dialog.SetControlEnabled(EFFECTSDLG_PASS, false);

                    }
                    else
                    {
                        pComboBox = m_Dialog.GetComboBox(EFFECTSDLG_PASS);
                        if(pComboBox)
                        {
                            pComboBox->SetEnabled( true );

                            int index = 0;
                            pComboBox->RemoveAllItems();    

                            std::vector<std::wstring>& PassList = TechniqueList[ STATE_GET( TechniqueSelected ) ].Passes;
                            for( std::vector<std::wstring>::const_iterator itr = PassList.begin();
                                itr != PassList.end();
                                itr++, index++)
                            {
                                pComboBox->AddItem( itr->c_str(), UlongToPtr( index ) );
                            }

                            pComboBox->SetSelectedByData(  ULongToPtr( STATE_GET( PassSelected ) ) );
                        }
                    }
                }
            }
        }
    }

    pEditBox = m_Dialog.GetEditBox(EFFECTSDLG_DEFINES);
    if( pEditBox )
    {
        pEditBox->SetVisible( true );
        pEditBox->SetEnabled( true );

        wstring defineStr;
        BuildShaderDefines( defineStr );
        pEditBox->SetText( defineStr.c_str() );

    }


    pEditBox = m_Dialog.GetEditBox(EFFECTSDLG_INCLUDES);
    if( pEditBox )
    {
        pEditBox->SetVisible( true );
        pEditBox->SetEnabled( true );

        pEditBox->SetText( STATE_GET( Includes ).c_str() );
    }

    
    pCheckBox = m_Dialog.GetCheckBox(EFFECTSDLG_DEBUGPS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( DebugPixelShaders ) );

    pCheckBox = m_Dialog.GetCheckBox(EFFECTSDLG_DEBUGVS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( DebugVertexShaders ) );

    pCheckBox = m_Dialog.GetCheckBox(EFFECTSDLG_OPTIMIZE);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( OptimizeShaders ) );

    pCheckBox = m_Dialog.GetCheckBox(EFFECTSDLG_VALIDATE);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( ValidateShaders ) );

    pCheckBox = m_Dialog.GetCheckBox(EFFECTSDLG_PRESHADE);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( Preshaders ) );

    pCheckBox = m_Dialog.GetCheckBox(EFFECTSDLG_PARTIALS);
    if(pCheckBox)
        pCheckBox->SetChecked( STATE_GET( PartialPrecision ) );

    pRadio = m_Dialog.GetRadioButton(EFFECTSDLG_DYNAFLOWGOOD);
    if(pRadio)
        pRadio->SetChecked( STATE_GET( PreferDynamicFlow ) );

    pRadio = m_Dialog.GetRadioButton(EFFECTSDLG_DYNAFLOWBAD);
    if(pRadio)
        pRadio->SetChecked( ! STATE_GET( PreferDynamicFlow ) );
}

void EffectHUD::OnEvent( UINT nEvent, int nControlID, CDXUTControl* pControl )
{
   switch( nControlID )
    {
        case EFFECTSDLG_OK:
            if(ResetDevice)
                STATE_SET(Reset, DXVRESET_RESETDEVICE);
            else if(ReloadContent)
                STATE_SET(Reset, DXVRESET_RELOADCONTENT);


            if(ResetDevice || ReloadContent)
            {
                CDXUTEditBox* pEditBox;
                
                pEditBox = m_Dialog.GetEditBox(EFFECTSDLG_DEFINES);
                if(pEditBox)
                    DXVGetApp().ParseShaderDefines( pEditBox->GetText() );

                pEditBox = m_Dialog.GetEditBox(EFFECTSDLG_INCLUDES);
                if(pEditBox)
                    STATE_SET( Includes, pEditBox->GetText() );
            }

            ResetDevice = false;
            ReloadContent = false;
            SetActive(false);
            DXVGetMainHUD().SetActive(true);
            break;
        case EFFECTSDLG_TECHNIQUEMODE:
            STATE_SET( TechniqueMode , (DXVTECHNIQUEMODE) PtrToUlong( ((CDXUTComboBox*)pControl)->GetSelectedData() )  );
            Refresh();
            break;
        case EFFECTSDLG_TECHNIQUE:
            STATE_SET( TechniqueSelected , (int) PtrToUlong( ((CDXUTComboBox*)pControl)->GetSelectedData() )  );
            Refresh();
            break;
        case EFFECTSDLG_PASSMODE:
            STATE_SET( PassMode , (DXVPASSMODE) PtrToUlong( ((CDXUTComboBox*)pControl)->GetSelectedData() )  );
            Refresh();
            break;
        case EFFECTSDLG_PASS:
            STATE_SET( PassSelected , (int) PtrToUlong( ((CDXUTComboBox*)pControl)->GetSelectedData() )  );
            break;
        case EFFECTSDLG_DEFINES:
            ReloadContent = true;
            break;
        case EFFECTSDLG_INCLUDES:
            ReloadContent = true;
            break;
        case EFFECTSDLG_DEBUGVS:
            STATE_SET( DebugVertexShaders , ((CDXUTCheckBox*)pControl)->GetChecked() );
            ResetDevice = true;
            break;
        case EFFECTSDLG_DEBUGPS:
            STATE_SET( DebugPixelShaders , ((CDXUTCheckBox*)pControl)->GetChecked() );
            ResetDevice = true;
            break;
        case EFFECTSDLG_OPTIMIZE:
            STATE_SET( OptimizeShaders , ((CDXUTCheckBox*)pControl)->GetChecked() );
            ReloadContent = true;
            break;
        case EFFECTSDLG_VALIDATE:
            STATE_SET( ValidateShaders , ((CDXUTCheckBox*)pControl)->GetChecked() );
            ReloadContent = true;
            break;
        case EFFECTSDLG_PRESHADE:
            STATE_SET( Preshaders , ((CDXUTCheckBox*)pControl)->GetChecked() );
            ReloadContent = true;
            break;
        case EFFECTSDLG_PARTIALS:
            STATE_SET( PartialPrecision , ((CDXUTCheckBox*)pControl)->GetChecked() );
            ReloadContent = true;
            break;
        case EFFECTSDLG_DYNAFLOWGOOD:
            STATE_SET( PreferDynamicFlow , ((CDXUTCheckBox*)pControl)->GetChecked() );
            ReloadContent = true;
            break;
        case EFFECTSDLG_DYNAFLOWBAD:
            STATE_SET( PreferDynamicFlow , ! ((CDXUTCheckBox*)pControl)->GetChecked() );
            ReloadContent = true;
            break;
   }
}
