//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Client's CObjectSentrygun
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "c_baseobject.h"
#include "c_tf_player.h"
#include "vgui/ILocalize.h"
#include "c_obj_dispenser.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: RecvProxy that converts the Team's player UtlVector to entindexes
//-----------------------------------------------------------------------------
void RecvProxy_HealingList(  const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	C_ObjectDispenser *pDispenser = (C_ObjectDispenser*)pStruct;

	CBaseHandle *pHandle = (CBaseHandle*)(&(pDispenser->m_hHealingTargets[pData->m_iElement])); 
	RecvProxy_IntToEHandle( pData, pStruct, pHandle );

	// update the heal beams
	pDispenser->m_bUpdateHealingTargets = true;
}

void RecvProxyArrayLength_HealingArray( void *pStruct, int objectID, int currentArrayLength )
{
	C_ObjectDispenser *pDispenser = (C_ObjectDispenser*)pStruct;

	if ( pDispenser->m_hHealingTargets.Size() != currentArrayLength )
		pDispenser->m_hHealingTargets.SetSize( currentArrayLength );

	// update the heal beams
	pDispenser->m_bUpdateHealingTargets = true;
}

//-----------------------------------------------------------------------------
// Purpose: Dispenser object
//-----------------------------------------------------------------------------

IMPLEMENT_CLIENTCLASS_DT(C_ObjectDispenser, DT_ObjectDispenser, CObjectDispenser)
	RecvPropInt( RECVINFO( m_iAmmoMetal ) ),

	RecvPropArray2( 
		RecvProxyArrayLength_HealingArray,
		RecvPropInt( "healing_array_element", 0, SIZEOF_IGNORE, 0, RecvProxy_HealingList ), 
		MAX_PLAYERS, 
		0, 
		"healing_array"
		)
END_RECV_TABLE()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_ObjectDispenser::C_ObjectDispenser()
{
	m_bUpdateHealingTargets = false;
	m_bPlayingSound = false;

	m_pDamageEffects = NULL;
}

C_ObjectDispenser::~C_ObjectDispenser()
{
	StopSound( "Building_Dispenser.Heal" );
}

void C_ObjectDispenser::GetStatusText( wchar_t *pStatus, int iMaxStatusLen )
{
	float flHealthPercent = (float)GetHealth() / (float)GetMaxHealth();
	wchar_t wszHealthPercent[32];
	_snwprintf(wszHealthPercent, sizeof(wszHealthPercent)/sizeof(wchar_t) - 1, L"%d%%", (int)( flHealthPercent * 100 ) );

	wchar_t *pszTemplate;

	if ( IsBuilding() )
	{
		pszTemplate = g_pVGuiLocalize->Find( "#TF_ObjStatus_Dispenser_Building" );
	}
	else
	{
		pszTemplate = g_pVGuiLocalize->Find( "#TF_ObjStatus_Dispenser" );
	}

	if ( pszTemplate )
	{
		g_pVGuiLocalize->ConstructString( pStatus, iMaxStatusLen, pszTemplate,
			1,
			wszHealthPercent );
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : updateType - 
//-----------------------------------------------------------------------------
void C_ObjectDispenser::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	if ( m_bUpdateHealingTargets )
	{
		UpdateEffects();
		m_bUpdateHealingTargets = false;
	}
}

void C_ObjectDispenser::UpdateEffects( void )
{
	// Find all the targets we've stopped healing
	bool bStillHealing[MAX_DISPENSER_HEALING_TARGETS];
	for ( int i = 0; i < m_hHealingTargetEffects.Count(); i++ )
	{
		bStillHealing[i] = false;

		// Are we still healing this target?
		for ( int target = 0; target < m_hHealingTargets.Count(); target++ )
		{
			if ( m_hHealingTargets[target] && m_hHealingTargets[target] == m_hHealingTargetEffects[i].pTarget )
			{
				bStillHealing[i] = true;
				break;
			}
		}
	}

	// Now remove all the dead effects
	for ( int i = m_hHealingTargetEffects.Count()-1; i >= 0; i-- )
	{
		if ( !bStillHealing[i] )
		{
			ParticleProp()->StopEmission( m_hHealingTargetEffects[i].pEffect );
			m_hHealingTargetEffects.Remove(i);
		}
	}

	// Now add any new targets
	for ( int i = 0; i < m_hHealingTargets.Count(); i++ )
	{
		C_BaseEntity *pTarget = m_hHealingTargets[i].Get();

		// Loops through the healing targets, and make sure we have an effect for each of them
		if ( pTarget )
		{
			bool bHaveEffect = false;
			for ( int targets = 0; targets < m_hHealingTargetEffects.Count(); targets++ )
			{
				if ( m_hHealingTargetEffects[targets].pTarget == pTarget )
				{
					bHaveEffect = true;
					break;
				}
			}

			if ( bHaveEffect )
				continue;

			const char *pszEffectName;
			if ( GetTeamNumber() == TF_TEAM_RED )
			{
				pszEffectName = "dispenser_heal_red";
			}
			else
			{
				pszEffectName = "dispenser_heal_blue";
			}

			CNewParticleEffect *pEffect = ParticleProp()->Create( pszEffectName, PATTACH_POINT_FOLLOW, "heal_origin" );
			ParticleProp()->AddControlPoint( pEffect, 1, pTarget, PATTACH_ABSORIGIN_FOLLOW, NULL, Vector(0,0,50) );

			int iIndex = m_hHealingTargetEffects.AddToTail();
			m_hHealingTargetEffects[iIndex].pTarget = pTarget;
			m_hHealingTargetEffects[iIndex].pEffect = pEffect;

			// Start the sound over again every time we start a new beam
			StopSound( "Building_Dispenser.Heal" );

			CLocalPlayerFilter filter;
			EmitSound( filter, entindex(), "Building_Dispenser.Heal" );

			m_bPlayingSound = true;
		}
	}

	// Stop the sound if we're not healing anyone
	if ( m_bPlayingSound && m_hHealingTargets.Count() == 0 )
	{
		m_bPlayingSound = false;

		// stop the sound
		StopSound( "Building_Dispenser.Heal" );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Damage level has changed, update our effects
//-----------------------------------------------------------------------------
void C_ObjectDispenser::UpdateDamageEffects( BuildingDamageLevel_t damageLevel )
{
	if ( m_pDamageEffects )
	{
		m_pDamageEffects->StopEmission( false, false );
		m_pDamageEffects = NULL;
	}

	const char *pszEffect = "";

	switch( damageLevel )
	{
	case BUILDING_DAMAGE_LEVEL_LIGHT:
		pszEffect = "dispenserdamage_1";
		break;
	case BUILDING_DAMAGE_LEVEL_MEDIUM:
		pszEffect = "dispenserdamage_2";
		break;
	case BUILDING_DAMAGE_LEVEL_HEAVY:
		pszEffect = "dispenserdamage_3";
		break;
	case BUILDING_DAMAGE_LEVEL_CRITICAL:
		pszEffect = "dispenserdamage_4";
		break;

	default:
		break;
	}

	if ( Q_strlen(pszEffect) > 0 )
	{
		m_pDamageEffects = ParticleProp()->Create( pszEffect, PATTACH_ABSORIGIN );
	}
}

//-----------------------------------------------------------------------------
// Control screen 
//-----------------------------------------------------------------------------

DECLARE_VGUI_SCREEN_FACTORY( CDispenserControlPanel, "screen_obj_dispenser_blue" );
DECLARE_VGUI_SCREEN_FACTORY( CDispenserControlPanel_Red, "screen_obj_dispenser_red" );

//-----------------------------------------------------------------------------
// Constructor: 
//-----------------------------------------------------------------------------
CDispenserControlPanel::CDispenserControlPanel( vgui::Panel *parent, const char *panelName )
: BaseClass( parent, "CDispenserControlPanel" ) 
{
	m_pAmmoProgress = new RotatingProgressBar( this, "MeterArrow" );
}

//-----------------------------------------------------------------------------
// Deactivates buttons we can't afford
//-----------------------------------------------------------------------------
void CDispenserControlPanel::OnTickActive( C_BaseObject *pObj, C_TFPlayer *pLocalPlayer )
{
	BaseClass::OnTickActive( pObj, pLocalPlayer );

	Assert( dynamic_cast<C_ObjectDispenser*>(pObj) );
	C_ObjectDispenser *pDispenser = static_cast<C_ObjectDispenser*>(pObj);

	float flMetal = pDispenser->GetMetalAmmoCount() / (float)DISPENSER_MAX_METAL_AMMO;

	m_pAmmoProgress->SetProgress( flMetal );
}