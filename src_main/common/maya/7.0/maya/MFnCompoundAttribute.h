
#ifndef _MFnCompoundAttribute
#define _MFnCompoundAttribute
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
// CLASS:    MFnCompoundAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnCompoundAttribute)
// 
//  MFnCompoundAttribute is the function set for compound dependency node 
//  attributes.
//
//  Compound attributes allow the grouping of related attributes into a larger
//  unit.  It is possible to connect to a compound attribute as a whole, or 
//  to any of the individual children. 
//
//  For example, the three attributes RED, GREEN, and BLUE could be grouped
//  into a compound attribute of type COLOR.  It is then possible to connect
//  two COLOR attributes together.  This removes the need to connect each
//  child explicitly.
//
//  A second use for compound attributes is when there are multi attributes that
//  relate to each other on an element-by-element basis.  An example of this 
//  is the weighted matrix node that has a multi attribute with matrices that
//  must be matched with the multi attribute that provides the weights for each
//  matrix.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>

// *****************************************************************************

// DECLARATIONS

class MString;

// *****************************************************************************

// CLASS DECLARATION (MFnCompoundAttribute)

/// Compound attribute function set
/**
   Function set for compound attributes of dependency nodes
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnCompoundAttribute : public MFnAttribute 
{

	declareMFn(MFnCompoundAttribute, MFnAttribute);

public:
    ///
	MObject 	create( const MString& full,
						const MString& brief,
						MStatus* ReturnStatus = NULL );
    ///
	MStatus		addChild( const MObject & child );
    ///
	MStatus		removeChild( const MObject & child );
    ///
	unsigned	numChildren( MStatus* ReturnStatus = NULL ) const;
    ///
	MObject 	child( unsigned index, MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		getAddAttrCmds(
					MStringArray&	cmds,
					bool			useLongNames = false
				) const;

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
#endif /* _MFnCompoundAttribute */
