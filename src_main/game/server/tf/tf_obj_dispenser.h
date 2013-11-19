//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Engineer's Dispenser
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_OBJ_DISPENSER_H
#define TF_OBJ_DISPENSER_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_obj.h"

class CTFPlayer;

// ------------------------------------------------------------------------ //
// Resupply object that's built by the player
// ------------------------------------------------------------------------ //
class CObjectDispenser : public CBaseObject
{
	DECLARE_CLASS( CObjectDispenser, CBaseObject );

public:
	DECLARE_SERVERCLASS();

	CObjectDispenser();
	~CObjectDispenser();

	static CObjectDispenser* Create(const Vector &vOrigin, const QAngle &vAngles);

	virtual void	Spawn();
	virtual void	GetControlPanelInfo( int nPanelIndex, const char *&pPanelName );
	virtual void	Precache();
	virtual bool	ClientCommand( CTFPlayer *pPlayer, const CCommand &args );

	virtual void	DetonateObject( void );
	virtual void	OnGoActive( void );	
	virtual bool	StartBuilding( CBaseEntity *pBuilder );
	virtual int		DrawDebugTextOverlays(void) ;
	virtual void	SetModel( const char *pModel );

	void RefillThink( void );
	void DispenseThink( void );

	virtual void StartTouch( CBaseEntity *pOther );
	virtual void EndTouch( CBaseEntity *pOther );

	virtual int	ObjectCaps( void ) { return (BaseClass::ObjectCaps() | FCAP_IMPULSE_USE); }

	bool DispenseAmmo( CTFPlayer *pPlayer );

	void StartHealing( CBaseEntity *pOther );
	void StopHealing( CBaseEntity *pOther );

	void AddHealingTarget( CBaseEntity *pOther );
	void RemoveHealingTarget( CBaseEntity *pOther );
	bool IsHealingTarget( CBaseEntity *pTarget );

	bool CouldHealTarget( CBaseEntity *pTarget );

	Vector GetHealOrigin( void );

	CUtlVector< EHANDLE >	m_hHealingTargets;

private:

	//CNetworkArray( EHANDLE, m_hHealingTargets, MAX_DISPENSER_HEALING_TARGETS );


	// Entities currently being touched by this trigger
	CUtlVector< EHANDLE >	m_hTouchingEntities;

	CNetworkVar( int, m_iAmmoMetal );

	float m_flNextAmmoDispense;

	EHANDLE m_hTouchTrigger;

	DECLARE_DATADESC();
};

#endif // TF_OBJ_DISPENSER_H
