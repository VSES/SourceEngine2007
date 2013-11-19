//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Upgrade that damages the object over time
//
//=============================================================================//

#ifndef TF_OBJ_SAPPER_H
#define TF_OBJ_SAPPER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_obj_baseupgrade_shared.h"

// ------------------------------------------------------------------------ //
// Sapper upgrade
// ------------------------------------------------------------------------ //
class CObjectSapper : public CBaseObjectUpgrade
{
	DECLARE_CLASS( CObjectSapper, CBaseObjectUpgrade );

public:
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

	CObjectSapper();

	virtual void	Spawn();
	virtual void	Precache();
	virtual bool	IsHostileUpgrade( void ) { return true; }
	virtual void	FinishedBuilding( void );
	virtual void	SetupAttachedVersion( void );
	virtual void	DetachObjectFromObject( void );
	virtual void	UpdateOnRemove( void );
	virtual void	OnGoActive( void );

	virtual void	SapperThink( void );

	virtual int		OnTakeDamage( const CTakeDamageInfo &info );

private:
	float m_flSapperDamageAccumulator;
	float m_flLastThinkTime;
};

#endif // TF_OBJ_SAPPER_H
