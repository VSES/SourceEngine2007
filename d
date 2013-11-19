[33mcommit 9cbf1e751330ce71d00a81270160250a1a2bd4fb[m
Author: LestaD <lestad@bk.ru>
Date:   Tue Nov 19 23:47:43 2013 +0400

    asdasd

[1mdiff --git a/se2007/engine/DevShotGenerator.cpp b/se2007/engine/DevShotGenerator.cpp[m
[1mnew file mode 100644[m
[1mindex 0000000..450443f[m
[1m--- /dev/null[m
[1m+++ b/se2007/engine/DevShotGenerator.cpp[m
[36m@@ -0,0 +1,149 @@[m
[32m+[m[32m//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//[m
[32m+[m[32m//[m
[32m+[m[32m// Purpose:[m[41m [m
[32m+[m[32m//[m
[32m+[m[32m//=============================================================================//[m
[32m+[m
[32m+[m[32m#include "DevShotGenerator.h"[m
[32m+[m[32m#include "cmd.h"[m
[32m+[m[32m#include "fmtstr.h"[m
[32m+[m[32m#include "host.h"[m
[32m+[m[32m#include "tier0/icommandline.h"[m
[32m+[m[32m#include <tier0/dbg.h>[m
[32m+[m
[32m+[m[32m// memdbgon must be the last include file in a .cpp file!!![m
[32m+[m[32m#include "tier0/memdbgon.h"[m
[32m+[m
[32m+[m[32m#define PAUSE_FRAMES_BETWEEN_MAPS	300[m
[32m+[m[32m#define PAUSE_TIME_BETWEEN_MAPS		2.0f[m
[32m+[m
[32m+[m[32m//-----------------------------------------------------------------------------[m
[32m+[m[32m// Purpose:[m[41m [m
[32m+[m[32m//-----------------------------------------------------------------------------[m
[32m+[m[32mvoid DevShotGenerator_Usage()[m
[32m+[m[32m{[m
[32m+[m	[32m//[m[41m [m
[32m+[m	[32mMsg( "-makedevshots usage:\n" );[m
[32m+[m	[32mMsg( "  [ -usedevshotsfile filename ] -- get map list from specified file, default is to build for maps/*.bsp\n" );[m
[32m+[m	[32mMsg( "  [ -startmap mapname ] -- restart generation at specified map (after crash, implies resume)\n" );[m
[32m+[m	[32mMsg( "  [ -condebug ] -- prepend console.log entries with mapname or engine if not in a map\n" );[m
[32m+[m	[32mMsg( "  [ +map mapname ] -- generate devshots for specified map and exit after that map\n" );[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid DevShotGenerator_Init()[m
[32m+[m[32m{[m
[32m+[m	[32m// check for devshot generation[m
[32m+[m	[32mif ( CommandLine()->FindParm("-makedevshots") )[m
[32m+[m	[32m{[m
[32m+[m		[32mbool usemaplistfile = false;[m
[32m+[m		[32mif ( CommandLine()->FindParm("-usedevshotsfile") )[m
[32m+[m		[32m{[m
[32m+[m			[32musemaplistfile = true;[m
[32m+[m		[32m}[m
[32m+[m		[32mDevShotGenerator().EnableDevShotGeneration( usemaplistfile );[m
[32m+[m	[32m}[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid DevShotGenerator_Shutdown()[m
[32m+[m[32m{[m
[32m+[m	[32mDevShotGenerator().Shutdown();[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid DevShotGenerator_BuildMapList()[m
[32m+[m[32m{[m
[32m+[m	[32mDevShotGenerator().BuildMapList();[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mCDevShotGenerator g_DevShotGenerator;[m
[32m+[m[32mCDevShotGenerator &DevShotGenerator()[m
[32m+[m[32m{[m
[32m+[m	[32mreturn g_DevShotGenerator;[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid CL_DevShots_NextMap()[m
[32m+[m[32m{[m
[32m+[m	[32mDevShotGenerator().NextMap();[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mstatic ConCommand devshots_nextmap( "devshots_nextmap", CL_DevShots_NextMap, "Used by the devshots system to go to the next map in the devshots maplist." );[m
[32m+[m
[32m+[m[32m//-----------------------------------------------------------------------------[m
[32m+[m[32m// Purpose: Constructor[m
[32m+[m[32m//-----------------------------------------------------------------------------[m
[32m+[m[32mCDevShotGenerator::CDevShotGenerator()[m[41m [m
[32m+[m[32m{[m
[32m+[m	[32mm_bUsingMapList = false;[m
[32m+[m	[32mm_bDevShotsEnabled = false;[m
[32m+[m	[32mm_iCurrentMap = 0;[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid CDevShotGenerator::BuildMapList()[m
[32m+[m[32m{[m
[32m+[m	[32mif ( !m_bDevShotsEnabled )[m
[32m+[m		[32mreturn;[m
[32m+[m
[32m+[m	[32mDevShotGenera