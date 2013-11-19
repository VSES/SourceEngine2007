#ifndef _MSceneMessage
#define _MSceneMessage
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
// CLASS:    MSceneMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MSceneMessage)
//
// This class is used to register callbacks for scene related messages.
//
// The addCallback method registers a function that will be executed
// whenever the specified message occurs. An id is returned and is used
// to remove the callback.
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

// CLASS DECLARATION (MSceneMessage)

/// Scene messages
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MSceneMessage : public MMessage
{
public:
	///
	enum Message {
		/// called after any operation that changes which files are loaded
		kSceneUpdate,
		/// called before a File > New operation
		kBeforeNew,
		/// called after a File > New operation
		kAfterNew,
		/// called before a File > Import operation
		kBeforeImport,
		/// called after a File > Import operation
		kAfterImport,
		/// called before a File > Open operation
		kBeforeOpen,
		/// called after a File > Open operation
		kAfterOpen,
		/// called before a File > Export operation
		kBeforeExport,
		/// called after a File > Export operation
		kAfterExport,
		/// called before a File > Save (or SaveAs)  operation
		kBeforeSave,
		/// called after a File > Save (or SaveAs)  operation
		kAfterSave,
        /// called before a File > Reference operation
        kBeforeReference,
        /// called after a File > Reference operation
        kAfterReference,
        /// called before a File > RemoveReference operation
        kBeforeRemoveReference,
        /// called after a File > RemoveReference operation
        kAfterRemoveReference,
		/// called before a File > ImportReference operation
		kBeforeImportReference,
		/// called after a File > ImportReference operation
		kAfterImportReference,
		/// called before a File > ExportReference operation
		kBeforeExportReference,
		/// called after a File > ExportReference operation
		kAfterExportReference,
		/// called before a File > UnloadReference operation
		kBeforeUnloadReference,
		/// called after a File > UnloadReference operation
		kAfterUnloadReference,

		/// called before a Software Render begins
		kBeforeSoftwareRender,
		/// called after a Software Render ends
		kAfterSoftwareRender,
		/// called before each frame of a Software Render
		kBeforeSoftwareFrameRender,
		/// called after each frame of a Software Render
		kAfterSoftwareFrameRender,
		/// called when an interactive render is interrupted by the user
		kSoftwareRenderInterrupted,

		/// called on interactive or batch startup after initialization
		kMayaInitialized,
		/// called just before Maya exits
		kMayaExiting,

		/// called prior to File > New operation, allows user to cancel action 
		kBeforeNewCheck,		
		/// called prior to File > Open operation, allows user to cancel action 
		kBeforeOpenCheck,
		/// called prior to File > Save operation, allows user to cancel action 
		kBeforeSaveCheck,
		/// last value of the enum
		kLast
   };

public:
	// Since there is no data associated with scene messages there
	// is a single "addCallback" function for all message types.
	//
	///
	static MCallbackId	addCallback( Message, void (*func)( void* clientData ),
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );

	// Register a callback, which has the option to abort the current operation. This is done by
	// returning a false value in the supplied return code parameter
	//
	///
	static MCallbackId	addCallback( Message, void (*func)( bool* retCode, void* clientData ),
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );

private:
private:
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MSceneMessage */
