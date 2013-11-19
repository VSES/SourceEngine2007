#ifndef __STDLCD_H__
#define __STDLCD_H__

#ifdef _WIN32
#define _WINCON_	// skip console definitions
#define _WINREG_	// skip registry definitions
#include <windows.h>
//Valve requires tier0 headers before tchar.h
//#include <tchar.h> 
#endif

#define LOGIASSERT	__noop
#define LOGITRACE	__noop

//Add your own definitions here
#include "bitmap/bitmap.h"
#include "basetypes.h"
#include "tier0/dbg.h"
#include <malloc.h>
#include <memory.h>
#include "nvtc.h"
// #include "mathlib.h"
//#include "vector.h"
#include "utlmemory.h"
#include "tier1/strtools.h"

#endif