//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ====
//
//=============================================================================

#include <windows.h>
#include <tier0/dbg.h>
#include <io.h>
#include <WorldSize.h>
#include "fgdlib/GameData.h"
#include "fgdlib/HelperInfo.h"
#include "KeyValues.h"
#include "filesystem_tools.h"
#include "tier1/strtools.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#pragma warning(disable:4244)


const int MAX_ERRORS = 5;


static GameDataMessageFunc_t g_pMsgFunc = NULL;


//-----------------------------------------------------------------------------
// Sets the function used for emitting error messages while loading gamedata files.
//-----------------------------------------------------------------------------
void GDSetMessageFunc(GameDataMessageFunc_t pFunc)
{
	g_pMsgFunc = pFunc;
}


//-----------------------------------------------------------------------------
// Purpose: Fetches the next token from the file.
// Input  : tr - 
//			ppszStore - Destination buffer, one of the following:
//				pointer to NULL - token will be placed in an allocated buffer
//				pointer to non-NULL buffer - token will be placed in buffer
//			ttexpecting - 
//			pszExpecting - 
// Output : 
//-----------------------------------------------------------------------------
static bool DoGetToken(TokenReader &tr, char **ppszStore, int nSize, trtoken_t ttexpecting, const char *pszExpecting)
{
	trtoken_t ttype;

	if (*ppszStore != NULL)
	{
		// Reads the token into the given buffer.
		ttype = tr.NextToken(*ppszStore, nSize);
	}
	else
	{
		// Allocates a buffer to hold the token.
		ttype = tr.NextTokenDynamic(ppszStore);
	}

	if (ttype == TOKENSTRINGTOOLONG)
	{
		GDError(tr, "unterminated string or string too long");
		return false;
	}

	//
	// Check for a bad token type.
	//
	char *pszStore = *ppszStore;
	bool bBadTokenType = false;
	if ((ttype != ttexpecting) && (ttexpecting != TOKENNONE))
	{
		//
		// If we were expecting a string and got an integer, don't worry about it.
		// We can translate from integer to string.
		//
		if (!((ttexpecting == STRING) && (ttype == INTEGER)))
		{
			bBadTokenType = true;
		}
	}

	if (bBadTokenType && (pszExpecting == NULL))
	{
		//
		// We didn't get the expected token type but no expected
		// string was specified.
		//
		char *pszTokenName;
		switch (ttexpecting)
		{
			case IDENT:
			{
				pszTokenName = "identifier";
				break;
			}

			case INTEGER:
			{
				pszTokenName = "integer";
				break;
			}

			case STRING:
			{
				pszTokenName = "string";
				break;
			}

			case OPERATOR:
			default:
			{
				pszTokenName = "symbol";
				break;
			}
		}
		
		GDError(tr, "expecting %s", pszTokenName);
		return false;
	}
	else if (bBadTokenType || ((pszExpecting != NULL) && !IsToken(pszStore, pszExpecting)))
	{
		//
		// An expected string was specified, and we got either the wrong type or
		// the right type but the wrong string,
		//
		GDError(tr, "expecting '%s', but found '%s'", pszExpecting, pszStore);
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : tr - 
//			error - 
// Output : 
//-----------------------------------------------------------------------------
bool GDError(TokenReader &tr, char *error, ...)
{
	char szBuf[128];
	va_list vl;
	va_start(vl, error);
	vsprintf(szBuf, error, vl);
	va_end(vl);

	if (g_pMsgFunc)
	{
		// HACK: should use an enumeration for error level
		g_pMsgFunc(1, tr.Error(szBuf));
	}
	
	if (tr.GetErrorCount() >= MAX_ERRORS)
	{
		if (g_pMsgFunc)
		{
			// HACK: should use an enumeration for error level
			g_pMsgFunc(1, "   - too many errors; aborting.");
		}
		
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Fetches the next token from the file.
// Input  : tr - The token reader object with which to fetch the token.
//			pszStore - Buffer in which to place the token, NULL to discard the token.
//			ttexpecting - The token type that we are expecting. If this is not TOKENNONE
//				and token type read is different, the operation will fail.
//			pszExpecting - The token string that we are expecting. If this string
//				is not NULL and the token string read is different, the operation will fail.
// Output : Returns TRUE if the operation succeeded, FALSE if there was an error.
//			If there was an error, the error will be reported in the message window.
//-----------------------------------------------------------------------------
bool GDGetToken(TokenReader &tr, char *pszStore, int nSize, trtoken_t ttexpecting, const char *pszExpecting)
{
	Assert(pszStore != NULL);
	if (pszStore != NULL)
	{
		return DoGetToken(tr, &pszStore, nSize, ttexpecting, pszExpecting);
	}

	return false;
}


//-----------------------------------------------------------------------------
// Purpose: Fetches the next token from the file.
// Input  : tr - The token reader object with which to fetch the token.
//			pszStore - Buffer in which to place the token, NULL to discard the token.
//			ttexpecting - The token type that we are expecting. If this is not TOKENNONE
//				and token type read is different, the operation will fail.
//			pszExpecting - The token string that we are expecting. If this string
//				is not NULL and the token string read is different, the operation will fail.
// Output : Returns TRUE if the operation succeeded, FALSE if there was an error.
//			If there was an error, the error will be reported in the message window.
//-----------------------------------------------------------------------------
bool GDSkipToken(TokenReader &tr, trtoken_t ttexpecting, const char *pszExpecting)
{
	//
	// Read the next token into a buffer and discard it.
	//
	char szDiscardBuf[MAX_TOKEN];
	char *pszDiscardBuf = szDiscardBuf;
	return DoGetToken(tr, &pszDiscardBuf, sizeof(szDiscardBuf), ttexpecting, pszExpecting);
}


//-----------------------------------------------------------------------------
// Purpose: Fetches the next token from the file, allocating a buffer exactly
//			large enough to hold the token.
// Input  : tr - 
//			ppszStore - 
//			ttexpecting - 
//			pszExpecting - 
// Output : 
//-----------------------------------------------------------------------------
bool GDGetTokenDynamic(TokenReader &tr, char **ppszStore, trtoken_t ttexpecting, const char *pszExpecting)
{
	if (ppszStore == NULL)
	{
		return false;
	}

	*ppszStore = NULL;
	return DoGetToken(tr, ppszStore, -1, ttexpecting, pszExpecting);
}


//-----------------------------------------------------------------------------
// Purpose: Constructor.
//-----------------------------------------------------------------------------
GameData::GameData(void)
{
	m_nMaxMapCoord = 8192;
	m_nMinMapCoord = -8192;
}


//-----------------------------------------------------------------------------
// Purpose: Destructor.
//-----------------------------------------------------------------------------
GameData::~GameData(void)
{
	ClearData();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void GameData::ClearData(void)
{
	// delete classes.
	int nCount = m_Classes.Count();
	for (int i = 0; i < nCount; i++)
	{
		GDclass *pm = m_Classes.Element(i);
		delete pm;
	}
	m_Classes.RemoveAll();
}


//-----------------------------------------------------------------------------
// Purpose: Loads a gamedata (FGD) file into this object.
// Input  : pszFilename - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL GameData::Load(const char *pszFilename)
{
	TokenReader tr;

	if(GetFileAttributes(pszFilename) == 0xffffffff)
		return FALSE;

	if(!tr.Open(pszFilename))
		return FALSE;

	trtoken_t ttype;
	char szToken[128];

	while (1)
	{
		if (tr.GetErrorCount() >= MAX_ERRORS)
		{
			break;
		}

		ttype = tr.NextToken(szToken, sizeof(szToken));

		if(ttype == TOKENEOF)
			break;

		if(ttype != OPERATOR || !IsToken(szToken, "@"))
		{
			if(!GDError(tr, "expected @"))
				return FALSE;
		}

		// check what kind it is, and parse a new object
		if (tr.NextToken(szToken, sizeof(szToken)) != IDENT)
		{
			if(!GDError(tr, "expected identifier after @"))
				return FALSE;
		}

		if (IsToken(szToken, "baseclass") || IsToken(szToken, "pointclass") || IsToken(szToken, "solidclass") || IsToken(szToken, "keyframeclass") || IsToken(szToken, "moveclass") || IsToken(szToken, "npcclass") || IsToken(szToken, "filterclass"))
		{
			//
			// New class.
			//
			GDclass *pNewClass = new GDclass;
			if (!pNewClass->InitFromTokens(tr, this))
			{
				tr.IgnoreTill(OPERATOR, "@");	// go to next section
				delete pNewClass;
			}
			else
			{
				if (IsToken(szToken, "baseclass"))			// Not directly available to user.
				{
					pNewClass->SetBaseClass(true);
				}
				else if (IsToken(szToken, "pointclass"))	// Generic point class.
				{
					pNewClass->SetPointClass(true);
				}
				else if (IsToken(szToken, "solidclass"))	// Tied to solids.
				{
					pNewClass->SetSolidClass(true);
				}
				else if (IsToken(szToken, "npcclass"))		// NPC class - can be spawned by npc_maker.
				{
					pNewClass->SetPointClass(true);
					pNewClass->SetNPCClass(true);
				}
				else if (IsToken(szToken, "filterclass"))	// Filter class - can be used as a filter
				{
					pNewClass->SetPointClass(true);
					pNewClass->SetFilterClass(true);
				}
				else if (IsToken(szToken, "moveclass"))		// Animating
				{
					pNewClass->SetMoveClass(true);
					pNewClass->SetPointClass(true);
				}
				else if (IsToken(szToken, "keyframeclass"))	// Animation keyframes
				{
					pNewClass->SetKeyFrameClass(true);
					pNewClass->SetPointClass(true);
				}

				// Check and see if this new class matches an existing one. If so we will override the previous definition.
				int nExistingClassIndex = 0;
				GDclass *pExistingClass = ClassForName(pNewClass->GetName(), &nExistingClassIndex);
				if (NULL != pExistingClass)
				{
					m_Classes.InsertAfter(nExistingClassIndex, pNewClass);
					m_Classes.Remove(nExistingClassIndex);
				}
				else
				{
					m_Classes.AddToTail(pNewClass);
				}
			}
		}
		else if (IsToken(szToken, "include"))
		{
			if (GDGetToken(tr, szToken, sizeof(szToken), STRING))
			{
				// Let's assume it's in the same directory.
				char justPath[MAX_PATH], loadFilename[MAX_PATH];
				if ( Q_ExtractFilePath( pszFilename, justPath, sizeof( justPath ) ) )
				{
					Q_snprintf( loadFilename, sizeof( loadFilename ), "%s%s", justPath, szToken );
				}
				else
				{
					Q_strncpy( loadFilename, szToken, sizeof( loadFilename ) );
				}

				// First try our fully specified directory
				if (!Load(loadFilename))
				{
					// Failing that, try our start directory
					if (!Load(szToken))
					{
						GDError(tr, "error including file: %s", szToken);
					}
				}
			}
		}
		else if (IsToken(szToken, "mapsize"))
		{
			if (!ParseMapSize(tr))
			{
				// Error in map size specifier, skip to next @ sign. 
				tr.IgnoreTill(OPERATOR, "@");
			}
		}
		else
		{
			GDError(tr, "unrecognized section name %s", szToken);
			tr.IgnoreTill(OPERATOR, "@");
		}
	}

	if (tr.GetErrorCount() > 0)
	{
		return FALSE;
	}

	tr.Close();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Purpose: Parses the "mapsize" specifier, which should be of the form:
//
//			mapsize(min, max)
//
//			ex: mapsize(-8192, 8192)
//
// Input  : tr - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool GameData::ParseMapSize(TokenReader &tr)
{
	if (!GDSkipToken(tr, OPERATOR, "("))
	{
		return false;
	}

	char szToken[128];
	if (!GDGetToken(tr, szToken, sizeof(szToken), INTEGER))
	{
		return false;
	}
	int nMin = atoi(szToken);	

	if (!GDSkipToken(tr, OPERATOR, ","))
	{
		return false;
	}

	if (!GDGetToken(tr, szToken, sizeof(szToken), INTEGER))
	{
		return false;
	}
	int nMax = atoi(szToken);	

	if (nMin != nMax)
	{
		m_nMinMapCoord = min(nMin, nMax);
		m_nMaxMapCoord = max(nMin, nMax);
	}

	if (!GDSkipToken(tr, OPERATOR, ")"))
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : pszName - 
//			piIndex - 
// Output : 
//-----------------------------------------------------------------------------
GDclass *GameData::ClassForName(const char *pszName, int *piIndex)
{
	int nCount = m_Classes.Count();
	for (int i = 0; i < nCount; i++)
	{
		GDclass *mp = m_Classes.Element(i);
		if(!strcmp(mp->GetName(), pszName))
		{
			if(piIndex)
				piIndex[0] = i;
			return mp;
		}
	}

	return NULL;
}


// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgoff.h"
