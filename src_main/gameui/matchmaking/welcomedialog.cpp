//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Matchmaking's "main menu"
//
//=============================================================================//

#include "welcomedialog.h"
#include "gameui_interface.h"
#include "vgui_controls/MessageDialog.h"
#include "ixboxsystem.h"
#include "engineinterface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//--------------------------------
// CPlayerMatchDialog
//--------------------------------
CWelcomeDialog::CWelcomeDialog( vgui::Panel *pParent ) : BaseClass( pParent, "WelcomeDialog" )
{
	// do nothing
} 

//---------------------------------------------------------
// Purpose: Set the title and menu positions
//---------------------------------------------------------
void CWelcomeDialog::PerformLayout( void )
{
	BaseClass::PerformLayout();
}

//-----------------------------------------------------------------
// Purpose: Forward commands to the matchmaking base panel
//-----------------------------------------------------------------
void CWelcomeDialog::OnCommand( const char *pCommand )
{
	BaseClass::OnCommand( pCommand );
}

//-------------------------------------------------------
// Keyboard input
//-------------------------------------------------------
void CWelcomeDialog::OnKeyCodePressed( vgui::KeyCode code )
{
	switch( code )
	{
	case KEY_XBUTTON_B:
		if ( GameUI().IsInLevel() )
		{
			m_pParent->OnCommand( "ResumeGame" );
		}
		break;

	default:
		BaseClass::OnKeyCodePressed( code );
		break;
	}
}