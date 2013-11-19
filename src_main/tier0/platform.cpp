//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "pch_tier0.h"
#include <time.h>

#ifdef _LINUX
#include "platform_linux.cpp"
#else
#if defined(_WIN32) && !defined(_X360)
#define WINDOWS_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0403
#include <windows.h>
#endif
#include <assert.h>
#include "tier0/platform.h"
#include "xbox/xbox_console.h"
#ifndef _X360
#include "tier0/vcrmode.h"
#endif
#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)
#include "tier0/memalloc.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
#endif

#ifndef _X360
extern VCRMode_t g_VCRMode;
#endif
static LARGE_INTEGER g_PerformanceFrequency;
static LARGE_INTEGER g_MSPerformanceFrequency;
static LARGE_INTEGER g_ClockStart;

static void InitTime()
{
	if( !g_PerformanceFrequency.QuadPart )
	{
		QueryPerformanceFrequency(&g_PerformanceFrequency);
		g_MSPerformanceFrequency.QuadPart = g_PerformanceFrequency.QuadPart / 1000;
		QueryPerformanceCounter(&g_ClockStart);
	}
}

double Plat_FloatTime()
{
	InitTime();

	LARGE_INTEGER CurrentTime;

	QueryPerformanceCounter( &CurrentTime );

	double fRawSeconds = (double)( CurrentTime.QuadPart - g_ClockStart.QuadPart ) / (double)(g_PerformanceFrequency.QuadPart);

#if !defined(STEAM) && !defined(_X360)
	if (g_VCRMode == VCR_Disabled)
		return fRawSeconds;
	
	return VCRHook_Sys_FloatTime( fRawSeconds );
#else
	return fRawSeconds;
#endif
}

unsigned long Plat_MSTime()
{
	InitTime();

	LARGE_INTEGER CurrentTime;

	QueryPerformanceCounter( &CurrentTime );

	return (unsigned long) ( ( CurrentTime.QuadPart - g_ClockStart.QuadPart ) / g_MSPerformanceFrequency.QuadPart);
}

void GetCurrentDate( int *pDay, int *pMonth, int *pYear )
{
	struct tm *pNewTime;
	time_t long_time;

	time( &long_time );                /* Get time as long integer. */
	pNewTime = localtime( &long_time ); /* Convert to local time. */

	*pDay = pNewTime->tm_mday;
	*pMonth = pNewTime->tm_mon + 1;
	*pYear = pNewTime->tm_year + 1900;
}

bool vtune( bool resume )
{
#ifndef _X360
	static bool bInitialized = false;
	static void (__cdecl *VTResume)(void) = NULL;
	static void (__cdecl *VTPause) (void) = NULL;

	// Grab the Pause and Resume function pointers from the VTune DLL the first time through:
	if( !bInitialized )
	{
		bInitialized = true;

		HINSTANCE pVTuneDLL = LoadLibrary( "vtuneapi.dll" );

		if( pVTuneDLL )
		{
			VTResume = (void(__cdecl *)())GetProcAddress( pVTuneDLL, "VTResume" );
			VTPause  = (void(__cdecl *)())GetProcAddress( pVTuneDLL, "VTPause" );
		}
	}

	// Call the appropriate function, as indicated by the argument:
	if( resume && VTResume )
	{
		VTResume();
		return true;

	} 
	else if( !resume && VTPause )
	{
		VTPause();
		return true;
	}
#endif
	return false;
}

bool Plat_IsInDebugSession()
{
#if defined( _WIN32 ) && !defined( _X360 )
	return (IsDebuggerPresent() != 0);
#elif defined( _WIN32 ) && defined( _X360 )
	return (XBX_IsDebuggerPresent() != 0);
#else
	return false;
#endif
}

void Plat_DebugString( const char * psz )
{
#if defined( _WIN32 ) && !defined( _X360 )
	::OutputDebugStringA( psz );
#elif defined( _WIN32 ) && defined( _X360 )
	XBX_OutputDebugString( psz );
#endif
}


const tchar *Plat_GetCommandLine()
{
#ifdef TCHAR_IS_WCHAR
	return GetCommandLineW();
#else
	return GetCommandLine();
#endif
}

const char *Plat_GetCommandLineA()
{
	return GetCommandLineA();
}

// -------------------------------------------------------------------------------------------------- //
// Memory stuff. 
//
// DEPRECATED. Still here to support binary back compatability of tier0.dll
//
// -------------------------------------------------------------------------------------------------- //
#ifndef _X360
#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)

typedef void (*Plat_AllocErrorFn)( unsigned long size );

void Plat_DefaultAllocErrorFn( unsigned long size )
{
}

Plat_AllocErrorFn g_AllocError = Plat_DefaultAllocErrorFn;
#endif

#ifndef _X360
CRITICAL_SECTION g_AllocCS;
class CAllocCSInit
{
public:
	CAllocCSInit()
	{
		InitializeCriticalSection( &g_AllocCS );
	}
} g_AllocCSInit;
#endif

#ifndef _X360
PLATFORM_INTERFACE void* Plat_Alloc( unsigned long size )
{
	EnterCriticalSection( &g_AllocCS );
#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)
		void *pRet = g_pMemAlloc->Alloc( size );
#else
		void *pRet = malloc( size );
#endif
	LeaveCriticalSection( &g_AllocCS );
	if ( pRet )
	{
		return pRet;
	}
	else
	{
#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)
		g_AllocError( size );
#endif
		return 0;
	}
}
#endif

#ifndef _X360
PLATFORM_INTERFACE void* Plat_Realloc( void *ptr, unsigned long size )
{
	EnterCriticalSection( &g_AllocCS );
#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)
		void *pRet = g_pMemAlloc->Realloc( ptr, size );
#else
		void *pRet = realloc( ptr, size );
#endif
	LeaveCriticalSection( &g_AllocCS );
	if ( pRet )
	{
		return pRet;
	}
	else
	{
#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)
		g_AllocError( size );
#endif
		return 0;
	}
}
#endif

#ifndef _X360
PLATFORM_INTERFACE void Plat_Free( void *ptr )
{
	EnterCriticalSection( &g_AllocCS );
#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)
		g_pMemAlloc->Free( ptr );
#else
		free( ptr );
#endif
	LeaveCriticalSection( &g_AllocCS );
}
#endif

#ifndef _X360
#if !defined(STEAM) && !defined(NO_MALLOC_OVERRIDE)
PLATFORM_INTERFACE void Plat_SetAllocErrorFn( Plat_AllocErrorFn fn )
{
	g_AllocError = fn;
}
#endif
#endif

#endif

#endif // _LINUX
