//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: The plugin entry and exit points
//
//=============================================================================

// Maya includes
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

// Local includes
#include "vstSmdIOCmd.h"
#include "vstSmdIOTranslator.h"

#ifndef EXPORT
	#ifdef _WIN32
		#define EXPORT	_declspec( dllexport )
	#else 
		#define EXPORT	/* */
	#endif
#endif

//-----------------------------------------------------------------------------
//
// Purpose: Entry point that Maya calls when this plugin is loaded into Maya
// Input  : The Maya MObject representing the plugin
// Output : MStatus::kSuccess if everything went ok, MStatus::kFailure otherwise
//
//-----------------------------------------------------------------------------
EXPORT MStatus initializePlugin(
	MObject mObject )
{
	MStatus regVal;
	MStatus retVal;

	MFnPlugin pluginFn( mObject, "Valve", "1.0", "Any" );

	if ( !pluginFn.registerCommand( "vstSmdIO", CVstSmdIOCmd::Factory, CVstSmdIOCmd::SyntaxFactory ) )
		return MS::kFailure;

	if ( !pluginFn.registerCommand( "vstsmdio", CVstSmdIOCmd::Factory, CVstSmdIOCmd::SyntaxFactory ) )
		return MS::kFailure;

	if ( !pluginFn.registerFileTranslator( "Valve SMD Model", "vstSmdModel.xpm", CVstSmdModel::Factory, "vstSmdModelTranslatorOptions", "" ) )
		return MS::kFailure;

	if ( !pluginFn.registerFileTranslator( "Valve SMD Animated Model", "vstSmdAnimatedModel.xpm", CVstSmdModelAnimation::Factory, "vstSmdModelAnimationTranslatorOptions", "" ) )
		return MS::kFailure;

	if ( !pluginFn.registerFileTranslator( "Valve SMD Physics Model", "vstSmdPhysics.xpm", CVstSmdPhysics::Factory, "vstSmdPhysicsTranslatorOptions", "" ) )
		return MS::kFailure;

	if ( !pluginFn.registerFileTranslator( "Valve SMD Animation", "vstSmdAnimation.xpm", CVstSmdAnimation::Factory, "vstSmdAnimationTranslatorOptions", "" ) )
		return MS::kFailure;

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose: Exit point that Maya calls when this plugin is unloaded from Maya
// Input  : The Maya MObject representing the plugin
// Output : MStatus::kSuccess if everything went ok, MStatus::kFailure otherwise
//
//-----------------------------------------------------------------------------
EXPORT MStatus uninitializePlugin(
	MObject mObject )
{
	MFnPlugin pluginFn( mObject );

	if ( !pluginFn.deregisterCommand( "vstsmdio" ) )
		return MS::kFailure;

	if ( !pluginFn.deregisterCommand( "vstSmdIO" ) )
		return MS::kFailure;

	if ( !pluginFn.deregisterFileTranslator( "Valve SMD Model" ) )
		return MS::kFailure;

	if ( !pluginFn.deregisterFileTranslator( "Valve SMD Animated Model" ) )
		return MS::kFailure;

	if ( !pluginFn.deregisterFileTranslator( "Valve SMD Physics Model" ) )
		return MS::kFailure;

	if ( !pluginFn.deregisterFileTranslator( "Valve SMD Animation" ) )
		return MS::kFailure;

	return MS::kSuccess;
}
