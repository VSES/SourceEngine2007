
#ifndef _MSelectionList
#define _MSelectionList
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
// CLASS:    MSelectionList
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MSelectionList)
//
//	This class implements a list of MObjects.
//
//	The global selection list is a special case where the objects on the list
//	are also active objects in Maya.
//
//	Besides the usual list methods, this class also provides an add method
//	which retrieves objects from Maya, such as dependency nodes, by name.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MDagPath;
class MString;
class MStringArray;
class MPlug;

// *****************************************************************************

// CLASS DECLARATION (MSelectionList)

/// A list of MObjects
/**
  Implement a list of MObjects.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MSelectionList  
{
public:

	///
	enum MergeStrategy {
		///
		kMergeNormal=0,
		///
		kXORWithList,
		///
		kRemoveFromList
	};
		
    ///
	MSelectionList();
    ///
	MSelectionList( const MSelectionList & src );

    ///
	virtual ~MSelectionList();

    ///
	MStatus			clear	();
    ///
	bool			isEmpty	( MStatus * ReturnStatus = NULL ) const;
    ///
	unsigned int	length	( MStatus * ReturnStatus = NULL ) const; 
    ///
	MStatus		    getDependNode ( unsigned index, MObject &depNode ) const;
    ///
	MStatus		    getDagPath    ( unsigned index, MDagPath &dagPath, 
								    MObject &component = MObject::kNullObj
									) const;
    ///
	MStatus		    getPlug	( unsigned index, MPlug &plug ) const;

    ///
	MStatus			add		( const MObject & object,
							  const bool mergeWithExisting = false );
    ///
	MStatus			add		( const MDagPath & object, 
							  const MObject & component = MObject::kNullObj,
							  const bool mergeWithExisting = false );
	///
	MStatus         add     ( const MString & matchString );

    ///
	MStatus			add		( const MPlug & plug,
							  const bool mergeWithExisting = false );

    ///
	MStatus			remove	( unsigned int index );
    ///
	MStatus			replace	( unsigned index, const MObject & item );
    ///
	MStatus			replace	( unsigned index,
							  const MDagPath& item,
							  const MObject& component = MObject::kNullObj );
    ///
	MStatus			replace	( unsigned index, const MPlug & plug );

    ///
	bool			hasItem ( const MObject & item,
							  MStatus* ReturnStatus = NULL ) const;
	///
	bool			hasItem ( const MDagPath& item,
							  const MObject& component = MObject::kNullObj,
							  MStatus* ReturnStatus = NULL ) const;
    ///
	bool			hasItem ( const MPlug & plug,
							  MStatus* ReturnStatus = NULL ) const;

    ///
	bool			hasItemPartly ( const MDagPath& item,
									const MObject& component,
									MStatus* ReturnStatus = NULL ) const;
    ///
	MStatus			toggle ( const MDagPath& item,
							 const MObject& component = MObject::kNullObj );

	///
	MSelectionList& operator =( const MSelectionList& other );

	///
	MStatus			merge( const MSelectionList& other, 
						   const MergeStrategy strategy = kMergeNormal );
	///
	MStatus			merge( const MDagPath& object,
						   const MObject& component = MObject::kNullObj,
						   const MergeStrategy strategy = kMergeNormal );

	///
	MStatus         getSelectionStrings( MStringArray & array ) const;
	///
	MStatus         getSelectionStrings( unsigned index,
										 MStringArray & array ) const;


protected:
// No protected members

private:
	MSelectionList( void * );
	static const char* className();
	void merge( const void*, const MergeStrategy strategy );
	void * list_data;
	bool fOwn;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MSelectionList */
