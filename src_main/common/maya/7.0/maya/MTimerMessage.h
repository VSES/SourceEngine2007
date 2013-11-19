#ifndef _MTimerMessage
#define _MTimerMessage
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related
// material (collectively the "Data") in these files contain unpublished
// information proprietary to Alias Systems Corp. ("Alias") and/or its
// licensors, which is protected by Canadian and US federal copyright law and
// by international treaties.
//
// The Data may not be disclosed or distributed to third parties or be copied
// or duplicated, in whole or in part, without the prior written consent of
// Alias.
//
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY,
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
//
// CLASS:    MTimerMessage
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MTimerMessage)
//
// This class is used to register callbacks that are based on a fixed
// time interval.  Unlike other callbacks, a timer callback is
// executed with the highest priority within Maya and take precidence
// over *all* other Maya events.  Use with care as it is possible to
// construct a callback that will prevent any other part of Maya from
// running.
//
// If the execution time of a callback exceeds its period/2 the timer
// will be reset.  This will give Maya 1 period in which to do other
// things.
//
// To remove a callback use MMessage::removeCallback.
// All callbacks that are registered by a plug-in must be removed by that
// plug-in when it is unloaded. Failure to do so will result in a fatal error.
//
// ****************************************************************************

#if defined __cplusplus

// INCLUDED HEADER FILES


#include <maya/MMessage.h>

// ****************************************************************************
//
// DECLARATIONS
//
// ****************************************************************************
//
// CLASS DECLARATION (MTimerMessage)

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

/// Class used to register callbacks that are based on a fixed time interval.
class OPENMAYA_EXPORT MTimerMessage : public MMessage
{ 
public:
	typedef void (*sleepCallbackPtr)();
	///
	static MCallbackId	addTimerCallback(
								float period,
								void (*func)( float elapsedTime, float lastTime, void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
	///
	static MStatus setSleepCallback( sleepCallbackPtr sleepFunc );
	///
	static sleepCallbackPtr sleepCallback( );

private: 
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// ****************************************************************************

#endif /* __cplusplus */
#endif /* _MTimerMessage */
