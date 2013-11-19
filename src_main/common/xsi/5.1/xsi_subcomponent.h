//*****************************************************************************
/*!
   \file xsi_subcomponent.h
   \brief SubComponent class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISUBCOMPONENT_H__
#define __XSISUBCOMPONENT_H__

#include <xsi_siobject.h>

namespace XSI {

//*****************************************************************************
/*! \class SubComponent xsi_subcomponent.h
	\brief Provides access to the subelements of an object's Geometry.
 */
//*****************************************************************************

class SICPPSDKDECL SubComponent : public SIObject
{
public:
	/*! Default constructor. */
	SubComponent();

	/*! Default destructor. */
	~SubComponent();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	SubComponent(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	SubComponent(const SubComponent& in_obj);

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
	\return The new SubComponent object.
	*/
	SubComponent& operator=(const SubComponent& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new SubComponent object.
	*/
	SubComponent& operator=(const CRef& in_ref);

	private:
	SubComponent * operator&() const;
	SubComponent * operator&();
};

};

#endif // __XSISUBCOMPONENT_H__
