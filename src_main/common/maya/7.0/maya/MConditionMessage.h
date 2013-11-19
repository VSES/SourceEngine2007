
#ifndef _MConditionMessage
#define _MConditionMessage
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
// CLASS:    MConditionMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MConditionMessage)
//
// This class is used to register callbacks for changes to specific
// conditions and/or to determine the current state of a condition.
// 
// The addConditionCallback method allows callbacks to be registered
// for the following messages:
// 	   Condition Changed
// 
// The first parameter passed to the add callback method is the 
// name of the condition that will trigger the callback.  The names of
// available conditions can be retrieved with the getConditionNames
// method.
// 
// Callbacks that are registered for conditions will be passed an
// bool parameter that is the state of the condition.
// 
// Each method returns an id which is used to remove the callback.
// 
// To remove a callback use MMessage::removeCallback.
// 
// All callbacks that are registered by a plug-in must be removed by that
// plug-in when it is unloaded. Failure to do so will result in a fatal error.
//
// The getConditionState method is used to return the current status
// of the specified condition.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>

// *****************************************************************************

// DECLARATIONS


// *****************************************************************************

// CLASS DECLARATION (MConditionMessage)

/// Condition change messages 
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MConditionMessage : public MMessage
{ 
public:
	///
	static MCallbackId	addConditionCallback(
								const MString& condition,
								void (*func)( bool state,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///
	static MStatus		getConditionNames( MStringArray & names );

	///
	static bool			getConditionState( const MString& condition,
										   MStatus * ReturnStatus = NULL );

private: 
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MConditionMessage */
