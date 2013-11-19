//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Slowly damages the object it's attached to
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "tf_player.h"
#include "tf_team.h"
#include "tf_gamerules.h"
#include "tf_obj.h"
#include "tf_obj_sentrygun.h"
#include "tf_obj_sapper.h"
#include "ndebugoverlay.h"

// ------------------------------------------------------------------------ //

#define SAPPER_MINS				Vector(0, 0, 0)
#define SAPPER_MAXS				Vector(1, 1, 1)

#define SAPPER_MODEL_SENTRY_1	"models/buildables/sapper_sentry1.mdl"
#define SAPPER_MODEL_SENTRY_2	"models/buildables/sapper_sentry2.mdl"
#define SAPPER_MODEL_SENTRY_3	"models/buildables/sapper_sentry3.mdl"
#define SAPPER_MODEL_TELEPORTER	"models/buildables/sapper_teleporter.mdl"
#define SAPPER_MODEL_DISPENSER	"models/buildables/sapper_dispenser.mdl"

#define SAPPER_MODEL_SENTRY_1_PLACEMENT		"models/buildables/sapper_placement_sentry1.mdl"
#define SAPPER_MODEL_SENTRY_2_PLACEMENT		"models/buildables/sapper_placement_sentry2.mdl"
#define SAPPER_MODEL_SENTRY_3_PLACEMENT		"models/buildables/sapper_placement_sentry3.mdl"
#define SAPPER_MODEL_TELEPORTER_PLACEMENT	"models/buildables/sapper_placement_teleporter.mdl"
#define SAPPER_MODEL_DISPENSER_PLACEMENT	"models/buildables/sapper_placement_dispenser.mdl"

BEGIN_DATADESC( CObjectSapper )
	DEFINE_THINKFUNC( SapperThink ),
END_DATADESC()

IMPLEMENT_SERVERCLASS_ST(CObjectSapper, DT_ObjectSapper)
END_SEND_TABLE();

LINK_ENTITY_TO_CLASS(obj_attachment_sapper, CObjectSapper);
PRECACHE_REGISTER(obj_attachment_sapper);

ConVar	obj_sapper_health( "obj_sapper_health", "100", FCVAR_NONE, "Sapper health" );
ConVar	obj_sapper_amount( "obj_sapper_amount", "25", FCVAR_NONE, "Amount of health inflicted by a Sapper object per second" );

#define SAPPER_THINK_CONTEXT		"SapperThink"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CObjectSapper::CObjectSapper()
{
	m_iHealth = obj_sapper_health.GetInt();
	SetMaxHealth( m_iHealth );

	UseClientSideAnimation();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectSapper::UpdateOnRemove()
{
	StopSound( "Weapon_Sapper.Timer" );

	BaseClass::UpdateOnRemove();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectSapper::Spawn()
{
	SetModel( SAPPER_MODEL_SENTRY_1_PLACEMENT );

	m_takedamage = DAMAGE_YES;
	m_iHealth = obj_sapper_health.GetInt();

	SetType( OBJ_ATTACHMENT_SAPPER );

	BaseClass::Spawn();

	Vector mins = SAPPER_MINS;
	Vector maxs = SAPPER_MAXS;
	CollisionProp()->SetSurroundingBoundsType( USE_SPECIFIED_BOUNDS, &mins, &maxs );

    m_fObjectFlags.Set( m_fObjectFlags | OF_ALLOW_REPEAT_PLACEMENT );

	SetSolid( SOLID_NONE );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectSapper::Precache()
{
	int iModelIndex;

	iModelIndex = PrecacheModel( SAPPER_MODEL_SENTRY_1 );
	PrecacheGibsForModel( iModelIndex );

	iModelIndex = PrecacheModel( SAPPER_MODEL_SENTRY_2 );
	PrecacheGibsForModel( iModelIndex );

	iModelIndex = PrecacheModel( SAPPER_MODEL_SENTRY_3 );
	PrecacheGibsForModel( iModelIndex );

	iModelIndex = PrecacheModel( SAPPER_MODEL_TELEPORTER );
	PrecacheGibsForModel( iModelIndex );

	iModelIndex = PrecacheModel( SAPPER_MODEL_DISPENSER );
	PrecacheGibsForModel( iModelIndex );

	PrecacheModel( SAPPER_MODEL_SENTRY_1_PLACEMENT );
	PrecacheModel( SAPPER_MODEL_SENTRY_2_PLACEMENT );
	PrecacheModel( SAPPER_MODEL_SENTRY_3_PLACEMENT );
	PrecacheModel( SAPPER_MODEL_TELEPORTER_PLACEMENT );
	PrecacheModel( SAPPER_MODEL_DISPENSER_PLACEMENT );

	PrecacheScriptSound( "Weapon_Sapper.Plant" );
	PrecacheScriptSound( "Weapon_Sapper.Timer" );

	BaseClass::Precache();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectSapper::FinishedBuilding( void )
{
	BaseClass::FinishedBuilding();

	if ( GetParentObject() )
	{
		GetParentObject()->OnAddSapper();
	}

	EmitSound( "Weapon_Sapper.Plant" );

	// start looping "Weapon_Sapper.Timer", killed when we die
	EmitSound( "Weapon_Sapper.Timer" );

	m_flSapperDamageAccumulator = 0;
	m_flLastThinkTime = gpGlobals->curtime;

	SetContextThink( &CObjectSapper::SapperThink, gpGlobals->curtime + 0.1, SAPPER_THINK_CONTEXT );
}

//-----------------------------------------------------------------------------
// Purpose: Change our model based on the object we are attaching to
//-----------------------------------------------------------------------------
void CObjectSapper::SetupAttachedVersion( void )
{
	CBaseObject *pObject = dynamic_cast<CBaseObject *>( m_hBuiltOnEntity.Get() );

	Assert( pObject );

	if ( !pObject )
	{
		DestroyObject();
		return;
	}

	if ( IsPlacing() )
	{
		switch( pObject->GetType() )
		{
		case OBJ_SENTRYGUN:
			{
				// what level?
				CObjectSentrygun *pSentry = dynamic_cast<CObjectSentrygun *>( pObject );
				Assert( pSentry );
				if ( !pSentry )
				{
					DestroyObject();
					return;
				}

				int iLevel = pSentry->GetUpgradeLevel();

				switch( iLevel )
				{
				case 1:
					SetModel( SAPPER_MODEL_SENTRY_1_PLACEMENT );
					break;
				case 2:
					SetModel( SAPPER_MODEL_SENTRY_2_PLACEMENT );
					break;
				case 3:
					SetModel( SAPPER_MODEL_SENTRY_3_PLACEMENT );
					break;
				default:
					Assert(0);
					break;
				}
			}
			break;

		case OBJ_TELEPORTER_ENTRANCE:
		case OBJ_TELEPORTER_EXIT:
			SetModel( SAPPER_MODEL_TELEPORTER_PLACEMENT );
			break;

		case OBJ_DISPENSER:
			SetModel( SAPPER_MODEL_DISPENSER_PLACEMENT );
			break;

		default:
			Assert( !"what kind of object are we trying to place a sapper on?" );
			break;
		}
	}	

	BaseClass::SetupAttachedVersion();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectSapper::OnGoActive( void )
{
	// set new model
	CBaseObject *pObject = dynamic_cast<CBaseObject *>( m_hBuiltOnEntity.Get() );

	Assert( pObject );

	if ( !pObject )
	{
		DestroyObject();
		return;
	}

	switch( pObject->GetType() )
	{
	case OBJ_SENTRYGUN:
		{
			// what level?
			CObjectSentrygun *pSentry = dynamic_cast<CObjectSentrygun *>( pObject );
			Assert( pSentry );
			if ( !pSentry )
			{
				DestroyObject();
				return;
			}

			int iLevel = pSentry->GetUpgradeLevel();

			switch( iLevel )
			{
			case 1:
				SetModel( SAPPER_MODEL_SENTRY_1 );
				break;
			case 2:
				SetModel( SAPPER_MODEL_SENTRY_2 );
				break;
			case 3:
				SetModel( SAPPER_MODEL_SENTRY_3 );
				break;
			default:
				Assert(0);
				break;
			}
		}
		break;

	case OBJ_TELEPORTER_ENTRANCE:
	case OBJ_TELEPORTER_EXIT:
		SetModel( SAPPER_MODEL_TELEPORTER );
		break;

	case OBJ_DISPENSER:
		SetModel( SAPPER_MODEL_DISPENSER );
		break;

	default:
		Assert( !"what kind of object are we trying to place a sapper on?" );
		break;
	}

	UTIL_SetSize( this, SAPPER_MINS, SAPPER_MAXS );
	SetSolid( SOLID_NONE );

	BaseClass::OnGoActive();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectSapper::DetachObjectFromObject( void )
{
	if ( GetParentObject() )
	{
		GetParentObject()->OnRemoveSapper();
	}

	BaseClass::DetachObjectFromObject();
}

//-----------------------------------------------------------------------------
// Purpose: Slowly destroy the object I'm attached to
//-----------------------------------------------------------------------------
void CObjectSapper::SapperThink( void )
{
	if ( !GetTeam() )
		return;

	CBaseObject *pObject = GetParentObject();
	if ( !pObject )
	{
		DestroyObject();
		return;
	}

	SetNextThink( gpGlobals->curtime + 0.1, SAPPER_THINK_CONTEXT );

	// Don't bring objects back from the dead
	if ( !pObject->IsAlive() || pObject->IsDying() )
		return;

	// how much damage to give this think?
	float flTimeSinceLastThink = gpGlobals->curtime - m_flLastThinkTime;
	float flDamageToGive = ( flTimeSinceLastThink ) * obj_sapper_amount.GetFloat();

	// add to accumulator
	m_flSapperDamageAccumulator += flDamageToGive;

	int iDamage = (int)m_flSapperDamageAccumulator;

	m_flSapperDamageAccumulator -= iDamage;

	CTakeDamageInfo info;
	info.SetDamage( iDamage );
	info.SetAttacker( this );
	info.SetInflictor( this );
	info.SetDamageType( DMG_CRUSH );

	pObject->TakeDamage( info );

	m_flLastThinkTime = gpGlobals->curtime;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CObjectSapper::OnTakeDamage( const CTakeDamageInfo &info )
{
	if ( info.GetDamageCustom() != TF_DMG_WRENCH_FIX )
	{
		return 0;
	}

	return BaseClass::OnTakeDamage( info );
}
