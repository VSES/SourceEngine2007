//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#if !defined( PROFILE_H )
#define PROFILE_H
#ifdef _WIN32
#pragma once
#endif

void Host_ReadConfiguration( const bool readDefault = false );
void Host_WriteConfiguration( const char *dirname, const char *filename );

#endif // PROFILE_H