//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "hudelement.h"
#include <vgui_controls/Panel.h>
#include <vgui/isurface.h>
#include "clientmode_csnormal.h"
#include "cs_gamerules.h"
#include "hud_numericdisplay.h"


class CHudC4 : public CHudElement, public vgui::Panel
{
public:
	DECLARE_CLASS_SIMPLE( CHudC4, vgui::Panel );

	CHudC4( const char *name );

	virtual bool ShouldDraw();	
	virtual void Paint();
	virtual void Init();


private:
	CPanelAnimationVar( Color, m_clrIcon, "IconColor", "IconColor" );
	CPanelAnimationVar( Color, m_clrFlash, "FlashColor", "FlashColor" );

	CHudTexture *m_pIcon;

	float m_flNextFlashTime;
	bool m_bFlash;
};


DECLARE_HUDELEMENT( CHudC4 );


CHudC4::CHudC4( const char *pName ) :
	vgui::Panel( NULL, "HudC4" ), CHudElement( pName )
{
	SetParent( g_pClientMode->GetViewport() );
	m_pIcon = NULL;

	SetHiddenBits( HIDEHUD_PLAYERDEAD );
}


void CHudC4::Init()
{
	m_flNextFlashTime = 0;
	m_bFlash = false;
}


bool CHudC4::ShouldDraw()
{
	C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();

	// if we are spectating another player first person, check this player
	if ( pPlayer && (pPlayer->GetObserverMode() == OBS_MODE_IN_EYE) )
	{
		pPlayer = ToCSPlayer( pPlayer->GetObserverTarget() );
	}
	
	return pPlayer && pPlayer->HasC4();
}


void CHudC4::Paint()
{
	if ( !m_pIcon )
	{
		m_pIcon = gHUD.GetIcon( "c4" );
	}

	C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();

	if ( !pPlayer )
		return;

	if( pPlayer->m_bInBombZone )
	{
		if( m_flNextFlashTime < gpGlobals->curtime )
		{
			m_bFlash = !m_bFlash;
			m_flNextFlashTime = gpGlobals->curtime + 0.15;
		}
	}
	else
	{
		m_bFlash = false;
	}

	int x, y, w, h;
	GetBounds( x, y, w, h );

	if ( m_pIcon )
	{
		if( m_bFlash )
		{
			m_pIcon->DrawSelf( 0, 0, w, h, m_clrFlash );
		}
		else
		{
			m_pIcon->DrawSelf( 0, 0, w, h, m_clrIcon );
		}
	}
}

