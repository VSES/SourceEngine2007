//*****************************************************************************
/*!
   \file xsi_texture.h
   \brief Texture Shaders class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITEXTURE_H__
#define __XSITEXTURE_H__

#include <xsi_shader.h>


namespace XSI {

class CDoubleArray;
class CStatus;
class ImageClip2;

//*****************************************************************************
/*! \class Texture xsi_texture.h
	\brief Specific Shader object which is a member of the Texture Shaders family (siTextureShaderFamily).

	\sa OGLMaterial

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube",L"MeshSurface",L"" ,myCube );

		Material myMaterial;
		myCube.AddMaterial(L"Phong",false,L"",myMaterial);

		// Adding an image
		CValueArray args(4L);
		CValue outArg;
		args[0] = CValue( CString(L"Image") );
		args[1] = CValue(myCube.GetRef());
		args[2] = CValue((short)1);
		args[3] = CValue(false);

		app.ExecuteCommand( L"BlendInPresets", args, outArg );

		// Adding a texture support
		args[0] = myCube.GetRef();
		args[1] = (LONG)siTxtPlanarXY;
		args[2] = (LONG)siTxtDefaultSpherical;
		args[3] = CString(L"Texture_Support");
		app.ExecuteCommand( L"CreateTextureSupport", args, outArg );

		Texture myTexture= myMaterial.GetCurrentTexture();
		app.LogMessage( L"The texture: " +  myTexture.GetFullName() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Texture : public Shader
{
public:
	/*! Default constructor. */
	Texture();

	/*! Default destructor. */
	~Texture();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Texture(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Texture(const Texture& in_obj);

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
	\return The new Texture object.
	*/
	Texture& operator=(const Texture& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Texture object.
	*/
	Texture& operator=(const CRef& in_ref);

	/*! Calculates the UVW values. The values are read from cluster properties, and the following texture
	objects are used in the calculation:

		\li Texture projection definition
		\li Texture shader
		\li Image clip

	You can specify the texture transformation effects you want to apply using bit	flags. Note that some
	texture transformation effects will only be approximate, such as in the case of creating texture repeats
	where this method cannot resample the surface to create the additional sample points required.

	\param in_flags bit flags to specify which transformations to apply to the UVW cluster property content.
		The bit flags values are the ones  located in the ::siTextureEffectsComputation enum.
	\retval out_aUVWValues array containing the UVW. This array is in the same order as the cluster property.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus GetTransformValues(LONG in_flags,CDoubleArray& out_aUVWValues);

	/*! Returns the image clip bound to this texture.
	\note Textures of type color map lookup have no image nodes, so in those cases no ImageClip2 object is returned.
	\since 5.0
	*/
	ImageClip2 GetImageClip() const;

	private:
	Texture * operator&() const;
	Texture * operator&();
};

};

#endif // __XSITEXTURE_H__
