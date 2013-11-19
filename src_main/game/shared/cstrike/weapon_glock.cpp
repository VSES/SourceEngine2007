//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_csbase.h"
#include "fx_cs_shared.h"


#if defined( CLIENT_DLL )

	#define CWeaponGlock C_WeaponGlock
	#include "c_cs_player.h"

#else

	#include "cs_player.h"

#endif


class CWeaponGlock : public CWeaponCSBase
{
public:
	DECLARE_CLASS( CWeaponGlock, CWeaponCSBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CWeaponGlock();

	virtual void Spawn();

	virtual void PrimaryAttack();
	virtual void SecondaryAttack();
	virtual bool Deploy();

	virtual void ItemPostFrame();

	void GlockFire( float flSpread, bool bFireBurst );
	void FireRemaining( int &shotsFired, float &shootTime );
	
	virtual bool Reload();

	virtual void WeaponIdle();

	virtual CSWeaponID GetWeaponID( void ) const		{ return WEAPON_GLOCK; }

private:
	
	CWeaponGlock( const CWeaponGlock & );

	CNetworkVar( bool, m_bBurstMode );

	int		m_iGlock18ShotsFired;	// used to keep track of the shots fired during the Glock18 burst fire mode.
	float	m_flGlock18Shoot;		// time to shoot the remaining bullets of the glock18 burst fire
	float	m_flLastFire;
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponGlock, DT_WeaponGlock )

BEGIN_NETWORK_TABLE( CWeaponGlock, DT_WeaponGlock )
	#ifdef CLIENT_DLL
		RecvPropBool( RECVINFO( m_bBurstMode ) )
	#else
		SendPropBool( SENDINFO( m_bBurstMode ) )
	#endif
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponGlock )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_glock, CWeaponGlock );
PRECACHE_WEAPON_REGISTER( weapon_glock );



CWeaponGlock::CWeaponGlock()
{
	m_flLastFire = gpGlobals->curtime;
}


void CWeaponGlock::Spawn( )
{
	BaseClass::Spawn();

	m_bBurstMode = false;
	m_iGlock18ShotsFired = 0;
	m_flGlock18Shoot = 0.0;
	m_flAccuracy = 0.9;
}

bool CWeaponGlock::Deploy( )
{
	m_iGlock18ShotsFired = 0;
	m_flGlock18Shoot = 0.0;
	m_flAccuracy = 0.9;

	return BaseClass::Deploy();
}

void CWeaponGlock::SecondaryAttack()
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	if ( m_bBurstMode )
	{
		ClientPrint( pPlayer, HUD_PRINTCENTER, "#Switch_To_SemiAuto" );
		m_bBurstMode = false;
	}
	else
	{
		ClientPrint( pPlayer, HUD_PRINTCENTER, "#Switch_To_BurstFire" );
		m_bBurstMode = true;
	}
	
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.3;
}

void CWeaponGlock::PrimaryAttack()
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	if ( m_bBurstMode )
	{
		if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
			GlockFire( 1.2f * (1 - m_flAccuracy), true );
		
		else if (pPlayer->GetAbsVelocity().Length2D() > 5)
			GlockFire( 0.185f * (1 - m_flAccuracy), true );
		
		else if ( FBitSet( pPlayer->GetFlags(), FL_DUCKING ) )
			GlockFire( 0.095f * (1 - m_flAccuracy), true );
		
		else
		    GlockFire( 0.3f * (1 - m_flAccuracy), true );
	}
	else
	{
		if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
			GlockFire( 1.0f * (1 - m_flAccuracy), false );
		
		else if (pPlayer->GetAbsVelocity().Length2D() > 5)
			GlockFire( 0.165f * (1 - m_flAccuracy), false );
		
		else if ( FBitSet( pPlayer->GetFlags(), FL_DUCKING ) )
			GlockFire( 0.075f * (1 - m_flAccuracy), false );
		
		else
		    GlockFire( 0.1f * (1 - m_flAccuracy), false );
	}
}


// GOOSEMAN : FireRemaining used by Glock18
void CWeaponGlock::FireRemaining( int &shotsFired, float &shootTime )
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		Error( "!pPlayer" );

	float nexttime = 0.1;
	m_iClip1--;
	if (m_iClip1 < 0)
	{
		m_iClip1 = 0;
		shotsFired = 3;
		shootTime = 0.0f;
		return;
	}

	// TODO FIXME damage = 18, rangemode 0.9
	
	// Dispatch the FX right away with full accuracy.
	FX_FireBullets( 
		pPlayer->entindex(),
		pPlayer->Weapon_ShootPosition(), 
		pPlayer->EyeAngles() + 2.0f * pPlayer->GetPunchAngle(), 
		GetWeaponID(),
		Secondary_Mode,
		CBaseEntity::GetPredictionRandomSeed() & 255, // wrap it for network traffic so it's the same between client and server
		0.05 );
	
	/*
	#if defined( CLIENT_WEAPONS )
		int flag = FEV_NOTHOST;
	#else
		int flag = 0;
	#endif

	PLAYBACK_EVENT_FULL( flag, m_pPlayer->edict(), m_usFireGlock18,
		0.0, (float *)&g_vecZero, (float *)&g_vecZero, 
		vecDir.x,
		vecDir.y,
		m_pPlayer->pev->punchangle.x * 10000,
		m_pPlayer->pev->punchangle.y * 10000, 
		m_iClip1 ? 0 : 1, 0 );
	*/

	//pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	shotsFired++;

	if (shotsFired != 3)
		shootTime = gpGlobals->curtime + nexttime;
	else
		shootTime = 0.0;
}


void CWeaponGlock::ItemPostFrame()
{
	if ( m_flGlock18Shoot != 0.0 )
		FireRemaining( m_iGlock18ShotsFired, m_flGlock18Shoot );

	BaseClass::ItemPostFrame();
}


void CWeaponGlock::GlockFire( float flSpread, bool bFireBurst )
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	float flCycleTime = GetCSWpnData().m_flCycleTime;

	if ( bFireBurst )
	{
		m_iGlock18ShotsFired = 0;
		flCycleTime = 0.5f;
	}
	else
	{
		pPlayer->m_iShotsFired++;

		if (pPlayer->m_iShotsFired > 1)
			return;
	}

	// Mark the time of this shot and determine the accuracy modifier based on the last shot fired...
	m_flAccuracy -= (0.275)*(0.325 - (gpGlobals->curtime - m_flLastFire));

	if (m_flAccuracy > 0.9)
		m_flAccuracy = 0.9;
	else if (m_flAccuracy < 0.6)
		m_flAccuracy = 0.6;

	m_flLastFire = gpGlobals->curtime;
	
	if (m_iClip1 <= 0)
	{
		if (m_bFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.2;
		}

		return;
	}

	m_iClip1--;

	pPlayer->DoMuzzleFlash();

	//SetPlayerShieldAnim();
	
	// player "shoot" animation
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	// non-silenced
	//pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	//pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	FX_FireBullets( 
		pPlayer->entindex(),
		pPlayer->Weapon_ShootPosition(), 
		pPlayer->EyeAngles() + 2.0f * pPlayer->GetPunchAngle(), 
		GetWeaponID(),
		Primary_Mode,
		CBaseEntity::GetPredictionRandomSeed() & 255, // wrap it for network traffic so it's the same between client and server
		flSpread );

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->curtime + flCycleTime;

	if (!m_iClip1 && pPlayer->GetAmmoCount( m_iPrimaryAmmoType ) <= 0)
	{
		// HEV suit - indicate out of ammo condition
		pPlayer->SetSuitUpdate("!HEV_AMO0", false, 0);
	}

	SetWeaponIdleTime( gpGlobals->curtime + 2.5 );

	if (bFireBurst)
	{
		// Fire off the next two rounds
		m_flGlock18Shoot = gpGlobals->curtime + 0.1;
		m_iGlock18ShotsFired++;

		SendWeaponAnim( ACT_VM_SECONDARYATTACK );
	}
	else
	{
		SendWeaponAnim( ACT_VM_PRIMARYATTACK );
	}

	//ResetPlayerShieldAnim();
}


bool CWeaponGlock::Reload()
{
	if ( m_flGlock18Shoot != 0 )
		return true;

	if ( !DefaultPistolReload() )
		return false;

	m_flAccuracy = 0.9;
	return true;
}

void CWeaponGlock::WeaponIdle()
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	if (m_flTimeWeaponIdle > gpGlobals->curtime)
		return;

	if ( pPlayer->HasShield() )
	{
		SetWeaponIdleTime( gpGlobals->curtime + 20 );
				
		//MIKETODO: shields
		//if ( FBitSet(m_iWeaponState, WPNSTATE_SHIELD_DRAWN) )
		//	 SendWeaponAnim( GLOCK18_SHIELD_IDLE, UseDecrement() ? 1:0 );
	}
	else
	{
		// only idle if the slid isn't back
		if (m_iClip1 != 0)
		{
			SendWeaponAnim( ACT_VM_IDLE );
		}
	}
}
