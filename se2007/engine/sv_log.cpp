//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Header: $
// $NoKeywords: $
//=============================================================================//

#include "server_pch.h"
#include <time.h>
#include "server.h"
#include "sv_log.h"
#include "filesystem.h"
#include "filesystem_engine.h"
#include "tier0/vcrmode.h"
#include "sv_main.h"
#include "tier0/icommandline.h"
#include <proto_oob.h>
#include "GameEventManager.h"
#include "netadr.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar sv_logsdir( "sv_logsdir", "logs", FCVAR_ARCHIVE, "Folder in the game directory where server logs will be stored." );
static ConVar sv_logfile( "sv_logfile", "1", FCVAR_ARCHIVE, "Log server information in the log file." );
static ConVar sv_logflush( "sv_logflush", "0", FCVAR_ARCHIVE, "Flush the log file to disk on each write (slow)." );
static ConVar sv_logecho( "sv_logecho", "1", FCVAR_ARCHIVE, "Echo log information to the console." );
static ConVar sv_log_onefile( "sv_log_onefile", "0", FCVAR_ARCHIVE, "Log server information to only one file." );
static ConVar sv_logbans( "sv_logbans", "0", FCVAR_ARCHIVE, "Log server bans in the server logs." ); // should sv_banid() calls be logged in the server logs?

CLog g_Log;	// global Log object

CON_COMMAND( log, "Enables logging to file, console, and udp < on | off >." )
{
	if ( args.ArgC() != 2 )
	{
		ConMsg( "Usage:  log < on | off >\n" );

		if ( g_Log.IsActive() )
		{
			bool bHaveFirst = false;

			ConMsg( "currently logging to: " );

			if ( sv_logfile.GetInt() )
			{
				ConMsg( "file" );
				bHaveFirst = true;
			}

			if ( sv_logecho.GetInt() )
			{
				if ( bHaveFirst )
				{
					ConMsg( ", console" );
				}
				else
				{
					ConMsg( "console" );
					bHaveFirst = true;
				}
			}

			if ( g_Log.UsingLogAddress() )
			{
				if ( bHaveFirst )
				{
					ConMsg( ", udp" );
				}
				else
				{
					ConMsg( "udp" );
					bHaveFirst = true;
				}
			}
	
			if ( !bHaveFirst )
			{
				ConMsg( "no destinations! (file, console, or udp)\n" );	
				ConMsg( "check \"sv_logfile\", \"sv_logecho\", and \"logaddress_list\"" );	
			}

			ConMsg( "\n" );
		}
		else 
		{
			ConMsg( "not currently logging\n" );
		}
		return;
	}

	if ( !Q_stricmp( args[1], "off" ) || !Q_stricmp( args[1], "0" ) )
	{
		if ( g_Log.IsActive() )
		{
			g_Log.Close();
			g_Log.SetLoggingState( false );
			ConMsg( "Server logging disabled.\n" );
		}
	}
	else if ( !Q_stricmp( args[1], "on" ) || !Q_stricmp( args[1], "1" ) )
	{
		g_Log.SetLoggingState( true );
		ConMsg( "Server logging enabled.\n" );
		g_Log.Open();
	}
	else
	{
		ConMsg( "log:  unknown parameter %s, 'on' and 'off' are valid\n", args[1] );
	}
}

// changed log_addaddress back to logaddress_add to be consistent with GoldSrc
CON_COMMAND( logaddress_add, "Set address and port for remote host <ip:port>." )
{
	netadr_t adr;
	const char *pszIP, *pszPort;

	if ( args.ArgC() != 4 && args.ArgC() != 2 )
	{
		ConMsg( "Usage:  logaddress_add ip:port\n" );
		return;
	}

	pszIP = args[1];

	if ( args.ArgC() == 4 )
	{
		pszPort	= args[3];
	}
	else
	{
		pszPort = Q_strstr( pszIP, ":" );

		// if we have "IP:port" as one argument inside quotes
		if ( pszPort )
		{
			// add one to remove the :
			pszPort++;
		}
		else
		{
			// default port
			pszPort = "27015";
		}
	}

	if ( !Q_atoi( pszPort ) )
	{
		ConMsg( "logaddress_add:  must specify a valid port\n" );
		return;
	}

	if ( !pszIP || !pszIP[0] )
	{
		ConMsg( "logaddress_add:  unparseable address\n" );
		return;
	}

	char szAdr[32];
	Q_snprintf( szAdr, sizeof( szAdr ), "%s:%s", pszIP, pszPort );

	if ( NET_StringToAdr( szAdr, &adr ) )
	{
		if ( g_Log.AddLogAddress( adr ) )
		{
			ConMsg( "logaddress_add:  %s\n", adr.ToString() );
		}
		else
		{
			ConMsg( "logaddress_add:  %s is already in the list\n", adr.ToString() );
		}
	}
	else
	{
		ConMsg( "logaddress_add:  unable to resolve %s\n", szAdr );
	}
}

CON_COMMAND( logaddress_delall, "Remove all udp addresses being logged to" )
{
	g_Log.DelAllLogAddress();
}

CON_COMMAND( logaddress_del, "Remove address and port for remote host <ip:port>." )
{
	netadr_t adr;
	const char *pszIP, *pszPort;

	if ( args.ArgC() != 4 && args.ArgC() != 2 )
	{
		ConMsg( "Usage:  logaddress_del ip:port\n" );
		return;
	}

	pszIP = args[1];

	if ( args.ArgC() == 4 )
	{
		pszPort	= args[3];
	}
	else
	{
		pszPort = Q_strstr( pszIP, ":" );

		// if we have "IP:port" as one argument inside quotes
		if ( pszPort )
		{
			// add one to remove the :
			pszPort++;
		}
		else
		{
			// default port
			pszPort = "27015";
		}
	}

	if ( !Q_atoi( pszPort ) )
	{
		ConMsg( "logaddress_del:  must specify a valid port\n" );
		return;
	}

	if ( !pszIP || !pszIP[0] )
	{
		ConMsg( "logaddress_del:  unparseable address\n" );
		return;
	}

	char szAdr[32];
	Q_snprintf( szAdr, sizeof( szAdr ), "%s:%s", pszIP, pszPort );

	if ( NET_StringToAdr( szAdr, &adr ) )
	{
		if ( g_Log.DelLogAddress( adr ) )
		{
			ConMsg( "logaddress_del:  %s\n", adr.ToString() );
		}
		else
		{
			ConMsg( "logaddress_del:  address %s not found in the list\n", adr.ToString() );
		}
	}
	else
	{
		ConMsg( "logaddress_del:  unable to resolve %s\n", szAdr );
	}
}

CON_COMMAND( logaddress_list, "List all addresses currently being used by logaddress." )
{
	g_Log.ListLogAddress();
}

CLog::CLog()
{
	Reset();
}

CLog::~CLog()
{

}

void CLog::Reset( void )	// reset all logging streams
{
	m_LogAddresses.RemoveAll();
	m_hLogFile = FILESYSTEM_INVALID_HANDLE;
	m_bActive = false;
	m_flLastLogFlush = realtime;
	m_bFlushLog = false;
#ifndef _XBOX
	if ( CommandLine()->CheckParm( "-flushlog" ) )
	{
		m_bFlushLog = true;
	}
#endif
}

void CLog::Init( void )
{
	Reset();

	// listen to these events
	g_GameEventManager.AddListener( this, "server_spawn", true );
	g_GameEventManager.AddListener( this, "server_shutdown", true );
	g_GameEventManager.AddListener( this, "server_cvar", true );
	g_GameEventManager.AddListener( this, "server_message", true );
	g_GameEventManager.AddListener( this, "server_addban", true );
	g_GameEventManager.AddListener( this, "server_removeban", true );
}

void CLog::Shutdown()
{
	Close();
	Reset();
	g_GameEventManager.RemoveListener( this );
}

void CLog::SetLoggingState( bool state )
{
	m_bActive = state;
}

void CLog::RunFrame() 
{
	if ( m_bFlushLog && m_hLogFile != FILESYSTEM_INVALID_HANDLE && ( realtime - m_flLastLogFlush ) > 1.0f )
	{
		m_flLastLogFlush = realtime;
		g_pFileSystem->Flush( m_hLogFile );
	}
}

bool CLog::AddLogAddress(netadr_t addr)
{
	int i = 0;
	
	for ( i = 0; i < m_LogAddresses.Count(); ++i )
	{
		if ( m_LogAddresses.Element(i).CompareAdr(addr, false) )
		{
			// found!
			break;
		}
	}

	if ( i < m_LogAddresses.Count() )
	{
		// already in the list
		return false;
	}

	m_LogAddresses.AddToTail( addr );
	return true;
}

bool CLog::DelLogAddress(netadr_t addr)
{
	int i = 0;
	
	for ( i = 0; i < m_LogAddresses.Count(); ++i )
	{
		if ( m_LogAddresses.Element(i).CompareAdr(addr, false) )
		{
			// found!
			break;
		}
	}

	if ( i < m_LogAddresses.Count() )
	{
		m_LogAddresses.Remove(i);
		return true;
	}

	return false;
}

void CLog::ListLogAddress( void )
{
	netadr_t *pElement;
	const char *pszAdr;
	int count = m_LogAddresses.Count();

	if ( count <= 0 )
	{
		ConMsg( "logaddress_list:  no addresses in the list\n" );
	}
	else
	{
		if ( count == 1 )
		{
			ConMsg( "logaddress_list: %i entry\n", count );
		}
		else
		{
			ConMsg( "logaddress_list: %i entries\n", count );
		}

		for ( int i = 0 ; i < count ; ++i )
		{
			pElement = &m_LogAddresses.Element(i);
			pszAdr = pElement->ToString();
			
			ConMsg( "%s\n", pszAdr );
		}
	}
}

bool CLog::UsingLogAddress( void )
{
	return ( m_LogAddresses.Count() > 0 );
}

void CLog::DelAllLogAddress( void )
{
	if ( m_LogAddresses.Count() > 0 )
	{
		ConMsg( "logaddress_delall:  all addresses cleared\n" );
		m_LogAddresses.RemoveAll();
	}
	else
	{
		ConMsg( "logaddress_delall:  no addresses in the list\n" );
	}
}

/*
==================
Log_PrintServerVars

==================
*/
void CLog::PrintServerVars( void )
{
	const ConCommandBase	*var;			// Temporary Pointer to cvars

	if ( !IsActive() )
	{
		return;
	}

	Printf( "server cvars start\n" );
	// Loop through cvars...
	for ( var= g_pCVar->GetCommands() ; var ; var=var->GetNext() )
	{
		if ( var->IsCommand() )
			continue;

		if ( !( var->IsFlagSet( FCVAR_NOTIFY ) ) )
			continue;

		Printf( "\"%s\" = \"%s\"\n", var->GetName(), ((ConVar*)var)->GetString() );
	}

	Printf( "server cvars end\n" );
}

bool CLog::IsActive( void )
{
	return m_bActive;
}

/*
==================
Log_Printf

Prints a log message to the server's log file, console, and possible a UDP address
==================
*/
void CLog::Printf( const char *fmt, ... )
{
	va_list			argptr;
	static char		string[1024];
	
	if ( !IsActive() )
	{
		return;
	}

	va_start ( argptr, fmt );
	Q_vsnprintf ( string, sizeof( string ), fmt, argptr );
	va_end   ( argptr );

	Print( string );
}

void CLog::Print( const char * text )
{
	static char	string[1100];

	if ( !IsActive() || !text || !text[0] )
	{
		return;
	}

	if ( Q_strlen( text ) > 1024 )
	{
		DevMsg( 1, "CLog::Print: string too long (>1024 bytes)." );
		return;
	}

	tm today;
	VCRHook_LocalTime( &today );

	Q_snprintf( string, sizeof( string ), "L %02i/%02i/%04i - %02i:%02i:%02i: %s",
		today.tm_mon+1, today.tm_mday, 1900 + today.tm_year,
		today.tm_hour, today.tm_min, today.tm_sec, text );

	// Echo to server console
	if ( sv_logecho.GetInt() ) 
	{
		ConMsg( "%s", string );
	}

	// Echo to log file
	if ( sv_logfile.GetInt() && ( m_hLogFile != FILESYSTEM_INVALID_HANDLE ) )
	{
		g_pFileSystem->FPrintf( m_hLogFile, "%s", string );
		if ( sv_logflush.GetBool() )
		{
			g_pFileSystem->Flush( m_hLogFile );
		}
	}

	// Echo to UDP port
	if ( m_LogAddresses.Count() > 0 )
	{
		// out of band sending
		for ( int i = 0 ; i < m_LogAddresses.Count() ; i++ )
		{
			NET_OutOfBandPrintf(NS_SERVER, m_LogAddresses.Element(i), "%c%s", S2A_LOGSTRING, string );
		}
	}
}

void CLog::FireGameEvent( IGameEvent *event )
{
	if ( !IsActive() )
	{
		return;
	}

	// log server events

	const char * name = event->GetName();

	if ( !name || !name[0])
	{
		return;
	}

	if ( Q_strcmp(name, "server_spawn") == 0 )
	{
		Printf( "Started map \"%s\" (CRC \"%i\")\n", sv.GetMapName(), ( int ) sv.worldmapCRC );
	}

	else if ( Q_strcmp(name, "server_shutdown") == 0 )
	{
		Printf( "server_message: \"%s\"\n", event->GetString("reason") );
	}

	else if ( Q_strcmp(name, "server_cvar") == 0 )
	{
		Printf( "server_cvar: \"%s\" \"%s\"\n", event->GetString("cvarname"), event->GetString("cvarvalue")  );
	}

	else if ( Q_strcmp(name, "server_message") == 0 )
	{
		Printf( "server_message: \"%s\"\n", event->GetString("text") );
	}
	
	else if ( Q_strcmp(name, "server_addban") == 0 )
	{
		if ( sv_logbans.GetInt() > 0 )
		{
			const int userid = event->GetInt( "userid" );
			const char *pszName = event->GetString( "name" );
			const char *pszNetworkid = event->GetString( "networkid" );	
			const char *pszIP = event->GetString( "ip" );	
			const char *pszDuration = event->GetString( "duration" );
			const char *pszCmdGiver = event->GetString( "by" );
			const char *pszResult = NULL;
			
			if ( Q_strlen( pszIP ) > 0 )
			{
				pszResult = event->GetInt( "kicked" ) > 0 ? "was kicked and banned by IP" : "was banned by IP";

				if ( userid > 0 )
				{
					Printf( "Addip: \"%s<%i><%s><>\" %s \"%s\" by \"%s\" (IP \"%s\")\n", 
										pszName,
										userid, 
										pszNetworkid, 
										pszResult,
										pszDuration,
										pszCmdGiver,
										pszIP );
				}
				else
				{
					Printf( "Addip: \"<><><>\" %s \"%s\" by \"%s\" (IP \"%s\")\n", 
										pszResult,
										pszDuration,
										pszCmdGiver,
										pszIP );
				}
			}
			else
			{
				pszResult = event->GetInt( "kicked" ) > 0 ? "was kicked and banned" : "was banned";

				if ( userid > 0 )
				{
					Printf( "Banid: \"%s<%i><%s><>\" %s \"%s\" by \"%s\"\n", 
										pszName,
										userid, 
										pszNetworkid, 
										pszResult,
										pszDuration,
										pszCmdGiver );
				}
				else
				{
					Printf( "Banid: \"<><%s><>\" %s \"%s\" by \"%s\"\n", 
										pszNetworkid, 
										pszResult,
										pszDuration,
										pszCmdGiver );
				}
			}
		}
	}

	else if ( Q_strcmp(name, "server_removeban") == 0 )
	{
		if ( sv_logbans.GetInt() > 0 )
		{
			const char *pszNetworkid = event->GetString( "networkid" );	
			const char *pszIP = event->GetString( "ip" );	
			const char *pszCmdGiver = event->GetString( "by" );
			
			if ( Q_strlen( pszIP ) > 0 )
			{
				Printf( "Removeip: \"<><><>\" was unbanned by \"%s\" (IP \"%s\")\n", 
								pszCmdGiver,
								pszIP );
			}
			else
			{
				Printf( "Removeid: \"<><%s><>\" was unbanned by \"%s\"\n", 
								pszNetworkid, 
								pszCmdGiver );
			}
		}		
	}
}

/*
====================
Log_Close

Close logging file
====================
*/
void CLog::Close( void )
{
	if ( m_hLogFile != FILESYSTEM_INVALID_HANDLE )
	{
		Printf( "Log file closed\n" );
		g_pFileSystem->Close( m_hLogFile );
	}

	m_hLogFile = FILESYSTEM_INVALID_HANDLE;
}

/*
====================
Log_Open

Open logging file
====================
*/
void CLog::Open( void )
{
	char szFileBase[ MAX_OSPATH ];
	char szTestFile[ MAX_OSPATH ];
	int i;
	FileHandle_t fp = 0;

	if ( !m_bActive || !sv_logfile.GetInt() )
	{
		return;
	}

	// do we already have a log file (and we only want one)?
	if ( m_hLogFile && sv_log_onefile.GetInt() )
	{
		return;		
	}

	Close();

	// Find a new log file slot
	tm today;
	VCRHook_LocalTime( &today );
	const char *pszLogsDir = sv_logsdir.GetString();

	// safety check for invalid paths
	if ( !COM_IsValidPath( pszLogsDir ) )
	{
		pszLogsDir = "logs";
	}
		
	Q_snprintf( szFileBase, sizeof( szFileBase ), "%s/L%02i%02i", pszLogsDir, today.tm_mon + 1, today.tm_mday );

	for ( i = 0; i < 1000; i++ )
	{
		Q_snprintf( szTestFile, sizeof( szTestFile ), "%s%03i.log", szFileBase, i );

		Q_FixSlashes( szTestFile );
		COM_CreatePath( szTestFile );

		fp = g_pFileSystem->Open( szTestFile, "r", "LOGDIR" );
		if ( !fp )
		{
			COM_CreatePath( szTestFile );

			fp = g_pFileSystem->Open( szTestFile, "wt", "LOGDIR" );
			if ( !fp )
			{
				i = 1000;
			}
			else
			{
				ConMsg( "Server logging data to file %s\n", szTestFile );
			}
			break;
		}
		g_pFileSystem->Close( fp );
	}

	if ( i == 1000 )
	{
		ConMsg( "Unable to open logfiles under %s\nLogging disabled\n", szFileBase );
		return;
	}

	if ( fp )
	{
		m_hLogFile = fp;
	}
	Printf( "Log file started (file \"%s\") (game \"%s\") (version \"%i\")\n", szTestFile, com_gamedir, build_number() );
}


