//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: The plugin entry and exit points
//
//=============================================================================

#define WIN32_LEAN_AND_MEAN
#include <io.h>

// Valve Headers

#include "tier0/dbg.h"
#include "appframework/appframework.h"
#include "appframework/IAppSystemgroup.h"
#include "tier0/icommandline.h"
#include "filesystem_init.h"
#include "stringpool.h"
#include "utlstring.h"
#include "utlvector.h"

// Maya Headers

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MHWShaderSwatchGenerator.h>

// Local Headers

#include "valveMaya.h"
#include "CVsMayaMasterApp.h"
#include "VsMayaMPxFactory.h"

#include "vsVmt.h"

// Static Variables

static SpewOutputFunc_t s_savedSpewOutputFunc(NULL);

static CVsMayaMasterApp s_vsMayaMasterApp;
static CSteamApplication s_steamApplication( &s_vsMayaMasterApp );

//-----------------------------------------------------------------------------
// Purpose: Valve Spew interface to throw away all of the spew output...
//          Just a bit of a hack for now to get around warning messages that
//          don't affect users and aren't important and will be fixed but
//          not for a while
// Input:	The type of spew message as well as a NULL terminated C string
//			containing the message to spew
// Output:	
//-----------------------------------------------------------------------------

SpewRetval_t VsSpewIgnoreFunc( SpewType_t type, char const *pMsg )
{	
	return SPEW_CONTINUE;
}


//-----------------------------------------------------------------------------
// Purpose: Valve Spew interface to Maya's output
// Input:	The type of spew message as well as a NULL terminated C string
//			containing the message to spew
// Output:	
//-----------------------------------------------------------------------------

SpewRetval_t VsSpewOutputFunc( SpewType_t type, char const *pMsg )
{	
	SpewRetval_t spewReturnVal( SPEW_CONTINUE );

	CUtlString tmpMsg( pMsg );
	char *const pBegin( tmpMsg.Get() );

#ifdef _DEBUG
	OutputDebugString( pBegin );
#endif // __DEBUG

	// For Maya, remove any spurious line endings
	for ( char *pEnd( pBegin + tmpMsg.Length() - 1 ); pEnd > pBegin; --pEnd )
		if ( *pEnd <= 32 || *pEnd >= 127 )
			*pEnd = '\0';
		else
			break;
	
	switch ( type )
	{
	case SPEW_ERROR:
	case SPEW_ASSERT:
		MGlobal::displayError( pBegin );
		spewReturnVal = SPEW_DEBUGGER;
		break;
	case SPEW_WARNING:
		MGlobal::displayWarning( pBegin );
		break;
	default:
		MGlobal::displayInfo( pBegin );
		break;
	}
	
	return spewReturnVal;
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
	// First thing, connect Valve Spew to VsSpewFunc saving the previous one
	s_savedSpewOutputFunc = GetSpewOutputFunc();

#ifdef _DEBUG
	// Now set the Valve Spew to VsSpewFunc
	SpewOutputFunc( VsSpewOutputFunc );
#else // __DEBUG
	SpewOutputFunc( VsSpewIgnoreFunc );
#endif // __DEBUG

	// Set up the app instance
	SetAppInstance( MhInstPlugin );

#ifdef _DEBUG
	CommandLine()->CreateCmdLine( "maya.exe -allowdebug" );
#endif

	// Tell ApplicationFramework to use VPROJECT to find dlls
	FileSystem_UseVProjectBinDir( true );

	// Figure out which other valveSource Maya plug-ins should be loaded
	MFnPlugin pluginFn( mObject, "Valve", "1.0", "Any" );

	// Register Maya Stuff; gotta do it before loading plugins
	MStatus retVal = CVsMayaMPxFactoryBase::RegisterEverything( pluginFn );

	MString loadPath( pluginFn.loadPath() + "/valveSource" );

	MString fileSpec( loadPath + "/vs*.mll" );

	__finddata64_t findData;
	intptr_t findHandle;

	if ( ( findHandle = _findfirst64( fileSpec.asChar(), &findData ) ) != -1 )
	{
		do
		{
			MString mllPath( loadPath + MString( "/" ) + findData.name );
			s_vsMayaMasterApp.LoadPlugin( findData.name, mllPath.asChar() );
		}
		while ( _findnext64( findHandle, &findData ) != -1 );
	}

	_findclose( findHandle );

	// Turn off Async mode in filesystem
//	CommandLine()->AppendParm( "-noasync", NULL );

	// Startup Steam
	s_steamApplication.Startup();

#ifndef _DEBUG
	// Now set the Valve Spew to VsSpewFunc
	SpewOutputFunc( VsSpewOutputFunc );
#endif // __DEBUG

	const MString &swatchName( MHWShaderSwatchGenerator::initialize() );
	const MString userClassification( "shader/surface/utility/:swatch/" + swatchName );

	pluginFn.registerNode( CVsVmtNode::s_name, CVsVmtNode::m_id, CVsVmtNode::Factory, CVsVmtNode::Initialize, MPxNode::kHwShaderNode, &userClassification );

	return retVal;
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

	MFnPlugin pluginFn( mObject );

	// Now detach all Steamy stuff
	s_steamApplication.Shutdown();

	pluginFn.deregisterNode( CVsVmtNode::m_id );

	retVal = CVsMayaMPxFactoryBase::DeregisterEverything( pluginFn );

	// Last thing, if Valve Spew is still connected to our output func, reconnect it to the original
	if ( s_savedSpewOutputFunc && GetSpewOutputFunc() == VsSpewOutputFunc )
	{
		SpewOutputFunc( s_savedSpewOutputFunc );
	}

	return retVal;
}
