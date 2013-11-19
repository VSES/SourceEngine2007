
#ifndef _MArgList
#define _MArgList
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
// CLASS:    MArgList
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MArgList)
//
//  This class is used to create and retreive argument lists that can be
//  passed to API class methods that require arglists.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

class MVector;
class MPoint;
class MString;
class MVectorArray;
class MPointArray;
class MStringArray;
class MMatrix;
class MDoubleArray;
class MIntArray;
class MDistance;
class MAngle;
class MTime;

// *****************************************************************************

// CLASS DECLARATION (MArgList)

/// Create and retreive argument lists.
/**
  Manipulate argument lists.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MArgList  
{
public:
	///
	static const unsigned kInvalidArgIndex;
	///
					MArgList();
	///
					MArgList( const MArgList& other );
	///
					~MArgList();
	///
	unsigned		length( MStatus* ReturnStatus= NULL ) const;
	///
	bool			asBool(		unsigned index, MStatus* ret=NULL ) const;
	///
	int				asInt(		unsigned index, MStatus* ret=NULL ) const;
	///
	double			asDouble(	unsigned index, MStatus* ret=NULL ) const;
	///
	MString			asString(	unsigned index, MStatus* ret=NULL ) const;
	///
	MDistance		asDistance(	unsigned index, MStatus* ret=NULL ) const;
	///
	MAngle			asAngle(	unsigned index, MStatus* ret=NULL ) const;
	///
	MTime			asTime(		unsigned index, MStatus* ret=NULL ) const;
	///
	MVector			asVector(	unsigned& index,
								unsigned numElements=3,
								MStatus* ReturnStatus=NULL ) const;
	///
	MPoint			asPoint(	unsigned& index,
							 unsigned numElements=3, MStatus* ReturnStatus=NULL ) const;
	///
	MStatus			get( unsigned index, bool& ret ) const;
	///
	MStatus			get( unsigned index, int& ret ) const;
	///
	MStatus			get( unsigned index, double& ret ) const;
	///
	MStatus			get( unsigned index, MString& ret ) const;
	///
	MStatus			get( unsigned index, MDistance& ret ) const;
	///
	MStatus			get( unsigned index, MAngle& ret ) const;
	///
	MStatus			get( unsigned index, MTime& ret ) const;
	///
	MStatus			get( unsigned& index, MVector& ret,
						 unsigned numElements=3 ) const;
	///
	MStatus			get( unsigned& index, MPoint& ret,
						 unsigned numElements=3 ) const;
	///
	MStatus			get( unsigned& index, MMatrix& ret ) const;
	///
	MStatus			get( unsigned& index, MIntArray& ret ) const;
	///
	MStatus			get( unsigned& index, MDoubleArray& ret ) const;
	///
	MStatus         get( unsigned& index, MStringArray& ret ) const;
	///
	MArgList&		operator =( const MArgList& other );

	// Argument list helper methods
	///
	unsigned		flagIndex (	const char *shortFlag,
								const char *longFlag = NULL ) const;

	// Command argument construction methods

	///
	MStatus			addArg( bool arg );
	///
	MStatus			addArg( int arg );
	///
	MStatus			addArg( double arg );
	///
	MStatus 		addArg( const MString & arg );
	///
	MStatus 		addArg( const MDistance & arg );
	///
	MStatus 		addArg( const MAngle & arg );
	///
	MStatus 		addArg( const MTime & arg );
	///
	MStatus			addArg( const MVector & arg );
	///
	MStatus			addArg( const MPoint & arg );


protected:
// No protected members

private:
	const char*		className() const;
	MArgList ( const void*, unsigned offset );
	const void*		fArgList;
	unsigned		fOffset;
	bool			fOwn;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MArgList */
