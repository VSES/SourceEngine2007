//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#if !defined(_STATIC_LINKED) || defined(_SHARED_LIB)

#include "stdlib.h"
#include "vstdlib/random.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef _LINUX
#define __cdecl 
#endif

void __cdecl srand(unsigned int)
{
}

int __cdecl rand()
{
	return RandomInt( 0, 0x7fff );
}

#endif // !_STATIC_LINKED || _SHARED_LIB
