//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "pch_tier0.h"

#include <stdio.h>
#include "tier0/fasttimer.h"

int64 g_ClockSpeed;	// Clocks/sec
unsigned long g_dwClockSpeed;
#if defined( _X360 ) && defined( _CERT )
unsigned long g_dwFakeFastCounter;
#endif
double g_ClockSpeedMicrosecondsMultiplier;
double g_ClockSpeedMillisecondsMultiplier;
double g_ClockSpeedSecondsMultiplier;

// Constructor init the clock speed.
CClockSpeedInit g_ClockSpeedInit;
