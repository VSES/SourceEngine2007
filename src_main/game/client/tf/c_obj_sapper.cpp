//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "c_obj_sapper.h"
#include <igameevents.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Start thinking
//-----------------------------------------------------------------------------
void C_ObjectSapper::OnDataChanged( DataUpdateType_t type )
{
	BaseClass::OnDataChanged( type );

	if ( type == DATA_UPDATE_CREATED )
	{
		SetNextClientThink( CLIENT_THINK_ALWAYS );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Create the sparking effect if we're built and ready
//-----------------------------------------------------------------------------
void C_ObjectSapper::ClientThink( void )
{
	IGameEvent *event = gameeventmanager->CreateEvent( "building_info_changed" );
	if ( event )
	{
		event->SetInt( "building_type", OBJ_ATTACHMENT_SAPPER );
		gameeventmanager->FireEventClientSide( event );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Status string for hud
//-----------------------------------------------------------------------------
void C_ObjectSapper::GetStatusText( wchar_t *pStatus, int iMaxStatusLen )
{
	float flHealthPercent = (float)GetHealth() / (float)GetMaxHealth();
	wchar_t wszHealthPercent[32];
	_snwprintf(wszHealthPercent, sizeof(wszHealthPercent)/sizeof(wchar_t) - 1, L"%d%%", (int)( flHealthPercent * 100 ) );

	wchar_t *pszTemplate;

	if ( IsBuilding() )
	{
		pszTemplate = g_pVGuiLocalize->Find( "#TF_ObjStatus_Sapper_Building" );
	}
	else
	{
		pszTemplate = g_pVGuiLocalize->Find( "#TF_ObjStatus_Sapper" );
	}

	if ( pszTemplate )
	{
		wchar_t wszTargetHealthPercent[32];
		wszTargetHealthPercent[0] = '\0';

		C_BaseObject *pParent = GetParentObject();
		Assert( pParent );
		if ( pParent )
		{
			float flTargetHealthPercent = (float)pParent->GetHealth() / (float)pParent->GetMaxHealth();
			_snwprintf(wszTargetHealthPercent, sizeof(wszTargetHealthPercent)/sizeof(wchar_t) - 1, L"%d%%", (int)( flTargetHealthPercent * 100 ) );
		}

		g_pVGuiLocalize->ConstructString( pStatus, iMaxStatusLen, pszTemplate,
			2,
			wszHealthPercent,
			wszTargetHealthPercent);
	}
}

IMPLEMENT_CLIENTCLASS_DT(C_ObjectSapper, DT_ObjectSapper, CObjectSapper)
END_RECV_TABLE()