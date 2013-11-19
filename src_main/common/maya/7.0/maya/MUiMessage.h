
#ifndef _MUiMessage
#define _MUiMessage
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
// CLASS:    MUiMessage
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MUiMessage)
//
// This class is used to register callbacks for ui deleted messages.
// There is 1 add callback method which will add callbacks for the
// following message
//     UI Deleted
//
// To remove a callback use MMessage::removeCallback.
//
// All callbacks that are registered by a plug-in must be removed by that
// plug-in when it is unloaded. Failure to do so will result in a fatal error.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MMessage.h>
class M3dView;

// ****************************************************************************

// DECLARATIONS


// ****************************************************************************

// CLASS DECLARATION (MUiMessage)

/// UI messages (OpenMayaUI)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MUiMessage : public MMessage
{
public:
	///
	static MCallbackId	addUiDeletedCallback(
		const MString& uiName,
		void (*func)( void* clientData ),
		void * clientData = NULL,
		MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	addCameraChangedCallback(
		const MString& panelName,
		void (*func)( MString &panelName, MObject &camera, void* clientData ),
		void * clientData = NULL,
		MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	add3dViewDestroyMsgCallback(
		const MString& panelName,
		void (*func)( MString &panelName, void* clientData ),
		void * clientData = NULL,
		MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	add3dViewPreRenderMsgCallback(
		const MString& panelName,
		void (*func)( MString &panelName, void* clientData ),
		void * clientData = NULL,
		MStatus * ReturnStatus = NULL );

	///
	static MCallbackId	add3dViewPostRenderMsgCallback(
		const MString& panelName,
		void (*func)( MString &panelName, void* clientData ),
		void * clientData = NULL,
		MStatus * ReturnStatus = NULL );

private:
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// ****************************************************************************

#endif /* __cplusplus */
#endif /* _MUiMessage */
