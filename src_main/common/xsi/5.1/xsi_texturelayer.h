//*****************************************************************************
/*!
   \file xsi_texturelayer.h
   \brief TextureLayer class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITEXTURELAYER_H__
#define __XSITEXTURELAYER_H__

#include <xsi_projectitem.h>
#include <xsi_texturelayerport.h>

namespace XSI {


//*****************************************************************************
/*! \class TextureLayer xsi_texturelayer.h
	\brief The TextureLayer is an object which simplifies layering of shader effects on top of
	shaders and materials.  Each layer has a set of properties which describe its characteristics (such
	as color, mixing mode, scale factor) to specify how it will affect the shader ports it drives.  The
	order that layers appear under a container indicates the order the layering is applied, similar to the
	way a "mix N colors" shader node works.

	TextureLayerPort objects are owned by TextureLayers, and are used to specify which shader ports the
	layer should affect.  If there are no ports, then the layer is just a stub or placeholder, and as such
	does not affect the material's rendering.

	TextureLayers can be created using Shader::CreateTextureLayer and Material::CreateTextureLayer.
	TextureLayers can have "sub" render trees connected to them as driving inputs, in the same way one would
	connect Shader objects together (using Parameter::ConnectFromPreset or Parameter::Connect).

	\sa Shader::GetTextureLayers, Shader::CreateTextureLayer, Material::GetTextureLayers,
		Material::CreateTextureLayer, TextureLayerPort

	\since 4.0

	\eg Shows how to create, enumerate and remove texture layers.

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();
		X3DObject mySphere;
		root.AddGeometry( L"Sphere",L"MeshSurface",L"" ,mySphere );
		Material myMaterial;
		mySphere.AddMaterial(L"Phong",false,L"",myMaterial);
		CRefArray	myShaders;
		Shader myPhong;
		myShaders = myMaterial.GetShaders();
		myPhong = myShaders[0];

		CRefArray	myLayers(3);
		// Add a layer at the end (since there are no others the "After"
		// flag is irrelevant).
		myLayers[0] = myPhong.CreateTextureLayer( L"B", true );

		// Add another layer before the other one.
		myLayers[1] = myPhong.CreateTextureLayer( L"A", false, myLayers[0] );

		// Create a third layer at the very start.
		myLayers[2] = myPhong.CreateTextureLayer( L"base", false );

		myLayers = myPhong.GetTextureLayers();

		CString		l_str = L"Created " + CValue(myLayers.GetCount()).GetAsText() + L" layers.";
		app.LogMessage( l_str );

		for ( LONG i = 0; i < myLayers.GetCount(); i++ )
		{
			app.LogMessage( CValue(i + 1L).GetAsText() + L": " + TextureLayer(myLayers[i]).GetName() );
		}

		myPhong.RemoveTextureLayer( myLayers[1] );
		myPhong.RemoveTextureLayer(	myLayers[2] );

		myLayers = myPhong.GetTextureLayers();
		l_str = L"Only " + CValue(myLayers.GetCount()).GetAsText() + L" remain(s) after removal.";
		app.LogMessage( l_str );
		for ( i = 0; i < myLayers.GetCount(); i++ )
		{
			app.LogMessage( CValue(i + 1L).GetAsText() + L": " + TextureLayer(myLayers[i]).GetFullName() );
		}

		// This example should output something like this:
		// 'INFO : "Created 3 layers."
		// 'INFO : "1: base"
		// 'INFO : "2: A"
		// 'INFO : "3: B"
		// 'INFO : "Only 1 remain(s) after removal."
		// 'INFO : "1: sphere.Material.Phong.base"
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL TextureLayer : public ProjectItem
{
public:
	/*! Default constructor. */
	TextureLayer();

	/*! Default destructor. */
	~TextureLayer();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	TextureLayer(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	TextureLayer(const TextureLayer& in_obj);

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
	\return The new TextureLayer object.
	*/
	TextureLayer& operator=(const TextureLayer& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new TextureLayer object.
	*/
	TextureLayer& operator=(const CRef& in_ref);

	/*! Moves texture layers up or down in the ordered stacks of layers. This considers the
	overall order of layers in other containers that share the given texture layers, so that
	the ordering is always consistent.

	\param in_lOffset Amount to move the layer. A negative value moves it up in the list,
		positive moves it down.  The absolute value is the number of steps to move in that
		direction. Any value is valid, if more offset steps are requested than are actually
		available, the layer will simply move to the end (for positive offsets) or beginning
		(for negative offsets) of all the containers where it exists.  So specifying a very
		large value of offset is a good way to set a layer to be the final layer in all
		containers where it appears.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus		Move( LONG in_lOffset );

	/*! Adds a TextureLayerPort object (shader connection) to a texture layer. These indicate what
	shader ports are being driven by the layer. If the layer has ports added to it from containers
	it is currently not part of, it will be added to these containers (at the correct place in the
	stack, according to the order dependencies).

	\param in_param Parameter on the target (must be a port on a shader or material) to which to
		add a texture layer port connection (that is, to be driven by this layer).  Note that if a
		target is specified on a (valid) container which currently does not nest the layer, the
		layer will be added to that container.
	\return The new TextureLayerPort, or an empty one if it failed to be created.
	*/
	TextureLayerPort	AddTextureLayerPort( const Parameter& in_param );

	/*! Removes a TextureLayerPort from the texture layer. This actually disconnects it and causes
	the layer to no longer affect that shader property. It is valid to have texture layers which have
	no port connections, these empty layers act like placeholders, and do not affect the rendered result
	in any way.  They will maintain their ordering in the containers in which they appear.  Of course,
	new port connections can be added to them at any time.

	\param in_port TextureLayerPort to remove from this texture layer.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus		RemoveTextureLayerPort( const TextureLayerPort& in_port );

	/*! Removes a TextureLayerPort from the texture layer. This actually disconnects it and causes the
	layer to no longer affect that shader property. It is valid to have texture layers which have no
	port connections, these empty layers act like placeholders, and do not affect the rendered result
	in any way.  They will maintain their ordering in the containers in which they appear.  Of course,
	new port connections can be added to them at any time.

	\param in_portFullName Name of the TextureLayerPort to remove from this texture layer.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus		RemoveTextureLayerPort( const CString& in_portFullName );

	/*! Returns an array of references to the TextureLayerPorts owned by the texture layer.
	\return Array of references to TextureLayerPort objects.
	*/
	CRefArray	GetTextureLayerPorts() const;

	private:
	TextureLayer * operator&() const;
	TextureLayer * operator&();
};

};

#endif // __XSITEXTURELAYER_H__
