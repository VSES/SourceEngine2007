//*****************************************************************************
/*!
   \file xsi_boolarray.h
   \brief CBoolArray class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIBOOLARRAY_H__
#define __XSIBOOLARRAY_H__

#include "sicppsdk.h"

namespace XSI {

class CStatus;

//*************************************************************************
/*! \class CBoolArray xsi_boolarray.h
   \brief The CBoolArray is an uni-dimensional array of bool values.

   	The array is zero-based, which means that the first item index is 0 and the larger index is one less
   	that the number of elements contained in the array.

	\sa CEdgeRefArray::GetIsHardArray
*/
//*************************************************************************
class SICPPSDKDECL CBoolArray
{
public:
	/*! Constructs a CBoolArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CBoolArray( LONG in_size=0);

	/*! Constructs a CBoolArray object from another CBoolArray object.
	\param in_array constant CBoolArray object.
	*/
	CBoolArray(const CBoolArray& in_array);

	/*! Default destructor. */
	~CBoolArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	*/
	CBoolArray& operator=( const CBoolArray& in_array );

	/*! Returns the number of items in this CBoolArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a bool to the end of this array.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const bool& in_item );

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

	/*! Accessor to elements at a given index. This function can only be called by constant objects, the returned
	value is read-only.
	\param in_index Index in this zero based array. The index must be smaller than the number of elements in the
		array, otherwise the results are unpredicted.
	\return A read-only reference to the indexed item.
	*/
	const bool& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index Index in this zero based array. The index must be smaller than the number of elements in the
		array, otherwise the results are unpredicted.
	\return A reference to the indexed item.
	*/
	bool& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CBoolArray to compare with.
	\return True if \c in_array contains the same number of elements as this array and that all members of
		\c in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CBoolArray& in_array) const;

	/*! Inequality operator.
	\param in_array CBoolArray to compare with.
	\return True if one member of \c in_array is different of the corresponding members in this	array or if
		the arrays are not of the same size.
	*/
	bool operator!=(const CBoolArray& in_array) const;


private:

	void* m_ptr;
};

};

#endif // __XSIDOUBLEARRAY_H__
