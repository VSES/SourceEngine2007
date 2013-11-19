#ifndef _MVectorArray
#define _MVectorArray
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
// CLASS:    MVectorArray
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MVectorArray)
//
//  This class implements an array of integers.  Common convenience functions
//  are available, and the implementation is compatible with the internal
//  Maya implementation so that it can be passed efficiently between plugins
//  and internal maya data structures.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

#include <maya/MVector.h>
#include <maya/MStatus.h>

// *****************************************************************************

// CLASS DECLARATION (MVectorArray)

/// Array of MVectors data type
/**
  Implement an array of MVectors data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MVectorArray  
{

public:
	///
					MVectorArray();
	///
					MVectorArray( const MVectorArray& other );
	///
					MVectorArray( const MVector vectors[], unsigned count );
	///
					MVectorArray( const double vectors[][3], unsigned count );
	///
					MVectorArray( const float vectors[][3], unsigned count );
	///
					MVectorArray( unsigned initialSize, 
								  const MVector &initialValue 
								  = MVector::zero );
	///
					~MVectorArray();
	///
 	const MVector&	operator[]( unsigned index ) const;
	///
 	MVector &		operator[]( unsigned index );
 	///
 	MVectorArray &  operator=( const MVectorArray & other );
	///
	MStatus			set( const MVector& element, unsigned index );
	///
	MStatus			set( double element[3], unsigned index );
	///
	MStatus			set( float element[3], unsigned index );
	///
	MStatus			setLength( unsigned length );
	///
	unsigned		length() const;
	///
	MStatus			remove( unsigned index );
	///
	MStatus			insert( const MVector & element, unsigned index );
	///
	MStatus			append( const MVector & element );
 	///
 	MStatus         copy( const MVectorArray& source );
	///
	MStatus			clear();
	///
	MStatus			get( double [][3] ) const;
	///
	MStatus			get( float [][3] ) const;
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MVectorArray &array);

protected:
// No protected members

private:
	MVectorArray( void* );
	void * arr;
	bool   own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MVectorArray */
