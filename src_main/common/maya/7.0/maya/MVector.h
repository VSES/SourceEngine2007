#ifndef _MVector
#define _MVector
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
// CLASS:    MVector
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MVector)
//
//	This class provides access to Maya's internal vector math library allowing
//	vectors to be handled easily, and in a manner compatible with internal
//	Maya data structures.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MTransformationMatrix.h>

// *****************************************************************************

// DECLARATIONS

class MMatrix;
class MFloatVector;
class MPoint;
class MQuaternion;
class MEulerRotation;
#define MVector_kTol 1.0e-10

// *****************************************************************************

// CLASS DECLARATION (MVector)

/// A vector math class for vectors of doubles
/**
  This class provides access to Maya's vector math library.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MVector  
{
public:
	
	///
	enum Axis {
		///
		kXaxis,
		///
		kYaxis,
		///
		kZaxis,
		///
		kWaxis
	};

	///
					MVector();
	///
					MVector( const MVector&);
	///
					MVector( const MFloatVector&);
	///
					MVector( const MPoint&);
	///
					MVector( double xx, double yy, double zz = 0.0);
	///
					MVector( const double[3] );
	///
					~MVector();
	///
 	MVector&		operator= ( const MVector& src );
	///
 	double&      	operator()( unsigned i );
	///
 	double   		operator()( unsigned i ) const;
	///
 	double&      	operator[]( unsigned i );
	///
	double			operator[]( unsigned i )const;
	///
 	MVector			operator^( const MVector& right) const;
	///
 	double          operator*( const MVector& right ) const;
	///
 	MVector&   		operator/=( double scalar );
	///
 	MVector     	operator/( double scalar ) const;
	///
 	MVector& 		operator*=( double scalar );
	///
 	MVector   		operator*( double scalar ) const;
	///
 	friend OPENMAYA_EXPORT MVector operator*( int, const MVector&);
	///
 	friend OPENMAYA_EXPORT MVector operator*( short, const MVector&);
	///
 	friend OPENMAYA_EXPORT MVector operator*( unsigned int, const MVector&);
	///
 	friend OPENMAYA_EXPORT MVector operator*( unsigned short, const MVector&);
	///
 	friend OPENMAYA_EXPORT MVector operator*( float, const MVector&);
	///
 	friend OPENMAYA_EXPORT MVector operator*( double, const MVector&);
	///
 	MVector   		operator+( const MVector& other) const;
	///
	MVector&		operator+=( const MVector& other );
	///
 	MVector   		operator-() const;
	///
 	MVector   		operator-( const MVector& other ) const;
	///
 	MVector  		operator*( const MMatrix&) const;
	///
 	MVector&		operator*=( const MMatrix&);
	///
 	friend OPENMAYA_EXPORT MVector operator*( const MMatrix&, const MVector&);
	///
 	bool          	operator!=( const MVector& other ) const;
	///
 	bool           	operator==( const MVector& other ) const;
	///
	MVector			rotateBy( double x, double y, double z, double w) const;
	///
	MVector			rotateBy( const double rotXYZ[3], 
							  MTransformationMatrix::RotationOrder order )
                              const;
	///
	MVector			rotateBy( MVector::Axis axis, const double angle ) const;
	///
	MVector			rotateBy( const MQuaternion & ) const;
	///
	MVector			rotateBy( const MEulerRotation & ) const;
	///
	MQuaternion		rotateTo( const MVector & ) const;
	///
	MStatus			get( double[3] ) const;
	///
 	double         	length() const;
	///
 	MVector  		normal() const;
	///
	MStatus			normalize();
	///
 	double       	angle( const MVector& other ) const;
	///
	bool			isEquivalent( const MVector& other,
						double tolerance = MVector_kTol ) const;
	///
 	bool          	isParallel( const MVector& other,
						double tolerance = MVector_kTol ) const;
	///
	MVector			transformAsNormal( const MMatrix & matrix ) const;


	///
	friend OPENMAYA_EXPORT IOS_REF(ostream)&	operator<<(IOS_REF(ostream)& os, const MVector& v);

	/// The null vector
	static const MVector zero;
	/// The vector <1,1,1>
	static const MVector one;
	/// Unit vector in the positive x direction
	static const MVector xAxis;
	/// Unit vector in the positive y direction
	static const MVector yAxis;
	/// Unit vector in the positive z direction
	static const MVector zAxis;
	/// Unit vector in the negative z direction
	static const MVector xNegAxis;
	/// Unit vector in the negative z direction
	static const MVector yNegAxis;
	/// Unit vector in the negative z direction
	static const MVector zNegAxis;
	/// The x component of the vector
	double x;
	/// The y component of the vector
	double y;
	/// The z component of the vector
	double z;

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
#endif /* _MVector */
