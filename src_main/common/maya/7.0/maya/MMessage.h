
#ifndef _MMessage
#define _MMessage
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
// CLASS:    MMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MMessage)
//
// This is the base class for message callbacks.
// 
// There are three types of messages in maya that are exposed through the
// API and they are the following:
// scene messages, dependency graph messages, and dependency node messages.
// can be registered for these messages using the addCallback
// methods of the MSceneMessage, MDGMessage, and MNodeMessage classes.
// 
// When a callback is added a number or id is returned. This id is used to
// keep track of the callback and is necessary for removing it.
// A callback id with the value 'NULL' represents an invalid callback.
// Use the removeCallback member function of this class for removing a
// callback.
//
// It is the user's responsibility to keep track of the callback id's
// and remove all callbacks for a plug-in when it is unloaded.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

typedef MUintPtrSz MCallbackId;

// Declaration for the callback table
typedef struct MMessageNode {
    void*           fClientPtr;
	void*			fServerPtr;
	void*           fSubClientPtr;
	MCallbackId		fId;
	bool			isValid;	// Unused
} * MMessageNodePtr;

// *****************************************************************************

// CLASS DECLARATION (MMessage)

class MIntArray;
class MObject;
class MCallbackIdArray;

/// Message base class
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MMessage
{ 
public:	
	///
    static MStatus  removeCallback( MCallbackId id );
	/// OBSOLETE
    static MStatus  removeCallbacks( MIntArray &ids );
	///
    static MStatus  removeCallbacks( MCallbackIdArray &ids );
	///
	static MCallbackId	currentCallbackId( MStatus* ReturnStatus = NULL );
	///
	static MStatus	nodeCallbacks( MObject& node, MIntArray& ids );

protected:
    static void addNode( MMessageNodePtr node ); 
    static void removeNode( MMessageNodePtr node );

private:
	static const char* 		className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MMessage */
