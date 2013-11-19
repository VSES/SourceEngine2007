//===== Copyright © 1996-2006, Valve Corporation, All rights reserved. ======//
//
// Base class for windows that draw vgui in Maya
//
//===========================================================================//

#ifndef VSVGUIWINDOW_H
#define VSVGUIWINDOW_H

#ifdef _WIN32
#pragma once
#endif


#include "imayavgui.h"
#include "vgui_controls/Frame.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IMayaVGui;


//-----------------------------------------------------------------------------
// The singleton is defined here twice just so we don't have to include valvemaya.h also
//-----------------------------------------------------------------------------
extern IMayaVGui *g_pMayaVGui;


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
namespace vgui
{
	class EditablePanel;
}


//-----------------------------------------------------------------------------
// Creates, destroys a maya vgui window
//-----------------------------------------------------------------------------
void CreateMayaVGuiWindow( vgui::EditablePanel *pRootPanel, const char *pPanelName );
void DestroyMayaVGuiWindow( const char *pPanelName );


//-----------------------------------------------------------------------------
// Factory used to install vgui windows easily
//-----------------------------------------------------------------------------
class CVsVguiWindowFactoryBase : public IMayaVguiWindowFactory
{
public:
	CVsVguiWindowFactoryBase( const char *pWindowTypeName, const char *pDccStartupCommand );

	// Returns the DCC command
	const char *GetDccStartupCommand() const;

	// Registers/deregisters all vgui windows
	static void RegisterAllVguiWindows( );
	static void UnregisterAllVguiWindows( );

protected:
	const char *m_pWindowTypeName;
	const char *m_pDccStartupCommand;

private:
	CVsVguiWindowFactoryBase *m_pNext;
	static CVsVguiWindowFactoryBase *s_pFirstCommandFactory;
};


template< class T >
class CVsVguiWindowFactory : public CVsVguiWindowFactoryBase
{
	typedef CVsVguiWindowFactoryBase BaseClass;

public:
	CVsVguiWindowFactory( const char *pWindowTypeName, const char *pDccCommand ) : BaseClass( pWindowTypeName, pDccCommand )
	{
	}

	virtual void CreateVguiWindow( const char *pPanelName )
	{
		T *pVguiPanel = new T( NULL, pPanelName );
		vgui::Frame* pFrame = dynamic_cast<vgui::Frame*>( pVguiPanel );
		if ( pFrame )
		{
			pFrame->SetSizeable( false );
			pFrame->SetCloseButtonVisible( false );
			pFrame->SetMoveable( false );
		}
		CreateMayaVGuiWindow( pVguiPanel, pPanelName );
	}

	virtual void DestroyVguiWindow( const char *pPanelName )
	{
		DestroyMayaVGuiWindow( pPanelName );
	}

private:
};


#define INSTALL_MAYA_VGUI_WINDOW( _className, _windowTypeName, _dccCommand )	\
	static CVsVguiWindowFactory< _className > s_VsVguiWindowFactory##_className##( _windowTypeName, _dccCommand )


#endif // VSVGUIWINDOW_H
