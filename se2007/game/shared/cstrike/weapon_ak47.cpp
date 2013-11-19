//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_csbasegun.h"


#if defined( CLIENT_DLL )

	#define CAK47 C_AK47
	#include "c_cs_player.h"

#else

	#include "cs_player.h"

#endif


class CAK47 : public CWeaponCSBaseGun
{
public:
	DECLARE_CLASS( CAK47, CWeaponCSBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CAK47();

	virtual void PrimaryAttack();

	virtual CSWeaponID GetWeaponID( void ) const		{ return WEAPON_AK47; }

private:

	void AK47Fire( float flSpread );

	CAK47( const CAK47 & );
};

IMPLEMENT_NETWORKCLASS_ALIASED( AK47, DT_WeaponAK47 )

BEGIN_NETWORK_TABLE( CAK47, DT_WeaponAK47 )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CAK47 )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_ak47, CAK47 );
PRECACHE_WEAPON_REGISTER( weapon_ak47 );

// ---------------------------------------------------------------------------- //
// CAK47 implementation.
// ---------------------------------------------------------------------------- //

CAK47::CAK47()
{
}

void CAK47::AK47Fire( float flSpread )
{
	if ( !CSBaseGunFire( flSpread, GetCSWpnData().m_flCycleTime, true ) )
		return;

	CCSPlayer *pPlayer = GetPlayerOwner();

	// CSBaseGunFire can kill us, forcing us to drop our weapon, if we shoot something that explodes
	if ( !pPlayer )
		return;

	if (pPlayer->GetAbsVelocity().Length2D() > 5 )
		pPlayer->KickBack ( 1.5, 0.45, 0.225, 0.05, 6.5, 2.5, 7 );
	else if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
		pPlayer->KickBack ( 2, 1.0, 0.5, 0.35, 9, 6, 5 );
	else if ( FBitSet( pPlayer->GetFlags(), FL_DUCKING ) )
		pPlayer->KickBack ( 0.9, 0.35, 0.15, 0.025, 5.5, 1.5, 9 );
	else
		pPlayer->KickBack ( 1, 0.375, 0.175, 0.0375, 5.75, 1.75, 8 );
}


void CAK47::PrimaryAttack()
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;
	
	if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
		AK47Fire( 0.04f + 0.4f * m_flAccuracy );
	else if (pPlayer->GetAbsVelocity().Length2D() > 140)
		AK47Fire( 0.04f + 0.07f * m_flAccuracy );
	else
		AK47Fire( 0.0275f * m_flAccuracy );
}

