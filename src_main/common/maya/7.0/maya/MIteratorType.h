#ifndef _MIteratorType
#define _MIteratorType

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
// CLASS:    MIteratorType
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MIteratorType)
//
// Use MIteratorType class object when we need to specify more than one
// filter, ie, filter list, to the iterator.
// MIteratorType provides functionalities to set and get the filter list
// or individual types of filter. This class should be used in conjunction
// with DAG/DG/DependencyNodes iterators for using filter list (list of 
// MFn::Type objects) on them, thus enabling faster traversal thro' iterators.
//
// Also, the class provides functionalities for specifying the type of
// object the iterator will be reset to. This could be an MObject, an MPlug
// or an MDagPath.
// 
// *****************************************************************************

#include <maya/MIntArray.h>
#include <maya/MTypes.h>
#include <maya/MStatus.h>

/// MIteratorType class
/**
  This class can be used on iterators when more than one type of 
  filters needs to be specified.
*/
class OPENMAYA_EXPORT MIteratorType
{
public:

	/// Type of object the iterator deals with.
	enum objFilterType
	{
		/// This is to indicate that the root of the iterator has to be reset to an MObject
		kMObject,
		/// This is to indicate that the root of the iterator has to be reset to an MDagPath object.
		kMDagPathObject,
		/// This is to indicate that the root of the iterator has to be reset to an MPlug object.
		kMPlugObject
	};

	///
						MIteratorType(MStatus* ReturnStatus = NULL);
	///
						MIteratorType( const MIteratorType &, MStatus* ReturnStatus = NULL);
	///
						~MIteratorType();
	///
	void				setFilterType(MFn::Type type, MStatus* ReturnStatus = NULL);
	///
	void				setFilterList(MIntArray& listOfFilters, MStatus* ReturnStatus = NULL);
	///
	void				setObjectType(objFilterType objType, MStatus* ReturnStatus = NULL);
	///
	MFn::Type			getFilterType(MStatus* ReturnStatus = NULL);
	///
	MStatus				getFilterList(MIntArray& listOfFilters);
	///
	MIteratorType::objFilterType	
						getObjectType(MStatus* ReturnStatus = NULL);
	///
	bool				filterListEnabled();

private:
	MFn::Type			filterType;
	MIntArray			filterList;
	bool				useFilterList;
	objFilterType		objType;
};

#endif /* _MIteratorType */
