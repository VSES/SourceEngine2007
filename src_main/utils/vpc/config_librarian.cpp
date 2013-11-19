//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_Librarian_OutputFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLibrarian, get_OutputFile, put_OutputFile );
}

bool VPC_Config_Librarian_AdditionalDependencies( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLibrarian, get_AdditionalDependencies, put_AdditionalDependencies );
}

bool VPC_Config_Librarian_AdditionalLibraryDirectories( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLibrarian, get_AdditionalLibraryDirectories, put_AdditionalLibraryDirectories );
}

bool VPC_Config_Librarian_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLibrarian, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_Librarian_ModuleDefinitionFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLibrarian, get_ModuleDefinitionFile, put_ModuleDefinitionFile );
}

bool VPC_Config_Librarian_IgnoreAllDefaultLibraries( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLibrarian, get_IgnoreAllDefaultLibraries, put_IgnoreAllDefaultLibraries );
}

bool VPC_Config_Librarian_IgnoreSpecificLibrary( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLibrarian, get_IgnoreDefaultLibraryNames, put_IgnoreDefaultLibraryNames );
}

bool VPC_Config_Librarian_ExportNamedFunctions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLibrarian, get_ExportNamedFunctions, put_ExportNamedFunctions );
}

bool VPC_Config_Librarian_ForceSymbolReferences( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLibrarian, get_ForceSymbolReferences, put_ForceSymbolReferences );
}

bool VPC_Config_Librarian_UseUNICODEResponseFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLibrarian, get_UseUnicodeResponseFiles, put_UseUnicodeResponseFiles );
}

bool VPC_Config_Librarian_LinkLibraryDependencies( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLibrarian, get_LinkLibraryDependencies, put_LinkLibraryDependencies );
}

bool VPC_Config_Librarian_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLibrarian, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_librarianProperties[] =
{
	// General
	{"$OutputFile",						VPC_Config_Librarian_OutputFile},
	{"$AdditionalDependencies",			VPC_Config_Librarian_AdditionalDependencies},
	{"$AdditionalLibraryDirectories",	VPC_Config_Librarian_AdditionalLibraryDirectories},
	{"$SuppressStartupBanner",			VPC_Config_Librarian_SuppressStartupBanner},
	{"$ModuleDefinitionFileName",		VPC_Config_Librarian_ModuleDefinitionFileName},
	{"$IgnoreAllDefaultLibraries",		VPC_Config_Librarian_IgnoreAllDefaultLibraries},
	{"$IgnoreSpecificLibrary",			VPC_Config_Librarian_IgnoreSpecificLibrary},
	{"$ExportNamedFunctions",			VPC_Config_Librarian_ExportNamedFunctions},
	{"$ForceSymbolReferences",			VPC_Config_Librarian_ForceSymbolReferences},
	{"$UseUNICODEResponseFiles",		VPC_Config_Librarian_UseUNICODEResponseFiles},
	{"$LinkLibraryDependencies",		VPC_Config_Librarian_LinkLibraryDependencies},

	// Command Line
	{"$AdditionalOptions",				VPC_Config_Librarian_AdditionalOptions},						
	{NULL}
};