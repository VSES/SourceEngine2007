//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Engineer's Sentrygun
//
// $NoKeywords: $
//=============================================================================//

#ifndef TF_OBJ_SENTRYGUN_H
#define TF_OBJ_SENTRYGUN_H
#ifdef _WIN32
#pragma once
#endif

#include "tf_obj.h"
#include "tf_projectile_rocket.h"

class CTFPlayer;

enum
{
	SENTRY_LEVEL_1 = 0,
	// SENTRY_LEVEL_1_UPGRADING,
	SENTRY_LEVEL_2,
	// SENTRY_LEVEL_2_UPGRADING,
	SENTRY_LEVEL_3,
};

// ------------------------------------------------------------------------ //
// Sentrygun object that's built by the player
// ------------------------------------------------------------------------ //
class CObjectSentrygun : public CBaseObject
{
	DECLARE_CLASS( CObjectSentrygun, CBaseObject );

public:
	DECLARE_SERVERCLASS();

	CObjectSentrygun();

	static CObjectSentrygun* Create(const Vector &vOrigin, const QAngle &vAngles);

	virtual void	Spawn();
	virtual void	Precache();
	virtual void	OnGoActive( void );
	virtual int		DrawDebugTextOverlays(void);
	virtual int		OnTakeDamage( const CTakeDamageInfo &info );
	virtual void	Killed( const CTakeDamageInfo &info );
	virtual void	SetModel( const char *pModel );

	virtual bool	StartBuilding( CBaseEntity *pBuilder );
	virtual void	StartPlacement( CTFPlayer *pPlayer );

	// Engineer hit me with a wrench
	virtual bool	OnWrenchHit( CTFPlayer *pPlayer );

	virtual void	OnStartDisabled( void );
	virtual void	OnEndDisabled( void );

	virtual int		GetTracerAttachment( void );

	void			UpgradeThink( void );
	virtual bool	IsUpgrading( void ) const;

	int				GetUpgradeLevel( void ) { return m_iUpgradeLevel; }

private:

	// Main think
	void SentryThink( void );

	// If the players hit us with a wrench, should we upgrade
	bool CanBeUpgraded( CTFPlayer *pPlayer );
	void StartUpgrading( void );
	void FinishUpgrading( void );

	// Target acquisition
	bool FindTarget( void );
	bool ValidTargetPlayer( CTFPlayer *pPlayer, const Vector &vecStart, const Vector &vecEnd );
	bool ValidTargetObject( CBaseObject *pObject, const Vector &vecStart, const Vector &vecEnd );
	void FoundTarget( CBaseEntity *pTarget, const Vector &vecSoundCenter );
	bool FInViewCone ( CBaseEntity *pEntity );
	int Range( CBaseEntity *pTarget );

	// Rotations
	void SentryRotate( void );
	bool MoveTurret( void );

	// Attack
	void Attack( void );
	bool Fire( void );
	void MakeTracer( const Vector &vecTracerSrc, const trace_t &tr, int iTracerType );

	int GetBaseTurnRate( void );
	
private:
	CNetworkVar( int, m_iState );

	float m_flNextAttack;

	// Upgrade Level ( 1, 2, 3 )
	CNetworkVar( int, m_iUpgradeLevel );

	// Rotation
	int m_iRightBound;
	int m_iLeftBound;
	int	m_iBaseTurnRate;
	bool m_bTurningRight;

	QAngle m_vecCurAngles;
	QAngle m_vecGoalAngles;

	float m_flTurnRate;

	// Time when the upgrade animation will complete
	float m_flUpgradeCompleteTime;

	CNetworkVar( int, m_iUpgradeMetal );
	CNetworkVar( int, m_iUpgradeMetalRequired );

	// Ammo
	CNetworkVar( int, m_iAmmoShells );
	CNetworkVar( int, m_iMaxAmmoShells );
	CNetworkVar( int, m_iAmmoRockets );
	CNetworkVar( int, m_iMaxAmmoRockets );

	int	m_iAmmoType;

	float m_flNextRocketAttack;

	// Target player / object
	CHandle<CBaseEntity> m_hEnemy;

	//cached attachment indeces
	int m_iAttachments[4];

	int m_iPitchPoseParameter;
	int m_iYawPoseParameter;

	float m_flLastAttackedTime;

	float m_flHeavyBulletResist;

	int m_iPlacementBodygroup;

	DECLARE_DATADESC();
};

// sentry rocket class just to give it a unique class name, so we can distinguish it from other rockets
class CTFProjectile_SentryRocket : public CTFProjectile_Rocket
{
public:
	DECLARE_CLASS( CTFProjectile_SentryRocket, CTFProjectile_Rocket );
	DECLARE_NETWORKCLASS();

	CTFProjectile_SentryRocket();

	// Creation.
	static CTFProjectile_SentryRocket *Create( const Vector &vecOrigin, const QAngle &vecAngles, CBaseEntity *pOwner = NULL, CBaseEntity *pScorer = NULL );	

	virtual void Spawn();
};

#endif // TF_OBJ_SENTRYGUN_H
