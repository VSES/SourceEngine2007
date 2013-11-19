//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=====================================================================================//

#include "audio_pch.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

bool snd_firsttime = true;

/* 
 * Global variables. Must be visible to window-procedure function 
 *  so it can unlock and free the data block after it has been played. 
 */ 
IAudioDevice *g_AudioDevice = NULL;

/*
==================
S_BlockSound
==================
*/
void S_BlockSound( void )
{
	if ( !g_AudioDevice )
		return;

	g_AudioDevice->Pause();
}

/*
==================
S_UnblockSound
==================
*/
void S_UnblockSound( void )
{
	if ( !g_AudioDevice )
		return;

	g_AudioDevice->UnPause();
}

/*
==================
AutoDetectInit

Try to find a sound device to mix for.
Returns a CAudioNULLDevice if nothing is found.
==================
*/
IAudioDevice *IAudioDevice::AutoDetectInit( bool waveOnly )
{
	IAudioDevice *pDevice = NULL;

	// JAY: UNDONE: Handle fake sound device here "fakedma"
//	if (g_pSoundServices->CheckParm("-simsound"))
//		fakedma = true;

	if ( IsPC() )
	{
		if ( waveOnly )
		{
			pDevice = Audio_CreateWaveDevice();
			if ( !pDevice )
				goto NULLDEVICE;
		}

		if ( !pDevice )
		{
			if ( snd_firsttime )
			{
				pDevice = Audio_CreateDirectSoundDevice();
			}
		}

		// if DirectSound didn't succeed in initializing, try to initialize
		// waveOut sound, unless DirectSound failed because the hardware is
		// already allocated (in which case the user has already chosen not
		// to have sound)
		// UNDONE: JAY: This doesn't test for the hardware being in use anymore, REVISIT
		if ( !pDevice )
		{
			pDevice = Audio_CreateWaveDevice();
		}
	}
#if defined( _X360 )
	else
	{
		pDevice = Audio_CreateXAudioDevice();
		if ( pDevice )
		{
			// xaudio requires threaded mixing
			S_EnableThreadedMixing( true );
		}
	}
#endif

NULLDEVICE:
	snd_firsttime = false;

	if ( !pDevice )
	{
		if ( snd_firsttime )
			DevMsg( "No sound device initialized\n" );

		return Audio_GetNullDevice();
	}

	return pDevice;
}

/*
==============
SNDDMA_Shutdown

Reset the sound device for exiting
===============
*/
void SNDDMA_Shutdown( void )
{
	if ( g_AudioDevice != Audio_GetNullDevice() )
	{
		if ( g_AudioDevice )
		{
			g_AudioDevice->Shutdown();
			delete g_AudioDevice;
		}

		// the NULL device is always valid
		g_AudioDevice = Audio_GetNullDevice();
	}
}

