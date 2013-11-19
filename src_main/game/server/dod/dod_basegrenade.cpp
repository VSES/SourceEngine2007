//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "dod_basegrenade.h"
#include "dod_player.h"
#include "dod_gamerules.h"
#include "func_break.h"
#include "physics_saverestore.h"
#include "grenadetrail.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern ConVar sv_gravity;

ConVar dod_grenadegravity( "dod_grenadegravity", "-420", FCVAR_CHEAT, "gravity applied to grenades", true, -2000, true, -300 );
extern ConVar dod_bonusround;

IMotionEvent::simresult_e	CGrenadeController::Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular )
{
	linear.x = linear.y = 0;
	linear.z = dod_grenadegravity.GetFloat();

	angular.x = angular.y = angular.z = 0;

	return SIM_GLOBAL_ACCELERATION;
}

BEGIN_SIMPLE_DATADESC( CGrenadeController )
END_DATADESC()

BEGIN_DATADESC( CDODBaseGrenade )

	DEFINE_THINKFUNC( DetonateThink ),
	
	DEFINE_EMBEDDED( m_GrenadeController ),
	DEFINE_PHYSPTR( m_pMotionController ),	// probably not necessary
	
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CDODBaseGrenade, DT_DODBaseGrenade )
	SendPropVector( SENDINFO( m_vInitialVelocity ), 
				20,		// nbits
				0,		// flags
				-3000,	// low value
				3000	// high value
				)
END_SEND_TABLE()

CDODBaseGrenade::CDODBaseGrenade()
{
	m_hSmokeTrail = NULL;
}

CDODBaseGrenade::~CDODBaseGrenade( void )
{
	if ( m_pMotionController != NULL )
	{
		physenv->DestroyMotionController( m_pMotionController );
		m_pMotionController = NULL;
	}
}

void CDODBaseGrenade::Spawn( void )
{
	m_bUseVPhysics = true;

	BaseClass::Spawn();
	
	SetSolid( SOLID_BBOX );	// So it will collide with physics props!

	UTIL_SetSize( this, Vector(-4,-4,-4), Vector(4,4,4) );

	if( m_bUseVPhysics )
	{		 
		SetCollisionGroup( COLLISION_GROUP_WEAPON );
		IPhysicsObject *pPhysicsObject = VPhysicsInitNormal( SOLID_BBOX, 0, false );

		if ( pPhysicsObject )
		{
			m_pMotionController = physenv->CreateMotionController( &m_GrenadeController );
			m_pMotionController->AttachObject( pPhysicsObject, true );

			pPhysicsObject->EnableGravity( false );
		}
		
		m_takedamage	= DAMAGE_EVENTS_ONLY;
	}
	else
	{
		SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_CUSTOM );
		m_takedamage	= DAMAGE_NO;
	}

	AddSolidFlags( FSOLID_NOT_STANDABLE );

	m_iHealth		= 1;
	
	SetFriction( GetGrenadeFriction() );
	SetElasticity( GetGrenadeElasticity() );

	// Remember our owner's team
	ChangeTeam( GetThrower()->GetTeamNumber() );

	m_flDamage = 150;
	m_DmgRadius = m_flDamage * 2.5f;

	SetThink( &CDODBaseGrenade::DetonateThink );
	SetNextThink( gpGlobals->curtime + 0.1 );
}

void CDODBaseGrenade::CreateSmokeTrail( void )
{
	if ( m_hSmokeTrail )
		return;

	// Smoke trail.
	if ( (m_hSmokeTrail = CGrenadeTrail::CreateGrenadeTrail()) != NULL )
	{
		m_hSmokeTrail->m_SpawnRate = 60;
		m_hSmokeTrail->m_ParticleLifetime = 0.3f;
		
		m_hSmokeTrail->SetLifetime( 10 );
		m_hSmokeTrail->FollowEntity( this, "0" );
	}
}

void CDODBaseGrenade::DetonateThink( void )
{
	if (!IsInWorld())
	{
		Remove( );
		return;
	}

	Vector foo;
	AngularImpulse a;	
		
	VPhysicsGetObject()->GetVelocity( &foo, &a );

	if ( foo.LengthSqr() < ( 100*100 ) )
	{
		if ( m_hSmokeTrail.Get() )
		{
			m_hSmokeTrail->m_ParticleLifetime = 1.0;
			m_hSmokeTrail->m_SpawnRate = 30;			
		}
	}

	if( gpGlobals->curtime > m_flDetonateTime )
	{
		Detonate();
		return;
	}

	if (GetWaterLevel() != 0)
	{
		SetAbsVelocity( GetAbsVelocity() * 0.5 );
	}

	SetNextThink( gpGlobals->curtime + 0.2 );
}

//Sets the time at which the grenade will explode
void CDODBaseGrenade::SetDetonateTimerLength( float timer )
{
	m_flDetonateTime = gpGlobals->curtime + timer;
}

void CDODBaseGrenade::ResolveFlyCollisionCustom( trace_t &trace, Vector &vecVelocity )
{
	//Assume all surfaces have the same elasticity
	float flSurfaceElasticity = 1.0;

	//Don't bounce off of players with perfect elasticity
	if( trace.m_pEnt && trace.m_pEnt->IsPlayer() )
	{
		flSurfaceElasticity = 0.3;
	}
	
	float flTotalElasticity = GetElasticity() * flSurfaceElasticity;
	flTotalElasticity = clamp( flTotalElasticity, 0.0f, 0.9f );

	// NOTE: A backoff of 2.0f is a reflection
	Vector vecAbsVelocity;
	PhysicsClipVelocity( GetAbsVelocity(), trace.plane.normal, vecAbsVelocity, 2.0f );
	vecAbsVelocity *= flTotalElasticity;

	// Get the total velocity (player + conveyors, etc.)
	VectorAdd( vecAbsVelocity, GetBaseVelocity(), vecVelocity );
	float flSpeedSqr = DotProduct( vecVelocity, vecVelocity );

	// Stop if on ground.
	if ( trace.plane.normal.z > 0.7f )			// Floor
	{
		// Verify that we have an entity.
		CBaseEntity *pEntity = trace.m_pEnt;
		Assert( pEntity );

		// Are we on the ground?
		if ( vecVelocity.z < ( sv_gravity.GetFloat() * gpGlobals->frametime ) )
		{
			if ( pEntity->IsStandable() )
			{
				SetGroundEntity( pEntity );
			}

			vecAbsVelocity.z = 0.0f;
		}
		SetAbsVelocity( vecAbsVelocity );

		if ( flSpeedSqr < ( 30 * 30 ) )
		{
			if ( pEntity->IsStandable() )
			{
				SetGroundEntity( pEntity );
			}

			// Reset velocities.
			SetAbsVelocity( vec3_origin );
			SetLocalAngularVelocity( vec3_angle );
		}
		else
		{
			Vector vecDelta = GetBaseVelocity() - vecAbsVelocity;	
			Vector vecBaseDir = GetBaseVelocity();
			VectorNormalize( vecBaseDir );
			float flScale = vecDelta.Dot( vecBaseDir );

			VectorScale( vecAbsVelocity, ( 1.0f - trace.fraction ) * gpGlobals->frametime, vecVelocity ); 
			VectorMA( vecVelocity, ( 1.0f - trace.fraction ) * gpGlobals->frametime, GetBaseVelocity() * flScale, vecVelocity );
			PhysicsPushEntity( vecVelocity, &trace );
		}
	}
	else
	{
		// If we get *too* slow, we'll stick without ever coming to rest because
		// we'll get pushed down by gravity faster than we can escape from the wall.
		if ( flSpeedSqr < ( 30 * 30 ) )
		{
			// Reset velocities.
			SetAbsVelocity( vec3_origin );
			SetLocalAngularVelocity( vec3_angle );
		}
		else
		{
			SetAbsVelocity( vecAbsVelocity );
		}
	}
	
	BounceSound();
}

char *CDODBaseGrenade::GetExplodingClassname( void )
{
	Assert( !"Baseclass must implement this" );
	return NULL;
}

void CDODBaseGrenade::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( !CanBePickedUp() )
		return;

	if ( !pActivator->IsPlayer() )
		return;

	CDODPlayer *pPlayer = ToDODPlayer( pActivator );

	//Don't pick up grenades while deployed
	CBaseCombatWeapon *pWpn = pPlayer->GetActiveWeapon();
	if ( pWpn && !pWpn->CanHolster() )
	{
		return;
	}

	DODRoundState state = DODGameRules()->State_Get();

	if ( dod_bonusround.GetBool() )
	{
		int team = pPlayer->GetTeamNumber();

		// if its after the round and bonus round is on, we can only pick it up if we are winners

		if ( team == TEAM_ALLIES && state == STATE_AXIS_WIN )
			return;

		if ( team == TEAM_AXIS && state == STATE_ALLIES_WIN )
			return;
	}
	else
	{
		// if its after the round, and bonus round is off, don't allow anyone to pick it up
		if ( state != STATE_RND_RUNNING )
			return;
	}	

	OnPickedUp();

	char *szClsName = GetExplodingClassname();

	Assert( szClsName );

	CBaseCombatWeapon *pWeapon = dynamic_cast<CBaseCombatWeapon *>( pPlayer->GiveNamedItem( szClsName ) );
	
	Assert( pWeapon && "Wpn pointer has to be valid for us to pick up this grenade" );

	if( pWeapon )
	{
		variant_t flDetTime;
		flDetTime.SetFloat( m_flDetonateTime );
		pWeapon->AcceptInput( "DetonateTime", this, this, flDetTime, 0 );

#ifdef _DEBUG
		bool bSuccess = 
#endif
			pPlayer->Weapon_Switch( pWeapon );

		Assert( bSuccess );		
		
		//Remove the one we picked up
		SetThink( NULL );
		UTIL_Remove( this );

		if ( m_hSmokeTrail )
		{
			m_hSmokeTrail->Remove();
			m_hSmokeTrail = NULL;
		}
	}
}

int CDODBaseGrenade::OnTakeDamage( const CTakeDamageInfo &info )
{
	if( info.GetDamageType() & DMG_BULLET )
	{
		// Don't allow players to shoot grenades
		return 0;
	}
	else if( info.GetDamageType() & DMG_BLAST )
	{
		// Don't allow explosion force to move grenades
		return 0;
	}

	VPhysicsTakeDamage( info );

	return BaseClass::OnTakeDamage( info );
}

void CDODBaseGrenade::Detonate()
{
	// Don't explode after the round has ended
	if ( dod_bonusround.GetBool() == false && DODGameRules()->State_Get() != STATE_RND_RUNNING )
	{
		SetDamage( 0 );
	}

	if( m_hSmokeTrail )
	{
		m_hSmokeTrail->Remove();
		m_hSmokeTrail = NULL;
	}

	// stun players in a radius
	const float flStunDamage = 100;

	CTakeDamageInfo info( this, GetThrower(), GetBlastForce(), GetAbsOrigin(), flStunDamage, DMG_STUN );
	DODGameRules()->RadiusStun( info, GetAbsOrigin(), m_DmgRadius );

	BaseClass::Detonate();
}

bool CDODBaseGrenade::CreateVPhysics()
{
	// Create the object in the physics system
	VPhysicsInitNormal( SOLID_BBOX, 0, false );
	return true;
}

// this will hit only things that are in newCollisionGroup, but NOT in collisionGroupAlreadyChecked
class CTraceFilterCollisionGroupDelta : public CTraceFilterEntitiesOnly
{
public:
	// It does have a base, but we'll never network anything below here..
	DECLARE_CLASS_NOBASE( CTraceFilterCollisionGroupDelta );

	CTraceFilterCollisionGroupDelta( const IHandleEntity *passentity, int collisionGroupAlreadyChecked, int newCollisionGroup )
		: m_pPassEnt(passentity), m_collisionGroupAlreadyChecked( collisionGroupAlreadyChecked ), m_newCollisionGroup( newCollisionGroup )
	{
	}

	virtual bool ShouldHitEntity( IHandleEntity *pHandleEntity, int contentsMask )
	{
		if ( !PassServerEntityFilter( pHandleEntity, m_pPassEnt ) )
			return false;
		CBaseEntity *pEntity = EntityFromEntityHandle( pHandleEntity );

		if ( pEntity )
		{
			if ( g_pGameRules->ShouldCollide( m_collisionGroupAlreadyChecked, pEntity->GetCollisionGroup() ) )
				return false;
			if ( g_pGameRules->ShouldCollide( m_newCollisionGroup, pEntity->GetCollisionGroup() ) )
				return true;
		}

		return false;
	}

protected:
	const IHandleEntity *m_pPassEnt;
	int		m_collisionGroupAlreadyChecked;
	int		m_newCollisionGroup;
};


const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.2f;

void CDODBaseGrenade::VPhysicsUpdate( IPhysicsObject *pPhysics )
{
	BaseClass::VPhysicsUpdate( pPhysics );
	Vector vel;
	AngularImpulse angVel;
	pPhysics->GetVelocity( &vel, &angVel );

	Vector start = GetAbsOrigin();
	// find all entities that my collision group wouldn't hit, but COLLISION_GROUP_NONE would and bounce off of them as a ray cast
	CTraceFilterCollisionGroupDelta filter( this, GetCollisionGroup(), COLLISION_GROUP_NONE );
	trace_t tr;

	UTIL_TraceLine( start, start + vel * gpGlobals->frametime, CONTENTS_HITBOX|CONTENTS_MONSTER|CONTENTS_SOLID, &filter, &tr );

	if ( tr.startsolid )
	{
		if ( !m_bInSolid )
		{
			// UNDONE: Do a better contact solution that uses relative velocity?
			vel *= -GRENADE_COEFFICIENT_OF_RESTITUTION; // bounce backwards
			pPhysics->SetVelocity( &vel, NULL );
		}
		m_bInSolid = true;
		return;
	}
	m_bInSolid = false;
	if ( tr.DidHit() )
	{
		Vector dir = vel;
		VectorNormalize(dir);

		float flPercent = vel.Length() / 2000;
		float flDmg = 5 * flPercent;

		// send a tiny amount of damage so the character will react to getting bonked
		CTakeDamageInfo info( this, GetThrower(), pPhysics->GetMass() * vel, GetAbsOrigin(), flDmg, DMG_CRUSH );
		tr.m_pEnt->DispatchTraceAttack( info, dir, &tr );
		ApplyMultiDamage();

		if ( vel.Length() > 1000 )
		{
			CTakeDamageInfo stunInfo( this, GetThrower(), vec3_origin, GetAbsOrigin(), flDmg, DMG_STUN );
			tr.m_pEnt->TakeDamage( stunInfo );
		}

		// reflect velocity around normal
		vel = -2.0f * tr.plane.normal * DotProduct(vel,tr.plane.normal) + vel;

		// absorb 80% in impact
		vel *= GetElasticity();
		angVel *= -0.5f;
		pPhysics->SetVelocity( &vel, &angVel );
	}
}

float CDODBaseGrenade::GetElasticity( void )
{
	return GRENADE_COEFFICIENT_OF_RESTITUTION;
}

const float DOD_GRENADE_WINDOW_BREAK_DAMPING_AMOUNT = 0.5f;

// If we hit a window, let it break and continue on our way
// with a damped speed
void CDODBaseGrenade::VPhysicsCollision( int index, gamevcollisionevent_t *pEvent )
{
	CBreakable *pBreakable = dynamic_cast<CBreakable*>( pEvent->pEntities[!index] );
	if ( pBreakable && pBreakable->GetMaterialType() == matGlass && VPhysicsGetObject() )
	{
		// don't stop, go through this entity after breaking it
		Vector dampedVelocity = DOD_GRENADE_WINDOW_BREAK_DAMPING_AMOUNT * pEvent->preVelocity[index];
		VPhysicsGetObject()->SetVelocity( &dampedVelocity, &pEvent->preAngularVelocity[index] );
	}	
	else
		BaseClass::VPhysicsCollision( index, pEvent );
}