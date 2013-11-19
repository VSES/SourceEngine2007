//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCQuaternion.h | Main header file for SIBCQuaternion implementation
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

//***************************************************************************************
// Includes
//***************************************************************************************
#include <float.h>
#include <SIBCUtil.h>
#include <SIBCMatrix.h>
#include <SIBCVector.h>


//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __CSIBCQUATERNION_H__
#define __CSIBCQUATERNION_H__

//***************************************************************************************
// CSIBCQuaternion | Implementation of a Quaternion class.
//***************************************************************************************

//! Generic class representing quaternions.

/*! The notation used in this quaternion class (W, X, Y, Z) makes the quaternions closely
	related to 4D vectors (See CSIBCVector4D). In general, the quaternion can be thought
	of as a scalar plus a vector, where the W component is the vector, and X, Y and Z are
	the coefficients of the I, J and K matricies respectively.
	
	The components of the CSIBCQuaternion class are represented by single precision floating-point 
	numbers. Its corresponding double precision class, CSIBCQuaterniond, has much more 
	functionality, such as the ability to interpolate and multiply quaternions.

*/
class XSICOREEXPORT CSIBCQuaternion
{
    public:
		
		/*! Default Constructor.
			Sets all components of the quaternion to zero, except W, which is set to one.
			\sa CSIBCQuaternion::Init()
		*/
        CSIBCQuaternion() :
            m_fW(1.0), m_fX(0.0), m_fY(0.0), m_fZ(0.0) {}

		/*! Copy Copnstructor.
			Copies the data from \p in_quat to the new quaternion.
			\param	in_quat	Quaternion containing data to copy to this quaternion.
			\sa CSIBCQuaternion::operator=
			\sa CSIBCQuaternion::Init(CSIBCQuaternion &)
		*/
        CSIBCQuaternion(const CSIBCQuaternion &in_quat) { *this = in_quat; }  

        ~CSIBCQuaternion() {}

		/*! Initializes the quaternion components values all to zero, except W, which is 
			set to one.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly initialized.
			</ul>

			\sa CSIBCQuaternion::CSIBCQuaternion()
			\sa CSIBCQuaternion::Init
		*/
        inline SI_Error Init();

		/*! Initializes the quaternion component values to those of \p in_quat.
			\param	in_quat	The quaternion from which to copy initialization values.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly initialized.
			</ul>

			\sa CSIBCQuaternion::CSIBCQuaternion(CSIBCQuaternion &)
			\sa CSIBCQuaternion::Init
		*/
        SI_Error Init(CSIBCQuaternion &in_quat);
		
		/*! Initializes the quaternion components values to the values given.
			\param	in_dX	The new value for the X component of the quaternion.
			\param	in_dY	The new value for the Y component of the quaternion.
			\param	in_dZ	The new value for the Z component of the quaternion.
			\param	in_dW	The new value for the W component of the quaternion.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly initialized.
			</ul>

			\sa CSIBCQuaternion::Init
			\sa CSIBCQuaternion::Set
		*/
        SI_Error Init(SI_Float in_dX,
                      SI_Float in_dY, 
                      SI_Float in_dZ, 
                      SI_Float in_dW); 

		/*! Initializes the quaternion to the quaternion represented by the euler rotations
			given by the euler angles (pitch-roll-yaw) of the parameters.
			\param	in_dX	Value for the pitch euler angle, for the quaternion value.
			\param	in_dY	Value for the roll euler angle, for the quaternion value.
			\param	in_dZ	Value for the yaw euler angle, for the quaternion value.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly initialized.
			</ul>

			\sa CSIBCQuaternion::Init
		*/
        SI_Error Init(SI_Float in_dX, SI_Float in_dY, SI_Float in_dZ);

		/*! Gets the X component of the quaternion.
			\return	SI_Float	The X component of the quaternion.
			\sa CSIBCQuaternion::GetY
			\sa CSIBCQuaternion::GetZ
			\sa CSIBCQuaternion::GetW
			\sa CSIBCQuaternion::SetX
		*/
        SI_Float GetX() const { return m_fX; }

		/*! Gets the Y component of the quaternion.
			\return	SI_Float	The Y component of the quaternion.
			\sa CSIBCQuaternion::GetX
			\sa CSIBCQuaternion::GetZ
			\sa CSIBCQuaternion::GetW
			\sa CSIBCQuaternion::SetY
		*/
        SI_Float GetY() const { return m_fY; }

		/*! Gets the Z component of the quaternion.
			\return	SI_Float	The Z component of the quaternion.
			\sa CSIBCQuaternion::GetX
			\sa CSIBCQuaternion::GetY
			\sa CSIBCQuaternion::GetW
			\sa CSIBCQuaternion::SetZ
		*/
        SI_Float GetZ() const { return m_fZ; }

		/*! Gets the W component of the quaternion.
			\return	SI_Float	The W component of the quaternion.
			\sa CSIBCQuaternion::GetX
			\sa CSIBCQuaternion::GetY
			\sa CSIBCQuaternion::GetZ
			\sa CSIBCQuaternion::SetW
		*/
        SI_Float GetW() const { return m_fW; }

		/*! Sets the X component of the quaternion.
			\param	in_dX	The new values for the X component of the quaternion.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly set.
			</ul>

			\sa CSIBCQuaternion::Set
			\sa CSIBCQuaternion::SetY
			\sa CSIBCQuaternion::SetZ
			\sa CSIBCQuaternion::SetW
			\sa CSIBCQuaternion::GetX
		*/
		inline SI_Error SetX(SI_Float in_dX);

		/*! Sets the Y component of the quaternion.
			\param	in_dY	The new values for the Y component of the quaternion.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly set.
			</ul>

			\sa CSIBCQuaternion::Set
			\sa CSIBCQuaternion::SetX
			\sa CSIBCQuaternion::SetZ
			\sa CSIBCQuaternion::SetW
			\sa CSIBCQuaternion::GetY
		*/
        inline SI_Error SetY(SI_Float in_dY);

		/*! Sets the Z component of the quaternion.
			\param	in_dZ	The new values for the Z component of the quaternion.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly set.
			</ul>

			\sa CSIBCQuaternion::Set
			\sa CSIBCQuaternion::SetX
			\sa CSIBCQuaternion::SetY
			\sa CSIBCQuaternion::SetW
			\sa CSIBCQuaternion::GetZ
		*/
        inline SI_Error SetZ(SI_Float in_dZ);

		/*! Sets the W component of the quaternion.
			\param	in_dW	The new values for the W component of the quaternion.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly set.
			</ul>

			\sa CSIBCQuaternion::Set
			\sa CSIBCQuaternion::SetX
			\sa CSIBCQuaternion::SetY
			\sa CSIBCQuaternion::SetZ
			\sa CSIBCQuaternion::GetW
		*/
        inline SI_Error SetW(SI_Float in_dW);

		/*! Returns the rotation matrix corresponding to this quaternion. The quaternion must
			be a unit quaternion for this operation to succeed.
			\param	out_mat	Reference to the matrix object to receive the rotation matrix.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly converted.
				<li>SI_ERR_ERROR_MSG - The quaternion was not a unit quaternion.
			</ul>

			\sa CSIBCQuaternion::GetXYZRotation
			\sa CSIBCQuaternion::GetAxisAngle
		*/
		SI_Error GetMatrix(CSIBCMatrix4x4 &out_mat);

		/*! Returns the euler rotation angles (pitch-roll-yaw) represented by this quaternion.
			\param	out_vect	Vector to receive the euler rotation angles.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly converted.
			</ul>

			\sa CSIBCQuaternion::GetMatrix
			\sa CSIBCQuaternion::GetAxisAngle
		*/
        inline SI_Error GetXYZRotation(CSIBCVector3D &out_vect);

		/*! Returns the euler rotation angles (pitch-roll-yaw) represented by this quaternion.
			\param	out_vect	Vector to receive the euler rotation angles.
			\param	in_dX		Ignored.
			\param	in_dY		Ignored.
			\param	in_dZ		Ignored.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly converted.
			</ul>

			\sa CSIBCQuaternion::GetMatrix
			\sa CSIBCQuaternion::GetAxisAngle
		*/
        SI_Error GetXYZRotation(SI_Float in_dX,
                                SI_Float in_dY, 
                                SI_Float in_dZ,
                                CSIBCVector3D &out_vect);

		/*! Gets the axis of rotation, and rotation angle about the axis, that is 
			represented by this quaternion.
			\param	out_vAxis	Receives the axis of rotation.
			\param	out_fAngle	Receives the angle of rotation about the axis (in radians).
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly converted.
				<li>SI_ERR_ERROR_MSG - The quaternion could not be converted, axis components 
					and angle are all zero.
			</ul>

			\sa CSIBCQuaternion::GetXYZRotation
		*/
		SI_Error GetAxisAngle(CSIBCVector3D &out_vAxis,
                              float &out_fAngle);

		/*! Sets all components of the quaternion.
			\param	in_dX	Value for the X component of the quaternion.
			\param	in_dY	Value for the Y component of the quaternion.
			\param	in_dZ	Value for the Z component of the quaternion.
			\param	in_dW	Value for the W component of the quaternion.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion was properly set.
			</ul>

			\sa CSIBCQuaternion::SetX
			\sa CSIBCQuaternion::SetY
			\sa CSIBCQuaternion::SetZ
			\sa CSIBCQuaternion::SetW
		*/
        inline SI_Error Set(SI_Float in_dX,
                            SI_Float in_dY,
                            SI_Float in_dZ,
                            SI_Float in_dW);

		/*! Sets this quaternion equal to \p in_quat.
			\param	in_quat	Quaternion to set this quaternion equal to.
			\sa CSIBCQuaternion::Init(CSIBCQuaternion &)
		*/
        inline void operator=(const CSIBCQuaternion &in_quat);

		/*! Tests to see whether this quaternion is valid. If the quaternion is invalid,
			an assertion will occur.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The quaternion is valid.
			</ul>
		*/
        inline SI_Error SelfTest();

    protected:

    private:

        // Attributes
        SI_Float m_fX, m_fY, m_fZ, m_fW;
};


#endif



