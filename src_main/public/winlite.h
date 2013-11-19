//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//

#ifndef WINLITE_H
#define WINLITE_H
#pragma once

// 
// Prevent tons of unused windows definitions
//
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#if !defined( _X360 )
#pragma warning(push, 1)
#pragma warning(disable: 4005)
#include <windows.h>
#pragma warning(pop)
#endif
#undef PostMessage

#pragma warning( disable: 4800 )	// forcing value to bool 'true' or 'false' (performance warning)

#endif // WINLITE_H
