//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCRotMatd.h | Main header file for SIBCRotMatd implementation
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

#ifndef _ROTMAT_H_
#define _ROTMAT_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "SIBCMathLib.h"

//******************************************************************************
// CSIBCRotMatd | 3X3 rotation matrix.
//******************************************************************************

//! Representation of a 3X3 rotation matrix.

/*!	CSIBCRotMatd is a specialzed 3X3 matrix that represents rotations. It uses
	double precision-floating point numbers to represent elements in the matrix.

	There are several other classes which represent rotations, most notably
	CSIBCRotationd, which represents a general rotation class, not specific
	to matricies, quaternions (see CSIBCQuaternion and CSIBCQuaterniond), etc. 
	Several useful functions for rotations can be found in \link rotutil_group
	Utility functions for angles and rotations\endlink. 

	There are several other matrix classes aswell. Classes with specific applications, 
	such as CSIBCXfoMatd used for representing transformation matricies, Other matrix 
	classes are not as specialized as CSIBCRotMatd or CSIBCXfoMatd, and are used to 
	represent general matricies. These classes include CSIBCMatrix4x4, CSIBCMatrix44d and
	CSIBCMartrix33d.
*/
class XSICOREEXPORT CSIBCRotMatd : private CSIBCMatrix33d
{
	// Public members
	public:
		//****************************************
		// Constructors/Destructor
		//****************************************

		/*! Default Constructor.
			Constructs a rotation matrix representing the identity rotation
			(meaning no rotation at all).
			\sa CSIBCRotMatd::SetIdentity
		*/
		CSIBCRotMatd( void );

		/*! Constructor.
			Sets the rows of the new matrix to be \p in_vct0, \p in_vct1, and \p in_vct2. If
			\p in_bAreRows is false, then the input vectors are used as the columns of the
			new matrix. The user is responsible for ensuring that the vectors given represent
			a possible rotation matrix (vectors are unit vectors, orthogonal to each other).

			\param	in_vct0	The vector to use for the first row (or column) of the matrix.
			\param	in_vct1	The vector to use for the second row (or column) of the matrix.
			\param	in_vct2	The vector to use for the third row (or column) of the matrix.
			\param	in_bAreRows	Whether the vectors given represent rows or columns of the matrix
					(true == Rows, false == Columns). Defaults to true (rows).
		*/
		CSIBCRotMatd(	const CSIBCVector3Dd& in_vct0,
						const CSIBCVector3Dd& in_vct1,
						const CSIBCVector3Dd& in_vct2,
						const bool in_bAreRows = true );

		/*! Constructor.
			Specifies the value of each element in the new matrix. The user is responsible
			for ensuring that the elements given represent a possible rotation matrix (rows
			must represent unit vectors, and be orthogonal to each other).

			\param	in_dA00	The value for the element in row 1, column 1.
			\param	in_dA01	The value for the element in row 1, column 2
			\param	in_dA02	The value for the element in row 1, column 3
			\param	in_dA10	The value for the element in row 2, column 1.
			\param	in_dA11	The value for the element in row 2, column 2
			\param	in_dA12	The value for the element in row 2, column 3
			\param	in_dA20	The value for the element in row 3, column 1.
			\param	in_dA21	The value for the element in row 3, column 2
			\param	in_dA22	The value for the element in row 3, column 3
		*/
		CSIBCRotMatd( 
			const double in_dA00, const double in_dA01, const double in_dA02, 
			const double in_dA10, const double in_dA11, const double in_dA12, 
			const double in_dA20, const double in_dA21, const double in_dA22 );

		// Destructor.
		~CSIBCRotMatd();

		//****************************************
		// General access functions:
		//****************************************

		/*! Returns an element of the marix.
			\param	in_nRow	Row index of the element to retrieve (zero indexed).
			\param	in_nCol Column index of the element to retrieve (zero indexed).
			\return	double	The value of the element at the given matrix position.
			\sa CSIBCRotMatd::Set(int, int, const double)
		*/
		double Get( const int in_nRow, const int in_nCol ) const;

		//****************************************
		// Matrix element modification functions:
		//****************************************

		/*! Sets an element of the matrix.
			\param	in_nRow	Row index of the element to set (zero indexed).
			\param	in_nCol	Column index of the element to set (zero indexed).
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Set
			\sa CSIBCRotMatd::SetRow
			\sa CSIBCRotMatd::SetCol
			\sa CSIBCRotMatd::Get
		*/
		CSIBCRotMatd& Set( int in_nRow, int in_nCol, const double in_dVal );

		/*! Sets all elements of the matrix.
			\param	in_dVal	3x3 array of double values, containing the new values for all 
					the matrix elements.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Set
			\sa CSIBCRotMatd::SetRow
			\sa CSIBCRotMatd::SetCol
			\sa CSIBCRotMatd::Get
		*/
		CSIBCRotMatd& Set( double in_dVal[3][3] );

		/*! Sets all elements of the matrix to those contained in \p in_mat.
			\param	in_mat	Matrix to copy all element values from.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Set
			\sa CSIBCRotMatd::SetRow
			\sa CSIBCRotMatd::SetCol
			\sa CSIBCRotMatd::Get
		*/
		CSIBCRotMatd& Set( const CSIBCMatrix33d& in_mat );

		/*! Sets all elements of the matrix to those contained in \p in_mat.
			\param	in_mat	Matrix to copy all element values from.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Set
			\sa CSIBCRotMatd::SetRow
			\sa CSIBCRotMatd::SetCol
			\sa CSIBCRotMatd::Get
		*/
		CSIBCRotMatd& Set( const CSIBCRotMatd& in_mat );

		// @member Set	| Set rotation from one of the main axes
		//					(X, Y or Z) and an angle.
		/*!	Sets the rotation matrix to be a rotation about the major axis \p in_axis,
			of \p in_dAngle radians.
			\param	in_axis		The major axis for the rotation matrix.
			\param	in_dAngle	Angle of rotation about the major axis, in radians.
			\reurn	CSIBCRotMatd&	Reference to this matrix.
		*/
		CSIBCRotMatd& Set( const E3DAxisType in_axis, const double in_dAngle );

		/*!	Sets all elements in a row of the matrix.
			\param	in_nRow	Row index of the row to set (zero indexed).
			\param	in_vct	Vector containing the values to replace the specified row elements with.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Set
			\sa CSIBCRotMatd::SetRow
			\sa CSIBCRotMatd::SetCol
			\sa CSIBCRotMatd::Get
		*/
		CSIBCRotMatd& SetRow( const int in_nRow, const CSIBCVector3Dd& in_vct );

		/*!	Sets all elements in a row of the matrix.
			\param	in_nRow	Row index of the row to set (zero indexed).
			\param	in_dA0	Value for the element in the first column of the row.
			\param	in_dA1	Value for the element in the second column of the row.
			\param	in_dA2	Value for the element in the third column of the row.
			\sa CSIBCRotMatd::Set
			\sa CSIBCRotMatd::SetRow
			\sa CSIBCRotMatd::SetCol
			\sa CSIBCRotMatd::Get
		*/
		CSIBCRotMatd& SetRow( const int in_nRow, double in_dA0, double in_dA1, double in_dA2 );

		/*!	Sets all elements in a column of the matrix.
			\param	in_nCol	Column index of the column to set (zero indexed).
			\param	in_vct	Vector containing the values to replace the specified column elements with.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Set
			\sa CSIBCRotMatd::SetRow
			\sa CSIBCRotMatd::SetCol
			\sa CSIBCRotMatd::Get
		*/
		CSIBCRotMatd& SetCol( const int in_nCol, const CSIBCVector3Dd& in_vct );

		/*!	Sets all elements in a column of the matrix.
			\param	in_nCol	Column index of the column to set (zero indexed).
			\param	in_dA0	Value for the element in the first row of the column.
			\param	in_dA1	Value for the element in the second row of the column.
			\param	in_dA2	Value for the element in the third row of the column.
			\sa CSIBCRotMatd::Set
			\sa CSIBCRotMatd::SetRow
			\sa CSIBCRotMatd::SetCol
			\sa CSIBCRotMatd::Get
		*/
		CSIBCRotMatd& SetCol( const int in_nCol, double in_dA0, double in_dA1, double in_dA2 );

		/*! Sets all elements in the matrix to zero.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::SetIdentity
			\sa CSIBCRotMatd::Set
		*/
		CSIBCRotMatd& SetNull();

		/*! Sets this matrix to be the identity rotation matrix.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa	CSIBCRotMatd::SetNull
			\sa CSIBCRotMatd::Set
		*/
		CSIBCRotMatd& SetIdentity( void );


		//****************************************
		// Matrix comparison functions:
		//****************************************

		/*! Determines whether all corresponding elements of the matricies \p in_mat1 and
			\p in_mat2 are equal. Since this method uses floating-point comparison, a threshhold
			value of \p in_dEpsilon is used for equality comparison.
			\param	in_mat1	First matrix for equality comparison.
			\param	in_mat2	Second matrix for equality comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool	true if the matricies are equal, false otherwise.
			\sa CSIBCMatrix33d::operator==
			\sa CSIBCMatrix33d::operator!=
		*/
		friend bool AreAlmostEqual( const CSIBCRotMatd& in_mat1,
								const CSIBCRotMatd& in_mat2,
								const double in_dEpsilon = PICO_EPS );


		/*! Determines whether all corresponding elements of this matrix and \p in_mat are
			exactly equal. 
			\param	in_mat	The matrix to compare equality with.
			\return	bool	true if the matricies are exactly equal, false otherwise.
			\sa CSIBCRotMatd::AreAlmostEqual
			\sa CSIBCRotMatd::operator!=
		*/
		bool operator ==( const CSIBCMatrix33d & in_mat ) const;

		/*! Determines whether any corresponding elements of this matrix and \p in_mat are
			different.
			\param	in_mat	The matrix to compare inequality with.
			\return	bool	true if the matricies differ in any element, false otherwise.
			\sa CSIBCRotMatd::AreAlmostEqual
			\sa CSIBCRotMatd::operator==
		*/
		bool operator !=( const CSIBCMatrix33d & in_mat ) const;

		//****************************************
		// Matrix algebra functions:
		//****************************************

		/*! Computes the matrix multiplication of two matricies (\p in_mat1 * \p in_mat2) and stores the
			result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left).
			\param	in_mat2	The second matrix in the multiplication (on the right).
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa	CSIBCRotMatd::Mul
			\sa CSIBCRotMatd::MulTransByReg
			\sa CSIBCRotMatd::MulRegByTrans
			\sa	CSIBCRotMatd::Div
		*/
		CSIBCRotMatd& Mul( const CSIBCRotMatd& in_mat1, const CSIBCRotMatd& in_mat2 );

		/*! Computes the matrix multiplication of this matrix and \p in_mat (\p this * \p in_mat) and
			stores the result in this matrix.
			\param	in_mat	The matrix to right-multiply this matrix by.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Mul
			\sa CSIBCRotMatd::MulTransByReg
			\sa CSIBCRotMatd::MulRegByTrans
			\sa CSIBCRotMatd::Div
		*/
		CSIBCRotMatd& Mul( const CSIBCRotMatd& in_mat );

		/*! Computes the matrix multiplication of the transpose of \p in_mat1 and \p in_mat2
			(\p in_mat1^T * \p in_mat2) and stores the result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left), to be transposed 
					before multiplication.
			\param	in_mat2	The second matrix in the multiplication (on the right).
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa	CSIBCRotMatd::Mul
			\sa CSIBCRotMatd::MulRegByTrans
			\sa CSIBCRotMatd::Div
		*/
		CSIBCRotMatd& MulTransByReg( const CSIBCRotMatd& in_mat1,
								   const CSIBCRotMatd& in_mat2 );

		/*! Computes the matrix multiplication of \p in_mat1 and the transpose of \p in_mat2
			(\p in_mat1 * \p in_mat2^T) and stores the result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left).
			\param	in_mat2	The second matrix in the multiplication (on the right), to be transposed 
					before multiplication.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa	CSIBCRotMatd::Mul
			\sa CSIBCRotMatd::MulTransByReg
			\sa CSIBCRotMatd::Div
		*/
		CSIBCRotMatd& MulRegByTrans( const CSIBCRotMatd& in_mat1,
								   const CSIBCRotMatd& in_mat2 );

		/*! Sets this matrix to the negative of \p in_mat. The 'negative' of a matrix is produced
			by multiplying each element of the matrix by -1.
			\param	in_mat	Matrix containing the data to set this matrix to the negative of.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa	CSIBCRotMatd::Negate()
		*/
		CSIBCRotMatd& Negate( const CSIBCRotMatd& in_mat );

		/*! Sets this matrix to its negative. The 'negative' of a matrix is produced
			by multiplying each element of the matrix by -1.
			\return	CSIBCRotMatd&	Reference to this matrix
			\sa CSIBCRotMatd::Negate( const CSIBCRotMatd& )
		*/
		CSIBCRotMatd& Negate();

		/*! Computes the determinant of this matrix.
			\return	double	The determinant of this matrix.
			\sa CSIBCRotMatd::GetTrace
			\sa CSIBCRotMatd::IsSingular
		*/
		double GetDet( void ) const;

		/*! Computes the trace of this matrix. A trace is the sum of all the diagonal elements of a matrix.
			\return	double	The trace of this matrix.
			\sa CSIBCRotMatd::GetDet
		*/
		double GetTrace( void ) const;

		/*! Sets this matrix to be the transpose of \p in_mat.
			\param	in_mat	Matrix containing the transpose of the desired matrix.
			\return	CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Transpose()
			\sa CSIBCRotMatd::TransposeInverse
			\sa CSIBCRotMatd::IsSymmetric
		*/
		CSIBCRotMatd& Transpose( const CSIBCRotMatd& in_mat );

		/*! Sets this matrix to its transpose.
			\return CSIBCRotMatd&	Reference to this matrix.
			\sa CSIBCRotMatd::Transpose( const CSIBCRotMatd& )
			\sa CSIBCRotMatd::TransposeInverse
		*/
		CSIBCRotMatd& Transpose( void );

		/*! Sets this matrix to be the inverse of \p in_mat.
			\param	in_mat	Matrix containing the inverse of the desired matrix.
			\return	bool	true if \p in_mat could be inverted and the assignment was successful,
					false otherwise.
			\sa CSIBCRotMatd::Invert()
			\sa CSIBCRotMatd::TransposeInverse
		*/
		CSIBCRotMatd& Invert( const CSIBCRotMatd& in_mat );

		/*! Sets this matrix to its inverse.
			\return	bool	true if this matrix could be inverted and the assignment was successful,
					false otherwise.
			\sa CSIBCRotMatd::Invert( const CSIBCRotMatd& )
			\sa CSIBCRotMatd::TransposeInverse
		*/
		CSIBCRotMatd& Invert( void );
	
	// Protected members
	protected:

	// Private members
	private:

		// Disable copy constructor and operator =
		CSIBCRotMatd( const CSIBCRotMatd& in_mat );
		CSIBCRotMatd& operator =( const CSIBCRotMatd& in_mat );
};


#endif
