//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Pool of all per-level strings. Allocates memory for strings, 
//			consolodating duplicates. The memory is freed on behalf of clients
//			at level transition. Strings are of type string_t.
//
// $NoKeywords: $
//=============================================================================//

#ifndef GAMESTRINGPOOL_H
#define GAMESTRINGPOOL_H

#if defined( _WIN32 )
#pragma once
#endif

class IGameSystem;

//-----------------------------------------------------------------------------
// String allocation
//-----------------------------------------------------------------------------
string_t AllocPooledString( const char *pszValue );
string_t FindPooledString( const char *pszValue );

#define AssertIsValidString( s )	AssertMsg( s == NULL_STRING || s == FindPooledString( STRING(s) ), "Invalid string " #s );
		 
//-----------------------------------------------------------------------------
// String system accessor
//-----------------------------------------------------------------------------
IGameSystem *GameStringSystem();

#endif // GAMESTRINGPOOL_H
