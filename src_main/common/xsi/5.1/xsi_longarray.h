//*****************************************************************************
/*!
   \file xsi_longarray.h
   \brief CLongArray class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSILONGARRAY_H__
#define __XSILONGARRAY_H__

#include "sicppsdk.h"


namespace XSI {

class CStatus;
class CString;

//*************************************************************************
/*! \class CLongArray xsi_longarray.h
	\brief The CLongArray is an uni-dimensional array of LONG values.

	The array is zero-based, which means that the first item index is 0 and the larger index is one less than
	the number of elements contained in the array.

	Items are added with CLongArray::Add. You can also use CLongArray::Attach to encapsulate an external array.
	When an array is attached, its items are accessible through the CLongArray API.

	\sa Cluster::FindIndices, CClusterElementArray::GetArray, X3DObject::AddPolygonMesh, CTriangleRefArray::GetIndexArray,
		Triangle::GetIndexArray, CSegmentRefArray::GetIndexArray, CSampleRefArray::GetIndexArray, PolygonMesh::Get,
		PolygonMesh::Set, Point::GetIndexArray, CNurbsSampleRefArray::GetUVSamplingCoordinateArray,
		CNurbsSampleRefArray::GetIndexArrayFromUVSamplingCoordinateArray, Geometry::AddCluster, Facet::GetIndexArray,
		ClusterProperty::PutItemsByIndex, ClusterProperty::GetItemsByIndex, Geometry::CreateSubComponent
*/
//*************************************************************************
class SICPPSDKDECL CLongArray
{
public:
	/*! Constructs a CLongArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CLongArray( LONG in_size=0);

	/*! Constructs a CLongArray object from another CLongArray object.
	\param in_array constant CLongArray object.
	*/
	CLongArray(const CLongArray& in_array);

	/*! Default destructor. */
	~CLongArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CLongArray& operator=( const CLongArray& in_array );

	/*! Adds the value of each element of \c in_array to the value of the corresponding element
	of this CLongArray. The result is stored in this CLongArray object.
	\param in_array A constant CLongArray object.
	\return A reference to this array.
	\since Gemini
	*/
	CLongArray& operator+=( const CLongArray& in_array );

	/*! Subtracts the value of each element of \c in_array from the value of the corresponding element
	of this CLongArray. The result is stored in this CLongArray object.
	\param in_array A constant CLongArray object.
	\return A reference to this array.
	\since Gemini
	*/
	CLongArray& operator-=( const CLongArray& in_array );

	/*! Returns the number of items in this CLongArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Encapsulates a pointer to LONG without taking ownership of it (i.e. the memory is not released
	when the CLongArray is deleted). The content of the CLongArray is erased before attaching the external
	array. Attach does not take a copy of the external array, however functions like Add and Resize will
	force a copy of the encapsulated array.

	\param in_pArray Array to encapsulate.
	\param in_nSize Number of elements in the array.

	\return CStatus::OK success
	\return CStatus::Fail Operation failed.
	\return CStatus::InvalidArgument Pointer is null or size is <= 0.

	\sa CLongArray::Add, CLongArray::Resize
	\since Gemini
	*/
	CStatus Attach( LONG* in_pArray, LONG in_nSize );

	/*! Adds a LONG at the end of this array.
	\param in_item New item to be added at the end of the array.
	\note There is a performance cost when calling Add if an external array has been attached with the Attach
		method: CLongArray takes a copy of the external array before appending the item.
	\return CStatus::OK success
	\sa CLongArray::Attach
	*/
	CStatus Add( const LONG& in_item );

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

	\sa CLongArray::Attach
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
	\sa CLongArray::Attach
	*/
	CStatus Resize( LONG in_size, bool in_bReset );

	/*! Accessor to elements at a given index. This function can only be called by constant objects, the returned
	value is read-only.
	\param in_index index in this zero based array.The index must be smaller than the number of elements
		in the array, otherwise the results are unpredicted.
	\return A read-only reference to the indexed item.
	*/
	const LONG& operator[](LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.The index must be smaller than the number of elements
		in the array, otherwise the results are unpredicted.
	\return A reference to the indexed item.
	*/
	LONG& operator[](LONG in_index );

	/*! Equality operator.
	\param in_array CLongArray to compare with.
	\return true if in_array contains the same number of elements as this array and if all members of
		in_array are equal to the corresponding members of the current array.
	*/
	bool operator==(const CLongArray& in_array) const;

	/*! Inequality operator.
	\param in_array CLongArray to compare with.
	\return true if one member of in_array is different from the corresponding member in the current array
		or if the arrays are not the same size.
	*/
	bool operator!=(const CLongArray& in_array) const;

	/*! Returns a string that contains the values of this array separated with the comma character.
	\return String of values.
	\since Gemini
	*/
	CString GetAsText() const;

	/*! Accessor to the internal array of LONGs. The array cannot be modified and the content is undefined
	when the CLongArray object goes out of scope.
	\return Pointer to an array of LONGs.
	\since Gemini
	*/
	const LONG* GetArray() const;

	/*! Appends the content of a CLongArray object to this one.
	\param in_array A constant CLongArray object.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_array::GetCount is 0
	\since Gemini
	*/
	CStatus Append( const CLongArray& in_array );

private:

	void* m_ptr;
};

};

#endif // __XSILONGARRAY_H__
