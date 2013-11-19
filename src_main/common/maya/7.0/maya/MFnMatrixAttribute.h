
#ifndef _MFnMatrixAttribute
#define _MFnMatrixAttribute
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
// CLASS:    MFnMatrixAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnMatrixAttribute)
//
// Function object for matrix attributes.  These attributes accept either a
// matrix of float values or double values depending on the type specified to
// the create method.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

class MString;
class MMatrix;
class MFloatMatrix;

// CLASS DECLARATION (MFnMatrixAttribute)

/// Matrix attribute function set
/**
  Function set for matrix attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnMatrixAttribute : public MFnAttribute 
{

	declareMFn(MFnMatrixAttribute, MFnAttribute);

public:

	///
	enum Type {
		/// matrix of float values
		kFloat,
		/// matrix of double values
		kDouble
	};
 
    ///
	MObject     create( const MString& fullName,
					    const MString& briefName, 
						Type matrixType = kDouble,
					    MStatus* ReturnStatus = NULL ); 
    ///
	MStatus     getDefault( MMatrix & def );
    ///
	MStatus     getDefault( MFloatMatrix & def );
    ///
	MStatus     setDefault( const MMatrix & def );
    ///
	MStatus     setDefault( const MFloatMatrix & def ); 

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
#endif /* _MFnMatrixAttribute */



