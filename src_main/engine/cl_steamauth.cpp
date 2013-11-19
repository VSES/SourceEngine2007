//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: steam state machine that handles authenticating steam users
//
//=============================================================================//
#ifdef _WIN32
#if !defined( _X360 )
#include "winlite.h"
#include <winsock2.h> // INADDR_ANY defn
#endif
#elif _LINUX
#include <netinet/in.h>
#endif

#include "cl_steamauth.h"
#include "interface.h"
#include "filesystem_engine.h"
#include "tier0/icommandline.h"
#include "tier0/vprof.h"
#include "host.h"
#include "cmd.h"
#include "common.h"
#ifndef SWDS
#include "vgui_baseui_interface.h"
#endif

#pragma warning( disable: 4355 ) // disables ' 'this' : used in base member initializer list'

extern int g_iSteamAppID;

//-----------------------------------------------------------------------------
// Purpose: singleton accessor
//-----------------------------------------------------------------------------
static CSteam3Client s_Steam3Client;
CSteam3Client  &Steam3Client()
{
	return s_Steam3Client;
}


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CSteam3Client::CSteam3Client() 
#if !defined(NO_STEAM)
:
			m_CallbackClientGameServerDeny( this, &CSteam3Client::OnClientGameServerDeny ),
			m_CallbackGameServerChangeRequested( this, &CSteam3Client::OnGameServerChangeRequested ),
			m_CallbackGameOverlayActivated( this, &CSteam3Client::OnGameOverlayActivated ),
			m_CallbackPersonaStateChanged( this, &CSteam3Client::OnPersonaUpdated )
#endif
{
	m_bActive = false;
	m_bGSSecure = false;

}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CSteam3Client::~CSteam3Client()
{
	Shutdown();
}


//-----------------------------------------------------------------------------
// Purpose: Unload the steam3 engine
//-----------------------------------------------------------------------------
void CSteam3Client::Shutdown()
{	
	if ( !m_bActive )
		return;

	m_bActive = false;	
#if !defined( NO_STEAM )
	SteamAPI_Shutdown();
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Initialize the steam3 connection
//-----------------------------------------------------------------------------
void CSteam3Client::Activate()
{
	if ( m_bActive )
		return;

	m_bActive = true;
	m_bGSSecure = false;

#if !defined( NO_STEAM )
	SteamAPI_Init(); // ignore failure, that will fall out later when they don't get a valid logon cookie
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Get the steam3 logon cookie to use
//-----------------------------------------------------------------------------
int CSteam3Client::InitiateConnection( void *pData, int cbMaxData, uint32 unIP, uint16 usPort, uint64 unGSSteamID, bool bSecure, void *pvSteam2GetEncryptionKey, int cbSteam2GetEncryptionKey )
{
	m_bGSSecure = bSecure;
#if !defined( NO_STEAM )
	if ( !SteamUser() )
		return 0;

	return SteamUser()->InitiateGameConnection( pData, cbMaxData, unGSSteamID, CGameID( g_iSteamAppID ), ntohl( unIP ), usPort, bSecure, pvSteam2GetEncryptionKey, cbSteam2GetEncryptionKey ); // port is already in host order
#else
	return 0;
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Tell steam that we are leaving a server
//-----------------------------------------------------------------------------
void CSteam3Client::TerminateConnection( uint32 unIP, uint16 usPort )
{
	m_bGSSecure = false;


#if !defined( NO_STEAM )
	if ( !SteamUser() )
		return;
	SteamUser()->TerminateGameConnection( ntohl( unIP ), usPort );
#endif
}


//-----------------------------------------------------------------------------
// Purpose: Process any callbacks we may have
//-----------------------------------------------------------------------------
void CSteam3Client::RunFrame()
{
#if !defined( NO_STEAM )
	VPROF_BUDGET( "CSteam3Client::RunFrame", VPROF_BUDGETGROUP_STEAM );
	SteamAPI_RunCallbacks();
#endif
}


#if !defined(NO_STEAM)
//-----------------------------------------------------------------------------
// Purpose: Disconnect the user from their current server
//-----------------------------------------------------------------------------
void CSteam3Client::OnClientGameServerDeny( ClientGameServerDeny_t *pClientGameServerDeny )
{
	if ( pClientGameServerDeny->m_uAppID == (uint32)g_iSteamAppID )
	{
		const char *pszReason = "Unknown";
		switch ( pClientGameServerDeny->m_uReason )
		{
			case ( k_EDenyInvalidVersion ) : pszReason = "Invalid version"; break;
			case ( k_EDenyGeneric ) : pszReason = "Kicked"; break;
			case ( k_EDenyNotLoggedOn ) : pszReason = "Not logged on"; break;
			case ( k_EDenyNoLicense ) : pszReason = "No license"; break;
			case ( k_EDenyCheater ) : pszReason = "VAC banned "; break;
			case ( k_EDenyLoggedInElseWhere ) : pszReason = "Dropped from server"; break;
			case ( k_EDenyUnknownText ) : pszReason = "Unknown"; break;
			case ( k_EDenyIncompatibleAnticheat ) : pszReason = "Incompatible Anti Cheat"; break;
			case ( k_EDenyMemoryCorruption ) : pszReason = "Memory corruption"; break;
			case ( k_EDenyIncompatibleSoftware ) : pszReason = "Incompatible software"; break;
			case ( k_EDenySteamConnectionLost ) : pszReason = "Steam connection lost"; break;
			case ( k_EDenySteamConnectionError ) : pszReason = "Steam connection error"; break;
			case ( k_EDenySteamResponseTimedOut ) : pszReason = "Response timed out"; break;
			case ( k_EDenySteamValidationStalled ) : pszReason = "Verification failed"; break;
		}

		Warning( "Disconnect: %s\n", pszReason );

		Host_Disconnect( true );
	}
	
}


extern ConVar	password;
//-----------------------------------------------------------------------------
// Purpose: Disconnect the user from their current server
//-----------------------------------------------------------------------------
void CSteam3Client::OnGameServerChangeRequested( GameServerChangeRequested_t *pGameServerChangeRequested )
{
	password.SetValue( pGameServerChangeRequested->m_rgchPassword );
	Msg( "Connecting to %s\n", pGameServerChangeRequested->m_rgchServer );
	Cbuf_AddText( va( "connect %s\n", pGameServerChangeRequested->m_rgchServer ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSteam3Client::OnGameOverlayActivated( GameOverlayActivated_t *pGameOverlayActivated )
{
	if ( Host_IsSinglePlayerGame() )
	{
		if ( pGameOverlayActivated->m_bActive )
		{
			Cbuf_AddText( "setpause" );
		}
		else
		{
#ifndef SWDS
			if ( !EngineVGui()->IsGameUIVisible() && 
				 !EngineVGui()->IsConsoleVisible() )
#endif
			{
				Cbuf_AddText( "unpause" );
			}
		}
	}
}

extern void UpdateNameFromSteamID( IConVar *pConVar, CSteamID *pSteamID );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CSteam3Client::OnPersonaUpdated( PersonaStateChange_t *pPersonaStateChanged )
{
	if ( pPersonaStateChanged->m_nChangeFlags & k_EPersonaChangeName )
	{
		if ( SteamUtils() && SteamFriends() && SteamUser() )
		{
			CSteamID steamID = SteamUser()->GetSteamID();
			IConVar *pConVar = g_pCVar->FindVar( "name" );
			UpdateNameFromSteamID( pConVar, &steamID );
		}
	}
}
#endif
