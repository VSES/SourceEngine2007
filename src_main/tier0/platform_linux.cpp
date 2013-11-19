//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "tier0/platform.h"
#include "tier0/vcrmode.h"
#include "tier0/memalloc.h"
#include "tier0/dbg.h"

#include <sys/time.h>
#include <unistd.h>

double Plat_FloatTime()
{
        struct timeval  tp;
        static int      secbase = 0;

        gettimeofday( &tp, NULL );

        if ( !secbase )
        {
                secbase = tp.tv_sec;
                return ( tp.tv_usec / 1000000.0 );
        }

	if (VCRGetMode() == VCR_Disabled)
		return (( tp.tv_sec - secbase ) + tp.tv_usec / 1000000.0 );
	
	return VCRHook_Sys_FloatTime( ( tp.tv_sec - secbase ) + tp.tv_usec / 1000000.0 );
}

unsigned long Plat_MSTime()
{
        struct timeval  tp;
        static int      secbase = 0;

        gettimeofday( &tp, NULL );

        if ( !secbase )
        {
                secbase = tp.tv_sec;
                return ( tp.tv_usec / 1000.0 );
        }

	return (unsigned long)( ( tp.tv_sec - secbase )*1000.0f + tp.tv_usec / 1000.0 );

}



bool vtune( bool resume )
{
}


// -------------------------------------------------------------------------------------------------- //
// Memory stuff.
// -------------------------------------------------------------------------------------------------- //

PLATFORM_INTERFACE void Plat_DefaultAllocErrorFn( unsigned long size )
{
}

typedef void (*Plat_AllocErrorFn)( unsigned long size );
Plat_AllocErrorFn g_AllocError = Plat_DefaultAllocErrorFn;

PLATFORM_INTERFACE void* Plat_Alloc( unsigned long size )
{
	void *pRet = g_pMemAlloc->Alloc( size );
	if ( pRet )
	{
		return pRet;
	}
	else
	{
		g_AllocError( size );
		return 0;
	}
}


PLATFORM_INTERFACE void* Plat_Realloc( void *ptr, unsigned long size )
{
	void *pRet = g_pMemAlloc->Realloc( ptr, size );
	if ( pRet )
	{
		return pRet;
	}
	else
	{
		g_AllocError( size );
		return 0;
	}
}


PLATFORM_INTERFACE void Plat_Free( void *ptr )
{
	g_pMemAlloc->Free( ptr );
}


PLATFORM_INTERFACE void Plat_SetAllocErrorFn( Plat_AllocErrorFn fn )
{
	g_AllocError = fn;
}

static char g_CmdLine[ 2048 ];
PLATFORM_INTERFACE void Plat_SetCommandLine( const char *cmdLine )
{
	strncpy( g_CmdLine, cmdLine, sizeof(g_CmdLine) );
	g_CmdLine[ sizeof(g_CmdLine) -1 ] = 0;
}

PLATFORM_INTERFACE const tchar *Plat_GetCommandLine()
{
	return g_CmdLine;
}
