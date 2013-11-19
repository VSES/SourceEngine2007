
#ifndef _MPolyMessage
#define _MPolyMessage
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
// CLASS:    MPolyMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPolyMessage)
//
// This class is used to register callbacks for poly messages.  Currently,
// you can use this class to listen to component id changes for vertices,
// edges and faces.
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
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

class MUintArray;
class MObject;

// *****************************************************************************

// CLASS DECLARATION (MPolyMessage)

/// Polygon component id change messages (OpenMaya)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPolyMessage : public MMessage
{
public:
	enum
	{
		///
		kVertexIndex = 0,
		///
		kEdgeIndex = 1,
		///
		kFaceIndex = 2,
		///
		kLastErrorIndex = 3
	};
	
	///
	static MCallbackId	addPolyComponentIdChangedCallback (
								MObject& node,
								bool wantIdModifications[],
								unsigned int count,
								void (*func)(MUintArray componentIds[],
												unsigned int count,
												void *clientData),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);

	///
	static MCallbackId	addPolyTopologyChangedCallback (
								MObject& node,
								void (*func)(MObject& node,
											 void *clientData),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);
	
	///								
	static unsigned int deletedId( void );

private:
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MPolyMessage */
