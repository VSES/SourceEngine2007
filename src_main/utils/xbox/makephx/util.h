//====== Copyright © 1996-2005, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

#ifndef UTIL_H
#define UTIL_H
#ifdef _WIN32
#pragma once
#endif

// UNDONE: Share this from somewhere
extern const char *nexttoken(char *token, const char *str, char sep);
extern void UTIL_StringToFloatArray( float *pVector, int count, const char *pString );
extern void UTIL_StringToVector( float *pVector, const char *pString );

#endif // UTIL_H
