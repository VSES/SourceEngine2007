//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// baseserver.cpp: implementation of the CBaseServer class.
//
//////////////////////////////////////////////////////////////////////



#if defined(_WIN32) && !defined(_X360)
#include "winlite.h"		// FILETIME
#elif defined(_LINUX)
#include <time.h>                  
#include <sys/sysinfo.h>          
#include <asm/param.h> // for HZ
#include <netinet/in.h>
#elif defined(_X360)
#else
#error "Includes for CPU usage calcs here"
#endif

#include "filesystem_engine.h"
#include "baseserver.h"
#include "sysexternal.h"
#include "quakedef.h"
#include "host.h"
#include "netmessages.h"
#include "master.h"
#include "sys.h"
#include "framesnapshot.h"
#include "sv_packedentities.h"
#include "dt_send_eng.h"
#include "dt_recv_eng.h"
#include "networkstringtable.h"
#include "sys_dll.h"
#include "host_cmd.h"
#include "sv_steamauth.h"
#include "SteamUserIDValidation.h"

#include <proto_oob.h>
#include <vstdlib/random.h>
#include <irecipientfilter.h>
#include <KeyValues.h>
#include <tier0/vprof.h>
#include <cdll_int.h>
#include <eiface.h>
#include <client_class.h>
#include "tier0/icommandline.h"
#include "sv_steamauth.h"
#include "tier0/vcrmode.h"
#include "sv_ipratelimit.h"

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


// Give new data to Steam's master server updater every N seconds.
// This is NOT how often packets are sent to master servers, only how often the
// game server talks to Steam's master server updater (which is on the game server's
// machine, not the Steam servers).
#define MASTER_SERVER_UPDATE_INTERVAL		2.0

static void SvTagsChangeCallback( IConVar *pConVar, const char *pOldValue, float flOldValue )
{
	ConVarRef var( pConVar );
#ifndef NO_STEAM
	if ( SteamGameServer() )
	{
		SteamGameServer()->GSSetGameType( var.GetString() );
	}
#endif
}

ConVar			sv_region( "sv_region","-1", FCVAR_NONE, "The region of the world to report this server in." );
static ConVar	sv_instancebaselines( "sv_instancebaselines", "1", FCVAR_DEVELOPMENTONLY, "Enable instanced baselines. Saves network overhead." );
static ConVar	sv_stats( "sv_stats", "1", 0, "Collect CPU usage stats" );
static ConVar	sv_enableoldqueries( "sv_enableoldqueries", "0", 0, "Enable support for old style (HL1) server queries" );
static ConVar	sv_password( "sv_password", "", FCVAR_NOTIFY | FCVAR_PROTECTED | FCVAR_DONTRECORD, "Server password for entry into multiplayer games" );
ConVar			sv_tags( "sv_tags", "", FCVAR_NOTIFY, "Server tags. Used to provide extra information to clients when they're browsing for servers. Separate tags with a comma.", SvTagsChangeCallback );
ConVar			sv_visiblemaxplayers( "sv_visiblemaxplayers", "-1", 0, "Overrides the max players reported to prospective clients" );
ConVar			sv_alternateticks( "sv_alternateticks", ( IsX360() ) ? "1" : "0", FCVAR_SPONLY, "If set, server only simulates entities on even numbered ticks.\n" );
ConVar			sv_allow_wait_command( "sv_allow_wait_command", "1", FCVAR_REPLICATED, "Allow or disallow the wait command on clients connected to this server." );
ConVar			sv_allow_color_correction( "sv_allow_color_correction", "1", FCVAR_REPLICATED, "Allow or disallow clients to use color correction on this server." );

// Steam has a matching one in matchmakingtypes.h
#define MAX_TAG_STRING_LENGTH		128

extern CNetworkStringTableContainer *networkStringTableContainerServer;
extern char gpszVersionString[32];
extern int g_iSteamAppID;
extern ConVar sv_stressbots;

extern char gpszVersionString[32];
extern char gpszProductString[32];

int g_CurGameServerID = 1;

// #define ALLOW_DEBUG_DEDICATED_SERVER_OUTSIDE_STEAM

bool AllowDebugDedicatedServerOutsideSteam()
{
#if defined( ALLOW_DEBUG_DEDICATED_SERVER_OUTSIDE_STEAM )
	return true;
#else
	return false;
#endif
}


static void SetMasterServerKeyValue( ISteamMasterServerUpdater *pUpdater, IConVar *pConVar )
{
	ConVarRef var( pConVar );
#ifndef NO_STEAM
	// For protected cvars, don't send the string
	if ( var.IsFlagSet( FCVAR_PROTECTED ) )
	{
		// If it has a value string and the string is not "none"
		if ( ( strlen( var.GetString() ) > 0 ) &&
				stricmp( var.GetString(), "none" ) )
		{
			pUpdater->SetKeyValue( var.GetName(), "1" );
		}
		else
		{
			pUpdater->SetKeyValue( var.GetName(), "0" );
		}
	}
	else
	{
		pUpdater->SetKeyValue( var.GetName(), var.GetString() );
	}

	if ( SteamGameServer() )
	{
		sv.RecalculateTags();
	}
#endif
}


static void ServerNotifyVarChangeCallback( IConVar *pConVar, const char *pOldValue, float flOldValue )
{
	if ( !pConVar->IsFlagSet( FCVAR_NOTIFY ) )
		return;
#ifndef NO_STEAM
	ISteamMasterServerUpdater *pUpdater = SteamMasterServerUpdater();
	if ( !pUpdater )
	{
		// This will force it to send all the rules whenever the master server updater is there.
		sv.SetMasterServerRulesDirty();
		return;
	}
	SetMasterServerKeyValue( pUpdater, pConVar );
#endif
	
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseServer::CBaseServer() : m_ServerQueryChallenges( 0, 1024 ) // start with 1K of entries, and alloc in 1K chunks
{
	// Just get a unique ID to talk to the steam master server updater.
	m_bRestartOnLevelChange = false;
	
	m_StringTables = NULL;
	m_pInstanceBaselineTable = NULL;
	m_pLightStyleTable = NULL;
	m_pUserInfoTable = NULL;
	m_pServerStartupTable = NULL;
	m_pDownloadableFileTable = NULL;

	m_fLastCPUCheckTime = 0;
	m_fStartTime = 0;
	m_fCPUPercent = 0;
	m_Socket = NS_SERVER;
	m_nTickCount = 0;
	
	m_szMapname[0] = 0;
	m_szSkyname[0] = 0;
	m_Password[0] = 0;
	worldmapCRC = 0;
	clientDllCRC = 0;

	serverclasses = serverclassbits = 0;
	m_nMaxclients = m_nSpawnCount = 0;
	m_flTickInterval = 0.03;
	m_nUserid = 0;
	m_bIsDedicated = false;
	m_fCPUPercent = 0;
	m_fStartTime = 0;
	m_fLastCPUCheckTime = 0;
	
	m_bMasterServerRulesDirty = true;
	m_flLastMasterServerUpdateTime = 0;
}

CBaseServer::~CBaseServer()
{

}

/*
================
SV_CheckChallenge

Make sure connecting client is not spoofing
================
*/
bool CBaseServer::CheckChallengeNr( netadr_t &adr, int nChallengeValue )
{
	// See if the challenge is valid
	// Don't care if it is a local address.
	if ( adr.IsLoopback() )
		return true;

	// X360TBD: network
	if ( IsX360() )
		return true;

	for (int i=0 ; i<m_ServerQueryChallenges.Count() ; i++)
	{
		if ( adr.CompareAdr(m_ServerQueryChallenges[i].adr, true) ) // base adr only
		{
			if (nChallengeValue != m_ServerQueryChallenges[i].challenge)
			{
				return false;
			}

			if ( net_time > ( m_ServerQueryChallenges[i].time+ CHALLENGE_LIFETIME) ) // allow challenge values to last for 1 hour
			{
				m_ServerQueryChallenges.FastRemove(i);
				ConMsg( "Old challenge from %s.\n", adr.ToString() );
				return false;
			}

			return true;
		}

		// clean up any old entries
		if ( net_time > ( m_ServerQueryChallenges[i].time+ CHALLENGE_LIFETIME) ) 
		{
			m_ServerQueryChallenges.FastRemove(i);
			i--; // backup one as we just shifted the whole vector back by the deleted element
		}
	}
	
	if ( nChallengeValue != -1 )
	{
		ConDMsg( "No challenge from %s.\n", adr.ToString() ); // this is a common message
	}
	return false;
}


const char *CBaseServer::GetPassword() const
{
	const char *password = sv_password.GetString();

	// if password is empty or "none", return NULL
	if ( !password[0] || !Q_stricmp(password, "none" ) )
	{
		return NULL;
	}

	return password;
}


void CBaseServer::SetPassword(const char *password)
{
	if ( password != NULL )
	{
		Q_strncpy( m_Password, password, sizeof(m_Password) );
	}
	else
	{
		m_Password[0] = 0; // clear password
	}
}

#define MAX_REUSE_PER_IP 5 // 5 outstanding connect request within timeout window, to account for NATs

/*
================
CheckIPConnectionReuse

Determine if this IP requesting the connect is connecting too often
================
*/
bool CBaseServer::CheckIPConnectionReuse( netadr_t &adr )
{
	int nSimultaneouslyConnections = 0;

	for ( int slot = 0 ; slot < m_Clients.Count() ; slot++ )
	{
		CBaseClient *client = m_Clients[slot];

		// if the user is connected but not fully in AND the addr's match
		if ( client->IsConnected() &&
			 !client->IsActive() &&
			 adr.CompareAdr ( client->m_NetChannel->GetRemoteAddress(), true ) )
		{
			nSimultaneouslyConnections++;
		}
	}
	
	if ( nSimultaneouslyConnections > MAX_REUSE_PER_IP ) 
	{
		Msg ("Too many connect packets from %s\n", adr.ToString( true ) );	
		return false; // too many connect packets!!!!
	}
	return true; // this IP is okay
}


int CBaseServer::GetNextUserID()
{
	// Note: we'll usually exit on the first pass of this loop..
	for ( int i=0; i < m_Clients.Count()+1; i++ )
	{
		int nTestID = (m_nUserid + i + 1) % SHRT_MAX;

		// Make sure no client has this user ID.		
		int iClient;
		for ( iClient=0; iClient < m_Clients.Count(); iClient++ )
		{
			if ( m_Clients[iClient]->GetUserID() == nTestID )
				break;
		}

		// Ok, no client has this ID, so return it.		
		if ( iClient == m_Clients.Count() )
			return nTestID;
	}
	
	Assert( !"GetNextUserID: can't find a unique ID." );
	return m_nUserid + 1;
}


/*
================
SV_ConnectClient

Initializes a CSVClient for a new net connection.  This will only be called
once for a player each game, not once for each level change.
================
*/
IClient *CBaseServer::ConnectClient ( netadr_t &adr, int protocol, int challenge, int authProtocol, 
							    const char *name, const char *password, const char *hashedCDkey, int cdKeyLen )
{
	COM_TimestampedLog( "CBaseServer::ConnectClient" );

	if ( !IsActive() )
	{
		return NULL;
	}

	if ( !name || !password || !hashedCDkey )
	{
		return NULL;
	}

	// Make sure protocols match up
	if ( !CheckProtocol( adr, protocol ) )
	{
		return NULL;
	}


	if ( !CheckChallengeNr( adr, challenge ) )
	{
		RejectConnection( adr, "Bad challenge.\n");
		return NULL;
	}

	// SourceTV checks password & restrictions later once we know
	// if its a normal spectator client or a relay proxy
	if ( !IsHLTV() )
	{
		if ( !CheckIPConnectionReuse( adr ) )
		{
			RejectConnection( adr, "Too many pending connections.\n");
			return NULL;
		}

#ifndef NO_STEAM
		// LAN servers restrict to class b IP addresses
		if ( !CheckIPRestrictions( adr, authProtocol ) )
		{
			RejectConnection( adr, "LAN servers are restricted to local clients (class C).\n");
			return NULL;
		}
#endif

		if ( !CheckPassword( adr, password, name ) )
		{
			// failed
			ConMsg ( "%s:  password failed.\n", adr.ToString() );
			// Special rejection handler.
			RejectConnection( adr, "Bad password.\n" );
			return NULL;
		}
	}

	COM_TimestampedLog( "CBaseServer::ConnectClient:  GetFreeClient" );

	CBaseClient	*client = GetFreeClient( adr );

	if ( !client )
	{
		RejectConnection( adr, "Server is full.\n" );
		return NULL;	// no free slot found
	}

	int nNextUserID = GetNextUserID();
	if ( !CheckChallengeType( client, nNextUserID, adr, authProtocol, hashedCDkey, cdKeyLen ) ) // we use the client pointer to track steam requests
	{
		return NULL;
	}

#ifndef _HLTVTEST
	if ( !FinishCertificateCheck( adr, authProtocol, hashedCDkey	) )
	{
		return NULL;
	}
#endif

	COM_TimestampedLog( "CBaseServer::ConnectClient:  NET_CreateNetChannel" );

	// create network channel
	INetChannel * netchan = NET_CreateNetChannel( m_Socket, &adr, adr.ToString(), client );

	if ( !netchan )
	{
		RejectConnection( adr, "Failed to create net channel.\n");
		return NULL;
	}

	// setup netchannl settings
	netchan->SetChallengeNr( challenge );
	
	COM_TimestampedLog( "CBaseServer::ConnectClient:  client->Connect" );

	// make sure client is reset and clear
	client->Connect( name, nNextUserID, netchan, false );

	m_nUserid = nNextUserID;

	// Will get reset from userinfo, but this value comes from sv_updaterate ( the default )
	client->m_fSnapshotInterval = 1.0f/20.0f;
	client->m_fNextMessageTime = net_time + client->m_fSnapshotInterval;
	// Force a full delta update on first packet.
	client->m_nDeltaTick = -1;
	client->m_nSignonTick = 0;
	client->m_pLastSnapshot = NULL;
	
	// Tell client connection worked, now use netchannels
	NET_OutOfBandPrintf ( m_Socket, adr, "%c00000000000000", S2C_CONNECTION );

	// Set up client structure.
	if ( authProtocol == PROTOCOL_HASHEDCDKEY )
	{
		// use hased CD key as player GUID
		Q_strncpy ( client->m_GUID, hashedCDkey, SIGNED_GUID_LEN );
		client->m_GUID[SIGNED_GUID_LEN] = '\0';
	}
	else if ( authProtocol == PROTOCOL_STEAM )
	{
		// StartSteamValidation() above initialized the clients networkid
	}

	if ( netchan && !netchan->IsLoopback() )
		ConMsg("Client \"%s\" connected (%s).\n", client->GetClientName(), netchan->GetAddress() );

	return client;
}

/*
================
RequireValidChallenge

Return true if this server query must provide a valid challenge number
================
*/
bool CBaseServer::RequireValidChallenge( netadr_t &adr )
{
	if ( sv_enableoldqueries.GetBool() == true )
	{
		return false; // don't enforce challenge numbers
	}

	return true;
}

/*
================
ValidChallenge

Return true if this challenge number is correct for this host (for server queries)
================
*/
bool CBaseServer::ValidChallenge( netadr_t & adr, int challengeNr )
{
	if ( !IsActive() )            // Must be running a server.
		return false ;

	if ( !IsMultiplayer() )   // ignore in single player
		return false ;

	if ( RequireValidChallenge( adr) )
	{
		if ( !CheckChallengeNr( adr, challengeNr ) )
		{
			ReplyServerChallenge( adr );
			return false;
		}
	}

	return true; 
}

bool CBaseServer::ValidInfoChallenge( netadr_t & adr, const char *nugget )
{
	if ( !IsActive() )            // Must be running a server.
		return false ;

	if ( !IsMultiplayer() )   // ignore in single player
		return false ;

	if ( RequireValidChallenge( adr) )
	{
		if ( Q_stricmp( nugget, A2S_KEY_STRING ) ) // if the string isn't equal then fail out
		{
			return false;
		}
	}

	return true; 
}


bool CBaseServer::ProcessConnectionlessPacket(netpacket_t * packet)
{
	bf_read msg = packet->message;	// handy shortcut 

	char c = msg.ReadChar();

	if ( !CheckConnectionLessRateLimits( packet->from ) )
	{
		return false;
	}

	switch ( c )
	{
		case A2A_PING :		NET_OutOfBandPrintf (packet->source, packet->from, "%c00000000000000", A2A_ACK );
							break;
	
		case A2A_PRINT :	// if ( net_remoteprint.GetInt() != 0 ) TODO
							{
								char str[1024];
								msg.ReadString( str, sizeof(str) );
								ConMsg( "A2C_PRINT from %s : %s", packet->from.ToString(), str );
							}
							break;

		case A2A_ACK :		ConMsg ("A2A_ACK from %s\n", packet->from.ToString() );
							break;


		case A2S_GETCHALLENGE :  ReplyChallenge( packet->from );
							break;
		
		case A2S_SERVERQUERY_GETCHALLENGE: ReplyServerChallenge( packet->from );
							break;

		case C2S_CONNECT :	{	char cdkey[STEAM_KEYSIZE];
								char name[256];
								char password[256];
								
								int protocol = msg.ReadLong();
								int authProtocol = msg.ReadLong();
								int challengeNr = msg.ReadLong();

								msg.ReadString( name, sizeof(name) );
								msg.ReadString( password, sizeof(password) );
								if ( authProtocol == PROTOCOL_STEAM )
								{
									int keyLen = msg.ReadShort();
									if ( keyLen < 0 || keyLen > sizeof(cdkey) )
									{
										RejectConnection( packet->from, "Invalid Steam key length\n" );
										break;
									}
									msg.ReadBytes( cdkey, keyLen );

									ConnectClient( packet->from, protocol, 
										challengeNr, authProtocol, name, password, cdkey, keyLen );	// cd key is actually a raw encrypted key	
								}
								else
								{
									msg.ReadString( cdkey, sizeof(cdkey) );
									ConnectClient( packet->from, protocol, 
										challengeNr, authProtocol, name, password, cdkey, strlen(cdkey) );
								}

							}
							break;
		
		case A2A_CUSTOM	:	// TODO fire game event with string and adr so 3rd party tool can get it
							break;
							
		default:
		{
			if ( IsUsingMasterLegacyMode() )
			{
				CGameServer *pThis = NULL;
				if ( !IsHLTV() )
					pThis = (CGameServer*)this;
					
				master->HandleUnknown( packet, this, pThis );
			}
#ifndef NO_STEAM
			// We don't understand it, let the master server updater at it.
			else if ( SteamMasterServerUpdater() && Steam3Server().IsMasterServerUpdaterSharingGameSocket() )
			{
				SteamMasterServerUpdater()->HandleIncomingPacket( 
					packet->message.GetBasePointer(), 
					packet->message.TotalBytesAvailable(),
					BigLong( packet->from.GetIP() ),
					packet->from.GetPort()
					);
			
				// This is where it will usually want to respond to something immediately by sending some
				// packets, so check for that immediately.
				ForwardPacketsFromMasterServerUpdater();
			}
#endif
		}
		break;
	}

	return true;
}

int CBaseServer::GetNumFakeClients() const
{
	int count = 0; 
	for ( int i = 0; i < m_Clients.Count(); i++ )
	{
		if ( m_Clients[i]->IsFakeClient() )
		{
			count++;
		}
	}
	return count;
}

/*
==================
void SV_CountPlayers

Counts number of connections.  Clients includes regular connections
==================
*/
int CBaseServer::GetNumClients( void ) const
{
	int count	= 0;

	for (int i=0 ; i < m_Clients.Count() ; i++ )
	{
		if ( m_Clients[ i ]->IsConnected() )
		{
			count++;
		}
	}

	return count;
}

/*
==================
void SV_CountPlayers

Counts number of HLTV connections.  Clients includes regular connections
==================
*/
int CBaseServer::GetNumProxies( void ) const
{
	int count	= 0;

	for (int i=0 ; i < m_Clients.Count() ; i++ )
	{
		if ( m_Clients[ i ]->IsConnected() && m_Clients[ i ]->IsHLTV() )
		{
			count++;
		}
	}

	return count;
}

int CBaseServer::GetNumPlayers()
{
	int count = 0;
	if ( !GetUserInfoTable())
	{
		return 0;
	}

	const int maxPlayers = GetUserInfoTable()->GetNumStrings();

	for ( int i=0; i < maxPlayers; i++ )
	{
		const player_info_t *pi = (const player_info_t *) m_pUserInfoTable->GetStringUserData( i, NULL );

		if ( !pi )
			continue;

		if ( pi->fakeplayer )
			continue;	// don't count bots

		count++;
	}

	return count;
}

bool CBaseServer::GetPlayerInfo( int nClientIndex, player_info_t *pinfo )
{
	if ( !pinfo )
		return false;

	if ( nClientIndex < 0 || nClientIndex >= GetUserInfoTable()->GetNumStrings() )
	{
		Q_memset( pinfo, 0, sizeof( player_info_t ) );
		return false;
	}

	player_info_t *pi = (player_info_t*) GetUserInfoTable()->GetStringUserData( nClientIndex, NULL );

	if ( !pi )
	{
		Q_memset( pinfo, 0, sizeof( player_info_t ) );
		return false;	
	}

	Q_memcpy( pinfo, pi, sizeof( player_info_t ) );

	// Fixup from network order (little endian)
	CByteswap byteswap;
	byteswap.SetTargetBigEndian( false );
	byteswap.SwapFieldsToTargetEndian( pinfo );

	return true;
}


void CBaseServer::UserInfoChanged( int nClientIndex )
{
	player_info_t pi;

	bool oldlock = networkStringTableContainerServer->Lock( false );
	if ( m_Clients[ nClientIndex ]->FillUserInfo( pi ) )
	{
		// Fixup to little endian for networking
		CByteswap byteswap;
		byteswap.SetTargetBigEndian( false );
		byteswap.SwapFieldsToTargetEndian( &pi );

		// update user info settings
		m_pUserInfoTable->SetStringUserData( nClientIndex, sizeof(pi), &pi );
	}
	else
	{
		// delete user data settings
		m_pUserInfoTable->SetStringUserData( nClientIndex, 0, NULL );
	}
	networkStringTableContainerServer->Lock( oldlock );
	
}

void CBaseServer::FillServerInfo(SVC_ServerInfo &serverinfo)
{
	static char gamedir[MAX_OSPATH];
	Q_FileBase( com_gamedir, gamedir, sizeof( gamedir ) );

	serverinfo.m_nProtocol		= PROTOCOL_VERSION;
	serverinfo.m_nServerCount	= GetSpawnCount();
	serverinfo.m_nMapCRC		= worldmapCRC;
	serverinfo.m_nClientCRC		= clientDllCRC;
	serverinfo.m_nMaxClients	= GetMaxClients();
	serverinfo.m_nMaxClasses	= serverclasses;
	serverinfo.m_bIsDedicated	= IsDedicated();
#ifdef _WIN32
	serverinfo.m_cOS			= 'W';
#else
	serverinfo.m_cOS			= 'L';
#endif

	// HACK to signal that the server is "new"
	serverinfo.m_cOS = tolower( serverinfo.m_cOS );

	serverinfo.m_fTickInterval	= GetTickInterval();
	serverinfo.m_szGameDir		= gamedir;
	serverinfo.m_szMapName		= GetMapName();
	serverinfo.m_szSkyName		= m_szSkyname;
	serverinfo.m_szHostName		= GetName();
	serverinfo.m_bIsHLTV		= IsHLTV();
}

/*
=================
SVC_GetChallenge

Returns a challenge number that can be used
in a subsequent client_connect command.
We do this to prevent denial of service attacks that
flood the server with invalid connection IPs.  With a
challenge, they must give a valid IP address.
=================
*/

void CBaseServer::ReplyChallenge(netadr_t &adr)
{
	char	buffer[STEAM_KEYSIZE+32];
	bf_write msg(buffer,sizeof(buffer));

	// get a free challenge number
	int challengeNr = GetChallengeNr( adr );
	int	authprotocol = GetChallengeType( adr );

	msg.WriteLong( CONNECTIONLESS_HEADER );
	
	msg.WriteByte( S2C_CHALLENGE );
	msg.WriteLong( challengeNr );
	msg.WriteLong( authprotocol );

#if !defined( NO_STEAM ) //#ifndef _XBOX
	if ( authprotocol == PROTOCOL_STEAM )
	{
		unsigned int encryptionsize = 0;
		char szEncryptionKey[ STEAM_KEYSIZE ];
		if ( !SteamGameServer() )
		{
			Warning( "SteamGetEncryptionKeyToSendToNewClient:  Library not loaded!\n" );
			RejectConnection( adr, "Failed to get server encryption key\n" );
			return;
		}

		if ( !SteamGameServer()->GSGetSteam2GetEncryptionKeyToSendToNewClient( szEncryptionKey, &encryptionsize, sizeof(szEncryptionKey) ) 
			|| encryptionsize == 0 )
		{
			Warning( "SteamGetEncryptionKeyToSendToNewClient:  Returned NULL!\n" );
			RejectConnection( adr, "Failed to get server encryption key\n" );
			return;
		}
#if !defined( NO_VCR )
		// Support VCR mode.
		if ( VCRGetMode() == VCR_Record )
		{
			VCRGenericRecord( "a", &encryptionsize, sizeof( encryptionsize ) );
			if ( encryptionsize )
			{
				VCRGenericRecord( "b", szEncryptionKey, encryptionsize );
			}
		}
		else if ( VCRGetMode() == VCR_Playback )
		{
			VCRGenericPlayback( "a", &encryptionsize, sizeof( encryptionsize ), true );
			if ( encryptionsize )
			{
				VCRGenericPlayback( "b", szEncryptionKey, encryptionsize, true );
			}			
		}
#endif
		if ( encryptionsize > STEAM_KEYSIZE )
		{
			Warning( "SteamGetEncryptionKeyToSendToNewClient:  Key size too big (%i/%i)\n",
				encryptionsize, STEAM_KEYSIZE );
			RejectConnection( adr, "Failed to get server encryption key\n" );
			return;
		}
		msg.WriteShort( encryptionsize );
		msg.WriteBytes( szEncryptionKey, encryptionsize );
		CSteamID steamID = Steam3Server().GetGSSteamID();
		uint64 unSteamID = steamID.ConvertToUint64();
		msg.WriteBytes( &unSteamID, sizeof(unSteamID) );
		msg.WriteByte( SteamGameServer()->BSecure() );
	}
#else
	msg.WriteShort( 1 );
	msg.WriteByte( 0 );
	uint64 unSteamID = 0;
	msg.WriteBytes( &unSteamID, sizeof(unSteamID) );
	msg.WriteByte( 0 );
#endif
	msg.WriteString( "000000" );	// padding bytes

	NET_SendPacket( NULL, m_Socket, adr, msg.GetData(), msg.GetNumBytesWritten() );
}


/*
=================
ReplyServerChallenge

Returns a challenge number that can be used
in a subsequent server query commands.
We do this to prevent DDoS attacks via bandwidth
amplification.
=================
*/
void CBaseServer::ReplyServerChallenge(netadr_t &adr)
{
	char	buffer[16];
	bf_write msg(buffer,sizeof(buffer));

	// get a free challenge number
	int challengeNr = GetChallengeNr( adr );
	
	msg.WriteLong( CONNECTIONLESS_HEADER );
	msg.WriteByte( S2C_CHALLENGE );
	msg.WriteLong( challengeNr );
	NET_SendPacket( NULL, m_Socket, adr, msg.GetData(), msg.GetNumBytesWritten() );
}

const char *CBaseServer::GetName( void ) const
{
	return host_name.GetString();
}

int CBaseServer::GetChallengeType(netadr_t &adr)
{
	if ( AllowDebugDedicatedServerOutsideSteam() )
		return PROTOCOL_HASHEDCDKEY;
	
	// don't auth SP games or local mp games if steam isn't running
#ifndef NO_STEAM
	if ( Host_IsSinglePlayerGame() || ( !SteamUser() && !IsDedicated() ))
#else
	if ( Host_IsSinglePlayerGame() || ( !IsDedicated() ))
#endif
	{
		return PROTOCOL_HASHEDCDKEY;
	}
	else
	{
		return PROTOCOL_STEAM;
	}
}

int CBaseServer::GetChallengeNr (netadr_t &adr)
{
	int		oldest = 0;
	float	oldestTime = FLT_MAX;
		
	// see if we already have a challenge for this ip
	for (int i = 0 ; i < m_ServerQueryChallenges.Count() ; i++)
	{
		if ( adr.CompareAdr (m_ServerQueryChallenges[i].adr, true) )
		{
			// reuse challenge number, but update time
			m_ServerQueryChallenges[i].time = net_time;
			return m_ServerQueryChallenges[i].challenge;
		}
		
		if (m_ServerQueryChallenges[i].time < oldestTime)
		{
			// remember oldest challenge
			oldestTime = m_ServerQueryChallenges[i].time;
			oldest = i;
		}
	}

	if ( m_ServerQueryChallenges.Count() > MAX_CHALLENGES )
	{
		m_ServerQueryChallenges.FastRemove( oldest );	
	}

	int newEntry = m_ServerQueryChallenges.AddToTail();
	// note the 0x0FFF of the top 16 bits, so that -1 will never be sent as a challenge
	m_ServerQueryChallenges[newEntry].challenge = (RandomInt(0,0x0FFF) << 16) | RandomInt(0,0xFFFF);
	m_ServerQueryChallenges[newEntry].adr = adr;
	m_ServerQueryChallenges[newEntry].time = net_time;
	
	return m_ServerQueryChallenges[newEntry].challenge;
}

void CBaseServer::GetNetStats( float &avgIn, float &avgOut )
{
	avgIn = avgOut = 0.0f;

	for (int i = 0; i < m_Clients.Count(); i++ )
	{
		CBaseClient	*cl = m_Clients[ i ];

		// Fake clients get killed in here.
		if ( cl->IsFakeClient() )
			continue;
	
		if ( !cl->IsConnected() )
			continue;

		INetChannel *netchan = cl->GetNetChannel();

		avgIn += netchan->GetAvgData(FLOW_INCOMING);
		avgOut += netchan->GetAvgData(FLOW_OUTGOING);
	}
}

void CBaseServer::CalculateCPUUsage( void )
{
	if ( !sv_stats.GetBool() )
	{
		return;
	}

	if(m_fStartTime==0)
	// record when we started
	{
		m_fStartTime=Sys_FloatTime();
	}

	if( Sys_FloatTime () > m_fLastCPUCheckTime+1)
	// only do this every 1 second
	{
#if defined ( _WIN32 ) 
		static float lastAvg=0;
		static __int64 lastTotalTime=0,lastNow=0;

		HANDLE handle;
		FILETIME creationTime, exitTime, kernelTime, userTime, nowTime;
 		__int64 totalTime,now;
			
		handle = GetCurrentProcess ();

		// get CPU time
		GetProcessTimes (handle, &creationTime, &exitTime,
						&kernelTime, &userTime);
		GetSystemTimeAsFileTime(&nowTime);

		if(lastNow==0)
		{
			memcpy(&lastNow,&creationTime,sizeof(__int64));;
		}


		memcpy(&totalTime,&userTime,sizeof(__int64));;
		memcpy(&now,&kernelTime,sizeof(__int64));;
		totalTime+=now;

		memcpy(&now,&nowTime,sizeof(__int64));;


		m_fCPUPercent = (double)(totalTime-lastTotalTime)/(double)(now-lastNow);
		
		// now save this away for next time
		if(Sys_FloatTime () > lastAvg+5) 
		// only do it every 5 seconds, so we keep a moving average
		{
			memcpy(&lastNow,&nowTime,sizeof(__int64));
			memcpy(&lastTotalTime,&totalTime,sizeof(__int64));
			lastAvg=m_fLastCPUCheckTime;
		}
#else
		// linux CPU % code here :)
		static int32 lastrunticks,lastcputicks;
		static float lastAvg=0;

		struct sysinfo infos; 
		int32 dummy;
		int length;
		char statFile[PATH_MAX];
		int32 now = time(NULL);
		int32 ctime,stime,start_time;
		FILE *pFile;
		int32 runticks,cputicks;

		snprintf(statFile,PATH_MAX,"/proc/%i/stat",getpid());
		
		// we can't use FS_Open() cause its outside our dir
		pFile = fopen(statFile, "r");
		if ( pFile == NULL )
        	{
			goto end;
        	}
	        sysinfo(&infos);

		fscanf(pFile,
			"%d %s %c %d %d %d %d %d %lu %lu \
			%lu %lu %lu %ld %ld %ld %ld %ld %ld %lu \
			%lu %ld %lu %lu %lu %lu %lu %lu %lu %lu \
			%lu %lu %lu %lu %lu %lu",
			&dummy,statFile,&dummy,&dummy,&dummy,&dummy,
			&dummy,&dummy,&dummy,&dummy, // end of first line
			&dummy,&dummy,&dummy,&ctime,&stime,
			&dummy,&dummy,&dummy,&dummy,&dummy, // end of second
			&start_time,&dummy,&dummy,&dummy,&dummy,
			&dummy,&dummy,&dummy,&dummy,&dummy, // end of third
			&dummy,&dummy,&dummy,&dummy,&dummy,&dummy);
		fclose(pFile);					

		runticks = infos.uptime*HZ -start_time; // time the process has been running
		cputicks = stime+ctime;
			
		if(lastcputicks==0)
		{
			lastcputicks=cputicks;
		}

		if(lastrunticks==0)
		{
			lastrunticks=runticks;
		}
		else
		{
			m_fCPUPercent = (float)(cputicks-lastcputicks)/(float)(runticks-lastrunticks);
		}	

		//ConMsg("%f %li %li %li %li\n",cpuPercent,
		//	cputicks,(cputicks-lastcputicks),
		//	(runticks-lastrunticks),runticks);
	
		// now save this away for next time
		if(Sys_FloatTime () > lastAvg+5) 
		{
			lastcputicks=cputicks;
			lastrunticks=runticks;	
			lastAvg=Sys_FloatTime();
		}

		// limit checking :)
		if( m_fCPUPercent > 0.999 )
			m_fCPUPercent = 0.999;
		if( m_fCPUPercent < 0 )
			m_fCPUPercent = 0;

end:
#endif
		m_fLastCPUCheckTime=Sys_FloatTime(); 
	}
}


//-----------------------------------------------------------------------------
// Purpose: Prepare for level transition, etc.
//-----------------------------------------------------------------------------
void CBaseServer::InactivateClients( void )
{
	for (int i = 0; i < m_Clients.Count(); i++ )
	{
		CBaseClient	*cl = m_Clients[ i ];

		// Fake clients get killed in here.
		if ( cl->IsFakeClient() && !cl->IsHLTV() )
		{
			// If we don't do this, it'll have a bunch of extra steam IDs for unauthenticated users.
			Steam3Server().NotifyClientDisconnect( cl );
			cl->Clear();
			continue;
		}
		else if ( !cl->IsConnected() )
		{
			continue;
		}

		cl->Inactivate();
	}
}

void CBaseServer::ReconnectClients( void )
{
	for (int i=0 ; i< m_Clients.Count() ; i++ )
	{
		CBaseClient *cl = m_Clients[i];
		
		if ( cl->IsConnected() )
		{
			cl->m_nSignonState = SIGNONSTATE_CONNECTED;
			NET_SignonState signon( cl->m_nSignonState, -1 );
			cl->SendNetMsg( signon );
		}
	}
}

/*
==================
SV_CheckTimeouts

If a packet has not been received from a client in sv_timeout.GetFloat()
seconds, drop the conneciton.

When a client is normally dropped, the CSVClient goes into a zombie state
for a few seconds to make sure any final reliable message gets resent
if necessary
==================
*/
void CBaseServer::CheckTimeouts (void)
{
	// Don't timeout in _DEBUG builds
	int i;

#if !defined( _DEBUG )
		
	for (i=0 ; i< m_Clients.Count() ; i++ )
	{
		IClient	*cl = m_Clients[ i ];
		
		if ( cl->IsFakeClient() || !cl->IsConnected() )
			continue;

		INetChannel *netchan = cl->GetNetChannel();

		if ( !netchan )
			continue;

	

		if ( netchan->IsTimedOut() )
		{
			cl->Disconnect ("%s timed out", cl->GetClientName() );
		}
	}
#endif

	for (i=0 ; i< m_Clients.Count() ; i++ )
	{
		IClient	*cl = m_Clients[ i ];
		
		if ( cl->IsFakeClient() || !cl->IsConnected() )
			continue;
		
		if ( cl->GetNetChannel() && cl->GetNetChannel()->IsOverflowed() )
		{
			cl->Disconnect( "Client %d overflowed reliable channel.", i );
		}
	}
}

// ==================
// check if clients update thier user setting (convars) and call 
// ==================
void CBaseServer::UpdateUserSettings(void)
{
	for (int i=0 ; i< m_Clients.Count() ; i++ )
	{
		CBaseClient	*cl = m_Clients[ i ];

		if ( cl->m_bConVarsChanged )
		{
			cl->UpdateUserSettings();
		}
	}
}

// ==================
// check if clients need the serverinfo packet sent
// ==================
void CBaseServer::SendPendingServerInfo()
{
	for (int i=0 ; i< m_Clients.Count() ; i++ )
	{
		CBaseClient	*cl = m_Clients[ i ];

		if ( cl->m_bSendServerInfo )
		{
			cl->SendServerInfo();
		}
	}
}

// compresses a packed entity, returns data & bits
const char *CBaseServer::CompressPackedEntity(ServerClass *pServerClass, const char *data, int &bits)
{
	static char s_packedData[MAX_PACKEDENTITY_DATA];

	bf_write writeBuf( "CompressPackedEntity", s_packedData, sizeof( s_packedData ) );

	const void *pBaselineData = NULL;
	int nBaselineBits = 0;

	Assert( pServerClass != NULL );
		
	GetClassBaseline( pServerClass, &pBaselineData, &nBaselineBits );
	nBaselineBits *= 8;

	Assert( pBaselineData != NULL );

	SendTable_WriteAllDeltaProps(
		pServerClass->m_pTable,
		pBaselineData,
		nBaselineBits,
		data,
		bits,
		-1,
		&writeBuf );
	
	//overwrite in bits with out bits
	bits = writeBuf.GetNumBitsWritten();

	return s_packedData;
}

// uncompresses a 
const char* CBaseServer::UncompressPackedEntity(PackedEntity *pPackedEntity, int &bits)
{
	UnpackedDataCache_t *pdc = framesnapshotmanager->GetCachedUncompressedEntity( pPackedEntity );

	if ( pdc->bits > 0 )
	{
		// found valid uncompressed version in cache
		bits= pdc->bits;
		return pdc->data;
	}

	// not in cache, so uncompress it

	const void *pBaseline;
	int nBaselineBytes = 0;

	GetClassBaseline( pPackedEntity->m_pServerClass, &pBaseline, &nBaselineBytes );

	Assert( pBaseline != NULL );

	// store this baseline in u.m_pUpdateBaselines
	bf_read oldBuf( "UncompressPackedEntity1", pBaseline, nBaselineBytes );
	bf_read newBuf( "UncompressPackedEntity2", pPackedEntity->GetData(), Bits2Bytes(pPackedEntity->GetNumBits()) );
	bf_write outBuf( "UncompressPackedEntity3", pdc->data, MAX_PACKEDENTITY_DATA );

	Assert( pPackedEntity->m_pClientClass );

	RecvTable_MergeDeltas( 
		pPackedEntity->m_pClientClass->m_pRecvTable,
		&oldBuf,
		&newBuf,
		&outBuf );

	bits = pdc->bits = outBuf.GetNumBitsWritten();
		
	return pdc->data;
}

/*
================
SV_CheckProtocol

Make sure connecting client is using proper protocol
================
*/
bool CBaseServer::CheckProtocol( netadr_t &adr, int nProtocol )
{
	if ( nProtocol != PROTOCOL_VERSION )
	{
		// Client is newer than server
		if ( nProtocol > PROTOCOL_VERSION )
		{
			RejectConnection( adr, "This server is using an older protocol ( %i ) than your client ( %i ).\n",
				PROTOCOL_VERSION, nProtocol  );
		}
		else
		// Server is newer than client
		{
			RejectConnection( adr, "This server is using a newer protocol ( %i ) than your client ( %i ).\n",
				PROTOCOL_VERSION, nProtocol );
		}
		return false;
	}

	// Success
	return true;
}

/*
================
SV_CheckKeyInfo

Determine if client is outside appropriate address range
================
*/
bool CBaseServer::CheckChallengeType( CBaseClient * client, int nNewUserID, netadr_t & adr, int nAuthProtocol, const char *pchLogonCookie, int cbCookie )
{
	if ( AllowDebugDedicatedServerOutsideSteam() )
		return true;

	// Check protocol ID
	if ( ( nAuthProtocol <= 0 ) || ( nAuthProtocol > PROTOCOL_LASTVALID ) )
	{
		RejectConnection( adr, "Invalid connection.\n");
		return false;
	}

	if ( ( nAuthProtocol == PROTOCOL_HASHEDCDKEY ) && (Q_strlen( pchLogonCookie ) <= 0 ||  Q_strlen(pchLogonCookie) != 32 ) )
	{
		RejectConnection( adr, "Invalid authentication certificate length.\n" );
		return false;
	}

	if ( nAuthProtocol == PROTOCOL_STEAM )
	{
		// Dev hack to allow 360/Steam PC cross platform play
// 		int ip0 = 207;
// 		int ip1 = 173;
// 		int ip2 = 179;
// 		int ip3Min = 230;
// 		int ip3Max = 245;
// 
// 		if ( adr.ip[0] == ip0 &&
// 			adr.ip[1] == ip1 &&
// 			adr.ip[2] == ip2 &&
// 			adr.ip[3] >= ip3Min &&
// 			adr.ip[3] <= ip3Max )
// 		{
// 			return true;
// 		}

		client->m_NetworkID.idtype = IDTYPE_STEAM;
		Q_memset( &client->m_NetworkID.uid.steamid, 0x0, sizeof(client->m_NetworkID.uid.steamid) );
		// Convert raw certificate back into data
#ifndef NO_STEAM
		if ( cbCookie <= 0 || cbCookie >= STEAM_KEYSIZE )
		{
			RejectConnection( adr, "STEAM certificate length error! %i/%i\n", cbCookie, STEAM_KEYSIZE );
			return false;
		}
#endif
		netadr_t checkAdr = adr;
		if ( adr.GetType() == NA_LOOPBACK || adr.IsLocalhost() )
		{
			checkAdr.SetIP( net_local_adr.addr_htonl() );
		}
#ifndef NO_STEAM
		if ( !Steam3Server().NotifyClientConnect( client, nNewUserID, checkAdr, pchLogonCookie, cbCookie ) 
			&& !Steam3Server().BLanOnly() ) // the userID isn't alloc'd yet so we need to fill it in manually
		{
			RejectConnection( adr, "STEAM validation rejected\n" );
			return false;
		}
#endif
	}
	else
	{
		if ( !Steam3Server().NotifyLocalClientConnect( client ) ) // the userID isn't alloc'd yet so we need to fill it in manually
		{
			RejectConnection( adr, "GSCreateLocalUser failed\n" );
			return false;
		}
	}

	return true;
}

bool CBaseServer::CheckIPRestrictions( const netadr_t &adr, int nAuthProtocol )
{
	// Determine if client is outside appropriate address range
	if ( adr.IsLoopback() )
		return true;

	// X360TBD: network
	if ( IsX360() )
		return true;

	// allow other users if they're on the same ip range
	if ( Steam3Server().BLanOnly() )
	{
		// allow connection, if client is in the same subnet 
		if ( adr.CompareClassBAdr( net_local_adr ) )
			return true;

		// allow connection, if client has a private IP
		if ( adr.IsReservedAdr() )
			return true;
		
		// reject connection
		return false;
	}

	return true;
}

void CBaseServer::SetMasterServerRulesDirty()
{
	m_bMasterServerRulesDirty = true;
}

bool CBaseServer::CheckPassword( netadr_t &adr, const char *password, const char *name )
{
	const char *server_password = GetPassword();

	if ( !server_password )
		return true;	// no password set

	if ( adr.IsLocalhost() || adr.IsLoopback() )
	{
		return true; // local client can always connect
	}

	int iServerPassLen = Q_strlen(server_password);

	if ( iServerPassLen != Q_strlen(password) )
	{
		return false; // different length cannot be equal
	}

	if ( Q_strncmp( password, server_password, iServerPassLen ) == 0)
	{
		return true; // passwords are equal
	}

	return false; // all test failed
}

float CBaseServer::GetTime() const
{
	return m_nTickCount * m_flTickInterval;
}

float CBaseServer::GetFinalTickTime() const
{
	return (m_nTickCount + (host_frameticks - host_currentframetick)) * m_flTickInterval;
}

void CBaseServer::DisconnectClient(IClient *client, const char *reason )
{
	client->Disconnect( reason );
}

void CBaseServer::Clear( void )
{
	if ( m_StringTables )
	{
		m_StringTables->RemoveAllTables();
		m_StringTables = NULL;
	}

	m_pInstanceBaselineTable = NULL;
	m_pLightStyleTable = NULL;
	m_pUserInfoTable = NULL;
	m_pServerStartupTable = NULL;

	m_State = ss_dead;
	
	m_nTickCount = 0;
	
	Q_memset( m_szMapname, 0, sizeof( m_szMapname ) );
	Q_memset( m_szSkyname, 0, sizeof( m_szSkyname ) );

	clientDllCRC = 0;
	worldmapCRC = 0;

	MEM_ALLOC_CREDIT();

	// Use a different limit on the signon buffer, so we can save some memory in SP (for xbox).
	if ( IsMultiplayer() || IsDedicated() )
	{
		m_SignonBuffer.EnsureCapacity( NET_MAX_PAYLOAD );
	}
	else
	{
		m_SignonBuffer.EnsureCapacity( 16384 );
	}
	
	m_Signon.StartWriting( m_SignonBuffer.Base(), m_SignonBuffer.Count() );
	m_Signon.SetDebugName( "m_Signon" );

	serverclasses = 0;
	serverclassbits = 0;

	m_ServerQueryChallenges.RemoveAll();
}

/*
================
SV_RejectConnection

Rejects connection request and sends back a message
================
*/
void CBaseServer::RejectConnection( const netadr_t &adr, char *fmt, ... )
{
	va_list		argptr;
	char	text[1024];

	va_start (argptr, fmt);
	Q_vsnprintf ( text, sizeof( text ), fmt, argptr);
	va_end (argptr);

	NET_OutOfBandPrintf( m_Socket, adr, "%c%s", S2C_CONNREJECT, text );
}

void CBaseServer::SetPaused(bool paused)
{
	if ( !IsPausable() )
	{
		return;
	}

	if ( !IsActive() )
		return;

	if ( paused )
	{
		m_State = ss_paused;
	}
	else
	{
		m_State = ss_active;
	}

	SVC_SetPause setpause( paused );
	BroadcastMessage( setpause );
}

//-----------------------------------------------------------------------------
// Purpose: General initialization of the server
//-----------------------------------------------------------------------------
void CBaseServer::Init (bool bIsDedicated)
{
	m_nMaxclients = 0;
	m_nSpawnCount = 0;
	m_nUserid = 1;
	m_bIsDedicated = bIsDedicated;
	m_Socket = NS_SERVER;	
	
	m_Signon.SetDebugName( "m_Signon" );
	
	g_pCVar->InstallGlobalChangeCallback( ServerNotifyVarChangeCallback );
	SetMasterServerRulesDirty();
	
	Clear();
}

INetworkStringTable *CBaseServer::GetInstanceBaselineTable( void )
{
	if ( m_pInstanceBaselineTable == NULL )
	{
		m_pInstanceBaselineTable = m_StringTables->FindTable( INSTANCE_BASELINE_TABLENAME );
	}

	return m_pInstanceBaselineTable;
}

INetworkStringTable *CBaseServer::GetLightStyleTable( void )
{
	if ( m_pLightStyleTable == NULL )
	{
		m_pLightStyleTable= m_StringTables->FindTable( LIGHT_STYLES_TABLENAME );
	}

	return m_pLightStyleTable;
}

INetworkStringTable *CBaseServer::GetUserInfoTable( void )
{
	if ( m_pUserInfoTable == NULL )
	{
		if ( m_StringTables == NULL )
		{
			return NULL;
		}
		m_pUserInfoTable = m_StringTables->FindTable( USER_INFO_TABLENAME );
	}

	return m_pUserInfoTable;
}

bool CBaseServer::GetClassBaseline( ServerClass *pClass, void const **pData, int *pDatalen )
{
	if ( sv_instancebaselines.GetInt() )
	{
		ErrorIfNot( pClass->m_InstanceBaselineIndex != INVALID_STRING_INDEX,
			("SV_GetInstanceBaseline: missing instance baseline for class '%s'", pClass->m_pNetworkName)
		);
		
		*pData = GetInstanceBaselineTable()->GetStringUserData(
			pClass->m_InstanceBaselineIndex,
			pDatalen );
		
		return *pData != NULL;
	}
	else
	{
		static char dummy[1] = {0};
		*pData = dummy;
		*pDatalen = 1;
		return true;
	}
}


bool CBaseServer::ShouldUpdateMasterServer()
{
	// If the game server itself is ever running, then it's the one who gets to update the master server.
	// (SourceTV will not update it in this case).
	return true;
}


void CBaseServer::CheckMasterServerRequestRestart()
{
#ifndef NO_STEAM
	if ( !SteamMasterServerUpdater() || !SteamMasterServerUpdater()->WasRestartRequested() )
		return;
#else
	return;
#endif

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


void CBaseServer::UpdateMasterServer()
{
#ifndef NO_STEAM
	if ( !ShouldUpdateMasterServer() )
		return;

	if ( IsUsingMasterLegacyMode() )
	{
		master->CheckHeartbeat( this );
		return;
	}
	
	if ( !SteamMasterServerUpdater() )
		return;
	
	// Only update every so often.
	double flCurTime = Plat_FloatTime();
	if ( flCurTime - m_flLastMasterServerUpdateTime < MASTER_SERVER_UPDATE_INTERVAL )
		return;

	m_flLastMasterServerUpdateTime = flCurTime;


	ForwardPacketsFromMasterServerUpdater();
	CheckMasterServerRequestRestart();
	

	if ( NET_IsDedicated() && sv_region.GetInt() == -1 )
    {
		sv_region.SetValue( 255 ); // HACK!HACK! undo me once we want to enforce regions

        //Log_Printf( "You must set sv_region in your server.cfg or use +sv_region on the command line\n" );
		//Con_Printf( "You must set sv_region in your server.cfg or use +sv_region on the command line\n" );
        //Cbuf_AddText( "quit\n" );
        //return;
    }

	static bool bUpdateMasterServers = !CommandLine()->FindParm( "-nomaster" );
	if ( !bUpdateMasterServers )
		return;

	bool bActive = IsActive() && IsMultiplayer() && g_bEnableMasterServerUpdater;
	if ( serverGameDLL && serverGameDLL->ShouldHideServer() )
		bActive = false;
	
	SteamMasterServerUpdater()->SetActive( bActive );

	if ( !bActive )
		return;

	UpdateMasterServerRules();
	UpdateMasterServerPlayers();
	UpdateMasterServerBasicData();
#endif
}


void CBaseServer::UpdateMasterServerRules()
{
#ifndef NO_STEAM
	// Only do this if the rules vars are dirty.
	if ( !m_bMasterServerRulesDirty )
		return;

	ISteamMasterServerUpdater *pUpdater = SteamMasterServerUpdater();
	if ( !pUpdater )
		return;
		
	pUpdater->ClearAllKeyValues();
	
	// Need to respond with game directory, game name, and any server variables that have been set that
	//  effect rules.  Also, probably need a hook into the .dll to respond with additional rule information.
	ConCommandBase *var;
	for ( var = g_pCVar->GetCommands() ; var ; var=var->GetNext() )
	{
		if ( !(var->IsFlagSet( FCVAR_NOTIFY ) ) )
			continue;

		ConVar *pConVar = dynamic_cast< ConVar* >( var );
		if ( !pConVar )
			continue;

		SetMasterServerKeyValue( pUpdater, pConVar );
	}

	if ( SteamGameServer() )
	{
		RecalculateTags();
	}

	// Ok.. it's all updated, only send incremental updates now until we decide they're all dirty.
	m_bMasterServerRulesDirty = false;
#endif
}


void CBaseServer::UpdateMasterServerBasicData()
{
#ifndef NO_STEAM
	ISteamMasterServerUpdater *pUpdater = SteamMasterServerUpdater();

	Assert( SteamMasterServerUpdater() != NULL );

	unsigned short nMaxReportedClients = GetMaxClients();
	if ( sv_visiblemaxplayers.GetInt() > 0 && sv_visiblemaxplayers.GetInt() < GetMaxClients() )
		nMaxReportedClients = sv_visiblemaxplayers.GetInt();

	pUpdater->SetBasicServerData(
		PROTOCOL_VERSION,
		IsDedicated(),
		sv_region.GetString(),
		gpszProductString,
		nMaxReportedClients,
		(GetPassword() != NULL),
		serverGameDLL->GetGameDescription() );
#endif
}


void CBaseServer::ForwardPacketsFromMasterServerUpdater()
{
#ifndef NO_STEAM
	ISteamMasterServerUpdater *p = SteamMasterServerUpdater();
	if ( !p )
		return;
	
	while ( 1 )
	{
		uint32 netadrAddress;
		uint16 netadrPort;
		unsigned char packetData[16 * 1024];
 		int len = p->GetNextOutgoingPacket( packetData, sizeof( packetData ), &netadrAddress, &netadrPort );
		if ( len <= 0 )
			break;
		
		// Send this packet for them..
		netadr_t adr( netadrAddress, netadrPort );
		NET_SendPacket( NULL, m_Socket, adr, packetData, len );
	}
#endif
}


/*
=================
SV_ReadPackets

Read's packets from clients and executes messages as appropriate.
=================
*/

void CBaseServer::RunFrame( void )
{
	VPROF_BUDGET( "CBaseServer::RunFrame", VPROF_BUDGETGROUP_OTHER_NETWORKING );

	NET_ProcessSocket( m_Socket, this );	

	CheckTimeouts();	// drop clients that timeed out

	UpdateUserSettings(); // update client settings 
	
	SendPendingServerInfo(); // send outstanding signon packets after ALL user settings have been updated

	CalculateCPUUsage(); // update CPU usage

	UpdateMasterServer();

	if ( m_bMasterServerRulesDirty )
	{
		m_bMasterServerRulesDirty = false;
		RecalculateTags();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *adr - 
//			*pslot - 
//			**ppClient - 
// Output : int
//-----------------------------------------------------------------------------
CBaseClient * CBaseServer::GetFreeClient( netadr_t &adr )
{
	CBaseClient *freeclient = NULL;
	
	for ( int slot = 0 ; slot < m_Clients.Count() ; slot++ )
	{
		CBaseClient *client = m_Clients[slot];

		if ( client->IsFakeClient() )
			continue;

		if ( client->IsConnected() )
		{
			if ( adr.CompareAdr ( client->m_NetChannel->GetRemoteAddress() ) )
			{
				ConMsg ( "%s:reconnect\n", adr.ToString() );

				RemoveClientFromGame( client );

				// perform a silent netchannel shutdown, don't send disconnect msg
				client->m_NetChannel->Shutdown( NULL );
				client->m_NetChannel = NULL;
		
				client->Clear();
				return client;
			}
		}
		else
		{
			// use first found free slot 
			if ( !freeclient )
			{
				freeclient = client; 
			}
		}
	}

	if ( !freeclient )
	{
		int count = m_Clients.Count();

		if ( count >= m_nMaxclients )
		{
			return NULL; // server full
		}

		// we have to create a new client slot
		freeclient = CreateNewClient( count );
		
		m_Clients.AddToTail( freeclient );
	}
	
	// Success
	return freeclient;
}

void CBaseServer::SendClientMessages ( bool bSendSnapshots )
{
	VPROF_BUDGET( "SendClientMessages", VPROF_BUDGETGROUP_OTHER_NETWORKING );
	
	for (int i=0; i< m_Clients.Count(); i++ )
	{
		CBaseClient* client = m_Clients[i];
		
		// Update Host client send state...
		if ( !client->ShouldSendMessages() )
			continue;

		// Connected, but inactive, just send reliable, sequenced info.
		if ( client->m_NetChannel )
		{
			client->m_NetChannel->Transmit();
			client->UpdateSendState();
		}
		else
		{
			Msg("Client has no netchannel.\n");
		}
	}
}

CBaseClient *CBaseServer::CreateFakeClient(const char *name)
{
	netadr_t adr; // it's an empty address

	CBaseClient *fakeclient = GetFreeClient( adr );
				
	if ( !fakeclient )
	{
		// server is full
		return NULL;		
	}

	INetChannel *netchan = NULL;
	if ( sv_stressbots.GetBool() )
	{
		netadr_t adr( 0, 0 ); // 0.0.0.0:0 signifies a bot. It'll plumb all the way down to winsock calls but it won't make them.
		netchan = NET_CreateNetChannel( m_Socket, &adr, adr.ToString(), fakeclient, true );
	}

	// a NULL netchannel signals a fakeclient
	m_nUserid = GetNextUserID();
	fakeclient->Connect( name, m_nUserid, netchan, true );

	// fake some cvar settings
	//fakeclient->SetUserCVar( "name", name ); // set already by Connect()
	fakeclient->SetUserCVar( "rate", "30000" );
	fakeclient->SetUserCVar( "cl_updaterate", "20" );
	fakeclient->SetUserCVar( "cl_interp_ratio", "1.0" );
	fakeclient->SetUserCVar( "cl_interp", "0.1" );
	fakeclient->SetUserCVar( "cl_interpolate", "0" );
	fakeclient->SetUserCVar( "cl_predict", "1" );
	fakeclient->SetUserCVar( "cl_predictweapons", "1" );
	fakeclient->SetUserCVar( "cl_lagcompensation", "1" );
	fakeclient->SetUserCVar( "closecaption","0" );
	fakeclient->SetUserCVar( "english", "1" );

	// create client in game.dll
	fakeclient->ActivatePlayer();

	fakeclient->m_nSignonTick = m_nTickCount;
			
	return fakeclient;
}

void CBaseServer::Shutdown( void )
{
	if ( !IsActive() )
		return;

	m_State = ss_dead;

	// Only drop clients if we have not cleared out entity data prior to this.
	for(  int i=m_Clients.Count()-1; i>=0; i-- )
	{
		CBaseClient * cl = m_Clients[ i ];
		if ( cl->IsConnected() )
		{
			cl->Disconnect( "Server shutting down" );
		}
		else
		{
			// free any memory do this out side here in case the reason the server is shutting down 
			//  is because the listen server client typed disconnect, in which case we won't call
			//  cl->DropClient, but the client might have some frame snapshot references left over, etc.
			cl->Clear();	
		}

		delete cl;

		m_Clients.Remove( i );
	}

	// Let drop messages go out
	Sys_Sleep( 100 );

	// clear everthing
	Clear();

#ifndef _XBOX
#ifndef NO_STEAM
	//  Tell master we are shutting down
	if ( SteamMasterServerUpdater() )
		SteamMasterServerUpdater()->NotifyShutdown();

	master->ShutdownConnection( this );
#endif
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Sends text to all active clients
// Input  : *fmt -
//			... -
//-----------------------------------------------------------------------------
void CBaseServer::BroadcastPrintf (const char *fmt, ...)
{
	va_list		argptr;
	char		string[1024];

	va_start (argptr,fmt);
	Q_vsnprintf (string, sizeof( string ), fmt,argptr);
	va_end (argptr);

	SVC_Print print( string );
	BroadcastMessage( print );	
}

void CBaseServer::BroadcastMessage( INetMessage &msg, bool onlyActive, bool reliable )
{
	for ( int i = 0; i < m_Clients.Count(); i++ )
	{
		CBaseClient *cl = m_Clients[ i ];

		if ( (onlyActive && !cl->IsActive()) || !cl->IsSpawned() )
		{
			continue;
		}

		if ( !cl->SendNetMsg( msg, reliable ) )
		{
			if ( msg.IsReliable() || reliable )
			{
				DevMsg( "BroadcastMessage: Reliable broadcast message overflow for client %s", cl->GetClientName() );
			}
		}
	}
}

void CBaseServer::BroadcastMessage( INetMessage &msg, IRecipientFilter &filter )
{
	if ( filter.IsInitMessage() )
	{
		// This really only applies to the first player to connect, but that works in single player well enought
		if ( IsActive() )
		{
			ConDMsg( "SV_BroadcastMessage: Init message being created after signon buffer has been transmitted\n" );
		}
		
		if ( !msg.WriteToBuffer( m_Signon ) )
		{
			Sys_Error( "SV_BroadcastMessage: Init message would overflow signon buffer!\n" );
			return;
		}
	}
	else
	{
		msg.SetReliable( filter.IsReliable() );

		int num = filter.GetRecipientCount();
	
		for ( int i = 0; i < num; i++ )
		{
			int index = filter.GetRecipientIndex( i );

			if ( index < 1 || index > m_Clients.Count() )
			{
				Msg( "SV_BroadcastMessage:  Recipient Filter for message type %i (reliable: %s, init: %s) with bogus client index (%i) in list of %i clients\n", 
						msg.GetType(), 
						filter.IsReliable() ? "yes" : "no",
						filter.IsInitMessage() ? "yes" : "no",
						index, num );

				if ( msg.IsReliable() )
					Host_Error( "Reliable message (type %i) discarded.", msg.GetType() );

				continue;
			}

			CBaseClient *cl = m_Clients[ index - 1 ];

			if ( !cl->IsSpawned() )
			{
				continue;
			}

			if ( !cl->SendNetMsg( msg ) )
			{
				if ( msg.IsReliable() )
				{
					DevMsg( "BroadcastMessage: Reliable filter message overflow for client %s", cl->GetClientName() );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Writes events to the client's network buffer
// Input  : *cl -
//			*pack -
//			*msg -
//-----------------------------------------------------------------------------
static ConVar sv_debugtempentities( "sv_debugtempentities", "0", 0, "Show temp entity bandwidth usage." );

static bool CEventInfo_LessFunc( CEventInfo * const &lhs, CEventInfo * const &rhs )
{
	return lhs->classID < rhs->classID;
}

void CBaseServer::WriteTempEntities( CBaseClient *client, CFrameSnapshot *pCurrentSnapshot, CFrameSnapshot *pLastSnapshot, bf_write &buf, int ev_max )
{
	char data[NET_MAX_PAYLOAD];
	SVC_TempEntities msg;
	msg.m_DataOut.StartWriting( data, sizeof(data) );
	bf_write &buffer = msg.m_DataOut; // shortcut
	
	CFrameSnapshot *pSnapshot;
	CEventInfo *pLastEvent = NULL;

	bool bDebug = sv_debugtempentities.GetBool();

	// limit max entities to field bit length
	ev_max = min( ev_max, ((1<<CEventInfo::EVENT_INDEX_BITS)-1) );

	if ( pLastSnapshot )
	{
		pSnapshot = pLastSnapshot->NextSnapshot();
	} 
	else
	{
		pSnapshot = pCurrentSnapshot;
	}

	CUtlRBTree< CEventInfo * >	sorted( 0, ev_max, CEventInfo_LessFunc );

	// Build list of events sorted by send table classID (makes the delta work better in cases with a lot of the same message type )
	while ( pSnapshot && ((int)sorted.Count() < ev_max) )
	{
		for( int i = 0; i < pSnapshot->m_nTempEntities; ++i )
		{
			CEventInfo *event = pSnapshot->m_pTempEntities[ i ];

			if ( client->IgnoreTempEntity( event ) )
				continue; // event is not seen by this player
			
			sorted.Insert( event );
			// More space still
			if ( (int)sorted.Count() >= ev_max )
				break;	
		}
		
		// stop, we reached our current snapshot
		if ( pSnapshot == pCurrentSnapshot )
			break; 

		// got to next snapshot
		pSnapshot = framesnapshotmanager->NextSnapshot( pSnapshot );
	}

	if ( sorted.Count() <= 0 )
		return;

	for ( int i = sorted.FirstInorder(); 
		i != sorted.InvalidIndex(); 
		i = sorted.NextInorder( i ) )
	{
		CEventInfo *event = sorted[ i ];

		if ( event->fire_delay == 0.0f )
		{
			buffer.WriteOneBit( 0 );
		} 
		else
		{
			buffer.WriteOneBit( 1 );
			buffer.WriteSBitLong( event->fire_delay*100.0f, 8 );
		}

		if ( pLastEvent && 
			pLastEvent->classID == event->classID )
		{
			buffer.WriteOneBit( 0 ); // delta against last temp entity

			int startBit = bDebug ? buffer.GetNumBitsWritten() : 0;

			SendTable_WriteAllDeltaProps( event->pSendTable, 
				pLastEvent->pData,
				pLastEvent->bits,
				event->pData,
				event->bits,
				-1,
				&buffer );

			if ( bDebug )
			{
				int length = buffer.GetNumBitsWritten() - startBit;
				DevMsg("TE %s delta bits: %i\n", event->pSendTable->GetName(), length );
			}
		}
		else
		{
			 // full update, just compressed against zeros in MP

			buffer.WriteOneBit( 1 );

			int startBit = bDebug ? buffer.GetNumBitsWritten() : 0;

			buffer.WriteUBitLong( event->classID, GetClassBits() );

			if ( IsMultiplayer() )
			{
				SendTable_WriteAllDeltaProps( event->pSendTable, 
					NULL,	// will write only non-zero elements
					0,
					event->pData,
					event->bits,
					-1,
					&buffer );
			}
			else
			{
				// write event with zero properties
				buffer.WriteBits( event->pData, event->bits );
			}

			if ( bDebug )
			{
				int length = buffer.GetNumBitsWritten() - startBit;
				DevMsg("TE %s full bits: %i\n", event->pSendTable->GetName(), length );
			}
		}

		if ( IsMultiplayer() )
		{
			// in single player, don't used delta compression, lastEvent remains NULL
			pLastEvent = event;
		}
	}

	// set num entries
	msg.m_nNumEntries = sorted.Count();
	msg.WriteToBuffer( buf );
}

void CBaseServer::SetMaxClients( int number )
{
	m_nMaxclients = clamp( number, 1, ABSOLUTE_PLAYER_LIMIT );
}

struct convar_tags_t
{
	const char *pszConVar;
	const char *pszTag;
};
// The list of convars that automatically turn on tags when they're changed.
// Convars in this list need to have the FCVAR_NOTIFY flag set on them, so the
// tags are recalculated and uploaded to the master server when the convar is changed.
convar_tags_t convars_to_check_for_tags[] =
{
	{ "mp_friendlyfire", "friendlyfire" },
	{ "mp_stalemate_enable", "suddendeath" },
	{ "sv_gravity", "gravity" },
	{ "tf_birthday", "birthday" },
	{ "mp_respawnwavetime", "respawntimes" },
	{ "sv_alltalk", "alltalk" },
	{ "mp_fadetoblack", "fadetoblack" },
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseServer::RecalculateTags( void )
{
	// We're going to modify the sv_tags convar here, which will cause this to be called again. Prevent recursion.
	static bool bRecalculatingTags = false;
	if ( bRecalculatingTags )
		return;

	bRecalculatingTags = true;

	// Check convars first
	for ( int i = 0; i < ARRAYSIZE(convars_to_check_for_tags); i++ )
	{
		ConVar *pConVar = g_pCVar->FindVar( convars_to_check_for_tags[i].pszConVar );
		if ( pConVar )
		{
			const char *pszDef = pConVar->GetDefault();
			const char *pszCur = pConVar->GetString();
			if ( Q_strcmp( pszDef, pszCur ) )
			{
				AddTag( convars_to_check_for_tags[i].pszTag );
			}
			else
			{
				RemoveTag( convars_to_check_for_tags[i].pszTag );
			}
		}
	}

	// Check maxplayers
	int minmaxplayers = 1;
	int maxmaxplayers = ABSOLUTE_PLAYER_LIMIT;
	int defaultmaxplayers = 1;
	serverGameClients->GetPlayerLimits( minmaxplayers, maxmaxplayers, defaultmaxplayers );
	unsigned short nMaxReportedClients = GetMaxClients();
	if ( sv_visiblemaxplayers.GetInt() > 0 && sv_visiblemaxplayers.GetInt() < GetMaxClients() )
	{
		nMaxReportedClients = sv_visiblemaxplayers.GetInt();
	}
	if ( nMaxReportedClients > maxmaxplayers )
	{
		AddTag( "increased_maxplayers" );
	}
	else
	{
		RemoveTag( "increased_maxplayers" );
	}

	bRecalculatingTags = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseServer::AddTag( const char *pszTag )
{
	CUtlVector<char*> TagList;
	V_SplitString( sv_tags.GetString(), ",", TagList );
	for ( int i = 0; i < TagList.Count(); i++ )
	{
		// Already in the tag list?
		if ( !Q_stricmp(TagList[i],pszTag) )
			return;
	}

	// Append it
	char tmptags[MAX_TAG_STRING_LENGTH];
	tmptags[0] = '\0';
	Q_strncpy( tmptags, pszTag, MAX_TAG_STRING_LENGTH );
	Q_strncat( tmptags, ",", MAX_TAG_STRING_LENGTH );
	Q_strncat( tmptags, sv_tags.GetString(), MAX_TAG_STRING_LENGTH );
	sv_tags.SetValue( tmptags );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CBaseServer::RemoveTag( const char *pszTag )
{
	const char *pszTags = sv_tags.GetString();
	if ( !pszTags || !pszTags[0] )
		return;

	char tmptags[MAX_TAG_STRING_LENGTH];
	tmptags[0] = '\0';

	CUtlVector<char*> TagList;
	bool bFoundIt = false;
	V_SplitString( sv_tags.GetString(), ",", TagList );
	for ( int i = 0; i < TagList.Count(); i++ )
	{
		// Keep any tags other than the specified one
		if ( Q_stricmp(TagList[i],pszTag) )
		{
			Q_strncat( tmptags, TagList[i], MAX_TAG_STRING_LENGTH );
			Q_strncat( tmptags, ",", MAX_TAG_STRING_LENGTH );
		}
		else
		{
			bFoundIt = true;
		}
	}

	// Didn't find it in our list?
	if ( !bFoundIt )
		return;

	sv_tags.SetValue( tmptags );
}
