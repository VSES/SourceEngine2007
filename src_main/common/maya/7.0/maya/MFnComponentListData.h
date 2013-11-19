
#ifndef _MFnComponentListData
#define _MFnComponentListData
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
// CLASS:    MFnComponentListData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnComponentListData)
//
// MFnComponentListData allows the creation and manipulation of component list
// (represented as MObjects) data objects for use in the dependency graph.
// 
// If a user written dependency node either accepts or produces MObjects
// containing component lists, then this class is used to extract or
// create the data that comes from or goes to other dependency graph
// nodes.  To access it, the MDataHandle::data method is used to get an
// MObject for the data and this should then be used to initialize an
// instance of MFnComponentListData.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnData.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnComponentListData)

/// Component list function set for dependency node data
/**
  Create and manipulate Component Lists dependency node data.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnComponentListData : public MFnData 
{

	declareMFn(MFnComponentListData, MFnData);

public:
	///
	unsigned		length( MStatus* ReturnStatus = NULL ) const;
	///
	bool			has( const MObject& obj,
						 MStatus* ReturnStatus = NULL ) const;
	///
	MObject			operator[]( unsigned index) const;
	///
	MStatus			add( MObject& );
	///
	MStatus			remove( const MObject& );
	///
	MStatus			remove( unsigned index );
	///
	MStatus			clear();
	///
	MObject			create( MStatus* ReturnStatus = NULL );

protected:
// No protected members

private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnComponentListData */
