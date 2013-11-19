//*****************************************************************************
/*!
   \file xsi_shader.h
   \brief Shader class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISHADER_H__
#define __XSISHADER_H__

#include <xsi_projectitem.h>
#include <xsi_texturelayer.h>

namespace XSI {

class Property ;

//*****************************************************************************
/*! \class Shader xsi_shader.h
	\brief The %Shader object represents a node in the render tree of an object.

	The initial %Shader of a render tree can be created with SceneItem::AddMaterial
	and additional shaders can be created and connected together with
	Parameter::ConnectFromPreset and Parameter::Connect.

	A %Shader is a type of \xl DataSource.

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

		CRefArray refShaders = myMaterial.GetShaders();
		for(LONG i = 0;i < refShaders.GetCount(); ++i)
		{
			app.LogMessage( L"The shader: " +
							Shader(refShaders[i]).GetFullName() );
		}
	\endcode

	\eg Phong Class Example
	The Shader object provides generalized access to the parameters of any
	shader object.  Each shader has its own unique definition for
	which parameters it exposes.  This example is a wrapper around the
	Shader object that works specifically for the Phong shader.

	In order to keep the example short we don't cover every single parameter of
	a phong, for example "incandescence", "reflectivity", "trans_glossy".
	However handling those parameters would follow the exact same pattern.
	In fact it is conceivable that this code could be generated automatically
	using a script.

	It is easy to find the entire list of parameters by calling
	ProjectItem::GetParameters.  They are also documented in the SPDL parameter
	section of the reference.

	\code
		class CPhong
		{
		public:
			CPhong( const Shader & in_phong )
				: shader( in_phong )
			{
				// This object only works if we are constructed with a valid
				// reference to a phong shader
				assert( shader.IsValid() ) ;
				assert( shader.GetProgID() == L"Softimage.material-phong.1" ) ;
			}

			//
			// GetMethods
			//

			// Access to simple parameters
			float GetShiny()
			{
				return shader.GetParameterValue(L"Shiny") ;
			}

			LONG GetReflectSamples()
			{
				return shader.GetParameterValue(L"reflect_samples") ;
			}

			bool IsDiffuseInUse()
			{
				return shader.GetParameterValue(L"diffuse_inuse") ;
			}

			// Access to color parameters
			CStatus GetAmbient(float& r, float& g, float& b, float& a)
			{
				return shader.GetColorParameterValue(L"ambient",r,g,b,a ) ;
			}

			CStatus GetDiffuse(float& r, float& g, float& b, float& a)
			{
				return shader.GetColorParameterValue(L"diffuse",r,g,b,a ) ;
			}

			//
			// Put Methods
			//
			void PutShiny( float in_v )
			{
				shader.PutParameterValue( L"Shiny", in_v ) ;
			}

			void PutRelectSamples( LONG in_v )
			{
				shader.PutParameterValue( L"reflect_samples", in_v ) ;
			}

			void SetDiffuseInUse( bool in_enable )
			{
				shader.PutParameterValue( L"diffuse_inuse", in_enable ) ;
			}

			void PutAmbient(float r, float g, float b, float a)
			{
				shader.PutColorParameterValue( L"ambient", r, g, b, a ) ;
			}

			void PutDiffuse(float r, float g, float b, float a)
			{
				shader.PutColorParameterValue( L"diffuse", r, g, b, a ) ;
			}

		private:
			CPhong() ;
			Shader shader ; // We use this C++ API object to read the information
		} ;
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Shader : public ProjectItem
{
public:
	/*! Constructs a Shader object. */
	Shader();

	/*! Destructor. */
	~Shader();

	/*! Constructs a Shader object from a CRef object.
	\param in_ref A reference to a shader.
	*/
	Shader(const CRef& in_ref);

	/*! Constructs a new Shader object from an existing Shader object.
	\param in_obj An existing Shader object to copy into this Shader object.
	*/
	Shader(const Shader& in_obj);

	/*! Returns true if this object supports the functionality of a specified class. For example,
	a Shader is a type of ProjectItem, so a Shader object supports ProjectItem functionality.
	\param in_ClassID Test if this object supports this class.
	\return True if this object supports the specified class, and false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the class ID for this object.
	\return The class ID.
	*/
	siClassID GetClassID() const;

	/*! Assigns a Shader object to an existing Shader object.
	\param in_obj A Shader object to be copied into this object.
	\return The reinitialized Shader object.
	*/
	Shader& operator=(const Shader& in_obj);


	/*! Assigns a CRef to this Shader object. The Shader object is cleared if the CRef is not a reference
	to an object that supports the Shader class.
	\param in_ref A reference to an object that supports the Shader class.
	\return The reinitialized Shader object.
	*/
	Shader& operator=(const CRef& in_ref);

	/*! Returns an array of references to the shaders connected to this Shader.
	\return CRefArray of references to the connected Shader objects.
	*/
	CRefArray	GetShaders() const;

	/*! Returns an array of references to the ImageClip objects in the shader tree whose root is this shader.
	\return Array of references to ImageClip objects.
	\since 4.0
	*/
	CRefArray	GetImageClips() const;

	/*! Returns the programmatic identifier (ProgID) of this shader. You can use the ProgID with
	Parameter::ConnectFromProgID to recreate the shader.
	\return The string representing the ProgID.
	*/
	CString		GetProgID() const;

	/*! Returns the output type of this shader. This is a convenient alternative to using Parameter::GetValueType
	for getting the parameter type of a color parameter.
	\note This function replaces Shader::GetOutputType.
	\return The shader output type.
	\since 5.0
	*/
	siShaderParameterType GetOutputType2() const;

	/*! This function is obsolete and replaced by Shader::GetOutputType2
	\deprecated 5.0 (Shader::GetOutputType2 uses ::siShaderParameterType enum values instead of ::siShaderOutputType)
	*/
	siShaderOutputType GetOutputType() const;


	/*! Returns the RGBA values of a color parameter. This is a convenient alternative to using
	ProjectItem::GetParameterValue for getting the RGBA values of a color parameter.

	\note This function returns CStatus::InvalidArgument if the parameter is not a color or if it is
		driven by a connected Shader.

	\param in_name The scripting name of the Parameter, for example \c diffuse, \c specular, or \c ambient.
	\retval out_red The Red component value, typically a number in the range 0.0 to 1.0.
	\retval out_green The Green component value, typically a number in the range 0.0 to 1.0.
	\retval out_blue The Blue component value, typically a number in the range 0.0 to 1.0.
	\retval out_alpha The Alpha component value.
	\param in_time Time (in frames) at which to get the value. By default, GetColorParameterValue gets the
		value at the current frame (represented by the value DBL_MAX).
	*/
	CStatus GetColorParameterValue(
				const CString& in_name,
				float& out_red,
				float& out_green,
				float& out_blue,
				float& out_alpha,
				double in_time = DBL_MAX ) ;

	/*! Sets the RGBA values for a color parameter. This is a convenient alternative to ProjectItem::PutParameterValue
    for setting the RGBA values of a color parameter.

	\note All shader colors are represented with floating-point precision.

	\param in_name The scripting name of the color parameter, for example "ambient".
	\param in_red The Red component value, typically a number in the range 0 to 1.0.
	\param in_green The Green component value, typically a number in the range 0 to 1.0.
	\param in_blue The Blue component value, typically a number in the range 0 to 1.0.
	\param in_alpha The Alpha component value.  If the Alpha channel is hidden in the user
		interface and is not used by the shader, setting the Alpha value has no effect.
	\param in_time Time (in frames) at which to set the color values. By default, PutColorParameterValue sets the
		value at the current frame (which is represented by the value DBL_MAX).
	*/
	CStatus PutColorParameterValue(
				const CString& in_name,
				float in_red,
				float in_green,
				float in_blue,
				float in_alpha = 1.0f,
				double in_time = DBL_MAX ) ;

	//
	// Texture layering methods -- same as in Material.
	//

	/*! Creates a new texture layer belonging to the container (the shader).
	\param in_strName The name of new texture layer.
	\param in_bAfter True to insert the layer after the reference layer (in_refLayer), and False to insert the layer
		before the reference layer. If there is no reference layer, then True adds the layer to the end of the list,
		and False adds the layer to the start of the list.
	\param in_refLayer The reference texture layer. Used to indicate where the new layer is inserted in the list.
	\return The new texture layer.
	\since 4.0
	*/
	TextureLayer	CreateTextureLayer( const CString& in_strName = CString(), bool in_bAfter = true, const TextureLayer& in_refLayer = TextureLayer() );

	/*! Adds an existing texture layer to the container.  The layer already belongs to at least one other
	texture layer container (either a Shader or a Material).
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

	/*! Returns a collection of TextureLayer objects owned by the shader.
	\return An array of references to the texture layers.
	\since 4.0
	*/
	CRefArray		GetTextureLayers() const;

	/*! Removes a texture layer from the container.  If this container is the only owner of the layer, then the layer
	is deleted from the scene, otherwise the layer is removed from this container (but is still owned by other containers).
	\param in_layer Texture layer to remove from this container.
	\return CStatus::OK if successful
	\return CStatus::Fail if the layer could not be removed.
	\since 4.0
	*/
	CStatus			RemoveTextureLayer( const TextureLayer& in_layer );


	/*! Returns true if the shader supports the specified renderer.
	\param in_strRenderer Name of a renderer.
	\return True if the renderer is supported, and False otherwise.
	\since 4.0
	*/
	bool HasRenderer( const CString &in_strRenderer ) const;

	/*! Returns the value of a renderer option. If the renderer or the option do not exist for this shader, an empty
	CValue is returned.
	\param in_strRenderer Name of a renderer.
	\param in_strOption Name of a render option.
	\return The value of the option.
	\since 4.0
	*/
	CValue GetRendererOption(
		const CString	&in_strRenderer,
		const CString	&in_strOption
		) const;

	/*! Returns the symbol name of the shader code executed by the renderer for this shader. If the symbol name is
	not set, an empty string is returned.
	\param in_strRenderer Name of a renderer.
	\return Symbol name of the shader code.
	\since 4.0
	*/
	CString GetSymbolName(
		const CString	&in_strRenderer
		) const;

	/*! Returns the file path of the shader code executed during rendering. If the file path is not set, an empty
	string is returned.
	\param in_strRenderer Name of a renderer.
	\return The file path of the shader code.
	\since 4.0
	*/
	CString GetCodePath(
		const CString	&in_strRenderer
		) const;

	/*! Returns the text of the shader code executed during rendering. If the text is not set, an empty string is returned.
	\param in_strRenderer Name of a renderer.
	\return The text of the shader code.
	\since 4.0
	*/
	CString GetCodeText(
		const CString	&in_strRenderer
		) const;

	/*! Adds a property to the shader. For example, you can add a UserDataBlob or CustomProperty to store custom user
	data inside a render tree.
	\param in_preset A preset name or the name of a self-installed CustomProperty.
	\param in_bBranch Should always be false.
	\param in_name Optional name for the new property.  If no name is specified, the name is based on in_preset.
	\return The new Property.
	\sa Shader::GetProperties, SceneItem::AddProperty
	\since 4.0
	*/
	Property 	AddProperty
	(
		const CString&	in_preset,
		bool			in_bBranch = false ,
		const CString&	in_name = CString()
	);

	/*! Returns an array of all Property objects applied to the shader. A Shader may have nested CustomProperty or
	UserDataBlob properties.
	\sa Shader::AddProperty, SceneItem::GetProperties
	\since 4.0
	*/
	CRefArray GetProperties() const ;


	/*! Returns the input type of a shader parameter. Before you connect a shader to the parameter, use
	Parameter::GetCapabilities to make sure that the parameter supports the siTexturable capability (see the
	::siCapabilities enum).
	\param in_paramscriptname The script name of a parameter.
	\return The shader input type.  If the parameter cannot be connected to a shader, siUnknownParameterType is returned.
	\since 5.0
	*/
	siShaderParameterType GetShaderInputType(const CString& in_scriptname) const;

	/*! Returns the root of the shader tree. The root may be a Camera, Light, Material, or a geometry shader.
	\since 5.0
	*/
	CRef GetRoot(void) const;

	private:
	Shader * operator&() const;
	Shader * operator&();
};

};

#endif // __XSISHADER_H__
