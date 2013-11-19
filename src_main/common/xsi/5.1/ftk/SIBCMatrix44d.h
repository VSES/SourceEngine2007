//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCMatrix44d.h | Main header file for SIBCMatrix44d implementation
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
#ifndef _MATRIX44_H_
#define _MATRIX44_H_

//******************************************************************************
// Includes
//******************************************************************************
#include <assert.h>
#include "SIBCMathLib.h"

#define _3D_NB_BITS_RESERVED_BY_MAT44 (unsigned char)1	//This number must represent the number of bits flags in m_MaskInfo 
														//reserved for the specific usage of CSIBCMatrix44d

//******************************************************************************
//
// CSIBCMatrix44d | General 4X4 matrix.
//
//******************************************************************************

//! Class representing matricies with 4 rows, and 4 columns.
/*! CSIBCMatrix44d objects represent matricies, and define a set of operations which are
	useful in the context of a matrix. 

	This class uses double precision floating point numbers to store element values. There exists
	three other matrix classes, CSIBCMatrix4x4, CSIBCMatrix33d and CSIBCMatrixMNd. They are used 
	to represent 4x4 matricies with single precision floating-point numbers, 3x3 matricies with double 
	precision floating-point numbers, and MxN matricies with double precision floating-point numbers,
	respectively.	
*/
class XSICOREEXPORT CSIBCMatrix44d
{
	public:

		//****************************************
		// Constructors/destructor:
		//****************************************

		/*! Default constructor
			Sets all elements in the matrix to zero.
			\sa CSIBCMatrix44d::SetNull
		*/
		CSIBCMatrix44d( void );

		/*! Constructor.
			Sets the rows of the new matrix to be \p in_vct0, \p in_vct1, \p in_vct2, and
			\p in_vct3. If \p in_bAreRows is false, then the input vectors are used as the 
			columns of the	new matrix.
			\param	in_vct0	The vector to use for the first row (or column) of the matrix.
			\param	in_vct1	The vector to use for the second row (or column) of the matrix.
			\param	in_vct2	The vector to use for the third row (or column) of the matrix.
			\param	in_vct3	The vector to use for the fourth row (or column) of the matrix.
			\param	in_bAreRows	Whether the vectors given represent rows or columns of the matrix
					(true == Rows, false == Columns). Defaults to true (rows).
		*/	
		CSIBCMatrix44d( 
			const CSIBCVector4Dd& in_vct0,
			const CSIBCVector4Dd& in_vct1,
			const CSIBCVector4Dd& in_vct2,
			const CSIBCVector4Dd& in_vct3,
			const bool in_bAreRows = true );

		/*! Constructor.
			Specifies the value of each element in the new matrix.
			\param	in_dA00	The value for the element in row 1, column 1.
			\param	in_dA01	The value for the element in row 1, column 2.
			\param	in_dA02	The value for the element in row 1, column 3.
			\param	in_dA03	The value for the element in row 1, column 4.
			\param	in_dA10	The value for the element in row 2, column 1.
			\param	in_dA11	The value for the element in row 2, column 2.
			\param	in_dA12	The value for the element in row 2, column 3.
			\param	in_dA13	The value for the element in row 2, column 4.
			\param	in_dA20	The value for the element in row 3, column 1.
			\param	in_dA21	The value for the element in row 3, column 2.
			\param	in_dA22	The value for the element in row 3, column 3.
			\param	in_dA23	The value for the element in row 3, column 4.
			\param	in_dA30	The value for the element in row 4, column 1.
			\param	in_dA31	The value for the element in row 4, column 2.
			\param	in_dA32	The value for the element in row 4, column 3.
			\param	in_dA33	The value for the element in row 4, column 4.
		*/
		CSIBCMatrix44d(
			const double in_dA00, const double in_dA01, const double in_dA02, const double in_dA03,
			const double in_dA10, const double in_dA11, const double in_dA12, const double in_dA13,
			const double in_dA20, const double in_dA21, const double in_dA22, const double in_dA23,
			const double in_dA30, const double in_dA31, const double in_dA32, const double in_dA33 );

		// Destructor.
		~CSIBCMatrix44d();

		//****************************************
		// General access functions:
		//****************************************

		/*! Returns an element of the marix.
			\param	in_nRow	Row index of the element to retrieve (zero indexed).
			\param	in_nCol Column index of the element to retrieve (zero indexed).
			\return	double	The value of the element at the given matrix position.
			\sa CSIBCMatrix44d::Set(int, int, const double)
		*/
		double Get( const int in_nRow, const int in_nCol ) const;

		//****************************************
		// Matrix element modification functions:
		//****************************************

		/*! Sets an element of the matrix.
			\param	in_nRow	Row index of the element to set (zero indexed).
			\param	in_nCol	Column index of the element to set (zero indexed).
			\param	in_dVal	Value for the element.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& Set( int in_nRow, int in_nCol, const double in_dVal );

		/*! Sets all elements of the matrix.
			\param	in_dA00	The value for the element in row 1, column 1.
			\param	in_dA01	The value for the element in row 1, column 2.
			\param	in_dA02	The value for the element in row 1, column 3.
			\param	in_dA03	The value for the element in row 1, column 4.
			\param	in_dA10	The value for the element in row 2, column 1.
			\param	in_dA11	The value for the element in row 2, column 2.
			\param	in_dA12	The value for the element in row 2, column 3.
			\param	in_dA13	The value for the element in row 2, column 4.
			\param	in_dA20	The value for the element in row 3, column 1.
			\param	in_dA21	The value for the element in row 3, column 2.
			\param	in_dA22	The value for the element in row 3, column 3.
			\param	in_dA23	The value for the element in row 3, column 4.
			\param	in_dA30	The value for the element in row 4, column 1.
			\param	in_dA31	The value for the element in row 4, column 2.
			\param	in_dA32	The value for the element in row 4, column 3.
			\param	in_dA33	The value for the element in row 4, column 4.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& Set(
			const double in_dA00, const double in_dA01, const double in_dA02, const double in_dA03,
			const double in_dA10, const double in_dA11, const double in_dA12, const double in_dA13,
			const double in_dA20, const double in_dA21, const double in_dA22, const double in_dA23,
			const double in_dA30, const double in_dA31, const double in_dA32, const double in_dA33 );


		/*! Sets all elements of the matrix.
			\param	in_dVal	4x4 array of double values, containing the new values for all 
					the matrix elements.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& Set( const double in_dArray[4][4] );

		/*! Sets all elements of the matrix.
			\param	in_dVal	4x4 array of double values, containing the new values for all
					the matrix elements.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& Set( const double *in_dPtr ) 
		{
			return Set( (const double (*)[4] ) in_dPtr );
		};

		
		/*! Sets all elements of the matrix to those contained in \p in_mat.
			\param	in_mat	Matrix to copy all element values from.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& Set( const CSIBCMatrix44d& in_mat );

		/*!	Sets all elements in a row of the matrix.
			\param	in_nRow	Row index of the row to set (zero indexed).
			\param	in_vct	Vector containing the values to replace the specified row elements with.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& SetRow( const int in_nRow, const CSIBCVector4Dd& in_vct );

		/*!	Sets all elements in a row of the matrix.
			\param	in_nRow	Row index of the row to set (zero indexed).
			\param	in_dA0	Value for the element in the first column of the row.
			\param	in_dA1	Value for the element in the second column of the row.
			\param	in_dA2	Value for the element in the third column of the row.
			\param	in_dA3	Value for the element in the fourth column of the row.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& SetRow( const int in_nRow, double in_dA0, double in_dA1, 
			double in_dA2, double in_dA3 );

		/*!	Sets all elements in a column of the matrix.
			\param	in_nCol	Column index of the column to set (zero indexed).
			\param	in_vct	Vector containing the values to replace the specified column elements with.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& SetCol( const int in_nCol, const CSIBCVector4Dd& in_vct );

		/*!	Sets all elements in a column of the matrix.
			\param	in_nCol	Column index of the column to set (zero indexed).
			\param	in_dA0	Value for the element in the first row of the column.
			\param	in_dA1	Value for the element in the second row of the column.
			\param	in_dA2	Value for the element in the third row of the column.
			\param	in_dA3	Value for the element in the fourth row of the column.
			\sa CSIBCMatrix44d::Set
			\sa CSIBCMatrix44d::SetRow
			\sa CSIBCMatrix44d::SetCol
			\sa CSIBCMatrix44d::Get
		*/
		CSIBCMatrix44d& SetCol( const int in_nCol, double in_dA0, double in_dA1,
			double in_dA2, double in_dA3 );

		/*! Sets all elements in the matrix to zero.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::SetIdentity
			\sa CSIBCMatrix44d::Set
		*/
		CSIBCMatrix44d& SetNull();

		/*! Sets this matrix to be the identity matrix.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa	CSIBCMatrix44d::SetNull
			\sa CSIBCMatrix44d::Set
		*/
		CSIBCMatrix44d& SetIdentity();

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
			\sa CSIBCMatrix44d::operator==
			\sa CSIBCMatrix44d::operator!=
		*/
		friend bool AreAlmostEqual( const CSIBCMatrix44d& in_mat1,
			const CSIBCMatrix44d& in_mat2, const double in_dEpsilon = PICO_EPS );

		/*! Determines whether all corresponding elements of this matrix and \p in_mat are
			exactly equal. 
			\param	in_mat	The matrix to compare equality with.
			\return	bool	true if the matricies are exactly equal, false otherwise.
			\sa CSIBCMatrix44d::AreAlmostEqual
			\sa CSIBCMatrix44d::operator!=
		*/
		bool operator ==( const CSIBCMatrix44d& in_mat ) const;

		/*! Determines whether any corresponding elements of this matrix and \p in_mat are
			different.
			\param	in_mat	The matrix to compare inequality with.
			\return	bool	true if the matricies differ in any element, false otherwise.
			\sa CSIBCMatrix44d::AreAlmostEqual
			\sa CSIBCMatrix44d::operator==
		*/
		bool operator !=( const CSIBCMatrix44d& in_mat ) const;

		/*! Determines whether all elements of this matrix are zero. Since this function uses
			floating-point comparison, a threshhold value of \p in_dEpsilon is used for equality
			comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS
			\return bool	true if the matrix elements are all zero, false otherwise.
			\sa CSIBCMatrix44d::SetNull
			\sa CSIBCMatrix44d::AreAlmostEqual
			\sa CSIBCMatrix44d::operator==
		*/
		bool IsNull( const double in_dEpsilon=PICO_EPS ) const;

		//****************************************
		// Matrix algebra functions:
		//****************************************
		
		/*! Computes the matrix addition of two matricies, and stores the result in this matrix.
			\param	in_mat1	The first matrix to add.
			\param	in_mat2	The second matrix to add.
			\return CSIBCMatrix33&	Reference to this matrix.
			\sa CSIBCMatrix44d::Add
			\sa CSIBCMatrix44d::Sub
		*/
		CSIBCMatrix44d& Add( const CSIBCMatrix44d& in_mat1, const CSIBCMatrix44d& in_mat2 );

		/*! Computes the matrix addition of this matrix and \p in_mat, and stores the result in this matrix.
			\param	in_mat	The matrix to add to this matrix.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa	CSIBCMatrix44d::Add
			\sa CSIBCMatrix44d::Sub
		*/
		CSIBCMatrix44d& Add( const CSIBCMatrix44d& in_mat );

		/*! Computes the difference of two matricies (\p in_mat1 - \p in_mat2) and stores the result in this matrix.
			\param	in_mat1	The matrix to subtract from.
			\param	in_mat2	The matrix to subtract.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa	CSIBCMatrix44d::Sub
			\sa CSIBCMatrix44d::Add
		*/
		CSIBCMatrix44d& Sub( const CSIBCMatrix44d& in_mat1, const CSIBCMatrix44d& in_mat2 );

		/*! Computes the difference of this matrix and \p in_mat2 (\p this - \p in_mat) and stores the 
			result in this matrix.
			\param	in_mat	The matrix to subtract.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa	CSIBCMatrix44d::Sub
			\sa CSIBCMatrix44d::Add
		*/
		CSIBCMatrix44d& Sub( const CSIBCMatrix44d& in_mat );

		/*! Computes the matrix multiplication of two matricies (\p in_mat1 * \p in_mat2) and stores the
			result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left).
			\param	in_mat2	The second matrix in the multiplication (on the right).
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa	CSIBCMatrix44d::Mul
			\sa CSIBCMatrix44d::MulTransByReg
			\sa CSIBCMatrix44d::MulRegByTrans
			\sa	CSIBCMatrix44d::Div
		*/
		CSIBCMatrix44d& Mul( const CSIBCMatrix44d& in_mat1, const CSIBCMatrix44d& in_mat2 );

		/*! Computes the matrix multiplication of this matrix and \p in_mat (\p this * \p in_mat) and
			stores the result in this matrix.
			\param	in_mat	The matrix to right-multiply this matrix by.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Mul
			\sa CSIBCMatrix44d::MulTransByReg
			\sa CSIBCMatrix44d::MulRegByTrans
			\sa CSIBCMatrix44d::Div
		*/
		CSIBCMatrix44d& Mul( const CSIBCMatrix44d& in_mat );

		/*! Computes the matrix multiplication of the transpose of \p in_mat1 and \p in_mat2
			(\p in_mat1^T * \p in_mat2) and stores the result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left), to be transposed 
					before multiplication.
			\param	in_mat2	The second matrix in the multiplication (on the right).
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa	CSIBCMatrix44d::Mul
			\sa CSIBCMatrix44d::MulRegByTrans
			\sa CSIBCMatrix44d::Div
		*/
		CSIBCMatrix44d& MulTransByReg( const CSIBCMatrix44d& in_mat1, const CSIBCMatrix44d& in_mat2 );

		/*! Computes the matrix multiplication of \p in_mat1 and the transpose of \p in_mat2
			(\p in_mat1 * \p in_mat2^T) and stores the result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left).
			\param	in_mat2	The second matrix in the multiplication (on the right), to be transposed 
					before multiplication.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa	CSIBCMatrix44d::Mul
			\sa CSIBCMatrix44d::MulTransByReg
			\sa CSIBCMatrix44d::Div
		*/
		CSIBCMatrix44d& MulRegByTrans( const CSIBCMatrix44d& in_mat1, const CSIBCMatrix44d& in_mat2 );

		/*! Sets this matrix to the negative of \p in_mat. The 'negative' of a matrix is produced
			by multiplying each element of the matrix by -1.
			\param	in_mat	Matrix containing the data to set this matrix to the negative of.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa	CSIBCMatrix44d::Negate()
		*/
		CSIBCMatrix44d& Negate( const CSIBCMatrix44d& in_mat );

		/*! Sets this matrix to its negative. The 'negative' of a matrix is produced
			by multiplying each element of the matrix by -1.
			\return	CSIBCMatrix44d&	Reference to this matrix
			\sa CSIBCMatrix44d::Negate( const CSIBCMatrix44d& )
		*/
		CSIBCMatrix44d& Negate();

		/*! Scales each element of \p in_mat by \p in_dFactor, and assigns the result to the
			corresponding element in this matrix.
			\param	in_dFactor	Scaling factor for elements of \p in_mat
			\param	in_mat		The input matrix, to scale and assign to this matrix.
			\return	CSIBCMatrix44d&	Reference to this matrix
			\sa CSIBCMatrix44d::Mul
			\sa CSIBCMatrix44d::Div
		*/
		CSIBCMatrix44d& Mul( const double in_dFactor, const CSIBCMatrix44d& in_mat );

		/*! Scales each element of \p in_mat by \p in_dFactor, and assigns the result to the
			corresponding element in this matrix.
			\param	in_mat		The input matrix, to scale and assign to this matrix.
			\param	in_dFactor	Scaling factor for elements of \p in_mat
			\return	CSIBCMatrix44d&	Reference to this matrix
			\sa CSIBCMatrix44d::Mul
			\sa CSIBCMatrix44d::Div
		*/
		CSIBCMatrix44d& Mul( const CSIBCMatrix44d& in_mat, const double in_dFactor );

		/*!	Scales each component of this matrix by \p in_dFactor
			\param	in_dFactor	Scaling factor for elements in this matrix
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Mul
			\sa CSIBCMatrix44d::Div
		*/
		CSIBCMatrix44d& Mul( const double in_dFactor );

		/*!	Divides each component of \p in_mat by \p in_dFactor and assigns the result to the
			corresponding element in this matrix.
			\param	in_mat		The input matrix, to divide and assign to this matrix.
			\param	in_dFactor	Dividing factor for elements of \p in_mat
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Div
			\sa CSIBCMatrix44d::Mul
		*/
		CSIBCMatrix44d& Div( const CSIBCMatrix44d& in_mat, const double in_dFactor );

		/*!	Divides each component of this matrix by \p in_dFactor.
			\param	in_dFactor	Dividing factor for elements of this matrix.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Div
			\sa CSIBCMatrix44d::Mul
		*/
		CSIBCMatrix44d& Div( const double in_dFactor );

		/*! Computes the determinant of this matrix.
			\return	double	The determinant of this matrix.
			\sa CSIBCMatrix44d::GetDet(int, int)
			\sa CSIBCMatrix44d::GetTrace
		*/
		double GetDet( void ) const;

		/*! Computes the determinant of the 3x3 matrix formed by removing row \p in_nRow, and
			column \p in_nCol from this matrix.
			\param	in_nRow	The row to remove from this matrix.
			\param	in_nRow	The column to remove from this matrix.
			\return	double	The determinant of the 3x3 matrix.
			\sa CSIBCMatrix44d::GetDet()
			\sa CSIBCMatrix44d::GetTrace
		*/
		double GetDet( int in_nRow, int in_nCol ) const;

		/*! Computes the trace of this matrix. A trace is the sum of all the diagonal elements of a matrix.
			\return	double	The trace of this matrix.
			\sa CSIBCMatrix44d::GetDet
		*/
		double GetTrace( void ) const;

		/*! Sets this matrix to be the transpose of \p in_mat.
			\param	in_mat	Matrix containing the transpose of the desired matrix.
			\return	CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Transpose()
			\sa CSIBCMatrix44d::TransposeInverse
		*/
		CSIBCMatrix44d& Transpose( const CSIBCMatrix44d& in_mat );

		/*! Sets this matrix to its transpose.
			\return CSIBCMatrix44d&	Reference to this matrix.
			\sa CSIBCMatrix44d::Transpose( const CSIBCMatrix44d& )
			\sa CSIBCMatrix44d::TransposeInverse
		*/
		CSIBCMatrix44d& Transpose( void );

		/*! Sets this matrix to be the inverse of \p in_mat.
			\param	in_mat	Matrix containing the inverse of the desired matrix.
			\return	bool	true if \p in_mat could be inverted and the assignment was successful,
					false otherwise.
			\sa CSIBCMatrix44d::Invert()
			\sa CSIBCMatrix44d::TransposeInverse
		*/
		bool Invert( const CSIBCMatrix44d& in_mat );

		/*! Sets this matrix to its inverse.
			\return	bool	true if this matrix could be inverted and the assignment was successful,
					false otherwise.
			\sa CSIBCMatrix44d::Invert( const CSIBCMatrix44d& )
			\sa CSIBCMatrix44d::TransposeInverse
		*/
		bool Invert( void );

		/*! Sets this matrix to the transpose of the inverse of \p in_mat.
			\param	in_mat	Matrix containing the transpose of the inverse of the desired matrix.
			\return	bool	true if the matrix could be inverted (and transposed) and assignment
					was successful, false otherwise.
			\sa CSIBCMatrix44d::TransposeInverse()
			\sa CSIBCMatrix44d::Transpose
			\sa CSIBCMatrix44d::Invert
		*/
		bool TransposeInverse( const CSIBCMatrix44d& in_mat );

		/*! Sets this matrix to the transpose of the inverse of itself.
			\return	bool	true if the matrix could be inverted (and transposed) and assignment
					was successful, false otherwise.
			\sa CSIBCMatrix44d::TransposeInverse( const CSIBCMatrix44d& )
			\sa CSIBCMatrix44d::Transpose
			\sa CSIBCMatrix44d::Invert
		*/
		bool TransposeInverse( void );

		//****************************************
		// Utility functions:
		//****************************************

		//****************************************
		// To be implemented later:
		//****************************************

		// Set this matrix to the symmetrical of the input matrix.
		/*! Not implemented yet */
		CSIBCMatrix44d& Symmetry( CSIBCMatrix44d& in_mat ) const;

		// Set this matrix to its symmetrical.
		/*! Not implemented yet */
		CSIBCMatrix44d& Symmetry( void ) const;

		// Set this matrix to the adjoint of the input matrix.
		/*! Not implemented yet */
		CSIBCMatrix44d& Adjoint( CSIBCMatrix44d& in_mat ) const;

		// Set this matrix to its adjoint.
		/*! Not implemented yet */
		CSIBCMatrix44d& Adjoint( void ) const;

		// Set this matrix to the product of V1^T and V2.
		/*! Not implemented yet */
		void SetOuterProduct( const CSIBCVector4Dd& in_vct1, const CSIBCVector4Dd& in_vct2 );

	// Protected members
	protected:

		//****************************************
		// Protected data
		//****************************************
		// double[4][4] | CSIBCMatrix44d | m_dMat | Matrix itself.
		double m_dMat[4][4];

		//****************************************
		// Utility functions:
		//****************************************
		// Computes the inverse of a matrix.
		bool ComputeInverse( const double in_dMat[4][4],
			double out_dInv[4][4], const bool bTransposeResult=false );


   protected:
		//****************************************
		// Private data
		//****************************************
		// unsigned char | CSIBCMatrix44d | m_MaskInfo | 
		// Flag to indicate if matrix content has changed 
		// since last call to SetContentUnchanged().

		unsigned char	m_MaskInfo;
};


#endif
