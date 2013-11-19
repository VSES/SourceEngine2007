#pragma once
#pragma warning(disable: 4995)
#include <list>
#include <string>
#pragma warning(default: 4995)

#include "d3d9.h"
#include "d3dx9.h"
#include "d3d10.h"
#include "d3dx10.h"
#include "dxut.h"
#include "dxutgui.h"
#include "dxutmisc.h"
#include "macros.h"


//to be used inside of derived HUD class headers to provide necessary functions
#define HUD_STATIC_ON_EVENT \
virtual void RegisterEventCallback() {  m_Dialog.SetCallback( StaticOnEvent, (void*) this ); } \
static void WINAPI StaticOnEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserData ) { if( pUserData ) (( HUD*) pUserData)->OnEvent( nEvent, nControlID, pControl ); }

class HUD
{
public:
    HUD();
    void OnRender( float fElapsedTime );
    void OnRender9( float fElapsedTime );
    void OnRender10( float fElapsedTime );

    HRESULT OnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice );
    HRESULT OnD3D9DeviceReset();
    void    OnD3D9DeviceLost();
    void    OnD3D9DeviceDestroyed();


    HRESULT OnD3D10DeviceCreated( ID3D10Device* pd3dDevice , const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
    HRESULT OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc );
    void    OnD3D10ReleasingSwapChain();
    void    OnD3D10DeviceDestroyed();

    virtual void Startup();
    virtual void Shutdown();


    CDXUTDialog* GetDialog() { return &m_Dialog; }
    bool IsActive() { return m_bActive; }
    void SetActive( bool bActive ) { m_bActive = bActive; if( bActive ) Refresh(); }
    virtual LRESULT MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    virtual void Refresh() = 0;
    virtual void OnEvent( UINT nEvent, int nControlID, CDXUTControl* pControl )=0;
    virtual ~HUD();

protected:
    virtual void CreateControls() = 0;
    virtual void RegisterEventCallback() = 0; //use HUD_STATIC_ON_EVENT
    
    IDirect3DStateBlock9* m_pStateBlock;
    ID3D10StateBlock* m_pStateBlock10;
    CDXUTDialog m_Dialog;
    bool m_bActive; 
};



class HudManager
{
public:
    FRIEND_SINGLETON_H( HudManager );

    static DWORD BackgroundColor;

    void ForceFocus();
    
    void Startup();
    void Shutdown();

    HRESULT Refresh();
    void OnRender( float fElapsedTime );

    bool OnKeyboardMsg( UINT nChar, bool bKeyDown, bool bAltDown );
    bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


    std::list<HUD*> HudList;

    CDXUTDialogResourceManager DialogResourceManager; 
protected:    
    static HRESULT CALLBACK StaticOnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
    static HRESULT CALLBACK StaticOnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
    static void    CALLBACK StaticOnD3D9DeviceLost( void* pUserContext );
    static void    CALLBACK StaticOnD3D9DeviceDestroyed( void* pUserContext );

    static HRESULT CALLBACK StaticOnD3D10DeviceCreated( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
    static HRESULT CALLBACK StaticOnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
    static void    CALLBACK StaticOnD3D10ReleasingSwapChain( void* pUserContext );
    static void    CALLBACK StaticOnD3D10DeviceDestroyed( void* pUserContext );

};

DECLARE_SINGLETON_H( HudManager );

