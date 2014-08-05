//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"

#include "npc_surface.h"

#include "ai_hull.h"
#include "saverestore_utlvector.h"
#include "dt_utlvector_send.h"
#include "physics_saverestore.h"
#include "vphysics/constraints.h"
#include "vcollide_parse.h"
#include "ragdoll_shared.h"
#include "physics_prop_ragdoll.h"
#include "collisionutils.h"
#include "te_effect_dispatch.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
//
// CNPC_Surface
//

#define SURFACE_MAX_LENGTH	500

//---------------------------------------------------------
// Custom Client entity
//---------------------------------------------------------
IMPLEMENT_SERVERCLASS_ST(CNPC_Surface, DT_NPC_Surface)
	SendPropInt( SENDINFO( m_nActiveParticles ), 12, SPROP_UNSIGNED ),
	SendPropFloat( SENDINFO( m_flRadius ), 12, 0, 0.0, 100.0 ),

	SendPropUtlVector(
		SENDINFO_UTLVECTOR( m_vecSurfacePos ),
		MAX_SURFACE_ELEMENTS, // max elements4
		SendPropVector( NULL, 0, sizeof( Vector ), -1, SPROP_COORD )),

	SendPropUtlVector(
		SENDINFO_UTLVECTOR( m_flSurfaceV ),
		MAX_SURFACE_ELEMENTS, // max elements
		SendPropFloat( NULL, 0, sizeof( float ), 6, 0, 0.0, 1.0 )),

	SendPropUtlVector(
		SENDINFO_UTLVECTOR( m_flSurfaceR ),
		MAX_SURFACE_ELEMENTS, // max elements
		SendPropFloat( NULL, 0, sizeof( float ), 6, 0, 0.0, 2.0 )),

END_SEND_TABLE()


//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------
BEGIN_DATADESC( CNPC_Surface )

	DEFINE_UTLVECTOR( m_vecSurfacePos,	FIELD_POSITION_VECTOR ),
	DEFINE_UTLVECTOR( m_flSurfaceV,	FIELD_FLOAT ),
	DEFINE_UTLVECTOR( m_flSurfaceR,	FIELD_FLOAT ),
	DEFINE_FIELD( m_flRadius,		FIELD_FLOAT ),

	DEFINE_AUTO_ARRAY(  m_bContact, FIELD_BOOLEAN ),

END_DATADESC()

//-------------------------------------

static ConVar	sv_surface_tension( "surface_tension", "5", 0, "How strong the surface tries to keep its shape" );
static ConVar	sv_surface_radius( "surface_radius", "8", 0, "Radius of each sphere" );
static ConVar	sv_surface_ideal( "surface_ideal", "2", 0, "ideal distance (N * radius) between each sphere" );
static ConVar	sv_surface_nearby( "surface_nearby", "3", 0, "acceptable distance (N * radius) between each and still be considered touching" );
static ConVar	sv_surface_scale( "surface_scale", "0.5" );


static ConVar	sv_lj_strength( "lj_strength", "1", 0 );



//-------------------------------------


/*

BEGIN_SIMPLE_DATADESC( CSurfaceController )

	DEFINE_FIELD( m_vecAngular, FIELD_VECTOR ),
	DEFINE_FIELD( m_vecLinear, FIELD_VECTOR ),
	DEFINE_FIELD( m_fIsStopped, FIELD_BOOLEAN ),

END_DATADESC()
*/


//-------------------------------------
// Purpose: Initialize the custom schedules
//-------------------------------------

//-------------------------------------

void CNPC_Surface::Precache()
{
	PrecacheModel( "models/Hydra.mdl" );
	BaseClass::Precache();
}
 

void CNPC_Surface::Activate( void )
{
	BaseClass::Activate();
}


//-----------------------------------------------------------------------------
// Purpose: Returns this monster's place in the relationship table.
//-----------------------------------------------------------------------------
Class_T	CNPC_Surface::Classify( void )
{
	return CLASS_BARNACLE; 
}

//-------------------------------------

void CNPC_Surface::Spawn()
{
	Precache();

	BaseClass::Spawn();

	SetModel( "models/Hydra.mdl" ); // ep3 hydra ???

	SetHullType(HULL_SMALL_CENTERED);
	SetHullSizeNormal();

	// setup model
	SetSolid( SOLID_BBOX );
	AddSolidFlags( FSOLID_FORCE_WORLD_ALIGNED | FSOLID_NOT_STANDABLE );
	SetCollisionBounds( -Vector(400,400,100), Vector(400,400,400) );
	SetMoveType( MOVETYPE_VPHYSICS );

	AddSolidFlags( FSOLID_CUSTOMRAYTEST | FSOLID_CUSTOMBOXTEST );

	AddEFlags( EFL_NO_DISSOLVE );
	SetBloodColor( BLOOD_COLOR_YELLOW );
	ClearEffects();
	m_iHealth			= 200;
	m_flFieldOfView		= -1.0;// indicates the width of this NPC's forward view cone ( as a dotproduct result )
	m_NPCState			= NPC_STATE_NONE;

	SetAbsAngles( QAngle( 0, 0, 0 ) );

	m_vecSurfacePos.EnsureCount( MAX_SURFACE_ELEMENTS );
	m_flSurfaceV.EnsureCount( MAX_SURFACE_ELEMENTS );
	m_flSurfaceR.EnsureCount( MAX_SURFACE_ELEMENTS );

	m_vecStart = GetAbsOrigin( );

	m_vecSurfacePos[0] = m_vecStart;
	m_flSurfaceV[0] = 0.0;
	m_flSurfaceR[0] = 1.0;

	m_flRadius = sv_surface_radius.GetFloat();

	for (int i = 1; i < MAX_SURFACE_ELEMENTS; i++)
	{
		m_vecSurfacePos[i] = m_vecSurfacePos[i-1];
		m_flSurfaceV[i] = m_flSurfaceV[i-1];
		m_flSurfaceR[i] = m_flSurfaceR[i-1];
	}

	NPCInit();
}



//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------


void CNPC_Surface::VPhysicsCollision( int index, gamevcollisionevent_t *pEvent )
{
	BaseClass::VPhysicsCollision( index, pEvent );

	// the sphere hit is:  i = pEvent->pObjects[index]->GetGameIndex();
	// CBaseEntity *pHit = pEvent->pEntities[!index];
	int nSphere = pEvent->pObjects[index]->GetGameIndex();

	m_bContact[nSphere] = true;

	#if 0
	static int count = 0;
	count ++;

	//if((float(rand()) / float(RAND_MAX)) < 0.1f)
	if(count % 10 == 0)
	{
		// Create splash effect
		CEffectData	data;
		data.m_fFlags  = 0;
		
		Vector pos;
		pEvent->pObjects[index]->GetPosition( &pos, NULL );
		
		Vector fountainOrigin(-1980, -1792, 1);
		Vector rad = pos - fountainOrigin;
		//pos += (rad * 0.25f);
		data.m_vOrigin = pos + Vector(0, 0, 5.0f);
		// FIXME: needs to be the correct vector for the impact!
		rad.NormalizeInPlace();
		data.m_vNormal = rad;//Vector( 0, 0, 1 );
		data.m_flScale = 10.0f;
		DispatchEffect( "watersplash", data );			
	}
	#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

void CNPC_Surface::OnSave( IEntitySaveUtils *pUtils )
{
	// TODO: copy current physics state to the temp arrays

	BaseClass::OnSave( pUtils );
}

void CNPC_Surface::OnRestore( void )
{
	BaseClass::OnRestore();

	// TODO: recreate physics state from the temp arrays
	CreateVPhysics( true );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

bool CNPC_Surface::CreateVPhysics()
{
	return CreateVPhysics( false );
}

bool CNPC_Surface::CreateVPhysics( bool bFromRestore )
{
	// setup individual spheres
	objectparams_t params = g_PhysDefaultObjectParams;
	params.pGameData = static_cast<void *>(this);

	int nMaterialIndex = physprops->GetSurfaceIndex("water");

	int i;
	for (i = 0; i < m_nActiveParticles; i++)
	{
		if (!bFromRestore)
		{
			m_vecSurfacePos[i] = GetAbsOrigin() + Vector( RandomFloat( -10, 10 ), RandomFloat( -10, 10 ), RandomFloat( 0, 2 ) ) * m_flRadius;
		}

		m_pSpheres[i] = physenv->CreateSphereObject( m_flRadius, nMaterialIndex, m_vecSurfacePos[i], GetAbsAngles(), &params, false );
		if ( m_pSpheres[i] )
		{
			Vector vVelocity = Vector( RandomFloat( -1, 1 ), RandomFloat( -1, 1 ), RandomFloat( 1, 2 ) ) * 10.0f;
			m_pSpheres[i]->SetVelocity( &vVelocity, NULL );
			//PhysSetGameFlags( m_pSpheres[i], FVPHYSICS_MULTIOBJECT_ENTITY );
			PhysSetGameFlags( m_pSpheres[i], FVPHYSICS_NO_SELF_COLLISIONS | FVPHYSICS_MULTIOBJECT_ENTITY ); // call collisionruleschanged if this changes dynamically
			m_pSpheres[i]->SetGameIndex( i );

			m_pSpheres[i]->SetMass( 10.0f );
			m_pSpheres[i]->EnableGravity( true );
			m_pSpheres[i]->EnableDrag( true );

			// m_pSpheres[i]->EnableMotion( false );

			float flDamping = 0.5f;
			float flAngDamping = 0.5f;
			m_pSpheres[i]->SetDamping( &flDamping, &flAngDamping );
			//m_pSpheres[i]->SetInertia( Vector( 1e30, 1e30, 1e30 ) );
		}
		// VPhysicsSetObject( m_pSpheres[i] );
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: return a list of all the physics objects
//-----------------------------------------------------------------------------

int CNPC_Surface::VPhysicsGetObjectList( IPhysicsObject **pList, int listMax )
{
	int count = 0;
	for ( int i = 0; i < listMax && i < m_nActiveParticles; i++ )
	{
		if (m_flSurfaceR[i] > 0.0f)
		{
			pList[count++] = m_pSpheres[i];
		}
	}

	return count;
}


bool CNPC_Surface::VPhysicsIsFlesh( void )
{
	return false;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

void CNPC_Surface::OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{
	CDefaultPlayerPickupVPhysics::OnPhysGunPickup( pPhysGunUser, reason );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

void CNPC_Surface::OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t Reason )
{
	CDefaultPlayerPickupVPhysics::OnPhysGunDrop( pPhysGunUser, Reason );
}

//-----------------------------------------------------------------------------
// Purpose: Detect that the physgun is trying to punt us.  Currently guess about damage
//-----------------------------------------------------------------------------

bool CNPC_Surface::OnAttemptPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{
	if ( reason == PUNTED_BY_CANNON )
	{
		Vector forward;
		pPhysGunUser->EyeVectors( &forward );

		Vector start, end;
		start = pPhysGunUser->Weapon_ShootPosition();

		float d1, d2;
		for ( int i = 0; i < m_nActiveParticles; i++ )
		{
			if (m_flSurfaceR[i] > 0.0f && IntersectInfiniteRayWithSphere( start, forward, m_vecSurfacePos[i], m_flRadius * 3, &d1, &d2 ))
			{
				Vector p1 = start + d1 * forward;

				// no idea what sort of forces to use when punting
				// also, forceoffset just applies a spin, it doesn't act like being hit with a larger sphere
				m_pSpheres[i]->ApplyForceOffset( forward * 1000.0f, p1 );
			}
		}

		return false;
	}
	return CDefaultPlayerPickupVPhysics::OnAttemptPhysGunPickup( pPhysGunUser, reason );
}

//-----------------------------------------------------------------------------
// Purpose: Apply collisions to multiple spheres instead of just the one hit
//-----------------------------------------------------------------------------

void CNPC_Surface::ApplyDamageForce( const CTakeDamageInfo &info )
{
	float flMinDist2 = (10 * m_flRadius);
	flMinDist2 = flMinDist2 * flMinDist2;

	// FIXME: this needs a better algorithm for radiating the force
	float flForce = info.GetDamageForce().Length();
	flMinDist2 *= max( 1.0, sqrt( flForce / 1000 ));

	if (! (info.GetDamageType() & DMG_BLAST))
	{
		// apply non-blast damage in the direction of the force
		for ( int i = 0; i < m_nActiveParticles; i++ )
		{
			if (m_flSurfaceR[i] > 0.0f)
			{
				float flDist2 = (m_vecSurfacePos[i] - info.GetDamagePosition()).LengthSqr();
				if (flDist2 < flMinDist2)
				{
					m_pSpheres[i]->ApplyForceOffset( info.GetDamageForce() * (1.0 - flDist2 / flMinDist2), info.GetDamagePosition() );
				}
			}
		}
	}
	else
	{
		// blast damage goes out from a point
		for ( int i = 0; i < m_nActiveParticles; i++ )
		{
			if (m_flSurfaceR[i] > 0.0f)
			{
				float flDist2 = (m_vecSurfacePos[i] - info.GetDamagePosition()).LengthSqr();
				if (flDist2 < flMinDist2)
				{
					Vector dir = (m_vecSurfacePos[i] - info.GetDamagePosition()) / sqrt( flDist2 );
					m_pSpheres[i]->ApplyForceCenter( dir * flForce * (1.0 - flDist2 / flMinDist2) );
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

int CNPC_Surface::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	CTakeDamageInfo tdInfo( info );
	
	// don't ever actually take damage
	tdInfo.SetDamage( 0 );

	if (tdInfo.GetDamageType() & (DMG_BULLET | DMG_CLUB))
	{
		ApplyDamageForce( tdInfo );
	}
	else if (info.GetDamageType() & DMG_BLAST)
	{
		ApplyDamageForce( tdInfo );
	}

	return BaseClass::OnTakeDamage_Alive( tdInfo );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

bool CNPC_Surface::TestCollision( const Ray_t &ray, unsigned int fContentsMask, trace_t& tr )
{
	int nLastHit = -1;

	if (ray.m_IsRay)
	{
		float d1, d2;

		for ( int i = 0; i < m_nActiveParticles; i++ )
		{
			if (m_flSurfaceR[i] > 0.0f && IntersectRayWithSphere( ray.m_Start, ray.m_Delta, m_vecSurfacePos[i], m_flRadius, &d1, &d2 ))
			{
				if (d1 < tr.fraction)
				{
					// NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 255, 0, 0, 20, .1);
					nLastHit = i;
					tr.fraction = d1;
				}
			}
		}
		if (nLastHit != -1)
		{
			tr.m_pEnt = this;
			tr.startpos = ray.m_Start;
			tr.endpos = ray.m_Start + ray.m_Delta * tr.fraction; 
			tr.contents = CONTENTS_SOLID;
			tr.hitbox = nLastHit;
			tr.hitgroup = HITGROUP_GENERIC;
			tr.plane.dist = tr.endpos.Length();
			tr.plane.normal = (tr.endpos - m_vecSurfacePos[nLastHit]) * (1 / m_flRadius);
			tr.plane.type = 0;
			tr.physicsbone = nLastHit;
		}
	}
	else
	{
		// FIXME: This isn't a valid test, Jay needs to make it real
		Vector vecMin = Vector( -m_flRadius, -m_flRadius, -m_flRadius) - ray.m_Extents;
		Vector vecMax = Vector( m_flRadius, m_flRadius, m_flRadius) + ray.m_Extents;

		trace_t boxtrace;

		tr.fraction = 1.0;

		for ( int i = 0; i < m_nActiveParticles; i++ )
		{
			if (m_flSurfaceR[i] > 0.0f && IntersectRayWithBox( m_vecSurfacePos[i] - ray.m_Start, -ray.m_Delta, vecMin, vecMax, 0.0, &boxtrace ))
			{
				if (boxtrace.fraction < tr.fraction)
				{
					if (tr.startsolid && !IsBoxIntersectingSphere( ray.m_Start - ray.m_Extents, ray.m_Start + ray.m_Extents, m_vecSurfacePos[i], m_flRadius ))
					{
						tr.startsolid = false;
						tr.allsolid = false;
					}
					else if (tr.allsolid && !IsBoxIntersectingSphere( ray.m_Start + ray.m_Delta - ray.m_Extents, ray.m_Start + ray.m_Delta + ray.m_Extents, m_vecSurfacePos[i], m_flRadius ))
					{
						tr.allsolid = false;
					}

					tr = boxtrace;
					tr.startpos = ray.m_Start;
					tr.endpos = ray.m_Start + tr.fraction * ray.m_Delta;
					nLastHit = i;
				}
			}
		}

		/*
		if (tr.startsolid)
		{
			NDebugOverlay::Box( ray.m_Start, -ray.m_Extents, ray.m_Extents, 255, 0, 0, 0, 10.0);
		}
		*/
		//Assert( !tr.startsolid );
		//Assert( !tr.allsolid );

		//Msg("%5.2f (%6.1f %6.1f %6.1f ) : ", gpGlobals->curtime, ray.m_Start.x, ray.m_Start.y, ray.m_Start.z );
		//Msg("%6.1f %6.1f %6.1f : %4.2f : %6.1f %6.1f %6.1f\n", tr.startpos.x, tr.startpos.y, tr.startpos.z, tr.fraction, tr.endpos.x, tr.endpos.y, tr.endpos.z );

		if (tr.fraction < 1.0)
		{
			tr.contents = CONTENTS_SOLID;
			tr.m_pEnt = this;
			tr.hitbox = nLastHit;
			tr.hitgroup = HITGROUP_GENERIC;
			tr.physicsbone = nLastHit;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------

//-------------------------------------


void CNPC_Surface::RunAI( void )
{
	BaseClass::RunAI();
}

//-------------------------------------

float CNPC_Surface::MaxYawSpeed()
{
	return 180;
}

//-------------------------------------

int CNPC_Surface::TranslateSchedule( int scheduleType ) 
{
	return BaseClass::TranslateSchedule( scheduleType );
}

//-------------------------------------

void CNPC_Surface::HandleAnimEvent( animevent_t *pEvent )
{
	BaseClass::HandleAnimEvent( pEvent );
}

//-------------------------------------

void CNPC_Surface::PrescheduleThink()
{
	BaseClass::PrescheduleThink();
}	

//-------------------------------------

int CNPC_Surface::SelectSchedule ()
{
	return BaseClass::SelectSchedule();
}

//-------------------------------------

void CNPC_Surface::StartTask( const Task_t *pTask )
{
	BaseClass::StartTask( pTask );
}

//-------------------------------------

void CNPC_Surface::RunTask( const Task_t *pTask )
{
	BaseClass::RunTask( pTask );
}

//-------------------------------------

Vector CNPC_Surface::EyePosition( ) 
{
	return GetAbsOrigin(); 
}

const QAngle &CNPC_Surface::EyeAngles()
{
	return GetAbsAngles();
}


Vector CNPC_Surface::BodyTarget( const Vector &posSrc, bool bNoisy)
{
	int iShortest = 0;
	float flShortestDist = (posSrc - m_vecSurfacePos[iShortest]).LengthSqr();
	for (int i = 1; i < m_nActiveParticles; i++)
	{
		if (m_flSurfaceR[i] > 0.0f)
		{
			float flDist = (posSrc - m_vecSurfacePos[i]).LengthSqr();
			if (flDist < flShortestDist)
			{
				iShortest = i;
				flShortestDist = flDist;
			}
		}
	}

	// NDebugOverlay::Box(m_body[iShortest].vecPos, Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 0, 0, 255, 20, .1);

	return m_vecSurfacePos[iShortest];
}

#if !defined(_X360) && defined( USE_BLOBULATOR )
//-------------------------------------

static ConVar	lj_InteractionRadius( "lj_InteractionRadius", "3", 0 );
static ConVar	lj_SurfaceTension( "lj_SurfaceTension", "1", 0 );
static ConVar	lj_Repulsion( "lj_Repulsion", "0.1", 0 );
static ConVar	lj_Attraction( "lj_Attraction", "0.1", 0 );
static ConVar	lj_MaxRepulsion( "lj_MaxRepulsion", "1", 0 );
static ConVar	lj_MaxAttraction( "lj_MaxAttraction", "1", 0 );


CLennardJonesForce::CLennardJonesForce( )
{
	m_fInteractionRadius = lj_InteractionRadius.GetFloat();
	m_fSurfaceTension = lj_SurfaceTension.GetFloat();
	m_fLennardJonesRepulsion = lj_Repulsion.GetFloat();
	m_fLennardJonesAttraction = lj_Attraction.GetFloat();
	m_fMaxRepulsion = lj_MaxRepulsion.GetFloat();
	m_fMaxAttraction = lj_MaxAttraction.GetFloat();

	m_pPhysTiler = new PhysTiler(m_fInteractionRadius);
}

CLennardJonesForce::~CLennardJonesForce( )
{
	delete m_pPhysTiler;
}


// TODO: I should make sure I don't have divide by zero errors.
// TODO: ts is not used
void CLennardJonesForce::addParticleForce(ImpParticle* a, ImpParticle* b, float distSq, float flStrength, float ts) const
{
	float d = sqrtf(distSq);

	//========================================================
	// based on equation of force between two molecules which is
	// factor * ((distance/bond_length)^-7 - (distance/bond_length)^-13)

	float f;
	if(a->group == b->group) // In the same group
	{
		float p = a->radius * 2.0f / (d+FLT_EPSILON);
		float p2 = p * p;
		float p4 = p2 * p2;


		// Surface tension:

		//Notes:
		// Can average the neighbor count between the two particles...
		// I tried this, and discovered that rather than averaging, I can take maybe take the
		// larger of the two neighbor counts, so the attraction between two particles on the surface will be strong, but
		// the attraction between a particle inside and a particle on the surface will be weak. I can also try
		// taking the min so that the attraction between a particle on the surface and a particle inside the fluid will
		// be strong, but the attraction between two particles completely on the inside will be weak.
		//
		// int symmetric_neighbor_count = min(a->neighbor_count, b->neighbor_count);
		//
		// Can try having neighbors only cause stronger attraction (no repulsion)
		// Can try lower exponents for the LennardJones forces.

		// This is a trick to prevent single particles from floating off... the less neighbors a particle has.. the more it sticks
		// This also tends to simulate surface tension
		float surface_tension_modifier = ((24.0f * m_fSurfaceTension) / (a->neighbor_count + b->neighbor_count + 0.1f)) + 1.0f;
		//float lennard_jones_force = fLennardJones * 2.0f * (p2 - (p4 * p4));
		float lennard_jones_force = m_fLennardJonesAttraction * p2 - m_fLennardJonesRepulsion*p4;
		f = surface_tension_modifier * lennard_jones_force;

		// This is some older code:
		//f = ((35.0f * LampScene::simulationSurfaceTension) / (a->neighbor_count + 0.1f)) * (p2 - (p4 * p4));
		// used to be 68'


		//float factor = (b->neighbor_count < 13 && neighbor_count < 13 ? 4.0f : 0.5f);
		//f = factor * (p2 - (p2 * p2 * p2 * p2));
	}
	else
	{
		// This was 3.5 ... made 3.0 so particles get closer when they collide
		if(d > a->radius * 3.0f) return;

		float p = a->radius * 4.0f / d;
		f = -1.0f * p * p;
	}

	// These checks are great to have, but are they really necessary?
	// It might also be good to have a limit on velocity

	// Attraction is a positive value.
	// Repulsion is negative.
	if(f < -m_fMaxRepulsion) f = -m_fMaxRepulsion;
	if(f > m_fMaxAttraction) f = m_fMaxAttraction;

	Point3D scaledr = (b->center - a->center) * (f/(d+FLT_EPSILON)) * flStrength; // Dividing by d scales distance down to a unit vector
	a->force.add(scaledr); 
	b->force.subtract(scaledr);
}

void CLennardJonesForce::AddForces( IPhysicsObject **pObject, int nObjects, float flRadius, float flStrength, Vector *pForces )
{
	int nParticles = nObjects;

	// hack: copy cvars into settings so it can be edited live
	m_fInteractionRadius = lj_InteractionRadius.GetFloat();
	m_fSurfaceTension = lj_SurfaceTension.GetFloat();
	m_fLennardJonesRepulsion = lj_Repulsion.GetFloat();
	m_fLennardJonesAttraction = lj_Attraction.GetFloat();
	m_fMaxRepulsion = lj_MaxRepulsion.GetFloat();
	m_fMaxAttraction = lj_MaxAttraction.GetFloat();

	// FIXME: this isn't thread safe
	static SmartArray<ImpParticle> imp_particles_sa; // This doesn't specify alignment, might have problems with SSE
	while(imp_particles_sa.size < nObjects)
	{
		imp_particles_sa.pushAutoSize(ImpParticle());
	}
	
	// centered and scaled?
	m_pPhysTiler->beginFrame(Point3D(0.0f, 0.0f, 0.0f));

	// Move the spheres into particles
	for(int i=0;i<nObjects;i++)
	{
		ImpParticle* particle = &(imp_particles_sa[i]);
		particle->force.clear();

		Vector pos;
		QAngle ang;
		pObject[i]->GetPosition( &pos, &ang );

		particle->center = pos * (1.0 / flRadius);
		particle->group = i/20;
		particle->neighbor_count = 0;
		m_pPhysTiler->insertParticle(particle);
	}

	m_pPhysTiler->processTiles();


	float timeStep = 1.0f; // This should be customizable
	float nearNeighborInteractionRadius = 2.3f;
	float nearNeighborInteractionRadiusSq = nearNeighborInteractionRadius * nearNeighborInteractionRadius;
	
	PhysParticleCache* pCache = m_pPhysTiler->getParticleCache();

	// Calculate number of near neighbors for each particle
	for(int i = 0; i < nParticles; i++)
	{
		ImpParticle *b1 = &(imp_particles_sa[i]);

		ImpParticleAndDist* node = pCache->get(b1);

		while(node->particle != NULL)
		{
			ImpParticle* b2 = node->particle;

			 // Compare addresses of the two particles. This makes sure we apply a force only once between a pair of particles.
			if(b1 < b2 && node->distSq < nearNeighborInteractionRadiusSq)
			{
				b1->neighbor_count++;
				b2->neighbor_count++;
			}

			node++;
		}
	}

	// Calculate forces on particles due to other particles
	for(int i = 0; i < nParticles; i++)
	{
		ImpParticle *b1 = &(imp_particles_sa[i]);

		ImpParticleAndDist* node = pCache->get(b1);

		while(node->particle != NULL)
		{
			ImpParticle* b2 = node->particle;

			// Compare addresses of the two particles. This makes sure we apply a force only once between a pair of particles.
			if(b1 < b2)
			{
				addParticleForce(b1, b2, node->distSq, flStrength, timeStep);
			}

			node++;
		}
	}

	m_pPhysTiler->endFrame();

	// forces into output array
	for(int i=0;i<nObjects;i++)
	{
		pForces[i] = imp_particles_sa[i].force.AsVector() * flRadius;
	}
}


//-----------------------------------------------------------------------------
//
// CNPC_Surface
//



LINK_ENTITY_TO_CLASS( npc_blob_fountain, CNPC_BlobFountain );


#if 1

IMotionEvent::simresult_e CBlobFountainController::Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular )
{
	if(CAI_BaseNPC::m_nDebugBits & bits_debugDisableAI)
	{
		pObject->EnableMotion(false);
		return IMotionEvent::SIM_NOTHING;
	}


	//if(m_pOwner->m_bPause) return IMotionEvent::SIM_NOTHING;
	//if(CAI_BaseNPC::m_nDebugBits & bits_debugDisableAI) return IMotionEvent::SIM_NOTHING;
/*
	bool newPause = CAI_BaseNPC::m_nDebugBits & bits_debugDisableAI;

	if

	if(newPause != m_bPause)
	{
		if(newPause)
		{
			for (int i = 0; i < m_nActiveParticles; i++)
			{
				m_pSpheres[i]->EnableMotion(false);
			}
		}
		else
		{
			for (int i = 0; i < m_nActiveParticles; i++)
			{
				m_pSpheres[i]->EnableMotion(true);
			}
		}
		m_bPause = newPause;
	}

	if(m_bPause) return;
	*/



	int nSphere = pObject->GetGameIndex();
#if 0
	if (m_pOwner && m_flLennardJonesTime != gpGlobals->curtime)
	{
		m_pOwner->Simulate( pController, pObject, deltaTime, linear, angular );
		m_flLennardJonesTime = gpGlobals->curtime;
	}

	linear += m_vecLennardJonesForce[nSphere] * (1.0f / deltaTime);
#endif

#if 1
	Vector pos, vecVel;
	QAngle ang;
	pObject->GetPosition( &pos, NULL );
	pObject->GetVelocity( &vecVel, NULL );
	float d = sv_surface_radius.GetFloat();
	Vector nozzle = m_pOwner->GetNozzle();
	Vector start = nozzle + Vector(0.0f,0.0f,d);
	pos = pos - nozzle;
	float dist = pos.Length();

	if(m_pOwner->m_iMode[nSphere] == 0)
	{
		if (dist < 10.0f)
		{
			if(vecVel.z < 0.1f && abs(vecVel.x) < 0.05f && abs(vecVel.y) < 0.05f)
			{
				m_pOwner->m_iContactTime[nSphere] = 0;
				m_pOwner->m_fRadius[nSphere] = 0.5f;
				m_pOwner->m_iMode[nSphere] = 1;
				m_pOwner->m_bContact[nSphere] = false;
				//linear.z += 10000 * (1.0f / deltaTime);
				//linear.z = 500000.0f;
				//linear = -vecVel / deltaTime;

				float v = 6000.0f / deltaTime;
				float azimuth = (2.0f * M_PI * (float(rand()) / float(RAND_MAX)));
				float polar = 0.04f * M_PI + (0.01f * (float(rand()) / float(RAND_MAX)));
				//static float azimuth = 0.0f;
				//azimuth += (0.05f * M_PI);
				//float cur_azimuth = azimuth + RandomFloat(-0.01, 0.01);

				linear.x = v * cos(azimuth) * sin(polar);
				linear.y = v * sin(azimuth) * sin(polar);
				linear.z = v * cos(polar);
			}
			//else
			//{
			//	pObject->SetVelocity( &Vector(0.0f, 0.0f, 0.0f), NULL );
			//}
		}
	}
	else if(m_pOwner->m_iMode[nSphere] == 1)
	{
		m_pOwner->m_fRadius[nSphere] = min(1.0f, m_pOwner->m_fRadius[nSphere] + 3.0f*deltaTime);
		if(m_pOwner->m_bContact[nSphere]) // vecVel.z < 0.1f && 
		{
			if(dist < 80.0f)
			{
				m_pOwner->m_fRadius[nSphere] = 0.0f;
				pObject->SetPosition(nozzle, QAngle(0.0f, 0.0f, 0.0f), true);
				Vector zero(0.0f, 0.0f, 0.0f);
				pObject->SetVelocity( &zero, NULL );
				m_pOwner->m_iMode[nSphere] = 0;
				m_pOwner->m_iContactTime[nSphere] = 0;
			}
			else
			{
				m_pOwner->m_fRadius[nSphere] = 0.0f; //max(0.0f, m_pOwner->m_fRadius[nSphere] - 2.0f*deltaTime);
				m_pOwner->m_iContactTime[nSphere] += deltaTime;
				if(m_pOwner->m_iContactTime[nSphere] >= 1.0f)
				{
					m_pOwner->m_iMode[nSphere] = 2;
					m_pOwner->m_iContactTime[nSphere] = 0;
					m_pOwner->m_bContact[nSphere] = false;
				}
			}
		}
	}
	else if(m_pOwner->m_iMode[nSphere] == 2)
	{
		pObject->SetPosition(nozzle, QAngle(0.0f, 0.0f, 0.0f), true);
		Vector zero(0.0f, 0.0f, 0.0f);
		pObject->SetVelocity( &zero, NULL );
		//Vector vVelocity = Vector( RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ) ) * 0.2f;
		//pObject->SetVelocity( &vVelocity, NULL );

		m_pOwner->m_iContactTime[nSphere] += deltaTime;
		if(m_pOwner->m_iContactTime[nSphere] >= 1.0f)
		{
			m_pOwner->m_iMode[nSphere] = 0;
			m_pOwner->m_iContactTime[nSphere] = 0;
		}
	}

	#if 0
	if(m_pOwner->m_bContact[nSphere]) // || (vecVel.z < -20.0f && pos.z < 40.0f))
	{
		m_pOwner->m_flSurfaceR[nSphere] = 0.0f; //max(0.0f, m_pOwner->m_flSurfaceR[nSphere] - 0.1f);
		m_pOwner->m_iContactTime[nSphere] ++;

		//if(m_pOwner->m_flSurfaceR[nSphere] <= 0.0f)
		if(m_pOwner->m_iContactTime[nSphere] >= 10)
		{
			m_pOwner->m_bContact[nSphere] = false;

			if(dist > 10.0f)
			{
				pObject->SetPosition(nozzle, QAngle(0.0f, 0.0f, 0.0f), true);
				Vector vVelocity = Vector( RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ) ) * 0.2f;
				pObject->SetVelocity( &vVelocity, NULL );
				//m_pOwner->m_flSurfaceR[nSphere] = 0.1f
				

				//Vector vVelocity(0.0f, 0.0f, 0.0f);
				//pObject->SetVelocity( &vVelocity, NULL );
			}
		}
	}
	else
	{
		//pObject->SetVelocity( &Vector(), &AngularImpulse());
		if (dist < 10.0f)
		{
			if(vecVel.z < 0.1f && abs(vecVel.x) < 0.1f && abs(vecVel.y) < 0.1f)
			{
				m_pOwner->m_iContactTime[nSphere] = 0;
				m_pOwner->m_flSurfaceR[nSphere] = 1.0f;
				//linear.z += 10000 * (1.0f / deltaTime);
				//linear.z = 500000.0f;
				//linear = -vecVel / deltaTime;

				float v = 6000.0f / deltaTime;
				float azimuth = (2.0f * M_PI * (float(rand()) / float(RAND_MAX)));
				float polar = 0.03f * M_PI; //(0.03f * M_PI * (float(rand()) / float(RAND_MAX))); // 

				linear.x = v * cos(azimuth) * sin(polar);
				linear.y = v * sin(azimuth) * sin(polar);
				linear.z = v * cos(polar);
			}
		}
	}
	#endif
#endif

	return IMotionEvent::SIM_GLOBAL_FORCE;
}

void CNPC_BlobFountain::Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular )
{
	m_force.AddForces( m_pSpheres, m_nActiveParticles, m_flRadius, sv_lj_strength.GetFloat(), m_pBlobFountainController->m_vecLennardJonesForce );
	//NetworkProp()->NetworkStateForceUpdate();
}

bool CNPC_BlobFountain::CreateVPhysics( bool bFromRestore )
{
	objectparams_t params = g_PhysDefaultObjectParams;
	params.pGameData = static_cast<void *>(this);

	int nMaterialIndex = physprops->GetSurfaceIndex("water");

	// FIXME: don't hardcode the number of particles
	m_nActiveParticles = 1000;

	//bool result = BaseClass::CreateVPhysics( bFromRestore );

	m_pBlobFountainController = new CBlobFountainController( this );
	m_pMotionController = physenv->CreateMotionController( m_pBlobFountainController );

	// find the ground under the starting position
	trace_t	tr;
	UTIL_TraceLine( m_vecStart+Vector(0,0,1), m_vecStart-Vector(0,0,64), MASK_SOLID_BRUSHONLY | CONTENTS_PLAYERCLIP | CONTENTS_MONSTERCLIP, this, COLLISION_GROUP_NONE, &tr );
	m_vecStart = tr.endpos;

	int i;
	for (i = 0; i < m_nActiveParticles; i++)
	{
		m_vecSurfacePos[i] = m_vecStart + Vector(0,0,10.0f);
		m_pSpheres[i] = physenv->CreateSphereObject( m_flRadius, nMaterialIndex, m_vecSurfacePos[i], GetAbsAngles(), &params, false );

		if ( m_pSpheres[i] )
		{
			Vector vVelocity = Vector( RandomFloat( -1, 1 ), RandomFloat( -1, 1 ), RandomFloat( -1, 1 ) ) * 10.0f;
			m_pSpheres[i]->SetVelocity( &vVelocity, NULL );
			//PhysSetGameFlags( m_pSpheres[i], FVPHYSICS_MULTIOBJECT_ENTITY );
			PhysSetGameFlags( m_pSpheres[i], FVPHYSICS_NO_SELF_COLLISIONS | FVPHYSICS_MULTIOBJECT_ENTITY ); // call collisionruleschanged if this changes dynamically
			m_pSpheres[i]->SetGameIndex( i );

			m_pSpheres[i]->SetMass( 10.0f );
			m_pSpheres[i]->EnableGravity( true );
			m_pSpheres[i]->EnableDrag( true );

			// m_pSpheres[i]->EnableMotion( false );

			float flDamping = 0.5f;
			float flAngDamping = 0.5f;
			m_pSpheres[i]->SetDamping( &flDamping, &flAngDamping );
			//m_pSpheres[i]->SetInertia( Vector( 1e30, 1e30, 1e30 ) );
			m_pSpheres[i]->EnableGravity( true );
			//m_pSpheres[i]->SetPosition(m_vecStart + Vector(0,0,10.0f), QAngle(0.0f, 0.0f, 0.0f), false);
			m_pMotionController->AttachObject( m_pSpheres[i], true );
			m_iContactTime[i] = 0;
			m_iMode[i] = 0;

			// 0: ready to launch, 1: launched, 2: contact
		}
	}

	return true;
}


void CNPC_BlobFountain::RunAI( void )
{
	/*
	bool newPause = ((CAI_BaseNPC::m_nDebugBits & bits_debugDisableAI) != 0);

	 //(sv_lj_strength.GetFloat() > 0.0f);

	if(newPause != m_bPause)
	{
		if(newPause)
		{
			for (int i = 0; i < m_nActiveParticles; i++)
			{
				m_pSpheres[i]->EnableMotion(false);
			}
		}
		else
		{
			for (int i = 0; i < m_nActiveParticles; i++)
			{
				m_pSpheres[i]->EnableMotion(true);
			}
		}
		m_bPause = newPause;
	}

	if(m_bPause) return;
	*/
	
	m_pMotionController->WakeObjects();

	// push spheres around to meet position targets

	//Vector vecGoal = m_vecStart;
	for (int i = 0; i < m_nActiveParticles; i++)
	{
		m_pSpheres[i]->EnableMotion(true);
		//m_pSpheres[i]->EnableGravity( true );
		{
			Vector pos;
			m_pSpheres[i]->GetPosition( &pos, NULL );
			m_vecSurfacePos[i] = pos;
			m_flSurfaceR[i] = m_fRadius[i];
		}
		#if 0
		if (m_bContact[i])
		{
			Vector vecVel;
			m_pSpheres[i]->GetVelocity( &vecVel, NULL );

			Vector estPos = m_vecSurfacePos[i] + vecVel;
			Vector delta( 0, 0, 0 );
			float dist(0.0f);

			// NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), m_bFloat[i] * 255, 255, m_bContact[i] * 255, 20, .1);
			// move sphere towards center target
			delta = vecGoal - estPos;
			dist = VectorNormalize( delta );
			// delta = delta * min( max( dist - m_flRadius * 4, 0 ), 500 ) * sv_surface_tension.GetFloat();
			delta = delta * min( dist, 100 ) * 0.2; // sv_surface_tension.GetFloat();

			/*
			if(dist > 10.0f)
			{
				m_pSpheres[i]->SetPosition(m_vecStart + Vector(0,0,m_flRadius), QAngle(), true);
				Vector vVelocity = Vector( RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ) ) * 0.2f;
				m_pSpheres[i]->SetVelocity( &vVelocity, NULL );
			}
			m_bContact[i] = false;
			*/

			/*
			if(dist < 10.0f)
			{
				m_pSpheres[i]->ApplyForceCenter( Vector(0.0f, 0.0f, 10000.0f) );
			}
			*/

			/*
			m_flSurfaceV[i] = Approach( 0.0f, m_flSurfaceV[i], 0.2f );

			m_pSpheres[i]->ApplyForceCenter( delta );
			*/


			//NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 0, 255, 0, 20, .1);
		}
		#endif
	}

	NetworkProp()->NetworkStateForceUpdate();
}

#else



IMotionEvent::simresult_e CBlobFountainController::Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular )
{

	int nSphere = pObject->GetGameIndex();

	if (m_pOwner && m_flLennardJonesTime != gpGlobals->curtime)
	{
		m_pOwner->Simulate( pController, pObject, deltaTime, linear, angular );
		m_flLennardJonesTime = gpGlobals->curtime;
	}

	linear += m_vecLennardJonesForce[nSphere] * (1.0f / deltaTime);

	Vector pos, vecVel;
	//QAngle ang;
	pObject->GetPosition( &pos, NULL );
	pObject->GetVelocity( &vecVel, NULL );
	//float d = sv_surface_radius.GetFloat();
	Vector nozzle = m_pOwner->GetNozzle();
	pos = pos - nozzle;
	float dist = pos.Length();
	pos.NormalizeInPlace();
	/*
	if(nSphere <= 200)
	{
		if(dist >= 150.0f)
		{
			linear -= pos * ((dist-150.0f) / deltaTime);
		}
	}
	else
	{
		*/
		if(dist >= 500.0f)
		{
			linear -= pos * ((dist-500.0f) / deltaTime);
		}
	//}

	float v = vecVel.Length();
	linear -=  vecVel * (1.0f / ((v+FLT_EPSILON) * deltaTime));


#if 0
	Vector pos, vecVel;
	QAngle ang;
	pObject->GetPosition( &pos, NULL );
	pObject->GetVelocity( &vecVel, NULL );
	float d = sv_surface_radius.GetFloat();
	Vector nozzle = m_pOwner->GetNozzle();
	Vector start = nozzle + Vector(0.0f,0.0f,d);
	pos = pos - nozzle;
	float dist = pos.Length();

	if(m_pOwner->m_iMode[nSphere] == 0)
	{
		if (dist < 10.0f)
		{
			if(vecVel.z < 0.1f && abs(vecVel.x) < 0.05f && abs(vecVel.y) < 0.05f)
			{
				m_pOwner->m_iContactTime[nSphere] = 0;
				m_pOwner->m_fRadius[nSphere] = 0.5f;
				m_pOwner->m_iMode[nSphere] = 1;
				m_pOwner->m_bContact[nSphere] = false;
				//linear.z += 10000 * (1.0f / deltaTime);
				//linear.z = 500000.0f;
				//linear = -vecVel / deltaTime;

				float v = 6000.0f / deltaTime;
				float azimuth = (2.0f * M_PI * (float(rand()) / float(RAND_MAX)));
				float polar = 0.04f * M_PI + (0.01f * (float(rand()) / float(RAND_MAX)));
				//static float azimuth = 0.0f;
				//azimuth += (0.05f * M_PI);
				//float cur_azimuth = azimuth + RandomFloat(-0.01, 0.01);

				linear.x = v * cos(azimuth) * sin(polar);
				linear.y = v * sin(azimuth) * sin(polar);
				linear.z = v * cos(polar);
			}
			//else
			//{
			//	pObject->SetVelocity( &Vector(0.0f, 0.0f, 0.0f), NULL );
			//}
		}
	}
	else if(m_pOwner->m_iMode[nSphere] == 1)
	{
		m_pOwner->m_fRadius[nSphere] = min(1.0f, m_pOwner->m_fRadius[nSphere] + 3.0f*deltaTime);
		if(m_pOwner->m_bContact[nSphere]) // vecVel.z < 0.1f && 
		{
			if(dist < 80.0f)
			{
				m_pOwner->m_fRadius[nSphere] = 0.0f;
				pObject->SetPosition(nozzle, QAngle(0.0f, 0.0f, 0.0f), true);
				Vector zero(0.0f, 0.0f, 0.0f);
				pObject->SetVelocity( &zero, NULL );
				m_pOwner->m_iMode[nSphere] = 0;
				m_pOwner->m_iContactTime[nSphere] = 0;
			}
			else
			{
				m_pOwner->m_fRadius[nSphere] = 0.0f; //max(0.0f, m_pOwner->m_fRadius[nSphere] - 2.0f*deltaTime);
				m_pOwner->m_iContactTime[nSphere] += deltaTime;
				if(m_pOwner->m_iContactTime[nSphere] >= 1.0f)
				{
					m_pOwner->m_iMode[nSphere] = 2;
					m_pOwner->m_iContactTime[nSphere] = 0;
					m_pOwner->m_bContact[nSphere] = false;
				}
			}
		}
	}
	else if(m_pOwner->m_iMode[nSphere] == 2)
	{
		pObject->SetPosition(nozzle, QAngle(0.0f, 0.0f, 0.0f), true);
		Vector zero(0.0f, 0.0f, 0.0f);
		pObject->SetVelocity( &zero, NULL );
		//Vector vVelocity = Vector( RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ) ) * 0.2f;
		//pObject->SetVelocity( &vVelocity, NULL );

		m_pOwner->m_iContactTime[nSphere] += deltaTime;
		if(m_pOwner->m_iContactTime[nSphere] >= 1.0f)
		{
			m_pOwner->m_iMode[nSphere] = 0;
			m_pOwner->m_iContactTime[nSphere] = 0;
		}
	}

	#if 0
	if(m_pOwner->m_bContact[nSphere]) // || (vecVel.z < -20.0f && pos.z < 40.0f))
	{
		m_pOwner->m_flSurfaceR[nSphere] = 0.0f; //max(0.0f, m_pOwner->m_flSurfaceR[nSphere] - 0.1f);
		m_pOwner->m_iContactTime[nSphere] ++;

		//if(m_pOwner->m_flSurfaceR[nSphere] <= 0.0f)
		if(m_pOwner->m_iContactTime[nSphere] >= 10)
		{
			m_pOwner->m_bContact[nSphere] = false;

			if(dist > 10.0f)
			{
				pObject->SetPosition(nozzle, QAngle(0.0f, 0.0f, 0.0f), true);
				Vector vVelocity = Vector( RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ) ) * 0.2f;
				pObject->SetVelocity( &vVelocity, NULL );
				//m_pOwner->m_flSurfaceR[nSphere] = 0.1f
				

				//Vector vVelocity(0.0f, 0.0f, 0.0f);
				//pObject->SetVelocity( &vVelocity, NULL );
			}
		}
	}
	else
	{
		//pObject->SetVelocity( &Vector(), &AngularImpulse());
		if (dist < 10.0f)
		{
			if(vecVel.z < 0.1f && abs(vecVel.x) < 0.1f && abs(vecVel.y) < 0.1f)
			{
				m_pOwner->m_iContactTime[nSphere] = 0;
				m_pOwner->m_flSurfaceR[nSphere] = 1.0f;
				//linear.z += 10000 * (1.0f / deltaTime);
				//linear.z = 500000.0f;
				//linear = -vecVel / deltaTime;

				float v = 6000.0f / deltaTime;
				float azimuth = (2.0f * M_PI * (float(rand()) / float(RAND_MAX)));
				float polar = 0.03f * M_PI; //(0.03f * M_PI * (float(rand()) / float(RAND_MAX))); // 

				linear.x = v * cos(azimuth) * sin(polar);
				linear.y = v * sin(azimuth) * sin(polar);
				linear.z = v * cos(polar);
			}
		}
	}
	#endif
#endif

	return IMotionEvent::SIM_GLOBAL_FORCE;
}

void CNPC_BlobFountain::Simulate( IPhysicsMotionController *pController, IPhysicsObject *pObject, float deltaTime, Vector &linear, AngularImpulse &angular )
{
	m_force.AddForces( m_pSpheres, m_nActiveParticles, m_flRadius * 3.0f, sv_lj_strength.GetFloat(), m_pBlobFountainController->m_vecLennardJonesForce );
	//NetworkProp()->NetworkStateForceUpdate();
}

bool CNPC_BlobFountain::CreateVPhysics( bool bFromRestore )
{
	objectparams_t params = g_PhysDefaultObjectParams;
	params.pGameData = static_cast<void *>(this);

	int nMaterialIndex = physprops->GetSurfaceIndex("water");

	// FIXME: don't hardcode the number of particles
	m_nActiveParticles = 300;

	//bool result = BaseClass::CreateVPhysics( bFromRestore );

	m_pBlobFountainController = new CBlobFountainController( this );
	m_pMotionController = physenv->CreateMotionController( m_pBlobFountainController );

	// find the ground under the starting position
	trace_t	tr;
	UTIL_TraceLine( m_vecStart+Vector(0,0,1), m_vecStart-Vector(0,0,64), MASK_SOLID_BRUSHONLY | CONTENTS_PLAYERCLIP | CONTENTS_MONSTERCLIP, this, COLLISION_GROUP_NONE, &tr );
	m_vecStart = tr.endpos;
	m_vecStart.z += 600.0f;

	for (int i = 0; i < m_nActiveParticles; i++)
	{
		m_vecSurfacePos[i] = m_vecStart + Vector( RandomFloat( -1, 1 ), RandomFloat( -1, 1 ), RandomFloat( -1, 1 ) ) * 400.0f;;
		m_pSpheres[i] = physenv->CreateSphereObject( m_flRadius, nMaterialIndex, m_vecSurfacePos[i], GetAbsAngles(), &params, false );

		if ( m_pSpheres[i] )
		{
			Vector vVelocity = Vector( RandomFloat( -1, 1 ), RandomFloat( -1, 1 ), RandomFloat( -1, 1 ) ) * 0.0f;
			m_pSpheres[i]->SetVelocity( &vVelocity, NULL );
			//PhysSetGameFlags( m_pSpheres[i], FVPHYSICS_MULTIOBJECT_ENTITY );
			PhysSetGameFlags( m_pSpheres[i], FVPHYSICS_NO_SELF_COLLISIONS | FVPHYSICS_MULTIOBJECT_ENTITY ); // call collisionruleschanged if this changes dynamically
			m_pSpheres[i]->SetGameIndex( i );

			m_pSpheres[i]->SetMass( 10.0f );
			m_pSpheres[i]->EnableGravity( false );
			m_pSpheres[i]->EnableDrag( true );
			float drag = 100.0f;
			m_pSpheres[i]->SetDragCoefficient(&drag, &drag);

			m_pSpheres[i]->EnableCollisions(false);

			// m_pSpheres[i]->EnableMotion( false );

			float flDamping = 0.5f;
			float flAngDamping = 0.5f;
			m_pSpheres[i]->SetDamping( &flDamping, &flAngDamping );
			//m_pSpheres[i]->SetInertia( Vector( 1e30, 1e30, 1e30 ) );
			//m_pSpheres[i]->EnableGravity( true );
			//m_pSpheres[i]->SetPosition(m_vecStart + Vector(0,0,10.0f), QAngle(0.0f, 0.0f, 0.0f), false);
			m_pMotionController->AttachObject( m_pSpheres[i], true );
			m_iContactTime[i] = 0;
			m_iMode[i] = 0;
			m_fRadius[i] = 1.0f;

			// 0: ready to launch, 1: launched, 2: contact
		}
	}

	return true;
}


void CNPC_BlobFountain::RunAI( void )
{
	m_pMotionController->WakeObjects();

	// push spheres around to meet position targets

	//Vector vecGoal = m_vecStart;
	for (int i = 0; i < m_nActiveParticles; i++)
	{
		//m_pSpheres[i]->EnableGravity( true );
		{
			Vector pos;
			m_pSpheres[i]->GetPosition( &pos, NULL );
			m_vecSurfacePos[i] = pos;
			m_flSurfaceR[i] = m_fRadius[i];
		}
		#if 0
		if (m_bContact[i])
		{
			Vector vecVel;
			m_pSpheres[i]->GetVelocity( &vecVel, NULL );

			Vector estPos = m_vecSurfacePos[i] + vecVel;
			Vector delta( 0, 0, 0 );
			float dist(0.0f);

			// NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), m_bFloat[i] * 255, 255, m_bContact[i] * 255, 20, .1);
			// move sphere towards center target
			delta = vecGoal - estPos;
			dist = VectorNormalize( delta );
			// delta = delta * min( max( dist - m_flRadius * 4, 0 ), 500 ) * sv_surface_tension.GetFloat();
			delta = delta * min( dist, 100 ) * 0.2; // sv_surface_tension.GetFloat();

			/*
			if(dist > 10.0f)
			{
				m_pSpheres[i]->SetPosition(m_vecStart + Vector(0,0,m_flRadius), QAngle(), true);
				Vector vVelocity = Vector( RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ), RandomFloat( -1.0f, 1.0f ) ) * 0.2f;
				m_pSpheres[i]->SetVelocity( &vVelocity, NULL );
			}
			m_bContact[i] = false;
			*/

			/*
			if(dist < 10.0f)
			{
				m_pSpheres[i]->ApplyForceCenter( Vector(0.0f, 0.0f, 10000.0f) );
			}
			*/

			/*
			m_flSurfaceV[i] = Approach( 0.0f, m_flSurfaceV[i], 0.2f );

			m_pSpheres[i]->ApplyForceCenter( delta );
			*/


			//NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 0, 255, 0, 20, .1);
		}
		#endif
	}

	NetworkProp()->NetworkStateForceUpdate();
}

#endif


//-----------------------------------------------------------------------------
//
// CNPC_BlobArmTest
//

//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------

BEGIN_DATADESC( CNPC_BlobArmTest )

	DEFINE_AUTO_ARRAY(  m_nOwnedSlot, FIELD_INTEGER ),
	DEFINE_AUTO_ARRAY(  m_nTargetSlot, FIELD_INTEGER ),
	DEFINE_AUTO_ARRAY(  m_bFloat, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bDoArms, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_hTarget, FIELD_EHANDLE ),
	DEFINE_FIELD( m_flSimTime, FIELD_TIME ),

END_DATADESC()


LINK_ENTITY_TO_CLASS( npc_surface, CNPC_BlobArmTest );
LINK_ENTITY_TO_CLASS( npc_blob_armtest, CNPC_BlobArmTest );

void CNPC_BlobArmTest::Spawn( void )
{
	BaseClass::Spawn( );

	m_flSimTime			= 0;
	m_bDoArms = false;

}

bool CNPC_BlobArmTest::CreateVPhysics( bool bFromRestore )
{
	// FIXME: don't hardcode the number of particles
	m_nActiveParticles = 128;

	bool result = BaseClass::CreateVPhysics( bFromRestore );

	return result;
}

void CNPC_BlobArmTest::RunAI( void )
{
	float flIdealDistance = m_flRadius * sv_surface_ideal.GetFloat();
	float flNearbyDistance = m_flRadius * sv_surface_nearby.GetFloat();

	int i, j;
	// copy physics positions into networked array
	for (i = 0; i < m_nActiveParticles; i++)
	{
		Vector pos;
		QAngle ang;
		m_pSpheres[i]->GetPosition( &pos, &ang );
		m_vecSurfacePos[i] = pos;
	}


	int nArmLength = MoveTowardsGoal( );


	// FIXME: this isn't thread safe
	static SmartArray<ImpParticle> imp_particles_sa; // This doesn't specify alignment, might have problems with SSE
	while(imp_particles_sa.size < m_nActiveParticles)
	{
		imp_particles_sa.pushAutoSize(ImpParticle());
	}
	
	PhysTiler *m_pPhysTiler = new PhysTiler(sv_surface_nearby.GetFloat());

	// centered and scaled?
	m_pPhysTiler->beginFrame(Point3D(0.0f, 0.0f, 0.0f));

	int nParticles = 0;

	// Move the spheres into particles
	float projection = 0.5;
	for(i = 0; i < m_nActiveParticles; i++)
	{
		if (m_flSurfaceR[i] > 0.0f)
		{
			ImpParticle* particle = &(imp_particles_sa[nParticles++]);

			Vector vecVel;
			m_pSpheres[i]->GetVelocity( &vecVel, NULL );
			Vector estPos = m_vecSurfacePos[i] + vecVel * projection;

			particle->center = estPos * (1.0 / m_flRadius);
			particle->group = 0;
			particle->neighbor_count = 0;
			particle->temp1 = i;
			m_pPhysTiler->insertParticle(particle);
		}
	}

	m_pPhysTiler->processTiles();

	PhysParticleCache* pCache = m_pPhysTiler->getParticleCache();

	for (int k = 0; k < nParticles; k++)
	{
		ImpParticle *b1 = &(imp_particles_sa[k]);
		i = b1->temp1;

		Vector estPos = b1->center.AsVector() * m_flRadius;
		Vector delta( 0, 0, 0 );

		// push against nearby spheres
		float flIdealDist2 = (flIdealDistance * flIdealDistance);
		float flNearbyDist2 = (flNearbyDistance * flNearbyDistance);
		bool bFloat = false;
		float flDist2;
		Vector dir;

		ImpParticleAndDist* node = pCache->get(b1);

		while(node->particle != NULL)
		{
			ImpParticle* b2 = node->particle;
			if (b2 == b1)
			{
				node++;
				continue;
			}
			j = b2->temp1;

			int bSameArm = (m_nArm[i] == m_nArm[j]) && (m_nArm[i] > -1);

			Vector estEffectorPos =  b2->center.AsVector() * m_flRadius;
			flDist2 = (estPos - estEffectorPos).LengthSqr();

			flNearbyDist2 = (m_flSurfaceR[i] + m_flSurfaceR[j]) * 0.5 * flNearbyDistance;
			flNearbyDist2 = flNearbyDist2 * flNearbyDist2;

			// NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j], 0, 255, 0, true, .1);

			if (!bSameArm && m_nTargetSlot[j] != 0)
			{
				if (flDist2 < flIdealDist2)
				{
					// repluse if they're too close, and they're not in the same group, and they're on an arm
					dir = (estPos - estEffectorPos);
					VectorNormalize( dir );
					delta += dir * min( (flIdealDist2 - flDist2), 100 );
					//NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j], 255, 0, 0, true, .1);
				}
				/*
				else if (flDist2 < flNearbyDist2)
				{
					dir = (estPos - m_vecSurfacePos[j] - vecVel2);
					VectorNormalize( dir );
					delta -= dir * 30;
					NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j], 255, 0, 0, true, .1);
				}
				*/
				// NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j], 255, 0, 0, true, .1);
			}
			if (!bFloat && flDist2 < flNearbyDist2 && ((m_vecSurfacePos[j].z <= m_vecSurfacePos[i].z) || (bSameArm && ((j % nArmLength) < (i % nArmLength)) && m_bFloat[j] ) ) )
			{
				//NDebugOverlay::Line(m_vecSurfacePos[j], m_vecSurfacePos[i], 0, 255, 0, true, .1);
				bFloat = true;
			}
	
			node++;
		}

		// figure out what to do with gravity
		if (!bFloat /* && (i % nArmLength == 0) */)
		{
			/*
			if (delta.z > 0)
				delta.z = 0;
			delta.z -= 300 * 0.1 * m_pSpheres[i]->GetMass();
			*/
			//NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 255, 0, 0, 20, .1);
		}

		m_pSpheres[i]->EnableGravity( !bFloat && !m_bContact[i] );
		m_bFloat[i] = bFloat;
		m_bContact[i] = false;

#if 0
		// pull toward next in chain
		if (i != 0 /* && i == ((int)gpGlobals->curtime) % m_vecSurfacePos.Count()*/)
		{
			dir = (m_vecSurfacePos[i-1] - estPos);
			if (DotProduct( dir, vecVel ) < 0.0)
			{
				float dist = VectorNormalize( dir );
				delta += dir * min( dist, 100 );
			}
			//NDebugOverlay::Line(m_vecSurfacePos[i-1], estPos, 0, 255, 0, true, .1);
			//Msg("%d : %.1f %.1f %.1f\n", i, delta.x, delta.y, delta.z );
		}
#endif

#if 0
		float dot = DotProduct( vVel, delta );
		if (dot > 0.0)
			delta.Init();
#endif

		// apply the force
		m_pSpheres[i]->ApplyForceCenter( delta );

		// NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 0, 255, 0, 20, .1);
	}

	m_pPhysTiler->endFrame();

	NetworkProp()->NetworkStateForceUpdate();
}



int CNPC_BlobArmTest::MoveTowardsGoal( void )
{
	int i;

	// alternate between arm mode and walk mode
	bool bDoArms = ((int)(gpGlobals->curtime / 17.0) % 2) == 1;

	if (bDoArms != m_bDoArms)
	{
		for (i = 0; i < m_nActiveParticles; i++)
		{
			m_nOwnedSlot[ i ] = 0;
			m_nTargetSlot[ i ] = 0;
		}
	}
	m_bDoArms = bDoArms;

	int nArmLength = 1;
	if (bDoArms)
	{
		nArmLength = 8;
	}
	else
	{
		m_flSimTime += 0.1;
	}
	Vector vecGoal( 0, sin( m_flSimTime * 0.2 ) * 250, 0 );

	vecGoal = vecGoal + m_vecStart;

	vecGoal.z = m_flRadius;

	float tension = sv_surface_tension.GetFloat(); // * (1 - sqrt( fabs( sin( gpGlobals->curtime * 0.3 ) ) ) );

	float projection = 0.5;
	// push spheres around to meet position targets
	for (i = 0; i < m_nActiveParticles; i++)
	{
		Vector vecVel;
		m_pSpheres[i]->GetVelocity( &vecVel, NULL );
		Vector estPos = m_vecSurfacePos[i] + vecVel * projection;
		Vector delta( 0, 0, 0 );
		float dist(0.0f);

		m_nArm[i] = (i / nArmLength);

		// NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), m_bFloat[i] * 255, 255, m_bContact[i] * 255, 20, .1);
		if (m_bFloat[i] || m_bContact[i] || fabs( vecVel.z ) < 1.0 )
		{
			int k = (i % nArmLength);
			int j = i - k;

			if (!bDoArms || i > m_nActiveParticles / 2)
			{
				m_nArm[i] = -1;
				delta = vecGoal - estPos;
				//delta.z *= fabs( cos( gpGlobals->curtime * 0.5 ) );
				dist = VectorNormalize( delta );
				delta = delta * min( max( dist - m_flRadius * (bDoArms ? 8 : 10), 0 ), 500 ) * tension;
				m_nOwnedSlot[i] = 0;
				m_nTargetSlot[i] = 1;
				m_flSurfaceV[i] = Approach( 0.0f, m_flSurfaceV[i], 0.2f );
			}
			else if (k == 0)
			{
				// move sphere towards center target
				delta = vecGoal - estPos;
				//delta.z *= fabs( cos( gpGlobals->curtime * 0.5 ) );
				dist = VectorNormalize( delta );
				delta = delta * min( max( dist - m_flRadius * (bDoArms ? 8 : 10), 0 ), 500 ) * tension;
				m_nOwnedSlot[i] = 0;
				m_nTargetSlot[i] = 1;
				m_flSurfaceV[i] = Approach( 0.0f, m_flSurfaceV[i], 0.2f );
			}
			else
			{
				// move sphere outward towards end of arm
				Vector out = m_vecSurfacePos[j] - vecGoal;
				VectorNormalize( out );

				if (m_nTargetSlot[i] > m_nOwnedSlot[j] + 1)
				{
					m_nTargetSlot[i] = m_nOwnedSlot[j] + 1;
				}

				// Vector target =  m_vecSurfacePos[j] + out * flIdealDistance * (m_nTargetSlot[i]);
				Vector target =  m_vecSurfacePos[j];
				if (i != j)
				{
					Vector vecTargetVel;
					m_pSpheres[i-1]->GetVelocity( &vecTargetVel, NULL );
					target = m_vecSurfacePos[i-1] + vecTargetVel * 0.1 + out * m_flRadius * (m_flSurfaceR[i-1] + m_flSurfaceR[i]);
					// Msg("%d : %.1f : %.1f %.1f\n", i, (m_flSurfaceR[i-1] + m_flSurfaceR[i]), m_flSurfaceR[i-1], m_flSurfaceR[i] ) ;
				}

				//target += Vector( 0, 0, m_flRadius ) * sin( j + gpGlobals->curtime * 3.0 + M_PI * (m_nTargetSlot[i] / (float)(nArmLength-1)) );
				delta = target - estPos;
				//NDebugOverlay::Line(m_vecSurfacePos[i], estPos + delta, 0, 255, 0, true, .1);
				//NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j] + out * flIdealDistance * (m_nTargetSlot[i]), 0, 255, 0, true, .1);
				dist = VectorNormalize( delta );
				if (dist < m_flRadius * m_flSurfaceR[i])
				{
					if (m_nTargetSlot[i] == k)
					{
						m_nOwnedSlot[j] = max( m_nOwnedSlot[j], k );
					}
					else
					{
						m_nTargetSlot[i]++;
					}
				}
				else if (dist >= m_flRadius * m_flSurfaceR[i] * 2 && m_nTargetSlot[i] > 0)
				{
					m_nTargetSlot[i]--;
					if (m_nOwnedSlot[j] > m_nTargetSlot[i])
					{
						m_nOwnedSlot[j] = m_nTargetSlot[i];
					}
					//NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 255, 0, 0, 20, .1);
				}

				//NDebugOverlay::Line( m_vecSurfacePos[j], m_vecSurfacePos[j] + out * flIdealDistance * m_nOwnedSlot[j], 255, 0, 0, true, .1);

				delta = delta * min( max( dist - 0, 0), 250 ) * m_pSpheres[i]->GetMass();
				//NDebugOverlay::Line(m_vecSurfacePos[j], m_vecSurfacePos[j] + out * flIdealDistance, 255, 0, 0, true, .1);
				// m_flSurfaceV[i] = clamp( Approach( (m_nTargetSlot[i]) / (float)max( m_nOwnedSlot[j] + 1, m_nTargetSlot[i]), m_flSurfaceV[i], 0.2f ), 0.0f, 1.0f );
				m_flSurfaceV[i] = Approach( (m_nTargetSlot[i]) / (float)nArmLength, m_flSurfaceV[i], 0.2f );
				m_flSurfaceV[i] = clamp( m_flSurfaceV[i], 0.0f, 1.0f );
			}
		}

		// tweak radius
		//float a = (m_bContact[i] || bFloat) ? 1.0 : sv_surface_scale.GetFloat();
		float a = sv_surface_scale.GetFloat();
		if (m_nTargetSlot[i] <= 1 ) a = 1.0;
		if (nArmLength > 1 && m_nTargetSlot[i] >= nArmLength - 1) a = (1.0 + sv_surface_scale.GetFloat()) * 0.5;
		float b = m_flSurfaceR[i];
		float c = Approach( a, b, 0.2f );
		m_flSurfaceR[i] = clamp( c, 0.0f, 1.0f );

		if (!m_bFloat[i])
		{
			delta.z = 0;
		}

		// apply the force
		m_pSpheres[i]->ApplyForceCenter( delta );
	}

	return nArmLength;
}





























//-----------------------------------------------------------------------------
//
// CNPC_BlobDemoMonster
//

//---------------------------------------------------------
// Save/Restore
//---------------------------------------------------------

BEGIN_DATADESC( CNPC_BlobDemoMonster )

	DEFINE_AUTO_ARRAY(  m_nOwnedSlot, FIELD_INTEGER ),
	DEFINE_AUTO_ARRAY(  m_nTargetSlot, FIELD_INTEGER ),
	DEFINE_AUTO_ARRAY(  m_bFloat, FIELD_BOOLEAN ),
	DEFINE_AUTO_ARRAY(  m_nArm, FIELD_INTEGER ),

	DEFINE_FIELD( m_vecGoal, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_vecPrevGoal, FIELD_POSITION_VECTOR ),

	DEFINE_FIELD( m_bDoArms, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bDoContactZ, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_hTarget, FIELD_EHANDLE ),
	DEFINE_FIELD( m_flSimTime, FIELD_TIME ),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_STRING, "MoveToPosition", InputMoveToPosition ),
	DEFINE_INPUTFUNC( FIELD_BOOLEAN, "DoArms", InputDoArms ),
	DEFINE_INPUTFUNC( FIELD_BOOLEAN, "DoContactZ", InputDoContactZ ),

END_DATADESC()


LINK_ENTITY_TO_CLASS( npc_blob_demomonster, CNPC_BlobDemoMonster );

void CNPC_BlobDemoMonster::Spawn( void )
{
	BaseClass::Spawn( );

	m_flSimTime			= 0;
	m_bDoArms = false;

	m_vecPrevGoal = GetAbsOrigin();
}

bool CNPC_BlobDemoMonster::CreateVPhysics( bool bFromRestore )
{
	// FIXME: don't hardcode the number of particles
	m_nActiveParticles = 256;
	m_flRadius = 6.5;

	bool result = BaseClass::CreateVPhysics( bFromRestore );

	return result;
}


	// Input handlers
void CNPC_BlobDemoMonster::InputMoveToPosition( inputdata_t &inputdata )
{
	if ( inputdata.value.Entity() != NULL )
	{
		SetTarget( inputdata.value.Entity() );
	}
	else if ( inputdata.value.String() != NULL )
	{
		CBaseEntity *pTargetEnt = gEntList.FindEntityByName( NULL, inputdata.value.String() );
		SetTarget( pTargetEnt );
	}
}

void CNPC_BlobDemoMonster::InputDoArms( inputdata_t &inputdata )
{
	m_bDoArms = inputdata.value.Bool();
}

void CNPC_BlobDemoMonster::InputDoContactZ( inputdata_t &inputdata )
{
	m_bDoContactZ = inputdata.value.Bool();
}



int CNPC_BlobDemoMonster::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	if (info.GetDamageType() & (DMG_BULLET | DMG_CLUB | DMG_BLAST))
	{
		Vector dir = m_vecGoal - info.GetInflictor()->GetAbsOrigin();
		VectorNormalize( dir );
		m_vecPrevGoal = m_vecPrevGoal + dir * 8;
	}

	return BaseClass::OnTakeDamage_Alive( info );
}


void CNPC_BlobDemoMonster::RunAI( void )
{
	int i;
	// copy physics positions into networked array
	for (i = 0; i < m_nActiveParticles; i++)
	{
		Vector pos;
		QAngle ang;
		m_pSpheres[i]->GetPosition( &pos, &ang );
		m_vecSurfacePos[i] = pos;
	}

#if 0
	// alternate between arm mode and walk mode
	bool bDoArms = ((int)(gpGlobals->curtime / 17.0) % 2) == 1;

	if (bDoArms != m_bDoArms)
	{
		for (i = 0; i < m_nActiveParticles; i++)
		{
			m_nOwnedSlot[ i ] = 0;
			m_nTargetSlot[ i ] = 0;
		}
	}
	m_bDoArms = bDoArms;
#endif

	int nArmLength = 1;
	if (m_bDoArms)
	{
		nArmLength = 16;
		m_flSimTime += 0.025;
	}
	else
	{
		m_flSimTime += 0.1;
	}
	Vector vecGoal = m_vecPrevGoal; // ( cos( m_flSimTime * 0.3 ) * 250, sin( m_flSimTime * 0.2 ) * 250, 0 );

	if (GetTarget() != NULL)
	{
		vecGoal = GetTarget()->GetAbsOrigin();
	}

	// keep track of direction of movement
	Vector forward = vecGoal - m_vecPrevGoal;
	float dist = VectorNormalize( forward );
	if (dist < 1.0)
	{
		forward *= dist;
	}
	else
	{
		vecGoal = m_vecPrevGoal + forward * min( dist, m_bDoArms ? 16 : 48 );
	}
	

	m_vecPrevGoal = m_vecPrevGoal * 0.8 + vecGoal * 0.2;


	vecGoal.z = m_flRadius;
	m_vecGoal = vecGoal;

	if (!m_bDoArms)
	{
		MoveTowardsGoal( );
	}
	else
	{
		CreateArms( forward );
	}

	RepulseNeighbors( );

}

void CNPC_BlobDemoMonster::RepulseNeighbors()
{
	float flIdealDistance = m_flRadius * sv_surface_ideal.GetFloat();
	float flNearbyDistance = m_flRadius * sv_surface_nearby.GetFloat();

	int i, j;
	// FIXME: this isn't thread safe
	static SmartArray<ImpParticle> imp_particles_sa; // This doesn't specify alignment, might have problems with SSE
	while(imp_particles_sa.size < m_nActiveParticles)
	{
		imp_particles_sa.pushAutoSize(ImpParticle());
	}
	
	PhysTiler *m_pPhysTiler = new PhysTiler(sv_surface_nearby.GetFloat());

	// centered and scaled?
	m_pPhysTiler->beginFrame(Point3D(0.0f, 0.0f, 0.0f));

	int nParticles = 0;

	// Move the spheres into particles
	float projection = 0.5;
	for(i = 0; i < m_nActiveParticles; i++)
	{
		if (m_flSurfaceR[i] > 0.0f)
		{
			ImpParticle* particle = &(imp_particles_sa[nParticles++]);

			Vector vecVel;
			m_pSpheres[i]->GetVelocity( &vecVel, NULL );
			Vector estPos = m_vecSurfacePos[i] + vecVel * projection;

			particle->center = estPos * (1.0 / m_flRadius);
			particle->group = 0;
			particle->neighbor_count = 0;
			particle->temp1 = i;
			m_pPhysTiler->insertParticle(particle);
		}
	}

	m_pPhysTiler->processTiles();

	PhysParticleCache* pCache = m_pPhysTiler->getParticleCache();

	for (int k = 0; k < nParticles; k++)
	{
		ImpParticle *b1 = &(imp_particles_sa[k]);
		i = b1->temp1;

		Vector estPos = b1->center.AsVector() * m_flRadius;
		Vector delta( 0, 0, 0 );

		// push against nearby spheres
		float flIdealDist2 = (flIdealDistance * flIdealDistance);
		float flNearbyDist2 = (flNearbyDistance * flNearbyDistance);
		bool bFloat = false;
		float flDist2;
		Vector dir;

		ImpParticleAndDist* node = pCache->get(b1);

		while(node->particle != NULL)
		{
			ImpParticle* b2 = node->particle;
			if (b2 == b1)
			{
				node++;
				continue;
			}
			j = b2->temp1;

			int bSameArm = (m_nArm[i] == m_nArm[j]) && (m_nArm[i] > -1);

			Vector estEffectorPos =  b2->center.AsVector() * m_flRadius;
			flDist2 = (estPos - estEffectorPos).LengthSqr();

			flNearbyDist2 = (m_flSurfaceR[i] + m_flSurfaceR[j]) * 0.5 * flNearbyDistance;
			flNearbyDist2 = flNearbyDist2 * flNearbyDist2;

			// NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j], 0, 255, 0, true, .1);

			if (!bSameArm && m_nTargetSlot[j] != 0)
			{
				if (flDist2 < flIdealDist2)
				{
					// repluse if they're too close, and they're not in the same group, and they're on an arm
					dir = (estPos - estEffectorPos);
					VectorNormalize( dir );
					delta += dir * min( (flIdealDist2 - flDist2), 100 );
					//NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j], 255, 0, 0, true, .1);
				}
				/*
				else if (flDist2 < flNearbyDist2)
				{
					dir = (estPos - m_vecSurfacePos[j] - vecVel2);
					VectorNormalize( dir );
					delta -= dir * 30;
					NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j], 255, 0, 0, true, .1);
				}
				*/
				// NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j], 255, 0, 0, true, .1);
			}
			if (!bFloat && flDist2 < flNearbyDist2 && ((m_vecSurfacePos[j].z <= m_vecSurfacePos[i].z) || (bSameArm && j < i) && m_bFloat[j] ) )
			{
				//NDebugOverlay::Line(m_vecSurfacePos[j], m_vecSurfacePos[i], 0, 255, 0, true, .1);
				bFloat = true;
			}
	
			node++;
		}

		// figure out what to do with gravity
		if (!bFloat /* && (i % nArmLength == 0) */)
		{
			/*
			if (delta.z > 0)
				delta.z = 0;
			delta.z -= 300 * 0.1 * m_pSpheres[i]->GetMass();
			*/
			//NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 255, 0, 0, 20, .1);
		}

		m_pSpheres[i]->EnableGravity( !bFloat && !m_bContact[i] );
		m_bFloat[i] = bFloat;
		m_bContact[i] = false;

#if 0
		// pull toward next in chain
		if (i != 0 /* && i == ((int)gpGlobals->curtime) % m_vecSurfacePos.Count()*/)
		{
			dir = (m_vecSurfacePos[i-1] - estPos);
			if (DotProduct( dir, vecVel ) < 0.0)
			{
				float dist = VectorNormalize( dir );
				delta += dir * min( dist, 100 );
			}
			//NDebugOverlay::Line(m_vecSurfacePos[i-1], estPos, 0, 255, 0, true, .1);
			//Msg("%d : %.1f %.1f %.1f\n", i, delta.x, delta.y, delta.z );
		}
#endif

#if 0
		float dot = DotProduct( vVel, delta );
		if (dot > 0.0)
			delta.Init();
#endif

		// apply the force
		m_pSpheres[i]->ApplyForceCenter( delta );

		// NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 0, 255, 0, 20, .1);
	}

	m_pPhysTiler->endFrame();

	NetworkProp()->NetworkStateForceUpdate();
}



void CNPC_BlobDemoMonster::MoveTowardsGoal( void )
{
	int i;

	float tension = sv_surface_tension.GetFloat(); // * (1 - sqrt( fabs( sin( gpGlobals->curtime * 0.3 ) ) ) );

	float projection = 0.5;
	// push spheres around to meet position targets
	for (i = 0; i < m_nActiveParticles; i++)
	{
		Vector vecVel;
		m_pSpheres[i]->GetVelocity( &vecVel, NULL );
		Vector estPos = m_vecSurfacePos[i] + vecVel * projection;
		Vector delta( 0, 0, 0 );
		float dist(0.0f);

		// NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), m_bFloat[i] * 255, 255, m_bContact[i] * 255, 20, .1);
		if (m_bFloat[i] || m_bContact[i] || fabs( vecVel.z ) < 1.0 )
		{
			m_nArm[i] = -1;
			delta = m_vecGoal - estPos;
			if (m_bDoContactZ)
				delta.z = 0;
			//delta.z *= fabs( cos( gpGlobals->curtime * 0.5 ) );
			dist = VectorNormalize( delta );
			delta = delta * min( max( dist - m_flRadius * 8, 0 ), 500 ) * tension;
			m_nOwnedSlot[i] = 0;
			m_nTargetSlot[i] = 1;
			m_flSurfaceV[i] = Approach( 0.0f, m_flSurfaceV[i], 0.2f );
		}

		// tweak radius
		//float a = (m_bContact[i] || bFloat) ? 1.0 : sv_surface_scale.GetFloat();
		float a = 1.0f; // sv_surface_scale.GetFloat();
		float b = m_flSurfaceR[i];
		float c = Approach( a, b, 0.2f );
		m_flSurfaceR[i] = clamp( c, 0.0f, 1.0f );

		if (!(m_bFloat[i] || (m_bDoContactZ && m_bContact[i])))
		{
			delta.z = 0;
		}

		// apply the force
		m_pSpheres[i]->ApplyForceCenter( delta );
	}
}



void CNPC_BlobDemoMonster::CreateArms( const Vector &vecForward  )
{
	int i;

	if (!m_bDoArms)
	{
		for (i = 0; i < m_nActiveParticles; i++)
		{
			m_nOwnedSlot[ i ] = 0;
			m_nTargetSlot[ i ] = 0;
		}
	}
	m_bDoArms = true;

	// NDebugOverlay::HorzArrow( GetAbsOrigin(), GetAbsOrigin() + vecForward * 32, 8, 255, 255, 255, 255, true, 0.1 );
	int nArmLength = 8;

	float tension = sv_surface_tension.GetFloat(); // * (1 - sqrt( fabs( sin( gpGlobals->curtime * 0.3 ) ) ) );

	float projection = 0.5;
	// push spheres around to meet position targets
	for (i = 0; i < m_nActiveParticles; i++)
	{
		Vector vecVel;
		m_pSpheres[i]->GetVelocity( &vecVel, NULL );
		Vector estPos = m_vecSurfacePos[i] + vecVel * projection;
		Vector delta( 0, 0, 0 );
		float dist(0.0f);

		m_nArm[i] = (i / nArmLength);

		if (!m_bFloat[i])
		{
			m_nTargetSlot[ i ] = 0;
		}

		// NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), m_bFloat[i] * 255, 255, m_bContact[i] * 255, 20, .1);
		if (m_bFloat[i] || m_bContact[i] || fabs( vecVel.z ) < 1.0 )
		{
			int k = (i % nArmLength);
			int j = i - k;

			if (k == 0)
			{
				// move sphere towards center target
				delta = m_vecGoal - estPos;
				//delta.z *= fabs( cos( gpGlobals->curtime * 0.5 ) );
				dist = VectorNormalize( delta );
				delta = delta * min( max( dist - m_flRadius * 5, 0 ), 500 ) * tension;
				if (m_bDoContactZ)
					delta.z = 0;
				m_nOwnedSlot[i] = 0;
				m_nTargetSlot[i] = 1;
				m_flSurfaceV[i] = Approach( 0.0f, m_flSurfaceV[i], 0.2f );
			}
			else
			{
				// move sphere outward towards end of arm
				Vector out = m_vecSurfacePos[j] - m_vecGoal;
				VectorNormalize( out );

				if (m_nTargetSlot[i] > m_nOwnedSlot[j] + 1)
				{
					m_nTargetSlot[i] = m_nOwnedSlot[j] + 1;
				}

				// Vector target =  m_vecSurfacePos[j] + out * flIdealDistance * (m_nTargetSlot[i]);
				int n = min( j + m_nTargetSlot[i], i - 1);
				Vector target =  m_vecSurfacePos[j];
				if (i != j)
				{
					Vector vecTargetVel;
					m_pSpheres[i-1]->GetVelocity( &vecTargetVel, NULL );
					Vector dir = out + 2 * vecForward * (n - j) / nArmLength;
					VectorNormalize( dir );
					target = m_vecSurfacePos[n] + vecTargetVel * 0.1f + dir * m_flRadius * (m_flSurfaceR[n] + m_flSurfaceR[i]);
					// Msg("%d : %d : %.1f : %.1f %.1f\n", i, n, (m_flSurfaceR[j+n] + m_flSurfaceR[i]), m_flSurfaceR[j+n], m_flSurfaceR[i] ) ;
				}

				//target += Vector( 0, 0, m_flRadius ) * sin( j + gpGlobals->curtime * 3.0 + M_PI * (m_nTargetSlot[i] / (float)(nArmLength-1)) );
				delta = target - estPos;
				//NDebugOverlay::Line(m_vecSurfacePos[i], estPos + delta, 0, 255, 0, true, .1);
				//NDebugOverlay::Line(m_vecSurfacePos[i], m_vecSurfacePos[j] + out * flIdealDistance * (m_nTargetSlot[i]), 0, 255, 0, true, .1);
				dist = VectorNormalize( delta );
				if (dist < m_flRadius * m_flSurfaceR[i])
				{
					if (m_nTargetSlot[i] == k)
					{
						m_nOwnedSlot[j] = max( m_nOwnedSlot[j], k );
					}
					else
					{
						m_nTargetSlot[i]++;
					}
				}
				else if (dist >= m_flRadius * m_flSurfaceR[i] * 2 && m_nTargetSlot[i] > 0)
				{
					m_nTargetSlot[i]--;
					if (m_nOwnedSlot[j] > m_nTargetSlot[i])
					{
						m_nOwnedSlot[j] = m_nTargetSlot[i];
					}
					//NDebugOverlay::Box(m_vecSurfacePos[i], Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), 255, 0, 0, 20, .1);
				}

				//NDebugOverlay::Line( m_vecSurfacePos[j], m_vecSurfacePos[j] + out * flIdealDistance * m_nOwnedSlot[j], 255, 0, 0, true, .1);

				delta = delta * min( max( dist - 0, 0), 250 ) * m_pSpheres[i]->GetMass();
				//NDebugOverlay::Line(m_vecSurfacePos[j], m_vecSurfacePos[j] + out * flIdealDistance, 255, 0, 0, true, .1);
				// m_flSurfaceV[i] = clamp( Approach( (m_nTargetSlot[i]) / (float)max( m_nOwnedSlot[j] + 1, m_nTargetSlot[i]), m_flSurfaceV[i], 0.2f ), 0.0f, 1.0f );
				m_flSurfaceV[i] = Approach( (m_nTargetSlot[i]) / (float)nArmLength, m_flSurfaceV[i], 0.2f );
				m_flSurfaceV[i] = clamp( m_flSurfaceV[i], 0.0f, 1.0f );
			}
		}

		// tweak radius
		//float a = (m_bContact[i] || bFloat) ? 1.0 : sv_surface_scale.GetFloat();
		float a = sv_surface_scale.GetFloat();
		if (m_nTargetSlot[i] <= 1 ) a = 1.0;
		if (nArmLength > 1 && m_nTargetSlot[i] >= nArmLength - 1) a = (1.0 + sv_surface_scale.GetFloat()) * 0.5;
		float b = m_flSurfaceR[i];

		// a = 1.0f;
		float c = Approach( a, b, 0.2f );
		m_flSurfaceR[i] = clamp( c, 0.0f, 1.0f );

		if (!m_bFloat[i])
		{
			delta.z = 0;
		}

		// apply the force
		m_pSpheres[i]->ApplyForceCenter( delta );
	}
}
#endif