//========= Copyright © 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: VPC
//
//=====================================================================================//

#include "vpc.h"

CUtlVector<project_t>		g_projects;
CUtlVector<group_t>			g_groups;
CUtlVector<projectIndex_t>	g_targetProjects;
CUtlVector<groupTag_t>		g_groupTags;

//-----------------------------------------------------------------------------
//	VPC_Group_ConditionHasDefinedType
//
//-----------------------------------------------------------------------------
bool VPC_Group_ConditionHasDefinedType( const char* pCondition, defineType_e type )
{
	char	symbol[MAX_SYSTOKENCHARS];

	for ( int i=0; i<g_defines.Count(); i++ )
	{
		if ( g_defines[i].type != type )
			continue;
	
		sprintf( symbol, "$%s", g_defines[i].name.String() );
		if ( V_stristr( pCondition, symbol ) )
		{
			// a define of expected type occurs in the conditional expression
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//	VPC_Group_FindOrCreateProject
//
//-----------------------------------------------------------------------------
projectIndex_t VPC_Group_FindOrCreateProject( char *pName, bool bCreate )
{
	for (int i=0; i<g_projects.Count(); i++)
	{
		if ( !stricmp( pName, g_projects[i].name.String() ) )
			return i;
	}

	if ( !bCreate )
		return INVALID_INDEX;

	int index = g_projects.AddToTail();
	memset( &g_projects[index], 0, sizeof( project_t ) );
	g_projects[index].name = pName;

	return index;
}

//-----------------------------------------------------------------------------
//	VPC_Group_CreateGroup
//
//-----------------------------------------------------------------------------
groupIndex_t VPC_Group_CreateGroup()
{
	groupIndex_t index = g_groups.AddToTail();
	memset( &g_groups[index], 0, sizeof( group_t ) );

	return index;
}

//-----------------------------------------------------------------------------
//	VPC_Group_FindOrCreateGroupTag
//
//-----------------------------------------------------------------------------
groupTagIndex_t VPC_Group_FindOrCreateGroupTag( char *pName, bool bCreate )
{
	for (int i=0; i<g_groupTags.Count(); i++)
	{
		if ( !stricmp( pName, g_groupTags[i].name.String() ) )
			return i;
	}

	if ( !bCreate )
		return INVALID_INDEX;

	groupTagIndex_t index = g_groupTags.AddToTail();
	memset( &g_groupTags[index], 0, sizeof( groupTag_t ) );
	g_groupTags[index].name = pName;

	return index;
}

//-----------------------------------------------------------------------------
//	VPC_GroupKeyword_Games
//
//-----------------------------------------------------------------------------
void VPC_GroupKeyword_Games()
{
	char	*pToken;

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
		else
		{
			VPC_FindOrCreateDefine( pToken, true, DEFINE_GAME );
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_GroupKeyword_Group
//
//-----------------------------------------------------------------------------
void VPC_GroupKeyword_Group()
{
	char			*pToken;
	bool			bFirstToken = true;
	groupIndex_t	groupIndex;
	projectIndex_t	projectIndex;

	groupIndex = VPC_Group_CreateGroup();

	while ( 1 )
	{
		if ( !bFirstToken )
		{
			pToken = Sys_PeekToken( g_pScriptData, false );
			if ( !pToken || !pToken[0] )
				break;
		}
		else
		{
			bFirstToken = false;
		}

		pToken = Sys_GetToken( &g_pScriptData, false, g_pScriptLine );
		if ( !pToken || !pToken[0] )
			VPC_SyntaxError();

		// specified tag now builds this group
		groupTagIndex_t groupTagIndex = VPC_Group_FindOrCreateGroupTag( pToken, true );
		g_groupTags[groupTagIndex].groups.AddToTail( groupIndex );
	}

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
		else
		{
			projectIndex = VPC_Group_FindOrCreateProject( pToken, false );
			if ( projectIndex != INVALID_INDEX )
			{
				int index = g_groups[groupIndex].projects.AddToTail();
				g_groups[groupIndex].projects[index] = projectIndex;
			}
			else
			{
				VPC_Warning( "No Project %s defined, ignoring.", pToken );
				continue;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_GroupKeyword_Project
//
//-----------------------------------------------------------------------------
void VPC_GroupKeyword_Project()
{
	char *pToken;

	pToken = Sys_GetToken( &g_pScriptData, false, g_pScriptLine );
	if ( !pToken || !pToken[0] )
		VPC_SyntaxError();

	if ( VPC_Group_FindOrCreateProject( pToken, false ) != INVALID_INDEX )
	{
		// already defined
		VPC_Warning( "project %s already defined", pToken );
		VPC_SyntaxError();
	}

	projectIndex_t projectIndex = VPC_Group_FindOrCreateProject( pToken, true );

	// create a default group that contains just this project
	groupIndex_t groupIndex = VPC_Group_CreateGroup();
	g_groups[groupIndex].projects.AddToTail( projectIndex );

	// create a default tag that matches the project name
	groupTagIndex_t groupTagIndex = VPC_Group_FindOrCreateGroupTag( pToken, true );
	g_groupTags[groupTagIndex].groups.AddToTail( groupIndex );
	g_groupTags[groupTagIndex].bSameAsProject = true;

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
			// end of setcion
			break;
		}
		else
		{
			scriptIndex_t scriptIndex = g_projects[projectIndex].scripts.AddToTail();
			memset( &g_projects[projectIndex].scripts[scriptIndex], 0, sizeof( script_t ) );

			g_projects[projectIndex].scripts[scriptIndex].name = pToken;

			pToken = Sys_PeekToken( g_pScriptData, false );
			if ( pToken && pToken[0] )
			{
				pToken = Sys_GetToken( &g_pScriptData, false, g_pScriptLine );
				g_projects[projectIndex].scripts[scriptIndex].condition = pToken;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//	VPC_ParseGroupScript
//
//-----------------------------------------------------------------------------
void VPC_ParseGroupScript( const char *pScriptName )
{
	int		scriptLine;
	char	*pScriptBuffer;
	char	*pScriptData;
	char	szScriptName[MAX_PATH];
	char	*pToken;

	// caller's pointer is aliased
	strcpy( szScriptName, pScriptName );
	V_FixSlashes( szScriptName );

	// parse the text script
	if ( !Sys_Exists( szScriptName ) )
	{
		VPC_Error( "Cannot open %s", szScriptName );	
	}

	VPC_Status( false, "Parsing: %s", szScriptName );
	Sys_LoadFile( szScriptName, (void**)&pScriptBuffer, true );

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
			pToken = Sys_GetToken( &g_pScriptData, false, g_pScriptLine );
			if ( !pToken || !pToken[0] )
			{
				// end of file
				VPC_SyntaxError();
			}

			pScriptData = g_pScriptData;

			// recurse into and run
			VPC_ParseGroupScript( pToken );

			// restore parser
			g_pScriptName = szScriptName;
			g_pScriptData = pScriptData;
			g_pScriptLine = &scriptLine;
		}
		else if ( !stricmp( pToken, "$games" ) )
		{
			VPC_GroupKeyword_Games();
		}
		else if ( !stricmp( pToken, "$group" ) )
		{
			VPC_GroupKeyword_Group();
		}
		else if ( !stricmp( pToken, "$project" ) )
		{
			VPC_GroupKeyword_Project();
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
}

//-----------------------------------------------------------------------------
//	VPC_GenerateBuildSet
//
//	Collect all the +XXX, remove all the -XXX
//	This allows removal to be the expected trumping operation.
//-----------------------------------------------------------------------------
void VPC_GenerateBuildSet()
{
	int			i;
	int			j;
	int			k;
	int			n;
	groupTag_t	*pGroupTag;
	group_t		*pGroup;
	char		*pCommand;

	// process +XXX commands
	for ( i=0; i<g_buildCommands.Count(); i++ )
	{
		pCommand = g_buildCommands[i].Get();
		if ( pCommand[0] == '-' )
			continue;

		groupTagIndex_t groupTagIndex = VPC_Group_FindOrCreateGroupTag( pCommand+1, false );
		if ( groupTagIndex == INVALID_INDEX )
			continue;
		pGroupTag = &g_groupTags[groupTagIndex];

		for ( j=0; j<pGroupTag->groups.Count(); j++ )
		{
			pGroup = &g_groups[pGroupTag->groups[j]];
			for ( k=0; k<pGroup->projects.Count(); k++ )
			{
				projectIndex_t targetProject = pGroup->projects[k];

				// find
				for ( n=0; n<g_targetProjects.Count(); n++ )
				{
					if ( g_targetProjects[n] == targetProject )
						break;
				}

				// not found, add
				if ( n == g_targetProjects.Count() )
				{
					int index = g_targetProjects.AddToTail();
					g_targetProjects[index] = targetProject;
				}
			}
		}
	}

	// process -XXX commands, explicitly remove tagge projects
	for ( i=0; i<g_buildCommands.Count(); i++ )
	{
		pCommand = g_buildCommands[i].Get();
		if ( pCommand[0] == '+' )
			continue;

		groupTagIndex_t groupTagIndex = VPC_Group_FindOrCreateGroupTag( pCommand+1, false );
		if ( groupTagIndex == INVALID_INDEX )
			continue;
		pGroupTag = &g_groupTags[groupTagIndex];

		for ( j=0; j<pGroupTag->groups.Count(); j++ )
		{
			pGroup = &g_groups[pGroupTag->groups[j]];
			for ( k=0; k<pGroup->projects.Count(); k++ )
			{
				g_targetProjects.FindAndRemove( pGroup->projects[k] );
			}
		}
	}
}