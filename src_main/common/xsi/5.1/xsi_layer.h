//*****************************************************************************
/*!
   \file xsi_layer.h
   \brief Layer class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSILAYER_H__
#define __XSILAYER_H__

#include <xsi_group.h>

namespace XSI {

//*****************************************************************************
/*! \class Layer xsi_layer.h
	\brief The Layer is a self-contained collection of Scene objects. Currently, it implements no
	functions of its own.

	You can create a new layer with the \xl CreateLayer command, and access a scene's layers via
	the Scene: see Scene::GetLayers or Scene::GetActiveLayer.

	\sa Scene::GetLayers, Scene::GetActiveLayer, \xl CreateLayer
 */
//*****************************************************************************
class SICPPSDKDECL Layer : public Group
{
public:
	/*! Default constructor. */
	Layer();

	/*! Default destructor. */
	~Layer();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Layer(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Layer(const Layer& in_obj);

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
	\return The new Layer object.
	*/
	Layer& operator=(const Layer& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Layer object.
	*/
	Layer& operator=(const CRef& in_ref);

	private:
	Layer * operator&() const;
	Layer * operator&();

	public:


};

};

#endif // __XSILAYER_H__
