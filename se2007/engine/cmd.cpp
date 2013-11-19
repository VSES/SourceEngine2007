//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//===========================================================================//


#include "quakedef.h"						 
#include "zone.h"
#include "tier0/vcrmode.h"
#include "demo.h"
#include "filesystem.h"
#include "filesystem_engine.h"
#include "eiface.h"
#include "server.h"
#include "sys.h"
#include "baseautocompletefilelist.h"
#include "tier0/icommandline.h"
#include "tier1/utlbuffer.h"
#include "gl_cvars.h"
#include "tier0/memalloc.h"
#include "netmessages.h"
#include "client.h"
#include "sv_plugin.h"
#include "tier1/CommandBuffer.h"
#include "cvar.h"
#include "vstdlib/random.h"
#include "tier1/utldict.h"
#include "tier0/vprof.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


// This denotes an execution marker in the command stream.
#define CMDSTR_ADD_EXECUTION_MARKER "[$&*,`]"


#ifdef _DEBUG
ConVar cl_debug_respect_cheat_vars( "cl_debug_respect_cheat_vars", "0", 0, "(debug builds only) - when set to 0, the client can change cheat vars." );
#endif


extern ConVar sv_allow_wait_command;


#define	MAX_ALIAS_NAME	32
#define MAX_COMMAND_LENGTH 1024

struct cmdalias_t
{
	cmdalias_t	*next;
	char		name[ MAX_ALIAS_NAME ];
	char		*value;
};

static cmdalias_t	*cmd_alias = NULL;

static CCommandBuffer s_CommandBuffer;
static CThreadFastMutex s_CommandBufferMutex;
#define LOCK_COMMAND_BUFFER() AUTO_LOCK(s_CommandBufferMutex)

static FileAssociationInfo g_FileAssociations[] =
{
	{ ".dem", "playdemo" },
	{ ".sav", "load" },
	{ ".bsp", "map" },
};


int g_iFilterCommandsByServerCanExecute = 0;		// If this is nonzero, then they want us to only run commands marked with FCVAR_SERVER_CAN_EXECUTE.
int g_iFilterCommandsByClientCmdCanExecute = 0;		// If this is nonzero, then they want us to only run commands marked with FCVAR_CLIENTCMD_CAN_EXECUTE.

// This is a list of cvars that are in the client DLL that we want FCVAR_CLIENTCMD_CAN_EXECUTE set on.
// In order to avoid patching the client DLL, we setup this list. Whenever the client DLL has gone out with the
// FCVAR_CLIENTCMD_CAN_EXECUTE flag set, we can get rid of this list.
CUtlDict<int,int> g_ExtraClientCmdCanExecuteCvars;

void Cmd_AddClientCmdCanExecuteVar( const char *pName )
{
	if ( g_ExtraClientCmdCanExecuteCvars.Find( pName ) == g_ExtraClientCmdCanExecuteCvars.InvalidIndex() )
		g_ExtraClientCmdCanExecuteCvars.Insert( pName );
}


//=============================================================================
// These functions manage a list of execution markers that we use to verify
// special commands in the command buffer.
//=============================================================================

static CUtlVector<int> g_ExecutionMarkers;

static int CreateExecutionMarker()
{
	if ( g_ExecutionMarkers.Count() > 2048 )
		g_ExecutionMarkers.Remove( 0 );

	int i = g_ExecutionMarkers.AddToTail( RandomInt( 0, 1<<30 ) );
	return g_ExecutionMarkers[i];
}

static bool FindAndRemoveExecutionMarker( int iCode )
{
	int i = g_ExecutionMarkers.Find( iCode );
	if ( i == g_ExecutionMarkers.InvalidIndex() )
		return false;
	
	g_ExecutionMarkers.Remove( i );
	return true;
}



//-----------------------------------------------------------------------------
// Used to allow cheats even if cheats aren't theoretically allowed
//-----------------------------------------------------------------------------
static bool g_bRPTActive = false;
void Cmd_SetRptActive( bool bActive )
{
	g_bRPTActive = bActive;
}

bool Cmd_IsRptActive()
{
	return g_bRPTActive;
}


//=============================================================================


//-----------------------------------------------------------------------------
// Just translates BindToggle <key> <cvar> into: bind <key> "increment var <cvar> 0 1 1"
//-----------------------------------------------------------------------------
CON_COMMAND( BindToggle, "Performs a bind <key> \"increment var <cvar> 0 1 1\"" )
{
	if( args.ArgC() <= 2 )
	{
		ConMsg( "BindToggle <key> <cvar>: invalid syntax specified\n" );
		return;
	}

	char newCmd[MAX_COMMAND_LENGTH];
	Q_snprintf( newCmd, sizeof(newCmd), "bind %s \"incrementvar %s 0 1 1\"\n", args[1], args[2] );

	Cbuf_InsertText( newCmd );
}


//-----------------------------------------------------------------------------
// Init, shutdown
//-----------------------------------------------------------------------------
void Cbuf_Init()
{
	// Wait for 1 execute time
	s_CommandBuffer.SetWaitDelayTime( 1 );
}

void Cbuf_Shutdown()
{
}


//-----------------------------------------------------------------------------
// Clears the command buffer
//-----------------------------------------------------------------------------
void Cbuf_Clear()
{
	Cbuf_Init();
}


//-----------------------------------------------------------------------------
// Adds command text at the end of the buffer
//-----------------------------------------------------------------------------
void Cbuf_AddText( const char *pText )
{
	LOCK_COMMAND_BUFFER();
	if ( !s_CommandBuffer.AddText( pText ) )
	{
		ConMsg( "Cbuf_AddText: buffer overflow\n" );
	}
}


//-----------------------------------------------------------------------------
// Adds command text at the beginning of the buffer
//-----------------------------------------------------------------------------
void Cbuf_InsertText( const char *pText )
{
	LOCK_COMMAND_BUFFER();
	// NOTE: This operation is only allowed when the command buffer
	// is in the middle of processing. If this assertion never triggers,
	// it's safe to eliminate Cbuf_InsertText altogether.
	// Otherwise, I have to add a feature to CCommandBuffer
	Assert( s_CommandBuffer.IsProcessingCommands() );
	Cbuf_AddText( pText );
}



void Cbuf_AddExecutionMarker( ECmdExecutionMarker marker )
{
	int iMarkerCode = CreateExecutionMarker();
	
	// CMDCHAR_ADD_EXECUTION_MARKER tells us there's a special execution thing here.
	// (char)marker tells it what to turn on
	// iRandomCode is for security, so only our code can stuff this command into the buffer.
	char str[512];
	Q_snprintf( str, sizeof( str ), ";%s %c %d;", CMDSTR_ADD_EXECUTION_MARKER, (char)marker, iMarkerCode );
	
	Cbuf_AddText( str );
}


//-----------------------------------------------------------------------------
// Executes commands in the buffer
//-----------------------------------------------------------------------------
static void Cbuf_ExecuteCommand( const CCommand &args, cmd_source_t source )
{
	// execute the command line
	const ConCommandBase *pCmd = Cmd_ExecuteCommand( args, source );

#if !defined(SWDS) && !defined(_XBOX)
	if ( pCmd && !pCmd->IsFlagSet( FCVAR_DONTRECORD ) )
	{
		demorecorder->RecordCommand( args.GetCommandString() );
	}
#endif
}


//-----------------------------------------------------------------------------
// Executes commands in the buffer
//-----------------------------------------------------------------------------
void Cbuf_Execute()
{
	VPROF("Cbuf_Execute");

	if ( !ThreadInMainThread() )
	{
		Warning( "Executing command outside main loop thread\n" );
		ExecuteOnce( DebuggerBreakIfDebugging() );
	}

	LOCK_COMMAND_BUFFER();

	// If text was added with Cbuf_AddText and then Cbuf_Execute gets called from within handler, we're going
	//  to execute the new commands anyway, so we can ignore this extra execute call here.
	if ( s_CommandBuffer.IsProcessingCommands() )
		return;

	// NOTE: The command buffer knows about execution time related to commands,
	// but since HL2 doesn't, we're going to spoof the command time to simply
	// be the the number of times Cbuf_Execute is called.
	s_CommandBuffer.BeginProcessingCommands( 1 );
	while ( s_CommandBuffer.DequeueNextCommand( ) )
	{
		Cbuf_ExecuteCommand( s_CommandBuffer.GetCommand(), src_command );
	}
	s_CommandBuffer.EndProcessingCommands( );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *param - 
// Output : static char const
//-----------------------------------------------------------------------------
static char const *Cmd_TranslateFileAssociation(char const *param )
{
	static char sz[ 512 ];
	char *retval = NULL;

	char temp[ 512 ];
	Q_strncpy( temp, param, sizeof( temp ) );
	Q_FixSlashes( temp );
	Q_strlower( temp );

	const char *extension = V_GetFileExtension(temp);
	// must have an extension to map
	if (!extension)
		return retval;

	int c = ARRAYSIZE( g_FileAssociations );
	for ( int i = 0; i < c; i++ )
	{
		FileAssociationInfo& info = g_FileAssociations[ i ];
		
		if ( ! Q_strcmp( extension, info.extension+1 ) && 
			 ! CommandLine()->FindParm(va( "+%s", info.command_to_issue ) ) )
		{
			// Translate if haven't already got one of these commands			
			Q_strncpy( sz, temp, sizeof( sz ) );
			Q_FileBase( sz, temp, sizeof( sz ) );

			Q_snprintf( sz, sizeof( sz ), "%s %s", info.command_to_issue, temp );
			retval = sz;
			break;
		}		
	}

	// return null if no translation, otherwise return commands
	return retval;
}

//-----------------------------------------------------------------------------
// Purpose: Adds command line parameters as script statements
// Commands lead with a +, and continue until a - or another +
// Also automatically converts .dem, .bsp, and .sav files to +playdemo etc command line options
// hl2 +cmd amlev1
// hl2 -nosound +cmd amlev1
// Output : void Cmd_StuffCmds_f
//-----------------------------------------------------------------------------
CON_COMMAND( stuffcmds, "Parses and stuffs command line + commands to command buffer." )
{		
	if ( args.ArgC() != 1 )
	{
		ConMsg( "stuffcmds : execute command line parameters\n" );
		return;
	}

	MEM_ALLOC_CREDIT();

	CUtlBuffer build( 0, 0, CUtlBuffer::TEXT_BUFFER );

	// arg[0] is the executable name
	for ( int i=1; i < CommandLine()->ParmCount(); i++ )
	{
		const char *szParm = CommandLine()->GetParm(i);
		if (!szParm) continue;

		if (szParm[0] == '-') 
		{
			// skip -XXX options and eat their args
			const char *szValue = CommandLine()->ParmValue(szParm);
			if ( szValue ) i++;
			continue;
		}
		if (szParm[0] == '+')
		{
			// convert +XXX options and stuff them into the build buffer
			const char *szValue = CommandLine()->ParmValue(szParm);
			if (szValue)
			{
				build.PutString(va("%s %s\n", szParm+1, szValue));
				i++;
			}
			else
			{
				build.PutString(szParm+1);
				build.PutChar('\n');
			}
		}
		else 
		{
			// singleton values, convert to command
			char const *translated = Cmd_TranslateFileAssociation( CommandLine()->GetParm( i ) );
			if (translated)
			{
				build.PutString(translated);
				build.PutChar('\n');
			}
		}
	}

	build.PutChar( '\0' );
		
	if ( build.TellPut() > 1 )
	{
		Cbuf_InsertText( (char *)build.Base() );
	}
}



bool IsValidFileExtension( const char *pszFilename )
{
	if ( !pszFilename )
	{
		return false;
	}

	if ( Q_strstr( pszFilename, ".exe" ) ||
		 Q_strstr( pszFilename, ".vbs" ) ||
		 Q_strstr( pszFilename, ".com" ) ||
		 Q_strstr( pszFilename, ".bat" ) ||
		 Q_strstr( pszFilename, ".dll" ) ||
		 Q_strstr( pszFilename, ".ini" ) ||
		 Q_strstr( pszFilename, ".gcf" ) ||
		 Q_strstr( pszFilename, ".sys" ) ||
		 Q_strstr( pszFilename, ".blob" ) )
	{
		return false;
	}

	return true;
}

/*
===============
Cmd_Exec_f
===============
*/

void Cmd_Exec_f( const CCommand &args )
{
	LOCK_COMMAND_BUFFER();
	char	*f;
	const char	*s;
	char	fileName[MAX_OSPATH];

	int argc = args.ArgC();
	if ( argc < 2 )
	{
		ConMsg( "exec <filename> [path id]: execute a script file\n" );
		return;
	}

	s = args[1];

	// Optional path ID. * means no path ID.
	const char *pPathID = NULL;
	if ( argc >= 3 )
	{
		pPathID = args[2];
	}
	else
	{
		pPathID = "*";
	}

	if ( !Q_stricmp( pPathID, "T" ) )
	{
		// Has an extension already?
		Q_snprintf( fileName, sizeof( fileName ), "T:/cfg/%s", s );
	}
	else if ( IsX360() && !Q_stricmp( pPathID, "x360" ) )
	{
		Q_snprintf( fileName, sizeof( fileName ), "cfg:/%s", s );
	}
	else
	{
		// Ensure it has an extension
		Q_snprintf( fileName, sizeof( fileName ), "//%s/cfg/%s", pPathID, s );
		Q_DefaultExtension( fileName, ".cfg", sizeof( fileName ) );
		
		// check path validity
		if ( !COM_IsValidPath( fileName ) )
		{
			ConMsg( "exec %s: invalid path.\n", fileName );
			return;
		}
	}

	// check for invalid file extensions
	if ( !IsValidFileExtension( fileName ) )
	{
		ConMsg( "exec %s: invalid file type.\n", fileName );
		return;
	}

	// 360 doesn't need to do costly existence checks
	if ( IsPC() && g_pFileSystem->FileExists( fileName ) )
	{
		// don't want to exec files larger than 1 MB
		// probably not a valid file to exec
		unsigned int size = g_pFileSystem->Size( fileName );
		if ( size > 1*1024*1024 )
		{
			ConMsg( "exec %s: file size larger than 1 MB!\n", s );
			return;
		}
	}

	char buf[16384];
	int len;
	f = (char *)COM_LoadStackFile( fileName, buf, sizeof( buf ), len );
	if ( !f )
	{
		if ( !V_stristr( s, "autoexec.cfg" ) && !V_stristr( s, "joystick.cfg" ) && !V_stristr( s, "game.cfg" ) )
		{
			ConMsg( "exec: couldn't exec %s\n", s );
		}
		return;
	}
	
	char *original_f = f;
	VCRHook_Cmd_Exec(&f);
	
	// In case f was allocated and VCR mode spoofed f, free the old one we allocated earlier.
	if ( original_f != buf && original_f != f )
	{
		free( original_f );
	}

	ConDMsg( "execing %s\n", s );
	
	// check to make sure we're not going to overflow the cmd_text buffer
	int hCommand = s_CommandBuffer.GetNextCommandHandle();

	// Execute each command immediately
	char *pszDataPtr = f;
	while( true )
	{
		// parse a line out of the source
		pszDataPtr = COM_ParseLine( pszDataPtr );

		// no more tokens
		if ( Q_strlen( com_token ) <= 0 )
			break;

		Cbuf_InsertText( com_token );

		// Execute all commands provoked by the current line read from the file
		while ( s_CommandBuffer.GetNextCommandHandle() != hCommand )
		{
			if( s_CommandBuffer.DequeueNextCommand( ) )
			{
				Cbuf_ExecuteCommand( s_CommandBuffer.GetCommand(), src_command );
			}
			else
			{
				Assert( 0 );
				break;
			}
		}
	}

	if ( f != buf )
	{
		// Hack for VCR playback. vcrmode allocates the memory but doesn't use the debug memory allocator,
		// so we don't want to free what it allocated.
		if ( f == original_f )
		{
			free( f );
		}
	}
}


/*
===============
Cmd_Echo_f

Just prints the rest of the line to the console
===============
*/
CON_COMMAND_F( echo, "Echo text to console.", FCVAR_SERVER_CAN_EXECUTE )
{
	int argc = args.ArgC();
	for ( int i=1; i<argc; i++ )
	{
		ConMsg ("%s ", args[i] );
	}
	ConMsg ("\n");
}

/*
===============
Cmd_Alias_f

Creates a new command that executes a command string (possibly ; seperated)
===============
*/
CON_COMMAND( alias, "Alias a command." )
{
	cmdalias_t	*a;
	char		cmd[MAX_COMMAND_LENGTH];
	int			i, c;
	const char	*s;

	int argc = args.ArgC();
	if ( argc == 1 )
	{
		ConMsg ("Current alias commands:\n");
		for (a = cmd_alias ; a ; a=a->next)
		{
			ConMsg ("%s : %s\n", a->name, a->value);
		}
		return;
	}

	s = args[1];
	if ( Q_strlen(s) >= MAX_ALIAS_NAME )
	{
		ConMsg ("Alias name is too long\n");
		return;
	}

// copy the rest of the command line
	cmd[0] = 0;		// start out with a null string
	c = argc;
	for (i=2 ; i< c ; i++)
	{
		Q_strncat(cmd, args[i], sizeof( cmd ), COPY_ALL_CHARACTERS);
		if (i != c)
		{
			Q_strncat (cmd, " ", sizeof( cmd ), COPY_ALL_CHARACTERS );
		}
	}
	Q_strncat (cmd, "\n", sizeof( cmd ), COPY_ALL_CHARACTERS);

	// if the alias already exists, reuse it
	for (a = cmd_alias ; a ; a=a->next)
	{
		if (!Q_strcmp(s, a->name))
		{
			if ( !Q_strcmp( a->value, cmd ) )		// Re-alias the same thing
				return;

			delete[] a->value;
			break;
		}
	}

	if (!a)
	{
		a = (cmdalias_t *)new cmdalias_t;
		a->next = cmd_alias;
		cmd_alias = a;
	}
	Q_strncpy (a->name, s, sizeof( a->name ) );	

	a->value = COM_StringCopy(cmd);
}

/*
=============================================================================

					COMMAND EXECUTION

=============================================================================
*/

cmd_source_t	cmd_source;
int				cmd_clientslot = -1;


//-----------------------------------------------------------------------------
// Purpose: 
// Output : void Cmd_Init
//-----------------------------------------------------------------------------
CON_COMMAND( cmd, "Forward command to server." )
{
	Cmd_ForwardToServer( args );
}

CON_COMMAND_AUTOCOMPLETEFILE( exec, Cmd_Exec_f, "Execute script file.", "cfg", cfg );




void Cmd_Init( void )
{
	Sys_CreateFileAssociations( ARRAYSIZE( g_FileAssociations ), g_FileAssociations );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void Cmd_Shutdown( void )
{
	// TODO, cleanup
	while ( cmd_alias )
	{
		cmdalias_t *next = cmd_alias->next;
		delete cmd_alias->value;	// created by StringCopy()
		delete cmd_alias;
		cmd_alias = next;
	}
}


//-----------------------------------------------------------------------------
// FIXME: Remove this! This is a temporary hack to deal with backward compat
//-----------------------------------------------------------------------------
void Cmd_Dispatch( const ConCommandBase *pCommand, const CCommand &command )
{
	ConCommand *pConCommand = const_cast<ConCommand*>( static_cast<const ConCommand*>( pCommand ) );
	pConCommand->Dispatch( command );
}


static void HandleExecutionMarker( const char *pCommand, const char *pMarkerCode )
{
	char cCommand = pCommand[0];
	int iMarkerCode = atoi( pMarkerCode );
	
	// Validate..
	if ( FindAndRemoveExecutionMarker( iMarkerCode ) )
	{
		// Ok, now it's validated, so do the command.
		if ( cCommand == eCmdExecutionMarker_Enable_FCVAR_SERVER_CAN_EXECUTE )
			++g_iFilterCommandsByServerCanExecute;
		else if ( cCommand == eCmdExecutionMarker_Disable_FCVAR_SERVER_CAN_EXECUTE )
			--g_iFilterCommandsByServerCanExecute;

		else if ( cCommand == eCmdExecutionMarker_Enable_FCVAR_CLIENTCMD_CAN_EXECUTE )
			++g_iFilterCommandsByClientCmdCanExecute;
		else if ( cCommand == eCmdExecutionMarker_Disable_FCVAR_CLIENTCMD_CAN_EXECUTE )
			--g_iFilterCommandsByClientCmdCanExecute;
	}
	else
	{
		static int cnt = 0;
		if ( ++cnt < 3 )
			Warning( "Invalid execution marker code.\n" );
	}
}

static bool ShouldPreventServerCommand( const ConCommandBase *pCommand )
{
	if ( !Host_IsSinglePlayerGame() )
	{
		if ( g_iFilterCommandsByServerCanExecute > 0 )
		{
			// If we don't understand the command, and it came from a server command, then forward it back to the server.
			// Lots of server plugins use this.  They use engine->ClientCommand() to have a client execute a command that 
			// they have hooked on the server. We disabled it once and they freaked. It IS redundant since they could just
			// code the call to their command on the server, but they complained loudly enough that we're adding it back in
			// since there's no exploit that we know of by allowing it.
			if ( !pCommand )
				return false;

			if ( !pCommand->IsFlagSet( FCVAR_SERVER_CAN_EXECUTE ) )
			{
				Warning( "FCVAR_SERVER_CAN_EXECUTE prevented server running command: %s\n", pCommand->GetName() );
				return true;
			}
		}
	}
	
	return false;
}

		
static bool ShouldPreventClientCommand( const ConCommandBase *pCommand )
{
	if ( g_iFilterCommandsByClientCmdCanExecute > 0 && 
		!pCommand->IsFlagSet( FCVAR_CLIENTCMD_CAN_EXECUTE ) && 
		g_ExtraClientCmdCanExecuteCvars.Find( pCommand->GetName() ) == g_ExtraClientCmdCanExecuteCvars.InvalidIndex() )
	{
		// If this command is in the game DLL, don't mention it because we're going to forward this
		// request to the server and let the server handle it.
		if ( !pCommand->IsFlagSet( FCVAR_GAMEDLL ) )
		{
			Warning( "FCVAR_CLIENTCMD_CAN_EXECUTE prevented running command: %s\n", pCommand->GetName() );
		}
		
		return true;
	}
	
	return false;
}


//-----------------------------------------------------------------------------
// A complete command line has been parsed, so try to execute it
// FIXME: lookupnoadd the token to speed search?
//-----------------------------------------------------------------------------
const ConCommandBase *Cmd_ExecuteCommand( const CCommand &command, cmd_source_t src, int nClientSlot )
{	
	// execute the command line
	if ( !command.ArgC() )
		return NULL;		// no tokens
			
	// First, check for execution markers.
	if ( Q_strcmp( command[0], CMDSTR_ADD_EXECUTION_MARKER ) == 0 )
	{
		if ( command.ArgC() == 3 )
		{
			HandleExecutionMarker( command[1], command[2] );
		}
		else
		{
			Warning( "WARNING: INVALID EXECUTION MARKER.\n" );
		}
		
		return NULL;
	}

	// check alias
	cmdalias_t *a;
	for ( a=cmd_alias; a; a=a->next )
	{
		if ( !Q_strcasecmp( command[0], a->name ) )
		{
			Cbuf_InsertText( a->value );
			return NULL;
		}
	}
	
	cmd_source = src;
	cmd_clientslot = nClientSlot;

	// check ConCommands
	const ConCommandBase *pCommand = g_pCVar->FindCommandBase( command[ 0 ] );

	// If we prevent a server command due to FCVAR_SERVER_CAN_EXECUTE not being set, then we get out immediately.
	if ( ShouldPreventServerCommand( pCommand ) )
		return NULL;

	if ( pCommand && pCommand->IsCommand() )
	{
		if ( !ShouldPreventClientCommand( pCommand ) && pCommand->IsCommand() )
		{
			bool isServerCommand = ( pCommand->IsFlagSet( FCVAR_GAMEDLL ) && 
									// Typed at console
									cmd_source == src_command &&
									// Not HLDS
									!sv.IsDedicated() );

			// Hook to allow game .dll to figure out who type the message on a listen server
			if ( serverGameClients )
			{
				// We're actually the server, so set it up locally
				if ( sv.IsActive() )
				{
					g_pServerPluginHandler->SetCommandClient( -1 );
							
#ifndef SWDS
					// Special processing for listen server player
					if ( isServerCommand )
					{
						g_pServerPluginHandler->SetCommandClient( cl.m_nPlayerSlot );
					}
#endif
				}
				// We're not the server, but we've been a listen server (game .dll loaded)
				//  forward this command tot he server instead of running it locally if we're still
				//  connected
				// Otherwise, things like "say" won't work unless you quit and restart
				else if ( isServerCommand )
				{
					if ( cl.IsConnected() )
					{
						Cmd_ForwardToServer( command );
						return NULL;
					}

					// It's a server command, but we're not connected to a server.  Don't try to execute it.
					return NULL;
				}
			}

			// Allow cheat commands in singleplayer, debug, or multiplayer with sv_cheats on
			if ( pCommand->IsFlagSet( FCVAR_CHEAT ) )
			{
				if ( !Host_IsSinglePlayerGame() && !CanCheat() )
				{
					// But.. if the server is allowed to run this command and the server DID run this command, then let it through.
					// (used by soundscape_flush)
					if ( g_iFilterCommandsByServerCanExecute == 0 || !pCommand->IsFlagSet( FCVAR_SERVER_CAN_EXECUTE ) )
					{
						Msg( "Can't use cheat command %s in multiplayer, unless the server has sv_cheats set to 1.\n", pCommand->GetName() );
						return NULL;
					}
				}
			}

			if ( pCommand->IsFlagSet( FCVAR_SPONLY ) )
			{
				if ( !Host_IsSinglePlayerGame() )
				{
					Msg( "Can't use command %s in multiplayer.\n", pCommand->GetName() );
					return NULL;
				}
			}

			Cmd_Dispatch( pCommand, command );
			return pCommand;
		}
	}

	// check cvars
	if ( cv->IsCommand( command ) )
		return pCommand;

	// forward the command line to the server, so the entity DLL can parse it
	if ( cmd_source == src_command )
	{
		if ( cl.IsConnected() )
		{
			Cmd_ForwardToServer( command );
			return NULL;
		}
	}
	
	Msg( "Unknown command \"%s\"\n", command[0] );
	return NULL;
}


//-----------------------------------------------------------------------------
// Sends the entire command line over to the server
//-----------------------------------------------------------------------------
void Cmd_ForwardToServer( const CCommand &args, bool bReliable )
{
	// YWB 6/3/98 Don't forward if this is a dedicated server
#ifndef SWDS
	char str[1024];

#ifndef _XBOX
	if ( demoplayer->IsPlayingBack() )
		return;		// not really connected
#endif

	str[0] = 0;
	if ( Q_strcasecmp( args[0], "cmd") != 0 )
	{
		Q_strncat( str, args[0], sizeof( str ), COPY_ALL_CHARACTERS );
		Q_strncat( str, " ", sizeof( str ), COPY_ALL_CHARACTERS );
	}
	
	if ( args.ArgC() > 1)
	{
		Q_strncat( str, args.ArgS(), sizeof( str ), COPY_ALL_CHARACTERS );
	}
	
	cl.SendStringCmd( str );
#endif
}
