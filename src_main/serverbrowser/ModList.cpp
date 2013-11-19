//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#include "pch_serverbrowser.h"

extern class IAppInformation *g_pAppInformation; // may be NULL

//-----------------------------------------------------------------------------
// Purpose: Singleton accessor
//-----------------------------------------------------------------------------
CModList &ModList()
{
	static CModList s_ModList;
	return s_ModList;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CModList::CModList()
{
	ParseSteamMods();
}

//-----------------------------------------------------------------------------
// Purpose: returns number of mods 
//-----------------------------------------------------------------------------
int CModList::ModCount()
{
	return m_ModList.Count();
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
const char *CModList::GetModName(int index)
{
	return m_ModList[index].description;
}

//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
const char *CModList::GetModDir(int index)
{
	return m_ModList[index].gamedir;
}


//-----------------------------------------------------------------------------
// Purpose: data accessor
//-----------------------------------------------------------------------------
int CModList::GetAppID(int index) const
{
	return m_ModList[index].appID;
}


//-----------------------------------------------------------------------------
// Purpose: returns the mod name for the associated gamedir
//-----------------------------------------------------------------------------
const char *CModList::GetModNameForModDir(const char *gamedir, int iAppID = 0 )
{
	for (int i = 0; i < m_ModList.Count(); i++)
	{
		if (!Q_stricmp(m_ModList[i].gamedir, gamedir) && ( iAppID == 0 || iAppID == m_ModList[i].appID ) )
		{
			return m_ModList[i].description;
		}
	}

	if ( ServerBrowserDialog().GetActiveModName() )
	{
		return ServerBrowserDialog().GetActiveGameName();
	}
	return "";
}


//-----------------------------------------------------------------------------
// Purpose: looks for liblist.gam in the mod dir
//-----------------------------------------------------------------------------
bool ExtractGameNameFromLiblist(char *gameDescription, const char *appPath, const char *gameDir)
{
	char fullPath[512];
	sprintf(fullPath, "%s\\%s\\liblist.gam", appPath, gameDir);

	FILE *f = fopen(fullPath, "rt");
	if (!f)
		return false;

	// read out the whole file into a buffer
	while (!feof(f))
	{
		char line[512];
		fgets(line, sizeof(line), f);
		// look for the game key
		if (!Q_strnicmp(line, "game ", 5))
		{
			// parse out from quote to quote
			char *start = strchr(line, '\"');
			if (!start)
				break;
			++start;
			char *end = strchr(start, '\"');
			if (!end)
				break;

			strncpy(gameDescription, start, end - start);
			gameDescription[end - start] = 0;
			break;
		}
	}

	fclose(f);
	return true;
}


//-----------------------------------------------------------------------------
// Purpose: sort the mod list in alphabetical order
//-----------------------------------------------------------------------------
int CModList::ModNameCompare( const mod_t *pLeft, const mod_t *pRight )
{
	return ( Q_stricmp( pLeft->description, pRight->description ) ); 
}

//-----------------------------------------------------------------------------
// Purpose: gets list of steam games we can filter for
//-----------------------------------------------------------------------------
void CModList::ParseSteamMods()
{
	// bugbug jmccaskey - fix this to assert again once IAppInformation is moved
	// out of SteamUI so it can work in the overlay, for now server browser will
	// only half work in the overlay...
	//Assert( g_pAppInformation );
	if ( g_pAppInformation )
	{
		int numSteamApps = g_pAppInformation->GetAppCount();
		for ( int i = 0; i < numSteamApps; i++ )
		{
			bool bIsSubscribed = g_pAppInformation->GetAppIsSubscribed( i );

			// skip this app if not subscribed
			if ( !bIsSubscribed )
				continue;

			bool bAppHasServers = g_pAppInformation->GetAppHasServers( i );

			// skip if NoServers
			if ( !bAppHasServers )
				continue;

			const char *pchGameDir = g_pAppInformation->GetAppGameDir( i );
			const char *pchName = g_pAppInformation->GetAppServerBrowserName( i );

			if ( pchGameDir && Q_strlen( pchGameDir ) > 0 
				&& pchName &&  Q_strlen( pchName ) > 0 )
			{
				// add the game directory to the list
				mod_t mod;
				
				if ( Q_IsAbsolutePath( pchGameDir ) ) // 3rd party mods are full paths, but the master server just wants the game dir 
				{	
					Q_strncpy( mod.gamedir, pchGameDir, sizeof(mod.gamedir) );
					Q_StripLastDir( mod.gamedir, sizeof(mod.gamedir) );
					Q_strncpy( mod.gamedir, pchGameDir + Q_strlen(mod.gamedir), Q_strlen(pchGameDir) - Q_strlen(mod.gamedir) + 1 );
				}
				else
				{
					Q_strncpy(mod.gamedir, pchGameDir, sizeof(mod.gamedir));
				}
				Q_strlower(mod.gamedir);
				Q_strncpy(mod.description, pchName, sizeof(mod.description));
				mod.appID = g_pAppInformation->GetAppID( i );

				m_ModList.AddToTail(mod);
			}
		}
	}
}
