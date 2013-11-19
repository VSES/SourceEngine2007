
#ifndef _MModelMessage
#define _MModelMessage
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
// CLASS:    MModelMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MModelMessage)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MMessage.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MModelMessage)

/// Scene messages
/**
*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MModelMessage : public MMessage
{
public:
	///
	enum Message {
		///
		kActiveListModified
   };

public:
	///
	static MCallbackId	addCallback( Message, void (*func)( void* clientData ),
									 void * clientData = NULL,
									 MStatus * ReturnStatus = NULL );


	///	Before duplicate callback
	static MCallbackId	addBeforeDuplicateCallback(
								void (*func)( void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

	///	After duplicate callback
	static MCallbackId	addAfterDuplicateCallback(
								void (*func)( void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

private:
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MModelMessage */
