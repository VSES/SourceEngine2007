//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: module for gathering performance stats for upload so that we can
//  monitor performance regressions and improvements
//
//=====================================================================================//


#ifndef STATGATHER_H
#define STATGATHER_H

#ifdef _WIN32
#pragma once
#endif

#include "tier0/platform.h"

// call once per frame to update internal stats. Should only call in "normal" game frames
// unless you also want to gather data on frame rates during menu displays, etc.
void UpdatePerfStats( void );

#endif // STATGATHER_H
