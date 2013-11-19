
#ifndef _MFnNumericData
#define _MFnNumericData
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
// CLASS:    MFnNumericData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnNumericData)
//
//  MFnNumericData allows the creation and manipulation of numeric data objects
//  for use in the dependency graph.  Normally, data objects are not required 
//  for the transmission of numeric data.  The graph supports numeric types 
//  directly (see the methods of MDataHandle).
//
//  Numeric data objects are useful if you have an attribute that accepts
//  generic data.  A generic attribute can accept multiple types of data,
//  so you cannot hardwire it to accept a specific type of numeric data.
//  So, generic attributes can only accept numeric data in the form of actual
//  data objects.
//
//  This function set only supports pairs and triples of numeric data.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnData.h>

// *****************************************************************************

// CLASS DECLARATION (MFnNumericData)

/// Numeric data function set.
/**
 MFnNumericData allows the manipulation of numeric data for
 dependency node attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnNumericData : public MFnData 
{

	declareMFn(MFnNumericData, MFnData);

public:

	///
	enum Type {
        ///
		kInvalid,
        /// Boolean
		kBoolean,
        /// One byte
		kByte,
        /// One character
		kChar,
        /// One short
		kShort,
		/// Two shorts
		k2Short,
		/// Three shorts
		k3Short,
        /// One long  (actually int since "long" is not platform-consistent)
		kLong,
		/// One int		
		kInt = kLong,
		/// Two longs  (actually 2 ints since "long" is not platform-consistent)
		k2Long,
		/// Two ints
		k2Int = k2Long,
		/// Three longs  (actually 3 ints since "long" is not platform-consistent)
		k3Long,
		/// Three ints
		k3Int = k3Long,
        /// One float
		kFloat,
		/// Two floats
		k2Float,
		/// Three floats
		k3Float,
        /// One double
		kDouble,
		/// Two doubles
		k2Double,
		/// Three doubles
		k3Double,
		///
		kLast
    };

	///
	MObject create( Type dataType, MStatus* ReturnStatus = NULL );

	///
	Type numericType( MStatus* ReturnStatus = NULL );
	///
	MStatus getData( short& val1, short& val2 );
	///
	MStatus getData( int& val1, int& val2 );
	///
	MStatus getData( float& val1, float& val2 );
	///
	MStatus getData( double& val1, double& val2 );
	///
	MStatus getData( short& val1, short& val2, short& val3 );
	///
	MStatus getData( int& val1, int& val2, int& val3 );
	///
	MStatus getData( float& val1, float& val2, float& val3 );
	///
	MStatus getData( double& val1, double& val2, double& val3 );

	///
	MStatus setData( short val1, short val2 );
	///
	MStatus setData( int val1, int val2 );
	///
	MStatus setData( float val1, float val2 );
	///
	MStatus setData( double val1, double val2 );
	///
	MStatus setData( short val1, short val2, short val3 );
	///
	MStatus setData( int val1, int val2, int val3 );
	///
	MStatus setData( float val1, float val2, float val3 );
	///
	MStatus setData( double val1, double val2, double val3 );

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
#endif /* _MFnNumericData */
