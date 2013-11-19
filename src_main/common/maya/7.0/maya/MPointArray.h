#ifndef _MPointArray
#define _MPointArray
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
// CLASS:    MPointArray
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPointArray)
//
//	This class implements an array of MPoints.  Common convenience functions
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
#include <maya/MPoint.h>

// *****************************************************************************

// DECLARATIONS


// *****************************************************************************

// CLASS DECLARATION (MPointArray)

/// Array of MPoint data type
/**
  Implement an array of MPoint data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPointArray  
{
public:
	///
					MPointArray();
	///
					MPointArray( const MPointArray& other );
	///
					MPointArray( const MPoint points[], unsigned count );
	///
					MPointArray( const double points[][4], unsigned count );
	///
					MPointArray( const float points[][4], unsigned count );
	///
					MPointArray( unsigned initialSize, 
								 const MPoint &initialValue 
								 = MPoint::origin );
	///
					~MPointArray();
	///
	const MPoint&	operator[]( unsigned index ) const;
	///
	MPoint&	        operator[]( unsigned index );
 	///
 	MPointArray &   operator=( const MPointArray & other );
	///
	MStatus			set( const MPoint& element, unsigned index);
	///
	MStatus 		set( unsigned index, double x, double y, double z=0, 
						 double w=1);
	///
	MStatus			set( double element[4], unsigned index);
	///
	MStatus			set( float element[4], unsigned index);
	///
	MStatus			setLength( unsigned length );
	///
	unsigned		length() const;
	///
	MStatus			remove( unsigned index );
	///
	MStatus			insert( const MPoint & element, unsigned index );
	///
	MStatus			append( const MPoint & element );
	///
	MStatus 		append( double x, double y, double z=0, double w=1 );
 	///
 	MStatus         copy( const MPointArray& source );
	///
	MStatus			clear();
	///
	MStatus			get( double [][4] ) const;
	///
	MStatus			get( float [][4] ) const;
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MPointArray &array);

protected:
// No protected members

private:
	MPointArray( void* );
	void * arr;
	bool   own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPointArray */
