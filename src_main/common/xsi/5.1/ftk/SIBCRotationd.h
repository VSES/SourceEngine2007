//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCRotationd.h	| Main header file for CSIBCRotationd implementation
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

#ifndef _ROTATION_H_
#define _ROTATION_H_

//******************************************************************************
// Includes
//******************************************************************************

#include "SIBCMathLib.h"

//******************************************************************************
// Typedefs
//******************************************************************************

// Utility functions
/*!	\addtogroup	rotutil_group	Utility functions for angles and rotations.
	@{
*/

/*!	Returns the angle corresponding to the given sine and cosine values.
	\param	dSinus		The sine value of the desired angle.
	\param	dCosinus	The cosine value of the desired angle.
	\return	double		The angle (in radians) corresponding to the sine and cosine values.
*/
double F3DArcSinCos( double dSinus, double dCosinus );

/*!	Limits \p io_dVal between \p in_dMin and \p in_dMax.
	\param	io_dVal	Value to limit, receives the result of the limiting.
	\param	in_dMin	The minimum value for \p io_dVal.
	\param	in_dMax	The maximum value for \p io_dVal.
*/
void F3DLimit( double& io_dVal, double in_dMin, double in_dMax );

/*! Converts a quaternion into a rotation matrix.
	\param	in_quat	Quaternion to convert.
	\param	out_mat	Rotation matrix to receive the converted quaternion.
*/
void F3DQuatToMat( const CSIBCQuaterniond& in_quat, CSIBCRotMatd& out_mat );

/*! Converts a rotation matrix into a quaternion.
	\param	in_mat	Rotation matrix to convert.
	\param	out_quat	Quaternion to receive the converted rotation matrix.
*/
void F3DMatToQuat( const CSIBCRotMatd& in_mat, CSIBCQuaterniond& out_quat );

/*! Converts a set of euler angles (pitch-roll-yaw) to a quaternion.
	\param	in_vct		Vector containing euler angles to convert.
	\param	out_quat	Quaternion to receive the converted euler angles.
*/
void F3DEulerToQuat( const CSIBCVector3Dd& in_vct, CSIBCQuaterniond& out_quat );

/*! Converts a set of euler angles (pitch-roll-yaw) to a rotation matrix.
	\param	in_vct		Vector containing euler angles to convert.
	\param	out_mat		Rotation matrix to receive the converted euler angles.
*/
void F3DEulerToMat( const CSIBCVector3Dd& in_vct, CSIBCRotMatd& out_mat ); 

/*! Converts a rotation matrix into a set of euler angles (pitch-roll-yaw).
	\param	in_mat		Rotation matrix to convert.
	\param	out_vct		Vector to receive the euler angles for the converted rotation matrix.
*/
void F3DMatToEuler( const CSIBCRotMatd& in_mat, CSIBCVector3Dd& out_vct );

/*!	Converts a rotation matrix into a set of euler angles (pitch-roll-yaw), which
	are closest to \p inout_vct. This is provided because a single rotation matrix
	produces an infinite number of possible euler angle representations. The result 
	is stored in \p in_outvct.
	\param	in_mat		Rotation matrix to convert.
	\param	inout_vct	Vector containing the euler angles that most closely represent
			the desired output euler angles. Receives the output of the converted
			rotation matrix.
*/
void F3DMatToEulerUsingTarget( const CSIBCRotMatd& in_mat, CSIBCVector3Dd& inout_vct );

/*!	Converts a quaternion to a angle rotation about an axis.
	\param	in_quat		Quaternion to convert.
	\param	out_vctAxis	Vector to receive the rotation axis given by the quaternion.
	\param	out_dAngle	double to receive the rotation about \p out_dAngle (in radians).
*/
void F3DQuatToAxisAngle( const CSIBCQuaterniond& in_quat, CSIBCVector3Dd& out_vctAxis, double& out_dAngle );

/*!	Converts a rotation angle about a given axis to a quaternion.
	\param	in_vctAxis	Axis of rotation.
	\param	in_dAngle	Rotation angle (in radians) of rotation about \p in_vctAngle
	\param	out_quat	Quaternion to receive the converted axis rotation.
*/
void F3DAxisAngleToQuat( const CSIBCVector3Dd& in_vctAxis, const double& in_dAngle, CSIBCQuaterniond& out_quat );

// Angle utility functions

/*!	Returns an angle corresponding to \p in_dDesiredAngle, in the range [-#M_2PI, #M_2PI].
	\param	in_dDesiredAngle	Angle to be bounded.
	\return	double				Angle corresponding to \p in_dDesiredAngle, bounded 
			between [-#M_2PI and #M_2PI].
*/
double F3DBoundAngleInMinus2PITo2PI( double in_dDesiredAngle);

/*!	Returns an angle corresponding to \p in_dAngle, in the range [0.0f, #M_2PI].
	\param	in_dAngle	Angle to be bounded.
	\return	double		Angle corresponding to \p in_dDesiredAngle, bounded between 
			[0.0f,#M_2PI].
*/
void F3DBoundAngleInZeroToTwoPI( double& in_dAngle );

/*!	Returns an angle corresponding to \p in_dAngle, in the range [-#M_2PI, #M_2PI]
	\param	in_dAngle	Angle to be bounded.
	\return	double		Angle corresponding to \p in_dDesiredAngle, bounded between 
			p-#M_PI, #M_PI].
*/
void F3DBoundAngleInMinusPIToPI( double& in_dAngle );

/*! Returns an angle corresponding to \p in_dDesiredAngle, which is as close as possible
	to \p in_dRefAngle.
	\param	in_dDesiredAngle	Desired angle, bounded by [-#M_PI, #M_PI].
	\param	in_dRefAngle		Reference angle, unbounded.
	\return	double				The angle corresponding to \p in_dDesiredAngle, which is 
			as close as possible to \p in_dRefAngle
*/
double F3DGetUnboundedAngle( double in_dDesiredAngle, double in_dRefAngle );

/*!	Bounds the angles in the first column of \p io_matMN, beginning at row \in_nFirstRow,
	and ending at row \p in_nLastRow to angles in the range [-#M_2PI, #M_2PI].
	\param	io_matMN		Matrix containing angles in the first column to be bounded.
	\param	in_nFirstRow	First row of \p io_matMN containing angles to be bound, zero-indexed
			(if negative, starts at the first row).
	\param	in_nLastRow		Last row of \p io_matMN containing angles to be bound, zero-indexed
			(if negative, ends at the last row).
*/
void F3DBoundAnglesInMatrixInMinus2PITo2PI( CSIBCMatrixMNd &io_matMN,	int	in_nFirstRow, int in_nLastRow );

/*! Determines which angle \p in_dAngle is closest to, \p in_dFirstAngle, or \p in_dSecondAngle.
	Note that in_dAngle should not be between \p in_dFirstAngle and \p in_dSecondAngle (in
	counter-clockwise order).
	\param	in_dAngle		Angle for comparison.
	\param	in_dFirstAngle	First angle for close-ness comparison.
	\param	in_dSecondAngle	Second angle for close-ness comparison.
	\return	double			\p in_dFirstAngle or \p in_dSecondAngle, whichever is closest to \p in_dAngle.
*/
double F3DGetClosestAngle( double in_dAngle, double in_dFirstAngle, double in_dSecondAngle );

/*! @} */

//******************************************************************************
//  Mask Values for m_oRotationInfo. These are used internally by CSIBCRotationd,
//	and do not require documentation.
//******************************************************************************

#define MASK_ORIGREP					0x03
#define BITVAL_ORIGREP_QUATERNION		0x00
#define BITVAL_ORIGREP_ROTATION_MATRIX	0x01
#define BITVAL_ORIGREP_EULER_ANGLES		0x02

#define MASK_ISQUAT_DIRTY				(1 << 2 )	//   00000100
#define MASK_ISMAT_DIRTY				(1 << 3 )	//   00001000
#define MASK_ISANGLES_DIRTY				(1 << 4 )	//   00010000

// Used to identify which of the three possible representations of a rotation is used.
typedef enum tagE3DROTREPTYPE
{
   QUATERNION_REP			= BITVAL_ORIGREP_QUATERNION,
   ROTATION_MATRIX_REP		= BITVAL_ORIGREP_ROTATION_MATRIX,
   EULER_ANGLES_REP			= BITVAL_ORIGREP_EULER_ANGLES
} E3DRotRepType;


//******************************************************************************
// CSIBCRotationd | Implementation of a rotation object.
//******************************************************************************

//! Class representing a generic rotation object.
/*!	This class is a generic method for representing rotations. It provides a method
	for representing a rotation without relying on it specific representation. It
	acts as an interface between different methods of storing rotations. For all
	operations on rotation objects, double-precision floating point numbers are used.
	Several useful functions for rotations can be found in \link rotutil_group
	Utility functions for angles and rotations\endlink. 

	There are several different specific methods used to store rotations, such 
	as: rotation matricies as in CSIBCRotMatd, quaternions as in CSIBCQuaternion and 
	CSIBCQuaterniond, euler angles, rotations about axis, etc.
*/

class XSICOREEXPORT CSIBCRotationd
{
	// Public members
	public:
		//****************************************
		// IMPLEMENTATION
		//****************************************

		/*! Default Constructor.
			Creates a rotation object representing the identity rotation.
			\sa CSIBCRotationd::SetIdentity
		*/
		CSIBCRotationd();

		/*! Constructs a rotation object representing the rotation given by the quaternion
			\p in_quat.
			\param	in_quat	Quaternion representing the rotation for the new rotation object.
		*/
		CSIBCRotationd( const CSIBCQuaterniond& in_quat );
		
		/*! Constructs a rotation object representing the rotation given by the rotation
			matrix \p in_matRot.
			\param	in_matRot	Rotation matrix representing the rotation for the new rotation object.
		*/
		CSIBCRotationd( const CSIBCRotMatd& in_matRot );

		/*! Constructs a rotation object representing the rotation given by the set of euler angles
			(pitch-roll-yaw) \p in_vctXYZAngles.
			\param	in_vctXYZAngles	Set of euler angles representing the rotation for the new rotation
					object.
		*/
		CSIBCRotationd( const CSIBCVector3Dd& in_vctXYZAngles );
		
		/*! Constructs a rotation object representing the rotation given by the set of euler angles
			(pitch-roll-yaw) in \p in_dX, \p in_dY and \p in_dZ.
			\param	in_dX	The euler angle containing the pitch angle (in radians).
			\param	in_dY	The euler angle containing the roll angle (in radians).
			\param	in_dZ	The euler angle containing the yaw angle (in radians).
		*/
		CSIBCRotationd( double in_dX, double in_dY, double in_dZ );

		/*! Constructs a rotation object representing the rotation given by the rotation of \p in_dAngle
			radians about one of the major axis, determined by \p in_axis.
			\param	in_axis		Major axis of rotation.
			\param	in_dAngle	Angle of rotation about the given major axis (in radians).
		*/
		CSIBCRotationd( const E3DAxisType in_axis, const double in_dAngle );

		/*! Constructs a rotation object representing the rotation given by the rotation of \p in_dAngle
			radians about an arbitrary axis, \p in_vctAxis.
			\param	in_vctAxis	Axis of rotation.
			\param	in_dAngle	Angle of rotation about the given axis (in radians).
		*/
		CSIBCRotationd( const CSIBCVector3Dd& in_vctAxis,
						  const double in_dAngle );

		/*!	Constructs a rotation object representing the rotation required to rotated \p in_vctFrom to
			\p in_vctTo.
			\param	in_vctFrom	Starting point of rotation.
			\param	in_vctTo	Ending point of rotation.
			\sa CSIBCRotationd::Set(const CSIBCVector3Dd&, const CSIBCVector3Dd&, int&)
		*/
		CSIBCRotationd( const CSIBCVector3Dd& in_vctFrom,
						  const CSIBCVector3Dd& in_vctTo );

		~CSIBCRotationd();

		/*! Set this rotation to the identity rotation (no rotation).
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa	CSIBCRotationd::IsIdentity
			\sa CSIBCRotationd::IsAlmostIdentity
		*/
		CSIBCRotationd& SetIdentity();

		/*! Determines whether this rotation is the identity rotation (no rotation). Note
			that this function uses no floating-point error range, meaning that the rotation
			object must be exactly the identity rotation.
			\return	bool	true if the rotation object is the identity rotation, false otherwise.
			\sa CSIBCRotationd::IsAlmostIdentity
			\sa CSIBCRotationd::SetIdentity
		*/	
		bool IsIdentity();

		/*! Determines whether this rotation is the identity rotation (no rotation). Since this 
			function uses floating-point comparison, a threshhold value of \p in_dEpsilon is used 
			for equality comparison.

			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool		true if the rotation object is the identity rotation, false otherwise.
			\sa CSIBCRotationd::IsIdentity
			\sa CSIBCRotationd::SetIdentity
		*/
		bool IsAlmostIdentity(const double& in_rdTolerance = PICO_EPS) const;

		/*!	Gets the quaternion representation of this rotation object.
			\param	out_quat			Receives the quaternion representation of this object.
			\return	CISBCQuaterniond&	Reference to \p out_quat.
			\sa CSIBCRotationd::GetMatrix
			\sa CSIBCRotationd::GetXYZAngles
			\sa CSIBCRotationd::GetAxisAngles
		*/
		CSIBCQuaterniond& GetQuat( CSIBCQuaterniond& out_quat );

		/*!	Gets the rotation matrix representation of this rotation object. Note that
			the return value of this function should not be freed.
			\return	CSIBCRotMatd*	Pointer to the rotation matrix representation of this 
					rotation object.
			\sa CSIBCRotationd::GetQuat
			\sa CSIBCRotationd::GetMatrix
			\sa CSIBCRotationd::GetXYZAngles
			\sa CSIBCRotationd::GetAxisAngles
		*/
		const CSIBCRotMatd* GetMatrix();

		/*!	Gets the rotation matrix representation of this rotation object. 
			\param	out_matRot		Receives the rotation matrix representation of this
					rotation object.
			\return	CSIBCRotMatd&	Reference to \p out_matRot.
			\sa CSIBCRotationd::GetQuat
			\sa CSIBCRotationd::GetMatrix
			\sa CSIBCRotationd::GetXYZAngles
			\sa CSIBCRotationd::GetAxisAngles
		*/
		CSIBCRotMatd& GetMatrix( CSIBCRotMatd& out_matRot );

		/*! Gets the euler angles representation of this rotation object 
			(pitch-roll-yaw, in radians).
			\param	out_vctXYZAngles	Receives the euler angles representation of this
					rotation object.
			\return	CSIBCVector3Dd&		Reference to \p out_vctXYZAngles.
			\sa CSIBCRotationd::GetXYZAngles
			\sa CSIBCRotationd::GetQuat
			\sa CSIBCRotationd::GetMatrix
			\sa CSIBCRotationd::GetAxisAngles
		*/
		CSIBCVector3Dd& GetXYZAngles( CSIBCVector3Dd& out_vctXYZAngles );

		/*! Gets the euler angles representation of this rotation object
			(pitch-roll-yaw in radians).
			\param	out_dX	Receives the pitch euler angle representation of this rotation object.
			\param	out_dY	Receives the roll euler angle representation of this rotation object.
			\param	out_dZ	Receives the yaw euler angle representation of this rotation object.
			\sa CSIBCRotationd::GetXYZAngles
			\sa CSIBCRotationd::GetQuat
			\sa CSIBCRotationd::GetMatrix
			\sa CSIBCRotationd::GetAxisAngles
		*/
		void GetXYZAngles( double& out_dX, double& out_dY, double& out_dZ );

		/*!	Gets the axis rotation representation of this rotation object.
			\param	out_vctAxis	Receives the arbitrary axis of rotation of this rotation object.
			\param	out_dAngle	Receives the angle of rotation (in radians) about the axis of
					rotation for this rotation object.
			\sa CSIBCRotationd::GetXYZAngles
			\sa CSIBCRotationd::GetQuat
			\sa CSIBCRotationd::GetMatrix
		*/
		void GetAxisAngle( CSIBCVector3Dd& out_vctAxis, double& out_dAngle );

		/*!	Sets this rotation object from another rotation object \p in_rot.
			\param	in_rot	Rotation object to set this object equal to.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Set
		*/
		CSIBCRotationd& Set( const CSIBCRotationd& in_rot );

		/*!	Sets this rotation object from a quaternionion, \p in_quat.
			\param	in_rot	Quaternion representing the rotation to set this object equal to.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Set
		*/
		CSIBCRotationd& Set( const CSIBCQuaterniond& in_quat );

		/*!	Sets this rotation object from a rotation matrix, \p in_matRot.
			\param	in_rot	Rotation matrix representing the rotation to set this object equal to.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Set
		*/
		CSIBCRotationd& Set( const CSIBCRotMatd& in_matRot );

		/*!	Sets this rotation object from a set of euler angles (pitch-roll-yaw, in 
			radians), \p in_vctXYZAngles.
			\param	in_vctXYZAngles	Sets of euler angles representing the rotation to set this 
					object equal to.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Set
		*/
		CSIBCRotationd& Set( const CSIBCVector3Dd& in_vctXYZAngles );

		/*!	Sets this rotation object from a set of euler angles (pitch-roll-yaw, in 
			radians), \p in_dX, \p in_dY and \p in_dZ.
			\param	in_dX	Pitch euler angle representing the rotation to set this object equal to.
			\param	in_dY	Roll euler angle representing the rotation to set this object equal to.
			\param	in_dZ	Yaw euler angle representing the rotation to set this object equal to.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Set
		*/
		CSIBCRotationd& Set( double in_dX, double in_dY, double in_dZ );

		/*! Sets this rotation object from a rotation angle \p in_dAngle about a major axis given
			by \p in_axis.
			\param	in_axis		Axis of rotation.
			\param	in_dAngle	Angle of rotation about \p in_axis to set this rotation 
					object equal to.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Set
		*/
		CSIBCRotationd& Set( const E3DAxisType in_axis, const double in_dAngle );

		/*!	Sets this rotation object from a rotation angle \p in_dAngle about an arbitrary axis
			given by \p in_vctAxis.
			\param	in_vctAxis	Axis of rotation.
			\param	in_dAngle	Angle of rotation about \p in_vctAxis to set this rotation 
					objecd equal to.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Set
		*/
		CSIBCRotationd& Set( const CSIBCVector3Dd& in_vctAxis, const double in_dAngle );


		/*! Sets this object to a rotation that will rotate around the normal of the plane 
			defined by the two given vectors \p in_vctFrom and \p in_vctTo (right-hand rule), 
			with an angle defined by the angle between those two vectors.
			\param	in_vctFrom	Starting point of rotation.
			\param	in_vctTo	Ending point of rotation.
			\param	out_bFlip	Set to true if \p in_vctFrom and \p in_vctTo are diametrically opposed,
					in this case, an arbitrary rotation is chosen between \p in_vctFrom and \p in_vctTo,
					false otherwise.
			\return	bool		true if the operation was successful and the rotation object was set,
					false if one or both of the input vectors is the null vector.
			\sa CSIBCRotationd::Set
		*/
		bool Set( const CSIBCVector3Dd& in_vctFrom, const CSIBCVector3Dd& in_vctTo, int& out_bFlip );

		/*!	Sets this rotation object to the rotation defined by the X, Y and Z axis of a rotation
			given by \p in_vctX, \p in_vctY and \p in_vctZ respectively. It is up to the user to
			ensure that the axis are unity vectors orthogonal to each other.
			\param	in_vctX		X-axis for the rotation to set this rotation object equal to.
			\param	in_vctY		Y-axis for the rotation to set this rotation object equal to.
			\param	in_vctZ		Z-axis for the rotation to set this rotation object equal to.
		*/
		CSIBCRotationd& Set( const CSIBCVector3Dd& in_vctX, const CSIBCVector3Dd& in_vctY,
						const CSIBCVector3Dd& in_vctZ );

		/*! Sets this rotation object to align with a given X world axis, using
			the minimal angular displacement.
			\param	in_vct		X-axis for the world vector to align with.
			\param	out_bFlip	Set to true if the current rotation object is diametrically
					opposed to \p in_vct, and indicates that a sudden flip in the rotation
					object has occurred, false otherwise.
			\return	bool		true if the alignment was successful, false if the input
					vector \p in_vct is the null vector.
			\sa CSIBCRotationd::MinAlignToY
			\sa CSIBCRotationd::MinAlignToZ
			\sa CSIBCRotationd::AlignToX
			\sa CSIBCRotationd::AlignAxesToVct
		*/					
		bool MinAlignToX( const CSIBCVector3Dd& in_vct, int& out_bFlip );

		/*! Sets this rotation object to align with a given Y world axis, using
			the minimal angular displacement.
			\param	in_vct		Y-axis for the world vector to align with.
			\param	out_bFlip	Set to true if the current rotation object is diametrically
					opposed to \p in_vct, and indicates that a sudden flip in the rotation
					object has occurred, false otherwise.
			\return	bool		true if the alignment was successful, false if the input
					vector \p in_vct is the null vector.
			\sa CSIBCRotationd::MinAlignToX
			\sa CSIBCRotationd::MinAlignToZ
			\sa CSIBCRotationd::AlignToY
			\sa CSIBCRotationd::AlignAxesToVct
		*/					
		bool MinAlignToY( const CSIBCVector3Dd& in_vct, int& out_bFlip );

		/*! Sets this rotation object to align with a given Z world axis, using
			the minimal angular displacement.
			\param	in_vct		Z-axis for the world vector to align with.
			\param	out_bFlip	Set to true if the current rotation object is diametrically
					opposed to \p in_vct, and indicates that a sudden flip in the rotation
					object has occurred, false otherwise.
			\return	bool		true if the alignment was successful, false if the input
					vector \p in_vct is the null vector.
			\sa CSIBCRotationd::MinAlignToX
			\sa CSIBCRotationd::MinAlignToY
			\sa CSIBCRotationd::AlignToZ
			\sa CSIBCRotationd::AlignAxesToVct
		*/					
		bool MinAlignToZ( const CSIBCVector3Dd& in_vct, int& out_bFlip );

		/*! Aligns this rotation object to align with a given X world axis.
			\param	in_vct		X-axis for the world vector to align with.
			\param	out_bFlip	Set to true if the current rotation object is diametrically
					opposed to \p in_vct, and indicates that a sudden flip in the rotation
					object has occurred, false otherwise.
			\return	bool		true if the alignment was successful, false if the input
					vector \p in_vct is the null vector.
			\sa CSIBCRotationd::AlignToY
			\sa CSIBCRotationd::AlignToZ
			\sa CSIBCRotationd::MinAlignToX
			\sa CSIBCRotationd::AlignAxesToVct
		*/					
		bool AlignToX( const CSIBCVector3Dd& in_vct, int& out_bFlip );

		/*! Aligns this rotation object to align with a given Y world axis.
			\param	in_vct		Y-axis for the world vector to align with.
			\param	out_bFlip	Set to true if the current rotation object is diametrically
					opposed to \p in_vct, and indicates that a sudden flip in the rotation
					object has occurred, false otherwise.
			\return	bool		true if the alignment was successful, false if the input
					vector \p in_vct is the null vector.
			\sa CSIBCRotationd::AlignToX
			\sa CSIBCRotationd::AlignToZ
			\sa CSIBCRotationd::MinAlignToY
			\sa CSIBCRotationd::AlignAxesToVct
		*/					
		bool AlignToY( const CSIBCVector3Dd& in_vct, int& out_bFlip );

		/*! Aligns this rotation object to align with a given Z world axis.
			\param	in_vct		Z-axis for the world vector to align with.
			\param	out_bFlip	Set to true if the current rotation object is diametrically
					opposed to \p in_vct, and indicates that a sudden flip in the rotation
					object has occurred, false otherwise.
			\return	bool		true if the alignment was successful, false if the input
					vector \p in_vct is the null vector.
			\sa CSIBCRotationd::AlignToX
			\sa CSIBCRotationd::AlignToY
			\sa CSIBCRotationd::MinAlignToZ
			\sa CSIBCRotationd::AlignAxesToVct
		*/					
		bool AlignToZ( const CSIBCVector3Dd& in_vct, int& out_bFlip );

		/*! Aligns this rotation object first with the major axis given by \p in_eFirstAxis,
			with an axis vector of \p in_vct1, then aligns the rotation object with the
			major axis given by \p in_eSecondAxis, with an axis vector of \p in_vct2. 

			\param	in_eFirstAxis	The major axis of the first alignment.
			\param	in_vct1			The world vector for the major axis defined by \p in_eFirstAxis.
			\param	in_eSecondAxis	The major axis of the secodn alignment.
			\param	in_vct2			The world vector for the major axis defined by \p in_eSecondAxis.
			\param	out_bFlip		Set to true if the current rotation object is diametrically
					opposed to \p in_vct1, during the first alignment, or \p in_vct2, during the
					second alignment. If true, indicates that a sudden flip in the rotation object has 
					occurred, false otherwise.
			\return	bool			true if the alignment was successful, false if the alignment
					axes in \p in_eFirstAxis and \p in_eSecondAxis are the same.
			\sa CSIBCRotationd::AlignToX
			\sa CSIBCRotationd::AlignToY
			\sa CSIBCRotationd::AlignToZ
		*/
		bool AlignAxesToVct( const E3DAxisType in_eFirstAxis,
						   const CSIBCVector3Dd& in_vct1,
						   const E3DAxisType in_eSecondAxis,
						   const CSIBCVector3Dd& in_vct2,
						   int& out_bFlip );

		/*!	Computes the multiplication of \p in_rot1 and \p in_rot2 (in_rot1 * in_rot2),
			and stores the result in this rotation object.
			\param	in_rot1	First rotation object in the multiplication (on the left).
			\param	in_rot2	Second rotation object in the multiplication (on the right).
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa	CSIBCRotationd::Mul
			\sa	CSIBCRotationd::LeftMul
			\sa CSIBCRotationd::MulUsingEulerAnglesTarget
			\sa CSIBCRotationd::MulAndUpdateEulerAngles
		*/
		CSIBCRotationd& Mul( CSIBCRotationd& in_rot1, CSIBCRotationd& in_rot2 ); 

		/*!	Computes the multiplication of this rotation object and \p in_rot (this * in_rot),
			and stores the result in this rotation object.
			\param	in_rot	Rotation object in the multiplication (on the right).
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa	CSIBCRotationd::Mul
			\sa	CSIBCRotationd::LeftMul
			\sa CSIBCRotationd::MulUsingEulerAnglesTarget
			\sa CSIBCRotationd::MulAndUpdateEulerAngles
		*/
		CSIBCRotationd& Mul( CSIBCRotationd& in_rot );

		/*!	Computes the multiplication of \p in_rot and this rotation object (in_rot * this),
			and stores the result in this rotation object.
			\param	in_rot	Rotation object in the multiplication (on the left).
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa	CSIBCRotationd::Mul
			\sa	CSIBCRotationd::LeftMul
			\sa CSIBCRotationd::MulUsingEulerAnglesTarget
			\sa CSIBCRotationd::MulAndUpdateEulerAngles
		*/
		CSIBCRotationd& LeftMul( CSIBCRotationd& in_rot );

		/*! Computes the multiplication of \p in_rot1 and \p in_rot2 (in_rot1 * in_rot2),
			and stores the result in this rotation object. The result sets the euler angles
			associated with the resultant rotation object closest to the set of euler angles
			given by \p in_oEulerAngles.
			\param	in_rot1	First rotation object in the multiplication (on the left).
			\param	in_rot2	Second rotation object in the multiplication (on the right).
			\param	in_oEulerAngles	Euler angles most closely representing the desired
					euler angles (used only with euler angle extraction of this object). 
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa	CSIBCRotationd::Mul
			\sa	CSIBCRotationd::LeftMul
			\sa CSIBCRotationd::MulAndUpdateEulerAngles
		*/
		CSIBCRotationd& MulUsingEulerAnglesTarget( CSIBCRotationd& in_rot1, 
											  CSIBCRotationd& in_rot2,
											  CSIBCVector3Dd& in_oEulerAngles );


		/*!	Computes the multiplication of \p in_rot1 and \p in_rot2 (in_rot1 * in_rot2),
			and stores the result in this rotation object. The result sets the euler angles
			associated with the resultant rotation object closest to the set of euler angles
			given by \p in_rot1 or \p in_rot2 depending on the value of \p in_ulUseRot1OrUseRot2AsTarget.

			\param	in_rot1	First rotation object in the multiplication (on the left).
			\param	in_rot2	Second rotation object in the multiplication (on the right).
			\param	in_ulUseRot1OrUseRot2AsTarget	If 1, uses \p in_rot1 as the euler angle target,
					if 2, uses \p in_rot2 as the euler angle target.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa	CSIBCRotationd::Mul
			\sa	CSIBCRotationd::LeftMul
			\sa CSIBCRotationd::MulUsingEulerAnglesTarget
		*/
		CSIBCRotationd& MulAndUpdateEulerAngles( CSIBCRotationd& in_rot1, 
		                                    CSIBCRotationd& in_rot2, 
											unsigned long in_ulUseRot1OrUseRot2AsTarget); 


		/*!	Sets this rotation object to the inverse of \p in_rot.
			\param	in_rot	Rotation object containing the inverse to set as this rotation object.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Invert
		*/
		CSIBCRotationd& Invert( CSIBCRotationd& in_rot );

		/*!	Sets this rotation object to its inverse.
			\return	CSIBCRotationd&	Reference to this rotation object.
			\sa CSIBCRotationd::Invert
		*/
		CSIBCRotationd& Invert();

		/*!	Sets this rotation object to the interpolation between the rotation objects
			\p in_rot1, and \p in_rot2 at a point of \p in_dU between them.
			\param	in_rot1	Starting point for rotation interpolation.
			\param	in_rot2	Ending point for rotation interpolation.
			\param	in_dU	Interpolation between \p in_rot1 and \p in_rot2, in the range [0.0f, 1.0f].
			\return	CSIBCRotationd&	Reference to this rotation object.
		*/
		CSIBCRotationd& Interpolate( CSIBCRotationd& in_rot1,
								CSIBCRotationd& in_rot2,
								const double in_dU );

		/*!	Applies this rotation object to a single vector.
			\param	in_vct	The vector to be rotated.
			\param	out_vct	Receives the rotated vector.
			\return	CSIBCVector3Dd&	Reference to \p out_vct.
			\sa CSIBCRotationd::RotateVct
			\sa CSIBCRotationd::InvRotateVct
		*/
		CSIBCVector3Dd& RotateVct( const CSIBCVector3Dd& in_vct, CSIBCVector3Dd& out_vct );

		/*!	Applies this rotation object to multiple vectors.
			\param	in_vcts		Array of vectors.
			\param	in_lNbVcts	Number of vectors in \p in_vcts (and \p out_vcts).
			\param	out_vcts	Array of vectors receiving the array rotated vectors.
			\sa CSIBCRotationd::RotateVct
			\sa CSIBCRotationd::InvRotateVct
		*/
		void RotateVct( const CSIBCVector3Dd *in_vcts,
					  const long in_lNbVcts,
					  CSIBCVector3Dd *out_vcts );

		/*! Applies the inverse of this rotation object to a single vector.
			\param	in_vct	The vector to be rotated.
			\param	out_vct	Receives the rotated vector.
			\return	CSIBCVector3Dd&	Reference to \p out_vct.
			\sa CSIBCRotationd::InvRotateVct
			\sa CSIBCRotationd::RotateVct
		*/
		CSIBCVector3Dd& InvRotateVct( const CSIBCVector3Dd& in_vct, CSIBCVector3Dd& out_vct );

		/*!	Applies the inverse of this rotation object to multiple vectors.
			\param	in_vcts		Array of vectors.
			\param	in_lNbVcts	Number of vectors in \p in_vcts (and \p out_vcts).
			\param	out_vcts	Array of vectors receiving the array rotated vectors.
			\sa CSIBCRotationd::InvRotateVct
			\sa CSIBCRotationd::RotateVct
		*/
		void InvRotateVct( const CSIBCVector3Dd *in_vcts,
						 const long in_lNbVcts,
						 CSIBCVector3Dd *out_vcts );

		/*!	Maps a direction vector \p in_vctDir from an orientation space, given by
			\p in_rotFrom, to world orientation space.
			\param	in_rotFrom	Rotation object representing the orientation space which
					\p in_vctDir is defined in.
			\param	in_vctDir	The direction vector, defined in \p in_rotFrom.
			\param	out_vctDir	Receives \p in_vctDir mapped from \p in_rotFrom to world
					orientation space.
			return	CSIBCVector3Dd&	Reference to \p out_vctDir
			\sa CSIBCRotationd::MapDirWorldToSpace
			\sa CSIBCRotationd::MapDirSpaceToSpace
		*/
        friend CSIBCVector3Dd& MapDirSpaceToWorld(
                                             CSIBCRotationd& in_rotFrom,
                                             const CSIBCVector3Dd& in_vctDir,
                                             CSIBCVector3Dd& out_vctDir );

		/*!	Maps a direction vector \p in_vctDir from world orientation space, to an
			orientation space given by \p in_rotTo.
			\param	in_vctTo	The orientation space to map \p in_vctDir to.
			\param	in_vctDir	The direction vector, defined in world orientation space.
			\param	out_vctDir	Receives \p in_vctDir mapped from world	orientation space 
					to \p in_rotFrom.
			return	CSIBCVector3Dd&	Reference to \p out_vctDir
			\sa CSIBCRotationd::MapDirSpaceToWorld
			\sa CSIBCRotationd::MapDirSpaceToSpace
		*/
        friend CSIBCVector3Dd& MapDirWorldToSpace(
                                             CSIBCRotationd& in_rotTo,
                                             const CSIBCVector3Dd& in_vctDir,
                                             CSIBCVector3Dd& out_vctDir );

		/*!	Maps a direction vector \p in_vctDir from one orientation space \p in_rotFrom, 
			to an another orientation space \p in_rotTo.
			\param	in_rotFrom	Rotation object representing the orientation space which
					\p in_vctDir is defined in.
			\param	in_vctTo	The orientation space to map \p in_vctDir to.
			\param	in_vctDir	The direction vector, defined in world orientation space.
			\param	out_vctDir	Receives \p in_vctDir mapped from world	orientation space 
					to \p in_rotFrom.
			return	CSIBCVector3Dd&	Reference to \p out_vctDir
			\sa CSIBCRotationd::MapDirSpaceToWorld
			\sa CSIBCRotationd::MapDirWorldToSpace
		*/
        friend CSIBCVector3Dd& MapDirSpaceToSpace(
                                             CSIBCRotationd& in_rotFrom,
                                             CSIBCRotationd& in_rotTo,
                                             const CSIBCVector3Dd& in_vctDir,
                                             CSIBCVector3Dd& out_vctDir );

		//////////////////////////////////////////
		// TO BE IMPLEMENTED LATER IF NEEDED... //
		//////////////////////////////////////////

		// IMPORTANT: In the following tests, don't forget to compare q1 to q2
		//            and q1 to -q2 if quaternion representation is used.

		// AreAlmostEqual		| Verify if two rotations are equal within epsilon.
		/*! Not implemented yet */
		friend bool AreAlmostEqual( const CSIBCRotationd& in_rot1,
								  const CSIBCRotationd& in_rot2,
								  const double in_dEpsilon = PICO_EPS );

		// operator==			| Verify equality between rotations.
		/*! Not implemented yet */
		bool operator ==( const CSIBCRotationd& in_rot ) const;

		// operator!=			| Verify non-equality between rotations.
		/*! Not implemented yet */
		bool operator !=( const CSIBCRotationd& in_rot ) const;


	// Protected members
	protected:

		//****************************************
		// IMPLEMENTATION
		//****************************************

		// SetMatrixRep       | Set rotation matrix representation as the
		//						original one and raise dirty flags of
		//						other representations.

		void SetQuatRep();

		// SetQuatRep         | Set quaternion representation as the
		//						original one and raise dirty flags of
		//						other representations.
		void SetMatrixRep();

		// SetXYZAnglesRep    | Set Euler angles representation as the
		//						original one and raise dirty flags of
		//						other representations.
		void SetXYZAnglesRep();

		// UpdateQuat         | Update the quaternion representation
		//						from the original representation.
		void UpdateQuat();

		// UpdateMatrix       | Update the rotation matrix representation
		//						from the original representation.
		void UpdateMatrix();
		
		// UpdateXYZAngles    | Update the Euler angles representation
		//						from the original representation.
		void UpdateXYZAngles();

		// NextAxis           | Return the next axis of the given one
		//						(in the order X, Y, Z).
		int NextAxis( int in_nAxis );

	// Private members
	private:

		//****************************************
		// ATTRIBUTES
		//****************************************
		// CSIBCQuaterniond | CSIBCRotationd | m_quat | Quaternion representation
		//          of the rotation (unitary quaternion).
		// CSIBCRotMatd | CSIBCRotationd | m_matRot | Rotation matrix
		//          representation.
		// CSIBCVector3Dd | CSIBCRotationd | m_vctXYZAngles | XYZ Euler angles
		//          representation.
		// double | CSIBCRotationd | m_dAngle | Original rotation angle
		//          used in the quaternion representation.
		CSIBCQuaterniond m_quat;
		CSIBCRotMatd m_matRot;
		CSIBCVector3Dd m_vctXYZAngles;
		double m_dAngle;

		//***********************************************************************
		// m_oRotationInfo comprises formely known m_bQuatDirty, m_bMatDirty
		// and m_bAnglesDirty and the enum m_eOrigRep.
		// Bit 0 and 1 = used for enum m_eOrigRep of type E3DRotRepType
		// E3DRotRepType : QUATERNION_REP		== value 0 ( bit 0 and  1 set to 0 )			//00
		//				   ROTATION_MATRIX_REP	== value 1 ( bit 0 set to 1, bit 1 set to 0 )	//01
		//				   EULER_ANGLES_REP		== value 2 ( bit 0 set to 0, bit 1 set to 1 )	//10
		// Bit 2 = m_bQuatDirty
		// Bit 3 = m_bMatDirty
		// Bit 4 = m_bAnglesDirty
		// Bit 5 to 7 = unused

		unsigned char m_oRotationInfo;

		 void _SetQuatDirty(bool in_bIsDirty );
		 void _SetMatDirty(bool in_bIsDirty );
		 void _SetAnglesDirty(bool in_bIsDirty );
		 void _SetOrigRep(E3DRotRepType in_eOrigRep );

		 bool _IsQuatDirty () const;
		 bool _IsMatDirty () const;
		 bool _AreAnglesDirty () const;
		 E3DRotRepType _GetOrigRep() const;

		// Disable copy constructor and assignment operator:
		CSIBCRotationd( const CSIBCRotationd& in_rot );
		CSIBCRotationd& operator =( const CSIBCRotationd& in_rot );
};

#endif
