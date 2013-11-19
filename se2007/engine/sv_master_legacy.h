//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#ifndef SV_MASTER_LEGACY_H
#define SV_MASTER_LEGACY_H
#ifdef _WIN32
#pragma once
#endif

class CBaseServer;
class CGameServer;

//-----------------------------------------------------------------------------
// Purpose: Implements a master server interface.
//-----------------------------------------------------------------------------
abstract_class IMaster
{
public:
	// Allow master server to register cvars/commands
	virtual void Init( void ) = 0;
	// System is shutting down
	virtual void Shutdown( void ) = 0;
	// Called any time a master function might need to initialize the master system
	virtual void InitConnection( void ) = 0;		  
	// Server is shutting down
	virtual void ShutdownConnection( CBaseServer *pServer ) = 0;
	// Add server to global master list
	virtual bool AddServer( struct netadr_s *adr ) = 0;
	// Add server to global master list
	virtual bool RemoveServer( struct netadr_s *adr ) = 0;
	// If parsing for server, etc. fails, always have at least one server around to use.
	virtual void UseDefault ( void ) = 0;
	// See if it's time to send the next heartbeat
	virtual void CheckHeartbeat( CBaseServer *pServer ) = 0;
	// Master sent back a challenge value, read it and send the actual heartbeat
	virtual void SendHeartbeat( netadr_t &from, int challangenr, CBaseServer *pServer ) = 0;
	// Console command to set/remove master server
	virtual void SetMaster_Legacy_f( const CCommand &args ) = 0;
	// Force a heartbeat to be issued right away
	virtual void Heartbeat_Legacy_f( void ) = 0;

	// find out master server addrs and check challenge numbers
	virtual bool IsMasterServerAddress( netadr_t *from ) = 0;
	virtual bool IsCorrectMasterChallenge( netadr_t *from, int challenge, CBaseServer *pServer ) = 0;
	virtual void HandleUnknown( struct netpacket_s *packet, CBaseServer *pServer, CGameServer *pGameServer ) = 0;
	
	// returns true if we should issue a "quit" on level change (due to being out of date)
	virtual bool RestartOnLevelChange() = 0;
	// set the restart on level change flag
	virtual void SetRestartOnLevelChange(bool state) = 0;
};

extern IMaster *master;
extern bool IsUsingMasterLegacyMode(); // This is the on/off switch to enable all this legacy stuff (controlled by cvar sv_master_legacy_mode).

void SetMaster_Legacy_f();

#endif // SV_MASTER_LEGACY_H
