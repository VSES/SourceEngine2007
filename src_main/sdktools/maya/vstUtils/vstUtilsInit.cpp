//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: The plugin entry and exit points
//
//=============================================================================

// Maya Headers

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include <maya/MHWShaderSwatchGenerator.h>
#include <maya/MPxTransformationMatrix.h>

// Valve Headers
#include "mathlib/mathlib.h"
#include "VsMayaMPxFactory.h"

//-----------------------------------------------------------------------------
//
// Purpose: Entry point that Maya calls when this plugin is loaded into Maya
// Input  : The Maya MObject representing the plugin
// Output : MStatus::kSuccess if everything went ok, MStatus::kFailure otherwise
//
//-----------------------------------------------------------------------------

EXPORT MStatus initializePlugin( MObject mObject )
{
	MStatus retVal;
	MStatus regStat;

	MFnPlugin pluginFn( mObject, "Valve", "1.0", "Any" );

	// For CVstAimNode
	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f, false, false, false, false );

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
	MStatus retVal;
	MStatus regStat;

	MFnPlugin pluginFn( mObject );

	return CVsMayaMPxFactoryBase::DeregisterEverything( pluginFn );
}