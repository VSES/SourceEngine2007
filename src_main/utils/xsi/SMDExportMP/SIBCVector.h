//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCVector.h | Main header file for SIBCVector2D/3D/4D implementation
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
// Defines
//***************************************************************************************
#ifndef __CSIBCVector_H__
#define __CSIBCVector_H__

//***************************************************************************************
// Includes
//**************************************************************************************
#include <math.h>
#include "SIBCUtil.h"

//***************************************************************************************
// Macros
//***************************************************************************************
#define PICO_EPS 0.00001f

#ifdef _PSX2_ASM
#include "libvu0.h"
#endif


//***************************************************************************************
// CSIBCVector2D | General purpose 2D vector class.
//***************************************************************************************

//! General purpose 2D vector class.
/*! Representation of a 2D vector, defining operations to access, modify and perform calculations
	on 2D vectors. This class uses single precision floating-point numbers for represent its data,
	and in its operations. There are seperate similar classes for 3D and 4D vectors, CSIBCVector3D and
	CSIBCVector4D, respectively, which also use single precision.
	
	Furthermore, there is another set of vector classes which use double precision floating-point numbers,
	CSIBCVector2Dd, CSIBCVector3Dd and CSIBCVector4Dd for 2D, 3D and 4D vectors respectively. Quaternions
	represented as 4D vectors are also available in CSIBCQuaternion and CSIBCQuaterniond with
	single precision and double precision, repspectively.
*/
class XSICOREEXPORT CSIBCVector2D
{
	public:     

		/*! Default constructor.
			Sets the values of each component of this vector to zero.
		*/		
		CSIBCVector2D();

		/*! Copy constructor.
			Sets the new vector's component values to be the same as \p i_vVector.
			\param	i_vVector	Vector to use for new vector's component values.
		*/
		CSIBCVector2D(const CSIBCVector2D& i_vVector);

		/*! Constructor.
			Sets the new vector to have \p i_fX and \p i_fY for its X and Y components, respectively.
			\param	i_fX	The value for the X component of the new vector.
			\param	i_fY	The value for the Y component of the new vector.
		*/
		CSIBCVector2D(const SI_Float i_fX, const SI_Float i_fY);

		~CSIBCVector2D();
		
		// Comparison

		/*! Determines whether this vector and \p i_vVector are equal to each other, within a 
			threshhold value given by i_fEpsilon.
			\param	i_vVector	Vector to compare equality with.
			\param	i_fEpsilon	Error range for floating-point comparison.
			\return	SI_Bool		TRUE if the vectors are equal, FALSE otherwise.
			\sa	CSIBCVector2D::IsOrthogonalTo
			\sa	CSIBCVector2D::IsNull
			\sa	CSIBCVector2D::operator==
		*/
		SI_Bool IsAlmostEqualTo(const CSIBCVector2D& i_vVector, const SI_Float i_fEpsilon);

		/*! Determines whether this vector and \p i_vVector are orthogonal (at 90 degrees to
			each other). This function uses a floating-point comparison error of #PICO_EPS.

			\param	i_vVector	The vector to compare orthogonality with.
			\return	SI_Bool		TRUE if the vectors are orthogonal, FALSE otherwise.
			\sa	CSIBCVector2D::IsAlmostEqualTo
			\sa	CSIBCVector2D::IsNull
			\sa	CSIBCVector2D::GetDot
		*/
		SI_Bool IsOrthogonalTo(const CSIBCVector2D& i_vVector);

		/*! Determines whether all of the components of this vector are zero. This function
			uses a floating-point comparison error of #PICO_EPS.

			\return	SI_Bool		TRUE if all of the vector's components are zero, FALSE otherwise.
			\sa	CSIBCVector2D::IsAlmostEqualTo
			\sa	CSIBCVector2D::IsNull
		*/
		SI_Bool IsNull() const;

		// Computed Info

		/*! Computes the dot product between this vector and \p i_vVector.
			\param	i_vVector	Vector to dot-product with this vector.
			\return	SI_Float	Dot product of the two vectors.
			\sa	CSIBCVector2D::IsOrthogonalTo
		*/
		SI_Float GetDot(const CSIBCVector2D& i_vVector) const;

		/*! Returns the distance between this vector and \p i_vVector.
			\param	i_vVector	Vector to determine distance from.
			\return	SI_Float	Distance from this vector to \p i_vVector.
			\sa	CSIBCVector2D::GetLength
		*/
		SI_Float GetDistanceFrom(const CSIBCVector2D& i_vVector) const;

		/*! Returns the length of this vector.
			\return	SI_Float	The length of this vector.
			\sa	CSIBCVector2D::GetSquaredLength
			\sa	CSIBCVector2D::GetMaxComponent
			\sa	CSIBCVector2D::GetMinComponent
		*/
		SI_Float GetLength() const;

		/*! Returns the squared length of this vector.
			\return SI_Float	The squared length of this vector.
			\sa	CSIBCVector2D::GetLength
			\sa	CSIBCVector2D::GetMaxComponent
			\sa	CSIBCVector2D::GetMinComponent
		*/
		SI_Float GetSquaredLength() const;

		/*! Returns the value and index of the maximum component of this vector.
			\param	o_nIndex	Pointer to an SI_Int to hold the value of the index that had the
					maximum component (0 == X, 1 == Y).
			\return	SI_Float	The value of the maximum component of this vector.
			\sa	CSIBCVector2D::Get
		*/
		SI_Float GetMaxComponent(SI_Int * o_nIndex) const;

		/*! Returns the value and index of the minimum component of this vector.
			\param	o_nIndex	Pointer to an SI_Int to hold the value of the index that had the
					maximum component (0 == X, 1 == Y).
			\return	SI_Float	The value of the maximum component of this vector.
			\sa	CSIBCVector2D::Get
		*/
		SI_Float GetMinComponent(SI_Int * o_nIndex) const;
		
		// Components

		/*! Gets the values of the vector components.
			\param	o_fX	Pointer to an SI_Float to receive the value of the X component.
			\param	o_fY	Pointer to an SI_Float to receive the value of the Y component.
			\sa	CSIBCVector2D::GetX
			\sa	CSIBCVector2D::GetY
			\sa CSIBCVector2D::Set
		*/
		void Get(SI_Float* o_fX, SI_Float* o_fY) const;

		/*!	Gets the X component of this vector.
			\return	SI_Float	The value of the X component of this vector.
			\sa CSIBCVector2D::GetY
			\sa CSIBCVector2D::Get
			\sa CSIBCVector2D::SetX
		*/
		const SI_Float GetX() const { return m_fX; }

		/*!	Gets the Y component of this vector.
			\return	SI_Float	The value of the Y component of this vector.
			\sa CSIBCVector2D::GetX
			\sa CSIBCVector2D::Get
			\sa CSIBCVector2D::SetY
		*/
		const SI_Float GetY() const { return m_fY; }

		/*! Sets the values of the X and Y components of this vector.
			\param	i_fX	The new value for the X component of this vector.
			\param	i_fY	The new value for the Y component of this vector.
			\return	CSIBCVector2D&	Reference to this vector.
			\sa CSIBCVector2D::SetX
			\sa CSIBCVector2D::SetY
			\sa CSIBCVector2D::Get
		*/
		CSIBCVector2D& Set(SI_Float i_fX, SI_Float i_fY);

		/*! Sets the value of the X component of this vector.
			\param	i_fX	The new value of the X component of this vector.
			\sa CSIBCVector2D::SetY
			\sa CSIBCVector2D::Set
			\sa CSIBCVector2D::GetX
		*/
		const void SetX(const SI_Float i_fX) { m_fX = i_fX; }

		/*! Sets the value of the Y component of this vector.
			\param	i_fY	The new value of the Y component of this vector.
			\sa CSIBCVector2D::SetX
			\sa CSIBCVector2D::Set
			\sa CSIBCVector2D::GetY
		*/
		const void SetY(const SI_Float i_fY) { m_fY = i_fY; }

		// Operations

		/*! Normalizes the vector to a length of one. If the vector is null, then it is
			initialized to (1.0f, 0.0f).
			\return	CSIBCVector2D&	Reference to this vector.
			\sa CSIBCVector2D::SetLength
			\sa	CSIBCVector2D::GetLength
		*/
		CSIBCVector2D& Normalize();

		/*! Sets this vector to null. (Components equal 0.0f).
			\return CSIBCVector2D&	Reference to this vector.
			\sa CSIBCVector2D::IsNull
			\sa CSIBCVector2D::Set
		*/
		CSIBCVector2D& SetNull();

		/*! Sets the length of this vector to be \p i_fLength. If the vector is null 
			(See CSIBCVector2D::IsNull), then the vector is unchanged.
			\param	i_fLength	New length of the vector.
			\return	CSIBCVector2D&	Reference to this vector.
			\sa CSIBCVector2D::GetLength
			\sa CSIBCVector2D::Set
		*/
		CSIBCVector2D& SetLength(const SI_Float i_fLength);

		/*! Sets all the components of this vector to be the negatives of their current values.
			\return	CSIBCVector2D&	Reference to this vector.
			\sa CSIBCVector2D::Set
		*/
		CSIBCVector2D& Negate();

		/*! Adds this vector to \p i_vVector, and stores the result in this vector.
			\param	i_vVector	The vector to add to this vector.
			\return	CSIBCVector2D&	Reference to this vector.
			\sa CSIBCVector2D::Sub
		*/
		CSIBCVector2D& Add(const CSIBCVector2D& i_vVector);

		/*! Subtracts \p i_vVector from this vector, and stores the result in this vector.
			\param	i_vVector	The vector to subtract from this vector.
			\return	CSIBCVector2D&	Reference to this vector.
			\sa CSIBCVector2D::Add
		*/
		CSIBCVector2D& Sub(const CSIBCVector2D&);

		/*!	Scales all components of this vector by \p i_fScale.
			\param	i_fScale	The amount to scale each of the components of this vector by.
			\return	CSIBCVector2D& Reference to this vector.
			\sa CSIBCVector2D::operator*
			\sa CSIBCVector2D::SetLength
		*/
		CSIBCVector2D& Scale(const SI_Float i_fScale);

		/*! Multiplies each component in this vector by its corresponding component in \p i_vVector.
			The result is stored in this vector.
			\param	i_vVector	Vector to multiply components with this vector.
			\return	CSIBCVector2D&	Reference to this vector
			\sa CSIBCVector2D::DivComp
		*/
		CSIBCVector2D& MulComp(const CSIBCVector2D& i_vVector);

		/*! Divides each component in this vector by its corresponding component in \p i_vVector.
			The result is stored in this vector.
			\param	i_vVector	Vector to divide components with this vector.
			\return CSIBCVector2D&	Reference to this vector.
			\sa CSIBCVector2D::operator/
			\sa CSIBCVector2D::MulComp
		*/
		CSIBCVector2D& DivComp(const CSIBCVector2D& i_vVector);
		
	    // overloaded operators

		/*! Determines whether this vector and \p i_vVector are equal. Note that this function does
			not use any floating-point comparison error, thus the two vectors must be exactly the
			same.
			\param	i_vVector	Vector to compare equality with.
			\return	SI_Bool		TRUE if the vectors are equal, FALSE otherwise.
			\sa	CSIBCVector2D::IsAlmostEqualTo
			\sa CSIBCVector2D::operator!=
		*/
		SI_Bool operator == (const CSIBCVector2D& i_vVector) const;

		/*! Determines whether this vector and \p i_vVector are not-equal. Note that this function does
			not use any floating-point comparison error, thus the two vectors must be exactly the
			same for a FALSE return value.
			\param	i_vVector	Vector to compare inequality with.
			\return	SI_Bool		TRUE if the vectors are not equal, FALSE otherwise.
			\sa	CSIBCVector2D::IsAlmostEqualTo
			\sa CSIBCVector2D::operator==
		*/
		SI_Bool operator != (const CSIBCVector2D& i_vVector) const;
		
		/*! Computes the vector addition between this vector and \p i_vVector.
			\param	i_vVector	The vector to add this vector with.
			\return	CSIBCVector2D	The result of the addition.
			\sa CSIBCVector2D::Add
			\sa CSIBCVector2D::operator-
		*/
		CSIBCVector2D operator+(const CSIBCVector2D &i_vVector) const;

		/*! Computes the vector subtraction of \p i_vVector from this vector.
			\param	i_vVector	The vector to subtract from this vector.
			\return	CSIBCVector2D	The result of the subtraction.
			\sa CSIBCVector2D::Sub
			\sa CSIBCVector2D::operator+
		*/
		CSIBCVector2D operator-(const CSIBCVector2D &i_vVector) const;

		/*! Divides each component in this vector by its corresponding component in \p i_vVector.
			\param	i_vVector	Vector to divide components with this vector.
			\return CSIBCVector2D	The result of the divisions.
			\sa CSIBCVector2D::DivComp
		*/
		CSIBCVector2D operator/(const CSIBCVector2D &i_vVector) const;

		/*! Multiplies each component of this vector by \p i_fFloat.
			\param	i_fFloat	The amount to scale each component of this vector by.
			\return	CSIBCVector2D	The result of the scaling.
			\sa CSIBCVector2D::Scale
			\sa CSIBCVector2D::MulComp
		*/	
		CSIBCVector2D operator*(const SI_Float i_fFloat) const;

		// Debug functions

		/*! Outputs the memory location of this object, and the values of each
			of its components.
			\sa SIBCVector2D::Get
		*/
		void	Dump();
		
		/*! The X component of this vector. */
		float	m_fX;

		/*! The Y component of this vector. */
		float	m_fY;


	protected:
};

//***************************************************************************************
// CSIBCVector3D | General purpose 3D vector class.
//***************************************************************************************

//! General purpose 3D vector class.
/*! Representation of a 3D vector, defining operations to access, modify and perform calculations
	on 2D vectors. This class uses single precision floating-point numbers for represent its data,
	and in its operations. There are seperate similar classes for 2D and 4D vectors, CSIBCVector2D and
	CSIBCVector4D, respectively, which also use single precision.
	
	Furthermore, there is another set of vector classes which use double precision floating-point numbers,
	CSIBCVector2Dd, CSIBCVector3Dd and CSIBCVector4Dd for 2D, 3D and 4D vectors respectively. Quaternions
	represented as 4D vectors are also available in CSIBCQuaternion and CSIBCQuaterniond with
	single precision and double precision, repspectively.
*/
class XSICOREEXPORT CSIBCVector3D
{
	public:     

		// Constructors and destructor
		/*! Default constructor.
			Sets the values of each component of this vector to zero.
		*/
		CSIBCVector3D();

		/*! Copy constructor.
			Sets the new vector's component values to be the same as \p i_vVector.
			\param	i_vVector	Vector to use for new vector's component values.
		*/
		CSIBCVector3D(const CSIBCVector3D& i_vVector);

		/*! Constructor.
			Sets the new vector to have \p i_fX, \p i_fY and \p i_fZ for its X, Y and 
			Z components, respectively.
			\param	i_fX	The value for the X component of the new vector.
			\param	i_fY	The value for the Y component of the new vector.
			\param	i_fZ	The value for the Z component of the new vector.
		*/
		CSIBCVector3D(const SI_Float i_fX, const SI_Float i_fY, const SI_Float i_fZ);

		~CSIBCVector3D() {;}
		
		// Comparison

		/*! Determines whether this vector and \p i_vVector are equal to each other, within a 
			threshhold value given by i_fEpsilon.

			\param	i_vVector	Vector to compare equality with.
			\param	i_fEpsilon	Error range for floating-point comparison.
			\return	SI_Bool		TRUE if the vectors are equal, FALSE otherwise.
			\sa	CSIBCVector3D::IsOrthogonalTo
			\sa	CSIBCVector3D::IsNull
			\sa	CSIBCVector3D::operator==
		*/
		SI_Bool IsAlmostEqualTo(const CSIBCVector3D& i_vVector, const SI_Float i_fEpsilon);

		/*! Determines whether this vector and \p i_vVector are orthogonal (at 90 degrees to
			each other). This function uses a floating-point comparison error of #PICO_EPS.

			\param	i_vVector	The vector to compare orthogonality with.
			\return	SI_Bool		TRUE if the vectors are orthogonal, FALSE otherwise.
			\sa	CSIBCVector3D::IsAlmostEqualTo
			\sa	CSIBCVector3D::IsNull
			\sa	CSIBCVector3D::GetDot
		*/
		SI_Bool IsOrthogonalTo(const CSIBCVector3D& i_vVector);

		/*! Determines whether the all components of this vector are zero. This function
			uses a floating-point comparison error of #PICO_EPS.

			\return	SI_Bool		TRUE if all of the vector's components are zero, FALSE otherwise.
			\sa	CSIBCVector3D::IsAlmostEqualTo
			\sa	CSIBCVector3D::IsNull
		*/
		SI_Bool IsNull() const;

		// Computed Info

		/*! Computes the dot product between this vector and \p i_vVector.
			\param	i_vVector	Vector to dot-product with this vector.
			\return	SI_Float	Dot product of the two vectors.
			\sa	CSIBCVector3D::IsOrthogonalTo
		*/
		SI_Float GetDot(const CSIBCVector3D& i_vVector) const;

		/*! Returns the distance between this vector and \p i_vVector.
			\param	i_vVector	Vector to determine distance from.
			\return	SI_Float	Distance from this vector to \p i_vVector.
			\sa	CSIBCVector3D::GetLength
		*/
		SI_Float GetDistanceFrom(const CSIBCVector3D& i_vVector) const;
		
		/*! Returns the length of this vector.
			\return	SI_Float	The length of this vector.
			\sa	CSIBCVector3D::GetSquaredLength
			\sa	CSIBCVector3D::GetMaxComponent
			\sa	CSIBCVector3D::GetMinComponent
		*/
		SI_Float GetLength() const;

		/*! Returns the squared length of this vector.
			\return SI_Float	The squared length of this vector.
			\sa	CSIBCVector3D::GetLength
			\sa	CSIBCVector3D::GetMaxComponent
			\sa	CSIBCVector3D::GetMinComponent
		*/
		SI_Float GetSquaredLength() const;

		/*! Returns the value and index of the maximum component of this vector.
			\param	o_nIndex	Pointer to an SI_Int to hold the value of the index that had the
					maximum component (0 == X, 1 == Y, 2 == Z).
			\return	SI_Float	The value of the maximum component of this vector.
			\sa	CSIBCVector3D::Get
		*/
		SI_Float GetMaxComponent(SI_Int * o_nIndex) const;

		/*! Returns the value and index of the minimum component of this vector.
			\param	o_nIndex	Pointer to an SI_Int to hold the value of the index that had the
					maximum component (0 == X, 1 == Y, 2 == Z).
			\return	SI_Float	The value of the maximum component of this vector.
			\sa	CSIBCVector3D::Get
		*/
		SI_Float GetMinComponent(SI_Int * o_nIndex) const;
		
		// Components

		/*! Gets the values of the vector components.
			\param	o_fX	Pointer to an SI_Float to receive the value of the X component.
			\param	o_fY	Pointer to an SI_Float to receive the value of the Y component.
			\param	o_fZ	Pointer to an SI_Float to receive the value of the Z component.
			\sa	CSIBCVector3D::GetX
			\sa	CSIBCVector3D::GetY
			\sa	CSIBCVector3D::GetZ
			\sa	CSIBCVector3D::GetW
			\sa CSIBCVector3D::Set
		*/
		void Get(SI_Float* o_fX, SI_Float* o_fY, SI_Float* o_fZ) const;

		/*!	Gets the X component of this vector.
			\return	SI_Float	The value of the X component of this vector.
			\sa CSIBCVector3D::GetY
			\sa CSIBCVector3D::GetZ
			\sa CSIBCVector3D::GetW
			\sa CSIBCVector3D::Get
		*/
		SI_Float GetX() const { return m_fX; }

		/*!	Gets the Y component of this vector.
			\return	SI_Float	The value of the Y component of this vector.
			\sa CSIBCVector3D::GetX
			\sa CSIBCVector3D::GetZ
			\sa CSIBCVector3D::GetW
			\sa CSIBCVector3D::Get
		*/
		SI_Float GetY() const { return m_fY; }

		/*!	Gets the Z component of this vector.
			\return	SI_Float	The value of the Z component of this vector.
			\sa CSIBCVector3D::GetX
			\sa CSIBCVector3D::GetY
			\sa CSIBCVector3D::GetW
			\sa CSIBCVector3D::Get
		*/
		SI_Float GetZ() const { return m_fZ; }

		/*!	Gets the W component of this vector. This function is used for compatibility
			with CSIBCVector4D objects, and always returns 1.0f.
			\return	SI_Float	The value of the W component of this vector.
			\sa CSIBCVector3D::GetX
			\sa CSIBCVector3D::GetY
			\sa CSIBCVector3D::GetZ
			\sa CSIBCVector3D::Get
		*/
		SI_Float GetW() const { return 1.0; }


		/*! Sets the values of the X, Y and Z components of this vector.
			\param	i_fX	The new value for the X component of this vector.
			\param	i_fY	The new value for the Y component of this vector.
			\param	i_fZ	The new value for the Z component of this vector.
			\return	CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::Get
		*/
		CSIBCVector3D& Set(SI_Float i_fX, SI_Float i_fY, SI_Float i_fZ);
		
		// Operations

		/*! Normalizes the vector to a length of one. If the vector is null, then it is
			initialized to (1.0f, 0.0f, 0.0f).
			\return	CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::SetLength
			\sa	CSIBCVector3D::GetLength
		*/
		CSIBCVector3D& Normalize();

		/*! Sets this vector to null. (Components equal 0.0f).
			\return CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::IsNull
			\sa CSIBCVector3D::Set
		*/
		CSIBCVector3D& SetNull();

		/*! Sets the length of this vector to be \p i_fLength. If the vector is null 
			(See CSIBCVector3D::IsNull), then the vector is unchanged.
			\param	i_fLength	New length of the vector.
			\return	CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::GetLength
			\sa CSIBCVector3D::Set
		*/
		CSIBCVector3D& SetLength(const SI_Float i_fLength);

		/*! Sets this vector to be the cross product of this vector and \p i_vVector.
			\param	i_vVector	The vector to cross this vector with.
			\return	CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::CrossProduct
		*/
		CSIBCVector3D& SetCross(const CSIBCVector3D& i_vVector);

		/*! Sets all the components of this vector to be the negatives of their current values.
			\return	CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::Set
		*/
		CSIBCVector3D& Negate();

		/*! Adds this vector to \p i_vVector, and stores the result in this vector.
			\param	i_vVector	The vector to add to this vector.
			\return	CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::Sub
		*/
		CSIBCVector3D& Add(const CSIBCVector3D& i_vVector);

		/*! Subtracts \p i_vVector from this vector, and stores the result in this vector.
			\param	i_vVector	The vector to subtract from this vector.
			\return	CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::Add
		*/
		CSIBCVector3D& Sub(const CSIBCVector3D& i_vVector);

		/*!	Scales all components of this vector by \p i_fScale.
			\param	i_fScale	The amount to scale each of the components of this vector by.
			\return	CSIBCVector3D& Reference to this vector.
			\sa CSIBCVector3D::Scale(CSIBCVector3D&, const SI_Float)
			\sa CSIBCVector3D::operator*
			\sa CSIBCVector3D::SetLength
		*/
		CSIBCVector3D& Scale(const SI_Float i_fScale);

		/*! Scales all components of this vector by \p i_fScale, and stores the result in \p o_vDest.
			\param	o_vDest		Vector to receive the scaled vector.
			\param	i_fScale	The amount to scale each component of this vector.
			\return	CSIBCVector3D&	Reference to this vector.
		*/	
		CSIBCVector3D& Scale(CSIBCVector3D& o_vDest, const SI_Float i_fScale);

		/*! Multiplies each component in this vector by its corresponding component in \p i_vVector.
			The result is stored in this vector.
			\param	i_vVector	Vector to multiply components with this vector.
			\return	CSIBCVector3D&	Reference to this vector
			\sa CSIBCVector3D::DivComp
			\sa CSIBCVector3D::CrossProduct
			\sa CSIBCVector3D::SetCross
		*/
		CSIBCVector3D& MulComp(const CSIBCVector3D& i_vVector);
		
		/*! Divides each component in this vector by its corresponding component in \p i_vVector.
			The result is stored in this vector.
			\param	i_vVector	Vector to divide components with this vector.
			\return CSIBCVector3D&	Reference to this vector.
			\sa CSIBCVector3D::operator/
			\sa CSIBCVector3D::MulComp
		*/
		CSIBCVector3D& DivComp(const CSIBCVector3D& i_vVector);
		
		/*! Computes the cross product between this vector and \p i_vVector. The operation is as
			follows: Vout = Vthis x Vin.

			\param	i_vVector	Vector to find the cross product with.
			\return	CSIBCVector3D	The result of the cross product.
			\sa	CSIBCVector3D::SetCross
		*/
		CSIBCVector3D  CrossProduct( const CSIBCVector3D& i_vVector)  const;

		/*! Scales the components of this vector by \p i_fScale, and add the result to
			the \p io_vDest vector. The result is stored in \p io_vDest.
			\param	io_vDest	Vector to add to this scale vector. Holds the results of this operation.
			\param	i_fScale	The amount to scale each component of this vector (before addition).

			\sa CSIBCVector3D::Add
			\sa CSIBCVector3D::Scale
		*/
		void ScaleAndAdd(CSIBCVector3D& io_vDest, const SI_Float i_fScale);

	    // overloaded operators

		/*! Determines whether this vector and \p i_vVector are equal. Note that this function does
			not use any floating-point comparison error, thus the two vectors must be exactly the
			same.
			\param	i_vVector	Vector to compare equality with.
			\return	SI_Bool		TRUE if the vectors are equal, FALSE otherwise.
			\sa	CSIBCVector3D::IsAlmostEqualTo
			\sa CSIBCVector3D::operator!=
		*/
		SI_Bool operator == (const CSIBCVector3D& i_vVector) const;

		/*! Determines whether this vector and \p i_vVector are not-equal. Note that this function does
			not use any floating-point comparison error, thus the two vectors must be exactly the
			same for a FALSE return value.
			\param	i_vVector	Vector to compare inequality with.
			\return	SI_Bool		TRUE if the vectors are not equal, FALSE otherwise.
			\sa	CSIBCVector3D::IsAlmostEqualTo
			\sa CSIBCVector3D::operator==
		*/
		SI_Bool operator != (const CSIBCVector3D& i_vVector) const;

		/*! Computes the vector addition between this vector and \p i_vVector.
			\param	i_vVector	The vector to add this vector with.
			\return	CSIBCVector3D	The result of the addition.
			\sa CSIBCVector3D::Add
			\sa CSIBCVector3D::operator+(const SI_Float&)
			\sa CSIBCVector3D::operator-(const CSIBCVector3D&)
			\sa CSIBCVector3D::operator-(const SI_Float&)
		*/
		CSIBCVector3D operator+(const CSIBCVector3D &i_vVector) const;

		/*! Computes the vector resulting from adding a constant value, \p i_fFloat, to
			each component of this vector.
			\param	i_fFloat	The amount to add to each component of this vector.
			\return	CSIBCVector3D	The result of the addition.
			\sa CSIBCVector3D::Add
			\sa CSIBCVector3D::operator+(const CSIBCVector3D&)
			\sa CSIBCVector3D::operator-(const CSIBCVector3D&)
			\sa CSIBCVector3D::operator-(const SI_Float&)
		*/
		CSIBCVector3D operator+(const SI_Float &i_fFloat) const;

		/*! Computes the vector subtraction of \p i_vVector from this vector.
			\param	i_vVector	The vector to subtract from this vector.
			\return	CSIBCVector3D	The result of the subtraction.
			\sa CSIBCVector3D::Sub
			\sa CSIBCVector3D::operator-(const SI_Float&)
			\sa CSIBCVector3D::operator+(const CSIBCVector3D &)
			\sa CSIBCVector3D::operator+(const SI_Float&)
		*/
		CSIBCVector3D operator-(const CSIBCVector3D &i_vVector) const;

		/*! Computes the vector resulting from subtracting a constant value, \p i_fFloat, from
			each component of this vector.
			\param	i_fFloat	The amount to subtract from each component of this vector.
			\return	CSIBCVector3D	The result of the subtraction.
			\sa CSIBCVector3D::Sub
			\sa CSIBCVector3D::operator-(const CSIBCVector3D&)
			\sa CSIBCVector3D::operator+(const SI_Float&)
			\sa CSIBCVector3D::operator+(const CSIBCVector3D&)
		*/
		CSIBCVector3D operator-(const SI_Float &i_fFloat) const;

		/*! Divides each component in this vector by its corresponding component in \p i_vVector.
			\param	i_vVector	Vector to divide components with this vector.
			\return CSIBCVector3D	The result of the divisions.
			\sa CSIBCVector3D::DivComp
		*/
		CSIBCVector3D operator/(const CSIBCVector3D &i_vVector) const;

		/*! Multiplies each component of this vector by \p i_fFloat.
			\param	i_fFloat	The amount to scale each component of this vector by.
			\return	CSIBCVector3D	The result of the scaling.
			\sa CSIBCVector3D::Scale
			\sa CSIBCVector3D::MulComp
		*/	
		CSIBCVector3D operator*(const SI_Float i_fFloat) const;

		/*! Outputs the memory location of this object, and the values of each
			of its components.
			\sa SIBCVector3D::Get
		*/
		void	Dump();

		/*! The X component of this vector. */
		float	m_fX;
#ifdef _PSX2_ASM
			__attribute__((aligned (16)))	// XYZ components (W is just padding)
#endif

		/*! The Y component of this vector. */
		float	m_fY;

		/*! The Z component of this vector. */
		float	m_fZ;


};


//***************************************************************************************
// CSIBCVector4D | General purpose vector class.
//***************************************************************************************

//! General purpuse 4D vector class. 
/*! Representation of a 4D vector, defining operations to access, modify and perform calculations
	on 2D vectors. This class uses single precision floating-point numbers for represent its data,
	and in its operations. There are seperate similar classes for 2D and 3D vectors, CSIBCVector2D and
	CSIBCVector3D, respectively, which also use single precision.
	
	Furthermore, there is another set of vector classes which use double precision floating-point numbers,
	CSIBCVector2Dd, CSIBCVector3Dd and CSIBCVector4Dd for 2D, 3D and 4D vectors respectively. Quaternions
	represented as 4D vectors are also available in CSIBCQuaternion and CSIBCQuaterniond with
	single precision and double precision, repspectively.
*/
class XSICOREEXPORT CSIBCVector4D
{
	public:     

		// Constructors and destructor

		/*! Default constructor.
			Sets the values of each component of this vector to zero.
		*/		
		CSIBCVector4D();

		/*! Copy constructor.
			Sets the new vector's component values to be the same as \p i_vVector.
			\param	i_vVector	Vector to use for new vector's component values.
		*/
		CSIBCVector4D(const CSIBCVector4D& i_vVector);

		/*! Constructor.
			Sets the new vector to have \p i_fX, \p i_fY, \p i_fZ and \p i_fW for its X, Y, Z and
			W components, respectively.
			\param	i_fX	The value for the X component of the new vector.
			\param	i_fY	The value for the Y component of the new vector.
			\param	i_fZ	The value for the Z component of the new vector.
			\param	i_fW	The value for the W component of the new vector.
		*/
		CSIBCVector4D(	const SI_Float i_fX, const SI_Float i_fY, 
						const SI_Float i_fZ, const SI_Float i_fW);

		/*! Constructor.
			Sets the new vector to have the X, Y and Z components values of \p i_vVct, and
			the W component \p i_fW.
			\param	i_vVct	The vector containing the X, Y and Z components to use for the new vector.
			\param	i_fW	The W value for the new vector (defaults to 1.0f).
		*/
		CSIBCVector4D(	const CSIBCVector3D& i_vVct, const SI_Float i_fW = 1.0f);

		~CSIBCVector4D();
		
		// Comparison

		/*! Determines whether this vector and \p i_vVector are equal to each other, within a 
			threshhold value given by i_fEpsilon.
			\param	i_vVector	Vector to compare equality with.
			\param	i_fEpsilon	Error range for floating-point comparison.
			\return	SI_Bool		TRUE if the vectors are equal, FALSE otherwise.
			\sa	CSIBCVector4D::IsOrthogonalTo
			\sa	CSIBCVector4D::IsNull
			\sa	CSIBCVector4D::operator==
		*/
		SI_Bool IsAlmostEqualTo(const CSIBCVector4D& i_vVector, const SI_Float i_fEpsilon);

		/*! Determines whether this vector and \p i_vVector are orthogonal (at 90 degrees to
			each other). This function uses a floating-point comparison error of #PICO_EPS.

			\param	i_vVector	The vector to compare orthogonality with.
			\return	SI_Bool		TRUE if the vectors are orthogonal, FALSE otherwise.
			\sa	CSIBCVector4D::IsAlmostEqualTo
			\sa	CSIBCVector4D::IsNull
			\sa	CSIBCVector4D::GetDot
		*/
		SI_Bool IsOrthogonalTo(const CSIBCVector4D& i_vVector);

		/*! Determines whether all of the components of this vector are zero. This function
			uses a floating-point comparison error of #PICO_EPS.

			\return	SI_Bool		TRUE if all of the vector's components are zero, FALSE otherwise.
			\sa	CSIBCVector4D::IsAlmostEqualTo
			\sa	CSIBCVector4D::IsNull
		*/
		SI_Bool IsNull() const;

		// Computed Info

		/*! Computes the dot product between this vector and \p i_vVector.
			\param	i_vVector	Vector to dot-product with this vector.
			\return	SI_Float	Dot product of the two vectors.
			\sa	CSIBCVector4D::IsOrthogonalTo
		*/
		SI_Float GetDot(const CSIBCVector4D& i_vVector) const;
		
		/*! Returns the distance between this vector and \p i_vVector.
			\param	i_vVector	Vector to determine distance from.
			\return	SI_Float	Distance from this vector to \p i_vVector.
			\sa	CSIBCVector4D::GetLength
		*/
		SI_Float GetDistanceFrom(const CSIBCVector4D& i_vVector) const;

		/*! Returns the length of this vector.
			\return	SI_Float	The length of this vector.
			\sa	CSIBCVector4D::GetSquaredLength
			\sa	CSIBCVector4D::GetMaxComponent
			\sa	CSIBCVector4D::GetMinComponent
		*/
		SI_Float GetLength() const;

		/*! Returns the squared length of this vector.
			\return SI_Float	The squared length of this vector.
			\sa	CSIBCVector4D::GetLength
			\sa	CSIBCVector4D::GetMaxComponent
			\sa	CSIBCVector4D::GetMinComponent
		*/
		SI_Float GetSquaredLength() const;

		/*! Returns the value and index of the maximum component of this vector.
			\param	o_nIndex	Pointer to an SI_Int to hold the value of the index that had the
					maximum component (0 == X, 1 == Y, 2 == Z, 3 == W).
			\return	SI_Float	The value of the maximum component of this vector.
			\sa	CSIBCVector4D::Get
		*/
		SI_Float GetMaxComponent(SI_Int * o_nIndex) const;

		/*! Returns the value and index of the minimum component of this vector.
			\param	o_nIndex	Pointer to an SI_Int to hold the value of the index that had the
					maximum component (0 == X, 1 == Y, 2 == Z, 3 == W).
			\return	SI_Float	The value of the maximum component of this vector.
			\sa	CSIBCVector4D::Get
		*/
		SI_Float GetMinComponent(SI_Int * o_nIndex) const;
		
		// Components

		/*! Gets the values of the vector components.
			\param	o_fX	Pointer to an SI_Float to receive the value of the X component.
			\param	o_fY	Pointer to an SI_Float to receive the value of the Y component.
			\param	o_fZ	Pointer to an SI_Float to receive the value of the Z component.
			\param	o_fW	Pointer to an SI_Float to receive the value of the W component.
			\sa CSIBCVector4D::Set
		*/
		void Get(SI_Float* o_fX, SI_Float* o_fY, SI_Float* o_fZ, SI_Float* o_fW) const;
		
		/*! Sets the values of the X, Y, Z and W components of this vector.
			\param	i_fX	The new value for the X component of this vector.
			\param	i_fY	The new value for the Y component of this vector.
			\param	i_fZ	The new value for the Z component of this vector.
			\param	i_fW	The new value for the W component of this vector.
			\return	CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::Get
			\sa CSIBCVector4D::operator=
		*/
		CSIBCVector4D& Set(SI_Float i_fX, SI_Float i_fY, SI_Float i_fZ, SI_Float i_fW);
		
		// Operations

		/*! Normalizes the vector to a length of one. If the vector is null, then it is
			initialized to (1.0f, 0.0f, 0.0f, 0.0f).
			\return	CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::SetLength
			\sa	CSIBCVector4D::GetLength
		*/
		CSIBCVector4D& Normalize();

		/*! Sets this vector to null. (Components equal 0.0f).
			\return CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::IsNull
			\sa CSIBCVector4D::Set
		*/
		CSIBCVector4D& SetNull();

		/*! Sets the length of this vector to be \p i_fLength. If the vector is null 
			(See CSIBCVector4D::IsNull), then the vector is unchanged.
			\param	i_fLength	New length of the vector.
			\return	CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::GetLength
			\sa CSIBCVector4D::Set
		*/
		CSIBCVector4D& SetLength(const SI_Float i_fLength);

		/*! Sets all the components of this vector to be the negatives of their current values.
			\return	CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::Set
		*/
		CSIBCVector4D& Negate();

		/*! Adds this vector to \p i_vVector, and stores the result in this vector.
			\param	i_vVector	The vector to add to this vector.
			\return	CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::Sub
		*/
		CSIBCVector4D& Add(const CSIBCVector4D& i_vVector);

		/*! Subtracts \p i_vVector from this vector, and stores the result in this vector.
			\param	i_vVector	The vector to subtract from this vector.
			\return	CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::Add
		*/
		CSIBCVector4D& Sub(const CSIBCVector4D& i_vVector);

		/*!	Scales all components of this vector by \p i_fScale.
			\param	i_fScale	The amount to scale each of the components of this vector by.
			\return	CSIBCVector4D& Reference to this vector.
			\sa CSIBCVector4D::SetLength
		*/
		CSIBCVector4D& Scale(const SI_Float i_fScale);

		/*! Multiplies each component in this vector by its corresponding component in \p i_vVector.
			The result is stored in this vector.
			\param	i_vVector	Vector to multiply components with this vector.
			\return	CSIBCVector4D&	Reference to this vector
			\sa CSIBCVector4D::DivComp
		*/
		CSIBCVector4D& MulComp(const CSIBCVector4D& i_vVector);

		/*! Divides each component in this vector by its corresponding component in \p i_vVector.
			The result is stored in this vector.
			\param	i_vVector	Vector to divide components with this vector.
			\return CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::operator/
			\sa CSIBCVector4D::MulComp
		*/
		CSIBCVector4D& DivComp(const CSIBCVector4D& i_vVector);
		
	    // overloaded operators

		/*! Determines whether this vector and \p i_vVector are equal. Note that this function does
			not use any floating-point comparison error, thus the two vectors must be exactly the
			same.
			\param	i_vVector	Vector to compare equality with.
			\return	SI_Bool		TRUE if the vectors are equal, FALSE otherwise.
			\sa	CSIBCVector4D::IsAlmostEqualTo
			\sa CSIBCVector4D::operator!=
		*/
		SI_Bool operator == (const CSIBCVector4D& i_vVector) const;

		/*! Determines whether this vector and \p i_vVector are not-equal. Note that this function does
			not use any floating-point comparison error, thus the two vectors must be exactly the
			same for a FALSE return value.
			\param	i_vVector	Vector to compare inequality with.
			\return	SI_Bool		TRUE if the vectors are not equal, FALSE otherwise.
			\sa	CSIBCVector4D::IsAlmostEqualTo
			\sa CSIBCVector4D::operator==
		*/
		SI_Bool operator != (const CSIBCVector4D& i_vVector) const;

		/*! Computes the vector addition between this vector and \p i_vVector.
			\param	i_vVector	The vector to add this vector with.
			\return	CSIBCVector4D	The result of the addition.
			\sa CSIBCVector4D::Add
			\sa CSIBCVector4D::operator-
		*/
		CSIBCVector4D operator+(const CSIBCVector4D & i_vVector);

		/*! Computes the vector subtraction of \p i_vVector from this vector.
			\param	i_vVector	The vector to subtract from this vector.
			\return	CSIBCVector4D	The result of the subtraction.
			\sa CSIBCVector4D::Sub
			\sa CSIBCVector4D::operator+
		*/
		CSIBCVector4D operator-(const CSIBCVector4D & i_vVector);

		/*! Divides each component in this vector by its corresponding component in \p i_vVector.
			\param	i_vVector	Vector to divide components with this vector.
			\return CSIBCVector4D	The result of the divisions.
			\sa CSIBCVector4D::DivComp
		*/
		CSIBCVector4D operator/(const CSIBCVector4D & i_vVector);

		/*! Sets this vector to be equal to \p i_vVector. 
			\param	i_vVector	The vector to copy data from.
			\return	CSIBCVector4D&	Reference to this vector.
			\sa CSIBCVector4D::Set
		*/
		CSIBCVector4D &operator = (const CSIBCVector3D & i_vVector);

		// Debug functions

		/*! Outputs the memory location of this object, and the values of each
			of its components.
			\sa SIBCVector4D::Get
		*/
		void	Dump();
		
		/*! The X component of this vector. */
		float	m_fX; 
#ifdef _PSX2_ASM
			__attribute__((aligned (16)))
#endif
		/*! The Y component of this vector. */
		float	m_fY;

		/*! The Z component of this vector. */		
		float	m_fZ;
		
		/*! The W component of this vector. */
		float	m_fW;

	protected:
};

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Constructor.
// CSIBCVector2D::CSIBCVector2D();
// CSIBCVector2D::CSIBCVector2D( const CSIBCVector2D& in_vct );
// CSIBCVector2D::CSIBCVector2D( const SI_Float in_dX, const SI_Float in_dY );
//
//***************************************************************************************
inline CSIBCVector2D::CSIBCVector2D()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Copy constructor.
// CSIBCVector2D& | in_vct | 2D vector to copy.
//
//***************************************************************************************
inline CSIBCVector2D::CSIBCVector2D
(
    const CSIBCVector2D& in_vct 
)
{
    m_fX = in_vct.m_fX;
    m_fY = in_vct.m_fY;
}




//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Constructor with xy values.
// SI_Float | in_dX | X value of 2D vector.
// SI_Float | in_dY | Y value of 2D vector.
//
//***************************************************************************************
inline CSIBCVector2D::CSIBCVector2D
(
    const SI_Float in_dX,
    const SI_Float in_dY
)
{
    m_fX = (SI_Float) in_dX;
    m_fY = (SI_Float) in_dY;
}

 

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Destructor. No dynamic allocation is done so this is empty.
//
//
//***************************************************************************************
inline CSIBCVector2D::~CSIBCVector2D()
{
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is almost equal to the other vector.
//
// CSIBCVector2D& | in_vct | 2D vector to compare.
// SI_Float | in_dEpsilon | Error tolerance.
// Returns a bool.
//
//***************************************************************************************
inline SI_Bool CSIBCVector2D::IsAlmostEqualTo
( 
    const CSIBCVector2D&	in_vct,
    const SI_Float			in_dEpsilon  
)
{
    return( fabs( m_fX - in_vct.m_fX ) < in_dEpsilon &&
			fabs( m_fY - in_vct.m_fY ) < in_dEpsilon);
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is orthogonal (90 degrees) to another.
//
// CSIBCVector2D& | in_vct | 2D vector to compare. 
// Returns a bool
//
//***************************************************************************************
inline SI_Bool CSIBCVector2D::IsOrthogonalTo
( 
    const CSIBCVector2D& in_vct
)
{
    if( fabs( GetDot( in_vct ) ) < PICO_EPS) 
        return TRUE;
    else
        return FALSE;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is the null vector.
//
//
//***************************************************************************************
inline SI_Bool CSIBCVector2D::IsNull() const 
{
    // No test is done on W
    const SI_Float l_dEpsilon = PICO_EPS;
    return ( fabs( m_fX ) < l_dEpsilon &&
		fabs( m_fY ) < l_dEpsilon );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns the dot product of this vector with another vector.
// CSIBCVector2D& | in_vct | Input vector.
// Returns dot product.
//
//***************************************************************************************
inline SI_Float CSIBCVector2D::GetDot
( 
    const CSIBCVector2D& in_vct
) const
{
    return ( m_fX*in_vct.m_fX + m_fY*in_vct.m_fY);
}

 
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns the distance of this vector to another one.
//
// CSIBCVector2D& | in_vct | Input vector.
// Distance of input vector from another vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector2D::GetDistanceFrom
( 
    const CSIBCVector2D& in_vct 
) const
{
    SI_Float rx, ry;

    rx = m_fX-in_vct.m_fX;  
    ry = m_fY-in_vct.m_fY;  

    return (SI_Float)sqrt( rx*rx + ry*ry );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the squared length of this vector.
// Returns the squared length of this vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector2D::GetSquaredLength() const
{
    return( m_fX*m_fX + m_fY*m_fY );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the length of this vector.
// Returns the length of this vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector2D::GetLength() const
{
    return (SI_Float)sqrt( GetSquaredLength() );
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Retuns the XY values of this vector.
//
// SI_Float* | out_dX | X value of the vector to be returned.
// SI_Float* | out_dY | Y value of the vector to be returned.
//
//***************************************************************************************
inline void CSIBCVector2D::Get
(
    SI_Float* out_dX,
    SI_Float* out_dY
) const
{
    *out_dX = m_fX;
    *out_dY = m_fY;
}

 
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the length of the vector to in_dLength units.
// 
// SI_Float | in_dLength | Length to set the vector to.
// Returns a pointer to the CSIBCVector2D object.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::SetLength
( 
    const SI_Float in_dLength 
)
{
    SI_Float  rLen = GetLength();

    if ( rLen > PICO_EPS )
    {
        Scale( in_dLength / rLen );
        return *this;
    }
    else
        return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Normalizes the vector. If the vector is null then we initialize to (1,0).
// Returns the normalized vector.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::Normalize()
{
    SI_Float  rLen = GetLength();

    if ( rLen > PICO_EPS )
    {
        m_fX /= (SI_Float) rLen ;
        m_fY /= (SI_Float) rLen ;
        return *this;
    }
    else
    {
		return Set(1.0f, 0.0f);
    }
}


  


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns TRUE if this vector is the same as the other.
//
// CSIBCVector2D& | in_v | Input vector.
// Returns a bool.
//
//***************************************************************************************
inline SI_Bool CSIBCVector2D::operator ==
( 
	const CSIBCVector2D& in_v
) const
{
    return ( ( m_fX == in_v.m_fX ) && ( m_fY == in_v.m_fY ) );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns TRUE if this vector is different from the other.
// CSIBCVector2D& | in_vct | Input vector.
// Returns a bool.
//
//***************************************************************************************
inline SI_Bool CSIBCVector2D::operator !=
( 
	const CSIBCVector2D& in_vct 
) const
{
    return !( *this==in_vct ) ;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Adds two vectors.
// CSIBCVector2D& | in_vtx | Input vector.
// Returns a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D CSIBCVector2D::operator+
(
	const CSIBCVector2D &in_vtx
)  const
{
	CSIBCVector2D v2;

	v2.m_fX = m_fX + in_vtx.m_fX;
	v2.m_fY = m_fY + in_vtx.m_fY;

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector sub operator.
// CSIBCVector2D& | in_vtx | Input vector.
// Returns a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D CSIBCVector2D::operator-
(
	const CSIBCVector2D &in_vtx	
) const
{
	CSIBCVector2D v2;

	v2.m_fX = m_fX - in_vtx.m_fX;
	v2.m_fY = m_fY - in_vtx.m_fY;

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector divide operator.
// CSIBCVector2D& | in_vtx | Input vector.
// Returns a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D CSIBCVector2D::operator/
(
	const CSIBCVector2D &in_vtx
) const
{
	CSIBCVector2D v2;

	if ((in_vtx.m_fX != 0.0f) &&
		(in_vtx.m_fY != 0.0f))
	{
		v2.m_fX = m_fX / in_vtx.m_fX;
		v2.m_fY = m_fY / in_vtx.m_fY;
	}

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector multiply operator.
// CSIBCVector2D& | in_vtx | Input vector.
// Returns a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D CSIBCVector2D::operator*
(
	const SI_Float in_flt
) const
{
	CSIBCVector2D v2;

	v2.m_fX = m_fX * in_flt;
	v2.m_fY = m_fY * in_flt;

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the vector to the NULL vector.
// Returns a NULL vector.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::SetNull()
{ 
    m_fX = m_fY = 0.0f; 

    return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets this vector to be the opposite vector.
// Returns a vector which is the opposite.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::Negate()
{
    m_fX = -m_fX;
    m_fY = -m_fY;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Adds a vector to this vector.
// CSIBCVector2D& | in_vct | Input vector.
// Returns a pointer to a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::Add
(
    const CSIBCVector2D& in_vct
)
{
    m_fX += in_vct.m_fX;
    m_fY += in_vct.m_fY;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Substracts a vector from this vector.
// CSIBCVector2D& | in_vct | Input vector.
// Returns a pointer to a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::Sub
( 
    const CSIBCVector2D& in_vct
)
{
    m_fX -= in_vct.m_fX;
    m_fY -= in_vct.m_fY;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Scales a vector by a given factor.
// SI_Float | in_dFactor | Scale factor.
// Returns a pointer to a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::Scale
( 
    const SI_Float in_dFactor
)
{
    m_fX *= (SI_Float) in_dFactor;
    m_fY *= (SI_Float) in_dFactor;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Does component-wise multiplication of the vector.
// CSIBCVector2D& | in_vct | Input vector.
// Returns a pointer to a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::MulComp
( 
     const CSIBCVector2D& in_vct    
)
{
    m_fX *= in_vct.m_fX;
    m_fY *= in_vct.m_fY;
    return *this; 
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Does component-wise division of the vector. 
// CSIBCVector2D& | in_vct | Input vector.
// Returns a pointer to a new CSIBCVector2D.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::DivComp
( 
    const CSIBCVector2D& in_vct
)
{
	if (in_vct.m_fX != 0.0f &&
		in_vct.m_fY != 0.0f)
	{
		m_fX /= in_vct.m_fX;
		m_fY /= in_vct.m_fY;
	}

    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the xy values of this vector.
//
// SI_Float | in_dX | x value of the vector.
// SI_Float | in_dY | y value of the vector.
// Returns a pointer to the new CSIBCVector2D object.
//
//***************************************************************************************
inline CSIBCVector2D& CSIBCVector2D::Set
(
    SI_Float in_dX,
    SI_Float in_dY
)
{
    m_fX = (SI_Float) in_dX;
    m_fY = (SI_Float) in_dY;
    return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Constructor.
//
// CSIBCVector3D::CSIBCVector3D();
// CSIBCVector3D::CSIBCVector3D( const CSIBCVector3D& in_vct );
// CSIBCVector3D::CSIBCVector3D( const SI_Float in_dX, const SI_Float in_dY, const SI_Float in_dZ );
//
//***************************************************************************************
inline CSIBCVector3D::CSIBCVector3D()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
    m_fZ = 0.0f;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Copy constructor.
// CSIBCVector3D& | in_vct | Input vector to be copied.
//
//***************************************************************************************
inline CSIBCVector3D::CSIBCVector3D
(
    const CSIBCVector3D& in_vct 
)
{
    m_fX = in_vct.m_fX;
    m_fY = in_vct.m_fY;
    m_fZ = in_vct.m_fZ;
}




//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Constructor with xyz values. We assume w = 1.
// SI_Float | in_dX | x value of the vector.
// SI_Float | in_dY | y value of the vector.
// SI_Float | in_dZ | z value of the vector.
//
//***************************************************************************************
inline CSIBCVector3D::CSIBCVector3D
(
    const SI_Float in_dX,
    const SI_Float in_dY,
    const SI_Float in_dZ
)
{
    m_fX = (SI_Float) in_dX;
    m_fY = (SI_Float) in_dY;
    m_fZ = (SI_Float) in_dZ;
}

 


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is almost equal to the other vector.
//
// CSIBCVector3D& | in_vct | 3D vector to compare.
// SI_Float | in_dEpsilon | Error tolerance.
// Returns a boolean.
//***************************************************************************************
inline SI_Bool CSIBCVector3D::IsAlmostEqualTo
( 
    const CSIBCVector3D&	in_vct,
    const SI_Float			in_dEpsilon  
)
{
    return( fabs( m_fX - in_vct.m_fX ) < in_dEpsilon &&
              fabs( m_fY - in_vct.m_fY ) < in_dEpsilon &&
              fabs( m_fZ - in_vct.m_fZ ) < in_dEpsilon ) ;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is orthogonal (90 degrees) to another.
// CSIBCVector3D& | in_vct | 3D vector to compare.
// Returns a boolean.
//
//***************************************************************************************
inline SI_Bool CSIBCVector3D::IsOrthogonalTo
( 
    const CSIBCVector3D& in_vct
)
{
    if( fabs( GetDot( in_vct ) ) < PICO_EPS) 
        return TRUE;
    else
        return FALSE;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is the null vector.
// Returns a boolean.
// 
//
//***************************************************************************************
inline SI_Bool CSIBCVector3D::IsNull() const 
{
    // No test is done on W
    const SI_Float l_dEpsilon = PICO_EPS;
    return ( fabs( m_fX ) < l_dEpsilon &&
            fabs( m_fY ) < l_dEpsilon &&
            fabs( m_fZ ) < l_dEpsilon );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the dot product of this vector with another vector.
// CSIBCVector3D& | in_vct | 3D vector to compare.
// Returns the dot product of this vector with another vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector3D::GetDot
( 
    const CSIBCVector3D& in_vct
) const
{
#ifdef _PSX2_ASM
	float fRet[4] __attribute__((aligned (16)));
	asm ("
		lqc2     		vf4,0x0(%1)
		lqc2 			vf5,0x0(%2)
		vmul.xyz 		vf5,vf4,vf5
		vaddy.x  		vf5,vf5,vf5
		vaddz.x  		vf5,vf5,vf5
		sqc2			vf5,0x0(%0)
	": : "=r" (fRet), "r" (&m_fX), "r" ( &(in_vct.m_fX)) );
	
	return fRet[0];
#else
	return ( in_vct.m_fX * m_fX + in_vct.m_fY * m_fY + in_vct.m_fZ * m_fZ );
#endif	
}

 
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the distance of this vector to another one.
// Returns the dot product of this vector with another vector.
// Returns the distance of this vector to another one.
//***************************************************************************************
inline SI_Float CSIBCVector3D::GetDistanceFrom
( 
    const CSIBCVector3D& in_vct 
) const
{
#ifdef _PSX2_ASM
	float fRet[4] __attribute__((aligned (16)));
	
	asm ("
		lqc2     		vf4,0x0(%1)
		lqc2 			vf5,0x0(%2)
		vsub.xyz 		vf5,vf4,vf5
		vmul.xyz  		vf5,vf5,vf5
		vaddy.x  		vf5,vf5,vf5
		vaddz.x  		vf5,vf5,vf5
		sqc2			vf5,0x0(%0)
	": : "=r" (fRet), "r" (&m_fX), "r" ( &(in_vct.m_fX)) );
	
	return (SI_Float)sqrt(fRet[0]);
#else
    SI_Float rx, ry, rz;

    rx = m_fX-in_vct.m_fX;  
    ry = m_fY-in_vct.m_fY;  
    rz = m_fZ-in_vct.m_fZ;  

    return (SI_Float)sqrt( rx*rx + ry*ry + rz*rz );
#endif	
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the length of this vector.
// Returns the length of this vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector3D::GetLength() const
{
#ifdef _PSX2_ASM
	SI_Float fRet[4] __attribute__((aligned (16)));
	
	asm ("
		lqc2     		vf5,0x0(%1)
		vmul.xyz  		vf5,vf5,vf5
		vaddy.x  		vf5,vf5,vf5
		vaddz.x  		vf5,vf5,vf5
		cop2     		2425789
		vwaitq
		vaddq.x  		vf5,vf0,Q
		sqc2			vf5,0x0(%0)
	": : "=r" (fRet), "r" (&m_fX) );
	
	return fRet[0];
#else
   return (SI_Float)sqrt( GetSquaredLength() );
#endif	
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the squared length of this vector.
// Returns the squared length of this vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector3D::GetSquaredLength() const
{
#ifdef _PSX2_ASM
	SI_Float fRet[4] __attribute__((aligned (16)));
	
	asm ("
		lqc2     		vf4,0x0(%1)
		vmul.xyz  		vf4,vf4,vf4
		vaddy.x  		vf4,vf4,vf4
		vaddz.x  		vf4,vf4,vf4
		sqc2			vf4,0x0(%0)
	": : "=r" (fRet), "r" (&m_fX) );
	
	return fRet[0];
#else	
    return( m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ );
#endif	
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns the XYZ values of this vector.
//
// SI_Float* | out_dX | x component of the vector. 
// SI_Float* | out_dY | y component of the vector.
// SI_Float* | out_dZ | z component of the vector.
//
//***************************************************************************************
inline void CSIBCVector3D::Get
(
    SI_Float* out_dX,
    SI_Float* out_dY,
    SI_Float* out_dZ
) const
{
    *out_dX = m_fX;
    *out_dY = m_fY;
    *out_dZ = m_fZ;
}

 
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the length of the vector to in_dLength units.
//
// SI_Float | in_dLength | length to set the vector to.
// Returns a pointer to the new vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::SetLength
( 
    const SI_Float in_dLength 
)
{
    SI_Float  rLen = GetLength();

    if ( rLen > PICO_EPS )
    {
        Scale( in_dLength / rLen );
        return *this;
    }
    else
        return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Normalizes the vector. If the vector is null then we initialize to (1,0,0,0).
// Returns the normalized vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::Normalize()
{
#ifdef _PSX2_ASM
	sceVu0Normalize( &m_fX, &m_fX );
        
    return *this;
#else
    SI_Float  rLen = GetLength();

    if ( rLen > PICO_EPS )
    {
        m_fX /= (SI_Float) rLen ;
        m_fY /= (SI_Float) rLen ;
        m_fZ /= (SI_Float) rLen ; 
        return *this;
    }
    else
    {
		return Set(1.0f, 0.0f, 0.0f);
    }
#endif
}


  


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns TRUE if this vector is the same as the other.
// CSIBCVector3D& | in_v | Input vector.
// Returns a boolean.
//
//***************************************************************************************
inline SI_Bool CSIBCVector3D::operator ==
( 
	const CSIBCVector3D& in_v
) const
{
    return ( ( m_fX == in_v.m_fX ) && ( m_fY == in_v.m_fY ) && ( m_fZ == in_v.m_fZ ) );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns TRUE if this vector is different from the other.
// CSIBCVector3D& | in_v | Input vector.
// Returns a boolean.
//
//***************************************************************************************
inline SI_Bool CSIBCVector3D::operator !=
( 
	const CSIBCVector3D& in_vct 
) const
{
    return !( *this==in_vct ) ;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector add operator.
//
// CSIBCVector3D& | in_vtx | Input vector.
// Returns a new vector.
//***************************************************************************************
inline CSIBCVector3D CSIBCVector3D::operator+
(
	const CSIBCVector3D &in_vtx				
) const
{
	CSIBCVector3D v2;

	v2.m_fX = m_fX + in_vtx.m_fX;
	v2.m_fY = m_fY + in_vtx.m_fY;
	v2.m_fZ = m_fZ + in_vtx.m_fZ;

	return v2;
}

inline CSIBCVector3D CSIBCVector3D::operator+
(
	const SI_Float &in_flt				
) const
{
	CSIBCVector3D v2;

	v2.m_fX = m_fX + in_flt;
	v2.m_fY = m_fY + in_flt;
	v2.m_fZ = m_fZ + in_flt;

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector subtract operator.
//
// CSIBCVector3D& | in_vtx | Input vector.
// Returns a new vector.
//
//***************************************************************************************
inline CSIBCVector3D CSIBCVector3D::operator-
(
	const CSIBCVector3D &in_vtx
) const
{
	CSIBCVector3D v2;

	v2.m_fX = m_fX - in_vtx.m_fX;
	v2.m_fY = m_fY - in_vtx.m_fY;
	v2.m_fZ = m_fZ - in_vtx.m_fZ;

	return v2;
}
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector subtract operator.
//
// CSIBCVector3D& | in_flt | Input vector.
// Returns a new vector.
//
//***************************************************************************************

inline CSIBCVector3D CSIBCVector3D::operator-
(
	const SI_Float &in_flt
) const
{
	CSIBCVector3D v2;

	v2.m_fX = m_fX - in_flt;
	v2.m_fY = m_fY - in_flt;
	v2.m_fZ = m_fZ - in_flt;

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector divide operator.
// CSIBCVector3D& | in_vtx | Iinput vector.
// Returns a new vector.
//
//***************************************************************************************
inline CSIBCVector3D CSIBCVector3D::operator/
(
	const CSIBCVector3D &in_vtx
) const
{
	CSIBCVector3D v2;

	if ((in_vtx.m_fX != 0.0f) &&
		(in_vtx.m_fY != 0.0f) &&
		(in_vtx.m_fZ != 0.0f))
	{
		v2.m_fX = m_fX / in_vtx.m_fX;
		v2.m_fY = m_fY / in_vtx.m_fY;
		v2.m_fZ = m_fZ / in_vtx.m_fZ;
	}

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector multiply operator.
// SI_Float| factor | Multiplication factor.
// Returns a new vector.
//
//***************************************************************************************
inline CSIBCVector3D CSIBCVector3D::operator*
(
	const SI_Float factor
) const
{
	CSIBCVector3D v2;
	
	v2.m_fX = m_fX * factor;
	v2.m_fY = m_fY * factor;
	v2.m_fZ = m_fZ * factor;

	return v2;
}
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the vector to the NULL vector.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::SetNull()
{ 
    m_fX = m_fY = m_fZ = 0.0f; 

    return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets this vector to be the opposite vector.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::Negate()
{
    m_fX = -m_fX;
    m_fY = -m_fY;
    m_fZ = -m_fZ;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Adds a vector to this vector.
//
// CSIBCVector3D& | in_vct | Input vector.
// Returns a pointer to the vector.
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::Add
(
    const CSIBCVector3D& in_vct
)
{
    m_fX += in_vct.m_fX;
    m_fY += in_vct.m_fY;
    m_fZ += in_vct.m_fZ;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Substracts a vector from this vector.
// CSIBCVector3D& | in_vct | Input vector.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::Sub
( 
    const CSIBCVector3D& in_vct
)
{
    m_fX -= in_vct.m_fX;
    m_fY -= in_vct.m_fY;
    m_fZ -= in_vct.m_fZ;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Scales a vector by a given factor.
// SIFloat | in_dFactor | Scale factor.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::Scale
( 
    const SI_Float in_dFactor
)
{
    m_fX *= (SI_Float) in_dFactor;
    m_fY *= (SI_Float) in_dFactor;
    m_fZ *= (SI_Float) in_dFactor;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Scales a vector by a given factor.
//
// CSIBCVector3D& | o_V | Vector to apply scaling to.
// SI_Float | in_dFactor | Scaling factor.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::Scale
( 
	CSIBCVector3D &o_V,
    const SI_Float in_dFactor
)
{
	o_V.m_fX = m_fX * in_dFactor;
	o_V.m_fY = m_fY * in_dFactor;
	o_V.m_fZ = m_fZ * in_dFactor;
    
    return *this;
}
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Scales a vector by a given factor.
//
// CSIBCVector3D& | o_V | Vector to apply scaling to.
// SI_Float | in_dFactor | Scaling factor.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline void CSIBCVector3D::ScaleAndAdd
(
	CSIBCVector3D &o_V, 
	const SI_Float in_dFactor
)
{
	o_V.m_fX += (m_fX * in_dFactor);
	o_V.m_fY += (m_fY * in_dFactor);
	o_V.m_fZ += (m_fZ * in_dFactor);
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Does component wise multiplication of the vector. 
// CSIBCVector3D& | in_vct | Input vector.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::MulComp
( 
     const CSIBCVector3D& in_vct    
)
{
    m_fX *= in_vct.m_fX;
    m_fY *= in_vct.m_fY;
    m_fZ *= in_vct.m_fZ;
    return *this; 
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Does component wise division of the vector. 
// CSIBCVector3D& | in_vct | Input vector.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::DivComp
( 
    const CSIBCVector3D& in_vct
)
{
	if (in_vct.m_fX != 0.0f &&
		in_vct.m_fY != 0.0f &&
		in_vct.m_fZ != 0.0f)
	{
		m_fX /= in_vct.m_fX;
		m_fY /= in_vct.m_fY;
		m_fZ /= in_vct.m_fZ;
	}

    return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets this vector to be the cross product of this and the other vector.
// CSIBCVector3D& | in_vct | Input vector.
// Returns a pointer to the vector.
//
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::SetCross
( 
    const CSIBCVector3D& in_vct
)
{
    SI_Float dX, dY, dZ;
    
    dX =  m_fY * in_vct.m_fZ - m_fZ * in_vct.m_fY ;
    dY = -m_fX * in_vct.m_fZ + m_fZ * in_vct.m_fX ;
    dZ =  m_fX * in_vct.m_fY - m_fY * in_vct.m_fX ;

   return( Set( dX, dY, dZ ) );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns a cross product.
// CSIBCVector3D& | in_vct | Input vector.
// Returns the new.
//
//***************************************************************************************
inline CSIBCVector3D  CSIBCVector3D::CrossProduct( const CSIBCVector3D &in_vct )  const
{
	CSIBCVector3D tmp;
#ifdef _PSX2_ASM
	sceVu0OuterProduct( &tmp.m_fX, (float*)&m_fX, (float*)&in_vct.m_fX );
#else
	tmp.m_fX =  m_fY * in_vct.m_fZ - m_fZ * in_vct.m_fY ;
    tmp.m_fY = -m_fX * in_vct.m_fZ + m_fZ * in_vct.m_fX ;
    tmp.m_fZ =  m_fX * in_vct.m_fY - m_fY * in_vct.m_fX ;
#endif
	return tmp;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the xyz values of this vector.
//
// SI_Float | in_dX | x component of the vector.
// SI_Float | in_dY | y component of the vector.
// SI_Float | in_dZ | z component of the vector.
// Returns a pointer to the vector.
//***************************************************************************************
inline CSIBCVector3D& CSIBCVector3D::Set
(
    SI_Float in_dX,
    SI_Float in_dY,
    SI_Float in_dZ
)
{
    m_fX = (SI_Float) in_dX;
    m_fY = (SI_Float) in_dY;
    m_fZ = (SI_Float) in_dZ;

    return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Constructor.
//
// CSIBCVector4D::CSIBCVector4D();
// CSIBCVector4D::CSIBCVector4D( const CSIBCVector4D& in_vct );
// inline CSIBCVector4D::CSIBCVector4D( const SI_Float in_dX, const SI_Float in_dY, const SI_Float in_dZ, const SI_Float in_dW );
//
//***************************************************************************************
inline CSIBCVector4D::CSIBCVector4D()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
    m_fZ = 0.0f;
	m_fW = 0.0f;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Copy constructor.
// CSIBCVector4D& | in_vct | Input vector.
//
//
//***************************************************************************************
inline CSIBCVector4D::CSIBCVector4D
(
    const CSIBCVector4D& in_vct 
)
{
    m_fX = in_vct.m_fX;
    m_fY = in_vct.m_fY;
    m_fZ = in_vct.m_fZ;
	m_fW = in_vct.m_fW;
}




//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Constructor with xyzw values.
// SI_Float | in_dX | x component of the vector.
// SI_Float | in_dY | y component of the vector.
// SI_Float | in_dZ | z component of the vector.
// SI_Float | in_dW | w component of the vector.
//***************************************************************************************
inline CSIBCVector4D::CSIBCVector4D
(
    const SI_Float in_dX,
    const SI_Float in_dY,
    const SI_Float in_dZ,
    const SI_Float in_dW
)
{
    m_fX = (SI_Float) in_dX;
    m_fY = (SI_Float) in_dY;
    m_fZ = (SI_Float) in_dZ;
    m_fW = (SI_Float) in_dW;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Constructor with xyzw values.
// SIBCVector3D& | in_vVct | xyz component of the vector.
// SI_Float | in_dW | w component of the vector.
//***************************************************************************************
inline CSIBCVector4D::CSIBCVector4D
(
	const CSIBCVector3D& in_vVct, const SI_Float in_dW
)
{
	m_fX = in_vVct.m_fX;
	m_fY = in_vVct.m_fY;
	m_fZ = in_vVct.m_fZ;
	m_fW = in_dW;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Destructor. No dynamic allocation is done so this is empty.
//
//
//***************************************************************************************
inline CSIBCVector4D::~CSIBCVector4D()
{
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is almost equal to the other vector.
//
// CSIBCVector4D& | in_vct | Input vector.
// SI_Float | in_dEpsilon | Error tolerance.
// Returns a boolean.
//
//***************************************************************************************
inline SI_Bool CSIBCVector4D::IsAlmostEqualTo
( 
    const CSIBCVector4D&	in_vct,
    const SI_Float			in_dEpsilon  
)
{
    return( fabs( m_fX - in_vct.m_fX ) < in_dEpsilon &&
		fabs( m_fY - in_vct.m_fY ) < in_dEpsilon &&
		fabs( m_fZ - in_vct.m_fZ ) < in_dEpsilon &&
		fabs( m_fW - in_vct.m_fW ) < in_dEpsilon ) ;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is orthogonal (90 degrees) to another.
// CSIBCVector4D& | in_vct | Input vector.
// Returns a boolean.
//
//***************************************************************************************
inline SI_Bool CSIBCVector4D::IsOrthogonalTo
( 
    const CSIBCVector4D& in_vct
)
{
    if( fabs( GetDot( in_vct ) ) < PICO_EPS) 
        return TRUE;
    else
        return FALSE;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Tests if this vector is the null vector.
// Returns a boolean.
//
//***************************************************************************************
inline SI_Bool CSIBCVector4D::IsNull() const 
{
    // No test is done on W
    const SI_Float l_dEpsilon = PICO_EPS;
    return ( fabs( m_fX ) < l_dEpsilon &&
        fabs( m_fY ) < l_dEpsilon &&
        fabs( m_fZ ) < l_dEpsilon &&
        fabs( m_fW ) < l_dEpsilon );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the dot product of this vector with another vector.
//
// CSIBCVector4D& | in_vct | Input vector.
// Returns the dot product of this vector with another vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector4D::GetDot
( 
    const CSIBCVector4D& in_vct
) const
{
    return ( m_fX*in_vct.m_fX + m_fY*in_vct.m_fY + m_fZ*in_vct.m_fZ + m_fW*in_vct.m_fW);
}

 
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the distance of this vector to another one.
//
// CSIBCVector4D& | in_vct | Input vector.
// Returns the distance of this vector to another one.
//
//***************************************************************************************
inline SI_Float CSIBCVector4D::GetDistanceFrom
( 
    const CSIBCVector4D& in_vct 
) const
{
    SI_Float rx, ry, rz, rw;

    rx = m_fX-in_vct.m_fX;  
    ry = m_fY-in_vct.m_fY;  
    rz = m_fZ-in_vct.m_fZ;  
    rw = m_fW-in_vct.m_fW;  

    return (SI_Float)sqrt( rx*rx + ry*ry + rz*rz + rw*rw );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns the length of this vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector4D::GetLength() const
{
    return (SI_Float)sqrt( GetSquaredLength() );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the squared length of this vector.
// Returns the suqared length of this vector.
//
//***************************************************************************************
inline SI_Float CSIBCVector4D::GetSquaredLength() const
{
    return( m_fX*m_fX + m_fY*m_fY + m_fZ*m_fZ + m_fW*m_fW);
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Gets the XYZW values of this vector.
//
// SI_Float* | out_dX | the x component of the vector.
// SI_Float* | out_dY | the y component of the vector.
// SI_Float* | out_dZ | the z component of the vector.
// SI_Float* | out_dW | the w component of the vector.
//***************************************************************************************
inline void CSIBCVector4D::Get
(
    SI_Float* out_dX,
    SI_Float* out_dY,
    SI_Float* out_dZ,
    SI_Float* out_dW
) const
{
    *out_dX = m_fX;
    *out_dY = m_fY;
    *out_dZ = m_fZ;
    *out_dW = m_fW;
}
 
//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the length of the vector to in_dLength units.
//
// SI_Float | in_dLength | Length of the vector.
// Returns a pointer to a vector object.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::SetLength
( 
    const SI_Float in_dLength 
)
{
    SI_Float  rLen = GetLength();

    if ( rLen > PICO_EPS )
    {
        Scale( in_dLength / rLen );
        return *this;
    }
    else
        return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Normalizes the vector. If the vector is null then we initialize to (1,0,0,0).
// Returns a pointer to a vector object.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::Normalize()
{
    SI_Float  rLen = GetLength();

    if ( rLen > PICO_EPS )
    {
        m_fX /= (SI_Float) rLen ;
        m_fY /= (SI_Float) rLen ;
        m_fZ /= (SI_Float) rLen ; 
        m_fW /= (SI_Float) rLen ; 
        return *this;
    }
    else
    {
		return Set(1.0f, 0.0f, 0.0f, 0.0f);
    }
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns TRUE if this vector is the same as the other.
//
// CSIBCVector4D& | in_v | Input vector.
// Returns a boolean.
//***************************************************************************************
inline SI_Bool CSIBCVector4D::operator ==
( 
	const CSIBCVector4D& in_v
) const
{
    return ( ( m_fX == in_v.m_fX ) && ( m_fY == in_v.m_fY ) 
		&& ( m_fZ == in_v.m_fZ ) && ( m_fW == in_v.m_fW ) );
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Returns TRUE if this vector is different from the other.
// CSIBCVector4D& | in_vct | Input vector.
//
//***************************************************************************************
inline SI_Bool CSIBCVector4D::operator !=
( 
	const CSIBCVector4D& in_vct 
) const
{
    return !( *this==in_vct ) ;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector add operator.
// CSIBCVector4D& | in_vtx | Input vector.
// Returns a vector.
//
//***************************************************************************************
inline CSIBCVector4D CSIBCVector4D::operator+
(
	const CSIBCVector4D &in_vtx
)
{
	CSIBCVector4D v2;

	v2.m_fX = m_fX + in_vtx.m_fX;
	v2.m_fY = m_fY + in_vtx.m_fY;
	v2.m_fZ = m_fZ + in_vtx.m_fZ;
	v2.m_fW = m_fW + in_vtx.m_fW;

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector sub operator.
// CSIBCVector4D& | in_vtx | Input vector.
// Returns a vector.
//
//***************************************************************************************
inline CSIBCVector4D CSIBCVector4D::operator-
(
	const CSIBCVector4D &in_vtx
)
{
	CSIBCVector4D v2;

	v2.m_fX = m_fX - in_vtx.m_fX;
	v2.m_fY = m_fY - in_vtx.m_fY;
	v2.m_fZ = m_fZ - in_vtx.m_fZ;
	v2.m_fW = m_fW - in_vtx.m_fW;

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Vector divide operator.
// CSIBCVector4D& | in_vtx | Input vector.
// Returns a vector.
//
//***************************************************************************************
inline CSIBCVector4D CSIBCVector4D::operator/
(
	const CSIBCVector4D &in_vtx
)
{
	CSIBCVector4D v2;

	if ((in_vtx.m_fX != 0.0f) &&
		(in_vtx.m_fY != 0.0f) &&
		(in_vtx.m_fZ != 0.0f) &&
		(in_vtx.m_fW != 0.0f))
	{
		v2.m_fX = m_fX / in_vtx.m_fX;
		v2.m_fY = m_fY / in_vtx.m_fY;
		v2.m_fZ = m_fZ / in_vtx.m_fZ;
		v2.m_fW = m_fW / in_vtx.m_fW;
	}

	return v2;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Assignment operator.
// CSIBCVector4D& | in_vtx | Input vector.
// Returns a pointer to a vector.
//
//***************************************************************************************
inline CSIBCVector4D &CSIBCVector4D::operator = (const CSIBCVector3D &in_vtx )
{
	m_fX = in_vtx.m_fX;
	m_fY = in_vtx.m_fY;
	m_fZ = in_vtx.m_fZ;	
	return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the vector to the NULL vector.
// Returns a pointer to a vector.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::SetNull()
{ 
    m_fX = m_fY = m_fZ = m_fW = 0.0f; 

    return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets this vector to be the opposite vector.
// Returns a pointer to a vector.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::Negate()
{
    m_fX = -m_fX;
    m_fY = -m_fY;
    m_fZ = -m_fZ;
    m_fW = -m_fW;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Adds a vector to this vector.
// CSIBCVector4D& | in_vct | Input vector.
// Returns a pointer to a vector.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::Add
(
    const CSIBCVector4D& in_vct
)
{
    m_fX += in_vct.m_fX;
    m_fY += in_vct.m_fY;
    m_fZ += in_vct.m_fZ;
    m_fW += in_vct.m_fW;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Substracts a vector from this vector.
// CSIBCVector4D& | in_vct | Input vector.
// Returns a pointer to a vector.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::Sub
( 
    const CSIBCVector4D& in_vct
)
{
    m_fX -= in_vct.m_fX;
    m_fY -= in_vct.m_fY;
    m_fZ -= in_vct.m_fZ;
    m_fW -= in_vct.m_fW;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Scales a vector by a given factor.
//
// SI_Float | in_dFactor | Scale factor.
// Returns a pointer to a vector object.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::Scale
( 
    const SI_Float in_dFactor
)
{
    m_fX *= (SI_Float) in_dFactor;
    m_fY *= (SI_Float) in_dFactor;
    m_fZ *= (SI_Float) in_dFactor;
    m_fW *= (SI_Float) in_dFactor;
    return *this;
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Does component-wise multiplication of the vector. 
//
// CSIBCVector4D& | in_vct | Input vector.
// Returns a pointer to a vector.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::MulComp
( 
     const CSIBCVector4D& in_vct    
)
{
    m_fX *= in_vct.m_fX;
    m_fY *= in_vct.m_fY;
    m_fZ *= in_vct.m_fZ;
    return *this; 
}


//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Does component wise division of the vector. 
// CSIBCVector4D& | in_vct | Input vector.
// Returns a pointer to a vector.
//
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::DivComp
( 
    const CSIBCVector4D& in_vct
)
{
	if (in_vct.m_fX != 0.0f &&
		in_vct.m_fY != 0.0f &&
		in_vct.m_fZ != 0.0f &&
		in_vct.m_fW != 0.0f)
	{
		m_fX /= in_vct.m_fX;
		m_fY /= in_vct.m_fY;
		m_fZ /= in_vct.m_fZ;
		m_fW /= in_vct.m_fW;
	}

    return *this;
}

//***************************************************************************************
//
// Author : Softimage Games Team
// Date   : 
//
// Sets the xyzw values of this vector.
//
// SI_Float | in_dX | x component of vector.
// SI_Float | in_dY | y component of vector.
// SI_Float | in_dZ | z component of vector.
// SI_Float | in_dW | w component of vector.
// Returns a pointer to a vector.
//***************************************************************************************
inline CSIBCVector4D& CSIBCVector4D::Set
(
    SI_Float in_dX,
    SI_Float in_dY,
    SI_Float in_dZ,
    SI_Float in_dW
)
{
    m_fX = (SI_Float) in_dX;
    m_fY = (SI_Float) in_dY;
    m_fZ = (SI_Float) in_dZ;
    m_fW = (SI_Float) in_dW;
    return *this;
}


#endif // CSIBCVector
