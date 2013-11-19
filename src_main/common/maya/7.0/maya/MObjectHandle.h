#ifndef _MObjectHandle
#define _MObjectHandle
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
// *****************************************************************************
//
// CLASS:    MObjectHandle
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MObjectHandle)
//
//  MObjectHandle is a wrapper class for the MObject class. An MObjectHandle
//	will provide a user with added information on the validity of an MObject.
//	Each MObjectHandle that is created registers an entry into a table to
//	maintain state tracking of the MObject that the handle was created for.
//	This will help users track when an MObject is invalid and should be
//	re-retrieved.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

////////////////////////////////////////////////////////////////////////////////

// CLASS DECLARATION (MObjectHandle)

/// Generic Class for validating MObjects

class OPENMAYA_EXPORT MObjectHandle
{
public:
	///
	MObjectHandle(); 
	///
	MObjectHandle( const MObject& object );
	/// 
	MObjectHandle( const MObjectHandle &handle ); 
	///
	~MObjectHandle();
	///
	MObject			object() const;
	///
	const MObject & objectRef() const;
	///
	bool			isValid() const;
	///
	bool			isAlive() const;
	
	///
	bool			operator==(const MObject &object) const;
	///
	bool			operator==(const MObjectHandle &object) const;
	///
	bool			operator!=(const MObject &object) const; 
	///
	bool			operator!=(const MObjectHandle &object) const;
	
	/// 
	MObjectHandle & operator=(const MObject &object );
	/// 
	MObjectHandle & operator=(const MObjectHandle &other );

private:

	// Internal object registration methods
	void			registerObject();
	void			deregisterObject();
	
	// Internal destructor callback
	static void		registerDestroyedCallback();
	static void		deregisterDestroyedCallback();
	static bool		fIsCallbackInitialized;
	static void		destroyedCallback(void*);

	MObject			fObject;
	bool			fIsAlive;
	bool			fIsDependNode;
	static int		fHandleCount;
};

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MObjectHandle */

