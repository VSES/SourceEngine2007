//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: The plugin entry and exit points
//
//=============================================================================

// Maya Headers

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>

// Valve Headers

#include "VsMayaMPxFactory.h"

#ifndef EXPORT
	#ifdef _WIN32
		#define EXPORT	_declspec( dllexport )
	#else 
		#define EXPORT	/* */
	#endif
#endif


//-----------------------------------------------------------------------------
// Purpose: Entry point that Maya calls when this plugin is loaded into Maya
// Input  : The Maya MObject representing the plugin
// Output : MStatus::kSuccess if everything went ok, MStatus::kFailure otherwise
//-----------------------------------------------------------------------------
EXPORT MStatus initializePlugin( MObject mObject )
{
	MFnPlugin pluginFn( mObject, "Valve", "1.0", "Any" );

	return CVsMayaMPxFactoryBase::RegisterEverything( pluginFn );
}


//-----------------------------------------------------------------------------
// Purpose: Exit point that Maya calls when this plugin is unloaded from Maya
// Input  : The Maya MObject representing the plugin
// Output : MStatus::kSuccess if everything went ok, MStatus::kFailure otherwise
//-----------------------------------------------------------------------------
EXPORT MStatus uninitializePlugin( MObject mObject )
{
	MFnPlugin pluginFn( mObject );

	return CVsMayaMPxFactoryBase::DeregisterEverything( pluginFn );
}