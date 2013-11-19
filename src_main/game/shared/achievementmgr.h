//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef ACHIEVEMENTMGR_H
#define ACHIEVEMENTMGR_H
#ifdef _WIN32
#pragma once
#endif
#include "baseachievement.h"
#include "GameEventListener.h"
#include "hl2orange.spa.h"
#include "iachievementmgr.h"
#include "utlmap.h"
#include "steam/steam_api.h"

class CAchievementMgr : public CAutoGameSystem, public CGameEventListener, public IAchievementMgr
{
public:
	CAchievementMgr();

	virtual bool Init();
	virtual void PostInit();
	virtual void Shutdown();
	virtual void LevelInitPreEntity();
	virtual void LevelShutdownPreEntity();
	virtual void InitializeAchievements();

	void OnMapEvent( const char *pchEventName );
	
	// Interfaces exported to other dlls for achievement list queries
	IAchievement* GetAchievementByIndex( int index );
	int GetAchievementCount();

	CBaseAchievement *GetAchievementByID( int iAchievementID );
	CUtlMap<int, CBaseAchievement *> &GetAchievements() { return m_mapAchievement; }

	CBaseAchievement *GetAchievementByName( const char *pchName );

	void UploadUserData();
	void DownloadUserData();
	void SaveGlobalState( bool bAsync = false );
	void LoadGlobalState();
	void SaveGlobalStateIfDirty( bool bAsync = false );
	void EnsureGlobalStateLoaded();
	void AwardAchievement( int iAchievementID );
	void PreRestoreSavedGame();
	void PostRestoreSavedGame();
	void ResetAchievements();
	void ResetAchievement( int iAchievementID );
	void PrintAchievementStatus();
	float GetLastClassChangeTime() { return m_flLastClassChangeTime; }
	float GetTeamplayStartTime() { return m_flTeamplayStartTime; }
	int	  GetMiniroundsCompleted() { return m_iMiniroundsCompleted; }
	const char *GetMapName() { return m_szMap; }
	void OnAchievementEvent( int iAchievementID );
	void SetDirty( bool bDirty ) { m_bDirty = bDirty; }
	bool CheckAchievementsEnabled();
#ifndef NO_STEAM
	bool LoggedIntoSteam() { return ( SteamUser() && SteamUserStats() && SteamUser()->BLoggedOn() ); }
#else
	bool LoggedIntoSteam() { return false; }
#endif

#if !defined(NO_STEAM)
	STEAM_CALLBACK( CAchievementMgr, Steam_OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived );
	STEAM_CALLBACK( CAchievementMgr, Steam_OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored );
#endif

private:
	void FireGameEvent( IGameEvent *event );
	void OnKillEvent( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event );
	void ResetAchievement_Internal( CBaseAchievement *pAchievement );

	CUtlMap<int, CBaseAchievement *> m_mapAchievement;					// map of all achievements
	CUtlVector<CBaseAchievement *>	 m_vecAchievement;					// vector of all achievements for accessing by index
	CUtlVector<CBaseAchievement *> m_vecKillEventListeners;				// vector of achievements that are listening for kill events
	CUtlVector<CBaseAchievement *> m_vecMapEventListeners;				// vector of achievements that are listening for map events
	CUtlVector<CBaseAchievement *> m_vecComponentListeners;				// vector of achievements that are listening for components that make up an achievement

	float m_flLevelInitTime;

	float m_flLastClassChangeTime;		// Time when player last changed class
	float m_flTeamplayStartTime;		// Time when player joined a non-spectating team.  Not updated if she switches game teams; cleared if she joins spectator
	float m_iMiniroundsCompleted;		// # of minirounds played since game start (for maps that have minirounds)
	char  m_szMap[MAX_PATH];			// file base of map name, cached since we access it frequently in this form
	bool  m_bDirty;						// do we have interesting state that needs to be saved
	bool  m_bGlobalStateLoaded;			// have we loaded global state

	CUtlVector<int> m_AchievementsAwarded;
};

// helper functions
const char *GetModelName( CBaseEntity *pBaseEntity );

#ifdef CLIENT_DLL
bool CalcPlayersOnFriendsList( int iMinPlayers );
bool CalcHasNumClanPlayers( int iClanTeammates );
int	CalcPlayerCount();
int	CalcTeammateCount();
#endif // CLIENT

class IMatchmaking;
extern ConVar	cc_achievement_debug;
extern IMatchmaking *matchmaking;

#ifdef CLIENT_DLL
void MsgFunc_AchievementEvent( bf_read &msg );
#endif // CLIENT_DLL
#endif // ACHIEVEMENTMGR_H
