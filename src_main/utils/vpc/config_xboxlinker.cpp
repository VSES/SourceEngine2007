//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_XboxLinker_OutputFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_OutputFile, put_OutputFile );
}

bool VPC_Config_XboxLinker_ShowProgress( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::linkProgressOption option = X360::linkProgressNotSet;
	if ( !stricmp( buff, "Not Set" ) )
		option = X360::linkProgressNotSet;
	else if ( !stricmp( buff, "Display All Progress Messages (/VERBOSE)" ) )
		option = X360::linkProgressAll;
	else if ( !stricmp( buff, "Displays Some Progress Messages (/VERBOSE:LIB)" ) )
		option = X360::linkProgressLibs;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxLinker, get_ShowProgress, put_ShowProgress, X360::linkProgressOption, option );
}

bool VPC_Config_XboxLinker_Version( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_Version, put_Version );
}

bool VPC_Config_XboxLinker_EnableIncrementalLinking( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::linkIncrementalType option = X360::linkIncrementalDefault;
	if ( !stricmp( buff, "Default" ) )
		option = X360::linkIncrementalDefault;
	else if ( !stricmp( buff, "No (/INCREMENTAL:NO)" ) )
		option =X360:: linkIncrementalNo;
	else if ( !stricmp( buff, "Yes (/INCREMENTAL)" ) )
		option = X360::linkIncrementalYes;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxLinker, get_LinkIncremental, put_LinkIncremental, X360::linkIncrementalType, option );
}

bool VPC_Config_XboxLinker_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_XboxLinker_IgnoreImportLibrary( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_IgnoreImportLibrary, put_IgnoreImportLibrary );
}

bool VPC_Config_XboxLinker_AdditionalLibraryDirectories( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_AdditionalLibraryDirectories, put_AdditionalLibraryDirectories );
}

bool VPC_Config_XboxLinker_LinkLibraryDependencies( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_LinkLibraryDependencies, put_LinkLibraryDependencies );
}

bool VPC_Config_XboxLinker_UseLibraryDependencyInputs( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_UseLibraryDependencyInputs, put_UseLibraryDependencyInputs );
}

bool VPC_Config_XboxLinker_UseUNICODEResponseFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_UseUnicodeResponseFiles, put_UseUnicodeResponseFiles );
}

bool VPC_Config_XboxLinker_AdditionalDependencies( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_AdditionalDependencies, put_AdditionalDependencies );
}

bool VPC_Config_XboxLinker_IgnoreAllDefaultLibraries( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_IgnoreAllDefaultLibraries, put_IgnoreAllDefaultLibraries );
}

bool VPC_Config_XboxLinker_IgnoreSpecificLibrary( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_IgnoreDefaultLibraryNames, put_IgnoreDefaultLibraryNames );
}

bool VPC_Config_XboxLinker_ModuleDefinitionFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_ModuleDefinitionFile, put_ModuleDefinitionFile );
}

bool VPC_Config_XboxLinker_ForceSymbolReferences( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_ForceSymbolReferences, put_ForceSymbolReferences );
}

bool VPC_Config_XboxLinker_GenerateDebugInfo( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_GenerateDebugInformation, put_GenerateDebugInformation );
}

bool VPC_Config_XboxLinker_GenerateProgramDatabaseFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_ProgramDatabaseFile, put_ProgramDatabaseFile );
}

bool VPC_Config_XboxLinker_StripPrivateSymbols( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_StripPrivateSymbols, put_StripPrivateSymbols );
}

bool VPC_Config_XboxLinker_GenerateMapFile( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_GenerateMapFile, put_GenerateMapFile );
}

bool VPC_Config_XboxLinker_MapFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_MapFileName, put_MapFileName );
}

bool VPC_Config_XboxLinker_MapExports( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_MapExports, put_MapExports );
}

bool VPC_Config_XboxLinker_StackCommitSize( const char *pPropertyName )
{
	SET_LONG_PROPERTY( pPropertyName, g_spXboxLinker, get_StackCommitSize, put_StackCommitSize );
}

bool VPC_Config_XboxLinker_References( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::optRefType option = X360::optReferencesDefault;
	if ( !stricmp( buff, "Default" ) )
		option = X360::optReferencesDefault;
	else if ( !stricmp( buff, "Keep Unreferenced Data (/OPT:NOREF)" ) )
		option = X360::optNoReferences;
	else if ( !stricmp( buff, "Eliminate Unreferenced Data (/OPT:REF)" ) )
		option = X360::optReferences;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxLinker, get_OptimizeReferences, put_OptimizeReferences, X360::optRefType, option );
}

bool VPC_Config_XboxLinker_EnableCOMDATFolding( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::optFoldingType option = X360::optFoldingDefault;
	if ( !stricmp( buff, "Default" ) )
		option = X360::optFoldingDefault;
	else if ( !stricmp( buff, "Do Not Remove Redundant COMDATs (/OPT:NOICF)" ) )
		option = X360::optNoFolding;
	else if ( !stricmp( buff, "Remove Redundant COMDATs (/OPT:ICF)" ) )
		option = X360::optFolding;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxLinker, get_EnableCOMDATFolding, put_EnableCOMDATFolding, X360::optFoldingType, option );
}

bool VPC_Config_XboxLinker_FunctionOrder( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_FunctionOrder, put_FunctionOrder );
}

bool VPC_Config_XboxLinker_ProfileGuidedDatabase( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_ProfileGuidedDatabase, put_ProfileGuidedDatabase );
}

bool VPC_Config_XboxLinker_LinkTimeCodeGeneration( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::LinkTimeCodeGenerationOption option = X360::LinkTimeCodeGenerationOptionDefault;
	if ( !stricmp( buff, "Default" ) )
		option = X360::LinkTimeCodeGenerationOptionDefault;
	else if ( !stricmp( buff, "Use Link Time Code Generation (/ltcg)" ) )
		option = X360::LinkTimeCodeGenerationOptionUse;
	else if ( !stricmp( buff, "Profile Guided Optimization - Instrument (/ltcg:pginstrument)" ) )
		option = X360::LinkTimeCodeGenerationOptionInstrument;
	else if ( !stricmp( buff, "Profile Guided Optimization - Optimize (/ltcg:pgoptimize)" ) )
		option = X360::LinkTimeCodeGenerationOptionOptimize;
	else if ( !stricmp( buff, "Profile Guided Optimization - Update (/ltcg:pgupdate)" ) )
		option = X360::LinkTimeCodeGenerationOptionUpdate;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxLinker, get_LinkTimeCodeGeneration, put_LinkTimeCodeGeneration, X360::LinkTimeCodeGenerationOption, option );
}

bool VPC_Config_XboxLinker_EntryPoint( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_EntryPointSymbol, put_EntryPointSymbol );
}

bool VPC_Config_XboxLinker_NoEntryPoint( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_ResourceOnlyDLL, put_ResourceOnlyDLL );
}

bool VPC_Config_XboxLinker_SetChecksum( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxLinker, get_SetChecksum, put_SetChecksum );
}

bool VPC_Config_XboxLinker_BaseAddress( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_BaseAddress, put_BaseAddress );
}

bool VPC_Config_XboxLinker_FixedBaseAddress( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::linkFixedBaseAddress option = X360::linkFixedBaseAddressDefault;
	if ( !stricmp( buff, "Default" ) )
		option = X360::linkFixedBaseAddressDefault;
	else if ( !stricmp( buff, "Generate a relocation section (/FIXED:NO)" ) )
		option = X360::linkFixedBaseAddressNo;
	else if ( !stricmp( buff, "Image must be loaded at a fixed address (/FIXED)" ) )
		option = X360::linkFixedBaseAddressYes;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxLinker, get_FixedBaseAddress, put_FixedBaseAddress, X360::linkFixedBaseAddress, option );
}

bool VPC_Config_XboxLinker_ImportLibrary( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_ImportLibrary, put_ImportLibrary );
}

bool VPC_Config_XboxLinker_MergeSections( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_MergeSections, put_MergeSections );
}

bool VPC_Config_XboxLinker_ErrorReporting( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::linkerErrorReportingType option = X360::linkerErrorReportingDefault;
	if ( !stricmp( buff, "Default" ) )
		option = X360::linkerErrorReportingDefault;
	else if ( !stricmp( buff, "Prompt Immediately (/ERRORREPORT:PROMPT)" ) )
		option = X360::linkerErrorReportingPrompt;
	else if ( !stricmp( buff, "Queue For Next Login (/ERRORREPORT:QUEUE)" ) )
		option = X360::linkerErrorReportingQueue;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxLinker, get_ErrorReporting, put_ErrorReporting, X360::linkerErrorReportingType, option );
}

bool VPC_Config_XboxLinker_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxLinker, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_xboxLinkerProperties[] =
{
	// General
	{"$OutputFile",						VPC_Config_XboxLinker_OutputFile},
	{"$ShowProgress",					VPC_Config_XboxLinker_ShowProgress},
	{"$Version",						VPC_Config_XboxLinker_Version},
	{"$EnableIncrementalLinking",		VPC_Config_XboxLinker_EnableIncrementalLinking},
	{"$SuppressStartupBanner",			VPC_Config_XboxLinker_SuppressStartupBanner},
	{"$IgnoreImportLibrary",			VPC_Config_XboxLinker_IgnoreImportLibrary},
	{"$AdditionalLibraryDirectories",	VPC_Config_XboxLinker_AdditionalLibraryDirectories},
	{"$LinkLibraryDependencies",		VPC_Config_XboxLinker_LinkLibraryDependencies},
	{"$UseLibraryDependencyInputs",		VPC_Config_XboxLinker_UseLibraryDependencyInputs},
	{"$UseUNICODEResponseFiles",		VPC_Config_XboxLinker_UseUNICODEResponseFiles},

	// Input
	{"$AdditionalDependencies",			VPC_Config_XboxLinker_AdditionalDependencies},
	{"$IgnoreAllDefaultLibraries",		VPC_Config_XboxLinker_IgnoreAllDefaultLibraries},
	{"$IgnoreSpecificLibrary",			VPC_Config_XboxLinker_IgnoreSpecificLibrary},
	{"$ModuleDefinitionFile",			VPC_Config_XboxLinker_ModuleDefinitionFile},
	{"$ForceSymbolReferences",			VPC_Config_XboxLinker_ForceSymbolReferences},

	// Debugging
	{"$GenerateDebugInfo",				VPC_Config_XboxLinker_GenerateDebugInfo},
	{"$GenerateProgramDatabaseFile",	VPC_Config_XboxLinker_GenerateProgramDatabaseFile},
	{"$StripPrivateSymbols",			VPC_Config_XboxLinker_StripPrivateSymbols},
	{"$GenerateMapFile",				VPC_Config_XboxLinker_GenerateMapFile},
	{"$MapFileName",					VPC_Config_XboxLinker_MapFileName},
	{"$MapExports",						VPC_Config_XboxLinker_MapExports},

	// System
	{"$StackCommitSize",				VPC_Config_XboxLinker_StackCommitSize},

	// Optimization
	{"$References",						VPC_Config_XboxLinker_References},
	{"$EnableCOMDATFolding",			VPC_Config_XboxLinker_EnableCOMDATFolding},
	{"$FunctionOrder",					VPC_Config_XboxLinker_FunctionOrder},
	{"$ProfileGuidedDatabase",			VPC_Config_XboxLinker_ProfileGuidedDatabase},
	{"$LinkTimeCodeGeneration",			VPC_Config_XboxLinker_LinkTimeCodeGeneration},

	// Advanced
	{"$EntryPoint",						VPC_Config_XboxLinker_EntryPoint},
	{"$NoEntryPoint",					VPC_Config_XboxLinker_NoEntryPoint},
	{"$SetChecksum",					VPC_Config_XboxLinker_SetChecksum},
	{"$BaseAddress",					VPC_Config_XboxLinker_BaseAddress},
	{"$FixedBaseAddress",				VPC_Config_XboxLinker_FixedBaseAddress},
	{"$ImportLibrary",					VPC_Config_XboxLinker_ImportLibrary},
	{"$MergeSections",					VPC_Config_XboxLinker_MergeSections},
	{"$ErrorReporting",					VPC_Config_XboxLinker_ErrorReporting},

	// Command Line
	{"$AdditionalOptions",				VPC_Config_XboxLinker_AdditionalOptions},						
	{NULL}
};