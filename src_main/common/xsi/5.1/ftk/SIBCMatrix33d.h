//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCMatrix33d.h | Main header file for SIBCMatrix33d implementation
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
//
// Defines
//
//******************************************************************************
#ifndef _MATRIX33_H_
#define _MATRIX33_H_

//******************************************************************************
//
// Includes
//
//******************************************************************************
#include "SIBCMathLib.h"

//******************************************************************************
//
// CSIBCMatrix33d | General 3X3 matrix.
//
//******************************************************************************

//! Class representing matricies with 3 rows, and 3 columns.
/*! CSIBCMatrix33d objects represent matricies, and define a set of operations which are
	useful in the context of a matrix. Unlike CSIBCMatrix4x4, there is no method of accessing
	the matrix data directly, thus it does not matter whether the values are stored in column
	major, or row major order.

	This class uses double precision floating point numbers to store element values. There exists
	three other matrix classes, CSIBCMatrix44d, CSIBCMatrix4x4 and CSIBCMatrixMNd. They are used 
	to represent 4x4 matricies with double precision floating-point numbers, 4x4 matricies with single
	precision floating-point numbers, and MxN matricies with double precision floating-point numbers,
	respectively.	
*/

class XSICOREEXPORT CSIBCMatrix33d
{
	// Public members
	public:

		//****************************************
		// Constructors/destructor:
		//****************************************

		/*! Default Constructor.
			Sets all elements in the matrix to zero.
			\sa CSIBCMatrix33d::SetNull
		*/
		CSIBCMatrix33d( void );

		/*! Constructor.
			Sets the rows of the new matrix to be \p in_vct0, \p in_vct1, and \p in_vct2. If
			\p in_bAreRows is false, then the input vectors are used as the columns of the
			new matrix.
			\param	in_vct0	The vector to use for the first row (or column) of the matrix.
			\param	in_vct1	The vector to use for the second row (or column) of the matrix.
			\param	in_vct2	The vector to use for the third row (or column) of the matrix.
			\param	in_bAreRows	Whether the vectors given represent rows or columns of the matrix
					(true == Rows, false == Columns). Defaults to true (rows).
		*/
		CSIBCMatrix33d( 
			const CSIBCVector3Dd& in_vct0,
			const CSIBCVector3Dd& in_vct1,
			const CSIBCVector3Dd& in_vct2,
			const bool in_bAreRows = true );

		/*! Constructor.
			Specifies the value of each element in the new matrix.
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
		CSIBCMatrix33d( 
			const double in_dA00, const double in_dA01, const double in_dA02, 
			const double in_dA10, const double in_dA11, const double in_dA12, 
			const double in_dA20, const double in_dA21, const double in_dA22 );

		/*! Constructor.
			Creates a 3x3 matrix from a 4x4 matrix, \p in_mat, by removing the 
			column and row from \p in_mat specified by \p in_nRow, and \p in_nCol.
			\param	in_nRow	The row to remove from \p in_mat (zero indexed).
			\param	in_nCol	The column to remove from \p in_mat (zero indexed).
			\param	in_mat	The 4x4 matrix to remove a row and a column from to produce
					the new matrix.

			\sa CSIBCMatrix33d::Set(int, int, const CSIBCMatrix44d&)
		*/
		CSIBCMatrix33d( int in_nRow, int in_nCol, const CSIBCMatrix44d& in_mat );


		// Destructor.
		~CSIBCMatrix33d();

		//****************************************
		// General access functions:
		//****************************************

		/*! Returns an element of the marix.
			\param	in_nRow	Row index of the element to retrieve (zero indexed).
			\param	in_nCol Column index of the element to retrieve (zero indexed).
			\return	double	The value of the element at the given matrix position.
			\sa CSIBCMatrix33d::Set(int, int, const double)
		*/
		double Get( const int in_nRow, const int in_nCol ) const;

		//****************************************
		// Matrix element modification functions:
		//****************************************
		
		/*! Sets an element of the matrix.
			\param	in_nRow	Row index of the element to set (zero indexed).
			\param	in_nCol	Column index of the element to set (zero indexed).
			\param	in_dVal	Value for the element.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& Set( int in_nRow, int in_nCol, const double in_dVal );

		/*! Sets all elements of the matrix.
			\param	in_dA00	The value for the element in row 1, column 1.
			\param	in_dA01	The value for the element in row 1, column 2.
			\param	in_dA02	The value for the element in row 1, column 3.
			\param	in_dA10	The value for the element in row 2, column 1.
			\param	in_dA11	The value for the element in row 2, column 2.
			\param	in_dA12	The value for the element in row 2, column 3.
			\param	in_dA20	The value for the element in row 3, column 1.
			\param	in_dA21	The value for the element in row 3, column 2.
			\param	in_dA22	The value for the element in row 3, column 3.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& Set( 
			const double in_dA00, const double in_dA01, const double in_dA02, 
			const double in_dA10, const double in_dA11, const double in_dA12, 
			const double in_dA20, const double in_dA21, const double in_dA22 );

		/*! Sets all elements of the matrix.
			\param	in_dVal	3x3 array of double values, containing the new values for all 
					the matrix elements.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& Set( double in_dVal[3][3] );

		/*! Sets all elements of the matrix to those contained in \p in_mat.
			\param	in_mat	Matrix to copy all element values from.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& Set( const CSIBCMatrix33d& in_mat );

		/*! Sets all elements of this matrix to be a submatrix of \p in_mat.
			\param	in_nRow	Row index of the row to remove from \p in_mat (zero indexed).
			\param	in_nCol	Column index of the column to remove from \p in_mat (zero indexed).
			\param	in_mat	The matrix to use element values from (with column \p in_nCol and row
					\p in_nRow removed).
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& Set( int in_nRow, int in_nCol, const CSIBCMatrix44d& in_mat );

		/*!	Sets all elements in a row of the matrix.
			\param	in_nRow	Row index of the row to set (zero indexed).
			\param	in_vct	Vector containing the values to replace the specified row elements with.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& SetRow( const int in_nRow, const CSIBCVector3Dd& in_vct );

		/*!	Sets all elements in a row of the matrix.
			\param	in_nRow	Row index of the row to set (zero indexed).
			\param	in_dA0	Value for the element in the first column of the row.
			\param	in_dA1	Value for the element in the second column of the row.
			\param	in_dA2	Value for the element in the third column of the row.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& SetRow( const int in_nRow, double in_dA0, double in_dA1, double in_dA2 );

		/*!	Sets all elements in a column of the matrix.
			\param	in_nCol	Column index of the column to set (zero indexed).
			\param	in_vct	Vector containing the values to replace the specified column elements with.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& SetCol( const int in_nCol, const CSIBCVector3Dd& in_vct );

		/*!	Sets all elements in a column of the matrix.
			\param	in_nCol	Column index of the column to set (zero indexed).
			\param	in_dA0	Value for the element in the first row of the column.
			\param	in_dA1	Value for the element in the second row of the column.
			\param	in_dA2	Value for the element in the third row of the column.
			\sa CSIBCMatrix33d::Set
			\sa CSIBCMatrix33d::SetRow
			\sa CSIBCMatrix33d::SetCol
			\sa CSIBCMatrix33d::Get
		*/
		CSIBCMatrix33d& SetCol( const int in_nCol, double in_dA0, double in_dA1,	double in_dA2 );

		/*! Sets all elements in the matrix to zero.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::SetIdentity
			\sa CSIBCMatrix33d::Set
		*/
		CSIBCMatrix33d& SetNull();

		/*! Sets this matrix to be the identity matrix.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::SetNull
			\sa CSIBCMatrix33d::Set
		*/
		CSIBCMatrix33d& SetIdentity();

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
		friend bool AreAlmostEqual( const CSIBCMatrix33d& in_mat1,
			const CSIBCMatrix33d& in_mat2, const double in_dEpsilon = PICO_EPS );

		/*! Determines whether all corresponding elements of this matrix and \p in_mat are
			exactly equal. 
			\param	in_mat	The matrix to compare equality with.
			\return	bool	true if the matricies are exactly equal, false otherwise.
			\sa CSIBCMatrix33d::AreAlmostEqual
			\sa CSIBCMatrix33d::operator!=
		*/
		bool operator ==( const CSIBCMatrix33d& in_mat ) const;

			/*! Determines whether any corresponding elements of this matrix and \p in_mat are
				different.
				\param	in_mat	The matrix to compare inequality with.
				\return	bool	true if the matricies differ in any element, false otherwise.
				\sa CSIBCMatrix33d::AreAlmostEqual
				\sa CSIBCMatrix33d::operator==
			*/
		bool operator !=( const CSIBCMatrix33d& in_mat ) const;

		/*! Determines whether all elements of this matrix are zero. Since this function uses
			floating-point comparison, a threshhold value of \p in_dEpsilon is used for equality
			comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS
			\return bool	true if the matrix elements are all zero, false otherwise.
			\sa CSIBCMatrix33d::SetNull
			\sa CSIBCMatrix33d::AreAlmostEqual
			\sa CSIBCMatrix33d::operator==
		*/
		bool IsNull( const double in_dEpsilon=PICO_EPS ) const;

		/*! Determines whether the matrix is singular. A singular matrix is a square matrix that has
			no inverse. A matrix is singular iff it has a determinant of zero. Since this function 
			uses floating-point comparison (for the determinant value), a threshhold value of 
			\p in_dEpsilon is used for equality comparison. 
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS
			\return	bool	true if the matrix is singular, false otherwise.
			\sa CSIBCMatrix33d::IsSymmetric
			\sa CSIBCMatrix33d::GetDet
		*/
		bool IsSingular( const double in_dEpsilon=PICO_EPS ) const;

		/*! Determines whether the matrix is symmetric. A symmetric matrix is one that is equal to
			its transpose. Since this function uses floating-point comparison, a threshhold value
			of \p in_dEpsilon is used for equality comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS
			\return	bool	true if the matrix is symmetric, false otherwise.
			\sa CSIBCMatrix33d::Symmetry
			\sa CSIBCMatrix33d::Transpose
		*/
		bool IsSymmetric( const double in_dEpsilon=PICO_EPS ) const;

		//****************************************
		// Matrix algebra functions:
		//****************************************

		/*! Computes the matrix addition of two matricies, and stores the result in this matrix.
			\param	in_mat1	The first matrix to add.
			\param	in_mat2	The second matrix to add.
			\return CSIBCMatrix33&	Reference to this matrix.
			\sa CSIBCMatrix33d::Add
			\sa CSIBCMatrix33d::Sub
		*/
		CSIBCMatrix33d& Add( const CSIBCMatrix33d& in_mat1, const CSIBCMatrix33d& in_mat2 );

		/*! Computes the matrix addition of this matrix and \p in_mat, and stores the result in this matrix.
			\param	in_mat	The matrix to add to this matrix.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Add
			\sa CSIBCMatrix33d::Sub
		*/
		CSIBCMatrix33d& Add( const CSIBCMatrix33d& in_mat );

		/*! Computes the difference of two matricies (\p in_mat1 - \p in_mat2) and stores the result in this matrix.
			\param	in_mat1	The matrix to subtract from.
			\param	in_mat2	The matrix to subtract.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Sub
			\sa CSIBCMatrix33d::Add
		*/
		CSIBCMatrix33d& Sub( const CSIBCMatrix33d& in_mat1, const CSIBCMatrix33d& in_mat2 );

		/*! Computes the difference of this matrix and \p in_mat2 (\p this - \p in_mat) and stores the 
			result in this matrix.
			\param	in_mat	The matrix to subtract.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Sub
			\sa CSIBCMatrix33d::Add
		*/
		CSIBCMatrix33d& Sub( const CSIBCMatrix33d& in_mat );

		/*! Computes the matrix multiplication of two matricies (\p in_mat1 * \p in_mat2) and stores the
			result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left).
			\param	in_mat2	The second matrix in the multiplication (on the right).
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Mul
			\sa CSIBCMatrix33d::MulTransByReg
			\sa CSIBCMatrix33d::MulRegByTrans
			\sa	CSIBCMatrix33d::Div
		*/
		CSIBCMatrix33d& Mul( const CSIBCMatrix33d& in_mat1, const CSIBCMatrix33d& in_mat2 );

		/*! Computes the matrix multiplication of this matrix and \p in_mat (\p this * \p in_mat) and
			stores the result in this matrix.
			\param	in_mat	The matrix to right-multiply this matrix by.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Mul
			\sa CSIBCMatrix33d::MulTransByReg
			\sa CSIBCMatrix33d::MulRegByTrans
			\sa CSIBCMatrix33d::Div
		*/
		CSIBCMatrix33d& Mul( const CSIBCMatrix33d& in_mat );

		/*! Computes the matrix multiplication of the transpose of \p in_mat1 and \p in_mat2
			(\p in_mat1^T * \p in_mat2) and stores the result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left), to be transposed 
					before multiplication.
			\param	in_mat2	The second matrix in the multiplication (on the right).
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Mul
			\sa CSIBCMatrix33d::MulRegByTrans
			\sa CSIBCMatrix33d::Div
		*/
		CSIBCMatrix33d& MulTransByReg( const CSIBCMatrix33d& in_mat1, const CSIBCMatrix33d& in_mat2 );

		/*! Computes the matrix multiplication of \p in_mat1 and the transpose of \p in_mat2
			(\p in_mat1 * \p in_mat2^T) and stores the result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left).
			\param	in_mat2	The second matrix in the multiplication (on the right), to be transposed 
					before multiplication.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Mul
			\sa CSIBCMatrix33d::MulTransByReg
			\sa CSIBCMatrix33d::Div
		*/
		CSIBCMatrix33d& MulRegByTrans( const CSIBCMatrix33d& in_mat1, const CSIBCMatrix33d& in_mat2 );

		/*! Sets this matrix to the negative of \p in_mat. The 'negative' of a matrix is produced
			by multiplying each element of the matrix by -1.
			\param	in_mat	Matrix containing the data to set this matrix to the negative of.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Negate()
		*/
		CSIBCMatrix33d& Negate( const CSIBCMatrix33d& in_mat );

		/*! Sets this matrix to its negative. The 'negative' of a matrix is produced
			by multiplying each element of the matrix by -1.
			\return	CSIBCMatrix33d&	Reference to this matrix
			\sa CSIBCMatrix33d::Negate( const CSIBCMatrix33d& )
		*/
		CSIBCMatrix33d& Negate();
		
		/*! Scales each element of \p in_mat by \p in_dFactor, and assigns the result to the
			corresponding element in this matrix.
			\param	in_dFactor	Scaling factor for elements of \p in_mat
			\param	in_mat		The input matrix, to scale and assign to this matrix.
			\return	CSIBCMatrix33d&	Reference to this matrix
			\sa CSIBCMatrix33d::Mul
			\sa CSIBCMatrix33d::Div
		*/
		CSIBCMatrix33d& Mul( const double in_dFactor, const CSIBCMatrix33d& in_mat );

		/*! Scales each element of \p in_mat by \p in_dFactor, and assigns the result to the
			corresponding element in this matrix.
			\param	in_mat		The input matrix, to scale and assign to this matrix.
			\param	in_dFactor	Scaling factor for elements of \p in_mat
			\return	CSIBCMatrix33d&	Reference to this matrix
			\sa CSIBCMatrix33d::Mul
			\sa CSIBCMatrix33d::Div
		*/
		CSIBCMatrix33d& Mul( const CSIBCMatrix33d& in_mat, const double in_dFactor );

		/*!	Scales each component of this matrix by \p in_dFactor
			\param	in_dFactor	Scaling factor for elements in this matrix
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Mul
			\sa CSIBCMatrix33d::Div
		*/
		CSIBCMatrix33d& Mul( const double in_dFactor );

		/*!	Divides each component of \p in_mat by \p in_dFactor and assigns the result to the
			corresponding element in this matrix.
			\param	in_mat		The input matrix, to divide and assign to this matrix.
			\param	in_dFactor	Dividing factor for elements of \p in_mat
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Div
			\sa CSIBCMatrix33d::Mul
		*/
		CSIBCMatrix33d& Div( const CSIBCMatrix33d& in_mat, const double in_dFactor );

		/*!	Divides each component of this matrix by \p in_dFactor.
			\param	in_dFactor	Dividing factor for elements of this matrix.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Div
			\sa CSIBCMatrix33d::Mul
		*/
		CSIBCMatrix33d& Div( const double in_dFactor );

		/*! Computes the determinant of this matrix.
			\return	double	The determinant of this matrix.
			\sa CSIBCMatrix33d::GetTrace
			\sa CSIBCMatrix33d::IsSingular
		*/
		double GetDet( void ) const;

		/*! Computes the trace of this matrix. A trace is the sum of all the diagonal elements of a matrix.
			\return	double	The trace of this matrix.
			\sa CSIBCMatrix33d::GetDet
		*/
		double GetTrace( void ) const;

		/*! Sets this matrix to be the transpose of \p in_mat.
			\param	in_mat	Matrix containing the transpose of the desired matrix.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Transpose()
			\sa CSIBCMatrix33d::TransposeInverse
			\sa CSIBCMatrix33d::IsSymmetric
		*/
		CSIBCMatrix33d& Transpose( const CSIBCMatrix33d& in_mat );

		/*! Sets this matrix to its transpose.
			\return CSIBCMatrix33d&	Reference to this matrix.
			\sa CSIBCMatrix33d::Transpose( const CSIBCMatrix33d& )
			\sa CSIBCMatrix33d::TransposeInverse
		*/
		CSIBCMatrix33d& Transpose( void );

		/*! Sets this matrix to be the inverse of \p in_mat.
			\param	in_mat	Matrix containing the inverse of the desired matrix.
			\return	bool	true if \p in_mat could be inverted and the assignment was successful,
					false otherwise.
			\sa CSIBCMatrix33d::Invert()
			\sa CSIBCMatrix33d::TransposeInverse
		*/
		bool Invert( const CSIBCMatrix33d& in_mat );

		/*! Sets this matrix to its inverse.
			\return	bool	true if this matrix could be inverted and the assignment was successful,
					false otherwise.
			\sa CSIBCMatrix33d::Invert( const CSIBCMatrix33d& )
			\sa CSIBCMatrix33d::TransposeInverse
		*/
		bool Invert( void );

		/*! Sets this matrix to the transpose of the inverse of \p in_mat.
			\param	in_mat	Matrix containing the transpose of the inverse of the desired matrix.
			\return	bool	true if the matrix could be inverted (and transposed) and assignment
					was successful, false otherwise.
			\sa CSIBCMatrix33d::TransposeInverse()
			\sa CSIBCMatrix33d::Transpose
			\sa CSIBCMatrix33d::Invert
		*/
		bool TransposeInverse( const CSIBCMatrix33d& in_mat );

		/*! Sets this matrix to the transpose of the inverse of itself.
			\return	bool	true if the matrix could be inverted (and transposed) and assignment
					was successful, false otherwise.
			\sa CSIBCMatrix33d::TransposeInverse( const CSIBCMatrix33d& )
			\sa CSIBCMatrix33d::Transpose
			\sa CSIBCMatrix33d::Inverse
		*/
		bool TransposeInverse( void );

		/*! Sets this matrix to the adjoint of \p in_mat.
			\param	in_mat	Matrix containing the adjoint of the desired matrix.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Adjoint()
		*/
		CSIBCMatrix33d& Adjoint( const CSIBCMatrix33d& in_mat );

		/*! Sets this matrix to its adjoint.
			\return	CSIBCMatrix33d&	Reference to this matrix.
			\sa	CSIBCMatrix33d::Adjoint( const CSIBCMatrix33d& )
		*/
		CSIBCMatrix33d& Adjoint( void );

		/*! Computes the eigenvectors and eigenvalues of this matrix. Since this method uses floating-point 
			comparison, a threshhold value of \p in_dEps is used for equality comparison.
			\param	out_matEigenVector	Matrix whose rows represent the eigenvectors of this matrix.
			\param	out_vctEigenValues	Vector whose components represent the eigenvalues of the corresponding
					eigenvectors in \p out_matEigenVector.
			\param	in_dEps				Floating-point comparison error range. Defaults to #PICO_EPS
		*/
		bool GetEigenVectors( CSIBCMatrix33d& out_matEigenVectors, CSIBCVector3Dd& out_vctEigenValues, const double in_dEps=PICO_EPS ) const;

		//****************************************
		// Utility functions:
		//****************************************

		//****************************************
		// To be implemented later:
		//****************************************

		// Set this matrix to the symmetrical of the input matrix.
		/*! Not implemented yet */
		CSIBCMatrix33d& Symmetry( CSIBCMatrix33d& in_mat ) const;
		
		// Set this matrix to its symmetrical.
		/*! Not implemented yet */
		CSIBCMatrix33d& Symmetry( void ) const;

		// Set this matrix to the product of V1^T and V2.
		/*! Not implemented yet */
		void SetOuterProduct( const CSIBCVector3Dd& in_vct1, const CSIBCVector3Dd& in_vct2 );

	// Protected members
	protected:

		//****************************************
		// Protected data
		//****************************************
		// double[3][3] | CSIBCMatrix33d | m_dMat | Matrix itself.
		double m_dMat[3][3];

		//****************************************
		// Utility functions:
		//****************************************

		// Computes the inverse of a matrix.
		bool ComputeInverse( const double in_dMat[3][3], double out_dInv[3][3], 
				const bool in_bTransposeResult=false, const double in_dDetMin=PICO_EPS ) const;
		// Computes the adjoint of a matrix.
		void ComputeAdjoint( const double in_dMat[3][3], double out_dAdj[3][3] ) const;
		
		// Computes the determinant of a matrix given its adjoint.
		double ComputeDetFast( double in_dMat[3][3], double in_dAdj[3][3] ) const;
		
		// Compute the determinant of a matrix and compute a measure of its scale (max. order of magnitude).
		void ComputeDetAndDetSize( const double in_dMat[3][3], 
				double &out_dDet, double &out_dDetSize ) const;
		// Computes eigenvalues and eigenvectors of a symmetric matrix
		bool ComputeEigen( const double in_dMat[3][3], double out_dMatEigenVectors[3][3], CSIBCVector3Dd &out_vctEigenValues,
				const double in_dEps=PICO_EPS ) const;
};

 
#endif

