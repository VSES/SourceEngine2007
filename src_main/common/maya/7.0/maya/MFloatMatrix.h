#ifndef _MFloatMatrix
#define _MFloatMatrix
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
// CLASS:    MFloatMatrix
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFloatMatrix)
//
//  This class provides access to Maya's internal matrix math library allowing
//  matrices to be handled easily, and in a manner compatible with internal
//  Maya data structures.
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

#define MFloatMatrix_kTol	1.0e-5F

// *****************************************************************************

// CLASS DECLARATION (MFloatMatrix)

/// A matrix math class for 4x4 matrices of floats
/**
  This class provides access to Maya's matrix math library
*/
#if defined(_WIN32)
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFloatMatrix  
{

public:
	///
						MFloatMatrix();
	///
						MFloatMatrix( const MFloatMatrix & src );
	///
						MFloatMatrix( const double m[4][4] );
	///
						MFloatMatrix( const float m[4][4] );
	///
						~MFloatMatrix();
	///
 	MFloatMatrix&		operator = (const MFloatMatrix &);
	///
	float&				operator()(unsigned row, unsigned col );
	///
	float				operator()(unsigned row, unsigned col ) const;
	///
	float* 		    	operator[]( unsigned row );
	///
	const float* 		operator[]( unsigned row ) const;
	///
	MStatus				get( double dest[4][4] ) const;
	///
	MStatus				get( float dest[4][4] ) const;
	///
 	MFloatMatrix     	transpose() const;
	///
 	MFloatMatrix &   	setToIdentity();
	///
 	MFloatMatrix &   	setToProduct( const MFloatMatrix & left,
									  const MFloatMatrix & right );
	///
 	MFloatMatrix &   	operator+=( const MFloatMatrix & right );
	///
 	MFloatMatrix    	operator+ ( const MFloatMatrix & right ) const;
	///
 	MFloatMatrix &   	operator-=( const MFloatMatrix & right );
	///
 	MFloatMatrix  		operator- ( const MFloatMatrix & right ) const;
	///
 	MFloatMatrix &   	operator*=( const MFloatMatrix & right );
	///
 	MFloatMatrix     	operator* ( const MFloatMatrix & right ) const;
	///
 	MFloatMatrix &   	operator*=( float );
	///
 	MFloatMatrix     	operator* ( float ) const;
	///
 	friend OPENMAYA_EXPORT MFloatMatrix	operator* ( float,
												const MFloatMatrix & right );
	///
 	bool          		operator==( const MFloatMatrix & other ) const;
	///
 	bool           		operator!=( const MFloatMatrix & other ) const;
	///
 	MFloatMatrix     	inverse() const;
	///
 	MFloatMatrix     	adjoint() const;
	///
 	MFloatMatrix     	homogenize() const;
	///
 	float       		det4x4() const;
	///
 	float         		det3x3() const;
	///
 	bool           		isEquivalent( const MFloatMatrix & other,
									  float tolerance = MFloatMatrix_kTol )
 									  const;

	///
	friend OPENMAYA_EXPORT IOS_REF(ostream)& operator<< ( IOS_REF(ostream)& os,
												const MFloatMatrix& m );

	/// the matrix data
 	float matrix[4][4];

protected:
// No protected members

private:
	static const char* className();
};

#if defined(_WIN32)
#pragma warning(default: 4522)
#endif // _WIN32

inline float& MFloatMatrix::operator()(unsigned row, unsigned col )
{
	return matrix[row][col];
}

inline float MFloatMatrix::operator()(unsigned row, unsigned col ) const
{
	return matrix[row][col];
}

inline float* MFloatMatrix::operator[]( unsigned row )
{
	return matrix[row];
}

inline const float* MFloatMatrix::operator[]( unsigned row ) const
{
	return matrix[row];
}

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFloatMatrix */
