//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#if defined( _WIN32 ) && !defined( _X360 )
#include "winlite.h"
#endif
#include "quakedef.h"
#include "idedicatedexports.h"
#include "engine_launcher_api.h"
#include "ivideomode.h"
#include "common.h"
#include "iregistry.h"
#include "keys.h"
#include "cdll_engine_int.h"
#include "traceinit.h"
#include "iengine.h"
#include "igame.h"
#include "tier0/vcrmode.h"
#include "engine_hlds_api.h"
#include "filesystem_engine.h"
#include "tier0/icommandline.h"
#include "cl_main.h"
#include "client.h"
#include "tier3/tier3.h"
#include "MapReslistGenerator.h"
#include "toolframework/itoolframework.h"
#include "DevShotGenerator.h"
#include "gl_shader.h"
#include "l_studio.h"
#include "IHammer.h"
#include "sys_dll.h"
#include "materialsystem/materialsystem_config.h"
#include "server.h"
#include "avi/iavi.h"
#include "avi/ibik.h"
#include "datacache/idatacache.h"
#include "vphysics_interface.h"
#include "inputsystem/iinputsystem.h"
#include "appframework/IAppSystemGroup.h"
#include "tier0/systeminformation.h"
#ifdef _WIN32
#include "vguimatsurface/imatsystemsurface.h"
#endif

// This is here just for legacy support of older .dlls!!!
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "eiface.h"

#ifndef SWDS
#include "sys_mainwind.h"
#include "vgui/isystem.h"
#include "vgui_controls/controls.h"
#include "igameuifuncs.h"
#include "cl_steamauth.h"

#endif // SWDS

#if defined(_WIN32)
#include <eh.h>
#endif

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#else
#include "xbox/xboxstubs.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
IDedicatedExports *dedicated = NULL;
extern CreateInterfaceFn g_AppSystemFactory;
IHammer *g_pHammer = NULL;
IPhysics *g_pPhysics = NULL;
IAvi *avi = NULL;
IBik *bik = NULL;
#ifndef SWDS
extern CreateInterfaceFn g_ClientFactory;
#endif

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
void Host_GetHostInfo(float *fps, int *nActive, int *nMaxPlayers, char *pszMap, int maxlen );
const char *Key_BindingForKey( int keynum );
void COM_ShutdownFileSystem( void );
void COM_InitFilesystem( const char *pFullModPath );
void Host_ReadPreStartupConfiguration();
void EditorToggle_f();

#ifdef _WIN32
HWND *pmainwindow = NULL;
#endif

//-----------------------------------------------------------------------------
// ConVars and console commands
//-----------------------------------------------------------------------------
#if !defined(SWDS)
static ConCommand editor_toggle( "editor_toggle", EditorToggle_f, "Disables the simulation and returns focus to the editor", FCVAR_CHEAT );
#endif



#ifndef SWDS
//-----------------------------------------------------------------------------
// Purpose: exports an interface that can be used by the launcher to run the engine
//			this is the exported function when compiled as a blob
//-----------------------------------------------------------------------------
void EXPORT F( IEngineAPI **api )
{
	CreateInterfaceFn factory = Sys_GetFactoryThis();	// This silly construction is necessary to prevent the LTCG compiler from crashing.
	*api = ( IEngineAPI * )(factory(VENGINE_LAUNCHER_API_VERSION, NULL));
}
#endif // SWDS

extern bool cs_initialized;
extern int			lowshift;
static char	*empty_string = "";

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
extern void SCR_UpdateScreen(void);
extern bool g_bMajorMapChange; 
extern bool g_bPrintingKeepAliveDots;

void Sys_ShowProgressTicks(char* specialProgressMsg)
{
#ifdef LATER
#define MAX_NUM_TICS 40

	static long numTics = 0;

	// Nothing to do if not using Steam
	if ( !g_pFileSystem->IsSteam() )
		return;

	// Update number of tics to show...
	numTics++;
	if ( isDedicated )
	{
		if ( g_bMajorMapChange )
		{
			g_bPrintingKeepAliveDots = TRUE;
			Msg(".");
		}
	}
	else
	{
		int i;
		int numTicsToPrint = numTics % (MAX_NUM_TICS-1);
		char msg[MAX_NUM_TICS+1];

		Q_strncpy(msg, ".", sizeof(msg));

		// Now add in the growing number of tics...
		for ( i = 1 ; i < numTicsToPrint ; i++ )
		{
			Q_strncat(msg, ".", sizeof(msg), COPY_ALL_CHARACTERS);
		}

		SCR_UpdateScreen();
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ClearIOStates( void )
{
#ifndef SWDS
	if ( g_ClientDLL ) 
	{
		g_ClientDLL->IN_ClearStates();
	}
#endif
}

void MoveConsoleWindowToFront()
{
#ifdef _WIN32
// TODO: remove me!!!!!

	// Move the window to the front.
	HINSTANCE hInst = LoadLibrary( "kernel32.dll" );
	if ( hInst )
	{
		typedef HWND (*GetConsoleWindowFn)();
		GetConsoleWindowFn fn = (GetConsoleWindowFn)GetProcAddress( hInst, "GetConsoleWindow" );
		if ( fn )
		{
			HWND hwnd = fn();
			ShowWindow( hwnd, SW_SHOW );
			UpdateWindow( hwnd );
			SetWindowPos( hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );
		}
		FreeLibrary( hInst );
	}
#endif
}

#if defined( _WIN32 ) && !defined( _X360 )
#include <conio.h>
#endif
CUtlVector<char> g_TextModeLine;

char NextGetch()
{
	return -1;
	// NOTE: for some reason, kbhit() KILLS performance on the client.. when using it, the client
	// goes so slow that it's player's motion is all jerky. If we need input, probably the
	// best thing to do is to hook the console window's wndproc and get the keydown messages.
	/*
	// Sort of hacky to overload the gamemsg loop with these messages, but it does the trick.
	if ( VCRGetMode() == VCR_Playback )
	{
		unsigned int uMsg, wParam;
		long lParam;
		if ( VCRHook_PlaybackGameMsg( uMsg, wParam, lParam ) )
		{
			Assert( uMsg == 0xFFFF );
			return (char)wParam;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if ( kbhit() )
		{
			char ch = getch();
			VCRHook_RecordGameMsg( 0xFFFF, ch, 0 );
			return ch;
		}
		else
		{
			VCRHook_RecordEndGameMsg();
			return -1;
		}
	}
	*/
}

void EatTextModeKeyPresses()
{
	if ( !g_bTextMode )
		return;
	
	static bool bFirstRun = true;
	if ( bFirstRun )
	{
		bFirstRun = false;
		MoveConsoleWindowToFront();
	}

	char ch;
	while ( (ch = NextGetch()) != -1 )
	{
		if ( ch == 8 )
		{
			// Backspace..
			if ( g_TextModeLine.Count() )
			{
				g_TextModeLine.Remove( g_TextModeLine.Count() - 1 );
			}
		}
		else if ( ch == '\r' )
		{
			// Finish the line.
			if ( g_TextModeLine.Count() )
			{
				g_TextModeLine.AddMultipleToTail( 2, "\n" );
				Cbuf_AddText( g_TextModeLine.Base() );
				g_TextModeLine.Purge();
			}
			printf( "\n" );
		}
		else
		{
			g_TextModeLine.AddToTail( ch );
		}

		printf( "%c", ch );
	}	
}


//-----------------------------------------------------------------------------
// The SDK launches the game with the full path to gameinfo.txt, so we need
// to strip off the path.
//-----------------------------------------------------------------------------
const char *GetModDirFromPath( const char *pszPath )
{
	char *pszSlash = Q_strrchr( pszPath, '\\' );
	if ( pszSlash )
	{
		return pszSlash + 1;
	}
	else if ( ( pszSlash  = Q_strrchr( pszPath, '/' ) ) != NULL )
	{
		return pszSlash + 1;
	}

	// Must just be a mod directory already.
	return pszPath;
}

//-----------------------------------------------------------------------------
// Purpose: Main entry
//-----------------------------------------------------------------------------
#ifndef SWDS
#include "gl_matsysiface.h"
#endif

//-----------------------------------------------------------------------------
// Inner loop: initialize, shutdown main systems, load steam to 
//-----------------------------------------------------------------------------
class CModAppSystemGroup : public CAppSystemGroup
{
	typedef CAppSystemGroup BaseClass;
public:
	// constructor
	CModAppSystemGroup( bool bServerOnly, CAppSystemGroup *pParentAppSystem = NULL )
		: BaseClass( pParentAppSystem ),
		m_bServerOnly( bServerOnly )
	{
	}

	CreateInterfaceFn GetFactory()
	{
		return CAppSystemGroup::GetFactory();
	}

	// Methods of IApplication
	virtual bool Create();
	virtual bool PreInit();
	virtual int Main();
	virtual void PostShutdown();
	virtual void Destroy();

private:

	bool IsServerOnly() const
	{
		return m_bServerOnly;
	}
	bool ModuleAlreadyInList( CUtlVector< AppSystemInfo_t >& list, const char *moduleName, const char *interfaceName );

	bool AddLegacySystems();
	bool	m_bServerOnly;
};

#ifndef SWDS
//-----------------------------------------------------------------------------
//
// Main engine interface exposed to launcher
//
//-----------------------------------------------------------------------------
class CEngineAPI : public CTier3AppSystem< IEngineAPI >
{
	typedef CTier3AppSystem< IEngineAPI > BaseClass;

public:
	virtual bool Connect( CreateInterfaceFn factory );
	virtual void Disconnect();
	virtual void *QueryInterface( const char *pInterfaceName );
	virtual InitReturnVal_t Init();
	virtual void Shutdown();

	// This function must be called before init
	virtual void SetStartupInfo( StartupInfo_t &info );

	virtual int Run( );

	// Sets the engine to run in a particular editor window
	virtual void SetEngineWindow( void *hWnd );

	// Posts a console command
	virtual void PostConsoleCommand( const char *pConsoleCommand );

	// Are we running the simulation?
	virtual bool IsRunningSimulation( ) const;

	// Start/stop running the simulation
	virtual void ActivateSimulation( bool bActive );

	// Reset the map we're on
	virtual void SetMap( const char *pMapName );

	bool MainLoop();

private:
	int RunListenServer();

	// Hooks a particular mod up to the registry
	void SetRegistryMod( const char *pModName );

	// One-time setup, based on the initially selected mod
	// FIXME: This should move into the launcher!
	bool OnStartup( void *pInstance, const char *pStartupModName );
	void OnShutdown();

	// Initialization, shutdown of a mod.
	bool ModInit( const char *pModName, const char *pGameDir );
	void ModShutdown();

	// Initializes, shuts down the registry
	bool InitRegistry( const char *pModName );
	void ShutdownRegistry();

	// Handles there being an error setting up the video mode
	InitReturnVal_t HandleSetModeError();

	// Purpose: Message pump when running stand-alone
	void PumpMessages();

	// Purpose: Message pump when running with the editor
	void PumpMessagesEditMode( bool &bIdle, long &lIdleCount );

	// Activate/deactivates edit mode shaders
	void ActivateEditModeShaders( bool bActive );

private:
	void *m_hEditorHWnd;
	bool m_bRunningSimulation;
	StartupInfo_t m_StartupInfo;
};


//-----------------------------------------------------------------------------
// Singleton interface
//-----------------------------------------------------------------------------
static CEngineAPI s_EngineAPI;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CEngineAPI, IEngineAPI, VENGINE_LAUNCHER_API_VERSION, s_EngineAPI );


//-----------------------------------------------------------------------------
// Connect, disconnect
//-----------------------------------------------------------------------------
bool CEngineAPI::Connect( CreateInterfaceFn factory ) 
{ 
	// Store off the app system factory...
	g_AppSystemFactory = factory;

	if ( !BaseClass::Connect( factory ) )
		return false;

	g_pFileSystem = g_pFullFileSystem;
	if ( !g_pFileSystem )
		return false;

	g_pFileSystem->SetWarningFunc( Warning );

	if ( !Shader_Connect( true ) )
		return false;

	g_pPhysics = (IPhysics*)factory( VPHYSICS_INTERFACE_VERSION, NULL );

	if ( IsPC() )
	{
		avi = (IAvi*)factory( AVI_INTERFACE_VERSION, NULL );
		if ( !avi )
			return false;
	}

	bik = (IBik*)factory( BIK_INTERFACE_VERSION, NULL );
	if ( !bik )
		return false;
	
	if ( !g_pStudioRender || !g_pDataCache || !g_pPhysics || !g_pMDLCache || !g_pMatSystemSurface || !g_pInputSystem )
	{
		Warning( "Engine wasn't able to acquire required interfaces!\n" );
		return false;
	}

	if (!g_pStudioRender)
	{
		Sys_Error( "Unable to init studio render system version %s\n", STUDIO_RENDER_INTERFACE_VERSION );
		return false;
	}

	g_pHammer = (IHammer*)factory( INTERFACEVERSION_HAMMER, NULL );

	ConnectMDLCacheNotify();

	return true; 
}

void CEngineAPI::Disconnect() 
{
	DisconnectMDLCacheNotify();

	g_pHammer = NULL;
	g_pPhysics = NULL;

	Shader_Disconnect();

	g_pFileSystem = NULL;

	BaseClass::Disconnect();

	g_AppSystemFactory = NULL;
}


//-----------------------------------------------------------------------------
// Query interface
//-----------------------------------------------------------------------------
void *CEngineAPI::QueryInterface( const char *pInterfaceName )
{
	// Loading the engine DLL mounts *all* engine interfaces
	CreateInterfaceFn factory = Sys_GetFactoryThis();	// This silly construction is necessary
	return factory( pInterfaceName, NULL );				// to prevent the LTCG compiler from crashing.
}


//-----------------------------------------------------------------------------
// Sets startup info
//-----------------------------------------------------------------------------
void CEngineAPI::SetStartupInfo( StartupInfo_t &info ) 
{
	g_bTextMode = info.m_bTextMode;

	// Set up the engineparms_t which contains global information about the mod
	host_parms.basedir = const_cast<char*>( info.m_pBaseDirectory );

	// Copy off all the startup info
	m_StartupInfo = info;

	// Needs to be done prior to init material system config
	TRACEINIT( COM_InitFilesystem( m_StartupInfo.m_pInitialMod ), COM_ShutdownFileSystem() );
}


//-----------------------------------------------------------------------------
// Init, shutdown
//-----------------------------------------------------------------------------
InitReturnVal_t CEngineAPI::Init() 
{
	InitReturnVal_t nRetVal = BaseClass::Init();
	if ( nRetVal != INIT_OK )
		return nRetVal;

	m_bRunningSimulation = false;

	// Initialize the FPU control word
#if !defined( SWDS ) && !defined( _X360 )
	_asm
	{
		fninit
	}
#endif

	SetupFPUControlWord();

	// This creates the videomode singleton object, it doesn't depend on the registry
	VideoMode_Create();

	// Initialize the editor hwnd to render into
	m_hEditorHWnd = NULL;

	// One-time setup
	// FIXME: OnStartup + OnShutdown should be removed + moved into the launcher
	// or the launcher code should be merged into the engine into the code in OnStartup/OnShutdown
	if ( !OnStartup( m_StartupInfo.m_pInstance, m_StartupInfo.m_pInitialMod ) )
	{
		return HandleSetModeError();
	}

	return INIT_OK; 
}

void CEngineAPI::Shutdown() 
{
	VideoMode_Destroy();
	BaseClass::Shutdown();
}


//-----------------------------------------------------------------------------
// Sets the engine to run in a particular editor window
//-----------------------------------------------------------------------------
void CEngineAPI::SetEngineWindow( void *hWnd )
{
	if ( !InEditMode() )
		return;

	// Detach input from the previous editor window
	game->InputDetachFromGameWindow();

	m_hEditorHWnd = hWnd;
	videomode->SetGameWindow( m_hEditorHWnd );
}


//-----------------------------------------------------------------------------
// Posts a console command
//-----------------------------------------------------------------------------
void CEngineAPI::PostConsoleCommand( const char *pCommand )
{
	Cbuf_AddText( pCommand );
}

	
//-----------------------------------------------------------------------------
// Is the engine currently rinning?
//-----------------------------------------------------------------------------
bool CEngineAPI::IsRunningSimulation() const
{
	return (eng->GetState() == IEngine::DLL_ACTIVE);
}


//-----------------------------------------------------------------------------
// Reset the map we're on
//-----------------------------------------------------------------------------
void CEngineAPI::SetMap( const char *pMapName )
{
//	if ( !Q_stricmp( sv.mapname, pMapName ) )
//		return;

	char buf[MAX_PATH];
	Q_snprintf( buf, MAX_PATH, "map %s", pMapName );
	Cbuf_AddText( buf );
}


//-----------------------------------------------------------------------------
// Start/stop running the simulation
//-----------------------------------------------------------------------------
void CEngineAPI::ActivateSimulation( bool bActive )
{
	// FIXME: Not sure what will happen in this case
	if ( ( eng->GetState() != IEngine::DLL_ACTIVE )	&&
		 ( eng->GetState() != IEngine::DLL_PAUSED ) )
	{
		return;
	}

	bool bCurrentlyActive = (eng->GetState() != IEngine::DLL_PAUSED);
	if ( bActive == bCurrentlyActive )
		return;

	// FIXME: Should attachment/detachment be part of the state machine in IEngine?
	if ( !bActive )
	{
		eng->SetNextState( IEngine::DLL_PAUSED );

		// Detach input from the previous editor window
		game->InputDetachFromGameWindow();
	}
	else
	{
		eng->SetNextState( IEngine::DLL_ACTIVE );

		// Start accepting input from the new window
		// FIXME: What if the attachment fails?
		game->InputAttachToGameWindow();
	}
}

	
//-----------------------------------------------------------------------------
// Purpose: Message pump when running stand-alone
//-----------------------------------------------------------------------------
void CEngineAPI::PumpMessages()
{
	MSG msg;
	while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// Get input from attached devices
	g_pInputSystem->PollInputState();

	if ( IsX360() )
	{
		// handle Xbox system messages
		XBX_ProcessEvents();
	}

	// NOTE: Under some implementations of Win9x, 
	// dispatching messages can cause the FPU control word to change
	if ( IsPC() )
	{
		SetupFPUControlWord();
	}

	game->DispatchAllStoredGameMessages();

	if ( IsPC() )
	{
		EatTextModeKeyPresses();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Message pump when running stand-alone
//-----------------------------------------------------------------------------
void CEngineAPI::PumpMessagesEditMode( bool &bIdle, long &lIdleCount )
{
	MSG msg;

	if ( bIdle && !g_pHammer->HammerOnIdle( lIdleCount++ ) )
	{
		bIdle = false;
	}

	// Get input from attached devices
	g_pInputSystem->PollInputState();

	while ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		if ( msg.message == WM_QUIT )
		{
			eng->SetQuitting( IEngine::QUIT_TODESKTOP );
			break;
		}

		if ( !g_pHammer->HammerPreTranslateMessage(&msg) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Reset idle state after pumping idle message.
		if ( g_pHammer->HammerIsIdleMessage(&msg) )
		{
			bIdle = true;
			lIdleCount = 0;
		}
	}

	// NOTE: Under some implementations of Win9x, 
	// dispatching messages can cause the FPU control word to change
	SetupFPUControlWord();

	game->DispatchAllStoredGameMessages();
}

//-----------------------------------------------------------------------------
// Activate/deactivates edit mode shaders
//-----------------------------------------------------------------------------
void CEngineAPI::ActivateEditModeShaders( bool bActive )
{
	if ( InEditMode() && ( g_pMaterialSystemConfig->bEditMode != bActive ) )
	{
		MaterialSystem_Config_t config = *g_pMaterialSystemConfig;
		config.bEditMode = bActive;
		OverrideMaterialSystemConfig( config );
	}
}


//-----------------------------------------------------------------------------
// Purpose: Message pump
//-----------------------------------------------------------------------------
bool CEngineAPI::MainLoop()
{
	bool bIdle = true;
	long lIdleCount = 0;

	// Main message pump
	while ( true )
	{
		// Pump messages unless someone wants to quit
		if ( eng->GetQuitting() != IEngine::QUIT_NOTQUITTING )
		{
			if ( eng->GetQuitting() != IEngine::QUIT_TODESKTOP )
				return true;
			return false;
		}

		// Pump the message loop
		if ( !InEditMode() )
		{
			PumpMessages();
		}
		else
		{
			PumpMessagesEditMode( bIdle, lIdleCount );
		}

		// Run engine frame + hammer frame
		if ( !InEditMode() || m_hEditorHWnd )
		{
			VCRSyncToken( "Frame" );

			// Deactivate edit mode shaders
			ActivateEditModeShaders( false );

			eng->Frame();

			// Reactivate edit mode shaders (in Edit mode only...)
			ActivateEditModeShaders( true );
		}

		if ( InEditMode() )
		{
			g_pHammer->RunFrame();
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
// Initializes, shuts down the registry
//-----------------------------------------------------------------------------
bool CEngineAPI::InitRegistry( const char *pModName )
{
	if ( IsPC() )
	{
		char szRegSubPath[MAX_PATH];
		Q_snprintf( szRegSubPath, sizeof(szRegSubPath), "%s\\%s", "Source", pModName );
		return registry->Init( szRegSubPath );
	}
	return true;
}

void CEngineAPI::ShutdownRegistry( )
{
	if ( IsPC() )
	{
		registry->Shutdown( );
	}
}


//-----------------------------------------------------------------------------
// One-time setup, based on the initially selected mod
// FIXME: This should move into the launcher!
//-----------------------------------------------------------------------------
bool CEngineAPI::OnStartup( void *pInstance, const char *pStartupModName )
{
	// This fixes a bug on certain machines where the input will 
	// stop coming in for about 1 second when someone hits a key.
	// (true means to disable priority boost)
	if ( IsPC() )
	{
		SetThreadPriorityBoost( GetCurrentThread(), true ); 
	}

	// FIXME: Turn videomode + game into IAppSystems?

	// Try to create the window
	COM_TimestampedLog( "game->Init" );

	// This has to happen before CreateGameWindow to set up the instance
	// for use by the code that creates the window
	if ( !game->Init( pInstance ) )
	{
		goto onStartupError;
	}

	// Try to create the window
	COM_TimestampedLog( "videomode->Init" );

	// This needs to be after Shader_Init and registry->Init
	// This way mods can have different default video settings
	if ( !videomode->Init( ) )
	{
		goto onStartupShutdownGame;
	}
	
	// We need to access the registry to get various settings (specifically,
	// InitMaterialSystemConfig requires it).
	if ( !InitRegistry( pStartupModName ) )
	{
		goto onStartupShutdownVideoMode;
	}

	materials->ModInit();

	// Setup the material system config record, CreateGameWindow depends on it
	// (when we're running stand-alone)
	InitMaterialSystemConfig( InEditMode() );

#if defined( _X360 )
	XBX_NotifyCreateListener( XNOTIFY_SYSTEM|XNOTIFY_LIVE|XNOTIFY_XMP );
#endif

	ShutdownRegistry();
	return true;

	// Various error conditions
onStartupShutdownVideoMode:
	videomode->Shutdown();

onStartupShutdownGame:
	game->Shutdown();

onStartupError:
	return false;
}


//-----------------------------------------------------------------------------
// One-time shutdown (shuts down stuff set up in OnStartup)
// FIXME: This should move into the launcher!
//-----------------------------------------------------------------------------
void CEngineAPI::OnShutdown()
{
	if ( videomode )
	{
		videomode->Shutdown();
	}

	// Shut down the game
	game->Shutdown();

	materials->ModShutdown();
	TRACESHUTDOWN( COM_ShutdownFileSystem() );
}

static bool IsValveMod( const char *pModName )
{
	// Figure out if we're running a Valve mod or not.
	return ( Q_stricmp( GetCurrentMod(), "cstrike" ) == 0 ||
		Q_stricmp( GetCurrentMod(), "dod" ) == 0 ||
		Q_stricmp( GetCurrentMod(), "hl1mp" ) == 0 ||
		Q_stricmp( GetCurrentMod(), "tf" ) == 0 ||
		Q_stricmp( GetCurrentMod(), "hl2mp" ) == 0 );
}

//-----------------------------------------------------------------------------
// Initialization, shutdown of a mod.
//-----------------------------------------------------------------------------
bool CEngineAPI::ModInit( const char *pModName, const char *pGameDir )
{
	// Set up the engineparms_t which contains global information about the mod
	host_parms.mod = COM_StringCopy( GetModDirFromPath( pModName ) );
	host_parms.game = COM_StringCopy( pGameDir );

	// By default, restrict server commands in Valve games and don't restrict them in mods.
	cl.m_bRestrictServerCommands = IsValveMod( host_parms.mod );
	cl.m_bRestrictClientCommands = cl.m_bRestrictServerCommands;

	// build the registry path we're going to use for this mod
	InitRegistry( pModName );

	// This sets up the game search path, depends on host_parms
	TRACEINIT( MapReslistGenerator_Init(), MapReslistGenerator_Shutdown() );
#if !defined( _X360 )
	TRACEINIT( DevShotGenerator_Init(), DevShotGenerator_Shutdown() );
#endif

	// Slam cvars based on mod/config.cfg
	Host_ReadPreStartupConfiguration();

	// Create the game window now that we have a search path
	// FIXME: Deal with initial window width + height better
	if ( !videomode || !videomode->CreateGameWindow( g_pMaterialSystemConfig->m_VideoMode.m_Width, g_pMaterialSystemConfig->m_VideoMode.m_Height, g_pMaterialSystemConfig->Windowed() ) )
	{
		return false;
	}

	return true;
}

void CEngineAPI::ModShutdown()
{
	COM_StringFree(host_parms.mod);
	COM_StringFree(host_parms.game);
	
	// Stop accepting input from the window
	game->InputDetachFromGameWindow();

#if !defined( _X360 )
	TRACESHUTDOWN( DevShotGenerator_Shutdown() );
#endif
	TRACESHUTDOWN( MapReslistGenerator_Shutdown() );

	ShutdownRegistry();
}


//-----------------------------------------------------------------------------
// Purpose: Handles there being an error setting up the video mode
// Output : Returns true on if the engine should restart, false if it should quit
//-----------------------------------------------------------------------------
InitReturnVal_t CEngineAPI::HandleSetModeError()
{
	// show an error, see if the user wants to restart
	if ( CommandLine()->FindParm( "-safe" ) )
	{
		Sys_MessageBox( "Failed to set video mode.\n\nThis game has a minimum requirement of DirectX 7.0 compatible hardware.\n", "Video mode error", false );
		return INIT_FAILED;
	}
	
	if ( CommandLine()->FindParm( "-autoconfig" ) )
	{
		if ( Sys_MessageBox( "Failed to set video mode - falling back to safe mode settings.\n\nGame will now restart with the new video settings.", "Video - safe mode fallback", true ))
		{
			CommandLine()->AppendParm( "-safe", NULL );
			return (InitReturnVal_t)INIT_RESTART;
		}
		return INIT_FAILED;
	}

	if ( Sys_MessageBox( "Failed to set video mode - resetting to defaults.\n\nGame will now restart with the new video settings.", "Video mode warning", true ) )
	{
		CommandLine()->AppendParm( "-autoconfig", NULL );
		return (InitReturnVal_t)INIT_RESTART;
	}

	return INIT_FAILED;
}


//-----------------------------------------------------------------------------
// Purpose: Main loop for non-dedicated servers
//-----------------------------------------------------------------------------
int CEngineAPI::RunListenServer()
{
	//
	// NOTE: Systems set up here should depend on the mod 
	// Systems which are mod-independent should be set up in the launcher or Init()
	//

	// Innocent until proven guilty
	int nRunResult = RUN_OK;

	// Happens every time we start up and shut down a mod
	if ( ModInit( m_StartupInfo.m_pInitialMod, m_StartupInfo.m_pInitialGame ) )
	{
		CModAppSystemGroup modAppSystemGroup( false, m_StartupInfo.m_pParentAppSystemGroup );

		// Store off the app system factory...
		g_AppSystemFactory = modAppSystemGroup.GetFactory();

		nRunResult = modAppSystemGroup.Run();

		g_AppSystemFactory = NULL;

		// Shuts down the mod
		ModShutdown();

		// Disconnects from the editor window
		videomode->SetGameWindow( NULL );
	}

	// Closes down things that were set up in OnStartup
	// FIXME: OnStartup + OnShutdown should be removed + moved into the launcher
	// or the launcher code should be merged into the engine into the code in OnStartup/OnShutdown
	OnShutdown();

	return nRunResult;
}

#if 0
CON_COMMAND( bigalloc, "huge alloc crash" )
{
	Msg( "pre-crash %d\n", g_pMemAlloc->MemoryAllocFailed() );
	void *buf = malloc( UINT_MAX );
	Msg( "post-alloc %d\n", g_pMemAlloc->MemoryAllocFailed() );
	*(int *)buf = 0;
}
#endif

extern void S_ClearBuffer();
extern char g_minidumpinfo[ 4096 ];
extern PAGED_POOL_INFO_t g_pagedpoolinfo;
extern bool g_bUpdateMinidumpComment;
static char errorText[4096];
static char ppi[ 512 ];
static PAGED_POOL_INFO_t final;

extern "C" void __cdecl WriteMiniDumpUsingExceptionInfo( unsigned int uStructuredExceptionCode, struct _EXCEPTION_POINTERS * pExceptionInfo	)
{
	// TODO: dynamically set the minidump comment from contextual info about the crash (i.e current VPROF node)?
#if !defined( NO_STEAM )

	if ( g_bUpdateMinidumpComment  )
	{
		try 
		{
			V_strcpy(errorText,"Crash: ");
			V_strncat( errorText, g_minidumpinfo, sizeof(errorText) );
			
			Plat_GetPagedPoolInfo( &final );

			V_snprintf( ppi, sizeof(ppi), "prev PP PAGES: used: %d, free %d\nfinal PP PAGES: used: %d, free %d\nmemalloc = %u\n", g_pagedpoolinfo.numPagesUsed, g_pagedpoolinfo.numPagesFree, final.numPagesUsed, final.numPagesFree, g_pMemAlloc->MemoryAllocFailed() );
			V_strncat( errorText, ppi, sizeof(errorText) );

			SteamAPI_SetMiniDumpComment( errorText );
		}
		catch ( ... )
		{
			// Oh oh
		}
	}

	SteamAPI_WriteMiniDump( uStructuredExceptionCode, pExceptionInfo, build_number() );
	// Clear DSound Buffers so the sound doesn't loop while the game shuts down
	try
	{
		S_ClearBuffer();
	}
	catch ( ... )
	{
	}
#endif
} 


//-----------------------------------------------------------------------------
// Purpose: Main 
//-----------------------------------------------------------------------------
int CEngineAPI::Run()
{
#ifdef _X360
	return RunListenServer(); // don't handle exceptions on 360 (because if we do then minidumps won't work at all)
#elif defined ( _WIN32 )
	if ( !Plat_IsInDebugSession() && !CommandLine()->FindParm( "-nominidumps") )
	{
		_set_se_translator( WriteMiniDumpUsingExceptionInfo );

		try  // this try block allows the SE translator to work
		{
			return RunListenServer();
		}
		catch( ... )
		{
			return RUN_OK;
		}
	}
	else
	{
		return RunListenServer();
	}
#else
	return RUN_OK; // linux doesn't do this
#endif
}
#endif // SWDS

bool g_bUsingLegacyAppSystems = false;

bool CModAppSystemGroup::AddLegacySystems()
{
	g_bUsingLegacyAppSystems = true;

	AppSystemInfo_t appSystems[] = 
	{
		{ "soundemittersystem", SOUNDEMITTERSYSTEM_INTERFACE_VERSION },
		{ "", "" }					// Required to terminate the list
	};

	if ( !AddSystems( appSystems ) ) 
		return false;

#if !defined( _LINUX )
//	if ( CommandLine()->FindParm( "-tools" ) )
	{
		AppModule_t toolFrameworkModule = LoadModule( "engine.dll" );
		if ( !AddSystem( toolFrameworkModule, VTOOLFRAMEWORK_INTERFACE_VERSION ) )
			return false;
	}
#endif

	return true;
}

//-----------------------------------------------------------------------------
// Instantiate all main libraries
//-----------------------------------------------------------------------------
bool CModAppSystemGroup::Create()
{
#ifndef SWDS
	if ( !IsServerOnly() )
	{
		if ( !ClientDLL_Load() )
			return false;
	}
#endif

	if ( !ServerDLL_Load() )
		return false;

	IClientDLLSharedAppSystems *clientSharedSystems = 0;

#ifndef SWDS
	if ( !IsServerOnly() )
	{
		clientSharedSystems = ( IClientDLLSharedAppSystems * )g_ClientFactory( CLIENT_DLL_SHARED_APPSYSTEMS, NULL );
		if ( !clientSharedSystems )
			return AddLegacySystems();
	}
#endif

	IServerDLLSharedAppSystems *serverSharedSystems = ( IServerDLLSharedAppSystems * )g_ServerFactory( SERVER_DLL_SHARED_APPSYSTEMS, NULL );
	if ( !serverSharedSystems )
	{
		Assert( !"Expected both game and client .dlls to have or not have shared app systems interfaces!!!" );
		return AddLegacySystems();
	}

	// Load game and client .dlls and build list then
	CUtlVector< AppSystemInfo_t >	systems;

	int i;
	int serverCount = serverSharedSystems->Count();
	for ( i = 0 ; i < serverCount; ++i )
	{
		const char *dllName = serverSharedSystems->GetDllName( i );
		const char *interfaceName = serverSharedSystems->GetInterfaceName( i );

		AppSystemInfo_t info;
		info.m_pModuleName = dllName;
		info.m_pInterfaceName = interfaceName;

		systems.AddToTail( info );
	}

	if ( !IsServerOnly() )
	{
		int clientCount = clientSharedSystems->Count();
		for ( i = 0 ; i < clientCount; ++i )
		{
			const char *dllName = clientSharedSystems->GetDllName( i );
			const char *interfaceName = clientSharedSystems->GetInterfaceName( i );

			if ( ModuleAlreadyInList( systems, dllName, interfaceName ) )
				continue;

			AppSystemInfo_t info;
			info.m_pModuleName = dllName;
			info.m_pInterfaceName = interfaceName;

			systems.AddToTail( info );
		}
	}

	AppSystemInfo_t info;
	info.m_pModuleName = "";
	info.m_pInterfaceName = "";
	systems.AddToTail( info );

	if ( !AddSystems( systems.Base() ) ) 
		return false;

#if !defined( _LINUX )
//	if ( CommandLine()->FindParm( "-tools" ) )
	{
		AppModule_t toolFrameworkModule = LoadModule( "engine.dll" );
		if ( !AddSystem( toolFrameworkModule, VTOOLFRAMEWORK_INTERFACE_VERSION ) )
			return false;
	}
#endif

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Fixme, we might need to verify if the interface names differ for the client versus the server
// Input  : list - 
//			*moduleName - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CModAppSystemGroup::ModuleAlreadyInList( CUtlVector< AppSystemInfo_t >& list, const char *moduleName, const char *interfaceName )
{
	for ( int i = 0; i < list.Count(); ++i )
	{
		if ( !Q_stricmp( list[ i ].m_pModuleName, moduleName ) )
		{
			if ( Q_stricmp( list[ i ].m_pInterfaceName, interfaceName ) )
			{
				Error( "Game and client .dlls requesting different versions '%s' vs. '%s' from '%s'\n",
					list[ i ].m_pInterfaceName, interfaceName, moduleName );
			}
			return true;
		}
	}

	return false;
}

bool CModAppSystemGroup::PreInit()
{
	return true;
}

void SV_ShutdownGameDLL();
int CModAppSystemGroup::Main()
{
	int nRunResult = RUN_OK;

	if ( IsServerOnly() )
	{
		// Start up the game engine
		if ( eng->Load( true, host_parms.basedir ) )
		{
			// If we're using STEAM, pass the map cycle list as resource hints...
#if LATER
			if ( g_pFileSystem->IsSteam() )
			{
				char *hints;
				if ( BuildMapCycleListHints(&hints) )
				{
					g_pFileSystem->HintResourceNeed(hints, true);
				}
				if ( hints )
				{
					free(hints);
				}
			}
#endif
			// Dedicated server drives frame loop manually
			dedicated->RunServer();

			SV_ShutdownGameDLL();
		}
	}
	else
	{
		eng->SetQuitting( IEngine::QUIT_NOTQUITTING );

		COM_TimestampedLog( "eng->Load" );

		// Start up the game engine
		if ( eng->Load( false, host_parms.basedir ) )					
		{
#if !defined(SWDS)
			toolframework->ServerInit( g_ServerFactory );

			if ( s_EngineAPI.MainLoop() )
			{
				nRunResult = RUN_RESTART;
			}

			// unload systems
			eng->Unload();

			toolframework->ServerShutdown();
#endif
			SV_ShutdownGameDLL();
		}
	}
	
	return nRunResult;
}

void CModAppSystemGroup::PostShutdown()
{
}

void CModAppSystemGroup::Destroy() 
{
	// unload game and client .dlls
	ServerDLL_Unload();
#ifndef SWDS
	if ( !IsServerOnly() )
	{
		ClientDLL_Unload();
	}
#endif
}

//-----------------------------------------------------------------------------
// Console command to toggle back and forth between the engine running or not
//-----------------------------------------------------------------------------
#ifndef SWDS
void EditorToggle_f()
{
	// Will switch back to the editor
	bool bActive = (eng->GetState() != IEngine::DLL_PAUSED);
	s_EngineAPI.ActivateSimulation( !bActive );
}
#endif // SWDS



//-----------------------------------------------------------------------------
//
// Purpose: Expose engine interface to launcher	for dedicated servers
//
//-----------------------------------------------------------------------------
class CDedicatedServerAPI : public CTier3AppSystem< IDedicatedServerAPI >
{
	typedef CTier3AppSystem< IDedicatedServerAPI > BaseClass;

public:
	CDedicatedServerAPI() :
	  m_pDedicatedServer( 0 )
	{
	}
	virtual bool Connect( CreateInterfaceFn factory );
	virtual void Disconnect();
	virtual void *QueryInterface( const char *pInterfaceName );

	virtual bool ModInit( ModInfo_t &info );
	virtual void ModShutdown( void );

	virtual bool RunFrame( void );

	virtual void AddConsoleText( char *text );
	virtual void UpdateStatus(float *fps, int *nActive, int *nMaxPlayers, char *pszMap, int maxlen );
	virtual void UpdateHostname(char *pszHostname, int maxlen);

private:
	int BuildMapCycleListHints( char **hints );

	CModAppSystemGroup *m_pDedicatedServer;
};


//-----------------------------------------------------------------------------
// Singleton
//-----------------------------------------------------------------------------
EXPOSE_SINGLE_INTERFACE( CDedicatedServerAPI, IDedicatedServerAPI, VENGINE_HLDS_API_VERSION );

//-----------------------------------------------------------------------------
// Connect, disconnect
//-----------------------------------------------------------------------------
bool CDedicatedServerAPI::Connect( CreateInterfaceFn factory ) 
{ 
	// Store off the app system factory...
	g_AppSystemFactory = factory;

	if ( !BaseClass::Connect( factory ) )
		return false;

	dedicated = ( IDedicatedExports * )factory( VENGINE_DEDICATEDEXPORTS_API_VERSION, NULL );
	if ( !dedicated )
		return false;

	g_pFileSystem = g_pFullFileSystem;
	g_pFileSystem->SetWarningFunc( Warning );

	if ( !Shader_Connect( false ) )
		return false;

	if ( !g_pStudioRender )
	{
		Sys_Error( "Unable to init studio render system version %s\n", STUDIO_RENDER_INTERFACE_VERSION );
		return false;
	}

	g_pPhysics = (IPhysics*)factory( VPHYSICS_INTERFACE_VERSION, NULL );

	if ( !g_pDataCache || !g_pPhysics || !g_pMDLCache )
	{
		Warning( "Engine wasn't able to acquire required interfaces!\n" );
		return false;
	}

	ConnectMDLCacheNotify();
	return true; 
}

void CDedicatedServerAPI::Disconnect() 
{
	DisconnectMDLCacheNotify();

	g_pPhysics = NULL;

	Shader_Disconnect();

	g_pFileSystem = NULL;

	ConVar_Unregister();

	dedicated = NULL;

	BaseClass::Disconnect();

	g_AppSystemFactory = NULL;
}

//-----------------------------------------------------------------------------
// Query interface
//-----------------------------------------------------------------------------
void *CDedicatedServerAPI::QueryInterface( const char *pInterfaceName )
{
	// Loading the engine DLL mounts *all* engine interfaces
	CreateInterfaceFn factory = Sys_GetFactoryThis();	// This silly construction is necessary
	return factory( pInterfaceName, NULL );				// to prevent the LTCG compiler from crashing.
}

//-----------------------------------------------------------------------------
// Creates the hint list for a multiplayer map rotation from the map cycle.
// The map cycle message is a text string with CR/CRLF separated lines.
//	-removes comments
//	-removes arguments
//-----------------------------------------------------------------------------
const char *szCommonPreloads  = "MP_Preloads";
const char *szReslistsBaseDir = "reslists2";
const char *szReslistsExt     = ".lst";

int CDedicatedServerAPI::BuildMapCycleListHints(char **hints)
{
	char szMap[ MAX_OSPATH + 2 ]; // room for one path plus <CR><LF>
	unsigned int length;
	char szMod[MAX_OSPATH];

	// Determine the mod directory.
	Q_FileBase(com_gamedir, szMod, sizeof( szMod ) );

	// Open mapcycle.txt
	char cszMapCycleTxtFile[MAX_OSPATH];
	Q_snprintf(cszMapCycleTxtFile, sizeof( cszMapCycleTxtFile ), "%s\\mapcycle.txt", szMod);
	FileHandle_t pFile = g_pFileSystem->Open(cszMapCycleTxtFile, "rb");
	if ( pFile == FILESYSTEM_INVALID_HANDLE )
	{
		ConMsg("Unable to open %s", cszMapCycleTxtFile);
		return 0;
	}

	// Start off with the common preloads.
	Q_snprintf(szMap, sizeof( szMap ), "%s\\%s\\%s%s\r\n", szReslistsBaseDir, szMod, szCommonPreloads, szReslistsExt);
	int hintsSize = strlen(szMap) + 1;
	*hints = (char*)malloc( hintsSize );
	if ( *hints == NULL )
	{
		ConMsg("Unable to allocate memory for map cycle hints list");
		g_pFileSystem->Close( pFile );
		return 0;
	}
	Q_strncpy( *hints, szMap, hintsSize );
		
	// Read in and parse mapcycle.txt
	length = g_pFileSystem->Size(pFile);
	if ( length )
	{
		char *pStart = (char *)malloc(length);
		if ( pStart && ( 1 == g_pFileSystem->Read(pStart, length, pFile) )
		   )
		{
			const char *pFileList = pStart;

			while ( 1 )
			{
				pFileList = COM_Parse( pFileList );
				if ( strlen( com_token ) <= 0 )
					break;

				Q_strncpy(szMap, com_token, sizeof(szMap));

				// Any more tokens on this line?
				if ( COM_TokenWaiting( pFileList ) )
				{
					pFileList = COM_Parse( pFileList );
				}

				char mapLine[sizeof(szMap)];
				Q_snprintf(mapLine, sizeof(mapLine), "%s\\%s\\%s%s\r\n", szReslistsBaseDir, szMod, szMap, szReslistsExt);
				*hints = (char*)realloc(*hints, strlen(*hints) + 1 + strlen(mapLine) + 1); // count NULL string terminators
				if ( *hints == NULL )
				{
					ConMsg("Unable to reallocate memory for map cycle hints list");
					g_pFileSystem->Close( pFile );
					return 0;
				}
				Q_strncat(*hints, mapLine, hintsSize, COPY_ALL_CHARACTERS);
			}
		}
	}

	g_pFileSystem->Close(pFile);

	// Tack on <moddir>\mp_maps.txt to the end to make sure we load reslists for all multiplayer maps we know of
	Q_snprintf(szMap, sizeof( szMap ), "%s\\%s\\mp_maps.txt\r\n", szReslistsBaseDir, szMod);
	*hints = (char*)realloc(*hints, strlen(*hints) + 1 + strlen(szMap) + 1); // count NULL string terminators
	Q_strncat( *hints, szMap, hintsSize, COPY_ALL_CHARACTERS );

	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : type - 0 == normal, 1 == dedicated server
//			*instance - 
//			*basedir - 
//			*cmdline - 
//			launcherFactory - 
//-----------------------------------------------------------------------------

bool CDedicatedServerAPI::ModInit( ModInfo_t &info )
{
	eng->SetQuitting( IEngine::QUIT_NOTQUITTING );

	// Set up the engineparms_t which contains global information about the mod
	host_parms.basedir = const_cast<char*>(info.m_pBaseDirectory);
	host_parms.mod = const_cast<char*>(GetModDirFromPath(info.m_pInitialMod));
	host_parms.game = const_cast<char*>(info.m_pInitialGame);

	g_bTextMode = info.m_bTextMode;

	TRACEINIT( COM_InitFilesystem( info.m_pInitialMod ), COM_ShutdownFileSystem() );
	materials->ModInit();

	// Setup the material system config record, CreateGameWindow depends on it
	// (when we're running stand-alone)
#ifndef SWDS
	InitMaterialSystemConfig( true );						// !!should this be called standalone or not?
#endif

	// Initialize general game stuff and create the main window
	if ( game->Init( NULL ) )
	{
		m_pDedicatedServer = new CModAppSystemGroup( true, info.m_pParentAppSystemGroup );

		// Store off the app system factory...
		g_AppSystemFactory = m_pDedicatedServer->GetFactory();

		m_pDedicatedServer->Run();
		return true;
	}

	return false;
}

void CDedicatedServerAPI::ModShutdown( void )
{
	if ( m_pDedicatedServer )
	{
		delete m_pDedicatedServer;
		m_pDedicatedServer = NULL;
	}

	g_AppSystemFactory = NULL;

	// Unload GL, Sound, etc.
	eng->Unload();

	// Shut down memory, etc.
	game->Shutdown();

	materials->ModShutdown();
	TRACESHUTDOWN( COM_ShutdownFileSystem() );
}

bool CDedicatedServerAPI::RunFrame( void )
{
	// Bail if someone wants to quit.
	if ( eng->GetQuitting() != IEngine::QUIT_NOTQUITTING )
	{
		return false;
	}

	// Run engine frame
	eng->Frame();
	return true;
}

void CDedicatedServerAPI::AddConsoleText( char *text )
{
	Cbuf_AddText( text );
}

void CDedicatedServerAPI::UpdateStatus(float *fps, int *nActive, int *nMaxPlayers, char *pszMap, int maxlen )
{
	Host_GetHostInfo( fps, nActive, nMaxPlayers, pszMap, maxlen );
}

void CDedicatedServerAPI::UpdateHostname(char *pszHostname, int maxlen)
{
	if ( pszHostname && ( maxlen > 0 ) )
	{
		Q_strncpy( pszHostname, sv.GetName(), maxlen );
	}
}

#ifndef SWDS

class CGameUIFuncs : public IGameUIFuncs
{
public:
	bool IsKeyDown( const char *keyname, bool& isdown )
	{
		isdown = false;
		if ( !g_ClientDLL )
			return false;

		return g_ClientDLL->IN_IsKeyDown( keyname, isdown );
	}

	const char	*GetBindingForButtonCode( ButtonCode_t code )
	{
		return ::Key_BindingForKey( code );
	}

	virtual ButtonCode_t GetButtonCodeForBind( const char *bind )
	{
		const char *pKeyName = Key_NameForBinding( bind );
		if ( !pKeyName )
			return KEY_NONE;
		return g_pInputSystem->StringToButtonCode( pKeyName ) ;
	}

	void GetVideoModes( struct vmode_s **ppListStart, int *pCount )
	{
		if ( videomode )
		{
			*pCount = videomode->GetModeCount();
			*ppListStart = videomode->GetMode( 0 );
		}
		else
		{
			*pCount = 0;
			*ppListStart = NULL;
		}
	}

	void GetDesktopResolution( int &width, int &height )
	{
		int refreshrate;
		game->GetDesktopInfo( width, height, refreshrate );
	}

	virtual void SetFriendsID( uint friendsID, const char *friendsName )
	{
		cl.SetFriendsID( friendsID, friendsName );
	}

	bool IsConnectedToVACSecureServer()
	{
		if ( cl.IsConnected() )
			return Steam3Client().BGSSecure();
		return false;
	}
};

EXPOSE_SINGLE_INTERFACE( CGameUIFuncs, IGameUIFuncs, VENGINE_GAMEUIFUNCS_VERSION );

#endif
