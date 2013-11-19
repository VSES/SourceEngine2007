//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "weapon_csbasegun.h"


#if defined( CLIENT_DLL )

	#define CWeaponAWP C_WeaponAWP
	#include "c_cs_player.h"

#else

	#include "cs_player.h"
	#include "KeyValues.h"

#endif

#define SNIPER_ZOOM_CONTEXT		"SniperRifleThink"


#ifdef AWP_UNZOOM
	ConVar sv_awpunzoomdelay( 
			"sv_awpunzoomdelay",
			"1.0",
			0,
			"how many seconds to zoom the zoom up after firing",
			true, 0,	// min value
			false, 0	// max value
			);
#endif


class CWeaponAWP : public CWeaponCSBaseGun
{
public:
	DECLARE_CLASS( CWeaponAWP, CWeaponCSBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif
	
	CWeaponAWP();

	virtual void Spawn();

	virtual void PrimaryAttack();
	virtual void SecondaryAttack();

	virtual void AWPFire( float flSpread );

	virtual float GetMaxSpeed() const;
	virtual bool IsAwp() const;

	virtual CSWeaponID GetWeaponID( void ) const		{ return WEAPON_AWP; }

private:

#ifndef CLIENT_DLL
	void				UnzoomThink( void );
#endif

	CWeaponAWP( const CWeaponAWP & );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponAWP, DT_WeaponAWP )

BEGIN_NETWORK_TABLE( CWeaponAWP, DT_WeaponAWP )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponAWP )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_awp, CWeaponAWP );
PRECACHE_WEAPON_REGISTER( weapon_awp );

#ifndef CLIENT_DLL

	BEGIN_DATADESC( CWeaponAWP )
		DEFINE_THINKFUNC( UnzoomThink ),
	END_DATADESC()

#endif

CWeaponAWP::CWeaponAWP()
{
}

void CWeaponAWP::Spawn()
{
	Precache();

	BaseClass::Spawn();
}


void CWeaponAWP::SecondaryAttack()
{
	#ifndef CLIENT_DLL
		CCSPlayer *pPlayer = GetPlayerOwner();

		if (pPlayer == NULL)
		{
			Assert(pPlayer != NULL);
			return;
		}

		if ( pPlayer->GetFOV() == pPlayer->GetDefaultFOV() )
		{
			pPlayer->SetFOV( pPlayer, 40, 0.15f );
		}
		else if ( pPlayer->GetFOV() == 40 )
		{
			pPlayer->SetFOV( pPlayer, 10, 0.08f );
		}
		else
		{
			pPlayer->SetFOV( pPlayer, pPlayer->GetDefaultFOV(), 0.1f );
		}

		pPlayer->ResetMaxSpeed();
		
	#endif

#ifndef CLIENT_DLL
	// If this isn't guarded, the sound will be emitted twice, once by the server and once by the client.
	// Let the server play it since if only the client plays it, it's liable to get played twice cause of
	// a prediction error. joy.
	EmitSound( "Default.Zoom" );

	// let the bots hear the rifle zoom
	IGameEvent * event = gameeventmanager->CreateEvent( "weapon_zoom" );
	if ( event )
	{
		event->SetInt( "userid", pPlayer->GetUserID() );
		gameeventmanager->FireEvent( event );
	}
#endif

	m_flNextSecondaryAttack = gpGlobals->curtime + 0.3;   
	m_zoomFullyActiveTime = gpGlobals->curtime + 0.15; // The worst zoom time from above.  

}


void CWeaponAWP::PrimaryAttack()
{
	CCSPlayer *pPlayer = GetPlayerOwner();
	if ( !pPlayer )
		return;

	if ( !FBitSet( pPlayer->GetFlags(), FL_ONGROUND ) )
		AWPFire( 0.85 );
	
	else if ( pPlayer->GetAbsVelocity().Length2D() > 140 )
		AWPFire( 0.25 );
	
	else if ( pPlayer->GetAbsVelocity().Length2D() > 10 )
		AWPFire( 0.10 );
	
	else if ( FBitSet( pPlayer->GetFlags(), FL_DUCKING ) )
		AWPFire( 0.0 );
	
	else
		AWPFire( 0.001 );
}

void CWeaponAWP::AWPFire( float flSpread )
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
		flSpread += 0.08;
	}

	if (pPlayer->GetFOV() != pPlayer->GetDefaultFOV())
	{	
		pPlayer->m_iLastZoom = pPlayer->GetFOV();
		
		#ifndef CLIENT_DLL
			#ifdef AWP_UNZOOM
				SetContextThink( &CWeaponAWP::UnzoomThink, gpGlobals->curtime + sv_awpunzoomdelay.GetFloat(), SNIPER_ZOOM_CONTEXT );
			#else
				pPlayer->m_bResumeZoom = true;
				pPlayer->SetFOV( pPlayer, pPlayer->GetDefaultFOV(), 0.1f );
			#endif
		#endif
	}

	if ( !CSBaseGunFire( flSpread, GetCSWpnData().m_flCycleTime, true ) )
		return;

	QAngle angle = pPlayer->GetPunchAngle();
	angle.x -= 2;
	pPlayer->SetPunchAngle( angle );
}

#ifndef CLIENT_DLL
void CWeaponAWP::UnzoomThink( void )
{
	CCSPlayer *pPlayer = GetPlayerOwner();

	if (pPlayer == NULL)
	{
		Assert(pPlayer != NULL);
		return;
	}

	pPlayer->SetFOV( pPlayer, pPlayer->GetDefaultFOV(), 0.1f );
}
#endif


float CWeaponAWP::GetMaxSpeed() const
{
	CCSPlayer *pPlayer = GetPlayerOwner();

	if (pPlayer == NULL)
	{
		Assert(pPlayer != NULL);
		return BaseClass::GetMaxSpeed();
	}

	if ( pPlayer->GetFOV() == pPlayer->GetDefaultFOV() )
	{
		return BaseClass::GetMaxSpeed();
	}
	else
	{
		// Slower speed when zoomed in.
		return 150;
	}
}


bool CWeaponAWP::IsAwp() const
{
	return true;
}

