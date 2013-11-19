#ifndef HL1MP_GAMERULES_H
#define HL1MP_GAMERULES_H
#pragma once


#include "gamerules.h"
#include "teamplay_gamerules.h"

extern ConVar sk_mp_dmg_multiplier ;

#ifdef CLIENT_DLL
	#define CHL1MPRules C_HL1MPRules
	#define CHL1MPGameRulesProxy C_HL1MPGameRulesProxy
#endif


class CHL1MPGameRulesProxy : public CGameRulesProxy
{
public:
	DECLARE_CLASS( CHL1MPGameRulesProxy, CGameRulesProxy );
	DECLARE_NETWORKCLASS();
};


class CHL1MPRules : public CTeamplayRules
{
public:
	DECLARE_CLASS( CHL1MPRules, CTeamplayRules );

#ifdef CLIENT_DLL
	DECLARE_CLIENTCLASS_NOBASE();
#else
	DECLARE_SERVERCLASS_NOBASE();
#endif

	CHL1MPRules();
	virtual ~CHL1MPRules();

	virtual void CreateStandardEntities( void );

	virtual bool IsTeamplay( void )
	{
		return m_bTeamPlayEnabled;
	}

	virtual float GetAmmoDamage( CBaseEntity *pAttacker, CBaseEntity *pVictim, int nAmmoType );
    virtual float GetDamageMultiplier( void );    

#ifdef CLIENT_DLL
#else
	virtual void Think ( void );

	virtual void GoToIntermission( void );

	virtual void InitDefaultAIRelationships( void );

	virtual float FlWeaponRespawnTime( CBaseCombatWeapon *pWeapon );
	virtual float FlItemRespawnTime( CItem *pItem );

	virtual const char *SetDefaultPlayerTeam( CBasePlayer *pPlayer );
	virtual void InitHUD( CBasePlayer *pPlayer );
	virtual void ChangePlayerTeam( CBasePlayer *pPlayer, const char *pTeamName, bool bKill, bool bGib );
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );

	virtual int GetTeamIndex( const char * pName );
#endif

private:

	const char *TeamWithFewestPlayers( void );

	CNetworkVar( bool, m_bTeamPlayEnabled );
};


inline CHL1MPRules* HL1MPRules()
{
	return static_cast<CHL1MPRules*>(g_pGameRules);
}


#endif
