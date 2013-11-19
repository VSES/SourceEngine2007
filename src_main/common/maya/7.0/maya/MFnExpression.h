
#ifndef _MFnExpression
#define _MFnExpression
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
// CLASS:    MFnExpression
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnExpression)
//
// Function set for expressions.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

class MDoubleArray;
class MPtrBase;

// *****************************************************************************

// CLASS DECLARATION (MFnExpression)

/// expression function set
/**

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnExpression : public MFnDependencyNode
{ 
	declareMFn( MFnExpression, MFnDependencyNode ); 

public:
	
	///
	enum UnitConversion {
		/// convert all units
		kAll,
		/// do not do any unit conversion
		kNone,
		/// only convert angular units
		kAngularOnly
	};

	///
	MObject  		create( const MString & expression,
							MObject & object = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );

	///
	MStatus			getExpression( MString & expression );
	///
	MStatus			setExpression( MString & expression );

	///
	MStatus			getDefaultObject( MObject & object );
	///
	MStatus			setDefaultObject( MObject & object );
	///
	bool			isAnimated( MStatus * ReturnStatus = NULL );
	///
	MStatus			setAnimated( bool value = false );

	///
	MStatus			evaluate( MDoubleArray & result );

	///
	UnitConversion  unitConversion( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus         setUnitConversion( UnitConversion conversion );
							

protected:
// No protected members

private:
// No private members

}; 

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

#endif /* __cplusplus */
#endif /* _MFnExpression */
