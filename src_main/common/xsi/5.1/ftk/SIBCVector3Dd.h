//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCVector3Dd.h | Main header file for CSIBCVector3Dd implementation.
//***************************************************************************************

/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE .

COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

//******************************************************************************
// Defines
//******************************************************************************

#ifndef _VECTOR3_H_
#define _VECTOR3_H_

//******************************************************************************
// Includes
//****************************************************************************** 
#include "SIBCMathLib.h"

//******************************************************************************
// Typedefs
//******************************************************************************

//******************************************************************************
// CSIBCVector3Dd | General 3D vector.
//******************************************************************************

//! General purpose 3D vector class.
/*! Representation of a 3D vector, defining operations to access, modify and perform calculations
	on 3D vectors. This class uses double precision floating-point numbers for represent its data,
	and in its operations. There are seperate similar classes for 2D and 4D vectors, CSIBCVector2Dd and
	CSIBCVector4Dd, respectively, which also use double precision. There also exists a 3D vector
	class with an implicit W component, CSIBCVectorwd.
	
	Furthermore, there is another set of vector classes which use single precision floating-point numbers,
	CSIBCVector2D, CSIBCVector3D and CSIBCVector4D for 2D, 3D and 4D vectors respectively. Quaternions
	represented as 4D vectors are also available in CSIBCQuaternion and CSIBCQuaterniond with
	single precision and double precision, repspectively.
*/
class XSICOREEXPORT CSIBCVector3Dd
{
	// Public members
	public:

		//****************************************
		// IMPLEMENTATION
		//****************************************

		/*!	Default Constructor.
			Constructs a new 3D vector.
		*/
		CSIBCVector3Dd( );

		/*! Constructor.
			Constructs a new 3D vector, with \p in_dX, \p in_dY and \p in_dZ as the X, Y and Z component
			values, respectively.
			\param	in_dX	The value for the X component of the new vector.
			\param	in_dY	The value for the Y component of the new vector.
			\param	in_dZ	The value for the Z component of the new vector.
		*/
		CSIBCVector3Dd( const double  in_dX, const double in_dY, const double  in_dZ );

		/*! Constructor.
			Sets the new vector to have the X, Y and Z values contained in the array \p in_dXYZ.
			\param	in_dXY	Array containing the X, Y and Z values for the new vector.
		*/
		CSIBCVector3Dd( const double in_dXYZ[3] );

		/*! Constructor.
			Sets the component determined by \p in_nIndex to \p in_dVal, and the other component to zero.
			\param	in_nIndex	Index of the component to set to \p in_dVal. (0 == X, 1 == Y).
		*/
		CSIBCVector3Dd( int in_nIndex, double in_dVal );

		~CSIBCVector3Dd();

		// Old stuff support: will be removed in next release.

		/*! Returns an array containing the X, Y and Z components of the vector. Note that
			the pointer obtained from this call should not be freed. Any modification to
			the values in the array results in a direct modification of this object.
			\return	const double*	Array containing the X, Y and Z components of this vector.
			\sa CSIBCVector3Dd::Get
		*/
		 const double * GetArray() const;
	
		// Individual access to private members

		/*! Gets the X component of this vector.
			\return	Value of the X component of this vector.
			\sa CSIBCVector3Dd::GetY
			\sa CSIBCVector3Dd::GetZ
			\sa CSIBCVector3Dd::SetX
			\sa CSIBCVector3Dd::Get
		*/
		double GetX( void ) const;

		/*! Gets the Y component of this vector.
			\return	Value of the Y component of this vector.
			\sa CSIBCVector3Dd::GetX
			\sa CSIBCVector3Dd::GetZ
			\sa CSIBCVector3Dd::SetY
			\sa CSIBCVector3Dd::Get
		*/
		double GetY( void ) const;

		/*! Gets the Z component of this vector.
			\return	Value of the Z component of this vector.
			\sa CSIBCVector3Dd::GetX
			\sa CSIBCVector3Dd::GetY
			\sa CSIBCVector3Dd::SetZ
			\sa CSIBCVector3Dd::Get
		*/
		double GetZ( void ) const;

		/*! Gets the value of the component specified by \p in_nIndex.
			\param	in_nIndex	Index of the component value to get (0 == X, 1 == Y, 2 == Z).
			\return	double		Value of the component specified by \p in_nIndex.
			\sa CSIBCVector3Dd::GetX
			\sa CSIBCVector3Dd::GetY
			\sa CSIBCVector3Dd::GetZ
			\sa CSIBCVector3Dd::Set
		*/
		double Get( const int in_nIndex ) const;


		/*! Gets the values of the X, Y and Z components of this vector.
			\param	out_dX	Receives the value of the X component
			\param	out_dY	Receives the value of the Y component
			\param	out_dZ	Receives the value of the Z component
			\sa CSIBCVector3Dd::GetX
			\sa CSIBCVector3Dd::GetY
			\sa CSIBCVector3Dd::GetZ
			\sa CSIBCVector3Dd::Set
		*/
		void Get( double& out_dX, double& out_dY, double& out_dZ ) const;

		/*!	Gets the value of largest component of the vector.
			\param	in_bInAbs	true if comparisons should be made with absolute values,
					false if comparisons should be made without absolute values. Defaults
					to true.
			\return	double		The value (or absolute value if \p in_bInAbs is true) of the
					largest component of the vector.
			\sa CSIBCVector3Dd::GetIndexOfMaxComponent
			\sa CSIBCVector3Dd::GetMinComponent
		*/
		double GetMaxComponent( bool in_bInAbs = true ) const;

		/*!	Gets the value of smallest component of the vector.
			\param	in_bInAbs	true if comparisons should be made with absolute values,
					false if comparisons should be made without absolute values. Defaults
					to true.
			\return	double		The value (or absolute value if \p in_bInAbs is true) of the
					smallest component of the vector.
			\sa CSIBCVector3Dd::GetIndexOfMinComponent
			\sa CSIBCVector3Dd::GetMaxComponent
		*/
		double GetMinComponent( bool in_bInAbs = true ) const;

		/*! Gets the index of the largest component of the vector.
			\param	in_bInAbs	true if comparisons should be made with absolute values,
					false if comparisons should be made without absolute values. Defaults
					to true.
			\return	int	Index of the largest component of the vector (0 == X, 1 == Y, 2 == Z).
			\sa CSIBCVector3Dd::GetMaxComponent
			\sa CSIBCVector3Dd::GetIndexOfMinComponent
		*/
		int GetIndexOfMaxComponent( bool in_bInAbs = true ) const;

		/*! Gets the index of the smallest component of the vector.
			\param	in_bInAbs	true if comparisons should be made with absolute values,
					false if comparisons should be made without absolute values. Defaults
					to true.
			\return	int	Index of the smallest component of the vector (0 == X, 1 == Y, 2 == Z).
			\sa CSIBCVector3Dd::GetMinComponent
			\sa CSIBCVector3Dd::GetIndexOfMaxComponent
		*/
		int GetIndexOfMinComponent( bool in_bInAbs = true ) const;

		/*! Sets the value of the X component of this vector.
			\param	in_dVal	The new value of the X component of this vector.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::SetY
			\sa CSIBCVector3Dd::SetZ
			\sa CSIBCVector3Dd::Set
			\sa CSIBCVector3Dd::GetX
		*/
		CSIBCVector3Dd& SetX ( const double in_dVal );

		/*! Sets the value of the Y component of this vector.
			\param	in_dVal	The new value of the Y component of this vector.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::SetX
			\sa CSIBCVector3Dd::SetZ
			\sa CSIBCVector3Dd::Set
			\sa CSIBCVector3Dd::GetY
		*/
		CSIBCVector3Dd& SetY ( const double in_dVal );

		/*! Sets the value of the Z component of this vector.
			\param	in_dVal	The new value of the Z component of this vector.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::SetX
			\sa CSIBCVector3Dd::SetY
			\sa CSIBCVector3Dd::Set
			\sa CSIBCVector3Dd::GetZ
		*/
		CSIBCVector3Dd& SetZ ( const double in_dVal );

		/*! Sets the value of this vector to the value of \p in_vct.
			\param	in_vct	Vector to set this vector equal to.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Set
			\sa CSIBCVector3Dd::SetX
			\sa CSIBCVector3Dd::SetY
			\sa CSIBCVector3Dd::SetZ
			\sa CSIBCVector3Dd::Get
		*/
		CSIBCVector3Dd& Set( const CSIBCVector3Dd& in_vct );

		/*! Sets the component given by \p in_nIndex to \p in_dVal.
			\param	in_nIndex	The index of the component to set (0 == X, 1 == Y, 2 == Z).
			\param	in_dVal		The value to set the component to.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Set
			\sa CSIBCVector3Dd::Get
		*/
		CSIBCVector3Dd& Set( const int in_nIndex, const double in_dVal );

		/*!	Sets the X, Y and Z components of the vector.
			\param	in_dX	The value for the X component of the vector.
			\param	in_dY	The value for the Y component of the vector.
			\param	in_dZ	The value for the Z component of the vector.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Set
			\sa CSIBCVector3Dd::GetX
			\sa CSIBCVector3Dd::GetY
			\sa CSIBCVector3Dd::GetZ
			\sa CSIBCVector3Dd::Get
		*/
		CSIBCVector3Dd& Set( const double in_dX, const double in_dY,
			const double in_dZ );


		/*!	Sets all the components of this vector to zero.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::IsNull
			\sa CSIBCVector3Dd::Set
		*/
		CSIBCVector3Dd&	SetNull( void );

		/*!	Determines whether all components of this vector are equal to zero. Since this method 
			uses floating-point comparison, a threshhold value of \p in_dEpsilon is used for equality 
			comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool	true if the vector is null, false otherwise.
			\sa CSIBCVector3Dd::SetNull
			\sa CSIBCVector3Dd::AreAlmostEqual
		*/
		bool IsNull( const double in_dEpsilon=PICO_EPS ) const;

		/*! Ensure that each of the components of this vector are at least as large as
			\p in_dEpsilon. If not, they are set to \p in_dEpsilon. Note that if 
			comparisons are done with absolute values, the sign of the vector components
			does not change, if they are less than \p in_dEpsilon.

			\param	in_bInAbs	true if comparisons are done with absolute values, 
					false if comparisons are done without absolute values. Defaults to true.
			\param	in_dEpsilon	Minimal value for components of this vector. Defaults to #PICO_EPS.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa	CSIBCVector3Dd::SetMinimal
			\sa CSIBCVector3Dd::AreAlmostEqual
		*/
		CSIBCVector3Dd&	SetMinimal ( bool in_bInAbs = true, const double in_dEpsilon=PICO_EPS );

		/*! Ensures that each of the components of this vector are at least as large as
			the components of \p in_vct. If they are not, they are set to be equal to the
			component of \p in_vct. Note that if comparisons are done with absolute values,
			the sign of the vector components does not change, if they are less than the 
			corresponding component values in \p in_vct.

			\param	in_vct		Vector containing minimal component values.
			\param	in_bInAbs	true if comparisons are done with absolute values, 
					false if comparisons are done without absolute values. Defaults to true.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::SetMinimal
			\sa CSIBCVector3Dd::AreAlmostEqual
		*/					
		CSIBCVector3Dd&	SetMinimal ( const CSIBCVector3Dd &, bool in_bInAbs = true );

		/*! Determines whether all corresponding components of the \p in_vct1 and
			\p in_vct2 are equal. Since this method uses floating-point comparison, a threshhold
			value of \p in_dEpsilon is used for equality comparison.
			\param	in_vct1	First vector for equality comparison.
			\param	in_vct2	Second vector for equality comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool	true if the vectors are equal, false otherwise.
			\sa CSIBCVector3Dd::operator==
			\sa CSIBCVector3Dd::operator!=
		*/
		friend bool	AreAlmostEqual( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2, 
			const double in_dEpsilon = PICO_EPS ) ;

		/*! Determines whether all corresponding components of this vector and \p in_vct are
			exactly equal. 
			\param	in_vct	The vector to compare equality with.
			\return	bool	true if the vectors are exactly equal, false otherwise.
			\sa CSIBCVector3Dd::AreAlmostEqual
			\sa CSIBCVector3Dd::operator!=
		*/
		bool operator == ( const CSIBCVector3Dd& in_vct ) const;

		/*! Determines whether any corresponding components of this vector and \p in_vct are
			different.
			\param	in_vct	The vector to compare inequality with.
			\return	bool	true if the vectors differ in any component, false otherwise.
			\sa CSIBCVector3Dd::AreAlmostEqual
			\sa CSIBCVector3Dd::operator==
		*/
		bool operator != ( const CSIBCVector3Dd& in_vct ) const;

		/*!	Sets this vector to the negative of \p in_vct.
			\param	in_vct	Vector containing the negative value for this vector.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa	CSIBCVector3Dd::Negate
		*/
		CSIBCVector3Dd&	Negate( const CSIBCVector3Dd& in_vct );

		/*! Sets this vector to its negative.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Negate
		*/
		CSIBCVector3Dd&	Negate( void );

		/*! Gets the squared length of this vector.
			\return	double	The squared length of this vector.
			\sa CSIBCVector3Dd::GetLength
			\sa CSIBCVector3Dd::SetLength
			\sa CSIBCVector3Dd::Normalize
		*/
		double GetSquaredLength( void ) const;

		/*! Gets the length of this vector.
			\return	double	The squared length of this vector.
			\sa CSIBCVector3Dd::GetSquaredLength
			\sa CSIBCVector3Dd::SetLength
			\sa CSIBCVector3Dd::Normalize
		*/
		double GetLength( void ) const;

		/*!	Sets the length of this vector.
			\param	in_dLength	New length for this vector.
			\return	bool		true if the length was set properly, false if the
					length of the current vector is zero.
			\sa CSIBCVector3Dd::Normalize
			\sa CSIBCVector3Dd::GetLength
		*/
		bool SetLength( const double in_dLength );

		/*!	Sets the vector to be a unit vector.
			\return	bool		true if the length was set properly, false if the
					length of the current vector is zero.
			\sa CSIBCVector3Dd::SetLength
			\sa CSIBCVector3Dd::GetLength
		*/
		bool Normalize( void );


		// Efficient arithmetic operations.

		/*! Adds this vector and \p in_vct. The result is stored in this vector.
			\param	in_vct	Vector to add to this vector.
			return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Add
			\sa CSIBCVector3Dd::Sub
		*/
		CSIBCVector3Dd& Add( const CSIBCVector3Dd&  in_vct	);

		/*! Adds \p in_vct1 and \p in_vct2. The result is stored in this vector.
			\param	in_vct1	First vector in the addition.
			\param	in_vct2	Second vector in the addition.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Add
			\sa CSIBCVector3Dd::Sub
		*/
		CSIBCVector3Dd& Add( const CSIBCVector3Dd&  in_vct1, const CSIBCVector3Dd& in_vct2 );

		/*! Subtracts \p in_vct from this vector. The result is stored in this vector.
			\param	in_vct	Vector to subtract from this vector.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Sub
			\sa CSIBCVector3Dd::Add
		*/
		CSIBCVector3Dd& Sub( const CSIBCVector3Dd& in_vct );

		/*! Subtracts \p in_vct2 from \p in_vct1. The result is stored in this vector.
			\param	in_vct1	First vector for the subtraction.
			\param	in_vct2	Second vector for the subtraction.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Sub
			\sa CSIBCVector3Dd::Add
		*/
		CSIBCVector3Dd& Sub( const CSIBCVector3Dd&  in_vct1, const CSIBCVector3Dd& in_vct2 );

		/*! Multiplies each component of this vector by \p in_dFactor. The result is stored
			in this vector.
			\param	in_dFactor	Factor to multiply the components of this vector by.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::Div
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
		*/
		CSIBCVector3Dd& Mul( const double in_dFactor );

		/*! Multiplies each of the components in \p in_vct by \p in_dFactor. The result
			is stored in this vector.
			\param	in_dFactor	Factor to multiply the components of \p in_vct by.
			\param	in_vct		Vector to multiply.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::Div
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
		*/
		CSIBCVector3Dd& Mul( const double in_dFactor, const CSIBCVector3Dd& in_vct );

		/*!	Multiplies each of the components in \p in_vct by \p in_dFactor. The result
			is stored in this vector.
			\param	in_vct		Vector to multiply.
			\param	in_dFactor	Factor to multiply the components of \p in_vct by.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::Div
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
		*/
		CSIBCVector3Dd& Mul( const CSIBCVector3Dd& in_vct, const double in_dFactor );

		/*! Multiplies a row vector \p in_vct, into a 3x3 matrix \p in_mat 
			(\p in_vct * \p in_mat), and stores the result in this vector. 
			\param	in_vct	Row vector to multiply (on the left).
			\param	in_mat	Matrix to multiply (on the right).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& Mul( const CSIBCVector3Dd& in_vct, const CSIBCMatrix33d& in_mat );

		/*! Multiplies a this vector (as a row vector), into a 3x3 matrix 
			\p in_mat (this * \p in_mat), and stores the result in this vector. 
			\param	in_vct	Row vector to multiply (on the left).
			\param	in_mat	Matrix to multiply (on the right).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& Mul( const CSIBCMatrix33d& in_mat );

		/*! Multiplies a 3x3 matrix \p in_mat, into a column vector, \p in_vct
			(\p in_mat * \p in_vct), and stores the result in this vector. 
			\param	in_mat	Matrix to multiply (on the left).
			\param	in_vct	Column vector to multiply (on the right).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& Mul( const CSIBCMatrix33d& in_mat, const CSIBCVector3Dd& in_vct );

		// TODO  Remove set of functions to multiply by CSIBCRotMatd as soon as 
		//       CSIBCRotMatd and CSIBCMatrix33d will both derived public from a
		//       common base matrix class.

		/*! Multiplies a row vector \p in_vct, into a 3x3 rotation matrix \p in_mat 
			(\p in_vct * \p in_mat), and stores the result in this vector. 
			\param	in_vct	Row vector to multiply (on the left).
			\param	in_mat	Rotation matrix to multiply (on the right).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& Mul( const CSIBCVector3Dd& in_vct, const CSIBCRotMatd& in_mat );

		/*! Multiplies a 3x3 rotation matrix \p in_mat, into a column vector, \p in_vct
			(\p in_mat * \p in_vct), and stores the result in this vector. 
			\param	in_mat	Rotation matrix to multiply (on the left).
			\param	in_vct	Column vector to multiply (on the right).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& Mul( const CSIBCRotMatd& in_mat, const CSIBCVector3Dd& in_vct );

		/*! Multiplies a row vector \p in_vct, into a 4x4 transformation matrix \p in_mat 
			(\p in_vct * \p in_mat), and stores the result in this vector. Since a 4-vector
			is required for the multiplication, the W component of \p in_vct is assumed to
			be 1. Since the output from this operation is normally a 4-vector, only the
			X, Y and Z components are used.
			\param	in_vct	Row vector to multiply (on the left).
			\param	in_mat	Rotation matrix to multiply (on the right).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& Mul( const CSIBCVector3Dd& in_vct, const CSIBCXfoMatd& in_mat );

		/*! Multiplies a row vector \p in_vct, into a 4x4 matrix \p in_mat 
			(\p in_vct * \p in_mat), and stores the result in this vector. Since a 4-vector
			is required for the multiplication, the W component of \p in_vct is assumed to
			be 1. The X, Y and Z output components are divided by the resultant W component.
			\param	in_vct	Row vector to multiply (on the left).
			\param	in_mat	Rotation matrix to multiply (on the right).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& Mul( const CSIBCVector3Dd& in_vct, const CSIBCMatrix44d& in_mat );

		/*!	Divides each component of this vector by \p in_dFactor. The result is stored
			in this vector.
			\param	in_dFactor	Factor to divide the components of this vector by.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Div
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::MulComp
		*/
		CSIBCVector3Dd& Div( const double in_dFactor );

		/*!	Divides each component of \p in_vct by \p in_dFactor. The result is stored
			in this vector.
			\param	in_vct		Vector to divide.
			\param	in_dFactor	Factor to divide the components of this vector by.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::Div
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::MulComp
		*/
		CSIBCVector3Dd& Div( const CSIBCVector3Dd& in_vct, const double in_dFactor );

		/*!	Multiplies each component of \p in_vct1 by the corresponding component in \p in_vct2.
			The result is stored in this vector.
			\param	in_vct1	First vector to multiply the components of.
			\param	in_vct2	Second vector to multiply the components of.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& MulComp( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2  );

		/*!	Multiplies each component of \p in_vct by the corresponding component in this vector.
			The result is stored in this vector.
			\param	in_vct	Vector to multiply the components of.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::Mul
			\sa CSIBCVector3Dd::Div
		*/
		CSIBCVector3Dd& MulComp( const CSIBCVector3Dd& in_vct );

		/*!	Divides each component of this vector by the corresponding component in \p in_vct.
			The result is stored in this vector.
			\param	in_vct	Vector to divide the components of this vector by.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::Div
			\sa CSIBCVector3Dd::Mul
		*/
		CSIBCVector3Dd& DivComp( const CSIBCVector3Dd& in_vct ); 

		/*!	Divides each component of \p in_vct1 by the corresponding component in \p in_vct2.
			The result is stored in this vector.
			\param	in_vct1	Vector to be divided by the components of \p in_vct2
			\param	in_vct2	Vector to divide the components of \p in_vct1 by.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::DivComp
			\sa CSIBCVector3Dd::MulComp
			\sa CSIBCVector3Dd::Div
			\sa CSIBCVector3Dd::Mul
		*/
		CSIBCVector3Dd& DivComp( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2 );
		

		/*!	Computes the dot product between the vectors \p in_vct1 and \p in_vct2.
			\param	in_vct1	First vector in the dot product.
			\param	in_vct2	Second vector in the dot product.
			\return	double	The dot product between \p in_vct1 and \p in_vct2.
			\sa CSIBCVector3Dd::SetCross
		*/
		friend double GetDot( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2 );

		/*! Computes the cross product between this vector and \p in_vct (this x \p in_vct).
			The result is stored in this vector
			\param	in_vct	Vector to cross with this vector (on the left).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::GetDot
			\sa CSIBCVector3Dd::SetCross
		*/
		CSIBCVector3Dd&	SetCross( const CSIBCVector3Dd& in_vct );

		/*! Computes the cross product between \p in_vct1 and \p in_vct2 (\p in_vct1 x \p in_vct2).
			The result is stored in this vector.
			\param	in_vct1	Vector to cross with this vector (on the left).
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::GetDot
			\sa CSIBCVector3Dd::SetCross
		*/
		CSIBCVector3Dd&	SetCross( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2 );

		/*! Computes the vector that is orthogonal to \p in_vct1, and in the same plane as
			\p in_vct1 and \p in_vct2. The result is stored in this vector.
			\param	in_vct1	Vector that the new vector should be orthogonal to.
			\param	in_vct2	Another vector, with \p in_vct1 that forms a plane in which
					the new vector should lie in.
			\return	bool	true if the orthogonal vector could be calculated, false if
					\p in_vct1 and \p in_vct2 are colinear. If false, this vector contains
					the null vector.
			\sa CSIBCVector3Dd::AreOrthogonal
			\sa CSIBCVector3Dd::SetOrthoProj
			\sa CSIBCVector3Dd::GetOrthogonalBasis
		*/
		bool SetOrthogonal( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2 );

		/*! Computes the vector that is orthogonal to \p in_vct1, and in the same plane as
			\p in_vct1 and \p in_vct2. The result is then projected on \p in_vct2. The
			result is stored in this vector.
			\param	in_vct1	Vector taht the new vector should be orthogonal to.
			\param	iN_vct2	Another vector, with \p in_vct1 that forms a plane in which
					the new vector should lie in. The result is then projected onto this vector.
			\return	bool	true if the orthogonal projected vector could be calculated, false
					if \p in_vct1 and \p in_vct2 are colinear. If false, this vector contains
					the null vector.
			\sa CSIBCVector3Dd::SetOrthogonal
			\sa CSIBCVector3Dd::AreOrthogonal
			\sa CSIBCVector3Dd::SetProjection
		*/
		bool SetOrthoProj( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2 );

		/*! Determines whether two vectors \p in_vct1 and \p in_vct2 are orthogonal to each other.
			Since this method uses floating-point comparison, a threshhold value of \p in_dEpsilon 
			is used for equality comparison.
			\param	in_vct1	First vector for orthogonality comparison.
			\param	in_vct2	Second vector for orthogonality comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool	true if the vectors are orthogonal, false otherwise.
			\sa CSIBCVector3Dd::SetOrthogonal
			\sa CSIBCVector3Dd::SetOrthoProj
			\sa CSIBCVector3Dd::GetOrthogonalBasis
		*/
		friend bool AreOrthogonal( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2, 
			const double in_dEpsilon = PICO_EPS );

		/*! Projects this vector onto \p in_vct. The projected vector is stored in this vector.
			\param	in_vct	Vector to project this vector onto.
			\return	bool	true if the vector is projected, false if the input vector is the
					null vector. If false, this vector keeps its original value.
			\sa CSIBCVector3Dd::SetOrthoProj
		*/
		bool SetProjection( const CSIBCVector3Dd& in_vct );

		/*! Determines whether \p in_vct1 and \p in_vct2 are parallel vectors. Since this method 
			uses floating-point comparison, a threshhold value of \p in_dEpsilon is used for 
			equality comparison.
			\param	in_vct1	The first vector in the parallelism comparison.
			\param	in_vct2	The second vector in the parallelism comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
		*/
		friend bool AreParallel( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2, 
			const double in_dEpsilon = PICO_EPS );

		/*!	Computes \p in_vct1 * 2 - \p in_vct2, and stores the result in this vector.
			\param	in_vct1		First vector in the computation.
			\param	in_vct2		Second vector in the computation.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::SetProjection
		*/
		CSIBCVector3Dd& SetMirror( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2 );

		/*!	Computes the distance between two points in 3D space given by 
			\p in_pnt1 and \p in_pnt2.
			\param	in_pnt1	Starting point for distance calculation.
			\param	in_pnt2	Ending point for distance calculation.
			\return	double	The scalar distance between the two points, in 3D space.
			\sa CSIBCVector3Dd::GetSquaredDistance
		*/
		friend double GetDistance( const CSIBCVector3Dd& in_pnt1, const CSIBCVector3Dd& in_pnt2 );

		/*! Computes the squared distance between two points in 3D space given by 
			\p in_pnt1 and \p in_pnt2.
			\param	in_pnt1	Starting point for distance calculation.
			\param	in_pnt2	Ending point for distance calculation.
			\return	double	The squared scalar distance between the two points, in 3D space.
			\sa CSIBCVector3Dd::GetDistance
		*/
		friend double GetSquaredDistance( const CSIBCVector3Dd& in_pnt1, const CSIBCVector3Dd& in_pnt2 );

		/*! Computes two vectors \p out_vct2 and \p out_vct3, which together with the input
			vector \p in_vct1 form an orthogonal basis. The out vectors are not normalized.
			\param	in_vct1		Vector to compute the orthogonal basis from.
			\param	out_vct2	Receives an orthogonal basis vector.
			\param	out_vct3	Receives an orthogonal basis vector.
			\sa CSIBCVector3Dd::GetOrthonormalBasis
			\sa CSIBCVector3Dd::SetOrthogonal
			\sa CSIBCVector3Dd::SetOrthoProj
		*/
		friend void GetOrthogonalBasis( const CSIBCVector3Dd& in_vct1, CSIBCVector3Dd& out_vct2, 
			CSIBCVector3Dd& out_vct3 );

		/*! Computes two vectors \p out_vct2 and \p out_vct3, which together with the input
			vector \p in_vct1 form an orthonormal basis. \p io_vct1 is normalized if it is
			not already normalized.
			\param	io_vct1		Vector to compute the orthonormal vector from. Normalized if not
					already normalized.
			\param	out_vct2	Receives an orthonormal basis vector.
			\param	out_vct3	Receives an orthonormal basis vector.
			\sa CSIBCVector3Dd::GetOrthogonalBasis
			\sa CSIBCVector3Dd::SetOrthogonal
			\sa CSIBCVector3Dd::SetOrthoProj
		*/
		friend void GetOrthonormalBasis( CSIBCVector3Dd& io_vct1, CSIBCVector3Dd& out_vct2, 
			CSIBCVector3Dd& out_vct3 );

		/*! Sets this vector to the midpoint between the two points \p in_pnt1 and \p in_pnt2.
			\param	in_pnt1	First point for midpoint determination.
			\param	in_pnt2	Second point for midpoint determination.
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::SetMean
			\sa CSIBCVector3Dd::LinearPositionInterpolate
		*/
		CSIBCVector3Dd& SetMidpoint( const CSIBCVector3Dd& in_pnt1, const CSIBCVector3Dd& in_pnt2 );


		/*! Computes the mean of the vector components of the vectors in the array \p in_vct.
			The result is stored in this vector.
			\param	in_vct		Array of vectors (\p in_nbVct long) to calculate the mean of.
			\param	in_nbVct	Number of vectors in the \p in_vct array.
			\return CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::SetMidpoint
			\sa CSIBCVector3Dd::LinearPositionInterpolate
		*/
		CSIBCVector3Dd& SetMean (	const CSIBCVector3Dd *in_vct, unsigned long in_nbVct );

		/*! Computes the linear interpolation between the two vectors \p in_vct1 and
			\p in_vct2, at a point \p in_dT between the two vectors.
			\param	in_vct1	Starting point for the interpolation.
			\param	in_vct2	Ending point for the interpolation.
			\param	in_dT	Interpolation point between the vectors, in the range [0.0f, 1.0f].
			\return	CSIBCVector3Dd&	Reference to this vector.
			\sa CSIBCVector3Dd::SetMidpoint
		*/
		CSIBCVector3Dd& LinearPositionInterpolate( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2, const double in_dT );

		/*! Computes the angle between the vectors defined by \p in_pntOrigin to \p in_pnt1,
			and \p in_pntOrigin to \p in_pnt2. The angle is in the range [0.0f, #M_PI].
			\param	in_pntOrigin	Origin point for the two vectors.
			\param	in_pnt1			End point of the first vector.
			\param	in_pnt2			End point of the second vector.
			\return	double			Angle (in radians) between the two vectors. In the 
					range [0.0f, #M_PI].
			\sa CSIBCVector3Dd::GetPntsAngleSigned
			\sa CSIBCVector3Dd::GetPntsCosAngle
			\sa CSIBCVector3Dd::GetVctsAngle
			\sa CSIBCVector3Dd::Get2DVctsAngleSigned
			\sa CSIBCVector3Dd::GetVctsCosAngle
		*/
		friend double GetPntsAngle(	const CSIBCVector3Dd& in_pntOrigin,
									const CSIBCVector3Dd& in_pnt1,
									const CSIBCVector3Dd& in_pnt2 );

		/*! Computes the angle between the vectors defined by \p in_pntOrigin to \p in_pnt1,
			and \p in_pntOrigin to \p in_pnt2. The angle is in the range [-#M_PI, #M_PI].
			This angle is positive if it corresponds to a counter-clockwise rotation, and 
			it is negative if it corresponds to a clockwise rotation.
			\param	in_pntOrigin	Origin point for the two vectors.
			\param	in_pnt1			End point of the first vector.
			\param	in_pnt2			End point of the second vector.
			\return	double			Angle (in radians) between the two vectors. In the 
					range [-#M_PI, #M_PI].
			\sa CSIBCVector3Dd::GetPntsAngle
			\sa CSIBCVector3Dd::GetPntsCosAngle
			\sa CSIBCVector3Dd::GetVctsAngle
			\sa CSIBCVector3Dd::Get2DVctsAngleSigned
			\sa CSIBCVector3Dd::GetVctsCosAngle
		*/
		friend double Get2DPntsAngleSigned(	const CSIBCVector3Dd& in_pntOrigin,
											const CSIBCVector3Dd& in_pnt1,
											const CSIBCVector3Dd& in_pnt2 );

		/*! Computes the cosine of the angle between the vectors defined by \p in_pntOrigin 
			to \p in_pnt1, and \p in_pntOrigin to \p in_pnt2.
			\param	in_pntOrigin	Origin point for the two vectors.
			\param	in_pnt1			End point of the first vector.
			\param	in_pnt2			End point of the second vector.
			\return	double			Cosine of the angle between the two vectors. In the
					range [-1.0f, 1.0f].
			\sa CSIBCVector3Dd::GetPntsAngle
			\sa CSIBCVector3Dd::Get2DPntsAngleSigned
			\sa CSIBCVector3Dd::GetVctsAngle
			\sa CSIBCVector3Dd::Get2DVctsAngleSigned
			\sa CSIBCVector3Dd::GetVctsCosAngle
		*/
		friend double GetPntsCosAngle(	const CSIBCVector3Dd& in_pntOrigin,
										const CSIBCVector3Dd& in_pnt1,
										const CSIBCVector3Dd& in_pnt2 );

		/*!	Computes the angle between the vectors \p in_vct1 and \p in_vct2.
			The angle is in the range of [0.0f, #M_PI].
			\param	in_vct1		First vector.
			\param	in_vct2		Second vector.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	double		Angle (in radians) between the two vectors. In the 
					range [0.0f, #M_PI].
			\sa CSIBCVector3Dd::GetPntsAngle
			\sa CSIBCVector3Dd::Get2DPntsAngleSigned
			\sa CSIBCVector3Dd::GetPntsCosAngle
			\sa CSIBCVector3Dd::Get2DVctsAngleSigned
			\sa CSIBCVector3Dd::GetVctsCosAngle
		*/
		friend double GetVctsAngle(	const CSIBCVector3Dd&	in_vct1,
									const CSIBCVector3Dd&	in_vct2,
									const double in_dEpsilon = PICO_EPS );

		/*! Computes the angle between the vectors \p in_vct1 and \p in_vct2. The angle 
			is in the range [-#M_PI, #M_PI]. This angle is positive if it corresponds 
			to a counter-clockwise rotation, and it is negative if it corresponds to a 
			clockwise rotation.
			\param	in_vct1		First vector.
			\param	in_vct2		Second vector.
			\return	double		Angle (in radians) between the two vectors. In the 
					range [-#M_PI, #M_PI].
			\sa CSIBCVector3Dd::GetPntsAngle
			\sa CSIBCVector3Dd::Get2DPntsAngleSigned
			\sa CSIBCVector3Dd::GetPntsCosAngle
			\sa CSIBCVector3Dd::GetVctsAngle
			\sa CSIBCVector3Dd::GetVctsCosAngle
		*/
		friend double Get2DVctsAngleSigned(	const CSIBCVector3Dd& in_vct1,
											const CSIBCVector3Dd& in_vct2 );

		
		/*! Computes the cosine of the angle between the vectors \p in_vct1 and \p in_vct2.
			\param	in_vct1		First vector.
			\param	in_vct2		Second vector.
			\return	double			Cosine of the angle between the two vectors. In the
					range [-1.0f, 1.0f].
			\sa CSIBCVector3Dd::GetPntsCosAngle
			\sa CSIBCVector3Dd::GetPntsAngle
			\sa CSIBCVector3Dd::Get2DPntsAngleSigned
			\sa CSIBCVector3Dd::GetVctsAngle
			\sa CSIBCVector3Dd::Get2DVctsAngleSigned
		*/
		friend double GetVctsCosAngle(	const CSIBCVector3Dd& in_vct1,
										const CSIBCVector3Dd& in_vct2,
										const double in_dEpsilon = PICO_EPS );


		/*!	Determines which 'side' of the vector formed between the points \p in_pntOrigin and
			\p in_pntTip, a point \p in_pnt is on. 
			\param	in_pnt			Point to determine which side of the vector it is on.
			\param	in_pntOrigin	Staring point of the vector.
			\param	in_pntTip		Ending point of the vector.
			\return	int				The side that the point is on. (-1 == right side, 1 == left side).
		*/
		friend int Get2DPntSide(	const CSIBCVector3Dd& in_pnt,
									const CSIBCVector3Dd& in_pntOrigin,
									const CSIBCVector3Dd& in_pntTip );


		/*! Calculates the area of the triangle with the verticies \p in_rPoint0,
			\p in_rPoint1 and \p in_rPoint2.
			\param	in_rPoint0	First vertex of the triangle.
			\param	in_rPoint1	Second vertex of the triangle.
			\param	in_rPoint2	Third vertex of the triangle.
		*/
		friend double GetTriangleArea( 
			const CSIBCVector3Dd& in_rPoint0,
			const CSIBCVector3Dd& in_rPoint1,
			const CSIBCVector3Dd& in_rPoint2 );

		/////////////////////////////
		// To be implemented later //
		/////////////////////////////

		// Cardinal & Bezier Position Interpolate doens't exist in SI3D3.7
		// What about Linear, Cardinal & Bezier VectorInterpolate

		// SetBounds	| Readjust boundaries (min and max) to contain a given point.
		/*! Not implemented yet */
		void SetBounds( const CSIBCVector3Dd& in_pnt, CSIBCVector3Dd& io_min, CSIBCVector3Dd& io_max );

		// Combine		| Compute this=s1*v1 + s2*v2.
		/*! Not implemented yet */
		CSIBCVector3Dd& Combine( const CSIBCVector3Dd& in_vct1, const double in_dS1, 
			const CSIBCVector3Dd& in_vct2, const double in_dS2 );

#ifdef _DEBUG
		 void SelfDebug() const;
		 void DoDebugInit();
#endif

		///////////////////////////////
		// To finish implementing    //
		///////////////////////////////
		
		// BsplinePositionInterpolate | Interpolates a BSpline position between four 3D vectors.
		/*! Not implemented yet. */
		CSIBCVector3Dd& BsplinePositionInterpolate( const CSIBCVector3Dd& in_vct1,
									const CSIBCVector3Dd& in_vct2, const CSIBCVector3Dd& in_vct3,
									const CSIBCVector3Dd& in_vct4, const double in_dT );

		// BsplineVectorInterpolate | Interpolates a BSpline vector between four 3D vectors.
		/*! Not implemented yet. */
		CSIBCVector3Dd& BsplineVectorInterpolate( const CSIBCVector3Dd& in_vct1,
									const CSIBCVector3Dd& in_vct2, const CSIBCVector3Dd& in_vct3,
									const CSIBCVector3Dd& in_vct4, const double in_dT );

		// ArePntsCollinear	| Verify if three points are collinear.
		/*! Not implemented yet. */
		friend bool ArePntsCollinear( const CSIBCVector3Dd& in_pnt1,
										const CSIBCVector3Dd& in_pnt2,
										const CSIBCVector3Dd& in_pnt3,
										const double in_dEpsilon = PICO_EPS );

		// AreVctsCoplanar	| Verify if two vectors are coplanar.
		/*! Not implemented yet. */
		friend bool AreVctsCoplanar ( const CSIBCVector3Dd& in_vct1,
										const CSIBCVector3Dd& in_vct2,
										const CSIBCVector3Dd& in_vct3,
										const double in_dEpsilon = PICO_EPS );

		// ArePntsCoplanar	| Verify if three points are coplanar.
		/*! Not implemented yet. */
		friend bool ArePntsCoplanar ( const CSIBCVector3Dd& in_pnt1,
										const CSIBCVector3Dd& in_pnt2,
										const CSIBCVector3Dd& in_pnt3,
										const CSIBCVector3Dd& in_pnt4,
										const double in_dEpsilon = PICO_EPS );

	// Private Members
	private:

		//****************************************
		// ATTRIBUTES
		//****************************************
		// double | CSIBCVector3Dd | m_dX | X component of the vector.
		// double | CSIBCVector3Dd | m_dY | Y component of the vector.
		// double | CSIBCVector3Dd | m_dZ | Z component of the vector.
		double  m_dX, m_dY, m_dZ;

	public:

	////////////////////////////////////////////////////////////////////////
	// Useful constants:
	////////////////////////////////////////////////////////////////////////
	static const CSIBCVector3Dd k_UnitVector;
};

typedef CSIBCVector3Dd* LP3DVECTOR3;

//******************************************************************************
//
// Author       : Michel Carigan
// Date         : 08/05/96
// Updated by   : Eric Cabot (02/25/97)
//
// Set each component to be at least epsilon (in absolute or not depending on in_bInAbs).
//
// A reference on this.
//
// This function works like the macro _SI_VCT_EPS in v3.51
// That is, each component are set to eps only if they are less than epsilon.
// In that case the sign is unchanged.
//
//
//******************************************************************************
inline  CSIBCVector3Dd&  
CSIBCVector3Dd::SetMinimal
    (
        bool in_bInAbs,             // Tells if the test is done in absolute value or not (true by default).
        const double in_dEpsilon    // The epsilon to compare with (PICO_EPS by default).
    )
{

    if( in_bInAbs )
    {

        if( fabs(m_dX) < in_dEpsilon )
            m_dX = ( ( m_dX >= 0 ) ? in_dEpsilon : -in_dEpsilon ) ;
        if( fabs(m_dY) < in_dEpsilon )
            m_dY = ( ( m_dY >= 0 ) ? in_dEpsilon : -in_dEpsilon ) ;
        if( fabs(m_dZ) < in_dEpsilon )
            m_dZ = ( ( m_dZ >= 0 ) ? in_dEpsilon : -in_dEpsilon ) ;
    }
    else
    {
        if( m_dX < in_dEpsilon )
        {
            m_dX = in_dEpsilon;
        }
        if( m_dY < in_dEpsilon )
        {
            m_dY = in_dEpsilon;
        }
        if( m_dZ < in_dEpsilon )
        {
            m_dZ = in_dEpsilon;
        }
    }

    return *this;
}

#endif
