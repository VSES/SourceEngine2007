//*****************************************************************************
/*!
   \file xsi_color.h
   \brief CColor struct and CColorArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSI_COLOR_H__
#define __XSI_COLOR_H__

#include "sicppsdk.h"
#include "xsi_status.h"

namespace XSI {

//*************************************************************************
/*! \struct CColor xsi_color.h
   \brief This struct represents an RGBA color.

	The RBGA component values are stored as double precision numbers.

	CColor objects are created from OGLLight and OGLMaterial objects.

	\note If a component value is greater than a signed, single-precision value, the component value is
	interpreted as a negative value.

	In most cases, the component values are normalized to the range 0 to 1. TrianglePoint::GetColor is an
	exception, and returns values in the range 0 to 255.

	Other XSI objects and properties store colors differently:

		\li Shaders and XSI objects have separate Parameter objects for each
			color component (for example, \c Scene_Material.Phong.diffuse.green).
			These component values are normalized and use double precision.

		\li Vertex Color properties store color component values as arrays of
			doubles inside a ClusterProperty.

		\li Wireframe colors are encoded as 10-bit values (0 to 1023), with the least
			significant bit ignored. Each of the R,G,B channels is 3 bits (a number from 0 to 7),
			and the wireframe color is encoded as: \code 	B2|B1|B0|G2|G1|G0|R2|R1|R0|0| \endcode

	\sa CColorArray, OGLLight::GetColor, OGLMaterial::GetDiffuse, OGLMaterial::GetSpecular,
		OGLMaterial::GetAmbient
 */
//*************************************************************************
struct CColor
{
	double r; /*!< Red component value */
	double g; /*!< Green component value */
	double b; /*!< Blue component value */
	double a; /*!< Alpha component value */

	/*! Constructs a CColor object. */
	CColor() : r(0),g(0),b(0),a(0) {}

	/*! Constructs a new CColor object from an existing CColor object.
	\param in_color An existing CColor object to copy into this CColor object.
	*/
	CColor(const CColor& in_color) : r(in_color.r),g(in_color.g),
		b(in_color.b),a(in_color.a) {}

	/*! Constructs a new CColor object from RGBA values.
	\param in_rc Red component value.
	\param in_gc Green component value.
	\param in_bc Blue component value.
	\param in_ac Alpha component value.
	*/
	CColor(double in_rc, double in_gc, double in_bc, double in_ac) :
		r(in_rc),g(in_gc),b(in_bc),a(in_ac) {}

	/*! Tests whether two CColor objects are equal.
	\param in_color CColor to compare with this CColor object.
	\return True if the colors are equal, and false otherwise.
	*/
	bool operator==(const CColor& in_color)const
	{
		return (r == in_color.r) && (g == in_color.g) &&
			   (b == in_color.b) && (a == in_color.a);
	}

	/*! Tests whether two CColor objects are different.
	\param in_color CColor to compare with this CColor object
	\return True if the colors are different, and false is they are the same.
	*/
	bool operator!=(const CColor& in_color)const
	{
		return !operator==(in_color);
	}
};

//*************************************************************************
/*! \class CColorArray xsi_color.h
	\brief The CColorArray is a one-dimensional, zero-based array of CColor objects.

	\sa CColor
*/
//*************************************************************************
class SICPPSDKDECL CColorArray
{
public:
	/*! Constructs a CColorArray and optionally initializes the array to a specified size.
	\param in_size Size of array. The default size is 0.
	*/
	CColorArray( LONG in_size=0);

	/*! Constructs a new CColorArray object from an existing CColorArray object.
	\param in_array An existing CColorArray object to copy into this CColorArray object.
	*/
	CColorArray(const CColorArray& in_array );

	/*! Destroys a Group object. */
	~CColorArray();

	/*! Assigns a CColorArray object to an existing CColorArray object.
	\param in_array A CColorArray object to be copied into this object.
	\return The reinitialized CColorArray object.
	*/
	CColorArray& operator=( const CColorArray& in_array );

	/*! Returns the number of items in this CColorArray.
	\return The number of items in the array
	*/
	LONG GetCount() const;

	/*! Adds a CColor to the end of this array.
	\param in_item New CColor to add to the array.
	\return CStatus::OK if successful.
	*/
	CStatus Add( const CColor& in_item );

	/*! Removes all elements from the array.
	\return CStatus::OK if successful.
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, and preserves the existing contents if the new size is
	larger than the current size.
	\param in_size New size of the array.
	\return CStatus::OK if successful.
	\return CStatus::InvalidArgument if in_size < 0.
	*/
	CStatus Resize( LONG in_size);

	/*! Returns the array element at a given index. This function can only be called by constant
	objects, and the returned value is read-only.
	\param in_index The index of the element to get. The index must be greater than or equal to zero,
		and less than the number of elements in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CColor& operator[]( LONG in_index ) const;

	/*! Returns an array element at the specified index.
	\param in_index The index of the element to get. The index must be greater than or equal to zero,
		and less than the number of elements in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	CColor& operator[]( LONG in_index );

	/*! Tests if two CColorArray objects are equal.
	\param in_array CColorArray to compare with this array.
	\return True if \c in_array has the same size as this array and the elements of \c in_array are
		equal to the corresponding elements in this array.
	*/
	bool operator==(const CColorArray& in_array) const;

	/*! Tests if two CColorArray objects are different.
	\param in_array CColorArray to compare with this array.
	\return True if at least one element is different, or if the arrays have different sizes.
	*/
	bool operator!=(const CColorArray& in_array) const;

private:
	void* m_ptr;
};


};

#endif //__XSI_COLOR_H__
