//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Thread management routines
//
// $NoKeywords: $
//=============================================================================//

#include "pch_tier0.h"

#include "tier0/valve_off.h"
#ifdef _WIN32
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>
#endif

#include "tier0/platform.h"
#include "tier0/dbg.h"
#include "tier0/threadtools.h"

void Plat_SetThreadName( unsigned long dwThreadID, const char8 *pszName )
{
	ThreadSetDebugName( dwThreadID, pszName );
}

unsigned long Plat_RegisterThread( const tchar *pszName )
{
	ThreadSetDebugName( pszName );
	return ThreadGetCurrentId();
}

unsigned long Plat_GetCurrentThreadID()
{
	return ThreadGetCurrentId();
}
