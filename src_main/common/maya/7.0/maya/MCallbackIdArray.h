#ifndef _MCallbackIdArray
#define _MCallbackIdArray
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
// CLASS:    MCallbackIdArray
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MCallbackIdArray)
//
//	This class implements an array of MCallbackId.  Common convenience functions
//	are available, and the implementation is compatible with the internal
//	Maya implementation so that it can be passed efficiently between plugins
//	and internal maya data structures.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MMessage.h>

// ****************************************************************************

// DECLARATIONS

// ****************************************************************************

// CLASS DECLARATION (MCallbackIdArray)

/// Array of MCallbackId data type
/**
  Implement an array of MCallbackId data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MCallbackIdArray  
{

public:
 	///
					MCallbackIdArray();
 	///
					MCallbackIdArray( const MCallbackIdArray& other );
 	///
					MCallbackIdArray( const MCallbackId src[], unsigned count );
	///
					MCallbackIdArray( unsigned initialSize, 
							   MCallbackId initialValue = 0 );
	///
					~MCallbackIdArray();
	///
 	MCallbackId		operator[]( unsigned index ) const;
 	///
 	MCallbackId&	 operator[]( unsigned index );
 	///
 	MCallbackIdArray& operator=( const MCallbackIdArray & other );
	///
	MStatus			set( MCallbackId element, unsigned index );
 	///
	MStatus			setLength( unsigned length );
	///
 	unsigned        length() const;
 	///
 	MStatus			remove( unsigned index );
 	///
 	MStatus			insert( MCallbackId element, unsigned index );
 	///
 	MStatus			append( MCallbackId element );
 	///
 	MStatus         copy( const MCallbackIdArray& source );
 	///
 	MStatus		 	clear();
	///
	MStatus			get( MCallbackId[] ) const;
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MCallbackIdArray &array);

protected:
// No protected members

private:
	MCallbackIdArray( void* );
 	void* fArray;
	bool   own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// ****************************************************************************
#endif /* __cplusplus */
#endif /* _MCallbackIdArray */
