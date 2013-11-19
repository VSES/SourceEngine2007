
#ifndef _MFnEnumAttribute
#define _MFnEnumAttribute
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
// CLASS:    MFnEnumAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnEnumAttribute)
//
// Function object for enumerated attributes.  These attributes accept fields as
// input. Each field consists of a short and its associated string, which is a
// descriptive name for the short.
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

// CLASS DECLARATION (MFnEnumAttribute)

/// Enumerated attribute function set
/**
  Function set for enumerated attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnEnumAttribute : public MFnAttribute
{

	declareMFn(MFnEnumAttribute, MFnAttribute);

public:

    ///
	MObject     create( const MString& fullName,
					    const MString& briefName, 
						short defaultValue = 0,
					    MStatus* ReturnStatus = NULL ); 
	///
	MStatus		addField( const MString & fieldString, short index);
	///
	MString     fieldName( short index, MStatus *ReturnStatus = NULL ) const;
	///
	short       fieldIndex( const MString & fieldString,
							MStatus *ReturnStatus = NULL ) const;
	///
	MStatus		getMin ( short& minValue ) const;
	///
	MStatus		getMax ( short& maxValue ) const;

	///
	MStatus 	setDefault( short index ); 
	///
	MStatus 	setDefault( const MString &fieldString );

	///
	MStatus		getDefault( short &index ) const; 
	///
	MStatus		getDefault( MString &fieldString ) const; 


protected:
// No protected members

private:

	MStatus		commonGetDefault( short &index ) const; 
	MStatus		commonSetDefault( short index ) const; 
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnEnumAttribute */
