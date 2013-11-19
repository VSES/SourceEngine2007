#include "DXUT.h"
#include "HUD.h"
#include "App.h"
#include "StateBlock.h"

DWORD HudManager::BackgroundColor = D3DCOLOR_ARGB(192, 98, 138, 192);

DECLARE_SINGLETON_CPP( HudManager );


HUD::~HUD()
{
    SAFE_RELEASE(m_pStateBlock);
    SAFE_RELEASE(m_pStateBlock10);
}

HUD::HUD()
{
    m_pStateBlock = NULL;
    m_pStateBlock10 = NULL;
    m_bActive = false;
}

//--------------------------------------------------------------------------------------
void HudManager::ForceFocus()
{
    for( std::list<HUD*>::iterator itr = HudList.begin(); itr != HudList.end(); itr++ )
    {
        if( (*itr)->IsActive() )
        {
            CDXUTDialog* pDialog =(*itr)->GetDialog();
            if( pDialog 
                && pDialog->GetVisible() 
                && !pDialog->GetMinimized() 
                && pDialog->IsKeyboardInputEnabled() )
            {
                pDialog->FocusDefaultControl();
                return;
            }
        }
    }
}

//--------------------------------------------------------------------------------------
void HudManager::Startup()
{
    DXVGetApp().OnD3D9DeviceCreated.AddFunc(HudManager::StaticOnD3D9DeviceCreated);
    DXVGetApp().OnD3D9DeviceReset.AddFunc(HudManager::StaticOnD3D9DeviceReset);
    DXVGetApp().OnD3D9DeviceLost.AddFunc(HudManager::StaticOnD3D9DeviceLost);
    DXVGetApp().OnD3D9DeviceDestroyed.AddFunc(HudManager::StaticOnD3D9DeviceDestroyed);

    DXVGetApp().OnD3D10DeviceCreated.AddFunc( HudManager::StaticOnD3D10DeviceCreated );
    DXVGetApp().OnD3D10SwapChainResized.AddFunc( HudManager::StaticOnD3D10ResizedSwapChain );
    DXVGetApp().OnD3D10SwapChainReleasing.AddFunc( HudManager::StaticOnD3D10ReleasingSwapChain );
    DXVGetApp().OnD3D10DeviceDestroyed.AddFunc( HudManager::StaticOnD3D10DeviceDestroyed);


}

void HudManager::Shutdown()
{
    for( std::list<HUD*>::iterator itr = HudList.begin(); 
        itr != HudList.end(); 
        itr = DXVGetHudManager().HudList.begin() )//keep pulling off of the beginning until this list is empty
    {
        (*itr)->Shutdown();
    }

    DXVGetApp().OnD3D9DeviceCreated.RemoveFunc(HudManager::StaticOnD3D9DeviceCreated);
    DXVGetApp().OnD3D9DeviceReset.RemoveFunc(HudManager::StaticOnD3D9DeviceReset);
    DXVGetApp().OnD3D9DeviceLost.RemoveFunc(HudManager::StaticOnD3D9DeviceLost);
    DXVGetApp().OnD3D9DeviceDestroyed.RemoveFunc(HudManager::StaticOnD3D9DeviceDestroyed);

    DXVGetApp().OnD3D10DeviceCreated.RemoveFunc( HudManager::StaticOnD3D10DeviceCreated );
    DXVGetApp().OnD3D10SwapChainResized.RemoveFunc( HudManager::StaticOnD3D10ResizedSwapChain );
    DXVGetApp().OnD3D10SwapChainReleasing.RemoveFunc( HudManager::StaticOnD3D10ReleasingSwapChain );
    DXVGetApp().OnD3D10DeviceDestroyed.RemoveFunc( HudManager::StaticOnD3D10DeviceDestroyed);
}


//--------------------------------------------------------------------------------------
HRESULT CALLBACK HudManager::StaticOnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )    
{ 
    HRESULT hr = S_OK;
    
    hr= DXVGetHudManager().DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ); 
    for( std::list<HUD*>::iterator itr = DXVGetHudManager().HudList.begin(); 
        itr != DXVGetHudManager().HudList.end() && hr == S_OK; 
        itr++ )
    {
        hr = (*itr)->OnD3D9DeviceCreated( pd3dDevice ); 
    }

    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT CALLBACK HudManager::StaticOnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) 
{ 
    HRESULT hr = S_OK;
    
    hr= DXVGetHudManager().DialogResourceManager.OnD3D9ResetDevice( ); 
    for( std::list<HUD*>::iterator itr = DXVGetHudManager().HudList.begin(); 
        itr != DXVGetHudManager().HudList.end() && hr == S_OK; 
        itr++ )
    {
        hr = (*itr)->OnD3D9DeviceReset( ); 
    }

    return hr;
}

//--------------------------------------------------------------------------------------
void    CALLBACK HudManager::StaticOnD3D9DeviceLost( void* pUserContext ) 
{ 
    for( std::list<HUD*>::iterator itr = DXVGetHudManager().HudList.begin(); 
        itr != DXVGetHudManager().HudList.end(); 
        itr++ )
    {
        (*itr)->OnD3D9DeviceLost( ); 
    }
    DXVGetHudManager().DialogResourceManager.OnD3D9LostDevice( ); 
}

//--------------------------------------------------------------------------------------
void    CALLBACK HudManager::StaticOnD3D9DeviceDestroyed( void* pUserContext ) 
{ 
    for( std::list<HUD*>::iterator itr = DXVGetHudManager().HudList.begin(); 
        itr != DXVGetHudManager().HudList.end(); 
        itr++ )
    {
        (*itr)->OnD3D9DeviceDestroyed( ); 
    }
    DXVGetHudManager().DialogResourceManager.OnD3D9DestroyDevice(); 
}


HRESULT CALLBACK HudManager::StaticOnD3D10DeviceCreated( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) 
{ 
    HRESULT hr = S_OK;
    
    hr= DXVGetHudManager().DialogResourceManager.OnD3D10CreateDevice( pd3dDevice ); 
    for( std::list<HUD*>::iterator itr = DXVGetHudManager().HudList.begin(); 
        itr != DXVGetHudManager().HudList.end() && hr == S_OK; 
        itr++ )
    {
        hr = (*itr)->OnD3D10DeviceCreated( pd3dDevice, pBackBufferSurfaceDesc ); 
    }

    return hr;
}

HRESULT CALLBACK HudManager::StaticOnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{ 
    HRESULT hr = S_OK;
    
    hr= DXVGetHudManager().DialogResourceManager.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ); 
    for( std::list<HUD*>::iterator itr = DXVGetHudManager().HudList.begin(); 
        itr != DXVGetHudManager().HudList.end() && hr == S_OK; 
        itr++ )
    {
        hr = (*itr)->OnD3D10ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc ); 
    }

    return hr;
}

void    CALLBACK HudManager::StaticOnD3D10ReleasingSwapChain( void* pUserContext ) 
{ 
    for( std::list<HUD*>::iterator itr = DXVGetHudManager().HudList.begin(); 
        itr != DXVGetHudManager().HudList.end(); 
        itr++ )
    {
        (*itr)->OnD3D10ReleasingSwapChain(); 
    }

    DXVGetHudManager().DialogResourceManager.OnD3D10ReleasingSwapChain(); 
}

void    CALLBACK HudManager::StaticOnD3D10DeviceDestroyed( void* pUserContext ) 
{ 
    for( std::list<HUD*>::iterator itr = DXVGetHudManager().HudList.begin(); 
        itr != DXVGetHudManager().HudList.end(); 
        itr++ )
    {
        (*itr)->OnD3D10DeviceDestroyed( ); 
    }
    DXVGetHudManager().DialogResourceManager.OnD3D10DestroyDevice(); 
}

//--------------------------------------------------------------------------------------
bool HudManager::OnKeyboardMsg( UINT nChar, bool bKeyDown, bool bAltDown )
{
    switch( nChar )
    {
    case VK_TAB:
        if( ((0x8000 & GetKeyState(VK_LCONTROL)) != 0)
            ||  ((0x8000 & GetKeyState(VK_RCONTROL)) != 0)
            ||  ((0x8000 & GetKeyState(VK_CONTROL)) != 0))
        {
            ForceFocus();

        }
        return true;
     }
    return false;
}

//--------------------------------------------------------------------------------------
bool HudManager::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    // this is for IME 
    if( DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam))
        return true;

    for( std::list<HUD*>::iterator itr = HudList.begin();
        itr != HudList.end();
        itr++ )
    {
        if( (*itr)->IsActive() && (*itr)->MsgProc( hWnd, uMsg, wParam, lParam ) )
            return true;
    }

    return false;
}

void HudManager::OnRender( float fElapsedTime )
{
    for( std::list<HUD*>::iterator itr = HudList.begin();
        itr != HudList.end();
        itr++ )
    {
        if( (*itr)->IsActive() ) 
            (*itr)->OnRender( fElapsedTime );
    }
}
//--------------------------------------------------------------------------------------
void HUD::Startup( )
{
    DXVGetHudManager().HudList.push_back(this);

    m_Dialog.Init( &DXVGetHudManager().DialogResourceManager , true ); 
    m_Dialog.EnableKeyboardInput( true );

    RegisterEventCallback();

    CreateControls();
    Refresh();
}

void HUD::Shutdown()
{
    DXVGetHudManager().DialogResourceManager.UnregisterDialog( &m_Dialog );
    DXVGetHudManager().HudList.remove(this);
}



//--------------------------------------------------------------------------------------
void HUD::OnRender( float fElapsedTime )
{
    if( STATE_GET( Version ) == DXUT_D3D10_DEVICE )
        OnRender10( fElapsedTime );
    else if( STATE_GET( Version ) == DXUT_D3D9_DEVICE )
        OnRender9( fElapsedTime );
}

//--------------------------------------------------------------------------------------
void HUD::OnRender9( float fElapsedTime )
{
    IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
        m_pStateBlock->Capture();
        pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID ); 
        m_Dialog.OnRender( fElapsedTime );
        m_pStateBlock->Apply();
        pd3dDevice->EndScene();
    }
}


//--------------------------------------------------------------------------------------
void HUD::OnRender10( float fElapsedTime )
{
    // Render the scene
    m_pStateBlock10->Capture();
    m_Dialog.OnRender( fElapsedTime );
    m_pStateBlock10->Apply();
}

//--------------------------------------------------------------------------------------

LRESULT HUD::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    return m_Dialog.MsgProc( hWnd, uMsg, wParam, lParam );
}



//--------------------------------------------------------------------------------------
HRESULT HUD::OnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice )
{
    if( pd3dDevice == NULL )
        return DXUT_ERR_MSGBOX( L"HUD::OnCreatedDevice", E_INVALIDARG );

  
    return S_OK;
}
//--------------------------------------------------------------------------------------
HRESULT HUD::OnD3D9DeviceReset()
{

    IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();
    pd3dDevice->BeginStateBlock();
    pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
    pd3dDevice->EndStateBlock( &m_pStateBlock );

    m_Dialog.RemoveAllControls();
    CreateControls();
    Refresh();

    return S_OK;
}

//--------------------------------------------------------------------------------------
void HUD::OnD3D9DeviceLost()
{
    SAFE_RELEASE( m_pStateBlock );
}

//--------------------------------------------------------------------------------------
void HUD::OnD3D9DeviceDestroyed()
{
}
//--------------------------------------------------------------------------------------


HRESULT HUD::OnD3D10DeviceCreated( ID3D10Device* pd3dDevice , const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
    HRESULT hr = S_OK;
    
    D3D10_STATE_BLOCK_MASK SBM;
    DXUT_Dynamic_D3D10StateBlockMaskEnableCapture( &SBM, D3D10_DST_RS_RASTERIZER_STATE, 0, 1 );
    V(DXUT_Dynamic_D3D10CreateStateBlock( pd3dDevice, &SBM, &m_pStateBlock10 ));

    return hr;
}


HRESULT HUD::OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc )
{
    m_Dialog.RemoveAllControls();
    CreateControls();
    Refresh();

    return S_OK;
}

void HUD::OnD3D10ReleasingSwapChain()
{
}

void HUD::OnD3D10DeviceDestroyed()
{
    SAFE_RELEASE( m_pStateBlock10 );
}


