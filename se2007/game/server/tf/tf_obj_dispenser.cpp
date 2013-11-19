//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Engineer's Dispenser
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"

#include "tf_obj_dispenser.h"
#include "engine/IEngineSound.h"
#include "tf_player.h"
#include "tf_team.h"
#include "vguiscreen.h"
#include "world.h"
#include "explode.h"
#include "triggers.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Ground placed version
#define DISPENSER_MODEL_PLACEMENT	"models/buildables/dispenser_blueprint.mdl"
#define DISPENSER_MODEL_BUILDING	"models/buildables/dispenser.mdl"
#define DISPENSER_MODEL				"models/buildables/dispenser_light.mdl"

#define DISPENSER_MINS			Vector( -20, -20, 0)
#define DISPENSER_MAXS			Vector( 20, 20, 55)	// tweak me

#define DISPENSER_TRIGGER_MINS			Vector(-70, -70, 0)
#define DISPENSER_TRIGGER_MAXS			Vector( 70,  70, 50)	// tweak me

#define REFILL_CONTEXT			"RefillContext"
#define DISPENSE_CONTEXT		"DispenseContext"

//-----------------------------------------------------------------------------
// Purpose: SendProxy that converts the Healing list UtlVector to entindices
//-----------------------------------------------------------------------------
void SendProxy_HealingList( const SendProp *pProp, const void *pStruct, const void *pData, DVariant *pOut, int iElement, int objectID )
{
	CObjectDispenser *pDispenser = (CObjectDispenser*)pStruct;

	// If this assertion fails, then SendProxyArrayLength_HealingArray must have failed.
	Assert( iElement < pDispenser->m_hHealingTargets.Size() );

	CBaseEntity *pEnt = pDispenser->m_hHealingTargets[iElement].Get();
	EHANDLE hOther = pEnt;

	SendProxy_EHandleToInt( pProp, pStruct, &hOther, pOut, iElement, objectID );
}

int SendProxyArrayLength_HealingArray( const void *pStruct, int objectID )
{
	CObjectDispenser *pDispenser = (CObjectDispenser*)pStruct;
	return pDispenser->m_hHealingTargets.Count();
}

IMPLEMENT_SERVERCLASS_ST( CObjectDispenser, DT_ObjectDispenser )
	SendPropInt( SENDINFO( m_iAmmoMetal ), 10 ),

	SendPropArray2( 
		SendProxyArrayLength_HealingArray,
		SendPropInt("healing_array_element", 0, SIZEOF_IGNORE, NUM_NETWORKED_EHANDLE_BITS, SPROP_UNSIGNED, SendProxy_HealingList), 
		MAX_PLAYERS, 
		0, 
		"healing_array"
		)

END_SEND_TABLE()

BEGIN_DATADESC( CObjectDispenser )
	DEFINE_THINKFUNC( RefillThink ),
	DEFINE_THINKFUNC( DispenseThink ),
END_DATADESC()


LINK_ENTITY_TO_CLASS(obj_dispenser, CObjectDispenser);
PRECACHE_REGISTER(obj_dispenser);

#define DISPENSER_MAX_HEALTH	150

// How much of each ammo gets added per refill
#define DISPENSER_REFILL_METAL_AMMO			40

// How much ammo is given our per use
#define DISPENSER_DROP_PRIMARY		40
#define DISPENSER_DROP_SECONDARY	40
#define DISPENSER_DROP_METAL		40

ConVar obj_dispenser_heal_rate( "obj_dispenser_heal_rate", "10.0", FCVAR_CHEAT |FCVAR_DEVELOPMENTONLY );

class CDispenserTouchTrigger : public CBaseTrigger
{
	DECLARE_CLASS( CDispenserTouchTrigger, CBaseTrigger );

public:
	CDispenserTouchTrigger() {}

	void Spawn( void )
	{
		BaseClass::Spawn();
		AddSpawnFlags( SF_TRIGGER_ALLOW_CLIENTS );
		InitTrigger();
		SetSolid( SOLID_BBOX );
		UTIL_SetSize(this, Vector(-70,-70,-70), Vector(70,70,70) );
	}

	virtual void StartTouch( CBaseEntity *pEntity )
	{
		CBaseEntity *pParent = GetOwnerEntity();

		if ( pParent )
		{
			pParent->StartTouch( pEntity );
		}
	}

	virtual void EndTouch( CBaseEntity *pEntity )
	{
		CBaseEntity *pParent = GetOwnerEntity();

		if ( pParent )
		{
			pParent->EndTouch( pEntity );
		}
	}
};

LINK_ENTITY_TO_CLASS( dispenser_touch_trigger, CDispenserTouchTrigger );

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CObjectDispenser::CObjectDispenser()
{
	SetMaxHealth( DISPENSER_MAX_HEALTH );
	m_iHealth = DISPENSER_MAX_HEALTH;
	UseClientSideAnimation();

	m_hTouchingEntities.Purge();

	SetType( OBJ_DISPENSER );
}

CObjectDispenser::~CObjectDispenser()
{
	if ( m_hTouchTrigger.Get() )
	{
		UTIL_Remove( m_hTouchTrigger );
	}

	int iSize = m_hHealingTargets.Count();
	for ( int i = iSize-1; i >= 0; i-- )
	{
		EHANDLE hOther = m_hHealingTargets[i];

		StopHealing( hOther );
	}

	StopSound( "Building_Dispenser.Idle" );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectDispenser::Spawn()
{
	SetModel( DISPENSER_MODEL_PLACEMENT );
	SetSolid( SOLID_BBOX );

	UTIL_SetSize(this, DISPENSER_MINS, DISPENSER_MAXS);
	m_takedamage = DAMAGE_YES;

	m_iAmmoMetal = 0;

	BaseClass::Spawn();
}

//-----------------------------------------------------------------------------
// Purpose: Start building the object
//-----------------------------------------------------------------------------
bool CObjectDispenser::StartBuilding( CBaseEntity *pBuilder )
{
	SetModel( DISPENSER_MODEL_BUILDING );

	CreateBuildPoints();

	return BaseClass::StartBuilding( pBuilder );
}

void CObjectDispenser::SetModel( const char *pModel )
{
	BaseClass::SetModel( pModel );
	UTIL_SetSize(this, DISPENSER_MINS, DISPENSER_MAXS);
}

//-----------------------------------------------------------------------------
// Purpose: Finished building
//-----------------------------------------------------------------------------
void CObjectDispenser::OnGoActive( void )
{
	CTFPlayer *pBuilder = GetBuilder();

	Assert( pBuilder );

	if ( !pBuilder )
		return;

	SetModel( DISPENSER_MODEL );

	// Put some ammo in the Dispenser
	m_iAmmoMetal = 25;

	// Begin thinking
	SetContextThink( &CObjectDispenser::RefillThink, gpGlobals->curtime + 3, REFILL_CONTEXT );
	SetContextThink( &CObjectDispenser::DispenseThink, gpGlobals->curtime + 0.1, DISPENSE_CONTEXT );

	m_flNextAmmoDispense = gpGlobals->curtime + 0.5;

	m_hTouchTrigger = CBaseEntity::Create( "dispenser_touch_trigger", GetAbsOrigin(), vec3_angle, this );

	BaseClass::OnGoActive();

	EmitSound( "Building_Dispenser.Idle" );
}

//-----------------------------------------------------------------------------
// Spawn the vgui control screens on the object
//-----------------------------------------------------------------------------
void CObjectDispenser::GetControlPanelInfo( int nPanelIndex, const char *&pPanelName )
{
	// Panels 0 and 1 are both control panels for now
	if ( nPanelIndex == 0 || nPanelIndex == 1 )
	{
		if ( GetTeamNumber() == TF_TEAM_RED )
		{
			pPanelName = "screen_obj_dispenser_red";
		}
		else
		{
			pPanelName = "screen_obj_dispenser_blue";
		}
	}
	else
	{
		BaseClass::GetControlPanelInfo( nPanelIndex, pPanelName );
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectDispenser::Precache()
{
	BaseClass::Precache();

	int iModelIndex;

	PrecacheModel( DISPENSER_MODEL_PLACEMENT );

	iModelIndex = PrecacheModel( DISPENSER_MODEL_BUILDING );
	PrecacheGibsForModel( iModelIndex );

	iModelIndex = PrecacheModel( DISPENSER_MODEL );
	PrecacheGibsForModel( iModelIndex );

	PrecacheVGuiScreen( "screen_obj_dispenser_blue" );
	PrecacheVGuiScreen( "screen_obj_dispenser_red" );

	PrecacheScriptSound( "Building_Dispenser.Idle" );
	PrecacheScriptSound( "Building_Dispenser.GenerateMetal" );
	PrecacheScriptSound( "Building_Dispenser.Heal" );

	PrecacheParticleSystem( "dispenser_heal_red" );
	PrecacheParticleSystem( "dispenser_heal_blue" );
}

//-----------------------------------------------------------------------------
// If detonated, do some damage
//-----------------------------------------------------------------------------
void CObjectDispenser::DetonateObject( void )
{
	/*
	float flDamage = min( 100 + m_iAmmoMetal, 250 );

	ExplosionCreate( 
		GetAbsOrigin(),
		GetAbsAngles(),
		GetBuilder(),
		flDamage,	//magnitude
		flDamage,		//radius
		0,
		0.0f,				//explosion force
		this,				//inflictor
		DMG_BLAST | DMG_HALF_FALLOFF);
	*/

	BaseClass::DetonateObject();
}

//-----------------------------------------------------------------------------
// Handle commands sent from vgui panels on the client 
//-----------------------------------------------------------------------------
bool CObjectDispenser::ClientCommand( CTFPlayer *pPlayer, const CCommand &args )
{
	const char *pCmd = args[0];
	if ( FStrEq( pCmd, "use" ) )
	{
		// I can't do anything if I'm not active
		if ( !ShouldBeActive() ) 
			return true;

		// player used the dispenser
		if ( DispenseAmmo( pPlayer ) )
		{
			CSingleUserRecipientFilter filter( pPlayer );
			pPlayer->EmitSound( filter, pPlayer->entindex(), "BaseCombatCharacter.AmmoPickup" );
		}

		return true;
	}
	else if ( FStrEq( pCmd, "repair" ) )
	{
		Command_Repair( pPlayer );
		return true;
	}

	return BaseClass::ClientCommand( pPlayer, args );
}

bool CObjectDispenser::DispenseAmmo( CTFPlayer *pPlayer )
{
	int iTotalPickedUp = 0;

	// primary
	int iPrimary = pPlayer->GiveAmmo( DISPENSER_DROP_PRIMARY, TF_AMMO_PRIMARY );
	iTotalPickedUp += iPrimary;

	// secondary
	int iSecondary = pPlayer->GiveAmmo( DISPENSER_DROP_SECONDARY, TF_AMMO_SECONDARY );
	iTotalPickedUp += iSecondary;

	// metal
	int iMetal = pPlayer->GiveAmmo( min( m_iAmmoMetal, DISPENSER_DROP_METAL ), TF_AMMO_METAL );
	m_iAmmoMetal -= iMetal;
	iTotalPickedUp += iMetal;

	if ( iTotalPickedUp > 0 )
	{
		EmitSound( "BaseCombatCharacter.AmmoPickup" );
		return true;
	}

	// return false if we didn't pick up anything
	return false;
}

void CObjectDispenser::RefillThink( void )
{
	SetContextThink( &CObjectDispenser::RefillThink, gpGlobals->curtime + 6, REFILL_CONTEXT );

	if ( IsDisabled() )
	{
		return;
	}

	// Auto-refill half the amount as tfc, but twice as often
	if ( m_iAmmoMetal < DISPENSER_MAX_METAL_AMMO )
	{
		m_iAmmoMetal = min( m_iAmmoMetal + DISPENSER_MAX_METAL_AMMO * 0.1, DISPENSER_MAX_METAL_AMMO );
		EmitSound( "Building_Dispenser.GenerateMetal" );
	}
}

//-----------------------------------------------------------------------------
// Generate ammo over time
//-----------------------------------------------------------------------------
void CObjectDispenser::DispenseThink( void )
{
	if ( IsDisabled() )
	{
		// Don't heal or dispense ammo
		SetContextThink( &CObjectDispenser::DispenseThink, gpGlobals->curtime + 0.1, DISPENSE_CONTEXT );

		// stop healing everyone
		for ( int i=m_hHealingTargets.Count()-1; i>=0; i-- )
		{
			EHANDLE hEnt = m_hHealingTargets[i];

			CBaseEntity *pOther = hEnt.Get();

			if ( pOther )
			{
				StopHealing( pOther );
			}
		}

		return;
	}

	if ( m_flNextAmmoDispense <= gpGlobals->curtime )
	{
		int iNumNearbyPlayers = 0;

		// find players in sphere, that are visible
		static float flRadius = 64;
		Vector vecOrigin = GetAbsOrigin() + Vector(0,0,32);

		CBaseEntity *pListOfNearbyEntities[32];
		int iNumberOfNearbyEntities = UTIL_EntitiesInSphere( pListOfNearbyEntities, 32, vecOrigin, flRadius, FL_CLIENT );
		for ( int i=0;i<iNumberOfNearbyEntities;i++ )
		{
			CTFPlayer *pPlayer = ToTFPlayer( pListOfNearbyEntities[i] );

			if ( !pPlayer || !pPlayer->IsAlive() )
				continue;

			DispenseAmmo( pPlayer );

			iNumNearbyPlayers++;
		}

		// Try to dispense more often when no players are around so we 
		// give it as soon as possible when a new player shows up
		m_flNextAmmoDispense = gpGlobals->curtime + ( ( iNumNearbyPlayers > 0 ) ? 1.0 : 0.1 );
	}	

	// for each player in touching list
	int iSize = m_hTouchingEntities.Count();
	for ( int i = iSize-1; i >= 0; i-- )
	{
		EHANDLE hOther = m_hTouchingEntities[i];

		CBaseEntity *pEnt = hOther.Get();
		bool bHealingTarget = IsHealingTarget( pEnt );
		bool bValidHealTarget = CouldHealTarget( pEnt );

		if ( bHealingTarget && !bValidHealTarget )
		{
			// if we can't see them, remove them from healing list
			// does nothing if we are not healing them already
			StopHealing( pEnt );
		}
		else if ( !bHealingTarget && bValidHealTarget )
		{
			// if we can see them, add to healing list	
			// does nothing if we are healing them already
			StartHealing( pEnt );
		}	
	}

	SetContextThink( &CObjectDispenser::DispenseThink, gpGlobals->curtime + 0.1, DISPENSE_CONTEXT );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectDispenser::StartTouch( CBaseEntity *pOther )
{
	// add to touching entities
	EHANDLE hOther = pOther;
	m_hTouchingEntities.AddToTail( hOther );

	if ( !IsBuilding() && !IsDisabled() && CouldHealTarget( pOther ) && !IsHealingTarget( pOther ) )
	{
		// try to start healing them
		StartHealing( pOther );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CObjectDispenser::EndTouch( CBaseEntity *pOther )
{
	// remove from touching entities
	EHANDLE hOther = pOther;
	m_hTouchingEntities.FindAndRemove( hOther );

	// remove from healing list
	StopHealing( pOther );
}

//-----------------------------------------------------------------------------
// Purpose: Try to start healing this target
//-----------------------------------------------------------------------------
void CObjectDispenser::StartHealing( CBaseEntity *pOther )
{
	AddHealingTarget( pOther );

	CTFPlayer *pPlayer = ToTFPlayer( pOther );

	if ( pPlayer )
	{
		pPlayer->m_Shared.Heal( GetOwner(), obj_dispenser_heal_rate.GetFloat(), true );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Stop healing this target
//-----------------------------------------------------------------------------
void CObjectDispenser::StopHealing( CBaseEntity *pOther )
{
	bool bFound = false;

	EHANDLE hOther = pOther;
	bFound = m_hHealingTargets.FindAndRemove( hOther );

	if ( bFound )
	{
		CTFPlayer *pPlayer = ToTFPlayer( pOther );

		if ( pPlayer )
		{
			pPlayer->m_Shared.StopHealing( GetOwner() );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Is this a valid heal target? and not already healing them?
//-----------------------------------------------------------------------------
bool CObjectDispenser::CouldHealTarget( CBaseEntity *pTarget )
{
	if ( !pTarget->FVisible( this, MASK_BLOCKLOS ) )
		return false;

	if ( pTarget->IsPlayer() && pTarget->IsAlive() )
	{
		CTFPlayer *pTFPlayer = ToTFPlayer( pTarget );

		// don't heal enemies unless they are disguised as our team
		int iTeam = GetTeamNumber();
		int iPlayerTeam = pTFPlayer->GetTeamNumber();

		if ( iPlayerTeam != iTeam && pTFPlayer->m_Shared.InCond( TF_COND_DISGUISED ) )
		{
			iPlayerTeam = pTFPlayer->m_Shared.GetDisguiseTeam();
		}

		if ( iPlayerTeam != iTeam )
		{
			return false;
		}

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CObjectDispenser::AddHealingTarget( CBaseEntity *pOther )
{
	// add to tail
	EHANDLE hOther = pOther;
	m_hHealingTargets.AddToTail( hOther );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CObjectDispenser::RemoveHealingTarget( CBaseEntity *pOther )
{
	// remove
	EHANDLE hOther = pOther;
	m_hHealingTargets.FindAndRemove( hOther );
}

//-----------------------------------------------------------------------------
// Purpose: Are we healing this target already
//-----------------------------------------------------------------------------
bool CObjectDispenser::IsHealingTarget( CBaseEntity *pTarget )
{
	EHANDLE hOther = pTarget;
	return m_hHealingTargets.HasElement( hOther );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int CObjectDispenser::DrawDebugTextOverlays(void) 
{
	int text_offset = BaseClass::DrawDebugTextOverlays();

	if (m_debugOverlays & OVERLAY_TEXT_BIT) 
	{
		char tempstr[512];
		Q_snprintf( tempstr, sizeof( tempstr ),"Metal: %d", m_iAmmoMetal );
		EntityText(text_offset,tempstr,0);
		text_offset++;
	}
	return text_offset;
}
