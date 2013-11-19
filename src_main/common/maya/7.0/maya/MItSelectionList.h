
#ifndef _MItSelectionList
#define _MItSelectionList
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
// CLASS:    MItSelectionList
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItSelectionList)
//
// Class for iterating over the items in an MSelection list.
// A filter can be specified so that only those items of interest on a selection
// list can be obtained.
// 
// If a filter is specified then the children of DAG selection items will be
// searched if the selection item does not match the filter.
// For example, if filter = MFn::kNurbsCurve and a transform is selected
// then the underlying shape for the transform will be in the iteration if it
// is a nurbs curve.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MStringArray.h>

// *****************************************************************************

// DECLARATIONS

class MDagPath;

// *****************************************************************************

// CLASS DECLARATION (MItSelectionList)

/// Iterate over the items in the selection list.
/**
  Class for iterating over the items in an MSelection list.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MItSelectionList
{
public:
    ///
	enum selItemType{
        ///
		kUnknownItem = -1,
        ///
		kDagSelectionItem,
        ///
		kAnimSelectionItem,
        ///
		kDNselectionItem
	};
    ///
	bool		isDone( MStatus * ReturnStatus = NULL ); 
    ///
	MStatus		next(); 
    ///
	MStatus		reset();
    ///
	MStatus		getDependNode( MObject &depNode );
    ///
	MStatus		getDagPath( MDagPath &dagPath, MObject &component );
	///
	MStatus     getDagPath( MDagPath &dagPath );
	///
	MStatus     getStrings( MStringArray & array );
    ///
	selItemType itemType( MStatus * ReturnStatus = NULL ); 
    ///
	MStatus		setFilter( MFn::Type filter );
	///
	bool		hasComponents( MStatus * ReturnStatus = NULL ) const;
    ///
	MItSelectionList( 	const MSelectionList & list, 
						MFn::Type = MFn::kInvalid,
						MStatus * ReturnStatus = NULL );
    ///
	virtual ~MItSelectionList();
protected:
// No protected members

private:
	static const char* 	className();
	MStatus				resetInner();
	void*				f_main_iter;
	void*				f_component_iter;
	void*				list_data;
	MFn::Type			f_filter;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItSelectionList */
