//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "gamevars_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// some shared cvars used by game rules
ConVar mp_forcecamera( 
	"mp_forcecamera", 
#ifdef TF_DLL
	"3",
#else
	"1", 
#endif
	FCVAR_REPLICATED,
	"Restricts spectator modes for dead players" );
	
ConVar mp_allowspectators(
	"mp_allowspectators", 
	"1.0", 
	FCVAR_REPLICATED,
	"toggles whether the server allows spectator mode or not" );

ConVar friendlyfire(
	"mp_friendlyfire",
	"0",
	FCVAR_REPLICATED | FCVAR_NOTIFY,
	"Allows team members to injure other members of their team"
	);

ConVar mp_fadetoblack( 
	"mp_fadetoblack", 
	"0", 
	FCVAR_REPLICATED | FCVAR_NOTIFY, 
	"fade a player's screen to black when he dies" );
