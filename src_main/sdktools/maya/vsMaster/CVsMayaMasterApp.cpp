//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

// Regular includes
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Maya includes

#include <maya/MGlobal.h>
#include <maya/MString.h>

// Valve includes

#include "vstdlib/cvar.h"
#include "inputsystem/iinputsystem.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/materialsystem_config.h"
#include "vguimatsurface/imatsystemsurface.h"
#include "vgui/IVGui.h"
#include "vgui/ISurface.h"
#include "mathlib/mathlib.h"
#include "UtlStringMap.h"
#include "datacache/idatacache.h"
#include "vgui/isystem.h"
#include "vgui/ischeme.h"
#include "vgui_controls/controls.h"
#include "filesystem.h"
#include "imayavgui.h"
#include "tier1/tier1.h"
#include "tier2/tier2dm.h"
#include "tier3/tier3dm.h"
#include "CVsMayaMasterApp.h"
#include "dmserializers/idmserializers.h"
#include "movieobjects/idmemakefileutils.h"
#include "datamodel/dmelementfactoryhelper.h"
#include "vstdlib/iprocessutils.h"


//-----------------------------------------------------------------------------
//
// Purpose: Load a Maya plugin and get the required Valve DLL list from it
// Input  :
// Output :
//
//-----------------------------------------------------------------------------
void CVsMayaMasterApp::LoadPlugin( const char *pModuleName, const char *pModulePath )
{
	MString melCmd;
	melCmd = "catchQuiet( loadPlugin(\"";
	melCmd += pModulePath;
	melCmd += "\") );";

	MGlobal::executeCommand( melCmd, false, false );

	const HMODULE hModule( GetModuleHandle( pModulePath ) );
	if ( !hModule )
		return;

	m_modules[ pModuleName ] = hModule;

	const FARPROC pVsMayaGetAppSystems( GetProcAddress( hModule, "VsMayaGetAppSystems" ) );
	if ( pVsMayaGetAppSystems )
	{
		const AppSystemInfo_t *( *VsMayaGetAppSystems )()( ( const AppSystemInfo_t *(*)() )( pVsMayaGetAppSystems ) );
		AddAppSystems( ( *VsMayaGetAppSystems )() );
	}
}


//-----------------------------------------------------------------------------
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------
void CVsMayaMasterApp::AddAppSystem( const char *pModuleName, const char *pInterfaceName )
{
	AppSystemInfo_t appSystemInfo;
	appSystemInfo.m_pModuleName = m_stringPool.ReferenceString( pModuleName );
	appSystemInfo.m_pInterfaceName = m_stringPool.ReferenceString( pInterfaceName );

	// Only add this pair if they don't already exist
	for ( int ai( 0 ); ai != m_appSystems.Count(); ++ai )
	{
		const AppSystemInfo_t &tmpAppSystemInfo( m_appSystems[ai] );
		if (
			tmpAppSystemInfo.m_pInterfaceName == appSystemInfo.m_pInterfaceName &&
			tmpAppSystemInfo.m_pModuleName == appSystemInfo.m_pModuleName )
		{
			m_stringPool.DereferenceString( pModuleName );
			m_stringPool.DereferenceString( pInterfaceName );

			return;
		}
	}

	m_appSystems.AddToTail( appSystemInfo );
}


//-----------------------------------------------------------------------------
// Purpose: Add a number of app systems at once
//-----------------------------------------------------------------------------
void CVsMayaMasterApp::AddAppSystems( const AppSystemInfo_t *pAppSystemList )
{
	while ( pAppSystemList->m_pModuleName[0] )
	{
		AddAppSystem( pAppSystemList->m_pModuleName, pAppSystemList->m_pInterfaceName );
		++pAppSystemList;
	}
}


//-----------------------------------------------------------------------------
// Are we running in interactive mode?
//-----------------------------------------------------------------------------
bool CVsMayaMasterApp::IsInteractive() const
{
	return ( MGlobal::mayaState() == MGlobal::kInteractive );
}


//-----------------------------------------------------------------------------
// Purpose: Load the collected Valve DLLs and handle any funny exceptions
// Input:	
// Output:	
//-----------------------------------------------------------------------------
bool CVsMayaMasterApp::Create()
{
	bool retVal( true );

	// Add standard app systems: the ones needed to do vgui w/ matsys
	// We're going to assume all plugins at least use this; that way
	// we can put all the window creation code here in the master app
	if ( IsInteractive() )
	{
		AddAppSystem( "vstdlib.dll", PROCESS_UTILS_INTERFACE_VERSION );
		AddAppSystem( "inputsystem.dll", INPUTSYSTEM_INTERFACE_VERSION );
		AddAppSystem( "materialsystem.dll", MATERIAL_SYSTEM_INTERFACE_VERSION );
		AddAppSystem( "vguimatsurface.dll", VGUI_SURFACE_INTERFACE_VERSION );
		AddAppSystem( "vgui2.dll", VGUI_IVGUI_INTERFACE_VERSION );
	}
	else
	{
		AddAppSystem( "vstdlib.dll", PROCESS_UTILS_INTERFACE_VERSION );
		AddAppSystem( "materialsystem.dll", MATERIAL_SYSTEM_INTERFACE_VERSION );
	}

	// Build up the dependency & ordering information for DLLs
	CUtlStringMap<int> tierMap;

	// Tier 0
	tierMap["tier0.dll"] = 0;

	// Tier 1
	tierMap["vstdlib.dll"] = 1;

	// Tier 2
	tierMap["filesystem.dll"] = 2;
	tierMap["inputsystem.dll"] = 2;
	tierMap["materialsystem.dll"] = 2;
	tierMap["shaderapidx9.dll"] = 2;
	tierMap["valve_avi.dll"] = 2;
	tierMap["p4lib.dll"] = 2;

	// Tier 3
	tierMap["datacache.dll"] = 3;
	tierMap["soundemittersystem.dll"] = 3;
	tierMap["studiorender.dll"] = 3;
	tierMap["vguimatsurface.dll"] = 3;
	tierMap["vphysics.dll"] = 3;

	// Tier 4
	tierMap["vgui2.dll"] = 4;

	CUtlVector<AppSystemInfo_t> tier0;
	CUtlVector<AppSystemInfo_t> tier1;
	CUtlVector<AppSystemInfo_t> tier2;
	CUtlVector<AppSystemInfo_t> tier3;
	CUtlVector<AppSystemInfo_t> tier4;

	for ( int ai( 0 ); ai != m_appSystems.Count(); ++ai )
	{
		const AppSystemInfo_t &asi( m_appSystems[ai] );
		switch ( tierMap[ asi.m_pModuleName ] )
		{
		case 0:
			tier0.AddToTail( asi );
			break;
		case 1:
			tier1.AddToTail( asi );
			break;
		case 2:
			tier2.AddToTail( asi );
			break;
		case 3:
			// Tier3 Has Some Exceptions
			tier3.AddToTail( asi );
			break;
		case 4:
			tier4.AddToTail( asi );
			break;
		default:
			Warning( "Don't know what tier %s belongs to\n", asi.m_pModuleName );
			break;
		}
	}

	// Always Add in the cvar factory
	AppModule_t cvarModule = LoadModule( VStdLib_GetICVarFactory() );
	AddSystem( cvarModule, CVAR_INTERFACE_VERSION );

	AppSystemInfo_t emptyAppSystemInfo;
	emptyAppSystemInfo.m_pModuleName = m_stringPool.ReferenceString( "" );
	emptyAppSystemInfo.m_pInterfaceName = m_stringPool.ReferenceString( "" );

	if ( tier0.Size() )
	{
		tier0.AddToTail( emptyAppSystemInfo );
		if ( !AddSystems( tier0.Base() ) )
		{
			retVal = false;
		}
	}

	if ( tier1.Size() )
	{
		tier1.AddToTail( emptyAppSystemInfo );
		if ( !AddSystems( tier1.Base() ) )
		{
			retVal = false;
		}
	}

	if ( tier2.Size() )
	{
		tier2.AddToTail( emptyAppSystemInfo );
		if ( !AddSystems( tier2.Base() ) )
		{
			retVal = false;
		}
	}

	if ( tier3.Size() )
	{
		tier3.AddToTail( emptyAppSystemInfo );
		if ( !AddSystems( tier3.Base() ) )
		{
			retVal = false;
		}
	}

	if ( tier4.Size() )
	{
		tier4.AddToTail( emptyAppSystemInfo );
		if ( !AddSystems( tier4.Base() ) )
		{
			retVal = false;
		}
	}

	// Add in IMayaVGui, a service defined in vsMaster itself
	if ( IsInteractive() )
	{
		AppModule_t vsMasterModule = LoadModule( Sys_GetFactoryThis() ); 
		if ( !AddSystem( vsMasterModule, MAYA_VGUI_INTERFACE_VERSION ) )
		{
			retVal = false;
		}
		if ( !AddSystem( vsMasterModule, DMEMAKEFILE_UTILS_INTERFACE_VERSION ) )
		{
			retVal = false;
		}
	}

	// Clear the list of DLLs and the stringPool because they are no longer needed
	m_appSystems.Purge();
	m_stringPool.FreeAll();

	IMaterialSystem *pMaterialSystem = (IMaterialSystem*)FindSystem( MATERIAL_SYSTEM_INTERFACE_VERSION );
	if ( !pMaterialSystem )
	{
		Warning( "CVsMayaMasterApp::Create: Unable to connect to material system interface!\n" );
		return false;
	}
	pMaterialSystem->SetShaderAPI( IsInteractive() ? "shaderapidx9.dll" : "shaderapiempty.dll" );

	return retVal;
}


//-----------------------------------------------------------------------------
// Purpose: PreInit
//-----------------------------------------------------------------------------
bool CVsMayaMasterApp::PreInit( )
{
	bool retVal( true );

	CreateInterfaceFn factory = GetFactory();

	ConnectTier1Libraries( &factory, 1 );
	ConnectTier2Libraries( &factory, 1 );
	ConnectTier3Libraries( &factory, 1 );

	if ( !ConnectDataModel( factory ) )
		return false;

	if ( InitDataModel( ) != INIT_OK )
		return false;

	if ( IsInteractive() )
	{
		vgui::VGui_InitInterfacesList( "vsMaster", &factory, 1 );
	}

	for ( int mi = 0; mi < m_modules.GetNumStrings(); ++mi )
	{
		const FARPROC pVsMayaConnect( GetProcAddress( m_modules[mi], "VsMayaConnect" ) );
		if ( pVsMayaConnect )
		{
			bool( *VsMayaConnect )( CreateInterfaceFn )( ( bool (*)( CreateInterfaceFn ) )( pVsMayaConnect ) );
			if ( !( *VsMayaConnect )( factory ) )
			{
				retVal = false;
			}
		}
	}

	// Set up search paths
	if ( !SetupSearchPaths( NULL, false, true ) )
		return false;

	if ( g_pFullFileSystem )
	{
		char pPlatformPath[MAX_PATH];
		Q_snprintf( pPlatformPath, sizeof(pPlatformPath), "%s\\..\\platform", GetGameInfoPath() );

		g_pFullFileSystem->AddSearchPath( pPlatformPath, "PLATFORM" );
		g_pFullFileSystem->AddSearchPath( GetGameInfoPath(), "SKIN", PATH_ADD_TO_HEAD );
	}

	if ( g_pMaterialSystem )
	{
		g_pMaterialSystem->SetAdapter( 0, MATERIAL_INIT_ALLOCATE_FULLSCREEN_TEXTURE );
	}

	if ( g_pInputSystem )
	{
		g_pInputSystem->EnableMessagePump( false );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// Window management
//-----------------------------------------------------------------------------
void* CVsMayaMasterApp::Create3DWindow( char const *pTitle, bool bWindowed, int w, int h )
{
	WNDCLASSEX		wc;
	memset( &wc, 0, sizeof( wc ) );
	wc.cbSize		 = sizeof( wc );
	wc.style         = CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc   = DefWindowProc;
	wc.hInstance     = (HINSTANCE)GetAppInstance();
	wc.lpszClassName = "Valve001";
	wc.hIcon		 = NULL; //LoadIcon( s_HInstance, MAKEINTRESOURCE( IDI_LAUNCHER ) );
	wc.hIconSm		 = wc.hIcon;

	RegisterClassEx( &wc );

	// Note, it's hidden
	DWORD style = WS_POPUP | WS_CLIPSIBLINGS;

	if ( bWindowed )
	{
		// Give it a frame
		style |= WS_OVERLAPPEDWINDOW;
		style &= ~WS_THICKFRAME;
	}

	// Never a max box
	style &= ~WS_MAXIMIZEBOX;

	RECT windowRect;
	windowRect.top		= 0;
	windowRect.left		= 0;
	windowRect.right	= w;
	windowRect.bottom	= h;

	// Compute rect needed for that size client area based on window style
	AdjustWindowRectEx(&windowRect, style, FALSE, 0);

	// Create the window
	void *hWnd = CreateWindow( wc.lpszClassName, pTitle, style, 0, 0, 
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 
		NULL, NULL, (HINSTANCE)GetAppInstance(), NULL );

	if (!hWnd)
		return NULL;

	int     CenterX, CenterY;

	CenterX = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	CenterY = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
	CenterX = (CenterX < 0) ? 0: CenterX;
	CenterY = (CenterY < 0) ? 0: CenterY;

	// In VCR modes, keep it in the upper left so mouse coordinates are always relative to the window.
	SetWindowPos( (HWND)hWnd, NULL, CenterX, CenterY, 0, 0,
		SWP_NOSIZE | SWP_NOZORDER /* | SWP_SHOWWINDOW | SWP_DRAWFRAME*/ );

	return hWnd;
}


//-----------------------------------------------------------------------------
// Purpose: Main Startup
//-----------------------------------------------------------------------------
int CVsMayaMasterApp::Startup()
{
	if ( BaseClass::Startup() < 0 )
		return -1;

	// Create an invisible main window to draw into
	if ( g_pMaterialSystem )
	{
		g_pMaterialSystem->ModInit();
		if ( IsInteractive() )
		{
			MaterialVideoMode_t mode;
			g_pMaterialSystem->GetDisplayMode( mode );

			void *hWnd = Create3DWindow( "Valve3DWindow", true, mode.m_Width, mode.m_Height );

			MaterialSystem_Config_t config;
			config.SetFlag( MATSYS_VIDCFG_FLAGS_WINDOWED, true );
			config.SetFlag( MATSYS_VIDCFG_FLAGS_RESIZING, true );
			config.m_nAASamples = 1;
			config.m_VideoMode.m_Width = config.m_VideoMode.m_Height = 0;
			config.m_VideoMode.m_Format = IMAGE_FORMAT_BGRX8888;
			config.m_VideoMode.m_RefreshRate = 0;
			config.SetFlag( MATSYS_VIDCFG_FLAGS_LIMIT_WINDOWED_SIZE, true );
			g_pMaterialSystem->SetMode( hWnd, config );
		}
	}

	if ( g_pDataCache )
	{
		g_pDataCache->SetSize( 64 * 1024 * 1024 );
	}

	return 0;
}


void CVsMayaMasterApp::Shutdown()
{
	if ( g_pMaterialSystem )
	{
		g_pMaterialSystem->ModShutdown();
	}

	BaseClass::Shutdown();
}


//-----------------------------------------------------------------------------
// Purpose: Disconnect all Valve DLLs
//-----------------------------------------------------------------------------
void CVsMayaMasterApp::PostShutdown()
{
	// Iterate through all of the Maya plugins in the order they were loaded and call VsMayaDisconnect() on them
	for ( int mi = 0; mi < m_modules.GetNumStrings(); ++mi )
	{
		const FARPROC pVsMayaDisconnect( GetProcAddress( m_modules[mi], "VsMayaDisconnect" ) );
		if ( pVsMayaDisconnect )
		{
			void ( *VsMayaDisconnect )()( ( void (*)() )( pVsMayaDisconnect ) );
			( *VsMayaDisconnect )();
		}
	}

	ShutdownDataModel();
	DisconnectDataModel();

	DisconnectTier3Libraries( );
	DisconnectTier2Libraries( );
	DisconnectTier1Libraries( );
}


//-----------------------------------------------------------------------------
// Purpose: Unload all subsidiary plug-ins
//-----------------------------------------------------------------------------
void CVsMayaMasterApp::Destroy()
{
	// Iterate through all of the Maya plugins in the order they were loaded and unload them
	for ( int mi = 0; mi < m_modules.GetNumStrings(); ++mi )
	{
		MString melCmd;
		melCmd = "catchQuiet( unloadPlugin(\"";
		melCmd += m_modules.String( mi );
		melCmd += "\") );";

		MGlobal::executeCommand( melCmd, false, false );
	}
}

