//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCQuaterniond.h | Main header file for CSIBCQuaterniond implementation.
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
#ifndef _QUATERNION_H_
#define _QUATERNION_H_

//******************************************************************************
// Includes
//******************************************************************************
#include "SIBCMathLib.h"

//******************************************************************************
// Typedefs
//******************************************************************************

//******************************************************************************
// CSIBCQuaterniond	| Implementation of a quaternion q=W+Xi+Yj+Zk.
//******************************************************************************

//! Generic class representing quaternions.
/*! The notation used in this quaternion class (W, X, Y, Z) makes the quaternions closely
	related to 4D vectors (See CSIBCVector4Dd). In general, the quaternion can be thought
	of as a scalar plus a vector, where the W component is the vector, and X, Y and Z are
	the coefficients of the I, J and K matricies respectively.
	
	The components of the CSIBCQuaterniond class are represented by doulbe precision floating-point 
	numbers. Its corresponding single precision class, CSIBCQuaternion, has much less
	functionality, thus this class is preferred.
*/
class XSICOREEXPORT CSIBCQuaterniond
{
	// Public members
	public:
		//****************************************
		// IMPLEMENTATION
		//****************************************

		/*! Default Constructor.
			Sets all components of the quaternion to zero, except W, which is set to one.
			\sa CSIBCQuaterniond::SetIdentity
		*/			
		CSIBCQuaterniond();

		/*! Sets the new quaternion's values to those given as the parameters.
			\param	in_dW	The value for the W component of the quaternion.
			\param	in_dX	The value for the X component of the quaternion.
			\param	in_dY	The value for the Y component of the quaternion.
			\param	in_dZ	The value for the Z component of the quaternion.
			\sa CSIBCQuaterniond::Set
		*/			
		CSIBCQuaterniond( const double in_dW, const double in_dX,
			   const double in_dY, const double in_dZ );


		/*! Sets the new quaternion's vector values (X, Y, Z) as the corresponding
			component values in \p in_vct, and the W component to zero.
			\param	in_vct	Vector containing values for the new quaternion's vector components.
			\sa CSIBCQuaterniond::Set(const CSIBCVector3Dd&)
		*/
		CSIBCQuaterniond( const CSIBCVector3Dd& in_vct );

		/*! Sets the new quaternion's values as their corresponding components in \p in_vct.
			\param	in_vct	4D Vector containing component values for the new quaternion.
			\sa CSIBCQuaterniond::Set(const CSIBCVector4Dd&)
		*/
		CSIBCQuaterniond( const CSIBCVector4Dd& in_vct );

		~CSIBCQuaterniond();

		/*! Determines whether this quaternion is a 'pure' quaternion. Pure quaternions
			have a zero scalar (W) component. Since this function uses floating-point 
			comparison, a threshhold value of \p in_dEpsilon is used for equality comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool	true if the quaternion is pure, false otherwise.
			\sa CSIBCQuaterniond::IsIdentity
		*/
		bool IsPure( double in_dEpsilon = PICO_EPS ) const;

		/*! Returns the length of this quaternion.
			\return	double	The length of this quaternion.
			\sa CSIBCQuaterniond::GetSquaredLength
			\sa CSIBCQuaterniond::Normalize
		*/
		double GetLength() const;

		/*!	Returns the squared length of this quaternion.
			\return	double	The squared length of this quaternion.
			\sa CSIBCQuaterniond::GetLength
			\sa CSIBCQuaterniond::Normalize
		*/
		double GetSquaredLength() const;

		/*! Sets this quaternion to the identity quaternion (all components zero, except W
			is set to one).
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::IsIdentity
		*/
		CSIBCQuaterniond& SetIdentity();

		/*! Determines whether this quaternion is the identity quaternion (all components
			zero, except W is set to one). Note that this function uses exact equality
			comparison, thus the components must be their exact values.
			\return	bool	true if the quaternion is the identity quaternion, false otherwise.
			\sa CSIBCQuaterniond::SetIdentity
		*/
		bool IsIdentity();

		/*! Normalizes this quaternion to be a unit quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::GetLength
			\sa CSIBCQuaterniond::GetSquaredLength
		*/
		CSIBCQuaterniond& Normalize();

		/*! Returns the W (scalar) component of the quaternion.
			\return	double	The W component of the quaternion.
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::GetX
			\sa CSIBCQuaterniond::GetY
			\sa CSIBCQuaterniond::GetZ
		*/
		double GetW() const;

		/*! Returns the X component of the quaternion.
			\return	double	The X component of the quaternion.
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::GetW
			\sa CSIBCQuaterniond::GetY
			\sa CSIBCQuaterniond::GetZ
		*/
		double GetX() const;

		/*! Returns the Y component of the quaternion.
			\return	double	The Y component of the quaternion.
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::GetW
			\sa CSIBCQuaterniond::GetX
			\sa CSIBCQuaterniond::GetZ
		*/
		double GetY() const;

		/*! Returns the Z component of the quaternion.
			\return	double	The Z component of the quaternion.
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::GetW
			\sa CSIBCQuaterniond::GetX
			\sa CSIBCQuaterniond::GetY
		*/
		double GetZ() const;

		/*! Sets the W (scalar) component of the quaternion.
			\param	in_dW	New value for the W component of the quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::SetX
			\sa CSIBCQuaterniond::SetY
			\sa CSIBCQuaterniond::SetZ
		*/
		CSIBCQuaterniond& SetW( const double in_dW );

		/*! Sets the X component of the quaternion.
			\param	in_dX	NeX value for the X component of the quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::SetW
			\sa CSIBCQuaterniond::SetY
			\sa CSIBCQuaterniond::SetZ
		*/
		CSIBCQuaterniond& SetX( const double in_dX );

		/*! Sets the Y component of the quaternion.
			\param	in_dY	NeY value for the Y component of the quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::SetW
			\sa CSIBCQuaterniond::SetX
			\sa CSIBCQuaterniond::SetZ
		*/
		CSIBCQuaterniond& SetY( const double in_dY );

		/*! Sets the Z component of the quaternion.
			\param	in_dZ	NeZ value for the Z component of the quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::SetW
			\sa CSIBCQuaterniond::SetX
			\sa CSIBCQuaterniond::SetY
		*/
		CSIBCQuaterniond& SetZ( const double in_dZ );

		/*! Gets the quaternion values with a CSIBCVector4Dd.
			\param	out_vct	Vector to receive the values of this quaternion.
			\return	CSIBCVector4Dd&	Reference to \p out_vct.
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::Set
		*/
		CSIBCVector4Dd& Get( CSIBCVector4Dd& out_vct ) const;

		/*! Gets the quaternion values.
			\param	out_dW	double to receive the W component of the quaternion.
			\param	out_dX	double to receive the X component of the quaternion.
			\param	out_dY	double to receive the Y component of the quaternion.
			\param	out_dZ	double to receive the Z component of the quaternion.
			\sa CSIBCQuaterniond::Get
			\sa CSIBCQuaterniond::Set
		*/
		void Get(	double& out_dW, double& out_dX,
					double& out_dY, double& out_dZ ) const;

		/*! Sets the quaternion values.
			\param	in_dW	New value for the W component of this quaternion.
			\param	in_dX	New value for the X component of this quaternion.
			\param	in_dY	New value for the Y component of this quaternion.
			\param	in_dZ	New value for the Z component of this quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::Get
		*/
		CSIBCQuaterniond& Set( const double in_dW, const double in_dX,
						   const double in_dY, const double in_dZ );

		/*! Sets this quaternion's components from the corresponding components of \p in_vct.
			\param	in_vct	Vector containing new values for the quaternion's components.
			\return	CSIBCQuateriond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::Get
		*/
		CSIBCQuaterniond& Set( const CSIBCVector4Dd& in_vct );

		/*! Sets this quaternion's vector components from the corresponding components of
			\p in_vct. The scalar (W) component is set to zero.
			\param	in_vct	Vector containing new values for the quaternion's vector components.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::Get
		*/
		CSIBCQuaterniond& Set( const CSIBCVector3Dd& in_vct );

		/*! Sets this quaternion's components equal to the components of \p in_quat.			
			\param	in_quat	Quaternion 
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Set
			\sa CSIBCQuaterniond::Get
		*/
		CSIBCQuaterniond& Set( const CSIBCQuaterniond& in_quat);

		/*! Determines whether each of the components of \p in_quat1 and \p in_quat2 are
			equal to each other. Since this function uses floating-point comparison, a 
			threshhold value of \p in_dEpsilon is used for equality comparison.

			\param	in_quat1	The first quaternion for equality comparison.
			\param	in_quat2	The second quaternion for equality comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool		true if the quaternions are equal, false otherwise.
			\sa CSIBCQuaterniond::operator==
			\sa CSIBCQuaterniond::operator!=
		*/
		friend bool AreAlmostEqual(	const CSIBCQuaterniond& in_quat1,
									const CSIBCQuaterniond& in_quat2,
									const double in_dEpsilon = PICO_EPS );


		/*! Determines whether each of the components of this quaternion and \p in_quat are
			exactly equal to each other.
			\param	in_quat	Quaternion to compare equality with.
			\return	bool	true if the quaternions are exactly equal, false otherwise.
			\sa CSIBCQuaterniond::AreAlmostEqual
			\sa CSIBCQuaterniond::operator!=
		*/
		bool operator ==( const CSIBCQuaterniond& in_quat ) const;

		/*! Determines whether any of the components of this quaternion and \p in_quat are
			different. Note that this function does not use floating-point comparison error,
			thus any difference in component values will result in a true return value.

			\param	in_quat	Quaternion to compare inequality with.
			\return	bool	true if the quaternions are not equal, false if they are exactly equal.
			\sa CSIBCQuaterniond::operator==
			\sa CSIBCQuaterniond::AreAlmostEqual
		*/
		bool operator !=( const CSIBCQuaterniond& in_quat ) const;

		/*!	Negates all components of this quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Invert
			\sa CSIBCQuaterniond::Conjugate
		*/
		CSIBCQuaterniond& Negate();

		/*!	Sets this quaternion to the inverse of the unit quaternion \p in_quat.
			Since a unitary input quaternion is assumed, this function is equivalent
			to CSIBCQuaterniond::Conjugate(const CSIBCQuateriond &).
			\param	in_quat	Quaternion whose inverse contains the new values for this 
					quaternion's components.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Invert
			\sa CSIBCQuaterniond::Conjugate
			\sa CSIBCQuaterniond::Normalize
		*/
		CSIBCQuaterniond& Invert( const CSIBCQuaterniond& in_quat );

		/*! Sets this quaternion to its inverse. It is assumed that this quaternion
			is a unit quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Invert
			\sa CSIBCQuaterniond::Conjugate
			\sa CSIBCQuaterniond::Normalize
		*/
		CSIBCQuaterniond& Invert();

		/*!	Sets this quaternion to the conjugate of \p in_quat. A conjugate quaternion is
			a quaternion that has each of its vector components negated.
			\param	in_quat	Quaternion whose conjugate contains the new values for this
					quaternion's components.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Invert
			\sa CSIBCQuaterniond::Conjugate
			\sa CSIBCQuaterniond::Normalize
		*/
		CSIBCQuaterniond& Conjugate( const CSIBCQuaterniond& in_quat );

		/*! Sets this quaternion to its conjugate. A conjugate quaternion is
			a quaternion that has each of its vector components negated.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Invert
			\sa CSIBCQuaterniond::Conjugate
			\sa CSIBCQuaterniond::Normalize
		*/
		CSIBCQuaterniond& Conjugate();

		/*!	Adds \p in_quat to this quaternion.
			\param	in_quat	Quaternion to add to this quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Add
			\sa CSIBCQuaterniond::Sub
		*/
		CSIBCQuaterniond& Add( const CSIBCQuaterniond& in_quat  );

		/*! Adds \p in_quat1 and \p in_quat2 and stores the result in this quaternion.
			\param	in_quat1	First quaternion for addition.
			\param	in_quat2	Second quaternion for addition.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Add
			\sa CSIBCQuaterniond::Sub
		*/
		CSIBCQuaterniond& Add( const CSIBCQuaterniond& in_quat1, const CSIBCQuaterniond& in_quat2 );

		/*! Subtracts \p in_quat from this quaternion.
			\param	in_quat	Quaternion to subtract from this quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa	CSIBCQuaterniond::Sub
			\sa	CSIBCQuaterniond::Add
		*/
		CSIBCQuaterniond& Sub( const CSIBCQuaterniond& in_quat );

		/*! Subtracts \p in_quat2 from \p in_quat1 and stores the result in this quaterinon.
			\param	in_quat1	Quaternion for subtraction.
			\param	in_quat2	Quaternion to subtract from \p in_quat1.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa	CSIBCQuaterniond::Sub
			\sa	CSIBCQuaterniond::Add
		*/
		CSIBCQuaterniond& Sub( const CSIBCQuaterniond& in_quat1, const CSIBCQuaterniond& in_quat2 ); 

		/*! Computes the right-multiplication of \p in_quat1 by \p in_quat2 (\p in_quat1 * \p in_quat2), 
			and stores the result in this quaternion.
			\param	in_quat1	First quaternion for the multiplication (on the left).
			\param	in_quat2	Second quaternion for the multiplication (on the right).
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa	CSIBCQuaterniond::Mul
		*/
		CSIBCQuaterniond& Mul( const CSIBCQuaterniond& in_quat1, const CSIBCQuaterniond& in_quat2 );

		/*! Computes the right-multiplication of this quaternion by \p in_quat (this * \p in_quat), 
			and stores the result in this quaternion.
			\param	in_quat	Quaternion for the multiplication (on the right).
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa	CSIBCQuaterniond::Mul
		*/
		CSIBCQuaterniond& Mul( const CSIBCQuaterniond& in_quat );

		/*! Computes the right-multiplication of the pure quaternion represented by \p in_quatPure,
			and the quaternion \p in_quat (\p in_quatPure * \p in_quat), and stores the result in
			this quaternion.
			\param	in_quatPure	Vector representing the vector components of a pure 
					quaternion (W component is zero), for the multiplication (on the left).
			\param	in_quat	Quaternion for the multiplication (on the right).
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Mul
		*/
		CSIBCQuaterniond& Mul( const CSIBCVector3Dd& in_quatPure, const CSIBCQuaterniond& in_quat );

		/*! Computes the right-multiplication of the quaternion \p in_quat and the pure quaternion
			represented by \p in_quatPure (\p in_quat * \p in_quatPure), and stores the result in
			this quaternion.
			\param	in_quat	Quaternion for the multiplication (on the left).
			\param	in_quatPure	Vector representing the vector components of a pure
					quaternion (W component is zero), for the multiplication (on the right).
			\return	CSIBCQuaterniond&	Reference to this quaterion.
			\sa CSIBCQuaterniond::Mul
		*/
		CSIBCQuaterniond& Mul( const CSIBCQuaterniond& in_quat, const CSIBCVector3Dd& in_quatPure );

		/*! Scales \p in_quat by a factor of \p in_dFactor, and stores the result in this quaternion.
			\param	in_quat	Quaternion to scale.
			\param	in_dFactor	Scaling factor for quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Mul
		*/
		CSIBCQuaterniond& Mul( const CSIBCQuaterniond& in_quat, const double in_dFactor );

		/*!	Scales \p in_quat by a factor of \p in_dFactor, and stores the result in this quaternion.
			\param	in_quat	Quaternion to scale.
			\param	in_dFactor	Scaling factor for quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Mul
		*/
		CSIBCQuaterniond& Mul( const double in_dFactor, const CSIBCQuaterniond& in_quat );

		/*! Scales this quaternion by a factor of \p in_dFactor.
			\param	in_dFactor	Scaling factor to apply to this quaternion.
			\return	CSIBCQuaterniond&	Reference to this quaternion.
			\sa CSIBCQuaterniond::Mul
		*/
		CSIBCQuaterniond& Mul( const double in_dFactor );

		/*! Computes the dot product of \p in_quat1 and \p in_quat2.
			\param	in_quat1	First quaternion for the dot product.
			\param	in_quat2	Second quaternion for the dot product.
			\return	double		The dot product of \p in_quat1 and \p in_quat2.
		*/
        friend double GetDot( const CSIBCQuaterniond& in_quat1,
                                   const CSIBCQuaterniond& in_quat2 );

		/*! Computes the spherical linear interpolation of two unit quaternions,
			where \p in_quatStart and \p in_quatEnd are the starting and ending
			quaternions, respectively. The interpolation point bewteen the two
			vectors is given by \p in_dU, in the range [0.0f, 1.0f]. The interpolated
			quaternion is stored in this quaternion.

			\param	in_quatStart	Unit quaternion for the starting interpolation point.
			\param	in_quatEnd		Unit quaternion for the ending interpolation end.
			\param	in_dU			Interpolation point between \p in_quatStart and \p in_quatEnd.
					Must be in the range [0.0f, 1.0f].
			\return	CSIBCQuaterniond&	Reference to this quaternion.
		*/
		CSIBCQuaterniond& Slerp( const CSIBCQuaterniond& in_quatStart,
					  const CSIBCQuaterniond& in_quatEnd,
					  const double in_dU );

		////////////////////////////////
		// To be implemented later... //
		////////////////////////////////

		// BisectSlerp     | Fast slerp for u=0.5
		/*! Not implemented yet. */
		CSIBCQuaterniond& BisectSlerp( const CSIBCQuaterniond& in_quatStart,
							const CSIBCQuaterniond& in_quatEnd );

		// DoubleSlerp     | Fast slerp for u=2.0
		/*! Not implemented yet. */
		CSIBCQuaterniond& DoubleSlerp( const CSIBCQuaterniond& in_quatStart,
							const CSIBCQuaterniond& in_quatEnd );

		// Ln              | Compute ln(q) (for a unit quaternion). Result is a pure quaternion.
		/*! Not implemented yet. */
		bool Ln( CSIBCQuaterniond& out_quat );

		// Exp             | Compute e^q (for a pure quaternion). Result is a unit quaternion.
		/*! Not implemented yet. */
		bool Exp( CSIBCQuaterniond& out_quat );

		// Squad           | Quadrangle interpolation of quaternions, using three spherical 
		//					 linear interpolations.
		/*! Not implemented yet. */
		CSIBCQuaterniond& Squad( const CSIBCQuaterniond& in_quatB0,
					  const CSIBCQuaterniond& in_quatS1,
					  const CSIBCQuaterniond& in_quatS2,
					  const CSIBCQuaterniond& in_quatB3,
					  const double in_dU,
					  const double in_dT );

	// Private members
	private:

		//****************************************
		// IMPLEMENTATION
		//****************************************

		//****************************************
		// ATTRIBUTES
		//****************************************

		// double | CSIBCQuaterniond | m_dW | W (scalar) component of the quaternion.
		// double | CSIBCQuaterniond | m_dX | X component of the quaternion.
		// double | CSIBCQuaterniond | m_dY | Y component of the quaternion.
		// double | CSIBCQuaterniond | m_dZ | Z component of the quaternion.
		double m_dW, m_dX, m_dY, m_dZ;

		// Disable copy constructor and operator =
		// CSIBCQuaterniond         | Copy constructor.
		CSIBCQuaterniond( const CSIBCQuaterniond& in_quat );
		// operator =               | Assignment operator.
		CSIBCQuaterniond& operator =( const CSIBCQuaterniond& in_quat );
};

#endif
