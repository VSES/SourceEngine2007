#ifndef _MFloatArray
#define _MFloatArray
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
// CLASS:    MFloatArray
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFloatArray)
//
//  This class implements an array of floats.  Common convenience functions
//  are available, and the implementation is compatible with the internal
//  Maya implementation so that it can be passed efficiently between plugins
//  and internal maya data structures.
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFloatArray)

/// Array of floats data type
/**
  Implement an array of floats data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFloatArray  
{
public:
 	///
					MFloatArray();
 	///
					MFloatArray( const MFloatArray& other );
 	///
					MFloatArray( const float src[], unsigned count );
 	///
					MFloatArray( const double src[], unsigned count );
	///
					MFloatArray( unsigned initialSize, 
								 float initialValue = 0 );
	///
					~MFloatArray();
	///
 	float			operator[]( unsigned index ) const;
 	///
 	float &	 	    operator[]( unsigned index );
 	///
 	MFloatArray &   operator=( const MFloatArray & other );
	///
	MStatus			set( float element, unsigned index );
	///
	MStatus			set( double element, unsigned index );
 	///
	MStatus			setLength( unsigned length );
	///
 	unsigned        length() const;
 	///
 	MStatus         remove( unsigned index );
 	///
 	MStatus         insert( float element, unsigned index );
 	///
 	MStatus         append( float element );
 	///
 	MStatus         copy( const MFloatArray& source );
 	///
 	MStatus		 	clear();
	///
	MStatus			get( float[] ) const;
	///
	MStatus			get( double[] ) const;
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MFloatArray &array);

protected:
// No protected members

private:
	MFloatArray( void* );
 	void * arr;
	bool   own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFloatArray */
