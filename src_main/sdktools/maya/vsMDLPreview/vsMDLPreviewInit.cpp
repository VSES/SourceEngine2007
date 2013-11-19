//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: The plugin entry and exit points
//
//=============================================================================

// Maya Headers

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxTransformationMatrix.h>

// Local Headers
#include "valvemaya.h"
#include "tier0/platform.h"
#include "appframework/appframework.h"
#include "vstdlib/iprocessutils.h"
#include "datamodel/idatamodel.h"
#include "datamodel/dmelementfactoryhelper.h"
#include "IStudioRender.h"
#include "datacache/idatacache.h"
#include "datacache/imdlcache.h"
#include "vphysics_interface.h"
#include "vsmayampxfactory.h"
#include "vgui/iinput.h"
#include "p4lib/ip4.h"
#include "vgui_controls/controls.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"


void UninstallAssetBuilderCallback();


//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
static bool g_okToUnload = true;


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
		{ "p4lib.dll",				P4_INTERFACE_VERSION },

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

	// Hook in the appropriate MEL scripts
	MString mayaCommand( "source vsSaveChanges" );
	MGlobal::executeCommand( mayaCommand );

	return CVsMayaMPxFactoryBase::RegisterEverything( pluginFn );
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

	// FIXME: Should we have a generic framework of stuff to call during uninitialize?
	UninstallAssetBuilderCallback();

	MFnPlugin pluginFn( mObject );
	return CVsMayaMPxFactoryBase::DeregisterEverything( pluginFn );
}
