//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_Debugging_Command( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spDebugSettings, get_Command, put_Command );
}

bool VPC_Config_Debugging_CommandArguments( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spDebugSettings, get_CommandArguments, put_CommandArguments );
}

bool VPC_Config_Debugging_WorkingDirectory( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spDebugSettings, get_WorkingDirectory, put_WorkingDirectory );
}

bool VPC_Config_Debugging_Attach( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spDebugSettings, get_Attach, put_Attach );
}

bool VPC_Config_Debugging_MapDVDDrive( const char *pPropertyName )
{
	VPC_Error( "Setting '%s' Not Implemented", pPropertyName );
	return false;
}

bool VPC_Config_Debugging_DebuggerType( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	TypeOfDebugger option = DbgNativeOnly;
	if ( !stricmp( buff, "Native Only" ) )
		option = DbgNativeOnly;
	else if ( !stricmp( buff, "Managed Only" ) )
		option = DbgManagedOnly;
	else if ( !stricmp( buff, "Mixed Only" ) )
		option = DbgMixed;
	else if ( !stricmp( buff, "Auto Only" ) )
		option = DbgAuto;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spDebugSettings, get_DebuggerType, put_DebuggerType, TypeOfDebugger, option );
}

bool VPC_Config_Debugging_SQLDebugging( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spDebugSettings, get_SQLDebugging, put_SQLDebugging );
}

bool VPC_Config_Debugging_RemoteMachine( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spDebugSettings, get_RemoteMachine, put_RemoteMachine );
}

bool VPC_Config_Debugging_Environment( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spDebugSettings, get_Environment, put_Environment );
}

bool VPC_Config_Debugging_MergeEnvironment( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spDebugSettings, get_EnvironmentMerge, put_EnvironmentMerge );
}

property_t g_debuggingProperties[] =
{
	{"$Command",					VPC_Config_Debugging_Command},
	{"$CommandArguments",			VPC_Config_Debugging_CommandArguments},
	{"$WorkingDirectory",			VPC_Config_Debugging_WorkingDirectory, PLATFORM_WIN32},
	{"$Attach",						VPC_Config_Debugging_Attach, PLATFORM_WIN32},
	{"$DebuggerType",				VPC_Config_Debugging_DebuggerType, PLATFORM_WIN32},
	{"$Environment",				VPC_Config_Debugging_Environment, PLATFORM_WIN32},
	{"$MergeEnvironment",			VPC_Config_Debugging_MergeEnvironment, PLATFORM_WIN32},
	{"$SQLDebugging",				VPC_Config_Debugging_SQLDebugging, PLATFORM_WIN32},
	{"$MapDVDDrive",				VPC_Config_Debugging_MapDVDDrive, PLATFORM_X360},
	{"$RemoteMachine",				VPC_Config_Debugging_RemoteMachine, PLATFORM_X360},
	{NULL}
};
