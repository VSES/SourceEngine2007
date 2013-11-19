#pragma once
#include "Macros.h"
#include "Render.h"
#include "Model10.h"

extern const D3D10_INPUT_ELEMENT_DESC DeclFSQ10[];

class Render10 : public Render
{
protected:

    Render10() : Render()
    {
        DepthStencilTex = NULL;
        DepthStencilRTV = NULL;
        ImageTex = NULL;
        ImageRTV = NULL;
        ImageSRV = NULL;

        SolidMode = NULL;
        WireMode = NULL;

        FSQLayout = NULL;
        FSQMesh = NULL;
        FSQEffect = NULL;
        FSQImageVariable = NULL;

        pFont = NULL;
        pSprite = NULL;

        m_pInitialState = NULL;
    }


    static bool    CALLBACK StaticIsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext ) { return DXVGetRender10().IsD3D10DeviceAcceptable( Adapter, Output, DeviceType, BackBufferFormat, bWindowed ); }
    static HRESULT CALLBACK StaticOnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ) { return DXVGetRender10().OnD3D10CreateDevice( pd3dDevice, pBackBufferSurfaceDesc ); }
    static HRESULT CALLBACK StaticOnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext ){ return DXVGetRender10().OnD3D10ResizedSwapChain( pd3dDevice, pSwapChain, pBackBufferSurfaceDesc ); }
    static void    CALLBACK StaticOnD3D10ReleasingSwapChain( void* pUserContext ) { return DXVGetRender10().OnD3D10ReleasingSwapChain(); }
    static void    CALLBACK StaticOnD3D10DestroyDevice( void* pUserContext ) { return DXVGetRender10().OnD3D10DestroyDevice(); }

    bool    IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BackBufferFormat, bool bWindowed);
    HRESULT OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
    HRESULT OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
    void    OnD3D10ReleasingSwapChain();
    void    OnD3D10DestroyDevice();

    HRESULT OnFrameRender( Transform& frameIterator );
    HRESULT OnFrameRender( ModelSubset10& meshIterator, D3DXMATRIX& WorldMatrix );
    void RenderText();


    HRESULT CreateFSQ( ID3D10Device* pd3dDevice );

    ID3DX10Font* pFont;
    ID3DX10Sprite* pSprite;

    ID3D10Texture2D*        DepthStencilTex;
    ID3D10DepthStencilView* DepthStencilRTV;
    ID3D10Texture2D*        ImageTex;
    ID3D10RenderTargetView* ImageRTV;
    ID3D10ShaderResourceView* ImageSRV;

    ID3D10RasterizerState* SolidMode;
    ID3D10RasterizerState* WireMode;


    ID3D10InputLayout*      FSQLayout;
    ID3DX10Mesh*            FSQMesh;
    ID3D10Effect*            FSQEffect;
    ID3D10EffectShaderResourceVariable* FSQImageVariable;


    ID3D10StateBlock* m_pInitialState;


public:
    FRIEND_SINGLETON_H( Render10 );

    virtual ~Render10() { Render::~Render(); }

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

DECLARE_SINGLETON_H( Render10 );
