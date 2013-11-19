#include "sas.h"
#include "DXUT.h"
#include "DXUTmisc.h"
#include "SDKmisc.h"//dxut
#include "windows.h"
#include "Render9.h"
#include "App.h"
#include "stateblock.h"
#include "camera.h"
#include "Model9.h"
#include "hud.h"
#include "FileSystem.h"

DECLARE_SINGLETON_CPP( Render9 );
    
DXUTDeviceVersion Render9::GetVersion() { return DXUT_D3D9_DEVICE; }

Render9::Render9()
{
    Image = NULL;
    pFont = NULL;
    pSprite = NULL;

}



void Render9::Startup()
{
    DXUTSetCallbackD3D9DeviceAcceptable( Render9::StaticIsD3D9DeviceAcceptable );
    
    DXVGetApp().OnD3D9DeviceCreated.AddFunc( Render9::StaticOnD3D9DeviceCreated );
    DXVGetApp().OnD3D9DeviceReset.AddFunc( Render9::StaticOnD3D9DeviceReset );
    DXVGetApp().OnD3D9DeviceLost.AddFunc( Render9::StaticOnD3D9DeviceLost );
    DXVGetApp().OnD3D9DeviceDestroyed.AddFunc( Render9::StaticOnD3D9DeviceDestroyed);
}

void Render9::Shutdown()
{
    SAFE_RELEASE(Image);
}

bool    Render9::IsD3D9DeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed )
{
    return true;
}


bool    Render9::OnModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings )
{
    const D3DCAPS9* pCaps= DXUTGetD3D9DeviceCaps();

    HWND hwnd = DXUTGetHWNDDeviceWindowed();
    if(pDeviceSettings->d3d9.pp.Windowed != FALSE)
    {
        SetParent(hwnd, STATE_GET(Dialog) );
        DXVGetApp().OnSize();
    }
    else
        SetParent(hwnd, NULL);

    if( STATE_GET(DebugPixelShaders) )
        pDeviceSettings->d3d9.DeviceType =  D3DDEVTYPE_REF;

    // if debug or device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW then switch to SWVP.
    if( STATE_GET(DebugVertexShaders) ||
        (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
        pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
    {
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    pDeviceSettings->d3d9.pp.SwapEffect = D3DSWAPEFFECT_COPY;//required for single manual refresh
    pDeviceSettings->d3d9.pp.BackBufferCount = 1; //required for D3DSWAPEFFECT_COPY

    return true;
}

HRESULT Render9::OnD3D9DeviceCreated( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
    return S_OK;
}


HRESULT Render9::OnD3D9DeviceReset( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
    HRESULT hr = S_OK;

    SAFE_RELEASE(Image);
    V_RETURN( pd3dDevice->CreateRenderTarget(
                                            pBackBufferSurfaceDesc->Width,
                                            pBackBufferSurfaceDesc->Height,
                                            pBackBufferSurfaceDesc->Format,
                                            pBackBufferSurfaceDesc->MultiSampleType,
                                            pBackBufferSurfaceDesc->MultiSampleQuality,
                                            false,
                                            &Image,
                                            NULL));

    DXVGetCamera().Resize();

    DXVGetModel9().OnD3D9DeviceReset(pd3dDevice, pBackBufferSurfaceDesc);

    V( D3DXCreateFont( pd3dDevice, 12, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &pFont ) );

    V( D3DXCreateSprite( pd3dDevice, &pSprite ) );

    Load();

    return hr;
}


void Render9::OnD3D9DeviceLost()
{
    Clear();

    SAFE_RELEASE(pFont);
    SAFE_RELEASE(pSprite);
    SAFE_RELEASE(Image);
    DXVGetModel9().OnD3D9DeviceLost();
}

void Render9::OnD3D9DeviceDestroyed()
{
    DXVGetModel9().OnD3D9DeviceDestroyed();
}


void Render9::OnFrameRender( double fTime, float fElapsedTime )
{
    HRESULT hr = S_OK;
    //OutputDebugStringW(L"Render\n");

    LPDIRECT3DDEVICE9 pDevice = DXUTGetD3D9Device();
    LPDIRECT3DSURFACE9 pBackBuffer = NULL;


    bool ManualRefreshMode = STATE_GET(ManualRefreshMode);
    bool ManualRefreshNeeded = STATE_GET(ManualRefreshNeeded);

    V( pDevice->GetRenderTarget(0, &pBackBuffer));
    if(ManualRefreshMode)
    {
        V( pDevice->SetRenderTarget(0, Image));
    }
    else
    {
        V( pDevice->SetRenderTarget(0, pBackBuffer));
    }

    if(!ManualRefreshMode || ManualRefreshNeeded)
    {
        V( pDevice->Clear(0, NULL,  D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 64, 64, 64), 1.0f, 0 ) );

        V( pDevice->BeginScene() );
        if(DXVGetModel9().GetRoot())
        {
            UpdateRootBoundValueTable();

            V( OnFrameRender( *DXVGetModel9().GetRoot() ));

            if(STATE_GET(ShowBounds) )
                DXVGetModel9().ShowBounds.Draw();
        }

        RenderText();

        V( pDevice->EndScene() ); 


    }
    
    if(ManualRefreshMode)
    {
        V( D3DXLoadSurfaceFromSurface(pBackBuffer, NULL, NULL, Image, NULL, NULL, D3DX_FILTER_NONE, 0) );
    }
    V( pDevice->SetRenderTarget(0, pBackBuffer));

    SAFE_RELEASE(pBackBuffer);
}

HRESULT Render9::OnFrameRender( Transform& frameIterator )
{

    HRESULT hr = S_OK;

    for(ModelSubset9* meshIterator = (ModelSubset9*)frameIterator.GetModelSubset();
        meshIterator != NULL;
        meshIterator = (ModelSubset9*)meshIterator->GetNextModelSubset() )
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


HRESULT Render9::OnFrameRender( ModelSubset9& meshIterator, D3DXMATRIX& WorldMatrix )
{
    HRESULT hr = S_OK;

    UpdateBoundValueTable( meshIterator.pBoundValueTable,  WorldMatrix);

    DWORD AttribCount = 0;
    meshIterator.AsMeshContainer()->MeshData.pMesh->GetAttributeTable(NULL, &AttribCount);
    UINT MaterialMax = (UINT)meshIterator.Materials.size();
    for( UINT iMaterial = 0; iMaterial < MaterialMax; iMaterial++ )
    {
        Material* pMaterial = meshIterator.Materials[iMaterial];
        if(!pMaterial || !pMaterial->Effect )
            continue;

        ID3DXEffect* pEffect = ((Sas::Effect9*)pMaterial->Effect)->GetD3DXEffect();
        if(!pEffect )
            continue;

        DWORD oldFillMode;
        if( STATE_GET( FillMode ) != DXVFILLMODE_AUTO)
        {
            V( DXUTGetD3D9Device()->GetRenderState(D3DRS_FILLMODE, &oldFillMode) );
        }
   
        UINT numPasses;
        V( pEffect->Begin( &numPasses, 0 ) );//RESTORE STATE

        if( STATE_GET( PassMode ) == DXVPASSMODE_UPTO)
        {
            numPasses = __min( (STATE_GET( PassSelected )+1), ((int)numPasses));
        }

        for( UINT iPass = 0; iPass < numPasses; iPass++ )
        {
            V( pEffect->BeginPass( iPass ) );

            if( STATE_GET( FillMode )  == DXVFILLMODE_WIRE )
            {
                 V( DXUTGetD3D9Device()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME) );
            }
            else if( STATE_GET( FillMode )  == DXVFILLMODE_SOLID )
            {
                 V( DXUTGetD3D9Device()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID) );
            }

            hr = meshIterator.AsMeshContainer()->MeshData.pMesh->DrawSubset(iMaterial);
            if( FAILED(hr) )
            {
                DXVGetApp().OutputW( L"DrawSubset failed. HRESULT = %s", DXGetErrorString(hr) );
                hr = S_OK;
            }

            V( pEffect->EndPass() );
        }

        V( pEffect->End() );

        if( STATE_GET( FillMode ) != DXVFILLMODE_AUTO)
        {
            V( DXUTGetD3D9Device()->SetRenderState(D3DRS_FILLMODE, oldFillMode) );
        }
    }

    if( STATE_GET(ShowNormals) )
    {
        meshIterator.ShowNormals.Draw(D3DDECLUSAGE_NORMAL, WorldMatrix);
    }

    if( STATE_GET(ShowBinormals) )
    {
        meshIterator.ShowNormals.Draw(D3DDECLUSAGE_BINORMAL, WorldMatrix);
    }

    if( STATE_GET(ShowTangents) )
    {
        meshIterator.ShowNormals.Draw(D3DDECLUSAGE_TANGENT, WorldMatrix);
    }

    if( STATE_GET(ShowPoints) )
    {
        meshIterator.ShowPoints.Draw(WorldMatrix);
    }

    if( STATE_GET(ShowEdges) )
    {
        meshIterator.ShowEdges.Draw(WorldMatrix);
    }

    if( STATE_GET(ShowCreases) )
    {
        meshIterator.ShowCreases.Draw(WorldMatrix);
    }

    if( STATE_GET(ShowAdjacencies) )
    {
        meshIterator.ShowAdjacencies.Draw(WorldMatrix);
    }
    return hr;
}

void Render9::OnFrameMove( double fTime, float fElapsedTime )
{
    //OutputDebugStringW(L"Move");
    DXVGetModel9().FrameMove(fElapsedTime);
}

void Render9::Load()
{
    EffectInfo.clear();
    DXVGetModel9().Load();
}



void Render9::Clear()
{
    DXVGetModel9().Clear();

    Sas::Host::GetRootTable()->Clear();
    DXVGetFileWatcher().Clear();
}

float Render9::GetModelRadius() { return DXVGetModel9().GetRadius(); }
const D3DXVECTOR3& Render9::GetModelCenter() { return *DXVGetModel9().GetCenter(); }
const D3DXVECTOR3& Render9::GetMin() { return *DXVGetModel9().GetMin(); }
const D3DXVECTOR3& Render9::GetMax() { return *DXVGetModel9().GetMax(); }



//--------------------------------------------------------------------------------------
void Render9::RenderText()
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
