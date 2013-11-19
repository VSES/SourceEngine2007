//*****************************************************************************
/*!
   \file xsi_portgroup.h
   \brief PortGroup class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPORTGROUP_H__
#define __XSIPORTGROUP_H__

#include <xsi_siobject.h>

namespace XSI {

class ProjectItem;

//*****************************************************************************
/*! \class PortGroup xsi_portgroup.h
 	\brief A portgroup is a group of Port objects on an Operator. The port group is a logical grouping of multiple
 	port connections that are normally scoped by an X3DObject. A single portgroup can contain both InputPort and
 	OutputPort objects.

 	\note The %PortGroup object should not be confused with the Group object.

	You can determine the number of port groups defined by an operator using the Operator::GetNumPortGroups function
	and determine the number of ports in a %PortGroup using the Operator::GetNumPortsInGroup function.

	For built-in Operators the port groups contain all ports that will be connected to parts of the selected or
	picked object. For example, when the Twist operator is applied to a selected object, the operator has ports that
	read from the object's local KinematicState and Geometry and write to the result object's geometry.

	However, for typical Self-Installed Custom Operators, all outputs and inputs can be in a single PortGroup because
	the individual targets for each port are specifically provided at the time of calls to CustomOperator::AddInputPort
	or the \xl %AddCustomOp command.  In fact, the concept of PortGroup can be completely ignored for most custom
	operators.  However any dynamic input inside an advanced operators should be in its own PortGroup to permit usage
	of Operator::ConnectToGroup.

	Multiple objects can connect to the same port group, for example, the loft operator may read from many curves to
	generate the resulting mesh. Each of these input curves are connected to the same port group and each connection
	is called a port group instance. You can determine the number of objects connected to a port group by using
	Operator::GetNumInstancesInGroup or PortGroup::GetInstanceCount.

	Use SIObject::GetParent to get the Operator for this PortGroup and SIObject::GetName to get the name of this port
	group.

	From the port you can determine which port group a port belongs to using the Port::GetGroupName or Port::GetGroupIndex
	functions. You can determine which port group instance a port belongs to using the Port::GetGroupInstance function.

	\sa Operator, OperatorContext, SIObject, InputPort, OutputPort
	\since 4.0
 */
//*****************************************************************************

class SICPPSDKDECL PortGroup : public SIObject
{
public:
	/*! Default constructor. */
	PortGroup();

	/*! Default destructor. */
	~PortGroup();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PortGroup(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PortGroup(const PortGroup& in_obj);

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
	\return The new PortGroup object.
	*/
	PortGroup& operator=(const PortGroup& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PortGroup object.
	*/
	PortGroup& operator=(const CRef& in_ref);

	/*! Returns true if all the ports are optional
	\param in_port index of port to test; otherwise all ports are checked.
	\return true if connected.
	*/
	bool IsOptional(LONG in_port=-1) const;

	/*! Returns the port index in port group.
	\return LONG > 0 success
	\return -1 failure
	*/
	LONG GetIndex() const;

	/*! Returns the minimum number of objects required for a connection
	\return LONG > 0 success
	\return -1 failure
	*/
	LONG GetMin() const;

	/*! Returns the maximum number of objects allowed to be connected.
	\return LONG > 0 success
	\return -1 failure
	*/
	LONG GetMax() const;

	/*! Returns the flags used to determine which is the main group, created group, secondary group, optional group.
	\return LONG > 0 success
	\return -1 failure
	*/
	LONG GetFlags() const;

	/*! Returns the pick prompt string, this is the string displayed in the status bar when a picking session is
	started to pick objects to be connected to the operator.
	\return The pick prompt string (empty on failure or if no pick string was specified).
	*/
	CString GetPickPrompt() const;

	/*! Returns the filter string id, this is the same name used to look up filters .
	\return The filter name (empty on failure or if no pick string was specified).
	*/
	CString GetFilter() const;

	/*! Returns a collection of all ports defined for this group .
	\return The collection of Port objects (empty on failure or objects connected).
	*/
	CRefArray GetPorts() const;

	/*! Returns the number of object connected to this group.
	\return The number of object connected to this group.
	*/
	LONG GetInstanceCount() const;

	/*! Returns true if the port group has ports that support branch and group connections.
	\param in_port Index of port to test; otherwise all ports are checked.
	\return true if the port group has ports that support branch and group connections.
	 */
	bool SupportsBranchGroup(LONG in_port=-1) const;

	/*! Adds input port to operator port group. The port is assumed to be mandatory.
	\deprecated Gemini This method is obsolete and must not be used. Use CustomOperator::AddInputPort instead.	
	\param  target Object to be connected to the port.
	\param  name Port name.
	\param  insertat %Port index within group.
	\param  flags Mask of input port flags described by ::siPortFlags.
	\retval pst The returned status code, returns CStatus::OK for success.
	\return A reference to the new InputPort.
	*/
	CRef AddInputPort( const CRef& target, const CString& name=CString(), LONG insertat=-1, LONG flags=0, CStatus* pst = 0 );

	/*! Adds output port to operator port group. The port is assumed to be mandatory.
	\deprecated Gemini This method is obsolete and must not be used. Use CustomOperator::AddOutputPort instead.
	\param  target Object to be connected to the port.
	\param  name Port name.
	\param  insertat %Port index within group.
	\param  flags Mask of output port flags described by ::siPortFlags.
	\retval pst The returned status code, returns CStatus::OK for success.
	\return A reference to the new OutputPort.
	*/
	CRef AddOutputPort( const CRef& target, const CString& name=CString(), LONG insertat=-1, LONG flags=0, CStatus* pst = 0 );

	/*! Adds output port to operator port group. The port is assumed to be mandatory.
	\deprecated Gemini This method is obsolete and must not be used. Use CustomOperator::AddIOPort instead.
	\param  target Object to be connected to the port.
	\param  name Port name.
	\param  insertat %Port index within group.
	\param  flags Mask of port flags described by ::siPortFlags.
	\retval pst The returned status code, returns CStatus::OK for success.
	\return Array of references to the new input and output ports.
	*/
	CRefArray AddIOPort( const CRef& target, const CString& name=CString(), LONG insertat=-1, LONG flags=0, CStatus* pst = 0 );

	private:
	PortGroup * operator&() const;
	PortGroup * operator&();


};

};

#endif // __XSIPORTGROUP_H__
