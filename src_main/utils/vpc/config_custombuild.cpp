//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_CustomBuild_CommandLine( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCustomBuildTool, get_CommandLine, put_CommandLine );
}

bool VPC_Config_CustomBuild_Description( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCustomBuildTool, get_Description, put_Description );
}

bool VPC_Config_CustomBuild_Outputs( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCustomBuildTool, get_Outputs, put_Outputs );
}

bool VPC_Config_CustomBuild_AdditionalDependencies( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCustomBuildTool, get_AdditionalDependencies, put_AdditionalDependencies );
}

property_t g_customBuildProperties[] =
{
	{"$CommandLine",			VPC_Config_CustomBuild_CommandLine},
	{"$Description",			VPC_Config_CustomBuild_Description},
	{"$Outputs",				VPC_Config_CustomBuild_Outputs},
	{"$AdditionalDependencies",	VPC_Config_CustomBuild_AdditionalDependencies},
	{NULL}
};
