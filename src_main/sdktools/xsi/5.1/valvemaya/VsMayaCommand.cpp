//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose:
//
//=============================================================================

#include "vsmayacommand.h"
#include "xsi_pluginregistrar.h"
#include "xsi_ref.h"
#include "xsi_context.h"


//-----------------------------------------------------------------------------
//
// Base class for valve maya commands
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVsMayaCommand::CVsMayaCommand()
{
}

	
//-----------------------------------------------------------------------------
//
// Helper class to install commands
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static members
//-----------------------------------------------------------------------------
CVsMayaCommandFactoryBase *CVsMayaCommandFactoryBase::s_pFirstCommandFactory;


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVsMayaCommandFactoryBase::CVsMayaCommandFactoryBase( const char *pCommandName )
{
	m_CommandName = pCommandName;

	// Hook into the list of command factories
	m_pNextFactory = s_pFirstCommandFactory;
	s_pFirstCommandFactory = this;
}


//-----------------------------------------------------------------------------
// Returns the commandname associated with the factory
//-----------------------------------------------------------------------------
const char *CVsMayaCommandFactoryBase::GetCommandName()
{
	return m_CommandName;
}


//-----------------------------------------------------------------------------
// Registers/deregisters all commands
//-----------------------------------------------------------------------------
bool CVsMayaCommandFactoryBase::RegisterAllCommands( XSI::PluginRegistrar& in_reg )
{
	CVsMayaCommandFactoryBase *pFactory;
	for ( pFactory = s_pFirstCommandFactory; pFactory; pFactory = pFactory->m_pNextFactory )
	{
		XSI::CString str;
		str.PutAsciiString( pFactory->GetCommandName() );
		pFactory->OnRegister( in_reg );
		in_reg.RegisterCommand( str );
	}

	return true;
}

void CVsMayaCommandFactoryBase::DeregisterAllCommands( const XSI::PluginRegistrar& in_reg )
{
}

