//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: This file should go away eventually.
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#include "quakedef.h"
#include <ctype.h>
#include "host.h"
#include "cvar.h"
#include "server.h"
#include "sv_master_legacy.h"
#include "proto_oob.h"
#include "sv_main.h" // SV_GetFakeClientCount()
#include "tier0/icommandline.h"
#include "FindSteamServers.h"
#include "filesystem_engine.h"
#include "tier0/vcrmode.h"
#include "sv_steamauth.h"
#include "hltvserver.h"
#include "eiface.h"
#include "PlayerState.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


ConVar sv_master_legacy_mode( "sv_master_legacy_mode", "1", 0, "Use old (outside-of-Steam) code to communicate with master servers." );
extern ConVar sv_region;
extern ConVar sv_visiblemaxplayers;
extern ConVar sv_lan;
extern ConVar sv_tags;

extern char gpszVersionString[32];
extern char gpszProductString[32];
extern int g_iSteamAppID;

#define MASTER_SERVER_PROTOCOL_VERSION		7


// These can be owned by Steam after we get rid of this legacy code.
#define S2A_INFO_SRC			'I'	// + Address, hostname, map, gamedir, gamedescription, active players, maxplayers, protocol
#define	S2M_HEARTBEAT			'a'	// + challeange + sequence + active + #channels + channels
#define S2M_HEARTBEAT2			'0' // New style heartbeat
#define	S2M_SHUTDOWN			'b' // no params
#define M2A_CHALLENGE			's'	// + challenge value 
#define M2S_REQUESTRESTART		'O' // HLMaster rejected a server's connection because the server needs to be updated
#define A2S_RULES				'V'	// request rules list from server
#define S2A_RULES				'E' // + number of rules + string key and string value pairs
#define A2S_INFO				'T' // server info request - this must match the Goldsrc engine
#define S2A_PLAYER				'D' // + Playernum, name, frags, /*deaths*/, time on server
#define A2S_PLAYER				'U'	// request player list


bool IsUsingMasterLegacyMode()
{
#ifndef NO_STEAM
	return sv_master_legacy_mode.GetBool() || !SteamMasterServerUpdater();
#else
	return true;
#endif
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// THIS 'INFO' STUFF CAME FROM INFO.CPP WHICH NO LONGER NEEDS TO EXIST IN THE ENGINE.
// IT IS HERE UNTIL WE GET RID OF THIS LEGACY CODE.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define MAX_KV_LEN 127

void Info_RemoveKey ( char *s, const char *key )
{
	char	*start;
	char	pkey[512];
	char	value[512];
	char	*o;

	if (Q_strstr (key, "\\"))
	{
		Msg ("Can't use a key with a \\\n");
		return;
	}

	while (1)
	{
		start = s;
		if (*s == '\\')
			s++;
		o = pkey;
		while (*s != '\\')
		{
			if (!*s)
				return;
			*o++ = *s++;
		}
		*o = 0;
		s++;

		o = value;
		while (*s != '\\' && *s)
		{
			if (!*s)
				return;
			*o++ = *s++;
		}
		*o = 0;

		if (!Q_strcmp (key, pkey) )
		{
			//Q_strcpy (start, s);	// remove this part
			Q_memmove( start, s, strlen( s )+1 );
			return;
		}

		if (!*s)
			return;
	}

}

bool Info_IsKeyImportant( const char *key )
{
	if ( key[0] == '*' )
		return true;
	if ( !Q_strcmp( key, "name" ) )
		return true;
	if ( !Q_strcmp( key, "model" ) )
		return true;
	if ( !Q_strcmp( key, "rate" ) )
		return true;
	if ( !Q_strcmp( key, "cl_updaterate" ) )
		return true;
	if ( !Q_strcmp( key, "cl_lw" ) )
		return true;
	if ( !Q_strcmp( key, "cl_lc" ) )
		return true;
	if ( !Q_strcmp( key, "tracker" ) )
		return true;

	return false;
}

char *Info_FindLargestKey( char *s, int maxsize )
{
	char	key[256];
	char	value[256];
	char	*o;
	int		l;
	static char largest_key[256];
	int     largest_size = 0;

	*largest_key = 0;

	if (*s == '\\')
		s++;
	while (*s)
	{
		int size = 0;

		o = key;
		while (*s && *s != '\\')
			*o++ = *s++;

		l = o - key;
		*o = 0;
		size = Q_strlen( key );

		if (!*s)
		{
			return largest_key;
		}

		o = value;
		s++;
		while (*s && *s != '\\')
			*o++ = *s++;
		*o = 0;

		if (*s)
			s++;

		size += Q_strlen( value );

		if ( (size > largest_size) && !Info_IsKeyImportant(key) )
		{
			largest_size = size;
			Q_strncpy( largest_key, key, sizeof( largest_key ) );
		}
	}

	return largest_key;
}

void Info_SetValueForStarKey ( char *s, const char *key, const char *value, int maxsize )
{
	char	newArray[1024], *v;
	int		c;

	if (Q_strstr (key, "\\") || Q_strstr (value, "\\") )
	{
		Msg ("Can't use keys or values with a \\\n");
		return;
	}

	if (Q_strstr (key, "\"") || Q_strstr (value, "\"") )
	{
		Msg ("Can't use keys or values with a \"\n");
		return;
	}

	if (Q_strlen(key) > MAX_KV_LEN || Q_strlen(value) > MAX_KV_LEN)
	{
		Msg ("Keys and values must be < %i characters.\n", MAX_KV_LEN + 1 );
		return;
	}
	Info_RemoveKey (s, key);
	if (!value || !Q_strlen(value))
		return;

	Q_snprintf (newArray, sizeof( newArray ), "\\%s\\%s", key, value);

 	if ( (int)(Q_strlen(newArray) + Q_strlen(s)) >= maxsize)
	{
		// no more room in buffer to add key/value
		if ( Info_IsKeyImportant( key ) )
		{
			// keep removing the largest key/values until we have room
			char *largekey;
			do {
				largekey = Info_FindLargestKey( s, maxsize );
				Info_RemoveKey( s, largekey );
			} while ( ((int)(Q_strlen(newArray) + Q_strlen(s)) >= maxsize) && *largekey != 0 );

			if ( largekey[0] == 0 )
			{
				// no room to add setting
				Msg ("Info string length exceeded\n");
				return;
			}
		}
		else
		{
			// no room to add setting
			Msg ("Info string length exceeded\n");
			return;
		}
	}

	// only copy ascii values
	s += Q_strlen(s);
	v = newArray;
	while (*v)
	{
		c = (unsigned char)*v++;
		// client only allows highbits on name
		if (stricmp(key, "name") != 0) {
			c &= 127;
			if (c < 32 || c > 127)
				continue;
			// auto lowercase team
			if (stricmp(key, "team") == 0)
				c = tolower(c);
		}
		if (c > 13)
			*s++ = c;
	}
	*s = 0;
}

void Info_SetValueForKey (char *s, const char *key, const char *value, int maxsize)
{
	if (key[0] == '*')
	{
		Msg ("Can't set * keys\n");
		return;
	}

	Info_SetValueForStarKey (s, key, value, maxsize);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// END INFO STUFF
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
// Purpose: List of master servers and some state info about them
//-----------------------------------------------------------------------------
class CHeartbeat
{
public:
	// Challenge request send time, <0 if no challenge sent
	float				challengetime; 
	// Last one is Main master
	int					challangenr;
	// Time we sent last heartbeat
	float				last_heartbeat;      
	// base server instance
	CBaseServer			*pServer;
};

class CMasterserver
{
public:
	// master server IP address
	netadr_t				addr;
	// heartbeats sent to this master server
	CUtlVector<CHeartbeat>	heartbeats;
};

//-----------------------------------------------------------------------------
// Purpose: Implements the master server interface
//-----------------------------------------------------------------------------
class CMaster : public IMaster
{
public:
	CMaster( void );
	virtual ~CMaster( void );

	// Heartbeat functions.
	void Init();
	void Shutdown( void );
	// Sets up master address
	void InitConnection(void);		  
	void ShutdownConnection(CBaseServer *pServer);
	bool AddServer( struct netadr_s *adr );
	bool RemoveServer( struct netadr_s *adr );
	void UseDefault ( void );
	void CheckHeartbeat (CBaseServer *pServer);
	void SendHeartbeat(netadr_t &from, int challangenr, CBaseServer *pServer);

	void SetMaster_Legacy_f( const CCommand &args );
	void Heartbeat_Legacy_f( void );

	CHeartbeat* GetHeartbeat( CMasterserver *master, CBaseServer *pServer );
	bool IsMasterServerAddress( netadr_t *from );
	bool IsCorrectMasterChallenge( netadr_t *from, int challenge, CBaseServer *pServer );
	bool RestartOnLevelChange() { return m_bRestartOnLevelChange; }
	void SetRestartOnLevelChange(bool state)  { m_bRestartOnLevelChange = state; }
	virtual void HandleUnknown( struct netpacket_s *packet, CBaseServer *pServer, CGameServer *pGameServer );
	
	void ReplyInfo( netadr_t &adr, CBaseServer *pServer );
	void ReplyRules( netadr_t &from, CBaseServer *pServer );
	void ReplyPlayers( netadr_t &adr, CGameServer *pServer );
	

private:
	// List of known master servers
	CUtlVector<CMasterserver>	m_MasterServers;
		
	// If nomaster is true, the server will not send heartbeats to the master server
	bool		m_bNoMasters;
	int			m_iHeartBeatTimeout;
	bool		m_bRestartOnLevelChange;
	void		*m_hSteamDLL;

};

static CMaster s_MasterServer;
IMaster *master = (IMaster *)&s_MasterServer;

#define	HEARTBEAT_SECONDS	300 // default time between sending heartbeats to the masters
#define HB_TIMEOUT 15 // time to wait for a challenge response in seconds

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMaster::CMaster( void )
{
	m_bNoMasters		= false; 
	m_iHeartBeatTimeout = HEARTBEAT_SECONDS;
	m_bRestartOnLevelChange = false;
}

CMaster::~CMaster( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: Sends a heartbeat to the master server
// Input  : *p - 
//-----------------------------------------------------------------------------
#define MAX_SINFO 2048

void CMaster::SendHeartbeat ( netadr_t &adr, int challangenr, CBaseServer *pServer )
{
	if ( !IsUsingMasterLegacyMode() )
		return;
	
	static char	message[MAX_SINFO+5];    // Buffer for sending heartbeat
	int			players;          // Number of active client connections
	char        szGD[ MAX_OSPATH ];
	char		info[ MAX_SINFO ];
	char		szOS[2];

	CMasterserver *pMasterServer = NULL;

	// find server
	for ( int i=0; i<m_MasterServers.Count(); i++ )
	{
		if ( adr.CompareAdr( m_MasterServers[i].addr ) )
		{
			pMasterServer = &m_MasterServers[i];
			break;
		}
	}

	if ( !pMasterServer )
		return;

	CHeartbeat *pHeartbeat = GetHeartbeat( pMasterServer, pServer );

	if ( !pHeartbeat )
		return;

	if ( !pServer->IsMultiplayer() || (serverGameDLL && serverGameDLL->ShouldHideServer()) )
		return;

	// check if time between challenge request and master response was too long 
	if ( (net_time - pHeartbeat->challengetime ) >= HB_TIMEOUT )
		return;

	pHeartbeat->challengetime = 0; // reset challenge time
	pHeartbeat->challangenr = challangenr; // remember last challenge number

	InitConnection();

	//
	// count active users
	//
	players = pServer->GetNumClients();

	Q_memset(message, 0, sizeof(message));
	
	Q_FileBase( com_gamedir, szGD, sizeof( szGD ) );
	
	bool bHasPW = pServer->GetPassword() != NULL;
		
#if _WIN32
	Q_strcpy( szOS, "w" );
#else
	Q_strcpy( szOS, "l" );
#endif

	info[0]='\0';

	Info_SetValueForKey( info, "protocol", va( "%i", MASTER_SERVER_PROTOCOL_VERSION ), MAX_SINFO );
	Info_SetValueForKey( info, "challenge", va( "%i", challangenr ), MAX_SINFO );
	Info_SetValueForKey( info, "players", va( "%i", players ), MAX_SINFO );
	Info_SetValueForKey( info, "max", va( "%i", pServer->GetMaxClients() ), MAX_SINFO );
	Info_SetValueForKey( info, "bots", va( "%i", pServer->GetNumFakeClients() ), MAX_SINFO );
	Info_SetValueForKey( info, "gamedir", szGD, MAX_SINFO );
	Info_SetValueForKey( info, "map", pServer->GetMapName(), MAX_SINFO );
	Info_SetValueForKey( info, "password", bHasPW?"1":"0", MAX_SINFO );
	Info_SetValueForKey( info, "os", szOS , MAX_SINFO );
	Info_SetValueForKey( info, "lan", Steam3Server().BLanOnly() ?"1":"0", MAX_SINFO ); 
	Info_SetValueForKey( info, "region", sv_region.GetString(), MAX_SINFO );
	Info_SetValueForKey( info, "gametype", sv_tags.GetString(), MAX_SINFO );
	
	if ( pServer->IsHLTV() )
	{
		Info_SetValueForKey( info, "type", "p"  , MAX_SINFO ); // p = HLTV proxy
		Info_SetValueForKey( info, "secure", "0", MAX_SINFO ); // never requires a secure client
		Info_SetValueForKey( info, "version", "1.0.0.0", MAX_SINFO );
		Info_SetValueForKey( info, "product", "srctv", MAX_SINFO );

		// Info_SetValueForKey( info, "proxy", hltv->IsMasterProxy()?"1":"2", MAX_SINFO );
	}
	else // game server
	{
		Info_SetValueForKey( info, "type", pServer->IsDedicated() ? "d" : "l"  , MAX_SINFO );
		Info_SetValueForKey( info, "secure", Steam3Server().BSecure() ? "1" : "0", MAX_SINFO );
		Info_SetValueForKey( info, "version", gpszVersionString, MAX_SINFO );
		Info_SetValueForKey( info, "product", gpszProductString, MAX_SINFO );
	}
		

	// Send to master
	Q_snprintf (message, sizeof(message), "%c\n%s\n", S2M_HEARTBEAT2, info );

	NET_SendPacket ( NULL, pServer->m_Socket, adr, (unsigned char *)message, Q_strlen(message) );
}

CHeartbeat* CMaster::GetHeartbeat( CMasterserver *pMasterServer, CBaseServer *pServer )
{
	for ( int i=0; i<pMasterServer->heartbeats.Count(); i++ )
	{
		if ( pMasterServer->heartbeats[i].pServer == pServer )
			return &pMasterServer->heartbeats[i];
	}

	// not found, add one
	int index = pMasterServer->heartbeats.AddToTail();

	CHeartbeat *pHeartBeat = &pMasterServer->heartbeats[index];

	pHeartBeat->pServer = pServer;
	pHeartBeat->challangenr = -1;
	pHeartBeat->last_heartbeat = -9999;
	pHeartBeat->challengetime = 0;

	return pHeartBeat;
}

//-----------------------------------------------------------------------------
// Purpose: Requests a challenge so we can then send a heartbeat
//-----------------------------------------------------------------------------
void CMaster::CheckHeartbeat (CBaseServer *pServer)
{
	if ( !IsUsingMasterLegacyMode() )
		return;
	
	unsigned char c = A2S_GETCHALLENGE;

	if ( m_bNoMasters ||      // We are ignoring heartbeats
		!pServer->IsActive() ||
		!pServer->IsMultiplayer() ||
		(serverGameDLL && serverGameDLL->ShouldHideServer()) )
	{
		return;
	}

	InitConnection();


	for ( int i=0; i<m_MasterServers.Count(); i++ )
	{
		CHeartbeat *pHeartbeat = GetHeartbeat( &m_MasterServers[i], pServer );

		if ( !pHeartbeat )
			continue;

		// Time for another try?
		if ( ( net_time - pHeartbeat->last_heartbeat) < m_iHeartBeatTimeout)  // not time to send yet
			continue;
		
		// Should we resend challenge request?
		if ( pHeartbeat->challengetime > 0 && ( net_time - pHeartbeat->challengetime ) < HB_TIMEOUT )
			continue;
		
		pHeartbeat->challengetime		 = net_time; 
		pHeartbeat->last_heartbeat       = net_time;  // Flag at start so we don't just keep trying for hb's when

		// Send to master asking for a challenge #
		NET_SendPacket ( NULL, pServer->m_Socket, m_MasterServers[i].addr, &c, 1 );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Server is shutting down, unload master servers list, tell masters that we are closing the server
//-----------------------------------------------------------------------------
void CMaster::ShutdownConnection( CBaseServer *pServer )
{
	if ( !host_initialized )
		return;

	if ( m_bNoMasters ||      // We are ignoring heartbeats
		!pServer->IsMultiplayer() ||   // not a multiplayer server.
		(serverGameDLL && serverGameDLL->ShouldHideServer()) )   // server doesn't want to be seen
	{
		return;
	}

	InitConnection();

	char	message[4];
	Q_snprintf (message, sizeof(message), "%c\n", S2M_SHUTDOWN);
	
	for (int i=0; i<m_MasterServers.Count(); i++ )
	{
		// send goodbye message
		NET_SendPacket( NULL, pServer->m_Socket, m_MasterServers[i].addr, (unsigned char*)message, Q_strlen(message) + 1  );

		// remove server instance from list
		for ( int j=0; j<m_MasterServers[i].heartbeats.Count(); j++ )
		{
			if ( m_MasterServers[i].heartbeats[j].pServer == pServer )
			{
				m_MasterServers[i].heartbeats.Remove( j );
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Add server to the master list
// Input  : *adr - 
//-----------------------------------------------------------------------------
bool CMaster::AddServer( netadr_t *adr )
{
	if ( IsMasterServerAddress( adr ) )
		return false;  // already in list

	int index = m_MasterServers.AddToTail();

	m_MasterServers[index].addr = *adr;

	return true;
}

bool CMaster::RemoveServer(struct netadr_s *adr)
{
	for ( int i=0; i<m_MasterServers.Count(); i++ )
	{
		if ( m_MasterServers[i].addr.CompareAdr( *adr ) )
		{
			m_MasterServers.Remove(i);
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Add built-in default master if steam doesn't parse
//-----------------------------------------------------------------------------
void CMaster::UseDefault ( void )
{
	// don't bother with a default, if you can't find a HL2MS give up
}

#if defined( _WIN32 ) && !defined( _X360 )
// Instead of including windows.h
extern "C"
{
	extern void * __stdcall GetProcAddress( void *hModule, const char *pszProcName );
};
#endif


//-----------------------------------------------------------------------------
// Purpose: Initializes the default master server address
//-----------------------------------------------------------------------------
void CMaster::InitConnection( void )
{
	if ( !IsUsingMasterLegacyMode() )
		return;
	
	static bool bInitialized = false;

	if ( m_bNoMasters )
	{
		return;
	}

	// Already able to initialize at least once?
	if ( bInitialized )
		return;

	if ( CommandLine()->FindParm( "-nomaster" ) )
	{
		ConMsg( "Master server communication disabled.\n" );
		m_bNoMasters = true;
		bInitialized = true;
		return;
	}

#if !defined( NO_STEAM )

	if ( !m_hSteamDLL )
	{
#ifdef _WIN32
		m_hSteamDLL = Sys_LoadModule( "steam.dll" );
#elif _LINUX
		m_hSteamDLL = Sys_LoadModule( "libsteamvalidateuseridtickets_i486.so" );
#else
		#error "Define the steam.dll I should load!"
#endif

	}

	typedef int (*PFSteamFindServersNumServers)( ESteamServerType eServerType);
	typedef int (*PFSteamFindServersIterateServer)( ESteamServerType eServerType, unsigned int nServer, char *szIpAddrPort, int szIpAddrPortLen);
	PFSteamFindServersNumServers pfnSteamFindServersNumServers = NULL;
	PFSteamFindServersIterateServer pfnSteamFindServersIterateServer = NULL;

	if ( m_hSteamDLL )
	{
#ifdef _LINUX
#define GetProcAddress dlsym
#endif
		pfnSteamFindServersNumServers = (PFSteamFindServersNumServers) GetProcAddress( m_hSteamDLL, "SteamFindServersNumServers" );
		pfnSteamFindServersIterateServer = (PFSteamFindServersIterateServer)GetProcAddress( m_hSteamDLL, "SteamFindServersIterateServer" );
	}

	if ( !m_hSteamDLL || !pfnSteamFindServersNumServers || !pfnSteamFindServersIterateServer )
	{
		ConMsg( "Unable to load Steam library.\n" );
		m_bNoMasters = true;
		bInitialized = true;
		return;
	}

	unsigned int numServers = 0;
	if ( VCRGetMode() != VCR_Playback )
	{
		numServers = pfnSteamFindServersNumServers( eSteamHalfLife2MasterServer );
	}
#if !defined( NO_VCR )
	VCRGenericValue( "a", &numServers, sizeof( numServers ) );
#endif
	int nCount = 0; // number of servers added
	if ( numServers == eSteamFindSteamServersLibraryError )
	{
		//const char * err = SteamFindServersGetErrorString();
		//Assert( err );
		return;
	}
	else if ( numServers == eSteamFindSteamServersLibraryBusy )
	{
		return; 
	}
	else
	{
		bInitialized = true; // needed here to stop recursion in AddServer() below

		numServers = min( numServers, 2 ); // only heartbeat to this many servers
										   // the servers have their own TCP/IP peer to peer network
										   // so you only need to heartbeat to one to show up on all of them.
									   	   // Heartbeat to 2 to take into account possible packet loss.
										   // heartbeating to more causes more useless traffic into the servers 
										   // AND on the peer to peer network!!!!
						
											// You only need to pick the first 2 because the SteamFindServers
											// library randomises the order of returned IP addrs.


		for( unsigned int i = 0; i < numServers; i++ )
		{
			char szAdr[32];
			netadr_t adr;

			if ( VCRGetMode() != VCR_Playback )
			{
				pfnSteamFindServersIterateServer( eSteamHalfLife2MasterServer, i, szAdr, sizeof(szAdr) );
			}
#if !defined( NO_VCR )
			VCRGenericValue( "a", szAdr, sizeof( szAdr ) );
#endif
			// Can we resolve it any better
			if ( !NET_StringToAdr( szAdr, &adr ) )
				continue;

			ConMsg( "Adding master server %s\n", adr.ToString() );
			AddServer( &adr );
			nCount++;
		}
	}

	if ( NET_IsDedicated() && sv_region.GetInt() == -1 )
    {
		sv_region.SetValue( 255 ); // HACK!HACK! undo me once we want to enforce regions

        //Log_Printf( "You must set sv_region in your server.cfg or use +sv_region on the command line\n" );
		//ConMsg( "You must set sv_region in your server.cfg or use +sv_region on the command line\n" );
        //Cmd_ExecuteString( "quit\n", src_command);
        //return;
    }

	// So we don't do this a second time.
	bInitialized = true;

	if ( !nCount )
	{
		ConMsg( "No masters loaded\nUsing default master\n" );
		UseDefault();
	}
#endif // NO_STEAM
}

//-----------------------------------------------------------------------------
// Purpose: Add/remove master servers
//-----------------------------------------------------------------------------
void CMaster::SetMaster_Legacy_f( const CCommand &args )
{
	char   szMasterAddress[128]; // IP:Port string of master
	const char    *pszCmd = NULL;
	netadr_t adr;
	
	int count = args.ArgC();

	InitConnection();

	// Usage help
	if ( count < 2 )
	{
		ConMsg("Usage:\nsetmaster <add | remove | enable | disable> <IP:port>\n");

		
		if ( m_MasterServers.Count() == 0 )
		{
			ConMsg("Current:  None\n");
		}
		else
		{
			ConMsg("Current:\n");

			for ( int i=0; i<m_MasterServers.Count();i++)
			{
				ConMsg("  %i:  %s\n", i+1, m_MasterServers[i].addr.ToString() );
			}
		}
		return;
	}

	pszCmd = args[1];

	if ( !pszCmd || !pszCmd[0] )
		return;

	// build master address
	szMasterAddress[0] = 0;
	adr.Clear();

	for ( int i= 2; i<count; i++ )
	{
		Q_strcat( szMasterAddress, args[i], sizeof( szMasterAddress ) );
	}

	if ( Q_strlen( szMasterAddress ) > 0 )
	{
		// Convert to a valid address
		if ( !NET_StringToAdr ( szMasterAddress, &adr ) )
		{
			ConMsg(" Invalid address \"%s\", setmaster command ignored\n", szMasterAddress );
			return;
		}

		// set to default port if no port was given
		if ( adr.GetPort() == 0 )
		{
			adr.SetPort( PORT_MASTER );
		}
	}

	
	// Check for disabling...
	if ( !Q_stricmp( pszCmd, "disable") )
	{
		m_bNoMasters = true;
	}
	else if (!Q_stricmp( pszCmd, "enable") )
	{
		m_bNoMasters = false;
	}
	else if ( !Q_stricmp( pszCmd, "add" ) )
	{
		if ( AddServer( &adr ) )
		{
			ConMsg ( "Adding master at %s\n", adr.ToString() );
		}
		else
		{
			ConMsg ( "Master at %s already in list\n", adr.ToString() );
		}

		// If we get here, masters are definitely being used.
		m_bNoMasters = false;
	}
	else if ( !Q_stricmp( pszCmd, "remove" ) )
	{
		// Find master server
		if ( !RemoveServer( &adr ) )
		{
			ConMsg( "Can't remove master %s, not in list\n", adr.ToString() );
		}
	}
	else
	{
		ConMsg( "Invalid setmaster command\n" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Send a new heartbeat to the master
//-----------------------------------------------------------------------------
void CMaster::Heartbeat_Legacy_f ()
{
	for ( int i=0; i<m_MasterServers.Count(); i++ )
	{
		for (int j=0; j<m_MasterServers[i].heartbeats.Count(); j++ )
		{
			// force heartbeats for all known servers
			m_MasterServers[i].heartbeats[j].last_heartbeat = -9999;
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: Adds master server console commands
//-----------------------------------------------------------------------------
void CMaster::Init()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CMaster::Shutdown(void)
{
	m_MasterServers.Purge();
}

bool CMaster::IsMasterServerAddress( netadr_t *from )
{
	for (int i=0; i<m_MasterServers.Count(); i++)
	{
		if ( from->CompareAdr( m_MasterServers[i].addr ) )
		{
			return true;
		}
	}

	return false;
}

bool CMaster::IsCorrectMasterChallenge( netadr_t *from, int challenge, CBaseServer *pServer )
{
	// check it came from a master server we heartbeat too and its the same heartbeat challenge we use

	for ( int i=0; i<m_MasterServers.Count(); i++ )
	{
		if ( !m_MasterServers[i].addr.CompareAdr( *from ) )
			continue;

		for ( int j=0; j<m_MasterServers[i].heartbeats.Count(); j++ )
		{
			CHeartbeat *pHeartbeat = &m_MasterServers[i].heartbeats[j];

			if ( pHeartbeat->challangenr == challenge &&
				 pHeartbeat->pServer == pServer )
			{
				return true;
			}
		}
	}

	return false; // not found
}

void CMaster::HandleUnknown( netpacket_t *packet, CBaseServer *pServer, CGameServer *pGameServer )
{
	bf_read msg = packet->message;	// handy shortcut 
	char c = msg.ReadChar();

	switch ( c )
	{
		case M2A_CHALLENGE : 
#ifndef _XBOX
							if ( IsMasterServerAddress( &packet->from ) )
							{
								msg.ReadChar(); // nibble off the \n
								SendHeartbeat ( packet->from, msg.ReadLong(), pServer );
							}
#else
							Assert( 0 );
#endif
							break;

		case M2S_REQUESTRESTART :
#ifndef _XBOX
							if ( IsMasterServerAddress( &packet->from ) )
							{
								int challenge = msg.ReadLong();
								if ( !msg.IsOverflowed() && IsCorrectMasterChallenge( &packet->from, challenge, pServer ) )
								{
									// Connection was rejected by the HLMaster (out of date version)

									// hack, vgui console looks for this string; 
									Msg("%cMasterRequestRestart\n", 3);

	#ifndef _WIN32
									if (CommandLine()->FindParm(AUTO_RESTART))
									{
										Msg("Your server will be restarted on map change.\n");
										Log("Your server will be restarted on map change.\n");
										SetRestartOnLevelChange( true );
									}
	#endif
	#ifdef _WIN32
									if (g_pFileSystem->IsSteam())
	#else
									else if ( 1 ) // under linux assume steam
	#endif
									{
										Msg("Your server needs to be restarted in order to receive the latest update.\n");
										Log("Your server needs to be restarted in order to receive the latest update.\n");
									}
									else
									{
										Msg("Your server is out of date.  Please update and restart.\n");
									}
								}
							}
#else
							Assert( 0 );
#endif
							break;		
		case A2S_INFO :		{
							char nugget[64];
							nugget[0]=0;
							if ( msg.GetNumBytesLeft() >= Q_strlen(A2S_KEY_STRING) )
							{
								msg.ReadString( nugget, sizeof(nugget) - 2 );
								nugget[ sizeof(nugget)-1 ] = 0;
							}
					
							if ( pServer->ValidInfoChallenge( packet->from, nugget ) )
							{
								ReplyInfo( packet->from, pServer );
							}
							break;
							}

		case A2S_PLAYER :	{
							int challengeNr = -1;
							if ( msg.GetNumBytesLeft() >= 4 )
							{
								challengeNr = msg.ReadLong();
							}

							if ( pServer->ValidChallenge( packet->from, challengeNr ) )
							{
								if ( pGameServer )
									ReplyPlayers( packet->from, pGameServer );
							}
							break;
							}

		case A2S_RULES	:	{
							int challengeNr = -1;
							if ( msg.GetNumBytesLeft() >= 4 )
							{
								challengeNr = msg.ReadLong();
							}

							if ( pServer->ValidChallenge( packet->from, challengeNr ) )
							{
								ReplyRules( packet->from, pServer );
							}
							break;
							}
	}
}


/*
================
SVC_Info

Responds with short or long info for broadcast scans
================
*/

void CMaster::ReplyInfo( netadr_t &adr, CBaseServer *pServer )
{
	if ( serverGameDLL && serverGameDLL->ShouldHideServer() )
		return;

	byte	data[1400];
	char	gd[MAX_OSPATH];

	bf_write buf( "SVC_Info->buf", data, sizeof(data) );

	buf.WriteLong( CONNECTIONLESS_HEADER );

	buf.WriteByte( S2A_INFO_SRC );

	buf.WriteByte( PROTOCOL_VERSION );

	buf.WriteString( pServer->GetName() );
	buf.WriteString( pServer->GetMapName() );
	Q_FileBase( com_gamedir, gd, sizeof( gd ) );
	buf.WriteString( gd );
	buf.WriteString( (char *)serverGameDLL->GetGameDescription() );

	uint appID = g_iSteamAppID;
	buf.WriteShort( appID );

	// this is a quick workaround from goldsrc for the admin mod reserved slots UI problem
	int visibleClients = pServer->GetMaxClients();
	if ( !pServer->IsHLTV() && sv_visiblemaxplayers.GetInt() > 0 && sv_visiblemaxplayers.GetInt() < pServer->GetMaxClients() )
	{
		visibleClients = sv_visiblemaxplayers.GetInt();
	}

	// player info
	buf.WriteByte( pServer->GetNumClients() );
	buf.WriteByte( visibleClients );
	buf.WriteByte( pServer->GetNumFakeClients() );

	// Additional info....
	if ( pServer->IsHLTV() )
		buf.WriteByte( 'p' );	// p = SourceTV proxy
	else if ( pServer->IsDedicated() )
		buf.WriteByte( 'd' );	// d = dedicated server
	else
		buf.WriteByte( 'l' );	// l = listen server

#if defined(_WIN32)
	buf.WriteByte( 'w' );
#else // LINUX?
	buf.WriteByte( 'l' );
#endif

	// Password?
	buf.WriteByte( pServer->GetPassword() ? 1 : 0 );

#ifndef _XBOX
	// VAC state, needs to be hooked up
	bool bIsSecure = Steam3Server().BSecure() && !pServer->IsHLTV();
#else
	bool bIsSecure = true;
#endif
	buf.WriteByte( bIsSecure?1:0 );

	char verString[40];
	Q_snprintf( verString, sizeof(verString), "%s", gpszVersionString );
	buf.WriteString( verString );

	// temp hack until we kill the legacy interface
#define S2A_EXTRA_DATA_HAS_GAME_PORT				0x80		// Next 2 bytes include the game port.
#define S2A_EXTRA_DATA_HAS_SPECTATOR_DATA			0x40		// Next 2 bytes include the spectator port, then the spectator server name.
#define S2A_EXTRA_DATA_HAS_GAMETAG_DATA				0x20		// Next bytes are the game tag string
	
	// Write a byte with some flags that describe what is to follow.
	byte nNewFlags = 0;
	
	if ( !hltv || ( hltv && ( !hltv->IsActive() || !hltv->IsTVRelay() ) ) )
		nNewFlags |= S2A_EXTRA_DATA_HAS_GAME_PORT;

	if ( hltv && hltv->IsActive() )
		nNewFlags |= S2A_EXTRA_DATA_HAS_SPECTATOR_DATA;

	const char *pchGameType = sv_tags.GetString();
	if ( pchGameType && Q_strlen(pchGameType) > 0 )
		nNewFlags |= S2A_EXTRA_DATA_HAS_GAMETAG_DATA;

	buf.WriteByte( nNewFlags );

	// Write the rest of the data.
	if ( nNewFlags & S2A_EXTRA_DATA_HAS_GAME_PORT )
	{
		buf.WriteShort( NET_GetUDPPort( NS_SERVER ) );
	}

	if ( nNewFlags & S2A_EXTRA_DATA_HAS_SPECTATOR_DATA )
	{
		buf.WriteShort( NET_GetUDPPort( NS_HLTV ) );
		if ( hltv )
			buf.WriteString( hltv->GetName() );
		else
			buf.WriteString( pServer->GetName() );		
	}

	if ( nNewFlags & S2A_EXTRA_DATA_HAS_GAMETAG_DATA )
	{
		buf.WriteString( pchGameType );
	}

	NET_SendPacket ( NULL, pServer->m_Socket, adr, buf.GetData(), buf.GetNumBytesWritten() );
}


/*
=================
SVC_RuleInfo

More detailed server information
=================
*/
void CMaster::ReplyRules( netadr_t &from, CBaseServer *pServer )
{
	if ( serverGameDLL && serverGameDLL->ShouldHideServer() )
		return;
	
	int nNumRules;
	const ConCommandBase *var;

	byte data[8192];
	bf_write buf( "SVC_RuleInfo->buf", data, sizeof(data) );

	nNumRules = cv->CountVariablesWithFlags( FCVAR_NOTIFY );
	if (nNumRules <= 0)        // No server rules active.
		return;

	buf.WriteLong( CONNECTIONLESS_HEADER );

	// Find Player
	buf.WriteByte( S2A_RULES );  // All players coming now.
	buf.WriteShort( nNumRules);

	// Need to respond with game directory, game name, and any server variables that have been set that
	//  effect rules.  Also, probably need a hook into the .dll to respond with additional rule information.
	for ( var = g_pCVar->GetCommands() ; var ; var=var->GetNext() )
	{
		if ( var->IsCommand() )
			continue;

		if (!(var->IsFlagSet( FCVAR_NOTIFY ) ) )
			continue;

		buf.WriteString( var->GetName() );   // Cvar Name

		// For protected cvars, don't send the string
		if ( var->IsFlagSet( FCVAR_PROTECTED ) )
		{
			// If it has a value string and the string is not "none"
			if ( ( strlen( ((ConVar*)var)->GetString() ) > 0 ) &&
				 stricmp( ((ConVar*)var)->GetString(), "none" ) )
			{
				buf.WriteString( "1" );
			}
			else
			{
				buf.WriteString( "0" );
			}
		}
		else
		{
			buf.WriteString( ((ConVar*)var)->GetString() ); // Value
		}
	}

	NET_SendPacket ( NULL, pServer->m_Socket, from, buf.GetData(), buf.GetNumBytesWritten() );
}


void CMaster::ReplyPlayers( netadr_t &adr, CGameServer *pServer )
{
	if ( serverGameDLL && serverGameDLL->ShouldHideServer() )
		return;

	byte	data[1024 * 6];

	bf_write buf( "SVC_PlayerInfo", data, sizeof(data) );

	buf.WriteLong( CONNECTIONLESS_HEADER );

	// Find Player
	buf.WriteByte( S2A_PLAYER );  // All players coming now.
	
	buf.WriteByte( pServer->GetNumClients() );

	for (int i=0; i < pServer->GetClientCount() ; i++)
	{
		CGameClient	*client = pServer->Client(i);
		
		if ( !client->IsConnected() )
			continue;

		CPlayerState *pl = serverGameClients->GetPlayerState( client->edict );
		if ( !pl )
			continue;

		buf.WriteByte( i );
		buf.WriteString( client->GetClientName() );
		buf.WriteLong( pl->frags );  // need to ask game dll for frags

		if ( client->IsFakeClient() )
		{
			buf.WriteFloat( -1.0f );
		}
		else
		{
			buf.WriteFloat( client->GetNetChannel()->GetTimeConnected() );
		}
	}

	NET_SendPacket ( NULL, pServer->m_Socket, adr, buf.GetData(), buf.GetNumBytesWritten() );
}
