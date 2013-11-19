//*****************************************************************************
/*!
   \file xsi_rotation.h
   \brief CRotation class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGEXSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#ifndef __XSIROTATION_H__
#define __XSIROTATION_H__

#include "sicppsdk.h"
#include "xsi_vector3.h"

namespace XSI {

namespace MATH {

class CVector4;
class CQuaternion;

//*****************************************************************************
/*! \class CRotation xsi_rotation.h
	\brief Provides conversion facilities for various rotation representations
	including Euler angles, Quaternion, Axis angle and rotation matrix.

	\note All angle values are expressed in radians.

	\sa CTransformation, CQuaternion

	\eg
	\code
		using namespace XSI;
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		// Create a cube
		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface",L"",myCube );

		// Rotate it 60 degrees
		KinematicState  cubeKine = myCube.GetKinematics().GetGlobal();
		cubeKine.PutParameterValue( L"rotx", 60.0 );

		// Get the current rotation for the cube
		CRotation cubeRotation = cubeKine.GetTransform().GetRotation();

		// Halve the current rotation angle values
		CVector3 eulerAngles = cubeRotation.GetXYZAngles();
		eulerAngles.ScaleInPlace( 0.5 );

		// Put the halved value back into x
		cubeKine.PutParameterValue( L"rotx", RadiansToDegrees( eulerAngles.GetX() ) );
  	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL CRotation
{
public:
	/*! Defines the rotation order of a rotation for the Euler angles representation.
	*/
	enum RotationOrder
	{
		siXYZ = 0,	/*!< X, Y, Z order */
		siXZY = 1,	/*!< X, Z, Y order  */
		siYXZ = 2,	/*!< Y, X, Z order  */
		siYZX = 3,	/*!< Y, Z, X order  */
		siZXY = 4,	/*!< Z, X, Y order  */
		siZYX = 5		/*!< Z, Y, X order  */
	};

	/*! Default constructor. */
	CRotation();

	/*! Copy constructor.
	\param in_rotation Rotation to copy
	 */
	CRotation( const CRotation& in_rotation);

	/*! Constructor (from CVector3 of Euler angles).
	\param in_EulerAngles Euler angles to construct from
	\param in_RotOrder Rotation order to use
	\sa CRotation::SetFromXYZAngles, CRotation::PutRotationOrder
	*/
	CRotation( const CVector3 &in_EulerAngles, RotationOrder in_RotOrder = siXYZ);

	/*! Constructor (from XYZ Euler angles).
	\param in_dX X angle
	\param in_dY Y angle
	\param in_dZ Z angle
	\param in_RotOrder Rotation order to use
	\sa CRotation::SetFromXYZAngles, CRotation::PutRotationOrder
	*/
	CRotation( double in_dX, double in_dY, double in_dZ, RotationOrder in_RotOrder = siXYZ);


	/*! Constructor (from a quaternion).
	\param in_Quat Quaternion to construct from
	\sa CRotation::GetQuaternion, CRotation::SetFromQuaternion
	*/
	CRotation( const CQuaternion &in_Quat );

	/*! Constructor (using axis angle representation).
	\param in_Axis Axis of rotation
	\param in_dAngle Rotation angle around the axis
	\sa CRotation::GetAxisAngle, CRotation::SetFromAxisAngle
	*/
	CRotation( const CVector3 &in_Axis,  double in_dAngle );

	/*! Constructor (from a rotation matrix).
	\param in_Matrix Rotation matrix
	\sa CRotation::GetMatrix, CRotation::SetFromMatrix
	*/
	CRotation( const CMatrix3 &in_Matrix );

	/*! Default destructor. */
	~CRotation();

	/*! Assignment operator.
	\param in_rotation Rotation to copy
	\return A reference to this rotation.
	 */
	CRotation & operator=( const CRotation & in_rotation);

	/*! Copies the content of another rotation.
	\param in_SrcRot Rotation to copy
	\return This rotation
	*/
	CRotation& CRotation::Copy( const CRotation &in_SrcRot);

	/*! Returns the Euler angles representation of the rotation as a CVector3.
	\return CVector3 Vector containing the X, Y and Z Euler angles
	\sa CRotation::SetFromXYZAngles, CRotation::GetRotationOrder
	*/
	CVector3 GetXYZAngles() const;

	/*! Returns the Euler angles's representation of the rotation.
	\param io_dX X angle
	\param io_dY Y angle
	\param io_dZ Z angle
	\sa CRotation::SetFromXYZAngles, CRotation::GetRotationOrder
	*/
	void GetXYZAngles( double &io_dX, double &io_dY, double &io_dZ ) const;

	/*! Returns the rotation order of this rotation for the Euler angles representation.
	\return CRotation::RotationOrder
	*/
	RotationOrder GetRotationOrder() const;

	/*! Sets the rotation order for the Euler angles representation.
	\param in_RotationOrder New rotation order to use
	\sa CRotation::GetRotationOrder
	*/
	void PutRotationOrder( RotationOrder in_RotationOrder );

	/*! Sets the current rotation from Euler angles.
	\param in_EulerAngles Vector containing the rotation angles
	\param in_Order Rotation order
	\return This rotation
	\sa CRotation::GetXYZAngles, CRotation::PutRotationOrder
	*/
	CRotation& SetFromXYZAngles( const CVector3 &in_EulerAngles, RotationOrder in_Order = siXYZ);

	/*! Sets the rotation from Euler angles.
	\param in_dX X angle
	\param in_dY Y angle
	\param in_dZ Z angle
	\param in_Order Requested rotation order
	\return This rotation
	\sa CRotation::GetXYZAngles, CRotation::PutRotationOrder
	*/
	CRotation& SetFromXYZAngles( double in_dX, double in_dY, double in_dZ, RotationOrder in_Order = siXYZ);

	/*! Sets this rotation from three orthonormal axes.
	\warning No verification is performed about their orthonormality
	\param in_XAxis The X axis
	\param in_YAxis The Y axis
	\param in_ZAxis The Z axis
	\return This rotation
	*/
	CRotation& SetFromXYZAxes( const CVector3 &in_XAxis,  const CVector3 &in_YAxis, const CVector3 &in_ZAxis);

	/*! Multiplies this rotation with another one in place.
	\param in_Rotation Rotation to multiply
	\return This rotation
	*/
	CRotation& operator *= ( const CRotation &in_Rotation );

	/*! Multiplies this rotation with another one in place.
	\param in_Rotation Rotation to multiply
	\return This rotation
	*/
	CRotation& MulInPlace( const CRotation &in_Rotation );

	/*! Stores the result of the multiplication of two rotations.
	\param in_Rotation1 First rotation
	\param in_Rotation2 Second rotation
	\return This rotation
	*/
	CRotation& Mul( const CRotation &in_Rotation1, const CRotation &in_Rotation2 );

	/*! Inverts the rotation in place.
	\return This rotation
	*/
	CRotation& InvertInPlace();

	/*! Stores the invert of a rotation.
	\param in_Rot Rotation to invert
	\return This rotation
	*/
	CRotation& Invert( const CRotation &in_Rot );

	/*! Returns the quaternion representation of the rotation.
	\return CQuaternion
	\sa CRotation::SetFromQuaternion
	*/
	CQuaternion GetQuaternion() const;

	/*! Sets the rotation from a quaternion.
	\param in_Quat Quaternion to set from
	\return This rotation
	\sa CRotation::GetQuaternion
	*/
	CRotation& SetFromQuaternion( const CQuaternion &in_Quat);

	/*! Sets the rotation from a quaternion.
	\param in_Quat Quaternion to set from
	\return This rotation
	\sa CRotation::SetFromQuaternion
	*/
	CRotation& operator = ( const CQuaternion &in_Quat );

	/*! Returns the axis angle representation of the rotation.
	\param io_dAngle Rotation angle around the axis
	\return CVector3 Axis of rotation
	\sa CRotation::SetFromAxisAngle
	*/
	CVector3 GetAxisAngle( double &io_dAngle) const;

	/*! Sets the rotation from an axis angle.
	\param in_Axis Axis of rotation
	\param in_dAngle Rotation angle around the axis
	\return This rotation
	\sa CRotation::GetAxisAngle
	*/
	CRotation& SetFromAxisAngle( const CVector3 &in_Axis,  double in_dAngle );

	/*! Returns the rotation matrix.
	\return CMatrix3
	\sa CRotation::SetFromMatrix
	*/
	CMatrix3 GetMatrix() const;

	/*! Sets the rotation from a rotation matrix.
	\param in_Matrix Rotation matrix
	\return This rotation
	\sa CRotation::GetMatrix
	*/
	CRotation& SetFromMatrix( const CMatrix3 &in_Matrix);

	/*! Sets the rotation from a rotation matrix.
	\param in_Matrix Rotation matrix
	\return This rotation
	\sa CRotation::SetFromMatrix
	*/
	CRotation& operator= (const CMatrix3 &in_Matrix);

	/*! Sets the rotation to identity rotation.
	*/
	void SetIdentity();

	/*! Tests the equality of this rotation against in_Rot, with a
	tolerance of in_dEpsilon.

	\param in_Rot Rotation to compare against
	\param in_dEpsilon Tolerance.  Rotations are considered equal if each
		parameter pair has a difference lower than in_dEpsilon.
	\return true if rotations are approximately equal; false if not equal
	\sa CRotation::Equals
	*/
	bool EpsilonEquals( const CRotation &in_Rot, double in_dEpsilon ) const;

	/*! Performs exact comparison of two quaternions.
	\note Each parameter of both quaternions must be exactly the same for a match.
	\param in_Rot Rotation to compare against
	\sa CRotation::EpsilonEquals
	*/
	bool Equals( const CRotation &in_Rot ) const;

	/*! Performs exact comparison of two quaternions.
	\note Each parameter of both quaternions must be exactly the same for a match.
	\param in_Rot Rotation to compare against
	\sa CRotation::EpsilonEquals, CRotation::Equals
	*/
	bool operator == ( const CRotation &in_Rot ) const;

	/*! Inequality operator tests the strict inequality of this rotation with the rotation in_rotation.
	\param in_Rot Operand rotation.
	\return true if equal else false.
	\sa CRotation::Equals
	 */
	bool operator !=(const CRotation & in_Rot ) const;
private:
	void *m_ptr;
};

};
};

#endif // __XSIROTATION_H__
