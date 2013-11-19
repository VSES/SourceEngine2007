//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#ifndef SYSEXTERNAL_H
#define SYSEXTERNAL_H
#ifdef _WIN32
#pragma once
#endif

// an error will cause the entire program to exit
void Sys_Error(const char *psz, ...);

#endif // SYSEXTERNAL_H
