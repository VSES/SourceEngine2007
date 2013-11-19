//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#include "cbase.h"

#ifdef CLIENT_DLL

#include "achievementmgr.h"
#include "baseachievement.h"
#include "c_dod_player.h"
#include "dod_shareddefs.h"
#include "c_dod_objective_resource.h"
#include "dod_gamerules.h"

CAchievementMgr g_AchievementMgrDOD;	// global achievement mgr for DOD


class CAchievementDODThrowBackGren : public CBaseAchievement
{
	void Init() 
	{
		// listen for player kill enemy events, base class will increment count each time that happens
		SetFlags( ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL );
		SetGoal( 1 );
	}

	virtual void Event_EntityKilled( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event ) 
	{
		C_DODPlayer *pLocalPlayer = C_DODPlayer::GetLocalDODPlayer();

		if ( pAttacker == pLocalPlayer && pVictim->GetTeamNumber() != pAttacker->GetTeamNumber() )
		{
			// if we are allies and killed with a german grenade
			// or if we are axis and killed with a us grenade

			const char *killedwith = event->GetString( "weapon" );
			int iLocalTeam = pLocalPlayer->GetTeamNumber();

			if ( ( iLocalTeam == TEAM_ALLIES &&	( FStrEq( killedwith, "frag_ger" ) || FStrEq( killedwith, "riflegren_ger" ) ) ) || 
				 ( iLocalTeam == TEAM_AXIS && ( FStrEq( killedwith, "frag_us" ) || FStrEq( killedwith, "riflegren_us" ) ) ) )
			{
				// This kill was made with an enemy grenade.
				IncrementCount();
			}
		}
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODThrowBackGren, ACHIEVEMENT_DOD_THROW_BACK_GREN, "ACHIEVEMENT_DOD_THROW_BACK_GREN", 1 );


class CAchievementDODConsecutiveHeadshots : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );

		// Handled on server
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODConsecutiveHeadshots, ACHIEVEMENT_DOD_CONSECUTIVE_HEADSHOTS, "ACHIEVEMENT_DOD_CONSECUTIVE_HEADSHOTS", 1 );


class CAchievementDODMGPositionStreak : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );

		// Handled on server
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODMGPositionStreak, ACHIEVEMENT_DOD_MG_POSITION_STREAK, "ACHIEVEMENT_DOD_MG_POSITION_STREAK", 1 );


class CAchievementDODWinKnifeFight : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL );
		SetGoal( 1 );
	}

	virtual void Event_EntityKilled( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event ) 
	{
		C_DODPlayer *pLocalPlayer = C_DODPlayer::GetLocalDODPlayer();

		if ( pAttacker == pLocalPlayer && pVictim->GetTeamNumber() != pAttacker->GetTeamNumber() )
		{
			const char *killedwith = event->GetString( "weapon" );

			if ( FStrEq( killedwith, "amerknife" ) || FStrEq( killedwith, "spade" ) )
			{
				C_DODPlayer *pDodVictim = ToDODPlayer( pVictim );

				if ( pDodVictim )
				{
					CWeaponDODBase *pWpn = pDodVictim->GetActiveDODWeapon();

					if ( pWpn->GetDODWpnData().m_WeaponType == WPN_TYPE_MELEE )
					{
						// Kill was made with a melee weapon, killer had melee weapon out
						IncrementCount();
					}
				}
			}			
		}
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODWinKnifeFight, ACHIEVEMENT_DOD_WIN_KNIFE_FIGHT, "ACHIEVEMENT_DOD_WIN_KNIFE_FIGHT", 1 );

const char *pszOfficialMaps[] =
{
	"dod_anzio",
	"dod_avalanche",
	"dod_argentan",
	"dod_colmar",
	"dod_donner",
	"dod_flash",
	"dod_jagd",
	"dod_kalt",
	"dod_palermo"
};

// file we'll save our list of viewed intro movies in
#define CUSTOM_MAPS_FILE				"customcount.res"

class CAchievementDODCustomMaps : public CBaseAchievement
{
	// Requires a player to kill at least one player on 5 different non-official maps

	void Init() 
	{
		SetFlags( ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_GLOBAL );
		SetGoal( 5 );

		m_bCheckedCurrentMap = false;
	}

	virtual void ListenForEvents()
	{
		// hax, this is called from LevelInitPreEntity, init per-level here
		m_bCheckedCurrentMap = false;
	}

	virtual void Event_EntityKilled( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event ) 
	{
		if ( m_bCheckedCurrentMap ) 
			return;

		C_DODPlayer *pLocalPlayer = C_DODPlayer::GetLocalDODPlayer();

		if ( pAttacker == pLocalPlayer && pVictim->GetTeamNumber() != pAttacker->GetTeamNumber() )
		{
			char szMap[MAX_PATH];
			Q_FileBase( engine->GetLevelName(), szMap, ARRAYSIZE( szMap ) );

			if ( !IsOfficialMap( szMap ) && !HasPlayedThisCustomMap() )
			{
				IncrementCount();

				Msg( "Count is now %d\n", m_iCount );

				UTIL_IncrementMapKey( "killed_a_player" );
			}

			// stop listening
			m_bCheckedCurrentMap = true;
		}
	}

	bool HasPlayedThisCustomMap( void )
	{
		return ( UTIL_GetMapKeyCount( "killed_a_player" ) > 0 );
	}

	bool IsOfficialMap( const char *pszMapName )
	{
		bool bFound = false;
	
		for ( int i=0;i<ARRAYSIZE(pszOfficialMaps);i++ )
		{
			if ( FStrEq( pszMapName, pszOfficialMaps[i] ) )
			{
				bFound = true;
				break;
			}
		}	

		return bFound;
	}

	bool m_bCheckedCurrentMap;

};
DECLARE_ACHIEVEMENT( CAchievementDODCustomMaps, ACHIEVEMENT_DOD_PLAY_CUSTOM_MAPS, "ACHIEVEMENT_DOD_PLAY_CUSTOM_MAPS", 1 );


class CAchievementDODKillsWithGrenade : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME );
		SetGoal( 1 );

		m_flLastKillTime = 0;
		m_iKillCount = 0;
	}

	virtual void Event_EntityKilled( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event ) 
	{
		C_DODPlayer *pLocalPlayer = C_DODPlayer::GetLocalDODPlayer();

		if ( pAttacker == pLocalPlayer )
		{
			// reject non-grenade/non-rocket inflictors
			const char *killedwith = event->GetString( "weapon" );

			if ( Q_strncmp( killedwith, "frag_", 5 ) &&
				 Q_strncmp( killedwith, "riflegren_", 10 ) &&
				 !FStrEq( killedwith, "pschreck" ) &&
				 !FStrEq( killedwith, "bazooka" ) )
			{
				m_flLastKillTime = 0;
				return;
			}

			if ( ( gpGlobals->curtime - m_flLastKillTime ) > 0.25 )
			{
				m_iKillCount = 0;
			}

			m_iKillCount++;
			m_flLastKillTime = gpGlobals->curtime;

			if ( m_iKillCount == 4 )
			{
				IncrementCount();
			}
		}
	}

private:
	float m_flLastKillTime;
	int m_iKillCount;
};
DECLARE_ACHIEVEMENT( CAchievementDODKillsWithGrenade, ACHIEVEMENT_DOD_KILLS_WITH_GRENADE, "ACHIEVEMENT_DOD_KILLS_WITH_GRENADE", 1 );


class CAchievementDODLongRangeRocket : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );

		// Handled on server
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODLongRangeRocket, ACHIEVEMENT_DOD_LONG_RANGE_ROCKET, "ACHIEVEMENT_DOD_LONG_RANGE_ROCKET", 1 );


class CAchievementDODEndRoundKills : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_LISTEN_PLAYER_KILL_ENEMY_EVENTS | ACH_SAVE_WITH_GAME );
		SetGoal( 1 );

		m_iKillCount = 0;
	}

	virtual void ListenForEvents()
	{
		ListenForGameEvent( "player_spawn" );
	}

	// Reset the count when we spawn
	void FireGameEvent_Internal( IGameEvent *event )
	{
		if ( m_iKillCount > 0 &&  0 == Q_strcmp( event->GetName(), "player_spawn" ) && C_BasePlayer::GetLocalPlayer() )
		{
			int iUserID = event->GetInt("userid");

			if ( iUserID == C_BasePlayer::GetLocalPlayer()->GetUserID() )
			{
				m_iKillCount = 0;
			}
		}
	}

	// count kills in endround. No requirement that your team must have won the round - grenades count
	virtual void Event_EntityKilled( CBaseEntity *pVictim, CBaseEntity *pAttacker, CBaseEntity *pInflictor, IGameEvent *event ) 
	{
		DODRoundState state = DODGameRules()->State_Get();

		if ( state == STATE_ALLIES_WIN || state == STATE_AXIS_WIN )
		{
			Assert( pAttacker == C_BasePlayer::GetLocalPlayer() );

			if ( pVictim->GetTeamNumber() != pAttacker->GetTeamNumber() )
			{
				m_iKillCount++;

				if ( m_iKillCount > 3 )
				{
					IncrementCount();
				}
			}
		}
	}

	int m_iKillCount;
};
DECLARE_ACHIEVEMENT( CAchievementDODEndRoundKills, ACHIEVEMENT_DOD_END_ROUND_KILLS, "ACHIEVEMENT_DOD_END_ROUND_KILLS", 1 );


class CAchievementDODCapLastFlag : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );

		// handled on server
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODCapLastFlag, ACHIEVEMENT_DOD_CAP_LAST_FLAG, "ACHIEVEMENT_DOD_CAP_LAST_FLAG", 1 );


class CAchievementDODUseEnemyWeapons : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );
	}

	// Handled on server
};
DECLARE_ACHIEVEMENT( CAchievementDODUseEnemyWeapons, ACHIEVEMENT_DOD_USE_ENEMY_WEAPONS, "ACHIEVEMENT_DOD_USE_ENEMY_WEAPONS", 1 );


class CAchievementDODKillDominatingMG : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );

		// Handled on server
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODKillDominatingMG, ACHIEVEMENT_DOD_KILL_DOMINATING_MG, "ACHIEVEMENT_DOD_KILL_DOMINATING_MG", 1 );


class CAchievementDODColmarDefense : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );

		// Handled on server
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODColmarDefense, ACHIEVEMENT_DOD_COLMAR_DEFENSE, "ACHIEVEMENT_DOD_COLMAR_DEFENSE", 1 );


class CAchievementDODJagdOvertimeCap : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );

		// Handled on server
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODJagdOvertimeCap, ACHIEVEMENT_DOD_JAGD_OVERTIME_CAP, "ACHIEVEMENT_DOD_JAGD_OVERTIME_CAP", 1 );


class CAchievementDODWeaponMastery : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 1 );

		// Handled on server
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODWeaponMastery, ACHIEVEMENT_DOD_WEAPON_MASTERY, "ACHIEVEMENT_DOD_WEAPON_MASTERY", 1 );


class CAchievementDODBlockCaptures : public CBaseAchievement
{
	void Init() 
	{
		SetFlags( ACH_SAVE_GLOBAL );
		SetGoal( 25 );
	}

	virtual void ListenForEvents()
	{
		ListenForGameEvent( "dod_capture_blocked" );
	}

	void FireGameEvent_Internal( IGameEvent *event )
	{
		Assert( FStrEq( event->GetName(), "dod_capture_blocked" ) );

		C_DODPlayer *pPlayer = C_DODPlayer::GetLocalDODPlayer();

		if ( pPlayer && pPlayer->entindex() == event->GetInt("blocker") )
		{
			IncrementCount();
		}
	}
};
DECLARE_ACHIEVEMENT( CAchievementDODBlockCaptures, ACHIEVEMENT_DOD_BLOCK_CAPTURES, "ACHIEVEMENT_DOD_BLOCK_CAPTURES", 1 );


#endif // CLIENT_DLL