//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"  
#include "shake.h" 
#include "weapon_dodbasegun.h"


#if defined( CLIENT_DLL )
	#define CDODSemiAutoWeapon C_DODSemiAutoWeapon
#endif

class CDODSemiAutoWeapon : public CWeaponDODBaseGun
{
public:
	DECLARE_CLASS( CDODSemiAutoWeapon, CWeaponDODBaseGun );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CDODSemiAutoWeapon();

	virtual void Spawn();
	virtual void PrimaryAttack( void );

	virtual DODWeaponID GetWeaponID( void ) const { return WEAPON_NONE; }

private:
	CDODSemiAutoWeapon( const CDODSemiAutoWeapon & );
};