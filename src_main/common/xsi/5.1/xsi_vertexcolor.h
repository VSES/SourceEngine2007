//*****************************************************************************
/*!
   \file xsi_vertexcolor.h
   \brief CVertexColor and CVertexColorArray declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSI_VERTEXCOLOR_H__
#define __XSI_VERTEXCOLOR_H__

#include "sicppsdk.h"
#include "xsi_status.h"

namespace XSI {

//*************************************************************************
/*! \struct CVertexColor xsi_vertexcolor.h
	\brief This structure represents a vertex RGBA color. Unlike CColor, each component value of CVertexColor is a byte.

	CVertexColor can be created from Triangle and TriangleVertex objects.

	\sa CVertexColorArray
 */
//*************************************************************************
struct CVertexColor
{
	unsigned char r; /*!< Red component value */
	unsigned char g; /*!< Green component value */
	unsigned char b; /*!< Blue component value */
	unsigned char a; /*!< Alpha component value */

	/*! Default constructor. */
	CVertexColor() : r(0),g(0),b(0),a(0) {}

	/*! Copy constructor.
	\param in_color Constant CColor object
	*/
	CVertexColor(const CVertexColor& in_color) : r(in_color.r),
		g(in_color.g), b(in_color.b),a(in_color.a) {}

	/*! Constructor. Creates a CVertexColor object from r,g,b,a values.
	\param in_rc Red component value.
	\param in_gc Green component value.
	\param in_bc Blue component value.
	\param in_ac Alpha component value.
	*/
	CVertexColor(unsigned char in_rc, unsigned char in_gc,
		unsigned char in_bc, unsigned char in_ac) : r(in_rc),g(in_gc),
			b(in_bc),a(in_ac) {}

	/*! Equality operator.
	\param in_vertexcolor CVertexColor to compare with.
	\return true if they are equal; false if they are not equal.
	*/
	bool operator==(const CVertexColor& in_vertexcolor)const
	{
		return (r == in_vertexcolor.r) && (g == in_vertexcolor.g) &&
			   (b == in_vertexcolor.b) && (a == in_vertexcolor.a);
	}

	/*! Inequality operator.
	\param in_vertexcolor CVertexColor to compare with.
	\return true if they are not equal; false if they are equal.
	*/
	bool operator!=(const CVertexColor& in_vertexcolor)const
	{
		return ! operator==(in_vertexcolor);
	}
};


//*************************************************************************
/*! \class CVertexColorArray xsi_vertexcolor.h
	\brief The CVertexColorArray is an uni-dimensional array of CVertexColor objects.

	The array is zero-based, which means that the first item index is 0 and the larger index is one less
	that the number of elements contained in the array.

	\sa CVertexColor, CTriangleVertexRefArray::GetColorArray, CTriangleRefArray::GetColorArray,
		Triangle::GetColorArray
*/
//*************************************************************************
class SICPPSDKDECL CVertexColorArray
{
public:
	/*! Constructs a CVertexColorArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CVertexColorArray( LONG in_size=0);

	/*! Construct a CVertexColorArray object from another CVertexColorArray object.
	\param in_array  constant CVertexColorArray object.
	*/
	CVertexColorArray(const CVertexColorArray& in_array );

	/*! Default destructor. */
	~CVertexColorArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CVertexColorArray& operator=( const CVertexColorArray& in_array );

	/*! Returns the number of items in this CVertexColorArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a CVertexColor at the end of this array's.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CVertexColor& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if new new size is larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be called by constant objects, the
	returned value is read-only.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CVertexColor& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	CVertexColor& operator[](LONG in_index );

	/*! Equality operator.
	\param in_array CVertexColorArray to compare with.
	\return true if in_array contains the same number of elements as this array and if all members
		of in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CVertexColorArray& in_array) const;

	/*! Inequality operator.
	\param in_array
	\return true if one member of in_array is different from the corresponding members in this
		array or if the arrays are not of the same size.
	*/
	bool operator!=(const CVertexColorArray& in_array) const;

private:
	void* m_ptr;
};


};

#endif //__XSI_COLOR_H__
