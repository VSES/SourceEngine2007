//*****************************************************************************
/*!
   \file xsi_texturelayerport.h
   \brief TextureLayerPort class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITEXTURELAYERPORT_H__
#define __XSITEXTURELAYERPORT_H__

#include <xsi_projectitem.h>

namespace XSI {


//*****************************************************************************
/*! \class TextureLayerPort xsi_texturelayerport.h
	\brief The TextureLayerPort is an object used to describe which specific shader and material
	parameters a TextureLayer affects.  Like shader connections, they indicate what shader ports
	are being driven by the layer.

	The port has a few properties which describe how the layer affects a single shading parameter,
	generally these are used to modify the layer's settings in a port-specific way, for example
	by inverting or scaling the overall effect.  A layer may be set up to blend its color by 50% with
	the layer beneath it.  However, that layer's the ambient port effect may only be 25% whereas the diffuse
	port contribution could be set to 100%.

	\sa TextureLayer::GetTextureLayerPorts, TextureLayer::AddTextureLayerPort, Shader
	\since 4.0

	\eg This C++ example shows how to create texture layer ports, plus enumerating and removing them.
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

		TextureLayer	myLayer;
		// Add a layer (since there are no others the "After" flag is irrelevant).
		myLayer = myPhong.CreateTextureLayer( L"DirtLayer", true );

		CRefArray		myPorts(3);
		// Add some texture layer port connections on this layer.
		myPorts[0] = myLayer.AddTextureLayerPort( myPhong.GetParameters().GetItem( L"ambient" ) );
		myPorts[1] = myLayer.AddTextureLayerPort( myPhong.GetParameters().GetItem( L"diffuse" ) );
		myPorts[2] = myLayer.AddTextureLayerPort( myPhong.GetParameters().GetItem( L"specular" ) );

		myPorts = myLayer.GetTextureLayerPorts();
		app.LogMessage( L"Created " + CValue(myPorts.GetCount()).GetAsText() + L" ports." );

		for ( LONG i = 0; i < myPorts.GetCount(); i++ )
		{
			TextureLayerPort	curPort = myPorts[i];
			app.LogMessage( CValue(i + 1L).GetAsText() + L": " + curPort.GetName() +
				L" --> " + curPort.GetTarget().GetFullName() );
		}

		myLayer.RemoveTextureLayerPort( myPorts[1] );
		myLayer.RemoveTextureLayerPort(	myLayer.GetFullName() + L"." + TextureLayerPort(myPorts[2]).GetName() );

		myPorts = myLayer.GetTextureLayerPorts();
		app.LogMessage( L"Only " + CValue(myPorts.GetCount()).GetAsText() + L" remain(s) after removal." );
		for ( i = 0; i < myPorts.GetCount(); i++ )
		{
			app.LogMessage( CValue(i + 1L).GetAsText() + L": " + TextureLayerPort(myPorts[i]).GetFullName() );
		}

		// This example should output something like this:
		// INFO : "Created 3 ports."
		// INFO : "1: ambient --> sphere.Material.Phong.ambient"
		// INFO : "2: diffuse --> sphere.Material.Phong.diffuse"
		// INFO : "3: specular --> sphere.Material.Phong.specular"
		// INFO : "Only 1 remain(s) after removal."
		// INFO : "1: sphere.Material.Phong.DirtLayer.ambient"
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL TextureLayerPort : public ProjectItem
{
public:
	/*! Default constructor. */
	TextureLayerPort();

	/*! Default destructor. */
	~TextureLayerPort();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	TextureLayerPort(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	TextureLayerPort(const TextureLayerPort& in_obj);

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
	\return The new TextureLayerPort object.
	*/
	TextureLayerPort& operator=(const TextureLayerPort& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new TextureLayerPort object.
	*/
	TextureLayerPort& operator=(const CRef& in_ref);

	/*! Returns a reference to the Parameter which is driven by this port.
	\return The driven Parameter.
	*/
	Parameter	GetTarget() const;

	private:
	TextureLayerPort * operator&() const;
	TextureLayerPort * operator&();
};

};

#endif // __XSITEXTURELAYERPORT_H__
