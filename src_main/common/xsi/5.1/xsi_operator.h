//*****************************************************************************
/*!
   \file xsi_operator.h
   \brief Operator class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIOPERATOR_H__
#define __XSIOPERATOR_H__

#include <xsi_projectitem.h>

namespace XSI {

class Port;

//*****************************************************************************
/*! \class Operator xsi_operator.h
	\brief The Operator object represents an operator in the scene graph. Objects or Parameters
	are connected to the operator via InputPorts or OutputPorts. These ports are organized into
	port groups. The port groups contain all ports that will be connected to parts of the selected
	or picked object. For example, when the \xl Twist_op operator is applied to a selected object the
	operator has ports that read from the object's local KinematicState and Geometry and write to
	the result object's geometry.

	You can determine the number of port groups defined by an operator using Operator::GetNumPortGroups
	and determine of the number of ports in a group using Operator::GetNumPortsInGroup. To access
	individual ports, use either Operator::GetPort property or Operator::GetPortAt function.

	Multiple objects connected to the same port group for example, the loft operator may read from
	many curves to generate the resulting mesh. Each of these input curves are connected to the same
	port group and each connection is called a port group instance. You can determine the number of
	objects connected to a port group by using Operator::GetNumInstancesInGroup.

	From the port you can determine which port group a port belongs to using Port::GetGroupName or
	Port::GetGroupIndex. You can determine which port group instance a port belongs to using
	Port.GroupInstance.

	Port::GetIndex refers to the index of the port within the port group and is different from the
	index used to access the list of InputPorts or OutputPorts (available through the Operator::GetInputPorts
	and Operator::GetOutputPorts).

	Almost all Operators expose one or more Parameters which control the behavior of the operator. For
	example, the "subdivu" and "subdivv" parameters on a polygon mesh geometry operator control the number
	of polygons that a polygon mesh is generated with.

	The most common way to create and connect an operator is by calling the \xl ApplyOp command. However
	there are many specialized commands for creating operators, for example \xl ApplyTopoOp, \xl ApplyHairOp
	and \xl ApplyGenOp. Operators are also often created indirectly, for example calling X3DObject::AddGeometry
	creates an operator that generates the geometry.

	Selecting an operator in the "RV - SDK Explorer" view is a good way to learn more about its ports and parameters.

	\sa UpdateContext, Port, InputPort, OutputPort

	\eg Illustrates how to generate a mesh by using the loft operator and a number of input curves. The
	code also illustrates how to traverse the operator's port group, port group instances and ports and
	logs the name of the port, its type and the full path name of the object connected to the port.

	\code
		using namespace XSI;
		Application app;

		Model root = app.GetActiveSceneRoot();

		CValueArray args(2);
		CValue outArg;

		// Create an arc
		args[0] = CString(L"Arc");
		args[1] = CString(L"NurbsCurve");
		app.ExecuteCommand( L"CreatePrim", args, outArg );

		// Get arc from return value
		X3DObject arc(outArg);

		// Duplicate arc 4 times and translate in y
		args.Resize(19);
		args[0] = arc;					// source object
		args[1] = (LONG)4;				// number of copies
		args[9] = (LONG)siApplyRepeatXForm;	// Xform
		args[18] = (double)1;			// Ty
		app.ExecuteCommand( L"Duplicate", args, outArg );

		// Get array of duplicated arcs from return value
		CValueArray objs(outArg);

		// Create array containing arc and duplicates
		objs[0] = CValue(arc);

		// Apply loft operator
		args.Resize(2);
		args[0] = CString(L"Loft");
		args[1] = objs;
		app.ExecuteCommand( L"ApplyOp", args, outArg );

		// Get create operator from return value
		Operator op = ((const CValueArray&)outArg)[0];

		// Traverse port group, instances and ports and
		// log port connections

		// For each port group
		for ( LONG idxGroup = 0; idxGroup < op.GetNumPortGroups(); idxGroup++ )
		{
			// for each instance in a port group
			for ( LONG idxInstance = 0; idxInstance < op.GetNumInstancesInGroup( idxGroup ); idxInstance++ )
			{
				// for each port in a  port group instance
				for ( LONG idxPort = 0; idxPort < op.GetNumPortsInGroup( idxGroup ); idxPort++ )
				{
					// get a specific port
					Port port = op.GetPortAt( idxPort, idxGroup, idxInstance );

					// if the port is an input port
					CString porttypestr;

					if ( port.GetPortType() == siPortInput )
						porttypestr = L"input";
					else if ( port.GetPortType() == siPortOutput )
						porttypestr = L"output";
					else
						porttypestr = L"error";

					SIObject target = port.GetTarget();

					// log details of port connection to target
					CString msgstr;

					msgstr += op.GetName();
					msgstr += L" group:";
					msgstr += CValue(port.GetGroupIndex()).GetAsText();
					msgstr += L" instance:";
					msgstr += CValue(port.GetGroupInstance()).GetAsText();
					msgstr += L" ";
					msgstr += porttypestr;
					msgstr += L"port:";
					msgstr += CValue(port.GetIndex()).GetAsText();
					msgstr += L":";
					msgstr += target.GetFullName();

					app.LogMessage( msgstr );
				}
			}
		}
		// 'INFO : "Loft group:0 instance:0 inputport:0:arc20.crvlist"
		// 'INFO : "Loft group:0 instance:0 inputport:1:arc20.kine.global"
		// 'INFO : "Loft group:0 instance:1 inputport:0:arc21.crvlist"
		// 'INFO : "Loft group:0 instance:1 inputport:1:arc21.kine.global"
		// 'INFO : "Loft group:0 instance:2 inputport:0:arc22.crvlist"
		// 'INFO : "Loft group:0 instance:2 inputport:1:arc22.kine.global"
		// 'INFO : "Loft group:0 instance:3 inputport:0:arc23.crvlist"
		// 'INFO : "Loft group:0 instance:3 inputport:1:arc23.kine.global"
		// 'INFO : "Loft group:0 instance:4 inputport:0:arc24.crvlist"
		// 'INFO : "Loft group:0 instance:4 inputport:1:arc24.kine.global"
		// 'INFO : "Loft group:1 instance:0 inputport:0:surfmsh.kine.global"
		// 'INFO : "Loft group:1 instance:0 outputport:1:surfmsh.surfmsh"
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Operator : public ProjectItem
{
public:
	/*! Default constructor. */
	Operator();

	/*! Default destructor. */
	~Operator();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Operator(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Operator(const Operator& in_obj);

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
	\return The new Operator object.
	*/
	Operator& operator=(const Operator& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Operator object.
	*/
	Operator& operator=(const CRef& in_ref);

	/*! Returns an individual Port object. The port group name and index arguments are optional.
	If both arguments are not provided, the first port with the given name will be returned.
 	\param in_portname port name.
	\param in_groupname port group name.
	\param in_instance  port group instance index.
	\return The Port object.
	 */
	Port GetPort
	(
		const CString& in_portname,
		const CString& in_groupname,
		LONG in_instance = 0
	) const;

	/*! Returns the specified Port object.
	\note Regardless of the order that ports are added to a PortGroup, input ports are always indexed
		before output ports. For example, if a custom operator is built with two output ports A and B
		and two input ports C and D then port 0 will be C, port 1 will be D, port 2 will be A and
		port 3 will be B.
 	\param in_port	Index of the port within the group
	\param in_group %PortGroup index.
	\param in_instance  %PortGroup instance index.
	\return The Port object.
	*/
	Port GetPortAt
	(
		LONG in_port,
		LONG in_group = 0,
		LONG in_instance = 0
	) const;

	/*! Returns an array of InputPort reference objects.
	\return Array of Port reference objects.
	*/
	CRefArray GetInputPorts() const;

	/*! Returns an array of OutputPort reference objects. If this function is accessed from within
	the scope of an operator update then an empty array be returned.
	\return Array of Port reference objects.
	*/
	CRefArray GetOutputPorts() const;

	/*! Returns the number of port groups.
	\return LONG > 0		success
	\return -1			failure
	*/
	LONG GetNumPortGroups() const;

	/*! Returns the number of ports in port group.
	\param in_index  port group index.
	\return LONG > 0		success
	\return -1			failure
	*/
	LONG GetNumPortsInGroup(LONG in_index) const;

	/*! Returns the number of port group instances in port group.
	\param in_index  port group index.
	\return LONG > 0		success
	\return -1			failure
	*/
	LONG GetNumInstancesInGroup(LONG in_index) const;

	/*! Returns true if the operator is muted.
	\return true	if the operator is muted; false	otherwise.
	\since 4.0
	*/
	bool GetMute(void) const;

	/*! Sets the muted state of the operator.  Not all %XSI operators can be muted, but all Custom
	Operators support this function.
	\param in_mute		mute state, pass true to mute.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 4.0
	*/
	CStatus PutMute(bool in_mute);

	/*! Disconnects all ports. This function is not undoable.
	\param in_force	Force disconnection of operator even if connected to created output objects
		( \b Not \b Implemented - will always delete )
	\return CStatus::OK success
	\return CStatus::False operator doesn't support muting
	\return CStatus::Fail failure
	\since 4.0
	*/
	CStatus Disconnect(bool in_force);

	/*! Connects operator ports using objects specified through calls to Operator::AddInputPort and
	Operator::AddOutputPort.
	\param in_mode	Specifies the construction mode in which the operator will be applied. This only
		applies to output connections made to Geometry objects, the mode will be ignored for
		all other types of connections.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 4.0
	*/
	CStatus Connect(siConstructionMode in_mode = siConstructionModeDefault);

	/*! Connect operators using object specified. The object will be matched to the port group.
	\param  in_obj object to connect.
	\param in_mode	Specifies the construction mode in which the operator will be applied. This only
		applies to output connections made to Geometry objects, the mode will be ignored for
		all other types of connections.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 4.0
	*/
	CStatus Connect(const CRef& in_obj,siConstructionMode in_mode = siConstructionModeDefault);

	/*! Connects operator using objects specified. The objects will be matched to the port groups.
	\param  in_objs objects to connect.
	\param in_mode	Specifies the construction mode in which the operator will be applied. This only
		applies to output connections made to Geometry objects, the mode will be ignored for
		all other types of connections.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 4.0
	*/
	CStatus Connect(const CRefArray& in_objs, siConstructionMode in_mode = siConstructionModeDefault);

	/*! Connects operator using the specified \xl StringExpression. The string expression must be a
	valid connection set specification.
	\param  in_objs objects to connect.
	\param in_mode	Specifies the construction mode in which the operator will be applied. This only
		applies to output connections made to Geometry objects, the mode will be ignored for all
		other types of connections.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 4.0
	*/
	CStatus Connect(const CString& in_objs,siConstructionMode in_mode = siConstructionModeDefault);

	/*! Returns an array of PortGroup reference objects.
	\return Array of PortGroup reference objects.
	\since 4.0
	 */
	CRefArray GetPortGroups() const;

	/*! Returns true if the operator is connected to at least 1 output port.
	\return true if operator is connected.
	\since 4.0
	 */
	bool IsConnected() const;

	/*! Returns true if the operator has ports that support branch and group connections.
	\param  group index of group to check, pass group==-1 to check all groups and port==-1 to check
	\param  port index of port to check, pass port==-1 to check all port within the specified group.
	\return true if the operator has ports that support branch and group connections.
	\since 4.0
	 */
	bool SupportsBranchGroup(LONG group = -1, LONG port = -1 ) const;

	/*! Connects an object to a new instance of a PortGroup. This function is useful for dynamic
	operators, where optional or multi-instance connections are made after the object is already connected.
	\note This function only works for Self-Installed Custom Operators if there is a single Port inside the
		port group. This limitation exists because you can only pass a single object in the in_obj argument.
	\param  in_Group Index of the port group.
	\param  in_obj The object to connect to the new port group instance
	\retval  out_Instance Returns the index of new port group instance.
	\return CStatus::OK if successful, and CStatus::Fail otherwise.
	\since Gemini
	*/

	CStatus ConnectToGroup( LONG in_Group, CRef & in_obj, LONG & out_Instance );

	/*! Removes a port group instance from a port group.
	\param  in_Group Index of the port group.
	\param  in_Instance Index of the instance to remove
	\param  in_Force Not implemented. True to disconnect the object even if the port is not optional.
	\return Returns CStatus::OK if successful, and CStatus::Fail otherwise.
	\since Gemini
	*/
	CStatus DisconnectGroup( LONG in_Group, LONG in_Instance, bool in_Force = false );

	/*! Returns the value of an input port specified by index.
	\note For Self-Installed Custom Operators the OperatorContext::GetInputValue function is often more convenient.
	\param in_port	Port index.
	\param in_group PortGroup index.
	\param in_instance  %PortGroup instance index.
	\return Value of the %Port.
	\since Gemini
	*/
	CValue GetInputValue(	LONG in_port,
							LONG in_group = 0,
							LONG in_instance = 0 ) const;

	/*! Returns the value of an input port specified by name.
	\note For Self-Installed Custom Operators the OperatorContext::GetInputValue function is often more convenient.
 	\param in_port	Port name.
	\param in_group PortGroup name.
	\param in_instance  %PortGroup instance index.
	\return Value of the %Port.
	\since Gemini
	*/
	CValue GetInputValue(	const CString&	in_port,
							const CString&	in_group = CString(),
							LONG			in_instance = 0 ) const;

	private:
	Operator * operator&() const;
	Operator * operator&();

};

};

#endif // __XSIOPERATOR_H__
