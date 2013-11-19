//*****************************************************************************
/*!
   \file xsi_rig.h
   \brief Rig class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIRIG_H__
#define __XSIRIG_H__

#include <xsi_null.h>
#include <xsi_status.h>

namespace XSI {

class Directed;
class X3DObject;

//*****************************************************************************
/*! \class Rig xsi_rig.h
	\brief Base class for object with an interest i.e.: CameraRig and LightRig.
 */
//*****************************************************************************

class SICPPSDKDECL Rig : public Null
{
public:
	/*! Default constructor. */
	Rig();

	/*! Default destructor. */
	~Rig();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Rig(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Rig(const Rig& in_obj);

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
	\return The new Rig object.
	*/
	Rig& operator=(const Rig& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Rig object.
	*/
	Rig& operator=(const CRef& in_ref);

	/*! Adds a directed object to this rig.
	\param in_dirObj	directed object.
	\param in_interest  interest of this directed object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus  AddDirectedObject
	(
		Directed&	in_dirObj,
		X3DObject&	in_interest
	);

	private:
	Rig * operator&() const;
	Rig * operator&();
};

};

#endif // __XSIRIG_H__
