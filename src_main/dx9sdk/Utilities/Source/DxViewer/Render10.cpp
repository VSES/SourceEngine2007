#include "sas.h"
#include "DXUT.h"
#include "DXUTmisc.h"
#include "SDKmisc.h"//DXUT

#include "windows.h"
#include "resource.h"

#include "App.h"
#include "stateblock.h"
#include "camera.h"
#include "hud.h"
#include "Model10.h"
#include "Render10.h"
#include "IncludeHandler10.h"
#include "FileSystem.h"


#include "ShowShared10.h"



const D3D10_INPUT_ELEMENT_DESC DeclFSQ10[] =
{
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
};


DECLARE_SINGLETON_CPP( Render10 );

DXUTDeviceVersion Render10::GetVersion() { return DXUT_D3D10_DEVICE; }

void Render10::Shutdown()
{
}

void Render10::Startup()
{
    DXUTSetCallbackD3D10DeviceAcceptable( Render10::StaticIsD3D10DeviceAcceptable );
    
    DXVGetApp().OnD3D10DeviceCreated.AddFunc( Render10::StaticOnD3D10CreateDevice );
    DXVGetApp().OnD3D10SwapChainResized.AddFunc( Render10::StaticOnD3D10ResizedSwapChain );
    DXVGetApp().OnD3D10SwapChainReleasing.AddFunc( Render10::StaticOnD3D10ReleasingSwapChain );
    DXVGetApp().OnD3D10DeviceDestroyed.AddFunc( Render10::StaticOnD3D10DestroyDevice);
}

bool    Render10::IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BackBufferFormat, bool bWindowed)
{
    return true;
}


bool    Render10::OnModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings )
{
    HWND hwnd = DXUTGetHWNDDeviceWindowed();
    if(pDeviceSettings->d3d10.sd.Windowed != FALSE)
    {
        SetParent(hwnd, STATE_GET(Dialog) );
        DXVGetApp().OnSize();
    }
    else
        SetParent(hwnd, NULL);

    if( STATE_GET(DebugPixelShaders) )
        pDeviceSettings->d3d10.DriverType = D3D10_DRIVER_TYPE_REFERENCE;

#if defined(DEBUG) | defined(_DEBUG)
        pDeviceSettings->d3d10.CreateFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

    pDeviceSettings->d3d10.sd.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;//required for single manual refresh
    pDeviceSettings->d3d10.sd.BufferCount = 1; //required for D3DSWAPEFFECT_COPY

    return true;
}

HRESULT Render10::OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
    HRESULT hr = S_OK;

    D3D10_STATE_BLOCK_MASK SBM;
    V(DXUT_Dynamic_D3D10StateBlockMaskEnableAll( &SBM));
    V(DXUT_Dynamic_D3D10CreateStateBlock( pd3dDevice, &SBM, &m_pInitialState ));

    m_pInitialState->Capture();


    D3D10_RASTERIZER_DESC desc, solidDesc, wireDesc;
    

    ZeroMemory(&desc, sizeof(D3D10_RASTERIZER_DESC));
    desc.FillMode=D3D10_FILL_SOLID;
    desc.CullMode=D3D10_CULL_NONE;//D3D10_CULL_BACK;
    desc.FrontCounterClockwise=false;
    desc.DepthBias = 0;
    desc.DepthBiasClamp = 0.0f;
    desc.SlopeScaledDepthBias = 0.0f;
    desc.DepthClipEnable= true;
    desc.ScissorEnable=false;
    desc.MultisampleEnable=false;
    desc.AntialiasedLineEnable=false;

    solidDesc = desc;
    solidDesc.FillMode = D3D10_FILL_SOLID;

    SAFE_RELEASE(SolidMode);
    V( pd3dDevice->CreateRasterizerState( &solidDesc, &SolidMode) );


    wireDesc = desc;
    wireDesc.FillMode = D3D10_FILL_WIREFRAME;

    SAFE_RELEASE(WireMode);
    V( pd3dDevice->CreateRasterizerState( &wireDesc, &WireMode) );



    V( CreateFSQ(pd3dDevice) );


    DXVGetShowShared10().Startup();

    return hr;
}

HRESULT Render10::CreateFSQ( ID3D10Device* pd3dDevice )
{
    HRESULT hr = S_OK;
    
    ID3D10Blob* errors = NULL;

    D3DX10CreateMesh(
            pd3dDevice, 
            DeclFSQ10, 
            sizeof(DeclFSQ10)/sizeof(D3D10_INPUT_ELEMENT_DESC), 
            "SV_Position", 
            4, 
            2, 
            0, 
            &FSQMesh);

    V( FSQMesh->SetIndexData( (void*)IbFSQ, 6) );
    V( FSQMesh->SetVertexData(0, (void*)VbFSQ) );
    V( FSQMesh->CommitToDevice() );


    V( D3DX10CreateEffectFromResourceW( 
            NULL, 
            MAKEINTRESOURCE(IDR_FSQ10FX), 
            L"FSQ", 
            NULL, 
            &DXVGetIncludeHandler10(L""),
            "fx_4_0",
            0, 
            0, 
            pd3dDevice, 
            NULL, 
            NULL, 
            &FSQEffect, 
            &errors,
            NULL ) );

    if(errors)
        DXVGetApp().OutputA( (char*)errors->GetBufferPointer() );


    if(SUCCEEDED(hr))
    {
        FSQImageVariable =  FSQEffect->GetVariableByName("Image")->AsShaderResource();

        D3D10_PASS_DESC FSQPassDesc;
        V( FSQEffect->GetTechniqueByIndex(0)->GetPassByIndex(0)->GetDesc( &FSQPassDesc ) );

        V( pd3dDevice->CreateInputLayout(
                DeclFSQ10, 
                sizeof(DeclFSQ10)/sizeof(D3D10_INPUT_ELEMENT_DESC),
                FSQPassDesc.pIAInputSignature,
                FSQPassDesc.IAInputSignatureSize,
                &FSQLayout ) );
    }
    SAFE_RELEASE(errors);
    return hr;

}

HRESULT Render10::OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
    HRESULT hr = S_OK;


    // Create depth stencil texture.
    SAFE_RELEASE( DepthStencilTex );
    D3D10_TEXTURE2D_DESC DescDST;
    DescDST.Width = pBackBufferSurfaceDesc->Width;
    DescDST.Height = pBackBufferSurfaceDesc->Height;
    DescDST.MipLevels = 1;
    DescDST.ArraySize = 1;
    DescDST.Format = DXGI_FORMAT_R32_TYPELESS;
    DescDST.SampleDesc.Count = 1;
    DescDST.SampleDesc.Quality = 0;
    DescDST.Usage = D3D10_USAGE_DEFAULT;
    DescDST.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    DescDST.CPUAccessFlags = 0;
    DescDST.MiscFlags = 0;
    V( pd3dDevice->CreateTexture2D( &DescDST, NULL, &DepthStencilTex ) );

    // Create the depth stencil view
    SAFE_RELEASE( DepthStencilRTV );
    D3D10_DEPTH_STENCIL_VIEW_DESC DescDSV;
    DescDSV.Format = DXGI_FORMAT_D32_FLOAT;
    DescDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
    DescDSV.Texture2D.MipSlice = 0;
    V( pd3dDevice->CreateDepthStencilView( DepthStencilTex, &DescDSV, &DepthStencilRTV ) );



    SAFE_RELEASE( ImageTex );
    D3D10_TEXTURE2D_DESC DescRTT = 
    {
        pBackBufferSurfaceDesc->Width,
        pBackBufferSurfaceDesc->Height,
        1,//mip
        1,//array
        DXGI_FORMAT_R8G8B8A8_UNORM,
        {1,0}, //sampler Count,Quality
        D3D10_USAGE_DEFAULT,
        D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE, 
        0, //CPUAccessFlags
        0, //MiscFlags
    };
    V( pd3dDevice->CreateTexture2D( &DescRTT, NULL, &ImageTex) );

    SAFE_RELEASE(ImageRTV)
    D3D10_RENDER_TARGET_VIEW_DESC DescRTV;
    DescRTV.Format = DescRTT.Format;
    DescRTV.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
    DescRTV.Texture2D.MipSlice = 0;
    V( pd3dDevice->CreateRenderTargetView( ImageTex, &DescRTV, &ImageRTV ) );


    SAFE_RELEASE(ImageSRV)
    D3D10_SHADER_RESOURCE_VIEW_DESC DescSRV;
    DescSRV.Format = DescRTT.Format;
    DescSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    DescSRV.Texture2D.MipLevels = 1;
    DescSRV.Texture2D.MostDetailedMip = 0;
    V( pd3dDevice->CreateShaderResourceView( ImageTex, &DescSRV, &ImageSRV ) );
    
    
    V( D3DX10CreateFont( pd3dDevice, 12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &pFont ) );

    V( D3DX10CreateSprite( pd3dDevice, 256, &pSprite ) );


    return hr;
}

void    Render10::OnD3D10ReleasingSwapChain()
{
    if(m_pInitialState)
        m_pInitialState->Apply();

    SAFE_RELEASE( pFont );
    SAFE_RELEASE( pSprite );

    SAFE_RELEASE( DepthStencilTex );
    SAFE_RELEASE( DepthStencilRTV );
    SAFE_RELEASE( ImageTex );
    SAFE_RELEASE( ImageRTV );
    SAFE_RELEASE( ImageSRV );
}

void    Render10::OnD3D10DestroyDevice()
{
    if(m_pInitialState)
        m_pInitialState->Apply();
    SAFE_RELEASE(m_pInitialState);

    Clear();

    OnD3D10ReleasingSwapChain();

    SAFE_RELEASE(SolidMode);
    SAFE_RELEASE(WireMode);

    SAFE_RELEASE(FSQLayout);
    SAFE_RELEASE(FSQMesh);
    SAFE_RELEASE(FSQEffect);

    DXVGetShowShared10().Shutdown();

}


void Render10::OnFrameRender( double fTime, float fElapsedTime )
{
    HRESULT hr = S_OK;
    //OutputDebugStringW(L"Render\n");

    ID3D10Device* pDevice = DXUTGetD3D10Device();
    ID3D10RenderTargetView* pRTV = DXUTGetD3D10RenderTargetView();

    bool ManualRefreshMode = STATE_GET(ManualRefreshMode);
    bool ManualRefreshNeeded = STATE_GET(ManualRefreshNeeded);

    if(ManualRefreshMode)
    {
        pDevice->OMSetRenderTargets(1, &ImageRTV, DepthStencilRTV);
        if(ManualRefreshNeeded)
            pDevice->ClearRenderTargetView(ImageRTV, (FLOAT*)D3DXCOLOR(0.25f,0.25f,0.25f,0.0f) );
    }
    else
    {
        pDevice->OMSetRenderTargets(1, &pRTV, DepthStencilRTV);
        pDevice->ClearRenderTargetView(pRTV, (FLOAT*)D3DXCOLOR(0.25f,0.25f,0.25f,0.0f) );
    }


    if(!ManualRefreshMode || ManualRefreshNeeded)
    {
        pDevice->ClearDepthStencilView( DepthStencilRTV, D3D10_CLEAR_DEPTH, 1.0, 0 );

        if(DXVGetModel10().GetRoot())
        {
            UpdateRootBoundValueTable();

            V( OnFrameRender( *DXVGetModel10().GetRoot() ));

            if(STATE_GET(ShowBounds))
                DXVGetShowShared10().DrawBounds();
        }

        RenderText();
    }

    if(ManualRefreshMode)
    {
        pDevice->OMSetRenderTargets(1, &pRTV, DepthStencilRTV);
        pDevice->IASetInputLayout(FSQLayout);
        V( FSQImageVariable->SetResource( ImageSRV ) );
        V( FSQEffect->GetTechniqueByIndex(0)->GetPassByIndex(0)->Apply(0) );
        FSQMesh->DrawSubset(0);
    }

}

HRESULT Render10::OnFrameRender( Transform& frameIterator )
{

    HRESULT hr = S_OK;

    for(ModelSubset10* meshIterator = (ModelSubset10*)frameIterator.GetModelSubset();
        meshIterator != NULL;
        meshIterator = (ModelSubset10*)meshIterator->GetNextModelSubset() )
    {
        hr = OnFrameRender( *meshIterator, frameIterator.WorldMatrix );
        if(FAILED(hr)) return hr;
    }

    for( Transform* itr = frameIterator.GetFirstChild();
        itr != NULL;
        itr = itr->GetNextSibling() )
    {
        hr = OnFrameRender( *itr );
        if(FAILED(hr)) return hr;
    }

    return hr;
}

HRESULT Render10::OnFrameRender( ModelSubset10& meshIterator, D3DXMATRIX& WorldMatrix )
{
    HRESULT hr = S_OK;
    ID3D10RasterizerState* pOldRS = NULL;
    ID3D10Device* pDevice = DXUTGetD3D10Device();




    UpdateBoundValueTable( meshIterator.pBoundValueTable,  WorldMatrix);

    UINT AttribCount = 0;
    meshIterator.pMesh10->GetAttributeTable(NULL, &AttribCount);
    UINT MaterialMax = (UINT)meshIterator.Materials.size();
    for( UINT iMaterial = 0; iMaterial < MaterialMax; iMaterial++ )
    {
        Material10* pMaterial = (Material10*) meshIterator.Materials[iMaterial];
        if(!pMaterial || !pMaterial->Effect)
            continue;

        ID3D10Effect* pEffect = ((Sas::Effect10*)pMaterial->Effect)->GetD3DEffect();
        if(!pEffect)
            continue;


        UINT iTec = STATE_GET( TechniqueSelected );

        ID3D10EffectTechnique* technique = pEffect->GetTechniqueByIndex( iTec ); 

        if(!technique->IsValid())
            continue;
    
        UINT numPasses = (UINT)pMaterial->Techniques[iTec].Passes.size();
        if( STATE_GET( PassMode ) == DXVPASSMODE_UPTO)
        {
            numPasses = __min( (STATE_GET( PassSelected )+1), ((int)numPasses));
        }

        for( UINT iPass = 0; iPass < numPasses; iPass++ )
        {
            ID3D10EffectPass* pass = technique->GetPassByIndex(iPass);
            if(!pass->IsValid())
                continue;

            pass->Apply(0);

            if( STATE_GET( FillMode )  == DXVFILLMODE_WIRE )
            {
                DXUTGetD3D10Device()->RSSetState(WireMode);
            }
            else if( STATE_GET( FillMode )  == DXVFILLMODE_SOLID )
            {
                DXUTGetD3D10Device()->RSSetState(SolidMode) ;
            }

            pDevice->IASetInputLayout( pMaterial->Techniques[iTec].Passes[iPass].Layout );
            hr = meshIterator.pMesh10->DrawSubset(iMaterial);
            if( FAILED(hr) )
            {
                DXVGetApp().OutputW( L"DrawSubset failed. HRESULT = %s", DXGetErrorString(hr) );
                hr = S_OK;
            }
            pDevice->IASetInputLayout(NULL);


        }

        if( STATE_GET( FillMode ) != DXVFILLMODE_AUTO)
        {
            DXUTGetD3D10Device()->RSSetState(pOldRS);
        }
    }

    SAFE_RELEASE( pOldRS );


    if(STATE_GET(ShowAdjacencies))
        DXVGetShowShared10().DrawAdjacencies(WorldMatrix, meshIterator.pMesh10, (UINT)meshIterator.Materials.size(), meshIterator.ShowLayout );

    if(STATE_GET(ShowCreases))
        DXVGetShowShared10().DrawCreases(WorldMatrix, meshIterator.pMesh10, (UINT)meshIterator.Materials.size(), meshIterator.ShowLayout );

    if(STATE_GET(ShowEdges))
        DXVGetShowShared10().DrawEdges(WorldMatrix, meshIterator.pMesh10, (UINT)meshIterator.Materials.size(), meshIterator.ShowLayout );

    if(STATE_GET(ShowNormals))
        DXVGetShowShared10().DrawNormals(WorldMatrix, meshIterator.pMesh10, (UINT)meshIterator.Materials.size(), meshIterator.ShowLayout );

    if(STATE_GET(ShowBinormals))
        DXVGetShowShared10().DrawBinormals(WorldMatrix, meshIterator.pMesh10, (UINT)meshIterator.Materials.size(), meshIterator.ShowLayout );

    if(STATE_GET(ShowTangents))
        DXVGetShowShared10().DrawTangents(WorldMatrix, meshIterator.pMesh10, (UINT)meshIterator.Materials.size(), meshIterator.ShowLayout );

    if(STATE_GET(ShowPoints))
        DXVGetShowShared10().DrawPoints(WorldMatrix, meshIterator.pMesh10, (UINT)meshIterator.Materials.size(), meshIterator.ShowLayout );

    return hr;
}

void Render10::OnFrameMove( double fTime, float fElapsedTime )
{
    if(DXVGetModel10().GetRoot())
    {
        DXVGetModel10().FrameMove(fElapsedTime);
        DXVGetShowShared10().Move();
    }

}

void Render10::Load()
{
    EffectInfo.clear();
    DXVGetModel10().Load();
}

void Render10::Clear()
{
    DXVGetModel10().Clear();

    Sas::Host::GetRootTable()->Clear();
    DXVGetFileWatcher().Clear();
}

float Render10::GetModelRadius() { return DXVGetModel10().GetRadius(); }
const D3DXVECTOR3& Render10::GetModelCenter() { return *DXVGetModel10().GetCenter(); }
const D3DXVECTOR3& Render10::GetMin() { return *DXVGetModel10().GetMin(); }
const D3DXVECTOR3& Render10::GetMax() { return *DXVGetModel10().GetMax(); }


//--------------------------------------------------------------------------------------
void Render10::RenderText()
{
    // The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    // If NULL is passed in as the sprite object, then it will work however the 
    // pFont->DrawText() will not be batched together.  Batching calls will improves performance.
    //const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetBackBufferSurfaceDesc();
    if( STATE_GET( ShowStatistics ) )
    {    
        CDXUTTextHelper txtHelper( pFont, pSprite, 15 );

        // Output statistics
        txtHelper.Begin();
        txtHelper.SetInsertionPos( 5, 5 );
        txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
        
        txtHelper.DrawTextLine( DXUTGetFrameStats() );
        txtHelper.DrawTextLine( DXUTGetDeviceStats() );
        txtHelper.DrawFormattedTextLine( L"FPS: %f", DXUTGetFPS() );

        

        txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
        txtHelper.End();
    }
}
