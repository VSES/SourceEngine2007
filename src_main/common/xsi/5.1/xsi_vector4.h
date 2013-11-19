//*****************************************************************************
/*!
   \file xsi_vector4.h
   \brief CVector4 and CVector4Array class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIVECTOR4_H__
#define __XSIVECTOR4_H__

#include "sicppsdk.h"
#include "xsi_status.h"

namespace XSI {


namespace MATH {

class CTransformation;
class CMatrix4;

//*****************************************************************************
/*! \class CVector4 xsi_vector4.h
	\brief A 4-element vector that is represented by double precision floating point x,y,z,w coordinates.

	\sa MATH::CMatrix4, MATH::CVector4Array
 */
//*****************************************************************************
class SICPPSDKDECL CVector4
{
public:
	/*! Default constructor. */
	CVector4();

	/*! Constructor.
	\param in_dX Value of the x component
	\param in_dY Value of the y component
	\param in_dZ Value of the z component
	\param in_dW Value of the w component
	*/
	CVector4(double in_dX, double in_dY, double in_dZ, double in_dW);

	/*! Copy constructor.
	\param in_vector4 constant class object.
	*/
	CVector4( const CVector4& in_vector4);

	/*! Default destructor. */
	~CVector4();

	/*! Assignment operator.
	\param in_vector4 constant class object.
	\return A reference to this quaternion.
	*/
	CVector4 & operator=( const CVector4 & in_vector4 );

	/*! Equality operator tests the strict equality of this vector with the specified vector.
	\param in_vector4 Operand vector.
	\return true if equal else false.
	\sa CVector4::Equals
	*/
	bool operator ==(const CVector4 & in_vector4 )const;

	/*! Inequality operator tests the strict inequality of this vector with the specified vector.
	\param in_vector4 Operand vector.
	\return true if equal else false.
	\sa CVector4::Equals
	*/
	bool operator !=(const CVector4 & in_vector4 )const;

	/*! Negates this vector (this = - this).
	\return A reference to this vector.
	\sa CVector4::NegateInPlace
	*/
	CVector4& operator ~();

	/*! Adds the input vector to this one (this = this + in_vector4).
	\param in_vector4 Operand vector.
	\return A reference to this vector.
	\sa CVector4::AddInPlace
	*/
	CVector4& operator +=(const CVector4& in_vector4);

	/*! Subtracts the input vector4 (in_vector4) from this one (this = this - in_vector4).
	\param in_vector4 Operand vector.
	\return A reference to this vector.
	\sa CVector4::SubInPlace
	*/
	CVector4& operator -=(const CVector4 & in_vector4);


	/*! Right-multiplies this vector by the matrix m in place. (this = this . m )
	\param in_matrix4 Operand matrix.
	\return A reference to this vector.
	\sa CVector4::MulByMatrix4InPlace
	*/
	CVector4& operator *=(const CMatrix4& in_matrix4);

	/*! Right-multiplies this vector by the transformation t in place (this = this . t).
	\param in_transformation transformation operand.
	\return A reference to this vector.
	\sa CVector4::MulByTransformationInPlace
	*/
	CVector4& operator *=(const CTransformation& in_transformation);

	/*! Scales this vector by the input scalar (alpha) (this = alpha * this).
	\param in_dAlpha  Scalar multiplicator.
	\return A reference to this vector.
	\sa CVector4::ScaleInPlace
	*/
	CVector4& operator *=(const double& in_dAlpha);

	/*! Read-only accessors to the X,Y,Z,W components.
	\param in_sIndex  0,1,2,3 for X,Y,Z and W values.
	\return The value of the specified component. If the index supplied is out of range the function returns DBL_MAX.
	\sa CVector4::GetX(), CVector4::GetY(), CVector4::GetZ(), CVector4::GetW()
	*/
	double operator[](const short& in_sIndex);

	/*! Right-multiplies this vector by the matrix m in place (this = this . m ).
	\param in_matrix4 Operand matrix.
	\return A reference to this vector.
	*/
	CVector4& MulByMatrix4InPlace(const CMatrix4& in_matrix4);

	/*! Right-multiplies the vector v by the matrix m and store the result in this vector (this = v . m ).
	\param in_vector4 Operand vector
	\param in_matrix4 Operand matrix.
	\return A reference to this vector.
	*/
	CVector4& MulByMatrix4
	(
		const CVector4& in_vector4,
		const CMatrix4& in_matrix4
	);

	/*! Right-multiplies this vector by the transformation t in place (this = this . t).
	\param in_transformation transformation operand.
	\return A reference to this vector.
	*/
	CVector4& MulByTransformationInPlace
	(
		const CTransformation& in_transformation
	);

	/*! Right-multiplies the vector v by the transformation t and store the result in this vector (this = v . t ).
	\param in_vector4 transformation operand.
	\param in_transformation transformation operand.
	\return A reference to this vector.
	*/
	CVector4& MulByTransformation
	(
		const CVector4&			in_vector4,
		const CTransformation&	in_transformation
	);

	/*! Sets this vector to a null vector.
	\return A reference to this vector.
	*/
	CVector4& PutNull();

	/*! Returns the length of this vector.
	\return Length of this vector.
	*/
	double GetLength()const;

	/*! Returns the squared length of this vector.
	\return Squared length of this vector.
	*/
	double GetLengthSquared()const;

	/*! Normalizes this vector in place.
	\return CStatus::OK if the normalization has been computed,
	*/
	CStatus NormalizeInPlace();

	/*! Sets the value of this vector to the normalization of inputed vector.
	\param in_vector4 vector operand.
	\return CStatus::OK if the normalization has been computed.
	*/
	CStatus Normalize(const CVector4& in_vector4);

	/*! Returns the dot product of this vector and the inputed vector.
	\param in_vector4 vector operand.
	\return Dot product of this vector and vector in_vector4.
	*/
	double Dot(const CVector4& in_vector4)const;


	/*! Sets this vector to the vector cross product of vectors A and B.
	\param in_vector4A vector operand.
	\param in_vector4B vector operand.
	\return A reference to this vector.
	*/
	CVector4& Cross(const CVector4& in_vector4A, const CVector4& in_vector4B);

	/*! Sets each component of this vector to its absolute value.
	\return A reference to this vector.
	*/
	CVector4& AbsoluteInPlace();

	/*! Sets each component of the vector in_vector4 to its absolute value and places the modified values into this vector
	\return A reference to this vector.
	*/
	CVector4& Absolute(const CVector4& in_vector4);

	/*! Tests the equality of this vector with the vector in_vector3, with a tolerance of Epsilon.
	\param in_vector4 Operand vector.
	\param in_dEpsilon Error margin Possible Values: [0, +INF[ should be a positive value
	\return true if equal else false.
	*/
	bool EpsilonEquals
	(
		const CVector4& in_vector4,
		double in_dEpsilon
	)const;

	/*! Tests the strict equality of this vector with the vector in_vector3.
	\param in_vector4 Operand vector.
	\return true if equal else false.
	*/
	bool Equals(const CVector4& in_vector4)const;


	/*! Sets the value of this vector to the scalar multiplication of itself by s and then adds vector v (this = s*this + v).
	\param in_dS  Scalar multiplicator.
	\param in_vector4 Operand vector.
	\return A reference to this vector.
	*/
	CVector4& ScaleAddInPlace(double in_dS,const CVector4& in_vector4);

	/*! Sets the value of this vector to the scalar multiplication of vector A by s and then adds vector B (this = s*v1 + v2).
	\param in_dS  Scalar multiplicator.
	\param in_vector4A Operand vector.
	\param in_vector4B Operand vector.
	\return A reference to this vector.
	*/
	CVector4& ScaleAdd
	(
		double			in_dS,
		const CVector4& in_vector4A,
		const CVector4&	in_vector4B
	);

	/*! Scales this vector by the input scalar (alpha) (this = alpha * this).
	\param in_dAlpha  Scalar multiplicator.
	\return A reference to this vector.
	*/
	CVector4& ScaleInPlace(double in_dAlpha);

	/*! Scales the input vector (v) by (alpha) store the result into this one (this = alpha * v).
	\param in_dAlpha  Scalar multiplicator.
	\param in_vector4 Operand vector.
	\return A reference to this vector.
	*/
	CVector4& Scale(double in_dAlpha, const CVector4& in_vector4);

	/*! Negates this vector (this = - this).
	\return A reference to this vector.
	*/
	CVector4& NegateInPlace();

	/*! Negates the input vector (v) store the result into this one (this = - v).
	\param in_vector4 Operand vector.
	\return A reference to this vector.
	*/
	CVector4& Negate(const CVector4& in_vector4);

	/*! Subtracts the input vector (v) from this one (this = this - v).
	\param in_vector4 Operand vector.
	\return A reference to this vector.
	*/
	CVector4& SubInPlace(const CVector4& in_vector4);

	/*! Subtracts the input vector (A) from the input vector (B) and stores the result into this one (this = A - B).
	\param in_vector4A Operand vector.
	\param in_vector4B Operand vector.
	\return A reference to this vector.
	*/
	CVector4& Sub(const CVector4& in_vector4A, const CVector4& in_vector4B);

	/*! Adds the input vector to this one (this = this + in_vector3).
	\param in_vector4 Operand vector.
	\return A reference to this vector.
	*/
	CVector4& AddInPlace(const CVector4& in_vector4);

	/*! Adds both input vectors and store the result in this one (this = A + B).
	\param in_vector4A Operand vector.
	\param in_vector4B Operand vector.
	\return A reference to this vector.
	*/
	CVector4& Add(const CVector4& in_vector4A,const CVector4& in_vector4B);

	/*! Returns the X, Y, Z and W values of the vector.
	\retval out_dX X value to get in the vector.
	\retval out_dY Y value to get in the vector.
	\retval out_dZ Z value to get in the vector.
	\retval out_dW W value to get in the vector.
	*/
	void Get(	double& out_dX, double& out_dY,
				double& out_dZ, double& out_dW )const;

	/*! Sets the X, Y, Z and W values of the vector.
	\param in_dX X value to set in the vector.
	\param in_dY Y value to set in the vector.
	\param in_dZ Z value to set in the vector.
	\param in_dW W value to set in the vector.
	\return A reference to the vector.
	*/
	CVector4& Set(double in_dX, double in_dY, double in_dZ, double in_dW);

	/*! Returns the W value of this vector.
	\return W value of this vector.
	*/
	double GetW()const;

	/*! Sets the W value of this vector.
	\param in_dW z value.
	\return A reference to this vector.
	*/
	CVector4& PutW(double in_dW);


	/*! Returns the Z value of this vector.
	\return Z value of this vector.
	*/
	double GetZ()const;

	/*! Sets the Z value of this vector.
	\param in_dZ z value.
	\return A reference to this vector.
	*/
	CVector4& PutZ(double in_dZ);

	/*! Returns the Y value of this vector.
	\return Y value of this vector.
	*/
	double GetY()const;

	/*! Sets the Y value of this vector.
	\param in_dY y value.
	\return A reference to this vector.
	*/
	CVector4& PutY(double in_dY);

	/*! Returns the X value of this vector.
	\return X value of this vector.
	*/
	double GetX()const;

	/*! Sets the X value of this vector.
	\param in_dX x value.
	\return A reference to this vector.
	*/
	CVector4& PutX(double in_dX);

private:
	double m_dX,m_dY,m_dZ,m_dW;
};

//*************************************************************************
/*! \class CVector4Array xsi_vector3.h
	\brief The CVector4Array is an uni-dimensional array of CVector4.

	The array is zero-based, which means that the first item index is 0 and the larger index is one less that the
	number of elements contained in the array.

	\sa MATH::CVector4, CControlPointRefArray::GetArray, CControlPointRefArray::PutArray,
		CNurbsCurveData.m_aControlPoints, CNurbsSurfaceData.m_aControlPoints
*/
//*************************************************************************
class SICPPSDKDECL CVector4Array
{
public:
	/*! Constructs a CVector4Array and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CVector4Array( LONG in_size=0);

	/*! Constructs a CVector4Array object from another CVector4Array object.
	\param in_ArrayToCopy constant CVector4Array object.
	*/
	CVector4Array(const CVector4Array& in_ArrayToCopy);

	/*! Default destructor. */
	~CVector4Array();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CVector4Array& operator=( const CVector4Array& in_array );

	/*! Returns the number of items in this CVector4Array
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a LONG at the end of this array.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CVector4& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if new new size is larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be called by constant objects, the returned
	value is read-only.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero and smaller
		than the number of elements in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CVector4& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero and smaller
		than the number of elements in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	CVector4& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CVector4Array to compare with.
	\return true if in_array contains the same number of elements as this array and if all members of
		in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CVector4Array& in_array) const;

	/*! Inequality operator.
	\param in_array
	\return true if one member of in_array is different from the corresponding members in this array
		or if the arrays are not of the same size.
	*/
	bool operator!=(const CVector4Array& in_array) const;

private:

	void* m_ptr;
};

};
};

#endif // __XSIVECTOR4_H__
