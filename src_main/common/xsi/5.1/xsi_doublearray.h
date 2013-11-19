//*****************************************************************************
/*!
   \file xsi_doublearray.h
   \brief CDoubleArray class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIDOUBLEARRAY_H__
#define __XSIDOUBLEARRAY_H__

#include "sicppsdk.h"

namespace XSI {

class CStatus;

//*************************************************************************
/*! \class CDoubleArray xsi_doublearray.h
	\brief The CDoubleArray is an uni-dimensional array of doubles.

	The array is zero-based, which means that the first item index is 0 and the larger index is one less that the
	number of elements contained in the array.

	Items are added with CDoubleArray::Add. You can also use CDoubleArray::Attach to encapsulate an external array.
	When an array is attached, its items are accessible through the CDoubleArray API.

	\sa CClusterPropertyElementArray::GetArray, CClusterPropertyElementArray::GetItem, CClusterPropertyElementArray::PutItem,
		CClusterPropertyElementArray::GetItemsByIndex, CClusterPropertyElementArray::PutItemsByIndex, FCurve::SetKeys,
		CKnotArray::GetArray, CNurbsCurveData::m_aKnots, CNurbsSurfaceData::m_uKnots, CNurbsSurfaceData::m_vKnots
*/
//*************************************************************************
class SICPPSDKDECL CDoubleArray
{
public:
	/*! Constructs a CDoubleArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CDoubleArray( LONG in_size=0);

	/*! Constructs a CDoubleArray object from another CDoubleArray object.
	\param in_array constant CDoubleArray object.
	*/
	CDoubleArray(const CDoubleArray& in_array);

	/*! Default destructor. */
	~CDoubleArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CDoubleArray& operator=( const CDoubleArray& in_array );

	/*! Adds the value of each element of \c in_array to the value of the corresponding element
	of this CDoubleArray. The result is stored in this CDoubleArray object.
	\param in_array A constant CDoubleArray object.
	\return A reference to this array.
	\since Gemini
	*/
	CDoubleArray& operator+=( const CDoubleArray& in_array );

	/*! Subtracts the value of each element of \c in_array from the value of the corresponding element
	of this CDoubleArray. The result is stored in this CDoubleArray object.
	\param in_array A constant CDoubleArray object.
	\return A reference to this array.
	\since Gemini
	*/
	CDoubleArray& operator-=( const CDoubleArray& in_array );

	/*! Returns the number of items in this CDoubleArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Encapsulates a pointer to float without taking ownership of it (i.e. the memory is not released
	when the CDoubleArray is deleted). The content of the CDoubleArray is erased before attaching the
	external array. Attach does not take a copy of the external array, however functions like Add and
	Resize will force a copy of the encapsulated array.

	\param in_pArray Array to encapsulate.
	\param in_nSize Number of elements in the array.

	\return CStatus::OK success
	\return CStatus::Fail Operation failed.
	\return CStatus::InvalidArgument Pointer is null or size is <= 0.

	\sa CDoubleArray::Add, CDoubleArray::Resize
	\since Gemini
	*/
	CStatus Attach( double* in_pArray, LONG in_nSize );

	/*! Adds a double at the end of this array.
	\param in_item New item to be added at the end of the array.
	\note There is a performance cost when calling Add if an external array has been attached with the
		Attach method: CDoubleArray takes a copy of the external array before appending the item.
	\return CStatus::OK success
	\sa CDoubleArray::Attach
	*/
	CStatus Add( const double& in_item );

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

	\sa CDoubleArray::Attach
	*/
	CStatus Resize( LONG in_size );

	/*! Reallocates memory for the array, preserves its contents if the new size is larger than existing size.

	\note There is a performance cost when calling Resize if an external array has 
	been attached with the Attach method. The method takes a copy of the external 
	array before resizing.

	\param in_size New size of the array.
	\param in_bReset Set the array with 0 if true. If false the array is not reset and the content is preserved.
	
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	\sa CDoubleArray::Attach
	*/
	CStatus Resize( LONG in_size, bool in_bReset );

	/*! Accessor to elements at a given index. This function can only be called by constant objects, the returned
	value is read-only.
	\param in_index Index in this zero-based array.The index must be smaller than the number of elements
		in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const double& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index Index in this zero-based array.The index must be smaller than the number of elements
		in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	double& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CDoubleArray to compare with.
	\return True if in_array contains the same number of elements as this array and all members of \c in_array
		are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CDoubleArray& in_array) const;

	/*! Inequality operator.
	\param in_array CDoubleArray to compare with.
	\return true if one member of \c in_array is different from any of the corresponding members in this
		array or if the arrays are not the same size.
	*/
	bool operator!=(const CDoubleArray& in_array) const;

	/*! Returns a string that contains the values of this array separated with the comma character.
	\return String of values.
	\since Gemini
	*/
	CString GetAsText() const;

	/*! Accessor to the internal array of doubles. The array cannot be modified and the content is undefined
	when the CDoubleArray object goes out of scope.
	\return Pointer to an array of doubles.
	\since Gemini
	*/
	const double* GetArray() const;

	/*! Appends the content of a CDoubleArray object to this one.
	\param in_array A constant CDoubleArray object.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_array::GetCount is 0
	\since Gemini
	*/
	CStatus Append( const CDoubleArray& in_array );

private:

	void* m_ptr;
};

};

#endif // __XSIDOUBLEARRAY_H__
