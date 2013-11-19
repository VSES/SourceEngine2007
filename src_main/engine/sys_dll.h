//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Expose functions from sys_dll.cpp.
//
// $NoKeywords: $
//=============================================================================//

#ifndef SYS_DLL_H
#define SYS_DLL_H

#ifdef _WIN32
#pragma once
#endif


#include "interface.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IHammer;
class IDataCache;
class IPhysics;
class IMDLCache;
class IMatSystemSurface;
class IAvi;
class IBik;
class IInputSystem;
class IDedicatedExports;
class ISoundEmitterSystemBase;

typedef unsigned short AVIHandle_t;


//-----------------------------------------------------------------------------
// Class factories
//-----------------------------------------------------------------------------
// This factory gets to many of the major app-single systems,
// including the material system, vgui, vgui surface, the file system.
extern CreateInterfaceFn g_AppSystemFactory;

// this factory connect the AppSystemFactory + client.dll + gameui.dll
extern CreateInterfaceFn g_GameSystemFactory;


//-----------------------------------------------------------------------------
// Singleton interfaces
//-----------------------------------------------------------------------------
extern IHammer *g_pHammer;
extern IDataCache *g_pDataCache;
extern IPhysics *g_pPhysics;
extern IMDLCache *g_pMDLCache;
extern IMatSystemSurface *g_pMatSystemSurface;
extern IInputSystem *g_pInputSystem;
extern IAvi *avi;
extern IBik *bik;
extern IDedicatedExports *dedicated;

//-----------------------------------------------------------------------------
// Other singletons
//-----------------------------------------------------------------------------
extern AVIHandle_t g_hCurrentAVI;


inline bool InEditMode()
{
	return g_pHammer != NULL;
}


struct modinfo_t
{
	char szInfo[ 256 ];
	char szDL  [ 256 ];
	char szHLVersion[ 32 ];
	int version;
	int size;
	bool svonly;
	bool cldll;
};

extern modinfo_t gmodinfo;

void LoadEntityDLLs( const char *szBaseDir );
void UnloadEntityDLLs( void );

// This returns true if someone called Error() or Sys_Error() and we're exiting.
// Since we call exit() from inside those, some destructors need to be safe and not crash.
bool IsInErrorExit();

// error message
bool Sys_MessageBox(const char *title, const char *info, bool bShowOkAndCancel);

bool ServerDLL_Load();
void ServerDLL_Unload();

extern CreateInterfaceFn g_ServerFactory;

#endif


