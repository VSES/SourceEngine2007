//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_XMLDocGen_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXMLDocGenTool, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_XMLDocGen_ValidateIntelliSense( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXMLDocGenTool, get_ValidateIntelliSense, put_ValidateIntelliSense );
}

bool VPC_Config_XMLDocGen_AdditionalDocumentFiles( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXMLDocGenTool, get_AdditionalDocumentFiles, put_AdditionalDocumentFiles );
}

bool VPC_Config_XMLDocGen_OutputDocumentFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXMLDocGenTool, get_OutputDocumentFile, put_OutputDocumentFile );
}

bool VPC_Config_XMLDocGen_DocumentLibraryDependencies( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXMLDocGenTool, get_DocumentLibraryDependencies, put_DocumentLibraryDependencies );
}

bool VPC_Config_XMLDocGen_UseUNICODEResponseFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXMLDocGenTool, get_UseUnicodeResponseFiles, put_UseUnicodeResponseFiles );
}

bool VPC_Config_XMLDocGen_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXMLDocGenTool, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_xmlDocGenProperties[] =
{
	// General
	{"$SuppressStartupBanner",			VPC_Config_XMLDocGen_SuppressStartupBanner, PLATFORM_WIN32},
	{"$ValidateIntelliSense",			VPC_Config_XMLDocGen_ValidateIntelliSense, PLATFORM_WIN32},
	{"$AdditionalDocumentFiles",		VPC_Config_XMLDocGen_AdditionalDocumentFiles, PLATFORM_WIN32},
	{"$OutputDocumentFile",				VPC_Config_XMLDocGen_OutputDocumentFile, PLATFORM_WIN32},
	{"$DocumentLibraryDependencies",	VPC_Config_XMLDocGen_DocumentLibraryDependencies, PLATFORM_WIN32},
	{"$UseUNICODEResponseFiles",		VPC_Config_XMLDocGen_UseUNICODEResponseFiles, PLATFORM_WIN32},

	// Command Line
	{"$AdditionalOptions",				VPC_Config_XMLDocGen_AdditionalOptions, PLATFORM_WIN32},
	{NULL}
};