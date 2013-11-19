//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef BASECLIENT_H
#define BASECLIENT_H
#ifdef _WIN32
#pragma once
#endif

#include <const.h>
#include <checksum_crc.h>
#include <iclient.h>
#include <protocol.h>
#include <iservernetworkable.h>
#include <bspfile.h>
#include <KeyValues.h>
#include <bitvec.h>
#include <igameevents.h>
#include "smartptr.h"
#include "userid.h"
#include "tier1/bitbuf.h"

// class CClientFrame;
class CBaseServer;
class CClientFrame;
struct player_info_s;
class CFrameSnapshot;
class CEventInfo;
class CSteamID;

struct Spike_t
{
public:
	Spike_t() : 
		m_nBits( 0 )
	{
		m_szDesc[ 0 ] = 0;
	}
	char	m_szDesc[ 64 ];
	int		m_nBits;
};

class CNetworkStatTrace
{
public:
	CNetworkStatTrace() : 
		m_nMinWarningBytes( 0 ), m_nStartBit( 0 ), m_nCurBit( 0 )
	{
	}
	int						m_nMinWarningBytes;
	int						m_nStartBit;
	int						m_nCurBit;
	CUtlVector< Spike_t >	m_Records;
};


class CBaseClient : public IGameEventListener2, public IClient, public IClientMessageHandler
{
	typedef struct CustomFile_s
	{
		CRC32_t			crc;	//file CRC
		unsigned int	reqID;	// download request ID
	} CustomFile_t;

public:
	CBaseClient();
	virtual ~CBaseClient();

public:

	int			GetPlayerSlot() const { return m_nClientSlot; };
	int			GetUserID() const { return m_UserID; };
	const USERID_t		GetNetworkID() const { return m_NetworkID; };
	const char		*GetClientName() const { return m_Name; };
	INetChannel		*GetNetChannel() { return m_NetChannel; };
	IServer			*GetServer() { return (IServer*)m_Server; };
	const char		*GetUserSetting(const char *cvar) const;
	const char		*GetNetworkIDString() const;
	uint			GetFriendsID() const { return m_nFriendsID; }
	const char		*GetFriendsName() const { return m_FriendsName; }
	void			UpdateName( const char *pszDefault );

	virtual	void	Connect(const char * szName, int nUserID, INetChannel *pNetChannel, bool bFakePlayer);
	virtual	void	Inactivate( void );
	virtual	void	Reconnect( void );
	virtual	void	Disconnect( const char *reason, ... );

	virtual	void	SetRate( int nRate, bool bForce );
	virtual	int		GetRate( void ) const;
	
	virtual void	SetUpdateRate( int nUpdateRate, bool bForce );
	virtual int		GetUpdateRate( void ) const;

	virtual void	Clear( void );
	virtual void	DemoRestart( void ); // called when client started demo recording

	virtual	int		GetMaxAckTickCount() const { return max( m_nSignonTick, m_nDeltaTick ); };

	virtual bool	ExecuteStringCommand( const char *s );
	virtual bool	SendNetMsg(INetMessage &msg, bool bForceReliable = false);
	
	virtual void	ClientPrintf (const char *fmt, ...);

	virtual	bool	IsConnected( void ) const { return m_nSignonState >= SIGNONSTATE_CONNECTED; };	
	virtual	bool	IsSpawned( void ) const { return m_nSignonState >= SIGNONSTATE_NEW; };	
	virtual	bool	IsActive( void ) const { return m_nSignonState == SIGNONSTATE_FULL; };
	virtual	bool	IsFakeClient( void ) const { return m_bFakePlayer; };
	virtual	bool	IsHLTV( void ) const { return m_bIsHLTV; }
	virtual	bool	IsHearingClient( int index ) const { return false; };
	virtual	bool	IsProximityHearingClient( int index ) const { return false; };

	virtual void	SetMaxRoutablePayloadSize( int nMaxRoutablePayloadSize );

public: // IClientMessageHandlers
	
	PROCESS_NET_MESSAGE( Tick );
	PROCESS_NET_MESSAGE( StringCmd );
	PROCESS_NET_MESSAGE( SetConVar );
	PROCESS_NET_MESSAGE( SignonState );
	
	PROCESS_CLC_MESSAGE( ClientInfo );
	PROCESS_CLC_MESSAGE( BaselineAck );
	PROCESS_CLC_MESSAGE( ListenEvents );
	
	virtual void	ConnectionStart(INetChannel *chan);

public: // IGameEventListener
	virtual void	FireGameEvent( IGameEvent *event );

public:

	virtual	bool	UpdateAcknowledgedFramecount(int tick);
	virtual bool	ShouldSendMessages( void );
	virtual void	UpdateSendState( void );
			void	ForceFullUpdate( void ) { UpdateAcknowledgedFramecount(-1); }
	
	virtual bool	FillUserInfo( player_info_s &userInfo );
	virtual void	UpdateUserSettings();
	virtual bool	SetSignonState(int state, int spawncount);
	virtual void	WriteGameSounds(bf_write &buf);
	
	virtual CClientFrame *GetDeltaFrame( int nTick );
	virtual void	SendSnapshot( CClientFrame *pFrame );
	virtual bool	SendServerInfo( void );
	virtual bool	SendSignonData( void );
	virtual void	SpawnPlayer( void );
	virtual void	ActivatePlayer( void );
	virtual void	SetName( const char * name );
	virtual void	SetUserCVar( const char *cvar, const char *value);
	virtual void	FreeBaselines();
	virtual bool	IgnoreTempEntity( CEventInfo *event );
	
	void			SetSteamID( const CSteamID &steamID );

	bool			IsTracing() const;
	void			SetTraceThreshold( int nThreshold );
	void			TraceNetworkData( bf_write &msg, char const *fmt, ... );
	void			TraceNetworkMsg( int nBits, char const *fmt, ... );

	bool			IsFullyAuthenticated( void ) { return m_bFullyAuthenticated; }
	void			SetFullyAuthenticated( void ) { m_bFullyAuthenticated = true; }

private:	

	void			OnRequestFullUpdate();


public:

	// Array index in svs.clients:
	int				m_nClientSlot;	
	// entity index of this client (different from clientSlot+1 in HLTV mode):
	int				m_nEntityIndex;	
	
	int				m_UserID;			// identifying number on server
	char			m_Name[MAX_PLAYER_NAME_LENGTH];			// for printing to other people
	char			m_GUID[SIGNED_GUID_LEN + 1]; // the clients CD key

	USERID_t		m_NetworkID;         // STEAM assigned userID ( 0x00000000 ) if none
	CSteamID		*m_SteamID;			// This is allocated when the client is authenticated, and NULL until then.
	
	uint32			m_nFriendsID;		// client's friends' ID
	char			m_FriendsName[MAX_PLAYER_NAME_LENGTH];

	KeyValues		*m_ConVars;			// stores all client side convars
	bool			m_bConVarsChanged;	// true if convars updated and not changes process yet
	bool			m_bSendServerInfo;	// true if we need to send server info packet to start connect
	CBaseServer		*m_Server;			// pointer to server object
	bool			m_bIsHLTV;			// if this a HLTV proxy ?
	
	// Client sends this during connection, so we can see if
	//  we need to send sendtable info or if the .dll matches
	CRC32_t			m_nSendtableCRC;

	// a client can have couple of cutomized files distributed to all other players
	CustomFile_t	m_nCustomFiles[MAX_CUSTOM_FILES];
	int				m_nFilesDownloaded;	// counter of how many files we downloaded from this client

	//===== NETWORK ============
	INetChannel		*m_NetChannel;		// The client's net connection.
	int				m_nSignonState;		// connection state
	int				m_nDeltaTick;		// -1 = no compression.  This is where the server is creating the
										// compressed info from.
	int				m_nSignonTick;		// tick the client got his signon data
	CSmartPtr<CFrameSnapshot,CRefCountAccessorLongName> m_pLastSnapshot;	// last send snapshot

	CFrameSnapshot	*m_pBaseline;			// current entity baselines as a snapshot
	int				m_nBaselineUpdateTick;	// last tick we send client a update baseline signal or -1
	CBitVec<MAX_EDICTS>	m_BaselinesSent;	// baselines sent with last update
	int				m_nBaselineUsed;		// 0/1 toggling flag, singaling client what baseline to use
	
		
	// This is used when we send out a nodelta packet to put the client in a state where we wait 
	// until we get an ack from them on this packet.
	// This is for 3 reasons:
	// 1. A client requesting a nodelta packet means they're screwed so no point in deluging them with data.
	//    Better to send the uncompressed data at a slow rate until we hear back from them (if at all).
	// 2. Since the nodelta packet deletes all client entities, we can't ever delta from a packet previous to it.
	// 3. It can eat up a lot of CPU on the server to keep building nodelta packets while waiting for
	//    a client to get back on its feet.
	int				m_nForceWaitForTick;
	
	bool			m_bFakePlayer;		// JAC: This client is a fake player controlled by the game DLL
	bool		   m_bReceivedPacket;	// true, if client received a packet after the last send packet

	bool			m_bFullyAuthenticated;

	// The datagram is written to after every frame, but only cleared
	// when it is sent out to the client.  overflow is tolerated.

	// Time when we should send next world state update ( datagram )
	double         m_fNextMessageTime;   
	// Default time to wait for next message
	float          m_fSnapshotInterval;  

	enum
	{
		SNAPSHOT_SCRATCH_BUFFER_SIZE = 32768,
	};

	unsigned int		m_SnapshotScratchBuffer[ SNAPSHOT_SCRATCH_BUFFER_SIZE / 4 ];

private:
	void				StartTrace( bf_write &msg );
	void				EndTrace( bf_write &msg );


	CNetworkStatTrace	m_Trace;
};



#endif // BASECLIENT_H
