//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_PreBuildEvent_CommandLine( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spPreBuildEventTool, get_CommandLine, put_CommandLine );
}

bool VPC_Config_PreBuildEvent_Description( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spPreBuildEventTool, get_Description, put_Description );
}

bool VPC_Config_PreBuildEvent_ExcludedFromBuild( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spPreBuildEventTool, get_ExcludedFromBuild, put_ExcludedFromBuild );
}

property_t g_preBuildEventProperties[] =
{
	{"$CommandLine",		VPC_Config_PreBuildEvent_CommandLine},
	{"$Description",		VPC_Config_PreBuildEvent_Description},
	{"$ExcludedFromBuild",	VPC_Config_PreBuildEvent_ExcludedFromBuild},
	{NULL}
};

bool VPC_Config_PreLinkEvent_CommandLine( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spPreLinkEventTool, get_CommandLine, put_CommandLine );
}

bool VPC_Config_PreLinkEvent_Description( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spPreLinkEventTool, get_Description, put_Description );
}

bool VPC_Config_PreLinkEvent_ExcludedFromBuild( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spPreLinkEventTool, get_ExcludedFromBuild, put_ExcludedFromBuild );
}

property_t g_preLinkEventProperties[] =
{
	{"$CommandLine",		VPC_Config_PreLinkEvent_CommandLine},
	{"$Description",		VPC_Config_PreLinkEvent_Description},
	{"$ExcludedFromBuild",	VPC_Config_PreLinkEvent_ExcludedFromBuild},
	{NULL}
};

bool VPC_Config_PostBuildEvent_CommandLine( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spPostBuildEventTool, get_CommandLine, put_CommandLine );
}

bool VPC_Config_PostBuildEvent_Description( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spPostBuildEventTool, get_Description, put_Description );
}

bool VPC_Config_PostBuildEvent_ExcludedFromBuild( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spPostBuildEventTool, get_ExcludedFromBuild, put_ExcludedFromBuild );
}

property_t g_postBuildEventProperties[] =
{
	{"$CommandLine",		VPC_Config_PostBuildEvent_CommandLine},
	{"$Description",		VPC_Config_PostBuildEvent_Description},
	{"$ExcludedFromBuild",	VPC_Config_PostBuildEvent_ExcludedFromBuild},
	{NULL}
};
