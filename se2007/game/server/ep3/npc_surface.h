//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#ifndef NPC_SURFACE_H
#define NPC_SURFACE_H

#if defined( _WIN32 )
#pragma once
#endif

#include "AI_BaseNPC.h"
#include "soundenvelope.h"
#include "player_pickup.h"

class CNPC_Surface;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

#define			MAX_SURFACE_ELEMENTS 1000 // 200

class CNPC_Surface;

#include "../common/blobulator/Physics/PhysParticleCache_inl.h"
#include "../common/blobulator/Physics/PhysTiler.h"


class CLennardJonesForce
{
public:
	CLennardJonesForce( );
	~CLennardJonesForce( );

public:
	void InitParams( void )
	{
	}

	virtual void AddForces( 
		IPhysicsObject **pObjects, 
		int nObjects, 
		float flRadius,
		float flStrength,
		Vector *pForces );

	// TODO: Have to destroy PhysTiler in destructor somewhere!!!!

	//ParticleCache* m_pParticleCache;
	PhysTiler* m_pPhysTiler;
	float m_fInteractionRadius;
	float m_fSurfaceTension;
	float m_fLennardJonesRepulsion;
	float m_fLennardJonesAttraction;
	float m_fMaxRepulsion;
	float m_fMaxAttraction;

private:
	virtual void addParticleForce(ImpParticle* a, ImpParticle* b, float distSq, float flStrength, float ts) const;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------


class CNPC_Surface : public CAI_BaseNPC, public CDefaultPlayerPickupVPhysics
{
	DECLARE_CLASS( CNPC_Surface, CAI_BaseNPC );
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

public:
	CNPC_Surface()
	{
	}

	void		Spawn( void );
	void		Precache( void );
	void		Activate( void );

	void		OnSave( IEntitySaveUtils *pUtils );
	void		OnRestore( void );

	Class_T		Classify( void );

	virtual void VPhysicsCollision( int index, gamevcollisionevent_t *pEvent );
	virtual bool CreateVPhysics();
	virtual bool CreateVPhysics( bool bFromRestore );
	virtual int	 VPhysicsGetObjectList( IPhysicsObject **pList, int listMax );
	virtual bool VPhysicsIsFlesh( void );

	// Player pickup
	virtual void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );
	virtual void OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t Reason );
	virtual bool OnAttemptPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );

	virtual int  OnTakeDamage_Alive( const CTakeDamageInfo &info );
	virtual bool TestCollision( const Ray_t &ray, unsigned int fContentsMask, trace_t& tr );

	void		RunAI( void );

	//void		DoArmTest( void );
	//void		DoFountain( void );

	float		MaxYawSpeed( void );
	int			TranslateSchedule( int scheduleType );
	int			SelectSchedule( void );

	void		PrescheduleThink( void );

	void        HandleAnimEvent( animevent_t *pEvent );

	void		StartTask( const Task_t *pTask );
	void		RunTask( const Task_t *pTask );

	virtual void ApplyDamageForce( const CTakeDamageInfo &info );

	Vector		m_vecStart;

	CUtlVector< Vector > m_vecSurfacePos;
	CUtlVector< float > m_flSurfaceV;
	CUtlVector< float > m_flSurfaceR;

	IPhysicsObject	*m_pSpheres[ MAX_SURFACE_ELEMENTS ];

	// keep track of the sphere being in contact with a surface
	bool m_bContact[ MAX_SURFACE_ELEMENTS ];

	CNetworkVar( int, m_nActiveParticles );
	CNetworkVar( float, m_flRadius );

public:
	virtual void Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular ) { };

	virtual Vector		EyePosition( void );
	virtual const QAngle &EyeAngles( void );

	virtual	Vector		BodyTarget( const Vector &posSrc, bool bNoisy) ;

	// --------------------------------

	// DEFINE_CUSTOM_AI;

private:
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

class CBlobFountainController;

class CNPC_BlobFountain : public CNPC_Surface
{
	DECLARE_CLASS( CNPC_BlobFountain, CNPC_Surface );
	// DECLARE_DATADESC();

public:
	bool		CreateVPhysics( bool bFromRestore );
	void		RunAI( void );
	const Vector &GetNozzle()	{ return m_vecStart; };

	void Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular );

	CLennardJonesForce			m_force;
	CBlobFountainController		*m_pBlobFountainController;
	IPhysicsMotionController	*m_pMotionController;
	
	float m_iContactTime[ MAX_SURFACE_ELEMENTS ];
	float m_fRadius[ MAX_SURFACE_ELEMENTS ];
	int m_iMode[ MAX_SURFACE_ELEMENTS ];
	bool m_bPause;

	// --------------------------------

	// DEFINE_CUSTOM_AI;

private:
};

class CBlobFountainController : public IMotionEvent
{
public:
	DECLARE_SIMPLE_DATADESC();

	CBlobFountainController( CNPC_BlobFountain *pOwner ) { m_pOwner = pOwner; };

	IMotionEvent::simresult_e Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular );

	CNPC_BlobFountain *m_pOwner;
	bool		m_bActive;

	Vector m_vecLennardJonesForce[ MAX_SURFACE_ELEMENTS ];
	float		m_flLennardJonesTime;
};




//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------



class CNPC_BlobArmTest : public CNPC_Surface
{
	DECLARE_CLASS( CNPC_BlobArmTest, CNPC_Surface );
	DECLARE_DATADESC();

public:
	void		Spawn( void );
	bool		CreateVPhysics( bool bFromRestore );
	void		RunAI( void );
	int			MoveTowardsGoal( void );

	int m_nOwnedSlot[ MAX_SURFACE_ELEMENTS ];
	int m_nTargetSlot[ MAX_SURFACE_ELEMENTS ];
	bool m_bFloat[ MAX_SURFACE_ELEMENTS ];
	int m_nArm[ MAX_SURFACE_ELEMENTS ];

	bool		m_bDoArms;
	EHANDLE		m_hTarget;
	float		m_flSimTime;
};


//-----------------------------------------------------------------------------



class CNPC_BlobDemoMonster : public CNPC_Surface
{
	DECLARE_CLASS( CNPC_BlobDemoMonster, CNPC_Surface );
	DECLARE_DATADESC();

public:
	void		Spawn( void );
	bool		CreateVPhysics( bool bFromRestore );
	int			OnTakeDamage_Alive( const CTakeDamageInfo &info );
	void		RunAI( void );
	void		MoveTowardsGoal( void );
	void		CreateArms( const Vector &vecForward );
	void		RepulseNeighbors( void );

	// Input handlers
	void InputMoveToPosition( inputdata_t &inputdata );
	void InputDoArms( inputdata_t &inputdata );
	void InputDoContactZ( inputdata_t &inputdata );

	int m_nOwnedSlot[ MAX_SURFACE_ELEMENTS ];
	int m_nTargetSlot[ MAX_SURFACE_ELEMENTS ];
	bool m_bFloat[ MAX_SURFACE_ELEMENTS ];
	int m_nArm[ MAX_SURFACE_ELEMENTS ];

	Vector		m_vecGoal;
	Vector		m_vecPrevGoal;

	bool		m_bDoArms;
	bool		m_bDoContactZ;
	EHANDLE		m_hTarget;
	float		m_flSimTime;
};


//-----------------------------------------------------------------------------



#endif // NPC_SURFACE_H
