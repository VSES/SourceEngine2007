//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

// Unitl I figure out what the real issue is, this stays ON
// Cannot seem to destruct the COM Object more than once
#define VC8_CORRUPTION_BUG

CComPtr<VCProjectEngine>	g_spEngine;
CComQIPtr<VCProject>		g_spProject;
CComQIPtr<VCFilter>			g_spFilter;
CComQIPtr<VCFile>			g_spFile;
CUtlVector<scriptList_t>	g_scriptList;
CUtlVector<macro_t>			g_macros;

//-----------------------------------------------------------------------------
//	VPC_FindOrCreateMacro
//
//-----------------------------------------------------------------------------
macro_t *VPC_FindOrCreateMacro( const char *pName, bool bCreate, const char *pValue )
{
	for (int i=0; i<g_macros.Count(); i++)
	{
		if ( !stricmp( pName, g_macros[i].name.String() ) )
		{
			if ( pValue && stricmp( pValue, g_macros[i].value.String() ) )
			{
				// update
				g_macros[i].value = pValue;			
			}

			return &g_macros[i];
		}
	}

	if ( !bCreate )
		return NULL;

	int index = g_macros.AddToTail();
	memset( &g_macros[index], 0, sizeof( macro_t ) );

	g_macros[index].name  = pName;
	g_macros[index].value = pValue;

	return &g_macros[index];
}

//-----------------------------------------------------------------------------
//	VPC_CreateDefaultProject
//
//-----------------------------------------------------------------------------
void VPC_CreateDefaultProject()
{
	HRESULT		hr;
	char		szProjectName[MAX_PATH];

	if ( !g_spProject )
	{
		strcpy( szProjectName, "UNNAMED" );

		// create
		CComPtr<IDispatch> spDispProj = NULL;
		CComBSTR bstr = szProjectName; 
		hr = g_spEngine->CreateProject( bstr, &spDispProj );
		if ( FAILED( hr ) || !spDispProj )
		{
			VPC_Error( "Cannot create the specified project %s", szProjectName );
		}

		g_spProject = spDispProj;
		if ( !g_spProject )
		{
			VPC_Error( "Cannot query VCProject interface." );
		}

		bstr = g_szOutputFilename; 
		hr = g_spProject->put_ProjectFile( bstr );
		if ( FAILED( hr ) )
		{
			VPC_Error( "Cannot set the project output filename to %s", g_szOutputFilename );
		}

		// create the required platform
		const char	*pPlatformName = NULL;
		defineIndex_t defineIndex = VPC_FindOrCreateDefine( "WIN32", false, DEFINE_NULL );
		if ( defineIndex != INVALID_INDEX && g_defines[defineIndex].bSet )
		{
			pPlatformName = "Win32";
		}
		else
		{
			defineIndex = VPC_FindOrCreateDefine( "X360", false, DEFINE_NULL );
			if ( defineIndex != INVALID_INDEX && g_defines[defineIndex].bSet )
			{
				pPlatformName = "Xbox 360";
			}
		}
		if ( !pPlatformName )
		{
			VPC_Error( "No target platform specified" );
		}

		// create reserved $PLATFORM - tracks the active platform
		VPC_FindOrCreateMacro( "PLATFORM", true, pPlatformName );

		bstr = pPlatformName;
		hr = g_spProject->AddPlatform( bstr );
		if ( FAILED( hr ) )
		{
			VPC_Error( "Cannot add the platform %s to %s", pPlatformName, g_szOutputFilename );
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_AddFile
//
//-----------------------------------------------------------------------------
void VPC_Keyword_AddFile( int depth )
{
	char							*pToken;
	HRESULT							hr;
	CComPtr<IDispatch>				spDispFile;
	CComBSTR						bstr;
	bool							bAllowNextLine = false;
	int								i;
	char							szFilename[MAX_PATH];
	char							*pScriptSave;
	char							activeToken[MAX_SYSTOKENCHARS];
	bool							bResult = true;
	char							*pNextToken;
	CUtlVector<CUtlString>			files;

	while ( 1 )
	{
		pScriptSave = g_pScriptData;

		pToken = Sys_GetToken( &g_pScriptData, bAllowNextLine, g_pScriptLine );
		if ( !pToken || !pToken[0] )
			VPC_SyntaxError();

		strcpy( activeToken, pToken );
		pToken = activeToken;

		bResult = true;
		pNextToken = Sys_PeekToken( g_pScriptData, false );
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

		ResolveMacrosInString( pToken, szFilename, sizeof( szFilename ) );

		if ( !stricmp( pToken, "\\" ) )
		{
			bAllowNextLine = true;
			continue;
		}
		else
		{
			bAllowNextLine = false;
		}

		CUtlString string = szFilename;
		files.AddToTail( string );

		// check for another optional file
		pToken = Sys_PeekToken( g_pScriptData, bAllowNextLine );
		if ( !pToken || !pToken[0] )
			break;
	}

	if ( !bResult )
	{
		// files have been conditionally removed
		files.Purge();
	}

	// check for optional section
	bool bHasSection = false;
	pToken = Sys_PeekToken( g_pScriptData, true );
	if ( pToken && pToken[0] && !stricmp( pToken, "{" ) )
	{
		bHasSection = true;
	}

	if ( !files.Count() && bHasSection )
	{
		// optional section has been conditionally removed
		Sys_SkipBracedSection( &g_pScriptData, g_pScriptLine );
		return;
	}

	if ( bHasSection )
	{
		// found optional section, parse opening brace
		pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
		if ( !pToken || !pToken[0] || stricmp( pToken, "{" ) )
			VPC_SyntaxError();
	}

	// save parser state
	char *saveData = g_pScriptData;
	int saveLine = *g_pScriptLine;

	for ( i=0; i<files.Count(); i++ )
	{
		spDispFile = NULL;
		bstr = files[i].String(); 
		if ( depth == 0 )
		{
			// add at root
			hr = g_spProject->AddFile( bstr, &spDispFile );
		}
		else
		{
			// add at subfolder
			hr = g_spFilter->AddFile( bstr, &spDispFile );
		}
		if ( FAILED( hr ) || !spDispFile )
		{
			// resolve failure
			if ( hr == VCPROJ_E_FILE_EXISTS )
			{
				// file already exists, not an error
				VPC_Warning( "File %s already exists in project", files[i].String() );

				// find the matching object
				CComPtr<IDispatch> spDispFiles = NULL;
				if ( depth == 0 )
				{
					// at root
					hr = g_spProject->get_Files( &spDispFiles );
				}
				else
				{
					// at subfolder
					hr = g_spFilter->get_Files( &spDispFiles );
				}
				if ( FAILED( hr ) || !spDispFiles )
				{
					VPC_Error( "Cannot get list of files" );
				}

				// enumerate
				CComQIPtr<IVCCollection> spFiles = spDispFiles;
				CComVariant ccom = files[i].String();
				spDispFile = NULL;
				hr = spFiles->Item( ccom, &spDispFile );
				if ( FAILED( hr ) )
				{
					VPC_Error( "Cannot enumurate file %s", files[i].String() );
				}
			}
			else
			{
				VPC_Error( "Could not add file %s to project", files[i].String() );
			}
		}

		g_spFile = spDispFile;

		// apply optional section to each file
		if ( bHasSection && g_spFile )
		{
			// restore parser state
			g_pScriptData  = saveData;
			*g_pScriptLine = saveLine;
			while ( 1 )
			{
				pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
				if ( !pToken || !pToken[0] )
					break;

				if ( !stricmp( pToken, "}" ) )
				{
					// end of section
					break;
				}
				if ( !stricmp( pToken, "$configuration" ) )
				{
					VPC_Keyword_FileConfiguration();
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_RemoveFile
//
//-----------------------------------------------------------------------------
void VPC_Keyword_RemoveFile( int depth )
{
	char							*pToken;
	HRESULT							hr;
	CComPtr<IDispatch>				spDispFile;
	CComBSTR						bstr;
	bool							bAllowNextLine = false;
	int								i;
	char							szFilename[MAX_PATH];
	char							*pScriptSave;
	char							activeToken[MAX_SYSTOKENCHARS];
	bool							bResult = true;
	char							*pNextToken;
	CUtlVector<CUtlString>			files;

	while ( 1 )
	{
		pScriptSave = g_pScriptData;

		pToken = Sys_GetToken( &g_pScriptData, bAllowNextLine, g_pScriptLine );
		if ( !pToken || !pToken[0] )
			VPC_SyntaxError();

		strcpy( activeToken, pToken );
		pToken = activeToken;

		bResult = true;
		pNextToken = Sys_PeekToken( g_pScriptData, false );
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

		ResolveMacrosInString( pToken, szFilename, sizeof( szFilename ) );

		if ( !stricmp( pToken, "\\" ) )
		{
			bAllowNextLine = true;
			continue;
		}
		else
		{
			bAllowNextLine = false;
		}

		CUtlString string = szFilename;
		files.AddToTail( string );

		// check for another optional file
		pToken = Sys_PeekToken( g_pScriptData, bAllowNextLine );
		if ( !pToken || !pToken[0] )
			break;
	}

	if ( !bResult )
	{
		// files have been conditionally removed
		files.Purge();
	}

	for ( i=0; i<files.Count(); i++ )
	{
		// find the matching object
		CComPtr<IDispatch> spDispFiles = NULL;
		if ( depth == 0 )
		{
			// at root
			hr = g_spProject->get_Files( &spDispFiles );
		}
		else
		{
			// at subfolder
			hr = g_spFilter->get_Files( &spDispFiles );
		}
		if ( FAILED( hr ) || !spDispFiles )
		{
			VPC_Error( "Cannot get list of files" );
		}

		// enumerate
		CComQIPtr<IVCCollection> spFiles = spDispFiles;
		CComVariant ccom = files[i].String();
		spDispFile = NULL;
		hr = spFiles->Item( ccom, &spDispFile );
		if ( FAILED( hr ) || !spDispFile )
		{
			if ( !FAILED( hr ) && !spDispFile )
			{
				VPC_Warning( "File %s does not exist in project", szFilename );
				return;
			}
			else
			{
				VPC_Error( "Cannot enumurate file" );
			}
		}

		if ( depth == 0 )
		{
			// remove from root
			hr = g_spProject->RemoveFile( spDispFile );
		}
		else
		{
			// remove at subfolder
			hr = g_spFilter->RemoveFile( spDispFile );
		}
		if ( FAILED( hr ) )
		{
			VPC_Error( "Could not remove file %s from project", szFilename );
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_Folder
//
//-----------------------------------------------------------------------------
void VPC_Keyword_Folder( int depth )
{
	char				*pToken;
	char				folderName[MAX_PATH];
	HRESULT				hr;
	CComPtr<IDispatch>	spDispProj;

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, folderName, sizeof( folderName ) ) )
	{
		Sys_SkipBracedSection( &g_pScriptData, g_pScriptLine );
		return;		
	}

	spDispProj = NULL;
	CComBSTR bstr = folderName; 
	if ( depth == 0 )
	{
		// add to root
		hr = g_spProject->AddFilter( bstr, &spDispProj );
	}
	else
	{
		// add as subfolder
		hr = g_spFilter->AddFilter( bstr, &spDispProj );
	}
	if ( FAILED( hr ) || !spDispProj )
	{
		// resolve failure
		if ( hr == VCPROJ_E_FILTER_EXISTS )
		{
			// folder already exists, not an error
			// find the matching object
			CComPtr<IDispatch> spDispFilters = NULL;
			if ( depth == 0 )
			{
				// at root
				hr = g_spProject->get_Filters( &spDispFilters );
			}
			else
			{
				// at subfolder
				hr = g_spFilter->get_Filters( &spDispFilters );
			}
			if ( FAILED( hr ) || !spDispFilters )
			{
				VPC_Error( "Cannot get list of folders" );
			}

			// enumerate
			CComQIPtr<IVCCollection> spFilters = spDispFilters;
			CComVariant ccom = folderName;
			hr = spFilters->Item( ccom, &spDispProj );
			if ( FAILED( hr ) || !spDispProj )
			{
				VPC_Error( "Cannot enumrate folder" );
			}
		}
		else
		{
			VPC_Error( "Cannot add folder %s", folderName );
		}
	}
	g_spFilter = spDispProj;

	pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
	if ( !pToken || !pToken[0] || stricmp( pToken, "{" ) )
		VPC_SyntaxError();

	depth++;
	while ( 1 )
	{
		pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
		if ( !pToken || !pToken[0] )
			break;

		if ( !stricmp( pToken, "}" ) )
		{
			// pop
			break;
		}
		else if ( !stricmp( pToken, "$file" ) )
		{
			// add file
			VPC_Keyword_AddFile( depth );
		}
		else if ( !stricmp( pToken, "-$file" ) )
		{
			// remove file
			VPC_Keyword_RemoveFile( depth );
		}
		else if ( !stricmp( pToken, "$folder" ) )
		{
			// descend into subdirectory
			VPC_Keyword_Folder( depth );

			// restore
			g_spFilter = spDispProj;
		}
		else
		{
			VPC_SyntaxError();
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_Shaders
//
//-----------------------------------------------------------------------------
void VPC_Keyword_Shaders( int depth )
{
	char						*pToken;
	char						shadersName[MAX_PATH];
	CUtlBuffer					vpcBuffer;
	CUtlVector< CUtlString >	fxcList;
	CUtlVector< CUtlString >	vshList;
	CUtlVector< CUtlString >	pshList;
	CUtlVector< CUtlString >	otherList;
	int							i;
	char						*pScriptName;
	char						*pScriptData;
	int							*pScriptLine;
	bool						bIgnoreRedundancyWarning;
	char						*pScriptBuffer;
	int							scriptLine;						

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, shadersName, sizeof( shadersName ) ) )
	{
		return;		
	}

	int scriptLen = Sys_LoadFile( shadersName, (void**)&pScriptBuffer, true );
	if ( scriptLen <= 0 )
	{
		VPC_Error( "Cannot open shader script %s", shadersName );
	}

	// parse the shader list file into types (fxc,vsh,psh)
	scriptLine = 1;
	while ( 1 )
	{
		pToken = Sys_GetToken( &pScriptBuffer, true, &scriptLine );
		if ( !pToken || !pToken[0] )
		{
			// end of file
			break;
		}

		if ( V_stristr( pToken, ".fxc" ) )
		{
			fxcList.AddToTail( pToken );
		}
		else if ( V_stristr( pToken, ".vsh" ) )
		{
			vshList.AddToTail( pToken );
		}
		else if ( V_stristr( pToken, ".psh" ) )
		{
			pshList.AddToTail( pToken );
		}
		else
		{
			otherList.AddToTail( pToken );
		}
	}

	if ( !fxcList.Count() && !vshList.Count() && !pshList.Count() && !otherList.Count() )
	{
		VPC_Warning( "No shaders found in %s", shadersName );
		return;
	}

	// generate a vpc compatible file to generate the shader file hierarchy
	vpcBuffer.SetBufferType( true, true );
	vpcBuffer.Printf( "$Folder \"Shader Source\" \n" );
	vpcBuffer.Printf( "{\n" );

	// add the shader file as a convienence
	vpcBuffer.Printf( "$file \"%s\"\n", shadersName );
	vpcBuffer.Printf( "{\n" );
	vpcBuffer.Printf( "$Configuration\n" );
	vpcBuffer.Printf( "{\n" );
	vpcBuffer.Printf( "$ExcludedFromBuild \"Yes\"\n" );
	vpcBuffer.Printf( "}\n" );
	vpcBuffer.Printf( "}\n" );

	// fxc files
	if ( fxcList.Count() )
	{
		vpcBuffer.Printf( "$Folder \"fxc\" \n" );
		vpcBuffer.Printf( "{\n" );
		for ( i=0; i<fxcList.Count(); i++ )
		{
			vpcBuffer.Printf( "$file \"%s\"\n", fxcList[i].String() );
			vpcBuffer.Printf( "{\n" );
			vpcBuffer.Printf( "$Configuration\n" );
			vpcBuffer.Printf( "{\n" );
			vpcBuffer.Printf( "$ExcludedFromBuild \"Yes\"\n" );
			vpcBuffer.Printf( "}\n" );
			vpcBuffer.Printf( "}\n" );
		}
		vpcBuffer.Printf( "}\n" );
	}

	// vsh files
	if ( vshList.Count() )
	{
		vpcBuffer.Printf( "$Folder \"vsh\" \n" );
		vpcBuffer.Printf( "{\n" );
		for ( i=0; i<vshList.Count(); i++ )
		{
			vpcBuffer.Printf( "$file \"%s\"\n", vshList[i].String() );
			vpcBuffer.Printf( "{\n" );
			vpcBuffer.Printf( "$Configuration\n" );
			vpcBuffer.Printf( "{\n" );
			vpcBuffer.Printf( "$ExcludedFromBuild \"Yes\"\n" );
			vpcBuffer.Printf( "}\n" );
			vpcBuffer.Printf( "}\n" );
		}
		vpcBuffer.Printf( "}\n" );
	}

	// psh files
	if ( pshList.Count() )
	{
		vpcBuffer.Printf( "$Folder \"psh\" \n" );
		vpcBuffer.Printf( "{\n" );
		for ( i=0; i<pshList.Count(); i++ )
		{
			vpcBuffer.Printf( "$file \"%s\"\n", pshList[i].String() );
			vpcBuffer.Printf( "{\n" );
			vpcBuffer.Printf( "$Configuration\n" );
			vpcBuffer.Printf( "{\n" );
			vpcBuffer.Printf( "$ExcludedFromBuild \"Yes\"\n" );
			vpcBuffer.Printf( "}\n" );
			vpcBuffer.Printf( "}\n" );
		}
		vpcBuffer.Printf( "}\n" );
	}

	// other files
	if ( otherList.Count() )
	{
		// psh files
		vpcBuffer.Printf( "$Folder \"other\" \n" );
		vpcBuffer.Printf( "{\n" );
		for ( i=0; i<otherList.Count(); i++ )
		{
			vpcBuffer.Printf( "$file \"%s\"\n", otherList[i].String() );
			vpcBuffer.Printf( "{\n" );
			vpcBuffer.Printf( "$Configuration\n" );
			vpcBuffer.Printf( "{\n" );
			vpcBuffer.Printf( "$ExcludedFromBuild \"Yes\"\n" );
			vpcBuffer.Printf( "}\n" );
			vpcBuffer.Printf( "}\n" );
		}
		vpcBuffer.Printf( "}\n" );
	}

	// end of shader folder
	vpcBuffer.Printf( "}\n" );

	// save parser
	pScriptName = g_pScriptName;
	pScriptData = g_pScriptData;
	pScriptLine = g_pScriptLine;
	bIgnoreRedundancyWarning = g_bIgnoreRedundancyWarning;

	g_pScriptName = "Internal List";
	g_pScriptData = (char*)vpcBuffer.Base();
	g_pScriptLine = &scriptLine;
	g_bIgnoreRedundancyWarning = true;
	scriptLine = 1;

	pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
	if ( pToken && pToken[0] && !stricmp( pToken, "$folder" ) )
	{
		VPC_Keyword_Folder( depth );
	}

	// restore parser
	g_pScriptName = pScriptName;
	g_pScriptData = pScriptData;
	g_pScriptLine = pScriptLine;
	g_bIgnoreRedundancyWarning = bIgnoreRedundancyWarning;
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_Macro
//
//-----------------------------------------------------------------------------
void VPC_Keyword_Macro( void )
{
	char	*pToken;
	char	macro[MAX_SYSTOKENCHARS];
	char	value[MAX_SYSTOKENCHARS];

	pToken = Sys_GetToken( &g_pScriptData, false, g_pScriptLine );
	if ( !pToken || !pToken[0] )
		VPC_SyntaxError();
	strcpy( macro, pToken );
	
	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, value, sizeof( value ) ) )
	{
		return;
	}

	VPC_FindOrCreateMacro( macro, true, value );
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_MacroRequired
//
//	$MacroRequired <Macro> [DefaultValue] [Condition]
//
//	Forces a script to error if a macro that it depends on was not set.
//	The Default will be used if the macro was not defined, otherwise error.
//	This is to allow a required macro in a base script to have a concept
//	of a default initialization value.
//-----------------------------------------------------------------------------
void VPC_Keyword_MacroRequired( void )
{
	char	macroName[MAX_SYSTOKENCHARS];
	char	macroDefaultValue[MAX_SYSTOKENCHARS];
	char	*pToken;

	macroDefaultValue[0] = '\0';
	
	pToken = Sys_GetToken( &g_pScriptData, false, g_pScriptLine );
	if ( !pToken || !pToken[0] )
	{
		VPC_SyntaxError();
	}
	strcpy( macroName, pToken );

	// optional default macro value or conditional
	pToken = Sys_PeekToken( g_pScriptData, false );
	if ( pToken && pToken[0] )
	{
		if ( pToken[0] == '[' )
		{
			// evaulate argument as conditional
			if ( !VPC_EvaluateCondition( pToken ) )
			{
				return;
			}
		}
		else
		{
			// argument is a defualt macro value
			if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, macroDefaultValue, sizeof( macroDefaultValue ) ) )
			{
				return;
			}
		}
	}

	// find macro, needs to be present and non-empty
	macro_t *pMacro = VPC_FindOrCreateMacro( macroName, false, NULL );
	if ( !pMacro || !strlen( pMacro->value ) )
	{
		if ( macroDefaultValue[0] )
		{
			VPC_FindOrCreateMacro( macroName, true, macroDefaultValue );
		}
		else
		{
			VPC_Error( "Required Macro '%s', not defined or empty [%s line:%d (or previous line)]", macroName, g_pScriptName, *g_pScriptLine );
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_LoadAddressMacro
//
//	$LoadAddressMacroAuto	<MacroName>
//	{
//		<ProjectName>	<BaseAddress>
//	}
//
//	Specialized instruction to populate the load address macro based on a project
//	name.
//-----------------------------------------------------------------------------
void VPC_Keyword_LoadAddressMacro( void )
{
	char	szProjectName[MAX_SYSTOKENCHARS];
	char	szMacroName[MAX_SYSTOKENCHARS];
	char	szBaseAddress[MAX_SYSTOKENCHARS];
	char	*pToken;

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, szMacroName, sizeof( szMacroName ) ) )
	{
		Sys_SkipBracedSection( &g_pScriptData, g_pScriptLine );
		return;		
	}

	pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
	if ( !pToken || !pToken[0] || stricmp( pToken, "{" ) )
	{
		VPC_SyntaxError();
	}
	
	while ( 1 )
	{
		pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
		if ( !pToken || !pToken[0] )
		{
			break;
		}
		strcpy( szProjectName, pToken );

		if ( !stricmp( pToken, "}" ) )
		{
			break;
		}
		else
		{
			if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, szBaseAddress, sizeof( szBaseAddress ) ) )
			{
				continue;
			}
		
			if ( !stricmp( szProjectName, g_szProjectName ) )
			{
				// set Macro
				VPC_FindOrCreateMacro( szMacroName, true, szBaseAddress );
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_LoadAddressMacroAuto
//
//	$LoadAddressMacroAuto	<MacroName> <BaseAddress>
//	{
//		<ProjectName>	<Length>
//	}
//
//	Specialized instruction to populate the load address macro based on a project
//	name.
//-----------------------------------------------------------------------------
void VPC_Keyword_LoadAddressMacroAuto( void )
{
	char	szProjectName[MAX_SYSTOKENCHARS];
	char	szMacroName[MAX_SYSTOKENCHARS];
	char	szBaseAddress[MAX_SYSTOKENCHARS];
	char	szLength[MAX_SYSTOKENCHARS];
	char	*pToken;

	pToken = Sys_GetToken( &g_pScriptData, false, g_pScriptLine );
	if ( !pToken || !pToken[0] )
	{
		VPC_SyntaxError();
	}
	strcpy( szMacroName, pToken );

	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, szBaseAddress, sizeof( szBaseAddress ) ) )
	{
		Sys_SkipBracedSection( &g_pScriptData, g_pScriptLine );
		return;		
	}
	unsigned int baseAddress = 0;
	sscanf( szBaseAddress, "%x", &baseAddress );

	pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
	if ( !pToken || !pToken[0] || stricmp( pToken, "{" ) )
	{
		VPC_SyntaxError();
	}
	
	while ( 1 )
	{
		pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
		if ( !pToken || !pToken[0] )
		{
			break;
		}
		strcpy( szProjectName, pToken );

		if ( !stricmp( szProjectName, g_szProjectName ) )
		{
			// set Macro
			char szMacroValue[MAX_SYSTOKENCHARS];
			sprintf( szMacroValue, "0x%8.8x", baseAddress );
			VPC_FindOrCreateMacro( szMacroName, true, szMacroValue );
		}

		if ( !stricmp( pToken, "}" ) )
		{
			break;
		}
		else
		{
			unsigned int dllLength = 0;
			if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, szLength, sizeof( szLength ) )	)
			{
				continue;
			}
			if ( strstr( szLength, "." ) )
			{
				// assume float format
				float fLength = 0;
				sscanf( szLength, "%f", &fLength );
				dllLength = fLength * 1024.0f * 1024.0f;
			}
			else
			{
				sscanf( szLength, "%d", &dllLength );
			}
			AlignValue( dllLength, 64*1024 );

			baseAddress += dllLength;
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_IgnoreRedundancyWarning
//
//-----------------------------------------------------------------------------
void VPC_Keyword_IgnoreRedundancyWarning( void )
{
	char	value[MAX_SYSTOKENCHARS];
	
	if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, value, sizeof( value ) ) )
	{
		return;
	}

	VARIANT_BOOL bVal = StringToBool( value );
	if ( !bVal )
		g_bIgnoreRedundancyWarning = false;
	else
		g_bIgnoreRedundancyWarning = true;
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_Linux
//
//-----------------------------------------------------------------------------
void VPC_Keyword_Linux( void )
{
	// always ignore everything in this block
	// parsed and processed by a different tool
	Sys_SkipBracedSection( &g_pScriptData, g_pScriptLine );
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_Project
//
//-----------------------------------------------------------------------------
void VPC_Keyword_Project()
{
	char	*pToken;
	HRESULT	hr;
	char	szProjectName[MAX_PATH];

	// check for optional project name
	pToken = Sys_PeekToken( g_pScriptData, false );

	if ( pToken && pToken[0] && stricmp( pToken, "{" ) )
	{
		// get optional project name
		pToken = Sys_GetToken( &g_pScriptData, false, g_pScriptLine );
		if ( !pToken || !pToken[0] )
		{
			VPC_SyntaxError();
		}

		ResolveMacrosInString( pToken, szProjectName, sizeof( szProjectName ) );

		if ( g_bDecorateProject )
		{
			// caller wants decorated project names, add the platform
			macro_t *pMacro = VPC_FindOrCreateMacro( "PLATFORM", false, NULL );
			if ( pMacro )
			{
				char szPlatform[MAX_PATH];
				sprintf( szPlatform, " (%s)", pMacro->value.String() );
				strcat( szProjectName, szPlatform );
			}
		}

		// change name
		CComBSTR bstr = szProjectName; 
		hr = g_spProject->put_Name( bstr );
		if ( FAILED( hr ) )
		{
			VPC_Error( "Cannot change the specified project name from to %s", szProjectName );
		}
	}
	else
	{
		CComBSTR bstr = NULL;
		hr = g_spProject->get_Name( &bstr );
		if ( FAILED( hr ) || !bstr )
		{
			VPC_Error( "Cannot get the specified project name" );
		}
		CW2CT szString( bstr );
		strcpy( szProjectName, szString.m_psz );
	}

	pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
	if ( !pToken || !pToken[0] || stricmp( pToken, "{" ) )
		VPC_SyntaxError();

	while ( 1 )
	{
		pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
		if ( !pToken || !pToken[0] )
			break;

		if ( !stricmp( pToken, "}" ) )
		{
			// the unnamed project does not get written, once it is named it will be written on closing scope
			if ( stricmp( szProjectName, "UNNAMED" ) )
			{
				VPC_Status( true, "Saving... Project: '%s' File: '%s'", szProjectName, g_szOutputFilename );
				hr = g_spProject->Save();
				if ( FAILED( hr ) )
				{
					VPC_Error( "Cannot save the specified project %s", szProjectName );
				}
				g_bGeneratedProject = true;
			}
			break;
		}
		else if ( !stricmp( pToken, "$Folder" ) )
		{
			// root level folder
			VPC_Keyword_Folder( 0 );
		}
		else if ( !stricmp( pToken, "$File" ) )
		{
			// add root level file
			VPC_Keyword_AddFile( 0 );
		}
		else if ( !stricmp( pToken, "-$File" ) )
		{
			// remove root level file
			VPC_Keyword_RemoveFile( 0 );
		}
		else if ( !stricmp( pToken, "$Shaders" ) )
		{
			// add root level shaders folder
			VPC_Keyword_Shaders( 0 );
		}
		else
		{
			VPC_SyntaxError();
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_ParseProjectScript
//
//-----------------------------------------------------------------------------
void VPC_ParseProjectScript( const char *pScriptName, int depth )
{
	int		scriptLine;
	char	*pScriptBuffer;
	char	*pScriptData;
	char	szScriptName[MAX_PATH];
	char	*pToken;
	char	szBigBuffer[MAX_SYSTOKENCHARS];
	bool	bIgnoreRedundancyWarning;
	HRESULT	hr;

	if ( !depth )
	{
		// startup initialization
		VPC_Status( true, "" );

		if ( !g_spEngine )
		{
			CoInitialize( NULL );

			// Access the msdev engine object
			hr = CoCreateInstance( __uuidof(VCProjectEngineObject), NULL, CLSCTX_INPROC_SERVER, __uuidof(VCProjectEngine), (void**)&g_spEngine );
			if ( FAILED( hr ) || !g_spEngine )
			{
				VPC_Error( "Cannot create VCProjectEngine object." );		
			}

			// bizarre bug I blame ms for (at least one operation required)
			// empty operation, otherwise rips in deconstructor when no other operations are done
			CComPtr<IDispatch> spDisp = NULL;
			CComBSTR bstr = "DUMMY DUMMY"; 
			g_spEngine->CreateProject( bstr, &spDisp );
			if ( spDisp )
			{
				g_spEngine->RemoveProject( spDisp );
			}
		}
	}

	strcpy( szScriptName, pScriptName );
	V_FixSlashes( szScriptName );

	// parse the text script
	if ( !Sys_Exists( szScriptName ) )
	{
		VPC_Error( "Cannot open %s", szScriptName );	
	}

	bIgnoreRedundancyWarning = g_bIgnoreRedundancyWarning;

	// create the default project
	// must have a root project for most operations
	VPC_CreateDefaultProject();

	if ( !depth )
	{
		// create reserved $QUOTE - used for embedding quotes
		VPC_FindOrCreateMacro( "QUOTE", true, "\"" );

		// create reserved $ROOTSCRIPT - tracks the root script
		VPC_FindOrCreateMacro( "ROOTSCRIPT", true, szScriptName );

		// create reserved $PROJECTNAME - tracks the undecorated pure project name
		// $(ProjectName) can be auto-decorated, making it unuseable by scripts expecting a pure project name
		VPC_FindOrCreateMacro( "PROJECTNAME", true, g_szProjectName );
	}

	// always spew the root script
	bool bSpew = ( depth == 0 );
	VPC_Status( bSpew, "Parsing: %s", szScriptName );
	int scriptLen = Sys_LoadFile( szScriptName, (void**)&pScriptBuffer, true );

	int index = g_scriptList.AddToTail();
	memset( &g_scriptList[index], 0, sizeof( scriptList_t ) );

	g_scriptList[index].m_scriptName = szScriptName;
	g_scriptList[index].m_crc = CRC32_ProcessSingleBuffer( pScriptBuffer, scriptLen );

	// create reserved $CRCCHECK
	V_strncpy( szBigBuffer, "set path=$SRCDIR\\..\\game\\bin;%path%\n", sizeof( szBigBuffer ) );
	V_strncat( szBigBuffer, "if exist $SRCDIR\\devtools\\bin\\vpc.exe $SRCDIR\\devtools\\bin\\vpc.exe", sizeof( szBigBuffer ) );
	for ( int i=0; i<g_scriptList.Count(); i++ )
	{
		char szCRCArg[MAX_SYSTOKENCHARS];
		V_snprintf( szCRCArg, sizeof( szCRCArg ), " -crc %s %8.8x", g_scriptList[i].m_scriptName.Get(), g_scriptList[i].m_crc );
		V_strncat( szBigBuffer, szCRCArg, sizeof( szBigBuffer ) ); 
	}
	VPC_FindOrCreateMacro( "CRCCHECK", true, szBigBuffer );

	g_pScriptName = szScriptName;
	g_pScriptLine = &scriptLine;
	pScriptData   = pScriptBuffer;

	// reset parsing
	g_pScriptData  = pScriptData;
	*g_pScriptLine = 1;

	while ( 1 )
	{
		pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
		if ( !pToken || !pToken[0] )
		{
			// end of file
			break;
		}

		if ( !stricmp( pToken, "$include" ) )
		{
			if ( ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, szBigBuffer, sizeof( szBigBuffer ) ) )
			{
				pScriptData = g_pScriptData;

				// recurse into and run
				VPC_ParseProjectScript( szBigBuffer, depth+1 );

				// restore parser
				g_pScriptName = szScriptName;
				g_pScriptData = pScriptData;
				g_pScriptLine = &scriptLine;

				// restore state
				g_bIgnoreRedundancyWarning = bIgnoreRedundancyWarning;
			}
		}
		else if ( !stricmp( pToken, "$configuration" ) )
		{
			VPC_Keyword_Configuration();
		}
		else if ( !stricmp( pToken, "$project" ) )
		{
			VPC_Keyword_Project();
		}
		else if ( !stricmp( pToken, "$macro" ) )
		{
			VPC_Keyword_Macro();
		}
		else if ( !stricmp( pToken, "$MacroRequired" ) )
		{
			VPC_Keyword_MacroRequired();
		}
		else if ( !stricmp( pToken, "$LoadAddressMacro" ) )
		{
			VPC_Keyword_LoadAddressMacro();
		}
		else if ( !stricmp( pToken, "$LoadAddressMacroAuto" ) )
		{
			VPC_Keyword_LoadAddressMacroAuto();
		}
		else if ( !stricmp( pToken, "$IgnoreRedundancyWarning" ) )
		{
			VPC_Keyword_IgnoreRedundancyWarning();
		}
		else if ( !stricmp( pToken, "$Linux" ) )
		{
			VPC_Keyword_Linux();
		}
		else
		{
			VPC_SyntaxError();
		}
	}

	free( pScriptBuffer );

	// for safety, force callers to restore to proper state
	g_pScriptName = NULL;
	g_pScriptData = NULL;
	g_pScriptLine = NULL;

	if ( !depth )
	{
		g_scriptList.Purge();
		g_macros.Purge();

#if !defined( VC8_CORRUPTION_BUG )
		g_spEngine = NULL;
#endif

		CComPtr<IDispatch> spDisp = NULL;
		g_spProject = spDisp;
		g_spFilter = spDisp;
		g_spFile = spDisp;
		g_spConfig = spDisp;
		g_spDebugSettings = spDisp;
		g_spToolCol = spDisp;
		g_spCompiler = NULL;
		g_spLinker = NULL;
		g_spBrowseInfoTool = NULL;
		g_spResources = NULL;
		g_spPreBuildEventTool = NULL;
		g_spPreLinkEventTool = NULL;
		g_spPostBuildEventTool = NULL;
		g_spCustomBuildTool = NULL;
		g_spLibrarian = NULL;
		g_spManifestTool = NULL;
		g_spXMLDocGenTool = NULL;
		g_spXboxCompiler = NULL;
		g_spXboxLinker = NULL;
		g_spXboxImageTool = NULL;
		g_spXboxDeploymentTool = NULL;

#if !defined( VC8_CORRUPTION_BUG )
		CoUninitialize();
#endif
	}
}