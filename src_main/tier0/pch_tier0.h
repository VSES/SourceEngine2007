//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//


#if defined(_WIN32) && !defined(_X360)
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0403
#include <windows.h>
#include <assert.h>
#endif

// tier0
#include "tier0/basetypes.h"
#include "tier0/dbgflag.h"
#include "tier0/dbg.h"
#ifdef STEAM
#include "tier0/memhook.h"
#endif
#include "tier0/validator.h"

// First include standard libraries
#include "tier0/valve_off.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <map>
#include <stddef.h>
#ifdef _LINUX
#include <ctype.h>
#include <limits.h>
#define _MAX_PATH PATH_MAX
#endif

#include "tier0/valve_on.h"







