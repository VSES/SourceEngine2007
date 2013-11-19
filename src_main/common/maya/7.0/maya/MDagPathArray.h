#ifndef _MDagPathArray
#define _MDagPathArray
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
// CLASS:    MDagPathArray
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDagPathArray)
//
//	The DAG Path Array Class provides methods for manipulating arrays of DAG
//	Paths.
//
//	Methods are provided for obtaining the DAG Path elements by index, clearing
//	the array, determining the length of the array, and removing, inserting and
//	appending array elements.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MDagPath;

// *****************************************************************************

// CLASS DECLARATION (MDagPathArray)

///  Indexable Array of DAG Paths
/**

Provides methods for manipulating arrays of DAG Paths.

Arrays of DAG Paths are useful for storing and manipluating multiple Paths
to a particular DAG Node.  The DAG Path method MDagPath::getAllPathsTo()
and DAG Node Function Set method MFnDagNode::getAllPaths() implicitly
return an array of DAG Paths.

These arrays may also be used to manage Paths for a number of different
Nodes.

DAG Path arrays are used in conjunction with DAG Paths (MDagPath) and
individual elements of the arrays can be parameters to some methods of the
DAG Node Function Set (MFnDagNode).

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MDagPathArray  
{

public:
	///
					MDagPathArray();
	///
					~MDagPathArray();
	///
	const MDagPath&	operator[]( unsigned index ) const;
	///
	MDagPath &		operator[]( unsigned index );
	///
	MStatus			setLength( unsigned length );
	///
	unsigned		length() const;
	///
	MStatus			remove( unsigned index );
	///
	MStatus			insert( const MDagPath & element, unsigned index );
	///
	MStatus			append( const MDagPath & element );
	///
	MStatus			clear();
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MDagPathArray &array);

protected:
// No protected members

private:
	///
	void * arr;

	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDagPathArray */
