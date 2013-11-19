//*****************************************************************************
/*!
   \file xsi_uv.h
   \brief CUV struct and CUVArray class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSI_UV_H__
#define __XSI_UV_H__

#include "sicppsdk.h"
#include "xsi_status.h"

namespace XSI {

//*************************************************************************
/*! \struct CUV xsi_uv.h
	\brief This structure represents a UV coordinate.

	CUV can be created from Triangle and TriangleVertex objects.

	\sa CUVArray, TriangleVertex::GetUV
 */
//*************************************************************************
struct CUV
{
	double u; /*!< U coordinate value */
	double v; /*!< V coordinate value */

	/*! Default constructor. */
	CUV() : u(0),v(0) {}

	/*! Copy constructor.
	\param in_uv Constant CUV object.
	*/
	CUV(const CUV& in_uv) : u(in_uv.u),v(in_uv.v) {}

	/*! Constructor. Creates a CUV object from uv values.
	\param in_u U coordinate value.
	\param in_v V coordinate value.
	*/
	CUV(double in_u, double in_v) : u(in_u),v(in_v) {}

	/*! Equality operator.
	\param in_uv CUV to compare with.
	\return true if they are equal; false if they are not equal.
	*/
	bool operator==(const CUV& in_uv)const
	{
		return (u == in_uv.u) && ( v == in_uv.v);
	}

	/*! Inequality operator.
	\param in_uv CUV to compare with.
	\return true if they are not equal; false if they are equal.
	*/
	bool operator!=(const CUV& in_uv)const
	{
		return ! operator==(in_uv);
	}
};

//*************************************************************************
/*! \class CUVArray xsi_uv.h
	\brief The CUVArray is an uni-dimensional array of CUV.

	The array is zero-based, which means that the first item index is zero and the larger index is one
	less that the number of elements contained in the array.

	\sa CUV,  CTriangleVertexRefArray::GetUVArray,
	CTriangleRefArray::GetUVArray, Triangle::GetUVArray
*/
//*************************************************************************
class SICPPSDKDECL CUVArray
{
public:
	/*! Constructs a CUVArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CUVArray( LONG in_size=0);

	/*! Construct a CUVArray object from another CUVArray object.
	\param in_array  constant CUVArray object.
	*/
	CUVArray(const CUVArray& in_array );

	/*! Default destructor. */
	~CUVArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	*/
	CUVArray& operator=( const CUVArray& in_array );

	/*! Returns the number of items in this CUVArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a CUV at the end of this array.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CUV& in_item );

	/*! Erases all elements contained in the array.
	\retval CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if the new size is larger
	than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be called by constant
	objects, the returned value is read-only.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return CUV&  A read-only reference to the indexed item.
	*/
	const CUV& operator[]( LONG  in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero
		and smaller than the number of element in the array, otherwise the results are unpredictable.
	\return CUV& A reference to the indexed item.
	*/
	CUV& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CUVArray to compare with.
	\return true if in_array contains the same number of elements as this array and if all members of
		in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CUVArray& in_array) const;

	/*! Inequality
	\param in_array
	\return true if one member of in_array is different from the corresponding members in this array
		or if the arrays are not of the same size.
	*/
	bool operator!=(const CUVArray& in_array) const;

private:

	void* m_ptr;
};


};

#endif //__XSI_UV_H__
