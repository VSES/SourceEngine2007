//*****************************************************************************
/*!
   \file xsi_library.h
   \brief Library class declaration.

   © Copyright 1998-2004 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSI_LIBRARY_H__
#define __XSI_LIBRARY_H__

#include <xsi_source.h>

namespace XSI {

//*****************************************************************************
/*! \class Library xsi_library.h
	\brief A library is a container of objects of the same type. For example, a material library contains a
	list of materials.

	Library storage may be internal or external. If internal, the library is saved in the scene file. If external,
	the library is saved in a separate file. The library can be exported to and imported from dotXSI or a native
	binary format.

	\note The parent of a library is the Scene object.

	\sa Scene::GetActiveMaterialLibrary
 	\since 4.0

	\eg Shows how to get the parameter values from a shared material. Parameters on a shared material have
		instance-specific values for each object sharing the material. This applies to the ImageShader.tspace_id,
		Material.ImageClipName, Material.UV, and Material.CAV parameters.

	\note You need to define the \xl CreateProjection and \xl CreateImageClip commands as functions using the
		source code from the cmdstub.cpp example.

	\code
		CStatus st;
		Application app;

		Scene scene = app.GetActiveProject().GetActiveScene();

		Model root = scene.GetRoot();

		// Create a cube with a texture projection (not connected)
		X3DObject cube; st = root.AddGeometry( L"Cube", L"MeshSurface", L"MyCubeWithTexture", cube );

		// Create a projection for the cube
		CreateProjection(
			cube.GetRef(),
			siTxtCubic,
			siTxtDefaultCubic ,
			L"MyCubicTextureSupport",
			L"MyCubicTextureProjection" );

		// Create a sphere with a texture projection (not connected)
		X3DObject sphere; st = root.AddGeometry( L"Sphere", L"MeshSurface", L"MySphereWithTexture", sphere );

		// Create a projection for the sphere
		CreateProjection(
			sphere.GetRef(),
			siTxtSpherical,
			siTxtDefaultSpherical,
			L"MySphericalTextureSupport",
			L"MySphericalTextureProjection" );

		// Create a group with a material hooked up to a texture image
		CRefArray objs;

		objs.Add( cube );
		objs.Add( sphere );

		Group group; st = root.AddGroup( objs, L"MyGroup", false, group );

		Material mat; st = group.AddMaterial(L"Phong", false, L"MyGroupMaterialWithTexture", mat );

		CRefArray shaders = mat.GetShaders();

		Shader phong = shaders[0];

		Parameter ambient = phong.GetParameter(L"ambient");
		Parameter diffuse = phong.GetParameter(L"diffuse");

		// Create an image clip
		CString strFileName = app.GetInstallationPath( siFactoryPath ) + L"\\Data\\XSI_SAMPLES\\Pictures\\xsilogo.jpg";

		CRef imageclip = CreateImageClip( strFileName, L"MyImageClip" );

		// Connect the imageclip to phong.ambient
		CRef prevsrc, src;

		st = ambient.ConnectFromPreset( L"Image", siTextureShaderFamily, prevsrc, src );

		Shader imageshader = src;

		// Connect the imageclip to phong.diffuse
		st = diffuse.Connect( imageshader, prevsrc ) ;

		// Connect image.tex to imageclip
		Parameter tex = imageshader.GetParameter(L"tex");
		st = tex.Connect( imageclip, prevsrc );

		// Get tspace_id
		Parameter tspace_id = imageshader.GetParameter( L"tspace_id" );

		// Set cube.material.image.tspace_id = MyCubicTextureProjection
		st = tspace_id.PutInstanceValue(cube, L"MyCubicTextureProjection");

		// Set sphere.material.image.tspace_id = MySphericalTextureProjection
		st = tspace_id.PutInstanceValue(sphere, L"MySphericalTextureProjection");

		// Dump the tspace ids for objects found using materials
		// from material library
		Library lib = scene.GetActiveMaterialLibrary();

		CRefArray mat_refs = lib.GetItems();

		for ( LONG i=0; i< mat_refs.GetCount(); i++ )
		{
			Material mat( mat_refs[i] );

			CRefArray ref_illum_shaders = mat.GetShaders();

			if ( ref_illum_shaders.GetCount() == 0  ) continue;

			Shader illum_shader = 	ref_illum_shaders[0];
			CRefArray ref_image_shaders = illum_shader.GetShaders();

			Shader image = ref_image_shaders[0];

			CRefArray param_refs = image.GetParameters();

			for ( LONG j=0; i < param_refs.GetCount(); i++ )
			{
				 Parameter image_param( param_refs[i] );

				 // If the parameter is an instance value look up
				 // its object value
				 if ( image_param.HasInstanceValue() )
				 {
						CRefArray owner_refs = mat.GetOwners();

						for ( LONG k=0; k < owner_refs.GetCount(); k++ )
						{
							CRef owner_ref = owner_refs[k];

							if ( owner_ref == lib ) continue;

							if ( owner_ref.IsA( siGroupID ) )
							{
								Group group = owner_ref;

								// Log the complete path to the parameter under the object that uses the material
								// with an instance value.
								app.LogMessage( image_param.GetFullName() + L" = " + image_param.GetValue().GetAsText()  );

								CRefArray members = group.GetMembers();

								for ( LONG l=0; l < members.GetCount(); l++ )
								{
									SceneItem sceneitem = members[l];

									CValue val = image_param.GetInstanceValue( members[l] );

									// Log complete path to parameter under object that uses material
									// with instance value.
									app.LogMessage( sceneitem.GetFullName() + L" " + image_param.GetName() + L" = " + val.GetAsText()  );
								}
							}
							else
							{
								CValue val = image_param.GetInstanceValue( owner_ref );

								// Log complete path to parameter under object that uses material
								// with instance value.
								app.LogMessage( image_param.GetName() + L" = " + val.GetAsText()  );
							}
						}
				 }
			}
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Library : public Source
{
public:
	/*! Constructs a Library object. */
			Library();

	/*! Destroys a Library object. */
	~Library();

	/*! Constructs a Library object from a CRef object.
	\param in_ref A reference to a library.
	*/
	Library(const CRef& in_ref);

	/*! Constructs a new Library object from an existing Library object.
	\param in_obj An existing Library object to copy into this Library object.
	*/
	Library(const Library& in_obj);

	/*! Returns True if this object supports the functionality of a specified class. For example,
	a Library is a type of Source, so a Library object supports Source functionality.
	\param in_ClassID Test if this object supports this class.
	\return True if this object supports the specified class, and false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the class ID for this object.
	\return The class ID.
	*/
	siClassID GetClassID() const;

	/*! Assigns a Library object to an existing Library object.
	\param in_obj A Library object to be copied into this object.
	\return The reinitialized Library object.
	*/
	Library& operator=(const Library& in_obj);

	/*! Assigns a CRef to this Library object. The Library object is cleared
	if the CRef is not a reference to an object that supports the Library class.
	\param in_ref A reference to an object that supports the Library class.
	\return The reinitialized Library object.
	*/
	Library& operator=(const CRef& in_ref);

	/*! Returns an array of the ProjectItem objects in this library.
	*/
	CRefArray GetItems()const;

	private:
	Library * operator&() const;
	Library * operator&();
};

};

#endif // __XSI_LIBRARY_H__
