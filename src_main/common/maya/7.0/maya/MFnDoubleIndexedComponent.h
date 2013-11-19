
#ifndef _MFnDoubleIndexedComponent
#define _MFnDoubleIndexedComponent
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
// CLASS:    MFnDoubleIndexedComponent
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnDoubleIndexedComponent)
// 
//    This function set allows you to create, edit, and query double indexed
//    components. Double indexed components store 2 dimensional index
//    values.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MString.h>
#include <maya/MFnComponent.h>

// *****************************************************************************

// DECLARATIONS

class MIntArray;

// *****************************************************************************

// CLASS DECLARATION (MFnDoubleIndexedComponent)

/// Double indexed component function set
/**
*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnDoubleIndexedComponent : public MFnComponent 
{

	declareMFn( MFnDoubleIndexedComponent, MFnComponent );

public:

	// Create a double indexed component of the given type.
	// Allowable types are
	//
	//    MFn::kSurfaceCVComponent
	//    MFn::kSurfaceEPComponent
	//    MFn::kSurfaceKnotComponent
	//    MFn::kMeshVtxFaceComponent
	//
	///
	MObject		create( MFn::Type compType, MStatus * ReturnStatus = NULL );

	///
    MStatus 	addElement( int uIndex, int vIndex );
	///
    MStatus 	addElements( const MIntArray& uIndexArray, 
							 const MIntArray& vIndexArray );

	///
    MStatus		getElement( int index, int & uIndex, int & vIndex ) const;
	///
    MStatus 	getElements( MIntArray& uIndexArray, 
							 MIntArray& vIndexArray ) const;

	///
	MStatus		setCompleteData( int maxU, int maxV );
	///
	MStatus		getCompleteData( int & maxU, int & maxV ) const;
    
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
#endif /* _MFnDoubleIndexedComponent */
