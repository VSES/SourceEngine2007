
#ifndef _MAttributeSpecArray
#define _MAttributeSpecArray
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
// CLASS:    MAttributeSpecArray
//
//    Array class for MAttributeSpec.
//
//    See MAttributeSpec for more information.
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAttributeSpecArray)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MAttributeSpec;

// *****************************************************************************

// CLASS DECLARATION (MAttributeSpecArray)

/// An attribute specification array
/**
  Implement an array of MAttributeSpecs data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MAttributeSpecArray  
{

public:
	///
	MAttributeSpecArray();
	///
	MAttributeSpecArray( const MAttributeSpecArray& other );
	///
	~MAttributeSpecArray();

	///
	MAttributeSpec	operator[]( unsigned index ) const;
 	///
 	MAttributeSpecArray & operator=( const MAttributeSpecArray & other );

	///
	MStatus			set( const MAttributeSpec& element, unsigned index );
	///
	MStatus			setLength( unsigned length );
	///
	unsigned		length() const;
	///
	MStatus			remove( unsigned index );
	///
	MStatus			insert( const MAttributeSpec & element, unsigned index );
	///
	MStatus			append( const MAttributeSpec & element );
	///
	MStatus			clear();
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;

protected:
// No protected members

private:
	MAttributeSpecArray( void* );
	void * arr;
	bool   own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAttributeSpecArray */
