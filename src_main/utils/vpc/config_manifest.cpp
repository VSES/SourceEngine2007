//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_Manifest_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spManifestTool, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_Manifest_VerboseOutput( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spManifestTool, get_VerboseOutput, put_VerboseOutput );
}

bool VPC_Config_Manifest_AssemblyIdentity( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_AssemblyIdentity, put_AssemblyIdentity );
}

bool VPC_Config_Manifest_UseUNICODEResponseFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spManifestTool, get_UseUnicodeResponseFiles, put_UseUnicodeResponseFiles );
}

bool VPC_Config_Manifest_UseFAT32WorkAround( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spManifestTool, get_UseFAT32Workaround, put_UseFAT32Workaround  );
}

bool VPC_Config_Manifest_AdditionalManifestFiles( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_AdditionalManifestFiles, put_AdditionalManifestFiles );
}

bool VPC_Config_Manifest_InputResourceManifests( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_InputResourceManifests, put_InputResourceManifests );
}

bool VPC_Config_Manifest_EmbedManifest( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spManifestTool, get_EmbedManifest, put_EmbedManifest );
}

bool VPC_Config_Manifest_OutputManifestFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_OutputManifestFile, put_OutputManifestFile );
}

bool VPC_Config_Manifest_ManifestResourceFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_ManifestResourceFile, put_ManifestResourceFile );
}

bool VPC_Config_Manifest_GenerateCatalogFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spManifestTool, get_GenerateCatalogFiles, put_GenerateCatalogFiles );
}

bool VPC_Config_Manifest_DependencyInformationFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_DependencyInformationFile, put_DependencyInformationFile );
}

bool VPC_Config_Manifest_TypeLibraryFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_TypeLibraryFile, put_TypeLibraryFile );
}

bool VPC_Config_Manifest_RegistrarScriptFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_RegistrarScriptFile, put_RegistrarScriptFile );
}

bool VPC_Config_Manifest_ComponentFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_ComponentFileName, put_ComponentFileName );
}

bool VPC_Config_Manifest_ReplacementsFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_ReplacementsFile, put_ReplacementsFile );
}

bool VPC_Config_Manifest_UpdateFileHashes( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spManifestTool, get_UpdateFileHashes, put_UpdateFileHashes );
}

bool VPC_Config_Manifest_UpdateFileHashesSearchPath( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_UpdateFileHashesSearchPath, put_UpdateFileHashesSearchPath );
}

bool VPC_Config_Manifest_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spManifestTool, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_manifestProperties[] =
{
	// General
	{"$SuppressStartupBanner",			VPC_Config_Manifest_SuppressStartupBanner, PLATFORM_WIN32},
	{"$VerboseOutput",					VPC_Config_Manifest_VerboseOutput, PLATFORM_WIN32},
	{"$AssemblyIdentity",				VPC_Config_Manifest_AssemblyIdentity, PLATFORM_WIN32},
	{"$UseUNICODEResponseFiles",		VPC_Config_Manifest_UseUNICODEResponseFiles, PLATFORM_WIN32},
	{"$UseFAT32WorkAround",				VPC_Config_Manifest_UseFAT32WorkAround, PLATFORM_WIN32},

	// Input And Output
	{"$AdditionalManifestFiles",		VPC_Config_Manifest_AdditionalManifestFiles, PLATFORM_WIN32},
	{"$InputResourceManifests",			VPC_Config_Manifest_InputResourceManifests, PLATFORM_WIN32},
	{"$EmbedManifest",					VPC_Config_Manifest_EmbedManifest, PLATFORM_WIN32},
	{"$OutputManifestFile",				VPC_Config_Manifest_OutputManifestFile, PLATFORM_WIN32},
	{"$ManifestResourceFile",			VPC_Config_Manifest_ManifestResourceFile, PLATFORM_WIN32},
	{"$GenerateCatalogFiles",			VPC_Config_Manifest_GenerateCatalogFiles, PLATFORM_WIN32},
	{"$DependencyInformationFile",		VPC_Config_Manifest_DependencyInformationFile, PLATFORM_WIN32},

	// Isolated COM
	{"$TypeLibraryFile",				VPC_Config_Manifest_TypeLibraryFile, PLATFORM_WIN32},
	{"$RegistrarScriptFile",			VPC_Config_Manifest_RegistrarScriptFile, PLATFORM_WIN32},
	{"$ComponentFileName",				VPC_Config_Manifest_ComponentFileName, PLATFORM_WIN32},
	{"$ReplacementsFile",				VPC_Config_Manifest_ReplacementsFile, PLATFORM_WIN32},

	// Advanced
	{"$UpdateFileHashes",				VPC_Config_Manifest_UpdateFileHashes, PLATFORM_WIN32},
	{"$UpdateFileHashesSearchPath",		VPC_Config_Manifest_UpdateFileHashesSearchPath, PLATFORM_WIN32},

	// Command Line
	{"$AdditionalOptions",				VPC_Config_Manifest_AdditionalOptions, PLATFORM_WIN32},				
	{NULL}
};