//*****************************************************************************
/*!
   \file xsi_materiallibrary.h
   \brief MaterialLibrary class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSI_MATERIALLIBRARY_H__
#define __XSI_MATERIALLIBRARY_H__

#include <xsi_library.h>

namespace XSI {

//*****************************************************************************
/*! \class MaterialLibrary xsi_materiallibrary.h
	\brief The material library object is a specialized source library for storing and sharing materials.

	The storage may be internal or external. If internal, the library is saved in the scene file. If external,
	the library is saved in a separate file. The library can be exported to and imported from dotXSI or a
	native binary format.

	\note The parent of a material library is the Scene object.

	\sa Scene::GetActiveMaterialLibrary()
	\sa Library
 	\since 5.0

	\eg
	\code
		using namespace XSI;

		Application app;
		Scene scene = app.GetActiveProject().GetActiveScene();

		MaterialLibrary matlib = scene.GetActiveMaterialLibrary();

		Material newPhongMat = matlib.CreateMaterial(L"Phong", L"MyNewPhong" );

		CRefArray materials = matlib.GetItems();

		for ( LONG i=0; i < materials.GetCount(); i++ )
		{
			Material mat( materials[i] );

			if (mat == newPhongMat)
			{
				// found the new mat
				app.LogMessage( L"Found My New Phong Material: " + newPhongMat.GetName() );
			}
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL MaterialLibrary : public Library
{
public:
	/*! Constructs a MaterialLibrary object. */
	MaterialLibrary();

	/*! Destroys a MaterialLibrary object. */
	~MaterialLibrary();

	/*! Constructs a MaterialLibrary object from a CRef object.
	\param in_ref A reference to a library.
	*/
	MaterialLibrary(const CRef& in_ref);

	/*! Constructs a new MaterialLibrary object from an existing MaterialLibrary object.
	\param in_obj An existing MaterialLibrary object to copy into this MaterialLibrary object.
	*/
	MaterialLibrary(const MaterialLibrary& in_obj);

	/*! Returns True if this object supports the functionality of a specified class. For example,
	a MaterialLibrary is a type of Source, so a MaterialLibrary object supports Source functionality.
	\param in_ClassID Test if this object supports this class.
	\return True if this object supports the specified class, and false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Gets the class ID for this object.
	\return The class ID.
	*/
	siClassID GetClassID() const;

	/*! Assigns a MaterialLibrary object to an existing MaterialLibrary object.
	\param in_obj A MaterialLibrary object to be copied into this object.
	\return The reinitialized MaterialLibrary object.
	*/
	MaterialLibrary& operator=(const MaterialLibrary& in_obj);

	/*! Assigns a CRef to this MaterialLibrary object. The MaterialLibrary object is cleared
	if the CRef is not a reference to an object that supports the MaterialLibrary class.
	\param in_ref A reference to an object that supports the MaterialLibrary class.
	\return The reinitialized MaterialLibrary object.
	*/
	MaterialLibrary& operator=(const CRef& in_ref);

	/*! Creates and adds a Material to the material library. The newly created material is not attached
	to any object.

	\param in_strPreset Name of a shader preset to apply to object. If the name is empty, the material is
		still created and added but it will have no shaders connected to its parameters.
	\param in_strName	 Name of the new Material object.

	\return CRef The newly created Material object.
	\since 5.0

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		// Create a 2x2 grid w the mesh builder and assign a new material created in
		// the current material library
		X3DObject myObj;
		CMeshBuilder msBuilder;
		root.AddPolygonMesh( L"myObj", myObj, msBuilder );

		// Add vertices to mesh
		double myVertexPositionArray[27] =
		{
			-4, 0, -4,
			-4, 0,  0,
			-4, 0,  4,
			 0, 0, -4,
			 0, 0,  0,
			 0, 0,  4,
			 4, 0, -4,
			 4, 0,  0,
			 4, 0,  4
		};

		msBuilder.AddVertices( 9, myVertexPositionArray );

		// Appends polygons to mesh
		LONG pPolyVertexCounts[4] = {4,4,4,4};
		LONG pVtxIndices[16] =
		{
			0, 1, 4, 3,
			1, 2, 5, 4,
			3, 4, 7, 6,
			4, 5, 8, 7
		};

		msBuilder.AddPolygons( 4, pPolyVertexCounts, pVtxIndices );

		// Generate the new mesh
		msBuilder.Build(false);

		// Set polygon 1 and 3 with a new phong material created in the
		// current material library
		Scene scene = app.GetActiveProject().GetActiveScene();
		MaterialLibrary matlib = scene.GetActiveMaterialLibrary();
		Material myPhongMat = matlib.CreateMaterial(L"Phong", L"MyNewPhong" );

		LONG myPolys[2] = {1,3};
		msBuilder.SetPolygonsMaterial( myPhongMat, 2, myPolys );
	\endcode
	*/
	CRef CreateMaterial(
		const CString& in_strPreset=CString(),
		const CString& in_strName=CString() );

	private:
	MaterialLibrary * operator&() const;
	MaterialLibrary * operator&();
};

};

#endif // __XSI_MATERIALLIBRARY_H__
