//*****************************************************************************
/*!
   \file xsi_customoperator.h
   \brief CustomOperator class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICUSTOMOPERATOR_H__
#define __XSICUSTOMOPERATOR_H__

#include <xsi_operator.h>

namespace XSI {

class ParamDef;
class PortGroup;

//*****************************************************************************
/*! \class CustomOperator xsi_customoperator.h
	\brief The CustomOperator object represents a custom operator (ie., a scripted or compiled operator) in the scene graph.

	\sa OperatorContext, PortGroup, Port, InputPort, OutputPort
	\since 4.0
 */
//*****************************************************************************

class SICPPSDKDECL CustomOperator : public Operator
{
public:
	/*! Default constructor. */
	CustomOperator();

	/*! Default destructor. */
	~CustomOperator();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	CustomOperator(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	CustomOperator(const CustomOperator& in_obj);

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
	\return The new CustomOperator object.
	*/
	CustomOperator& operator=(const CustomOperator& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new CustomOperator object.
	*/
	CustomOperator& operator=(const CRef& in_ref);

	/*! Returns the filename of the script or module containing the implementation of the custom operator.
	\return The filename of the implementation script or module.
	*/
	CString GetFileName(void) const;

	/*! Sets the filename of the script or module containing the implementation of the custom operator.
	\param in_str The filename of the implementation script or module in a CString.
	\return CStatus
	*/
	CStatus PutFileName(const CString& in_str);

	/*! Returns the API type used by the implementation of the custom operator.
	\return The implementation language: \c COM, \c CPP, \c VBScript, \c JScript, \c PerlScript or \c Python
	*/
	CString GetLanguage(void) const;

	/*! Returns the script code used by the custom operator implementation.
	\note If the custom operator is compiled then an empty string is returned.
	\return The implementation script code.
	\return Empty string for compiled operators
	*/
	CString GetCode(void) const;

	/*! Sets the script code used by the custom operator implementation.
	\note If the custom operator is compiled then CStatus::Fail is returned.
	\param  code A string containing the script code.
	\return CStatus::OK
	\return CStatus::Fail for compiled operators
	*/
	CStatus PutCode(const CString& code);

	/*! Returns non zero value if the custom operator is in debug mode.
	 * \return true for debug mode; false otherwise.
	 */
	LONG GetDebug(void) const;

	/*! Sets the debug mode of the operator. Pass a non zero value to set the operator in Debug mode. The operator
	list sends a message to the history log each time request data, notify and update are called.
	\param debug \c 0 to disable messages; anything other LONG to enable them
	\return CStatus::OK
	\return CStatus::Fail
	*/
	CStatus PutDebug(LONG debug);

	/*! Returns true if the custom operator will call \c Update on each notify.
	\return True if the custom operator will always call update on each notify.
	*/
	bool GetAlwaysEvaluate(void) const;

	/*! Sets the operator to call \c Update on each time change.
	\param  val  True to force the operator to evaluate on each time change.
	\return CStatus::OK
	\return CStatus::Fail
	*/
	CStatus PutAlwaysEvaluate(bool val);

	/*! Adds a new custom parameter to the custom operator object.
	\param  in_paramdef The parameter definition (created via Factory::CreateParamDef)
	\param  io_parameter Returns the new Parameter object.
	\return CStatus::OK success
	\return CStatus::Fail failure

	*/
	CStatus AddParameter(
		const CRef& in_paramdef,
		Parameter&	io_parameter );

	/*! Removes the specified custom parameter from the custom operator.
	\param  in_param Parameter to remove.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus RemoveParameter( const Parameter& in_param );

	/*! Adds an input port to the operator. The port is assumed to be return. The group will be automatically assigned if not specified.
	\param  obj The object to be connected to the port
	\param  name Name for the port.
	\param  group The index of the group to add port.
	\param  insertat Not implemented
	\param  flags A mask of input port flags described by ::siOptionalInputPort (see ::siPortFlags).
	\retval pst The returned status code (CStatus::OK for success).
	\return A reference to the new InputPort.
	*/
	CRef AddInputPort( const CRef& obj, const CString& name=CString(), LONG group=-1, LONG insertat=-1, LONG flags=0, CStatus* pst = 0 );

	/*! Adds an output port to the operator. The port is assumed to be return. The group will be automatically assigned if not specified.
	\param  obj The object to be connected to the port
	\param  name Name for the port.
	\param  group The index of the group to add port.
	\param  insertat Not implemented
	\param  flags A mask of output port flags described by ::siCreatedOutputPort (see ::siPortFlags).
	\retval pst The returned status code (CStatus::OK for success).
	\return A reference to the new InputPort.
	*/
	CRef AddOutputPort( const CRef& obj, const CString& name=CString(), LONG group=-1, LONG insertat=-1, LONG flags=0, CStatus* pst = 0 );

	/*! Adds an input/output port to the operator. The port is assumed to be return. The group will be automatically assigned if not specified.
	\param  obj The object to be connected to the port
	\param  name Name for the port. The output port name is prefixed with Out and the input port with In.
	\param  group The index of the group to add port.
	\param  insertat Not implemented
	\param  flags A mask of port flags described by ::siOptionalInputPort + ::siCreatedOutputPort (see ::siPortFlags).
	\retval pst The returned status code (CStatus::OK for success).
	\return Array of references to the newly created input and output ports.
	*/
	CRefArray AddIOPort( const CRef& obj, const CString& name=CString(), LONG group=-1, LONG insertat=-1, LONG flags=0, CStatus* pst = 0 );

	/*! Adds a port group to operator. If the operator has branch group support then the filter must also match the group as well as the
	allowed components; otherwise groups will be rejected when Operator::Connect is called.

	\param  name Name of the port group.
	\param  groupmin  Minimum number of objects required for a connection.
	\param  groupmax  Maximum number of objects allowed to be connected.
	\param  filterid The filter string id, this is the same name used to look up filters.
	\param  pickprompt String to show user during picking session
	\param  flags Port flags (see ::siPortFlags).
	\retval pst The returned status code (CStatus::OK for success).
	\return A reference to the new PortGroup.
	*/
	CRef AddPortGroup( const CString& name, LONG groupmin=1, LONG groupmax=1, const CString& filterid=CString(), const CString& pickprompt=CString(), LONG flags=0, CStatus* pst = 0 );

	/*! Tests whether the operator contains a valid port definition and syntactically correct code.
	Note: This method is not implemented for Self-Installed Custom Operators.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Validate();

	/*!  Adds an input port to the operator. This function makes it possible to define a connection to a custom operator by specifying
	the type of object it needs to connect to, rather than providing an existing object.

	This is for use in advanced operators, where the port may be optional and dynamically connected after the Operator is instantiated.
	Each port defined in this way must be in its own PortGroup so that the connection can be established through Operator::ConnectToGroup.

	For example, if an operator supports an optional connection to a ClusterProperty, the CustomOperator::AddInputPort
	function forces the existence of a %ClusterProperty at the time of the operator definition, even if the operator is not meant to
	connect to a %ClusterProperty until some later time.  This function avoids the need for any ClusterProperty to actually exist in
	the scene.

	\param in_TargetClassID The type of object that can connect to this object, for example siKinematicStateID, siClusterPropertyID,
		siParameterID or siPrimitiveID.
	\param in_strPortName The name of the port.  If empty a name is automatically generated.
	\param in_PortGroup Index of the port group that will contain the new port
	\param in_InsertAt Not implemented
	\param in_flags Mask of port group flags from the ::siPortFlags enum
	\return A reference to the new InputPort
	\since Gemini
	*/
	CRef AddInputPortByClassID(	siClassID in_TargetClassID,
								const CString& in_strPortName=CString(),
								LONG in_PortGroup=-1,
								LONG in_InsertAt=0,
								LONG in_flags=0 );

	/*! Adds an OutputPort to the custom operator. This function makes it possible to define a connection to a custom operator by
	specifying the type of object it will connect to, rather than providing an actual existing object as is the case with
	CustomOperator::AddOutputPort. This is for use in advanced operators, where the port may be optional and dynamically connected
	after the operator is instantiated.

	\note Use of this function is rarely necessary because an operator must connect to at least one output at the time of creation,
		and normally only has only one output.

	\param in_TargetClassID The type of object that can connect to this object, for example siKinematicStateID, siClusterPropertyID,
		siParameterID or siPrimitiveID.
	\param in_strPortName The name of the port.  If empty a name is automatically generated.
	\param in_PortGroup Index of the port group that will contain the new port
	\param in_InsertAt Not implemented
	\param in_flags Mask of port group flags from the ::siPortFlags enum
	\return A reference to the new OutputPort
	\since Gemini
	*/
	CRef AddOutputPortByClassID(siClassID in_TargetClassID,
								const CString& in_strPortName=CString(),
								LONG in_PortGroup=-1,
								LONG in_InsertAt=0,
								LONG in_flags=0 );

	/*! Adds an InputPort and an OutputPort to the custom operator. This function makes it possible to define a connection to a custom
	operator by specifying the type of object it needs to connect to, rather than providing an actual existing object as is the case
	with CustomOperator::AddIOPort. This is for use in advanced operators, where the port may be optional and dynamically connected
	after the operator is instantiated.

	\param in_TargetClassID The type of object that can connect to this object, for example siKinematicStateID, siClusterPropertyID,
		siParameterID or siPrimitiveID.
	\param in_strPortName The name of the port.  If empty a name is automatically generated.
	\param in_PortGroup Index of the port group that will contain the new port
	\param in_InsertAt Not implemented
	\param in_flags Mask of port group flags from the ::siPortFlags enum
	\return An array of references to the new InputPort and OutputPort
	\since Gemini
	*/
	CRefArray AddIOPortByClassID(siClassID in_TargetClassID,
								const CString& in_strPortName=CString(),
								LONG in_PortGroup=-1,
								LONG in_InsertAt=0,
								LONG in_flags=0 );


	private:
	CustomOperator * operator&() const;
	CustomOperator * operator&();

};

};

#endif // __XSICUSTOMOPERATOR_H__
