//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef C_OBJ_SENTRYGUN_H
#define C_OBJ_SENTRYGUN_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseobject.h"
#include "ObjectControlPanel.h"
#include "c_tf_projectile_rocket.h"

class C_MuzzleFlashModel;

//-----------------------------------------------------------------------------
// Purpose: Sentry object
//-----------------------------------------------------------------------------
class C_ObjectSentrygun : public C_BaseObject
{
	DECLARE_CLASS( C_ObjectSentrygun, C_BaseObject );
public:
	DECLARE_CLIENTCLASS();

	C_ObjectSentrygun();

	int GetUpgradeLevel( void ) { return m_iUpgradeLevel; }

	void GetAmmoCount( int &iShells, int &iMaxShells, int &iRockets, int & iMaxRockets );

	void GetStatusText( wchar_t *pStatus, int iMaxStatusLen );

	virtual bool	IsUpgrading( void ) const;

	virtual void GetTargetIDString( wchar_t *sIDString, int iMaxLenInBytes );
	virtual void GetTargetIDDataString( wchar_t *sDataString, int iMaxLenInBytes );

	virtual BuildingHudAlert_t GetBuildingAlertLevel( void );

	int GetUpgradeMetal( void ) { return m_iUpgradeMetal; }
	int GetUpgradeMetalRequired( void ) { return SENTRYGUN_UPGRADE_METAL; }

	virtual const char *GetHudStatusIcon( void );

	int GetKills( void ) { return m_iKills; }

	virtual void GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType );

	virtual CStudioHdr *OnNewModel( void );
	virtual void UpdateDamageEffects( BuildingDamageLevel_t damageLevel );

	virtual void OnPlacementStateChanged( bool bValidPlacement );

	void DebugDamageParticles();

	virtual void	OnPreDataChanged( DataUpdateType_t updateType );
	virtual void	OnDataChanged( DataUpdateType_t updateType );

	// ITargetIDProvidesHint
public:
	virtual void	DisplayHintTo( C_BasePlayer *pPlayer );

private:

	void UpgradeLevelChanged();

private:
	int m_iState;

	int m_iUpgradeLevel;
	int	m_iOldUpgradeLevel;

	int m_iAmmoShells;
	int m_iMaxAmmoShells;
	int m_iAmmoRockets;

	int m_iUpgradeMetal;

	int m_iKills;

	CNewParticleEffect *m_pDamageEffects;

	int m_iPlacementBodygroup;

	int m_iOldBodygroups;

private:
	C_ObjectSentrygun( const C_ObjectSentrygun & ); // not defined, not accessible
};

class C_TFProjectile_SentryRocket : public C_TFProjectile_Rocket
{
	DECLARE_CLASS( C_TFProjectile_SentryRocket, C_TFProjectile_Rocket );
public:
	DECLARE_CLIENTCLASS();

	virtual void CreateRocketTrails( void ) {}
};

#endif	//C_OBJ_SENTRYGUN_H