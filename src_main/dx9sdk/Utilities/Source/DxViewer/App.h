#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include "sas.h"
#include "DXUT.h"
#include "DXUTGUI.h"
#include "DXUTENUM.h"
#include "Dispatch.h"
#include "macros.h"

#include "windows.h"
#include "commdlg.h"

#include "Stateblock.h"



class SasMsgListener : public Sas::IMessageListener
{
public:
    FRIEND_SINGLETON_H( SasMsgListener );
    virtual void OnSasMessage( Sas::MessageLevel level, const WCHAR* message );

};

DECLARE_SINGLETON_H( SasMsgListener );

class App
{
public:
    FRIEND_SINGLETON_H( App );

    Dispatcher<LPDXUTCALLBACKD3D9DEVICECREATED>            OnD3D9DeviceCreated;//FIFO
    Dispatcher<LPDXUTCALLBACKD3D9DEVICERESET>            OnD3D9DeviceReset;//FIFO
    Dispatcher<LPDXUTCALLBACKD3D9DEVICELOST>            OnD3D9DeviceLost;//LIFO
    Dispatcher<LPDXUTCALLBACKD3D9DEVICEDESTROYED>        OnD3D9DeviceDestroyed;//LIFO
    
    Dispatcher<LPDXUTCALLBACKD3D10DEVICECREATED>        OnD3D10DeviceCreated;//FIFO
    Dispatcher<LPDXUTCALLBACKD3D10SWAPCHAINRESIZED>        OnD3D10SwapChainResized;//FIFO
    Dispatcher<LPDXUTCALLBACKD3D10SWAPCHAINRELEASING>    OnD3D10SwapChainReleasing;//LIFO
    Dispatcher<LPDXUTCALLBACKD3D10DEVICEDESTROYED>        OnD3D10DeviceDestroyed;//LIFO

    INT Main( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );

    void Shutdown();

    void ParseCommandLine(LPSTR args);
    void RegisterCallbacks();
    bool CreateMainDialog();
    void CreateRenderWindow();


    void LoadFile( LPCWSTR filepath );
    void LoadX( LPCWSTR filepath );
    void LoadFX( LPCWSTR filepath );
    void LoadDDS( LPCWSTR filepath );
    void Reload();

    void ParseShaderDefines(const TCHAR* defines);

    bool ShowDdsOrFxOrXDialog(bool bX, bool bFx, bool bDds);
    void ShowEnvironmentDialog();
    bool ShowFileDialog(OPENFILENAME& ofn); //Open File helper...

    void OnVersionChange();
    void OnDropFile(WPARAM wParam);
    void OnSize();
    void OutputA(const CHAR* message, ... );
    void OutputW(const WCHAR* message, ... );
    void OutputClear();

    void Help();

    void ForceDeviceChangeByState();
    void ForceDeviceChange(DXUTDeviceVersion newVer);


    INT_PTR OnDialogMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    LRESULT OnRenderMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing );
    bool    OnKeyboardMsg( UINT nChar, bool bKeyDown, bool bAltDown );
    void    OnFrameMove( double fTime, float fElapsedTime );
    void    OnFrameRender( double fTime, float fElapsedTime);
    bool    OnModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings);

    float LastRenderTime;
    int RenderCount;

protected:
    App():
        OnD3D10SwapChainReleasing(DISPATCH_LIFO),
        OnD3D10DeviceDestroyed(DISPATCH_LIFO),
        OnD3D9DeviceLost(DISPATCH_LIFO),
        OnD3D9DeviceDestroyed(DISPATCH_LIFO)
    {
        LastRenderTime = 0;
        RenderCount = 0;
    }

    static INT_PTR CALLBACK StaticDialogMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )  { return DXVGetApp().OnDialogMsg( hWnd, uMsg, wParam, lParam); }
    static LRESULT CALLBACK StaticOnRenderMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext ) { return DXVGetApp().OnRenderMsg( hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing); }
    static void    CALLBACK StaticOnKeyboardMsg( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext ) { DXVGetApp().OnKeyboardMsg( nChar, bKeyDown, bAltDown ); }
    static void    CALLBACK StaticOnFrameMove( double fTime, float fElapsedTime, void* pUserContext ) { DXVGetApp().OnFrameMove( fTime, fElapsedTime ); }

    static void    CALLBACK StaticOnFrameRender9( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext ) { App::StaticOnFrameRender( fTime, fElapsedTime, pUserContext); }
    static void    CALLBACK StaticOnFrameRender10( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext ) { App::StaticOnFrameRender( fTime, fElapsedTime, pUserContext); }
    static void    CALLBACK StaticOnFrameRender( double fTime, float fElapsedTime, void* pUserContext ) { DXVGetApp().OnFrameRender(fTime,fElapsedTime); }
    static bool       CALLBACK StaticOnModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext ) { return DXVGetApp().OnModifyDeviceSettings( pDeviceSettings); }

    static HRESULT CALLBACK StaticOnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
    static HRESULT CALLBACK StaticOnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
    static void    CALLBACK StaticOnD3D9DeviceLost( void* pUserContext );
    static void    CALLBACK StaticOnD3D9DeviceDestroyed( void* pUserContext );

    static HRESULT CALLBACK StaticOnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) ;
    static HRESULT CALLBACK StaticOnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
    static void    CALLBACK StaticOnD3D10ReleasingSwapChain( void* pUserContext );
    static void    CALLBACK StaticOnD3D10DestroyDevice( void* pUserContext );


    void ParseShaderDefine(DefineList& Defines, const TCHAR* statement);
    void AddShaderDefine(DefineList& Defines, const TCHAR* name, const TCHAR* value);


};

DECLARE_SINGLETON_H( App );
