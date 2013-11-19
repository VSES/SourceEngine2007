//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: This file provides some of the goldsrc functionality for xwad.
//
//=============================================================================//

#ifndef GOLDSRC_STANDIN_H
#define GOLDSRC_STANDIN_H
#ifdef _WIN32
#pragma once
#endif


typedef float vec_t;
typedef float vec3_t[3];

typedef unsigned char byte;
typedef int qboolean;


void Msg( const char *pMsg, ... );
void Warning( const char *pMsg, ... );
void Error( const char *pMsg, ... );

int		LoadFile (char *filename, void **bufferptr);
void	SaveFile (char *filename, void *buffer, int count);

short	BigShort (short l);
short	LittleShort (short l);
int		BigLong (int l);
int		LittleLong (int l);
float	BigFloat (float l);
float	LittleFloat (float l);


FILE	*SafeOpenWrite (char *filename);
FILE	*SafeOpenRead (char *filename);
void	SafeRead (FILE *f, void *buffer, int count);
void	SafeWrite (FILE *f, void *buffer, int count);


#endif // GOLDSRC_STANDIN_H
