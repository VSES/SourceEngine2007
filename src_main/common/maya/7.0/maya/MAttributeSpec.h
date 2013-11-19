
#ifndef _MAttributeSpec
#define _MAttributeSpec
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
// CLASS:    MAttributeSpec
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAttributeSpec)
//
//		Class that encapsulates component/attribute information
//		for generating selection items.
//
//      This class is used by MPxSurfaceShape::matchComponent for validating
//      attributes specified as strings and converting that specification to
//      a component object.
//
//		The attribute specification ".foo.bar[3].x" would be
//		expressed like this (using a pseudo-structure description):
//
//		MAttributeSpecArray[0..3] {
//			MAttributeSpec[0] {
//				name      = "foo"
//				dimension = 0
//			}
//			MAttributeSpec[1] {
//				name      = "bar"
//				dimension = 1
//				MAttributeIndex[0] = {
//					type      = kInteger
//					isRange   = false
//					isBounded = true
//					value     = 3
//				}
//			}
//			MAttributeSpec[2] {
//				name      = "x"
//				dimension = 0
//			}
//		}
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MAttributeIndex;
class MString;

// *****************************************************************************

// CLASS DECLARATION (MAttributeSpec)

/// An attribute specification
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MAttributeSpec {
public:
	///
	MAttributeSpec();
	///
	MAttributeSpec( const char * );
	///
	MAttributeSpec( const MString & );
	///
	MAttributeSpec( const MAttributeSpec & );
	///
	~MAttributeSpec();

public:
	///
	const MString 			name() const;
	///
	int						dimensions() const;
	///
	void					setName( const MString & );
	///
	void					setDimensions( int );

public:
	///
	MAttributeSpec &		operator =(const MAttributeSpec &);
	///
	const MAttributeIndex	operator [](int) const;
	///
	MAttributeIndex 		operator[](int);
	///
	bool					operator==(const MAttributeSpec &) const;

private:
	MAttributeSpec( void* );
	void*	data;
	bool	own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAttributeSpec */
