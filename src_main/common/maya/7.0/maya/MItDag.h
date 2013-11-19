
#ifndef _MItDag
#define _MItDag
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
// CLASS:    MItDag
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItDag)
//
// Use the DAG iterator to traverse the DAG either depth first or breadth
// first, visiting each node and, if desired, retrieving the node (as an
// MObject).  The DAG iterator provides a basic filtering capability, so
// that DAG node retrieval can be limited to a  specific type (MFn::Type)
// of node.  With filtering enabled the iterator checks to see if the node
// is compatible with the type of Function Set specified by the filter.
// See MFn::Type for a list of all valid Function set types.
// 
// Since each object, if retrieved, is returned as an MObject, the
// MObject::hasFn() method can be used to further check for compatible
// function set types since an MObjects may be compatible with more than
// one function set).
// 
// Any compatible Function Set can be attached to the retrieved object to
// query or or edit it.  Often you will want to use the DAG node Function
// Set (MFnDagNode), which is compatible with all DAG objects, to perform
// basic queries on each node as the iterator traverses the DAG.
// 
// The iterator also provides the capability to reset the root of the
// iteration, the type of traversal, and the filter.
// 
// Additionally, the iterator can be queried for the root, mode and type
// of traversal, and to determine if the the traversal has been completed.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MIntArray.h>

// *****************************************************************************

// DECLARATIONS

class MDagPath;
class MDagPathArray;
class MString;
class MIteratorType;

// *****************************************************************************

// CLASS DECLARATION (MItDag)

/// DAG Iterator
/**

Class MItDag provides the capability to traverse the DAG and to retrieve
specific nodes for subsequent querying and editing using compatible
Function Sets.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MItDag  
{
public:
    /// Secondary Direction of Traversal (Vertical or Horizontal)
	enum TraversalType {
        ///
		kInvalidType = 0,
        ///
		kDepthFirst,
        ///
		kBreadthFirst
	};
    ///
	MItDag( TraversalType = kDepthFirst,
			MFn::Type = MFn::kInvalid,
			MStatus * ReturnStatus = NULL );
    ///
	MItDag( MIteratorType& dagInfoObject,
			TraversalType = kDepthFirst,
			MStatus* ReturnStatus = NULL);
    ///
	MStatus       reset();
    ///
	MStatus       reset( const MObject & object,
						 TraversalType = kDepthFirst,
						 MFn::Type = MFn::kInvalid );
    ///
	MStatus       reset( const MDagPath & path,
						 TraversalType = kDepthFirst,
						 MFn::Type = MFn::kInvalid );
    ///
	MStatus	      reset(MIteratorType& dagInfoObject,
						const MObject* object,
						const MDagPath* path,
						TraversalType = kDepthFirst);

    ///
	MObject       item( MStatus * ReturnStatus = NULL );
    ///
	MStatus       next();

    ///
	MStatus       prune();
    ///
	bool          isDone( MStatus * ReturnStatus = NULL ) const; 
    ///
	MObject       root( MStatus * ReturnStatus = NULL ); 
    ///
	unsigned      depth( MStatus * ReturnStatus = NULL ) const; 
    ///
	TraversalType getType( MStatus * ReturnStatus = NULL ) const; 

	///
	MStatus       getPath( MDagPath& path ) const;
	///
	MStatus       getAllPaths( MDagPathArray& paths ) const;
    ///
	MString       fullPathName(MStatus *ReturnStatus = NULL) const;
    ///
    MString       partialPathName(MStatus *ReturnStatus = NULL) const;
	///
	bool          isInstanced( bool indirect = true,
                               MStatus * ReturnStatus = NULL ) const;
	///
	unsigned int  instanceCount( bool total,
				                 MStatus * ReturnStatus = NULL ) const;
	///
	MStatus	      traverseUnderWorld( bool flag );
	///
	bool          willTraverseUnderWorld( MStatus * ReturnStatus = NULL ) const;

	///
	virtual ~MItDag();

protected:
// No protected members

private:
	static const char* className();
	bool objectHasFnOnFilterOrList(const MObject& mobject);
	bool dagPathHasFnOnFilterOrList(const MDagPath& dpath);
	MStatus filterObject(const MObject & object, MItDag::TraversalType type);
	MStatus filterDagPath(const MDagPath & path, MItDag::TraversalType type);
	void*		iterator_data;
	MFn::Type	type_filter;
    bool        df;

	MIntArray	filterList;
	bool		useFilterList;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItDag */
