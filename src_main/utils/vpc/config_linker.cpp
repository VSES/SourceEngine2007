//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_Linker_OutputFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_OutputFile, put_OutputFile );
}

bool VPC_Config_Linker_ShowProgress( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	linkProgressOption option = linkProgressNotSet;
	if ( !stricmp( buff, "Not Set" ) )
		option = linkProgressNotSet;
	else if ( !stricmp( buff, "Display All Progress Messages (/VERBOSE)" ) )
		option = linkProgressAll;
	else if ( !stricmp( buff, "Displays Some Progress Messages (/VERBOSE:LIB)" ) )
		option = linkProgressLibs;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_ShowProgress, put_ShowProgress, linkProgressOption, option );
}

bool VPC_Config_Linker_Version( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_Version, put_Version );
}

bool VPC_Config_Linker_EnableIncrementalLinking( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	linkIncrementalType option = linkIncrementalDefault;
	if ( !stricmp( buff, "Default" ) )
		option = linkIncrementalDefault;
	else if ( !stricmp( buff, "No (/INCREMENTAL:NO)" ) )
		option = linkIncrementalNo;
	else if ( !stricmp( buff, "Yes (/INCREMENTAL)" ) )
		option = linkIncrementalYes;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_LinkIncremental, put_LinkIncremental, linkIncrementalType, option );
}

bool VPC_Config_Linker_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_Linker_IgnoreImportLibrary( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_IgnoreImportLibrary, put_IgnoreImportLibrary );
}

bool VPC_Config_Linker_RegisterOutput( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_RegisterOutput, put_RegisterOutput );
}

bool VPC_Config_Linker_AdditionalLibraryDirectories( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_AdditionalLibraryDirectories, put_AdditionalLibraryDirectories );
}

bool VPC_Config_Linker_LinkLibraryDependencies( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_LinkLibraryDependencies, put_LinkLibraryDependencies );
}

bool VPC_Config_Linker_UseLibraryDependencyInputs( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_UseLibraryDependencyInputs, put_UseLibraryDependencyInputs );
}

bool VPC_Config_Linker_UseUNICODEResponseFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_UseUnicodeResponseFiles, put_UseUnicodeResponseFiles );
}

bool VPC_Config_Linker_AdditionalDependencies( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_AdditionalDependencies, put_AdditionalDependencies );
}

bool VPC_Config_Linker_IgnoreAllDefaultLibraries( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_IgnoreAllDefaultLibraries, put_IgnoreAllDefaultLibraries );
}

bool VPC_Config_Linker_IgnoreSpecificLibrary( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_IgnoreDefaultLibraryNames, put_IgnoreDefaultLibraryNames );
}

bool VPC_Config_Linker_ModuleDefinitionFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_ModuleDefinitionFile, put_ModuleDefinitionFile );
}

bool VPC_Config_Linker_AddModuleToAssembly( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_AddModuleNamesToAssembly, put_AddModuleNamesToAssembly );
}

bool VPC_Config_Linker_EmbedManagedResourceFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_EmbedManagedResourceFile, put_EmbedManagedResourceFile );
}

bool VPC_Config_Linker_ForceSymbolReferences( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_ForceSymbolReferences, put_ForceSymbolReferences );
}

bool VPC_Config_Linker_DelayLoadedDLLs( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_DelayLoadDLLs, put_DelayLoadDLLs );
}

bool VPC_Config_Linker_AssemblyLinkResource( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_AssemblyLinkResource, put_AssemblyLinkResource );
}

bool VPC_Config_Linker_GenerateManifest( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_GenerateManifest, put_GenerateManifest );
}

bool VPC_Config_Linker_ManifestFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_ManifestFile, put_ManifestFile );
}

bool VPC_Config_Linker_AdditionalManifestDependencies( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_AdditionalManifestDependencies, put_AdditionalManifestDependencies );
}

bool VPC_Config_Linker_AllowIsolation( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_AllowIsolation, put_AllowIsolation );
}

bool VPC_Config_Linker_GenerateDebugInfo( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_GenerateDebugInformation, put_GenerateDebugInformation );
}

bool VPC_Config_Linker_GenerateProgramDatabaseFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_ProgramDatabaseFile, put_ProgramDatabaseFile );
}

bool VPC_Config_Linker_StripPrivateSymbols( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_StripPrivateSymbols, put_StripPrivateSymbols );
}

bool VPC_Config_Linker_GenerateMapFile( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_GenerateMapFile, put_GenerateMapFile );
}

bool VPC_Config_Linker_MapFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_MapFileName, put_MapFileName );
}

bool VPC_Config_Linker_MapExports( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_MapExports, put_MapExports );
}

bool VPC_Config_Linker_DebuggableAssembly( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	linkAssemblyDebug option = linkAssemblyDebugDefault;
	if ( !stricmp( buff, "No Debuggable attribute emitted" ) )
		option = linkAssemblyDebugDefault;
	else if ( !stricmp( buff, "Runtime tracking and disable optimizations (/ASSEMBLYDEBUG)" ) )
		option = linkAssemblyDebugFull;
	else if ( !stricmp( buff, "No runtime tracking and enable optimizations (/ASSEMBLYDEBUG:DISABLE)" ) )
		option = linkAssemblyDebugDisable;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_AssemblyDebug, put_AssemblyDebug, linkAssemblyDebug, option );
}

bool VPC_Config_Linker_SubSystem( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	subSystemOption option = subSystemNotSet;
	if ( !stricmp( buff, "Not Set" ) )
		option = subSystemNotSet;
	else if ( !stricmp( buff, "Console (/SUBSYSTEM:CONSOLE)" ) )
		option = subSystemConsole;
	else if ( !stricmp( buff, "Windows (/SUBSYSTEM:WINDOWS)" ) )
		option = subSystemWindows;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_SubSystem, put_SubSystem, subSystemOption, option );
}

bool VPC_Config_Linker_HeapReserveSize( const char *pPropertyName )
{
	SET_LONG_PROPERTY( pPropertyName, g_spLinker, get_HeapReserveSize, put_HeapReserveSize );
}

bool VPC_Config_Linker_HeapCommitSize( const char *pPropertyName )
{
	SET_LONG_PROPERTY( pPropertyName, g_spLinker, get_HeapCommitSize, put_HeapCommitSize );
}

bool VPC_Config_Linker_StackReserveSize( const char *pPropertyName )
{
	SET_LONG_PROPERTY( pPropertyName, g_spLinker, get_StackReserveSize, put_StackReserveSize );
}

bool VPC_Config_Linker_StackCommitSize( const char *pPropertyName )
{
	SET_LONG_PROPERTY( pPropertyName, g_spLinker, get_StackCommitSize, put_StackCommitSize );
}

bool VPC_Config_Linker_EnableLargeAddresses( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	addressAwarenessType option = addrAwareDefault;
	if ( !stricmp( buff, "Default" ) )
		option = addrAwareDefault;
	else if ( !stricmp( buff, "Do Not Support Addresses Larger Than 2 Gigabytes (/LARGEADDRESSAWARE:NO)" ) )
		option = addrAwareNoLarge;
	else if ( !stricmp( buff, "Support Addresses Larger Than 2 Gigabytes (/LARGEADDRESSAWARE)" ) )
		option = addrAwareLarge;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_LargeAddressAware, put_LargeAddressAware, addressAwarenessType, option );
}

bool VPC_Config_Linker_TerminalServer( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	termSvrAwarenessType option = termSvrAwareDefault;
	if ( !stricmp( buff, "Default" ) )
		option = termSvrAwareDefault;
	else if ( !stricmp( buff, "Not Terminal Server Aware (/TSAWARE:NO)" ) )
		option = termSvrAwareNo;
	else if ( !stricmp( buff, "Application is Terminal Server Aware (/TSAWARE)" ) )
		option = termSvrAwareYes;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_TerminalServerAware, put_TerminalServerAware, termSvrAwarenessType, option );
}

bool VPC_Config_Linker_SwapRunFromCD( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_SwapRunFromCD, put_SwapRunFromCD );
}

bool VPC_Config_Linker_SwapRunFromNetwork( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_SwapRunFromNet, put_SwapRunFromNet );
}

bool VPC_Config_Linker_Driver( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	driverOption option = driverDefault;
	if ( !stricmp( buff, "Not Set" ) )
		option = driverDefault;
	else if ( !stricmp( buff, "Driver (/DRIVER)" ) )
		option = driver;
	else if ( !stricmp( buff, "Up Only (/DRIVER:UPONLY)" ) )
		option = driverUpOnly;
	else if ( !stricmp( buff, "WDM (/DRIVER:WDM)" ) )
		option = driverWDM;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_driver, put_driver, driverOption, option );
}

bool VPC_Config_Linker_References( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	optRefType option = optReferencesDefault;
	if ( !stricmp( buff, "Default" ) )
		option = optReferencesDefault;
	else if ( !stricmp( buff, "Keep Unreferenced Data (/OPT:NOREF)" ) )
		option = optNoReferences;
	else if ( !stricmp( buff, "Eliminate Unreferenced Data (/OPT:REF)" ) )
		option = optReferences;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_OptimizeReferences, put_OptimizeReferences, optRefType, option );
}

bool VPC_Config_Linker_EnableCOMDATFolding( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	optFoldingType option = optFoldingDefault;
	if ( !stricmp( buff, "Default" ) )
		option = optFoldingDefault;
	else if ( !stricmp( buff, "Do Not Remove Redundant COMDATs (/OPT:NOICF)" ) )
		option = optNoFolding;
	else if ( !stricmp( buff, "Remove Redundant COMDATs (/OPT:ICF)" ) )
		option = optFolding;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_EnableCOMDATFolding, put_EnableCOMDATFolding, optFoldingType, option );
}

bool VPC_Config_Linker_OptimizeForWindows98( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	optWin98Type option = optWin98Default;
	if ( !stricmp( buff, "Default" ) )
		option = optWin98Default;
	else if ( !stricmp( buff, "No (/OPT:NOWIN98)" ) )
		option = optWin98No;
	else if ( !stricmp( buff, "Yes (/OPT:WIN98)" ) )
		option = optWin98Yes;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_OptimizeForWindows98, put_OptimizeForWindows98, optWin98Type, option );
}

bool VPC_Config_Linker_FunctionOrder( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_FunctionOrder, put_FunctionOrder );
}

bool VPC_Config_Linker_ProfileGuidedDatabase( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_ProfileGuidedDatabase, put_ProfileGuidedDatabase );
}

bool VPC_Config_Linker_LinkTimeCodeGeneration( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	LinkTimeCodeGenerationOption option = LinkTimeCodeGenerationOptionDefault;
	if ( !stricmp( buff, "Default" ) )
		option = LinkTimeCodeGenerationOptionDefault;
	else if ( !stricmp( buff, "Use Link Time Code Generation (/ltcg)" ) )
		option = LinkTimeCodeGenerationOptionUse;
	else if ( !stricmp( buff, "Profile Guided Optimization - Instrument (/ltcg:pginstrument)" ) )
		option = LinkTimeCodeGenerationOptionInstrument;
	else if ( !stricmp( buff, "Profile Guided Optimization - Optimize (/ltcg:pgoptimize)" ) )
		option = LinkTimeCodeGenerationOptionOptimize;
	else if ( !stricmp( buff, "Profile Guided Optimization - Update (/ltcg:pgupdate)" ) )
		option = LinkTimeCodeGenerationOptionUpdate;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_LinkTimeCodeGeneration, put_LinkTimeCodeGeneration, LinkTimeCodeGenerationOption, option );
}

bool VPC_Config_Linker_MIDLCommands( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_MidlCommandFile, put_MidlCommandFile );
}

bool VPC_Config_Linker_IgnoreEmbeddedIDL( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_IgnoreEmbeddedIDL, put_IgnoreEmbeddedIDL );
}

bool VPC_Config_Linker_MergeIDLBaseFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_MergedIDLBaseFileName, put_MergedIDLBaseFileName );
}

bool VPC_Config_Linker_TypeLibrary( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_TypeLibraryFile, put_TypeLibraryFile );
}

bool VPC_Config_Linker_TypeLibResourceID( const char *pPropertyName )
{
	SET_LONG_PROPERTY( pPropertyName, g_spLinker, get_TypeLibraryResourceID, put_TypeLibraryResourceID );
}

bool VPC_Config_Linker_EntryPoint( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_EntryPointSymbol, put_EntryPointSymbol );
}

bool VPC_Config_Linker_NoEntryPoint( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_ResourceOnlyDLL, put_ResourceOnlyDLL );
}

bool VPC_Config_Linker_SetChecksum( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_SetChecksum, put_SetChecksum );
}

bool VPC_Config_Linker_BaseAddress( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_BaseAddress, put_BaseAddress );
}

bool VPC_Config_Linker_FixedBaseAddress( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	linkFixedBaseAddress option = linkFixedBaseAddressDefault;
	if ( !stricmp( buff, "Default" ) )
		option = linkFixedBaseAddressDefault;
	else if ( !stricmp( buff, "Generate a relocation section (/FIXED:NO)" ) )
		option = linkFixedBaseAddressNo;
	else if ( !stricmp( buff, "Image must be loaded at a fixed address (/FIXED)" ) )
		option = linkFixedBaseAddressYes;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_FixedBaseAddress, put_FixedBaseAddress, linkFixedBaseAddress, option );
}

bool VPC_Config_Linker_TurnOffAssemblyGeneration( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_TurnOffAssemblyGeneration, put_TurnOffAssemblyGeneration );
}

bool VPC_Config_Linker_DelayLoadedDLL( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_DelayLoadDLLs, put_DelayLoadDLLs );
}

bool VPC_Config_Linker_ImportLibrary( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_ImportLibrary, put_ImportLibrary );
}

bool VPC_Config_Linker_MergeSections( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_MergeSections, put_MergeSections );
}

bool VPC_Config_Linker_TargetMachine( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	machineTypeOption option = machineNotSet;
	if ( !stricmp( buff, "Not Set" ) )
		option = machineNotSet;
	else if ( !stricmp( buff, "MachineX86 (/MACHINE:X86)" ) )
		option = machineX86;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_TargetMachine, put_TargetMachine, machineTypeOption, option );
}

bool VPC_Config_Linker_Profile( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_Profile, put_Profile );
}

bool VPC_Config_Linker_CLRThreadAttribute( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	eCLRThreadAttribute option = CLRThreadAttributeNone;
	if ( !stricmp( buff, "No threading attribute set" ) )
		option = CLRThreadAttributeNone;
	else if ( !stricmp( buff, "MTA threading attribute (/CLRTHREADATTRIBUTE:MTA)" ) )
		option = CLRThreadAttributeMTA;
	else if ( !stricmp( buff, "STA threading attribute (/CLRTHREADATTRIBUTE:STA)" ) )
		option = CLRThreadAttributeSTA;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_CLRThreadAttribute, put_CLRThreadAttribute, eCLRThreadAttribute, option );
}

bool VPC_Config_Linker_CLRImageType( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	eCLRImageType option = CLRImageTypeDefault;
	if ( !stricmp( buff, "Default image type" ) )
		option = CLRImageTypeDefault;
	else if ( !stricmp( buff, "Force IJW image (/CLRIMAGETYPE:IJW)" ) )
		option = CLRImageTypeIJW;
	else if ( !stricmp( buff, "Force pure IL image (/CLRIMAGETYPE:PURE)" ) )
		option = CLRImageTypePure;
	else if ( !stricmp( buff, "Force safe IL image (/CLRIMAGETYPE:SAFE)" ) )
		option = CLRImageTypeSafe;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_CLRImageType, put_CLRImageType, eCLRImageType, option );
}

bool VPC_Config_Linker_KeyFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_KeyFile, put_KeyFile );
}

bool VPC_Config_Linker_KeyContainer( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_KeyContainer, put_KeyContainer );
}

bool VPC_Config_Linker_DelaySign( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_DelaySign, put_DelaySign );
}

bool VPC_Config_Linker_ErrorReporting( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	linkerErrorReportingType option = linkerErrorReportingDefault;
	if ( !stricmp( buff, "Default" ) )
		option = linkerErrorReportingDefault;
	else if ( !stricmp( buff, "Prompt Immediately (/ERRORREPORT:PROMPT)" ) )
		option = linkerErrorReportingPrompt;
	else if ( !stricmp( buff, "Queue For Next Login (/ERRORREPORT:QUEUE)" ) )
		option = linkerErrorReportingQueue;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spLinker, get_ErrorReporting, put_ErrorReporting, linkerErrorReportingType, option );
}

bool VPC_Config_Linker_CLRUnmanagedCodeCheck( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spLinker, get_CLRUnmanagedCodeCheck, put_CLRUnmanagedCodeCheck );
}

bool VPC_Config_Linker_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spLinker, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_linkerProperties[] =
{
	// General
	{"$OutputFile",						VPC_Config_Linker_OutputFile},
	{"$ShowProgress",					VPC_Config_Linker_ShowProgress},
	{"$Version",						VPC_Config_Linker_Version},
	{"$EnableIncrementalLinking",		VPC_Config_Linker_EnableIncrementalLinking},
	{"$SuppressStartupBanner",			VPC_Config_Linker_SuppressStartupBanner},
	{"$IgnoreImportLibrary",			VPC_Config_Linker_IgnoreImportLibrary},
	{"$RegisterOutput",					VPC_Config_Linker_RegisterOutput},
	{"$AdditionalLibraryDirectories",	VPC_Config_Linker_AdditionalLibraryDirectories},
	{"$LinkLibraryDependencies",		VPC_Config_Linker_LinkLibraryDependencies},
	{"$UseLibraryDependencyInputs",		VPC_Config_Linker_UseLibraryDependencyInputs},
	{"$UseUNICODEResponseFiles",		VPC_Config_Linker_UseUNICODEResponseFiles},

	// Input
	{"$AdditionalDependencies",			VPC_Config_Linker_AdditionalDependencies},
	{"$IgnoreAllDefaultLibraries",		VPC_Config_Linker_IgnoreAllDefaultLibraries},
	{"$IgnoreSpecificLibrary",			VPC_Config_Linker_IgnoreSpecificLibrary},
	{"$ModuleDefinitionFile",			VPC_Config_Linker_ModuleDefinitionFile},
	{"$AddModuleToAssembly",			VPC_Config_Linker_AddModuleToAssembly},
	{"$EmbedManagedResourceFile",		VPC_Config_Linker_EmbedManagedResourceFile},
	{"$ForceSymbolReferences",			VPC_Config_Linker_ForceSymbolReferences},
	{"$DelayLoadedDLLs",				VPC_Config_Linker_DelayLoadedDLLs},
	{"$AssemblyLinkResource",			VPC_Config_Linker_AssemblyLinkResource},

	// Manifest File
	{"$GenerateManifest",				VPC_Config_Linker_GenerateManifest},			
	{"$ManifestFile",					VPC_Config_Linker_ManifestFile},
	{"$AdditionalManifestDependencies",	VPC_Config_Linker_AdditionalManifestDependencies},
	{"$AllowIsolation",					VPC_Config_Linker_AllowIsolation},

	// Debugging
	{"$GenerateDebugInfo",				VPC_Config_Linker_GenerateDebugInfo},
	{"$GenerateProgramDatabaseFile",	VPC_Config_Linker_GenerateProgramDatabaseFile},
	{"$StripPrivateSymbols",			VPC_Config_Linker_StripPrivateSymbols},
	{"$GenerateMapFile",				VPC_Config_Linker_GenerateMapFile},
	{"$MapFileName",					VPC_Config_Linker_MapFileName},
	{"$MapExports",						VPC_Config_Linker_MapExports},
	{"$DebuggableAssembly",				VPC_Config_Linker_DebuggableAssembly},

	// System
	{"$SubSystem",						VPC_Config_Linker_SubSystem},
	{"$HeapReserveSize",				VPC_Config_Linker_HeapReserveSize},
	{"$HeapCommitSize",					VPC_Config_Linker_HeapCommitSize},
	{"$StackReserveSize",				VPC_Config_Linker_StackReserveSize},
	{"$StackCommitSize",				VPC_Config_Linker_StackCommitSize},
	{"$EnableLargeAddresses",			VPC_Config_Linker_EnableLargeAddresses},
	{"$TerminalServer",					VPC_Config_Linker_TerminalServer},
	{"$SwapRunFromCD",					VPC_Config_Linker_SwapRunFromCD},
	{"$SwapRunFromNetwork",				VPC_Config_Linker_SwapRunFromNetwork},
	{"$Driver",							VPC_Config_Linker_Driver},					

	// Optimization
	{"$References",						VPC_Config_Linker_References},
	{"$EnableCOMDATFolding",			VPC_Config_Linker_EnableCOMDATFolding},
	{"$OptimizeForWindows98",			VPC_Config_Linker_OptimizeForWindows98},
	{"$FunctionOrder",					VPC_Config_Linker_FunctionOrder},
	{"$ProfileGuidedDatabase",			VPC_Config_Linker_ProfileGuidedDatabase},
	{"$LinkTimeCodeGeneration",			VPC_Config_Linker_LinkTimeCodeGeneration},

	// Embedded IDL
	{"$MIDLCommands",					VPC_Config_Linker_MIDLCommands},
	{"$IgnoreEmbeddedIDL",				VPC_Config_Linker_IgnoreEmbeddedIDL},
	{"$MergeIDLBaseFileName",			VPC_Config_Linker_MergeIDLBaseFileName},
	{"$TypeLibrary",					VPC_Config_Linker_TypeLibrary},
	{"$TypeLibResourceID",				VPC_Config_Linker_TypeLibResourceID},

	// Advanced
	{"$EntryPoint",						VPC_Config_Linker_EntryPoint},
	{"$NoEntryPoint",					VPC_Config_Linker_NoEntryPoint},
	{"$SetChecksum",					VPC_Config_Linker_SetChecksum},
	{"$BaseAddress",					VPC_Config_Linker_BaseAddress},
	{"$FixedBaseAddress",				VPC_Config_Linker_FixedBaseAddress},
	{"$TurnOffAssemblyGeneration",		VPC_Config_Linker_TurnOffAssemblyGeneration},
	{"$DelayLoadedDLL",					VPC_Config_Linker_DelayLoadedDLL},
	{"$ImportLibrary",					VPC_Config_Linker_ImportLibrary},
	{"$MergeSections",					VPC_Config_Linker_MergeSections},
	{"$TargetMachine",					VPC_Config_Linker_TargetMachine},
	{"$Profile",						VPC_Config_Linker_Profile},
	{"$CLRThreadAttribute",				VPC_Config_Linker_CLRThreadAttribute},
	{"$CLRImageType",					VPC_Config_Linker_CLRImageType},
	{"$KeyFile",						VPC_Config_Linker_KeyFile},
	{"$KeyContainer",					VPC_Config_Linker_KeyContainer},
	{"$DelaySign",						VPC_Config_Linker_DelaySign},
	{"$ErrorReporting",					VPC_Config_Linker_ErrorReporting},
	{"$CLRUnmanagedCodeCheck",			VPC_Config_Linker_CLRUnmanagedCodeCheck},

	// Command Line
	{"$AdditionalOptions",				VPC_Config_Linker_AdditionalOptions},						
	{NULL}
};