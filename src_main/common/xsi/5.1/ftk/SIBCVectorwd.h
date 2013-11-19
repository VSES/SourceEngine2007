//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCVectorwd.h | Main header file for CSIBCVectorwd implementation.
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

#ifndef _VECTORW_H_
#define _VECTORW_H_

//******************************************************************************
// Includes
//****************************************************************************** 

#include "SIBCMathLib.h"

//******************************************************************************
// Typedefs
//******************************************************************************

//******************************************************************************
// CSIBCVectorwd | General 3D vector.
//******************************************************************************

//! General purpose 3D vector class, with implicit W component.
/*! Representation of a 3D vector, with an implicit W componennt, defining operations to access, 
	modify and perform calculations on 3D vectors. This class uses double precision floating-point 
	numbers for represent its data, and in its operations. There are seperate similar classes for 
	2D and 4D vectors, CSIBCVector2Dd and CSIBCVector4Dd, respectively, which also use double precision,
	and also a 3D vector class that does not have an implicit W component, CSIBCVector3Dd.
	
	Furthermore, there is another set of vector classes which use single precision floating-point numbers,
	CSIBCVector2D, CSIBCVector3D and CSIBCVector4D for 2D, 3D and 4D vectors respectively. Quaternions
	represented as 4D vectors are also available in CSIBCQuaternion and CSIBCQuaterniond with
	single precision and double precision, repspectively.
*/
class XSICOREEXPORT CSIBCVectorwd
{
	// Public members
	public:

		//****************************************
		// IMPLEMENTATION
		//****************************************

		/*!	Default Constructor.
			Constructs a new 3D vector.
		*/
		CSIBCVectorwd ( );

		/*! Constructor.
			Constructs a new 3D vector, with \p in_dX, \p in_dY, \p in_dZ and \p in_dW as the 
			X, Y, Z and W component values, respectively.
			\param	in_dX	The value for the X component of the new vector.
			\param	in_dY	The value for the Y component of the new vector.
			\param	in_dZ	The value for the Z component of the new vector.
			\param	in_dW	The value for the W component of the new vector.
		*/
		CSIBCVectorwd ( const double in_dX, const double in_dY,
						const double in_dZ, const double in_dW );

		~CSIBCVectorwd();
					 
		// Individual access to private member:

		/*! Gets the X component of this vector.
			\return	Value of the X component of this vector.
			\sa CSIBCVectorwd::GetY
			\sa CSIBCVectorwd::GetZ
			\sa CSIBCVectorwd::GetW
			\sa CSIBCVectorwd::SetX
			\sa CSIBCVectorwd::Get
		*/
		double GetX( void ) const;

		/*! Gets the Y component of this vector.
			\return	Value of the Y component of this vector.
			\sa CSIBCVectorwd::GetX
			\sa CSIBCVectorwd::GetZ
			\sa CSIBCVectorwd::GetW
			\sa CSIBCVectorwd::SetY
			\sa CSIBCVectorwd::Get
		*/
		double GetY( void ) const;

		/*! Gets the Z component of this vector.
			\return	Value of the Z component of this vector.
			\sa CSIBCVectorwd::GetX
			\sa CSIBCVectorwd::GetY
			\sa CSIBCVectorwd::GetW
			\sa CSIBCVectorwd::SetZ
			\sa CSIBCVectorwd::Get
		*/
		double GetZ( void ) const;

		/*! Gets the W component of this vector.
			\return	Value of the W component of this vector.
			\sa CSIBCVectorwd::GetX
			\sa CSIBCVectorwd::GetY
			\sa CSIBCVectorwd::GetZ
			\sa CSIBCVectorwd::SetW
			\sa CSIBCVectorwd::Get
		*/
		double GetW( void ) const;

		/*! Gets the value of the component specified by \p in_nIndex.
			\param	in_nIndex	Index of the component value to get (0 == X, 1 == Y, 
					2 == Z, 3 == W).
			\return	double		Value of the component specified by \p in_nIndex.
			\sa CSIBCVectorwd::GetX
			\sa CSIBCVectorwd::GetY
			\sa CSIBCVectorwd::GetZ
			\sa CSIBCVectorwd::GetW
			\sa CSIBCVectorwd::Set
		*/
		double Get( const int in_iIndex ) const;

		/*! Gets the values of the X, Y, Z and W components of this vector.
			\param	out_dX	Receives the value of the X component
			\param	out_dY	Receives the value of the Y component
			\param	out_dZ	Receives the value of the Z component
			\param	out_dW	Receives the value of the W component
			\sa CSIBCVectorwd::GetX
			\sa CSIBCVectorwd::GetY
			\sa CSIBCVectorwd::GetZ
			\sa CSIBCVectorwd::GetW
			\sa CSIBCVectorwd::Set
		*/
		void  Get(	double& out_dX, double& out_dY,
					double& out_dZ, double& out_dW ) const;

		/*! Sets the value of the X component of this vector.
			\param	in_dVal	The new value of the X component of this vector.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::SetY
			\sa CSIBCVectorwd::SetZ
			\sa CSIBCVectorwd::SetW
			\sa CSIBCVectorwd::Set
			\sa CSIBCVectorwd::GetX
		*/
		CSIBCVectorwd& SetX( const double in_dVal );

		/*! Sets the value of the Y component of this vector.
			\param	in_dVal	The new value of the Y component of this vector.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::SetX
			\sa CSIBCVectorwd::SetZ
			\sa CSIBCVectorwd::SetW
			\sa CSIBCVectorwd::Set
			\sa CSIBCVectorwd::GetY
		*/
		CSIBCVectorwd& SetY( const double in_dVal );

		/*! Sets the value of the Z component of this vector.
			\param	in_dVal	The new value of the Z component of this vector.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::SetX
			\sa CSIBCVectorwd::SetY
			\sa CSIBCVectorwd::SetW
			\sa CSIBCVectorwd::Set
			\sa CSIBCVectorwd::GetZ
		*/
		CSIBCVectorwd& SetZ( const double in_dVal );

		/*! Sets the value of the W component of this vector.
			\param	in_dVal	The new value of the W component of this vector.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::SetX
			\sa CSIBCVectorwd::SetY
			\sa CSIBCVectorwd::SetZ
			\sa CSIBCVectorwd::Set
			\sa CSIBCVectorwd::GetW
		*/
		CSIBCVectorwd& SetW( const double in_dVal );

		/*! Sets the component given by \p in_nIndex to \p in_dVal.
			\param	in_nIndex	The index of the component to set (0 == X, 1 == Y, 
					2 == Z, 3 == W).
			\param	in_dVal		The value to set the component to.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Set
			\sa CSIBCVectorwd::Get
		*/
		CSIBCVectorwd& Set( const int in_nIndex, const double in_dVal );


		/*!	Sets the X, Y, Z and W components of the vector.
			\param	in_dX	The value for the X component of the vector.
			\param	in_dY	The value for the Y component of the vector.
			\param	in_dZ	The value for the Z component of the vector.
			\param	in_dW	The value for the W component of the vector.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Set
			\sa CSIBCVectorwd::SetX
			\sa CSIBCVectorwd::SetY
			\sa CSIBCVectorwd::SetZ
			\sa CSIBCVectorwd::SetW
			\sa CSIBCVectorwd::Get
		*/
		CSIBCVectorwd& Set(	const double in_dX, const double in_dY,
							const double in_dZ, const double in_dW );


		/*!	Sets all the components of this vector to zero.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::IsNull
			\sa CSIBCVectorwd::Set
		*/
		CSIBCVectorwd& SetNull( void );

		/*!	Determines whether all components of this vector are equal to zero. Since this method 
			uses floating-point comparison, a threshhold value of \p in_dEpsilon is used for equality 
			comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool	true if the vector is null, false otherwise.
			\sa CSIBCVectorwd::SetNull
			\sa CSIBCVectorwd::AreAlmostEqual
		*/
		bool IsNull( const double in_dEpsilon=PICO_EPS ) const;

		/*! Sets each component of this vector to be at least \p in_dEpsilon (in absolute value).
			This does not change the sign of the component.
			\param	in_dEpsilon	Value for the minimum absolute value of each component. Defaults to #PICO_EPS.
			\return	CSIBCVectorwd&	Reference to this vector
			\sa CSIBCVectorwd::Set
		*/
		CSIBCVectorwd& SetMinimal ( const double in_dEpsilon=PICO_EPS );

		/*! Determines whether all corresponding components of this vector and
			\p in_vct are equal. Since this method uses floating-point comparison, a threshhold
			value of \p in_dEpsilon is used for equality comparison.
			\param	in_vct	Vector for equality comparison.
			\param	in_dEpsilon	Floating-point comparison error range. Defaults to #PICO_EPS.
			\return	bool	true if the vectors are equal, false otherwise.
			\sa CSIBCVectorwd::operator==
			\sa CSIBCVectorwd::operator!=
		*/
		bool AreAlmostEqual ( const CSIBCVectorwd& in_vct,
							  const double in_dEpsilon = PICO_EPS ) ;

		/*! Determines whether all corresponding components of this vector and \p in_vct are
			exactly equal. 
			\param	in_vct	The vector to compare equality with.
			\return	bool	true if the vectors are exactly equal, false otherwise.
			\sa CSIBCVectorwd::AreAlmostEqual
			\sa CSIBCVectorwd::operator!=
		*/
		bool operator == ( const CSIBCVectorwd& in_vct ) const;

		/*! Determines whether any corresponding components of this vector and \p in_vct are
			different.
			\param	in_vct	The vector to compare inequality with.
			\return	bool	true if the vectors differ in any component, false otherwise.
			\sa CSIBCVectorwd::AreAlmostEqual
			\sa CSIBCVectorwd::operator==
		*/
		bool operator != ( const CSIBCVectorwd& in_vct ) const;

		/*! Sets this vector to its negative.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Negate
		*/
		CSIBCVectorwd& Negate( void );

		/*! Gets the length of this vector.
			\return	double	The squared length of this vector.
			\sa CSIBCVectorwd::GetSquaredLength
			\sa CSIBCVectorwd::SetLength
			\sa CSIBCVectorwd::Normalize
		*/
		double GetLength( void ) const;

		/*!	Sets the length of this vector.
			\param	in_dLength	New length for this vector.
			\return	bool		true if the length was set properly, false if the
					length of the current vector is zero.
			\sa CSIBCVectorwd::Normalize
			\sa CSIBCVectorwd::GetLength
		*/
		bool SetLength( const double in_dLength );

		/*!	Sets the vector to be a unit vector.
			\return	bool		true if the length was set properly, false if the
					length of the current vector is zero.
			\sa CSIBCVectorwd::SetLength
			\sa CSIBCVectorwd::GetLength
		*/
		bool Normalize( void );

		// Efficient arithmetic operations.


		/*! Adds this vector and \p in_vct. The W components are not added, and the
			W component of the resultant is taken from this vector. The result is 
			stored in this vector.
			\param	in_vct	Vector to add to this vector.
			return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Add
			\sa CSIBCVectorwd::Sub
		*/
		CSIBCVectorwd& Add( const CSIBCVectorwd& in_vct );

		/*! Adds \p in_vct1 and \p in_vct2. The W components are not added, and the
			W component of the resultant is taken from this vector. The result is 
			stored in this vector.
			\param	in_vct1	First vector in the addition.
			\param	in_vct2	Second vector in the addition.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Add
			\sa CSIBCVectorwd::Sub
		*/
		CSIBCVectorwd& Add( const CSIBCVectorwd& in_vct1, const CSIBCVectorwd& in_vct2	);

		/*! Subtracts \p in_vct from this vector. The W components are not subtracted,
			and the W component of the resultant is taken from this vector. The result 
			is stored in this vector.
			\param	in_vct	Vector to subtract from this vector.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Sub
			\sa CSIBCVectorwd::Add
		*/
		CSIBCVectorwd& Sub( const CSIBCVectorwd& in_vct1 );
		
		/*! Subtracts \p in_vct2 from \p in_vct1. The W components are not subtracted,
			and the W component of the resultant is taken from this vector. The result 
			is stored in this vector.
			\param	in_vct1	First vector for the subtraction.
			\param	in_vct2	Second vector for the subtraction.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Sub
			\sa CSIBCVectorwd::Add
		*/
		CSIBCVectorwd& Sub( const CSIBCVectorwd& in_vct1, const CSIBCVectorwd& in_vct2	);

		/*! Multiplies each component of this vector, except W, by \p in_dFactor. The 
			result is stored in this vector.
			\param	in_dFactor	Factor to multiply the components of this vector by.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Mul
			\sa CSIBCVectorwd::Div
			\sa CSIBCVectorwd::MulComp
			\sa CSIBCVectorwd::DivComp
		*/
		CSIBCVectorwd& Mul( const double in_vct );

		/*! Multiplies each of the components in \p in_vct, except W, by \p in_dFactor. 
			The result is stored in this vector.
			\param	in_dFactor	Factor to multiply the components of \p in_vct by.
			\param	in_vct		Vector to multiply.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Mul
			\sa CSIBCVectorwd::Div
			\sa CSIBCVectorwd::MulComp
			\sa CSIBCVectorwd::DivComp
		*/
		CSIBCVectorwd& Mul( const double in_dFactor, const CSIBCVectorwd& in_vct );

		/*!	Multiplies each of the components in \p in_vct, except W, by \p in_dFactor. 
			The result is stored in this vector.
			\param	in_vct		Vector to multiply.
			\param	in_dFactor	Factor to multiply the components of \p in_vct by.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Mul
			\sa CSIBCVectorwd::Div
			\sa CSIBCVectorwd::MulComp
			\sa CSIBCVectorwd::DivComp
		*/
		CSIBCVectorwd& Mul( const CSIBCVectorwd& in_vct, const double in_dFactor );

		/*!	Divides each component of this vector, except W, by \p in_dFactor. 
			The result is stored in this vector.
			\param	in_dFactor	Factor to divide the components of this vector by.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Div
			\sa CSIBCVectorwd::Mul
			\sa CSIBCVectorwd::DivComp
			\sa CSIBCVectorwd::MulComp
		*/
		CSIBCVectorwd& Div( const double in_vct1);

		/*!	Divides each component of \p in_vct, except W, by \p in_dFactor. 
			The result is stored in this vector.
			\param	in_vct		Vector to divide.
			\param	in_dFactor	Factor to divide the components of this vector by.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::Div
			\sa CSIBCVectorwd::Mul
			\sa CSIBCVectorwd::DivComp
			\sa CSIBCVectorwd::MulComp
		*/
		CSIBCVectorwd& Div( const double in_dFactor, const CSIBCVectorwd& in_vct );

		/*!	Multiplies each component of \p in_vct1 by the corresponding component in \p in_vct2,
			except for the W component which is set to the W component of this vector. The result 
			is stored in this vector.
			\param	in_vct1	First vector to multiply the components of.
			\param	in_vct2	Second vector to multiply the components of.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::MulComp
			\sa CSIBCVectorwd::DivComp
			\sa CSIBCVectorwd::Mul
			\sa CSIBCVectorwd::Div
		*/
		CSIBCVectorwd& MulComp( const CSIBCVectorwd& in_vct1, const CSIBCVectorwd& in_vct2  );

		/*!	Multiplies each component of \p in_vct by the corresponding component in this vector,
			except for the W component which is set to the W component of this vector. The result 
			is stored in this vector.
			\param	in_vct	Vector to multiply the components of.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::MulComp
			\sa CSIBCVectorwd::DivComp
			\sa CSIBCVectorwd::Mul
			\sa CSIBCVectorwd::Div
		*/
		CSIBCVectorwd& MulComp( const CSIBCVectorwd& in_vct );

		/*!	Divides each component of this vector by the corresponding component in \p in_vct,
			except for the W component which is set to the W component of this vector. The 
			result is stored in this vector.
			\param	in_vct	Vector to divide the components of this vector by.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::DivComp
			\sa CSIBCVectorwd::MulComp
			\sa CSIBCVectorwd::Div
			\sa CSIBCVectorwd::Mul
		*/
		CSIBCVectorwd& DivComp( const CSIBCVectorwd& in_vct );

		/*!	Divides each component of \p in_vct1 by the corresponding component in \p in_vct2,
			except for the W component which is set to the W component of this vector. The 
			result is stored in this vector.
			\param	in_vct1	Vector to be divided by the components of \p in_vct2
			\param	in_vct2	Vector to divide the components of \p in_vct1 by.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::DivComp
			\sa CSIBCVectorwd::MulComp
			\sa CSIBCVectorwd::Div
			\sa CSIBCVectorwd::Mul
		*/
		CSIBCVectorwd& DivComp( const CSIBCVectorwd& in_vct1, const CSIBCVectorwd& in_vct2 );


		/*!	Computes the dot product between the vectors \p in_vct1 and \p in_vct2. Only the
			X, Y and Z components are used in this calculation.
			\param	in_vct1	First vector in the dot product.
			\param	in_vct2	Second vector in the dot product.
			\return	double	The dot product between \p in_vct1 and \p in_vct2.
		*/
		friend double GetDot(	const CSIBCVectorwd& in_vct1,
								const CSIBCVectorwd& in_vct2 );


		/*!	Computes the distance between two points in 3D space given by 
			\p in_pnt1 and \p in_pnt2.
			\param	in_pnt1	Starting point for distance calculation.
			\param	in_pnt2	Ending point for distance calculation.
			\return	double	The scalar distance between the two points, in 3D space.
			\sa CSIBCVectorwd::GetSquaredDistance
		*/
		friend double GetDistance(	const CSIBCVectorwd& in_pnt1, 
									const CSIBCVectorwd& in_pnt2 );

		/*! Computes the squared distance between two points in 3D space given by 
			\p in_pnt1 and \p in_pnt2.
			\param	in_pnt1	Starting point for distance calculation.
			\param	in_pnt2	Ending point for distance calculation.
			\return	double	The squared scalar distance between the two points, in 3D space.
			\sa CSIBCVectorwd::GetDistance
		*/
		friend double GetSquaredDistance (	const CSIBCVectorwd& in_pnt1, 
											const CSIBCVectorwd& in_pnt2 );


		/*! Sets this vector to the midpoint between the two points \p in_pnt1 and \p in_pnt2.
			The W component for the resultant is taken from the current vector.
			\param	in_pnt1	First point for midpoint determination.
			\param	in_pnt2	Second point for midpoint determination.
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::SetMean
			\sa CSIBCVectorwd::LinearPositionInterpolate
		*/
		CSIBCVectorwd& SetMidpoint (	const CSIBCVectorwd& in_pnt1,
										const CSIBCVectorwd& in_pnt2 );


		/*! Computes the linear interpolation between the two vectors \p in_vct1 and
			\p in_vct2, at a point \p in_dT between the two vectors. The W component
			is unaffected and taken from the current vector.
			\param	in_vct1	Starting point for the interpolation.
			\param	in_vct2	Ending point for the interpolation.
			\param	in_dT	Interpolation point between the vectors, in the range [0.0f, 1.0f].
			\return	CSIBCVectorwd&	Reference to this vector.
			\sa CSIBCVectorwd::SetMidpoint
		*/
		CSIBCVectorwd& LinearPositionInterpolate (	const CSIBCVectorwd& in_vct1,
													const CSIBCVectorwd& in_vct2, 
													const double in_dT );

		
		/////////////////////////////
		// To be implemented later //
		/////////////////////////////
		


		// SetOrthogonal	| Given v1 and v2, compute the vector that
		// 					  is orthogonal to v1, and that is in the same
		// 					  plane as v1 and v2.
		/*! Not implemented yet. */
		bool SetOrthogonal ( const CSIBCVectorwd& in_vct1,
							 const CSIBCVectorwd& in_vct2 );
		
		// SetOrthoProj 	| Same as SetOrthogonal, but the resulting
		//					  vector is the projection of v2.
		/*! Not implemented yet. */
		bool SetOrthoProj  ( const CSIBCVectorwd& in_vct1,
							 const CSIBCVectorwd& in_vct2 );

		// AreOrthogonal	| Verify if two vectors are orthogonal.
		/*! Not implemented yet. */
		friend bool AreOrthogonal ( const CSIBCVectorwd& in_vct1,
									const CSIBCVectorwd& in_vct2,
									const double in_dEpsilon = PICO_EPS );

		// SetProjection	| Project this vector onto another vector.
		/*! Not implemented yet. */
		CSIBCVectorwd& SetProjection ( const CSIBCVectorwd& in_vct );

		// SetMirror 		| Compute this=2*v2 - v1.					 
		/*! Not implemented yet. */
		CSIBCVectorwd& SetMirror(	const CSIBCVectorwd& in_vct1,
									const CSIBCVectorwd& in_vct2 );

		/*! Not implemented yet. */
		CSIBCVectorwd& BsplinePositionInterpolate ( const CSIBCVectorwd& in_vct1,
											const CSIBCVectorwd& in_vct2, 
											const CSIBCVectorwd& in_vct3, 
											const CSIBCVectorwd& in_vct4, 
											const double in_dT );


		// Combine			| Compute this=s1*v1 + s2*v2.
		/*! Not implemented yet. */
		 CSIBCVectorwd& Combine(	const CSIBCVectorwd& in_vct1, const double in_dS1,
									const CSIBCVectorwd& in_vct2, const double in_dS2 );

		// SetBounds		| Readjust boundaries (min and max) to contain a given point.
		/*! Not implemented yet. */
		 void SetBounds		( const CSIBCVectorwd& in_pnt,
							  CSIBCVectorwd& io_min,
							  CSIBCVectorwd& io_max );

		/*! Not implemented yet. */
		 friend double GetVctAngle	(	const CSIBCVectorwd& in_vct1,
										const CSIBCVectorwd& in_vct2,
										const bool in_bIsSigned = false );

		// GetPntAngle	| Compute angle (signed or not) between three points.
		/*! Not implemented yet. */
		friend double GetPntAngle(	const CSIBCVectorwd& in_pnt1,
									const CSIBCVectorwd& in_pnt2,
									const CSIBCVectorwd& in_pnt3,
									const bool in_bIsSigned = false );

		// GetVctCosAngle	| Compute cosinus of angle between two vectors.
		/*! Not implemented yet. */
		  friend double GetVctCosAngle( const CSIBCVectorwd& in_vct1,
										const CSIBCVectorwd& in_vct2 );

		// GetPntCosAngle	| Compute cosinus of angle between three points.
		/*! Not implemented yet. */
		friend double GetPntCosAngle(	const CSIBCVectorwd& in_pnt1,
										const CSIBCVectorwd& in_pnt2,
										const CSIBCVectorwd& in_pnt3 );

		// ArePntCollinear | Verify if three points are collinear.
		/*! Not implemented yet. */
		friend bool ArePntCollinear(	const CSIBCVectorwd& in_pnt1,
										const CSIBCVectorwd& in_pnt2,
										const CSIBCVectorwd& in_pnt3,
										const double in_dEpsilon = PICO_EPS );

		// AreVctCoplanar	| Verify if two vectors are coplanar.
		/*! Not implemented yet. */
		friend bool AreVctCoplanar (	const CSIBCVectorwd& in_vct1,
										const CSIBCVectorwd& in_vct2,
										const CSIBCVectorwd& in_vct3,
										const double in_dEpsilon = PICO_EPS );

		// ArePntCoplanar	| Verify if three points are coplanar.
		/*! Not implemented yet. */
		friend bool ArePntCoplanar(	const CSIBCVectorwd& in_pnt1,
									const CSIBCVectorwd& in_pnt2,
									const CSIBCVectorwd& in_pnt3,
									const CSIBCVectorwd& in_pnt4,
									const double in_dEpsilon = PICO_EPS );

		// GetVctAngle		| Compute angle (signed or not) between two vectors.

		
	// Private Members
	private:

		//****************************************
		// ATTRIBUTES
		//****************************************
		// double | CSIBCVectorwd | m_dX | X component of the vector.
		// double | CSIBCVectorwd | m_dY | Y component of the vector.
		// double | CSIBCVectorwd | m_dZ | Z component of the vector.
		double  m_dX, m_dY, m_dZ, m_dW;

		//Disable copy constructor and operator =
		// CSIBCVectorwd	  | The copy Constructor
		CSIBCVectorwd ( const CSIBCVectorwd& in_vct );
		// operator=					| Assigment operator.
		CSIBCVectorwd& operator = ( const CSIBCVectorwd& in_vct );
};

//******************************************************************************
//
// Author : Michel Carigan
// Date  : 08/15/96
//
// CSIBCVectorwd& | CSIBCVectorwd | SetMinimal | Sets each component to be at least
//                                epsilon (in absolute).
//
// CSIBCVectorwd& SetMinimal( const double in_dEpsilon = PICO_EPS )
//
// double | in_dEpsilon | Specify the user minimal value. If not given, the default
//                                        value will be <t PICO_EPS>
//
// A reference on this.
//
// This function works like the macro _SI_VCT_EPS in v3.51
// That is, each component are set to eps only if they are less than epsilon.
// In that case the sign is unchanged.
// The w component is set to 1.0
//
//
//******************************************************************************
inline  CSIBCVectorwd&  
CSIBCVectorwd::SetMinimal
    (
        const double in_dEpsilon /* = PICO_EPS */
    )
{
    if( fabs(m_dX) < in_dEpsilon )
        m_dX = ( ( m_dX >= 0 ) ? in_dEpsilon : -in_dEpsilon ) ;
    if( fabs(m_dY) < in_dEpsilon )
        m_dY = ( ( m_dY >= 0 ) ? in_dEpsilon : -in_dEpsilon ) ;
    if( fabs(m_dZ) < in_dEpsilon )
        m_dZ = ( ( m_dZ >= 0 ) ? in_dEpsilon : -in_dEpsilon ) ;
    m_dW = 1;
    return *this;
}


#endif	// _VECTORW_H_
