//===== Copyright © 1996-2006, Valve Corporation, All rights reserved. ======//
//
// Factory class for installing vgui window helpers
// NOTE: This is in a separate file to avoid needing to add library dependencies
//
//===========================================================================//

#include "vsvguiwindow.h"

//-----------------------------------------------------------------------------
//
// Base class for creating vgui windows
//
//-----------------------------------------------------------------------------
CVsVguiWindowFactoryBase *CVsVguiWindowFactoryBase::s_pFirstCommandFactory;

CVsVguiWindowFactoryBase::CVsVguiWindowFactoryBase( const char *pWindowTypeName )
{
	// Link the factory into a list
	m_pNext = s_pFirstCommandFactory;
	s_pFirstCommandFactory = this;
	m_pWindowTypeName = pWindowTypeName;
}


//-----------------------------------------------------------------------------
// Registers/deregisters all vgui window types
//-----------------------------------------------------------------------------
void CVsVguiWindowFactoryBase::RegisterAllVguiWindows( )
{
	CVsVguiWindowFactoryBase *pFactory;
	for ( pFactory = s_pFirstCommandFactory; pFactory; pFactory = pFactory->m_pNext )
	{
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
