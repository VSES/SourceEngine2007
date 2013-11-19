//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
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

#ifndef _XFOMAT_H_
#define _XFOMAT_H_

//******************************************************************************
// Includes
//******************************************************************************
#include "SIBCMathLib.h"

//This number must represent the number of bits flags in 
//CSIBCMatrix44d::m_MaskInfo reserved for the specific usage of CSIBCXfoMatd.
#define _3D_NB_BITS_RESERVED_BY_XFOMAT (unsigned char)( 3 )	

//******************************************************************************
// CSIBCXfoMatd | 4x4 transformation matrix.
//******************************************************************************

//! Class representing a 4x4 transformation matrix.
/*!	CSIBCXfoMatd is a specialized 4X4 matrix that represents transformations. It
	uses double precision floating-point numbers for its data, and operations (as
	it is derived from CSIBCMatrix44d).

	Although transformations may be defined simply with any generic 4X4, this
	class provides extra methods to make specifying and working with transformation
	matricies easier. A class for general transformations, independent of 
	matrix form is provided in CSIBCTranfod.

	There are several other matrix classes aswell. Classes with specific applications, 
	such as CSIBCRotMatd used for representing rotation matricies, Other matrix 
	classes are not as specialized as CSIBCRotMatd or CSIBCXfoMatd, and are used to 
	represent general matricies. These classes include CSIBCMatrix4x4, CSIBCMatrix44d and
	CSIBCMartrix33d.
*/
	
class XSICOREEXPORT CSIBCXfoMatd : private CSIBCMatrix44d
{
	// Public members
	public:
		//****************************************
		// Constructors/destructor:
		//****************************************

		/*! Default constructor
			Sets the new matrix to be the identity matrix.
			\sa CSIBCXfoMatd::SetIdentity
		*/
		CSIBCXfoMatd( void );

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
		CSIBCXfoMatd( 
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
		CSIBCXfoMatd( 
			const double in_dA00, const double in_dA01, const double in_dA02, const double in_dA03, 
			const double in_dA10, const double in_dA11, const double in_dA12, const double in_dA13, 
			const double in_dA20, const double in_dA21, const double in_dA22, const double in_dA23, 
			const double in_dA30, const double in_dA31, const double in_dA32, const double in_dA33 );

		//****************************************
		// General access functions:
		//****************************************

		/*! Returns an element of the marix.
			\param	in_nRow	Row index of the element to retrieve (zero indexed).
			\param	in_nCol Column index of the element to retrieve (zero indexed).
			\return	double	The value of the element at the given matrix position.
			\sa CSIBCXfoMatd::Set(int, int, const double)
		*/
		double Get( const int in_nRow, const int in_nCol ) const;

		/*!	Returns a pointer to the 4X4 array containing the data for this matrix. Note
			that the pointer returned from this function should not be freed. Any modification
			to the array results in modification of this object directly.
			\return	const double*	Pointer to the 4X4 array containing the data for this matrix.
			\sa CSIBCXfoMatd::Get
		*/
		const double * GetArray() const;

		//****************************************
		// Matrix element modification functions:
		//****************************************

		/*! Sets an element of the matrix.
			\param	in_nRow	Row index of the element to set (zero indexed).
			\param	in_nCol	Column index of the element to set (zero indexed).
			\param	in_dVal	Value for the element.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& Set( int in_nRow, int in_nCol, const double in_dVal );

		/*! Sets all elements of the matrix.
			\param	in_dVal	4x4 array of double values, containing the new values for all 
					the matrix elements.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& Set( double in_dVal[4][4] );

		/*! Sets the top-left submatrix of this matrix.
			\param	in_dVal	3x3 array of double values, containing the new values for the
					elements of the top-left submatrix.
			\return	CSIBCXfoMatd&	Reference to this matrix.
		*/
		CSIBCXfoMatd& Set( double in_dVal[3][3] );

		/*! Sets all elements of the matrix to those contained in \p in_mat.
			\param	in_mat	Matrix to copy all element values from.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& Set( const CSIBCMatrix44d& in_mat );

		/*!	Sets all elements of the top-left submatrix of this matrix to those
			contained in \p in_mat.
			\param	in_mat	Matrix to copy top-left submatrix element value from.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& Set( const CSIBCMatrix33d& in_mat );

		/*! Sets all elements of the matrix to those contained in \p in_mat.
			\param	in_mat	Matrix to copy all element values from.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& Set( const CSIBCXfoMatd& in_mat );

		/*! Sets the transformation matrix to be a pure rotation, defined by \p in_mat.
			\param	in_mat	Rotation to set this transformation matrix to.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/		
		CSIBCXfoMatd& Set( const CSIBCRotMatd& in_mat );

		/*!	Sets the transformation matrix to be a pure rotation, defined by the transpose
			of \p in_mat.
			\param	in_mat	Rotation transpose to set this transformation matrix to.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& SetWithTranspose( const CSIBCRotMatd& in_mat );

		/*!	Sets all elements in a row of the matrix.
			\param	in_nRow	Row index of the row to set (zero indexed).
			\param	in_vct	Vector containing the values to replace the specified row elements with.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& SetRow( const int in_nRow, const CSIBCVector4Dd& in_vct );

		/*!	Sets all elements in a row of the matrix.
			\param	in_nRow	Row index of the row to set (zero indexed).
			\param	in_dA0	Value for the element in the first column of the row.
			\param	in_dA1	Value for the element in the second column of the row.
			\param	in_dA2	Value for the element in the third column of the row.
			\param	in_dA3	Value for the element in the fourth column of the row.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& SetRow( const int in_nRow, double in_dA0, double in_dA1, double in_dA2, double in_dA3 );

		/*!	Sets all elements in a column of the matrix.
			\param	in_nCol	Column index of the column to set (zero indexed).
			\param	in_vct	Vector containing the values to replace the specified column elements with.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& SetCol( const int in_nCol, const CSIBCVector4Dd& in_vct );

		/*!	Sets all elements in a column of the matrix.
			\param	in_nCol	Column index of the column to set (zero indexed).
			\param	in_dA0	Value for the element in the first row of the column.
			\param	in_dA1	Value for the element in the second row of the column.
			\param	in_dA2	Value for the element in the third row of the column.
			\param	in_dA3	Value for the element in the fourth row of the column.
			\sa CSIBCXfoMatd::Set
			\sa CSIBCXfoMatd::SetRow
			\sa CSIBCXfoMatd::SetCol
			\sa CSIBCXfoMatd::Get
		*/
		CSIBCXfoMatd& SetCol( const int in_nCol, double in_dA0, double in_dA1, double in_dA2, double in_dA3 );

		/*! Sets this matrix to be the identity matrix.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Set
		*/
		CSIBCXfoMatd& SetIdentity( void );

		//****************************************
		// Matrix comparison functions:
		//****************************************

		/*! Determines whether all corresponding elements of the matricies \p in_mat1 and
			\p in_mat2 are equal. Since this method uses floating-point comparison, a threshhold
			value of \p in_dEpsilon is used for equality comparison.
			\param	in_mat1	First matrix for equality comparison.
			\param	in_mat2	Second matrix for equality comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #HEXA_EPS.
			\return	bool	true if the matricies are equal, false otherwise.
			\sa CSIBCXfoMatd::operator==
			\sa CSIBCXfoMatd::operator!=
		*/
		friend bool AreAlmostEqual( const CSIBCXfoMatd& in_mat1,
								const CSIBCXfoMatd& in_mat2,
								const double in_dEpsilon = HEXA_EPS );

		/*! Determines whether all corresponding elements of this matrix and \p in_mat are
			exactly equal. 
			\param	in_mat	The matrix to compare equality with.
			\return	bool	true if the matricies are exactly equal, false otherwise.
			\sa CSIBCXfoMatd::AreAlmostEqual
			\sa CSIBCXfoMatd::operator!=
		*/
		bool operator ==( const CSIBCMatrix44d& in_mat ) const;

		/*! Determines whether any corresponding elements of this matrix and \p in_mat are
			different.
			\param	in_mat	The matrix to compare inequality with.
			\return	bool	true if the matricies differ in any element, false otherwise.
			\sa CSIBCXfoMatd::AreAlmostEqual
			\sa CSIBCXfoMatd::operator==
		*/
		bool operator !=( const CSIBCMatrix44d& in_mat ) const;

		//****************************************
		// Matrix algebra functions:
		//****************************************

		/*! Computes the matrix multiplication of two matricies (\p in_mat1 * \p in_mat2) and stores the
			result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left).
			\param	in_mat2	The second matrix in the multiplication (on the right).
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa	CSIBCXfoMatd::Mul
			\sa CSIBCXfoMatd::MulTransByReg
		*/
		CSIBCXfoMatd& Mul( const CSIBCXfoMatd& in_mat1, const CSIBCXfoMatd& in_mat2 );

		/*! Computes the matrix multiplication of this matrix and \p in_mat (\p this * \p in_mat) and
			stores the result in this matrix.
			\param	in_mat	The matrix to right-multiply this matrix by.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Mul
			\sa CSIBCXfoMatd::MulTransByReg
		*/
		CSIBCXfoMatd& Mul( const CSIBCXfoMatd& in_mat );

		/*! Computes the matrix multiplication of two matricies (\p in_mat1 * \p in_mat2) and stores the
			result in this matrix.
			\param	in_mat1	The tranformation matrix in the multiplication (on the left).
			\param	in_mat2	The pure rotation matrix in the multiplication (on the right).
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa	CSIBCXfoMatd::Mul
			\sa CSIBCXfoMatd::MulTransByReg
		*/
		CSIBCXfoMatd& Mul( const CSIBCXfoMatd& in_xfomat, const CSIBCRotMatd& in_rotmat );

		/*! Computes the matrix multiplication of two matricies (\p in_mat1 * \p in_mat2) and stores the
			result in this matrix.
			\param	in_mat1	The pure rotation matrix in the multiplication (on the left).
			\param	in_mat2	The tranformation matrix in the multiplication (on the right).
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa	CSIBCXfoMatd::Mul
			\sa CSIBCXfoMatd::MulTransByReg
		*/
		CSIBCXfoMatd& Mul( const CSIBCRotMatd& in_rotmat, const CSIBCXfoMatd& in_xfomat );

		
		/*! Computes the matrix multiplication of the transpose of \p in_mat1 and \p in_mat2
			(\p in_mat1^T * \p in_mat2) and stores the result in this matrix.
			\param	in_mat1	The first matrix in the multiplication (on the left), to be transposed 
					before multiplication.
			\param	in_mat2	The second matrix in the multiplication (on the right).
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa	CSIBCXfoMatd::Mul
			\sa CSIBCXfoMatd::MulRegByTrans
			\sa CSIBCXfoMatd::Div
		*/
		CSIBCXfoMatd& MulTransByReg( const CSIBCRotMatd& in_rotmat,
							const CSIBCXfoMatd& in_xfomat );

		/*! Computes the determinant of this matrix.
			\return	double	The determinant of this matrix.
			\sa CSIBCXfoMatd::GetDet33
			\sa CSIBCXfoMatd::GetTrace
		*/
		double GetDet( void ) const;

		/*! Computes the determines of the top-left 3x3 submatrix of this matrix.
			\return	double	The determinant of the top-left 3x3 submatrix.
			\sa CSIBCXfoMatd::GetDet
			\sa CSIBCXfoMatd::GetTrace
		*/
		double GetDet33( void ) const;

		/*! Computes the trace of this matrix. A trace is the sum of all the diagonal elements of a matrix.
			\return	double	The trace of this matrix.
			\sa CSIBCXfoMatd::GetDet
			\sa CSIBCXfoMatd::GetDet33
		*/
		double GetTrace( void ) const;

		/*! Sets this matrix to be the transpose of \p in_mat.
			\param	in_mat	Matrix containing the transpose of the desired matrix.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Transpose()
		*/
		CSIBCXfoMatd& Transpose( const CSIBCXfoMatd& in_mat );

		/*! Sets this matrix to its transpose.
			\return CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::Transpose( const CSIBCXfoMatd& )
		*/
		CSIBCXfoMatd& Transpose( void );

		/*! Sets this matrix to be the inverse of \p in_mat.
			\param	in_mat	Matrix containing the inverse of the desired matrix.
			\return	bool	true if \p in_mat could be inverted and the assignment was successful,
					false otherwise.
			\sa CSIBCXfoMatd::Invert()
		*/
		bool Invert( const CSIBCXfoMatd& in_mat );

		/*! Sets this matrix to its inverse.
			\return	bool	true if this matrix could be inverted and the assignment was successful,
					false otherwise.
			\sa CSIBCXfoMatd::Invert( const CSIBCXfoMatd& )
		*/
		bool Invert( void );

		/*!	Gets the translation contained in this matrix.
			\param	out_vctTrs	Receives the translation in the X, Y and Z directions.
			\return	CSIBCVector3Dd&	Reference to \p out_vctTrs.
			\sa CSIBCXfoMatd::SetTranslation
			\sa CSIBCXfoMatd::AddTranslation
		*/
		CSIBCVector3Dd& GetTranslation( CSIBCVector3Dd& out_vctTrs ) const;

		/*!	Sets the translation contained in this matrix.
			\param	in_vctTrs	Vector containing the translations in the X, Y and Z directions.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::GetTranslation
			\sa CSIBCXfoMatd::AddTranslation
		*/
		CSIBCXfoMatd& SetTranslation( const CSIBCVector3Dd& in_vctTrs );

		/*!	Adds a translation to the translation in this matrix.
			\param	in_vctTrs	Vector containing the translations to add in the X, Y and Z directions.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::GetTranslation
			\sa CSIBCXfoMatd::SetTranslation
		*/
		CSIBCXfoMatd& AddTranslation( const CSIBCVector3Dd& in_vctTrs );

		/*!	Gets the rotation matrix contained in this matrix as a rotation matrix.
			\param	out_matRot	Receives the rotation matrix.
			\return	CSIBCVector3Dd&	Reference to \p out_matRot.
			\sa CSIBCXfoMatd::GetRotation
			\sa CSIBCXfoMatd::SetRotation
			\sa CSIBCXfoMatd::AddRotation
		*/
		CSIBCRotMatd& GetRotation( CSIBCRotMatd& out_matRot ) const;

		/*!	Gets the rotation matrix contained in this matrix as a rotation object.
			\param	out_rot	Receives the rotation object.
			\return	CSIBCVector3Dd&	Reference to \p out_rot.
			\sa CSIBCXfoMatd::GetRotation
			\sa CSIBCXfoMatd::SetRotation
			\sa CSIBCXfoMatd::AddRotation
		*/
		CSIBCRotationd& GetRotation( CSIBCRotationd& out_rot ) const;

		/*! Sets the rotation contained in this matrix.
			\param	in_matRot	Rotation matrix containing the rotation to set for this matrix.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::GetRotation
			\sa CSIBCXfoMatd::AddRotation
		*/
		CSIBCXfoMatd& SetRotation( const CSIBCRotMatd& in_matRot );

		/*!	Adds a rotation to the rotation in this matrix.
			\param	in_matRot	Rotation matrix containing the rotation to add.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::GetRotation
			\sa CSIBCXfoMatd::SetRotation
		*/
		CSIBCXfoMatd& AddRotation( const CSIBCRotMatd& in_matRot );

		/*! Gets the scaling of this matrix.
			\param	out_vctScl	Receives the scaling components in the X, Y and Z directions.
			\return	CSIBCVector3Dd&	Reference to \p out_vctScl.
			\sa CSIBCXfoMatd::SetScaling
			\sa CSIBCXfoMatd::AddScaling
		*/	
		CSIBCVector3Dd& GetScaling( CSIBCVector3Dd& out_vctScl ) const;

		/*! Sets the scaling of this matrix.
			\param	in_vctScl		Vector containing the scaling in the X, Y and Z directions.
			\return	CSIBCXfoMatd&	Reference to this matrix.
			\sa CSIBCXfoMatd::GetScaling
			\sa CSIBCXfoMatd::AddScaling
		*/
		CSIBCXfoMatd& SetScaling( const CSIBCVector3Dd& in_vctScl );

		/*! Adds scaling to this matrix.
			\param	in_vctScl		Vector containing the scaling to add in the X, Y and Z directions.
			\param	in_bPreScaling	Whether the scaling operations should be applied before or after
					the translation in this matrix. if true scaling operations are applied before,
					false if scaling operations are to be applied after.
			\return	CSIBCXfoMatd&	Reference to this matirx.
			\sa CSIBCXfoMatd::GetScaling
			\sa CSIBCXfoMatd::SetScaling
		*/
		CSIBCXfoMatd& AddScaling( const CSIBCVector3Dd& in_vctScl, bool in_bPreScaling = true );

		/*!	Sets the scaling and rotation for this transformation matrix. Note that it is faster to
			use this method, than a combination of CSIBCXfoMatd::SetScaling, and CSIBCXfoMatd::SetRotation.
			\param	in_vctScl	Vector containing the scaling in the X, Y and Z directions.
			\param	in_rot		Rotation object containing the rotation for this matrix.
			\sa CSIBCXfoMatd::SetScaling
			\sa CSIBCXfoMatd::SetRotation
			\sa CSIBCXfoMatd::GetScalingAndRotation
		*/
		void SetScalingAndRotation( const CSIBCVector3Dd& in_vctScl, CSIBCRotationd& in_rot );

		/*! Gets the scaling and rotation for this transformation matrix. Note that it is faster to
			use this method, than a combination of CSIBCXfoMatd::GetScaling, and CSIBCXfoMatd::GetRotation.
			\param	out_vct		Receives the scaling components in the X, Y and Z directions.
			\param	out_rot		Receives the rotation object for this matrix.
			\sa CSIBCXfoMatd::GetScaling
			\sa CSIBCXfoMatd::GetRotation
			\sa CSIBCXfoMatd::SetScalingAndRotation
		*/
		void GetScalingAndRotation( CSIBCVector3Dd& out_vctScl, CSIBCRotationd& out_rot ) const;

		/*! Gets the affine tranformation represented by this transformation matrix.
			\param	out_sclRot	Receives the affine scaling/rotation matrix associated with this 
					transformation matrix.
			\param	out_matSRT	Receives the affine transformation matrix associated with this
					transformation matrix.
		*/
		void GetAffineDecomposition ( CSIBCRotMatd& out_sclRot, CSIBCXfoMatd& out_matSRT ) const;

	// Protected members
	protected:

		// If you have to reserve other bits in CSIBCMatrix44d::m_MaskInfo for the specific usage of CSIBCMatrix44d
		// please update the value of _3D_NB_BITS_RESERVED_BY_XFOMAT accordingly.
		enum { 
			_3D_X_NEGATIVE				= (unsigned char)( 1 << (_3D_NB_BITS_RESERVED_BY_MAT44 + 0) ),
			_3D_Y_NEGATIVE				= (unsigned char)( 1 << (_3D_NB_BITS_RESERVED_BY_MAT44 + 1) ),
			_3D_Z_NEGATIVE				= (unsigned char)( 1 << (_3D_NB_BITS_RESERVED_BY_MAT44 + 2) ),
			_3D_ALLAXES_NEGATIVE_MASK	= (unsigned char)( _3D_X_NEGATIVE | _3D_Y_NEGATIVE | _3D_Z_NEGATIVE ),
		};

	// Private members
	private:

		// Please do not re-enable the following cast operator as it will cause problems for the following Scenario:
		// CSIBCMatrix44d l_mat;
		// CSIBCVector3Dd l_vct;
		// l_vct.Mul( l_vct, l_mat );
		// there are two Mul functions applicable:
		// 1-   CSIBCVector3Dd & CSIBCVector3Dd::Mul( const CSIBCVector3Dd &, const CSIBCMatrix44d & );
		// 2-   CSIBCVector3Dd & CSIBCVector3Dd::Mul( const CSIBCVector3Dd &, const CSIBCXfoMatd & );
		// Unfortunately, if the following operator is enabled, the second Mul function will be taken
		// which is not what the client wants !!!
		// EC.
		CSIBCXfoMatd( const CSIBCMatrix44d & ); // Disable this cast operator
		CSIBCXfoMatd( const CSIBCMatrix33d& in_mat );

		//Disable copy constructor and operator =
		CSIBCXfoMatd( const CSIBCXfoMatd& in_mat );
		CSIBCXfoMatd& operator =( const CSIBCXfoMatd& in_mat );

		//Methods to handles SignBits for the scaling factor.
		void SetSclSignBits( const CSIBCVector3Dd &in_vctScl );	// SetSclSignBits | Set m_MaskInfo from in_vctScl.
		void XORSclSignBits( const CSIBCVector3Dd &in_vctScl );	// XORSclSignBits | XOR m_MaskInfo from in_vctScl.

		void SetSclSignBits( const unsigned char in_SclBits );	// SetSclSignBits | Set the sign bits of m_MaskInfo to those of in_SclBits.
		void XORSclSignBits( const unsigned char in_Bits1, const unsigned char in_Bits2 );// XORSclSignBits | Return the XOR of in_Mask1 and in_Mask2 masked with _3D_ALLAXES_NEGATIVE_MASK.
		unsigned char GetSclSignBits( ) const;				// GetSclSignBits | Return the sign bits sub-part of m_MaskInfo.
		void ClearSclSignBits();							// ClearSclSignBits | Clear the bit signs part of m_MaskInfo
		bool IsSclAxisNegative( int in_axe ) const;			// IsSclAxisNegative | Look if one scl bit is set.
};
typedef CSIBCXfoMatd* LP3DXFOMAT;
typedef const CSIBCXfoMatd* LPC3DXFOMAT;

#endif
