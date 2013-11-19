//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: dod game stats
//
// $NoKeywords: $
//=============================================================================//

// Some tricky business here - we don't want to include the precompiled header for the statreader
// and trying to #ifdef it out does funky things like ignoring the #endif. Define our header file
// separately and include it based on the switch

#include "cbase.h"

#ifdef GAME_DLL
#endif

#include <tier0/platform.h>
#include "cs_gamestats.h"

float	g_flGameStatsUpdateTime = 0.0f;
short	g_iTerroristVictories[CS_NUM_LEVELS];
short	g_iCounterTVictories[CS_NUM_LEVELS];
short	g_iWeaponPurchases[WEAPON_MAX];

short	g_iAutoBuyPurchases = 0;
short	g_iReBuyPurchases = 0;
short	g_iAutoBuyM4A1Purchases = 0;
short	g_iAutoBuyAK47Purchases = 0;
short	g_iAutoBuyFamasPurchases = 0;
short	g_iAutoBuyGalilPurchases = 0;
short	g_iAutoBuyVestHelmPurchases = 0;
short	g_iAutoBuyVestPurchases = 0;

//If you add or remove from this list, change CS_NUM_LEVELS in cs_gamestats.h
const char *pValidStatLevels[] =
{
	"cs_assault",
	"cs_compound",
	"cs_havana",
	"cs_italy",
	"cs_militia",
	"cs_office",
	"de_aztec",
	"de_cbble",
	"de_chateau",
	"de_dust2",
	"de_dust",
	"de_inferno",
	"de_nuke",
	"de_piranesi",
	"de_port",
	"de_prodigy",
	"de_tides",
	"de_train",
};


int GetCSLevelIndex( const char *pLevelName )
{
	for ( int i = 0; i < CS_NUM_LEVELS; i ++ )
	{
		if ( Q_strcmp( pLevelName, pValidStatLevels[i] ) == 0 )
			return i;
	}

	return -1;
}