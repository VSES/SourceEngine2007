//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: The plugin entry and exit points
//
//=============================================================================


// Local Headers
#include "valvemaya.h"
#include "tier0/platform.h"
#include "appframework/appframework.h"
#include "datamodel/idatamodel.h"
#include "datamodel/dmelementfactoryhelper.h"
#include "IStudioRender.h"
#include "datacache/idatacache.h"
#include "datacache/imdlcache.h"
#include "vphysics_interface.h"
//#include "vsmayacommand.h"
#include "vgui/iinput.h"
#include "vgui_controls/controls.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include "vstdlib/iprocessutils.h"

#include <windows.h>
// xsi headers
#include "xsi_pluginregistrar.h"
#include "xsi_decl.h"
// defines
#define PLUGIN_NAME	"vsMDLPreview"

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
static bool g_okToUnload = true;


BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}


	return TRUE;
}
//-----------------------------------------------------------------------------
// Interfaces used by movieobjects
//-----------------------------------------------------------------------------
class IGlobalFlexController *GlobalFlexController()
{
	return 0;
}


//-----------------------------------------------------------------------------
//
// Purpose: Valve entry point called before any Valve DLLs are loaded to determine
//			which Valve DLLs this plugin requires
// Input  :
// Output : A const pointer to an array of AppSystemInfo_t's which must exist
//			after this function returns, could do a reference counted smart
//			pointer perhaps
//
//-----------------------------------------------------------------------------
DLL_EXPORT const AppSystemInfo_t *VsMayaGetAppSystems( )
{
	static const AppSystemInfo_t appSystems[] = 
	{
		{ "vstdlib.dll",			PROCESS_UTILS_INTERFACE_VERSION },
		{ "studiorender.dll",		STUDIO_RENDER_INTERFACE_VERSION },
		{ "vphysics.dll",			VPHYSICS_INTERFACE_VERSION },
		{ "datacache.dll",			DATACACHE_INTERFACE_VERSION },
		{ "datacache.dll",			MDLCACHE_INTERFACE_VERSION },

		{ "", "" }	// Required to terminate the list
	};

	return appSystems;
}


//-----------------------------------------------------------------------------
//
// Purpose: Valve entry point called after the Valve DLLs are loaded
// Input  : CreateInterfaceFn factory
// Output : true is everything is good, false otherwise
//
//-----------------------------------------------------------------------------
DLL_EXPORT bool VsMayaConnect( CreateInterfaceFn factory )
{
	if ( !ValveMaya::ConnectLibraries( factory ) )
		return false;

	if ( !g_pFullFileSystem || !g_pDataModel )
	{
		Warning( "vsMDLPreview is missing a required interface!\n" );
		return false;
	}

	// Can't unload this plugin until the libraries have been disconnected
	g_okToUnload = false;

	return true;
}


//-----------------------------------------------------------------------------
//
// Purpose: Valve entry point called when the Valve libraries are being unloaded
// Input  :
// Output :
//
//-----------------------------------------------------------------------------
DLL_EXPORT void VsMayaDisconnect()
{
	ValveMaya::DisconnectLibraries();

	// Ok to unload this plugin as the libraries have been disconnected
	g_okToUnload = true;
}

XSIPLUGINCALLBACK XSI::CStatus XSILoadPlugin( XSI::PluginRegistrar& in_reg )
{
	in_reg.PutAuthor( L"Valve" );
	in_reg.PutName( L"Valve Model Preview" );
	in_reg.PutVersion( 1, 0 );
	
	// register XSI commands here
 
	return XSI::CStatus::OK;	
}

//********************************************************************
//
// @mfunc	XSILoadPlugin | Called by XSI when it needs to unload the
//							plugin.
//
//
//********************************************************************
XSIPLUGINCALLBACK XSI::CStatus XSIUnloadPlugin( const XSI::PluginRegistrar& in_reg )
{
	return XSI::CStatus::OK;
}
/*
//-----------------------------------------------------------------------------
//
// Purpose: Entry point that Maya calls when this plugin is loaded into Maya
// Input  : The Maya MObject representing the plugin
// Output : MStatus::kSuccess if everything went ok, MStatus::kFailure otherwise
//
//-----------------------------------------------------------------------------
EXPORT MStatus initializePlugin( MObject mObject )
{
	MFnPlugin pluginFn( mObject, "Valve", "1.0", "Any" );
	MStatus nRetVal = CVsMayaCommandFactoryBase::RegisterAllCommands( pluginFn );
	if ( !nRetVal )
		return nRetVal;

	// Hook in the appropriate MEL scripts
	MGlobal::executeCommand( "source vsMDLPreview" );

	return nRetVal;
}


//-----------------------------------------------------------------------------
//
// Purpose: Exit point that Maya calls when this plugin is unloaded from Maya
// Input  : The Maya MObject representing the plugin
// Output : MStatus::kSuccess if everything went ok, MStatus::kFailure otherwise
//
//-----------------------------------------------------------------------------
EXPORT MStatus uninitializePlugin( MObject mObject )
{
	if ( !g_okToUnload )
		return MS::kFailure;

	MFnPlugin pluginFn( mObject );
	return CVsMayaCommandFactoryBase::DeregisterAllCommands( pluginFn );
}
*/
