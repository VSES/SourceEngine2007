//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_XboxCompiler_AdditionalIncludeDirectories( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_AdditionalIncludeDirectories, put_AdditionalIncludeDirectories );
}

bool VPC_Config_XboxCompiler_DebugInformationFormat( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::debugOption option = X360::debugDisabled;
	if ( !stricmp( buff, "Disabled" ) )
		option = X360::debugDisabled;
	else if ( !stricmp( buff, "C7 Compatible (/Z7)" ) )
		option = X360::debugOldStyleInfo;
	else if ( !stricmp( buff, "Program Database (/Zi)" ) )
		option = X360::debugEnabled;
	else if ( !stricmp( buff, "Program Database for Edit & Continue (/ZI)" ) )
		option = X360::debugEditAndContinue;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_DebugInformationFormat, put_DebugInformationFormat, X360::debugOption, option );
}

bool VPC_Config_XboxCompiler_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_XboxCompiler_WarningLevel( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::warningLevelOption option = X360::warningLevel_0;
	if ( !stricmp( buff, "Off: Turn Off All Warnings (/W0)" ) )
		option = X360::warningLevel_0;
	else if ( !stricmp( buff, "Level 1 (/W1)" ) )
		option = X360::warningLevel_1;
	else if ( !stricmp( buff, "Level 2 (/W2)" ) )
		option = X360::warningLevel_2;
	else if ( !stricmp( buff, "Level 3 (/W3)" ) )
		option = X360::warningLevel_3;
	else if ( !stricmp( buff, "Level 4 (/W4)" ) )
		option = X360::warningLevel_4;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_WarningLevel, put_WarningLevel, X360::warningLevelOption, option );
}

bool VPC_Config_XboxCompiler_TreatWarningsAsErrors( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_WarnAsError, put_WarnAsError );
}

bool VPC_Config_XboxCompiler_UseUNICODEResponseFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_UseUnicodeResponseFiles, put_UseUnicodeResponseFiles );
}

bool VPC_Config_XboxCompiler_Optimization( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::optimizeOption option = X360::optimizeDisabled;
	if ( !stricmp( buff, "Disabled (/Od)" ) )
		option = X360::optimizeDisabled;
	else if ( !stricmp( buff, "Minimize Size (/O1)" ) )
		option = X360::optimizeMinSpace;
	else if ( !stricmp( buff, "Maximize Speed (/O2)" ) )
		option = X360::optimizeMaxSpeed;
	else if ( !stricmp( buff, "Full Optimization (/Ox)" ) )
		option = X360::optimizeFull;
	else if ( !stricmp( buff, "Custom" ) )
		option = X360::optimizeCustom;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_Optimization, put_Optimization, X360::optimizeOption, option );
}

bool VPC_Config_XboxCompiler_InlineFunctionExpansion( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::inlineExpansionOption option = X360::expandDisable;
	if ( !stricmp( buff, "Default" ) )
		option = X360::expandDisable;
	else if ( !stricmp( buff, "Only __inline (/Ob1)" ) )
		option = X360::expandOnlyInline;
	else if ( !stricmp( buff, "Any Suitable (/Ob2)" ) )
		option = X360::expandAnySuitable;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_InlineFunctionExpansion, put_InlineFunctionExpansion, X360::inlineExpansionOption, option );
}

bool VPC_Config_XboxCompiler_EnableIntrinsicFunctions( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_EnableIntrinsicFunctions, put_EnableIntrinsicFunctions );
}

bool VPC_Config_XboxCompiler_FavorSizeOrSpeed( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::favorSizeOrSpeedOption option = X360::favorNone;
	if ( !stricmp( buff, "Neither" ) )
		option = X360::favorNone;
	else if ( !stricmp( buff, "Favor Fast Code (/Ot)" ) )
		option = X360::favorSpeed;
	else if ( !stricmp( buff, "Favor Small Code (/Os)" ) )
		option = X360::favorSize;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_FavorSizeOrSpeed, put_FavorSizeOrSpeed, X360::favorSizeOrSpeedOption, option );
}

bool VPC_Config_XboxCompiler_EnableFiberSafeOptimizations( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_EnableFiberSafeOptimizations, put_EnableFiberSafeOptimizations );
}

bool VPC_Config_XboxCompiler_WholeProgramOptimization( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_WholeProgramOptimization, put_WholeProgramOptimization );
}

bool VPC_Config_XboxCompiler_PreprocessorDefinitions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_PreprocessorDefinitions, put_PreprocessorDefinitions );
}

bool VPC_Config_XboxCompiler_IgnoreStandardIncludePath( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_IgnoreStandardIncludePath, put_IgnoreStandardIncludePath );
}

bool VPC_Config_XboxCompiler_GeneratePreprocessedFile( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::preprocessOption option = X360::preprocessNo;
	if ( !stricmp( buff, "No" ) )
		option = X360::preprocessNo;
	else if ( !stricmp( buff, "With Line Numbers (/P)" ) )
		option = X360::preprocessYes;
	else if ( !stricmp( buff, "Without Line Numbers (/EP /P)" ) )
		option = X360::preprocessNoLineNumbers;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_GeneratePreprocessedFile, put_GeneratePreprocessedFile, X360::preprocessOption, option );
}

bool VPC_Config_XboxCompiler_KeepComments( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_KeepComments, put_KeepComments );
}

bool VPC_Config_XboxCompiler_EnableStringPooling( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_StringPooling, put_StringPooling );
}

bool VPC_Config_XboxCompiler_EnableMinimalRebuild( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_MinimalRebuild, put_MinimalRebuild );
}

bool VPC_Config_XboxCompiler_EnableCPPExceptions( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::cppExceptionHandling option = X360::cppExceptionHandlingNo;
	if ( !stricmp( buff, "No" ) )
		option = X360::cppExceptionHandlingNo;
	else if ( !stricmp( buff, "Yes (/EHsc)" ) )
		option = X360::cppExceptionHandlingYes;
	else if ( !stricmp( buff, "Yes With SEH Exceptions (/EHa)" ) )
		option = X360::cppExceptionHandlingYesWithSEH;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_ExceptionHandling, put_ExceptionHandling, X360::cppExceptionHandling, option );
}

bool VPC_Config_XboxCompiler_SmallerTypeCheck( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_SmallerTypeCheck, put_SmallerTypeCheck );
}

bool VPC_Config_XboxCompiler_BasicRuntimeChecks( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::basicRuntimeCheckOption option = X360::runtimeBasicCheckNone;
	if ( !stricmp( buff, "Default" ) )
		option = X360::runtimeBasicCheckNone;
	else if ( !stricmp( buff, "Stack Frames (/RTCs)" ) )
		option = X360::runtimeCheckStackFrame;
	else if ( !stricmp( buff, "Uninitialized Variables (/RTCu)" ) )
		option = X360::runtimeCheckUninitVariables;
	else if ( !stricmp( buff, "Both (/RTC1, equiv. to /RTCsu)" ) )
		option = X360::runtimeBasicCheckAll;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_BasicRuntimeChecks, put_BasicRuntimeChecks, X360::basicRuntimeCheckOption, option );
}

bool VPC_Config_XboxCompiler_RuntimeLibrary( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::runtimeLibraryOption option = X360::rtMultiThreaded;
	if ( !stricmp( buff, "Multi-threaded (/MT)" ) )
		option = X360::rtMultiThreaded;
	else if ( !stricmp( buff, "Multi-threaded Debug (/MTd)" ) )
		option = X360::rtMultiThreadedDebug;
	else if ( !stricmp( buff, "Multi-threaded DLL (/MD)" ) )
		option = X360::rtMultiThreadedDLL;
	else if ( !stricmp( buff, "Multi-threaded Debug DLL (/MDd)" ) )
		option = X360::rtMultiThreadedDebugDLL;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_RuntimeLibrary, put_RuntimeLibrary, X360::runtimeLibraryOption, option );
}

bool VPC_Config_XboxCompiler_StructMemberAlignment( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::structMemberAlignOption option = X360::alignNotSet;
	if ( !stricmp( buff, "Default" ) )
		option = X360::alignNotSet;
	else if ( !stricmp( buff, "1 Byte (/Zp1)" ) )
		option = X360::alignSingleByte;
	else if ( !stricmp( buff, "2 Bytes (/Zp2)" ) )
		option = X360::alignTwoBytes;
	else if ( !stricmp( buff, "4 Bytes (/Zp4)" ) )
		option = X360::alignFourBytes;
	else if ( !stricmp( buff, "8 Bytes (/Zp8)" ) )
		option = X360::alignEightBytes;
	else if ( !stricmp( buff, "16 Bytes (/Zp16)" ) )
		option = X360::alignSixteenBytes;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_StructMemberAlignment, put_StructMemberAlignment, X360::structMemberAlignOption, option );
}

bool VPC_Config_XboxCompiler_BufferSecurityCheck( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_BufferSecurityCheck, put_BufferSecurityCheck );
}

bool VPC_Config_XboxCompiler_EnableFunctionLevelLinking( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_EnableFunctionLevelLinking, put_EnableFunctionLevelLinking );
}

bool VPC_Config_XboxCompiler_FloatingPointModel( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::floatingPointModel option = X360::FloatingPointPrecise;
	if ( !stricmp( buff, "Precise (/fp:precise)" ) )
		option = X360::FloatingPointPrecise;
	else if ( !stricmp( buff, "Strict (/fp:strict)" ) )
		option = X360::FloatingPointStrict;
	else if ( !stricmp( buff, "Fast (/fp:fast)" ) )
		option = X360::FloatingPointFast;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_floatingPointModel, put_floatingPointModel, X360::floatingPointModel, option );
}

bool VPC_Config_XboxCompiler_EnableFloatingPointExceptions( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_FloatingPointExceptions, put_FloatingPointExceptions );
}

bool VPC_Config_XboxCompiler_DisableLanguageExtensions( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_DisableLanguageExtensions, put_DisableLanguageExtensions );
}

bool VPC_Config_XboxCompiler_DefaultCharUnsigned( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_DefaultCharIsUnsigned, put_DefaultCharIsUnsigned );
}

bool VPC_Config_XboxCompiler_TreatWCHAR_TAsBuiltInType( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_TreatWChar_tAsBuiltInType, put_TreatWChar_tAsBuiltInType );
}

bool VPC_Config_XboxCompiler_ForceConformanceInForLoopScope( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_ForceConformanceInForLoopScope, put_ForceConformanceInForLoopScope );
}

bool VPC_Config_XboxCompiler_EnableRunTimeTypeInfo( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_RuntimeTypeInfo, put_RuntimeTypeInfo );
}

bool VPC_Config_XboxCompiler_OpenMPSupport( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_OpenMP, put_OpenMP );
}

bool VPC_Config_XboxCompiler_CreateUsePrecompiledHeader( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::pchOption option = X360::pchNone;
	if ( !stricmp( buff, "Not Using Precompiled Headers" ) )
		option = X360::pchNone;
	else if ( !stricmp( buff, "Create Precompiled Header (/Yc)" ) )
		option = X360::pchCreateUsingSpecific;
	else if ( !stricmp( buff, "Use Precompiled Header (/Yu)" ) )
		option = X360::pchUseUsingSpecific;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_UsePrecompiledHeader, put_UsePrecompiledHeader, X360::pchOption, option );
}

bool VPC_Config_XboxCompiler_CreateUsePCHThroughFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_PrecompiledHeaderThrough, put_PrecompiledHeaderThrough );
}

bool VPC_Config_XboxCompiler_PrecompiledHeaderFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_PrecompiledHeaderFile, put_PrecompiledHeaderFile );
}

bool VPC_Config_XboxCompiler_ExpandAttributedSource( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_ExpandAttributedSource, put_ExpandAttributedSource );
}

bool VPC_Config_XboxCompiler_AssemblerOutput( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::asmListingOption option = X360::asmListingNone;
	if ( !stricmp( buff, "No Listing" ) )
		option = X360::asmListingNone;
	else if ( !stricmp( buff, "Assembly-Only Listing (/FA)" ) )
		option = X360::asmListingAssemblyOnly;
	else if ( !stricmp( buff, "Assembly, Machine Code and Source (/FAcs)" ) )
		option = X360::asmListingAsmMachineSrc;
	else if ( !stricmp( buff, "Assembly With Machine Code (/FAc)" ) )
		option = X360::asmListingAsmMachine;
	else if ( !stricmp( buff, "Assembly With Source Code (/FAs)" ) )
		option = X360::asmListingAsmSrc;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_AssemblerOutput, put_AssemblerOutput, X360::asmListingOption, option );
}

bool VPC_Config_XboxCompiler_ASMListLocation( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_AssemblerListingLocation, put_AssemblerListingLocation );
}

bool VPC_Config_XboxCompiler_ObjectFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_ObjectFile, put_ObjectFile );
}

bool VPC_Config_XboxCompiler_ProgramDatabaseFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_ProgramDataBaseFileName, put_ProgramDataBaseFileName );
}

bool VPC_Config_XboxCompiler_EnableBrowseInformation( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::browseInfoOption option = X360::brInfoNone;
	if ( !stricmp( buff, "None" ) )
		option = X360::brInfoNone;
	else if ( !stricmp( buff, "Include All Browse Information (/FR)" ) )
		option = X360::brAllInfo;
	else if ( !stricmp( buff, "No Local Symbols (/Fr)" ) )
		option = X360::brNoLocalSymbols;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_BrowseInformation, put_BrowseInformation, X360::browseInfoOption, option );
}

bool VPC_Config_XboxCompiler_BrowseFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_BrowseInformationFile, put_BrowseInformationFile );
}

bool VPC_Config_XboxCompiler_CompileAs( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::CompileAsOptions option = X360::compileAsDefault;
	if ( !stricmp( buff, "Default" ) )
		option = X360::compileAsDefault;
	else if ( !stricmp( buff, "Compile as C Code (/TC)" ) )
		option = X360::compileAsC;
	else if ( !stricmp( buff, "Compile as C++ Code (/TP)" ) )
		option = X360::compileAsCPlusPlus;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_CompileAs, put_CompileAs, X360::CompileAsOptions, option );
}

bool VPC_Config_XboxCompiler_DisableSpecificWarnings( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_DisableSpecificWarnings, put_DisableSpecificWarnings );
}

bool VPC_Config_XboxCompiler_ForceIncludes( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_ForcedIncludeFiles, put_ForcedIncludeFiles );
}

bool VPC_Config_XboxCompiler_ShowIncludes( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_ShowIncludes, put_ShowIncludes );
}

bool VPC_Config_XboxCompiler_UndefinePreprocessorDefinitions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_UndefinePreprocessorDefinitions, put_UndefinePreprocessorDefinitions );
}

bool VPC_Config_XboxCompiler_UndefineAllPreprocessorDefinitions( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_UndefineAllPreprocessorDefinitions, put_UndefineAllPreprocessorDefinitions );
}

bool VPC_Config_XboxCompiler_UseFullPaths( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_UseFullPaths, put_UseFullPaths );
}

bool VPC_Config_XboxCompiler_OmitDefaultLibraryNames( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_OmitDefaultLibName, put_OmitDefaultLibName );
}

bool VPC_Config_XboxCompiler_RegisterReservation( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_RegisterReservation, put_RegisterReservation );
}

bool VPC_Config_XboxCompiler_TrapIntegerDividesOptimization( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_TrapIntegerDivides, put_TrapIntegerDivides );
}

bool VPC_Config_XboxCompiler_PreschedulingOptimization( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_Prescheduling, put_Prescheduling );
}

bool VPC_Config_XboxCompiler_InlineAssemblyOptimization( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_InlineAssembly, put_InlineAssembly );
}

bool VPC_Config_XboxCompiler_Stalls( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spXboxCompiler, get_Stalls, put_Stalls );
}

bool VPC_Config_XboxCompiler_CallAttributedProfiling( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	X360::enumCallAttributedProfiling option = X360::CallAttributedProfilingDisabled;
	if ( !stricmp( buff, "Disabled" ) )
		option = X360::CallAttributedProfilingDisabled;
	else if ( !stricmp( buff, "Call profiler around function calls. (/fastcap)" ) )
		option = X360::CallAttributedProfilingFastCAP;
	else if ( !stricmp( buff, "Call profiler within function calls. (/callcap)" ) )
		option = X360::CallAttributedProfilingCallCAP;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spXboxCompiler, get_CallAttributedProfiling, put_CallAttributedProfiling, X360::enumCallAttributedProfiling, option );
}

bool VPC_Config_XboxCompiler_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spXboxCompiler, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_xboxCompilerProperties[] =
{
	// General
	{"$AdditionalIncludeDirectories",		VPC_Config_XboxCompiler_AdditionalIncludeDirectories},
	{"$DebugInformationFormat",				VPC_Config_XboxCompiler_DebugInformationFormat},
	{"$SuppressStartupBanner",				VPC_Config_XboxCompiler_SuppressStartupBanner},
	{"$WarningLevel",						VPC_Config_XboxCompiler_WarningLevel},
	{"$TreatWarningsAsErrors",				VPC_Config_XboxCompiler_TreatWarningsAsErrors},
	{"$UseUNICODEResponseFiles",			VPC_Config_XboxCompiler_UseUNICODEResponseFiles},			

	// Optimization
	{"$Optimization",						VPC_Config_XboxCompiler_Optimization},
	{"$InlineFunctionExpansion",			VPC_Config_XboxCompiler_InlineFunctionExpansion},
	{"$EnableIntrinsicFunctions",			VPC_Config_XboxCompiler_EnableIntrinsicFunctions},
	{"$FavorSizeOrSpeed",					VPC_Config_XboxCompiler_FavorSizeOrSpeed},
	{"$EnableFiberSafeOptimizations",		VPC_Config_XboxCompiler_EnableFiberSafeOptimizations},
	{"$WholeProgramOptimization",			VPC_Config_XboxCompiler_WholeProgramOptimization},

	// Preprocessor
	{"$PreprocessorDefinitions",			VPC_Config_XboxCompiler_PreprocessorDefinitions},
	{"$IgnoreStandardIncludePath",			VPC_Config_XboxCompiler_IgnoreStandardIncludePath},
	{"$GeneratePreprocessedFile",			VPC_Config_XboxCompiler_GeneratePreprocessedFile},
	{"$KeepComments",						VPC_Config_XboxCompiler_KeepComments},

	// Code Generation
	{"$EnableStringPooling",				VPC_Config_XboxCompiler_EnableStringPooling},
	{"$EnableMinimalRebuild",				VPC_Config_XboxCompiler_EnableMinimalRebuild},
	{"$EnableC++Exceptions",				VPC_Config_XboxCompiler_EnableCPPExceptions},
	{"$SmallerTypeCheck",					VPC_Config_XboxCompiler_SmallerTypeCheck},
	{"$BasicRuntimeChecks",					VPC_Config_XboxCompiler_BasicRuntimeChecks},
	{"$RuntimeLibrary",						VPC_Config_XboxCompiler_RuntimeLibrary},
	{"$StructMemberAlignment",				VPC_Config_XboxCompiler_StructMemberAlignment},
	{"$BufferSecurityCheck",				VPC_Config_XboxCompiler_BufferSecurityCheck},
	{"$EnableFunctionLevelLinking",			VPC_Config_XboxCompiler_EnableFunctionLevelLinking},
	{"$FloatingPointModel",					VPC_Config_XboxCompiler_FloatingPointModel},
	{"$EnableFloatingPointExceptions",		VPC_Config_XboxCompiler_EnableFloatingPointExceptions},

	// Language
	{"$DisableLanguageExtensions",			VPC_Config_XboxCompiler_DisableLanguageExtensions},
	{"$DefaultCharUnsigned",				VPC_Config_XboxCompiler_DefaultCharUnsigned},
	{"$TreatWCHAR_TAsBuiltInType",			VPC_Config_XboxCompiler_TreatWCHAR_TAsBuiltInType},
	{"$ForceConformanceInForLoopScope",		VPC_Config_XboxCompiler_ForceConformanceInForLoopScope},
	{"$EnableRunTimeTypeInfo",				VPC_Config_XboxCompiler_EnableRunTimeTypeInfo},
	{"$OpenMPSupport",						VPC_Config_XboxCompiler_OpenMPSupport},

	// Precompiled Headers
	{"$Create/UsePrecompiledHeader",		VPC_Config_XboxCompiler_CreateUsePrecompiledHeader},
	{"$Create/UsePCHThroughFile",			VPC_Config_XboxCompiler_CreateUsePCHThroughFile},
	{"$PrecompiledHeaderFile",				VPC_Config_XboxCompiler_PrecompiledHeaderFile},

	// Output Files
	{"$ExpandAttributedSource",				VPC_Config_XboxCompiler_ExpandAttributedSource},
	{"$AssemblerOutput",					VPC_Config_XboxCompiler_AssemblerOutput},
	{"$ASMListLocation",					VPC_Config_XboxCompiler_ASMListLocation},
	{"$ObjectFileName",						VPC_Config_XboxCompiler_ObjectFileName},
	{"$ProgramDatabaseFileName",			VPC_Config_XboxCompiler_ProgramDatabaseFileName},

	// Browse Information
	{"$EnableBrowseInformation",			VPC_Config_XboxCompiler_EnableBrowseInformation},
	{"$BrowseFile",							VPC_Config_XboxCompiler_BrowseFile},

	// Advanced
	{"$CompileAs",							VPC_Config_XboxCompiler_CompileAs},
	{"$DisableSpecificWarnings",			VPC_Config_XboxCompiler_DisableSpecificWarnings},
	{"$ForceIncludes",						VPC_Config_XboxCompiler_ForceIncludes},
	{"$ShowIncludes",						VPC_Config_XboxCompiler_ShowIncludes},
	{"$UndefinePreprocessorDefinitions",	VPC_Config_XboxCompiler_UndefinePreprocessorDefinitions},
	{"$UndefineAllPreprocessorDefinitions",	VPC_Config_XboxCompiler_UndefineAllPreprocessorDefinitions},
	{"$UseFullPaths",						VPC_Config_XboxCompiler_UseFullPaths},
	{"$OmitDefaultLibraryNames",			VPC_Config_XboxCompiler_OmitDefaultLibraryNames},

	// Xbox 360
	{"$RegisterReservation",				VPC_Config_XboxCompiler_RegisterReservation},
	{"$TrapIntegerDividesOptimization",		VPC_Config_XboxCompiler_TrapIntegerDividesOptimization},
	{"$PreschedulingOptimization",			VPC_Config_XboxCompiler_PreschedulingOptimization},
	{"$InlineAssemblyOptimization",			VPC_Config_XboxCompiler_InlineAssemblyOptimization},
	{"$Stalls",								VPC_Config_XboxCompiler_Stalls},
	{"$CallAttributedProfiling",			VPC_Config_XboxCompiler_CallAttributedProfiling},

	// Command Line
	{"$AdditionalOptions",					VPC_Config_XboxCompiler_AdditionalOptions},
	{NULL}
};