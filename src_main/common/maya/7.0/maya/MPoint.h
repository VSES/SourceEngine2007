#ifndef _MPoint
#define _MPoint
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
// CLASS:    MPoint
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPoint)
//
//	This class provides an implementation of a point.  Numerous convienence
//	operators are provided to help with the manipulation of points. This
//	includes operators that work with the MVector and MMatrix classes.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MMatrix;
class MVector;
class MFloatVector;

#define MPoint_kTol	1.0e-10

// *****************************************************************************

// CLASS DECLARATION (MPoint)

/// implementation of a point
/**
  This class implements the Maya representation of a point
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPoint  
{
public:
	///
					MPoint();	// defaults to 0,0,0,1
	///
					MPoint( const MPoint & srcpt );  
	///
					MPoint( const MVector & src );  
	///
					MPoint( const MFloatVector & src );  
	///
					MPoint( double xx, double yy,
							double zz = 0.0, double ww = 1.0 );
	///
					MPoint( const double[4] );
	///
					MPoint( const float[4] );
	///
					~MPoint();
	///
	MStatus			get( double[4] ) const;
	///
	MStatus			get( float[4] ) const;
	///
	MPoint & 		operator=( const MPoint & src );
	///
	double &        operator()(unsigned i);
	///
	double  		operator()(unsigned i) const;
	///
	double &        operator[](unsigned i);
	///
	double  		operator[](unsigned i) const;
	///
	MVector   		operator-( const MPoint & other ) const;
	///
	MPoint   		operator+( const MVector & other ) const;
	///
	MPoint   		operator-( const MVector & other ) const;
	///
	MPoint & 		operator+=( const MVector & vector );
	///
	MPoint & 		operator-=( const MVector & vector );
	///
	MPoint			operator*(const double scale) const;
	///
	MPoint			operator/(const double scale) const;
	///
	MPoint   		operator*(const MMatrix &) const;
	///
	MPoint & 		operator*=(const MMatrix &);
	///
	friend OPENMAYA_EXPORT MPoint operator*(const MMatrix &, const MPoint &);
	///
	bool           	operator==( const MPoint & other ) const;
	///
	bool           	operator!=( const MPoint & other ) const;
	///
	MPoint & 		cartesianize();
	///
	MPoint & 		rationalize();
	///
	MPoint & 		homogenize();
	///
	double     		distanceTo( const MPoint & other ) const;
	///
	bool           	isEquivalent( const MPoint & other,
								  double tolerance = MPoint_kTol) const;

	///
	friend OPENMAYA_EXPORT IOS_REF(ostream)& operator<<(IOS_REF(ostream)& os, const MPoint& p);

	///
	static const MPoint origin;
public:
	/// the x component of the point
	double			x;
	/// the y component of the point
	double			y;
	/// the z component of the point
	double			z;
	/// the w component of the point
	double			w;

protected:
// No protected members

private:

	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPoint */
