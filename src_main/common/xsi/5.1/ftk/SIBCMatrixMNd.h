//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCMatrixMNd.h | Main header file for SIBCMatrixMNd implementation
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

#ifndef _MATRIXMN_H_
#define _MATRIXMN_H_

//******************************************************************************
// Includes
//******************************************************************************
#include "SIBCMathLib.h"

//******************************************************************************
// CSIBCMatrixMNd | General MXN matrix.
//******************************************************************************

//! Class representing matricies with M rows, and N columns.
/*! CSIBCMatrixMNd objects represent matricies of arbitrary size, which are not 
	necessarily square (as with other matrix classes). Because the matricies represented
	by CSIBCMatrixMNd are of arbitrary size, there are considerably less operations
	that are available through this class, as compared to other matrix classes.

	This class uses double precision floating point numbers to store element values. There exists
	three other matrix classes, CSIBCMatrix44d, CSIBCMatrix33d and CSIBCMatrixMNd. They are used 
	to represent 4x4 matricies with double precision floating-point numbers, 3x3 matricies with double 
	precision floating-point numbers, and MxN matricies with double precision floating-point numbers,
	respectively.	
*/
class XSICOREEXPORT CSIBCMatrixMNd
{
	// Public members
	public:

		//****************************************
		// Constructors/destructor:
		//****************************************

		/*! Default Constructor.
			Sets this matrix to be a zero row, zero column matrix (null).
		*/
		CSIBCMatrixMNd();

		// Destructor.
		 ~CSIBCMatrixMNd();

		//****************************************
		// Matrix modification functions:
		//****************************************

		/*! Sets the dimensions of this matrix to \p in_M rows, and \p in_N columns.
			If the dimensions of the matrix are the same as they previously were, nothing
			is done. Otherwise, the matrix is resized, and all element values are lost.
			\param	in_M	The number of rows to resize the matrix to.
			\param	in_N	The number of columns to resize the matrix to.
			\return	bool	Always true.
			\sa CSIBCMatrixMNd::Set
			\sa CSIBCMatrixMNd::GetNbRow
			\sa CSIBCMatrixMNd::GetNbCol
		*/
		bool SetDimensions(const int in_M, const int in_N);

		/*! Sets this matrix to be a duplicate of \p in_mat.
			\param	in_mat	The matrix to copy data from.
			\return	bool	Always true.
			\sa CSIBCMatrixMNd::SetDimensions
			\sa CSIBCMatrixMNd::Copy
		*/
		bool Set( const CSIBCMatrixMNd& in_mat);

		/*! Sets this matrix's elements equal to the corresponding elements in \p in_mat.
			This function should only be used if this matrix and \p in_mat have the same
			dimensions.
			\param	in_mat	Matrix to copy element values from.
			\sa CSIBCMatrixMNd::Set
		*/
		void Copy( const CSIBCMatrixMNd& in_mat);

		/*! Sets this matrix to be the transpose of \p in_mat.
			\param	in_mat	Matrix containing the desired matrix to transpose.
			\return	CSIBCMatrixMNd&	Reference to this matrix.
		*/
		CSIBCMatrixMNd& Transpose(const CSIBCMatrixMNd& in_mat);

		//****************************************
		// General access functions:
		//****************************************

		/*! Returns the number of rows in this matrix.
			\return	int	The number of rows in this matrix.
			\sa CSIBCMatrixMNd::GetNbCol
			\sa CSIBCMatrixMNd::SetDimensions
		*/
		int GetNbRow( void ) const;

		/*! Returns the number of columns in this matrix.
			\return	int	The number of columns in this matrix.
			\sa CSIBCMatrixMNd::GetNbRow
			\sa CSIBCMatrixMNd::SetDimensions
		*/
		int GetNbCol( void ) const;

		/*! Returns the element value at row \p in_nRow, column \p in_nCol.
			\param	in_nRow	The index of the row of the element to retrieve (zero-indexed).
			\param	in_nCol	The index of the column of the element to retrieve (zero-indexed).
			\return	double	The value of the element at row \p in_nRow, column \p in_nCol.
			\sa CSIBCMatrixMNd::Set
		*/
		double Get( const int in_nRow, const int in_nCol ) const;

		//****************************************
		// Matrix element modification functions:
		//****************************************
		
		/*! Sets the element at row \p in_nRow, column \p in_nCol to \p in_dVal.
			\param	in_nRow	The index of the row of the element to set (zero-indexed).
			\param	in_nCol	The index of the column of the element to set (zero-indexed).
			\param	in_dVal	The value to set.
			\sa	CSIBCMatrixMNd::Get
			\sa CSIBCMatrixMNd::SetIdentity
		*/
		void Set( int in_nRow, int in_nCol, const double in_dVal );

		/*! Sets this matrix to the identity matrix, if it is a square matrix.
			\sa CSIBCMatrixMNd::Set
		*/
		void SetIdentity( void );

		//****************************************
		// Matrix algebra functions:
		//****************************************

		/*! Computes the matrix-multiplication of \p in_mat1 and \p in_mat2, (\p in_mat1 * \p in_mat2)
			and stores the result in this matrix. It is up to the user to ensure that matrix multiplication
			can be performed (\p in_mat1's number of columns is equal to \p in_mat2's number of rows).
			\param	in_mat1	First matrix to multiply (on the left).
			\param	in_mat2 Second matrix to multiply (on the right).
			\return	CSIBCMatrixMNd&	Reference to this matrix.
			\sa CSIBCMatrixMNd::Mul
			\sa CSIBCMatrixMNd::LeftMul
		*/
		CSIBCMatrixMNd& Mul( const CSIBCMatrixMNd& in_mat1, const CSIBCMatrixMNd& in_mat2 );

		/*! Computes the matrix-multiplication of this matrix and \p in_mat, (this * \p in_mat),
			and stores the result in this matrix. It is up to the user to ensure that matrix multiplication
			can be performed (this matrix's number of columns is equal to \p in_mat's number of rows).
			\param	in_mat	The matrix to right-multiply this matrix by.
			\return	bool	Always true.
			\sa CSIBCMatrixMNd::Mul
			\sa CSIBCMatrixMNd::LeftMul
		*/
		bool Mul( const CSIBCMatrixMNd& in_mat );

		/*! Computes the matrix-multiplication of \p in_mat and this matrix, (\p in_mat * this),
			and stores the result in this matrix. It is up to the user to ensure that matrix multiplication
			can be performed (\p in_mat's number of columns is equal to this matrix's number of rows).
			\param	in_mat	The matrix to left-multiply this matrix by.
			\return	bool	Always true.
			\sa CSIBCMatrixMNd::Mul
		*/
		bool LeftMul( const CSIBCMatrixMNd& in_mat );

		/*! Sets this matrix to the Moore-Penrose pseudo-inverse using Greville's method.
			\param	in_mat	Matrix containing the matrix to take the Moore-Penrose pseudo-inverse of.
			\return	bool	Always true.
			\sa CSIBCMatrixMNd::GrevilleInverse
		*/
		bool GrevilleInverse(const CSIBCMatrixMNd& in_mat);

		/*! Sets this matrix to the Moore-Penrose pseudo-inverse using Greville's method, using only
			the first \p in_NbRow rows, and the first \p in_NbCol columns of of \p in_mat.
			\param	in_mat		Matrix containing the matrix to take the Moore-Penrose pseudo-inverse of.
			\param	in_NbRow	Number of rows to use from the beginning \p in_mat.
			\param	in_NbCol	Number of columns to use from the beginning of \p in_mat.
			\return	bool		Always true.
			\sa CSIBCMatrixMNd::GrevilleInverse
		*/
		bool GrevilleInverse(const CSIBCMatrixMNd& in_mat, const unsigned int in_NbRow,	const unsigned int in_NbCol);

		/*! Computes the matrix-addition of \p in_mat1 and \p in_mat2. The result
			is stored in this matrix. Note that this matrix, \p in_mat1 and \p in_mat2 should 
			all be of the same dimensions.
			\param	in_mat1	The first matrix in the addition.
			\param	in_mat2	The second matrix in the addition.
			\return	CSIBCMatrixMNd&	Reference to this matrix.
			\sa CSIBCMatrixMNd::Add
			\sa CSIBCMatrixMNd::Sub
		*/
		CSIBCMatrixMNd& Add( const CSIBCMatrixMNd& in_mat1, const CSIBCMatrixMNd& in_mat2);

		/*! Computes the matrix-subtraction of \p in_mat1 - \p in_mat2. The result
			is stored in this matrix. Note that this matrix, \p in_mat1 and \p in_mat2 should 
			all be of the same dimensions.
			\param	in_mat1	The first matrix in the subtraction.
			\param	in_mat2	The second matrix in the subtraction.
			\return	CSIBCMatrixMNd&	Reference to this matrix.
			\sa CSIBCMatrixMNd::Add
			\sa CSIBCMatrixMNd::Sub
		*/
		CSIBCMatrixMNd& Sub( const CSIBCMatrixMNd& in_mat1, const CSIBCMatrixMNd& in_mat2);

		/*! Computes the matrix-addition of this matrix and \p in_mat. The result
			is stored in this matrix. Note that this matrix and \p in_mat should both
			be of the same dimensions.
			\param	in_mat	The matrix to add to this matrix.
			\return	CSIBCMatrixMNd&	Reference to this matrix.
			\sa CSIBCMatrixMNd::Add
			\sa CSIBCMatrixMNd::Sub
		*/
		CSIBCMatrixMNd& Add( const CSIBCMatrixMNd& in_mat );

		/*! Computes the matrix-subtraction of this matrix - \p in_mat. The result
			is stored in this matrix. Note that this matrix and \p in_mat should both
			be of the same dimensions.
			\param	in_mat	The matrix to subtract from this matrix.
			\return	CSIBCMatrixMNd&	Reference to this matrix.
			\sa CSIBCMatrixMNd::Add
			\sa CSIBCMatrixMNd::Sub
		*/
		CSIBCMatrixMNd& Sub( const CSIBCMatrixMNd& in_mat );

	// Protected members
	protected:

		//****************************************
		// Protected data
		//****************************************

	// Private members
	private:
		//****************************************
		// Matrix algebra functions:
		//****************************************

		// Output the inverse of the input vector.
		void InverseVector(double *in_pdVct, double * out_pdInvVct, int in_len);

		// Returns the sum of the absolute values of elements in vector.
		double SumAbsVal( double *in_pdVct, int in_len);

		//****************************************
		// Private data
		//****************************************

		//	Number of rows in the matrix (M)
		int m_nRow;

		//	Number of columns in the matrix (N)
		int m_nCol;

		//	Pointer to MXN Matrix
		double**	m_dMatrixMN;

		////////////////////////////////////////////////////////////////////////
		// Debugging goodies
		////////////////////////////////////////////////////////////////////////

		//Disable copy constructor and operator = 
		CSIBCMatrixMNd ( const CSIBCMatrixMNd& in_matrix );

		// Assignment operator.
		CSIBCMatrixMNd& operator =( const CSIBCMatrixMNd& in_matrix );
};

#endif // _MATRIXMN_H_
