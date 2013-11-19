#ifndef _MColorArray
#define _MColorArray
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
//
// CLASS:    MColorArray
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MColorArray)
//
//	This class implements an array of MColor's.  Common convenience functions
//	are available, and the implementation is compatible with the internal
//	Maya implementation so that it can be passed efficiently between plugins
//	and internal maya data structures.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MColor.h>

// *****************************************************************************

// DECLARATIONS


// *****************************************************************************

// CLASS DECLARATION (MColorArray)

/// Array of MColor data type
/**
  Implement an array of MColor data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MColorArray  
{
public:
	///
					MColorArray();
	///
					MColorArray( const MColorArray& other );
	///
					MColorArray( const MColor colors[], unsigned count );
	///
					MColorArray( const float colors[][4], unsigned count );
	///
					MColorArray( const double colors[][4], unsigned count );
	///
					MColorArray( unsigned initialSize, 
								 const MColor &initialValue );
	///
					~MColorArray();
	///
	const MColor&	operator[]( unsigned index ) const;
	///
	MColor&	        operator[]( unsigned index );
 	///
 	MColorArray &   operator=( const MColorArray & other );
	///
	MStatus			set( const MColor& element, unsigned index );
	///
	MStatus 		set( unsigned index, float r, float g, float b, float a=1 );
	///
	MStatus			set( float element[4], unsigned index);
	///
	MStatus			set( float elements[][4], unsigned size);
	///
	MStatus			set( double elements[][4], unsigned size);
	///
	MStatus			setLength( unsigned length );
	///
	unsigned		length() const;
	///
	MStatus			remove( unsigned index );
	///
	MStatus			insert( const MColor & element, unsigned index );
	///
	MStatus			append( const MColor & element );
	///
	MStatus 		append( float r, float g, float b, float a=1 );
	///
	MStatus			clear();
	///
	MStatus			get( float [][4] ) const;
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MColorArray &array);

protected:
// No protected members

private:
	MColorArray( void* );
	void * arr;
	bool   own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MColorArray */
