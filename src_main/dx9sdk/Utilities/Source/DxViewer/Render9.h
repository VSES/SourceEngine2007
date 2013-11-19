#pragma once
#include "macros.h"
#include "Render.h"
#include "Model9.h"

struct DXUTDeviceSettings;

class Render9 : public Render
{
    FRIEND_SINGLETON_H( Render9 );
protected:
    Render9(); 

    static bool    CALLBACK StaticIsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext ){ return DXVGetRender9().IsD3D9DeviceAcceptable( pCaps, AdapterFormat, BackBufferFormat, bWindowed); }
    static HRESULT CALLBACK StaticOnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )    { return DXVGetRender9().OnD3D9DeviceCreated( pd3dDevice, pBackBufferSurfaceDesc); }
    static HRESULT CALLBACK StaticOnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) { return DXVGetRender9().OnD3D9DeviceReset( pd3dDevice, pBackBufferSurfaceDesc); }
    static void    CALLBACK StaticOnD3D9DeviceLost( void* pUserContext ) { DXVGetRender9().OnD3D9DeviceLost(); }
    static void    CALLBACK StaticOnD3D9DeviceDestroyed( void* pUserContext ) { DXVGetRender9().OnD3D9DeviceDestroyed(); }

    bool    IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed );
    HRESULT OnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
    HRESULT OnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
    void    OnD3D9DeviceLost();
    void    OnD3D9DeviceDestroyed();

    HRESULT OnFrameRender( Transform& frameIterator );
    HRESULT OnFrameRender( ModelSubset9& meshIterator, D3DXMATRIX& WorldMatrix );
    void RenderText();



    LPDIRECT3DSURFACE9 Image;
    ID3DXFont* pFont;
    ID3DXSprite* pSprite;
public:
    virtual ~Render9() { Render::~Render(); }

    virtual DXUTDeviceVersion GetVersion();
    virtual void Startup();
    virtual void Shutdown();
    virtual void OnFrameMove( double fTime, float fElapsedTime );
    virtual void OnFrameRender( double fTime, float fElapsedTime );
    virtual bool OnModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings );

    virtual void Load();
    virtual void Clear();

    virtual float GetModelRadius();
    virtual const D3DXVECTOR3& GetModelCenter();
    virtual const D3DXVECTOR3& GetMin();
    virtual const D3DXVECTOR3& GetMax();

};

DECLARE_SINGLETON_H( Render9 );
