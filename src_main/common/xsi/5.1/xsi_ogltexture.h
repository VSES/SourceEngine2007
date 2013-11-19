//*****************************************************************************
/*!
   \file xsi_ogltexture.h
   \brief OGLTexture class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIOGLTEXTURE_H__
#define __XSIOGLTEXTURE_H__

#include <xsi_base.h>

namespace XSI {

//*****************************************************************************
/*! \class OGLTexture xsi_ogltexture.h
	\brief The OGLTexture object represents the OGL texture properties of a material
	object.

	OGLTexture objects are read-only. OGLTexture can be created with
	Material.OGLTexture.

	\sa Material

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		CValueArray args(4);
		args[0] = CValue( CString(L"Image") );
		args[1] = CValue(myGrid.GetRef());
		args[2] = CValue((short)1);
		args[3] = CValue(false);

		CStatus st;
		CValue outArg;
		st = app.ExecuteCommand( L"BlendInPresets", args, outArg );
		if ( st.GetCode() != CStatus::OK )
		{
			app.LogMessage( L"BlendInPresets failed" );
			return;
		}

		OGLTexture myOGLTexture = myGrid.GetMaterial().GetOGLTexture();

		app.LogMessage( L"Texture's width: " +
			CValue(myOGLTexture.GetWidth()).GetAsText() );
		app.LogMessage( L"Texture's height: " +
			CValue(myOGLTexture.GetHeight()).GetAsText() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL OGLTexture : public CBase
{
public:
	/*! Default constructor. */
	OGLTexture();

	/*! Default destructor. */
	~OGLTexture();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	OGLTexture(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	OGLTexture(const OGLTexture& in_obj);

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
	\return The new OGLTexture object.
	*/
	OGLTexture& operator=(const OGLTexture& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new OGLTexture object.
	*/
	OGLTexture& operator=(const CRef& in_ref);

	/*! Returns the width of this texture's image in pixels.
	\return The width of this texture's image in pixels.
	*/
	short	GetWidth() const;

	/*! Returns the height of this texture's image in pixels.
	\return height of this texture's image in pixels.
	*/
	short	GetHeight() const;

	/*! Returns this texture's image file path name.
	\return The texture image file path name.
	*/
	CString	GetFullName() const;

	private:
	OGLTexture * operator&() const;
	OGLTexture * operator&();
};

};

#endif // __XSIOGLTEXTURE_H__
