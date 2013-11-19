#ifndef _MFnNumericAttribute
#define _MFnNumericAttribute
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
// CLASS:    MFnNumericAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnNumericAttribute)
//
//	MFnNumericAttribute is the function set for dependency node attributes
//	that store one, two or three untyped numeric values.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MString;

// *****************************************************************************

// CLASS DECLARATION (MFnNumericAttribute)

/// Numeric attribute function set
/**
  Function set for numeric attributes of dependency nodes
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnNumericAttribute : public MFnAttribute 
{

	declareMFn(MFnNumericAttribute, MFnAttribute);

public:
    ///
	MObject 	create( const MString& fullName,
						const MString& briefName,
						MFnNumericData::Type unitType,
						double defaultValue =0,
						MStatus* ReturnStatus = NULL );
    ///
	MObject 	createColor( const MString& fullName,
							 const MString& briefName,
							 MStatus* ReturnStatus = NULL );
    ///
	MObject 	createPoint( const MString& fullName,
							 const MString& briefName,
							 MStatus* ReturnStatus = NULL );
    ///
	MObject	    create( const MString& fullName,
						const MString& briefName,
						const MObject & child1,
						const MObject & child2,
						const MObject & child3 = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
    ///
	MFnNumericData::Type	unitType( MStatus* ReturnStatus = NULL ) const;
    ///
	MObject 	child( unsigned index, MStatus* ReturnStatus = NULL ) const;
    ///
	bool		hasMin( MStatus* ReturnStatus = NULL) const;
    ///
	bool		hasMax( MStatus* ReturnStatus = NULL) const;
	///
	bool        hasSoftMin( MStatus* ReturnStatus = NULL) const;
	///
	bool        hasSoftMax( MStatus* ReturnStatus = NULL) const;
    ///
	MStatus		getMin( double& min1 ) const;
    ///
	MStatus		getMin( double& min1, double& min2 ) const;
    ///
	MStatus		getMin( double& min1, double& min2, double& min3 ) const;
    ///
	MStatus		getMax( double& max1 ) const;
    ///
	MStatus		getMax( double& max1, double& max2 ) const;
    ///
	MStatus		getMax( double& max1, double& max2, double& max3 ) const;
    ///
	MStatus		getSoftMin( double& min1 ) const;
    ///
	MStatus		getSoftMax( double& max1 ) const;
    ///
	MStatus		setMin( double min1 );
    ///
	MStatus		setMin( double min1, double min2 );
    ///
	MStatus		setMin( double min1, double min2, double min3 );
    ///
	MStatus		setMax( double max1 );
    ///
	MStatus		setMax( double max1, double max2 );
    ///
	MStatus		setMax( double max1, double max2, double max3 );
    ///
	MStatus		setSoftMin( double min1 );
    ///
	MStatus		setSoftMax( double max1 );
	///
	MStatus		getDefault( bool & def1 );
    ///
	MStatus		getDefault( char & def1 );
    ///
	MStatus		getDefault( int & def1 );
    ///
	MStatus		getDefault( int & def1, int & def2 );
    ///
	MStatus		getDefault( int & def1, int & def2, int & def3 );
    ///
	MStatus		getDefault( float & def1 );
    ///
	MStatus		getDefault( float & def1, float & def2 );
    ///
	MStatus		getDefault( float & def1, float & def2, float & def3 );
    ///
	MStatus		getDefault( double & def1 );
    ///
	MStatus		getDefault( double & def1, double & def2 );
    ///
	MStatus		getDefault( double & def1, double & def2, double & def3 );
    ///
	MStatus		setDefault( bool def1 );
    ///
	MStatus		setDefault( char def1 );
    ///
	MStatus		setDefault( int def1 );
    ///
	MStatus		setDefault( int def1, int def2 );
    ///
	MStatus		setDefault( int def1, int def2, int def3 );
    ///
	MStatus		setDefault( float def1 );
    ///
	MStatus		setDefault( float def1, float def2 );
    ///
	MStatus		setDefault( float def1, float def2, float def3 );
    ///
	MStatus		setDefault( double def1 );
    ///
	MStatus		setDefault( double def1, double def2 );
    ///
	MStatus		setDefault( double def1, double def2, double def3 );

protected:
// No protected members

private:

	virtual bool objectChanged( MFn::Type, MStatus * );

	MObject 	internalCreate( const MString& fullName,
						const MString& briefName,
						MFnNumericData::Type unitType,
								const char** childSuffixes,
						double defaultValue,
						MStatus* ReturnStatus);

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnNumericAttribute */
