//*****************************************************************************
/*!
   \file xsi_quaternion.h
   \brief CQuaternion class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGEXSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#ifndef __XSIQUATERNION_H__
#define __XSIQUATERNION_H__

#include "sicppsdk.h"
#include "xsi_rotation.h"

namespace XSI {

namespace MATH {

class CVector3;
class CVector4;

//*****************************************************************************
/*! \class CQuaternion xsi_quaternion.h
	\brief Implementation of a quaternion q=a+bi+cj+dk.

	A quaternion is often noted q=(s,v) where s is a scalar and v is a 3D vector, or either
	noted q=(W,X,Y,Z), where W, X, Y and Z are scalar, and X, Y and Z are the components of
	vector v in previous notation.

	You can set a quaternion using various rotation representation with the help of the
	CRotation class.

	\sa CRotation, CTransformation

	\eg Using CRotation to construct a CQuaternion from various rotation representations.
	\code
		using namespace XSI;
		using namespace MATH;

		CVector3 eulerAngles( PI, PI * 2.0, PI / 2.0 );
		CRotation eulerAnglesRot( eulerAngles, CRotation::siXYZ );
		CQuaternion quat( eulerAnglesRot );

		CVector3 axis( 1.0, 2.0, 3.0 );
		CRotation axisAngleRot( axis, PI );
		quat = axisAngleRot;

		CMatrix3 identityMatrix;
		quat.SetFromRotation( CRotation(identityMatrix) );
		\endcode

		\b Example: Performing linear extrapolation using quaternions
		\code
		// Quaternion linear extrapolation example
		using namespace XSI;
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		// define start and end point for rotation
		CQuaternion startQuat;
		CRotation rotationEnd( CVector3( PI, 0.0, 0.0 ) );
		CQuaternion endQuat( rotationEnd );

		// number of interpolation to perform + interpolation increment
		const size_t nbInterpolations = 10;
		const double interpolationIncrement = 1.0 / nbInterpolations;

		CQuaternion interpolatedQuaternion;
		CRotation currentRotation;
		X3DObject currentObject;
		CVector3 rotationAngles;

		// Create cubes rotated using interpolated values
		for ( double i = 0.0; i < 1.0; i += interpolationIncrement )
		{
			// interpolate values
			interpolatedQuaternion.Slerp( startQuat, endQuat, i );

			// create cube
			root.AddGeometry( L"Cube", L"MeshSurface", L"", currentObject );

			// apply interpolated value in X
			currentRotation = interpolatedQuaternion;
			rotationAngles = currentRotation.GetXYZAngles();
			KinematicState globalKS = currentObject.GetKinematics().GetGlobal();
			globalKS.PutParameterValue( L"rotx",
				(double)RadiansToDegrees(rotationAngles.GetX()));
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL CQuaternion
{
public:
	/*! Default constructor.
	Constructs the identity quaternion.
	\sa CQuaternion::SetIdentity
	*/
	CQuaternion();

	/*! Copy constructor.
	\param in_quaternion constant class object.
	 */
	CQuaternion( const CQuaternion& in_quaternion);

	/*! Default destructor. */
	~CQuaternion();

	/*! Assignment operator.
	\param in_quaternion constant class object.
	\return This quaternion
	 */
	CQuaternion & operator=( const CQuaternion & in_quaternion);

	/*! Constructor.
	\param in_dW W component
	\param in_dX X component of the quaternion's vector
	\param in_dY Y component of the quaternion's vector
	\param in_dZ Z component of the quaternion's vector
	*/
	CQuaternion( const double in_dW, const double in_dX,
               const double in_dY, const double in_dZ );

	/*! Constructor.
	\note The W component of the quaternion will be set to 1.0
	\param in_vct Vector containing the X, Y and Z values
	*/
	CQuaternion( const CVector3& in_vct );

	/*! Constructor.
	\param in_vct Vector containing the X, Y, Z and W values
	*/
	CQuaternion( const CVector4& in_vct );

	/*! Constructor (from a rotation).
	\param in_Rot Rotation to construct from
	*/
	CQuaternion( const CRotation& in_Rot );

	/*! Sets the quaternion from a rotation
	\param in_Rot Rotation to set from
	\return This quaternion
	*/
	CQuaternion& operator = ( const CRotation &in_Rot );

	/*! Sets the quaternion from a rotation
	\param in_Rot Rotation to set from
	\return This quaternion
	*/
	CQuaternion& SetFromRotation( const CRotation &in_Rot );

	/*! Returns a rotation that represents the current quaternion
	\return CRotation
	*/
	CRotation GetRotation() const;

	/*! Normalizes the quaternion
	*/
	CQuaternion& Normalize();

	/*! Tests the equality of this quaternion against in_Quat, with a
	tolerance of in_dEpsilon.

	\param in_Quat Quaternion to compare against
	\param in_dEpsilon Tolerance.  Quaternions are considered equal if each
		parameter pair have a difference lower than in_dEpsilon.
	\return true Quaternions are approximately equal
	\return false Quarternions are not equal
	*/
	bool EpsilonEquals( const CQuaternion &in_Quat, double in_dEpsilon ) const;

	/*! Performs an exact comparison of two quaternions.
	\note Each parameter of both quaternions must be exactly the same fora match.
	\param in_Quat Quaternion to compare against
	\sa CQuaternion::EpsilonEquals
	*/
	bool Equals( const CQuaternion &in_Quat ) const;

	/*! Performs a comparison.
	\note Each parameter of both quaternions must be exactly the same for a match.
	\param in_Quat Quaternion to compare against
	\sa CQuaternion::EpsilonEquals, CQuaternion::Equals
	*/
	bool operator == ( const CQuaternion &in_Quat ) const;

	/*!	Performs spherical linear interpolation of two unit quaternions whithin
	this quaternion.
	\param in_Quat1 First quaternion
	\param in_Quat2 Second quaternion
	\param in_dU Interpolation factor within [0.0 .. 1.0].
	\return This quaternion
	*/
	CQuaternion& Slerp( const CQuaternion &in_Quat1, const CQuaternion &in_Quat2, double in_dU);

	/*! Multiplies this quaternion with another one in place.
	\param in_Quat Quaternion to multiply
	\return This quaternion
	\sa CQuaternion::Mul, CQuaternion::MulInPlace
	*/
	CQuaternion& operator *= ( const CQuaternion &in_Quat );

	/*! Multiplies this quaternion with another one in place.
	\param in_Quat Quaternion to multiply
	\return This quaternion
	\sa CQuaternion::Mul
	*/
	CQuaternion& MulInPlace( const CQuaternion &in_Quat );

	/*! Stores the result of the multiplication of two quaternions.
	\param in_Quat1 First quaternion
	\param in_Quat2 Second quaternion
	\return This quaternion
	*/
	CQuaternion& Mul( const CQuaternion &in_Quat1, const CQuaternion &in_Quat2);

	/*! Subtracts a quaternion from the current quaternion in place.
	\param in_Quat Quaternion to subtract
	\return This quaternion
	\sa CQuaternion::SubInPlace, CQuaternion::Sub
	*/
	CQuaternion& operator -= ( const CQuaternion &in_Quat );

	/*! Subtracts a quaternion from the current quaternion in place.
	\param in_Quat Quaternion to subtract
	\return This quaternion
	\sa CQuaternion::Sub
	*/
	CQuaternion& SubInPlace( const CQuaternion &in_Quat );

	/*! Stores the result of the subtraction of two quaternions.
	\param in_Quat1 First quaternion
	\param in_Quat2 Second quaternion
	\return This quaternion
	\sa CQuaternion::SubInPlace
	*/
	CQuaternion& Sub( const CQuaternion &in_Quat1,  const CQuaternion &in_Quat2);

	/*! Negates all the components of this quaternion.
	\return This quaternion
	\sa CQuaternion::Negate
	*/
	CQuaternion& NegateInPlace();

	/*! Stores the result of the negation of a given quaternion without changing it.
	\param in_Quat Quaternion to negate
	\return This quaternion
	\sa CQuaternion::NegateInPlace
	*/
	CQuaternion& Negate( const CQuaternion &in_Quat);

	/*! Returns the squared length of this quaternion.
	\return The squared length
	\sa CQuaternion::GetLength
	*/
	double GetLengthSquared() const;

	/*! Returns the length of this quaternion.
	\return Length of this quaternion
	\sa CQuaternion::GetLengthSquared
	*/
	double GetLength() const;

	/*! Adds a quaternion to this quaternion.
	\param in_Quat Quaternion to add
	\sa CQuaternion::AddInPlace, CQuaternion::Add
	*/
	CQuaternion& operator += ( const CQuaternion &in_Quat);

	/*! Adds a quaternion to this quaternion.
	\param in_Quat Quaternion to add
	\sa CQuaternion::Add
	*/
	CQuaternion& AddInPlace( const CQuaternion &in_Quat );

	/*! Stores the result of the addition of two quaternions.
	\param in_Quat1 First quaternion
	\param in_Quat2 Second quaternion
	\return This quaternion
	\sa CQuaternion::AddInPlace
	*/
	CQuaternion& Add( const CQuaternion &in_Quat1,  const CQuaternion &in_Quat2);

	/*! Conjugates this quaternion.
	\return This quaternion
	\sa CQuaternion::ConjugateInPlace
	*/
	CQuaternion& ConjugateInPlace();

	/*! Stores the conjugate of a quaternion without changing it.
	\param in_Quat Quaternion to conjugate.
	\return This quaternion
	\sa CQuaternion::ConjugateInPlace
	*/
	CQuaternion& Conjugate( const CQuaternion &in_Quat);

	/*! Inverts this quaternion.
	\return This quaternion
	\sa CQuaternion::Invert
	*/
	CQuaternion& InvertInPlace();

	/*! Stores the invert of a quaternion without changing it.
	\param in_Quat Quaternion to invert
	\return This quaternion
	\sa CQuaternion::Invert
	*/
	CQuaternion& Invert( const CQuaternion &in_Quat);

	/*! Sets this quaternion to identity quaternion.
	\return This quaternion
	*/
	CQuaternion& SetIdentity();

	/*! Copies the content of a quaternion.
	\param in_Quat Quaternion to copy
	\return This quaternion
	*/
	CQuaternion&  Copy( const CQuaternion &in_Quat );

	/*! Returns the components of this quaternion.
	\retval out_dW The current value of the W component
	\retval out_dX The current value of the X component
	\retval out_dY The current value of the Y component
	\retval out_dZ The current value of the Z component
	*/
	void Get( double &out_dW, double &out_dX, double &out_dY, double &out_dZ ) const;

	/*! Returns the components of this quaternion.
	\return Vector that holds the values of the X, Y and Z components
	*/
	CVector3 Get() const;

	/*! Returns the components of this quaternion.
	\param io_XYZWVector Vector that holds the values of the X, Y, Z and W components
	*/
	void Get( CVector4 &io_XYZWVector ) const;

	/*! Sets the quaternion's components.
	\param in_dW New value for the W component
	\param in_dX New value for the X component
	\param in_dY New value for the Y component
	\param in_dZ New value for the Z component
	\return This quaternion
	*/
	CQuaternion& Set( double in_dW,  double in_dX,  double in_dY,  double in_dZ);

	/*! Sets the quaternion's components.
	\note The W value is left as is.
	\param in_XYZVector Vector that holds the new values for the X, Y and Z components
	\return This quaternion
	*/
	CQuaternion& Set( const CVector3 &in_XYZVector );

	/*! Sets the quaternion's components.
	\param in_XYZWVector Vector that holds the new values for the X, Y and Z
		components.  The W component is set to 1.0.
	\return This quaternion
	*/
	CQuaternion& Set( const CVector4 &in_XYZWVector );

	/*! Gets the value of a single component.
	\param in_nIndex 0-based index of the component to get.  This value
		can be 0 (W), 1(X), 2(Y) or 3(Z).
	\return The requested value
	\return 0.0 if an invalid index was provided.
	\sa CQuaternion::PutValue
	*/
	double GetValue( short in_nIndex ) const;

	/*! Sets the value of a single component.
	\note The quaternion is unchanged if the provided index is invalid
	\param in_nIndex 0-based index of the component to set.  This value
		can be 0 (W), 1(X), 2(Y) or 3(Z).
	\param newVal new value
	\return This quaternion
	\sa CQuaternion::SetValue
	*/
	CQuaternion& PutValue( short in_nIndex,  double newVal);

	/*! Returns the W component.
	\return The W component
	*/
	double GetW() const;

	/*! Sets the W component.
	\param newVal The new value for the component
	\return This quaternion
	*/
	CQuaternion& PutW( double newVal);

	/*! Returns the Z component.
	\return The Z component
	*/
	double GetZ() const;

	/*! Sets the Z component.
	\param newVal The new value for the component
	\return This quaternion
	*/
	CQuaternion& PutZ( double newVal);

	/*! Returns the Y component.
	\return The Y component
	*/
	double GetY() const;

	/*! Sets the Y component.
	\param newVal The new value for the component
	\return This quaternion
	*/
	CQuaternion& PutY( double newVal);

	/*! Returns the X component.
	\return The X component
	*/
	double GetX() const;

	/*! Sets the X component.
	\param newVal The new value for the component
	\return This quaternion
	*/
	CQuaternion& PutX( double newVal);

	/*! Sets the quaternion using X,Y,Z Euler angles (in radians).
	\param x The Euler X value in radians
	\param y The Euler Y value in radians
	\param z The Euler Z value in radians
	\return This quaternion
	*/
	CQuaternion& SetFromXYZAnglesValues( double x, double y, double z );

	/*! Returns the X,Y,Z Euler angles (in radians) for this quaternion.
	\retval x The Euler X value in radians
	\retval y The Euler Y value in radians
	\retval z The Euler Z value in radians
	*/
	void GetXYZAnglesValues( double& x, double& y, double& z )const;

	/*! Inequality operator (tests the strict inequality of this quaternion
	with the quaternion in_quat).
	\param in_quat Operand quaternion.
	\return true if equal else false.
	\sa CQuaternion::Equals
	 */
	bool operator !=(const CQuaternion & in_quat ) const;

private:
	double m_dX, m_dY, m_dZ, m_dW;
};

};
};

#endif // __XSIQUATERNION_H__
