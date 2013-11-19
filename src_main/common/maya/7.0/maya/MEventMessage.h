
#ifndef _MEventMessage
#define _MEventMessage
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
// CLASS:    MEventMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MEventMessage)
//
// This class is used to register callbacks for event messages.
// There is 1 add callback method which will add callbacks for the
// following message
//     Event occurred
//
// The list of available event names can be retrieved with the
// getEventNames method.
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
#include <maya/MStringArray.h>

// *****************************************************************************

// DECLARATIONS


// *****************************************************************************

// CLASS DECLARATION (MEventMessage)

/// Event messages 
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MEventMessage : public MMessage
{ 
public:
	///
	static MCallbackId	addEventCallback(
								const MString& event,
								void (*func)( void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///
	static MStatus		getEventNames( MStringArray & names );

private: 
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MEventMessage */
