//*****************************************************************************
/*!
   \file xsi_vector3.h
   \brief CVector3 and CVector3Array class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIVECTOR3_H__
#define __XSIVECTOR3_H__

#include "sicppsdk.h"
#include "xsi_status.h"


namespace XSI {

namespace MATH {

class CTransformation;
class CMatrix3;
class CMatrix4;

//*****************************************************************************
/*! \class CVector3 xsi_vector3.h
	\brief A 3-element vector that is represented by double precision floating point x,y,z coordinates.

	In this API vectors are considered as row vectors as described in [Foley, J., and A. van Dam,
	Fundamentals of Interactive Computer Graphics, Addison-Wesley, Reading, MA, 1982]. This is
	important when you multiply a CVector3 by a CMatrix3 or a CMatrix4.

	\sa CMatrix3, CMatrix4, CTransformation

  	\eg
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"",myCube );

		KinematicState  globalKinematicState = myCube.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posy", 4.0);
		globalKinematicState.PutParameterValue(L"posx", 2.0);

		X3DObject myCube2;
		myCube.AddGeometry( L"Cube", L"MeshSurface",L"",myCube2);

		globalKinematicState = myCube2.GetKinematics().GetGlobal();
		globalKinematicState.PutParameterValue(L"posz",3.0);
		globalKinematicState.PutParameterValue(L"rotx",45.0);

		CTransformation localTransformation =
								myCube.GetKinematics().GetLocal().GetTransform();
		CTransformation globalTransformation = globalKinematicState.GetTransform();

		CVector3 translation(localTransformation.GetTranslation());

		app.LogMessage(L"The translation of the cube relative to its parent: x " +
						CValue(translation.GetX()).GetAsText() +
						L" y " +
						CValue(translation.GetY()).GetAsText() +
						L" z " +
						CValue(translation.GetZ()).GetAsText());

		MapObjectPoseToWorldSpace(	globalTransformation, localTransformation);

		translation = localTransformation.GetTranslation();

		app.LogMessage(L"The translation of the cube relative to the origin of the universe: x " +
					  CValue(translation.GetX()).GetAsText() +
					  L" y " + CValue(translation.GetY()).GetAsText() +
					  L" z " + CValue(translation.GetZ()).GetAsText());
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL CVector3
{
public:
	/*! Default constructor. */
	CVector3();

	/*! Constructor.
	\param in_dX Value of the x component
	\param in_dY Value of the y component
	\param in_dZ Value of the z component
	*/
	CVector3(double in_dX, double in_dY, double in_dZ);

	/*! Copy constructor.
	\param in_vector3 constant class object.
	*/
	CVector3( const CVector3& in_vector3);

	/*! Default destructor. */
	~CVector3();

	/*! Assignment operator.
	\param in_vector3 constant class object.
	\return A reference to this vector.
	*/
	CVector3 & operator=( const CVector3 & in_vector3 );

	/*! Equality operator tests the strict equality of this CVector3 with the specified vector3.
	\param in_vector3 Operand vector.
	\return true if equal else false.
	\sa CVector3::Equals
	*/
	bool operator ==(const CVector3 & in_vector3 )const;

	/*! Inequality operator tests the strict inequality of this CVector3 with the specified vector3.
	\param in_vector3 Operand vector.
	\return true if equal else false.
	\sa CVector3::Equals
	*/
	bool operator !=(const CVector3 & in_vector3 )const;

	/*! Negates this vector (this = - this).
	\return A reference to this vector.
	\sa CVector3::NegateInPlace
	*/
	CVector3& operator ~();

	/*! Adds the input vector to this one (this = this + in_vector3)
	\param in_vector3 Operand vector..
	\return A reference to this vector.
	\sa CVector3::AddInPlace
	*/
	CVector3& operator +=(const CVector3& in_vector3);

	/*! Subtracts the input vector (in_vector3) from this one (this = this - in_vector3).
	\param in_vector3 Operand vector.
	\return A reference to this vector.
	\sa CVector3::SubInPlace
	*/
	CVector3& operator -=(const CVector3 & in_vector3);


	/*! Right-multiplies this vector by the matrix m in place (this = this . m ).
	\param in_matrix3 Operand matrix.
	\return A reference to this vector.
	\sa CVector3::MulByMatrix3InPlace
	*/
	CVector3& operator *=(const CMatrix3& in_matrix3);

	/*! Right-multiplies this vector by the matrix m in place (this = this . m ).
	\param in_matrix4 Operand matrix.
	\return A reference to this vector.
	\sa CVector3::MulByMatrix4InPlace
	*/
	CVector3& operator *=(const CMatrix4& in_matrix4);

	/*! Right-multiplies this vector by the transformation t in place (this = this . t).
	\param in_transformation transformation operand.
	\return A reference to this vector.
	\sa CVector3::MulByTransformationInPlace
	*/
	CVector3& operator *=(const CTransformation& in_transformation);

	/*! Scales this vector by the input scalar (alpha) (this = alpha * this).
	\param in_dAlpha  Scalar multiplicator.
	\return A reference to this vector.
	\sa CVector3::ScaleInPlace
	*/
	CVector3& operator *=(const double& in_dAlpha);

	/*! Read-only accessors to the X,Y,Z components.
	\param in_sIndex  0,1,2 for X,Y and Z values.
	\return The value of the specified component. If the index supplied is out of range the function returns DBL_MAX.
	\sa CVector3::GetX(), CVector3::GetY(), CVector3::GetZ()
	*/
	double operator[](const short& in_sIndex);

	/*! Right-multiplies this vector by the matrix m in place (this = this . m ).
	\param in_matrix3 Operand matrix.
	\return A reference to this vector.
	*/
	CVector3& MulByMatrix3InPlace(const CMatrix3& in_matrix3);

	/*! Right-multiplies the vector v by the matrix m and store the result in this vector (this = v . m ).
	\param in_vector3 Operand vector
	\param in_matrix3 Operand matrix.
	\return A reference to this vector.
	*/
	CVector3& MulByMatrix3
	(
		const CVector3& in_vector3,
		const CMatrix3& in_matrix3
	);

	/*! Right-multiplies this vector by the matrix m in place (this = this . m ).
	\param in_matrix4 Operand matrix.
	\return A reference to this vector.
	*/
	CVector3& MulByMatrix4InPlace(const CMatrix4& in_matrix4);

	/*! Right-multiplies the vector v by the matrix m and store the result in this vector (this = v . m ).
	\param in_vector3 Operand vector
	\param in_matrix4 Operand matrix.
	\return A reference to this vector.
	*/
	CVector3& MulByMatrix4
	(
		const CVector3& in_vector3,
		const CMatrix4& in_matrix4
	);

	/*! Right-multiplies this vector by the transformation t in place (this = this . t).
	\param in_transformation transformation operand.
	\return A reference to this vector.
	*/
	CVector3& MulByTransformationInPlace
	(
		const CTransformation& in_transformation
	);

	/*! Right-multiplies the vector v by the transformation t and stores the result in this vector (this = v . t ).
	\param in_vector3 transformation operand.
	\param in_transformation transformation operand.
	\return A reference to this vector.
	*/
	CVector3& MulByTransformation
	(
		const CVector3&			in_vector3,
		const CTransformation&	in_transformation
	);

	/*! Sets this vector to a null vector.
	\return A reference to this vector.
	*/
	CVector3& PutNull();

	/*! Returns the index of the maximum component of this vector.
	\return Index of the maximum component of this vector.
	*/
	short GetMaxComponentIndex()const;

	/*! Returns the maximum component value of this vector.
	\return Maximum component value of this vector.
	*/
	double GetMaxComponent()const;

	/*! Returns the index of the minimum component of this vector.
	\return Index of the minimum component of this vector.
	*/
	short GetMinComponentIndex()const;

	/*! Returns the minimum component value of this vector.
	\return Minimum component value of this vector.
	*/
	double GetMinComponent()const;

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
	\param in_vector3 vector operand.
	\return CStatus::OK if the normalization has been computed.
	*/
	CStatus Normalize(const CVector3& in_vector3);

	/*! Returns the dot product of this vector and the inputed vector.
	\param in_vector3 vector operand.
	\return Dot product of this vector and vector in_vector3.
	*/
	double Dot(const CVector3& in_vector3)const;


	/*! Sets this vector to the vector cross product of vectors A and B.
	\param in_vector3A vector operand.
	\param in_vector3B vector operand.
	\return A reference to this vector.
	*/
	CVector3& Cross(const CVector3& in_vector3A, const CVector3& in_vector3B);

	/*! Returns the angle (in radians) between this vector and the inputed vector.
	\param in_vector3 vector operand.
	\return Angle (in radians) between this vector and the inputed vector.
	*/
	double GetAngle(const CVector3& in_vector3 )const;

	/*! Linearly interpolates between vectors A and B and places the result into this vector:
	this = (1-alpha)*A + alpha*B.
	\param in_vector3A vector operand.
	\param in_vector3B vector operand.
	\param in_dAlpha Scalar interpolator \n
	\b Possible \b Values: [0.0, 1.0] Domain of validity
	\return A reference to this vector.
	*/
	CVector3& LinearlyInterpolate
	(
		const CVector3& in_vector3A,
		const CVector3& in_vector3B,
		double in_dAlpha
	);

	/*! Sets each component of this vector3 to its absolute value.
	\return A reference to this vector.
	*/
	CVector3& AbsoluteInPlace();

	/*! Sets each component of the specified vector3 to its absolute value and places the
	modified values into this vector.
	\return A reference to this vector.
	*/
	CVector3& Absolute(const CVector3& in_vector3);

	/*! Clamps this vector3's components to the range [MinValue, MaxValue].
	\param in_dMinValue Lower bound of the clamping domain.
	\param in_dMaxValue Higher bound of the clamping domain.
	\return A reference to this vector.
	*/
	CVector3& ClampInPlace(double in_dMinValue, double in_dMaxValue);

	/*! Clamps the specified vector3's components to the range [MinValue, MaxValue] and places the
	values into this vector3.
	\param in_dMinValue Lower bound of the clamping domain.
	\param in_dMaxValue Higher bound of the clamping domain.
	\param in_vector3 Operand vector.
	\return A reference to this vector.
	*/
	CVector3& Clamp
	(
		double			in_dMinValue,
		double			in_dMaxValue,
		const CVector3& in_vector3
	);

	/*! Tests the equality of this vector3 with the specified vector3, with a tolerance of Epsilon.
	\param in_vector3 Operand vector.
	\param in_dEpsilon Error margin \n
	\b Possible \b Values: [0, +INF[ should be a positive value
	\return true if equal else false.
	*/
	bool EpsilonEquals
	(
		const CVector3& in_vector3,
		double in_dEpsilon
	)const;

	/*! Tests the strict equality of this vector3 with the specified vector3.
	\param in_vector3 Operand vector.
	\return true if equal else false.
	*/
	bool Equals(const CVector3& in_vector3)const;


	/*! Sets the value of this vector to the scalar multiplication of itself by s and then adds
	vector3 v. (this = s*this + v)
	\param in_dS  Scalar multiplicator.
	\param in_vector3 Operand vector.
	\return A reference to this vector.
	*/
	CVector3& ScaleAddInPlace(double in_dS,const CVector3& in_vector3);

	/*! Sets the value of this vector3 to the scalar multiplication of vector3 A by s and then
	adds vector3 B. (this = s*v1 + v2).
	\param in_dS  Scalar multiplicator.
	\param in_vector3A Operand vector.
	\param in_vector3B Operand vector.
	\return A reference to this vector.
	*/
	CVector3& ScaleAdd
	(
		double			in_dS,
		const CVector3& in_vector3A,
		const CVector3&	in_vector3B
	);

	/*! Scales this vector by the input scalar (alpha) (this = alpha * this).
	\param in_dAlpha  Scalar multiplicator.
	\return A reference to this vector.
	*/
	CVector3& ScaleInPlace(double in_dAlpha);

	/*! Scales the input vector (v) by (alpha) store the result into this one (this = alpha * v).
	\param in_dAlpha  Scalar multiplicator.
	\param in_vector3 Operand vector.
	\return A reference to this vector.
	*/
	CVector3& Scale(double in_dAlpha, const CVector3& in_vector3);

	/*! Negates this vector (this = - this).
	\return A reference to this vector.
	*/
	CVector3& NegateInPlace();

	/*! Negates the input vector (v) store the result into this one (this = - v).
	\param in_vector3 Operand vector.
	\return A reference to this vector.
	*/
	CVector3& Negate(const CVector3& in_vector3);

	/*! Subtracts the input vector3 (v) from this one (this = this - v).
	\param in_vector3 Operand vector.
	\return A reference to this vector.
	*/
	CVector3& SubInPlace(const CVector3&in_vector3);

	/*! Subtracts the input vector3 (A) from the input vector3 (B) and stores the result into
	this one (this = A - B).
	\param in_vector3A Operand vector.
	\param in_vector3B Operand vector.
	\return A reference to this vector.
	*/
	CVector3& Sub(const CVector3& in_vector3A, const CVector3& in_vector3B);

	/*! Adds the input vector to this one (this = this + in_vector3).
	\param in_vector3 Operand vector.
	\return A reference to this vector.
	*/
	CVector3& AddInPlace(const CVector3& in_vector3);

	/*! Adds both input vectors and stores the result in this one. (this = A + B)
	\param in_vector3A Operand vector.
	\param in_vector3B Operand vector.
	\return A reference to this vector.
	*/
	CVector3& Add(const CVector3& in_vector3A,const CVector3& in_vector3B);

	/*! Returns the X, Y and Z values of the vector.
	\param io_dX X value to get in the vector.
	\param io_dY Y value to get in the vector.
	\param io_dZ Z value to get in the vector.
	*/
	void Get(double& io_dX, double& io_dY, double& io_dZ)const;

	/*! Sets the X, Y and Z values of the vector.
	\param in_dX X value to set in the vector.
	\param in_dY Y value to set in the vector.
	\param in_dZ Z value to set in the vector.
	\return A reference to the vector.
	*/
	CVector3& Set(double in_dX, double in_dY, double in_dZ);

	/*! Returns the Z value of this vector.
	\return Z value of this vector.
	*/
	double GetZ()const;

	/*! Sets the Z value of this vector.
	\param in_dZ z value.
	\return A reference to this vector.
	*/
	CVector3& PutZ(double in_dZ);

	/*! Returns the Y value of this vector.
	\return Y value of this vector.
	*/
	double GetY()const;

	/*! Sets the Y value of this vector.
	\param in_dY y value.
	\return A reference to this vector.
	*/
	CVector3& PutY(double in_dY);

	/*! Returns the X value of this vector.
	\return X value of this vector.
	*/
	double GetX()const;

	/*! Sets the X value of this vector.
	\param in_dX x value.
	\return A reference to this vector.
	*/
	CVector3& PutX(double in_dX);

private:
	double m_dX,m_dY,m_dZ;
};


//*************************************************************************
/*! \class CVector3Array xsi_vector3.h
	\brief The CVector3Array is an uni-dimensional array of CVector3.

	The array is zero-based, which means that the first item index is 0 and the larger index is one
	less that the number of elements contained in the array.

	\sa MATH::CVector3, CFacetRefArray::GetNormalArray, CPointRefArray::GetPositionArray,
		CPointRefArray::PutPositionArray, CPointRefArray::GetNormalArray, CPolygonFaceRefArray::GetNormalArray,
		PolygonMesh::Get, PolygonMesh::Set, CSegmentRefArray::GetNormalArray, Triangle::GetPositionArray,
		Triangle::GetPolygonNodeNormalArray, CTriangleRefArray::GetPositionArray,
		CTriangleRefArray::GetPolygonNodeNormalArray, CTriangleVertexRefArray::GetPositionArray,
		CTriangleVertexRefArray::GetNormalArray, X3DObject::AddPolygonMesh
*/
//*************************************************************************
class SICPPSDKDECL CVector3Array
{
public:
	/*! Constructs a CVector3Array and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CVector3Array( LONG in_size=0);

	/*! Constructs a CVector3Array object from another CVector3Array object.
	\param in_Array constant CVector3Array object.
	*/
	CVector3Array(const CVector3Array& in_Array);

	/*! Default destructor. */
	~CVector3Array();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	*/
	CVector3Array& operator=( const CVector3Array& in_array );

	/*! Returns the number of items in this CVector3Array
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a LONG at the end of this array's.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CVector3& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if the new size is larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be
	called by constant objects, the returned value is read-only.
	\param in_index index in this zero based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CVector3& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	CVector3& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CVector3Array to compare with.
	\return true if in_array contains the same number of elements as this array and if all members
		of in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CVector3Array& in_array) const;

	/*! Inequality operator.
	\param in_array
	\return true if one member of in_array is different from the corresponding members in this
		array or if the arrays are not of the same size.
	*/
	bool operator!=(const CVector3Array& in_array) const;


private:

	void* m_ptr;
};

};
};

#endif // __XSIVECTOR3_H__
