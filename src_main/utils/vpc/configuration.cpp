//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

CComQIPtr<VCConfiguration>			g_spConfig;
CComQIPtr<VCDebugSettings>			g_spDebugSettings;
CComQIPtr<IVCCollection>			g_spToolCol;
CComPtr<VCCLCompilerTool>			g_spCompiler;
CComPtr<VCLinkerTool>				g_spLinker;
CComPtr<VCBscMakeTool>				g_spBrowseInfoTool;
CComPtr<VCResourceCompilerTool>		g_spResources;
CComPtr<VCPreBuildEventTool>		g_spPreBuildEventTool;
CComPtr<VCPreLinkEventTool>			g_spPreLinkEventTool;
CComPtr<VCPostBuildEventTool>		g_spPostBuildEventTool;
CComPtr<VCCustomBuildTool>			g_spCustomBuildTool;
CComPtr<VCLibrarianTool>			g_spLibrarian;
CComPtr<VCManifestTool>				g_spManifestTool;
CComPtr<VCXDCMakeTool>				g_spXMLDocGenTool;
CComPtr<X360::VCCLX360CompilerTool>	g_spXboxCompiler;
CComPtr<X360::VCX360LinkerTool>		g_spXboxLinker;
CComPtr<X360::VCX360ImageTool>		g_spXboxImageTool;
CComPtr<X360::VCX360DeploymentTool>	g_spXboxDeploymentTool;

bool VPC_Config_GetConfigurations( const char *pszConfigName )
{
	HRESULT hr;

	// get configuration
	CComPtr<IDispatch> spDispConfigs = NULL;
	hr = g_spProject->get_Configurations( &spDispConfigs );
	if ( FAILED( hr ) || !spDispConfigs )
	{
		VPC_Error( "Could not get configurations from %s", g_szOutputFilename );
	}

	CComQIPtr<IVCCollection> spConfigCol = spDispConfigs;
	CComPtr<IDispatch> spDispConfig = NULL;
	CComVariant ccom = pszConfigName;
	hr = spConfigCol->Item( ccom, &spDispConfig );
	if ( FAILED( hr ) || !spDispConfig )
	{
		VPC_Error( "Could not get %s configuration from %s", pszConfigName, g_szOutputFilename );
	}
	g_spConfig = spDispConfig;

	CComPtr<IDispatch> spDispDebugSettings = NULL;
	hr = g_spConfig->get_DebugSettings( &spDispDebugSettings );
	if ( FAILED( hr ) || !spDispDebugSettings )
	{
		VPC_Error( "Could not get debug settings from %s configuration in %s", pszConfigName, g_szOutputFilename );
	}
	g_spDebugSettings = spDispDebugSettings;	

	return true;
}

bool VPC_Config_GetTool()
{
	HRESULT hr;

	// Access the Tools collection for this configuration
	CComPtr<IDispatch> spDispTools = NULL;
	hr = g_spConfig->get_Tools( &spDispTools );
	if ( FAILED( hr ) || !spDispTools )
	{
		VPC_Error( "Could not get tools from configuration in %s", g_szOutputFilename );
	}
	g_spToolCol = spDispTools;

	return true;
}

bool VPC_Config_GetCompilerTool( VCFileConfiguration *spFileConfig )
{
	HRESULT	hr = S_OK;

	g_spCompiler = NULL;
	g_spXboxCompiler = NULL;

	CComPtr<IDispatch> spDispTool = NULL;

	if ( VPC_IsDefineSet( "WIN32" ) )
	{
		if ( spFileConfig )
		{
			hr = spFileConfig->get_Tool( &spDispTool );
		}
		else
		{
			// Enumerate the Tools collection by name to get the tool
			hr = g_spToolCol->Item( CComVariant( L"VCCLCompilerTool" ), &spDispTool );
		}
		if ( FAILED( hr ) || !spDispTool )
		{
			VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
		}

		// Get the interface
		hr = spDispTool->QueryInterface( __uuidof( VCCLCompilerTool ), (void**)&g_spCompiler );
	}
	else if ( VPC_IsDefineSet( "X360" ) )
	{
		if ( spFileConfig )
		{
			hr = spFileConfig->get_Tool( &spDispTool );
		}
		else
		{
			// Enumerate the Tools collection by name to get the tool
			hr = g_spToolCol->Item( CComVariant( L"VCCLX360CompilerTool" ), &spDispTool );
		}
		if ( FAILED( hr ) || !spDispTool )
		{
			VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
		}

		// Get the interface
		hr = spDispTool->QueryInterface( __uuidof( X360::VCCLX360CompilerTool ), (void**)&g_spXboxCompiler );
	}

	if ( FAILED( hr ) || ( !g_spCompiler && !g_spXboxCompiler ) )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetLinkerTool()
{
	HRESULT hr = S_OK;

	g_spLinker = NULL;
	g_spXboxLinker = NULL;

	CComPtr<IDispatch> spDispTool = NULL;

	if ( VPC_IsDefineSet( "WIN32" ) )
	{
		// Enumerate the Tools collection by name to get the tool
		hr = g_spToolCol->Item( CComVariant( L"VCLinkerTool" ), &spDispTool );
		if ( FAILED( hr ) || !spDispTool )
		{
			VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
		}

		// Get the interface
		hr = spDispTool->QueryInterface( __uuidof( VCLinkerTool ), (void**)&g_spLinker );
	}
	else if ( VPC_IsDefineSet( "X360" ) )
	{
		// Enumerate the Tools collection by name to get the tool
		hr = g_spToolCol->Item( CComVariant( L"VCX360LinkerTool" ), &spDispTool );
		if ( FAILED( hr ) || !spDispTool )
		{
			VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
		}

		// Get the interface
		hr = spDispTool->QueryInterface( __uuidof( X360::VCX360LinkerTool ), (void**)&g_spXboxLinker );
	}
	if ( FAILED( hr ) || ( !g_spLinker && !g_spXboxLinker ) )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetManifestTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCManifestTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spManifestTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCManifestTool ), (void**)&g_spManifestTool );
	if ( FAILED( hr ) || !g_spManifestTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetXMLDocGenTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCXDCMakeTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spXMLDocGenTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCXDCMakeTool ), (void**)&g_spXMLDocGenTool );
	if ( FAILED( hr ) || !g_spXMLDocGenTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}
bool VPC_Config_GetBrowseInfoTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCBscMakeTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spBrowseInfoTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCBscMakeTool ), (void**)&g_spBrowseInfoTool );
	if ( FAILED( hr ) || !g_spBrowseInfoTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetResourceCompilerTool( VCFileConfiguration *spFileConfig )
{
	HRESULT hr;

	CComPtr<IDispatch> spDispTool = NULL;
	if ( spFileConfig )
	{
		hr = spFileConfig->get_Tool( &spDispTool );
	}
	else
	{
		// Enumerate the Tools collection by name to get the tool
		hr = g_spToolCol->Item( CComVariant( L"VCResourceCompilerTool" ), &spDispTool );
	}
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spResources = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCResourceCompilerTool ), (void**)&g_spResources );
	if ( FAILED( hr ) || !g_spResources )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetLibrarianTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCLibrarianTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spLibrarian = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCLibrarianTool ), (void**)&g_spLibrarian );
	if ( FAILED( hr ) || !g_spLibrarian )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetPreBuildEventTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCPreBuildEventTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spPreBuildEventTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCPreBuildEventTool ), (void**)&g_spPreBuildEventTool );
	if ( FAILED( hr ) || !g_spPreBuildEventTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetPreLinkEventTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCPreLinkEventTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spPreLinkEventTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCPreLinkEventTool ), (void**)&g_spPreLinkEventTool );
	if ( FAILED( hr ) || !g_spPreLinkEventTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetPostBuildEventTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCPostBuildEventTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spPostBuildEventTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCPostBuildEventTool ), (void**)&g_spPostBuildEventTool );
	if ( FAILED( hr ) || !g_spPostBuildEventTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetCustomBuildTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCCustomBuildTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spCustomBuildTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( VCCustomBuildTool ), (void**)&g_spCustomBuildTool );
	if ( FAILED( hr ) || !g_spCustomBuildTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetXboxImageTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCX360ImageTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spXboxImageTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( X360::VCX360ImageTool ), (void**)&g_spXboxImageTool );
	if ( FAILED( hr ) || !g_spXboxImageTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

bool VPC_Config_GetXboxDeploymentTool()
{
	HRESULT hr;

	// Enumerate the Tools collection by name to get the tool
	CComPtr<IDispatch> spDispTool = NULL;
	hr = g_spToolCol->Item( CComVariant( L"VCX360DeploymentTool" ), &spDispTool );
	if ( FAILED( hr ) || !spDispTool )
	{
		VPC_Error( "Could not get tool from configuration in %s", g_szOutputFilename );
	}

	// Get the interface
	g_spXboxDeploymentTool = NULL;
	hr = spDispTool->QueryInterface( __uuidof( X360::VCX360DeploymentTool ), (void**)&g_spXboxDeploymentTool );
	if ( FAILED( hr ) || !g_spXboxDeploymentTool )
	{
		VPC_Error( "Could not get tool interface from configuration in %s", g_szOutputFilename );
	}

	return true;
}

//-----------------------------------------------------------------------------
//	VPC_Config_Keyword
//
//-----------------------------------------------------------------------------
void VPC_Config_Keyword( configKeyword_e keyword, VCFileConfiguration *spFileConfig = NULL )
{
	char		*pToken;
	char		propertyName[MAX_SYSTOKENCHARS];
	property_t	*pProperties;
	int			i;
	int			j;

	switch ( keyword )
	{
	case KEYWORD_GENERAL:
		pProperties = g_generalProperties;
		break;
	case KEYWORD_DEBUGGING:
		pProperties = g_debuggingProperties;
		break;
	case KEYWORD_COMPILER:
		pProperties = g_compilerProperties;
		VPC_Config_GetCompilerTool( spFileConfig );
		break;
	case KEYWORD_XBOXCOMPILER:
		pProperties = g_xboxCompilerProperties;
		VPC_Config_GetCompilerTool( spFileConfig );
		break;
	case KEYWORD_LINKER:
		pProperties = g_linkerProperties;
		VPC_Config_GetLinkerTool();
		break;
	case KEYWORD_XBOXLINKER:
		pProperties = g_xboxLinkerProperties;
		VPC_Config_GetLinkerTool();
		break;
	case KEYWORD_MANIFEST:
		pProperties = g_manifestProperties;
		VPC_Config_GetManifestTool();
		break;
	case KEYWORD_XMLDOCGEN:
		pProperties = g_xmlDocGenProperties;
		VPC_Config_GetXMLDocGenTool();
		break;
	case KEYWORD_BROWSEINFO:
		pProperties = g_browseInfoProperties;
		VPC_Config_GetBrowseInfoTool();
		break;
	case KEYWORD_RESOURCES:
		pProperties = g_resourcesProperties;
		VPC_Config_GetResourceCompilerTool( spFileConfig );
		break;
	case KEYWORD_PREBUILDEVENT:
		pProperties = g_preBuildEventProperties;
		VPC_Config_GetPreBuildEventTool();
		break;
	case KEYWORD_PRELINKEVENT:
		pProperties = g_preLinkEventProperties;
		VPC_Config_GetPreLinkEventTool();
		break;
	case KEYWORD_POSTBUILDEVENT:
		pProperties = g_postBuildEventProperties;
		VPC_Config_GetPostBuildEventTool();
		break;
	case KEYWORD_CUSTOMBUILDSTEP:
		pProperties = g_customBuildProperties;
		VPC_Config_GetCustomBuildTool();
		break;
	case KEYWORD_LIBRARIAN:
		pProperties = g_librarianProperties;
		// access to librarian tool is only allowed after config has been set to static lib
		// having a library block before the config block that sets the config as a lib is a fatal error
		VPC_Config_GetLibrarianTool();
		break;
	case KEYWORD_XBOXIMAGE:
		pProperties = g_xboxImageProperties;
		VPC_Config_GetXboxImageTool();
		break;
	case KEYWORD_XBOXDEPLOYMENT:
		pProperties = g_xboxDeploymentProperties;
		VPC_Config_GetXboxDeploymentTool();
		break;
	default:
		Sys_SkipBracedSection( &g_pScriptData, g_pScriptLine );
		assert( 0 );
		return;
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
			// end of section
			break;
		}

		strcpy( propertyName, pToken );

		// find in table, jump to handler
		for ( i=0; ;i++ )
		{
			if ( !pProperties[i].pName )
			{
				// unknown property
				VPC_SyntaxError( "Unknown property %s", propertyName );
			}
			if ( !stricmp( propertyName, pProperties[i].pName ) )
			{
				if ( pProperties[i].platformMask )
				{
					int bMask = 0;
					for ( j=0; j<g_defines.Count(); j++ )
					{
						if ( g_defines[j].bDefined && g_defines[j].type == DEFINE_PLATFORM && g_defines[j].bSet )
						{
							if ( !stricmp( g_defines[j].name.String(), "win32" ) )
								bMask |= PLATFORM_WIN32;
							else if ( !stricmp( g_defines[j].name.String(), "linux" ) )
								bMask |= PLATFORM_LINUX;
							else if ( !stricmp( g_defines[j].name.String(), "x360" ) )
								bMask |= PLATFORM_X360;
							else if ( !stricmp( g_defines[j].name.String(), "ps3" ) )
								bMask |= PLATFORM_PS3;
							else
								VPC_Error( "Unknown platform %s", g_defines[j].name.String() );
						}
					}

					if ( !( bMask && pProperties[i].platformMask ) )
					{
						VPC_Warning( "Property %s not availiable for current target platform, Ignoring", propertyName );
						Sys_SkipRestOfLine( &g_pScriptData, g_pScriptLine );
						break;
					}
				}

				pToken = Sys_PeekToken( g_pScriptData, false );
				if ( pToken && pToken[0] )
				{
					if ( pProperties[i].handler )
					{
						if ( !pProperties[i].handler( propertyName ) )
						{
							VPC_Error( "Failed to set %s", propertyName );
						}
					}
					else
					{
						VPC_Warning( "Not Implemented: Ignoring %s", propertyName );
						Sys_SkipRestOfLine( &g_pScriptData, g_pScriptLine );
					}
				}
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_Configuration
//
//-----------------------------------------------------------------------------
void VPC_Keyword_Configuration()
{
	char					*pToken;
	CComBSTR				bstr;
	char					szConfigName[MAX_PATH];
	HRESULT					hr;
	bool					bAllowNextLine = false;
	int						i;
	CUtlVector<CUtlString>	configs;
	char					buff[MAX_SYSTOKENCHARS];

	while ( 1 )
	{
		pToken = Sys_GetToken( &g_pScriptData, bAllowNextLine, g_pScriptLine );
		if ( !pToken || !pToken[0] )
			break;

		if ( !stricmp( pToken, "\\" ) )
		{
			bAllowNextLine = true;
			continue;
		}
		else
		{
			bAllowNextLine = false;
		}

		int index = configs.AddToTail();
		configs[index] = pToken;

		// check for another optional config
		pToken = Sys_PeekToken( g_pScriptData, bAllowNextLine );
		if ( !pToken || !pToken[0] || !stricmp( pToken, "{" ) || !stricmp( pToken, "}" ) || (pToken[0] == '$') )
			break;
	}

	// no configuration specified, use all known
	if ( !configs.Count() )
	{
		// get configuration
		CComPtr<IDispatch> spDispConfigs = NULL;
		hr = g_spProject->get_Configurations( &spDispConfigs );
		if ( FAILED( hr ) || !spDispConfigs )
		{
			VPC_Error( "Could not get configurations from %s", g_szOutputFilename );
		}
		CComQIPtr<IVCCollection> spConfigCol = spDispConfigs;
		
		long count=0;
		hr = spConfigCol->get_Count( &count );
		if ( FAILED( hr ) || !count )
		{
			VPC_Error( "Could not get configurations from %s", g_szOutputFilename );
		}

		for ( i=0; i<count; i++ )
		{
			CComPtr<IDispatch> spDispConfig = NULL;
			CComVariant ccom = i+1;

			hr = spConfigCol->Item( ccom, &spDispConfig );
			if ( FAILED( hr ) || !spDispConfig )
			{
				VPC_Error( "Could not get configuration in %s", g_szOutputFilename );
			}	
			CComQIPtr<VCConfiguration> spConfig = spDispConfig;

			CComBSTR bstr;
			spConfig->get_ConfigurationName( &bstr );
			CW2CT szString( bstr );

			int index = configs.AddToTail();
			configs[index] = szString.m_psz;
		}
	}

	// save parser state
	char *saveData = g_pScriptData;
	int saveLine = *g_pScriptLine;

	for ( i=0; i<configs.Count(); i++ )
	{
		// restore parser state
		g_pScriptData  = saveData;
		*g_pScriptLine = saveLine;

		strcpy( szConfigName, configs[i].String() );

		bstr = szConfigName;
		hr = g_spProject->AddConfiguration( bstr );
		if ( FAILED( hr ) )
		{
			VPC_Error( "Could not add configuration %s to %s", szConfigName, g_szOutputFilename );
		}

		// get access objects
		VPC_Config_GetConfigurations( szConfigName );
		VPC_Config_GetTool();

		pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
		if ( !pToken || !pToken[0] || stricmp( pToken, "{" ) )
		{
			VPC_SyntaxError();
		}

		while ( 1 )
		{
			g_pScriptData = Sys_SkipToValidToken( g_pScriptData, NULL, g_pScriptLine );
			if ( !ParsePropertyValue( &g_pScriptData, g_pScriptLine, NULL, buff, sizeof( buff ) ) )
			{
				Sys_SkipBracedSection( &g_pScriptData, g_pScriptLine );
				continue;
			}

			if ( !stricmp( buff, "}" ) )
			{
				// end of section
				break;
			}

			if ( !stricmp( buff, "$General" ) )
			{
				VPC_Config_Keyword( KEYWORD_GENERAL );
			}
			else if ( !stricmp( buff, "$Debugging" ) )
			{
				VPC_Config_Keyword( KEYWORD_DEBUGGING );
			}
			else if ( !stricmp( buff, "$Compiler" ) )
			{
				if ( VPC_IsDefineSet( "WIN32" ) )
					VPC_Config_Keyword( KEYWORD_COMPILER );
				else if ( VPC_IsDefineSet( "X360" ) )
					VPC_Config_Keyword( KEYWORD_XBOXCOMPILER );
				else
					VPC_Error( "Unspecified Platform" );
			}
			else if ( !stricmp( buff, "$Librarian" ) )
			{
				VPC_Config_Keyword( KEYWORD_LIBRARIAN );
			}
			else if ( !stricmp( buff, "$Linker" ) )
			{
				if ( VPC_IsDefineSet( "WIN32" ) )
					VPC_Config_Keyword( KEYWORD_LINKER );
				else if ( VPC_IsDefineSet( "X360" ) )
					VPC_Config_Keyword( KEYWORD_XBOXLINKER );
				else
					VPC_Error( "Unspecified Platform" );
			}
			else if ( !stricmp( buff, "$ManifestTool" ) )
			{
				VPC_Config_Keyword( KEYWORD_MANIFEST );
			}
			else if ( !stricmp( buff, "$XMLDocumentGenerator" ) )
			{
				VPC_Config_Keyword( KEYWORD_XMLDOCGEN );
			}
			else if ( !stricmp( buff, "$BrowseInformation" ) )
			{
				VPC_Config_Keyword( KEYWORD_BROWSEINFO );
			}
			else if ( !stricmp( buff, "$Resources" ) )
			{
				VPC_Config_Keyword( KEYWORD_RESOURCES );
			}
			else if ( !stricmp( buff, "$PreBuildEvent" ) )
			{
				VPC_Config_Keyword( KEYWORD_PREBUILDEVENT );
			}
			else if ( !stricmp( buff, "$PreLinkEvent" ) )
			{
				VPC_Config_Keyword( KEYWORD_PRELINKEVENT );
			}
			else if ( !stricmp( buff, "$PostBuildEvent" ) )
			{
				VPC_Config_Keyword( KEYWORD_POSTBUILDEVENT );
			}
			else if ( !stricmp( buff, "$CustomBuildStep" ) )
			{
				VPC_Config_Keyword( KEYWORD_CUSTOMBUILDSTEP );
			}
			else if ( !stricmp( buff, "$Xbox360ImageConversion" ) )
			{
				VPC_Config_Keyword( KEYWORD_XBOXIMAGE );
			}
			else if ( !stricmp( buff, "$ConsoleDeployment" ) )
			{
				VPC_Config_Keyword( KEYWORD_XBOXDEPLOYMENT );
			}
			else
				VPC_SyntaxError();
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_Keyword_FileConfiguration
//
//-----------------------------------------------------------------------------
void VPC_Keyword_FileConfiguration()
{
	char											*pToken;
	CComBSTR										bstr;
	char											szConfigName[MAX_PATH];
	HRESULT											hr;
	bool											bAllowNextLine = false;
	int												numConfigs = 0;
	CComQIPtr<VCFileConfiguration>					spConfigs[100];
	CComQIPtr<VCFileConfiguration>					spFileConfig;
	CComPtr<VCResourceCompilerTool>					spResources;
	CComPtr<VCCLCompilerTool>						spCompiler;
	CComPtr<X360::VCCLX360CompilerTool>				spXboxCompiler;

	while ( 1 )
	{
		pToken = Sys_GetToken( &g_pScriptData, bAllowNextLine, g_pScriptLine );
		if ( !pToken || !pToken[0] )
			break;

		if ( !stricmp( pToken, "\\" ) )
		{
			bAllowNextLine = true;
			continue;
		}
		else
		{
			bAllowNextLine = false;
		}

		strcpy( szConfigName, pToken );

		CComPtr<IDispatch> spDispCol = NULL;
		hr = g_spFile->get_FileConfigurations( &spDispCol );
		if ( FAILED( hr ) || !spDispCol )
		{
			VPC_Error( "Could not get file configurations in %s", g_szOutputFilename );
		}

		CComQIPtr<IVCCollection> spFileConfigs = spDispCol;
		CComPtr<IDispatch> spDispFileConfig = NULL;
		CComVariant ccom = szConfigName;
		hr = spFileConfigs->Item( ccom, &spDispFileConfig );
		if ( FAILED( hr ) || !spDispFileConfig )
		{
			VPC_Error( "Could not get file configuration %s in %s", szConfigName, g_szOutputFilename );
		}
				
		spFileConfig = spDispFileConfig;
		spConfigs[numConfigs++] = spFileConfig;
		if ( numConfigs >= ARRAYSIZE( spConfigs ) )
			VPC_Error( "Exceeded number of projects" );

		// check for another optional config
		pToken = Sys_PeekToken( g_pScriptData, bAllowNextLine );
		if ( !pToken || !pToken[0] || !stricmp( pToken, "{" ) || !stricmp( pToken, "}" ) || (pToken[0] == '$') )
			break;
	}

	// no configuration specified, use all known
	if ( !numConfigs )
	{
		CComPtr<IDispatch> spDispCol = NULL;
		hr = g_spFile->get_FileConfigurations( &spDispCol );
		if ( FAILED( hr ) || !spDispCol )
		{
			VPC_Error( "Could not get file configurations in %s", g_szOutputFilename );
		}

		CComQIPtr<IVCCollection> spFileConfigs = spDispCol;

		long count=0;
		hr = spFileConfigs->get_Count( &count );
		if ( FAILED( hr ) || !count )
		{
			VPC_Error( "Could not get file configurations in %s", g_szOutputFilename );
		}

		for ( int i=0; i<count; i++ )
		{
			CComPtr<IDispatch> spDispFileConfig = NULL;
			CComVariant ccom = i+1;

			hr = spFileConfigs->Item( ccom, &spDispFileConfig );
			if ( FAILED( hr ) || !spDispFileConfig )
			{
				VPC_Error( "Could not get file configuration %d in %s", i, g_szOutputFilename );
			}
				
			spFileConfig = spDispFileConfig;
			spConfigs[numConfigs++] = spFileConfig;
			if ( numConfigs >= ARRAYSIZE( spConfigs ) )
				VPC_Error( "Exceeded number of projects" );
		}
	}

	// save parser state
	char *saveData = g_pScriptData;
	int saveLine = *g_pScriptLine;

	for ( int i=0; i<numConfigs; i++ )
	{
		// restore parser state
		g_pScriptData  = saveData;
		*g_pScriptLine = saveLine;

		spFileConfig = spConfigs[i];

		pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
		if ( !pToken || !pToken[0] || stricmp( pToken, "{" ) )
			VPC_SyntaxError();

		while ( 1 )
		{
			pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
			if ( !pToken || !pToken[0] )
				VPC_SyntaxError();

			if ( !stricmp( pToken, "}" ) )
			{
				// end of section
				break;
			}

			if ( !stricmp( pToken, "$ExcludedFromBuild" ) )
			{
				pToken = Sys_PeekToken( g_pScriptData, false );
				if ( !pToken || !pToken[0] )
					continue;

				pToken = Sys_GetToken( &g_pScriptData, true, g_pScriptLine );
				if ( !pToken || !pToken[0] )
					VPC_SyntaxError();

				VARIANT_BOOL value = StringToBool( pToken );
				hr = spFileConfig->put_ExcludedFromBuild( value );
				if ( FAILED( hr ) )
				{
					VPC_Error( "Failed to set %s", "$ExcludedFromBuild" );
				}
			}
			else if ( !stricmp( pToken, "$Compiler" ) )
			{
				spCompiler = g_spCompiler;
				spXboxCompiler = g_spXboxCompiler;

				if ( VPC_IsDefineSet( "WIN32" ) )
					VPC_Config_Keyword( KEYWORD_COMPILER, spFileConfig );
				else if ( VPC_IsDefineSet( "X360" ) )
					VPC_Config_Keyword( KEYWORD_XBOXCOMPILER, spFileConfig );
				else
					VPC_Error( "Unspecified Platform" );

				g_spCompiler = spCompiler;
				g_spXboxCompiler = spXboxCompiler;
			}
			else if ( !stricmp( pToken, "$Resources" ) )
			{
				spResources = g_spResources;
				VPC_Config_Keyword( KEYWORD_RESOURCES, spFileConfig );
				g_spResources = spResources;
			}
			else
				VPC_SyntaxError();
		}
	}
}

void VPC_Config_SpewProperties( configKeyword_e keyword )
{
	property_t	*pProperties;
	const char	*pTitle;

	switch ( keyword )
	{
	case KEYWORD_GENERAL:
		pProperties = g_generalProperties;
		pTitle = "GENERAL";
		break;
	case KEYWORD_DEBUGGING:
		pProperties = g_debuggingProperties;
		pTitle = "DEBUGGING";
		break;
	case KEYWORD_COMPILER:
		pTitle = "WIN32 COMPILER";
		pProperties = g_compilerProperties;
		break;
	case KEYWORD_XBOXCOMPILER:
		pTitle = "XBOX COMPILER";
		pProperties = g_xboxCompilerProperties;
		break;
	case KEYWORD_LINKER:
		pTitle = "WIN32 LINKER";
		pProperties = g_linkerProperties;
		break;
	case KEYWORD_XBOXLINKER:
		pTitle = "XBOX LINKER";
		pProperties = g_xboxLinkerProperties;
		break;
	case KEYWORD_MANIFEST:
		pTitle = "MANIFEST TOOL";
		pProperties = g_manifestProperties;
		break;
	case KEYWORD_XMLDOCGEN:
		pTitle = "XML DOCUMENT GENERATOR";
		pProperties = g_xmlDocGenProperties;
		break;
	case KEYWORD_BROWSEINFO:
		pProperties = g_browseInfoProperties;
		pTitle = "BROWSE INFORMATION";
		break;
	case KEYWORD_RESOURCES:
		pProperties = g_resourcesProperties;
		pTitle = "RESOURCES";
		break;
	case KEYWORD_PREBUILDEVENT:
		pProperties = g_preBuildEventProperties;
		pTitle = "PRE BUILD EVENT";
		break;
	case KEYWORD_PRELINKEVENT:
		pProperties = g_preLinkEventProperties;
		pTitle = "PRE LINK EVENT";
		break;
	case KEYWORD_POSTBUILDEVENT:
		pProperties = g_postBuildEventProperties;
		pTitle = "POST BUILD EVENT";
		break;
	case KEYWORD_CUSTOMBUILDSTEP:
		pProperties = g_customBuildProperties;
		pTitle = "CUSTOM BUILD STEP";
		break;
	case KEYWORD_LIBRARIAN:
		pProperties = g_librarianProperties;
		pTitle = "LIBRARIAN";
		break;
	case KEYWORD_XBOXIMAGE:
		pProperties = g_xboxImageProperties;
		pTitle = "XBOX IMAGE";
		break;
	case KEYWORD_XBOXDEPLOYMENT:
		pProperties = g_xboxDeploymentProperties;
		pTitle = "XBOX DEPLOYMENT";
		break;
	default:
		assert( 0 );
		return;
	}

	printf( "\n--- %s ---\n", pTitle );
	for ( int i=0; ;i++ )
	{
		if ( !pProperties[i].pName )
			break;

		printf( "%s\n", pProperties[i].pName );
	}
}