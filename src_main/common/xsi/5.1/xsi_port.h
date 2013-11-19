//*****************************************************************************
/*!
   \file xsi_port.h
   \brief Port class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPORT_H__
#define __XSIPORT_H__

#include <xsi_siobject.h>

namespace XSI {

class ProjectItem;

//*****************************************************************************
/*! \class Port xsi_port.h
 	\brief A port is an input or output connection to another object.

 	Actually, a port is more like a connection point on an operator, which may or may not be actually
 	connected to something (some operator ports are optional).

	Use SIObject::GetName to get the name of this port.

	\sa Operator, OperatorContext, UpdateContext, SIObject, InputPort, OutputPort
 */
//*****************************************************************************

class SICPPSDKDECL Port : public SIObject
{
public:
	/*! Default constructor. */
	Port();

	/*! Default destructor. */
	~Port();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Port(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Port(const Port& in_obj);

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
	\return The new Port object.
	*/
	Port& operator=(const Port& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Port object.
	*/
	Port& operator=(const CRef& in_ref);

	/*! Returns true if the port is connected, false otherwise
	\return true if connected; false otherwise.
	*/
	bool IsConnected() const;

	/*! Returns the port index of this port as a <link idref="Long"/> within its port group or -1 if
	there is a failure.

	\note This index refers to the index of the port within the port group and is different from
		the index used to access the CRefArrays of InputPort or OutputPort references (available
		through Operator::GetInputPorts and Operator::GetOutputPorts).

	\note This index is rarely a concern when implementing Self-Installed Custom Operators, because
		OperatorContext::GetInputValue is based on the CRefArray index for the InputPort and normally
		there is only a single output port (see OperatorContext::GetOutputPort).

	\note When the Port is an output port, returned from a call to CustomProperty::AddOutputPort, this
		index may change if any input ports are added afterwards.
	\return LONG > 0 success
	\return -1 failure
	*/
	LONG GetIndex() const;

	/*! Returns the type of the port, input or output.
	\return The Port type.
	*/
	siPortType GetPortType() const;

	/*! Returns a reference object for the object connected to the port.
	If this function is called from the Update() context of a custom operator
	it will return an empty CRef.
	\retval out_pTarget the object to which the port is connected.
	\return A reference to the object connected to the port.
	\sa \xl Port_Target2
	*/
	CRef GetTarget(void) const;

	/*! Returns the index of the port group to which the port belongs.
	\return LONG > 0 success
	\return -1 means failure
	*/
	LONG GetGroupIndex(void) const;

	/*! Returns the name of the port group to which the port belongs.
	\return The name of the port group (empty string means failure)
	*/
	CString GetGroupName(void) const;

	/*! Returns the port group instance index for the port. There may be many
	objects of the same type connected to the same port group. Each object is
	connected to a port group instance, and within the port group instance there
	may be many ports.
	\return LONG > 0 success
	\return -1 means failure
	*/
	LONG GetGroupInstance(void) const;

	/*! Returns the mask of port flags described in ::siPortFlags.
	\return The mask containing port flags
	\since 4.0
	*/
	siPortFlags GetFlags(void) const;

	/*! Returns true if the port supports branch and group connections.
	\return true if the port supports branch and group connections.
	\since 4.0
	 */
	bool GetBranchGroup() const;

	/*! Returns a string containing the path to the target object.
	\return The path to the target object.
	\since 4.0
	 */
	CString GetTargetPath() const;

	private:
	Port * operator&() const;
	Port * operator&();


};

};

#endif // __XSIPORT_H__
