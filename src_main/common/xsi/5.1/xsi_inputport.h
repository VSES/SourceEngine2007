//*****************************************************************************
/*!
   \file xsi_inputport.h
   \brief InputPort class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIINPUTPORT_H__
#define __XSIINPUTPORT_H__

#include <xsi_port.h>

namespace XSI {

class CValue;

//*****************************************************************************
/*! \class InputPort xsi_inputport.h
	\brief An InputPort object represents an input port of an Operator.

	\sa Port, OutputPort, Operator
 */
//*****************************************************************************
class SICPPSDKDECL InputPort : public Port
{
public:
	/*! Default constructor. */
	InputPort();

	/*! Default destructor. */
	~InputPort();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	InputPort(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	InputPort(const InputPort& in_obj);

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
	\return The new InputPort object.
	*/
	InputPort& operator=(const InputPort& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new InputPort object.
	*/
	InputPort& operator=(const CRef& in_ref);

	/*! Returns the current value of the data attached to the port. This function can only be called from within
	a custom operator's Update() callback; if called outside of a custom operator it returns an empty value.
	\return The current value.
	\return Empty if the port is not connected or if the function failed.
	*/
	CValue GetValue() const;

	/*! Returns true if the connection of target to port is optional, true otherwise.
	\return true if the port is optional
	\since 4.0
	*/
	bool GetOptional() const;

	private:
	InputPort * operator&() const;
	InputPort * operator&();
};


};

#endif // __XSIPORT_H__
