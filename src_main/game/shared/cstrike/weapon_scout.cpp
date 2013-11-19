//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_csbasegun.h"


#if defined( CLIENT_DLL )

	#define CWeaponScout C_WeaponScout
	#include "c_cs_player.h"

#else

	#include "cs_player.h"
	#include "KeyValues.h"

#endif


class CWeaponScout : public CWeaponCSBaseGun
{
public:
	DECLARE_CLASS( CWeaponScout, CWeaponCSBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	
	CWeaponScout();

	virtual void PrimaryAttack();
	virtual void SecondaryAttack();

	virtual float GetMaxSpeed() const;

	virtual CSWeaponID GetWeaponID( void ) const		{ return WEAPON_SCOUT; }


private:
	
	CWeaponScout( const CWeaponScout & );

	void SCOUTFire( float flSpread );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponScout, DT_WeaponScout )

BEGIN_NETWORK_TABLE( CWeaponScout, DT_WeaponScout )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponScout )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_scout, CWeaponScout );
PRECACHE_WEAPON_REGISTER( weapon_scout );



CWeaponScout::CWeaponScout()
{
}

void CWeaponScout::SecondaryAttack()
{
	#ifndef CLIENT_DLL
		CCSPlayer *pPlayer = GetPlayerOwner();
		if (pPlayer == NULL)
		{
			Assert(pPlayer != NULL);
			return;
		}

		if (pPlayer->GetFOV() == pPlayer->GetDefaultFOV())
		{
			pPlayer->SetFOV( pPlayer, 40, 0.15f );
		}
		else if (pPlayer->GetFOV() == 40)
		{
			pPlayer->SetFOV( pPlayer, 15, 0.05 );
		}
		else if (pPlayer->GetFOV() == 15)
		{
			pPlayer->SetFOV( pPlayer, pPlayer->GetDefaultFOV(), 0.05f );
		}

		pPlayer->ResetMaxSpeed();
	#endif

		m_flNextSecondaryAttack = gpGlobals->curtime + 0.3;   
		m_zoomFullyActiveTime = gpGlobals->curtime + 0.15; // The worst zoom time from above.  

#ifndef CLIENT_DLL
	// If this isn't guarded, the sound will be emitted twice, once by the server and once by the client.
	// Let the server play it since if only the client plays it, it's liable to get played twice cause of
	// a prediction error. joy.
	EmitSound( "Default.Zoom" ); // zoom sound

	// let the bots hear the rifle zoom
	IGameEvent * event = gameeventmanager->CreateEvent( "weapon_zoom" );
	if( event )
	{
		event->SetInt( "userid", pPlayer->GetUserID() );
		gameeventmanager->FireEvent( event );
	}
#endif
}

void CWeaponScout::PrimaryAttack( void )
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if (pPlayer == NULL)
	{
		Assert(pPlayer != NULL);
		return;
	}

	if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
		SCOUTFire( 0.2 );
	else if (pPlayer->GetAbsVelocity().Length2D() > 170)
		SCOUTFire( 0.075 );
	else if ( FBitSet( pPlayer->GetFlags(), FL_DUCKING ) )
		SCOUTFire( 0.0 );
	else
		SCOUTFire( 0.007 );
}

void CWeaponScout::SCOUTFire( float flSpread )
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if (pPlayer == NULL)
	{
		Assert(pPlayer != NULL);
		return;
	}

	// If we are not zoomed in, or we have very recently zoomed and are still transitioning, the bullet diverts more.
	if (pPlayer->GetFOV() == pPlayer->GetDefaultFOV() || (gpGlobals->curtime < m_zoomFullyActiveTime))
	{
		flSpread += 0.025;
	}

	if (pPlayer->GetFOV() != pPlayer->GetDefaultFOV())
	{	
		pPlayer->m_bResumeZoom = true;
		pPlayer->m_iLastZoom = pPlayer->GetFOV();
		
		#ifndef CLIENT_DLL
			pPlayer->SetFOV( pPlayer, pPlayer->GetDefaultFOV(), 0.05f );
		#endif
	}

	if ( !CSBaseGunFire( flSpread, GetCSWpnData().m_flCycleTime, true ) )
		return;

	QAngle angle = pPlayer->GetPunchAngle();
	angle.x -= 2;
	pPlayer->SetPunchAngle( angle );
}


float CWeaponScout::GetMaxSpeed() const
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if (pPlayer == NULL)
	{
		Assert(pPlayer != NULL);
		return BaseClass::GetMaxSpeed();
	}

	if ( pPlayer->GetFOV() == pPlayer->GetDefaultFOV() )
		return BaseClass::GetMaxSpeed();
	else
		return 220;	// zoomed in.
}
