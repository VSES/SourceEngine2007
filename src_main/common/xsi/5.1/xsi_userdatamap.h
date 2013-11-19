//*****************************************************************************
/*!
   \file xsi_userdatamap.h
   \brief UserDataMap class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIUSERDATAMAP_H__
#define __XSIUSERDATAMAP_H__

#include <xsi_property.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_customproperty.h>
#include <xsi_userdataarray.h>
#include <xsi_bitarray.h>

namespace XSI {

//*****************************************************************************
/*! \class UserDataMap xsi_userdatamap.h
	\brief The %UserDataMap is a Property that can be attached to any cluster.  It
	permits the storage of variable length binary user data on the individual components
	of a geometry.  The data is stored as part of the scene file and can be accessed via
	plug-ins using the C++ API.

	%XSI Plug-ins (both persistent and immediate effects) can use this property to save and
	retrieve data that is associated with individual vertices, polygons, edges, or other
	components of an object.  Example data possibilities include custom game structures,
	raw simulation data, matrices, comments and images.

	There are two ways that a %UserDataMap can be used - for binary data or for templated data.
	With a Binary %UserDataMap, the structure of the user data is left completely up to the
	plug-in.  This is well suited to C++ plug-ins.  The second type of %UserDataMap
	uses a CustomProperty to strictly define the structure of the user data
	(see UserDataMap::GetTemplate).  Templated user data can be inspected in the UI
	(see the \xl InspectUserData command) in addition to scripting and C++ access.

	UserDataMaps are created with the \xl CreateUserDataMap command or
	SceneItem::AddProperty.

	%UserDataMap differs from the CustomProperty and UserDataBlob objects in that it
	stores separate data for each individual component of a cluster.

	The %UserDataMap is similar to a ClusterProperty, which also stores
	per-component data, but the %UserDataMap allow variable length data, whereas %ClusterProperty
	objects always store a fixed number of Doubles at each component.

	The User Data Map provides similar functionality to that available
	from the SAAPHIRE SAA_subelement* UserData functions.  For example,
	the Name of the property corresponds to the Tag argument
	to functions like SAA_subelementSetUserData.

	It is possible to save a preset of a %UserDataMap and to reapply the user data on a different
	object.  However, for the best results, the number of components in both clusters should be
	the same.

	\sa Cluster, ClusterProperty, UserDataBlob

	\eg This example stores and retrieves binary user data on a user data map
	\code
		using namespace XSI;
		Application app ;
		Model root = app.GetActiveSceneRoot();

		// Create a cone mesh surface
		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"ConeWithUserDataMap", myCone );

		// Create a cluster
		Cluster myCls ;
		CLongArray indices(3);
		indices[0] = 8;
		indices[1] = 16;
		indices[2] = 24;

		myCone.GetActivePrimitive().GetGeometry().AddCluster(siVertexCluster,
			L"VertexCluster", indices, myCls ) ;

		SceneItem myClsSceneItem( myCls ) ;

		// Add the user data map
		UserDataMap myMap ;
		myClsSceneItem.AddProperty( L"UserDataMap", false, L"udm", myMap ) ;

		// This is the structure that we would like to save inside XSI
		struct ExampleData
		{
			double x ;
			INT    y ;
			INT    z ;
		} ;

		ExampleData defaultValues ;
		defaultValues.x = 1.1 ;
		defaultValues.y = 10 ;

		// Initialize the user data map to default values
		for ( LONG i = 0 ; i < myMap.GetCount() ; i++ )
		{
			defaultValues.z = i ;
			myMap.PutItemValue( i, (unsigned char*)&defaultValues, sizeof( ExampleData )) ;
		}

		// Read the values from index 1
		unsigned char* pInternalData = NULL ;
		UINT cntData = 0 ;
		myMap.GetItemValue( 1, pInternalData, cntData ) ;

		// pData will point to x=1.1, y=10, z=1
		ExampleData *pData = (ExampleData*) pInternalData ;

		// Copy the structure locally before erasing the data inside XSI
		ExampleData myCopy = *pData ;
		pInternalData = NULL ;
		pData = NULL ;
		myMap.ClearItem( 1 ) ;

		// Save the values that used to be at index 1 onto index 0
		myMap.PutItemValue( 0, (unsigned char*)&myCopy, sizeof( ExampleData ) ) ;

		// Now print out the contents
		for ( i = 0 ; i < myMap.GetCount() ; i++ )
		{
			if ( myMap.IsItemEmpty( i ) )
			{
				app.LogMessage( L"Map item " + CValue(i).GetAsText() + L" is empty" ) ;
			}
			else
			{
				unsigned char* pInternalData = NULL ;
				UINT cntData = 0 ;
				myMap.GetItemValue( i, pInternalData, cntData ) ;
				pData = (ExampleData*) pInternalData ;

				app.LogMessage( L"Map item " + CValue(i).GetAsText() +
								L" has values: x = " + CValue(pData->x).GetAsText() +
											L" y = " + CValue((LONG)pData->y).GetAsText() +
											L" z = " + CValue((LONG)pData->z).GetAsText() ) ;
			}

		}

		//Output of running this script is the following:
		//'INFO : "Map item 0 has values: x = 1.1 y = 10 z = 1"
		//'INFO : "Map item 1 is empty"
		//'INFO : "Map item 2 has values: x = 1.1 y = 10 z = 2"
	\endcode

	\eg This example also stores and retrieves templated user data on a user data map
	It is practically the same as the previous example,
	except that it uses the template support to use a CustomProperty object
	as the way of defining and accessing the format of the data.
	This approaches is moderately slower, because it involves transferring data
	back and forth between the CustomProperty and the UserDataMap.  However its
	memory cost is practically the same as the other example and it has the advantage
	that users can edit the values from the user interface using the \xl InspectUserData
	command.
	\code
		using namespace XSI;
		Application app ;
		Model root = app.GetActiveSceneRoot();

		// Create a cone mesh surface
		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"GridWithUserDataMap", myGrid );

		CLongArray indices(3);
		indices[0] = 1;
		indices[1] = 3;
		indices[2] = 4;

		Cluster myCls ;
		myGrid.GetActivePrimitive().GetGeometry().AddCluster(siEdgeCluster,
			L"AllEdges", indices, myCls ) ;

		SceneItem myClsSceneItem( myCls ) ;

		// Add the user data map
		UserDataMap myMap ;
		myClsSceneItem.AddProperty( L"UserDataMap", false, L"udm", myMap ) ;

		// Create the custom property that acts as our template
		CustomProperty myTemplate ;
		myClsSceneItem.AddCustomProperty( L"MyTemplate", false, myTemplate ) ;

		Parameter param ;
		myTemplate.AddParameter(
			L"x",
			CValue::siDouble,
			siAnimatable,
			L"", // Name
			L"", // Description
			1.1,  // Default
			param ) ;

		myTemplate.AddParameter(
			L"y",
			CValue::siInt4,
			siAnimatable,
			L"",
			L"",
			10l,
			param ) ;

		myTemplate.AddParameter(
			L"z",
			CValue::siInt4,
			siAnimatable,
			L"",
			L"",
			0l,
			param ) ;

		// Sets the Custom Property as the template for the data in the UserDataMap
		myMap.PutTemplate( myTemplate ) ;

		ProjectItem templateProjItm(myTemplate)  ;

		unsigned char * pData = NULL ;
		UINT cntData ;

		// Initialize the user data map with default values
		for ( LONG i = 0 ; i < myMap.GetCount() ; i++ )
		{
			templateProjItm.PutParameterValue( L"z", i ) ;
			myTemplate.GetBinaryData( pData, cntData ) ;

			myMap.PutItemValue( i, pData, cntData ) ;

			// We need to release the memory returned by GetBinaryData
			myTemplate.FreeBinaryData( pData ) ;
		}

		// Read the values from Index 1
		unsigned char* pInternalData = NULL ;

		myMap.GetItemValue( 1, pInternalData, cntData ) ;

		// Custom Property contents will now be x=1.1, y=10, z=1
		myTemplate.PutBinaryData( pInternalData, cntData ) ;

		// Remove the data from index 1 now that we have read it
		pInternalData = NULL ;
		pData = NULL ;
		myMap.ClearItem( 1 ) ;

		// Save the values that used to be at index 1 onto index 0
		myTemplate.GetBinaryData( pData, cntData ) ;
		myMap.PutItemValue( 0, pData, cntData ) ;
		myTemplate.FreeBinaryData( pData ) ;

		// Now print out the contents
		for ( i = 0 ; i < myMap.GetCount() ; i++ )
		{
			if ( myMap.IsItemEmpty( i ) )
			{
				app.LogMessage( L"Map item " + CValue(i).GetAsText() + L" is empty" ) ;
			}
			else
			{
				unsigned char* pInternalData = NULL ;
				UINT cntData = 0 ;
				myMap.GetItemValue( i, pInternalData, cntData ) ;

				myTemplate.PutBinaryData( pInternalData, cntData ) ;

				// Now we can just read the values off the custom property set
				app.LogMessage( L"Map item " + CValue(i).GetAsText() +
								L" has values: x = " +
								CValue(templateProjItm.GetParameterValue( L"x" )).GetAsText() +
								L" y = " +
								CValue(templateProjItm.GetParameterValue( L"y" )).GetAsText() +
								L" z = " +
								CValue(templateProjItm.GetParameterValue( L"z" )).GetAsText() ) ;
			}
		}

	\endcode

*/
//*****************************************************************************

class SICPPSDKDECL UserDataMap : public Property
{
public:
	/*! Default constructor. */
	UserDataMap();

	/*! Default destructor. */
	~UserDataMap();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	UserDataMap(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	UserDataMap(const UserDataMap& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new %UserDataMap object.
	*/
	UserDataMap& operator=(const UserDataMap& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new %UserDataMap object.
	*/
	UserDataMap& operator=(const CRef& in_ref);

	/*! Returns the number of elements in the map.  This is equal to the size of the
	cluster that the %UserDataMap is attached to.
	\return Size of map.
	*/
	LONG GetCount() const ;

	/*! Flushes all data from the %UserDataMap.  This does not change the size
	of the %UserDataMap, but all items on the map will be empty.
	*/
	void Clear() ;

	/*! When the %UserDataMap is created this flag is initialized to the correct value
	according to the Endian byte order of the computer that %XSI is running on.  For example,
	LittleEndian is set to true on x86 processors and false on MIPS processors.  This flag is
	useful when transferring %XSI scenes with binary data between platforms with different byte
	order - a plug-in can use this flag to determine whether to byte-swap the data retrieved from
	UserDataMap::GetItemValue.  Plug-ins which pack their data in a consistent order, no matter
	what platform they are on, can ignore this property.

	\note This can be ignored for templated UserDataMaps, since they always use a little endian
		binary representation (see CustomProperty::GetBinaryData).

	\return true for x86 processors and false for MIPS processors
	*/
	bool IsLittleEndian() const ;

	/*! Sets the Little Endian flag.
 	\param 	in_bLittleEndian true for x86 processors and false for MIPS processors
	*/
	void PutLittleEndian( bool in_bLittleEndian ) ;

	/*! A %UserDataMap can be associated with a CustomProperty that serves as a
	template for the data inside the %UserDataMap.  This allows the storage of
	strongly typed data on each component.  The template defines the format
	of the data, but the values of the data are stored separately for each component.

	For example, a User Data Map on a polygon cluster may have a Template with a
	integer parameter X and a floating point parameter Y.  This means that each polygon can
	store its own value of X and Y.

	The transfer of data between the associated custom property and individual components can be performed
	using UserDataMap::GetItemValue and CustomProperty::GetBinaryData.

	\return Returns the template as a CustomProperty object.  If no template then an invalid
			CustomProperty object is returned.
	\sa CustomProperty
	*/
	CustomProperty	GetTemplate() const ;

	/*! Sets the template for the %UserDataMap.  To remove an existing template this can
	be called with an invalid CustomProperty object.
 	\param 	in_Template template to set
	\return CStatus
	*/
	CStatus PutTemplate( const CustomProperty& in_Template ) ;

	/*! Gets access to the data on a particular component inside the UserDataMap. The
	out_buffer will point to NULL if there is no user data on the specified component.

	The way this property is used depends on whether the %UserDataMap has an associated
	template or not:

	\par In the context of templated %UserDataMap...
		This property is used to transfer user data to and from the
		CustomProperty::GetBinaryData property referenced by UserDataMap::Template.
		In this case you do not need to understand the format of the data, because
		you read and write the values via the ProjectItem functions, such as
		ProjectItem::GetParameters.

	\par In the context of a binary %UserDataMap...
	    This property is used to read binary data that was previously saved on a geometry component.

	For performance purposes, the returned pointer points directly to internal XSI data.  The
	caller does not need to free this memory and should treat this data as read-only and
	potentially transitive.  To hold the data, it should be copied by the caller rather than
	holding the pointer.

	When transferring binary user data between platforms it may be necessary to perform byte-swapping.
	See UserDataMap::LittleEndian.

 	\param 	in_Offset 	index of the item
 	\retval	out_buffer 	buffer
 	\retval	out_size 	data size
	\return CStatus
	*/
	CStatus GetItemValue( LONG in_Offset, const unsigned char *& out_buffer, UINT& out_size ) const ;

	/*! Sets binary data into a user data map.

    On a templated %UserDataMap, you can remember the current state of the associated
	CustomProperty by calling CustomProperty::GetBinaryData and setting the returned buffer
	via this method.

	On binary %UserDataMaps, the caller can set data of any size or structure.

	\note Binary user data should not contain any memory addresses or other information that is transitive.

    %XSI will copy the provided buffer so the caller does not need to maintain the provided
	data in memory.

 	\param in_Offset 	index of the item to set
 	\param in_buffer 	buffer to set
 	\param in_size 		new size
	\return CStatus
	*/
	CStatus PutItemValue( LONG in_Offset, const unsigned char * in_buffer, UINT in_size ) ;

	/*! Returns true if there is no user data on the specified component.
	The index must be in the range 0 to UserDataMap.Count - 1.
	A common scenario for using this property is inside a loop which
	visits each component in the user data map to see if any user data has been set.

 	\param in_lOffset index of the item with the data to check
	\return true if no user data
	\sa Cluster.GetElements, ClusterProperty
	*/
	bool IsItemEmpty( LONG in_lOffset ) const ;

	/*! Returns the size (in bytes) of the data attached to a
	particular component of the %UserDataMap.

	\param in_lOffset index of the item with the data to check
	\return The size (in bytes) of the user data.
	*/
	UINT GetItemSize( LONG in_lOffset ) const ;

	/*! Removes any user data from the specified index.  To clear the entire property
	at once use UserDataMap::Clear.

	\param in_Offset index of the item to clear
	*/
	void ClearItem( LONG in_Offset ) ;

	/*! Returns an array containing the user data values stored in this map.
	The number of elements in the array matches the number of geometry
	elements (N).

	\retval out_data Array of user data values.
	\return CStatus
	\sa CGeometryAccessor::GetClusterProperties
	\since 5.0
	*/
	CStatus GetValues( CUserDataArray& out_data ) const;

	/*! Returns an array containing the user data values stored in this map.
	The number of elements in the array matches the number of geometry
	elements (N). The function also returns a bit array of size N which
	identifies the indices of the geometry elements set in the user data
	map. You can use the bit array items to quickly identify what are the
	elements affected by the map.

	\retval out_data Array of user data values.
	\retval out_elemIdx Array of element index flags.
	\return CStatus
	\sa CGeometryAccessor::GetClusterProperties
	\since 5.0
	*/
	CStatus GetValues( CUserDataArray& out_data, CBitArray& out_elemIdx) const;

	/*! Sets the user data values stored in this map. The function sets the
	geometry elements in the order specified by in_pValues starting from
	index 0. Values are set directly on the property and the operation is
	not undoable.

	For large data sets, you can use an offset index to set the values in
	chunks. The offset is used for indexing into in_pElements which allows
	you	to optimize the memory allocation required for setting the element
	values.

	\param in_pValues Array of size in_nElements containing the user data values.
	\param in_pSizeArray Array of size in_nValues containing the size of each user data specified in n_pValues.
	\param in_nValues Number of values to set.
	\param in_nOffset Specifies the starting index in the cluster elements.

	\return CStatus::OK Success.
	\return CStatus::InvalidArgument Returns an error if in_nElements is greater than the
		number of cluster elements or smaller than zero. Also returns an error if in_pElements or in_pValues are invalid.

	\sa CClusterPropertyBuilder, CGeometryAccessor::GetClusterProperties
	\since 5.0
	*/
	CStatus SetValues(
		const unsigned char**	in_pValues,
		const LONG*				in_pSizeArray,
		LONG					in_nValues,
		LONG					in_nOffset=0);

	/*! Sets the user data values stored in this map. Values are set directly on
	the property and the operation is not undoable.

	\param in_pElements Array of size in_nElements containing the geometry elements to set.
	\param in_pValues Array of size in_nElements containing the user data values.
	\param in_pSizeArray Array of size in_nElements containing the size of each user data specified in in_pValues.
	\param in_nElements Number of elements in in_pElements.

	\return CStatus::OK Success.
	\return CStatus::InvalidArgument Returns an error if in_nElements is greater than the
		number of cluster elements or smaller than zero. Also returns an error if in_pElements or in_pValues are invalid.

	\sa CClusterPropertyBuilder, CGeometryAccessor::GetClusterProperties
	\since 5.0
	*/
	CStatus SetValues(
		const LONG*				in_pElements,
		const unsigned char**	in_pValues,
		const LONG*				in_pSizeArray,
		LONG					in_nElements );

	/*! Sets the user data stored in this map with an array of values of a fixed
	size. The function sets the geometry elements in the order specified by
	in_pValues starting from index 0. This version allows you to set the user data
	values without the need of allocating an array to hold the data size. The
	function assumes that each item has the same data size. Values are set
	directly on the property and the operation is not undoable.

	For large data sets, you can use an offset index to set the values in
	chunks. The offset is used for indexing into the cluster elements which
	allows you to optimize the memory allocation required for setting the element
	values.

	\param in_pValues Array of size in_nValues containing the user data values.
	\param in_nValues Number of values in in_pValues.
	\param in_nSize The size of each values contained in in_pValues.
	\param in_nOffset Specifies the starting index in the cluster elements.

	\return CStatus::OK Success.
	\return CStatus::InvalidArgument Returns an error if in_nElements is greater than the
		number of cluster elements or smaller than zero. Also returns an error if in_pElements or in_pValues are invalid.

	\sa CClusterPropertyBuilder, CGeometryAccessor::GetClusterProperties
	\since 5.0
	*/
	CStatus SetFixedSizeValues(
		const unsigned char**	in_pValues,
		LONG					in_nValues,
		LONG					in_nSize,
		LONG					in_nOffset=0);

	/*! Sets the user data stored in this map with an array of values of a fixed
	size. The function sets the geometry elements in the order specified by
	in_ppValues starting from index 0. This version allows you to set the
	user data values without the need of allocating an array to hold the
	data size. Values are set directly on the property and the operation is not undoable.

	\param in_pElements Array of size in_nElements containing the geometry elements to set.
	\param in_pValues Array of size in_nElements containing the user data values.
	\param in_nElements Number of elements in in_pElements.
	\param in_nSize The size of each values contained in in_pValues.

	\return CStatus::OK Success.
	\return CStatus::InvalidArgument Returns an error if in_nElements is greater than the
		number of cluster elements or smaller than zero. Also returns an error if in_pElements or in_pValues are invalid.

	\sa CClusterPropertyBuilder, CGeometryAccessor::GetClusterProperties
	\since 5.0
	*/
	CStatus SetFixedSizeValues(
		const LONG*				in_pElements,
		const unsigned char**	in_pValues,
		LONG					in_nElements,
		LONG					in_nSize );

	private:
	UserDataMap * operator&() const;
	UserDataMap * operator&();
};

};
#endif // __XSIUSERDATAMAP_H__
