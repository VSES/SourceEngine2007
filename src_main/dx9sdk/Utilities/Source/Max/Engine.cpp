#include "StdAfx.h"
#include "MaxPipeline.h"
#include "Engine.h"
#include "PipelineLock.h"

UINT __stdcall CEngine::StaticRunThread(void* param)
{
    CEngine* engine = (CEngine*)param;
    return engine->RunThread();
}

UINT __stdcall CEngine::RunThread()
{
    EnterCriticalSection(&DeviceAndViewerSection);
          
    HRESULT hr;

    D3DPRESENT_PARAMETERS pp;
    ZeroMemory(&pp, sizeof(D3DPRESENT_PARAMETERS)); 

    D3d.Set(Direct3DCreate9(D3D_SDK_VERSION));
    if (!D3d.IsNull())
    {
        D3DDISPLAYMODE Mode;

        D3d->GetAdapterDisplayMode(0, &Mode);

        pp.BackBufferWidth  = 1;
        pp.BackBufferHeight = 1;
        pp.BackBufferFormat = Mode.Format;
        pp.BackBufferCount  = 1;
        pp.SwapEffect       = D3DSWAPEFFECT_COPY;
        pp.Windowed         = TRUE;

        // Create a device window
        FocusWindow = CreateWindow(TEXT("STATIC"), TEXT("D3D Device Window"), WS_POPUP, 0, 0, 1, 1, NULL, NULL, NULL, NULL);

        if (SUCCEEDED(hr = D3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, FocusWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING|D3DCREATE_MULTITHREADED, &pp, D3dDevice)))
        {
        }
    }

    LeaveCriticalSection(&DeviceAndViewerSection);
    SetEvent(DeviceCreatedEvent);

    while (SUCCEEDED(hr) && !DestroyCalled)
    {
		bool TriggerLostEvent = false;
		CPipelineLock lock;

        if (Pipeline->SceneWriteLock(false, lock))
        {
            // Test the cooperative level to see if it's okay to render
            if (D3DERR_DEVICENOTRESET == D3dDevice->TestCooperativeLevel())
            {
				TriggerLostEvent = true;
			}

            Pipeline->SceneWriteUnlock(lock);
		}

		if (TriggerLostEvent)
		{
		    Pipeline->TriggerDeviceEvents.OnD3DDeviceLost();

			Pipeline->SceneWriteLock(true, lock);
			{
				D3dDevice->Reset(&pp);
				if (FAILED(hr) && (D3DERR_DEVICELOST != hr))
				{
                    // TODO: handle this, exit thread?
				}
			}
			Pipeline->SceneWriteUnlock(lock);

			Pipeline->TriggerDeviceEvents.OnD3DDeviceReset();
		}

        if (!DestroyCalled)
            Sleep(200);
    }

    Pipeline->TriggerDeviceEvents.OnD3DDeviceDestroy();
    D3dDevice.Reset();

    return hr;
}

CEngine::CEngine()
{
    DestroyCalled = false;
    ThreadId = 0;
    Thread = NULL;
	FocusWindow = NULL;

    Pipeline = NULL;
}

HRESULT CEngine::Create( CPipeline* pipeline)
{
    // Start clean
    Destroy();
    DestroyCalled = false;

    Pipeline = pipeline;

    // Launch the thread which will create the device and check for device state changes
    Thread = _beginthreadex( NULL, 0, StaticRunThread, this, 0, &ThreadId );
    if( Thread == NULL )
        return E_FAIL;

    GetSingleObject(true, DeviceCreatedEvent);

    return S_OK;
}


HRESULT CEngine::Destroy()
{
    DestroyCalled = true;

    WaitForSingleObject( (HANDLE)Thread, INFINITE );
    TerminateThread((HANDLE)Thread, 0);
    CloseHandle( (HANDLE)Thread );

    if (FocusWindow)
    {
        DestroyWindow(FocusWindow);
        FocusWindow = NULL;
    }

    Thread = 0;
    ThreadId = 0;

    DestroyCalled = false;

    return S_OK;
}


HRESULT CEngine::GetD3DObject( IDirect3D9** ppObject )
{
#if 0
    IDirect3D9* pd3d = DXUTGetD3DObject();
    if( pd3d == NULL )
        return E_FAIL;

    pd3d->AddRef();
    *ppObject = pd3d;
#endif
    return S_OK;
}


HRESULT CEngine::GetD3DDevice( IDirect3DDevice9** ppDevice )
{
    if (D3dDevice.IsNull())
        return E_FAIL;

    D3dDevice->AddRef();
    *ppDevice = D3dDevice;

    return S_OK;
}


bool CALLBACK CEngine::IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed )
{
    // No fallback defined by this app, so reject any device that 
    // doesn't support at least ps1.1
    if( pCaps->PixelShaderVersion < D3DPS_VERSION(1,1) )
        return false;

    return true;
}


void CALLBACK CEngine::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps )
{
#if 0
    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
         pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
    {
        pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
    else
    {
        pDeviceSettings->BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }

    // This application is designed to work on a pure device by not using 
    // IDirect3D9::Get*() methods, so create a pure device if supported and using HWVP.
    if ((pCaps->DevCaps & D3DDEVCAPS_PUREDEVICE) != 0 && 
        (pDeviceSettings->BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) != 0 )
        pDeviceSettings->BehaviorFlags |= D3DCREATE_PUREDEVICE;

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;                            
        pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif

    // Force multithreaded
    pDeviceSettings->BehaviorFlags |= D3DCREATE_MULTITHREADED;
#endif
}



