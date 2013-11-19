//--------------------------------------------------------------------------------------
// File: Viewer.cpp
//
// Creates a window for viewing a standard semantics and annotations mesh 
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------
#include "pch.h"


char g_strDefaultEffect[] = 
{
    "//\n"
    "// Default effect\n"
    "// Single texture\n"
    "// Copyright (c) 2000 Microsoft Corporation. All rights reserved.\n"
    "//\n"
    "\n"
	"bool UseTexture0 = false;\n"
    "texture Texture0 < string Name = \"\"; >;\n"
    "\n"
    "float4 Ambient\n"
    "<\n"
    "> = float4( 0.4f, 0.4f, 0.4f, 1.0f );\n"
    "\n"
    "float4 Diffuse\n"
    "<\n"
    "> = float4( 0.6f, 0.6f, 0.6f, 1.0f );\n"
    "\n"
    "float4 Specular\n"
    "<\n"
    "> = float4( 1.0f, 1.0f, 1.0f, 1.0f );\n"
    "\n"
    "float4 Emissive\n"
    "<\n"
    "> = float4( 0.0f, 0.0f, 0.0f, 1.0f );\n"
    "\n"
    "float  Power\n"
    "<\n"
    "> = 4;\n"
    "\n"
    "float4 g_vLightColor\n"
    "<\n"
	"string SasBindAddress = \"Sas.DirectionalLight[0].Color\";\n"
    "> = float4( 1.0f, 1.0f, 1.0f, 1.0f );\n"
    "\n"
    "float3 g_vLightDirection\n"
    "<\n"
	"string SasBindAddress = \"Sas.DirectionalLight[0].Direction\";\n"
    "> = float3( 0.5f, 0.75f, 1.0f );\n"
    "\n"
    "float4x4 g_mWorld\n"
	"<\n"
	"string SasBindAddress = \"Sas.Skeleton.MeshToJointToWorld[0]\";\n"
	">;\n"
    "float4x4 g_mView\n"
	"<\n"
	"string SasBindAddress = \"Sas.Camera.WorldToView\";\n"
	">;\n"
    "float4x4 g_mProjection\n"
	"<\n"
	"string SasBindAddress = \"Sas.Camera.Projection\";\n"
	">;\n"
    "//float4x4 g_mWorldViewProjection; // World * View * Projection matrix\n"
    "\n"
    "\n"
    "\n"
    "//--------------------------------------------------------------------------------------\n"
    "// Texture samplers\n"
    "//--------------------------------------------------------------------------------------\n"
    "sampler MeshTextureSampler =\n"
    "sampler_state\n"
    "{\n"
    "Texture = <Texture0>;\n"
    "MipFilter = LINEAR;\n"
    "MinFilter = LINEAR;\n"
    "MagFilter = LINEAR;\n"
    "};\n"
    "\n"
    "\n"
    "//--------------------------------------------------------------------------------------\n"
    "// Name: VS\n"
    "// Type: Vertex Shader\n"
    "// Desc: Projection transform\n"
    "//--------------------------------------------------------------------------------------\n"
    "void VS( float4 vPosObj: POSITION,\n"
    "float3 vNormalObj: NORMAL,\n"
    "float2 vTexCoordIn: TEXCOORD0,\n"
    "out float4 vPosProj: POSITION,\n"
    "out float4 vColorOut: COLOR0,\n"
    "out float2 vTexCoordOut: TEXCOORD0\n"
    ")\n"
    "{\n"
    "// Transform the position into projected space for display and world space for lighting\n"
    "//vPosProj = mul( vPosObj, g_mWorldViewProjection );\n"
    "vPosProj = mul( vPosObj, g_mWorld );\n"
    "vPosProj = mul( vPosProj, g_mView );\n"
    "vPosProj = mul( vPosProj, g_mProjection );\n"
    "\n"
    "\n"
	//"vColorOut = float4(1.0, 1.0, 1.0, 1.0);\n"
    "// Transform the normal into world space for lighting\n"
    "float3 vNormalWorld = mul( vNormalObj, (float3x3)g_mWorld );\n"
    "\n"
    "// Compute ambient and diffuse lighting\n"
    "vColorOut = g_vLightColor * Ambient;\n"
    "vColorOut += g_vLightColor * Diffuse * saturate( dot( -g_vLightDirection, vNormalWorld ) );\n"
    "\n"
    "// Pass the texture coordinate\n"
    "vTexCoordOut = vTexCoordIn;\n"
    "}\n"
    "\n"
    "\n"
    "\n"
    "//--------------------------------------------------------------------------------------\n"
    "// Name: PS\n"
    "// Type: Pixel Shader\n"
    "// Desc: Modulate the texture by the vertex color\n"
    "//--------------------------------------------------------------------------------------\n"
    "void PS( float4 vColorIn: COLOR0,\n"
    "float2 vTexCoord: TEXCOORD0,\n"
    "out float4 vColorOut: COLOR0 )\n"
    "{\n"
    "// Sample and modulate the texture\n"
	"if(UseTexture0){ vColorIn *= tex2D( MeshTextureSampler, vTexCoord ); }\n"
    "vColorOut = vColorIn;\n"
    "}\n"
    "\n"
    "\n"
    "//--------------------------------------------------------------------------------------\n"
    "// Techniques\n"
    "//--------------------------------------------------------------------------------------\n"
    "technique T0\n"
    "{\n"
    "pass P0\n"
    "{\n"
    "VertexShader = compile vs_1_1 VS();\n"
    "PixelShader = compile ps_1_1 PS();\n"
    "}\n"
    "}\n"
};

struct SasCamera
{
	union
	{
		struct Components
		{
			D3DMATRIX WorldToView;
			D3DMATRIX Projection;
			float NearFarClipping[2];
			D3DVECTOR Position;
		} components;

		float floats[sizeof(Components)/sizeof(float)];
	}Data;

	SasCamera(
			const D3DMATRIX& worldToView,
			const D3DMATRIX& projection,
			float nearVal,
			float farVal,
			const D3DVECTOR& position)
	{
		Data.components.WorldToView = worldToView;
		Data.components.Projection = projection;
		Data.components.NearFarClipping[0] = nearVal;
		Data.components.NearFarClipping[1] = farVal;
		Data.components.Position = position;
	};
};

struct SasAmbientLight
{
	union
	{
		struct Components
		{
			D3DVECTOR Color;
		} components;

		float floats[sizeof(Components)/sizeof(float)];
	}Data;

	SasAmbientLight(
			const D3DVECTOR& color)
	{
		Data.components.Color=color;
	};
};

struct SasPointLight
{
	union
	{
		struct Components
		{
			D3DVECTOR Color;
			D3DVECTOR Position;
			float Range;
		} components;

		float floats[sizeof(Components)/sizeof(float)];
	}Data;

	SasPointLight(
			const D3DVECTOR& color,
			const D3DVECTOR& position,
			float range)
	{
		Data.components.Color=color;
		Data.components.Position=position;
		Data.components.Range=range;
	};
};

struct SasDirectionLight
{
	union
	{
		struct Components
		{
			D3DVECTOR Color;
			D3DVECTOR Direction;
		} components;

		float floats[sizeof(Components)/sizeof(float)];
	}Data;

	SasDirectionLight(
			const D3DVECTOR& color,
			const D3DVECTOR& direction)
	{
		Data.components.Color=color;
		Data.components.Direction=direction;
	};
};

struct SasSpotLight
{
	union
	{
		struct Components
		{
			D3DVECTOR Color;
			D3DVECTOR Position;
			D3DVECTOR Direction;
			float Range;
			float Theta;
			float Phi;
		} components;

		float floats[sizeof(Components)/sizeof(float)];
	}Data;

	SasSpotLight(
			const D3DVECTOR& color,
			const D3DVECTOR& position,
			const D3DVECTOR& direction,
			float range,
			float theta,
			float phi)
	{
		Data.components.Color=color;
		Data.components.Position=position;
		Data.components.Direction=direction;
		Data.components.Range=range;
		Data.components.Theta=theta;
		Data.components.Phi=phi;
	};
};

void
CMayaViewer::GetOrAddEffectFromShader( DWORD AssignmentId, LPDXCCSHADERPROPERTIES shader, LPD3DXEFFECT* ppEffect)
{
	if(!shader)
	{
		*ppEffect= NULL;
		return;
	}

    POSITION pos= m_MaterialMap.Lookup(AssignmentId);
    if(pos == NULL)
    {
        LPDIRECT3DDEVICE9 pDevice= NULL;
        m_pPreviewPipeline->AccessEngine()->GetD3DDevice( &pDevice);

        LPD3DXEFFECT pEffect= NULL;
        LPDXCCRESOURCE pResource= NULL;
        LPD3DXBUFFER pErrors= NULL;
       
		if(!DXCC_SUCCEEDED( DXCCLoadEffectFromShaderProperties( shader, pDevice, &g_SasIncluder, m_pPreviewPipeline->AccessManager(), NULL, NULL, NULL, &pEffect, &pErrors)) )
		{
			MGlobal::displayWarning("DirectX Viewer could not load one or more FX files");
		}		

        pos = m_MaterialMap.SetAt(AssignmentId, MaterialToEffect() );
	    MaterialToEffect& MtoE= m_MaterialMap.GetValueAt(pos);
		MtoE.Effect = pEffect;
		MtoE.RecentlyUsed= true;

		if(pEffect)
			m_EffectMap.AddEffect( pEffect );

		DXCC_RELEASE(pDevice);
		DXCC_RELEASE(pEffect);
		DXCC_RELEASE(pResource);
		DXCC_RELEASE(pErrors);
    }

    if(pos)
    {
        MaterialToEffect& MtoE= m_MaterialMap.GetValueAt(pos);

        MtoE.RecentlyUsed= true;

		MtoE.Effect.CopyTo( ppEffect );
    }
}

void
CMayaViewer::GetOrAddEffectFromStdMtl( DWORD AssignmentId, LPD3DXMATERIAL pStdMtl, LPD3DXEFFECT* ppEffect)
{
	if(!pStdMtl)
	{
		*ppEffect= NULL;
		return;
	}

    POSITION pos= m_MaterialMap.Lookup(AssignmentId);
    if(pos == NULL)
    {
        LPDIRECT3DDEVICE9 pDevice= NULL;
        m_pPreviewPipeline->AccessEngine()->GetD3DDevice( &pDevice);

        LPD3DXEFFECT pEffect= NULL;
        //LPDXCCRESOURCE pResource= NULL;
        //LPD3DXBUFFER pErrors= NULL;
       
		if(DXCC_SUCCEEDED( m_pDefaultEffect->CloneEffect( pDevice, &pEffect ) ))
		{
			DXCC_CHECK_HRESULT( pEffect->SetVector( "Ambient" , (const D3DXVECTOR4*)(const void*)&pStdMtl->MatD3D.Ambient ) );
			DXCC_CHECK_HRESULT( pEffect->SetVector( "Diffuse" , (const D3DXVECTOR4*)(const void*)&pStdMtl->MatD3D.Diffuse ) );
			DXCC_CHECK_HRESULT( pEffect->SetVector( "Specular" , (const D3DXVECTOR4*)(const void*)&pStdMtl->MatD3D.Specular ) );
			DXCC_CHECK_HRESULT( pEffect->SetFloat( "Power" , pStdMtl->MatD3D.Power ) );
			DXCC_CHECK_HRESULT( pEffect->SetVector( "Emissive" , (const D3DXVECTOR4*)(const void*)&pStdMtl->MatD3D.Emissive ) );
			
			if(pStdMtl->pTextureFilename != NULL && pStdMtl->pTextureFilename[0] != '\0')
			{
				LPDIRECT3DBASETEXTURE9 pTexture= NULL;

				if(DXCC_SUCCEEDED( DXCCLoadTextureFromFile(pDevice, m_pPreviewPipeline->AccessManager(), pStdMtl->pTextureFilename, NULL, NULL, &pTexture) ))
				{
					DXCC_CHECK_HRESULT( pEffect->SetBool( "UseTexture0" , TRUE ) );
					DXCC_CHECK_HRESULT( pEffect->SetTexture( "Texture0" , pTexture ) );

					DXCC_RELEASE(pTexture);
				}
			}
 		}


        pos = m_MaterialMap.SetAt(AssignmentId, MaterialToEffect() );
	    MaterialToEffect& MtoE= m_MaterialMap.GetValueAt(pos);
		MtoE.Effect = pEffect;
		MtoE.RecentlyUsed= true;

		if(pEffect)
			m_EffectMap.AddEffect( pEffect );

		DXCC_RELEASE(pDevice);
		DXCC_RELEASE(pEffect);
		//DXCC_RELEASE(pResource);
		//DXCC_RELEASE(pErrors);
    }

    if(pos)
    {
        MaterialToEffect& MtoE= m_MaterialMap.GetValueAt(pos);

        MtoE.RecentlyUsed= true;

#ifdef DEBUG
		D3DXVECTOR4 v;
		DXCC_CHECK_HRESULT( MtoE.Effect->GetVector( "Ambient" , &v ) );
		DXCC_ASSERT( 0 == memcmp( &pStdMtl->MatD3D.Ambient, (void*)&v, sizeof(D3DXVECTOR3) ) );
		
		DXCC_CHECK_HRESULT( MtoE.Effect->GetVector( "Diffuse" , &v ) );
		DXCC_ASSERT( 0 == memcmp( &pStdMtl->MatD3D.Diffuse, (void*)&v, sizeof(D3DXVECTOR3) ) );
	
		DXCC_CHECK_HRESULT( MtoE.Effect->GetVector( "Specular" , &v ) );
		DXCC_ASSERT( 0 == memcmp( &pStdMtl->MatD3D.Specular, (void*)&v, sizeof(D3DXVECTOR3) ) );

	
		DXCC_CHECK_HRESULT( MtoE.Effect->GetFloat( "Power" , &v.x ) );
		DXCC_ASSERT( 0 == memcmp( &pStdMtl->MatD3D.Power, (void*)&v, sizeof(FLOAT) ) );
	
		DXCC_CHECK_HRESULT( MtoE.Effect->GetVector( "Emissive" , &v ) );
		DXCC_ASSERT( 0 == memcmp( &pStdMtl->MatD3D.Emissive, (void*)&v, sizeof(D3DXVECTOR3) ) );
#endif

		MtoE.Effect.CopyTo( ppEffect );
    }
}

void 
CMayaViewer::RemoveUnusedShaders()
{
    for( POSITION pos= m_MaterialMap.GetHeadPosition();
        pos != NULL;)
    {
		POSITION nextPos= pos;
		m_MaterialMap.GetNext(nextPos);
		do
		{
			MaterialToEffect& MtoE= m_MaterialMap.GetValueAt(pos);

			if(MtoE.RecentlyUsed == false)
			{
				if( MtoE.Effect != NULL )
				{
					m_EffectMap.RemoveEffect( MtoE.Effect );

					DXCCEffectRemoveResources( MtoE.Effect, m_pPreviewPipeline->AccessManager() );
				}

				m_MaterialMap.RemoveAt(pos);
			}

		}
		while(0);
		pos= nextPos;
    }
}

void 
CMayaViewer::ResetUsedShaders()
{
    for( POSITION pos= m_MaterialMap.GetHeadPosition();
        pos != NULL; 
		m_MaterialMap.GetNext(pos))
    {
        MaterialToEffect& MtoE= m_MaterialMap.GetValueAt(pos);
        MtoE.RecentlyUsed = false;
    }
}

CGrowableArray< CMayaViewer* >& CMayaViewer::GetViewerList()
{
    // Using an accessor function gives control of the construction order
    static CGrowableArray< CMayaViewer* > ViewerList;
    return ViewerList;
}


//-------------------------------------------------------------------------------------
UINT __stdcall CMayaViewer::StaticRunThread( void* pParam )
{
    CMayaViewer* pViewer = (CMayaViewer*) pParam;
    return pViewer->RunThread();
}


//-------------------------------------------------------------------------------------
LRESULT CALLBACK CMayaViewer::StaticMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // Loop through all known viewers and see if this message is intended for the owned
    // window
    CGrowableArray< CMayaViewer* >& Viewers = GetViewerList();

    for( int i=0; 
        i < Viewers.GetSize(); 
        i++ )
    {
        CMayaViewer* pViewer = Viewers[ i ];

        if( pViewer->GetRenderWindow() == hWnd )
            return pViewer->MsgProc( hWnd, uMsg, wParam, lParam );
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}





//-------------------------------------------------------------------------------------
CMayaViewer::CMayaViewer()
{
    // Add this instance to the global list
    GetViewerList().Add( this );

    // TODO: flush this out
    m_pFont = NULL;
    m_pTextSprite = NULL;
    m_bCreated = false;
    m_bVisible = false;
    m_hWndBound = NULL;

    m_dwWidth = 0;
    m_dwHeight = 0;

	D3DXMatrixIdentity(&m_SpotLight);


	m_LockSpotLightToCameraByLKey= true;
	m_LockSpotLightToCameraByLMouse= false;
}


//-------------------------------------------------------------------------------------
CMayaViewer::~CMayaViewer()
{
    // Remove this instance from the global list
    GetViewerList().Remove( GetViewerList().IndexOf( this ) );
}


//-------------------------------------------------------------------------------------
UINT __stdcall CMayaViewer::RunThread()
{
    EnterCriticalSection(&DeviceAndViewerSection);
    HRESULT hr= S_OK;

    // Register the window class
    WNDCLASS wndClass;
    wndClass.style = CS_DBLCLKS;
    wndClass.lpfnWndProc = StaticMsgProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = m_CreateArgs.hInstance;
    wndClass.hIcon = m_CreateArgs.hIcon;
    wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = L"Direct3DWindowClass";

    if( !RegisterClass( &wndClass ) )
    {
        DWORD dwError = GetLastError();
        if( dwError != ERROR_CLASS_ALREADY_EXISTS )
            return HRESULT_FROM_WIN32(dwError);
    }

    // Set the window's initial style.  It is invisible initially since it might
    // be resized later
    DWORD dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
                            WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    
    // Create the render window
    m_hWndShell = (HWND) M3dView::applicationShell();
    m_hWnd = CreateWindow( L"Direct3DWindowClass", m_CreateArgs.strWindowTitle, dwWindowStyle,
                            m_CreateArgs.x, m_CreateArgs.y, m_CreateArgs.width, m_CreateArgs.height, NULL,
                            m_CreateArgs.hMenu, m_CreateArgs.hInstance, 0 );
    if( m_hWnd == NULL )
    {
        DWORD dwError = GetLastError();
        return HRESULT_FROM_WIN32( dwError );
    }

	SetParent(m_hWnd, m_hWndShell);

    Hide();
    BindToWindow( NULL, false );


	m_iFrameNumber= 0;

    // Start a timer to check for resize
    SetTimer( m_hWnd, 0xffff, 200, NULL );
        
    LeaveCriticalSection(&DeviceAndViewerSection);

    // Now that the window has been created start the message loop
    //bool bGotMsg;
    MSG  msg;
    msg.message = WM_NULL;
	//prime for WM_QUIT on first loop

	for(msg.message = WM_NULL;
		WM_QUIT != msg.message;
		Sleep( 60 ))
    {
		if(m_bDestroyCalled == true)
		{
			m_bDestroyCalled = false;
			PostQuitMessage(0);
		}

		// Use PeekMessage() so we can use idle time to render the scene. 
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) 
		&& (WM_QUIT != msg.message))
        {
            // Translate and dispatch the message
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        
		//check for quick message again encase peekmessage fetched us a new one.
		if(WM_QUIT != msg.message 
			&& m_bDestroyCalled == false)
        {
			CManagerLock Lock;
  			if(m_pPreviewPipeline->SceneReadLock(false, Lock))
			{
				// Render a frame during idle time (no messages are waiting)
				if( m_nPauseCount == 0 )
				{
					m_fTime = (float) DXUTGetGlobalTimer()->GetAbsoluteTime();
					m_fElapsedTime = m_fTime - m_fPreviousTime;
					m_fPreviousTime = m_fTime;
					m_iFrameNumber++;
					FrameMove();

					if(m_pSwapChain)
						Render();
				}

				m_pPreviewPipeline->SceneReadUnlock(Lock);
			}
        }
    }

	UnregisterClass( wndClass.lpszClassName, GetWindowInstance(m_hWndShell) );

    return 0;
}


//-------------------------------------------------------------------------------------
HRESULT CMayaViewer::Create( CManager* pPreviewPipeline,    
                         D3DPRESENT_PARAMETERS* pPresentationParameters,
                         const WCHAR* strWindowTitle, 
                         HINSTANCE hInstance, 
                         HICON hIcon, 
                         HMENU hMenu,
                         int width,
                         int height,
                         int x, 
                         int y )
{
    m_pPreviewPipeline = pPreviewPipeline;
	m_dwWidth = width;
    m_dwHeight = height;

    m_bDestroyCalled = false;


    // Register to receive device state notification
    m_pPreviewPipeline->TriggerDeviceEvents.RegisterEvents( this );
/*    m_pPreviewPipeline->TriggerManagerEvents.RegisterEvents( this );
    m_pPreviewPipeline->TriggerViewerEvents.RegisterEvents( this );
*/

    if( hInstance == NULL )
        hInstance = GetModuleHandle( NULL );

    ZeroMemory( &m_CreateArgs, sizeof(CreateArgs) );
    m_CreateArgs.PresentParams = *pPresentationParameters;
    StringCchCopyW( m_CreateArgs.strWindowTitle, MAX_PATH, strWindowTitle );
    m_CreateArgs.hInstance = hInstance;
    m_CreateArgs.hIcon = hIcon;
    m_CreateArgs.hMenu = hMenu;
    m_CreateArgs.width = width;
    m_CreateArgs.height = height;
    m_CreateArgs.x = x;
    m_CreateArgs.y = y;

    // Launch the thread which will create the window and run the message/render loop
    m_hThread = _beginthreadex( NULL, 0, StaticRunThread, this, 0, &m_nThreadID );
    if( m_hThread == NULL )
        return E_FAIL;

    Run();
    return S_OK;
}

MaterialToEffect::~MaterialToEffect()
{
	//DXCC_RELEASE(Effect);CComPtr<> auto releases
}
//-------------------------------------------------------------------------------------
HRESULT CMayaViewer::Destroy()
{

    if( m_pPreviewPipeline )
    {
        m_pPreviewPipeline->TriggerDeviceEvents.UnregisterEvents( this );
    }

	OnD3DDeviceLost();
	OnD3DDeviceDestroy();

    m_bDestroyCalled = true;
	//wait a reasonable length of time for the viewer to quit.
    WaitForSingleObject( (HANDLE)m_hThread, 500 );//INFINITE);//
	TerminateThread((HANDLE)m_hThread, 0);
    CloseHandle( (HANDLE)m_hThread );

	m_hWnd= 0;
    m_hThread = 0;
    m_nThreadID = 0;

    return S_OK;
}


//-------------------------------------------------------------------------------------
void CMayaViewer::BindToWindow( HWND hWnd, BOOL bVisible  )
{
    DWORD dwWindowStyle = 0;
    
    if( hWnd != NULL )
    {
        // Docking
        dwWindowStyle |= WS_CHILD;
    }
    else
    {
        // Undocking
        dwWindowStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
                        WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    }

    if( bVisible )
		dwWindowStyle |= WS_VISIBLE;

    // Switch window styles
#ifdef _WIN64
    LONG_PTR nResult = SetWindowLongPtr( m_hWnd, GWL_STYLE, (LONG_PTR)dwWindowStyle );
#else
    LONG_PTR nResult = SetWindowLongPtr( m_hWnd, GWL_STYLE, (LONG_PTR)(LONG)dwWindowStyle );
#endif 
 
    if( nResult == 0 )
    {
        DXUT_ERR_MSGBOX( L"SetWindowLongPtr", HRESULT_FROM_WIN32(GetLastError()) );
        return;
    }

    SetParent( m_hWnd, hWnd );
	if(hWnd == NULL )   
	{
		SetWindowLongPtr( m_hWnd, GWLP_HWNDPARENT, (LONG_PTR)(LONG)(HWND)M3dView::applicationShell() );
	}

    /*
    WINDOWPLACEMENT placement = {0};
    placement.length = sizeof(WINDOWPLACEMENT);
    placement.showCmd = hWnd ? SW_MAXIMIZE : SW_NORMAL;
    SetWindowPlacement( DXUTGetHWND(), &placement );
    */

    nResult = SetWindowPos( m_hWnd, hWnd ? hWnd : 0, 0, 0, m_CreateArgs.width, m_CreateArgs.height, SWP_FRAMECHANGED | SWP_NOMOVE );
    if( nResult == 0 )
    {
        DXTRACE_ERR( L"SetWindowPos", HRESULT_FROM_WIN32(GetLastError()) );
        return;
    }

    m_hWndBound = hWnd;
    CheckForResize( true );
    
    // Calling show will in turn call Run(). We're also forcing Maya to completely repaint here
    // to avoid artifacts
    if( bVisible )
    {
        Show();
        ShowWindow( m_hWndShell, SW_HIDE );
        ShowWindow( m_hWndShell, SW_SHOW );
    }
}


//-------------------------------------------------------------------------------------
void CMayaViewer::CheckForResize( BOOL bForceResize )
{
    HRESULT hr;

    RECT rcClient;
    GetClientRect( m_hWndBound ? m_hWndBound : m_hWnd, &rcClient );
    
    DWORD dwWindowWidth = rcClient.right - rcClient.left;
    DWORD dwWindowHeight = rcClient.bottom - rcClient.top;

	if(dwWindowWidth != 0 && dwWindowHeight != 0)
	{    
		if( bForceResize || (m_dwWidth != dwWindowWidth) || (m_dwHeight != dwWindowHeight))
		{
			m_dwWidth = dwWindowWidth;
			m_dwHeight = dwWindowHeight;
		    
			if( m_hWndBound )
			{
				SetWindowPos( m_hWnd, HWND_TOP, 
							0, 0, m_dwWidth, m_dwHeight, 
							( m_bVisible && m_dwWidth > 0 ) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW );
			}

			// Flush the resources and recreate the swap chain using the new dimensions
			V( OnD3DDeviceLost() );
			V( OnD3DDeviceDestroy() );
			V( OnD3DDeviceCreate() );
			V( OnD3DDeviceReset() );
		}
	}
}


//-------------------------------------------------------------------------------------
void CMayaViewer::Hide()
{
    m_bVisible = false;
	g_PreviewPipeline.m_ViewerBinding[0]= '\0';
    ShowWindow( m_hWnd, SW_HIDE );
    Pause();
}


//-------------------------------------------------------------------------------------
void CMayaViewer::Show()
{
    m_bVisible = true;
    ShowWindow( m_hWnd, SW_SHOW );
    Run();
}


//-------------------------------------------------------------------------------------
HRESULT CMayaViewer::GetD3DSwapChain( IDirect3DSwapChain9** ppSwapChain )
{
    *ppSwapChain = m_pSwapChain;

    if( m_pSwapChain == NULL )
        return E_FAIL;

    return S_OK;
}


//-------------------------------------------------------------------------------------
HRESULT CMayaViewer::OnD3DDeviceCreate()
{
    CManagerLock Lock;
	m_pPreviewPipeline->SceneWriteLock(true, Lock);

    HRESULT hr;
    IDirect3DDevice9* pd3dDevice = NULL;
    
    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye(0.0f, 0.0f, -5.0f);
    D3DXVECTOR3 vecAt (0.0f, 0.0f, -0.0f);
    m_Camera.SetViewParams( &vecEye, &vecAt );

    CEngine* pEngine = m_pPreviewPipeline->AccessEngine();
    hr = pEngine->GetD3DDevice( &pd3dDevice );

    if( FAILED(hr) || pd3dDevice == NULL )
        goto LCleanReturn;

    // Initialize the font
    V( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &m_pFont ) );
    if( FAILED(hr) )
        goto LCleanReturn;

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = 0;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

	{//LOAD DEFAULT SHADER
		LPD3DXBUFFER pErrors= NULL;
		// Load the default effect file
		hr= D3DXCreateEffect( 
			pd3dDevice, 
			g_strDefaultEffect, 
			sizeof(g_strDefaultEffect), 
			NULL, 
			&g_SasIncluder, 
			dwShaderFlags, 
			NULL, 
			&m_pDefaultEffect, 
			&pErrors );
		if(!DXCC_SUCCEEDED(hr))
		{

			MessageBoxA(         
				(HWND)M3dView::applicationShell(),
				(LPCSTR)(pErrors?pErrors->GetBufferPointer():NULL),
				"Effect Load Error",
				MB_OK|MB_ICONEXCLAMATION);

			DXCC_RELEASE(pErrors);
			goto LCleanReturn;
		}
		
		DXCC_RELEASE(pErrors);
	}//END//LOAD DEFAULT SHADER


    ResetUsedShaders();
    RemoveUnusedShaders();
    m_EffectMap.AddEffect( m_pDefaultEffect );

    // Read the mesh
    m_LightWidget.StaticOnCreateDevice( pd3dDevice );
    m_LightWidget.SetLightDirection( D3DXVECTOR3(0.0f, 0.0f, 1.0f) );

    // TODO: move this to the load function and rely on the object radius
    m_Camera.SetRadius( 3.0f, 0.5f, 10.0f );
    m_LightWidget.SetRadius( 0.5f );

    hr = S_OK;

LCleanReturn:
    SAFE_RELEASE( pd3dDevice );
    
    m_bCreated = true;

	m_pPreviewPipeline->SceneWriteUnlock(Lock);
    return hr;
}


//-------------------------------------------------------------------------------------
HRESULT CMayaViewer::OnD3DDeviceReset()
{
 	//REMOVED: LOCKS HERE MAY CAUSE THREAD DEADLOCK
    //CManagerLock Lock;
	//m_pPreviewPipeline->SceneWriteLock(true, Lock);

    HRESULT hr;

    CEngine* pEngine = m_pPreviewPipeline->AccessEngine();
    IDirect3DSurface9* pBackBuffer = NULL;
    IDirect3DDevice9* pd3dDevice = NULL;
    IDirect3DSurface9* pSurfDeviceDepthStencil = NULL;
    
	hr = pEngine->GetD3DDevice( &pd3dDevice );
    if( FAILED(hr) || pd3dDevice == NULL )
        goto LCleanReturn;

    // Create the render chain
    D3DPRESENT_PARAMETERS pp;
    pp = m_CreateArgs.PresentParams;
    pp.BackBufferWidth = m_dwWidth;
    pp.BackBufferHeight = m_dwHeight;
    V( pd3dDevice->CreateAdditionalSwapChain( &pp, &m_pSwapChain ) );
    if( FAILED(hr) )
        goto LCleanReturn;

    // Create the depth stencil surface
    V( pd3dDevice->GetDepthStencilSurface( &pSurfDeviceDepthStencil ) );
    if( FAILED(hr) )
        goto LCleanReturn;

    D3DSURFACE_DESC desc;
    V( pSurfDeviceDepthStencil->GetDesc( &desc ) );
    if( FAILED(hr) )
        goto LCleanReturn;

    V( pd3dDevice->CreateDepthStencilSurface( m_dwWidth, m_dwHeight, desc.Format, desc.MultiSampleType, desc.MultiSampleQuality, false, &m_pDepthStencil, NULL ) );
    if( FAILED(hr) )
        goto LCleanReturn;

    V( m_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer ) );
    if( FAILED(hr) )
        goto LCleanReturn;

    D3DSURFACE_DESC BackBufferDesc;
    V( pBackBuffer->GetDesc( &BackBufferDesc ) );
    SAFE_RELEASE( pBackBuffer );

    if( FAILED(hr) )
        goto LCleanReturn;

	ResetUsedShaders();
	RemoveUnusedShaders();

    if( m_pDefaultEffect )
        m_pDefaultEffect->OnResetDevice();

    m_LightWidget.OnResetDevice( &BackBufferDesc );

    if( m_pFont )
    {
        V( m_pFont->OnResetDevice() );
        if( FAILED(hr) )
            goto LCleanReturn;
    }
    
    // Create a sprite to help batch calls when drawing many lines of text
    V( D3DXCreateSprite( pd3dDevice, &m_pTextSprite ) );
    if( FAILED(hr) )
        goto LCleanReturn;

    // Setup the camera's projection parameters
    float fAspectRatio;
    fAspectRatio = BackBufferDesc.Width / (FLOAT)BackBufferDesc.Height;
    
    m_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 1000.0f );
    m_Camera.SetWindow( BackBufferDesc.Width, BackBufferDesc.Height );
    m_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );

    hr = S_OK;

LCleanReturn:
    SAFE_RELEASE( pBackBuffer );
    SAFE_RELEASE( pd3dDevice );
    SAFE_RELEASE( pSurfDeviceDepthStencil );

    Run();

 	//REMOVED: LOCKS HERE MAY CAUSE THREAD DEADLOCK
	//m_pPreviewPipeline->SceneWriteUnlock(Lock);
    return hr;
}



//--------------------------------------------------------------------------------------
// Before handling window messages, the sample framework passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then the sample framework will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CMayaViewer::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    // Pass windows messages to camera so it can respond to user input
    m_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );
    m_LightWidget.HandleMessages( hWnd, uMsg, wParam, lParam );

    switch( uMsg )
    {
/*
        case WM_LBUTTONDOWN:
			{
				if( 0 != (GetKeyState(VK_RBUTTON) & 0x8000) )
				{
					m_LockSpotLightToCameraByLMouse= true;
				}
				break;
			}
        case WM_RBUTTONDOWN:
			{
				if( 0 != (GetKeyState(VK_LBUTTON) & 0x8000) )
				{
					m_LockSpotLightToCameraByLMouse= true;
				}
				break;
			}
*/
        case WM_KEYDOWN:
        {
            switch( wParam )
            {
/*
				case 'L':
					m_LockSpotLightToCameraByLKey = !m_LockSpotLightToCameraByLKey;
					break;
*/
                case VK_ESCAPE:
                    Destroy();
                    break;
            }

            break;
        }

        case WM_TIMER:
            CheckForResize();
            break;

        case WM_CLOSE:
            Hide();
            return 0;

        case WM_PAINT:
            Render();
            break;

    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}



//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CMayaViewer::FrameMove()
{
    // Update the camera's position based on user input 
    m_Camera.FrameMove( m_fElapsedTime );
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, the sample framework will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CMayaViewer::Render()
{
    HRESULT hr;
    D3DXMATRIX mWorld;
    D3DXMATRIX mView;
    D3DXMATRIX mProj;
    D3DXMATRIX mWorldViewProjection;

    
    if( m_bCreated == false || m_pSwapChain == NULL)
        return;

    // Get read lock
    CManagerLock SceneLock;


	//TODO:  note when there are multiple viewer ReadLock will not be enough.
	//they will need to be in 1 thread or use a locking mechanism to make sure
	//they are not colliding on the state changes.
	if(m_pPreviewPipeline->SceneReadLock( false , SceneLock))
	{
        ResetUsedShaders();

		CEngine* pEngine = m_pPreviewPipeline->AccessEngine();
		IDirect3DDevice9* pd3dDevice = NULL;
		hr = pEngine->GetD3DDevice( &pd3dDevice );
		if( DXCC_SUCCEEDED(hr) 
			&& pd3dDevice != NULL 
			&& D3D_OK == pd3dDevice->TestCooperativeLevel())
		{

			IDirect3DSurface9* pSurfOldRenderTarget = NULL;
			V( pd3dDevice->GetRenderTarget( 0, &pSurfOldRenderTarget ) );

			IDirect3DSurface9* pSurfOldDepthStencil = NULL;
			V( pd3dDevice->GetDepthStencilSurface( &pSurfOldDepthStencil ) );
			V( pd3dDevice->SetDepthStencilSurface( m_pDepthStencil ) );

			IDirect3DSurface9* pSurfNewRenderTarget = NULL;
			hr= m_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pSurfNewRenderTarget );
			V( pd3dDevice->SetRenderTarget( 0, pSurfNewRenderTarget ) );
   			SAFE_RELEASE( pSurfNewRenderTarget );
		 
			// Clear the render target and the zbuffer 
			V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 100), 1.0f, 0) );

			// Render the scene
			if( SUCCEEDED( pd3dDevice->BeginScene() ) )
			{
				float zero= 0.0f;
				float one= 1.0f;
				D3DXMATRIX identity;

				D3DXMatrixIdentity( &identity );

	
				mView= g_PreviewPipeline.PerspectiveCamera_GetView();
 				mProj= g_PreviewPipeline.PerspectiveCamera_GetProjection();
				// Adjust for aspect ratio
				mProj._22 *= (float)m_dwWidth / m_dwHeight;


				D3DXVECTOR3 vAmbient( 0.1f, 0.1f, 0.1f);
				D3DXVECTOR3 vDiffuse( 0.9f, 0.9f, 0.9f );
				D3DXVECTOR3 vDirLightDirection( m_LightWidget.GetLightDirection() );

				if(m_LockSpotLightToCameraByLKey || m_LockSpotLightToCameraByLMouse)
				{
					D3DXMatrixInverse( &m_SpotLight, NULL, &mView );
					m_LockSpotLightToCameraByLMouse= false;
				}

				D3DXVECTOR4 vSpotLightPosition4;
				D3DXVECTOR4 vZero(0.0f ,0.0f,0.0f, 1.0f);
				D3DXVec4Transform(&vSpotLightPosition4, &vZero , &m_SpotLight);
				D3DXVECTOR3 vSpotLightPosition3( (float*)&vSpotLightPosition4 );


				D3DXVECTOR3 vSpotLightDirection;
				D3DXVECTOR3 vZAxis(0.0f,0.0f,1.0f);
				D3DXVec3TransformNormal(&vSpotLightDirection,  &vZAxis , &m_SpotLight);
	
				D3DXMATRIX mViewInv;
				D3DXMatrixInverse(&mViewInv, NULL, &mView);
				D3DXVECTOR4 CameraPosition4;
				D3DXVec4Transform( &CameraPosition4, &vZero, &mViewInv);
				D3DXVECTOR3 CameraPosition3((FLOAT*)&CameraPosition4);
				SasCamera sasCamera(mView, mProj, 1.0f, 1000.0f, CameraPosition3);

				SasAmbientLight ambientLight(vAmbient);
				SasPointLight pointLight(vDiffuse, vSpotLightPosition3, 1000.0f);
				SasDirectionLight directionLight(vDiffuse, vDirLightDirection);
				SasSpotLight spotLight(vDiffuse, vSpotLightPosition3, vSpotLightDirection, 1000.0f, D3DX_PI/3.0f, D3DX_PI/2.0f );

				float times[3]= {(float)m_fTime, (float)m_fPreviousTime, (float)m_iFrameNumber};

	

				m_EffectMap.SetStandardParameterAsValue( SASBA_Camera, sasCamera.Data.floats, sizeof(sasCamera));
				m_EffectMap.SetStandardParameterAsMatrix( SASBA_Camera_WorldToView, (D3DXMATRIX*)&sasCamera.Data.components.WorldToView ) ;
				m_EffectMap.SetStandardParameterAsMatrix( SASBA_Camera_Projection, (D3DXMATRIX*)&sasCamera.Data.components.Projection ) ;
				m_EffectMap.SetStandardParameterAsFloats( SASBA_Camera_NearFarClipping, sasCamera.Data.components.NearFarClipping, 2 );
				m_EffectMap.SetStandardParameterAsFloats( SASBA_Camera_Position, (FLOAT*)&sasCamera.Data.components.Position, 3 ) ;

	
				m_EffectMap.SetStandardParameterAsFloats( SASBA_Time, (float*) times, 3 );  
				m_EffectMap.SetStandardParameterAsFloats( SASBA_Time_Now, (float*) &times[0], 1 );  
				m_EffectMap.SetStandardParameterAsFloats( SASBA_Time_Last, (float*) &times[1], 1 );  
				m_EffectMap.SetStandardParameterAsFloats( SASBA_Time_FrameNumber, (float*) &times[2], 1 );  


				m_EffectMap.SetStandardParameterAsValue( SASBA_AmbientLight, (float*) ambientLight.Data.floats, sizeof(ambientLight) );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_AmbientLight_Color, (float*) &ambientLight.Data.components.Color, 3 );
				m_EffectMap.SetStandardParameterAsFloats( SASBA_NumAmbientLights, (float*) &one, 1 );

				m_EffectMap.SetStandardParameterAsValue( SASBA_PointLight, (float*) pointLight.Data.floats, sizeof(pointLight) );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_PointLight_Color, (float*) &pointLight.Data.components.Color, 3 );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_PointLight_Position, (float*) &pointLight.Data.components.Position, 3 );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_PointLight_Range, (float*) &pointLight.Data.components.Range, 1 );
				m_EffectMap.SetStandardParameterAsFloats( SASBA_NumPointLights, (float*) &one, 1 );

				m_EffectMap.SetStandardParameterAsValue( SASBA_DirectionalLight, (float*) directionLight.Data.floats, sizeof(directionLight) );
				m_EffectMap.SetStandardParameterAsFloats( SASBA_DirectionalLight_Color, (float*) &directionLight.Data.components.Color, 3 );
				m_EffectMap.SetStandardParameterAsFloats( SASBA_DirectionalLight_Direction, (float*) &directionLight.Data.components.Direction, 3 );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_NumDirectionalLights, (float*) &one, 1 );

				m_EffectMap.SetStandardParameterAsValue( SASBA_SpotLight, (float*) spotLight.Data.floats, sizeof(spotLight) );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_SpotLight_Color, (float*) &spotLight.Data.components.Color, 3 );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_SpotLight_Position, (float*) &spotLight.Data.components.Position, 3 );
				m_EffectMap.SetStandardParameterAsFloats( SASBA_SpotLight_Direction, (float*) &spotLight.Data.components.Direction, 3 );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_SpotLight_Range, (float*) &spotLight.Data.components.Range, 1 );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_SpotLight_Theta, (float*) &spotLight.Data.components.Theta, 1 );
 				m_EffectMap.SetStandardParameterAsFloats( SASBA_SpotLight_Phi, (float*) &spotLight.Data.components.Phi, 1 );
				m_EffectMap.SetStandardParameterAsFloats( SASBA_NumSpotLights, (float*) &one, 1 );


				m_EffectMap.SetStandardParameterAsFloats( SASBA_NumShadows, (float*) &zero, 1);



				LPDXCCFRAME pFrameRoot = m_pPreviewPipeline->AccessRoot();
				pFrameRoot->AddRef();
		        
				if( pFrameRoot != NULL )
					DrawFrame( pFrameRoot );
		        
				SAFE_RELEASE( pFrameRoot );


				//TODO: get the eye point from the maya camera
				//V( m_LightWidget.OnRender( D3DCOLOR_ARGB(255, 255, 255, 255), &mView, &mProj, m_Camera.GetEyePt() ) );
		        
				//if( m_pFont && m_pTextSprite )
				//    RenderText();

				V( pd3dDevice->EndScene() );

				if(m_pSwapChain)
					V( m_pSwapChain->Present( NULL, NULL, m_hWnd, NULL, 0 ) );

			}

			pd3dDevice->SetRenderTarget( 0, pSurfOldRenderTarget );
			SAFE_RELEASE( pSurfOldRenderTarget );
			pd3dDevice->SetDepthStencilSurface( pSurfOldDepthStencil );
			SAFE_RELEASE( pSurfOldDepthStencil );
		}

		SAFE_RELEASE( pd3dDevice );
		
        RemoveUnusedShaders();

		m_pPreviewPipeline->SceneReadUnlock(SceneLock);
 
	}

}


//-----------------------------------------------------------------------------
// Name: DrawFrame()
// Desc: Called to render a frame in the hierarchy
//-----------------------------------------------------------------------------
void CMayaViewer::DrawFrame( IDXCCFrame* pFrame)
{
    HRESULT hr = S_OK;

    if(pFrame->NumMembers() > 0)
    {

		float one= 1.0f;

		m_EffectMap.SetStandardParameterAsMatrix( SASBA_Skeleton_MeshToJointToWorld, pFrame->GetWorldMatrix() ) ;
		m_EffectMap.SetStandardParameterAsFloats( SASBA_Skeleton_NumJoints, &one, 1 );


    }

    for( UINT iMesh=0; iMesh < pFrame->NumMembers(); iMesh++ )
    {
        IDXCCMesh* pMesh = NULL;
        hr = pFrame->QueryMember( iMesh, IID_IDXCCMesh, (void**) &pMesh );
        if( FAILED(hr) )
            continue;

        DrawMesh( pFrame, pMesh );

		SAFE_RELEASE(pMesh);
    }

    for( UINT iChild=0; iChild < pFrame->NumChildren(); iChild++ )
    {
        IDXCCFrame* pChild = NULL;
        hr = pFrame->GetChild( iChild, &pChild );
        if( FAILED(hr) )
            continue;

        DrawFrame( pChild );

		SAFE_RELEASE(pChild);
    }
}


//--------------------------------------------------------------------------------------
void CMayaViewer::DrawMesh( IDXCCFrame* pParent, IDXCCMesh* pMesh )
{
    HRESULT hr;

    ID3DXBuffer* pAttribs = NULL;
    DWORD dwNumAttribs = 0;
    hr = pMesh->GetAttributeBindingList( &pAttribs, &dwNumAttribs );
    if( FAILED(hr) )
        return;

    DWORD* pdwAttrib = NULL;
    if( pAttribs )
         pdwAttrib = (DWORD*) pAttribs->GetBufferPointer();

    if( dwNumAttribs > 0 )
    {
        if (pdwAttrib)
        {

			if(pMesh->IsSkinned() == false && pMesh->NumBones() > 0)
			{
				hr= pMesh->DoSkinning();
				if( FAILED(hr) )
					return;
			}

            for( UINT iMaterial = 0; iMaterial < dwNumAttribs; iMaterial++ )
            {
                DWORD dwAttrib = *( pdwAttrib + iMaterial );

				ID3DXEffect* pEffect = NULL;

				DWORD shaderAID;
				LPDXCCSHADERPROPERTIES pShader= NULL;
                hr = pMesh->GetAttributeBoundShader( dwAttrib, &pShader, &shaderAID);
                if( SUCCEEDED(hr) && (pShader != NULL))
				{
					GetOrAddEffectFromShader( shaderAID, pShader, &pEffect );
					DXCC_RELEASE(pShader);

				}
				
				if(!pEffect)
				{
					DWORD stdMtlAID;
					LPD3DXMATERIAL pStdMtl= NULL;
					hr = pMesh->GetAttributeBoundMaterial( dwAttrib, &pStdMtl, &stdMtlAID );
					if( SUCCEEDED(hr) && (pStdMtl != NULL))
					{
						GetOrAddEffectFromStdMtl( stdMtlAID, pStdMtl, &pEffect );
					}
				}

				if(!pEffect && m_pDefaultEffect)
				{
					m_pDefaultEffect->AddRef();
					pEffect=m_pDefaultEffect;
				}
				else if( !m_pDefaultEffect )
					continue;

                D3DXHANDLE hTechnique;
                V( pEffect->FindNextValidTechnique( NULL, &hTechnique ) );
                V( pEffect->SetTechnique( hTechnique ) );

                UINT numPasses;
                V( pEffect->Begin(&numPasses, 0) );

                for( UINT iPass = 0; iPass < numPasses; iPass++ )
                {
                    V( pEffect->BeginPass(iPass) );

                    // The effect interface queues up the changes and performs them 
                    // with the CommitChanges call. You do not need to call CommitChanges if 
                    // you are not setting any parameters between the BeginPass and EndPass.
                    // V( pEffect->CommitChanges() );

                    // Render the mesh with the applied technique
                    V( pMesh->DrawSubset( dwAttrib ) );
                    
                    V( pEffect->EndPass() );
                }
                V( pEffect->End() );

			    SAFE_RELEASE(pEffect);
            }
        }
    }
    else
    {   
        if( m_pDefaultEffect )
        {
            D3DXHANDLE hTechnique;
            V( m_pDefaultEffect->FindNextValidTechnique( NULL, &hTechnique ) );
            V( m_pDefaultEffect->SetTechnique( hTechnique ) );

            UINT numPasses;
            V( m_pDefaultEffect->Begin(&numPasses, 0) );

            for( UINT iPass = 0; iPass < numPasses; iPass++ )
            {
                V( m_pDefaultEffect->BeginPass(iPass) );

                // The effect interface queues up the changes and performs them 
                // with the CommitChanges call. You do not need to call CommitChanges if 
                // you are not setting any parameters between the BeginPass and EndPass.
                // V( pEffect->CommitChanges() );

                // Render the mesh with the applied technique
                V( pMesh->DrawSubset( 0 ) );
                
                V( m_pDefaultEffect->EndPass() );
            }
            V( m_pDefaultEffect->End() );
        }
      
    }

    SAFE_RELEASE( pAttribs );
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void CMayaViewer::RenderText()
{
    // The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    // If NULL is passed in as the sprite object, then it will work however the 
    // pFont->DrawText() will not be batched together.  Batching calls will improves performance.
    CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );

    WCHAR strBuffer[MAX_PATH] = {0};
    StringCchPrintfW( strBuffer, MAX_PATH, L"Time: %f", m_fTime );
    txtHelper.DrawTextLine( strBuffer );
    
    txtHelper.End();
}



//-------------------------------------------------------------------------------------
HRESULT CMayaViewer::OnD3DDeviceLost()
{ 

    CManagerLock Lock;
	m_pPreviewPipeline->SceneWriteLock(true, Lock);

    UINT ref;

	Pause();

    m_LightWidget.StaticOnLostDevice();
    
    if( m_pFont )
        m_pFont->OnLostDevice();
    
    if( m_pDefaultEffect )
        m_pDefaultEffect->OnLostDevice();

    ref= DXCC_RELEASE( m_pDepthStencil );
    ref= DXCC_RELEASE( m_pTextSprite );
    ref= DXCC_RELEASE( m_pSwapChain );

	ResetUsedShaders();
	RemoveUnusedShaders();

	m_pPreviewPipeline->SceneWriteUnlock(Lock);
    return S_OK;
}


//-------------------------------------------------------------------------------------
HRESULT CMayaViewer::OnD3DDeviceDestroy()
{
	CManagerLock Lock;
	m_pPreviewPipeline->SceneWriteLock(true, Lock);

	UINT ref;
    m_bCreated = false;

	ResetUsedShaders();
	RemoveUnusedShaders();

	m_LightWidget.StaticOnDestroyDevice();
    ref= DXCC_RELEASE( m_pFont );
    ref= DXCC_RELEASE( m_pTextSprite );
    ref= DXCC_RELEASE( m_pDefaultEffect );

	m_pPreviewPipeline->SceneWriteUnlock(Lock);
    return S_OK;
}
