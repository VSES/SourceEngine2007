#ifndef _MFloatVector
#define _MFloatVector
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
// CLASS:    MFloatVector
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFloatVector)
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
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MFloatMatrix;
class MVector;
class MPoint;
#define MFloatVector_kTol 1.0e-5F

// *****************************************************************************

// CLASS DECLARATION (MFloatVector)

/// A vector math class for vectors of floats
/**
  This class provides access to Maya's vector math library.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFloatVector  
{
public:
	///
						MFloatVector();
	///
						MFloatVector( const MFloatVector&);
	///
						MFloatVector( const MVector&);
	///
						MFloatVector( const MPoint&);
	///
						MFloatVector( float xx, float yy, float zz = 0.0);
	///
						MFloatVector( const float[3] );
	///
						~MFloatVector();
	///
 	MFloatVector&		operator= ( const MFloatVector& src );
	///
 	float&     		 	operator()( unsigned i );
	///
 	float   			operator()( unsigned i ) const;
	///
 	float&     		 	operator[]( unsigned i );
	///
	float				operator[]( unsigned i )const;
	///
 	MFloatVector		operator^( const MFloatVector& right) const;
	///
 	MFloatVector&   	operator/=( float scalar );
	///
 	MFloatVector 	    operator/( float scalar ) const;
	///
 	MFloatVector& 		operator*=( float scalar );
	///
 	MFloatVector   		operator*( float scalar ) const;
	///
 	friend OPENMAYA_EXPORT MFloatVector	operator*( int,
												   const MFloatVector& );
	///
 	friend OPENMAYA_EXPORT MFloatVector	operator*( short,
												   const MFloatVector& );
	///
 	friend OPENMAYA_EXPORT MFloatVector	operator*( unsigned int,
												   const MFloatVector& );
	///
 	friend OPENMAYA_EXPORT MFloatVector	operator*( unsigned short,
												   const MFloatVector& );
	///
 	friend OPENMAYA_EXPORT MFloatVector	operator*( float,
												   const MFloatVector& );
	///
 	friend OPENMAYA_EXPORT MFloatVector	operator*( double,
												   const MFloatVector& );
	///
 	MFloatVector   		operator+( const MFloatVector& other) const;
	///
	MFloatVector&		operator+=( const MFloatVector& other );
	///
 	MFloatVector   		operator-() const;
	///
 	MFloatVector   		operator-( const MFloatVector& other ) const;
	///
 	MFloatVector  		operator*( const MFloatMatrix&) const;
	///
 	MFloatVector&		operator*=( const MFloatMatrix&);
	///
 	friend OPENMAYA_EXPORT MFloatVector	operator*( const MFloatMatrix&,
												   const MFloatVector& );
	///
 	float      		    operator*( const MFloatVector& other ) const;
	///
 	bool       		   	operator!=( const MFloatVector& other ) const;
	///
 	bool       	    	operator==( const MFloatVector& other ) const;
	///
	MStatus				get( float[3] ) const;
	///
 	float      		   	length() const;
	///
 	MFloatVector  		normal() const;
	///
	MStatus				normalize();
	///
 	float      		 	angle( const MFloatVector& other ) const;
	///
	bool				isEquivalent( const MFloatVector& other,
									  float tolerance = MFloatVector_kTol )
									  const;
	///
 	bool       		   	isParallel( const MFloatVector& other,
									float tolerance = MFloatVector_kTol )
									const;
	///
	MFloatVector		transformAsNormal( const MFloatMatrix & matrix ) const;

	///
	friend OPENMAYA_EXPORT IOS_REF(ostream)& operator<<( IOS_REF(ostream)& os,
												const MFloatVector& v );

	/// The null vector
	static const MFloatVector zero;
	/// The vector <1.0,1.0,1.0>
	static const MFloatVector one;
	/// Unit vector in the positive x direction
	static const MFloatVector xAxis;
	/// Unit vector in the positive y direction
	static const MFloatVector yAxis;
	/// Unit vector in the positive z direction
	static const MFloatVector zAxis;
	/// Unit vector in the negative z direction
	static const MFloatVector xNegAxis;
	/// Unit vector in the negative z direction
	static const MFloatVector yNegAxis;
	/// Unit vector in the negative z direction
	static const MFloatVector zNegAxis;
	/// The x component of the vector
	float x;
	/// The y component of the vector
	float y;
	/// The z component of the vector
	float z;

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
#endif /* _MFloatVector */
