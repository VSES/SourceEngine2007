//====== Copyright © 1996-2004, Valve Corporation, All rights reserved. =======
//
// Purpose: packfile manipulation functions
//
//=============================================================================

#ifndef PACKFILE_H
#define PACKFILE_H
#ifdef _WIN32
#pragma once
#endif

#include "tier0/platform.h"

// creates a new empty pack file
bool Pack_CreateFile(const char *pakfile);

// appends an entire file to the pak
bool Pack_AppendFile(const char *pakfile, const char *file);

// clears all the files in the pak
void Pack_Clear(const char *pakfile);

// returns true if the packfile is valid
bool Pack_Validate(const char *pakfile);

// incremental addition of files
// this is stateful, only one file can be appended at one time
bool Pack_StartAppendingFile(const char *pakfile, const char *pFileName);
void Pack_AppendChunkToFile(const void *pv, int bytesToWrite);
int Pack_GetSizeMB();
void Pack_FinishAppendingFile();
int Pack_GetFileSizeMB(const char *pakfile);

#endif // PACKFILE_H
