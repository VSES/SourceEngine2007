
#ifndef _MDagPath
#define _MDagPath
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
// CLASS:    MDagPath
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDagPath)
//
//	The DAG Path Class provides methods for obtaining one or all Paths to a
//	specified DAG Node, determining if a Path is valid and if two Paths are
//	equivalent, obtaining the length, transform, and inclusive and exclusive
//	matrices of Path, as well as performing Path to Path assignment.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MMatrix;
class MDagPathArray;
class MString;

// *****************************************************************************

// CLASS DECLARATION (MDagPath)

///  DAG Path
/**

Provides methods for obtaining one or all Paths to a specified DAG Node,
determining if a Path is valid and if two Paths are equivalent, obtaining the
length, transform, and inclusive and exclusive matrices of a Path, as well as
performing Path to Path assignment.

DAG Paths may be used as parameters to some methods in the DAG Node Function
Set (MFnDagNode).

It is often useful to combine DAG Paths into DAG Path arrays (MDagPathArray).

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MDagPath  
{
 
public:
	///
	MDagPath();
	///
	MDagPath( const MDagPath& src );
	///
	virtual ~MDagPath();

	///
	static MStatus	getAllPathsTo( const MObject & entity, 
								   MDagPathArray & pathArray );
	///
	static MStatus	getAPathTo( const MObject & entity, 
								MDagPath & path );
	///
	bool            hasFn( MFn::Type type,
						   MStatus * ReturnStatus = NULL ) const;
	///
	MFn::Type       apiType(MStatus * ReturnStatus = NULL) const;

	///
	bool        	isValid(MStatus * ReturnStatus = NULL) const;
	///
	MObject      	node(MStatus * ReturnStatus = NULL) const;
	///
	MObject   		transform( MStatus * ReturnStatus = NULL ) const;
	///
	unsigned int    length(MStatus * ReturnStatus = NULL) const; 
	///
	MStatus         extendToShape();
	///
	MStatus         extendToShapeDirectlyBelow( unsigned int );
	///
	MStatus         numberOfShapesDirectlyBelow( unsigned int& );
	///
	MStatus         push( const MObject &child );
	///
	MStatus         pop( unsigned num = 1 );
	///
	unsigned        childCount(  MStatus * ReturnStatus = NULL ) const;
	///
	MObject 	    child( unsigned i, MStatus * ReturnStatus = NULL ) const;
	///
	MMatrix      	inclusiveMatrix(MStatus * ReturnStatus = NULL) const;
	///
	MMatrix      	exclusiveMatrix(MStatus * ReturnStatus = NULL) const;
	///
	MMatrix      	inclusiveMatrixInverse(MStatus * ReturnStatus = NULL)const;
	///
	MMatrix      	exclusiveMatrixInverse(MStatus * ReturnStatus = NULL)const;
	///
	MDagPath&		operator = ( const MDagPath& src);
	///
	bool			operator == ( const MDagPath& src) const;
	///
	MStatus         set( const MDagPath& src);

	///
	unsigned		pathCount(MStatus * ReturnStatus = NULL) const;
	///
	MStatus			getPath( MDagPath & path, unsigned i = 0 ) const;
	///
	MString         fullPathName(MStatus *ReturnStatus = NULL) const;
	///
	MString         partialPathName(MStatus *ReturnStatus = NULL) const;

	///
	bool            isInstanced( MStatus *ReturnStatus = NULL ) const;
	///
	unsigned        instanceNumber( MStatus *ReturnStatus = NULL ) const;

	/// OBSOLETE
	static MDagPath getAPathTo( const MObject & entity, 
								MStatus * ReturnStatus = NULL );

protected:
// No protected members

private:

	static const char* className();
 	void * data;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDagPath */
