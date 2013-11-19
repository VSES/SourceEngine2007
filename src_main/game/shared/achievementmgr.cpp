//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"
#ifdef _WIN32
#include "winerror.h"
#endif
#include "achievementmgr.h"
#include "icommandline.h"
#include "KeyValues.h"
#include "filesystem.h"
#include "steam/steam_api.h"
#include "inputsystem/InputEnums.h"
#include "usermessages.h"
#include "fmtstr.h"
#ifdef CLIENT_DLL
#include "achievement_notification_panel.h"
#include "c_playerresource.h"
#else
#include "enginecallback.h"
#endif // CLIENT_DLL
#ifndef _X360
#include "steam/isteamuserstats.h"
#include "steam/isteamfriends.h"
#include "steam/isteamutils.h"
#else
#include "xbox/xbox_win32stubs.h"
#endif
#include "tier3/tier3.h"
#include "vgui/ILocalize.h"
#ifdef _X360
#include "ixboxsystem.h"
#endif  // _X360
#include "engine/imatchmaking.h"
#include "tier0/vprof.h"

ConVar	cc_achievement_debug("achievement_debug", "0", FCVAR_CHEAT | FCVAR_REPLICATED, "Turn on achievement debug msgs." );
const char *COM_GetModDirectory();

extern ConVar developer;


#ifdef SWDS
// Hack this for now until we get steam_api recompiling in the Steam codebase.
ISteamUserStats *SteamUserStats()
{
	return NULL;
}
#endif

//-----------------------------------------------------------------------------
// Purpose: Write helper
//-----------------------------------------------------------------------------
static void WriteAchievementGlobalState( KeyValues *pKV )
{
#ifdef _X360
	if ( XBX_GetStorageDeviceId() == XBX_INVALID_STORAGE_ID || XBX_GetStorageDeviceId() == XBX_STORAGE_DECLINED )
		return;
#endif

	char szFilename[_MAX_PATH];

	if ( IsX360() )
	{
		Q_snprintf( szFilename, sizeof( szFilename ), "cfg:/%s_GameState.txt", COM_GetModDirectory() );
	}
	else
	{
		Q_snprintf( szFilename, sizeof( szFilename ), "GameState.txt" );
	}

	pKV->SaveToFile( filesystem, szFilename, "MOD" );
	pKV->deleteThis();

#ifdef _X360
	if ( xboxsystem )
	{
		xboxsystem->FinishContainerWrites();
	}
#endif
}
#ifndef _LINUX

//-----------------------------------------------------------------------------
// Purpose: Async save thread
//-----------------------------------------------------------------------------
class CAchievementSaveThread : public CWorkerThread 
{
public:
	CAchievementSaveThread() :
	  m_pKV( NULL )
	  {
		  SetName( "AchievementSaveThread" );	
	  }

	  ~CAchievementSaveThread()
	  {
	  }

	  enum
	  {
		  CALL_FUNC,
		  EXIT,
	  };

	  void WriteAchievementGlobalState( KeyValues *pKV )
	  {
		  Assert( !m_pKV );
		  m_pKV = pKV;
		  CallWorker( CALL_FUNC );
		  Assert( !m_pKV );
	  }

	  int Run()
	  {
		  unsigned nCall;
		  while ( WaitForCall( &nCall ) )
		  {
			  if ( nCall == EXIT )
			  {
				  Reply( 1 );
				  break;
			  }

			  KeyValues *pKV = m_pKV;
			  m_pKV = NULL;
			  Reply( 1 );
			  ::WriteAchievementGlobalState( pKV );
		  }
		  return 0;
	  }

private:
	KeyValues *m_pKV;
};

static CAchievementSaveThread g_AchievementSaveThread;

#endif //_LINUX
//-----------------------------------------------------------------------------
// Purpose: constructor
//-----------------------------------------------------------------------------
CAchievementMgr::CAchievementMgr() : CAutoGameSystem( "CAchievementMgr" )
#if !defined(NO_STEAM)
, m_CallbackUserStatsReceived( this, &CAchievementMgr::Steam_OnUserStatsReceived ),
m_CallbackUserStatsStored( this, &CAchievementMgr::Steam_OnUserStatsStored )
#endif
{
	SetDefLessFunc( m_mapAchievement );
	m_flLastClassChangeTime = 0;
	m_flTeamplayStartTime = 0;
	m_iMiniroundsCompleted = 0;
	m_szMap[0] = 0;
	m_bDirty = false;
	m_bGlobalStateLoaded = false;

	m_AchievementsAwarded.RemoveAll();
}

//-----------------------------------------------------------------------------
// Purpose: Initializer
//-----------------------------------------------------------------------------
bool CAchievementMgr::Init()
{
	// We can be created on either client (for multiplayer games) or server
	// (for single player), so register ourselves with the engine so UI has a uniform place 
	// to go get the pointer to us

#ifdef _DEBUG
	// There can be only one achievement manager instance; no one else should be registered
	IAchievementMgr *pAchievementMgr = engine->GetAchievementMgr();
	Assert( NULL == pAchievementMgr );
#endif // _DEBUG

	// register ourselves
	engine->SetAchievementMgr( this );

	// register for events
#ifdef GAME_DLL
	ListenForGameEvent( "entity_killed" );
	ListenForGameEvent( "game_init" );
#else
	ListenForGameEvent( "player_death" );
	ListenForGameEvent( "player_stats_updated" );
	usermessages->HookMessage( "AchievementEvent", MsgFunc_AchievementEvent );
#endif // CLIENT_DLL

#ifdef TF_CLIENT_DLL
	ListenForGameEvent( "localplayer_changeclass" );
	ListenForGameEvent( "localplayer_changeteam" );
	ListenForGameEvent( "teamplay_round_start" );	
	ListenForGameEvent( "teamplay_round_win" );
#endif // TF_CLIENT_DLL

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: called at init time after all systems are init'd.  We have to
//			do this in PostInit because the Steam app ID is not available earlier
//-----------------------------------------------------------------------------
void CAchievementMgr::PostInit()
{
#ifndef _LINUX
	if ( !g_AchievementSaveThread.IsAlive() )
	{
		g_AchievementSaveThread.Start();
		if ( IsX360() )
		{
			ThreadSetAffinity( (ThreadHandle_t)g_AchievementSaveThread.GetThreadHandle(), XBOX_PROCESSOR_3 );
		}
	}
#endif //_LINUX

	// get current game dir
	const char *pGameDir = COM_GetModDirectory();

	CBaseAchievementHelper *pAchievementHelper = CBaseAchievementHelper::s_pFirst;
	while ( pAchievementHelper )
	{
		// create and initialize all achievements and insert them in our map
		CBaseAchievement *pAchievement = pAchievementHelper->m_pfnCreate();
		pAchievement->m_pAchievementMgr = this;
		pAchievement->Init();
		pAchievement->CalcProgressMsgIncrement();

		// only add an achievement if it does not have a game filter (only compiled into the game it
		// applies to, or truly cross-game) or, if it does have a game filter, the filter matches current game.
		// (e.g. EP 1/2/... achievements are in shared binary but are game specific, they have a game filter for runtime check.)
		const char *pGameDirFilter = pAchievement->m_pGameDirFilter;
		if ( !pGameDirFilter || ( 0 == Q_strcmp( pGameDir, pGameDirFilter ) ) )
		{
			m_mapAchievement.Insert( pAchievement->GetAchievementID(), pAchievement );
		}
		else
		{
			// achievement is not for this game, don't use it
			delete pAchievement;
		}

		pAchievementHelper = pAchievementHelper->m_pNext;
	}

	FOR_EACH_MAP( m_mapAchievement, iter )
	{
		m_vecAchievement.AddToTail( m_mapAchievement[iter] );
	}

	// load global state from file
	LoadGlobalState();

	// download achievements/stats from Steam/XBox Live
	DownloadUserData();

}

//-----------------------------------------------------------------------------
// Purpose: Shuts down the achievement manager
//-----------------------------------------------------------------------------
void CAchievementMgr::Shutdown()
{
#ifndef _LINUX
	g_AchievementSaveThread.CallWorker( CAchievementSaveThread::EXIT );
#endif

	SaveGlobalState();

	FOR_EACH_MAP( m_mapAchievement, iter )
	{
		delete m_mapAchievement[iter];
	}
	m_mapAchievement.RemoveAll();
	m_vecAchievement.RemoveAll();
	m_vecKillEventListeners.RemoveAll();
	m_vecMapEventListeners.RemoveAll();
	m_vecComponentListeners.RemoveAll();
	m_AchievementsAwarded.RemoveAll();
	m_bGlobalStateLoaded = false;
}

//-----------------------------------------------------------------------------
// Purpose: Cleans up all achievements and then re-initializes them
//-----------------------------------------------------------------------------
void CAchievementMgr::InitializeAchievements()
{
	Shutdown();
	PostInit();
}

//-----------------------------------------------------------------------------
// Purpose: called on level init
//-----------------------------------------------------------------------------
void CAchievementMgr::LevelInitPreEntity()
{
	// load global state if we haven't already; X360 users may not have had a storage device available or selected at boot time
	EnsureGlobalStateLoaded();

#ifdef GAME_DLL
	// For single-player games, achievement mgr must live on the server.  (Only the server has detailed knowledge of game state.)
	Assert( !GameRules()->IsMultiplayer() );	
#else
	// For multiplayer games, achievement mgr must live on the client.  (Only the client can read/write player state from Steam/XBox Live.)
	Assert( GameRules()->IsMultiplayer() );
#endif 

	// clear list of achievements listening for events
	m_vecKillEventListeners.RemoveAll();
	m_vecMapEventListeners.RemoveAll();
	m_vecComponentListeners.RemoveAll();

	m_AchievementsAwarded.RemoveAll();

	m_flLastClassChangeTime = 0;
	m_flTeamplayStartTime = 0;
	m_iMiniroundsCompleted = 0;

	// client and server have map names available in different forms (full path on client, just file base name on server), 
	// cache it in base file name form here so we don't have to have different code paths each time we access it
#ifdef CLIENT_DLL	
	Q_FileBase( engine->GetLevelName(), m_szMap, ARRAYSIZE( m_szMap ) );
#else
	Q_strncpy( m_szMap, gpGlobals->mapname.ToCStr(), ARRAYSIZE( m_szMap ) );
#endif // CLIENT_DLL

	if ( IsX360() )
	{
		// need to remove the .360 extension on the end of the map name
		char *pExt = Q_stristr( m_szMap, ".360" );
		if ( pExt )
		{
			*pExt = '\0';
		}
	}

	// look through all achievements, see which ones we want to have listen for events
	FOR_EACH_MAP( m_mapAchievement, iAchievement )
	{
		CBaseAchievement *pAchievement = m_mapAchievement[iAchievement];

		// if the achievement only applies to a specific map, and it's not the current map, skip it
		const char *pMapNameFilter = pAchievement->m_pMapNameFilter;
		if ( pMapNameFilter && ( 0 != Q_strcmp( m_szMap, pMapNameFilter ) ) )
			continue;

		// if the achievement needs kill events, add it as a listener
		if ( pAchievement->GetFlags() & ACH_LISTEN_KILL_EVENTS )
		{
			m_vecKillEventListeners.AddToTail( pAchievement );
		}
		// if the achievement needs map events, add it as a listener
		if ( pAchievement->GetFlags() & ACH_LISTEN_MAP_EVENTS )
		{
			m_vecMapEventListeners.AddToTail( pAchievement );
		}
		// if the achievement needs map events, add it as a listener
		if ( pAchievement->GetFlags() & ACH_LISTEN_COMPONENT_EVENTS )
		{
			m_vecComponentListeners.AddToTail( pAchievement );
		}
		if ( pAchievement->IsActive() )
		{
			pAchievement->ListenForEvents();
		}
	}

	m_flLevelInitTime = gpGlobals->curtime;
}


//-----------------------------------------------------------------------------
// Purpose: called on level shutdown
//-----------------------------------------------------------------------------
void CAchievementMgr::LevelShutdownPreEntity()
{
	// make all achievements stop listening for events 
	FOR_EACH_MAP( m_mapAchievement, iAchievement )
	{
		CBaseAchievement *pAchievement = m_mapAchievement[iAchievement];
		pAchievement->StopListeningForAllEvents();
	}

	// save global state if we have any changes
	SaveGlobalStateIfDirty();
}

//-----------------------------------------------------------------------------
// Purpose: returns achievement for specified ID
//-----------------------------------------------------------------------------
CBaseAchievement *CAchievementMgr::GetAchievementByID( int iAchievementID )
{
	int iAchievement = m_mapAchievement.Find( iAchievementID );
	if ( iAchievement != m_mapAchievement.InvalidIndex() )
	{
		return m_mapAchievement[iAchievement];
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: returns achievement with specified name.  NOTE: this iterates through
//			all achievements to find the name, intended for debugging purposes.
//			Use GetAchievementByID for fast lookup.
//-----------------------------------------------------------------------------
CBaseAchievement *CAchievementMgr::GetAchievementByName( const char *pchName )
{
	VPROF("GetAchievementByName");
	FOR_EACH_MAP_FAST( m_mapAchievement, i )
	{
		CBaseAchievement *pAchievement = m_mapAchievement[i];
		if ( pAchievement && 0 == ( Q_stricmp( pchName, pAchievement->GetName() ) ) )
			return pAchievement;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: downloads user data from Steam or XBox Live
//-----------------------------------------------------------------------------
void CAchievementMgr::DownloadUserData()
{
	if ( IsPC() )
	{
#ifndef NO_STEAM
		if ( SteamUserStats() )
		{
			// request stat download; will get called back at OnUserStatsReceived when complete
			SteamUserStats()->RequestCurrentStats( CGameID( engine->GetAppID() ) );
		}
#endif
	}
	else if ( IsX360() )
	{
#if defined( _X360 )
		if ( XBX_GetPrimaryUserId() == INVALID_USER_ID )
			return;

		// Download achievements from XBox Live
		bool bDownloadSuccessful = true;
		int nTotalAchievements = 99;
		uint bytes;
		int ret = xboxsystem->EnumerateAchievements( XBX_GetPrimaryUserId(), 0, 0, nTotalAchievements, &bytes, 0, false );
		if ( ret != ERROR_SUCCESS )
		{
			Warning( "Enumerate Achievements failed! Error %d", ret );
			bDownloadSuccessful = false;
		}
		
		// Enumerate the achievements from Live
		void *pBuffer = new byte[bytes];
		if ( bDownloadSuccessful )
		{
			ret = xboxsystem->EnumerateAchievements( XBX_GetPrimaryUserId(), 0, 0, nTotalAchievements, pBuffer, bytes, false );

			if ( ret != nTotalAchievements )
			{
				Warning( "Enumerate Achievements failed! Error %d", ret );
				bDownloadSuccessful = false;
			}
		}

		if ( bDownloadSuccessful )
		{
			// Give live a chance to mark achievements as unlocked, in case the achievement manager
			// wasn't able to get that data (storage device missing, read failure, etc)
			XACHIEVEMENT_DETAILS *pXboxAchievements = (XACHIEVEMENT_DETAILS*)pBuffer;
			for ( int i = 0; i < nTotalAchievements; ++i )
			{
				CBaseAchievement *pAchievement = GetAchievementByID( pXboxAchievements[i].dwId );
				if ( !pAchievement )
					continue;

				// Give Live a chance to claim the achievement as unlocked
				if ( AchievementEarned( pXboxAchievements[i].dwFlags ) )
				{
					pAchievement->SetAchieved( true );
				}
			}
		}

		delete pBuffer;
#endif // X360
	}
}

const char *COM_GetModDirectory()
{
	static char modDir[MAX_PATH];
	if ( Q_strlen( modDir ) == 0 )
	{
		const char *gamedir = CommandLine()->ParmValue("-game", CommandLine()->ParmValue( "-defaultgamedir", "hl2" ) );
		Q_strncpy( modDir, gamedir, sizeof(modDir) );
		if ( strchr( modDir, '/' ) || strchr( modDir, '\\' ) )
		{
			Q_StripLastDir( modDir, sizeof(modDir) );
			int dirlen = Q_strlen( modDir );
			Q_strncpy( modDir, gamedir + dirlen, sizeof(modDir) - dirlen );
		}
	}

	return modDir;
}

//-----------------------------------------------------------------------------
// Purpose: downloads user data from Steam or XBox Live
//-----------------------------------------------------------------------------
void CAchievementMgr::UploadUserData()
{
	if ( IsPC() )
	{
#ifndef NO_STEAM
		if ( SteamUserStats() )
		{
			// Upload current Steam client achievements & stats state to Steam.  Will get called back at OnUserStatsStored when complete.
			// Only values previously set via SteamUserStats() get uploaded
			CGameID gameID( engine->GetAppID() );
			SteamUserStats()->StoreStats( gameID );
		}
#endif
	}
}

//-----------------------------------------------------------------------------
// Purpose: loads global state from file
//-----------------------------------------------------------------------------
void CAchievementMgr::LoadGlobalState()
{
	if ( IsX360() )
	{
#ifdef _X360
		if ( XBX_GetStorageDeviceId() == XBX_INVALID_STORAGE_ID || XBX_GetStorageDeviceId() == XBX_STORAGE_DECLINED )
			return;
#endif
	}

	char	szFilename[_MAX_PATH];

	if ( IsX360() )
	{
		Q_snprintf( szFilename, sizeof( szFilename ), "cfg:/%s_GameState.txt", COM_GetModDirectory() );
	}
	else
	{
		Q_snprintf( szFilename, sizeof( szFilename ), "GameState.txt" );
	}

	KeyValues *pKV = new KeyValues("GameState" );
	if ( pKV->LoadFromFile( filesystem, szFilename, "MOD" ) )
	{
		KeyValues *pNode = pKV->GetFirstSubKey();
		while ( pNode )
		{
			// look up this achievement
			int iAchievementID = pNode->GetInt( "id", 0 );
			if ( iAchievementID > 0 )
			{
				CBaseAchievement *pAchievement = GetAchievementByID( iAchievementID );
				if ( pAchievement )
				{
					// set the count
					if ( pNode->GetInt( "value" ) > 0 )
					{
						pAchievement->SetCount( pAchievement->GetGoal() );						
					}
					else if ( !pAchievement->HasComponents() )
					{						
						pAchievement->SetCount( pNode->GetInt( "data" ) );
					}

					// if this achievement has components, set the component bits
					if ( pAchievement->HasComponents() )
					{
						int64 iComponentBits = pNode->GetUint64( "data" );
						pAchievement->SetComponentBits( iComponentBits );
					}
					pAchievement->SetProgressShown( pNode->GetInt( "msg" ) );
					pAchievement->EvaluateIsAlreadyAchieved();
				}
			}

			pNode = pNode->GetNextKey();
		}

		m_bGlobalStateLoaded = true;
	}
}

//-----------------------------------------------------------------------------
// Purpose: saves global state to file
//-----------------------------------------------------------------------------

void CAchievementMgr::SaveGlobalState( bool bAsync )
{
	VPROF_BUDGET( "CAchievementMgr::SaveGlobalState", "Achievements" );

	KeyValues *pKV = new KeyValues("GameState" );
	FOR_EACH_MAP( m_mapAchievement, i )
	{
		CBaseAchievement *pAchievement = m_mapAchievement[i];
		if ( pAchievement->ShouldSaveGlobal() )
		{
			KeyValues *pNode = pKV->CreateNewKey();
			pNode->SetInt( "id", pAchievement->GetAchievementID() );
			pNode->SetInt( "value", pAchievement->IsAchieved() ? 1 : 0 );
			if ( pAchievement->HasComponents() )
			{
				pNode->SetUint64( "data", pAchievement->GetComponentBits() );
			}
			else
			{
				if ( !pAchievement->IsAchieved() )
				{
					pNode->SetInt( "data", pAchievement->GetCount() );
				}
			}
			pNode->SetInt( "msg", pAchievement->GetProgressShown() );
		}
	}

	if ( !bAsync )
	{
		WriteAchievementGlobalState( pKV );
	}
	else
	{
#ifndef _LINUX
		g_AchievementSaveThread.WriteAchievementGlobalState( pKV );
#endif
	}
	m_bDirty = false;
}

//-----------------------------------------------------------------------------
// Purpose: loads global state if we have not already successfully loaded it
//-----------------------------------------------------------------------------
void CAchievementMgr::EnsureGlobalStateLoaded()
{
	if ( !m_bGlobalStateLoaded )
	{
		LoadGlobalState();
	}
}

//-----------------------------------------------------------------------------
// Purpose: saves global state to file if there have been any changes
//-----------------------------------------------------------------------------
void CAchievementMgr::SaveGlobalStateIfDirty( bool bAsync )
{
	if ( m_bDirty )
	{
		SaveGlobalState( bAsync );
	}
}

//-----------------------------------------------------------------------------
// Purpose: awards specified achievement
//-----------------------------------------------------------------------------
void CAchievementMgr::AwardAchievement( int iAchievementID )
{
	CBaseAchievement *pAchievement = GetAchievementByID( iAchievementID );
	Assert( pAchievement );
	if ( !pAchievement )
		return;

	if ( !CheckAchievementsEnabled() )
	{
		Msg( "Achievements disabled, ignoring achievement unlock for %s\n", pAchievement->GetName() );
		return;
	}

	if ( pAchievement->IsAchieved() )
	{
		if ( cc_achievement_debug.GetInt() > 0 )
		{
			Msg( "Achievement award called but already achieved: %s\n", pAchievement->GetName() );
		}
		return;
	}
	pAchievement->SetAchieved( true );

	if ( cc_achievement_debug.GetInt() > 0 )
	{
		Msg( "Achievement awarded: %s\n", pAchievement->GetName() );
	}

	// save state at next good opportunity.  (Don't do it immediately, may hitch at bad time.)
	m_bDirty = true;	

	if ( IsPC() )
	{		
#ifndef NO_STEAM
		if ( SteamUserStats() )
		{
			VPROF_BUDGET( "AwardAchievement", VPROF_BUDGETGROUP_STEAM );
			// set this achieved in the Steam client
			CGameID gameID( engine->GetAppID() );
			bool bRet = SteamUserStats()->SetAchievement( gameID, pAchievement->GetName() );
			//		Assert( bRet );
			if ( bRet )
			{
				// upload achievement to steam
				UploadUserData();
				m_AchievementsAwarded.AddToTail( iAchievementID );
			}
		}
#endif
	}
	else if ( IsX360() )
	{
#ifdef _X360
		if ( xboxsystem )
			xboxsystem->AwardAchievement( XBX_GetPrimaryUserId(), iAchievementID );
#endif
	}
}

//-----------------------------------------------------------------------------
// Purpose: clears state for all achievements
//-----------------------------------------------------------------------------
void CAchievementMgr::PreRestoreSavedGame()
{
	// load global state if we haven't already; X360 users may not have had a storage device available or selected at boot time
	EnsureGlobalStateLoaded();

	FOR_EACH_MAP( m_mapAchievement, i )
	{
		m_mapAchievement[i]->PreRestoreSavedGame();
	}
}

//-----------------------------------------------------------------------------
// Purpose: clears state for all achievements
//-----------------------------------------------------------------------------
void CAchievementMgr::PostRestoreSavedGame()
{
	FOR_EACH_MAP( m_mapAchievement, i )
	{
		m_mapAchievement[i]->PostRestoreSavedGame();
	}
}

extern bool IsInCommentaryMode( void );
//-----------------------------------------------------------------------------
// Purpose: checks if achievements are enabled
//-----------------------------------------------------------------------------
bool CAchievementMgr::CheckAchievementsEnabled()
{
	// if PC, Steam must be running and user logged in
	if ( IsPC() && !LoggedIntoSteam() )
	{
		Msg( "Achievements disabled: Steam not running.\n" );
		return false;
	}

#if defined( _X360 )
	uint state = XUserGetSigninState( XBX_GetPrimaryUserId() );
	if ( state == eXUserSigninState_NotSignedIn )
	{
		Msg( "Achievements disabled: not signed in to XBox user account.\n" );
		return false;
	}
#endif

	// can't be in commentary mode, user is invincible
	if ( IsInCommentaryMode() )
	{
		Msg( "Achievements disabled: in commentary mode.\n" );
		return false;
	}

#ifdef CLIENT_DLL
	// achievements disabled if playing demo
	if ( engine->IsPlayingDemo() )
	{
		Msg( "Achievements disabled: demo playing.\n" );
		return false;
	}
#endif // CLIENT_DLL

	if ( IsPC() )
	{
		// Don't award achievements if cheats are turned on.  
		ConVarRef sv_cheats( "sv_cheats" );
		if ( sv_cheats.GetBool() )
		{
			// Cheats get turned on automatically if you run with -dev which many people do internally, so allow cheats if developer is turned on and we're not running
			// on Steam public
			if ( ( developer.GetInt() == 0 ) )
			{
				Msg( "Achievements disabled: cheats turned on.\n" );
				return false;
			}
		}
	}

	return true;
}

#ifdef CLIENT_DLL

//-----------------------------------------------------------------------------
// Purpose: Returns the whether all of the local player's team mates are
//			on her friends list, and if there are at least the specified # of
//			teammates.  Involves cross-process calls to Steam so this is mildly
//			expensive, don't call this every frame.
//-----------------------------------------------------------------------------
bool CalcPlayersOnFriendsList( int iMinFriends )
{
	// Got message during connection
	if ( !g_PR )
		return false;

	Assert( g_pGameRules->IsMultiplayer() );

	// Do a cheap rejection: check teammate count first to see if we even need to bother checking w/Steam
	// Subtract 1 for the local player.
	if ( CalcPlayerCount()-1 < iMinFriends )
		return false;

	// determine local player team
	int iLocalPlayerIndex =  GetLocalPlayerIndex();
	uint64 XPlayerUid = 0;

	if ( IsPC() )
	{
		if ( !g_pGameRules->IsMultiplayer() )
			return false;
	}
	else if ( IsX360() )
	{
		if ( !matchmaking )
			return false;

		XPlayerUid = XBX_GetPrimaryUserId();
	}
	else
	{
		// other platforms...?
		return false;
	}
	// Loop through the players
	int iTotalFriends = 0;
	for( int iPlayerIndex = 1 ; iPlayerIndex <= MAX_PLAYERS; iPlayerIndex++ )
	{
		// find all players who are on the local player's team
		if( ( iPlayerIndex != iLocalPlayerIndex ) && ( g_PR->IsConnected( iPlayerIndex ) ) )
		{
			if ( IsPC() )
			{
				player_info_t pi;
				if ( !engine->GetPlayerInfo( iPlayerIndex, &pi ) )
					continue;
				if ( !pi.friendsID )
					continue;
#ifndef NO_STEAM
				// check and see if they're on the local player's friends list
				CSteamID steamID( pi.friendsID, 1, SteamUtils()->GetConnectedUniverse(), k_EAccountTypeIndividual );
				if ( !SteamFriends()->HasFriend( steamID, /*k_EFriendFlagImmediate*/ 0x04 ) )
					continue;
#endif
			}
			else if ( IsX360() )
			{
				uint64 XUid[1];
				XUid[0] = matchmaking->PlayerIdToXuid( iPlayerIndex );
				BOOL bFriend;
#ifdef _X360
				XUserAreUsersFriends( XPlayerUid, XUid, 1, &bFriend, NULL );
#endif // _X360
				if ( !bFriend )
					continue;
			}

			iTotalFriends++;
		}
	}

	return (iTotalFriends >= iMinFriends);
}

//-----------------------------------------------------------------------------
// Purpose: Returns whether there are a specified # of teammates who all belong
//			to same clan as local player. Involves cross-process calls to Steam 
//			so this is mildly expensive, don't call this every frame.
//-----------------------------------------------------------------------------
bool CalcHasNumClanPlayers( int iClanTeammates )
{
	Assert( g_pGameRules->IsMultiplayer() );
#ifndef NO_STEAM
	if ( IsPC() )
	{
		// Do a cheap rejection: check teammate count first to see if we even need to bother checking w/Steam
		// Subtract 1 for the local player.
		if ( CalcPlayerCount()-1 < iClanTeammates )
			return false;

		if ( !SteamFriends() || !SteamUtils() || !g_pGameRules->IsMultiplayer() )
			return false;

		// determine local player team
		int iLocalPlayerIndex =  GetLocalPlayerIndex();

		for ( int iClan = 0; iClan < SteamFriends()->GetClanCount(); iClan++ )
		{
			int iClanMembersOnTeam = 0;
			CSteamID clanID = SteamFriends()->GetClanByIndex( iClan );
			// enumerate all players
			for( int iPlayerIndex = 1 ; iPlayerIndex <= MAX_PLAYERS; iPlayerIndex++ )
			{
				if( ( iPlayerIndex != iLocalPlayerIndex ) && ( g_PR->IsConnected( iPlayerIndex ) ) )
				{
					player_info_t pi;
					if ( engine->GetPlayerInfo( iPlayerIndex, &pi ) && ( pi.friendsID ) )
					{	
						// check and see if they're on the local player's friends list
						CSteamID steamID( pi.friendsID, 1, SteamUtils()->GetConnectedUniverse(), k_EAccountTypeIndividual );
						if ( SteamFriends()->IsUserInSource( steamID, clanID ) )
						{
							iClanMembersOnTeam++;
							if ( iClanMembersOnTeam == iClanTeammates )
								return true;
						}
					}
				}
			}
		}

		return false;
	}
	else if ( IsX360() )
	{
		// TODO: implement for 360
		return false;
	}
	else 
	{
		// other platforms...?
		return false;
	}
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: Returns the # of teammates of the local player
//-----------------------------------------------------------------------------
int	CalcTeammateCount()
{
	Assert( g_pGameRules->IsMultiplayer() );

	// determine local player team
	int iLocalPlayerIndex =  GetLocalPlayerIndex();
	int iLocalPlayerTeam = g_PR->GetTeam( iLocalPlayerIndex );

	int iNumTeammates = 0;
	for( int iPlayerIndex = 1 ; iPlayerIndex <= MAX_PLAYERS; iPlayerIndex++ )
	{
		// find all players who are on the local player's team
		if( ( iPlayerIndex != iLocalPlayerIndex ) && ( g_PR->IsConnected( iPlayerIndex ) ) && ( g_PR->GetTeam( iPlayerIndex ) == iLocalPlayerTeam ) )
		{
			iNumTeammates++;
		}
	}
	return iNumTeammates;
}

//-----------------------------------------------------------------------------
// Purpose: Returns the # of teammates of the local player
//-----------------------------------------------------------------------------
int	CalcPlayerCount()
{
	int iCount = 0;
	for( int iPlayerIndex = 1 ; iPlayerIndex <= MAX_PLAYERS; iPlayerIndex++ )
	{
		// find all players who are on the local player's team
		if( g_PR->IsConnected( iPlayerIndex ) )
		{
			iCount++;
		}
	}
	return iCount;
}

#endif // CLIENT_DLL

//-----------------------------------------------------------------------------
// Purpose: Resets all achievements.  For debugging purposes only
//-----------------------------------------------------------------------------
void CAchievementMgr::ResetAchievements()
{
	if ( !IsPC() )
	{
		DevMsg( "Only available on PC\n" );
		return;
	}

	if ( !LoggedIntoSteam() )
	{
		Msg( "Steam not running, achievements disabled. Cannot reset achievements.\n" );
		return;
	}

	CGameID gameID( engine->GetAppID() );
	FOR_EACH_MAP( m_mapAchievement, i )
	{
		CBaseAchievement *pAchievement = m_mapAchievement[i];
		ResetAchievement_Internal( pAchievement );
	}
	if ( SteamUserStats() )
	{
		SteamUserStats()->StoreStats( gameID );
	}
	SaveGlobalState();
}

void CAchievementMgr::ResetAchievement( int iAchievementID )
{
	if ( !IsPC() )
	{
		DevMsg( "Only available on PC\n" );
		return;
	}

	if ( !LoggedIntoSteam() )
	{
		Msg( "Steam not running, achievements disabled. Cannot reset achievements.\n" );
		return;
	}

	CGameID gameID( engine->GetAppID() );
	CBaseAchievement *pAchievement = GetAchievementByID( iAchievementID );
	Assert( pAchievement );
	if ( pAchievement )
	{
		ResetAchievement_Internal( pAchievement );
		if ( SteamUserStats() )
		{
			SteamUserStats()->StoreStats( gameID );
		}
		SaveGlobalState();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Resets all achievements.  For debugging purposes only
//-----------------------------------------------------------------------------
void CAchievementMgr::PrintAchievementStatus()
{
	if ( IsPC() && !LoggedIntoSteam() )
	{
		Msg( "Steam not running, achievements disabled. Cannot view or unlock achievements.\n" );
		return;
	}

	Msg( "%42s %-20s %s\n", "Name:", "Status:", "Point value:" );
	int iTotalAchievements = 0, iTotalPoints = 0;
	FOR_EACH_MAP( m_mapAchievement, i )
	{
		CBaseAchievement *pAchievement = m_mapAchievement[i];

		Msg( "%42s ", pAchievement->GetName() );	

		CFailableAchievement *pFailableAchievement = dynamic_cast<CFailableAchievement *>( pAchievement );
		if ( pAchievement->IsAchieved() )
		{
			Msg( "%-20s", "ACHIEVED" );
		}
		else if ( pFailableAchievement && pFailableAchievement->IsFailed() )
		{
			Msg( "%-20s", "FAILED" );
		}
		else 
		{
			char szBuf[255];
			Q_snprintf( szBuf, ARRAYSIZE( szBuf ), "(%d/%d)%s", pAchievement->GetCount(), pAchievement->GetGoal(),
				pAchievement->IsActive() ? "" : " (inactive)" );
			Msg( "%-20s", szBuf );
		}
		Msg( " %d   ", pAchievement->GetPointValue() );
		pAchievement->PrintAdditionalStatus();
		Msg( "\n" );
		iTotalAchievements++;
		iTotalPoints += pAchievement->GetPointValue();
	}
	Msg( "Total achievements: %d  Total possible points: %d\n", iTotalAchievements, iTotalPoints );
}

//-----------------------------------------------------------------------------
// Purpose: called when a game event is fired
//-----------------------------------------------------------------------------
void CAchievementMgr::FireGameEvent( IGameEvent *event )
{
	VPROF_( "CAchievementMgr::FireGameEvent", 1, VPROF_BUDGETGROUP_STEAM, false, 0 );
	const char *name = event->GetName();
	if ( 0 == Q_strcmp( name, "entity_killed" ) )
	{
#ifdef GAME_DLL
		CBaseEntity *pVictim = UTIL_EntityByIndex( event->GetInt( "entindex_killed", 0 ) );
		CBaseEntity *pAttacker = UTIL_EntityByIndex( event->GetInt( "entindex_attacker", 0 ) );
		CBaseEntity *pInflictor = UTIL_EntityByIndex( event->GetInt( "entindex_inflictor", 0 ) );
		OnKillEvent( pVictim, pAttacker, pInflictor, event );
#endif // GAME_DLL
	}
	else if ( 0 == Q_strcmp( name, "game_init" ) )
	{
#ifdef GAME_DLL
		// clear all state as though we were loading a saved game, but without loading the game
		PreRestoreSavedGame();
		PostRestoreSavedGame();
#endif // GAME_DLL
	}
#ifdef CLIENT_DLL
	else if ( 0 == Q_strcmp( name, "player_death" ) )
	{
		CBaseEntity *pVictim = ClientEntityList().GetEnt( engine->GetPlayerForUserID( event->GetInt("userid") ) );
		CBaseEntity *pAttacker = ClientEntityList().GetEnt( engine->GetPlayerForUserID( event->GetInt("attacker") ) );
		OnKillEvent( pVictim, pAttacker, NULL, event );
	}
	else if ( 0 == Q_strcmp( name, "localplayer_changeclass" ) )
	{
		// keep track of when the player last changed class
		m_flLastClassChangeTime =  gpGlobals->curtime;
	}
	else if ( 0 == Q_strcmp( name, "localplayer_changeteam" ) )
	{
		// keep track of the time of transitions to and from a game team
		C_BasePlayer *pLocalPlayer = C_BasePlayer::GetLocalPlayer();
		if ( pLocalPlayer )
		{
			int iTeam = pLocalPlayer->GetTeamNumber();
			if ( iTeam > TEAM_SPECTATOR )
			{
				if ( 0 == m_flTeamplayStartTime )
				{
					// player transitioned from no/spectator team to a game team, mark the time
					m_flTeamplayStartTime = gpGlobals->curtime;
				}				
			}
			else
			{
				// player transitioned to no/spectator team, clear the teamplay start time
				m_flTeamplayStartTime = 0;
			}			
		}		
	}
	else if ( 0 == Q_strcmp( name, "teamplay_round_start" ) )
	{
		if ( event->GetBool( "full_reset" ) )
		{
			// we're starting a full round, clear miniround count
			m_iMiniroundsCompleted = 0;
		}
	}
	else if ( 0 == Q_strcmp( name, "teamplay_round_win" ) )
	{
		if ( false == event->GetBool( "full_round", true ) )
		{
			// we just finished a miniround but the round is continuing, increment miniround count
			m_iMiniroundsCompleted ++;
		}
	}
	else if ( 0 == Q_strcmp( name, "player_stats_updated" ) )
	{
		FOR_EACH_MAP( m_mapAchievement, i )
		{
			CBaseAchievement *pAchievement = m_mapAchievement[i];
			pAchievement->OnPlayerStatsUpdate();
		}
	}
#endif // CLIENT_DLL
}

//-----------------------------------------------------------------------------
// Purpose: called when a player or character has been killed
//-----------------------------------------------------------------------------
void CAchievementMgr::OnKillEvent( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event )
{
	// can have a NULL victim on client if victim has never entered local player's PVS
	if ( !pVictim )
		return;

	// if single-player game, calculate if the attacker is the local player and if the victim is the player enemy
	bool bAttackerIsPlayer = false;
	bool bVictimIsPlayerEnemy = false;
#ifdef GAME_DLL
	if ( !g_pGameRules->IsMultiplayer() )
	{
		CBasePlayer *pLocalPlayer = UTIL_GetLocalPlayer();
		if ( pLocalPlayer )
		{
			if ( pAttacker == pLocalPlayer )
			{
				bAttackerIsPlayer = true;
			}

			CBaseCombatCharacter *pBCC = dynamic_cast<CBaseCombatCharacter *>( pVictim );
			if ( pBCC && ( D_HT == pBCC->IRelationType( pLocalPlayer ) ) )
			{
				bVictimIsPlayerEnemy = true;
			}
		}
	}
#else
	C_BasePlayer *pLocalPlayer = C_BasePlayer::GetLocalPlayer();
	if ( pAttacker == pLocalPlayer )
	{
		bAttackerIsPlayer = true;
		if ( pVictim != pLocalPlayer)
		{
			bVictimIsPlayerEnemy = true;
		}		
	}
#endif // GAME_DLL

	// look through all the kill event listeners and notify any achievements whose filters we pass
	FOR_EACH_VEC( m_vecKillEventListeners, iAchievement )
	{
		CBaseAchievement *pAchievement = m_vecKillEventListeners[iAchievement];

		if ( !pAchievement->IsActive() )
			continue;

		// if this achievement only looks for kills where attacker is player and that is not the case here, skip this achievement
		if ( ( pAchievement->GetFlags() & ACH_FILTER_ATTACKER_IS_PLAYER ) && !bAttackerIsPlayer )
			continue;

		// if this achievement only looks for kills where victim is killer enemy and that is not the case here, skip this achievement
		if ( ( pAchievement->GetFlags() & ACH_FILTER_VICTIM_IS_PLAYER_ENEMY ) && !bVictimIsPlayerEnemy )
			continue;

#if GAME_DLL
		// if this achievement only looks for a particular victim class name and this victim is a different class, skip this achievement
		const char *pVictimClassNameFilter = pAchievement->m_pVictimClassNameFilter;
		if ( pVictimClassNameFilter && !pVictim->ClassMatches( pVictimClassNameFilter ) )
			continue;

		// if this achievement only looks for a particular inflictor class name and this inflictor is a different class, skip this achievement
		const char *pInflictorClassNameFilter = pAchievement->m_pInflictorClassNameFilter;
		if ( pInflictorClassNameFilter &&  ( ( NULL == pInflictor ) || !pInflictor->ClassMatches( pInflictorClassNameFilter ) ) )
			continue;

		// if this achievement only looks for a particular attacker class name and this attacker is a different class, skip this achievement
		const char *pAttackerClassNameFilter = pAchievement->m_pAttackerClassNameFilter;
		if ( pAttackerClassNameFilter && ( ( NULL == pAttacker ) || !pAttacker->ClassMatches( pAttackerClassNameFilter ) ) )
			continue;

		// if this achievement only looks for a particular inflictor entity name and this inflictor has a different name, skip this achievement
		const char *pInflictorEntityNameFilter = pAchievement->m_pInflictorEntityNameFilter;
		if ( pInflictorEntityNameFilter && ( ( NULL == pInflictor ) || !pInflictor->NameMatches( pInflictorEntityNameFilter ) ) )
			continue;
#endif // GAME_DLL

		// we pass all filters for this achievement, notify the achievement of the kill
		pAchievement->Event_EntityKilled( pVictim, pAttacker, pInflictor, event );
	}		
}

void CAchievementMgr::OnAchievementEvent( int iAchievementID )
{
	// handle event for specific achievement
	CBaseAchievement *pAchievement = GetAchievementByID( iAchievementID );
	Assert( pAchievement );
	if ( pAchievement )
	{
		if ( !pAchievement->IsAchieved() )
		{
			pAchievement->IncrementCount();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: called when a map-fired achievement event occurs
//-----------------------------------------------------------------------------
void CAchievementMgr::OnMapEvent( const char *pchEventName )
{
	Assert( pchEventName && *pchEventName );
	if ( !pchEventName || !*pchEventName ) 
		return;

	// see if this event matches the prefix for an achievement component
	FOR_EACH_VEC( m_vecComponentListeners, iAchievement )
	{
		CBaseAchievement *pAchievement = m_vecComponentListeners[iAchievement];
		Assert( pAchievement->m_pszComponentPrefix );
		if ( 0 == Q_strncmp( pchEventName, pAchievement->m_pszComponentPrefix, pAchievement->m_iComponentPrefixLen ) )
		{
			// prefix matches, tell the achievement a component was found
			pAchievement->OnComponentEvent( pchEventName );
			return;
		}
	}

	// look through all the map event listeners
	FOR_EACH_VEC( m_vecMapEventListeners, iAchievement )
	{
		CBaseAchievement *pAchievement = m_vecMapEventListeners[iAchievement];
		pAchievement->OnMapEvent( pchEventName );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Returns an achievement as it's abstract object. This interface is used by gameui.dll for getting achievement info.
// Input  : index - 
// Output : IBaseAchievement*
//-----------------------------------------------------------------------------
IAchievement* CAchievementMgr::GetAchievementByIndex( int index )
{
	Assert( m_vecAchievement.IsValidIndex(index) );
	return (IAchievement*)m_vecAchievement[index];
}

//-----------------------------------------------------------------------------
// Purpose: Returns total achievement count. This interface is used by gameui.dll for getting achievement info.
// Input  :  - 
// Output : Count of achievements in manager's vector.
//-----------------------------------------------------------------------------
int CAchievementMgr::GetAchievementCount()
{
	return m_vecAchievement.Count();
}


#if !defined(NO_STEAM)
//-----------------------------------------------------------------------------
// Purpose: called when stat download is complete
//-----------------------------------------------------------------------------
void CAchievementMgr::Steam_OnUserStatsReceived( UserStatsReceived_t *pUserStatsReceived )
{
	Assert( SteamUserStats() );
	if ( !SteamUserStats() )
		return;

	if ( pUserStatsReceived->m_eResult != k_EResultOK )
	{
		DevMsg( "CTFSteamStats: failed to download stats from Steam, EResult %d\n", pUserStatsReceived->m_eResult );
		return;
	}
													
	CGameID gameID( engine->GetAppID() );

	// run through the achievements and set their achieved state according to Steam data
	FOR_EACH_MAP( m_mapAchievement, i )
	{
		CBaseAchievement *pAchievement = m_mapAchievement[i];
		bool bAchieved = false;
		bool bRet = SteamUserStats()->GetAchievement( gameID, pAchievement->GetName(), &bAchieved );
		if ( bRet )
		{
			// set local achievement state
			pAchievement->SetAchieved( bAchieved );
		}
		else
		{
			DevMsg( "ISteamUserStats::GetAchievement failed for %s\n", pAchievement->GetName() );
		}
	}

	// send an event to anyone else who needs Steam user stat data
	IGameEvent *event = gameeventmanager->CreateEvent( "user_data_downloaded" );
	if ( event )
	{
#ifdef GAME_DLL
		gameeventmanager->FireEvent( event );
#else
		gameeventmanager->FireEventClientSide( event );
#endif
	}
}

//-----------------------------------------------------------------------------
// Purpose: called when stat upload is complete
//-----------------------------------------------------------------------------
void CAchievementMgr::Steam_OnUserStatsStored( UserStatsStored_t *pUserStatsStored )
{
	if ( k_EResultOK != pUserStatsStored->m_eResult )
	{
		DevMsg( "CAchievementMgr: failed to upload stats to Steam, EResult %d\n", pUserStatsStored->m_eResult );
	} 

	else
	{
		if ( m_AchievementsAwarded.Count() > 0 )
		{
#ifndef GAME_DLL
			// send a message to the server about our achievement
			if ( g_pGameRules && g_pGameRules->IsMultiplayer() )
			{
				C_BasePlayer *pLocalPlayer = C_BasePlayer::GetLocalPlayer();
				if ( pLocalPlayer )
				{
					char cmd[256];
					int iPlayerID = pLocalPlayer->GetUserID();
					unsigned short mask = UTIL_GetAchievementEventMask();

					Q_snprintf( cmd, sizeof( cmd ), "achievement_earned %d %d", m_AchievementsAwarded[0] ^ mask, ( iPlayerID ^ m_AchievementsAwarded[0] ) ^ mask );
					engine->ClientCmd_Unrestricted( cmd );
				}
			}
#endif			
			m_AchievementsAwarded.Remove( 0 );
		}
	}
}
#endif // !defined(NO_STEAM)


void CAchievementMgr::ResetAchievement_Internal( CBaseAchievement *pAchievement )
{
	Assert( pAchievement );
	CGameID gameID( engine->GetAppID() );
#ifndef NO_STEAM
	if ( SteamUserStats() )
	{
		SteamUserStats()->ClearAchievement( gameID, pAchievement->GetName() );		
	}
#endif
	pAchievement->SetAchieved( false );
	pAchievement->SetCount( 0 );	
	if ( pAchievement->HasComponents() )
	{
		pAchievement->SetComponentBits( 0 );
	}
	pAchievement->SetProgressShown( 0 );
	pAchievement->StopListeningForAllEvents();
	if ( pAchievement->IsActive() )
	{
		pAchievement->ListenForEvents();
	}
}

#ifdef CLIENT_DLL

void MsgFunc_AchievementEvent( bf_read &msg )
{
	int iAchievementID = (int) msg.ReadByte();
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;
	pAchievementMgr->OnAchievementEvent( iAchievementID );
}

CON_COMMAND_F( achievement_reset_all, "Clears all achievements", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;
	pAchievementMgr->ResetAchievements();
}

CON_COMMAND_F( achievement_reset, "<internal name> Clears specified achievement", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;

	if ( 2 != args.ArgC() )
	{
		Msg( "Usage: achievement_reset <internal name>\n" );
		return;
	}
	CBaseAchievement *pAchievement = pAchievementMgr->GetAchievementByName( args[1] );
	if ( !pAchievement )
	{
		Msg( "Achievement %s not found\n", args[1] );
		return;
	}
	pAchievementMgr->ResetAchievement( pAchievement->GetAchievementID() );

}

CON_COMMAND_F( achievement_status, "Shows status of all achievement", FCVAR_CHEAT )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;
	pAchievementMgr->PrintAchievementStatus();
}

CON_COMMAND_F( achievement_unlock, "<internal name> Unlocks achievement", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;

	if ( 2 != args.ArgC() )
	{
		Msg( "Usage: achievement_unlock <internal name>\n" );
		return;
	}
	CBaseAchievement *pAchievement = pAchievementMgr->GetAchievementByName( args[1] );
	if ( !pAchievement )
	{
		Msg( "Achievement %s not found\n", args[1] );
		return;
	}
	pAchievementMgr->AwardAchievement( pAchievement->GetAchievementID() );
}

CON_COMMAND_F( achievement_unlock_all, "Unlocks all achievements", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;

	int iCount = pAchievementMgr->GetAchievementCount();
	for ( int i = 0; i < iCount; i++ )
	{
		IAchievement *pAchievement = pAchievementMgr->GetAchievementByIndex( i );
		if ( !pAchievement->IsAchieved() )
		{
			pAchievementMgr->AwardAchievement( pAchievement->GetAchievementID() );
		}
	}	
}

CON_COMMAND_F( achievement_evaluate, "<internal name> Causes failable achievement to be evaluated", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;

	if ( 2 != args.ArgC() )
	{
		Msg( "Usage: achievement_evaluate <internal name>\n" );
		return;
	}
	CBaseAchievement *pAchievement = pAchievementMgr->GetAchievementByName( args[1] );
	if ( !pAchievement )
	{
		Msg( "Achievement %s not found\n", args[1] );
		return;
	}

	CFailableAchievement *pFailableAchievement = dynamic_cast<CFailableAchievement *>( pAchievement );
	Assert( pFailableAchievement );
	if ( pFailableAchievement )
	{
		pFailableAchievement->OnEvaluationEvent();
	}
}

CON_COMMAND_F( achievement_test_friend_count, "Counts the # of teammates on local player's friends list", FCVAR_DEVELOPMENTONLY )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;
	if ( 2 != args.ArgC() )
	{
		Msg( "Usage: achievement_test_friend_count <min # of teammates>\n" );
		return;
	}
	int iMinFriends = atoi( args[1] );
	bool bRet = CalcPlayersOnFriendsList( iMinFriends );
	Msg( "You %s have at least %d friends in the game.\n", bRet ? "do" : "do not", iMinFriends );
}

CON_COMMAND_F( achievement_test_clan_count, "Determines if specified # of teammates belong to same clan w/local player", FCVAR_DEVELOPMENTONLY )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;

	if ( 2 != args.ArgC() )
	{
		Msg( "Usage: achievement_test_clan_count <# of clan teammates>\n" );
		return;
	}
	int iClanPlayers = atoi( args[1] );
	bool bRet = CalcHasNumClanPlayers( iClanPlayers );
	Msg( "There %s %d players who you're in a Steam group with.\n", bRet ? "are" : "are not", iClanPlayers );
}

CON_COMMAND_F( achievement_mark_dirty, "Mark achievement data as dirty", FCVAR_DEVELOPMENTONLY | FCVAR_CHEAT )
{
	CAchievementMgr *pAchievementMgr = dynamic_cast<CAchievementMgr *>( engine->GetAchievementMgr() );
	if ( !pAchievementMgr )
		return;
	pAchievementMgr->SetDirty( true );
}


#endif // CLIENT_DLL

//-----------------------------------------------------------------------------
// Purpose: helper function to get entity model name
//-----------------------------------------------------------------------------
const char *GetModelName( CBaseEntity *pBaseEntity )
{
	CBaseAnimating *pBaseAnimating = dynamic_cast<CBaseAnimating *>( pBaseEntity );
	if ( pBaseAnimating )
	{
		CStudioHdr *pStudioHdr = pBaseAnimating->GetModelPtr();
		if ( pStudioHdr )
		{
			return pStudioHdr->pszName();
		}
	}

	return "";
}
