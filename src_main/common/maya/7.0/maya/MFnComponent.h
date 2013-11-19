
#ifndef _MFnComponent
#define _MFnComponent
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
// CLASS:    MFnComponent
//
//    This is the base class for all function sets which deal with
//    component objects.
//
//    Components are MObjects which hold index information for shapes.
//    The component object does not contain any information about the
//    shape, it just contains index values and type information.
//
//    Components can be marked as complete meaning that the component
//    represents a complete set of indices from 0 to numElements-1;
//
//    Components come is 3 varieties based upon the dimensions of the
//    index. The types are single, double, and triple indexed.
//    Examples of these types are mesh vertices (single indexed),
//    nurbs surface CVs (double indexed), and lattice points (triple indexed).
//
//    To create, edit, or query a specific type of component, use the
//    derived classes MFnSingleIndexedComponent, MFnDoubleIndexedComponent,
//    and MFnTripleIndexedComponent.
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnComponent)
// 
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

class MIntArray;

// *****************************************************************************

// CLASS DECLARATION (MFnComponent)

/// Base class for component function sets
/**
*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnComponent : public MFnBase 
{

	declareMFnNoConstructor( MFnComponent, MFnBase );

public:
	///
	MFnComponent();
    ///
	int        elementCount( MStatus* ReturnStatus = NULL ); 
	///
	MFn::Type	type( MStatus * ReturnStatus = NULL );
	///
	bool		isEmpty( MStatus * ReturnStatus = NULL ) const;
	///
	bool		isEqual( MObject &, MStatus * ReturnStatus = NULL ) const;
	///
	bool		isComplete( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus		setComplete( bool );
    
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
#endif /* _MFnComponent */
