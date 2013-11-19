//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "tier0/vprof.h"
#include "server.h"
#include "host_cmd.h"
#include "keys.h"
#include "screen.h"
#include "vengineserver_impl.h"
#include "host_saverestore.h"
#include "sv_filter.h"
#include "gl_matsysiface.h"
#include "pr_edict.h"
#include "world.h"
#include "checksum_engine.h"
#include "const.h"
#include "sv_main.h"
#include "host.h"
#include "demo.h"
#include "cdll_int.h"
#include "networkstringtableserver.h"
#include "networkstringtableclient.h"
#include "host_state.h"
#include "string_t.h"
#include "tier0/dbg.h"
#include "testscriptmgr.h"
#include "r_local.h"
#include "PlayerState.h"
#include "enginesingleuserfilter.h"
#include "profile.h"
#include "proto_version.h"
#include "protocol.h"
#include "cl_main.h"
#include "sv_steamauth.h"
#include "zone.h"
#include "datacache/idatacache.h"
#include "sys_dll.h"
#include "cmd.h"
#include "tier0/icommandline.h"
#include "filesystem.h"
#include "filesystem_engine.h"
#include "icliententitylist.h"
#include "icliententity.h"
#include "hltvserver.h"
#include "cdll_engine_int.h"
#ifndef SWDS
#include "vgui_baseui_interface.h"
#endif
#ifdef _WIN32
#include "sound.h"
#include "voice.h"
#endif
#include "sv_rcon.h"
#if defined( _X360 )
#include "xbox/xbox_console.h"
#include "xbox/xbox_launch.h"
#endif
#include "filesystem/IQueuedLoader.h"

#include "ixboxsystem.h"
extern IXboxSystem *g_pXboxSystem;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define STEAM_PREFIX "STEAM_"

#ifndef SWDS
bool g_bInEditMode = false;
bool g_bInCommentaryMode = false;
#endif

static void host_name_changed_f( IConVar *var, const char *pOldValue, float flOldValue )
{
	Steam3Server().NotifyOfServerNameChange();
}

ConVar	host_name		( "hostname", "", 0, "Hostname for server.", host_name_changed_f );
ConVar host_map( "host_map", "", 0, "Current map name." );

ConVar voice_recordtofile("voice_recordtofile", "0", 0, "Record mic data and decompressed voice data into 'voice_micdata.wav' and 'voice_decompressed.wav'");
ConVar voice_inputfromfile("voice_inputfromfile", "0", 0, "Get voice input from 'voice_input.wav' rather than from the microphone.");

char gpszVersionString[32];
char gpszProductString[32];
int g_iSteamAppID = 215;	// defaults to Source SDK Base (215) if no steam.inf can be found.

// Globals
int	gHostSpawnCount = 0;

// If any quit handlers balk, then aborts quit sequence
bool EngineTool_CheckQuitHandlers();

#if defined( _X360 )
CON_COMMAND( quit_x360, "" )
{
	int launchFlags = LF_EXITFROMGAME;

	// allocate the full payload
	int nPayloadSize = XboxLaunch()->MaxPayloadSize();
	byte *pPayload = (byte *)stackalloc( nPayloadSize );
	V_memset( pPayload, 0, sizeof( nPayloadSize ) );

	// payload is at least the command line
	// any user data needed must be placed AFTER the command line
	const char *pCmdLine = CommandLine()->GetCmdLine();
	int nCmdLineLength = (int)strlen( pCmdLine ) + 1;
	V_memcpy( pPayload, pCmdLine, min( nPayloadSize, nCmdLineLength ) );

	// add any other data here to payload, after the command line
	// ...

	// storage device may have changed since previous launch
	XboxLaunch()->SetStorageID( XBX_GetStorageDeviceId() );

	// Close the storage devices
	g_pXboxSystem->CloseContainers();
	// persist the user id
	bool bInviteRestart = args.FindArg( "invite" );
	DWORD nUserID = ( bInviteRestart ) ? XBX_GetInvitedUserId() : XBX_GetPrimaryUserId();
	XboxLaunch()->SetUserID( nUserID );

	if ( args.FindArg( "restart" ) )
	{
		launchFlags |= LF_GAMERESTART;
	}
	
	// If we're relaunching due to invite
	if ( bInviteRestart )
	{
		launchFlags |= LF_INVITERESTART;
		XNKID nSessionID = XBX_GetInviteSessionId();
		XboxLaunch()->SetInviteSessionID( &nSessionID );
	}

	bool bLaunch = XboxLaunch()->SetLaunchData( pPayload, nPayloadSize, launchFlags );
	if ( bLaunch )
	{
		COM_TimestampedLog( "Launching: \"%s\" Flags: 0x%8.8x", pCmdLine, XboxLaunch()->GetLaunchFlags() );
		g_pMaterialSystem->PersistDisplay();
		XBX_DisconnectConsoleMonitor();
		XboxLaunch()->Launch();
	}
}
#endif

/*
==================
Host_Quit_f
==================
*/
void Host_Quit_f (void)
{
#if !defined(SWDS)
	if ( !EngineTool_CheckQuitHandlers() )
	{
		return;
	}
#endif

	HostState_Shutdown();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CON_COMMAND( _restart, "Shutdown and restart the engine." )
{
	/*
	// FIXME:  How to handle restarts?
#ifndef SWDS
	if ( !EngineTool_CheckQuitHandlers() )
	{
		return;
	}
#endif
	*/

	HostState_Restart();
}

#ifndef SWDS
//-----------------------------------------------------------------------------
// A console command to spew out driver information
//-----------------------------------------------------------------------------
void Host_LightCrosshair (void);

static ConCommand light_crosshair( "light_crosshair", Host_LightCrosshair, "Show texture color at crosshair", FCVAR_CHEAT );

void Host_LightCrosshair (void)
{
	Vector endPoint;
	Vector lightmapColor;

	// max_range * sqrt(3)
	VectorMA( MainViewOrigin(), COORD_EXTENT * 1.74f, MainViewForward(), endPoint );
	
	R_LightVec( MainViewOrigin(), endPoint, true, lightmapColor );
	int r = LinearToTexture( lightmapColor.x );
	int g = LinearToTexture( lightmapColor.y );
	int b = LinearToTexture( lightmapColor.z );

	ConMsg( "Luxel Value: %d %d %d\n", r, g, b );
}
#endif

/*
==================
Host_Status_PrintClient

Print client info to console 
==================
*/
void Host_Status_PrintClient( IClient *client, bool bShowAddress, void (*print) (const char *fmt, ...) )
{
	INetChannelInfo *nci = client->GetNetChannel();

	const char *state = "challenging";

	if ( client->IsActive() )
		state = "active";
	else if ( client->IsSpawned() )
		state = "spawning";
	else if ( client->IsConnected() )
		state = "connecting";
	
	if ( nci != NULL )
	{
		print( "# %2i \"%s\" %s %s %i %i %s", 
			client->GetUserID(), client->GetClientName(), client->GetNetworkIDString(), COM_FormatSeconds( nci->GetTimeConnected() ),
			(int)(1000.0f*nci->GetAvgLatency( FLOW_OUTGOING )), (int)(100.0f*nci->GetAvgLoss(FLOW_INCOMING)), state );

		if ( bShowAddress ) 
		{
			print( " %s", nci->GetAddress() );
		}
	}
	else
	{
		print( "#%2i \"%s\" %s %s", 
			client->GetUserID(), client->GetClientName(), client->GetNetworkIDString(), state );
	}
	
	print( "\n" );

}

void Host_Client_Printf(const char *fmt, ...)
{
	va_list		argptr;
	char		string[1024];

	va_start (argptr,fmt);
	Q_vsnprintf (string, sizeof( string ), fmt,argptr);
	va_end (argptr);

	host_client->ClientPrintf( "%s", string );
}

//-----------------------------------------------------------------------------
// Host_Status_f
//-----------------------------------------------------------------------------
CON_COMMAND( status, "Display map and connection status." )
{
	IClient	*client;
	int j;
	void (*print) (const char *fmt, ...);

#if defined( _X360 )
	Vector org;
	QAngle ang;
	const char *pName;

	if ( cl.IsActive() )
	{
		pName = cl.m_szLevelNameShort;
		org = MainViewOrigin();
		VectorAngles( MainViewForward(), ang );
		IClientEntity *localPlayer = entitylist->GetClientEntity( cl.m_nPlayerSlot + 1 );
		if ( localPlayer )
		{
			org = localPlayer->GetAbsOrigin();
		}
	}
	else
	{
		pName = "";
		org.Init();
		ang.Init();
	}

	// send to vxconsole
	xMapInfo_t mapInfo;
	mapInfo.position[0] = org[0];
	mapInfo.position[1] = org[1];
	mapInfo.position[2] = org[2];
	mapInfo.angle[0]    = ang[0];
	mapInfo.angle[1]    = ang[1];
	mapInfo.angle[2]    = ang[2];
	mapInfo.build       = build_number();
	mapInfo.skill       = skill.GetInt();

	// generate the qualified path where .sav files are expected to be written
	char savePath[MAX_PATH];
	V_snprintf( savePath, sizeof( savePath ), "%s", saverestore->GetSaveDir() );
	V_StripTrailingSlash( savePath );
	g_pFileSystem->RelativePathToFullPath( savePath, "MOD", mapInfo.savePath, sizeof( mapInfo.savePath ) );
	V_FixSlashes( mapInfo.savePath );

	if ( pName[0] )
	{
		// generate the qualified path from where the map was loaded
		char mapPath[MAX_PATH];
		Q_snprintf( mapPath, sizeof( mapPath ), "maps/%s.360.bsp", pName );
		g_pFileSystem->GetLocalPath( mapPath, mapInfo.mapPath, sizeof( mapInfo.mapPath ) );
		Q_FixSlashes( mapInfo.mapPath );
	}
	else
	{
		mapInfo.mapPath[0] = '\0';
	}

	XBX_rMapInfo( &mapInfo );
#endif

	if ( cmd_source == src_command )
	{
		if ( !sv.IsActive() )
		{
			Cmd_ForwardToServer( args );
			return;
		}
		print = ConMsg;
	}
	else
	{
		print = Host_Client_Printf;
	}

	// ============================================================
	// Server status information.
	print( "hostname: %s\n", host_name.GetString() );

	const char *pchSecureReasonString = "";
	bool bGSSecure = Steam3Server().BSecure();
	if ( !bGSSecure && Steam3Server().BWantsSecure() )
	{
		if ( Steam3Server().BLoggedOn() )
		{
			pchSecureReasonString = "(secure mode enabled, connected to Steam3)";
		}
		else
		{
			pchSecureReasonString = "(secure mode enabled, disconnected from Steam3)";
		}
	}

	print( "version : %s/%d %d %s %s\n", gpszVersionString, PROTOCOL_VERSION, build_number(), bGSSecure ? "secure" : "insecure", pchSecureReasonString );
	
	if ( NET_IsMultiplayer() )
	{
		print( "udp/ip  :  %s:%i\n", net_local_adr.ToString(true), sv.GetUDPPort() );
	}

	print( "map     : %s at: %d x, %d y, %d z\n", sv.GetMapName(), (int)MainViewOrigin()[0], (int)MainViewOrigin()[1], (int)MainViewOrigin()[2]);

	if ( hltv && hltv->IsActive() )
	{
		print( "sourcetv:  port %i, delay %.1fs\n", hltv->GetUDPPort(), hltv->GetDirector()->GetDelay() );
	}

	int players = sv.GetNumClients();

	print( "players : %i (%i max)\n\n", players, sv.GetMaxClients() );
	// ============================================================

	// Early exit for this server.
	if ( args.ArgC() == 2 )
	{
		if ( !Q_stricmp( args[1], "short" ) )
		{
			for ( j=0 ; j < sv.GetClientCount() ; j++ )
			{
				client = sv.GetClient( j );

				if ( !client->IsActive() )
					continue;

				print( "#%i - %s\n" , j + 1, client->GetClientName() );
			}
			return;
		}
	}

	// the header for the status rows
	print( "# userid name uniqueid connected ping loss state" );
	if (cmd_source == src_command)
	{
		print( " adr" ); 
	}
	print( "\n" );

	for ( j=0 ; j < sv.GetClientCount() ; j++ )
	{
		client = sv.GetClient( j );

		if ( !client->IsConnected() )
			continue; // not connected yet, maybe challenging
		
		Host_Status_PrintClient( client, (cmd_source == src_command), print );
	}
}


//-----------------------------------------------------------------------------
// Host_Ping_f
//-----------------------------------------------------------------------------
CON_COMMAND( ping, "Display ping to server." )
{
	if ( cmd_source == src_command )
	{
		Cmd_ForwardToServer( args );
		return;
	}

	host_client->ClientPrintf( "Client ping times:\n" );

	for ( int i=0; i< sv.GetClientCount(); i++ )
	{
		IClient	*client = sv.GetClient(i);

		if ( !client->IsConnected() || client->IsFakeClient() )
			continue;

		host_client->ClientPrintf ("%4.0f ms : %s\n", 
			1000.0f * client->GetNetChannel()->GetAvgLatency( FLOW_OUTGOING ), client->GetClientName() );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : editmode - 
//-----------------------------------------------------------------------------
extern void GetPlatformMapPath( const char *pMapPath, char *pPlatformMapPath, int maxLength );

bool CL_HL2Demo_MapCheck( const char *name )
{
	if ( IsPC() && CL_IsHL2Demo() && !sv.IsDedicated() )
	{
		if (    !Q_stricmp( name, "d1_trainstation_01" ) || 
				!Q_stricmp( name, "d1_trainstation_02" ) || 
				!Q_stricmp( name, "d1_town_01" ) || 
				!Q_stricmp( name, "d1_town_01a" ) || 
				!Q_stricmp( name, "d1_town_02" ) || 
				!Q_stricmp( name, "d1_town_03" ) ||
				!Q_stricmp( name, "background01" ) ||
				!Q_stricmp( name, "background03" ) 
			)
		{
			return true;
		}
		return false;
	}

	return true;
}

bool CL_PortalDemo_MapCheck( const char *name )
{
	if ( IsPC() && CL_IsPortalDemo() && !sv.IsDedicated() )
	{
		if (    !Q_stricmp( name, "testchmb_a_00" ) || 
				!Q_stricmp( name, "testchmb_a_01" ) || 
				!Q_stricmp( name, "testchmb_a_02" ) || 
				!Q_stricmp( name, "testchmb_a_03" ) || 
				!Q_stricmp( name, "testchmb_a_04" ) || 
				!Q_stricmp( name, "testchmb_a_05" ) ||
				!Q_stricmp( name, "testchmb_a_06" ) ||
				!Q_stricmp( name, "background1" ) 
			)
		{
			return true;
		}
		return false;
	}

	return true;
}

void Host_Map_Helper( const CCommand &args, bool bEditmode, bool bBackground, bool bCommentary )
{
	int		i;
	char	name[MAX_QPATH];

	if ( cmd_source != src_command )
		return;
	

	if (args.ArgC() < 2)
	{
		Warning("No map specified\n");
		return;
	}

	GetPlatformMapPath( args[1], name, sizeof( name ) );

	COM_TimestampedLog( "*** Map Load: %s", name );
	
	// If I was in edit mode reload config file
	// to overwrite WC edit key bindings
#if !defined(SWDS)
	if ( !bEditmode )
	{
		if ( g_bInEditMode )
		{
			// Re-read config from disk
			Host_ReadConfiguration();
			g_bInEditMode = false;
		}
	}
	else
	{
		g_bInEditMode = true;
	}

	g_bInCommentaryMode = bCommentary;
#endif

	// If there is a .bsp on the end, strip it off!
	i = Q_strlen(name);
	if ( i > 4 && !Q_strcasecmp( name + i - 4, ".bsp" ) )
	{
		name[i-4] = 0;
	}

	if ( !g_pVEngineServer->IsMapValid( name ) )
	{
		Warning( "map load failed: %s not found or invalid\n", name );
		return;
	}

	if ( !CL_HL2Demo_MapCheck( name ) )
	{
		Warning( "map load failed: %s not found or invalid\n", name );
		return;	
	}

	if ( !CL_PortalDemo_MapCheck( name ) )
	{
		Warning( "map load failed: %s not found or invalid\n", name );
		return;	
	}

	// Stop demo loop
	cl.demonum = -1;

	Host_Disconnect( false );	// stop old game

	HostState_NewGame( name, false, bBackground );

	if (args.ArgC() == 10)
	{
		if (Q_stricmp(args[2], "setpos") == 0
			&& Q_stricmp(args[6], "setang") == 0) 
		{
			Vector newpos;
			newpos.x = atof( args[3] );
			newpos.y = atof( args[4] );
			newpos.z = atof( args[5] );

			QAngle newangle;
			newangle.x = atof( args[7] );
			newangle.y = atof( args[8] );
			newangle.z = atof( args[9] );
			
			HostState_SetSpawnPoint(newpos, newangle);
		}
	}
}

/*
======================
Host_Map_f

handle a 
map <servername>
command from the console.  Active clients are kicked off.
======================
*/
void Host_Map_f( const CCommand &args )
{
	Host_Map_Helper( args, false, false, false );
}


//-----------------------------------------------------------------------------
// handle a map_edit <servername> command from the console. 
// Active clients are kicked off.
// UNDONE: protect this from use if not in dev. mode
//-----------------------------------------------------------------------------
#ifndef SWDS
CON_COMMAND( map_edit, "" )
{
	Host_Map_Helper( args, true, false, false );
}
#endif


//-----------------------------------------------------------------------------
// Purpose: Runs a map as the background
//-----------------------------------------------------------------------------
void Host_Map_Background_f( const CCommand &args )
{
	Host_Map_Helper( args, false, true, false );
}


//-----------------------------------------------------------------------------
// Purpose: Runs a map in commentary mode
//-----------------------------------------------------------------------------
void Host_Map_Commentary_f( const CCommand &args )
{
	Host_Map_Helper( args, false, false, true );
}


//-----------------------------------------------------------------------------
// Restarts the current server for a dead player
//-----------------------------------------------------------------------------
CON_COMMAND( restart, "Restart the game on the same level (add setpos to jump to current view position on restart)." )
{
	if ( 
#if !defined(SWDS)
		demoplayer->IsPlayingBack() || 
#endif
		!sv.IsActive() )
		return;

	if ( sv.IsMultiplayer() )
		return;

	if ( cmd_source != src_command )
		return;

	bool bRememberLocation = ( args.ArgC() == 2 && !Q_stricmp( args[1], "setpos" ) );

	Host_Disconnect(false);	// stop old game

	if ( !CL_HL2Demo_MapCheck( sv.GetMapName() ) )
	{
		Warning( "map load failed: %s not found or invalid\n", sv.GetMapName() );
		return;	
	}

	if ( !CL_PortalDemo_MapCheck( sv.GetMapName() ) )
	{
		Warning( "map load failed: %s not found or invalid\n", sv.GetMapName() );
		return;	
	}

	HostState_NewGame( sv.GetMapName(), bRememberLocation, false );
}


//-----------------------------------------------------------------------------
// Restarts the current server for a dead player
//-----------------------------------------------------------------------------
CON_COMMAND( reload, "Reload the most recent saved game (add setpos to jump to current view position on reload).")
{
#ifndef SWDS
	const char *pSaveName;
	char name[MAX_OSPATH];
#endif

	if ( 
#if !defined(SWDS)
		demoplayer->IsPlayingBack() || 
#endif
		!sv.IsActive() )
		return;

	if ( sv.IsMultiplayer() )
		return;

	if (cmd_source != src_command)
		return;

	bool remember_location = false;
	if ( args.ArgC() == 2 && 
		!Q_stricmp( args[1], "setpos" ) )
	{
		remember_location = true;
	}

	// See if there is a most recently saved game
	// Restart that game if there is
	// Otherwise, restart the starting game map
#ifndef SWDS
	pSaveName = saverestore->FindRecentSave( name, sizeof( name ) );

	// Put up loading plaque
  	SCR_BeginLoadingPlaque();

	Host_Disconnect( false );	// stop old game

	if ( pSaveName && saverestore->SaveFileExists( pSaveName ) )
	{
		HostState_LoadGame( pSaveName, remember_location );
	}
	else
#endif
	{
		if ( !CL_HL2Demo_MapCheck( host_map.GetString() ) )
		{
			Warning( "map load failed: %s not found or invalid\n", host_map.GetString() );
			return;	
		}

		if ( !CL_PortalDemo_MapCheck( host_map.GetString() ) )
		{
			Warning( "map load failed: %s not found or invalid\n", host_map.GetString() );
			return;	
		} 

		HostState_NewGame( host_map.GetString(), remember_location, false );
	}
}


//-----------------------------------------------------------------------------
// Purpose: Goes to a new map, taking all clients along
// Output : void Host_Changelevel_f
//-----------------------------------------------------------------------------
void Host_Changelevel_f( const CCommand &args )
{
	if ( args.ArgC() < 2 )
	{
		ConMsg( "changelevel <levelname> : continue game on a new level\n" );
		return;
	}

	if ( !sv.IsActive() )
	{
		ConMsg( "Can't changelevel, not running server\n" );
		return;
	}

	if ( !g_pVEngineServer->IsMapValid( args[1] ) )
	{
		Warning( "changelevel failed: %s not found\n", args[1] );
		return;
	}

	if ( !CL_HL2Demo_MapCheck(args[1]) )
	{
		Warning( "changelevel failed: %s not found\n", args[1] );
		return;	
	}

	if ( !CL_PortalDemo_MapCheck(args[1]) )
	{
		Warning( "changelevel failed: %s not found\n", args[1] );
		return;	
	}

	HostState_ChangeLevelMP( args[1], args[2] );
}

//-----------------------------------------------------------------------------
// Purpose: Changing levels within a unit, uses save/restore
//-----------------------------------------------------------------------------
void Host_Changelevel2_f( const CCommand &args )
{
	if ( args.ArgC() < 2 )
	{
		ConMsg ("changelevel2 <levelname> : continue game on a new level in the unit\n");
		return;
	}

	if ( !sv.IsActive() )
	{
		ConMsg( "Can't changelevel2, not in a map\n" );
		return;
	}

	if ( !g_pVEngineServer->IsMapValid( args[1] ) )
	{
		if ( !CL_IsHL2Demo() || (CL_IsHL2Demo() && !(!Q_stricmp( args[1], "d1_trainstation_03" ) || !Q_stricmp( args[1], "d1_town_02a" ))) )	
		{
			Warning( "changelevel2 failed: %s not found\n", args[1] );
			return;
		}
	}

#if !defined(SWDS)
	// needs to be before CL_HL2Demo_MapCheck() check as d1_trainstation_03 isn't a valid map
	if ( IsPC() && CL_IsHL2Demo() && !sv.IsDedicated() && !Q_stricmp( args[1], "d1_trainstation_03" ) ) 
	{
		void CL_DemoTransitionFromTrainstation();
		CL_DemoTransitionFromTrainstation();
		return; 
	}

	// needs to be before CL_HL2Demo_MapCheck() check as d1_trainstation_03 isn't a valid map
	if ( IsPC() && CL_IsHL2Demo() && !sv.IsDedicated() && !Q_stricmp( args[1], "d1_town_02a" ) && !Q_stricmp( args[2], "d1_town_02_02a" )) 
	{
		void CL_DemoTransitionFromRavenholm();
		CL_DemoTransitionFromRavenholm();
		return; 
	}

	if ( IsPC() && CL_IsPortalDemo() && !sv.IsDedicated() && !Q_stricmp( args[1], "testchmb_a_07" ) ) 
	{
		void CL_DemoTransitionFromTestChmb();
		CL_DemoTransitionFromTestChmb();
		return; 
	}

#endif

	// allow a level transition to d1_trainstation_03 so the Host_Changelevel() can act on it
	if ( !CL_HL2Demo_MapCheck( args[1] ) ) 
	{
		Warning( "changelevel failed: %s not found\n", args[1] );
		return;	
	}

	HostState_ChangeLevelSP( args[1], args[2] );
}


//-----------------------------------------------------------------------------
// Purpose: Shut down client connection and any server
//-----------------------------------------------------------------------------
void Host_Disconnect( bool bShowMainMenu )
{
	if ( IsX360() )
	{
		g_pQueuedLoader->EndMapLoading( false );
	}

#ifndef SWDS
	if ( !sv.IsDedicated() )
	{
		cl.Disconnect(bShowMainMenu);
	}
#endif
	HostState_GameShutdown();
}


//-----------------------------------------------------------------------------
// Kill the client and any local server.
//-----------------------------------------------------------------------------
CON_COMMAND( disconnect, "Disconnect game from server." )
{
	cl.demonum = -1;
	Host_Disconnect(true);
}


//-----------------------------------------------------------------------------
// Output : void Host_Version_f
//-----------------------------------------------------------------------------
#define VERSION_KEY "PatchVersion="
#define PRODUCT_KEY "ProductName="
#define APPID_KEY "AppID="
#define PRODUCT_STRING "valve"
#define VERSION_STRING "1.0.1.0"

void Host_Version( void )
{
	char szFileName[MAX_OSPATH];
	char *buffer;
	int bufsize = 0;
	FileHandle_t fp = NULL;
	const char *pbuf = NULL;
	int gotKeys = 0;

	Q_strcpy( gpszVersionString, VERSION_STRING );
	Q_strcpy( gpszProductString, PRODUCT_STRING );

	sprintf( szFileName, "steam.inf" );

   // Mod's steam.inf is first option, the the steam.inf in the game GCF. 
	fp = g_pFileSystem->Open( szFileName, "r" );
	if ( fp )
	{
		bufsize = g_pFileSystem->Size( fp );
		buffer = ( char * )_alloca( bufsize + 1 );
		Assert( buffer );

		int iBytesRead = g_pFileSystem->Read( buffer, bufsize, fp );
		g_pFileSystem->Close( fp );

		buffer[iBytesRead] = '\0';

		// Read 
		pbuf = buffer;

		while ( 1 )
		{
			pbuf = COM_Parse( pbuf );
			if ( !pbuf )
				break;

			if ( Q_strlen( com_token )  <= 0 )
				break;

			if ( gotKeys >= 3 )
			{
				break;
			}

			if ( !Q_strnicmp( com_token, VERSION_KEY, Q_strlen( VERSION_KEY ) ) )
			{
				Q_strncpy( gpszVersionString, com_token+Q_strlen( VERSION_KEY ), sizeof( gpszVersionString ) - 1 );
				gpszVersionString[ sizeof( gpszVersionString ) - 1 ] = '\0';
				gotKeys++;
				continue;
			}

			if ( !Q_strnicmp( com_token, PRODUCT_KEY, Q_strlen( PRODUCT_KEY ) ) )
			{
				Q_strncpy( gpszProductString, com_token+Q_strlen( PRODUCT_KEY ), sizeof( gpszProductString ) - 1 );
				gpszProductString[ sizeof( gpszProductString ) - 1 ] = '\0';
				gotKeys++;
				continue;
			}

			if ( !Q_strnicmp( com_token, APPID_KEY, Q_strlen( APPID_KEY ) ) )
			{
				char szAppID[32];
				Q_strncpy( szAppID, com_token + Q_strlen( APPID_KEY ), sizeof( szAppID ) - 1 );
				g_iSteamAppID = atoi(szAppID);
				gotKeys++;
				continue;
			}
		}

	}
}

CON_COMMAND( version, "Print version info string." )
{
	ConMsg( "Protocol version %i\nExe version %s (%s)\n", PROTOCOL_VERSION, gpszVersionString, gpszProductString );
	ConMsg( "Exe build: "__TIME__" "__DATE__" (%i)\n", build_number() );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CON_COMMAND( pause, "Toggle the server pause state." )
{
#ifndef SWDS
	if ( !sv.IsDedicated() )
	{
		if ( !cl.m_szLevelName[ 0 ] )
			return;
	}
#endif

	if ( cmd_source == src_command )
	{
		Cmd_ForwardToServer( args );
		return;
	}

	if ( !sv.IsPausable() )
		return;

	// toggle paused state
	sv.SetPaused( !sv.IsPaused() );
	
	// send text messaage who paused the game
	sv.BroadcastPrintf( "%s %s the game\n", host_client->GetClientName(), sv.IsPaused() ? "paused" : "unpaused" );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CON_COMMAND( setpause, "Set the pause state of the server." )
{
#ifndef SWDS
	if ( !cl.m_szLevelName[ 0 ] )
		return;
#endif

	if ( cmd_source == src_command )
	{
		Cmd_ForwardToServer( args );
		return;
	}

	sv.SetPaused( true );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CON_COMMAND( unpause, "Unpause the game." )
{
#ifndef SWDS
	if ( !cl.m_szLevelName[ 0 ] )
		return;
#endif

	if ( cmd_source == src_command )
	{
		Cmd_ForwardToServer( args );
		return;
	}
	
	sv.SetPaused( false );
}


//-----------------------------------------------------------------------------
// Kicks a user off of the server using their userid or uniqueid
//-----------------------------------------------------------------------------
CON_COMMAND( kickid, "Kick a player by userid or uniqueid, with a message." )
{
	char		*who = "Console";
	const char	*pszArg1 = NULL, *pszMessage = NULL;
	IClient		*client = NULL;
	int			iSearchIndex = -1;
	char		szSearchString[128];
	int			argsStartNum = 1;
	bool		bSteamID = false;
	int			i = 0;

	if ( args.ArgC() <= 1 )
	{
		ConMsg( "Usage:  kickid < userid | uniqueid > { message }\n" );
		return;
	}

	// get the first argument
	pszArg1 = args[1];

	// if the first letter is a charcter then
	// we're searching for a uniqueid ( e.g. STEAM_ )
	if ( *pszArg1 < '0' || *pszArg1 > '9' )
	{
		// SteamID (need to reassemble it)
		if ( !Q_strnicmp( pszArg1, STEAM_PREFIX, strlen( STEAM_PREFIX ) ) && Q_strstr( args[2], ":" ) )
		{
			Q_snprintf( szSearchString, sizeof( szSearchString ), "%s:%s:%s", pszArg1, args[3], args[5] );
			argsStartNum = 5;
			bSteamID = true;
		}
		// some other ID (e.g. "UNKNOWN", "STEAM_ID_PENDING", "STEAM_ID_LAN")
		// NOTE: assumed to be one argument
		else
		{
			Q_snprintf( szSearchString, sizeof( szSearchString ), "%s", pszArg1 );
		}
	}
	// this is a userid
	else
	{
		iSearchIndex = Q_atoi( pszArg1 );
	}

	// check for a message
	if ( args.ArgC() > argsStartNum )
	{
		int j;
		int dataLen = 0;

		pszMessage = args.ArgS();
		for ( j = 1; j <= argsStartNum; j++ )
		{
			dataLen += Q_strlen( args[j] ) + 1; // +1 for the space between args
		}

		if ( bSteamID )
		{
			dataLen -= 5; // SteamIDs don't have spaces between the args[) values
		}

		if ( dataLen > Q_strlen( pszMessage ) ) // saftey check
		{
			pszMessage = NULL;
		}
		else
		{
			pszMessage += dataLen;
		}
	}

	// find this client
	for ( i = 0; i < sv.GetClientCount(); i++ )
	{
		client = sv.GetClient( i );

		if ( !client->IsConnected() )
		{
			continue;
		}

		// searching by UserID
		if ( iSearchIndex != -1 )
		{
			if ( client->GetUserID() == iSearchIndex )
			{
				// found!
				break;
			}
		}
		// searching by UniqueID
		else	
		{
			if ( Q_stricmp( client->GetNetworkIDString(), szSearchString ) == 0 ) 
			{
				// found!
				break;
			}
		}
	}

	// now kick them
	if ( i < sv.GetClientCount() )
	{
		if ( cmd_source != src_command )
		{
			who = host_client->m_Name;
		}

		// can't kick yourself!
		if ( host_client == client && !sv.IsDedicated() )
		{
			return;
		}

		if ( iSearchIndex != -1 || !client->IsFakeClient() )
		{
			if ( pszMessage )
			{
				client->Disconnect( "Kicked by %s : %s", who, pszMessage );
			}
			else
			{
				client->Disconnect( "Kicked by %s", who );
			}
		}
	}
	else
	{
		if ( iSearchIndex != -1 )
		{
			ConMsg( "userid \"%d\" not found\n", iSearchIndex );
		}
		else
		{
			ConMsg( "uniqueid \"%s\" not found\n", szSearchString );			
		}
	}
}

/*
==================
Host_Kick_f

Kicks a user off of the server using their name
==================
*/
CON_COMMAND( kick, "Kick a player by name." )
{
	char		*who = "Console";
	char		*pszName = NULL;
	IClient		*client = NULL;
	int			i = 0;
	char		name[64];

	if ( args.ArgC() <= 1 )
	{
		ConMsg( "Usage:  kick < name >\n" );
		return;
	}

	// copy the name to a local buffer
	memset( name, 0, sizeof(name) );
	Q_strncpy( name, args.ArgS(), sizeof(name) );
	pszName = name;

	// safety check
	if ( pszName && pszName[0] != 0 )
	{
		//HACK-HACK
		// check for the name surrounded by quotes (comes in this way from rcon)
		int len = Q_strlen( pszName ) - 1; // (minus one since we start at 0)
		if ( pszName[0] == '"' && pszName[len] == '"' )
		{
			// get rid of the quotes at the beginning and end
			pszName[len] = 0;
			pszName++;
		}

		for ( i = 0; i < sv.GetClientCount(); i++ )
		{
			client = sv.GetClient(i);

			if ( !client->IsConnected() )
				continue;

			// found!
			if ( Q_strcasecmp( client->GetClientName(), pszName ) == 0 ) 
				break;
		}

		// now kick them
		if ( i < sv.GetClientCount() )
		{
			if ( cmd_source != src_command )
			{
				who = host_client->m_Name;
			}

			// can't kick yourself!
			if ( host_client == client && !sv.IsDedicated() )
				return;

			client->Disconnect( "Kicked by %s", who );
		}
		else
		{
			ConMsg( "name \"%s\" not found\n", pszName );
		}
	}
}

/*
===============================================================================

DEBUGGING TOOLS

===============================================================================
*/


//-----------------------------------------------------------------------------
// Dump memory stats
//-----------------------------------------------------------------------------
CON_COMMAND( memory, "Print memory stats." )
{
	ConMsg( "Heap Used:\n" );
	int nTotal = g_pMemAlloc->GetSize( 0 );
	if (nTotal == -1)
	{
		ConMsg( "Corrupted!\n" );
	}
	else
	{
		ConMsg( "%5.2f MB (%d bytes)\n", nTotal/(1024.0f*1024.0f), nTotal );
	}

#ifdef VPROF_ENABLED
	ConMsg("\nVideo Memory Used:\n");
	CVProfile *pProf = &g_VProfCurrentProfile;
	int prefixLen = strlen( "TexGroup_Global_" );
	float total = 0.0f;
	for ( int i=0; i < pProf->GetNumCounters(); i++ )
	{
		if ( pProf->GetCounterGroup( i ) == COUNTER_GROUP_TEXTURE_GLOBAL )
		{
			float value = pProf->GetCounterValue( i ) * (1.0f/(1024.0f*1024.0f) );
			total += value;
			const char *pName = pProf->GetCounterName( i );
			if ( !Q_strnicmp( pName, "TexGroup_Global_", prefixLen ) )
			{
				pName += prefixLen;
			}
			ConMsg( "%5.2f MB: %s\n", value, pName );
		}
	}
	ConMsg("------------------\n");
	ConMsg( "%5.2f MB: total\n", total );
#endif

	ConMsg( "\nHunk Memory Used:\n" );
	Hunk_Print();
}

/*
===============================================================================

DEMO LOOP CONTROL

===============================================================================
*/


#ifndef SWDS

//MOTODO move all demo commands to demoplayer


//-----------------------------------------------------------------------------
// Purpose: Gets number of valid demo names
// Output : int
//-----------------------------------------------------------------------------
int Host_GetNumDemos()
{
	int c = 0;
#ifndef SWDS
	for ( int i = 0; i < MAX_DEMOS; ++i )
	{
		const char *demoname = cl.demos[ i ];
		if ( !demoname[ 0 ] )
			break;

		++c;
	}
#endif
	return c;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Host_PrintDemoList()
{
	int count = Host_GetNumDemos();

	int next = cl.demonum;
	if ( next >= count || next < 0 )
	{
		next = 0;
	}

#ifndef SWDS
	for ( int i = 0; i < MAX_DEMOS; ++i )
	{
		const char *demoname = cl.demos[ i ];
		if ( !demoname[ 0 ] )
			break;

		bool isnextdemo = next == i ? true : false;

		DevMsg( "%3s % 2i : %20s\n", isnextdemo ? "-->" : "   ", i, cl.demos[ i ] );
	}
#endif

	if ( !count )
	{
		DevMsg( "No demos in list, use startdemos <demoname> <demoname2> to specify\n" );
	}
}


#ifndef SWDS
//-----------------------------------------------------------------------------
//
// Con commands related to demos, not available on dedicated servers
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Purpose: Specify list of demos for the "demos" command
//-----------------------------------------------------------------------------
CON_COMMAND( startdemos, "Play demos in demo sequence." )
{
	int	c = args.ArgC() - 1;
	if (c > MAX_DEMOS)
	{
		Msg ("Max %i demos in demoloop\n", MAX_DEMOS);
		c = MAX_DEMOS;
	}
	Msg ("%i demo(s) in loop\n", c);

	for ( int i=1 ; i<c+1 ; i++ )
	{
		Q_strncpy( cl.demos[i-1], args[i], sizeof(cl.demos[0]) );
	}

	cl.demonum = 0;

	Host_PrintDemoList();

	if ( !sv.IsActive() && !demoplayer->IsPlayingBack() )
	{
		CL_NextDemo ();
	}
	else
	{
		cl.demonum = -1;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Return to looping demos, optional resume demo index
//-----------------------------------------------------------------------------
CON_COMMAND( demos, "Demo demo file sequence." )
{
	int oldn = cl.demonum;
	cl.demonum = -1;
	Host_Disconnect(false);
	cl.demonum = oldn;

	if (cl.demonum == -1)
		cl.demonum = 0;

	if ( args.ArgC() == 2 )
	{
		int numdemos = Host_GetNumDemos();
		if ( numdemos >= 1 )
		{
			cl.demonum = clamp( Q_atoi( args[1] ), 0, numdemos - 1 );
			DevMsg( "Jumping to %s\n", cl.demos[ cl.demonum ] );
		}
	}

	Host_PrintDemoList();

	CL_NextDemo ();
}

//-----------------------------------------------------------------------------
// Purpose: Stop current demo
//-----------------------------------------------------------------------------
CON_COMMAND_F( stopdemo, "Stop playing back a demo.", FCVAR_DONTRECORD )
{
	if ( !demoplayer->IsPlayingBack() )
		return;
	
	Host_Disconnect (true);
}

//-----------------------------------------------------------------------------
// Purpose: Skip to next demo
//-----------------------------------------------------------------------------
CON_COMMAND( nextdemo, "Play next demo in sequence." )
{
	if ( args.ArgC() == 2 )
	{
		int numdemos = Host_GetNumDemos();
		if ( numdemos >= 1 )
		{
			cl.demonum = clamp( Q_atoi( args[1] ), 0, numdemos - 1 );
			DevMsg( "Jumping to %s\n", cl.demos[ cl.demonum ] );
		}
	}
	Host_EndGame( false, "Moving to next demo..." );
}

//-----------------------------------------------------------------------------
// Purpose: Print out the current demo play order
//-----------------------------------------------------------------------------
CON_COMMAND( demolist, "Print demo sequence list." )
{
	Host_PrintDemoList();
}


//-----------------------------------------------------------------------------
// Purpose: Host_Soundfade_f
//-----------------------------------------------------------------------------
CON_COMMAND_F( soundfade, "Fade client volume.", FCVAR_SERVER_CAN_EXECUTE )
{
	float percent;
	float inTime, holdTime, outTime;

	if (args.ArgC() != 3 && args.ArgC() != 5)
	{
		Msg("soundfade <percent> <hold> [<out> <int>]\n");
		return;
	}

	percent = clamp( atof(args[1]), 0.0f, 100.0f );
	
	holdTime = max( 0.0f, atof(args[2]) );

	inTime = 0.0f;
	outTime = 0.0f;
	if (args.ArgC() == 5)
	{
		outTime = max( 0.0f, atof(args[3]) );
		inTime = max( 0.0f, atof( args[4]) );
	}

	S_SoundFade( percent, holdTime, outTime, inTime );
}

#endif // !SWDS

#endif


//-----------------------------------------------------------------------------
// Shutdown the server
//-----------------------------------------------------------------------------
CON_COMMAND( killserver, "Shutdown the server." )
{
	Host_Disconnect(true);
	
	if ( !sv.IsDedicated() )
	{
		// close network sockets
		NET_SetMutiplayer( false );
	}
}

#if !defined(SWDS)
void Host_VoiceRecordStart_f(void)
{
	if ( cl.IsActive() )
	{
		const char *pUncompressedFile = NULL;
		const char *pDecompressedFile = NULL;
		const char *pInputFile = NULL;
		
		if (voice_recordtofile.GetInt())
		{
			pUncompressedFile = "voice_micdata.wav";
			pDecompressedFile = "voice_decompressed.wav";
		}

		if (voice_inputfromfile.GetInt())
		{
			pInputFile = "voice_input.wav";
		}
#if !defined( NO_VOICE )
		if (Voice_RecordStart(pUncompressedFile, pDecompressedFile, pInputFile))
		{
		}
#endif
	}
}


void Host_VoiceRecordStop_f(void)
{
	if ( cl.IsActive() )
	{
#if !defined( NO_VOICE )
		if (Voice_IsRecording())
		{
			CL_SendVoicePacket(true);
			Voice_RecordStop();
		}
#endif
	}
}
#endif


//-----------------------------------------------------------------------------
// Purpose: Wrapper for modelloader->Print() function call
//-----------------------------------------------------------------------------
CON_COMMAND( listmodels, "List loaded models." )
{
	modelloader->Print();
}

/*
==================
Host_IncrementCVar
==================
*/
CON_COMMAND_F( incrementvar, "Increment specified convar value.", FCVAR_DONTRECORD )
{
	if( args.ArgC() != 5 )
	{
		Warning( "Usage: incrementvar varName minValue maxValue delta\n" );
		return;
	}

	const char *varName = args[ 1 ];
	if( !varName )
	{
		ConDMsg( "Host_IncrementCVar_f without a varname\n" );
		return;
	}

	ConVar *var = ( ConVar * )g_pCVar->FindVar( varName );
	if( !var )
	{
		ConDMsg( "cvar \"%s\" not found\n", varName );
		return;
	}

	float currentValue = var->GetFloat();
	float startValue = atof( args[ 2 ] );
	float endValue = atof( args[ 3 ] );
	float delta = atof( args[ 4 ] );
	float newValue = currentValue + delta;
	if( newValue > endValue )
	{
		newValue = startValue;
	}
	else if ( newValue < startValue )
	{
		newValue = endValue;
	}

	// Conver incrementvar command to direct sets to avoid any problems with state in a demo loop.
	Cbuf_AddText( va("%s %f", varName, newValue) );

	ConDMsg( "%s = %f\n", var->GetName(), newValue );
}


//-----------------------------------------------------------------------------
// Host_MultiplyCVar_f
//-----------------------------------------------------------------------------
CON_COMMAND_F( multvar, "Multiply specified convar value.", FCVAR_DONTRECORD )
{
	if (( args.ArgC() != 5 ))
	{
		Warning( "Usage: multvar varName minValue maxValue factor\n" );
		return;
	}

	const char *varName = args[ 1 ];
	if( !varName )
	{
		ConDMsg( "multvar without a varname\n" );
		return;
	}

	ConVar *var = ( ConVar * )g_pCVar->FindVar( varName );
	if( !var )
	{
		ConDMsg( "cvar \"%s\" not found\n", varName );
		return;
	}

	float currentValue = var->GetFloat();
	float startValue = atof( args[ 2 ] );
	float endValue = atof( args[ 3 ] );
	float factor = atof( args[ 4 ] );
	float newValue = currentValue * factor;
	if( newValue > endValue )
	{
		newValue = endValue;
	}
	else if ( newValue < startValue )
	{
		newValue = startValue;
	}

	// Conver incrementvar command to direct sets to avoid any problems with state in a demo loop.
	Cbuf_AddText( va("%s %f", varName, newValue) );

	ConDMsg( "%s = %f\n", var->GetName(), newValue );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CON_COMMAND( dumpstringtables, "Print string tables to console." )
{
	SV_PrintStringTables();
#ifndef SWDS
	CL_PrintStringTables();
#endif
}

// Register shared commands
ConCommand quit("quit", Host_Quit_f, "Exit the engine.");
static ConCommand cmd_exit("exit", Host_Quit_f, "Exit the engine.");

#ifndef SWDS
#ifdef VOICE_OVER_IP
static ConCommand startvoicerecord("+voicerecord", Host_VoiceRecordStart_f);
static ConCommand endvoicerecord("-voicerecord", Host_VoiceRecordStop_f);
#endif // VOICE_OVER_IP

#endif


#ifdef _DEBUG
//-----------------------------------------------------------------------------
// Purpose: Force a null pointer crash. useful for testing minidumps
//-----------------------------------------------------------------------------
void Host_Crash_f()
{ 
	char *p = 0;
	*p = 0;
}
static ConCommand crash( "crash", Host_Crash_f, "Cause the engine to crash (Debug!!)" );
#endif

CON_COMMAND_F( flush, "Flush unlocked cache memory.", FCVAR_CHEAT )
{
#if !defined( SWDS )
	g_ClientDLL->InvalidateMdlCache();
#endif // SWDS
	serverGameDLL->InvalidateMdlCache();
	g_pDataCache->Flush( true );
}

CON_COMMAND_F( flush_locked, "Flush unlocked and locked cache memory.", FCVAR_CHEAT )
{
#if !defined( SWDS )
	g_ClientDLL->InvalidateMdlCache();
#endif // SWDS
	serverGameDLL->InvalidateMdlCache();
	g_pDataCache->Flush( false );
}

CON_COMMAND( cache_print, "cache_print [section]\nPrint out contents of cache memory." )
{
	const char *pszSection = NULL;
	if ( args.ArgC() == 2 )
	{
		pszSection = args[ 1 ];
	}
	g_pDataCache->OutputReport( DC_DETAIL_REPORT, pszSection );
}

CON_COMMAND( cache_print_lru, "cache_print_lru [section]\nPrint out contents of cache memory." )
{
	const char *pszSection = NULL;
	if ( args.ArgC() == 2 )
	{
		pszSection = args[ 1 ];
	}
	g_pDataCache->OutputReport( DC_DETAIL_REPORT_LRU, pszSection );
}

CON_COMMAND( cache_print_summary, "cache_print_summary [section]\nPrint out a summary contents of cache memory." )
{
	const char *pszSection = NULL;
	if ( args.ArgC() == 2 )
	{
		pszSection = args[ 1 ];
	}
	g_pDataCache->OutputReport( DC_SUMMARY_REPORT, pszSection );
}
