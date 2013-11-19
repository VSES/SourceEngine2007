//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

bool VPC_Config_Compiler_AdditionalIncludeDirectories( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_AdditionalIncludeDirectories, put_AdditionalIncludeDirectories );
}

bool VPC_Config_Compiler_ResolveUsingReferences( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_AdditionalUsingDirectories, put_AdditionalUsingDirectories );
}

bool VPC_Config_Compiler_DebugInformationFormat( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	debugOption option = debugDisabled;
	if ( !stricmp( buff, "Disabled" ) )
		option = debugDisabled;
	else if ( !stricmp( buff, "C7 Compatible (/Z7)" ) )
		option = debugOldStyleInfo;
	else if ( !stricmp( buff, "Program Database (/Zi)" ) )
		option = debugEnabled;
	else if ( !stricmp( buff, "Program Database for Edit & Continue (/ZI)" ) )
		option = debugEditAndContinue;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_DebugInformationFormat, put_DebugInformationFormat, debugOption, option );
}

bool VPC_Config_Compiler_SuppressStartupBanner( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_SuppressStartupBanner, put_SuppressStartupBanner );
}

bool VPC_Config_Compiler_WarningLevel( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	warningLevelOption option = warningLevel_0;
	if ( !stricmp( buff, "Off: Turn Off All Warnings (/W0)" ) )
		option = warningLevel_0;
	else if ( !stricmp( buff, "Level 1 (/W1)" ) )
		option = warningLevel_1;
	else if ( !stricmp( buff, "Level 2 (/W2)" ) )
		option = warningLevel_2;
	else if ( !stricmp( buff, "Level 3 (/W3)" ) )
		option = warningLevel_3;
	else if ( !stricmp( buff, "Level 4 (/W4)" ) )
		option = warningLevel_4;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_WarningLevel, put_WarningLevel, warningLevelOption, option );
}

bool VPC_Config_Compiler_Detect64bitPortabilityIssues( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_Detect64BitPortabilityProblems, put_Detect64BitPortabilityProblems );
}

bool VPC_Config_Compiler_TreatWarningsAsErrors( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_WarnAsError, put_WarnAsError );
}

bool VPC_Config_Compiler_UseUNICODEResponseFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_UseUnicodeResponseFiles, put_UseUnicodeResponseFiles );
}

bool VPC_Config_Compiler_Optimization( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	optimizeOption option = optimizeDisabled;
	if ( !stricmp( buff, "Disabled (/Od)" ) )
		option = optimizeDisabled;
	else if ( !stricmp( buff, "Minimize Size (/O1)" ) )
		option = optimizeMinSpace;
	else if ( !stricmp( buff, "Maximize Speed (/O2)" ) )
		option = optimizeMaxSpeed;
	else if ( !stricmp( buff, "Full Optimization (/Ox)" ) )
		option = optimizeFull;
	else if ( !stricmp( buff, "Custom" ) )
		option = optimizeCustom;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_Optimization, put_Optimization, optimizeOption, option );
}

bool VPC_Config_Compiler_InlineFunctionExpansion( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	inlineExpansionOption option = expandDisable;
	if ( !stricmp( buff, "Default" ) )
		option = expandDisable;
	else if ( !stricmp( buff, "Only __inline (/Ob1)" ) )
		option = expandOnlyInline;
	else if ( !stricmp( buff, "Any Suitable (/Ob2)" ) )
		option = expandAnySuitable;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_InlineFunctionExpansion, put_InlineFunctionExpansion, inlineExpansionOption, option );
}

bool VPC_Config_Compiler_EnableIntrinsicFunctions( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_EnableIntrinsicFunctions, put_EnableIntrinsicFunctions );
}

bool VPC_Config_Compiler_FavorSizeOrSpeed( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	favorSizeOrSpeedOption option = favorNone;
	if ( !stricmp( buff, "Neither" ) )
		option = favorNone;
	else if ( !stricmp( buff, "Favor Fast Code (/Ot)" ) )
		option = favorSpeed;
	else if ( !stricmp( buff, "Favor Small Code (/Os)" ) )
		option = favorSize;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_FavorSizeOrSpeed, put_FavorSizeOrSpeed, favorSizeOrSpeedOption, option );
}

bool VPC_Config_Compiler_OmitFramePointers( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_OmitFramePointers, put_OmitFramePointers );
}

bool VPC_Config_Compiler_EnableFiberSafeOptimizations( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_EnableFiberSafeOptimizations, put_EnableFiberSafeOptimizations );
}

bool VPC_Config_Compiler_WholeProgramOptimization( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_WholeProgramOptimization, put_WholeProgramOptimization );
}

bool VPC_Config_Compiler_PreprocessorDefinitions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_PreprocessorDefinitions, put_PreprocessorDefinitions );
}

bool VPC_Config_Compiler_IgnoreStandardIncludePath( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_IgnoreStandardIncludePath, put_IgnoreStandardIncludePath );
}

bool VPC_Config_Compiler_GeneratePreprocessedFile( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	preprocessOption option = preprocessNo;
	if ( !stricmp( buff, "No" ) )
		option = preprocessNo;
	else if ( !stricmp( buff, "With Line Numbers (/P)" ) )
		option = preprocessYes;
	else if ( !stricmp( buff, "Without Line Numbers (/EP /P)" ) )
		option = preprocessNoLineNumbers;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_GeneratePreprocessedFile, put_GeneratePreprocessedFile, preprocessOption, option );
}

bool VPC_Config_Compiler_KeepComments( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_KeepComments, put_KeepComments );
}

bool VPC_Config_Compiler_EnableStringPooling( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_StringPooling, put_StringPooling );
}

bool VPC_Config_Compiler_EnableMinimalRebuild( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_MinimalRebuild, put_MinimalRebuild );
}

bool VPC_Config_Compiler_EnableCPPExceptions( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	cppExceptionHandling option = cppExceptionHandlingNo;
	if ( !stricmp( buff, "No" ) )
		option = cppExceptionHandlingNo;
	else if ( !stricmp( buff, "Yes (/EHsc)" ) )
		option = cppExceptionHandlingYes;
	else if ( !stricmp( buff, "Yes With SEH Exceptions (/EHa)" ) )
		option = cppExceptionHandlingYesWithSEH;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_ExceptionHandling, put_ExceptionHandling, cppExceptionHandling, option );
}

bool VPC_Config_Compiler_SmallerTypeCheck( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_SmallerTypeCheck, put_SmallerTypeCheck );
}

bool VPC_Config_Compiler_BasicRuntimeChecks( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	basicRuntimeCheckOption option = runtimeBasicCheckNone;
	if ( !stricmp( buff, "Default" ) )
		option = runtimeBasicCheckNone;
	else if ( !stricmp( buff, "Stack Frames (/RTCs)" ) )
		option = runtimeCheckStackFrame;
	else if ( !stricmp( buff, "Uninitialized Variables (/RTCu)" ) )
		option = runtimeCheckUninitVariables;
	else if ( !stricmp( buff, "Both (/RTC1, equiv. to /RTCsu)" ) )
		option = runtimeBasicCheckAll;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_BasicRuntimeChecks, put_BasicRuntimeChecks, basicRuntimeCheckOption, option );
}

bool VPC_Config_Compiler_RuntimeLibrary( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	runtimeLibraryOption option = rtMultiThreaded;
	if ( !stricmp( buff, "Multi-threaded (/MT)" ) )
		option = rtMultiThreaded;
	else if ( !stricmp( buff, "Multi-threaded Debug (/MTd)" ) )
		option = rtMultiThreadedDebug;
	else if ( !stricmp( buff, "Multi-threaded DLL (/MD)" ) )
		option = rtMultiThreadedDLL;
	else if ( !stricmp( buff, "Multi-threaded Debug DLL (/MDd)" ) )
		option = rtMultiThreadedDebugDLL;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_RuntimeLibrary, put_RuntimeLibrary, runtimeLibraryOption, option );
}

bool VPC_Config_Compiler_StructMemberAlignment( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	structMemberAlignOption option = alignNotSet;
	if ( !stricmp( buff, "Default" ) )
		option = alignNotSet;
	else if ( !stricmp( buff, "1 Byte (/Zp1)" ) )
		option = alignSingleByte;
	else if ( !stricmp( buff, "2 Bytes (/Zp2)" ) )
		option = alignTwoBytes;
	else if ( !stricmp( buff, "4 Bytes (/Zp4)" ) )
		option = alignFourBytes;
	else if ( !stricmp( buff, "8 Bytes (/Zp8)" ) )
		option = alignEightBytes;
	else if ( !stricmp( buff, "16 Bytes (/Zp16)" ) )
		option = alignSixteenBytes;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_StructMemberAlignment, put_StructMemberAlignment, structMemberAlignOption, option );
}

bool VPC_Config_Compiler_BufferSecurityCheck( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_BufferSecurityCheck, put_BufferSecurityCheck );
}

bool VPC_Config_Compiler_EnableFunctionLevelLinking( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_EnableFunctionLevelLinking, put_EnableFunctionLevelLinking );
}

bool VPC_Config_Compiler_EnableEnhancedInstructionSet( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	enhancedInstructionSetType option = enhancedInstructionSetTypeNotSet;
	if ( !stricmp( buff, "Not Set" ) )
		option = enhancedInstructionSetTypeNotSet;
	else if ( !stricmp( buff, "Streaming SIMD Extensions (/arch:SSE)" ) )
		option = enhancedInstructionSetTypeSIMD;
	else if ( !stricmp( buff, "Streaming SIMD Extensions 2 (/arch:SSE2)" ) )
		option = enhancedInstructionSetTypeSIMD2;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_EnableEnhancedInstructionSet, put_EnableEnhancedInstructionSet, enhancedInstructionSetType, option );
}

bool VPC_Config_Compiler_FloatingPointModel( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	floatingPointModel option = FloatingPointPrecise;
	if ( !stricmp( buff, "Precise (/fp:precise)" ) )
		option = FloatingPointPrecise;
	else if ( !stricmp( buff, "Strict (/fp:strict)" ) )
		option = FloatingPointStrict;
	else if ( !stricmp( buff, "Fast (/fp:fast)" ) )
		option = FloatingPointFast;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_floatingPointModel, put_floatingPointModel, floatingPointModel, option );
}

bool VPC_Config_Compiler_EnableFloatingPointExceptions( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_FloatingPointExceptions, put_FloatingPointExceptions );
}

bool VPC_Config_Compiler_DisableLanguageExtensions( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_DisableLanguageExtensions, put_DisableLanguageExtensions );
}

bool VPC_Config_Compiler_DefaultCharUnsigned( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_DefaultCharIsUnsigned, put_DefaultCharIsUnsigned );
}

bool VPC_Config_Compiler_TreatWCHAR_TAsBuiltInType( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_TreatWChar_tAsBuiltInType, put_TreatWChar_tAsBuiltInType );
}

bool VPC_Config_Compiler_ForceConformanceInForLoopScope( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_ForceConformanceInForLoopScope, put_ForceConformanceInForLoopScope );
}

bool VPC_Config_Compiler_EnableRunTimeTypeInfo( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_RuntimeTypeInfo, put_RuntimeTypeInfo );
}

bool VPC_Config_Compiler_OpenMPSupport( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_OpenMP, put_OpenMP );
}

bool VPC_Config_Compiler_CreateUsePrecompiledHeader( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	pchOption option = pchNone;
	if ( !stricmp( buff, "Not Using Precompiled Headers" ) )
		option = pchNone;
	else if ( !stricmp( buff, "Create Precompiled Header (/Yc)" ) )
		option =pchCreateUsingSpecific;
	else if ( !stricmp( buff, "Use Precompiled Header (/Yu)" ) )
		option = pchUseUsingSpecific;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_UsePrecompiledHeader, put_UsePrecompiledHeader, pchOption, option );
}

bool VPC_Config_Compiler_CreateUsePCHThroughFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_PrecompiledHeaderThrough, put_PrecompiledHeaderThrough );
}

bool VPC_Config_Compiler_PrecompiledHeaderFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_PrecompiledHeaderFile, put_PrecompiledHeaderFile );
}

bool VPC_Config_Compiler_ExpandAttributedSource( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_ExpandAttributedSource, put_ExpandAttributedSource );
}

bool VPC_Config_Compiler_AssemblerOutput( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	asmListingOption option = asmListingNone;
	if ( !stricmp( buff, "No Listing" ) )
		option = asmListingNone;
	else if ( !stricmp( buff, "Assembly-Only Listing (/FA)" ) )
		option = asmListingAssemblyOnly;
	else if ( !stricmp( buff, "Assembly, Machine Code and Source (/FAcs)" ) )
		option = asmListingAsmMachineSrc;
	else if ( !stricmp( buff, "Assembly With Machine Code (/FAc)" ) )
		option = asmListingAsmMachine;
	else if ( !stricmp( buff, "Assembly With Source Code (/FAs)" ) )
		option = asmListingAsmSrc;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_AssemblerOutput, put_AssemblerOutput, asmListingOption, option );
}

bool VPC_Config_Compiler_ASMListLocation( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_AssemblerListingLocation, put_AssemblerListingLocation );
}

bool VPC_Config_Compiler_ObjectFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_ObjectFile, put_ObjectFile );
}

bool VPC_Config_Compiler_ProgramDatabaseFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_ProgramDataBaseFileName, put_ProgramDataBaseFileName );
}

bool VPC_Config_Compiler_GenerateXMLDocumentationFiles( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_GenerateXMLDocumentationFiles, put_GenerateXMLDocumentationFiles );
}

bool VPC_Config_Compiler_XMLDocumentationFileName( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_XMLDocumentationFileName, put_XMLDocumentationFileName );
}

bool VPC_Config_Compiler_EnableBrowseInformation( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	browseInfoOption option = brInfoNone;
	if ( !stricmp( buff, "None" ) )
		option = brInfoNone;
	else if ( !stricmp( buff, "Include All Browse Information (/FR)" ) )
		option = brAllInfo;
	else if ( !stricmp( buff, "No Local Symbols (/Fr)" ) )
		option = brNoLocalSymbols;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_BrowseInformation, put_BrowseInformation, browseInfoOption, option );
}

bool VPC_Config_Compiler_BrowseFile( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_BrowseInformationFile, put_BrowseInformationFile );
}

bool VPC_Config_Compiler_CallingConvention( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	callingConventionOption option = callConventionCDecl;
	if ( !stricmp( buff, "__cdecl (/Gd)" ) )
		option = callConventionCDecl;
	else if ( !stricmp( buff, "__fastcall (/Gr)" ) )
		option = callConventionFastCall;
	else if ( !stricmp( buff, "__stdcall (/Gz)" ) )
		option = callConventionStdCall;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_CallingConvention, put_CallingConvention, callingConventionOption, option );
}

bool VPC_Config_Compiler_CompileAs( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	CompileAsOptions option = compileAsDefault;
	if ( !stricmp( buff, "Default" ) )
		option = compileAsDefault;
	else if ( !stricmp( buff, "Compile as C Code (/TC)" ) )
		option = compileAsC;
	else if ( !stricmp( buff, "Compile as C++ Code (/TP)" ) )
		option = compileAsCPlusPlus;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_CompileAs, put_CompileAs, CompileAsOptions, option );
}

bool VPC_Config_Compiler_DisableSpecificWarnings( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_DisableSpecificWarnings, put_DisableSpecificWarnings );
}

bool VPC_Config_Compiler_ForceIncludes( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_ForcedIncludeFiles, put_ForcedIncludeFiles );
}

bool VPC_Config_Compiler_ForceUsing( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_ForcedUsingFiles, put_ForcedUsingFiles );
}

bool VPC_Config_Compiler_ShowIncludes( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_ShowIncludes, put_ShowIncludes );
}

bool VPC_Config_Compiler_UndefinePreprocessorDefinitions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_UndefinePreprocessorDefinitions, put_UndefinePreprocessorDefinitions );
}

bool VPC_Config_Compiler_UndefineAllPreprocessorDefinitions( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_UndefineAllPreprocessorDefinitions, put_UndefineAllPreprocessorDefinitions );
}

bool VPC_Config_Compiler_UseFullPaths( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_UseFullPaths, put_UseFullPaths );
}

bool VPC_Config_Compiler_OmitDefaultLibraryNames( const char *pPropertyName )
{
	SET_BOOL_PROPERTY( pPropertyName, g_spCompiler, get_OmitDefaultLibName, put_OmitDefaultLibName );
}

bool VPC_Config_Compiler_ErrorReporting( const char *pPropertyName )
{
	char	buff[MAX_SYSTOKENCHARS];

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
		return true;

	compilerErrorReportingType option = compilerErrorReportingDefault;
	if ( !stricmp( buff, "Default" ) )
		option = compilerErrorReportingDefault;
	else if ( !stricmp( buff, "Prompt Immediately (/errorReport:prompt)" ) )
		option = compilerErrorReportingPrompt;
	else if ( !stricmp( buff, "Queue For Next Login (/errorReport:queue)" ) )
		option = compilerErrorReportingQueue;
	else
		VPC_SyntaxError();

	SET_LIST_PROPERTY( pPropertyName, g_spCompiler, get_ErrorReporting, put_ErrorReporting, compilerErrorReportingType, option );
}

bool VPC_Config_Compiler_AdditionalOptions( const char *pPropertyName )
{
	SET_STRING_PROPERTY( pPropertyName, g_spCompiler, get_AdditionalOptions, put_AdditionalOptions );
}

property_t g_compilerProperties[] =
{
	// General
	{"$AdditionalIncludeDirectories",		VPC_Config_Compiler_AdditionalIncludeDirectories},
	{"$Resolve#UsingReferences",			VPC_Config_Compiler_ResolveUsingReferences},
	{"$DebugInformationFormat",				VPC_Config_Compiler_DebugInformationFormat},
	{"$SuppressStartupBanner",				VPC_Config_Compiler_SuppressStartupBanner},
	{"$WarningLevel",						VPC_Config_Compiler_WarningLevel},
	{"$Detect64bitPortabilityIssues",		VPC_Config_Compiler_Detect64bitPortabilityIssues},
	{"$TreatWarningsAsErrors",				VPC_Config_Compiler_TreatWarningsAsErrors},
	{"$UseUNICODEResponseFiles",			VPC_Config_Compiler_UseUNICODEResponseFiles},

	// Optimization
	{"$Optimization",						VPC_Config_Compiler_Optimization},
	{"$InlineFunctionExpansion",			VPC_Config_Compiler_InlineFunctionExpansion},
	{"$EnableIntrinsicFunctions",			VPC_Config_Compiler_EnableIntrinsicFunctions},
	{"$FavorSizeOrSpeed",					VPC_Config_Compiler_FavorSizeOrSpeed},
	{"$OmitFramePointers",					VPC_Config_Compiler_OmitFramePointers},
	{"$EnableFiberSafeOptimizations",		VPC_Config_Compiler_EnableFiberSafeOptimizations},
	{"$WholeProgramOptimization",			VPC_Config_Compiler_WholeProgramOptimization},

	// Preprocessor
	{"$PreprocessorDefinitions",			VPC_Config_Compiler_PreprocessorDefinitions},
	{"$IgnoreStandardIncludePath",			VPC_Config_Compiler_IgnoreStandardIncludePath},
	{"$GeneratePreprocessedFile",			VPC_Config_Compiler_GeneratePreprocessedFile},
	{"$KeepComments",						VPC_Config_Compiler_KeepComments},

	// Code Generation
	{"$EnableStringPooling",				VPC_Config_Compiler_EnableStringPooling},
	{"$EnableMinimalRebuild",				VPC_Config_Compiler_EnableMinimalRebuild},
	{"$EnableC++Exceptions",				VPC_Config_Compiler_EnableCPPExceptions},
	{"$SmallerTypeCheck",					VPC_Config_Compiler_SmallerTypeCheck},
	{"$BasicRuntimeChecks",					VPC_Config_Compiler_BasicRuntimeChecks},
	{"$RuntimeLibrary",						VPC_Config_Compiler_RuntimeLibrary},
	{"$StructMemberAlignment",				VPC_Config_Compiler_StructMemberAlignment},
	{"$BufferSecurityCheck",				VPC_Config_Compiler_BufferSecurityCheck},
	{"$EnableFunctionLevelLinking",			VPC_Config_Compiler_EnableFunctionLevelLinking},
	{"$EnableEnhancedInstructionSet",		VPC_Config_Compiler_EnableEnhancedInstructionSet},
	{"$FloatingPointModel",					VPC_Config_Compiler_FloatingPointModel},
	{"$EnableFloatingPointExceptions",		VPC_Config_Compiler_EnableFloatingPointExceptions},

	// Language
	{"$DisableLanguageExtensions",			VPC_Config_Compiler_DisableLanguageExtensions},
	{"$DefaultCharUnsigned",				VPC_Config_Compiler_DefaultCharUnsigned},
	{"$TreatWCHAR_TAsBuiltInType",			VPC_Config_Compiler_TreatWCHAR_TAsBuiltInType},
	{"$ForceConformanceInForLoopScope",		VPC_Config_Compiler_ForceConformanceInForLoopScope},
	{"$EnableRunTimeTypeInfo",				VPC_Config_Compiler_EnableRunTimeTypeInfo},
	{"$OpenMPSupport",						VPC_Config_Compiler_OpenMPSupport},

	// Precompiled Headers
	{"$Create/UsePrecompiledHeader",		VPC_Config_Compiler_CreateUsePrecompiledHeader},
	{"$Create/UsePCHThroughFile",			VPC_Config_Compiler_CreateUsePCHThroughFile},
	{"$PrecompiledHeaderFile",				VPC_Config_Compiler_PrecompiledHeaderFile},

	// Output Files
	{"$ExpandAttributedSource",				VPC_Config_Compiler_ExpandAttributedSource},
	{"$AssemblerOutput",					VPC_Config_Compiler_AssemblerOutput},
	{"$ASMListLocation",					VPC_Config_Compiler_ASMListLocation},
	{"$ObjectFileName",						VPC_Config_Compiler_ObjectFileName},
	{"$ProgramDatabaseFileName",			VPC_Config_Compiler_ProgramDatabaseFileName},
	{"$GenerateXMLDocumentationFiles",		VPC_Config_Compiler_GenerateXMLDocumentationFiles},
	{"$XMLDocumentationFileName",			VPC_Config_Compiler_XMLDocumentationFileName},

	// Browse Information
	{"$EnableBrowseInformation",			VPC_Config_Compiler_EnableBrowseInformation},
	{"$BrowseFile",							VPC_Config_Compiler_BrowseFile},

	// Advanced
	{"$CallingConvention",					VPC_Config_Compiler_CallingConvention},
	{"$CompileAs",							VPC_Config_Compiler_CompileAs},
	{"$DisableSpecificWarnings",			VPC_Config_Compiler_DisableSpecificWarnings},
	{"$ForceIncludes",						VPC_Config_Compiler_ForceIncludes},
	{"$Force#Using",						VPC_Config_Compiler_ForceUsing},
	{"$ShowIncludes",						VPC_Config_Compiler_ShowIncludes},
	{"$UndefinePreprocessorDefinitions",	VPC_Config_Compiler_UndefinePreprocessorDefinitions},
	{"$UndefineAllPreprocessorDefinitions",	VPC_Config_Compiler_UndefineAllPreprocessorDefinitions},
	{"$UseFullPaths",						VPC_Config_Compiler_UseFullPaths},
	{"$OmitDefaultLibraryNames",			VPC_Config_Compiler_OmitDefaultLibraryNames},
	{"$ErrorReporting",						VPC_Config_Compiler_ErrorReporting},

	// Command Line
	{"$AdditionalOptions",					VPC_Config_Compiler_AdditionalOptions},
	{NULL}
};