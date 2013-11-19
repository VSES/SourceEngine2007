//===== Copyright © 1996-2006, Valve Corporation, All rights reserved. ======//
//
// Factory class for installing vgui window helpers
// NOTE: This is in a separate file to avoid needing to add library dependencies
//
//===========================================================================//

#include "vsvguiwindow.h"
#include <maya/MString.h>
#include <maya/MGlobal.h>


//-----------------------------------------------------------------------------
//
// Base class for creating vgui windows
//
//-----------------------------------------------------------------------------
CVsVguiWindowFactoryBase *CVsVguiWindowFactoryBase::s_pFirstCommandFactory;

CVsVguiWindowFactoryBase::CVsVguiWindowFactoryBase( const char *pWindowTypeName, const char *pDccCommand )
{
	// Link the factory into a list
	m_pNext = s_pFirstCommandFactory;
	m_pDccStartupCommand = pDccCommand;
	s_pFirstCommandFactory = this;
	m_pWindowTypeName = pWindowTypeName;
}


//-----------------------------------------------------------------------------
// Returns the DCC command
//-----------------------------------------------------------------------------
const char *CVsVguiWindowFactoryBase::GetDccStartupCommand() const
{
	return m_pDccStartupCommand;
}


//-----------------------------------------------------------------------------
// Registers/deregisters all vgui window types
//-----------------------------------------------------------------------------
void CVsVguiWindowFactoryBase::RegisterAllVguiWindows( )
{
	CVsVguiWindowFactoryBase *pFactory;
	for ( pFactory = s_pFirstCommandFactory; pFactory; pFactory = pFactory->m_pNext )
	{
		if ( pFactory->GetDccStartupCommand() )
		{
			// Hook in the appropriate MEL scripts
			MString mayaCommand( "source " );
			mayaCommand += pFactory->GetDccStartupCommand();
			MGlobal::executeCommand( mayaCommand );
		}
		g_pMayaVGui->InstallVguiWindowFactory( pFactory->m_pWindowTypeName, pFactory );
	}
}

void CVsVguiWindowFactoryBase::UnregisterAllVguiWindows( )
{
	CVsVguiWindowFactoryBase *pFactory;
	for ( pFactory = s_pFirstCommandFactory; pFactory; pFactory = pFactory->m_pNext )
	{
		g_pMayaVGui->RemoveVguiWindowFactory( pFactory->m_pWindowTypeName, pFactory );
	}
}
