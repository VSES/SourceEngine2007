//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_XboxDeployment_DeploymentType( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::enumXboxDeploymentType option = X360::DeploymentTypeCopy;
	if ( !stricmp( buff, "Copy to Hard Drive" ) )
		option = X360::DeploymentTypeCopy;
	else if ( !stricmp( buff, "Emulate DVD Drive" ) )
		option = X360::DeploymentTypeEmulate;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_DeploymentType, put_DeploymentType, X360::enumXboxDeploymentType, option );
}

bool VPC_Config_XboxDeployment_DeploymentFiles( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_DeploymentFiles, put_DeploymentFiles );
}

bool VPC_Config_XboxDeployment_ExcludedFromBuild( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_ExcludedFromBuild, put_ExcludedFromBuild );
}

bool VPC_Config_XboxDeployment_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_XboxDeployment_Progress( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_Progress, put_Progress );
}

bool VPC_Config_XboxDeployment_ForceCopy( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_ForceCopy, put_ForceCopy );
}

bool VPC_Config_XboxDeployment_DeploymentRoot( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_RemoteRoot, put_RemoteRoot );
}

bool VPC_Config_XboxDeployment_EmulationType( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::enumXboxEmulationType option = X360::EmulationTypeTurbo;
	if ( !stricmp( buff, "Zero Seek Times" ) )
		option = X360::EmulationTypeTurbo;
	else if ( !stricmp( buff, "Accurate Seek Times" ) )
		option = X360::EmulationTypeAccurate;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_EmulationType, put_EmulationType, X360::enumXboxEmulationType, option );
}

bool VPC_Config_XboxDeployment_Layout( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_EmulationLayout, put_EmulationLayout );
}

bool VPC_Config_XboxDeployment_LayoutRoot( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_LayoutRoot, put_LayoutRoot );
}

bool VPC_Config_XboxDeployment_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxDeploymentTool, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_xboxDeploymentProperties[] =
{
	// Common Settings
	{"$DeploymentType",			VPC_Config_XboxDeployment_DeploymentType},
	{"$DeploymentFiles",		VPC_Config_XboxDeployment_DeploymentFiles},

	// Copy To Hard Drive
	{"$ExcludedFromBuild",		VPC_Config_XboxDeployment_ExcludedFromBuild},
	{"$SuppressStartupBanner",	VPC_Config_XboxDeployment_SuppressStartupBanner},
	{"$Progress",				VPC_Config_XboxDeployment_Progress},
	{"$ForceCopy",				VPC_Config_XboxDeployment_ForceCopy},
	{"$DeploymentRoot",			VPC_Config_XboxDeployment_DeploymentRoot},

	// Emulate DVD Drive
	{"$EmulationType",			VPC_Config_XboxDeployment_EmulationType},
	{"$Layout",					VPC_Config_XboxDeployment_Layout},
	{"$LayoutRoot",				VPC_Config_XboxDeployment_LayoutRoot},
	{"$AdditionalOptions",		VPC_Config_XboxDeployment_AdditionalOptions},
	{NULL}
};