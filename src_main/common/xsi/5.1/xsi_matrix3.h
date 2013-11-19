//*****************************************************************************
/*!
   \file xsi_matrix3.h
   \brief Matrix3 class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIMATRIX3_H__
#define __XSIMATRIX3_H__

#include "sicppsdk.h"


namespace XSI {
namespace MATH {

class CVector3;
class CQuaternion;

//*****************************************************************************
/*! \class CMatrix3 xsi_matrix3.h
	\brief This class represents a double precision floating point 3 by 3 matrix.

	\eg
	\code
		using namespace XSI::MATH;
		CMatrix3 mat3(1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0);
		mat3.TransposeInPlace();
		Application app;
		app.LogMessage(CString(L"The transposed matrix is ") +
					   CValue(mat3.GetValue(0,0)).GetAsText() + CString(L", ")+
					   CValue(mat3.GetValue(0,1)).GetAsText() + CString(L", ")+
					   CValue(mat3.GetValue(0,2)).GetAsText() + CString(L", ")+
					   CValue(mat3.GetValue(1,0)).GetAsText() + CString(L", ")+
					   CValue(mat3.GetValue(1,1)).GetAsText() + CString(L", ")+
					   CValue(mat3.GetValue(1,2)).GetAsText() + CString(L", ")+
					   CValue(mat3.GetValue(2,0)).GetAsText() + CString(L", ")+
					   CValue(mat3.GetValue(2,1)).GetAsText() + CString(L", ")+
					   CValue(mat3.GetValue(2,2)).GetAsText());
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL CMatrix3
{
public:

	/*! Default constructor. */
	CMatrix3();

	/*! Constructor.
	\param in_d00 Value to set in this matrix[0,0].
	\param in_d01 Value to set in this matrix[0,1].
	\param in_d02 Value to set in this matrix[0,2].
	\param in_d10 Value to set in this matrix[1,0].
	\param in_d11 Value to set in this matrix[1,1].
	\param in_d12 Value to set in this matrix[1,2].
	\param in_d20 Value to set in this matrix[2,0].
	\param in_d21 Value to set in this matrix[2,1].
	\param in_d22 Value to set in this matrix[2,2].
	*/
	CMatrix3(double in_d00, double in_d01, double in_d02,
			 double in_d10, double in_d11, double in_d12,
			 double in_d20, double in_d21, double in_d22);

	/*! Copy constructor.
	\param in_matrix3 constant class object.
	*/
	CMatrix3( const CMatrix3& in_matrix3);

	/*! Default destructor. */
	~CMatrix3();

	/*! Assignment operator.
		\param in_matrix3& constant class object.
	*/
	CMatrix3  & operator=( const CMatrix3 & in_matrix3 );


	/*! Right-multiplies this CMatrix3 object by the matrix m and stores the result in the same CMatrix3 object (this = this . in_matrix3)
	\param in_matrix3 matrix to be multiplied with.
	\return A reference to this CMatrix3 object.
	\sa CMatrix3::MulInPlace
	 */
	CMatrix3 & operator*=(const CMatrix3 & in_matrix3 );

	/*! Accesses a row of this CMatrix3 object.
	\param in_sIndex  0,1,2 for the row of the matrix.
	\return A reference to this CVector3 object.
	\sa CVector3::operator[]
	 */
	CVector3& operator[](const short& in_sIndex);

	/*! Sets this CMatrix3 object to the identity matrix (this = id).
	\return A reference to this CMatrix3 object.
	 */
	CMatrix3&	SetIdentity();

	/*! Inverts this CMatrix3 object (if not singular) (this = this^-1).
	\return true if this CMatrix3 object has been inverted (not singular), else false.
	 */
	bool		InvertInPlace();

	/*! Inverts this CMatrix3 object (if not singular) (this = this^-1).
	\param in_matrix3 matrix to be inverted.
	\return true if this CMatrix3 object has been inverted (not singular), else false.
	 */
	bool		Invert(const CMatrix3& in_matrix3);

	/*! Sets this CMatrix3 object to the transpose of the inverse of itself (if not singular) (this = Transpose(this^-1)).
	\return true if this CMatrix3 object has been inverted (not singular), else false.
	 */
	bool		TransposeInverseInPlace();

	/*! Sets this CMatrix3 object to the transpose of the inverse of the input matrix (if not singular)
	(this = Transpose(in_matrix3^-1)).
	\param in_matrix3 CMatrix3 object to be inversed and transposed.
	\return true if this CMatrix3 object has been inverted (not singular), else false.
	 */
	bool		TransposeInverse(const CMatrix3& in_matrix3);

	/*! Transposes this CMatrix3 object in place (this = Transpose(this)).
	 */
	void		TransposeInPlace();

	/*! Transposes the specified matrix and stores the result in this CMatrix3 object (this = Transpose(in_matrix3)).
	\param in_matrix3 matrix to be inversed and transposed.
	 */
	void		Transpose(const CMatrix3& in_matrix3);

	/*! Right-multiplies this CMatrix3 object by the matrix m and stores the result in this CMatrix3 object
	(this = this . in_matrix3).
	\param in_matrix3 matrix to be multiplied with.
	\return A reference to the CMatrix3 object.
	 */
	CMatrix3&		MulInPlace(const CMatrix3& in_matrix3);

	/*! Right-multiplies the matrix A by the matrix B and stores the result in this CMatrix3 object
	(this = in_matrix3A . in_matrix3B).
	\param in_matrix3A matrix
	\param in_matrix3B matrix
	\return CMatrix3& A reference to the CMatrix3 object which contains the result of the multiplication.
	 */
	CMatrix3&		Mul(const CMatrix3& in_matrix3A, const CMatrix3& in_matrix3B);

	/*! Returns the 9 components of this CMatrix3 object.
	\param io_d00 Value of this matrix[0,0].
	\param io_d01 Value of this matrix[0,1].
	\param io_d02 Value of this matrix[0,2].
	\param io_d10 Value of this matrix[1,0].
	\param io_d11 Value of this matrix[1,1].
	\param io_d12 Value of this matrix[1,2].
	\param io_d20 Value of this matrix[2,0].
	\param io_d21 Value of this matrix[2,1].
	\param io_d22 Value of this matrix[2,2].
	 */
	void	Get(	double& io_d00, double& io_d01, double& io_d02,
					double& io_d10, double& io_d11, double& io_d12,
					double& io_d20, double& io_d21, double& io_d22) const;


	/*! Sets the 9 components of this CMatrix3 object.
	\param in_d00 Value to set to this matrix[0,0].
	\param in_d01 Value to set to this matrix[0,1].
	\param in_d02 Value to set to this matrix[0,2].
	\param in_d10 Value to set to this matrix[1,0].
	\param in_d11 Value to set to this matrix[1,1].
	\param in_d12 Value to set to this matrix[1,2].
	\param in_d20 Value to set to this matrix[2,0].
	\param in_d21 Value to set to this matrix[2,1].
	\param in_d22 Value to set to this matrix[2,2].
	 */
	void	Set(	double in_d00, double in_d01, double in_d02,
					double in_d10, double in_d11, double in_d12,
					double in_d20, double in_d21, double in_d22);

	/*! Sets the 9 components of this CMatrix3 object.
	\param in_dVal A 3x3 array of double values.
	 */
	void	Set(const double in_dVal[3][3]);

	/*! Returns the (i,j)th value of this CMatrix3 object.
	\param in_sRow access to the i th row
	\param in_sCol access to the j th column
	\return The (i,j)th value of this CMatrix3 object.
	 */
	double		GetValue(short in_sRow,short in_sCol )const;

	/*! Sets the (i,j)th value of this CMatrix3 object.
	\param in_sRow access to the i th row
	\param in_sCol access to the j th column
	\param in_dVal new value
	 */
	void		SetValue(short in_sRow, short in_sCol, double in_dVal );

	/*! Sets the CMatrix3 object using rotation CQuaternion.
	\param in_quaternion The CQuaternion containing the new matrix3 values
	\return The new CMatrix3 object.
	*/
	CMatrix3& SetFromQuaternion( const CQuaternion& in_quaternion );

	/*! Gets the rotation CQuaternion representation of this matrix3.
	\return Rotation CQuaternion
	*/
	CQuaternion GetQuaternion(void)const;

	/*! Tests the equality of this CMatrix3 object with the specified matrix, with a tolerance of Epsilon.
	\param in_matrix3 Operand matrix.
	\param in_dEpsilon Error margin Possible Values: [0, +INF[ should be a positive value
	\return true if equal else false.
	 */
	bool EpsilonEquals
	(
		const CMatrix3& in_matrix3,
		double in_dEpsilon
	) const;

	/*! Tests the strict equality of this CMatrix3 object with the specified matrix.
	\param in_matrix3 Operand matrix.
	\return true if equal else false.
	 */
	bool Equals(const CMatrix3& in_matrix3) const;
	/*! Equality operator. Tests the strict equality of this CMatrix3 object with the specified matrix.
	\param in_matrix3 Operand matrix3.
	\return true if equal else false.
	\sa CMatrix3::Equals
	 */
	bool operator ==(const CMatrix3 & in_matrix3 ) const;

	/*! Inequality operator. Tests the strict inequality of this CMatrix3 object with the specified matrix.
	\param in_matrix3 Operand matrix.
	\return true if equal else false.
	\sa CMatrix3::Equals
	 */
	bool operator !=(const CMatrix3 & in_matrix3 ) const;

private:
	double m_dMat[3][3];
};



};
};

#endif // __XSIMATRIX3_H__
