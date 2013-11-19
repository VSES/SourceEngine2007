//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_csbasegun.h"
#include "fx_cs_shared.h"


#if defined( CLIENT_DLL )

	#define CWeaponFamas C_WeaponFamas
	#include "c_cs_player.h"

#else

	#include "cs_player.h"

#endif


class CWeaponFamas : public CWeaponCSBaseGun
{
public:
	DECLARE_CLASS( CWeaponFamas, CWeaponCSBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CWeaponFamas();

	virtual void PrimaryAttack();
	virtual void SecondaryAttack();

	virtual void ItemPostFrame();

	void FamasFire( float flSpread, bool bFireBurst );
	void FireRemaining( int &shotsFired, float &shootTime );
	
	virtual CSWeaponID GetWeaponID( void ) const		{ return WEAPON_FAMAS; }

private:
	
	CWeaponFamas( const CWeaponFamas & );
	CNetworkVar( bool, m_bBurstMode );
	float	m_flFamasShoot;			// time to shoot the remaining bullets of the famas burst fire
	int		m_iFamasShotsFired;		// used to keep track of the shots fired during the Famas burst fire mode....
	float	m_fBurstSpread;			// used to keep track of current spread factor so that all bullets in spread use same spread
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponFamas, DT_WeaponFamas )

BEGIN_NETWORK_TABLE( CWeaponFamas, DT_WeaponFamas )
	#ifdef CLIENT_DLL
		RecvPropBool( RECVINFO( m_bBurstMode ) )
	#else
		SendPropBool( SENDINFO( m_bBurstMode ) )
	#endif
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponFamas )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_famas, CWeaponFamas );
PRECACHE_WEAPON_REGISTER( weapon_famas );



CWeaponFamas::CWeaponFamas()
{
	m_bBurstMode = false;
}

// Secondary attack could be three-round burst mode
void CWeaponFamas::SecondaryAttack()
{	
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	if ( m_bBurstMode )
	{
		ClientPrint( pPlayer, HUD_PRINTCENTER, "#Switch_To_FullAuto" );
		m_bBurstMode = false;
	}
	else
	{
		ClientPrint( pPlayer, HUD_PRINTCENTER, "#Switch_To_BurstFire" );
		m_bBurstMode = true;
	}
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.3;
}

void CWeaponFamas::PrimaryAttack()
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	// don't fire underwater
	if (pPlayer->GetWaterLevel() == 3)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = gpGlobals->curtime + 0.15;
		return;
	}
	
	if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )	// if player is in air
		FamasFire( 0.03f + 0.3f * m_flAccuracy, m_bBurstMode );
	
	else if ( pPlayer->GetAbsVelocity().Length2D() > 140 )	// if player is moving
		FamasFire( 0.03f + 0.07f * m_flAccuracy, m_bBurstMode );
	/* new code */
	else
		FamasFire( 0.02f * m_flAccuracy, m_bBurstMode );
}


// GOOSEMAN : FireRemaining used by Glock18
void CWeaponFamas::FireRemaining( int &shotsFired, float &shootTime )
{
	float nexttime = 0.1;

	m_iClip1--;

	if (m_iClip1 < 0)
	{
		m_iClip1 = 0;
		shotsFired = 3;
		shootTime = 0.0f;
		return;
	}

	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		Error( "!pPlayer" );

	// Famas burst mode
	FX_FireBullets(
		pPlayer->entindex(),
		pPlayer->Weapon_ShootPosition(),
		pPlayer->EyeAngles() + 2.0f * pPlayer->GetPunchAngle(),
		GetWeaponID(),
		Secondary_Mode,
		CBaseEntity::GetPredictionRandomSeed() & 255,
		m_fBurstSpread );
	

	SendWeaponAnim( ACT_VM_PRIMARYATTACK );

	pPlayer->DoMuzzleFlash();
	pPlayer->SetAnimation( PLAYER_ATTACK1 );

	shotsFired++;

	if (shotsFired != 3)
		shootTime = gpGlobals->curtime + nexttime;
	else
		shootTime = 0.0;
}


void CWeaponFamas::ItemPostFrame()
{
	if (m_flFamasShoot != 0.0 && m_flFamasShoot < gpGlobals->curtime )
		FireRemaining(m_iFamasShotsFired, m_flFamasShoot );

	BaseClass::ItemPostFrame();
}


void CWeaponFamas::FamasFire( float flSpread , bool bFireBurst )
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	float flCycleTime = GetCSWpnData().m_flCycleTime;

	// change a few things if we're in burst mode
	if (bFireBurst)
	{
		m_iFamasShotsFired = 0;
		flCycleTime = 0.55f;
	}
	else
	{
		flSpread += 0.01;
	}

	if ( !CSBaseGunFire( flSpread, flCycleTime, true ) )
		return;
	
	if ( pPlayer->GetAbsVelocity().Length2D() > 5 )
		pPlayer->KickBack ( 1, 0.45, 0.275, 0.05, 4, 2.5, 7 );
	
	else if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
		pPlayer->KickBack ( 1.25, 0.45, 0.22, 0.18, 5.5, 4, 5 );
	
	else if ( FBitSet( pPlayer->GetFlags(), FL_DUCKING ) )
		pPlayer->KickBack ( 0.575, 0.325, 0.2, 0.011, 3.25, 2, 8 );
	
	else
		pPlayer->KickBack ( 0.625, 0.375, 0.25, 0.0125, 3.5, 2.25, 8 );

	if (bFireBurst)
	{
		// Fire off the next two rounds
		m_flFamasShoot = gpGlobals->curtime + 0.05;	// 0.1
		m_fBurstSpread = flSpread;
		m_iFamasShotsFired++;
	}
}


