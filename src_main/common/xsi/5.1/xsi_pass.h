//*****************************************************************************
/*!
   \file xsi_pass.h
   \brief Pass class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPASS_H__
#define __XSIPASS_H__

#include <xsi_group.h>

namespace XSI {

//*****************************************************************************
/*! \class Pass xsi_pass.h
	\brief A render pass creates a picture layer of a scene that can be composited with any other
	passes to create a complete image. Passes also allow you to quickly re-render a single layer
	without re-rendering the entire scene. Later you can composite the rendered passes using %XSI
	FX, the fully-integrated compositing and effects toolset, or a compositing tool like Avid|DS.

	Each scene can contain as many render passes as you need. When you first create a scene in %XSI,
	it has a single pass named Default_pass. This is a "beauty pass" that is set to render every
	element of the scene--you can render a single beauty pass or you can render separate passes.
	Also, you can use preset passes such as matte, shadow, toon, and highlight, or you can create
	your own passes.
 */
//*****************************************************************************

class SICPPSDKDECL Pass : public Group
{
public:
	/*! Default constructor. */
	Pass();

	/*! Default destructor. */
	~Pass();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Pass(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Pass(const Pass& in_obj);

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
	\return The new Pass object.
	*/
	Pass& operator=(const Pass& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Pass object.
	*/
	Pass& operator=(const CRef& in_ref);

	private:
	Pass * operator&() const;
	Pass * operator&();
};

};

#endif // __XSIPASS_H__
