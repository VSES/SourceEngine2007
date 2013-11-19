//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//


#if defined(_WIN32) && !defined(_X360)
#include "winlite.h"
#endif
#include "quakedef.h"
#include "igame.h"
#include "errno.h"
#include "host.h"
#include "profiling.h"
#include "server.h"
#include "vengineserver_impl.h"
#include "filesystem_engine.h"
#include "sys.h"
#include "sys_dll.h"
#include "ivideomode.h"
#include "host_cmd.h"
#include "crtmemdebug.h"
#include "sv_log.h"
#include "sv_main.h"
#include "traceinit.h"
#include "dt_test.h"
#include "keys.h"
#include "gl_matsysiface.h"
#include "tier0/vcrmode.h"
#include "tier0/icommandline.h"
#include "cmd.h"
#include <ihltvdirector.h>
#include "MapReslistGenerator.h"
#include "DevShotGenerator.h"
#include "cdll_engine_int.h"
#include "dt_send.h"
#include "idedicatedexports.h"
#include "eifaceV21.h"
#include "steam/steam_api.h"

#ifdef _WIN32
#include "vgui_baseui_interface.h"
#include "tier0/systeminformation.h"
#if !defined( _X360 )
#include <io.h>
#endif
#endif
#include "toolframework/itoolframework.h"
#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define ONE_HUNDRED_TWENTY_EIGHT_MB	(128 * 1024 * 1024)

ConVar mem_min_heapsize( "mem_min_heapsize", "48", 0, "Minimum amount of memory to dedicate to engine hunk and datacache (in mb)" );
ConVar mem_max_heapsize( "mem_max_heapsize", "256", 0, "Maximum amount of memory to dedicate to engine hunk and datacache (in mb)" );
ConVar mem_max_heapsize_dedicated( "mem_max_heapsize_dedicated", "64", 0, "Maximum amount of memory to dedicate to engine hunk and datacache, for dedicated server (in mb)" );

#define MINIMUM_WIN_MEMORY			(unsigned)(mem_min_heapsize.GetInt()*1024*1024)
#define MAXIMUM_WIN_MEMORY			max( (unsigned)(mem_max_heapsize.GetInt()*1024*1024), MINIMUM_WIN_MEMORY )
#define MAXIMUM_DEDICATED_MEMORY	(unsigned)(mem_max_heapsize_dedicated.GetInt()*1024*1024)


char *CheckParm(const char *psz, char **ppszValue = NULL);
void SeedRandomNumberGenerator( bool random_invariant );
void Con_ColorPrintf( const Color& clr, const char *fmt, ... );

void COM_ShutdownFileSystem( void );
void COM_InitFilesystem( const char *pFullModPath );

modinfo_t			gmodinfo;

#ifdef _WIN32
extern HWND			*pmainwindow;
#endif
char				gszDisconnectReason[256];
char				gszExtendedDisconnectReason[256];
bool				gfExtendedError = false;
uint8				g_eSteamLoginFailure = 0;
bool				g_bV3SteamInterface = false;
CreateInterfaceFn	g_AppSystemFactory = NULL;

static bool			s_bIsDedicated = false;
ConVar *sv_noclipduringpause = NULL;

// Special mode where the client uses a console window and has no graphics. Useful for stress-testing a server
// without having to round up 32 people.
bool g_bTextMode = false;

// Set to true when we exit from an error.
bool g_bInErrorExit = false;

static FileFindHandle_t	g_hfind = FILESYSTEM_INVALID_FIND_HANDLE;

// The extension DLL directory--one entry per loaded DLL
CSysModule *g_GameDLL = NULL;

// Prototype of an global method function
typedef void (DLLEXPORT * PFN_GlobalMethod)( edict_t *pEntity );

IServerGameDLL	*serverGameDLL = NULL;
bool g_bServerGameDLLGreaterThanV4;
bool g_bServerGameDLLGreaterThanV5;
IServerGameEnts *serverGameEnts = NULL;

IServerGameClients *serverGameClients = NULL;
int g_iServerGameClientsVersion = 0;	// This matches the number at the end of the interface name (so for "ServerGameClients004", this would be 4).

IHLTVDirector	*serverGameDirector = NULL;

void Sys_InitArgv( char *lpCmdLine );
void Sys_ShutdownArgv( void );

//-----------------------------------------------------------------------------
// Purpose: Compare file times
// Input  : ft1 - 
//			ft2 - 
// Output : int
//-----------------------------------------------------------------------------
int Sys_CompareFileTime( long ft1, long ft2 )
{
	if ( ft1 < ft2 )
	{
		return -1;
	}
	else if ( ft1 > ft2 )
	{
		return 1;
	}

	return 0;
}


//-----------------------------------------------------------------------------
// Is slash?
//-----------------------------------------------------------------------------
inline bool IsSlash( char c )
{
	return ( c == '\\') || ( c == '/' );
}


//-----------------------------------------------------------------------------
// Purpose: Create specified directory
// Input  : *path - 
// Output : void Sys_mkdir
//-----------------------------------------------------------------------------
void Sys_mkdir( const char *path )
{
	char testpath[ MAX_OSPATH ];

	// Remove any terminal backslash or /
	Q_strncpy( testpath, path, sizeof( testpath ) );
	int nLen = Q_strlen( testpath );
	if ( (nLen > 0) && IsSlash( testpath[ nLen - 1 ] ) )
	{
		testpath[ nLen - 1 ] = 0;
	}

	// Look for URL
	const char *pPathID = "MOD";
	if ( IsSlash( testpath[0] ) && IsSlash( testpath[1] ) )
	{
		pPathID = NULL;
	}

	if ( g_pFileSystem->FileExists( testpath, pPathID ) )
	{
		// if there is a file of the same name as the directory we want to make, just kill it
		if ( !g_pFileSystem->IsDirectory( testpath, pPathID ) )
		{
			g_pFileSystem->RemoveFile( testpath, pPathID );
		}
	}

	g_pFileSystem->CreateDirHierarchy( path, pPathID );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *path - 
//			*basename - 
// Output : char *Sys_FindFirst
//-----------------------------------------------------------------------------
const char *Sys_FindFirst(const char *path, char *basename, int namelength )
{
	if (g_hfind != FILESYSTEM_INVALID_FIND_HANDLE)
	{
		Sys_Error ("Sys_FindFirst without close");
		g_pFileSystem->FindClose(g_hfind);		
	}

	const char* psz = g_pFileSystem->FindFirst(path, &g_hfind);
	if (basename && psz)
	{
		Q_FileBase(psz, basename, namelength );
	}

	return psz;
}

//-----------------------------------------------------------------------------
// Purpose: Sys_FindFirst with a path ID filter.
//-----------------------------------------------------------------------------
const char *Sys_FindFirstEx( const char *pWildcard, const char *pPathID, char *basename, int namelength )
{
	if (g_hfind != FILESYSTEM_INVALID_FIND_HANDLE)
	{
		Sys_Error ("Sys_FindFirst without close");
		g_pFileSystem->FindClose(g_hfind);		
	}

	const char* psz = g_pFileSystem->FindFirstEx( pWildcard, pPathID, &g_hfind);
	if (basename && psz)
	{
		Q_FileBase(psz, basename, namelength );
	}

	return psz;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *basename - 
// Output : char *Sys_FindNext
//-----------------------------------------------------------------------------
const char* Sys_FindNext(char *basename, int namelength)
{
	const char *psz = g_pFileSystem->FindNext(g_hfind);
	if ( basename && psz )
	{
		Q_FileBase(psz, basename, namelength );
	}

	return psz;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : void Sys_FindClose
//-----------------------------------------------------------------------------

void Sys_FindClose(void)
{
	if ( FILESYSTEM_INVALID_FIND_HANDLE != g_hfind )
	{
		g_pFileSystem->FindClose(g_hfind);
		g_hfind = FILESYSTEM_INVALID_FIND_HANDLE;
	}
}


//-----------------------------------------------------------------------------
// Purpose: OS Specific initializations
//-----------------------------------------------------------------------------
void Sys_Init( void )
{
	// Set default FPU control word to truncate (chop) mode for optimized _ftol()
	// This does not "stick", the mode is restored somewhere down the line.
//	Sys_TruncateFPU();	
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Sys_Shutdown( void )
{
}


//-----------------------------------------------------------------------------
// Purpose: Print to system console
// Input  : *fmt - 
//			... - 
// Output : void Sys_Printf
//-----------------------------------------------------------------------------
void Sys_Printf(char *fmt, ...)
{
	va_list		argptr;
	char		text[1024];

	va_start (argptr,fmt);
	Q_vsnprintf (text, sizeof( text ), fmt, argptr);
	va_end (argptr);
		
	if ( developer.GetInt() )
	{
#ifdef _WIN32
		wchar_t unicode[2048];
		::MultiByteToWideChar(CP_UTF8, 0, text, -1, unicode, sizeof( unicode ) / sizeof(wchar_t));
		unicode[(sizeof( unicode ) / sizeof(wchar_t)) - 1] = L'\0';
		OutputDebugStringW( unicode );
		Sleep( 0 );
#endif
	}

	if ( s_bIsDedicated )
	{
		printf( text );
	}
}


bool Sys_MessageBox(const char *title, const char *info, bool bShowOkAndCancel)
{
#ifdef _WIN32
	if (IDOK == ::MessageBox(NULL, title, info, MB_ICONEXCLAMATION | (bShowOkAndCancel ? MB_OKCANCEL : MB_OK)))
	{
		return true;
	}
	return false;
#elif _LINUX
	Warning( "%s\n", info );
	return true;
#else
#error "implement me"
#endif
}

bool g_bUpdateMinidumpComment = true;

//-----------------------------------------------------------------------------
// Purpose: Exit engine with error
// Input  : *error - 
//			... - 
// Output : void Sys_Error
//-----------------------------------------------------------------------------
void Sys_Error(const char *error, ...)
{
	extern char g_minidumpinfo[4096];

	va_list		argptr;
	char		text[1024];
	static      bool bReentry = false; // Don't meltdown

	va_start( argptr, error );
	Q_vsnprintf( text, sizeof( text ), error, argptr );
	va_end( argptr );

	if ( bReentry )
	{
		fprintf( stderr, "%s\n", text );
		return;
	}

	bReentry = true;

	if ( s_bIsDedicated )
	{
		printf( "%s\n", text );
	}
	else
	{
		Sys_Printf( "%s\n", text );
	}

	g_bInErrorExit = true;
	
#if !defined( SWDS )
	if ( IsPC() && videomode )
		videomode->Shutdown();
#endif

#ifdef _WIN32
	if ( IsPC() &&
		 !CommandLine()->FindParm( "-makereslists" ) &&
		 !CommandLine()->FindParm( "-nomessagebox" ) )
	{
		::MessageBox( NULL, text, "Engine Error", MB_OK | MB_TOPMOST );
	}

	if ( IsPC() )
	{
		DebuggerBreakIfDebugging();
	}
	else if ( !IsRetail() )
	{
		DebuggerBreak(); 
	}

#if !defined( _X360 )

#if !defined(NO_STEAM) && !defined(SWDS) && !defined(LINUX)
	char errorText[4096];
	V_strcpy(errorText,"Sys_Error: ");
	V_strncat( errorText, text, sizeof(errorText) );
	V_strncat( errorText, "\n", sizeof(errorText) );
	PAGED_POOL_INFO_t info;
	if ( SYSCALL_SUCCESS == Plat_GetPagedPoolInfo( &info ) )
	{
		V_snprintf( text, sizeof(text), "PP PAGES: used: %d, free %d\n", info.numPagesUsed, info.numPagesFree );
		V_strncat( errorText, text, sizeof(errorText) );
	}

	V_strncat( errorText, g_minidumpinfo, sizeof(errorText) );

	g_bUpdateMinidumpComment = false;
	SteamAPI_SetMiniDumpComment( errorText );
#endif

	if ( !Plat_IsInDebugSession() && !CommandLine()->FindParm( "-nominidumps") )
	{
#ifndef NO_STEAM
		// MiniDumpWrite() has problems capturing the calling thread's context 
		// unless it is called with an exception context.  So fake an exception.
		__try
		{
			RaiseException
				(
				0,							// dwExceptionCode
				EXCEPTION_NONCONTINUABLE,	// dwExceptionFlags
				0,							// nNumberOfArguments,
				NULL						// const ULONG_PTR* lpArguments
				);

			// Never get here (non-continuable exception)
		}
		// Write the minidump from inside the filter (GetExceptionInformation() is only 
		// valid in the filter)

		__except ( SteamAPI_WriteMiniDump( 0, GetExceptionInformation(), build_number() ), EXCEPTION_EXECUTE_HANDLER )
		{
			
			// We always get here because the above filter evaluates to EXCEPTION_EXECUTE_HANDLER
		}
#endif
	}
#endif
#endif

	host_initialized = false;
#if defined(_WIN32) && !defined( _X360 )
	// We don't want global destructors in our process OR in any DLL to get executed.
	// _exit() avoids calling global destructors in our module, but not in other DLLs.
	TerminateProcess( GetCurrentProcess(), 100 );
#else
	_exit( 100 );
#endif
}

bool IsInErrorExit()
{
	return g_bInErrorExit;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : msec - 
// Output : void Sys_Sleep
//-----------------------------------------------------------------------------
void Sys_Sleep( int msec )
{
#ifdef _WIN32
	Sleep ( msec );
#elif _LINUX
	usleep( msec );
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : hInst - 
//			ulInit - 
//			lpReserved - 
// Output : BOOL WINAPI   DllMain
//-----------------------------------------------------------------------------
#if defined(_WIN32) && !defined( _X360 )
BOOL WINAPI DllMain(HANDLE hInst, ULONG ulInit, LPVOID lpReserved)
{
	InitCRTMemDebug();
	if (ulInit == DLL_PROCESS_ATTACH)
	{
	} 
	else if (ulInit == DLL_PROCESS_DETACH)
	{
	}

	return TRUE;
}
#endif


//-----------------------------------------------------------------------------
// Purpose: Allocate memory for engine hunk
// Input  : *parms - 
//-----------------------------------------------------------------------------
void Sys_InitMemory( void )
{
#ifdef _WIN32
	// Allow overrides
	int nHeapSize = CommandLine()->ParmValue( "-heapsize", 0 ); 
	if ( nHeapSize )
	{
		host_parms.memsize = nHeapSize * 1024;
		return;
	}

	if ( CommandLine()->FindParm( "-minmemory" ) )
	{
		host_parms.memsize = MINIMUM_WIN_MEMORY;
		return;
	}

	host_parms.memsize = 0;

#if (_MSC_VER > 1200)
	// MSVC 6.0 doesn't support GlobalMemoryStatusEx()
	if ( IsPC() )
	{
		OSVERSIONINFOEX osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if ( GetVersionEx ((OSVERSIONINFO *)&osvi) )
		{
			if ( osvi.dwPlatformId >= VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 5 )
			{
				MEMORYSTATUSEX	memStat;
				ZeroMemory(&memStat, sizeof(MEMORYSTATUSEX));
				memStat.dwLength = sizeof(MEMORYSTATUSEX);
				if ( GlobalMemoryStatusEx( &memStat ) )
				{
					if ( memStat.ullTotalPhys > 0xFFFFFFFFUL )
					{
						host_parms.memsize = 0xFFFFFFFFUL;
					}
					else
					{
						host_parms.memsize = memStat.ullTotalPhys;
					}
				}
			}
		}
	}
#endif // (_MSC_VER > 1200)

	if ( !IsX360() )
	{
		if ( host_parms.memsize == 0 )
		{
			MEMORYSTATUS lpBuffer;
			// Get OS Memory status
			lpBuffer.dwLength = sizeof(MEMORYSTATUS);
			GlobalMemoryStatus( &lpBuffer );

			if ( lpBuffer.dwTotalPhys <= 0 )
			{
				host_parms.memsize = MAXIMUM_WIN_MEMORY;
			}
			else
			{
				host_parms.memsize = lpBuffer.dwTotalPhys;
			}	
		}
		if ( host_parms.memsize < ONE_HUNDRED_TWENTY_EIGHT_MB )
		{
			Sys_Error( "Available memory less than 128MB!!! %i\n", host_parms.memsize );
		}

		// take one quarter the physical memory
		if ( host_parms.memsize <= 512*1024*1024)
		{
			host_parms.memsize >>= 2;
			// Apply cap of 64MB for 512MB systems
			// this keeps the code the same as HL2 gold
			// but allows us to use more memory on 1GB+ systems
			if (host_parms.memsize > MAXIMUM_DEDICATED_MEMORY)
			{
				host_parms.memsize = MAXIMUM_DEDICATED_MEMORY;
			}
		}
		else
		{
			// just take one quarter, no cap
			host_parms.memsize >>= 2;
		}

		// At least MINIMUM_WIN_MEMORY mb, even if we have to swap a lot.
		if (host_parms.memsize < MINIMUM_WIN_MEMORY)
		{
			host_parms.memsize = MINIMUM_WIN_MEMORY;
		}

		// Apply cap
		if (host_parms.memsize > MAXIMUM_WIN_MEMORY)
		{
			host_parms.memsize = MAXIMUM_WIN_MEMORY;
		}
	}
	else
	{
		host_parms.memsize = 128*1024*1024;
	}
#else
	//FILE *meminfo=fopen("/proc/meminfo","r"); // read in meminfo file?
	// sysinfo() system call??

	// hard code 32 mb for dedicated servers
	host_parms.memsize = MAXIMUM_DEDICATED_MEMORY;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *parms - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
void Sys_ShutdownMemory( void )
{
	host_parms.memsize = 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Sys_InitAuthentication( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Sys_ShutdownAuthentication( void )
{
}

//-----------------------------------------------------------------------------
// Debug library spew output
//-----------------------------------------------------------------------------
CThreadLocalInt<> g_bInSpew;

SpewRetval_t Sys_SpewFunc( SpewType_t spewType, const char *pMsg )
{
	bool suppress = g_bInSpew;

	g_bInSpew = true;

	char temp[8192];
	char *pFrom = (char *)pMsg;
	char *pTo = temp;
	char *pLimit = &temp[sizeof(temp) - 2]; // always space for 2 chars plus null (ie %%)

	while ( *pFrom && pTo < pLimit )
	{
		*pTo = *pFrom++;
		if ( *pTo++ == '%' )
			*pTo++ = '%';
	}
	*pTo = 0;

	if ( !suppress )
	{
		// If this is a dedicated server, then we have taken over its spew function, but we still
		// want its vgui console to show the spew, so pass it into the dedicated server.
		if ( dedicated )
			dedicated->Sys_Printf( (char*)pMsg );

		if ( g_bTextMode )
		{
			printf( "%s", pMsg );
		}

		if ((spewType != SPEW_LOG) || (sv.GetMaxClients() == 1))
		{
			Color color;
			switch ( spewType )
			{
#ifndef SWDS
			case SPEW_WARNING:
				{
					color.SetColor( 255, 90, 90, 255 );
				}
				break;
			case SPEW_ASSERT:
				{
					color.SetColor( 255, 20, 20, 255 );
				}
				break;
			case SPEW_ERROR:
				{
					color.SetColor( 20, 70, 255, 255 );
				}
				break;
#endif
			default:
				{
					color = GetSpewOutputColor();
				}
				break;
			}
			Con_ColorPrintf( color, temp );

		}
		else
		{
			g_Log.Printf( temp );
		}
	}

	g_bInSpew = false;

	if (spewType == SPEW_ERROR)
	{
		Sys_Error( temp );
		return SPEW_ABORT;
	}
	if (spewType == SPEW_ASSERT)
	{
		if ( CommandLine()->FindParm( "-noassert" ) == 0 )
			return SPEW_DEBUGGER;
		else
			return SPEW_CONTINUE;
	}
	return SPEW_CONTINUE;
}

void DeveloperChangeCallback( IConVar *pConVar, const char *pOldString, float flOldValue )
{
	// Set the "developer" spew group to the value...
	ConVarRef var( pConVar );
	int val = var.GetInt();
	SpewActivate( "developer", val );

	// Activate console spew (spew value 2 == developer console spew)
	SpewActivate( "console", val ? 2 : 1 );
}

//-----------------------------------------------------------------------------
// Purpose: factory comglomerator, gets the client, server, and gameui dlls together
//-----------------------------------------------------------------------------
void *GameFactory( const char *pName, int *pReturnCode )
{
	void *pRetVal = NULL;

	// first ask the app factory
	pRetVal = g_AppSystemFactory( pName, pReturnCode );
	if (pRetVal)
		return pRetVal;

#ifndef SWDS
	// now ask the client dll
#ifdef _WIN32
	if (ClientDLL_GetFactory())
	{
		pRetVal = ClientDLL_GetFactory()( pName, pReturnCode );
		if (pRetVal)
			return pRetVal;
	}

	// gameui.dll
	if (EngineVGui()->GetGameUIFactory())
	{
		pRetVal = EngineVGui()->GetGameUIFactory()( pName, pReturnCode );
		if (pRetVal)
			return pRetVal;
	}
#endif
#endif	
	// server dll factory access would go here when needed

	return NULL;
}

// factory instance
CreateInterfaceFn g_GameSystemFactory = GameFactory;


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *lpOrgCmdLine - 
//			launcherFactory - 
//			*pwnd - 
//			bIsDedicated - 
// Output : int
//-----------------------------------------------------------------------------
int Sys_InitGame( CreateInterfaceFn appSystemFactory, const char* pBaseDir, void *pwnd, int bIsDedicated )
{
#ifdef BENCHMARK
	if ( bIsDedicated )
	{
		Error( "Dedicated server isn't supported by this benchmark!" );
	}
#endif

	extern void InitMathlib( void );
	InitMathlib();
	
	FileSystem_SetWhitelistSpewFlags();

	// Activate console spew
	// Must happen before developer.InstallChangeCallback because that callback may reset it 
	SpewActivate( "console", 1 );

	// Install debug spew output....
	developer.InstallChangeCallback( DeveloperChangeCallback );

	SpewOutputFunc( Sys_SpewFunc );
	
	// Assume failure
	host_initialized = false;
#ifdef _WIN32
	// Grab main window pointer
	pmainwindow = (HWND *)pwnd;
#endif
	// Remember that this is a dedicated server
	s_bIsDedicated = bIsDedicated ? true : false;

	memset( &gmodinfo, 0, sizeof( modinfo_t ) );

	static char s_pBaseDir[256];
	Q_strncpy( s_pBaseDir, pBaseDir, sizeof( s_pBaseDir ) );
	Q_strlower( s_pBaseDir );
	Q_FixSlashes( s_pBaseDir );
	host_parms.basedir = s_pBaseDir;

#ifdef _LINUX
	if ( CommandLine()->FindParm ( "-pidfile" ) )
	{	
		FILE *pidFile = g_pFileSystem->Open( CommandLine()->ParmValue ( "-pidfile", "srcds.pid" ), "w+" );
		if ( pidFile )
		{
			char dir[MAX_PATH];
			getcwd( dir, sizeof(dir) );
			g_pFileSystem->FPrintf( pidFile, "%i\n", getpid() );
			g_pFileSystem->Close(pidFile);
		}
		else
		{
			Warning("Unable to open pidfile (%s)\n", CommandLine()->CheckParm ( "-pidfile" ));
		}
	}
#endif


	// Initialize clock
	TRACEINIT( Sys_Init(), Sys_Shutdown() );

#if defined(_DEBUG)
	if ( IsPC() )
	{
		if( !CommandLine()->FindParm( "-nodttest" ) && !CommandLine()->FindParm( "-dti" ) )
		{
			RunDataTableTest();	
		}
	}
#endif

	// NOTE: Can't use COM_CheckParm here because it hasn't been set up yet.
	SeedRandomNumberGenerator( CommandLine()->FindParm( "-random_invariant" ) != 0 );

	TRACEINIT( Sys_InitMemory(), Sys_ShutdownMemory() );

	TRACEINIT( Host_Init( s_bIsDedicated ), Host_Shutdown() );

	if ( !host_initialized )
	{
		return 0;
	}

	TRACEINIT( Sys_InitAuthentication(), Sys_ShutdownAuthentication() );

	MapReslistGenerator_BuildMapList();

	return 1;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Sys_ShutdownGame( void )
{
	TRACESHUTDOWN( Sys_ShutdownAuthentication() );

	TRACESHUTDOWN( Host_Shutdown() );

	TRACESHUTDOWN( Sys_ShutdownMemory() );

	// TRACESHUTDOWN( Sys_ShutdownArgv() );

	TRACESHUTDOWN( Sys_Shutdown() );

	// Remove debug spew output....
	developer.InstallChangeCallback( 0 );
	SpewOutputFunc( 0 );
}


//-----------------------------------------------------------------------------
//
// Backward compatibility
//
//-----------------------------------------------------------------------------
#ifndef _XBOX
class CServerGameDLLV3 : public IServerGameDLL
{
public:
	CServerGameDLLV3( ServerGameDLLV3::IServerGameDLL *pServerGameDLL ) : 
		m_pServerGameDLL( pServerGameDLL ) 
	{
		m_bInittedSendProxies = false;
	}

	virtual bool DLLInit( CreateInterfaceFn engineFactory, CreateInterfaceFn physicsFactory, CreateInterfaceFn fileSystemFactory, CGlobalVars *pGlobals )
	{
		return m_pServerGameDLL->DLLInit( engineFactory, physicsFactory, fileSystemFactory, pGlobals );
	}

	virtual bool GameInit( void )
	{
		return m_pServerGameDLL->GameInit( );
	}

	virtual bool LevelInit( char const *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background )
	{
		return m_pServerGameDLL->LevelInit( pMapName, pMapEntities, pOldLevel, pLandmarkName, loadGame, background );
	}

	virtual void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
	{
		m_pServerGameDLL->ServerActivate( pEdictList, edictCount, clientMax );
	}

	virtual void GameFrame( bool simulating )
	{
		m_pServerGameDLL->GameFrame( simulating );
	}

	virtual void PreClientUpdate( bool simulating )
	{
		m_pServerGameDLL->PreClientUpdate( simulating );
	}

	virtual void LevelShutdown( void )
	{
		m_pServerGameDLL->LevelShutdown( );
	}

	virtual void GameShutdown( void )
	{
		m_pServerGameDLL->GameShutdown( );
	}

	virtual void DLLShutdown( void )
	{
		m_pServerGameDLL->DLLShutdown( );
	}

	virtual float GetTickInterval( void ) const
	{
		return m_pServerGameDLL->GetTickInterval( );
	}

	virtual ServerClass* GetAllServerClasses( void )
	{
		return m_pServerGameDLL->GetAllServerClasses( );
	}

	virtual const char *GetGameDescription( void )      
	{
		return m_pServerGameDLL->GetGameDescription( );
	}

	virtual void CreateNetworkStringTables( void )
	{
		return m_pServerGameDLL->CreateNetworkStringTables( );
	}

	virtual CSaveRestoreData *SaveInit( int size )
	{
		return m_pServerGameDLL->SaveInit( size );
	}

	virtual void SaveWriteFields( CSaveRestoreData *s, const char *c, void *v, datamap_t *d, typedescription_t *t, int i )
	{
		return m_pServerGameDLL->SaveWriteFields( s, c, v, d, t, i );
	}

	virtual void SaveReadFields( CSaveRestoreData *s, const char *c, void *v, datamap_t *d, typedescription_t *t, int i )
	{
		return m_pServerGameDLL->SaveReadFields( s, c, v, d, t, i );
	}

	virtual void SaveGlobalState( CSaveRestoreData *s )
	{
		m_pServerGameDLL->SaveGlobalState( s );
	}

	virtual void RestoreGlobalState( CSaveRestoreData *s )
	{
		m_pServerGameDLL->RestoreGlobalState( s );
	}

	virtual void PreSave( CSaveRestoreData *s )
	{
		m_pServerGameDLL->PreSave( s );
	}

	virtual void Save( CSaveRestoreData *s )
	{
		m_pServerGameDLL->Save( s );
	}

	virtual void GetSaveComment( char *comment, int maxlength, float flMinutes, float flSeconds, bool bNoTime = false )
	{
		m_pServerGameDLL->GetSaveComment( comment, maxlength );
	}

	virtual void PreSaveGameLoaded( char const *pSaveName, bool bCurrentlyInGame )
	{
	}

	virtual bool ShouldHideServer( void )
	{
		return false;
	}

	virtual void			InvalidateMdlCache()
	{
	}

	virtual void GetSaveCommentEx( char *comment, int maxlength, float flMinutes, float flSeconds )
	{
		m_pServerGameDLL->GetSaveComment( comment, maxlength );
	}

	virtual void WriteSaveHeaders( CSaveRestoreData *s )
	{
		m_pServerGameDLL->WriteSaveHeaders( s );
	}

	virtual void ReadRestoreHeaders( CSaveRestoreData *s )
	{
		m_pServerGameDLL->ReadRestoreHeaders( s );
	}

	virtual void Restore( CSaveRestoreData *s, bool b )
	{
		m_pServerGameDLL->Restore( s, b );
	}

	virtual bool IsRestoring()
	{
		return m_pServerGameDLL->IsRestoring( );
	}

	virtual int CreateEntityTransitionList( CSaveRestoreData *s, int i )
	{
		return m_pServerGameDLL->CreateEntityTransitionList( s, i );
	}

	virtual void BuildAdjacentMapList( void )
	{
		m_pServerGameDLL->BuildAdjacentMapList( );
	}

	virtual bool GetUserMessageInfo( int msg_type, char *name, int maxnamelength, int& size )
	{
		return m_pServerGameDLL->GetUserMessageInfo( msg_type, name, maxnamelength, size );
	}

	virtual CStandardSendProxies *GetStandardSendProxies()
	{
		if ( !m_bInittedSendProxies )
		{
			memset( &m_SendProxies, 0, sizeof( m_SendProxies ) );
			
			// Copy the version 1 info into the structure we export from here.
			CStandardSendProxiesV1 &out = m_SendProxies;
			const CStandardSendProxiesV1 &in = *m_pServerGameDLL->GetStandardSendProxies();
			out = in;
			
			m_bInittedSendProxies = true;
		}
		return &m_SendProxies;
	}

	virtual void PostInit()
	{
	}

	virtual void Think( bool finalTick )
	{
	}

	virtual void OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
	{
	}

private:
	ServerGameDLLV3::IServerGameDLL *m_pServerGameDLL;
	bool m_bInittedSendProxies;
	CStandardSendProxies m_SendProxies;
};
#endif


//
// Try to load a single DLL.  If it conforms to spec, keep it loaded, and add relevant
// info to the DLL directory.  If not, ignore it entirely.
//

CreateInterfaceFn g_ServerFactory;


#pragma optimize( "g", off )
static bool LoadThisDll( char *szDllFilename )
{
	CSysModule *pDLL = NULL;

	// Load DLL, ignore if cannot
	// ensures that the game.dll is running under Steam
	// this will have to be undone when we want mods to be able to run
	if ((pDLL = g_pFileSystem->LoadModule(szDllFilename, "GAMEBIN", false)) == NULL)
	{
		ConMsg("Failed to load %s\n", szDllFilename);
		goto IgnoreThisDLL;
	}

	// Load interface factory and any interfaces exported by the game .dll
	g_ServerFactory = Sys_GetFactory( pDLL );
	if ( g_ServerFactory )
	{
		g_bServerGameDLLGreaterThanV5 = true;
		g_bServerGameDLLGreaterThanV4 = true;			
		serverGameDLL = (IServerGameDLL*)g_ServerFactory(INTERFACEVERSION_SERVERGAMEDLL, NULL);
		if ( !serverGameDLL )
		{
#ifdef REL_TO_STAGING_MERGE_TODO
			// Need to merge eiface for this.
			g_bServerGameDLLGreaterThanV5 = false;
			serverGameDLL = (IServerGameDLL*)g_ServerFactory(INTERFACEVERSION_SERVERGAMEDLL_VERSION_5, NULL);
#endif			
			if ( !serverGameDLL )
			{
				g_bServerGameDLLGreaterThanV4 = false;
				serverGameDLL = (IServerGameDLL*)g_ServerFactory(INTERFACEVERSION_SERVERGAMEDLL_VERSION_4, NULL);

				if ( !serverGameDLL )
				{
#ifndef _XBOX
					ServerGameDLLV3::IServerGameDLL* pServerGameDLLV3 = (ServerGameDLLV3::IServerGameDLL*)g_ServerFactory( SERVERGAMEDLL_INTERFACEVERSION_3, NULL );
					if ( !pServerGameDLLV3 )
					{
						ConMsg( "Could not get IServerGameDLL interface from library %s", szDllFilename );
						goto IgnoreThisDLL;
					}
					serverGameDLL = new CServerGameDLLV3( pServerGameDLLV3 );				
#else
					Con_Printf( "Could not get IServerGameDLL interface from library %s", szDllFilename );
					goto IgnoreThisDLL;
#endif
				}
			}
		}


		serverGameEnts = (IServerGameEnts*)g_ServerFactory(INTERFACEVERSION_SERVERGAMEENTS, NULL);
		if ( !serverGameEnts )
		{
			ConMsg( "Could not get IServerGameEnts interface from library %s", szDllFilename );
			goto IgnoreThisDLL;
		}
		
		serverGameClients = (IServerGameClients*)g_ServerFactory(INTERFACEVERSION_SERVERGAMECLIENTS, NULL);
		if ( serverGameClients )
		{
			g_iServerGameClientsVersion = 4;
		}
		else
		{
			// Try the previous version.
			const char *pINTERFACEVERSION_SERVERGAMECLIENTS_V3 = "ServerGameClients003";
			serverGameClients = (IServerGameClients*)g_ServerFactory(pINTERFACEVERSION_SERVERGAMECLIENTS_V3, NULL);
			if ( serverGameClients )
			{
				g_iServerGameClientsVersion = 3;
			}
			else
			{
				ConMsg( "Could not get IServerGameClients interface from library %s", szDllFilename );
				goto IgnoreThisDLL;
			}
		}
		serverGameDirector = (IHLTVDirector*)g_ServerFactory(INTERFACEVERSION_HLTVDIRECTOR, NULL);
		if ( !serverGameDirector )
		{
			ConMsg( "Could not get IHLTVDirector interface from library %s", szDllFilename );
			// this is not a critical 
		}
	}
	else
	{
		ConMsg( "Could not find factory interface in library %s", szDllFilename );
		goto IgnoreThisDLL;
	}

	g_GameDLL = pDLL;
	return true;

IgnoreThisDLL:
	if (pDLL != NULL)
	{
		g_pFileSystem->UnloadModule(pDLL);
		serverGameDLL = NULL;
		serverGameEnts = NULL;
		serverGameClients = NULL;
	}
	return false;
}
#pragma optimize( "", on )

//
// Scan DLL directory, load all DLLs that conform to spec.
//
void LoadEntityDLLs( const char *szBaseDir )
{
	memset( &gmodinfo, 0, sizeof( modinfo_t ) );
	gmodinfo.version = 1;
	gmodinfo.svonly  = true;

	// Run through all DLLs found in the extension DLL directory
	g_GameDLL = NULL;
	sv_noclipduringpause = NULL;

	// Listing file for this game.
	KeyValues *modinfo = new KeyValues("modinfo");
	if (modinfo->LoadFromFile(g_pFileSystem, "gameinfo.txt"))
	{
		Q_strncpy( gmodinfo.szInfo, modinfo->GetString("url_info"), sizeof( gmodinfo.szInfo ) );
		Q_strncpy( gmodinfo.szDL, modinfo->GetString("url_dl"), sizeof( gmodinfo.szDL ) );
		gmodinfo.version = modinfo->GetInt("version");
		gmodinfo.size = modinfo->GetInt("size");
		gmodinfo.svonly = modinfo->GetInt("svonly") ? true : false;
		gmodinfo.cldll = modinfo->GetInt("cldll") ? true : false;
		Q_strncpy( gmodinfo.szHLVersion, modinfo->GetString("hlversion"), sizeof( gmodinfo.szHLVersion ) );
	}
	modinfo->deleteThis();
	
	// Load the game .dll
	char szDllFilename[ MAX_PATH ];
#ifdef _WIN32
	Q_snprintf( szDllFilename, sizeof( szDllFilename ), "server.dll" );
#elif _LINUX
	Q_snprintf( szDllFilename, sizeof( szDllFilename ), "server_i486.so" );
#else
#error "define server.dll type"
#endif

	LoadThisDll( szDllFilename );

	if ( serverGameDLL )
	{
		Msg("Game.dll loaded for \"%s\"\n", (char *)serverGameDLL->GetGameDescription());
	}
}

//-----------------------------------------------------------------------------
// Purpose: Retrieves a string value from the registry
//-----------------------------------------------------------------------------
#if defined(_WIN32)
void Sys_GetRegKeyValueUnderRoot( HKEY rootKey, const char *pszSubKey, const char *pszElement, char *pszReturnString, int nReturnLength, const char *pszDefaultValue )
{
	LONG lResult;           // Registry function result code
	HKEY hKey;              // Handle of opened/created key
	char szBuff[128];       // Temp. buffer
	ULONG dwDisposition;    // Type of key opening event
	DWORD dwType;           // Type of key
	DWORD dwSize;           // Size of element data

	// Assume the worst
	Q_strncpy(pszReturnString, pszDefaultValue, nReturnLength );

	// Create it if it doesn't exist.  (Create opens the key otherwise)
	lResult = VCRHook_RegCreateKeyEx(
		rootKey,	// handle of open key 
		pszSubKey,			// address of name of subkey to open 
		0ul,					// DWORD ulOptions,	  // reserved 
		"String",			// Type of value
		REG_OPTION_NON_VOLATILE, // Store permanently in reg.
		KEY_ALL_ACCESS,		// REGSAM samDesired, // security access mask 
		NULL,
		&hKey,				// Key we are creating
		&dwDisposition);    // Type of creation
	
	if (lResult != ERROR_SUCCESS)  // Failure
		return;

	// First time, just set to Valve default
	if (dwDisposition == REG_CREATED_NEW_KEY)
	{
		// Just Set the Values according to the defaults
		lResult = VCRHook_RegSetValueEx( hKey, pszElement, 0, REG_SZ, (CONST BYTE *)pszDefaultValue, Q_strlen(pszDefaultValue) + 1 ); 
	}
	else
	{
		// We opened the existing key. Now go ahead and find out how big the key is.
		dwSize = nReturnLength;
		lResult = VCRHook_RegQueryValueEx( hKey, pszElement, 0, &dwType, (unsigned char *)szBuff, &dwSize );

		// Success?
		if (lResult == ERROR_SUCCESS)
		{
			// Only copy strings, and only copy as much data as requested.
			if (dwType == REG_SZ)
			{
				Q_strncpy(pszReturnString, szBuff, nReturnLength);
				pszReturnString[nReturnLength - 1] = '\0';
			}
		}
		else
		// Didn't find it, so write out new value
		{
			// Just Set the Values according to the defaults
			lResult = VCRHook_RegSetValueEx( hKey, pszElement, 0, REG_SZ, (CONST BYTE *)pszDefaultValue, Q_strlen(pszDefaultValue) + 1 ); 
		}
	};

	// Always close this key before exiting.
	VCRHook_RegCloseKey(hKey);

}


//-----------------------------------------------------------------------------
// Purpose: Retrieves a DWORD value from the registry
//-----------------------------------------------------------------------------
void Sys_GetRegKeyValueUnderRootInt( HKEY rootKey, const char *pszSubKey, const char *pszElement, long *plReturnValue, const long lDefaultValue )
{
	LONG lResult;           // Registry function result code
	HKEY hKey;              // Handle of opened/created key
	ULONG dwDisposition;    // Type of key opening event
	DWORD dwType;           // Type of key
	DWORD dwSize;           // Size of element data

	// Assume the worst
	// Set the return value to the default
	*plReturnValue = lDefaultValue; 

	// Create it if it doesn't exist.  (Create opens the key otherwise)
	lResult = VCRHook_RegCreateKeyEx(
		rootKey,	// handle of open key 
		pszSubKey,			// address of name of subkey to open 
		0ul,					// DWORD ulOptions,	  // reserved 
		"String",			// Type of value
		REG_OPTION_NON_VOLATILE, // Store permanently in reg.
		KEY_ALL_ACCESS,		// REGSAM samDesired, // security access mask 
		NULL,
		&hKey,				// Key we are creating
		&dwDisposition);    // Type of creation

	if (lResult != ERROR_SUCCESS)  // Failure
		return;

	// First time, just set to Valve default
	if (dwDisposition == REG_CREATED_NEW_KEY)
	{
		// Just Set the Values according to the defaults
		lResult = VCRHook_RegSetValueEx( hKey, pszElement, 0, REG_DWORD, (CONST BYTE *)&lDefaultValue, sizeof( DWORD ) ); 
	}
	else
	{
		// We opened the existing key. Now go ahead and find out how big the key is.
		dwSize = sizeof( DWORD );
		lResult = VCRHook_RegQueryValueEx( hKey, pszElement, 0, &dwType, (unsigned char *)plReturnValue, &dwSize );

		// Success?
		if (lResult != ERROR_SUCCESS)
			// Didn't find it, so write out new value
		{
			// Just Set the Values according to the defaults
			lResult = VCRHook_RegSetValueEx( hKey, pszElement, 0, REG_DWORD, (LPBYTE)&lDefaultValue, sizeof( DWORD ) ); 
		}
	};

	// Always close this key before exiting.
	VCRHook_RegCloseKey(hKey);

}


void Sys_SetRegKeyValueUnderRoot( HKEY rootKey, const char *pszSubKey, const char *pszElement, const char *pszValue )
{
	LONG lResult;           // Registry function result code
	HKEY hKey;              // Handle of opened/created key
	//char szBuff[128];       // Temp. buffer
	ULONG dwDisposition;    // Type of key opening event
	//DWORD dwType;           // Type of key
	//DWORD dwSize;           // Size of element data

	// Create it if it doesn't exist.  (Create opens the key otherwise)
	lResult = VCRHook_RegCreateKeyEx(
		rootKey,			// handle of open key 
		pszSubKey,			// address of name of subkey to open 
		0ul,					// DWORD ulOptions,	  // reserved 
		"String",			// Type of value
		REG_OPTION_NON_VOLATILE, // Store permanently in reg.
		KEY_ALL_ACCESS,		// REGSAM samDesired, // security access mask 
		NULL,
		&hKey,				// Key we are creating
		&dwDisposition);    // Type of creation
	
	if (lResult != ERROR_SUCCESS)  // Failure
		return;

	// First time, just set to Valve default
	if (dwDisposition == REG_CREATED_NEW_KEY)
	{
		// Just Set the Values according to the defaults
		lResult = VCRHook_RegSetValueEx( hKey, pszElement, 0, REG_SZ, (CONST BYTE *)pszValue, Q_strlen(pszValue) + 1 ); 
	}
	else
	{
		/*
		// FIXE:  We might want to support a mode where we only create this key, we don't overwrite values already present
		// We opened the existing key. Now go ahead and find out how big the key is.
		dwSize = nReturnLength;
		lResult = VCRHook_RegQueryValueEx( hKey, pszElement, 0, &dwType, (unsigned char *)szBuff, &dwSize );

		// Success?
		if (lResult == ERROR_SUCCESS)
		{
			// Only copy strings, and only copy as much data as requested.
			if (dwType == REG_SZ)
			{
				Q_strncpy(pszReturnString, szBuff, nReturnLength);
				pszReturnString[nReturnLength - 1] = '\0';
			}
		}
		else
		*/
		// Didn't find it, so write out new value
		{
			// Just Set the Values according to the defaults
			lResult = VCRHook_RegSetValueEx( hKey, pszElement, 0, REG_SZ, (CONST BYTE *)pszValue, Q_strlen(pszValue) + 1 ); 
		}
	};

	// Always close this key before exiting.
	VCRHook_RegCloseKey(hKey);
}
#endif

void Sys_GetRegKeyValue( char *pszSubKey, char *pszElement,	char *pszReturnString, int nReturnLength, char *pszDefaultValue )
{
#if defined(_WIN32)
	Sys_GetRegKeyValueUnderRoot( HKEY_CURRENT_USER, pszSubKey, pszElement, pszReturnString, nReturnLength, pszDefaultValue );
#endif
}

void Sys_GetRegKeyValueInt( char *pszSubKey, char *pszElement, long *plReturnValue, long lDefaultValue)
{
#if defined(_WIN32)
	Sys_GetRegKeyValueUnderRootInt( HKEY_CURRENT_USER, pszSubKey, pszElement, plReturnValue, lDefaultValue );
#endif
}

void Sys_SetRegKeyValue( char *pszSubKey, char *pszElement,	const char *pszValue )
{
#if defined(_WIN32)
	Sys_SetRegKeyValueUnderRoot( HKEY_CURRENT_USER, pszSubKey, pszElement, pszValue );
#endif
}

#define SOURCE_ENGINE_APP_CLASS "Valve.Source"

void Sys_CreateFileAssociations( int count, FileAssociationInfo *list )
{
#if defined(_WIN32)
	if ( IsX360() )
		return;

	char appname[ 512 ];

	GetModuleFileName( 0, appname, sizeof( appname ) );
	Q_FixSlashes( appname );
	Q_strlower( appname );

	char quoted_appname_with_arg[ 512 ];
	Q_snprintf( quoted_appname_with_arg, sizeof( quoted_appname_with_arg ), "\"%s\" \"%%1\"", appname );
	char base_exe_name[ 256 ];
	Q_FileBase( appname, base_exe_name, sizeof( base_exe_name) );
	Q_DefaultExtension( base_exe_name, ".exe", sizeof( base_exe_name ) );

	// HKEY_CLASSES_ROOT/Valve.Source/shell/open/command == "u:\tf2\hl2.exe" "%1" quoted
	Sys_SetRegKeyValueUnderRoot( HKEY_CLASSES_ROOT, va( "%s\\shell\\open\\command", SOURCE_ENGINE_APP_CLASS ), "", quoted_appname_with_arg );
	// HKEY_CLASSES_ROOT/Applications/hl2.exe/shell/open/command == "u:\tf2\hl2.exe" "%1" quoted
	Sys_SetRegKeyValueUnderRoot( HKEY_CLASSES_ROOT, va( "Applications\\%s\\shell\\open\\command", base_exe_name ), "", quoted_appname_with_arg );

	for ( int i = 0; i < count ; i++ )
	{
		FileAssociationInfo *fa = &list[ i ];
		char binding[32];
		binding[0] = 0;
		// Create file association for our .exe
		// HKEY_CLASSES_ROOT/.dem == "Valve.Source"
		Sys_GetRegKeyValueUnderRoot( HKEY_CLASSES_ROOT, fa->extension, "", binding, sizeof(binding), "" );
		if ( Q_strlen( binding ) == 0 )
		{
			Sys_SetRegKeyValueUnderRoot( HKEY_CLASSES_ROOT, fa->extension, "", SOURCE_ENGINE_APP_CLASS );
		}
	}
#endif
}

void Sys_NoCrashDialog()
{
#if defined(_WIN32)
	::SetErrorMode(SetErrorMode(SEM_NOGPFAULTERRORBOX) | SEM_NOGPFAULTERRORBOX);
#endif
}

void Sys_TestSendKey( const char *pKey )
{
#if defined(_WIN32) && !defined(_XBOX)
	int key = pKey[0];
	if ( pKey[0] == '\\' && pKey[1] == 'r' )
	{
		key = VK_RETURN;
	}

	HWND hWnd = (HWND)game->GetMainWindow();
	PostMessageA( hWnd, WM_KEYDOWN, key, 0 );
	PostMessageA( hWnd, WM_KEYUP, key, 0 );

	//void Key_Event (int key, bool down);
	//Key_Event( key, 1 );
	//Key_Event( key, 0 );
#endif
}

void Sys_OutputDebugString(const char *msg)
{
	Plat_DebugString( msg );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void UnloadEntityDLLs( void )
{
	if ( !g_GameDLL )
		return;

	// Unlink the cvars associated with game DLL
	FileSystem_UnloadModule( g_GameDLL );
	g_GameDLL = NULL;
	serverGameDLL = NULL;
	serverGameEnts = NULL;
	serverGameClients = NULL;
	sv_noclipduringpause = NULL;
}

CON_COMMAND( star_memory, "Dump memory stats" )
{
	// get a current stat of available memory
	// 32 MB is reserved and fixed by OS, so not reporting to allow memory loggers sync
#ifdef _LINUX
	struct mallinfo memstats = mallinfo( );
	Msg( "sbrk size: %.2f MB, Used: %.2f MB, #mallocs = %d\n",
		 memstats.arena / ( 1024.0 * 1024.0), memstats.uordblks / ( 1024.0 * 1024.0 ), memstats.hblks );
#else
	MEMORYSTATUS stat;
	GlobalMemoryStatus( &stat );
	Msg( "Available: %.2f MB, Used: %.2f MB, Free: %.2f MB\n", 
		stat.dwTotalPhys/( 1024.0f*1024.0f ) - 32.0f,
		( stat.dwTotalPhys - stat.dwAvailPhys )/( 1024.0f*1024.0f ) - 32.0f, 
		stat.dwAvailPhys/( 1024.0f*1024.0f ) );
#endif
}
