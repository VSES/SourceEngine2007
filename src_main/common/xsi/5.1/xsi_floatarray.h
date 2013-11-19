//*****************************************************************************
/*!
   \file xsi_floatarray.h
   \brief CFloatArray class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIFLOATARRAY_H__
#define __XSIFLOATARRAY_H__

#include "sicppsdk.h"

namespace XSI {

class CStatus;

//*************************************************************************
/*! \class CFloatArray xsi_floatarray.h
	\brief The CFloatArray is an uni-dimensional array of floats.

	The array is zero-based, which means that the first item index is 0 and the larger index is one less than
	the number of elements contained in the array.

	Items are added with CFloatArray::Add. You can also use CFloatArray::Attach to encapsulate an external array.
	When an array is attached, its items are accessible through the CFloatArray API.

	\since 5.0
*/
//*************************************************************************
class SICPPSDKDECL CFloatArray
{
public:
	/*! Constructs a CFloatArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CFloatArray( LONG in_size=0);

	/*! Constructs a CFloatArray object from another CFloatArray object.
	\param in_array constant CFloatArray object.
	*/
	CFloatArray(const CFloatArray& in_array);

	/*! Default destructor. */
	~CFloatArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CFloatArray& operator=( const CFloatArray& in_array );

	/*! Adds the value of each element of \c in_array to the value of the corresponding element
	of this CFloatArray. The result is stored in this CFloatArray object.
	\param in_array A constant CFloatArray object.
	\return A reference to this array.
	\since Gemini
	*/
	CFloatArray& operator+=( const CFloatArray& in_array );

	/*! Subtracts the value of each element of \c in_array from the value of the corresponding
	element of this CFloatArray. The result is stored in this CFloatArray object.
	\param in_array A constant CFloatArray object.
	\return A reference to this array.
	\since Gemini
	*/
	CFloatArray& operator-=( const CFloatArray& in_array );

	/*! Returns the number of items in this CFloatArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Encapsulates a pointer to float without taking ownership of it (i.e. the memory is not released
	when the CFloatArray is deleted). The content of the CFloatArray is erased before attaching the external
	array. Attach does not take a copy of the external array, however functions like Add and Resize will
	force a copy of the encapsulated array.

	\param in_pArray Array to encapsulate.
	\param in_nSize Number of elements in the array.

	\return CStatus::OK success
	\return CStatus::Fail Operation failed.
	\return CStatus::InvalidArgument Pointer is null or size is <= 0.

	\sa CFloatArray::Add, CFloatArray::Resize
	\since Gemini
	*/
	CStatus Attach( float* in_pArray, LONG in_nSize );

	/*! Adds a float at the end of this array.
	\param in_item New item to be added at the end of the array.
	\note There is a performance cost when calling Add if an external array has been attached with the Attach
		method: CFloatArray takes a copy of the external array before appending the item.
	\return CStatus::OK success
	\sa CFloatArray::Attach
	*/
	CStatus Add( const float& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if the new size is larger than existing size.

	\note There is a performance cost when calling Resize if an external array has 
	been attached with the Attach method. The method takes a copy of the external 
	array before resizing.

	\param in_size New size of the array.
	
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0

	\sa CFloatArray::Attach
	*/
	CStatus Resize( LONG in_size);

	/*! Reallocates memory for the array, preserves its contents if new size is larger than existing size.

	\note There is a performance cost when calling Resize if an external array has 
	been attached with the Attach method. The method takes a copy of the external 
	array before resizing.

	\param in_size New size of the array.
	\param in_bReset Set the array with 0 if true. If false the array is not reset and the content is preserved.
	
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	\sa CFloatArray::Attach
	*/
	CStatus Resize( LONG in_size, bool in_bReset );

	/*! Accessor to elements at a given index. This function can only be
	called by constant objects, the returned value is read-only.
	\param in_index index in this zero-based array.The index must be smaller than the number of elements
		in the array, otherwise the results are unpredicted.
	\return A read-only reference to the indexed item.
	*/
	const float& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.The index must be smaller than the number of elements
		in the array, otherwise the results are unpredicted.
	\return A reference to the indexed item.
	*/
	float& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CFloatArray to compare with.
	\return true if in_array contains the same number of elements as this array and all members of
		in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CFloatArray& in_array) const;

	/*! Inequality operator.
	\param in_array
	\return true if one member of in_array is different of the corresponding members in this array or
		if the arrays are not of the same size.
	*/
	bool operator!=(const CFloatArray& in_array) const;

	/*! Returns a string that contains the values of this array separated with the comma character.
	\return String of values.
	\since Gemini
	*/
	CString GetAsText() const;

	/*! Accessor to the internal array of floats. The array cannot be modified and the content is undefined
	when the CFloatArray object goes out of scope.
	\return Pointer to an array of floats.
	\since Gemini
	*/
	const float* GetArray() const;

	/*! Appends the content of a CFloatArray object to this one.
	\param in_array A constant CFloatArray object.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_array::GetCount is 0
	\since Gemini
	*/
	CStatus Append( const CFloatArray& in_array );

private:

	void* m_ptr;
};

};

#endif // __XSIFLOATARRAY_H__
