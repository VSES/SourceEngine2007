//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Put things which require Valve libraries in a separate file so
//          things that don't reference these symbols don't need the extra
//          libs added
//
//=============================================================================

#include "mathlib/mathlib.h"
#include "tier1/tier1.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include "dme_controls/dmecontrols.h"
#include "imayavgui.h"
#include "vsvguiwindow.h"
#include "idccmain.h"

#include "valveMaya.h"

//-----------------------------------------------------------------------------
// Maya-specific library singletons
//-----------------------------------------------------------------------------
IMayaVGui *g_pMayaVGui;
IDCCMain *g_pDCCMain;


//-----------------------------------------------------------------------------
// Connect, disconnect
//-----------------------------------------------------------------------------
bool ValveMaya::ConnectLibraries( CreateInterfaceFn factory )
{
	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f, false, false, false, false );
	ConnectTier1Libraries( &factory, 1 );
	ConnectTier2Libraries( &factory, 1 );
	ConnectTier3Libraries( &factory, 1 );

	if ( !ConnectDataModel( factory ) )
		return false;

	if ( InitDataModel( ) != INIT_OK )
		return false;

	// initialize interfaces
	if ( !vgui::VGui_InitDmeInterfacesList( "ValveSource", &factory, 1 ) )
		return false;

	g_pMayaVGui = (IMayaVGui*)factory( MAYA_VGUI_INTERFACE_VERSION, NULL );
	if ( !g_pMayaVGui )
		return false;

	g_pDCCMain = (IDCCMain*)factory( DCC_MAIN_INTERFACE_VERSION, NULL );
	if ( !g_pDCCMain )
		return false;

	// Hook up all vgui window factories
	CVsVguiWindowFactoryBase::RegisterAllVguiWindows( );

	return true;
}

void ValveMaya::DisconnectLibraries()
{
	// Disconnect up all vgui window factories
	CVsVguiWindowFactoryBase::UnregisterAllVguiWindows( );
	g_pMayaVGui = NULL;
	g_pDCCMain = NULL;

	ShutdownDataModel();

	DisconnectDataModel();
	DisconnectTier3Libraries();
	DisconnectTier2Libraries();
	DisconnectTier1Libraries();
}
