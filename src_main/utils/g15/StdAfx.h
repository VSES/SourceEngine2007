// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#ifdef _WIN32
#include <windows.h>
#endif
#include "imageloader.h"
#include "basetypes.h"
#include "tier0/dbg.h"
#include <malloc.h>
#include <memory.h>
#include "nvtc.h"
#include "mathlib.h"
#include "vector.h"
#include "utlmemory.h"
#include "tier1/strtools.h"
#define ASSERT(x) Assert(x)
#define TRACE(x,y) 
#include "cbase.h"
//#include "cdll_client_int.h"
#ifdef GetTickCount
#undef GetTickCount
#define GetTickCount gpGlobals->tickcount
#endif
//#include <afx.h>

//#include <windows.h>


// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
