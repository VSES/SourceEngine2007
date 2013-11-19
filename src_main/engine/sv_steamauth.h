//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: steam state machine that handles authenticating steam users
//
//=============================================================================//

#ifndef STEAMUAUTHSERVER_H
#define STEAMUAUTHSERVER_H
#ifdef _WIN32
#pragma once
#endif

#include "baseclient.h"
#include "utlvector.h"
#include "netadr.h"

#include "steam/steam_gameserver.h"

class CSteam3
{
public:
	CSteam3();
	~CSteam3();
#if !defined(NO_STEAM)
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnLogonSuccess, LogonSuccess_t, m_CallbackLogonSuccess );
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnLogonFailure, LogonFailure_t, m_CallbackLogonFailure );
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnLoggedOff, LoggedOff_t, m_CallbackLoggedOff );
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnBeginLogonRetry, BeginLogonRetry_t, m_CallbackBeginLogonRetry );
	// game server callbacks
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnGSClientApprove, GSClientApprove_t, m_CallbackGSClientApprove );
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnGSClientDeny, GSClientDeny_t, m_CallbackGSClientDeny );
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnGSClientKick, GSClientKick_t, m_CallbackGSClientKick );
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnGSClientSteam2Deny, GSClientSteam2Deny_t, m_CallbackGSClientSteam2Deny );
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnGSClientSteam2Accept, GSClientSteam2Accept_t, m_CallbackGSClientSteam2Accept );
	STEAM_GAMESERVER_CALLBACK( CSteam3, OnGSPolicyResponse, GSPolicyResponse_t, m_CallbackGSPolicyResponse );
#endif

	// CSteam3 stuff
  	void Activate();
	void NotifyOfLevelChange();
	void NotifyOfServerNameChange();
	void SendUpdatedServerDetails();
	void Shutdown();

	bool NotifyClientConnect( CBaseClient *client, uint32 unUserID, netadr_t & adr, const void *pvCookie, uint32 ucbCookie );
	bool NotifyLocalClientConnect( CBaseClient *client );	// Used for local player on listen server and bots.
	void NotifyClientDisconnect( CBaseClient *client );

	void RunFrame();
#ifndef NO_STEAM
	bool BSecure() { return SteamGameServer() && SteamGameServer()->BSecure(); }
	bool BIsActive() { return SteamGameServer() && ( m_eServerMode >= eServerModeNoAuthentication ); }
	bool BLanOnly() { return m_eServerMode == eServerModeNoAuthentication; }
	bool BWantsSecure() { return m_eServerMode == eServerModeAuthenticationAndSecure; }
	bool BLoggedOn() { return SteamGameServer() && SteamGameServer()->BLoggedOn(); }
#else
	bool BSecure() { return false; }
	bool BIsActive() { return m_eServerMode >= eServerModeNoAuthentication; }
	bool BLanOnly() { return m_eServerMode == eServerModeNoAuthentication; }
	bool BWantsSecure() { return m_eServerMode == eServerModeAuthenticationAndSecure; }
	bool BLoggedOn() { return true; }
#endif
	bool CompareUserID( const USERID_t & id1, const USERID_t & id2 );
	CSteamID GetGSSteamID();
	void UpdateSpectatorPort( unsigned short unSpectatorPort );
	
	uint16 GetQueryPort() const	{ return m_QueryPort; }
	
	bool IsMasterServerUpdaterSharingGameSocket();

private:
	
	bool CheckForDuplicateSteamID( const CBaseClient *client );
	CBaseClient *ClientFindFromSteamID( CSteamID & steamIDFind );
	void OnGSClientDenyHelper( CBaseClient *cl, EDenyReason eDenyReason, const char *pchOptionalText );
	EServerMode GetCurrentServerMode();

	EServerMode	m_eServerMode;

	bool m_bMasterServerUpdaterSharingGameSocket;
	bool m_bLogOnFinished;
	bool m_bLoggedOn;
	bool		m_bLogOnResult;		// if true, show logon result
	bool		m_bHasActivePlayers;  // player stats updates are only sent if active players are avaliable
	CSteamID m_SteamIDGS;
	CSteamID m_steamIDLanOnly;
	bool m_bActive;
    bool m_bWantsSecure;
    bool m_bInitialized;
    
    // The port that we are listening for queries on.
	uint32		m_unIP;
	uint16		m_usPort;
    uint16		m_QueryPort;
};

// singleton accessor
CSteam3 &Steam3Server();


#endif
