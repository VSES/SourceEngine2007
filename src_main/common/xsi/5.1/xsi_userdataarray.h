//*****************************************************************************
/*!
   \file xsi_userdataarray.h
   \brief CUserDataArray class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIUSERDATAARRAY_H__
#define __XSIUSERDATAARRAY_H__

#include "sicppsdk.h"

namespace XSI {

class CStatus;

//*************************************************************************
/*! \class CUserDataArray xsi_userdataarray.h
	\brief A CUserDataArray is an uni-dimensional array used to store binary values. CUserDataArray contains items of type
	CUserDataArray::Value which holds the data set on a user data map property.

	The array is-zero based, which means that the first item index is 0 and the larger index is one less that the number of
	elements contained in the array.

	\sa UserDataMap::GetValues, UserDataMap::SetValues, UserDataMap::SetFixedSizeValues
	\since 5.0

	\eg
	\code
		#include "xsi_application.h"
		#include "xsi_userdatamap.h"
		#include "xsi_userdataarray.h"
		#include "xsi_x3dobject.h"
		#include "xsi_model.h"
		#include "xsi_primitive.h"
		#include "xsi_geometry.h"
		#include "xsi_polygonmesh.h"
		#include "xsi_clusterpropertybuilder.h"
		#include "xsi_geometryaccessor.h"

		Application app;

		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", mySphere );

		PolygonMesh mesh = mySphere.GetActivePrimitive().GetGeometry();

		// Adds a user data map on a complete cluster vertices
		CClusterPropertyBuilder cpBuilder = mesh.GetClusterPropertyBuilder();

		UserDataMap udm = cpBuilder.AddUserDataMap(XSI::siClusterVertexType);

		// Set the new property with user data on vertex 10, 20, 30
		struct FooData
		{
			FooData(float a,float b,float c) : x(a), y(b), z(c) {}
			float x,y,z;
		};

		FooData f1(10,10,10);
		FooData f2(20,20,20);
		FooData f3(30,30,30);

		unsigned char* fds[] =
		{
			(unsigned char*)&f1,
			(unsigned char*)&f2,
			(unsigned char*)&f3,
		};

		LONG vtxIndices[] = {10,20,30};
		udm.SetFixedSizeValues(	vtxIndices,
								(const unsigned char**)fds,
								3,
								sizeof(FooData) );

		// Get all user data values from the user data map
		CGeometryAccessor ga = mesh.GetGeometryAccessor();
		CRefArray udmaps = ga.GetUserDataMaps( XSI::siClusterVertexType );

		for (LONG i=0; i<udmaps.GetCount(); i++)
		{
			udm = udmaps[i];

			CUserDataArray data;
			udm.GetValues( data );

			// Log all vertex user data
			for (LONG j=0; j<data.GetCount(); j++)
			{
				CUserDataArray::Value val = data[j];
				FooData* pfd = (FooData*)val.m_pData;
				if (pfd)
				{
					app.LogMessage(	CString(L"FooData[") + CString(j) + CString(L"]=")
									+ CString(pfd->x) + L","
									+ CString(pfd->y) + L","
									+ CString(pfd->z) );
				}
				else
				{
					app.LogMessage(	CString(L"FooData[") + CString(j) + CString(L"]=empty") );
				}
			}
		}

		// Get only the user data values set on the user data map
		for (LONG i=0; i<udmaps.GetCount(); i++)
		{
			udm = udmaps[i];

			CUserDataArray data;
			CBitArray flags;
			udm.GetValues( data, flags );

			// Log only the vertex with user data
			LONG j = flags.GetIterator();
			while (flags.GetNextTrueBit(j))
			{
				CUserDataArray::Value val = data[j];
				FooData* pfd = (FooData*)val.m_pData;
				app.LogMessage(	CString(L"FooData[") + CString(j) + CString(L"]=")
								+ CString(pfd->x) + L","
								+ CString(pfd->y) + L","
								+ CString(pfd->z) );
			}
		}
	\endcode
*/
//*************************************************************************
class SICPPSDKDECL CUserDataArray
{
public:
	/* Defines the user data structure
	*/
	struct Value
	{
		/*! Constructs a CUserDataArray::Value. This class encapsulates
		the user data value contained by CUserDataArray.
		*/
		Value() : m_pData(0), m_nSize(0) {}

		/*! Equality operator.
		\param in_value CUserDataArray::Value to compare with.
		\return true if value size is greater than the value argument.
		*/
		bool operator==(const Value& in_value) const
		{
			return m_nSize > in_value.m_nSize;
		}

		const unsigned char* m_pData;	/*!< pointer to user data value */
		unsigned int m_nSize;			/*!< size of the user data value */
	};

	/*! Constructs a CUserDataArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CUserDataArray( LONG in_size=0);

	/*! Constructs a CUserDataArray object from another CUserDataArray object.
	\param in_array constant CUserDataArray object.
	*/
	CUserDataArray(const CUserDataArray& in_array);

	/*! Default destructor. */
	~CUserDataArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CUserDataArray& operator=( const CUserDataArray& in_array );

	/*! Returns the number of items in this CUserDataArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a value to the end of this array.
	\param in_val The new value.
	\return CStatus::OK success
	*/
	CStatus Add( const CUserDataArray::Value& in_val );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if new
	new size is larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be
	called by constant objects, the returned value is read-only.
	\param in_index index in this zero based array.The index must be smaller than
		the number of element in the array, otherwise the results are unpredicted.
	\return A read only reference to the indexed item.
	*/
	const CUserDataArray::Value& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero based array.The index must be smaller than
		the number of element in the array, otherwise the results are unpredicted.
	\return A reference to the indexed item.
	*/
	CUserDataArray::Value& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CUserDataArray to compare with.
	\return true if in_array contains the same number of elements as this array and
		that all members of in_array are equal to the corresponding one contained in
		this array.
	*/
	bool operator==(const CUserDataArray& in_array) const;

	/*! Inequality operator.
	\param in_array CUserDataArray to compare with.
	\return true if one member of in_array is different of the corresponding members
		in this	array or if the arrays are not of the same size.
	*/
	bool operator!=(const CUserDataArray& in_array) const;

private:

	void* m_ptr;
};

};

#endif // __XSIUSERDATAARRAY_H__
