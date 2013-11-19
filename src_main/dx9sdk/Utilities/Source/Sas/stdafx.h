//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#ifndef STRICT
#define STRICT
#endif

// If app hasn't choosen, set to work with Windows 98, Windows Me, Windows 2000, Windows XP and beyond
#ifndef WINVER
#define WINVER         0x0410
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410 
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0500 
#endif

//NONSTANDARD EXTENSIONS WARNINGS
#pragma warning(disable: 4201)
#pragma warning(disable: 4238)
#pragma warning(disable: 4239)
//END-NONSTANDARD EXTENSIONS WARNINGS
//UNREFERENCED PARAMTER WARNING WARNINGS //some multiuse interface implimentations do not make use
#pragma warning(disable: 4100)
/*
// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
#endif
*/
//#include <windows.h>
#include <assert.h>
#include <wchar.h>
//#include <mmsystem.h>
//#include <commctrl.h> // for InitCommonControls() 
//#include <shellapi.h> // for ExtractIcon()
//#include <new.h>      // for placement new
#include <math.h>      
#include <limits.h>      

#include <stdio.h>
#include <stdarg.h>


#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <malloc.h>
#include <windows.h>

// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) || defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>


#ifndef STRSAFE_NO_DEPRECATE
#pragma deprecated("strncpy")
#pragma deprecated("wcsncpy")
#pragma deprecated("_tcsncpy")
#pragma deprecated("wcsncat")
#pragma deprecated("strncat")
#pragma deprecated("_tcsncat")
#endif

#pragma warning( disable : 4995 ) // disable deprecated warning 
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <string>
#pragma warning( default : 4995 ) 
#pragma warning( default : 4996 ) 



#define SAS_STRINGSIZE 1024

#ifndef SAFE_DELETE
    #define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
    #define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
	#ifndef DEBUG_REFS
		#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
	#else
		#define SAFE_RELEASE(p)  \
		{ \
			if(p) \
			{ \
				UINT __ref = (p)->Release(); \
				(p)=NULL; \
				Sas::Host::Out( Sas::LEVEL_DEBUG, L"Release Ref: '%d', Variable: '%s' Function: '%s' File: '%s', Line: '%d'", __ref, TEXT( #p ),  TEXT( __FUNCTION__ ) ,  TEXT( __FILE__ ), __LINE__ ); \
			} \
		}
	#endif
#endif

#ifndef CHECK_REF
	#ifndef DEBUG_REFS
		#define CHECK_REF(p)
	#else
		#define CHECK_REF(p)  \
		{ \
			if(p) \
			{ \
				(p)->AddRef(); \
				UINT __ref = (p)->Release(); \
				Sas::Host::Out( Sas::LEVEL_DEBUG, L"Check Ref: '%d', Variable: '%s' Function: '%s' File: '%s', Line: '%d'", __ref, TEXT( #p ),  TEXT( __FUNCTION__ ) ,  TEXT( __FILE__ ), __LINE__ ); \
			} else \
				Sas::Host::Out( Sas::LEVEL_DEBUG, L"Check Ref: nil, Variable: '%s' Function: '%s' File: '%s', Line: '%d'", TEXT( #p ),  TEXT( __FUNCTION__ ) ,  TEXT( __FILE__ ), __LINE__ ); \
		}
	#endif
#endif
#ifndef LOOPSAFE_DELETE	
	#define LOOPSAFE_DELETE(p, c)       { if(p) { c* __tmp = (c*)(p); (p) = NULL; delete ((c*)(__tmp)); __tmp = NULL; } }
#endif 

//WCHAR* DuplicateString( const WCHAR* str );
void CharToWString( const CHAR* source, std::wstring& dest );
void WCharToString( const WCHAR* source, std::string& dest );
void WCharToWString( const WCHAR* source, std::wstring& dest );
void CharToString( const CHAR* source, std::string& dest );

struct StringCompareInsensitive
{
	bool operator()(const std::wstring& s1, const std::wstring& s2) const
	{
		int compare = _wcsicmp(s1.c_str(), s2.c_str());
		return (compare < 0);
	}
};

#endif