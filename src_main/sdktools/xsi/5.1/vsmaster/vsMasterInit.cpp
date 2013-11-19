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
#include "filesystem_init.h"
#include "stringpool.h"
#include "utlstring.h"
#include "utlvector.h"

// xsi headers
#include "xsi_application.h"
#include "xsi_string.h"
#include "xsi_pluginregistrar.h"
#include "xsi_decl.h"
#include "xsi_plugin.h"
#include "xsi_context.h"
#include "xsi_command.h"
#include "xsi_argument.h"

// Local Headers
#include <windows.h>
#include "CVsMayaMasterApp.h"
#include "IMayaVGui.h"
#include "vsmayacommand.h"

// defines
#define PLUGINS_SUBDIR		"bin"
#define PLUGINS_FILESPEC	"vs*.dll"

#ifdef _DEBUG
#define _XSITRACE(x) { XSI::Application ___app;\
						XSI::CString ___l_szMess;\
						___l_szMess.PutAsciiString(x);\
						___app.LogMessage(___l_szMess);}
#else
#define _XSITRACE(x)
#endif

// Static Variables
HINSTANCE	_gValveViewerInstance=0;
static SpewOutputFunc_t s_savedSpewOutputFunc(NULL);
static CVsMayaMasterApp s_vsMayaMasterApp;
static CSteamApplication s_steamApplication( &s_vsMayaMasterApp );
bool g_Initialized = false;

//-----------------------------------------------------------------------------
// Purpose: Valve Spew interface to XSI's output
// Input:	The type of spew message as well as a NULL terminated C string
//			containing the message to spew
// Output:	
//-----------------------------------------------------------------------------

SpewRetval_t VsSpewOutputFunc( SpewType_t type, char const *pMsg )
{	
	SpewRetval_t spewReturnVal( SPEW_CONTINUE );
	XSI::Application app;
	XSI::CString  l_String;
	l_String.PutAsciiString(pMsg);

#ifdef _DEBUG
	OutputDebugString( pMsg );
#endif // __DEBUG

	switch ( type )
	{
	case SPEW_ERROR:
	case SPEW_ASSERT:
		app.LogMessage( l_String, XSI::siErrorMsg );
#ifdef _DEBUG
		spewReturnVal = SPEW_DEBUGGER;
#endif // __DEBUG
		break;
	case SPEW_WARNING:
		app.LogMessage( l_String, XSI::siWarningMsg );
		break;
	default:
		app.LogMessage( l_String );
		break;
	}

	return spewReturnVal;
}

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

	_gValveViewerInstance = (HINSTANCE)hModule;

	return TRUE;
}

//********************************************************************
//
// @mfunc	XSILoadPlugin | Called by XSI to register the DLL and
//							the various plugin it hosts.
//							See the SDK documentation for more info.
//
//
//********************************************************************
XSIPLUGINCALLBACK XSI::CStatus XSILoadPlugin( XSI::PluginRegistrar& in_reg )
{
	in_reg.PutAuthor( L"Valve" );
	in_reg.PutName( L"Valve Integrated Viewer" );
	in_reg.PutVersion( 1, 0 );

	//
	// hack a command for now
	//
	in_reg.RegisterCommand( L"vsMaster" );

	// Hook in normal commands
	CVsMayaCommandFactoryBase::RegisterAllCommands( in_reg );

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
	// Shutdown all systems
	if ( g_Initialized )
	{
		// Now detach all Steamy stuff
		s_steamApplication.Shutdown();
		g_Initialized = false;
	}
	CVsMayaCommandFactoryBase::DeregisterAllCommands( in_reg );

	// Last thing, if Valve Spew is still connected to our output func, reconnect it to the original
	if ( s_savedSpewOutputFunc && GetSpewOutputFunc() == VsSpewOutputFunc )
	{
		SpewOutputFunc( s_savedSpewOutputFunc );
	}

	return XSI::CStatus::OK;
}


//-----------------------------------------------------------------------------
// We do not startup the application until an explicit command is run
// in order to avoid long load times
//-----------------------------------------------------------------------------
XSIPLUGINCALLBACK XSI::CStatus vsMaster_Init( const XSI::CRef& in_context )
{
	if ( !g_Initialized )
	{
		// First thing, connect Valve Spew to VsSpewFunc saving the previous one
		s_savedSpewOutputFunc = GetSpewOutputFunc();

		// Now set the Valve Spew to VsSpewFunc
		SpewOutputFunc( VsSpewOutputFunc );

		// Set up the app instance
		SetAppInstance( (HINSTANCE)GetModuleHandle(NULL) );

		// Tell ApplicationFramework to use VPROJECT to find dlls
		FileSystem_UseVProjectBinDir( true );

		// construct a path to the other plugins we need to load
		char l_szPath[MAX_PATH];
		char l_szDrive[MAX_PATH];
		char l_szDirectory[MAX_PATH];
		GetModuleFileName ( _gValveViewerInstance, l_szPath, MAX_PATH );
		_splitpath ( l_szPath, l_szDrive, l_szDirectory, NULL, NULL );
		char l_szSearchPath[MAX_PATH];
		char l_szFileSpec[MAX_PATH];
		sprintf(l_szSearchPath, "%s%s../../%s", l_szDrive, l_szDirectory, PLUGINS_SUBDIR);
		sprintf(l_szFileSpec, "%s//%s", l_szSearchPath, PLUGINS_FILESPEC);

		__finddata64_t findData;
		intptr_t findHandle;
		if ( ( findHandle = _findfirst64( l_szFileSpec, &findData ) ) != -1 )
		{
			do 
			{
				XSI::CString l_cPluginPath;
				XSI::CString l_cFilename;

				l_cPluginPath.PutAsciiString(l_szSearchPath);
				l_cFilename.PutAsciiString(findData.name);
				l_cPluginPath+= L"/";
				l_cPluginPath+= l_cFilename;

				s_vsMayaMasterApp.LoadPlugin( findData.name, l_szSearchPath );

				//app.LoadPlugin (l_cPluginPath);
			}
			while ( _findnext64( findHandle, &findData ) != -1 );
		}

		_findclose( findHandle );

		// Startup Steam
		s_steamApplication.Startup();
		g_Initialized = true;
	}

	return XSI::CStatus::OK;
}


XSIPLUGINCALLBACK XSI::CStatus vsMaster_Execute( XSI::CRef& in_context )
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
MStatus initializePlugin( MObject mObject )
{
	// First thing, connect Valve Spew to VsSpewFunc saving the previous one
	s_savedSpewOutputFunc = GetSpewOutputFunc();

	// Now set the Valve Spew to VsSpewFunc
	SpewOutputFunc( VsSpewOutputFunc );

	// Set up the app instance
	SetAppInstance( MhInstPlugin );

	// Tell ApplicationFramework to use VPROJECT to find dlls
	FileSystem_UseVProjectBinDir( true );

	// Figure out which other valveSource Maya plug-ins should be loaded
	MFnPlugin pluginFn( mObject, "Valve", "1.0", "Any" );

	// Register Maya Stuff; gotta do it before loading plugins
	MStatus retVal = CVsMayaCommandFactoryBase::RegisterAllCommands( pluginFn );

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

	// Startup Steam
	s_steamApplication.Startup();

	return retVal;
}


//-----------------------------------------------------------------------------
//
// Purpose: Exit point that Maya calls when this plugin is unloaded from Maya
// Input  : The Maya MObject representing the plugin
// Output : MStatus::kSuccess if everything went ok, MStatus::kFailure otherwise
//
//-----------------------------------------------------------------------------
MStatus uninitializePlugin( MObject mObject )
{
	MStatus retVal;

	MFnPlugin pluginFn( mObject );

	// Now detach all Steamy stuff
	s_steamApplication.Shutdown();

	retVal = CVsMayaCommandFactoryBase::DeregisterAllCommands( pluginFn );

	// Last thing, if Valve Spew is still connected to our output func, reconnect it to the original
	if ( s_savedSpewOutputFunc && GetSpewOutputFunc() == VsSpewOutputFunc )
	{
		SpewOutputFunc( s_savedSpewOutputFunc );
	}

	return retVal;
}
*/