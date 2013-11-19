#include "pch.h"

//-------------------------------------------------------------------------------------
UINT __stdcall CMayaEngine::StaticRunThread( void* pParam )
{
    CMayaEngine* pDevice = (CMayaEngine*) pParam;
    return pDevice->RunThread();
}


//-------------------------------------------------------------------------------------
UINT __stdcall CMayaEngine::RunThread()
{
    EnterCriticalSection(&DeviceAndViewerSection);
          
    HRESULT hr;

    IDirect3D9* pD3D = DXUTGetD3DObject();
    if( pD3D == NULL )
    {
        V( DXUTInit( false, false, false ) );
		if(FAILED(hr))
			goto LCleanReturn;


        pD3D = DXUTGetD3DObject();
    }

    // Create a device window
    m_hwndFocus = CreateWindow( L"STATIC", L"D3D Device Window", WS_POPUP, 0, 0, 1, 1, NULL, NULL, NULL, NULL );

	DXUTSetWindow( m_hwndFocus, m_hwndFocus, m_hwndFocus, false );


    // Enumerate for each adapter all of the supported display modes, 
    // device types, adapter formats, back buffer formats, window/full screen support, 
    // depth stencil formats, multisampling types/qualities, and presentations intervals.
    //
    // For each combination of device type (HAL/REF), adapter format, back buffer format, and
    // IsWindowed it will call the app's ConfirmDevice callback.  This allows the app
    // to reject or allow that combination based on its caps/etc.  It also allows the 
    // app to change the BehaviorFlags.  The BehaviorFlags defaults non-pure HWVP 
    // if supported otherwise it will default to SWVP, however the app can change this 
    // through the ConfirmDevice callback.
    CD3DEnumeration* pd3dEnum; 
    pd3dEnum = DXUTGetEnumeration();
    V( pd3dEnum->Enumerate( pD3D, IsDeviceAcceptable ) );
    if( FAILED(hr) )
        goto LCleanReturn;
    
    DXUTMatchOptions matchOptions;
    matchOptions.eAdapterOrdinal     = DXUTMT_PRESERVE_INPUT;
    matchOptions.eDeviceType         = DXUTMT_IGNORE_INPUT;
    matchOptions.eWindowed           = DXUTMT_PRESERVE_INPUT;
    matchOptions.eAdapterFormat      = DXUTMT_IGNORE_INPUT;
    matchOptions.eVertexProcessing   = DXUTMT_IGNORE_INPUT;
    matchOptions.eResolution         = DXUTMT_CLOSEST_TO_INPUT;
    matchOptions.eBackBufferFormat   = DXUTMT_IGNORE_INPUT;
    matchOptions.eBackBufferCount    = DXUTMT_IGNORE_INPUT;
    matchOptions.eMultiSample        = DXUTMT_IGNORE_INPUT;
    matchOptions.eSwapEffect         = DXUTMT_IGNORE_INPUT;
    matchOptions.eDepthFormat        = DXUTMT_IGNORE_INPUT;
    matchOptions.eStencilFormat      = DXUTMT_IGNORE_INPUT;
    matchOptions.ePresentFlags       = DXUTMT_IGNORE_INPUT;
    matchOptions.eRefreshRate        = DXUTMT_IGNORE_INPUT;
    matchOptions.ePresentInterval    = DXUTMT_IGNORE_INPUT;

    DXUTDeviceSettings deviceSettings;
    ZeroMemory( &deviceSettings, sizeof(DXUTDeviceSettings) );
    deviceSettings.AdapterOrdinal      = D3DADAPTER_DEFAULT;
    deviceSettings.pp.Windowed         = true;
    deviceSettings.pp.EnableAutoDepthStencil = false;
    
    deviceSettings.pp.BackBufferWidth  = 1;
    deviceSettings.pp.BackBufferHeight = 1;
    
    hr = DXUTFindValidDeviceSettings( &deviceSettings, &deviceSettings, &matchOptions );
    if( FAILED(hr) ) // the call will fail if no valid devices were found
    {
        DXUT_ERR( L"DXUTFindValidDeviceSettings", hr );
        goto LCleanReturn;
    }

    D3DCAPS9 caps;
    pD3D->GetDeviceCaps( deviceSettings.AdapterOrdinal, deviceSettings.DeviceType, &caps );
    ModifyDeviceSettings( &deviceSettings, &caps );

    hr = pD3D->CreateDevice( deviceSettings.AdapterOrdinal, deviceSettings.DeviceType, 
                             m_hwndFocus, deviceSettings.BehaviorFlags,
                             &deviceSettings.pp, &m_pd3dDevice );
    if( FAILED(hr) )
    {
        DXUT_ERR( L"CreateDevice", hr );
        goto LCleanReturn;
    }



    LeaveCriticalSection(&DeviceAndViewerSection);
    SetEvent(DeviceCreatedEvent);

    m_pPreviewPipeline->TriggerDeviceEvents.OnD3DDeviceCreate();
    m_pPreviewPipeline->TriggerDeviceEvents.OnD3DDeviceReset();

	bool bStillNoReset= false;
    while( m_bDestroyCalled == false )
    {
		hr= m_pd3dDevice->TestCooperativeLevel();

		bool bNoReset = (D3D_OK != hr);
	
		bool bTriggerLostEvent= false;

		if(bNoReset)
		{
			if(bStillNoReset)
				m_pPreviewPipeline->TriggerDeviceEvents.OnD3DDeviceLost();

			CManagerLock Lock;
			m_pPreviewPipeline->SceneWriteLock(true, Lock);
			{
				V( hr= m_pd3dDevice->Reset( &deviceSettings.pp ) );

				bStillNoReset= ( FAILED(hr) && D3DERR_DEVICELOST != hr );
			}
			m_pPreviewPipeline->SceneWriteUnlock(Lock);

			if(!bStillNoReset)
				m_pPreviewPipeline->TriggerDeviceEvents.OnD3DDeviceReset();
		}

        if(m_bDestroyCalled == false)
            Sleep( 200 );
    }

    hr = S_OK;

LCleanReturn:
	if(FAILED(hr))
	{
		LeaveCriticalSection(&DeviceAndViewerSection);
		SetEvent(DeviceCreatedEvent);
	}
    m_pPreviewPipeline->TriggerDeviceEvents.OnD3DDeviceDestroy();
    SAFE_RELEASE( m_pd3dDevice );
    DXUTShutdown();

    return hr;
}


//-------------------------------------------------------------------------------------
HRESULT CMayaEngine::Create( CManager* pPreviewPipeline )
{
    // Start clean
    Destroy();
    m_bDestroyCalled = false;

    m_pPreviewPipeline = pPreviewPipeline;

    // Launch the thread which will create the device and check for device state changes
    m_hThread = _beginthreadex( NULL, 0, StaticRunThread, this, 0, &m_nThreadID );
    if( m_hThread == NULL )
        return E_FAIL;

    GetSingleObject(true, DeviceCreatedEvent);

    return S_OK;
}


//-------------------------------------------------------------------------------------
HRESULT CMayaEngine::Destroy()
{
    m_bDestroyCalled = true;

    WaitForSingleObject( (HANDLE)m_hThread, INFINITE );
    TerminateThread((HANDLE)m_hThread, 0);
    CloseHandle( (HANDLE)m_hThread );

    DestroyWindow( m_hwndFocus );
    m_hwndFocus = NULL;
    m_hThread = 0;
    m_nThreadID = 0;



    m_bDestroyCalled = false;



    return S_OK;
}


//-------------------------------------------------------------------------------------
HRESULT CMayaEngine::GetD3DObject( IDirect3D9** ppObject )
{
    IDirect3D9* pd3d = DXUTGetD3DObject();
    if( pd3d == NULL )
        return E_FAIL;

    pd3d->AddRef();
    *ppObject = pd3d;

    return S_OK;
}


//-------------------------------------------------------------------------------------
HRESULT CMayaEngine::GetD3DDevice( IDirect3DDevice9** ppDevice )
{
    if( m_pd3dDevice == NULL )
        return E_FAIL;

    m_pd3dDevice->AddRef();
    *ppDevice = m_pd3dDevice;

    return S_OK;
}


//-------------------------------------------------------------------------------------
bool CALLBACK CMayaEngine::IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // No fallback defined by this app, so reject any device that 
    // doesn't support at least ps1.1
    if( pCaps->PixelShaderVersion < D3DPS_VERSION(1,1) )
        return false;

    return true;
}


//-------------------------------------------------------------------------------------
void CALLBACK CMayaEngine::ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps )
{
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
}



