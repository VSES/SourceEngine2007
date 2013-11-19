//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "pch_tier0.h"

#include "tier0/minidump.h"
#include "tier0/platform.h"

#if defined( _WIN32 ) && !defined( _X360 )

#if _MSC_VER >= 1300
#include "tier0/valve_off.h"
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>

#include <time.h>

// MiniDumpWriteDump() function declaration (so we can just get the function directly from windows)
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)
	(
	HANDLE hProcess, 
	DWORD dwPid, 
	HANDLE hFile, 
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);


// true if we're currently writing a minidump caused by an assert
static bool g_bWritingNonfatalMinidump = false;
// counter used to make sure minidump names are unique
static int g_nMinidumpsWritten = 0;

//-----------------------------------------------------------------------------
// Purpose: Creates a new file and dumps the exception info into it
// Input  : uStructuredExceptionCode	- windows exception code, unused.
//			pExceptionInfo				- call stack.
//			minidumpType				- type of minidump to write.
//			ptchMinidumpFileNameBuffer	- if not-NULL points to a writable tchar buffer
//										  of length at least _MAX_PATH to contain the name
//										  of the written minidump file on return.
//-----------------------------------------------------------------------------
bool WriteMiniDumpUsingExceptionInfo( 
	unsigned int uStructuredExceptionCode, 
	_EXCEPTION_POINTERS * pExceptionInfo, 
	MINIDUMP_TYPE minidumpType,
	tchar *ptchMinidumpFileNameBuffer /* = NULL */
	)
{
	if ( ptchMinidumpFileNameBuffer )
	{
		*ptchMinidumpFileNameBuffer = tchar( 0 );
	}

	// get the function pointer directly so that we don't have to include the .lib, and that
	// we can easily change it to using our own dll when this code is used on win98/ME/2K machines
	HMODULE hDbgHelpDll = ::LoadLibrary( "DbgHelp.dll" );
	if ( !hDbgHelpDll )
		return false;

	bool bReturnValue = false;
	MINIDUMPWRITEDUMP pfnMiniDumpWrite = (MINIDUMPWRITEDUMP) ::GetProcAddress( hDbgHelpDll, "MiniDumpWriteDump" );

	if ( pfnMiniDumpWrite )
	{
		// create a unique filename for the minidump based on the current time and module name
		time_t currTime = ::time( NULL );
		struct tm * pTime = ::localtime( &currTime );
		++g_nMinidumpsWritten;

		// strip off the rest of the path from the .exe name
		tchar rgchModuleName[MAX_PATH];
#ifdef TCHAR_IS_WCHAR
		::GetModuleFileNameW( NULL, rgchModuleName, sizeof(rgchModuleName) / sizeof(tchar) );
#else
		::GetModuleFileName( NULL, rgchModuleName, sizeof(rgchModuleName) / sizeof(tchar) );
#endif
		tchar *pch = _tcsrchr( rgchModuleName, '.' );
		if ( pch )
		{
			*pch = 0;
		}
		pch = _tcsrchr( rgchModuleName, '\\' );
		if ( pch )
		{
			// move past the last slash
			pch++;
		}
		else
		{
			pch = _T("unknown");
		}

		
		// can't use the normal string functions since we're in tier0
		tchar rgchFileName[MAX_PATH];
		_sntprintf( rgchFileName, sizeof(rgchFileName) / sizeof(tchar),
			_T("%s_%s_%d%.2d%2d%.2d%.2d%.2d_%d.mdmp"),
			pch,
			g_bWritingNonfatalMinidump ? "assert" : "crash",
			pTime->tm_year + 1900,	/* Year less 2000 */
			pTime->tm_mon + 1,		/* month (0 - 11 : 0 = January) */
			pTime->tm_mday,			/* day of month (1 - 31) */
			pTime->tm_hour,			/* hour (0 - 23) */
			pTime->tm_min,		    /* minutes (0 - 59) */
			pTime->tm_sec,		    /* seconds (0 - 59) */
			g_nMinidumpsWritten		// ensures the filename is unique
			);

		BOOL bMinidumpResult = FALSE;
#ifdef TCHAR_IS_WCHAR
		HANDLE hFile = ::CreateFileW( rgchFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
#else
		HANDLE hFile = ::CreateFile( rgchFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
#endif

		if ( hFile )
		{
			// dump the exception information into the file
			_MINIDUMP_EXCEPTION_INFORMATION	ExInfo;
			ExInfo.ThreadId	= ::GetCurrentThreadId();
			ExInfo.ExceptionPointers = pExceptionInfo;
			ExInfo.ClientPointers = FALSE;

			bMinidumpResult = (*pfnMiniDumpWrite)( ::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, minidumpType, &ExInfo, NULL, NULL );
			::CloseHandle( hFile );

			if ( bMinidumpResult )
			{
				bReturnValue = true;

				if ( ptchMinidumpFileNameBuffer )
				{
					// Copy the file name from "pSrc = rgchFileName" into "pTgt = ptchMinidumpFileNameBuffer"
					tchar *pTgt = ptchMinidumpFileNameBuffer;
					tchar const *pSrc = rgchFileName;
					while ( ( *( pTgt ++ ) = *( pSrc ++ ) ) != tchar( 0 ) )
						continue;
				}
			}

			// fall through to trying again
		}
		
		// mark any failed minidump writes by renaming them
		if ( !bMinidumpResult )
		{
			tchar rgchFailedFileName[_MAX_PATH];
			_sntprintf( rgchFailedFileName, sizeof(rgchFailedFileName) / sizeof(tchar), "(failed)%s", rgchFileName );
			rename( rgchFileName, rgchFailedFileName );
		}
	}

	::FreeLibrary( hDbgHelpDll );

	// call the log flush function if one is registered to try to flush any logs
	//CallFlushLogFunc();

	return bReturnValue;
}


void InternalWriteMiniDumpUsingExceptionInfo( unsigned int uStructuredExceptionCode, _EXCEPTION_POINTERS * pExceptionInfo )
{
	// First try to write it with all the indirectly referenced memory (ie: a large file).
	// If that doesn't work, then write a smaller one.
	int iType = MiniDumpWithDataSegs | MiniDumpWithIndirectlyReferencedMemory;
	if ( !WriteMiniDumpUsingExceptionInfo( uStructuredExceptionCode, pExceptionInfo, (MINIDUMP_TYPE)iType ) )
	{
		iType = MiniDumpWithDataSegs;
		WriteMiniDumpUsingExceptionInfo( uStructuredExceptionCode, pExceptionInfo, (MINIDUMP_TYPE)iType );
	}
}

// minidump function to use
static FnMiniDump g_pfnWriteMiniDump = InternalWriteMiniDumpUsingExceptionInfo;

//-----------------------------------------------------------------------------
// Purpose: Set a function to call which will write our minidump, overriding
//			the default function
// Input  : pfn -		Pointer to minidump function to set
// Output :				Previously set function
//-----------------------------------------------------------------------------
FnMiniDump SetMiniDumpFunction( FnMiniDump pfn )
{
	FnMiniDump pfnTemp = g_pfnWriteMiniDump;
	g_pfnWriteMiniDump = pfn;
	return pfnTemp;
}

//-----------------------------------------------------------------------------
// Purpose: writes out a minidump from the current process
//-----------------------------------------------------------------------------
void WriteMiniDump()
{
	// throw an exception so we can catch it and get the stack info
	g_bWritingNonfatalMinidump = true;
	__try
	{
		::RaiseException
			(
			0,							// dwExceptionCode
			EXCEPTION_NONCONTINUABLE,	// dwExceptionFlags
			0,							// nNumberOfArguments,
			NULL						// const ULONG_PTR* lpArguments
			);

		// Never get here (non-continuable exception)
	}
	// Write the minidump from inside the filter (GetExceptionInformation() is only 
	// valid in the filter)
	__except ( g_pfnWriteMiniDump( 0, GetExceptionInformation() ), EXCEPTION_EXECUTE_HANDLER )
	{
	}
	g_bWritingNonfatalMinidump = false;
}

DBG_OVERLOAD bool g_bInException = false;

//-----------------------------------------------------------------------------
// Purpose: Catches and writes out any exception throw by the specified function
//-----------------------------------------------------------------------------
void CatchAndWriteMiniDump( FnWMain pfn, int argc, tchar *argv[] )
{
	if ( Plat_IsInDebugSession() )
	{
		// don't mask exceptions when running in the debugger
		pfn( argc, argv );
	}
	else
	{
		try
		{
			_set_se_translator( g_pfnWriteMiniDump );
			pfn( argc, argv );
		}
		catch (...)
		{
			g_bInException = true;
			DMsg( "console", 1, _T("Fatal exception caught, minidump written\n") );
			// handle everything and just quit, we've already written out our minidump
		}
	}
}

#else

PLATFORM_INTERFACE void WriteMiniDump()
{
}

PLATFORM_INTERFACE void CatchAndWriteMiniDump( FnWMain pfn, int argc, tchar *argv[] )
{
	pfn( argc, argv );
}

#endif
#elif defined(_X360 )
PLATFORM_INTERFACE void WriteMiniDump()
{
	DmCrashDump(false);
}

#else // !_WIN32
#include "tier0/minidump.h"

PLATFORM_INTERFACE void WriteMiniDump()
{
}

PLATFORM_INTERFACE void CatchAndWriteMiniDump( FnWMain pfn, int argc, tchar *argv[] )
{
	pfn( argc, argv );
}

#endif 
