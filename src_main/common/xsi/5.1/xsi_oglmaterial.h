//*****************************************************************************
/*!
   \file xsi_oglmaterial.h
   \brief OGLMaterial class declaration.
	object.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

//*****************************************************************************//
//
// (c) Copyright 1996 - 2002 Softimage/Avid Corporation
//
// @doc	  EXPORTED
//
// @module xsi_oglmaterial.h
//
//*****************************************************************************

#ifndef __XSIOGLMATERIAL_H__
#define __XSIOGLMATERIAL_H__

#include <xsi_base.h>
#include <xsi_color.h>

namespace XSI {

//*****************************************************************************
/*! \class OGLMaterial xsi_oglmaterial.h
    \brief This object represents the OpenGL properties of a material
	object.

	OGLMaterial objects are read-only. OGLMaterial can be created with
	Material.OGLMaterial.

	\sa Material

  	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		Material myMaterial(myGrid.GetMaterial());

		OGLMaterial myOGLMaterial(myMaterial.GetOGLMaterial());
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL OGLMaterial : public CBase
{
public:
	/*! Default constructor. */
	OGLMaterial();

	/*! Default destructor. */
	~OGLMaterial();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	OGLMaterial(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	OGLMaterial(const OGLMaterial& in_obj);

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
	\return The new OGLMaterial object.
	*/
	OGLMaterial& operator=(const OGLMaterial& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new OGLMaterial object.
	*/
	OGLMaterial& operator=(const CRef& in_ref);

	/*! Returns the diffuse color of this material.
	\return The diffuse color.
	*/
	CColor          GetDiffuse() const;

	/*! Returns the specular color of this material.
	\return The specular color.
	*/
	CColor          GetSpecular() const;

	/*! Returns the ambient color of this material.
	\return The ambient color.
	*/
	CColor          GetAmbient() const;

	/*! The decay value of this material.
	\return The decay value.
	*/
	double			GetDecay() const;

	/*! Returns the shading model of this material. It is usually not possible to
	exactly represent the mental ray shaders attached to a material in OpenGL,
	however, XSI tries to guess the best OpenGL shading model to approximate these
	shaders.

	XSI chooses a shading model based on specific markings on shader
	parameters provided by the shader developer (using the ui "mapping" = "guid"
	syntax in the SPDL). These markings tell XSI if a parameter can be interpreted
	as a diffuse color, a specular color, etc. and based on the presence or absence of
	these markings, XSI guesses a shading model. For example, if there is a
	diffuse, specular, and ambient-like parameter, XSI chooses the Phong shading
	model (which in OpenGL, is actually Gouraud). If only diffuse and ambient-like
	parameters are present, Lambert would be used instead (since the Lambert
	shading model does not have a specular color). If only ambient is present,
	Constant is used.
	\return The shading model.
	*/
	siShadingModel  GetShadingModel() const;

	private:
	OGLMaterial * operator&() const;
	OGLMaterial * operator&();
};

};

#endif // __XSIOGLMATERIAL_H__
