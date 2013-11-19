//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_BrowseInfo_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spBrowseInfoTool, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_BrowseInfo_OutputFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spBrowseInfoTool, get_OutputFile, put_OutputFile );
}

bool VPC_Config_BrowseInfo_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spBrowseInfoTool, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_browseInfoProperties[] =
{
	// General
	{"$SuppressStartupBanner",		VPC_Config_BrowseInfo_SuppressStartupBanner},
	{"$OutputFile",					VPC_Config_BrowseInfo_OutputFile},
	{"$AdditionalOptions",			VPC_Config_BrowseInfo_AdditionalOptions},
	{NULL}
};