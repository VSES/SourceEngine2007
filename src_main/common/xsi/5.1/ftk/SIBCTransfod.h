//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
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
#ifndef _TRANSFO_H_
#define _TRANSFO_H_

//******************************************************************************
// Includes
//******************************************************************************
#include "SIBCMathLib.h"

//******************************************************************************
// Typedefs
//******************************************************************************


/*! \defgroup TransformationFlags 
	Flags that determine the applicability of transformation operation.
*/
/*@{*/

/*! Scaling factors */
const unsigned char oSiTransfoSclFac				=	0x01;

/*! Scaling rotation */
const unsigned char oSiTransfoSclRot				=	0x02;

/*! Scaling factors  and scaling rotation*/
const unsigned char oSiTransfoAllScl				=	0x03;

/*! Rotation */
const unsigned char oSiTransfoRot					=	0x04;

/*! Transformation */
const unsigned char oSiTransfoTrs					=	0x08;

/*! Transformation affacted by scaling */
const unsigned char oSiTransfoTrsAffectedByScl		=	0x10;	// Used only when combining transformations

/*! Transformation affacted by orientation */
const unsigned char oSiTransfoTrsAffectedByOri		=	0x20;	// Used only when combining transformations

/*! All transformation */
const unsigned char oSiTransfoAllTrs				=	0x38;

/*! None */
const unsigned char oSiTransfoNone					=	0x00;

/*! Everything */
const unsigned char oSiTransfoAll					=	0x3F;
/*@}*/

//! Types of scaling.
typedef enum tagE3DSCLTYPE
{
   XYZ_SCL_TYPE,		/*!< XYZ Scale Type	*/
   UNIFORM_SCL_TYPE,	/*!< Uniform Scale Type */
   VOLUMIC_SCL_TYPE		/*!< Volumetric Scale Type */
} E3DSclType;

//! Referential types for transformations.
typedef enum tagE3DTRANSFOSPACE
{
   LOCAL_TRANSFO_SPACE,		/*!< Local Transform Space */
   PARENT_TRANSFO_SPACE,	/*!< Parent Transform Space */
   OTHER_TRANSFO_SPACE		/*!< Other Transform Space */
} E3DTransfoSpace;

//! Types of vectors that could be transformed.
typedef enum tagE3DVECTORTYPE
{
   POINT_VECTOR_TYPE,      /*!< Affected by all S, R and T. */
   LINE_VECTOR_TYPE,       /*!< Affected by R and T only. */
   DIRECTION_VECTOR_TYPE   /*!< Affected by R only. */
} E3DVectorType;


//******************************************************************************
//  Mask Values for m_oTransfoInfo. These are used internally by CSIBCTransfod,
//	and thus do not require documentation.
//******************************************************************************
#define MASK_ARE_COMPONENTS_DIRTY		(1 << 0 )	// 0x1  00000001
#define MASK_IS_XFOMAT_DIRTY			(1 << 1 )	// 0x2  00000010
#define MASK_IS_SI_SCALING				(1 << 2 )	// 0x4  00000100

/*!	Class describing an SRT transformation of an object.
*/
class XSICOREEXPORT CSIBCTransfod
{
	public:
		/*! Default constructor for identity transformation.
		*/
		CSIBCTransfod();

		/*! Constructor from individual components
			\param in_vctScal Scaling factors
			\param in_rotScl Scaling orientation,
			\param in_rot Orientation 
			\param in_vctTrs Translation
		*/
		CSIBCTransfod( 
					const CSIBCVector3Dd&	in_vctScl,
					const CSIBCRotationd&	in_rotScl,
					const CSIBCRotationd&	in_rot,
					const CSIBCVector3Dd&	in_vctTrs );

		/*! Constructor
			\param in_vctScl Scaling factors
			\param in_rot Rotation
			\param in_vctTrs Translation
		*/
		CSIBCTransfod(	
					const CSIBCVector3Dd&	in_vctScl,
					const CSIBCRotationd&	in_rot,
					const CSIBCVector3Dd&	in_vctTrs );

		/*! Constructor
			\param in_vctScl Scaling factors
			\param in_rotScl Scaling orientation
		*/
		 CSIBCTransfod( 
					const CSIBCVector3Dd&	in_vctScl,
					const CSIBCRotationd&	in_rotScl );

		/*! Gets the softimage scaling flag
			\retval true Classical scaling
			\retval false Proportional scaling
		*/
		bool GetSIScalingFlag() const;

		/*! Sets the softimage scaling flag
			\param in_bSIScaling \b true for classical scaling, \b false for proportional scaling
			\return CSIBCTransfod Reference to this transformation
		*/
		CSIBCTransfod& SetSIScalingFlag( const bool in_bSIScaling );

		/*! Queries for the existence of scaling orientation in this transformation.
			\return Wheter or not scaling orientation is present
		*/
		 bool IsScalingOrientationExisting() const;

		///////////////////////////////////////////////////////////////////////
		// TODO  The following Get/Set functions assume that the referential //
		//       of this transformation object and of the given/received     //
		//       value are the same. We should provide some methods to       //
		//       Get/Set one or many transformation components in different  //
		//       referential spaces.                                         //
		///////////////////////////////////////////////////////////////////////

		 /*! Get the scaling factors component
			\param out_vctScl Output vector for scaling factors.
			\return CSIBCVector3Dd& Reference to \p out_vctScl
		 */
		 CSIBCVector3Dd&	GetScaling( CSIBCVector3Dd& out_vctScl );

		 /*! Get the scaling factors component
			\param out_dX X scaling factor.
			\param out_dY Y scaling factor.
			\param out_dZ Z scaling factor.
		 */
		 void			GetScaling( 
												double& out_dX, 
												double& out_dY, 
												double& out_dZ );

		/*! Gets the scaling factors and scaling orientation components.
			\param out_vctScl Output vector for scaling factors.
			\param out_rotSclOri Output rotation for scaling.
		*/
		void			GetScaling( 
												CSIBCVector3Dd& out_vctScl,
												CSIBCRotationd& out_rotSclOri );

		/*! Gets the scaling orientation component.
			\param in_rotSclOri Scaling orientation
			\return CSIBCRotationd Reference to \p in_rotSclOri
		*/
		 CSIBCRotationd&	GetScalingOrientation( CSIBCRotationd& in_rotSclOri );

		/*! Gets the scaling orientation component.
			\param out_vctSclOri Scaling orientation vector.
			\return CSIBCRotationd Reference to \p out_vctSclOri
		*/
		 CSIBCVector3Dd&	GetScalingOrientation( CSIBCVector3Dd&  out_vctSclOri );

		/*! Gets the scaling orientation component.
			\param out_rdX X Euler angle in radians.
			\param out_rdY Y Euler angle in radians.
			\param out_rdZ Z Euler angle in radians.
		*/
		 void			GetScalingOrientation( 
															double& out_rdX,
															double& out_rdY,
															double&	out_rdZ );

		/*! Gets the rotation component.
			\param out_rout The rotation component
			\return CSIBCRotationd Reference to \p out_rot
		*/
		 CSIBCRotationd&		GetRotation( CSIBCRotationd& out_rot );

		/*! Gets the rotation component under quaternion representation.
			\param out_quat Quaternion representation.
			\return CSIBCQuaterniond Reference to \p out_quat
		*/
		CSIBCQuaterniond&		GetRotation( CSIBCQuaterniond& out_quat );

		/*! Gets the rotation component under rotation matrix representation.
			\param out_matRot Rotation matrix representation.
			\return CSIBCRotMatd Reference to \p out_matRot
		*/
		CSIBCRotMatd&			GetRotation( CSIBCRotMatd& out_matRot );

		/*! Gets the pointer to the internal rotation matrix representation of
			the rotation component.
			\return CSIBCRotMatd Constant pointer to a rotation matrix that
			represents the rotation.
			\note Doing this is much faster than copying the whole matrix into 
			another object when no changes have to be done on it.  However,
			you have to be careful when using this function.  Make sure the
			pointer it returns is not stored permanently in a way that it
			becomes invalid (when the object is destroyed for example).
		*/
		const CSIBCRotMatd*	GetRotation();

		/*! Gets the rotation component under Euler angles representation.
			\param out_vctXYZAngles Euler angles representation.
			\return CSIBCVector3Dd Reference to \p out_vctXYZAngles
		*/
		CSIBCVector3Dd&		GetRotation( CSIBCVector3Dd& out_vctXYZAngles );

		/*! Gets rotation component under Euler angles representation.
			\param out_dX X Euler angle in radians.
			\param out_dY Y Euler angle in radians.
			\param out_dZ Z Euler angle in radians.
		*/
		void					GetRotation( 
													double& out_dX, 
													double& out_dY, 
													double& out_dZ );

		/*! Gets the rotation component under axis-angle representation.
			\param out_vctAxis Axis
			\param out_dAngle Angle
		*/
		void					GetRotation(
													CSIBCVector3Dd& out_vctAxis, 
													double& out_dAngle );

		/*! Gets the translation component.
			\param out_vctTrs Translation vector
			\return CSIBCVector3Dd Reference to \p out_vctTrs
		*/
		 CSIBCVector3Dd&	GetTranslation( CSIBCVector3Dd& out_vctTrs );

		/*! Gets the translation component.
			\param out_dX X translation.
			\param out_dY Y translation.
			\param out_dZ Z translation.
		*/
		 void			GetTranslation(
													double& out_dX, 
													double& out_dY, 
													double& out_dZ );

		 /*! Gets the transformation matrix
			\return CSIBCXfoMatd Pointer to the internally stored transformation matrix
			\note Much faster than getting the matrix itself (see the other
			implementation of GetMatrix) when no changes have to be done on it.
			BUT PLEASE BE CAREFUL WHEN USING THIS FUNCTION. Make sure the
			pointer it returns is not stored permanently in a way that it
			becomes invalid (when the object is destroyed for example).
		 */
		 const CSIBCXfoMatd*	GetMatrix();


		 /*! Gets a copy of the internal matrix representation of this transformation
			\param out_matXfo Destination for the copy
			\retval CSIBCXfoMatd Reference to the copied matrix
		 */
		 CSIBCXfoMatd&		GetMatrix( CSIBCXfoMatd& out_matXfo );

		 /*! Get the matrix representation of the inverse of this transformation.
			\param out_matInvXfo Destination for the copy
			\retval CSIBCXfoMatd Reference to the copied matrix
		 */
		 CSIBCXfoMatd&		GetInvMatrix( CSIBCXfoMatd& out_matInvXfo );

		/*! Sets the transformation components.
			\param in_vctScl Scaling factors
			\param in_rotScl Scaling orientation
			\param in_rot	 Orientation
			\param in_vctTrs Translation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	Set( 
										const	CSIBCVector3Dd&		in_vctScl,
										const	CSIBCRotationd&	in_rotScl,
										const	CSIBCRotationd&	in_rot,
										const	CSIBCVector3Dd&		in_vctTrs );
	
		/*! Sets the transformation components.
			\param in_vctScl Scaling factors
			\param in_rot	 Orientation
			\param in_vctTrs Translation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	Set( 
										const	CSIBCVector3Dd&		in_vctScl,
										const	CSIBCRotationd&	in_rot,
										const	CSIBCVector3Dd&		in_vctTrs );

		/*! Sets the transformation components from another transformation
			\param in_xfo Transformation to copy
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	Set( const CSIBCTransfod& in_xfo );

		/*! Sets the transformation components from a transformation matrix
			\param in_matXfo Transformation matrix for setting this transformation.
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	Set( const CSIBCXfoMatd& in_matXfo );

		/*! Sets the transformation components from a rotation
			\param in_rot Source rotation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	Set( const CSIBCRotationd& in_rot );

		/*! Sets the transformation components from a translation
			\param in_vct Translation vector
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	Set( const CSIBCVector3Dd& in_vct );

		/*! Sets the scaling factors
			\param in_vctScl Scaling factors
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetScaling( const CSIBCVector3Dd& in_vctScl );
		
		/*! Sets the scaling factors
			\param in_dX X scaling factor.
			\param in_dY Y scaling factor.
			\param in_dZ Z scaling factor.
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetScaling( 
												double in_dX, 
												double in_dY, 
												double in_dZ );

		/*! Sets the scaling factors and rotation
			\param in_vctScl Scaling factors
			\param in_rotSclOri Scaling orientation.
			\return Reference to this  transformation
		*/ 
		CSIBCTransfod&	SetScaling( 
												const	CSIBCVector3Dd&		in_vctScl,
												const	CSIBCRotationd&	in_rotSclOri );

		/*! Set scaling orientation without affecting other components.
			\param in_rotSclOri Scaling orientation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetScalingOrientation( const CSIBCRotationd& in_rotSclOri );

		/*! Set scaling orientation without affecting other components.
			\param in_vctSclOri Scaling orientation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetScalingOrientation( const CSIBCVector3Dd&  in_vctSclOri );
		
		/*! Set scaling orientation without affecting other components.
			\param in_dX X Scaling orientation
			\param in_dY Y Scaling orientation
			\param in_dZ Z Scaling orientation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetScalingOrientation( 
															const	double	in_dX,
															const	double	in_dY,
															const	double	in_dZ );


		/*! Sets the rotation without affecting other components.
			\param in_rot Orientation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetRotation( const CSIBCRotationd& in_rot );

		/*! Sets the rotation from a quaternion.
			\param in_quat Quaternion representation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetRotation( const CSIBCQuaterniond& in_quat );

		/*! Sets the rotation from a rotation matrix.
			\param in_matRot Rotation matrix.
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetRotation( const CSIBCRotMatd& in_matRot );

		/*! Sets the rotation from Euler Angles
			\param in_vctXYZAngles Euler angles
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetRotation( const CSIBCVector3Dd& in_vctXYZAngles );

		/*! Sets the rotation from Euler Angles
			\param in_dX X Euler angle in radians.
			\param in_dY Y Euler angle in radians.
			\param in_dZ Z Euler angle in radians.
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetRotation( 
												double	in_dX, 
												double	in_dY, 
												double	in_dZ );

		/*! Sets the rotation from an axis-angle
			\param in_axis Axis of rotation
			\param in_dAngle Angle around the axis
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetRotation( 
												const	E3DAxisType in_axis, 
												const	double		in_dAngle );

		/*! Sets the rotation from an axis-angle
			\param in_vctAxis Axis of rotation
			\param in_dAngle Angle around the axis
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	SetRotation( 
												const	CSIBCVector3Dd& in_vctAxis, 
												const	double		in_dAngle );

		/*! Sets the rotation from a FROM-TO vector representation.
			\param in_vctFrom Starting vector.
			\param in_vctTo Ending vector.
			\param out_bFlip Does the generated rotation produces a sudden flip? See comment in CSIBCRotationd::Set method.
			\return bool Reference to \p out_bFlip
			\sa CSIBCRotationd::Set.
		*/
		bool			SetRotation( 
												const	CSIBCVector3Dd&	in_vctFrom, 
												const	CSIBCVector3Dd& in_vctTo, 
												int&	out_bFlip );
		
		 /*! Sets the rotation from three orthonormal axes. No verification is 
		 performed about their orthonormality.
			\param in_vctX Unitary X axis (orthogonal to Y and Z).
			\param in_vctY Unitary Y axis (orthogonal to X and Z).
			\param in_vctZ Unitary Z axis (orthogonal to X and Y).
			\return Reference to this  transformation
		*/
		 CSIBCTransfod&	SetRotation( 
												const	CSIBCVector3Dd&	in_vctX, 
												const	CSIBCVector3Dd& in_vctY, 
												const	CSIBCVector3Dd& in_vctZ );

		/*! Set the translation without affecting other components.
			\param in_vctTrs Translation
			\return Reference to this  transformation
		*/
		 CSIBCTransfod&	SetTranslation( const CSIBCVector3Dd& in_vctTrs );

		 /*! Set the translation without affecting other components.
			\param in_dX X translation.
			\param in_dY Y translation.
			\param in_dZ Z translation.
			\return Reference to this  transformation
		*/
		 CSIBCTransfod&	SetTranslation( 
													double in_dX, 
													double in_dY, 
													double in_dZ );

		/*! Sets some or all components of this transformation to the identity.
			\param in_oWhichTransfo TransformationFlags Flag that detemines which transformation component must be affected
			\return Reference to this  transformation
			\sa TransformationFlags
		*/
		 CSIBCTransfod&	SetIdentity( const unsigned char in_oWhichTransfo = oSiTransfoAll );

		/*! Sets the transformation as the inverse of another transformation
			\param in_xfo Transformation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	Invert( CSIBCTransfod& in_xfo );
		
		/*! Inverts the current transformation
			\return Reference to this  transformation
		*/
		CSIBCTransfod&	Invert();

		/*! Combines two transformations into this one.
			\param in_xfo1 First transformation to combine.
			\param in_xfo2 Second transformation to combine.
			\param in_oWhichComponents TransformationFlags Which components to combine (oSiTransfoAll by default).
			\return Reference to this  transformation
			\note Given the transformations X1=(s1,u1,r1,t1) and X2=(s2,u2,r2,t2), we
			want to combine them into X=(s,u,r,t), where s stands for scaling
			factors, u for scaling orientation, r for rotation and t for
			translation. All components are under matrix representation.
			Under the SOFTIMAGE scaling scheme no shearing is possible and the
			u components are forced to be identity. The transformations are
			combined in the following way:
			\n s = s1 * s2 (component wise product)
			\n r = r1 * r2
			\n t = t1 * s2 * r2 * t2 (where s2, r2 and t2 are matrices).
			
			Under the Classical scaling scheme the shearing is possible. We
			could combined the transformations in the following way:
			\n   X = s1 * r1 * t1 * s2 * r2 * t2
			\n which will introduce shearing if r1 is not identity and s2 is
			not uniform. But we could also introduce shearing explicitly
			by using u components:
			\n   X = u1(T) * s1 * u1 * r1 * t1 * u2(T) * s2 * u2 * r2 * t2
			\n where u(T) stands for the transpose of u.
			
			In SOFTIMAGE scaling scheme, transformations must be combined
			using components representation. In classical scheme, they are
			combined by simply using product of matrices.

			\sa CSIBCTransfod::Div
			\sa TransformationFlags
		*/
		CSIBCTransfod& Mul( 
						CSIBCTransfod&	in_xfo1, 
						CSIBCTransfod& in_xfo2, 
						unsigned char		in_oWhichComponents = oSiTransfoAll );
		
		/*! Right multiplis this transformation by another transformation.
			\param in_xfo Transformation to multiply to the right.
			\param in_oWhichComponents TransformationFlags Which components to combine 
			(oSiTransfoAll by default).
			\return Reference to this  transformation
			\sa CSIBCTransfod::Div
			\sa TransformationFlags
		*/
		CSIBCTransfod& Mul( 
										CSIBCTransfod& in_xfo, 
										unsigned char		in_oWhichComponents = oSiTransfoAll );
		
		/*! Computes the product of a given transformation (T1) with the inverse of another one (T2), 
		that is, compute this = T1 * T2(-1).
			\param in_xfo1 First transformation to multiply.
			\param in_xfo2 Second transformation which inverse must be right-multiplied by the first one
			\param in_oWhichComponents TransformationFlags Which components to 
			combine (oSiTransfoAll by default).
			\param in_pXfo3 If non null, use *in_pXfo3 Euler angles as the target for the Euler angles 
			associated with this.
			\return Reference to this  transformation
			\note This is useful when we want to compensate a transformation when parenting --
			We want to preserve Tglobal of an object. \n We know that
			Tnewlocal * Tparent = Toldglobal, then we need to compute
			Tnewlocal = Toldglobal * Tparent(-1)
			and when computing global transformation of child we will get
			Tnewglobal = Tnewlocal * Tparent = Toldglobal.

			Or, when compensating a geometry when a given transformation 
			is applied to the center of the object.  Given an old vector Vlocal
			specified relative to a	transformation Tglobal, then its global value is
			Vglobal = Vlocal * Tglobal.  We apply T to the center. To preserve the 
			vector globally we	must compute Vnewlocal=Vlocal * Tglobal * T(-1).

			This function is the reverse of the CSIBCTransfod::Mul function.
			\sa CSIBCTransfod::Mul
			\sa TransformationFlags
		*/
		CSIBCTransfod& Dif( 
							CSIBCTransfod&	in_xfo1, 
							CSIBCTransfod& in_xfo2, 
							unsigned char		in_oWhichComponents = oSiTransfoAll, 
							CSIBCTransfod* in_pXfo3 = NULL );

		/*! Maps the transformation from a given space to the world space.
			\param in_xfoFrom Space into which this transformation is 
			currently represented.
			\return Reference to this  transformation
		*/
		CSIBCTransfod& MapSpaceToWorld( CSIBCTransfod& in_xfoFrom );

		/*! Maps this transformation from the world space to a given space.
			\param in_xfoTo Space into which this transformation will be 
			represented.
			\return Reference to this  transformation
		*/
		CSIBCTransfod& MapWorldToSpace( CSIBCTransfod& in_xfoTo );

		/*! Maps this transformation from a given space to another one.
			\param in_xfoFrom Space into which this transformation is 
			currently represented.
			\param in_xfoTo Space into which this transformation will be 
			represented.
			\return Reference to this  transformation
		*/
		CSIBCTransfod& MapSpaceToSpace(
													CSIBCTransfod& in_xfoFrom,
													CSIBCTransfod& in_xfoTo );

		/*! Apply a scaling to the transformation \b along local axes, parent's
		axes or any scaling axes, and \b about local position, parent's 
		position or any world position. Scaling can be Uniform, Volumic, or XYZ.
			\param in_vctSclApplied Scaling factors to apply.
			\param in_eAlong Apply scaling along \b local axes, \parent axes 
			or \b other scaling axes axes.
			\param in_protAlong Pointer to specific scaling axes. 
			When scaling is applied along OTHER scaling axes.
			\param in_eAbout Apply scaling about LOCAL position, PARENT's position
			or OTHER position.
			\param in_pvctAbout Pointer to specific position when scaling is applied 
			about OTHER position.
			\return Reference to this  transformation
			\note This function assumes that the transformation itself and
	        the parameters (scaling axes and reference position) are ALL 
			defined relative to the SAME SPACE. When the transformation is to
			be edited using parameters from different spaces, all must first
			be converted into the same common space as the transformation,
			or the opposite.

			When the common space is the WORLD space, then PARENT's axes and
			PARENT's position mean WORLD axes and WORLD position.
			Note also that passing identity scaling axes (main X,Y and Z axes)
			and/or identity position (null vector) would give the same results
			as specifying that PARENT's axes and/or PARENT's position must be 
			used. On the other hand, asking to use PARENT's axes or position
			will be more efficient.
		*/
		CSIBCTransfod& AddScaling( 
											const	CSIBCVector3Dd&		in_vctSclApplied,
											const	E3DTransfoSpace in_eAlong = LOCAL_TRANSFO_SPACE,
													CSIBCRotationd*	in_protAlong = NULL,
											const	E3DTransfoSpace in_eAbout = LOCAL_TRANSFO_SPACE,
											const	CSIBCVector3Dd*		in_pvctAbout = NULL,
											const	E3DSclType		in_eSclType = XYZ_SCL_TYPE );
		
		/*! Applies a rotation to this transformation ALONG local axes, 
		parent's axes or any rotation axes, and AROUND local position, 
		parent's position or any position.
		\param in_rotApplied Rotation to apply.
		\param in_eAlong Apply rotation along LOCAL axes,
		\param in_protAlong Pointer to specific rotation axes when rotation is 
		applied along OTHER rotation axes.
		\param in_eAround Apply rotation around LOCAL position, PARENT's position 
		or OTHER position.
		\param in_pvctAround Pointer to specific position when rotation is applied 
		around OTHER position.
		\param in_bIncrementEulerAngles If true, update the Euler angles of the
		resulting rotation using the previous angles as the target.
		\return Reference to this  transformation
		\note This function assumes that the transformation itself and
	    the parameters (scaling axes and reference position) are ALL 
		defined relative to the SAME SPACE. When the transformation is to
		be edited using parameters from different spaces, all must first
		be converted into the same common space as the transformation,
		or the opposite.

		When the common space is the WORLD space, then PARENT's axes and
		PARENT's position mean WORLD axes and WORLD position.
		Note also that passing identity scaling axes (main X,Y and Z axes)
		and/or identity position (null vector) would give the same results
		as specifying that PARENT's axes and/or PARENT's position must be 
		used. On the other hand, asking to use PARENT's axes or position
		will be more efficient.
		*/
		CSIBCTransfod& AddRotation( 
													CSIBCRotationd&	in_rotApplied,
											const	E3DTransfoSpace in_eAlong = LOCAL_TRANSFO_SPACE,
													CSIBCRotationd*	in_protAlong = NULL,
											const	E3DTransfoSpace	in_eAround = LOCAL_TRANSFO_SPACE,
											const	CSIBCVector3Dd*		in_pvctAround = NULL,
													bool			in_bIncrementEulerAngles = false);

		/*! Apply a translation to this transformation ALONG local axes, 
		parent's axes or any translation axes.
			\param in_vctTrsApplied Translation to apply.
			\param in_eAlong Apply translation along LOCAL axes,  PARENT's axes or OTHER 
			translation axes.
			\param in_protAlong Pointer to specific translation axes when translation is 
			applied along OTHER translation axes.
			\return Reference to this  transformation
			\note This function assumes that the transformation itself and
			the parameters (scaling axes and reference position) are ALL 
			defined relative to the SAME SPACE. When the transformation is to
			be edited using parameters from different spaces, all must first
			be converted into the same common space as the transformation,
			or the opposite.

			When the common space is the WORLD space, then PARENT's axes and
			PARENT's position mean WORLD axes and WORLD position.
			Note also that passing identity scaling axes (main X,Y and Z axes)
			and/or identity position (null vector) would give the same results
			as specifying that PARENT's axes and/or PARENT's position must be 
			used. On the other hand, asking to use PARENT's axes or position
			will be more efficient.
		*/
		CSIBCTransfod& AddTranslation( 
											const	CSIBCVector3Dd&		in_vctTrsApplied,
											const	E3DTransfoSpace	in_eAlong = LOCAL_TRANSFO_SPACE,
													CSIBCRotationd*	in_protAlong = NULL );
		/*! Applies a scaling orientation to this transformation ALONG local 
			axes, parent axes or any rotation axes, and AROUND local position, 
			parent position or any position.
			\param in_rSclOriApplied ScalingOrientation to apply.
			\param in_eAlong Apply rotation along LOCAL axes, PARENT axes or 
			OTHER rotation axes.
			\param in_pRotAlong Pointer to specific rotation axes when 
			rotation is applied along OTHER rotation axes.
			\param in_eAround Apply rotation around LOCAL position, PARENT 
			position or OTHER position.
			\param in_pVctAround Pointer to specific position when rotation is 
			applied around OTHER position.
			\param in_bIncrementEulerAngles | If true, update the Euler angles of 
			the resulting rotation using the previous angles as the target.
			\return Reference to this  transformation
			\note This function assumes that the transformation itself and
			the parameters (scaling axes and reference position) are ALL 
			defined relative to the SAME SPACE. When the transformation is to
			be edited using parameters from different spaces, all must first
			be converted into the same common space as the transformation,
			or the opposite.

			When the common space is the WORLD space, then PARENT's axes and
			PARENT's position mean WORLD axes and WORLD position.
			Note also that passing identity scaling axes (main X,Y and Z axes)
			and/or identity position (null vector) would give the same results
			as specifying that PARENT's axes and/or PARENT's position must be 
			used. On the other hand, asking to use PARENT's axes or position
			will be more efficient.
		*/
		CSIBCTransfod& AddScalingOrientation( 
													CSIBCRotationd&	in_rotApplied,
											const	E3DTransfoSpace in_eAlong = LOCAL_TRANSFO_SPACE,
													CSIBCRotationd*	in_protAlong = NULL,
											const	E3DTransfoSpace	in_eAround = LOCAL_TRANSFO_SPACE,
											const	CSIBCVector3Dd*		in_pvctAround = NULL,
													bool			in_bIncrementEulerAngles = false);

		/*! Gets a matrix to map a point, a line or a direction vector from a given space to 
		the world space.
			\param in_eVctType Get a mapping matrix for a point, a line or a direction vector?
			\param in_xfoFrom Space into which vectors are currently represented.
			\param out_matXfo Output transformation matrix.
			\return Reference to the output transformation matrix.
		*/
		friend CSIBCXfoMatd&	GetMapSpaceToWorld(
														const	E3DVectorType	in_eVctType,
																CSIBCTransfod&		in_xfoFrom,
																CSIBCXfoMatd&		out_matXfo );
		/*! Gets a matrix to map a point, a line or a direction vector from the world space to a given space.
			\param in_eVctType Get a mapping matrix for a point, a line or a direction vector?
			\param in_xfoTo Space into which vectors will be represented.
			\param out_matXfo Output transformation matrix.
			\return Reference to the output transformation matrix.
		*/
		friend CSIBCXfoMatd&	GetMapWorldToSpace(
														const	E3DVectorType	in_eVctType,
																CSIBCTransfod&		in_xfoTo,
																CSIBCXfoMatd&		out_matXfo );
		/*! Gets a matrix to map a point, a line or a direction vector from a given space to another one.
			\param in_eVctType Get a mapping matrix for a point, a line or a direction vector?
			\param in_xfoFrom Space into which vectors are currently represented.
			\param in_xfoTo Space into which vectors will be represented.
			\param out_matXfo Output transformation matrix.
			\return Reference to the output transformation matrix.
		*/
		friend CSIBCXfoMatd&	GetMapSpaceToSpace( 
														const	E3DVectorType	in_eVctType,
																CSIBCTransfod&		in_xfoFrom,
																CSIBCTransfod&		in_xfoTo,
																CSIBCXfoMatd&		out_matXfo );

	// @access Protected members
	protected:

		//****************************************
		// IMPLEMENTATION
		//****************************************
		// @member Factorize          | Factorize the transformation matrix
		//                              into individual scaling, scaling 
		//                              orientation, orientation and
		//                              translation components.
		// @member Combine            | Combine the individual scaling, scaling
		//                              orientation, orientation and translation
		//                              components into a transformation matrix.
		 void Factorize( const unsigned char in_oWhichComponents = oSiTransfoAll );
		 void Combine();

		// @member Init               | Initialize members of the object.
		 void Init();

	// @access Private members
	private:

		//****************************************
		// ATTRIBUTES
		//****************************************
		CSIBCRotationd		m_rot;		// @mdata	CSIBCRotationd		| CSIBCTransfod | m_rot	 | Orientation.
		CSIBCXfoMatd		m_matXfo;	// @mdata   CSIBCXfoMatd | CSIBCTransfod | m_matXfo | Transformation
									//          matrix to represent scaling, rotation, translation and shearing all
									//          at once.
		CSIBCVector3Dd		m_vctScl;	// @mdata   CSIBCVector3Dd		| CSIBCTransfod | m_vctScl  | XYZ scaling factors.
		CSIBCVector3Dd		m_vctTrs;	// @mdata	CSIBCVector3Dd		| CSIBCTransfod | m_vctTrs	 | Translation.
		CSIBCRotationd*	m_pSclOri;	// @mdata   CSIBCRotationd*	| CSIBCTransfod | m_pSclOri | Scaling Orientation.
									//          This field allows to introduce explicit shearing in the transformation.
									//			The memory holding this pointer is allocated when shearing is detected in the transformation matrix,
									//			or when shearing is set explicitely by the clients. This same memory is freed whenever there is
									//			no more shearing.

		// @devnote		Classical vs SI Scaling:
		//				With SOFTIMAGE scaling, scaling factors
		//				are always applied along local axes.
		//				In classical scaling, shearing can occur.
		//				This is equivalent to saying that the scaling factors
		//				are not applied along local axis, but along other axes.
		//				To deal with this, we introduce a Scaling Orientation matrix
		//				which represents a rotation applied before scaling the object.





		// @mdata   bool | CSIBCTransfod | m_bComponentsDirty | Do the individual
		//          transformation's components (Scaling, Scaling orientation,
		//          Orientation, Translation) corresponding to the transformation
		//          matrix are dirty?
		// @mdata   bool | CSIBCTransfod | m_bXfoMatDirty | Do the transformation
		//          matrix corresponding to the individual transformation's
		//          components (Scaling, Scaling orientation, Orientation,
		//          Translation) is dirty?
#ifdef _DEBUG

		bool m_bComponentsDirty;
		bool m_bXfoMatDirty;

		// @mdata   bool | CSIBCTransfod | m_bSIScaling | Do transformation's
		//          components are combined using SOFTIMAGE scaling (true) or
		//          classical scaling (false) ?
		bool m_bSIScaling;
#endif



		// m_oTransfoInfo comprises formely known m_bComponentsDirty, m_bXfoMatDirty
		// and m_bSIScaling booleans.
		// Bit 0 = m_bComponentsDirty
		// Bit 1 = m_bXfoMatDirty
		// Bit 2 = m_bSIScaling
		// Bit 3 to 7 = unused
		unsigned char m_oTransfoInfo;

		 void _SetComponentsDirty( const bool in_bIsDirty );
		 bool _AreComponentsDirty() const;
		 void _SetXfoMatDirty ( const bool in_bIsDirty );
		 bool _IsXfoMatDirty() const;
		 void _SetSIScaling( const bool in_bIsScaling );
		 bool _IsSIScaling() const;
		 bool HasShearing() const;
		 void _AllocateScalingOrientationMatrix(const bool in_bAllocate);


		// Disable Copy constructor and operator=
		// @member CSIBCTransfod         | Copy constructor.
		CSIBCTransfod( const CSIBCTransfod& in_xfo );
		CSIBCTransfod( const CSIBCXfoMatd& in_matXfo );
		CSIBCTransfod( const CSIBCRotationd& in_rot );
		CSIBCTransfod( const CSIBCVector3Dd& in_vctTrs );
		// @member =                  | Transformation assignment from another
		//                              transformation.
		CSIBCTransfod& operator =( const CSIBCTransfod& in_xfo );
		CSIBCTransfod& operator =( const CSIBCXfoMatd& in_matXfo );
};

typedef CSIBCTransfod* LP3DTRANSFO;
typedef const CSIBCTransfod* LPC3DTRANSFO;


#endif
