
#ifndef _MFileObject
#define _MFileObject
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
// CLASS:    MFileObject
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFileObject)
//
//	The MFileObject class implements an object that contains both a
//	filename and a search path.  The search path is specified by a single
//	string in which multiple elements are separated by ':' characters.  As
//	well this string can contain Unix environment variables, specified as
//	$VARNAME,
//
//	Filenames can be produced by the class by combining the path element
//	with the filename element of the MFileObject.  As well, methods are
//	available to test for the existance of the files produced.
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MString;

// *****************************************************************************

// CLASS DECLARATION (MFileObject)

/// Manipulate Unix filenames and search paths
/**
  Manipulate Unix filenames and search paths
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFileObject  
{
public:
	///
				MFileObject();
	///
				MFileObject( const MFileObject & other );
	///
				virtual ~MFileObject();
	///
	MFileObject& operator=( const MFileObject & other );
	///
	MStatus   	setName( const MString & fileName );
	///
	MStatus   	setRawPath( const MString & pathString );
	///
	MStatus   	setFullName( const MString & fileName );
	///
	MString  	name() const;
	///
	MString  	rawPath() const;
	///
	MString  	path() const;
	///
	MString  	fullName() const;
	///
	unsigned	pathCount();
	///
	MString  	ithPath( unsigned index );
	///
	MString  	ithFullName( unsigned index );
	///
	bool        exists();
	///
	bool        exists( unsigned index );
	///
	bool        isSet() const;

protected:
// No protected members

private:
	void * data;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFileObject */
