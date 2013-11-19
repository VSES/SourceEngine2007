//*****************************************************************************
/*!
   \file xsi_camera.h
   \brief Camera class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICAMERA_H__
#define __XSICAMERA_H__

#include <xsi_directed.h>

namespace XSI {

class CStatus;

//*****************************************************************************
/*! \class Camera xsi_camera.h
	\brief The Camera object represents an XSI camera in a scene.

	You can create a Camera with X3DObject::AddCamera. A new camera always has its interest located at the
	origin of the scene's 3D world.

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		CameraRig myCamRig;
		root.AddCameraRig( L"Camera", L"", myCamRig );

		Camera myCam(myCamRig.GetCamera());
		X3DObject myCamInterest(myCam.GetInterest());

		CString strCamName( myCam.GetName() );
		app.LogMessage( L"Camera interest" );
		app.LogMessage( strCamName + L".interest.name: " + myCam.GetInterest().GetName() );

		app.LogMessage( strCamName + L"'s near parameter: " + myCam.GetParameterValue(CString(L"near")).GetAsText() );
		app.LogMessage( strCamName + L"'s far parameter: " +  myCam.GetParameterValue(CString(L"far")).GetAsText() );
		app.LogMessage( strCamName + L"'s orthoheight parameter: " + myCam.GetParameterValue(CString(L"orthoheight")).GetAsText() );
		app.LogMessage( strCamName + L"'s fov parameter: " + myCam.GetParameterValue(CString(L"fov")).GetAsText() );
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL Camera : public Directed
{
public:
	/*! Constructs a Camera object. */
	Camera();

	/*! Destroys a Camera object. */
	~Camera();

	/*! Constructs a Camera object from a CRef object.
	\param in_ref A reference to a camera.
	*/
	Camera(const CRef& in_ref);

	/*! Constructs a new Camera object from an existing Camera object.
	\param in_obj An existing Camera object to copy into this Camera object.
	*/
	Camera(const Camera& in_obj);

	/*! Returns true if this object supports the functionality of a specified class. For example,
	a Camera is a type of SceneItem, so a Group object supports SceneItem functionality.
	\param in_ClassID Test if this object supports this class.
	\return True if this object supports the specified class, and false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the class ID for this object.
	\return The class ID.
	*/
	siClassID GetClassID() const;

	/*! Assigns a Camera object to an existing Camera object.
	\param in_obj A Camera object to be copied into this object.
	\return The reinitialized Camera object.
	*/
	Camera& operator=(const Camera& in_obj);

	/*! Assigns a CRef to this Camera object. The Camera object is cleared if the CRef is not a
	reference to an object that supports the Camera class.
	\param in_ref A reference to an object that supports the Camera class.
	\return The reinitialized Camera object.
	*/
	Camera& operator=(const CRef& in_ref);

	/*! Returns an array of references to the Shader objects connected to this camera.
	\return An array of shader references.
	*/
	CRefArray GetShaders() const;


	/*! Returns the input type of a parameter. Before you connect a Shader to the parameter, use
	Parameter::GetCapabilities to make sure that the parameter supports the ::siTexturable capability.
	\param in_paramscriptname The script name of a parameter.
	\return The shader input type (one of the ::siShaderParameterType values).  If the parameter cannot
		be connected to a shader, ::siUnknownParameterType is returned.
	\since 5.0
	*/
	siShaderParameterType GetShaderInputType(const CString& in_paramscriptname) const;

	/*! Returns all shaders that match a filter. For example, you can write a custom filter that matches
	shaders with texturespace and colormap parameters, and then use this custom filter with this function.
	\param in_filtername The name of a shader Filter (for example, ::siShaderFilter).
	\return An array of references to shaders that match the filter.
	\since 5.0
	*/
	CRefArray FindShaders(const CString& in_filtername) const;

	private:
	Camera * operator&() const;
	Camera * operator&();

};

};

#endif // __XSICAMERA_H__
