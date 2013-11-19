
#ifndef _MAttributeIndex
#define _MAttributeIndex
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
// CLASS:    MAttributeIndex
//
//    Class that provides access to the index part of an attribute
//    specification (MAttributeSpec).
//
//    See MAttributeSpec for more information.
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAttributeIndex)
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

// CLASS DECLARATION (MAttributeIndex)

/// The index information for an attribute specification
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MAttributeIndex {
public:
	///
	enum MIndexType {
		///
		kInteger,
		///
		kFloat
	};

	///
	MAttributeIndex();
	///
	~MAttributeIndex();
	///
	MAttributeIndex( const MAttributeIndex & );
	///
	MAttributeIndex( int );
	///
	MAttributeIndex( double );

public:
	///
	MIndexType		type() const;
	///
	bool			hasRange() const;
	///
	bool			hasValidRange() const;

	///
	bool			hasLowerBound() const;
	///
	bool			hasUpperBound() const;
	///
	MStatus			getLower( int & ) const;
	///
	MStatus			getLower( double & ) const;
	///
	MStatus			getUpper( int & ) const;
	///
	MStatus			getUpper( double & ) const;

	///
	bool			isBounded() const;
	///
	MStatus			getValue( int & ) const;
	///
	MStatus			getValue( double & ) const;

public:
	///
	MStatus			setType( MIndexType );
	///
	MStatus			setValue( int );
	///
	MStatus			setValue( double );
	///
	MStatus			setLower( int );
	///
	MStatus			setLower( double );
	///
	MStatus			setUpper( int );
	///
	MStatus			setUpper( double );
	
public:
	///
	MAttributeIndex & operator=( const MAttributeIndex & );
	///
	bool			operator==( const MAttributeIndex & ) const;
	///
	bool			operator!=( const MAttributeIndex & ) const;

private:
	MAttributeIndex( void* );
	friend class MAttributeSpec;
	void*	data;
	bool	own;
	static const char* className();
};


#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAttributeIndex */
