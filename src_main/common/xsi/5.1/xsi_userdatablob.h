//*****************************************************************************
/*!
   \file xsi_userdatablob.h
   \brief UserDataBlob class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIUSERDATABLOB_H__
#define __XSIUSERDATABLOB_H__

#include <xsi_property.h>
#include <xsi_value.h>
#include <xsi_status.h>

namespace XSI {

//*****************************************************************************
/*! \class UserDataBlob xsi_userdatablob.h
	\brief The %UserDataBlob is a Property that can be attached to many different parts of a scene, for example
	Cameras, Lights, other X3DObjects and Clusters. It allows a plug-in to read, write and persist binary user
	data as part of a scene.

	A %UserDataBlob is created by calling SceneItem::AddProperty with "%UserDataBlob" as the preset name.  Multiple
	UserDataBlobs can be stored on a single object and a %UserDataBlob can be branch applied.

	This object can be thought of as a "low-level" alternative to the CustomProperty object, which does not support
	binary data.  The UserDataMap is similar to the %UserDataBlob, but supports storing per-component binary data
	rather than object level binary data.  For example, to save data that applies to a single vertex, use a %UserDataMap;
	to save data that applies to all vertices in a cluster, use a %UserDataBlob instead.

	The contents of a UserDataBlob can be pushed into Mental Ray.  %Context information needed by the shader can be
	stored in the UserDataBlob and then retrieved through the Mental Ray user data API.  This feature is only available
	when the %UserDataBlob is nested directly under a Camera, Light or X3DObject.  The Parameter named "RenderData"
	should be set to True and the "UserDataID" parameter set to the Mental Ray user data tag value that the shader expects.
	Real-Time shaders do not need this data pushed to them because it is possible for them to read the contents of a
	%UserDataBlob directly through the C++ API.

	\eg
	\code
		using namespace XSI;

		Application app ;
		Model root = app.GetActiveSceneRoot();

		UserDataBlob myBlob ;
		root.AddProperty( L"UserDataBlob", false, L"udb", myBlob ) ;

		// This is the structure that we would like to save inside XSI
		struct ExampleData
		{
			double x ;
			int    y ;
			int    z ;
		} ;

		// Save the structure
		ExampleData data ;
		data.x = 90.1 ;
		data.y = 12 ;
		data.z = 14 ;

		myBlob.PutValue( (unsigned char*)&data, sizeof( ExampleData ) ) ;

		// Retrieve the structure again
		unsigned char * pBuffer = NULL ;
		unsigned int cntBuffer = 0 ;

		myBlob.GetValue( pBuffer, cntBuffer ) ;

		// We can conveniently read the contents of the user data
		// by casting back to the structure
		ExampleData * pData = (ExampleData*)pBuffer ;

		// Update our structure based on what is already on the userdatablob
		data.x = 3.15 + pData->x ;
		data.y = 12 + pData->y ;
		data.z = 0 ;

		// Save our updated values
		myBlob.PutValue( (unsigned char*)&data, sizeof( ExampleData ) ) ;

	\endcode

*/
//*****************************************************************************

class SICPPSDKDECL UserDataBlob : public Property
{
public:
	/*! Default constructor. */
	UserDataBlob();

	/*! Default destructor. */
	~UserDataBlob();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	UserDataBlob(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	UserDataBlob(const UserDataBlob& in_obj);

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
	\return The new UserDataBlob object.
	*/
	UserDataBlob& operator=(const UserDataBlob& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new UserDataBlob object.
	*/
	UserDataBlob& operator=(const CRef& in_ref);

	/*! Provides access to the data inside the UserDataBlob. The out_buffer will be set to point
	to NULL if there is no user data on the object.

	For performance purposes, the returned pointer points directly to internal %XSI data.  The
	caller does not need to free this memory and should treat this data as read-only and potentially
	transitive.  This pointer should not be held by the caller, and, if necessary, the data should
	be copied locally.

    \retval out_buffer On return this will point to the user data, if any.
	\retval out_size   Size, in bytes, of the buffer pointer to by out_buffer
	*/
	CStatus GetValue( const unsigned char *& out_buffer, unsigned int& out_size ) const ;

	/*! Sets binary data on a UserDataBlob.
	\note Binary user data should not contain any memory addresses or other information that is transitive.

    %XSI will copy the provided buffer so that the caller does not need to maintain the provided
	data in memory.

    \tip Passing 0 as the size is equivalent to calling UserDataBlob::Clear.

    \param in_buffer Pointer to a buffer containing the user data
	\param in_size   Size, in bytes, of the buffer
	*/
	CStatus PutValue( const unsigned char * in_buffer, unsigned int in_size ) ;

	/*! Returns true if there is no data on the %UserDataBlob.  This is equivalent to
	testing is UserDataBlob::Size returns 0.
	*/
	bool IsEmpty() const ;

	/*! Removes any used data from the %UserDataBlob.
	*/
	void Clear() ;

	/*! Returns the size, in bytes, of the data inside the %UserDataBlob.
	*/
	unsigned int Size() const ;

	private:
	UserDataBlob * operator&() const;
	UserDataBlob * operator&();
};

};
#endif // __XSIUSERDATABLOB_H__
