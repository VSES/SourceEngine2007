//*****************************************************************************
/*!
   \file xsi_transformation.h
   \brief Transformation class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITRANSFORMATION_H__
#define __XSITRANSFORMATION_H__

#include "sicppsdk.h"
#include "xsi_vector3.h"
#include "xsi_rotation.h"

namespace XSI {

namespace MATH {

//*****************************************************************************
/*! \class CTransformation xsi_transformation.h
	\brief This object represents a transformation which is the result of the combination of a Scaling,
	a Rotation and a Translation, in the S.R.T order.

	\sa CMatrix3, CMatrix4, CQuaternion, CVector3, CRotation

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

class SICPPSDKDECL CTransformation
{
public:

	/*! Default constructor. */
	CTransformation();

	/*! Copy constructor.
	\param in_transformation constant class object.
	*/
	CTransformation( const CTransformation& in_transformation);

	/*! Default destructor. */
	~CTransformation();

	/*! Assignment operator.
	\param in_transformation constant class object.
	\return A reference to this transformation.
	*/
	CTransformation & operator=( const CTransformation & in_transformation);

	/*! Returns the matrix representation values .
	\return The transformation values.
	*/
	CMatrix4	GetMatrix4()const;

	/*! Sets the matrix representation values of this transformation from the matrix in_matrix4.
	\param in_matrix4 matrix operand.
	*/
	void	SetMatrix4(const CMatrix4& in_matrix4);

	/*! Sets the rotation
	\param in_Rotation Source rotation
	\return A reference to this transformation.
	*/
	CTransformation& SetRotation( const CRotation &in_Rotation );

	/*! Returns the rotation
	\return CRotation
	*/
	CRotation GetRotation() const;

	/*! Sets the rotation angle (in degrees) around the X axis of this transformation.
	\param in_dX Rotation around X.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetRotX(const double& in_dX);

	/*! Returns the rotation angle (in degrees) around the X axis of this transformation.
	\return A double containing the rotation angle around X.
	\since Gemini
	*/
	double GetRotX() const;

	/*! Sets the rotation angle (in degrees) around the Y axis of this transformation.
	\param in_dY Rotation around Y.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetRotY(const double& in_dY);

	/*! Returns the rotation angle (in degrees) around the Y axis of this transformation.
	\return A double containing the rotation angle around Y.
	\since Gemini
	*/
	double GetRotY()const;

	/*! Sets the rotation angle (in degrees) around the Z axis of this transformation.
	\param in_dZ Rotation around Z.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetRotZ(const double& in_dZ);

	/*! Returns the rotation angle (in degrees) around the Z axis of this transformation.
	\return A double containing the rotation angle around Z.
	\since Gemini
	*/
	double GetRotZ()const;

	/*! Sets the rotation order.  %XSI uses the CRotation::siXYZ order by default.  The rotation order is
	usually changed in order to fix Gimbal locks or to reuse data from a package using a different rotation order.
	\param in_Order New rotation order to use
	\sa CTransformation::GetRotationOrder
	*/
	void PutRotationOrder( CRotation::RotationOrder in_Order );

	/*! Returns the current rotation order
	\return The rotation order
	\sa CTransformation::PutRotationOrder
	*/
	CRotation::RotationOrder GetRotationOrder() const;

	/*! Sets the Euler angles of the rotation and rotation order.  %XSI uses the CRotation::siXYZ order by default.
	The rotation order is usually changed in order to fix Gimbal locks or to reuse data from a package using a
	different rotation order.  Most users will use the overloaded function without the rotation order parameter if
	they work exclusively with %XSI.
	\param in_dX X euler angle value
	\param in_dY Y euler angle value
	\param in_dZ Z euler angle value
	\param in_Order Rotation order
	\return A reference to this transformation.
	\sa CTransformation::PutRotationOrder, CTransformation::GetRotationOrder
	 */
	CTransformation& SetRotationFromXYZAnglesValues
	(
		double in_dX,
		double in_dY,
		double in_dZ,
		CRotation::RotationOrder in_Order
	);

	/*! Sets the Euler angles of the rotation
	\note This function does not change the rotation order.  You can use the overloaded version of this function
		to set the rotation order
	\param in_dX X euler angle value
	\param in_dY Y euler angle value
	\param in_dZ Z euler angle value
	\return A reference to this transformation.
	 */
	CTransformation& SetRotationFromXYZAnglesValues
	(
		double in_dX,
		double in_dY,
		double in_dZ
	);

	/*! Returns the Euler angle values of the rotation.
	\param io_dX X euler angle value
	\param io_dY Y euler angle value
	\param io_dZ Z euler angle value
	*/
	void GetRotationFromXYZAnglesValues
	(
		double& io_dX,
		double& io_dY,
		double& io_dZ
	)const;

	/*! Combines this transformations with in_transformation then stores the result in this transformation
	\param in_transformation transformation operand
	\return A reference to this transformation.
	*/
	CTransformation& MulInPlace(const CTransformation& in_transformation);

	/*! Combines the transformations A and B then stores the result in this transformation.
	\param in_transformationA transformation operand
	\param in_transformationB transformation operand
	\return A reference to this transformation.
	*/
	CTransformation& Mul
	(
		const CTransformation & in_transformationA,
		const CTransformation & in_transformationB
	);

	/*! Applies a translation defined in the parent space of this transformation.
	\param in_vector3 translation vector.
	\return A reference to this transformation.
	*/
	CTransformation& AddParentTranslation(const CVector3& in_vector3);

	/*! Applies a translation defined in the local space of this transformation.
	\param in_vector3 translation vector.
	\return A reference to this transformation.
	*/
	CTransformation& AddLocalTranslation(const CVector3& in_vector3);

	/*! Applies a scaling defined in the parent space of this transformation.
	\param in_vector3 scaling vector.
	\return A reference to this transformation.
	*/
	CTransformation& AddParentScaling(const CVector3& in_vector3);

	/*! Applies a scaling defined in the local space of this transformation.
	\param in_vector3 scaling vector.
	\return A reference to this transformation.
	*/
	CTransformation& AddLocalScaling(const CVector3& in_vector3);

	/*! Sets the rotation part of this transformation from three orthonormal axes. No verification
	is performed about their orthonormality.
	\param in_vector3X unitary X axis (orthogonal to Y and Z)
	\param in_vector3Y unitary Y axis (orthogonal to X and Z)
	\param in_vector3Z unitary Z axis (orthogonal to X and Y)
	\return A reference to this transformation.
	*/
	CTransformation& SetRotationFromXYZAxes
	(
		const CVector3& in_vector3X,
		const CVector3& in_vector3Y,
		const CVector3& in_vector3Z
	);

	/*! Sets the translation of this transformation.
	\param in_dX translation in X.
	\param in_dY translation in Y.
	\param in_dZ translation in Z.
	\return A reference to this transformation.
	*/
	CTransformation& SetTranslationFromValues
	(
		double in_dX,
		double in_dY,
		double in_dZ
	);

	/*! Returns the translation values of this transformation.
	\param io_dX translation in X.
	\param io_dY translation in Y.
	\param io_dZ translation in Z.
	*/
	void GetTranslationValues
	(
		double& io_dX,
		double& io_dY,
		double& io_dZ
	)const;

	/*! Sets the scaling of this transformation.
	\param in_dX scaling in X.
	\param in_dY scaling in Y.
	\param in_dZ scaling in Z.
	\return A reference to this transformation.
	*/
	CTransformation& SetScalingFromValues
	(
		double in_dX,
		double in_dY,
		double in_dZ
	);

	/*! Returns the scaling values of this transformation.
	\param io_dX scaling in X.
	\param io_dY scaling in Y.
	\param io_dZ scaling in Z.
	*/
	void GetScalingValues
	(
		double& io_dX,
		double& io_dY,
		double& io_dZ
	)const;

	/*! Returns the scaling orientation values of this transformation.
	\param io_dX scaling in X.
	\param io_dY scaling in Y.
	\param io_dZ scaling in Z.
	*/
	void GetScalingOrientationXYZAngles
	(
		double& io_dX,
		double& io_dY,
		double& io_dZ
	)const;

	/*! Sets the scaling orientation angles of this transformation
	\param in_dX scaling orientation X angle.
	\param in_dY scaling orientation Y angle.
	\param in_dZ scaling orientation Z angle.
	\return A reference to this transformation.
	*/
	CTransformation& SetScalingOrientationFromXYZAngles
	(
		double in_dX,
		double in_dY,
		double in_dZ
	);

	/*! Returns whether this transformation has a valid scaling orientation.
	\return true if this transformation has a valid scaling orientation, else false.
	*/
	bool HasScalingOrientation()const;

	/*! Sets this transformation to identity.
	\return A reference to this transformation.
	*/
	CTransformation& SetIdentity();

	/*! Sets the translation of this transformation. The translation is defined by a CVector3.
	\param in_vector3 vector of translation.
	\return A reference to this transformation.
	*/
	CTransformation& SetTranslation(const CVector3& in_vector3);

	/*! Returns the translation vector of this transformation in a CVector3.
	\return The translation.
	*/
	CVector3 GetTranslation()const;

	/*! Sets the X position of this transformation.
	\param in_dX Position in X.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetPosX(const double& in_dX);

	/*! Returns the X position value of this transformation.
	\return A double containing the X position value.
	\since Gemini
	*/
	double GetPosX()const;

	/*! Sets the Y position of this transformation.
	\param in_dY Position in Y.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetPosY(const double& in_dY);

	/*! Returns the Y position value of this transformation.
	\return A double containing the Y position value.
	\since Gemini
	*/
	double GetPosY()const;

	/*! Sets the Z position of this transformation.
	\param in_dZ Position in Z.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetPosZ(const double& in_dZ);

	/*! Returns the Z position value of this transformation.
	\return A double containing the Z position value.
	\since Gemini
	*/
	double GetPosZ()const;

	/*! Sets the rotation part of this transformation by using the axis/angle representation.
	\param in_vector3 Axis of rotation.
	\param in_dAngle  Angle of rotation (in radians).
	\return A reference to this transformation.
	*/
	CTransformation& SetRotationFromAxisAngle
	(
		const CVector3& in_vector3,
		double			in_dAngle
	);

	/*! Sets the X, Y and Z angles (in radians) of the rotation part of this transformation.
	\param in_vector3 vector containing X,Y and Z angles values(in radians).
	\return A reference to this transformation.
	*/
	CTransformation& SetRotationFromXYZAngles(const CVector3& in_vector3);

	/*! Extracts the axis and angle from the rotation part of this transformation.
	\param io_vector3 Axis of rotation.
	\return Angle of rotation in radians as a double.
	*/
	double GetRotationAxisAngle (CVector3& io_vector3)const;

	/*! Extracts the X, Y and Z angles (in radians) from the rotation part of this transformation
	inside the CVector3.
	\return The rotation X,Y and Z angles (in radians) as a CVector3 object.
	*/
	CVector3 GetRotationXYZAngles()const;

	/*! Sets the scaling of this transformation by getting the values from a CVector3.
	\param in_vector3 Vector containing X,Y and Z scaling values.
	\return A reference to this transformation.
	*/
	CTransformation& SetScaling(const CVector3& in_vector3);

	/*! Returns the scaling values of this transformation in a CVector3.
	\return A CVector3 containing the X,Y and Z scaling values.
	*/
	CVector3 GetScaling()const;

	/*! Sets the scaling in X of this transformation.
	\param in_dX Scaling in X.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetSclX(const double& in_dX);

	/*! Returns the scaling value in X of this transformation.
	\return A double containing the X scaling value.
	\since Gemini
	*/
	double GetSclX()const;

	/*! Sets the scaling in Y of this transformation.
	\param in_dY Scaling in Y.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetSclY(const double& in_dY);

	/*! Returns the scaling value in Y of this transformation.
	\return A double containing the Y scaling valus.
	\since Gemini
	*/
	double GetSclY()const;

	/*! Sets the scaling in Z of this transformation.
	\param in_dZ Scaling in Z.
	\return A reference to this transformation.
	\since Gemini
	*/
	CTransformation& SetSclZ(const double& in_dZ);

	/*! Returns the scaling value in Z of this transformation.
	\return A double containing the Z scaling value.
	\since Gemini
	*/
	double GetSclZ()const;

	/*! Sets the rotation of this transformation by getting the values from a CMatrix3.
	\param in_matrix3 3x3 matrix containing the rotation information to set this tranformation's rotation.
	\return A reference to this transformation.
	*/
	CTransformation& SetRotationFromMatrix3(const CMatrix3& in_matrix3);

	/*! Returns the 3x3 matrix representation of the rotation part of this transformation.
	\return A CMatrix3 object containing rotation of this transformation.
	*/
	CMatrix3 GetRotationMatrix3()const;

	/*! Sets the rotation of this transformation by getting the values from a CQuaternion.
	\param in_quaternion quaternion containing the rotation information to set this tranformation's rotation.
	\return A reference to this transformation.
	*/
	CTransformation& SetRotationFromQuaternion(const CQuaternion& in_quaternion);

	/*! Returns the rotation quaternion for this transformation.
	\return A CQuaternion containing rotation of this transformation.
	*/
	CQuaternion GetRotationQuaternion(void)const;

	/*! Tests the equality of this transformation with the specified transformation, with a tolerance of Epsilon.
	\param in_transformation Operand transformation.
	\param in_dEpsilon Error margin Possible Values: [0, +INF[ should be a positive value
	\return true if equal else false.
	*/
	bool EpsilonEquals
	(
		const CTransformation& in_transformation,
		double in_dEpsilon
	) const;

	/*! Tests the strict equality of this transformation with the specified transformation.
	\param in_transformation Operand transformation.
	\return true if equal else false.
	*/
	bool Equals(const CTransformation& in_transformation) const;

	/*! Equality operator tests the strict equality of this transformation with the specified transformation.
	\param in_transformation Operand transformation.
	\return true if equal else false.
	\sa CTransformation::Equals
	*/
	bool operator ==(const CTransformation & in_transformation ) const;

	/*! Inequality operator tests the strict inequality of this matrix with the specified transform.
	\param in_transformation Operand transformation.
	\return true if equal else false.
	\sa CTransformation::Equals
	*/
	bool operator !=(const CTransformation & in_transformation ) const;
private:
	void* m_ptr;
	inline double GetPI(){return 3.14159265358979323846;}
	void DegreeToRadian(double& in_Rot);
	void RadianToDegree(double& in_Rot);

};

};
};

#endif // __XSITRANSFORMATION_H__
