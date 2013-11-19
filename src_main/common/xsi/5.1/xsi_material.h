//*****************************************************************************
/*!
   \file xsi_material.h
   \brief Material class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIMATERIAL_H__
#define __XSIMATERIAL_H__

#include <xsi_property.h>
#include <xsi_texturelayer.h>

namespace XSI {

class OGLMaterial;
class OGLTutorial;
class OGLTexture;
class ClusterProperty;
class ImageClip;
class ImageClip2;
class Texture;
class Library;
class CParameterRefArray;
class CLongArray;

//*****************************************************************************
/*! \class Material xsi_material.h
	\brief The Material object represents a material property of a SceneItem object.

	You can create a material with SceneItem::AddMaterial.

	\note If you get a material from a Material Library, use ProjectItem::GetParent, not ProjectItem::GetModel,
		to get the Library object (ProjectItem::GetModel returns an invalid Model).

	\sa Scene::GetActiveMaterialLibrary(), OGLMaterial

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube",L"MeshSurface",L"" ,myCube );

		Material myMaterial;
		myCube.AddMaterial(L"Phong",false,L"",myMaterial);

		app.LogMessage( CString(L"The material: ") + myMaterial.GetFullName() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Material : public Property
{
public:
	/*! Constructs a Material object. */
	Material();

	/*! Destroys a Material object. */
	~Material();

	/*! Constructs a Material object from a CRef object.
	\param in_ref A reference to a material.
	*/
	Material(const CRef& in_ref);

	/*! Constructs a new Material object from an existing Material object.
	\param in_obj An existing Material object to copy into this Material object.
	*/
	Material(const Material& in_obj);

	/*! Returns True if this object supports the functionality of a specified class. For example,
	a Material is a type of Property, so a Material object supports Property functionality.
	\param in_ClassID Test if this object supports this class.
	\return True if this object supports the specified class, and false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the class ID for this object.
	\return The class ID.
	*/
	siClassID GetClassID() const;

	/*! Assigns a Material object to an existing Material object.
	\param in_obj A Material object to be copied into this object.
	\return The reinitialized Material object.
	*/
	Material& operator=(const Material& in_obj);

	/*! Assigns a CRef to this material. The Material object is cleared
	if the CRef is not a reference to an object that supports the Material class.
	\param in_ref A reference to an object that supports the Material class.
	\return The reinitialized Material object.
	*/
	Material& operator=(const CRef& in_ref);

	/*! Returns the shaders connected to this material.
	\return An array of references to Shader objects.
	*/
	CRefArray	GetShaders() const;

	/*! Returns the image clips connected to the material.
	\return An array of references to ImageClip objects.
	\since 4.0
	*/
	CRefArray	GetImageClips() const;

	/*! Returns the OGLMaterial for this material. The OGLMaterial is a read-only object that holds
	the material rendering information used by OpenGL.
	*/
	OGLMaterial	GetOGLMaterial() const;

	/*! Returns the OGLTexture for this Material. The OGLTexture is a read-only object that holds
	the texturing information used by OpenGL.
	*/
	OGLTexture	GetOGLTexture() const;

	/*! Returns the ClusterProperty containing the current UV coordinages used to display the texture in OpenGL.
	*/
	ClusterProperty GetCurrentUV() const;

	/*! Obsolete. Use Material::GetCurrentImageClip2 instead.
	*/
	ImageClip	GetCurrentImageClip() const;

	/*! Returns the current image clip used for OpenGL display.
	\since 4.0
	*/
	ImageClip2	GetCurrentImageClip2() const;

	/*! Returns the texture using the default UV coordinates used in OpenGL.
	*/
	Texture	GetCurrentTexture() const;

	//
	// Texture layering methods -- same as in Shader.
	//


	/*! Creates a new texture layer belonging to the container (the material).

	\param in_strName The name of new texture layer.
	\param in_bAfter True to insert the layer after the reference layer (in_refLayer), and False to insert the layer
		before the reference layer. If there is no reference layer, then True adds the layer to the end of the list,
		and False adds the layer to the start of the list.
	\param in_refLayer The reference texture layer. Used to indicate where the new layer is inserted in the list.

	\return The new texture layer.
	\since 4.0
	*/
	TextureLayer	CreateTextureLayer( const CString& in_strName = CString(), bool in_bAfter = true, const TextureLayer& in_refLayer = TextureLayer() );

	/*! Adds an existing texture layer to the container.  The layer already belongs to at least one other texture layer
	container (either a Shader or a Material).

	\param in_layer The texture layer to add to this container.
	\param in_bAfter True to insert the layer after the reference layer (in_refLayer), and False to insert the layer
		before the reference layer. If there is no reference layer, then True adds the layer to the end of the list,
		and False adds the layer to the start of the list.
	\param in_refLayer The reference texture layer. Used to indicate where the new layer is inserted in the list.

	\return CStatus::OK if successful
	\return CStatus::Fail if the texture layer could not be created.
	\since 4.0
	*/
	CStatus			AddSharedTextureLayer( const TextureLayer& in_layer, bool in_bAfter = true, const TextureLayer& in_refLayer = TextureLayer() );

	/*! Returns the texture layers owned by the material.
	\return An array of references to TextureLayer objects.
	\since 4.0
	*/
	CRefArray		GetTextureLayers() const;

	/*! Removes a texture layer from the container.  If this container is the only owner of the layer, then the layer
	is deleted from the scene. Otherwise, the layer is removed from this container only.
	\param in_layer The texture layer to remove from this container.
	\return CStatus::OK if successful
	\return CStatus::Fail if the texture layer could not be removed.
	\since 4.0
	*/
	CStatus			RemoveTextureLayer( const TextureLayer& in_layer );


	/*! Returns the owners of this material.
	\return An array of references to the owners.
	\since 4.0
	*/
	CRefArray GetUsedBy() const;
	/*! Tests whether this material is the intended material or a stand-in.
	\return True if this material is the intended material, and False otherwise.
	\since 4.0
	*/
	bool    IsResolved();
	/*! Attempts to reconnect to the proper material. This works only when Material::IsResolved returns False.
	\return CStatus::OK if successful
	\return CStatus::Fail if the material could not be resolved.
	\since 4.0
	*/
	CStatus	Resolve();
	/*! Returns the full name of the material that this material is supposed to reference.
	\return The fullname of the material.
	\since 4.0
	*/
	CString	GetUnresolvedFullname() const;

	/*! Returns the material library that this material belongs to.
	\since 4.0
	*/
	Library	GetLibrary() const;

	/*! Adds a Property to the material. For example, you can add a UserDataBlob or CustomProperty to store custom
	user data inside a render tree.

	\param in_preset A preset name or the name of a self-installed CustomProperty.
	\param in_bBranch Should always be false.
	\param in_name Optional name for the new property.  If no name is specified, the name is based on in_preset.

	\sa Material::GetProperties, SceneItem::AddProperty
	\since 4.0
	*/
	Property 	AddProperty
	(
		const CString&	in_preset,
		bool			in_bBranch = false ,
		const CString&	in_name = CString()
	);

	/*! Returns the properties applied to the material. A material may have nested CustomProperty or UserDataBlob properties.
  	\return An array of Property objects.
	\sa Material::AddProperty, SceneItem::GetProperties
	\since 4.0
	*/
	CRefArray GetProperties() const ;

	/*!	Returns an array of CustomProperty objects connected to this material shader. These cluster properties are mostly UV
	and Color at Vertices properties. The function also returns the shader connection points, as Parameter objects, for each
	cluster property.

	\retval out_refArray Array of Parameter objects that connect the cluster properties.
  	\return Array of Cluster Property objects.

	\sa CGeometryAccessor::GetMaterials
	\since 5.0

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube",L"MeshSurface",L"" ,myCube );

		Material myMaterial;
		myCube.AddMaterial(L"Phong",false,L"",myMaterial);

		//Adding an image
		CValueArray args(4L);
		CValue outArg;
		args[0] = CValue( CString(L"Image") );
		args[1] = CValue(myCube.GetRef());
		args[2] = CValue((short)1);
		args[3] = CValue(false);

		app.ExecuteCommand( L"BlendInPresets", args, outArg );

		//Adding a texture support
		args[0] = myCube.GetRef();
		args[1] = (LONG)siTxtPlanarXY;
		args[2] = (LONG)siTxtDefaultSpherical;
		args[3] = CString(L"Texture_Support");
		app.ExecuteCommand( L"CreateTextureSupport", args, outArg );

		//Now get the cluster properties connected to this material
		Primitive prim = myCube.GetActivePrimitive();
		PolygonMesh mesh = prim.GetGeometry();
		CGeometryAccessor mt = mesh.GetGeometryAccessor();

		CRefArray mats = mt.GetMaterials();

		LONG nVals = mats.GetCount();

		for ( LONG i=0; i<nVals; i++ )
		{
			Material mat(mats[i]);

			CParameterRefArray params;
			CRefArray clsProps = mat.GetConnectedClusterProperties(params);

			CString str(mat.GetName());

			for (LONG j=0; j<clsProps.GetCount(); j++)
			{
				str += L"\n\t";

				ClusterProperty cp(clsProps[j]);

				str += cp.GetName();

				str += L" type: ";
				switch (cp.GetPropertyType())
				{
					case siClusterPropertyEnvelopeWeightType:
						str += L"siClusterPropertyEnvelopeWeightType";
						break;
					case siClusterPropertyShapeKeyType:
						str += L"siClusterPropertyShapeKeyType";
						break;
					case siClusterPropertyWeightMapType:
						str += L"siClusterPropertyWeightMapType";
						break;
					case siClusterPropertyUVType:
						str += L"siClusterPropertyUVType";
						break;
					case siClusterPropertyVertexColorType:
						str += L"siClusterPropertyVertexColorType";
						break;
					case siClusterPropertyUserNormalType:
						str += L"siClusterPropertyUserNormalType";
						break;
				};

				str += L" elem count: " + CString(cp.GetValueSize());

				// connection point parameter
				Parameter param = params[j];

				str += L"\n\t\tParameter path: ";
				str += param.GetFullName();
				str += L"\n";

				// show texture information
				Texture txt(param.GetParent());
				str += L"\t\tParameter parent: ";
				str += txt.GetFullName();
				str += L"\n";

				// get the image clip filename
				ImageClip2 clip(txt.GetImageClip());

				str += L"\t\tTexture image: ";
				str += clip.GetFileName();
			}

			app.LogMessage( CString(i) + L" : " + str );
		}
	\endcode
	*/
	CRefArray GetConnectedClusterProperties( CParameterRefArray& out_refArray ) const ;

	/*! Returns the input type of a material parameter. Before you connect a shader to the parameter, use
	Parameter::GetCapabilities to make sure that the parameter supports the siTexturable capability.
	\param in_paramscriptname The script name of a parameter.
	\return The shader input type.
	\return siUnknownParameterType if the parameter cannot be connected to a shader.
	\since 5.0
	*/
	siShaderParameterType GetShaderInputType(const CString& in_paramscriptname) const;

	/*! Returns all shaders that match a Filter. For example, you can write a custom filter that matches shaders
	with texturespace and colormap parameters, and then use this custom filter with this function.
	\param in_filtername The name of a shader Filter (for example, ::siShaderFilter).
	\return Array of shaders that match the filter.
	\since 5.0
	*/
	CRefArray FindShaders(const CString& in_filtername) const;

	private:
	Material * operator&() const;
	Material * operator&();
};

};

#endif // __XSIMATERIAL_H__
