//====== Copyright © 1996-2003, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef C_DOD_BASEGRENADE_H
#define C_DOD_BASEGRENADE_H
#ifdef _WIN32
#pragma once
#endif

#include "basegrenade_shared.h"

class C_DODBaseGrenade : public CBaseGrenade
{
public:
	DECLARE_CLASS( C_DODBaseGrenade, CBaseGrenade );
	DECLARE_NETWORKCLASS(); 

	virtual void Spawn();

public:
			C_DODBaseGrenade() {}
			C_DODBaseGrenade( const C_DODBaseGrenade& ) {}
	virtual ~C_DODBaseGrenade();

	virtual int DrawModel( int flags );
	virtual void PostDataUpdate( DataUpdateType_t type );

	virtual const char *GetOverviewSpriteName( void );

	float m_flSpawnTime;

	// This gets sent to the client and placed in the client's interpolation history
	// so the projectile starts out moving right off the bat.
	CNetworkVector( m_vInitialVelocity );
};


class C_DODRifleGrenadeUS : public C_DODBaseGrenade
{
public:
	DECLARE_CLASS( C_DODRifleGrenadeUS, C_DODBaseGrenade );
	DECLARE_NETWORKCLASS();

	virtual const char *GetOverviewSpriteName( void )
	{
		return "sprites/minimap_icons/minimap_riflegren_us";
	}
};

class C_DODRifleGrenadeGER : public C_DODBaseGrenade
{
public:
	DECLARE_CLASS( C_DODRifleGrenadeGER, C_DODBaseGrenade );
	DECLARE_CLIENTCLASS();

	virtual const char *GetOverviewSpriteName( void )
	{
		return "sprites/minimap_icons/minimap_riflegren_ger";
	}
};

#endif // C_DOD_BASEGRENADE_H
