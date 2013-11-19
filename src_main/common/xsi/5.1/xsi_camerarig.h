//*****************************************************************************
/*!
   \file xsi_camerarig.h
   \brief CameraRig class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICAMERARIG_H__
#define __XSICAMERARIG_H__

#include <xsi_rig.h>

namespace XSI {

//*****************************************************************************
/*! \class CameraRig xsi_camerarig.h
	\brief The CameraRig object is a specialized Null object that represents a hierarchy composed of a
	Camera and its X3DObject interest object.

	Each is parented under the same root object i.e. the CameraRig.

	\sa Camera

  	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		CameraRig myCameraRig;
		root.AddCameraRig( L"Camera", L"", myCameraRig );

		X3DObject interest(myCameraRig.GetCamera().GetInterest());
	\endcode

 */
//*****************************************************************************

class SICPPSDKDECL CameraRig : public Rig
{
public:
	/*! Default constructor. */
	CameraRig();

	/*! Default destructor. */
	~CameraRig();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	CameraRig(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	CameraRig(const CameraRig& in_obj);

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
	\return The new CameraRig object.
	*/
	CameraRig& operator=(const CameraRig& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new CameraRig object.
	*/
	CameraRig& operator=(const CRef& in_ref);

	/*! Returns the camera object of this rig.
	\return The Camera object for this rig.
	*/
	Camera GetCamera() const;

	private:
	CameraRig * operator&() const;
	CameraRig * operator&();

};

};

#endif // __XSICAMERARIG_H__
