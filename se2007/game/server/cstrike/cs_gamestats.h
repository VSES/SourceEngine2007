//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: The dod game stats header
//
// $NoKeywords: $
//=============================================================================//

#ifndef CS_GAMESTATS_H
#define CS_GAMESTATS_H
#ifdef _WIN32
#pragma once
#endif

#include "cs_blackmarket.h"

#define CS_STATS_BLOB_VERSION 3

extern const char *pValidStatLevels[];

#define CS_NUM_LEVELS 18

int GetCSLevelIndex( const char *pLevelName );

typedef struct
{
	char szGameName[8];
	byte iVersion;
	char szMapName[32];
	char ipAddr[4];
	short port;
	int serverid;
} gamestats_header_t;

typedef struct
{
	gamestats_header_t	header;
	short	iMinutesPlayed;

	short	iTerroristVictories[CS_NUM_LEVELS];
	short	iCounterTVictories[CS_NUM_LEVELS];
	short	iBlackMarketPurchases[WEAPON_MAX];

	short	iAutoBuyPurchases;
	short	iReBuyPurchases;
	short	iAutoBuyM4A1Purchases;
	short	iAutoBuyAK47Purchases;
	short	iAutoBuyFamasPurchases;
	short	iAutoBuyGalilPurchases;
	short	iAutoBuyVestHelmPurchases;
	short	iAutoBuyVestPurchases;

} cs_gamestats_t;

extern short	g_iWeaponPurchases[WEAPON_MAX];
extern float	g_flGameStatsUpdateTime;
extern short	g_iTerroristVictories[CS_NUM_LEVELS];
extern short	g_iCounterTVictories[CS_NUM_LEVELS];
extern short	g_iWeaponPurchases[WEAPON_MAX];

extern short	g_iAutoBuyPurchases;
extern short	g_iReBuyPurchases;
extern short	g_iAutoBuyM4A1Purchases;
extern short	g_iAutoBuyAK47Purchases;
extern short	g_iAutoBuyFamasPurchases;
extern short	g_iAutoBuyGalilPurchases;
extern short	g_iAutoBuyVestHelmPurchases;
extern short	g_iAutoBuyVestPurchases;


#endif // CS_GAMESTATS_H