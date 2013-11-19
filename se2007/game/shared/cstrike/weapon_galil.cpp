//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_csbasegun.h"


#if defined( CLIENT_DLL )

	#define CWeaponGalil C_WeaponGalil
	#include "c_cs_player.h"

#else

	#include "cs_player.h"

#endif


class CWeaponGalil : public CWeaponCSBaseGun
{
public:
	DECLARE_CLASS( CWeaponGalil, CWeaponCSBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CWeaponGalil();

	virtual void PrimaryAttack();

	virtual CSWeaponID GetWeaponID( void ) const		{ return WEAPON_GALIL; }

private:

	CWeaponGalil( const CWeaponGalil & );

	void GalilFire( float flSpread );

};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponGalil, DT_WeaponGalil )

BEGIN_NETWORK_TABLE( CWeaponGalil, DT_WeaponGalil )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponGalil )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_galil, CWeaponGalil );
PRECACHE_WEAPON_REGISTER( weapon_galil );



CWeaponGalil::CWeaponGalil()
{
}


void CWeaponGalil::PrimaryAttack()
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
	
	if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
		GalilFire( 0.04f + 0.3f * m_flAccuracy );
	else if (pPlayer->GetAbsVelocity().Length2D() > 140)
		GalilFire( 0.04f + 0.07f * m_flAccuracy );
	else
		GalilFire( 0.0375f * m_flAccuracy );
}

void CWeaponGalil::GalilFire( float flSpread )
{
	if ( !CSBaseGunFire( flSpread, GetCSWpnData().m_flCycleTime, true ) )
		return;

	CCSPlayer *pPlayer = GetPlayerOwner();

	// CSBaseGunFire can kill us, forcing us to drop our weapon, if we shoot something that explodes
	if ( !pPlayer )
		return;

	if (pPlayer->GetAbsVelocity().Length2D() > 5)
		pPlayer->KickBack (1.0, 0.45, 0.28, 0.045, 3.75, 3, 7);
	else if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
		pPlayer->KickBack (1.2, 0.5, 0.23, 0.15, 5.5, 3.5, 6);
	else if ( FBitSet( pPlayer->GetFlags(), FL_DUCKING ) )
		pPlayer->KickBack (0.6, 0.3, 0.2, 0.0125, 3.25, 2, 7);
	else
		pPlayer->KickBack (0.65, 0.35, 0.25, 0.015, 3.5, 2.25, 7);
}


