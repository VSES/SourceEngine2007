//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_XboxImage_OutputFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_OutputFileName, put_OutputFileName );
}

bool VPC_Config_XboxImage_TitleID( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_TitleID, put_TitleID );
}

bool VPC_Config_XboxImage_LANKey( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_LANKey, put_LANKey );
}

bool VPC_Config_XboxImage_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxImageTool, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_XboxImage_BaseAddress( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_BaseAddress, put_BaseAddress );
}

bool VPC_Config_XboxImage_HeapSize( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_XapiHeapSize, put_XapiHeapSize );
}

bool VPC_Config_XboxImage_WorkspaceSize( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_WorkspaceSize, put_WorkspaceSize );
}

bool VPC_Config_XboxImage_AdditionalSections( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_AdditionalSections, put_AdditionalSections );
}

bool VPC_Config_XboxImage_ExportByName( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxImageTool, get_ExportByName, put_ExportByName );
}

bool VPC_Config_XboxImage_OpticalDiscDriveMapping( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::xexPrivilegeFlag option = X360::xexPrivilegeNotSet;
	if ( !stricmp( buff, "Not Set" ) )
		option = X360::xexPrivilegeNotSet;
	else if ( !stricmp( buff, "Set" ) )
		option = X360::xexPrivilegeSet;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxImageTool, get_PrivilegeNoODDMapping, put_PrivilegeNoODDMapping, X360::xexPrivilegeFlag, option );
}

bool VPC_Config_XboxImage_PAL50Incompatible( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::xexPrivilegeFlag option = X360::xexPrivilegeNotSet;
	if ( !stricmp( buff, "Not Set" ) )
		option = X360::xexPrivilegeNotSet;
	else if ( !stricmp( buff, "Set" ) )
		option = X360::xexPrivilegeSet;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxImageTool, get_PrivilegePAL50Incompatible, put_PrivilegePAL50Incompatible, X360::xexPrivilegeFlag, option );
}

bool VPC_Config_XboxImage_ProjectDefaults( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_ProjectDefaults, put_ProjectDefaults );
}

bool VPC_Config_XboxImage_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxImageTool, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_xboxImageProperties[] =
{
	// General
	{"$OutputFile",					VPC_Config_XboxImage_OutputFile},
	{"$TitleID",					VPC_Config_XboxImage_TitleID},
	{"$LANKey",						VPC_Config_XboxImage_LANKey},
	{"$SuppressStartupBanner",		VPC_Config_XboxImage_SuppressStartupBanner},
	{"$BaseAddress",				VPC_Config_XboxImage_BaseAddress},
	{"$HeapSize",					VPC_Config_XboxImage_HeapSize},
	{"$WorkspaceSize",				VPC_Config_XboxImage_WorkspaceSize},
	{"$AdditionalSections",			VPC_Config_XboxImage_AdditionalSections},
	{"$ExportByName",				VPC_Config_XboxImage_ExportByName},
	
	// Privileges
	{"$OpticalDiscDriveMapping",	VPC_Config_XboxImage_OpticalDiscDriveMapping},
	{"$PAL50Incompatible",			VPC_Config_XboxImage_PAL50Incompatible},

	// Project Defaults
	{"$ProjectDefaults",			VPC_Config_XboxImage_ProjectDefaults},
	{"$AdditionalOptions",			VPC_Config_XboxImage_AdditionalOptions},
	{NULL}
};