//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef TFCCLASSMENU_H
#define TFCCLASSMENU_H
#ifdef _WIN32
#pragma once
#endif

#include <classmenu.h>
#include <commandpanel.h>
#include <vgui_controls/EditablePanel.h>
#include <FileSystem.h>
#include "tfc_shareddefs.h"
#include "cbase.h"
#include "tfc_gamerules.h"
#include "vgui_controls/imagepanel.h"


//-----------------------------------------------------------------------------
// Purpose: Draws the Terrorist class menu
//-----------------------------------------------------------------------------

class CTFCClassMenu : public CClassMenu
{
private:
	DECLARE_CLASS_SIMPLE( CTFCClassMenu, CClassMenu );
	
public:
	CTFCClassMenu::CTFCClassMenu(IViewPort *pViewPort);
	
	const char *GetName( void );
	void ShowPanel(bool bShow);
	void Update();
};


#endif // TFCCLASSMENU_H
