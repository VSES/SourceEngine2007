//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#ifndef UNICODEFILEHELPERS_H
#define UNICODEFILEHELPERS_H
#ifdef _WIN32
#pragma once
#endif

#include <stdlib.h>

// helper functions for parsing unicode file buffers
wchar_t *AdvanceOverWhitespace(wchar_t *start);
wchar_t *ReadUnicodeToken(wchar_t *start, wchar_t *token, int tokenBufferSize, bool &quoted);
wchar_t *ReadUnicodeTokenNoSpecial(wchar_t *start, wchar_t *token, int tokenBufferSize, bool &quoted);
wchar_t *ReadToEndOfLine(wchar_t *start);

// writing to unicode files via CUtlBuffer
class CUtlBuffer;
void WriteUnicodeString(CUtlBuffer &buffer, const wchar_t *string, bool addQuotes = false);
void WriteAsciiStringAsUnicode(CUtlBuffer &buffer, const char *string, bool addQuotes = false);



#endif // UNICODEFILEHELPERS_H
