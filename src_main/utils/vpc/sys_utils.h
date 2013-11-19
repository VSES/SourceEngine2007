//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
//	SYS_UTILS.H
//
//	System Utilities.
//=====================================================================================//

#pragma once

#define _CRT_SECURE_NO_DEPRECATE 1

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <io.h>
#include <string.h>
#include <stdarg.h>

#define MAX_SYSPRINTMSG		4096
#define MAX_SYSTOKENCHARS	4096

extern void		Sys_Error( const char *format, ... );
extern long		Sys_FileLength( const char* filename, int handle, bool bText = false );
extern int		Sys_LoadFile( const char *filename, void **bufferptr, bool bText = false );
extern void		Sys_StripPath( const char *path, char *outpath );
extern bool		Sys_Exists( const char *filename );
extern char		*Sys_PeekToken( char *dataptr, bool bAllowLineBreaks );
extern char		*Sys_GetToken( char **dataptr, bool crossline, int *numlines );
extern char		*Sys_SkipWhitespace( char *data, bool *hasNewLines, int *numlines );
extern void		Sys_SkipBracedSection( char **dataptr, int *numlines );
extern void		Sys_SkipRestOfLine( char **dataptr, int *numlines );
char			*Sys_SkipToValidToken( char *data, bool *pHasNewLines, int* pNumLines );










