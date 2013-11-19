//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef GAMEDATA_H
#define GAMEDATA_H
#ifdef _WIN32
#pragma once
#endif

#pragma warning(push, 1)
#pragma warning(disable:4701 4702 4530)
#include <fstream>
#pragma warning(pop)
#include "TokenReader.h"
#include "GDClass.h"
#include "InputOutput.h"
#include "UtlString.h"
#include "UtlVector.h"


class MDkeyvalue;
class GameData;
class KeyValues;

enum TEXTUREFORMAT;


typedef void (*GameDataMessageFunc_t)(int level, const char *fmt, ...);


#define MAX_DIRECTORY_SIZE	32


//-----------------------------------------------------------------------------
// Purpose: Contains the set of data that is loaded from a single FGD file.
//-----------------------------------------------------------------------------
class GameData
{
	public:

		GameData();
		~GameData();

		BOOL Load(const char *pszFilename);

		GDclass *ClassForName(const char *pszName, int *piIndex = NULL);

		void ClearData();

		inline int GetMaxMapCoord(void);
		inline int GetMinMapCoord(void);

		inline int GetClassCount();
		inline GDclass *GetClass(int nIndex);

	private:

		bool ParseMapSize(TokenReader &tr);

		CUtlVector<GDclass *> m_Classes;

		int m_nMinMapCoord;		// Min & max map bounds as defined by the FGD.
		int m_nMaxMapCoord;
};


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline int GameData::GetClassCount()
{
	return m_Classes.Count();
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
inline GDclass *GameData::GetClass(int nIndex)
{
	if (nIndex >= m_Classes.Count())
		return NULL;
		
	return m_Classes.Element(nIndex);
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int GameData::GetMinMapCoord(void)
{
	return m_nMinMapCoord;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int GameData::GetMaxMapCoord(void)
{
	return m_nMaxMapCoord;
}


void GDSetMessageFunc(GameDataMessageFunc_t pFunc);
bool GDError(TokenReader &tr, char *error, ...);
bool GDSkipToken(TokenReader &tr, trtoken_t ttexpecting = TOKENNONE, const char *pszExpecting = NULL);
bool GDGetToken(TokenReader &tr, char *pszStore, int nSize, trtoken_t ttexpecting = TOKENNONE, const char *pszExpecting = NULL);
bool GDGetTokenDynamic(TokenReader &tr, char **pszStore, trtoken_t ttexpecting, const char *pszExpecting = NULL);


#endif // GAMEDATA_H
