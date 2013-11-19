#ifndef _MFloatPoint
#define _MFloatPoint
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
// CLASS:    MFloatPoint
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFloatPoint)
//
//	This class provides an implementation of a point in float.  Numerous 
//	convienence operators are provided to help with the manipulation of 
//	points. This includes operators that work with the MFloatVector and 
//	MFloatMatrix classes.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MIOStreamFwd.h>

// *****************************************************************************

// DECLARATIONS

class MFloatMatrix;
class MFloatVector;

#define MFloatPoint_kTol	1.0e-10

// *****************************************************************************

// CLASS DECLARATION (MFloatPoint)

/// implementation of a point
/**
  This class implements the Maya representation of a point in floats
*/
#if defined(_WIN32)
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFloatPoint  
{
public:
	///
						MFloatPoint();	// defaults to 0,0,0,1
	///
						MFloatPoint( const MFloatPoint & srcpt );  
	///
						MFloatPoint( float xx, float yy,
									 float zz = 0.0, float ww = 1.0 );
	///
						MFloatPoint( const float[4] );
	///
						~MFloatPoint();
	///
	bool				get( float[4] ) const;
	///
	MFloatPoint &		operator=( const MFloatPoint & src );
	///
	float &         	operator()(unsigned i);
	///
	float				operator()(unsigned i) const;
	///
	float &         	operator[](unsigned i);
	///
	float				operator[](unsigned i) const;
	///
	MFloatVector		operator-( const MFloatPoint & other ) const;
	///
	MFloatPoint			operator+( const MFloatVector & other ) const;
	///
	MFloatPoint			operator-( const MFloatVector & other ) const;
	///
	MFloatPoint &		operator+=( const MFloatVector & vector );
	///
	MFloatPoint &		operator-=( const MFloatVector & vector );
	///
	MFloatPoint			operator*(const float scale) const;
	///
	MFloatPoint			operator/(const float scale) const;
	///
	MFloatPoint			operator*(const MFloatMatrix &) const;
	///
	MFloatPoint &		operator*=(const MFloatMatrix &);
	///
	friend OPENMAYA_EXPORT MFloatPoint operator*( const MFloatMatrix &,
												  const MFloatPoint & );
	///
	bool				operator==( const MFloatPoint & other ) const;
	///
	bool				operator!=( const MFloatPoint & other ) const;
	///
	MFloatPoint &		cartesianize();
	///
	MFloatPoint &		rationalize();
	///
	MFloatPoint &		homogenize();
	///
	float				distanceTo( const MFloatPoint & other ) const;
	///
	bool				isEquivalent( const MFloatPoint & other,
									  float tolerance = MFloatPoint_kTol)
									const;

	///
	friend OPENMAYA_EXPORT IOS_REF(ostream)& operator<< ( IOS_REF(ostream)& os,
												 const MFloatPoint& p );

	///
	static const MFloatPoint origin;
	/// the x component of the point
	float				x;
	/// the y component of the point
	float				y;
	/// the z component of the point
	float				z;
	/// the w component of the point
	float				w;

protected:
// No protected members

private:
// No private members

};

#if defined(_WIN32)
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFloatPoint */
