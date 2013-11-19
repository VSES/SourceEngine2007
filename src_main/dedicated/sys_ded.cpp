//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include <stdio.h>
#include <stdlib.h>
#include "isys.h"
#include "conproc.h"
#include "dedicated.h"
#include "engine_hlds_api.h"
#include "checksum_md5.h"
#include "mathlib/mathlib.h"
#include "tier0/vcrmode.h"
#include "tier0/dbg.h"
#include "tier1/strtools.h"
#include "tier0/icommandline.h"
#include "idedicatedexports.h"
#include "vgui/vguihelpers.h"
#include "appframework/AppFramework.h"
#include "filesystem_init.h"
#include "tier2/tier2.h"
#include "dedicated.h"
#include "vstdlib/cvar.h"

#ifdef _WIN32
#include <windows.h> 
#include <direct.h>
#include "KeyValues.h"
// filesystem_steam.cpp implements this useful function - mount all the caches for a given app ID.
extern void MountDependencies( int iAppId, CUtlVector<unsigned int> &depList );
#else
#define _chdir chdir
#endif

void* FileSystemFactory( const char *pName, int *pReturnCode );
bool InitInstance( );
void ProcessConsoleInput( void );
const char *UTIL_GetExecutableDir( );
bool NET_Init( void );
void NET_Shutdown( void );
const char *UTIL_GetBaseDir( void );
bool g_bVGui = false;

#if defined ( _WIN32 )
#include "console/TextConsoleWin32.h"
CTextConsoleWin32 console;
#else
#include "console/TextConsoleUnix.h"
CTextConsoleUnix console;
#endif

extern char *gpszCvars;

IDedicatedServerAPI *engine = NULL;

#ifdef _LINUX
extern char g_szEXEName[ 256 ];
#endif

#pragma warning(disable:4800)

//-----------------------------------------------------------------------------
// Implementation of IVCRHelpers.
//-----------------------------------------------------------------------------

class CVCRHelpers : public IVCRHelpers
{
public:
	virtual void	ErrorMessage( const char *pMsg )
	{
		printf( "ERROR: %s\n", pMsg );
	}

	virtual void*	GetMainWindow()
	{
		return 0;
	}
};
CVCRHelpers g_VCRHelpers;


SpewRetval_t DedicatedSpewOutputFunc( SpewType_t spewType, char const *pMsg ); // in sys_common.cpp

//-----------------------------------------------------------------------------
//
//  Server loop
//
//-----------------------------------------------------------------------------
void RunServer( void )
{
#ifdef _WIN32
	if(gpszCvars)
	{
		engine->AddConsoleText(gpszCvars);
	}
#endif

	int bDone = 0;

	// run 2 engine frames first to get the engine to load its resources
	if (g_bVGui)
	{
#ifdef _WIN32
		RunVGUIFrame();
#endif
	}
	if ( !engine->RunFrame() )
	{
		return;
	}
	if (g_bVGui)
	{
#ifdef _WIN32
		RunVGUIFrame();
#endif
	}

	if ( !engine->RunFrame() )
	{
		return;
	}

	if (g_bVGui)
	{
#ifdef _WIN32
		VGUIFinishedConfig();
		RunVGUIFrame();
#endif
	}
	
	while ( 1 )
	{
		if ( bDone )
			break;

		// Running really fast, yield some time to other apps
		if ( VCRGetMode() != VCR_Playback )
		{
			sys->Sleep( 1 );
		}
		
#if defined ( _WIN32 )
		MSG msg;

		while( VCRHook_PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			//if (!GetMessage( &msg, NULL, 0, 0))
			if ( msg.message == WM_QUIT )
			{
				bDone = true;
				break;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if  ( IsPC() )
		{
			// NOTE: Under some implementations of Win9x, 
			// dispatching messages can cause the FPU control word to change
			SetupFPUControlWord();
		}

		if ( bDone /*|| gbAppHasBeenTerminated*/ )
			break;
#endif // _WIN32

		if ( g_bVGui )
		{
#ifdef _WIN32
			RunVGUIFrame();
#endif
		}
		else
		{
			ProcessConsoleInput();
		}

		if ( !engine->RunFrame() )
		{
			bDone = true;
		}

		sys->UpdateStatus( 0  /* don't force */ );
	}

}

//-----------------------------------------------------------------------------
//
// initialize the console or wait for vgui to start the server
//
//-----------------------------------------------------------------------------
bool ConsoleStartup( CreateInterfaceFn dedicatedFactory )
{
#ifdef _WIN32
	if ( g_bVGui )
	{
		StartVGUI( dedicatedFactory );
		RunVGUIFrame();
		// Run the config screen
		while (VGUIIsInConfig()	&& VGUIIsRunning())
		{
			RunVGUIFrame();
		}

		if ( VGUIIsStopping() )
		{
			return false;
		}
	}
	else
#endif // _WIN32
	{
		if ( !console.Init() )
		{
			return false;	 
		}
	}
	return true;
}


//-----------------------------------------------------------------------------
// Instantiate all main libraries
//-----------------------------------------------------------------------------
bool CDedicatedAppSystemGroup::Create( )
{
	// Hook the debug output stuff (override the spew func in the appframework)
	SpewOutputFunc( DedicatedSpewOutputFunc );

	// Added the dedicated exports module for the engine to grab
	AppModule_t dedicatedModule = LoadModule( Sys_GetFactoryThis() );
	IAppSystem *pSystem = AddSystem( dedicatedModule, VENGINE_DEDICATEDEXPORTS_API_VERSION );
	if ( !pSystem )
		return false;

	return sys->LoadModules( this );
}

bool CDedicatedAppSystemGroup::PreInit( )
{
	// A little hack needed because dedicated links directly to filesystem .cpp files
	g_pFullFileSystem = NULL;

	if ( !BaseClass::PreInit() )
		return false;

	CFSSteamSetupInfo steamInfo;
	steamInfo.m_pDirectoryName = NULL;
	steamInfo.m_bOnlyUseDirectoryName = false;
	steamInfo.m_bToolsMode = false;
	steamInfo.m_bSetSteamDLLPath = false;
	steamInfo.m_bSteam = g_pFullFileSystem->IsSteam();
	steamInfo.m_bNoGameInfo = steamInfo.m_bSteam;
	if ( FileSystem_SetupSteamEnvironment( steamInfo ) != FS_OK )
		return false;

	CFSMountContentInfo fsInfo;
	fsInfo.m_pFileSystem = g_pFullFileSystem;
	fsInfo.m_bToolsMode = false;
	fsInfo.m_pDirectoryName = steamInfo.m_GameInfoPath;

	if ( FileSystem_MountContent( fsInfo ) != FS_OK )
		return false;

	if ( !NET_Init() )
		return false;

#ifdef _WIN32
	if ( CommandLine()->CheckParm( "-console" ) )
	{
		g_bVGui = false;
	}
	else
	{
		g_bVGui = true;
	}
#else
	// no VGUI under linux
	g_bVGui = false; 
#endif

	if ( !g_bVGui )
	{
		if ( !sys->CreateConsoleWindow() )
			return false;
	}

	return true;
}

int CDedicatedAppSystemGroup::Main( )
{
	if ( !ConsoleStartup( GetFactory() ) )
		return -1;

#ifdef _WIN32
	if ( g_bVGui )
	{
		RunVGUIFrame();
	}
	else
	{
		// mount the caches
		if (CommandLine()->CheckParm("-steam"))
		{
			// Add a search path for the base dir
			char fullLocationPath[MAX_PATH];
			if ( _getcwd( fullLocationPath, MAX_PATH ) )
			{
				g_pFullFileSystem->AddSearchPath( fullLocationPath, "MAIN" );
			}

			// Find the gameinfo.txt for our mod and mount it's caches
			char gameInfoFilename[MAX_PATH];
			Q_snprintf( gameInfoFilename, sizeof(gameInfoFilename) - 1, "%s\\gameinfo.txt", CommandLine()->ParmValue( "-game", "hl2" ) );
			KeyValues *gameData = new KeyValues( "GameInfo" );
			if ( gameData->LoadFromFile( g_pFullFileSystem, gameInfoFilename ) )
			{
				KeyValues *pFileSystem = gameData->FindKey( "FileSystem" );
				int iAppId = pFileSystem->GetInt( "SteamAppId" );
				if ( iAppId )
				{
					CUtlVector<unsigned int> depList;
					MountDependencies( iAppId, depList );
				}
			}
			gameData->deleteThis();

			// remove our base search path
			g_pFullFileSystem->RemoveSearchPaths( "MAIN" );
		}
	}
#endif

	// Set up mod information
	ModInfo_t info;
	info.m_pInstance = GetAppInstance();
	info.m_pBaseDirectory = UTIL_GetBaseDir();
	info.m_pInitialMod = CommandLine()->ParmValue( "-game", "hl2" );
	info.m_pInitialGame = CommandLine()->ParmValue( "-defaultgamedir", "hl2" );
	info.m_pParentAppSystemGroup = this;
	info.m_bTextMode = CommandLine()->CheckParm( "-textmode" );

	if ( engine->ModInit( info ) )
	{
		engine->ModShutdown();
	} // if engine->ModInit

	return 0;
}

void CDedicatedAppSystemGroup::PostShutdown()
{
#ifdef _WIN32
	if ( g_bVGui )
	{
		StopVGUI();
	}
#endif
	sys->DestroyConsoleWindow();
	console.ShutDown();
	NET_Shutdown();
	BaseClass::PostShutdown();
}

void CDedicatedAppSystemGroup::Destroy() 
{
}


//-----------------------------------------------------------------------------
// Gets the executable name
//-----------------------------------------------------------------------------
bool GetExecutableName( char *out, int nMaxLen )
{
#ifdef _WIN32
	if ( !::GetModuleFileName( ( HINSTANCE )GetModuleHandle( NULL ), out, nMaxLen ) )
	{
		return false;
	}
	return true;
#elif _LINUX
	Q_strncpy( out, g_szEXEName, nMaxLen );
	return true;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Return the directory where this .exe is running from
// Output : char
//-----------------------------------------------------------------------------
void UTIL_ComputeBaseDir( char *pBaseDir, int nMaxLen )
{
	int j;
	char *pBuffer = NULL;

	pBaseDir[ 0 ] = 0;

	if ( GetExecutableName( pBaseDir, nMaxLen ) )
	{
		pBuffer = strrchr( pBaseDir, CORRECT_PATH_SEPARATOR );
		if ( pBuffer && *pBuffer )
		{
			*(pBuffer+1) = '\0';
		}

		j = strlen( pBaseDir );
		if (j > 0)
		{
			if ( ( pBaseDir[ j-1 ] == '\\' ) || 
				 ( pBaseDir[ j-1 ] == '/' ) )
			{
				pBaseDir[ j-1 ] = 0;
			}
		}
	}

	char const *pOverrideDir = CommandLine()->CheckParm( "-basedir" );
	if ( pOverrideDir )
	{
		strcpy( pBaseDir, pOverrideDir );
	}

	Q_strlower( pBaseDir );
	Q_FixSlashes( pBaseDir );
}


//-----------------------------------------------------------------------------
// This class is a helper class used for steam-based applications.
// It loads up the file system in preparation for using it to load other
// required modules from steam.
//
// I couldn't use the one in appframework because the dedicated server
// inlines all the filesystem code.
//-----------------------------------------------------------------------------
class CDedicatedSteamApplication : public CSteamApplication
{
public:
	CDedicatedSteamApplication( CSteamAppSystemGroup *pAppSystemGroup );
	virtual bool Create( );
};


//-----------------------------------------------------------------------------
// This class is a helper class used for steam-based applications.
// It loads up the file system in preparation for using it to load other
// required modules from steam.
//
// I couldn't use the one in appframework because the dedicated server
// inlines all the filesystem code.
//-----------------------------------------------------------------------------
CDedicatedSteamApplication::CDedicatedSteamApplication( CSteamAppSystemGroup *pAppSystemGroup ) : CSteamApplication( pAppSystemGroup )
{
}


//-----------------------------------------------------------------------------
// Implementation of IAppSystemGroup
//-----------------------------------------------------------------------------
bool CDedicatedSteamApplication::Create( )
{
	// Add in the cvar factory
	AppModule_t cvarModule = LoadModule( VStdLib_GetICVarFactory() );
	AddSystem( cvarModule, CVAR_INTERFACE_VERSION );

	AppModule_t fileSystemModule = LoadModule( FileSystemFactory );
	m_pFileSystem = (IFileSystem*)AddSystem( fileSystemModule, FILESYSTEM_INTERFACE_VERSION );

	if ( !m_pFileSystem )
	{
		Warning( "Unable to load the file system!\n" );
		return false;
	}

	return true;
}



//-----------------------------------------------------------------------------
//
// Main entry point for dedicated server, shared between win32 and linux
//
//-----------------------------------------------------------------------------

int main(int argc, char **argv)
{
#ifndef _LINUX
	_asm
	{
		fninit
	}
#endif

	SetupFPUControlWord();

#ifdef _LINUX
	strcpy(g_szEXEName, *argv);
	// Store off command line for argument searching
	BuildCmdLine(argc, argv);
#endif

	MathLib_Init( 2.2f, 2.2f, 0.0f, 1.0f );

	// Store off command line for argument searching
	CommandLine()->CreateCmdLine( VCRHook_GetCommandLine() );
#ifndef _WIN32
	Plat_SetCommandLine( CommandLine()->GetCmdLine() );
#endif

	// Start VCR mode?
	const char *filename;
	if( CommandLine()->CheckParm( "-vcrrecord", &filename ) )
	{
		if ( !VCRStart( filename, true, &g_VCRHelpers ) )
		{
			Error( "-vcrrecord: can't open '%s' for writing.\n", filename );
			return -1;
		}
	}
	else if( CommandLine()->CheckParm( "-vcrplayback", &filename ) )
	{
		if ( !VCRStart( filename, false, &g_VCRHelpers ) )
		{
			Error( "-vcrplayback: can't open '%s' for reading.\n", filename );
			return -1;
		}
	}

	// Figure out the directory the executable is running from
	// and make that be the current working directory
	char pBasedir[ MAX_PATH ];
	UTIL_ComputeBaseDir( pBasedir, MAX_PATH );
	_chdir( pBasedir );

	// Rehook the command line through VCR mode.
	CommandLine()->CreateCmdLine( VCRHook_GetCommandLine() );

	if ( !InitInstance() )
		return -1;

	CDedicatedAppSystemGroup dedicatedSystems;
	CDedicatedSteamApplication steamApplication( &dedicatedSystems );
	return steamApplication.Run( );
}
