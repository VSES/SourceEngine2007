//*****************************************************************************
/*!
   \file xsi_uipersistable.h
   \brief UIPersistable class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIUIPERSISTABLE_H__
#define __XSIUIPERSISTABLE_H__

#include <xsi_uiobject.h>
#include <xsi_value.h>
#include <xsi_status.h>

namespace XSI {

class View;

//*****************************************************************************
/*! \class UIPersistable xsi_uipersistable.h
	\brief The UIPersistable object is the base class for XSI UI persistable objects such as
	the View and Layout objects.

	\sa View, Layout
	\since 4.0
*/
//*****************************************************************************

class SICPPSDKDECL UIPersistable : public UIObject
{
public:
	/*! Default constructor. */
	UIPersistable();

	/*! Default destructor. */
	~UIPersistable();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	UIPersistable(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	UIPersistable(const UIPersistable& in_obj);

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
	\return The new UIPersistable object.
	*/
	UIPersistable& operator=(const UIPersistable& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new UIPersistable object.
	*/
	UIPersistable& operator=(const CRef& in_ref);

	private:
	UIPersistable * operator&() const;
	UIPersistable * operator&();
};

};
#endif // __XSIUIPERSISTABLE_H__
