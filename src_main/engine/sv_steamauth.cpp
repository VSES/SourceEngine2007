//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: steam state machine that handles authenticating steam users
//
//===========================================================================//

#ifdef _WIN32
#if !defined( _X360 )
#include "winlite.h"
#include <winsock2.h> // INADDR_ANY defn
#endif
#elif _LINUX
#include <netinet/in.h>
#endif

#include "sv_steamauth.h"
#include "sv_filter.h"
#include "inetchannel.h"
#include "netadr.h"
#include "server.h"
#include "proto_oob.h"
#ifndef SWDS
#include "steam.h"
#endif
#include "host.h"
#include "tier0/vcrmode.h"
#include "sv_plugin.h"
#include "sv_log.h"
#include "filesystem_engine.h"
#include "filesystem_init.h"
#include "tier0/icommandline.h"
#include "steam/steam_gameserver.h"
#include "sv_master_legacy.h"
#include "hltvserver.h"

extern ConVar sv_lan;

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern int g_iSteamAppID;
extern char gpszVersionString[32];
#pragma warning( disable: 4355 ) // disables ' 'this' : used in base member initializer list'

ConVar sv_master_share_game_socket( "sv_master_share_game_socket", "1", 0, 
	"Use the game's socket to communicate to the master server. "
	"If this is 0, then it will create a socket on -steamport + 1 "
	"to communicate to the master server on." );


//-----------------------------------------------------------------------------
// Purpose: singleton accessor
//-----------------------------------------------------------------------------
static CSteam3 s_Steam3Server;
CSteam3  &Steam3Server()
{
	return s_Steam3Server;
}


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CSteam3::CSteam3() 
#if !defined(NO_STEAM)
:
	m_CallbackLogonSuccess( this, &CSteam3::OnLogonSuccess ),
	m_CallbackLogonFailure( this, &CSteam3::OnLogonFailure ),
	m_CallbackLoggedOff( this, &CSteam3::OnLoggedOff ),
	m_CallbackBeginLogonRetry( this, &CSteam3::OnBeginLogonRetry ),
	m_CallbackGSClientApprove( this, &CSteam3::OnGSClientApprove ),
	m_CallbackGSClientDeny( this, &CSteam3::OnGSClientDeny ),
	m_CallbackGSClientKick( this, &CSteam3::OnGSClientKick ),
	m_CallbackGSClientSteam2Deny( this, &CSteam3::OnGSClientSteam2Deny ),
	m_CallbackGSClientSteam2Accept( this, &CSteam3::OnGSClientSteam2Accept ),
	m_CallbackGSPolicyResponse( this, &CSteam3::OnGSPolicyResponse )
#endif
{
	m_bHasActivePlayers = false;
	m_bLogOnResult = false;
	m_eServerMode = (EServerMode)0;
    m_bWantsSecure = false;		// default to insecure currently, this may change
    m_bInitialized = false;
	m_bLogOnFinished = false;
	m_bMasterServerUpdaterSharingGameSocket = false;
	m_steamIDLanOnly.InstancedSet( 0,0, k_EUniversePublic, k_EAccountTypeInvalid );
	m_SteamIDGS.InstancedSet( 1, 0, k_EUniverseInvalid, k_EAccountTypeInvalid );
	m_QueryPort = 0;
}

//-----------------------------------------------------------------------------
// Purpose: detect current server mode based on cvars & settings
//-----------------------------------------------------------------------------
EServerMode CSteam3::GetCurrentServerMode()
{
	if ( sv_lan.GetBool() )
	{
		return eServerModeNoAuthentication;
	}
	else if ( CommandLine()->FindParm( "-insecure" ) )
	{
		return eServerModeAuthentication;
	}
	else 
	{
		return eServerModeAuthenticationAndSecure;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CSteam3::~CSteam3()
{
	Shutdown();
}


void CSteam3::Activate()
{
	// we are active, check if sv_lan changed
	if ( GetCurrentServerMode() == m_eServerMode )
	{
		// we are active and LANmode didnt change. done.
		return;
	}

	if ( BIsActive() )
	{
		// shut down before we change server mode
		Shutdown();
	}

	m_unIP = INADDR_ANY;
	m_usPort = 26900;

	if ( CommandLine()->FindParm( "-steamport" ) )
	{
		m_usPort = CommandLine()->ParmValue( "-steamport", 26900 );
	}

	ConVarRef ipname( "ip" );
	if ( ipname.IsValid() )
	{
		netadr_t ipaddr;
		NET_StringToAdr( ipname.GetString(), &ipaddr );
		if ( !ipaddr.IsLoopback() && !ipaddr.IsLocalhost() )
		{
			m_unIP = htonl( ipaddr.GetIP() );
		}
	}

	m_eServerMode = GetCurrentServerMode();

	char gamedir[MAX_OSPATH];
	Q_FileBase( com_gamedir, gamedir, sizeof( gamedir ) );

	// Figure out the game port. If we're doing a SrcTV relay, then ignore the NS_SERVER port and don't tell Steam that we have a game server.
	uint16 usGamePort;
	if ( hltv && hltv->IsTVRelay() )
		usGamePort = 0;
	else
		usGamePort = NET_GetUDPPort( NS_SERVER );
	
	uint16 usSpectatorPort = NET_GetUDPPort( NS_HLTV );
	if ( !hltv )
		usSpectatorPort = 0;
	
	uint16 usMasterServerUpdaterPort;
	if ( IsUsingMasterLegacyMode() || sv_master_share_game_socket.GetBool() )
	{
		m_bMasterServerUpdaterSharingGameSocket = true;
		usMasterServerUpdaterPort = MASTERSERVERUPDATERPORT_USEGAMESOCKETSHARE;
		if ( sv.IsActive() )
			m_QueryPort = usGamePort;
		else
			m_QueryPort = usSpectatorPort;
	}
	else
	{
		m_bMasterServerUpdaterSharingGameSocket = false;
		usMasterServerUpdaterPort = m_usPort;
		m_QueryPort = m_usPort;
	}
#ifndef NO_STEAM
	if ( !SteamGameServer_Init( 
			m_unIP, 
			m_usPort+1,	// Steam lives on -steamport + 1, master server updater lives on -steamport.
			usGamePort, 
			usSpectatorPort,
			usMasterServerUpdaterPort, 
			m_eServerMode, 
			g_iSteamAppID, 
			gamedir, 
			gpszVersionString ) )
	{
		Warning( "*********************************************************\n" );
		Warning( "*\tUnable to load Steam support library.*\n" );
		Warning( "*\tThis server will operate in LAN mode only.*\n" );
		Warning( "*********************************************************\n" );
		m_eServerMode = eServerModeNoAuthentication;
		sv_lan.SetValue( true );
		return;
	}
#endif
	SendUpdatedServerDetails();
}


//-----------------------------------------------------------------------------
// Purpose: game server stopped, shutdown Steam game server session
//-----------------------------------------------------------------------------
void CSteam3::Shutdown()
{
	if ( !BIsActive() )
		return;
#ifndef NO_STEAM
	SteamGameServer_Shutdown();
#endif
	m_bHasActivePlayers = false;
	m_bLogOnResult = false;
	m_SteamIDGS = k_steamIDNotInitYetGS;
	m_eServerMode = (EServerMode)0;
}


//-----------------------------------------------------------------------------
// Purpose: returns true if the userid's are the same
//-----------------------------------------------------------------------------
bool CSteam3::CompareUserID( const USERID_t & id1, const USERID_t & id2 )
{
	if ( id1.idtype != id2.idtype )
		return false;

	switch ( id1.idtype )
	{
	case IDTYPE_STEAM:
	case IDTYPE_VALVE:
		{
			return ( id1.uid.steamid.m_SteamInstanceID==id2.uid.steamid.m_SteamInstanceID && 
					 id1.uid.steamid.m_SteamLocalUserID.As64bits == id2.uid.steamid.m_SteamLocalUserID.As64bits);
		}
	default:
		break;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Purpose: returns true if this userid is already on this server
//-----------------------------------------------------------------------------
bool CSteam3::CheckForDuplicateSteamID( const CBaseClient *client )
{
	// in LAN mode we allow reuse of SteamIDs
	if ( BLanOnly() ) 
		return false;

	// Compare connecting client's ID to other IDs on the server
	for ( int i=0 ; i< sv.GetClientCount() ; i++ )
	{
		const IClient *cl = sv.GetClient( i );

		// Not connected, no SteamID yet
		if ( !cl->IsConnected() || cl->IsFakeClient() )
			continue;

		if ( cl->GetNetworkID().idtype != IDTYPE_STEAM )
			continue;

		// don't compare this client against himself in the list
		if ( client == cl )
			continue;

		if ( !CompareUserID( client->GetNetworkID(), cl->GetNetworkID() ) )
			continue;

		// SteamID is reused
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Called when secure policy is set
//-----------------------------------------------------------------------------
CSteamID CSteam3::GetGSSteamID()
{
	if ( BLanOnly() )
	{
		// return special LAN mode SteamID
		return k_steamIDLanModeGS; 
	}
	else
	{
		return m_SteamIDGS;
	}
}


#if !defined(NO_STEAM)
//-----------------------------------------------------------------------------
// Purpose: Called when spectator port changes
//-----------------------------------------------------------------------------
void CSteam3::UpdateSpectatorPort( unsigned short unSpectatorPort )
{
	ISteamGameServer *pGameServer = SteamGameServer();
	if ( pGameServer )
		pGameServer->GSUpdateSpectatorPort( unSpectatorPort );
}


//-----------------------------------------------------------------------------
// Purpose: Called when secure policy is set
//-----------------------------------------------------------------------------
void CSteam3::OnGSPolicyResponse( GSPolicyResponse_t *pPolicyResponse )
{
	if ( !BIsActive() )
		return;

	if ( SteamGameServer() && SteamGameServer()->BSecure() )
	{
		Msg( "   VAC secure mode is activated.\n" );
	}
	else
	{
		Msg( "   VAC secure mode disabled.\n" );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSteam3::OnLogonSuccess( LogonSuccess_t *pLogonSuccess )
{
	if ( !BIsActive() )
		return;

	if ( !m_bLogOnResult )
	{
		m_bLogOnResult = true;
		if ( !BLanOnly() )
		{
			Msg( "Connection to Steam servers successful.\n" );
		}
	}
	if ( SteamGameServer() )
	{
		m_SteamIDGS = SteamGameServer()->GetSteamID();
	}
	else
	{
		m_SteamIDGS = k_steamIDNotInitYetGS;
	}

	// send updated server details
	// OnLogonSuccess() gets called each time we logon, so if we get dropped this gets called
	// again and we get need to retell the AM our details
	SendUpdatedServerDetails();
}


//-----------------------------------------------------------------------------
// Purpose: callback on unable to connect to the steam3 backend
// Input  : eResult - 
//-----------------------------------------------------------------------------
void CSteam3::OnLogonFailure( LogonFailure_t *pLogonFailure )
{
	if ( !BIsActive() )
		return;

	if ( !m_bLogOnResult )
	{
		if ( pLogonFailure->m_eResult == k_EResultServiceUnavailable )
		{
			if ( !BLanOnly() )
			{
				Msg( "Connection to Steam servers successful (SU).\n" );
			}
		}
		else
		{
			// we tried to be in secure mode but failed
			// force into insecure mode
			// eventually change this to set sv_lan as well
			if (  !BLanOnly() )
			{
				Msg( "Could not establish connection to Steam servers.\n" );
			}
		}
	}

	m_bLogOnResult = true;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : eResult - 
//-----------------------------------------------------------------------------
void CSteam3::OnLoggedOff( LoggedOff_t *pLoggedOff )
{
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSteam3::OnBeginLogonRetry( BeginLogonRetry_t *pBeginLogonRetry )
{
}


//-----------------------------------------------------------------------------
// Purpose: Called when Steam2 denies a user connect
//-----------------------------------------------------------------------------
void CSteam3::OnGSClientSteam2Deny( GSClientSteam2Deny_t *pGSClientSteam2Deny )
{
	if ( !BIsActive() )
		return;

	CBaseClient *client = NULL;
	// Compare connecting client's ID to other IDs on the server
	for ( int i=0 ; i< sv.GetClientCount() ; i++ )
	{
		const IClient *cl = sv.GetClient( i );		
		if ( (uint32)cl->GetUserID() == pGSClientSteam2Deny->m_UserID )
		{
			client = (CBaseClient *)cl;
			break;
		}
	}

	if ( !client )
		return;
	
	OnGSClientDenyHelper( client, (EDenyReason)pGSClientSteam2Deny->m_eSteamError, NULL );
}


//-----------------------------------------------------------------------------
// Purpose: Called when Steam2 has processed a user login an accepts it (Steam3 login is still pending however)
//-----------------------------------------------------------------------------
void CSteam3::OnGSClientSteam2Accept( GSClientSteam2Accept_t *pGSClientSteam2Accept )
{
	if ( !BIsActive() )
		return;

	CBaseClient *client = NULL;
	// Compare connecting client's ID to other IDs on the server
	for ( int i=0 ; i< sv.GetClientCount() ; i++ )
	{
		const IClient *cl = sv.GetClient( i );	
		if ( (uint32)cl->GetUserID() == pGSClientSteam2Accept->m_UserID )
		{
			client = (CBaseClient *)cl;
			break;
		}
	}

	if ( !client )
		return;

	client->m_NetworkID.idtype		= IDTYPE_STEAM; 
	client->m_NetworkID.uid.steamid.m_SteamInstanceID = 1;
	client->SetSteamID( pGSClientSteam2Accept->m_SteamID );

	if ( Filter_IsUserBanned( client->GetNetworkID() ) )
	{
		sv.RejectConnection( client->GetNetChannel()->GetRemoteAddress(), "You have been banned from this server\n" );
		client->Disconnect( va( "STEAM UserID %s is banned", client->GetNetworkIDString() ) );
	}
	else if ( CheckForDuplicateSteamID( client ) )
	{
		client->Disconnect(  "STEAM UserID %s is already\nin use on this server", client->GetNetworkIDString() );					
	}
	else
	{
		char msg[ 512 ];
		sprintf( msg, "\"%s<%i><%s><>\" STEAM USERID validated\n", client->GetClientName(), client->GetUserID(), client->GetNetworkIDString() );

		DevMsg( "%s", msg );
		g_Log.Printf( "%s", msg );

		g_pServerPluginHandler->NetworkIDValidated( client->GetClientName(), client->GetNetworkIDString() );
	
		// Tell IServerGameClients if its version is high enough.
		if ( g_iServerGameClientsVersion >= 4 )
		{
			serverGameClients->NetworkIDValidated( client->GetClientName(), client->GetNetworkIDString() );
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : steamID - 
//-----------------------------------------------------------------------------
void CSteam3::OnGSClientApprove( GSClientApprove_t *pGSClientApprove )
{
	//Msg("Steam backend:Got approval for %x\n", pGSClientApprove->m_SteamID.ConvertToUint64() );
	// We got the approval message from the back end.
    // Note that if we dont get it, we default to approved anyway
	// dont need to send anything back

	CBaseClient *cl = ClientFindFromSteamID( pGSClientApprove->m_SteamID );
	if ( cl )
	{
		cl->SetFullyAuthenticated();
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : steamID - 
//			eDenyReason - 
//-----------------------------------------------------------------------------
void CSteam3::OnGSClientDeny( GSClientDeny_t *pGSClientDeny )
{
	if ( !BIsActive() )
		return;

	CBaseClient *cl = ClientFindFromSteamID( pGSClientDeny->m_SteamID );
	if ( !cl )
		return;

	OnGSClientDenyHelper( cl, pGSClientDeny->m_eDenyReason, pGSClientDeny->m_pchOptionalText );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : steamID - 
//			eDenyReason - 
//-----------------------------------------------------------------------------
void CSteam3::OnGSClientKick( GSClientKick_t *pGSClientKick )
{
	if ( !BIsActive() )
		return;

	CBaseClient *cl = ClientFindFromSteamID( pGSClientKick->m_SteamID );
	if ( cl )
	{
		// handle kick just like deny
		OnGSClientDenyHelper( cl, pGSClientKick->m_eDenyReason, NULL );
	}
}


//-----------------------------------------------------------------------------
// Purpose: helper for the two places that deny a user connect
// Input  : steamID - id to kick
//			eDenyReason - reason
//			pchOptionalText - some kicks also have a string with them
//-----------------------------------------------------------------------------
void CSteam3::OnGSClientDenyHelper( CBaseClient *cl, EDenyReason eDenyReason, const char *pchOptionalText )
{
	switch ( eDenyReason )
	{
		case k_EDenyInvalidVersion:
			cl->Disconnect( "Client version incompatible with server. \nPlease exit and restart" );
			break;
		case k_EDenyNotLoggedOn:
			if ( !BLanOnly() ) 
				cl->Disconnect( INVALID_STEAM_LOGON );
			break;
		case k_EDenyLoggedInElseWhere:
			if ( !BLanOnly() ) 
				cl->Disconnect( INVALID_STEAM_LOGGED_IN_ELSEWHERE );
			break;
		case k_EDenyNoLicense:
			cl->Disconnect( "This Steam account does not own this game. \nPlease login to the correct Steam account" );
			break;
		case k_EDenyCheater:
			if ( !BLanOnly() ) 
				cl->Disconnect( INVALID_STEAM_VACBANSTATE );
			break;
		case k_EDenyUnknownText:
			if ( pchOptionalText && pchOptionalText[0] )
			{
				cl->Disconnect( pchOptionalText );
			}
			else
			{
				cl->Disconnect( "Client dropped by server" );
			}
			break;
		case k_EDenyIncompatibleAnticheat:
			cl->Disconnect( "You are running an external tool that is\nincompatible with Secure servers" );
			break;
		case k_EDenyMemoryCorruption:
			cl->Disconnect( "Memory corruption detected" );
			break;
		case k_EDenyIncompatibleSoftware:
			cl->Disconnect( "You are running software that is\nnot compatible with Secure servers" );
			break;
		case k_EDenySteamConnectionLost:
			if ( !BLanOnly() ) 
				cl->Disconnect( INVALID_STEAM_LOGON );
			break;
		case k_EDenySteamConnectionError:
			if ( !BLanOnly() ) 
				cl->Disconnect( INVALID_STEAM_LOGON ); 
			break;
		case k_EDenySteamResponseTimedOut:
			cl->Disconnect( "Client timed out" );
			break;
		case k_EDenySteamValidationStalled:
			if ( BLanOnly() ) 
			{
				cl->m_NetworkID.uid.steamid.m_SteamInstanceID = 1;
				cl->m_NetworkID.uid.steamid.m_SteamLocalUserID.As64bits = 0 ;
				break; // allow lan only users in
			}
			else
			{
				cl->Disconnect( INVALID_STEAM_TICKET );
			}
			break;

		case k_EDenyGeneric:
		default:
			cl->Disconnect( "Client dropped by server" );
			break;
	}
}
#endif

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : steamIDFind - 
// Output : IClient
//-----------------------------------------------------------------------------
CBaseClient *CSteam3::ClientFindFromSteamID( CSteamID & steamIDFind )
{
	for ( int i=0 ; i< sv.GetClientCount() ; i++ )
	{
		CBaseClient *cl = (CBaseClient *)sv.GetClient( i );

		// Not connected, no SteamID yet
		if ( !cl->IsConnected() || cl->IsFakeClient() )
			continue;

		if ( cl->GetNetworkID().idtype != IDTYPE_STEAM )
			continue;

		USERID_t id = cl->GetNetworkID();
		CSteamID steamIDClient;
		steamIDClient.SetFromSteam2( &id.uid.steamid, steamIDFind.GetEUniverse() );
		if (steamIDClient == steamIDFind )
		{
			return cl;
		}
	}
	return NULL;
}


//-----------------------------------------------------------------------------
// Purpose: tell Steam that a new user connected
//-----------------------------------------------------------------------------
bool CSteam3::NotifyClientConnect( CBaseClient *client, uint32 unUserID, netadr_t & adr, const void *pvCookie, uint32 ucbCookie )
{
	if ( !BIsActive() ) 
		return true;

	if ( !client || client->IsFakeClient() )
		return false;
#ifndef NO_STEAM
	// Msg("S3: Sending client logon request for %x\n", steamIDClient.ConvertToUint64( ) );
	bool bRet = SteamGameServer()->GSSendUserConnect( unUserID, htonl( adr.GetIP() ), htons( adr.GetPort() ), pvCookie, ucbCookie );
#else
	bool bRet = false;
#endif
	SendUpdatedServerDetails();
	return bRet;
}

bool CSteam3::NotifyLocalClientConnect( CBaseClient *client )
{
#ifndef NO_STEAM
	CSteamID steamID;

	if ( SteamGameServer() && !SteamGameServer()->GSCreateUnauthenticatedUser( &steamID ) )
		return false;
	
	client->SetSteamID( steamID );
#endif
	SendUpdatedServerDetails();
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *client - 
//-----------------------------------------------------------------------------
void CSteam3::NotifyClientDisconnect( CBaseClient *client )
{
	if ( !client || !BIsActive() || !client->IsConnected() || client->IsFakeClient() )
		return;

	// Bots still get reported with GSCreateUnauthenticatedUser.
	if ( client->IsFakeClient() )
	{
#ifndef NO_STEAM
		if ( client->m_SteamID )
			SteamGameServer()->GSSendUserDisconnect( *client->m_SteamID, client->GetUserID() );
#endif
	}
	else
	{
		USERID_t id = client->GetNetworkID();
		if ( id.idtype != IDTYPE_STEAM )
			return;
#ifndef NO_STEAM
		CSteamID steamIDClient;
		steamIDClient.SetFromSteam2( &id.uid.steamid, SteamGameServerUtils()->GetConnectedUniverse() );
		// Msg("S3: Sending client disconnect for %x\n", steamIDClient.ConvertToUint64( ) );
		SteamGameServer()->GSSendUserDisconnect( steamIDClient, client->GetUserID() );
#endif
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSteam3::NotifyOfLevelChange()
{
	// we're changing levels, so we may not respond for a while
	if ( m_bHasActivePlayers )
	{
		m_bHasActivePlayers = false;
		SendUpdatedServerDetails();
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSteam3::NotifyOfServerNameChange()
{
	SendUpdatedServerDetails();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSteam3::RunFrame()
{
	bool bHasPlayers = ( sv.GetNumClients() > 0 );

	if ( m_bHasActivePlayers != bHasPlayers )
	{
		m_bHasActivePlayers = bHasPlayers;
		SendUpdatedServerDetails();
	}

	static double s_fLastRunCallback = 0.0f;
	double fCurtime = Plat_FloatTime();
	if ( fCurtime - s_fLastRunCallback > 0.1f )
	{
		s_fLastRunCallback = fCurtime;
#ifndef NO_STEAM
		SteamGameServer_RunCallbacks();
#endif
	}
}


//-----------------------------------------------------------------------------
// Purpose: lets the steam3 servers know our full details
// Input  : bChangingLevels - true if we're going to heartbeat slowly for a while
//-----------------------------------------------------------------------------
void CSteam3::SendUpdatedServerDetails()
{
	if ( !BIsActive() )
		return;
#ifndef NO_STEAM
	SteamGameServer()->GSUpdateStatus(
								sv.GetNumClients(), 
								sv.GetMaxClients(), 
								sv.GetNumFakeClients(), 
								sv.GetName(),
								hltv ? hltv->GetName() : sv.GetName(),
								(hltv && hltv->IsTVRelay()) ? hltv->GetMapName() : sv.GetMapName() 
								);
#endif
}

bool CSteam3::IsMasterServerUpdaterSharingGameSocket()
{
	return m_bMasterServerUpdaterSharingGameSocket;
}
