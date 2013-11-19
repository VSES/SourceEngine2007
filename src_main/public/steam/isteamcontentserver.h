//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: interface to steam for game servers
//
//=============================================================================

#ifndef ISTEAMCONTENTSERVER_H
#define ISTEAMCONTENTSERVER_H
#ifdef _WIN32
#pragma once
#endif


//-----------------------------------------------------------------------------
// Purpose: Functions for authenticating users via Steam to download content
//-----------------------------------------------------------------------------
class ISteamContentServer
{
public:
	// connection functions
	virtual void LogOn( uint32 uContentServerID ) = 0;
	virtual void LogOff() = 0;
	virtual bool BLoggedOn() = 0;

	// user authentication functions
	// the IP address and port should be in host order, i.e 127.0.0.1 == 0x7f000001
	virtual bool SendClientContentAuthRequest( CSteamID steamID, uint32 uContentID ) = 0;
	
	// some sort of status stuff here eventually
	
};

#define STEAMCONTENTSERVER_INTERFACE_VERSION "SteamContentServer001"


// callbacks


// client has been approved to download the content
struct CSClientApprove_t
{
	enum { k_iCallback = k_iSteamContentServerCallbacks + 1 };
	CSteamID m_SteamID;
	uint32 m_uContentID;
};


// client has been denied to connection to this game server
struct CSClientDeny_t
{
	enum { k_iCallback = k_iSteamContentServerCallbacks + 2 };
	CSteamID m_SteamID;
	uint32 m_uContentID;
	EDenyReason m_eDenyReason;
};


#endif // ISTEAMGAMESERVER_H
