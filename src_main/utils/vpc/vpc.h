//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#pragma once

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#include "mathlib/mathlib.h"
#include "UtlString.h"
#include "UtlRBTree.h"
#include "UtlVector.h"
#include "UtlBuffer.h"
#include "UtlDict.h"
#include "checksum_crc.h"
#include "strtools.h"
#include "sys_utils.h"
#include <atlbase.h>

#import <VCProjectEngine.dll> raw_interfaces_only raw_native_types rename( "GetObject", "GetObject1" ) rename( "PropertySheet", "PropertySheet1" )
using namespace VCProjectEngineLibrary;

#import <dte80a.olb> raw_interfaces_only raw_native_types rename( "GetObject", "GetObjectX" ) rename( "SearchPath", "SearchPathX" )

#pragma warning( push )
#pragma warning( disable : 4772 )
#import <VCPlatformX360.dll> raw_interfaces_only raw_native_types
#pragma warning( pop )
namespace X360 = VCPlatformX360Library;

#define VCPROJ_E_FILE_EXISTS	(0x80050500L) 
#define VCPROJ_E_FILTER_EXISTS	(0x80050501L)

#define PLATFORM_ALL	0x00
#define PLATFORM_WIN32	0x01
#define PLATFORM_LINUX	0x02
#define PLATFORM_X360	0x04
#define PLATFORM_PS3	0x08

typedef enum
{
	KEYWORD_GENERAL,
	KEYWORD_DEBUGGING,
	KEYWORD_COMPILER,
	KEYWORD_XBOXCOMPILER,
	KEYWORD_LIBRARIAN,
	KEYWORD_LINKER,
	KEYWORD_XBOXLINKER,
	KEYWORD_MANIFEST,
	KEYWORD_XMLDOCGEN,
	KEYWORD_BROWSEINFO,
	KEYWORD_RESOURCES,
	KEYWORD_PREBUILDEVENT,
	KEYWORD_PRELINKEVENT,
	KEYWORD_POSTBUILDEVENT,
	KEYWORD_CUSTOMBUILDSTEP,
	KEYWORD_XBOXIMAGE,
	KEYWORD_XBOXDEPLOYMENT,
	KEYWORD_MAX,
} configKeyword_e;

typedef bool (*procptr_t)( const char *pPropertyName );
typedef bool (*GetSymbolProc_t)( const char *pKey );

#define INVALID_INDEX -1

typedef struct
{
	char		*pName;
	procptr_t	handler;
	int			platformMask;
} property_t;

typedef enum
{
	DEFINE_NULL,
	DEFINE_PLATFORM,
	DEFINE_GAME,
	DEFINE_CUSTOM
} defineType_e;

typedef int defineIndex_t;
typedef struct
{
	CUtlString		name;
	CUtlString		upperCaseName;
	defineType_e	type;
	bool			bDefined;	// a symbol can be present but not defined (e.g. default symbols)
	bool			bSet;		// only used during iteration when symbol defined
} define_t;

typedef int scriptIndex_t;
typedef struct
{
	CUtlString		name;
	CUtlString		condition;
} script_t;

typedef int	projectIndex_t;
typedef struct
{
	CUtlString				name;
	CUtlVector<script_t>	scripts;
} project_t;

typedef int groupIndex_t;
typedef struct
{
	CUtlVector<projectIndex_t>	projects;
} group_t;

typedef int groupTagIndex_t;
typedef struct
{
	CUtlString					name;
	CUtlVector<groupIndex_t>	groups;

	// this tag is an implicit definition of the project
	bool						bSameAsProject;
} groupTag_t;

typedef struct
{
	CUtlString	name;
	CUtlString	value;
} macro_t;

typedef struct
{
	CUtlString	m_scriptName;
	CRC32_t		m_crc;
} scriptList_t;

extern void				VPC_Error( const char *format, ... );
extern void				VPC_Warning( const char *format, ... );
extern void				VPC_Status( bool bAlwaysSpew, const char *format, ... );
extern void				VPC_SyntaxError( const char *format = NULL, ... );
extern void				VPC_ParseGroupScript( const char *pScriptName );
extern groupTagIndex_t	VPC_Group_FindOrCreateGroupTag( char *pName, bool bCreate );
extern defineIndex_t	VPC_FindOrCreateDefine( const char *pName, bool bCreate, defineType_e type );
extern bool				VPC_IsDefineSet( const char *pName );
extern void				VPC_GenerateBuildSet();
extern bool				VPC_Group_ConditionHasDefinedType( const char *pCondition, defineType_e type );
extern bool				VPC_EvaluateCondition( const char *pCondition );
extern void				VPC_Keyword_Configuration();
extern void				VPC_Keyword_FileConfiguration();
extern VARIANT_BOOL		StringToBool( const char *pString );
extern bool				ParsePropertyValue( char **pScriptData, int *pScriptLine, char *baseString, char *outBuff, int outBuffSize );
extern void				VPC_Config_SpewProperties( configKeyword_e keyword );
extern bool				EvaluateExpression( bool &result, const char *InfixExpression, GetSymbolProc_t pGetSymbolProc );
extern void				ResolveMacrosInString( char *pString, char *pOutBuff, int outBuffSize );
extern void				VPC_ParseProjectScript( const char *pScriptName, int depth );

extern CUtlVector<project_t>		g_projects;
extern CUtlVector<group_t>			g_groups;
extern CUtlVector<groupTag_t>		g_groupTags;
extern CUtlVector<projectIndex_t>	g_targetProjects;
extern CUtlVector<define_t>			g_defines;
extern CUtlVector<macro_t>			g_macros;
extern CUtlVector<CUtlString>		g_buildCommands;

extern int					*g_pScriptLine;
extern char					*g_pScriptData;
extern char					*g_pScriptName;
extern char					g_szOutputFilename[];
extern char					g_szProjectName[];
extern bool					g_bIgnoreRedundancyWarning;
extern bool					g_bGeneratedProject;
extern bool					g_bDecorateProject;

extern CComQIPtr<VCFile>					g_spFile;
extern CComQIPtr<VCConfiguration>			g_spConfig;
extern CComQIPtr<VCDebugSettings>			g_spDebugSettings;
extern CComQIPtr<IVCCollection>				g_spToolCol;
extern CComPtr<VCCLCompilerTool>			g_spCompiler;
extern CComPtr<VCLinkerTool>				g_spLinker;
extern CComPtr<VCBscMakeTool>				g_spBrowseInfoTool;
extern CComPtr<VCResourceCompilerTool>		g_spResources;
extern CComPtr<VCPreBuildEventTool>			g_spPreBuildEventTool;
extern CComPtr<VCPreLinkEventTool>			g_spPreLinkEventTool;
extern CComPtr<VCPostBuildEventTool>		g_spPostBuildEventTool;
extern CComPtr<VCCustomBuildTool>			g_spCustomBuildTool;
extern CComPtr<VCLibrarianTool>				g_spLibrarian;
extern CComPtr<VCManifestTool>				g_spManifestTool;
extern CComPtr<VCXDCMakeTool>				g_spXMLDocGenTool;
extern CComQIPtr<VCProject>					g_spProject;
extern CComPtr<X360::VCCLX360CompilerTool>	g_spXboxCompiler;
extern CComPtr<X360::VCX360LinkerTool>		g_spXboxLinker;
extern CComPtr<X360::VCX360ImageTool>		g_spXboxImageTool;
extern CComPtr<X360::VCX360DeploymentTool>	g_spXboxDeploymentTool;

extern property_t g_generalProperties[];
extern property_t g_debuggingProperties[];
extern property_t g_compilerProperties[];
extern property_t g_linkerProperties[];
extern property_t g_browseInfoProperties[];
extern property_t g_resourcesProperties[];
extern property_t g_preBuildEventProperties[];
extern property_t g_preLinkEventProperties[];
extern property_t g_postBuildEventProperties[];
extern property_t g_customBuildProperties[];
extern property_t g_librarianProperties[];
extern property_t g_xboxCompilerProperties[];
extern property_t g_xboxLinkerProperties[];
extern property_t g_xboxDeploymentProperties[];
extern property_t g_xboxImageProperties[];
extern property_t g_manifestProperties[];
extern property_t g_xmlDocGenProperties[];

#define SET_BOOL_PROPERTY( pPropertyName, Tool, GetFunc, PutFunc )							\
	{																						\
	char buff[MAX_SYSTOKENCHARS];															\
	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )	\
		return true;																		\
	VARIANT_BOOL bVal = StringToBool( buff );												\
	VARIANT_BOOL bCurrentVal;																\
	HRESULT hr = Tool->GetFunc( &bCurrentVal );												\
	if ( FAILED( hr ) )																		\
		return false;																		\
	if ( bCurrentVal == bVal )																\
		VPC_Warning( "%s matches default setting, [%s line:%d]", pPropertyName, g_pScriptName, *g_pScriptLine );	\
	hr = Tool->PutFunc( bVal );																\
	return !FAILED( hr );																	\
	}

#define SET_LONG_PROPERTY( pPropertyName, Tool, GetFunc, PutFunc )							\
	{																						\
	char buff[MAX_SYSTOKENCHARS];															\
	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )	\
		return true;																		\
	long lValue = atol( buff );																\
	long currentValue;																		\
	HRESULT hr = Tool->GetFunc( &currentValue );											\
	if ( FAILED( hr ) )																		\
		return false;																		\
	if ( currentValue == lValue )															\
		VPC_Warning( "%s matches default setting, [%s line:%d]", pPropertyName, g_pScriptName, *g_pScriptLine );	\
	hr = Tool->PutFunc( lValue );															\
	return !FAILED( hr );																	\
	}

#define SET_STRING_PROPERTY( pPropertyName, Tool, GetFunc, PutFunc )									\
	{																									\
	char buff[MAX_SYSTOKENCHARS];																		\
	CComBSTR bstr;																						\
	HRESULT hr = Tool->GetFunc( &bstr );																\
	if ( FAILED( hr ) )																					\
		return false;																					\
	CW2CT szString( bstr );																				\
	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, szString.m_psz, buff, sizeof( buff ) ) )	\
		return true;																					\
	if ( szString.m_psz && !stricmp( szString.m_psz, buff ) )											\
		VPC_Warning( "%s matches default setting, [%s line:%d]", pPropertyName, g_pScriptName, *g_pScriptLine );	\
	bstr = buff;																						\
	hr = Tool->PutFunc( bstr );																			\
	return !FAILED( hr );																				\
	}	

#define SET_LIST_PROPERTY( pPropertyName, Tool, GetFunc, PutFunc, Type, NewOption )	\
	{																				\
	Type currentOption;																\
	HRESULT hr = Tool->GetFunc( &currentOption );									\
	if ( FAILED( hr ) )																\
		return false;																\
	if ( currentOption == NewOption )												\
		VPC_Warning( "%s matches default setting, [%s line:%d]", pPropertyName, g_pScriptName, *g_pScriptLine );	\
	hr = Tool->PutFunc( NewOption );												\
	return !FAILED( hr );															\
	}




