//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

int							*g_pScriptLine;
char						*g_pScriptData;
char						*g_pScriptName;
char						g_szConfigurationName[MAX_PATH];
char						g_szOutputFilename[MAX_PATH];
char						g_szStartDirectory[MAX_PATH];
char						g_szProjectName[MAX_PATH];
bool						g_bVerbose;
bool						g_bHelp;
bool						g_bUsageOnly;
bool						g_bSpewPlatforms;
bool						g_bSpewGames;
bool						g_bSpewGroups;
bool						g_bSpewProjects;
bool						g_bIgnoreRedundancyWarning;
bool						g_bSpewProperties;
bool						g_bTestMode;
bool						g_bGeneratedProject;
bool						g_bForce;
bool						g_bDecorateProject;
CUtlVector<define_t>		g_defines;
char						g_szTempGroupScript[MAX_PATH];
CUtlVector<CUtlString>		g_buildCommands;

//-----------------------------------------------------------------------------
//	VPC_Error
//
//-----------------------------------------------------------------------------
void VPC_Error( const char* format, ... )
{
	va_list		argptr;
	char		msg[MAX_SYSPRINTMSG];
		
	va_start( argptr,format );
	vsprintf( msg,format,argptr );
	va_end( argptr );

	Msg( "ERROR: %s\n", msg );

	if ( g_szTempGroupScript[0] )
	{
		// delete temp work file
		unlink( g_szTempGroupScript );
	}

	exit( 1 );
}

//-----------------------------------------------------------------------------
//	VPC_Warning
//
//-----------------------------------------------------------------------------
void VPC_Warning( const char* format, ... )
{
	va_list		argptr;
	char		msg[MAX_SYSPRINTMSG];

	va_start( argptr,format );
	vsprintf( msg,format,argptr );
	va_end( argptr );

	if ( g_bIgnoreRedundancyWarning && V_stristr( msg, "matches default setting" ) )
		return;

	Msg( "WARNING: %s\n", msg );
}

//-----------------------------------------------------------------------------
//	VPC_Status
//
//-----------------------------------------------------------------------------
void VPC_Status( bool bAlwaysSpew, const char* format, ... )
{
	va_list		argptr;
	char		msg[MAX_SYSPRINTMSG];

	va_start( argptr,format );
	vsprintf( msg,format,argptr );
	va_end( argptr );

	if ( bAlwaysSpew || g_bVerbose )
	{
		Msg( "%s\n", msg );
	}
}

//-----------------------------------------------------------------------------
//	VPC_SyntaxError
//
//-----------------------------------------------------------------------------
void VPC_SyntaxError( const char* format, ... )
{
	va_list		argptr;
	char		msg[MAX_SYSPRINTMSG];

	va_start( argptr, format );
	if ( format )
	{
		vsprintf( msg, format, argptr );
	}
	va_end( argptr );

	if ( format )
	{
		Msg( "Bad Syntax: %s\n", msg );
	}
	VPC_Error( "Bad Syntax: [%s line:%d (or previous line)]", g_pScriptName, *g_pScriptLine );
}

//-----------------------------------------------------------------------------
// Returns argument index if found, 0 otherwise
//----------------------------------------------------------------------------
int VPC_CheckCommandLine( int argc, char *argv[], const char *pArg )
{
	for ( int i=1; i<argc; i++ )
	{
		if ( argv[i][0] != '/' && argv[i][0] != '-' )
		{
			continue;
		}

		if ( !V_stricmp( argv[i]+1, pArg ) )
		{
			return i;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
//	StringToBool
//
//	Ignores allowable trailing characters.
//-----------------------------------------------------------------------------
VARIANT_BOOL StringToBool( const char *pString )
{
	if ( !strnicmp( pString, "no", 2 ) || 
		!strnicmp( pString, "off", 3 ) || 
		!strnicmp( pString, "false", 5 ) || 
		!strnicmp( pString, "0", 1 ) )
	{
		// false
		return 0;
	}
	else if ( !strnicmp( pString, "yes", 3 ) || 
			!strnicmp( pString, "on", 2 ) || 
			!strnicmp( pString, "true", 4  ) || 
			!strnicmp( pString, "1", 1 ) )
	{
		// true
		return -1;
	}
	else
	{
		// unknown boolean expression
		VPC_SyntaxError( "Unknown boolean expression" );
	}

	// assume false
	return 0;
}

//-----------------------------------------------------------------------------
//	VPC_FindOrCreateDefine
//
//	Case Insensitive
//-----------------------------------------------------------------------------
defineIndex_t VPC_FindOrCreateDefine( const char *pName, bool bCreate, defineType_e type )
{
	for (int i=0; i<g_defines.Count(); i++)
	{
		if ( !stricmp( pName, g_defines[i].name.String() ) )
			return i;
	}

	if ( !bCreate )
		return INVALID_INDEX;

	defineIndex_t index = g_defines.AddToTail();
	memset( &g_defines[index], 0, sizeof( define_t ) );

	char tempName[256];
	strcpy( tempName, pName );
	
	// primary internal use as lower case, but spewed to user as upper for style consistency
	g_defines[index].name = strlwr( tempName );
	g_defines[index].upperCaseName = strupr( tempName );
	g_defines[index].type = type;

	return index;
}

//-----------------------------------------------------------------------------
//	Case Insensitive. Returns TRUE if define has been set.
//-----------------------------------------------------------------------------
bool VPC_IsDefineSet( const char *pName )
{
	for (int i=0; i<g_defines.Count(); i++)
	{
		if ( !stricmp( pName, g_defines[i].name.String() ) )
		{
			return g_defines[i].bSet;
		}
	}
	return false;
}

bool ReplaceString( const char *pStream, const char *pSearch, const char *pReplace, char *pOutBuff, int outBuffSize )
{
	const char	*pFind;
	const char	*pStart = pStream;
	char		*pOut   = pOutBuff;
	int			len;
	bool		bReplaced = false;

	while ( 1 )
	{
		// find sub string
		pFind = V_stristr( pStart, pSearch );
		if ( !pFind )
		{
			/// end of string
			len = strlen( pStart );
			pFind = pStart + len;
			memcpy( pOut, pStart, len );
			pOut += len;
			break;
		}
		else
		{
			bReplaced = true;
		}

		// copy up to sub string
		len = pFind - pStart;
		memcpy( pOut, pStart, len );
		pOut += len;

		// substitute new string
		len = strlen( pReplace );
		memcpy( pOut, pReplace, len );
		pOut += len;

		// advance past sub string
		pStart = pFind + strlen( pSearch );
	}

	*pOut = '\0';

	return bReplaced;
}

void ResolveMacrosInString( char *pString, char *pOutBuff, int outBuffSize )
{
	char	macroName[MAX_SYSTOKENCHARS];
	char	buffer1[MAX_SYSTOKENCHARS];
	char	buffer2[MAX_SYSTOKENCHARS];
	int		i;

	// iterate and resolve user macros until all macros resolved
	strcpy( buffer1, pString );
	bool bDone;
	do
	{
		bDone = true;
		for ( i=0; i<g_macros.Count(); i++ )
		{
			sprintf( macroName, "$%s", g_macros[i].name.String() );
			if ( ReplaceString( buffer1, macroName, g_macros[i].value.String(), buffer2, sizeof( buffer2 ) ) )
			{
				bDone = false;
			}
			strcpy( buffer1, buffer2 );
		}
	}
	while ( !bDone );

	int len = min( outBuffSize, (int)strlen( buffer1 ) );
	memcpy( pOutBuff, buffer1, len );
	pOutBuff[len] = '\0';
}

//-----------------------------------------------------------------------------
//	VPC_ResolveSymbol
//
//	Callback for expression evaluator.
//-----------------------------------------------------------------------------
bool VPC_ResolveSymbol( const char *pSymbol )
{
	int offset = 0;

	if ( !stricmp( pSymbol, "$0" ) )
	{
		return false;
	}
	else if ( !stricmp( pSymbol, "$1" ) )
	{
		return true;
	}

	if ( pSymbol[0] == '$' )
		offset = 1;

	defineIndex_t defineIndex = VPC_FindOrCreateDefine( (char*)pSymbol+offset, false, DEFINE_NULL );
	if ( defineIndex != INVALID_INDEX )
	{
		// custom defines are gated by their 'definition' state
		if ( g_defines[defineIndex].type == DEFINE_CUSTOM )
		{
			return g_defines[defineIndex].bDefined;
		}

		// all other type of defines are gated by their 'set' state
		return g_defines[defineIndex].bSet;
	}

	// undefined
	return false;
}

//-----------------------------------------------------------------------------
//	VPC_EvaluateCondition
//
//-----------------------------------------------------------------------------
bool VPC_EvaluateCondition( const char* pCondition )
{
	bool result = false;

	EvaluateExpression( result, pCondition, VPC_ResolveSymbol );

	return result;
}

//-----------------------------------------------------------------------------
//	ParsePropertyValue
//
//	Handles expressions of the form <$BASE> <xxx> ... <xxx> [condition]
//	Output is a concatenated string.
//
//	Returns TRUE if expression should be used, FALSE if it should be ignored due
//	to an optional condition that evaluated TRUE.
//-----------------------------------------------------------------------------
bool ParsePropertyValue( char **pScriptData, int *pScriptLine, char *pBaseString, char *pOutBuff, int outBuffSize )
{
	char	*pToken;
	char	*pNextToken;
	char	*pOut = pOutBuff;
	char	*pScriptSave;
	int		remaining = outBuffSize-1;
	int		len;
	bool	bAllowNextLine = false;
	char	buffer1[MAX_SYSTOKENCHARS];
	char	buffer2[MAX_SYSTOKENCHARS];
	bool	bResult = true;
	char	activeToken[MAX_SYSTOKENCHARS];

	while ( 1 )
	{
		pScriptSave = *pScriptData;

		pToken = Sys_GetToken( pScriptData, bAllowNextLine, pScriptLine );
		if ( !pToken || !pToken[0] )
			VPC_SyntaxError();

		strcpy( activeToken, pToken );
		pToken = activeToken;

		pNextToken = Sys_PeekToken( *pScriptData, false );
		if ( !pNextToken || !pNextToken[0] )
		{
			// current token is last token
			// last token can be optional conditional, need to identify
			// backup and reparse up to last token
			pScriptSave = Sys_SkipWhitespace( pScriptSave, NULL, NULL );
			if ( pScriptSave && pScriptSave[0] == '[' )
			{
				// last token is an optional conditional
				bResult = VPC_EvaluateCondition( pToken );
				break;
			}
		}

		if ( !stricmp( pToken, "\\" ) )
		{
			bAllowNextLine = true;
			continue;
		}
		else
		{
			bAllowNextLine = false;
		}

		if ( !stricmp( pToken, "\\n" ) )
		{
			pToken = "\n";
		}

		if ( pToken[0] )
		{
			// handle reserved macro
			if ( !pBaseString )
				pBaseString = "";
			strcpy( buffer1, pToken );
			ReplaceString( buffer1, "$base", pBaseString, buffer2, sizeof( buffer2 ) );

			ResolveMacrosInString( buffer2, buffer1, sizeof( buffer1 ) );

			len = min( (int)strlen( buffer1 ), remaining );
			if ( len <= 0 )
				break;
			memcpy( pOut, buffer1, len );
			pOut += len;
			remaining -= len;
		}

		pToken = Sys_PeekToken( *pScriptData, false );
		if ( !pToken || !pToken[0] )
			break;
	}

	*pOut++ = '\0';

	if ( !pOutBuff[0] )
		VPC_SyntaxError();

	return bResult;
}

//-----------------------------------------------------------------------------
// Checks to ensure the bin path is in the same tree as the vpc_scripts
// Returns true if bin path valid
//-----------------------------------------------------------------------------
bool CheckBinPath( char *pOutBinPath, int outBinPathSize )
{
	char	szScriptPath[MAX_PATH];
	char	szDirectory[MAX_PATH];
	char	szLastDirectory[MAX_PATH];

	// non destructively determine the vpc_scripts directory
	bool bFound = false;
	szLastDirectory[0] = '\0';
	szScriptPath[0] = '\0';
	GetCurrentDirectory( sizeof( szDirectory ), szDirectory );
	while ( 1 )
	{
		V_ComposeFileName( szDirectory, "vpc_scripts", szScriptPath, sizeof( szScriptPath ) );
		struct _stat statBuf;
		if ( _stat( szScriptPath, &statBuf ) != -1 )
		{
			bFound = true;
			break;
		}

		// previous dir
		V_ComposeFileName( szDirectory, "..", szScriptPath, sizeof( szScriptPath ) );

		char fullPath[MAX_PATH];
		if ( _fullpath( fullPath, szScriptPath, sizeof( fullPath ) ) )
		{
			strcpy( szDirectory, fullPath );
		}

		if ( !V_stricmp( szDirectory, szLastDirectory ) )
		{
			// can back up no further
			break;
		}
		strcpy( szLastDirectory, szDirectory );
	}

	if ( !bFound )
	{
		VPC_Error( "Failed to determine source directory from current path. Expecting 'vpc_scripts' in source path." );
	}

	char szSourcePath[MAX_PATH];
	strcpy( szSourcePath, szDirectory );

	// check to ensure that executeable and src directory are in the same tree
	// executeable needs to be tightly bound to its vpc_scripts

	char szModuleBinPath[MAX_PATH];
	GetModuleFileName( NULL, szModuleBinPath, sizeof( szModuleBinPath ) );
	V_FixSlashes( szModuleBinPath );
	V_strncpy( pOutBinPath, szModuleBinPath, outBinPathSize );

	char *pString = V_stristr( szModuleBinPath, "\\devtools\\bin\\" );
	if ( pString )
	{
		// source dirs should match
		char chSave = *pString;
		*pString = '\0';
		bool bSame = stricmp( szSourcePath, szModuleBinPath ) == 0;
		*pString = chSave;

		if ( bSame )
		{
			return true;
		}
	}
	else
	{
		VPC_Error( "Executable not running from 'devtools/bin' but from unexpected directory '%s'", szModuleBinPath );
	}

	// mismatched, wierd bin patch could have been a result of user's environment path
	// use expected source path which is based on user's cwd to get the real bin path
	V_strncpy( pOutBinPath, szSourcePath, outBinPathSize );
	V_strncat( pOutBinPath, "\\devtools\\bin\\vpc.exe", outBinPathSize );
	struct _stat statBuf;
	if ( _stat( pOutBinPath, &statBuf ) == -1 )
	{
		VPC_Error( "Correct executeable missing, should be at '%s'", pOutBinPath );
	}

	// yikes, wrong executeable was started, agreed behavior was to restart based on user's cwd
	VPC_Status( false, "********************************************************************************\n" );
	VPC_Status( false, "Wrong Executable '%s' Running!\nRestarting at '%s'\n", szModuleBinPath, pOutBinPath );
	VPC_Status( false, "********************************************************************************\n" );
	
	return false;
}

//-----------------------------------------------------------------------------
// Sets the working directory to .../vpc_scripts as all scripts are 
// guaranteed relative to the vpc script directory.
//-----------------------------------------------------------------------------
void SetDefaultSourcePath()
{
	char	szSourcePath[MAX_PATH];
	char	szDirectory[MAX_PATH];
	char	szLastDirectory[MAX_PATH];

	// find vpc_scripts from cwd
	szLastDirectory[0] = '\0';
	bool bFound = false;
	while ( 1 )
	{
		GetCurrentDirectory( sizeof( szDirectory ), szDirectory );
		if ( !V_stricmp( szDirectory, szLastDirectory ) )
		{
			// can back up no further
			break;
		}
		strcpy( szLastDirectory, szDirectory );

		V_ComposeFileName( szDirectory, "vpc_scripts", szSourcePath, sizeof( szSourcePath ) );
		struct _stat statBuf;
		if ( _stat( szSourcePath, &statBuf ) != -1 )
		{
			bFound = true;
			break;
		}

		// previous dir
		V_ComposeFileName( szDirectory, "..", szSourcePath, sizeof( szSourcePath ) );
		SetCurrentDirectory( szSourcePath );
	}

	if ( !bFound )
	{
		VPC_Error( "Failed to determine source directory from current path. Expecting 'vpc_scripts' in source path." );
	}

	// always emit source path, identifies MANY redundant user problems
	// users can easily run from an unintended place due to botched path, mangled directories, etc
	Msg( "Source Path: %s\n", szDirectory );
}

//-----------------------------------------------------------------------------
//	VPC_CheckCRC
//
//	TRUE if crc matches, FALSE otherwise
//-----------------------------------------------------------------------------
bool VPC_CheckCRC( const char *pScriptName, CRC32_t crc )
{
	char	*pScriptBuffer;

	VPC_Status( false, "Checking CRC: %s", pScriptName );

	if ( !Sys_Exists( pScriptName ) )
	{
		return false;
	}

	int scriptLen = Sys_LoadFile( pScriptName, (void**)&pScriptBuffer, true );

	bool bResult = CRC32_ProcessSingleBuffer( pScriptBuffer, scriptLen ) == crc;

	free( pScriptBuffer );

	return bResult;
}

//-----------------------------------------------------------------------------
//	VPC_IsProjectCurrent
//
//-----------------------------------------------------------------------------
bool VPC_IsProjectCurrent( const char *pVCProjFilename )
{
	// default is project is stale
	bool bCRCValid = false;
	char *pBuffer = NULL;

	if ( !Sys_Exists( pVCProjFilename ) )
	{
		return false;
	}

	Sys_LoadFile( pVCProjFilename, (void **)&pBuffer, true );

	// all vpc generated VCProj's have a crc signature
	char *pCRCString = V_stristr( pBuffer, "vpc.exe -crc" );
	if ( !pCRCString )
	{
		// crc signature not found
		goto cleanUp;
	}
	
	VPC_Status( true, "" );

	// tokenize, expected stream is <-crc> <filename> <crcvalue>
	// parse and check all scripts
	unsigned int crcValue;
	char szScriptname[MAX_PATH];
	char *pToken;
	pCRCString += strlen( "vpc.exe " );
	for ( ;; )
	{
		pToken = Sys_GetToken( &pCRCString, false, NULL );
		if ( !pToken || !pToken[0] )
		{
			// end of stream
			break;	
		}
		if ( stricmp( pToken, "-crc" ) )
		{
			// syntax error
			bCRCValid = false;
			break;
		}

		pToken = Sys_GetToken( &pCRCString, false, NULL );
		if ( !pToken || !pToken[0] )
		{
			// syntax error
			bCRCValid = false;
			break;	
		}
		strcpy( szScriptname, pToken );

		pToken = Sys_GetToken( &pCRCString, false, NULL );
		if ( !pToken || !pToken[0] )
		{
			// syntax error
			bCRCValid = false;
			break;	
		}
		crcValue = 0;
		sscanf( pToken, "%x", &crcValue );

		if ( !VPC_CheckCRC( szScriptname, crcValue ) )
		{
			// crc mismatch
			bCRCValid = false;
			break;
		}
		else
		{
			// this script is valid, keep parsing
			bCRCValid = true;
		}
	}

cleanUp:
	if ( pBuffer )
	{
		free( pBuffer );
	}

	if ( bCRCValid )
	{
		VPC_Status( true, "Valid: '%s' Passes CRC Checks.", pVCProjFilename );
	}
	else
	{
		VPC_Status( true, "Stale: '%s' Requires Rebuild.", pVCProjFilename );
	}

	return bCRCValid;	
}

//-----------------------------------------------------------------------------
//	VPC_Usage
//
//-----------------------------------------------------------------------------
void VPC_Usage( void )
{
	int	i;

	// skip header if user requested specific detail
	bool bNoHeader = 
		g_bSpewPlatforms || 
		g_bSpewGames || 
		g_bSpewProjects ||
		g_bSpewGroups ||
		g_bSpewProperties ||
		g_buildCommands.Count();

	if ( !bNoHeader )
	{
		Msg( "\n" );

		if ( !g_bHelp )
		{
			// terse
			Msg( "Type vpc /h for help...\n" );
		}
		else
		{
			// verbose
			Msg( "usage: vpc [options] <+/-project or group>\n");

			Msg( "\n" );
			Msg( "Examples:\n" );

			Msg( "\n" );
			Msg( "  Single .vcproj generation:\n" );
			Msg( "    vpc +client /hl2     <-- Creates a Win32 .vcproj for the HL2 client.\n" );
			Msg( "    vpc +shaderapi /x360 <-- Creates a Xbox360 .vcproj for the shaderapi.\n" );

			Msg( "\n" );
			Msg( "  Multiple .vcproj generation - Multiple Projects for Games and Platforms:\n" );
			Msg( "    vpc +client /hl2 /tf           <-- Creates ALL the Win32 .vcprojs for the HL2 and TF client.\n" );
			Msg( "    vpc +gamedlls /allgames        <-- Creates ALL the Win32 .vcprojs for client and server for all GAMES.\n" );
			Msg( "    vpc +tools -tier0 /win32 /x360 <-- Creates ALL the Win32 and Xbox360 .vcprojs for the tool projects but not the tier0 project.\n" );

			Msg( "\n" );
			Msg( "  Use +/- to add or remove projects or groups.\n");
			Msg( "  Use /h spew final target build set only (no .vcproj created).\n");
			Msg( "\n" );
			Msg( "  Further details can be found on Valve Internal Wiki on VPC.\n" );

			Msg( "\n--- OPTIONS ---\n" );
			Msg( "[/v]:          Verbose\n" );
			Msg( "[/h]:          Help\n" );
			Msg( "[/?]:          Help\n" );
			Msg( "[/f]:          Force generate .vcproj, otherwise use crc checks\n" );
			Msg( "[/dp]:         Decorate project names with platform\n" );
			Msg( "[/profile]:    Set Reserved $PROFILE=1\n" );
			Msg( "[/retail]:     Set Reserved $RETAIL=1\n" );
			Msg( "[/callcap]:    Set Reserved $CALLCAP=1\n" );
			Msg( "[/cert]:       Set Reserved $CERT=1\n" );
			Msg( "[/memtest]:    Set Reserved $MEMTEST=1\n" );
			Msg( "[/testmode]:   Override output .vcproj file to be named 'test.vcproj'\n" );
			Msg( "[/platforms]:  Spew Platforms\n" );
			Msg( "[/games]:      Spew Games\n" );
			Msg( "[/projects]:   Spew Projects\n" );
			Msg( "[/groups]:     Spew Groups\n" );
			Msg( "[/properties]: Spew VS2005 Properties\n" );
		}
	}

	if ( g_defines.Count() && g_bSpewPlatforms )
	{
		bool bFirstDefine = false;
		for ( i=0; i<g_defines.Count(); i++ )
		{
			if ( g_defines[i].type != DEFINE_PLATFORM )
				continue;
			if ( !bFirstDefine )
			{
				Msg( "\n--- PLATFORMS ---\n" );
				bFirstDefine = true;
			}
			Msg( "%s%s\n", g_defines[i].upperCaseName.String(), g_defines[i].bDefined ? " = 1" : "" );
		}
	}

	if ( g_defines.Count() && g_bSpewGames )
	{
		bool bFirstGame = false;
		for ( i=0; i<g_defines.Count(); i++ )
		{
			if ( g_defines[i].type != DEFINE_GAME )
				continue;
			if ( !bFirstGame )
			{
				Msg( "\n--- GAMES ---\n" );
				bFirstGame = true;
			}
			Msg( "%s%s\n", g_defines[i].upperCaseName.String(), g_defines[i].bDefined ? " = 1" : "" );
		}
	}

	if ( g_projects.Count() && g_bSpewProjects )
	{
		// spew all sorted projects
		Msg( "\n--- PROJECTS ---\n" );
		CUtlRBTree<const char*> sorted( 0, 0, CaselessStringLessThan );
		for ( i=0; i<g_projects.Count(); i++ )
		{
			sorted.Insert( g_projects[i].name.String() );
		}
		for ( i=sorted.FirstInorder(); i != sorted.InvalidIndex(); i=sorted.NextInorder( i ) )
		{
			Msg( "[+/-] %s\n", sorted[i] );
		}
	}

	if ( g_groupTags.Count() && g_bSpewGroups )
	{
		// spew all sorted groups
		Msg( "\n--- GROUPS ---\n" );
		CUtlRBTree<const char*> sorted( 0, 0, CaselessStringLessThan );
		for ( i=0; i<g_groupTags.Count(); i++ )
		{
			if ( !g_groupTags[i].bSameAsProject )
			{
				sorted.Insert( g_groupTags[i].name.String() );
			}
		}
		for ( i=sorted.FirstInorder(); i != sorted.InvalidIndex(); i=sorted.NextInorder( i ) )
		{
			Msg( "[+/-] %s\n", sorted[i] );
		}
	}

	if ( g_bSpewProperties )
	{
		for ( i=0; i<KEYWORD_MAX; i++ )
		{
			VPC_Config_SpewProperties( (configKeyword_e)i );
		}
	}

	if ( g_buildCommands.Count() )
	{
		// spew details about each command
		Msg( "\nUser Build Commands:\n" );
		Msg( "--------------------\n" );
		for ( i=0; i<g_buildCommands.Count(); i++ )
		{
			Msg( "%s\n", g_buildCommands[i].String() );
			groupTagIndex_t groupTagIndex = VPC_Group_FindOrCreateGroupTag( g_buildCommands[i].Get()+1, false );
			if ( groupTagIndex == INVALID_INDEX )
			{
				Msg( "   ??? (Unknown Group)\n" );
			}
			else
			{
				groupTag_t *pGroupTag = &g_groupTags[groupTagIndex];
				for (int j=0; j<pGroupTag->groups.Count(); j++ )
				{
					group_t *pGroup = &g_groups[pGroupTag->groups[j]];
					for ( int k=0; k<pGroup->projects.Count(); k++ )
					{
						Msg( "   %s\n",  g_projects[pGroup->projects[k]].name.String() );
					}
				}
			}
		}
	
		Msg( "\nTarget Projects:\n" );
		Msg( "----------------\n" );
		if ( g_targetProjects.Count() )
		{
			for ( i=0; i<g_targetProjects.Count(); i++ )
			{
				Msg( "%s\n", g_projects[g_targetProjects[i]].name.String() );
			}
		}
		else
		{
			Msg( "Empty Set (no output)\n" );
		}

		Msg( "\nTarget Games:\n" );
		Msg( "-------------\n" );
		bool bHasDefine = false;
		for ( i=0; i<g_defines.Count(); i++ )
		{
			if ( g_defines[i].type != DEFINE_GAME )
				continue;
			if ( g_defines[i].bDefined )
			{
				Msg( "$%s = 1\n", g_defines[i].upperCaseName.String() );
				bHasDefine = true;
			}
		}
		if ( !bHasDefine )
		{
			Msg( "No Game Set!\n" );
		}

		Msg( "\nTarget Platforms:\n" );
		Msg( "-----------------\n" );
		bHasDefine = false;
		for ( i=0; i<g_defines.Count(); i++ )
		{
			if ( g_defines[i].type != DEFINE_PLATFORM )
				continue;
			if ( g_defines[i].bDefined )
			{
				Msg( "$%s = 1\n", g_defines[i].upperCaseName.String() );
				bHasDefine = true;
			}
		}
		if ( !bHasDefine )
		{
			Msg( "No Platform Set!\n" );
		}

		Msg( "\nCustom Defines:\n" );
		Msg( "-----------------\n" );
		bHasDefine = false;
		for ( i=0; i<g_defines.Count(); i++ )
		{
			if ( g_defines[i].type != DEFINE_CUSTOM )
				continue;
			if ( g_defines[i].bDefined )
			{
				Msg( "$%s = 1\n", g_defines[i].upperCaseName.String() );
				bHasDefine = true;
			}
		}
		if ( !bHasDefine )
		{
			Msg( "No Custom Defines Set!\n" );
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_ParseBuildOptions
//
//-----------------------------------------------------------------------------
void VPC_ParseBuildOptions( int argc, char *argv[] )
{
	// parse options
	// prefer +??? or -??? prefix syntax for groups and /??? for options because less confusing for new vpc users
	// for ease we will support -??? prefix syntax for matched options as well
	for ( int i=1; i<argc; i++ )
	{
		const char *pArg = argv[i];

		if ( ( pArg[0] == '-' ) || ( pArg[0] == '/' ) )
		{
			// skip past arg prefix
			const char *pArgName = pArg+1;

			// check options
			if ( !stricmp( pArgName, "h" ) || !stricmp( pArgName, "?" ) || !stricmp( pArgName, "help" ) )
			{
				g_bHelp = true;
				g_bUsageOnly = true;
			}
			else if ( !stricmp( pArgName, "v" ) || !stricmp( pArgName, "verbose" ) )
			{
				g_bVerbose = true;
			}
			else if ( !stricmp( pArgName, "q" ) || !stricmp( pArgName, "quiet" ) )
			{
				g_bVerbose = false;
			}
			else if ( !stricmp( pArgName, "testmode" ) || !stricmp( pArgName, "test" ) )
			{
				g_bTestMode = true;
			}
			else if ( !stricmp( pArgName, "f" ) || !stricmp( pArgName, "force" ) )
			{
				g_bForce = true;
			}
			else if ( !stricmp( pArgName, "dp" ) )
			{
				g_bDecorateProject = true;
			}
			else if ( !stricmp( pArgName, "platforms" ) || !stricmp( pArgName, "plats" ) )
			{
				g_bSpewPlatforms = true;
				g_bUsageOnly = true;
			}
			else if ( !stricmp( pArgName, "games" ) )
			{
				g_bSpewGames = true;
				g_bUsageOnly = true;
			}
			else if ( !stricmp( pArgName, "projects" ) )
			{
				g_bSpewProjects = true;
				g_bUsageOnly = true;
			}
			else if ( !stricmp( pArgName, "groups" ) )
			{
				g_bSpewGroups = true;
				g_bUsageOnly = true;
			}
			else if ( !stricmp( pArgName, "properties" ) )
			{
				g_bSpewProperties = true;
				g_bUsageOnly = true;
			}
			else if ( !stricmp( pArgName, "allgames" ) )
			{
				// shortcut for all games defined
				for ( int j=0; j<g_defines.Count(); j++ )
				{
					if ( g_defines[j].type == DEFINE_GAME )
					{
						g_defines[j].bDefined = true;
					}
				}
			}
			else
			{
				// not a recogined option, try defines
				// find in list of defines
				defineIndex_t defineIndex = VPC_FindOrCreateDefine( pArgName, false, DEFINE_NULL );
				if ( defineIndex == INVALID_INDEX )
				{
					// not a recognized define, add to build commands
					int index = g_buildCommands.AddToTail();
					g_buildCommands[index] = pArg;
				}
				else
				{
					// set as defined
					g_defines[defineIndex].bDefined = true;
				}
			}
		}
		else if ( pArg[0] == '+' )
		{
			// add to build commands
			int index = g_buildCommands.AddToTail();
			g_buildCommands[index] = pArg;
		}
	}
}

//-----------------------------------------------------------------------------
//	Special operating mode of VPC.  Returns TRUE if VPC ran validation tests.
//-----------------------------------------------------------------------------
bool VPC_RunCRCChecks( int argc, char *argv[] )
{
	// crc operation checks for script/vcproj sync
	// called during pre-build step of vcproj that was generated with expected crc(s)
	bool bCRCCheckOnly = false;
	for ( int i=1; i<argc; i++ )
	{
		if ( !stricmp( argv[i], "-crc" ) )
		{
			CRC32_t crc = 0;
			sscanf( argv[i+2], "%x", &crc );

			if ( !VPC_CheckCRC( argv[i+1], crc ) )
			{
				// Unrecoverable, fatal
				VPC_Error( "This VCPROJ is out of sync with its VPC scripts.  Please use VPC to re-generate!" );
			}

			bCRCCheckOnly = true;
		}
	}

	if ( bCRCCheckOnly )
	{
		VPC_Status( false, "Checking CRC: Passes CRC Checks." );
	}

	return bCRCCheckOnly;
}

//-----------------------------------------------------------------------------
//	Restart self from correct location and re-run. Returns FALSE if not applicable,
//	otherwise TRUE if restart occurred.
//-----------------------------------------------------------------------------
bool VPC_Restart( int argc, char *argv[], bool *pIsChild )
{
	// recursive restart guard
	// restart is a hidden internal param, always the last argument
	// presence identifies spawned process
	bool bIsRestart = false;
	if ( !stricmp( argv[argc-1], "/restart" ) )
	{
		bIsRestart = true;
	}
	*pIsChild = bIsRestart;

	char szBinPath[MAX_PATH];
	if ( !CheckBinPath( szBinPath, sizeof( szBinPath ) ) )
	{		
		if ( bIsRestart )
		{
			VPC_Error( "Cyclical Restart: Tell A Programmer!, Aborting." );
		}

		// replicate arguments, add -restart as a recursion guard for the new process
		char *newArgs[128];
		if ( argc >= ARRAYSIZE( newArgs ) - 2 )
		{
			VPC_Error( "Excessive Arguments: Tell A Programmer!, Aborting." ); 
		}
		int i;
		for ( i=0; i<argc; i++ )
		{
			newArgs[i] = argv[i];
		}
		newArgs[i++] = "/restart";
		newArgs[i++] = NULL;

		// restart using synchronous semantic, async semantic causes wierd hang
		int status = spawnv( _P_WAIT, szBinPath, newArgs );
		if ( !status )
		{
			// called process exited normally
			return true;
		}
		else if ( status > 0 )
		{
			// called process exited with error, pass it along
			exit( status );
		}

		// called process could not be started
		VPC_Error( "Restart of '%s' failed\n", szBinPath );
	}

	// process is running from correct location
	return false;
}

void VPC_CheckForXDK()
{
	// quick and dirty early check for 360 XDK ability
	// can only detect simplistic condition, VPC can't validate a perfect XDK/MSDEV installation
	bool bHasXDK = false;
	const char *pXDK = getenv( "XEDK" );
	if ( pXDK && pXDK[0] )
	{
		// look for expected compiler
		char fullPath[MAX_PATH];
		Q_strncpy( fullPath, pXDK, sizeof( fullPath ) );
		V_AppendSlash( fullPath, sizeof( fullPath ) );
		V_strncat( fullPath, "bin\\win32\\cl.exe", sizeof( fullPath ) );
		int fileSize = Sys_FileLength( fullPath, -1, false );
		if ( fileSize > 0 )
		{
			bHasXDK = true;
		}	
	}
	if ( !bHasXDK )
	{
		VPC_Error( "Cannot Build For Xbox 360, XDK is missing or damaged. Remove /x360 from command line." );
	}
}

//-----------------------------------------------------------------------------
//	OutputFunc
//
//	Valve's library printer.
//-----------------------------------------------------------------------------
SpewRetval_t OutputFunc( SpewType_t spewType, char const *pMsg )
{
	printf( pMsg );

	if ( spewType == SPEW_ERROR )
	{
		return SPEW_ABORT;
	}
	return ( spewType == SPEW_ASSERT ) ? SPEW_DEBUGGER : SPEW_CONTINUE; 
}

//-----------------------------------------------------------------------------
//	main
//
//-----------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
	const char	*pScriptName = NULL;
	const char	*pScriptNameVCProj = NULL;
	int			i;
	int			k;
	int			nScript;
	int			nGame;
	int			nPlatform;

	// set the valve library printer
	SpewOutputFunc( OutputFunc );

	// vpc operates tersely by preferred company opinion
	// verbosity necessary for debugging 
	g_bVerbose = VPC_CheckCommandLine( argc, argv, "v" ) != 0 || VPC_CheckCommandLine( argc, argv, "verbose" ) != 0;

	// crc checking needs to be fast, has no startup script requirements, run first
	if ( VPC_RunCRCChecks( argc, argv ) )
	{
		// vpc operating only in crc check mode
		return 0;
	}

	// vpc may have been run from wrong location, restart self
	bool bIsRestart = false;
	if ( VPC_Restart( argc, argv, &bIsRestart ) )
	{
		// successfully ran under restrt condition, all done
		return 0;
	}
	if ( bIsRestart )
	{
		// this process is the restart child, cull the internal private restart guard option
		// otherwise confused as a build option
		argc--;
	}

	Msg( "\nVPC - Valve Project Creator For Visual Studio 2005 (Build: %s %s)\n", __DATE__, __TIME__ );
	Msg( "(C) Copyright 1996-2007, Valve Corporation, All rights reserved.\n" );

	// possible extensions determine operation mode beyond expected normal user case
	bool bScriptIsVGC = false;
	bool bScriptIsVPC = false;
	bool bScriptIsVCProj = false;
	bool bHasBuildCommand = false;
	for ( i=1; i<argc; i++ )
	{
		const char *pArg = argv[i];
		if ( V_stristr( pArg, ".vgc" ) )
		{
			// caller explicitly providing group
			pScriptName = pArg;
			bScriptIsVGC = true;
			bHasBuildCommand = true;
			break;
		}
		else if ( V_stristr( pArg, ".vpc" ) )
		{
			// caller is using a local vpc, i.e. one that is not hooked into the groups
			pScriptName = pArg;
			bScriptIsVPC = true;
			bHasBuildCommand = true;
			break;
		}
		else if ( V_stristr( pArg, ".vcproj" ) )
		{
			// caller wants to re-gen the vcproj, this is commonly used by MSDEV to re-gen
			pScriptNameVCProj = pArg;
			bScriptIsVCProj = true;
			bHasBuildCommand = true;
			break;
		}
	}

	for ( i=1; i<argc; i++ )
	{
		if ( argv[i][0] == '-' || argv[i][0] == '+' )
		{
			bHasBuildCommand = true;
			break;
		}
	}
	
	if ( bScriptIsVPC )
	{
		// caller is specifying an explicit VPC, i.e. not a project from the default group
		// create a temporary file that mimics a VGC
		char *pTmpName = tmpnam( NULL );
		if ( !pTmpName || !pTmpName[0] )
		{
			VPC_Error( "Could not generate temp name. Tell a Programmer.\n" );
		}
		if ( pTmpName[0] == '\\' )
		{
			pTmpName++;
		}
		strcpy( g_szTempGroupScript, pTmpName );
		strcat( g_szTempGroupScript, "vgc" );
		strcat( g_szTempGroupScript, ".tmp" );

		// build the script
		FILE *fp = fopen( g_szTempGroupScript, "w+t" );
		if ( !fp )
		{
			VPC_Error( "Could not open temp file '%s'. Tell a Programmer.\n", g_szTempGroupScript );
		}

		char projectName[MAX_PATH];
		V_StripExtension( pScriptName, projectName, sizeof( projectName ) );
		fprintf( fp, "$Project \"%s\"\n", projectName );
		fprintf( fp, "{\n" );
		fprintf( fp, "\"%s\"\n", pScriptName );
		fprintf( fp, "}\n" );
		fclose( fp );

		pScriptName = g_szTempGroupScript;
		bScriptIsVPC = false;
		bScriptIsVGC = true;

		// fake a build command
		static char buildCommand[MAX_PATH];
		V_snprintf( buildCommand, sizeof( buildCommand ), "+%s", projectName );
		int index = g_buildCommands.AddToTail();
		g_buildCommands[index] = buildCommand;
	}

	if ( !bScriptIsVGC )
	{
		// using the default group requires the current directory to be slammed to the expected vpc_scripts
		SetDefaultSourcePath();

		// no script, use default group
		pScriptName = "vpc_scripts\\default.vgc";
		bScriptIsVGC = true;
	}

	GetCurrentDirectory( sizeof( g_szStartDirectory ), g_szStartDirectory );

	if ( !bHasBuildCommand )
	{
		// spew usage
		g_bUsageOnly = true;
	}

	// setup default platform defines
	VPC_FindOrCreateDefine( "WIN32", true, DEFINE_PLATFORM );
	VPC_FindOrCreateDefine( "WIN64", true, DEFINE_PLATFORM );
	VPC_FindOrCreateDefine( "LINUX", true, DEFINE_PLATFORM );
	VPC_FindOrCreateDefine( "X360", true, DEFINE_PLATFORM );
	VPC_FindOrCreateDefine( "PS3", true, DEFINE_PLATFORM );

	// setup default custom defines
	VPC_FindOrCreateDefine( "PROFILE", true, DEFINE_CUSTOM );		
	VPC_FindOrCreateDefine( "RETAIL", true, DEFINE_CUSTOM );
	VPC_FindOrCreateDefine( "CALLCAP", true, DEFINE_CUSTOM );
	VPC_FindOrCreateDefine( "CERT", true, DEFINE_CUSTOM );
	VPC_FindOrCreateDefine( "MEMTEST", true, DEFINE_CUSTOM );
	
	// parse and build tables from group script that options will reference
	if ( bScriptIsVGC )
	{
		VPC_ParseGroupScript( pScriptName );
	}

	if ( bScriptIsVCProj )
	{
		// caller is specifying the output vcproj, i.e. via tool shortcut from within MSDEV to re-gen
		// use the vpc standardized output vcproj name to determine re-gen parameters
		// mod and platform will be seperated by '_' after the project name
		// resolve to correct project, best will be longest match, due to project names like foo_? and foo_bar_?
		char szProject[MAX_PATH];
		unsigned int bestLen = 0;
		for ( i=0; i<g_projects.Count(); i++ )
		{
			if ( V_stristr( pScriptNameVCProj, g_projects[i].name.String() ) )
			{
				if ( bestLen < strlen( g_projects[i].name.String() ) )
				{
					bestLen = strlen( g_projects[i].name.String() );
					strcpy( szProject, g_projects[i].name.String() );
				}
			}
		}
		if ( bestLen == 0 )
		{
			VPC_Error( "Could not resolve '%s' to any known projects", pScriptNameVCProj );
		}

		// skip past known project
		char szBuffer[MAX_PATH];
		V_StripExtension( pScriptNameVCProj + strlen( szProject ), szBuffer, sizeof( szBuffer ) );

		// each token is seperated by '_'
		int numTokens = 0;
		char *pToken = szBuffer;
		char *pStart = pToken;	
		char szTokens[2][MAX_PATH];
		while ( numTokens < 2 )
		{
			if ( pStart[0] == '_' )
			{
				pStart++;
				pToken = strchr( pStart, '_' );
				if ( !pToken )
				{
					strcpy( szTokens[numTokens++], pStart );
					break;
				}
				else
				{
					strncpy( szTokens[numTokens], pStart, pToken-pStart );
					szTokens[numTokens][pToken-pStart] = '\0';
					numTokens++;
					pStart = pToken;
				}
			}
			else
			{
				break;
			}	
		}

		// re-build a commandline
		int localArgc = 0;
		char *localArgv[16];
		char argBuffers[16][MAX_PATH];
		for ( i=0; i<ARRAYSIZE( localArgv ); i++ )
		{
			localArgv[i] = argBuffers[i];
		}
		strcpy( localArgv[localArgc++], "vpc.exe" );
		sprintf( localArgv[localArgc++], "+%s", szProject );
		for ( i=0; i<numTokens; i++ )
		{
			sprintf( localArgv[localArgc++], "/%s", szTokens[i] );
		}
		VPC_ParseBuildOptions( localArgc, localArgv );
	}
	
	VPC_ParseBuildOptions( argc, argv );

	// no user specified platform defined, defaults to win32
	int nUserPlatformDefined = 0;
	for ( i=0; i<g_defines.Count(); i++ )
	{
		if ( g_defines[i].type == DEFINE_PLATFORM && g_defines[i].bDefined )
		{
			nUserPlatformDefined++;
		}
	}
	if ( !nUserPlatformDefined )
	{
		defineIndex_t defineIndex = VPC_FindOrCreateDefine( "WIN32", false, DEFINE_NULL );
		g_defines[defineIndex].bDefined = true;
		nUserPlatformDefined++;
	}

	defineIndex_t defineIndex = VPC_FindOrCreateDefine( "X360", false, DEFINE_NULL );
	if ( g_defines[defineIndex].bDefined )
	{
		VPC_CheckForXDK();
	}

	// determine user specified game
	bool bGameDefined = false;
	for ( i=0; i<g_defines.Count(); i++ )
	{
		if ( g_defines[i].type == DEFINE_GAME && g_defines[i].bDefined )
		{
			bGameDefined = true;
			break;
		}
	}

	// filter user's build commands
	// generate list of build targets
	VPC_GenerateBuildSet();

	if ( g_bUsageOnly )
	{
		// spew only
		VPC_Usage();
		return 0;
	}

	bool bProjectSkipped = false;
	if ( !g_targetProjects.Count() )
	{
		VPC_Error( "No recognized project(s) to build.  Use /h or /projects or /groups to spew more info." );
	}
	else
	{		
		for ( int nProject=0; nProject<g_targetProjects.Count(); nProject++ )
		{
			// evaluate conditional
			project_t *pProject = &g_projects[g_targetProjects[nProject]];
		
			// track the internal project name, unaffected by user name mangling
			strcpy( g_szProjectName, pProject->name.String() );

			for ( nScript=0; nScript<pProject->scripts.Count(); nScript++ )
			{
				// occurrence of defined type dictates auto-iteration through defines
				bool bHasPlatformCondition = VPC_Group_ConditionHasDefinedType( pProject->scripts[nScript].condition.String(), DEFINE_PLATFORM );
				bool bHasGameCondition = VPC_Group_ConditionHasDefinedType( pProject->scripts[nScript].condition.String(), DEFINE_GAME );

				// at least one platform is mandatory
				nPlatform = 0;
				if ( !bHasPlatformCondition )
				{
					if ( nUserPlatformDefined > 1 )
					{
						// this project has no information to auto-iterate through defined platforms
						VPC_Error( "Multiple platforms specified to build project '%s' that has no multiple platform awareness.", pProject->name.String() );
					}
					else
					{
						// find single specified platform
						for ( i=0; i<g_defines.Count(); i++ )
						{
							if ( g_defines[i].type == DEFINE_PLATFORM && g_defines[i].bDefined )
							{
								nPlatform = i;
								break;
							}
						}
					}
				}

				// each vpc script is written with paths relative to their base
				// force each script needs to start relative to their script location
				// this allows vpc to be invoked anywhere vpc, but the groups resolve their projects correctly
				char szScriptPath[MAX_PATH];
				sprintf( szScriptPath, "%s\\%s", g_szStartDirectory, pProject->scripts[nScript] );
				V_StripFilename( szScriptPath );
				SetCurrentDirectory( szScriptPath );
				Sys_StripPath( pProject->scripts[nScript].name.String(), szScriptPath );

				switch ( bHasGameCondition*2+bHasPlatformCondition )
				{
				case 0:
					// script has no conditions
					// set mandatory platform
					for ( k=0; k<g_defines.Count(); k++ )
					{
						g_defines[k].bSet = false;
					}
					g_defines[nPlatform].bSet = true;

					sprintf( g_szOutputFilename, "%s.vcproj", g_bTestMode ? "test" : pProject->name.String() );

					if ( g_bForce || !VPC_IsProjectCurrent( g_szOutputFilename ) )
					{
						VPC_ParseProjectScript( szScriptPath, 0 );
					}
					else
					{
						bProjectSkipped = true;
					}
					break;

				case 1:
					// iterate platform conditions only
					for ( nPlatform=0; nPlatform<g_defines.Count(); nPlatform++ )
					{
						if ( g_defines[nPlatform].type != DEFINE_PLATFORM || !g_defines[nPlatform].bDefined )
							continue;

						// set unique platform condition
						for ( k=0; k<g_defines.Count(); k++ )
						{
							g_defines[k].bSet = false;
						}
						g_defines[nPlatform].bSet = true;

						if ( VPC_EvaluateCondition( pProject->scripts[nScript].condition.String() ) )
						{
							strcpy( g_szOutputFilename, g_bTestMode ? "test" : pProject->name.String() );
							if ( stricmp( g_defines[nPlatform].name.String(), "win32" ) )
							{
								// non-pc platforms get decorated
								strcat( g_szOutputFilename, "_" );
								strcat( g_szOutputFilename, g_defines[nPlatform].name.String() );
							}
							strcat( g_szOutputFilename, ".vcproj" );

							if ( g_bForce || !VPC_IsProjectCurrent( g_szOutputFilename ) )
							{
								VPC_ParseProjectScript( szScriptPath, 0 );
							}
							else
							{
								bProjectSkipped = true;
							}
						}
					}
					break;

				case 2:
					// iterate game conditions only
					for ( nGame=0; nGame<g_defines.Count(); nGame++ )
					{
						if ( g_defines[nGame].type != DEFINE_GAME || !g_defines[nGame].bDefined )
							continue;

						// set unique game condition
						for ( k=0; k<g_defines.Count(); k++ )
						{
							g_defines[k].bSet = false;
						}
						g_defines[nGame].bSet = true;
						g_defines[nPlatform].bSet = true;

						if ( VPC_EvaluateCondition( pProject->scripts[nScript].condition.String() ) )
						{
							sprintf( g_szOutputFilename, "%s_%s.vcproj", g_bTestMode ? "test" : pProject->name.String(), g_defines[nGame].name.String() );

							if ( g_bForce || !VPC_IsProjectCurrent( g_szOutputFilename ) )
							{
								VPC_ParseProjectScript( szScriptPath, 0 );
							}
							else
							{
								bProjectSkipped = true;
							}
						}
					}
					break;

				case 3:
					// iterate games and platforms
					for ( nGame=0; nGame<g_defines.Count(); nGame++ )
					{
						if ( g_defines[nGame].type != DEFINE_GAME || !g_defines[nGame].bDefined )
							continue;

						// iterate each platform
						for ( nPlatform=0; nPlatform<g_defines.Count(); nPlatform++ )
						{
							if ( g_defines[nPlatform].type != DEFINE_PLATFORM || !g_defines[nPlatform].bDefined )
								continue;

							// set target platform and game condition
							for ( k=0; k<g_defines.Count(); k++ )
							{
								g_defines[k].bSet = false;
							}
							g_defines[nGame].bSet = true;
							g_defines[nPlatform].bSet = true;

							if ( VPC_EvaluateCondition( pProject->scripts[nScript].condition.String() ) )
							{
								sprintf( g_szOutputFilename, "%s_%s", g_bTestMode ? "test" : pProject->name.String(), g_defines[nGame].name.String() );
								if ( stricmp( g_defines[nPlatform].name.String(), "win32" ) )
								{
									// non-pc platforms get decorated
									strcat( g_szOutputFilename, "_" );
									strcat( g_szOutputFilename, g_defines[nPlatform].name.String() );
								}
								strcat( g_szOutputFilename, ".vcproj" );

								if ( g_bForce || !VPC_IsProjectCurrent( g_szOutputFilename ) )
								{
									VPC_ParseProjectScript( szScriptPath, 0 );
								}
								else
								{
									bProjectSkipped = true;
								}
							}
						}
					}
					break;

				default:
					assert( 0 );
				}
			}
		}
	}

	if ( !g_bGeneratedProject && !bProjectSkipped )
	{
		// Catch user attention to notify lack of any output
		// Novice users would not be aware of expected conditionals
		Msg( "\n" );
		Msg( "--------------------------\n" );
		Msg( "!!!No Project Generated!!!\n" );
		Msg( "--------------------------\n" );
		Msg( "\n" );
		Msg( "Possibly missing game, platform, or other conditional expected by script\n" );
		Msg( "Use /h verify desired target build set.\n" );
	}
	
	if ( g_szTempGroupScript[0] )
	{
		// delete temp work file
		unlink( g_szTempGroupScript );
	}

	return 0;
}