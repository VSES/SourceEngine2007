
#ifndef _MFnSet
#define _MFnSet
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
// CLASS:    MFnSet
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnSet)
//
//  MFnSet is the function set that is used for manipulating sets of objects.
//  Sets in Maya are dependency nodes, so it is possible for one set to 
//  contain others.
//
//  A set is a list of dependency nodes and dag nodes.  Sets are useful for
//  keeping track of lists of objects for many purposes such as selection
//  or applying common rendering parameters.  Sets in Maya are also dependency 
//  nodes, so it is possible for one set to contain others..
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MString.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS
 
class MObjectArray;
class MSelectionList;
class MDagPath;
class TsetCmd;
class Tstring;

// *****************************************************************************

// CLASS DECLARATION (MFnSet)

/// Function Set for Sets of Objects
/**
    Function set for sets of objects
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnSet : public MFnDependencyNode 
{

	declareMFn(MFnSet, MFnDependencyNode);

public:
	
	///
	enum Restriction {
		///
		kNone,
		///
		kVerticesOnly,
		///
		kEdgesOnly,
		///
		kFacetsOnly,
		///
		kEditPointsOnly,
		///
		kRenderableOnly
	};

	///
	MObject     create( const MSelectionList & members,
						         Restriction restriction = kNone, 
						         MStatus * ReturnStatus = NULL );

	///
	MStatus     getUnion( const MObject & withSet, MSelectionList & result );
	///
	MStatus     getUnion( const MObjectArray & setList,
						  MSelectionList & result );
	///
	MStatus     getIntersection( const MObject & withSet, 
								 MSelectionList & result );
	///
	MStatus     getIntersection( const MObjectArray & setList, 
								 MSelectionList & result );

	///
	MStatus     clear();
	///
	MStatus     getMembers( MSelectionList &members, bool flatten ) const;
	///
	MStatus     addMember( const MObject &obj );
	///
	MStatus     addMember( const MDagPath &obj, 
						   const MObject &component = MObject::kNullObj );
	///
	MStatus     addMember( const MPlug &plug );
	///
	MStatus     addMembers( const MSelectionList &list );
	///
	MStatus     removeMember( const MObject &obj );
	///
	MStatus     removeMember( const MDagPath &obj, const MObject &component );
	///
	MStatus     removeMember( const MPlug &plug );
	///
	MStatus     removeMembers( const MSelectionList &list );

	///
	bool        isMember( const MObject &object,
						  MStatus * ReturnStatus = NULL ) const;
	///
	bool        isMember( const MDagPath &object, 
						  const MObject &component = MObject::kNullObj,
						  MStatus * ReturnStatus = NULL ) const;
	///
	bool        isMember( const MPlug &plug,
						  MStatus * ReturnStatus = NULL ) const;

	///
	bool        intersectsWith( const MObject & otherSet, 
								 MStatus * ReturnStatus = NULL ) const;
	///
	bool        hasRestrictions( MStatus * ReturnStatus = NULL ) const;
	///
	Restriction restriction( MStatus * ReturnStatus = NULL ) const;
	///
	MString     annotation( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setAnnotation( const MString &annotation );

	// OBSOLETE METHODS
	MObject     create( const MSelectionList & members,
						         Restriction restriction = kNone, 
						         bool isLayer = false,
						         MStatus * ReturnStatus = NULL );
protected:
	// for use for api implementation of classes derived from setCmd
	//
	virtual		Tstring setCommandString();
	virtual		TsetCmd* setCommand();
private:
// No private members
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnSet */
