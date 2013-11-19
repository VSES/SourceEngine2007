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
#include "c_cs_player.h"

class CHudDefuser : public CHudElement, public vgui::Panel
{
public:
	DECLARE_CLASS_SIMPLE( CHudDefuser, vgui::Panel );

	CHudDefuser( const char *name );

	virtual bool ShouldDraw();	
	virtual void Paint();

private:
	CPanelAnimationVar( Color, m_clrIcon, "IconColor", "IconColor" );

	CHudTexture *m_pIcon;
};


DECLARE_HUDELEMENT( CHudDefuser );


CHudDefuser::CHudDefuser( const char *pName ) :
	vgui::Panel( NULL, "HudDefuser" ), CHudElement( pName )
{
	SetParent( g_pClientMode->GetViewport() );
	m_pIcon = NULL;

	SetHiddenBits( HIDEHUD_PLAYERDEAD );
}

bool CHudDefuser::ShouldDraw()
{
	C_CSPlayer *pPlayer = C_CSPlayer::GetLocalCSPlayer();
	return pPlayer && pPlayer->HasDefuser();
}

void CHudDefuser::Paint()
{
	if ( !m_pIcon )
	{
		m_pIcon = gHUD.GetIcon( "defuser" );
	}
	
	if ( m_pIcon )
	{
		int x, y, w, h;
		GetBounds( x, y, w, h );
		
		m_pIcon->DrawSelf( 0, 0, w, h, m_clrIcon );
	}
}

