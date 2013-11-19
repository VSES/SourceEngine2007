
#ifndef _MUserEventMessage
#define _MUserEventMessage
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
// CLASS:    MUserEventMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MUserEventMessage)
//
// This class is used to register user-defined event types, register
// callbacks with the user-defined event types, and to post user-defined
// messages.
//
// The registerUserEvent and deregisterUserEvent methods allow user event 
// types to be created and destroyed.  User events are identified by a unique
// string identifier.
//
// The addCallback method registers a function that will be executed
// whenever the specified message occurs. An id is returned and is used
// to remove the callback.
//
// The postUserEvent notifies all registered callbacks of the occurence of a
// user-defined event.
//
// To remove a callback use MMessage::removeCallback.
// All callbacks that are registered by a plug-in must be removed by that
// plug-in when it is unloaded. Failure to do so will result in a fatal error.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MMessage.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MUserEventMessage)

/// User-defined Message Types 
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MUserEventMessage : public MMessage
{ 
public:

	///
	static MStatus		registerUserEvent( const MString& event );
	///
	static bool			isUserEvent( const MString& event );
	///
	static MStatus		deregisterUserEvent( const MString& event );
	///
	static MCallbackId	addUserEventCallback(
								const MString& event,
								void (*func)( void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );
	///
	static MStatus		postUserEvent( 
								const MString& event, 
								void* clientData = NULL );

private: 

	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MUserEventMessage */
