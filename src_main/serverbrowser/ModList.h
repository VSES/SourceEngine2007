//========= Copyright © 1996-2001, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#ifndef MODLIST_H
#define MODLIST_H
#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Purpose: Handles parsing of half-life directory for mod info
//-----------------------------------------------------------------------------
class CModList
{
public:
	CModList();

	// returns number of mods 
	int ModCount();

	// returns the full name of the mod, index valid in range [0, ModCount)
	const char *GetModName(int index);

	// returns mod directory string
	const char *GetModDir(int index);

	int GetAppID( int index ) const;

	// returns the mod name for the associated gamedir
	const char *GetModNameForModDir(const char *gamedir, int iAppID );

private:
	struct mod_t
	{
		char description[64];
		char gamedir[64];
		int	appID;
	};

	static int ModNameCompare( const mod_t *pLeft, const mod_t *pRight );
	void ParseInstalledMods();
	void ParseSteamMods();

	CUtlVector<mod_t> m_ModList;
};

// singleton accessor
extern CModList &ModList();


#endif // MODLIST_H
