//*****************************************************************************
/*!
   \file xsi_operatorcontext.h
   \brief OperatorContext class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __OPERATORCONTEXT_H__
#define __OPERATORCONTEXT_H__

#include <xsi_base.h>
#include <xsi_value.h>
#include <xsi_time.h>
#include <xsi_context.h>

namespace XSI {

//*****************************************************************************
/*! \class OperatorContext xsi_operatorcontext.h
	\brief This class is provided as the context object in the %Operator update
	callback for a Self-Installed CustomOperator object (see PluginRegistrar::RegisterOperator).

	This object is offers all the context information necessary to implement a normal operator.
	However for advanced operators it is possible to retrieve more information
	via the CustomOperator object, which is available through Context::GetSource.

	\note The UpdateContext object continues to be used for legacy C++ Custom Operators
		based on a Preset and SPDL.

	The Context::GetUserData and Context::PutUserData functions provide a convenient way
	to store non-persisted, per-instance state information for the operator.  A common pattern
	is to use user data to store a pointer to C++ objects that actually implement the operator
	algorithm and store the operator's state.

	\sa UpdateContext, CustomOperator, Operator, PluginRegistrar
	\since Gemini
*/
//*****************************************************************************

class SICPPSDKDECL OperatorContext : public Context
{
public:

	/*! Default constructor. */
	OperatorContext();

	/*! Default destructor. */
	~OperatorContext();

	/*! Constructor.
	\param in_ref Constant reference object.
	*/
	OperatorContext(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj Constant class object.
	*/
	OperatorContext(const OperatorContext& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID Class type.
	\return True if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object.
	\param in_obj Constant class object.
	\return The new OperatorContext object.
	*/
	OperatorContext& operator=(const OperatorContext& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref Constant class object.
	\return The new OperatorContext object.
	*/
	OperatorContext& operator=(const CRef& in_ref);

	/*! Returns the target object connected to the OutputPort. The only state of the %XSI scene
	that an operator should modify during its evaluation is via the functions available on the
	object returned by this function. For example, if an operator writes to a ClusterProperty
	(for example, a Weight Map) then this function will return the %ClusterProperty and the
	operator code can set the values of the property via CClusterPropertyElementArray::PutArray.

	In the case of a multi-output operator the operator is called once per output, and this function
	will return the currently evaluating target.  In this case it may be necessary to also call
	OperatorContext::GetOutputPort to determine the name of the current output port, in order to
	know which output is being evaluated.

	\note If the operator is writing to a Parameter then use OperatorContext::GetOutputPort instead
		of this function.

	\return A reference to the %XSI object being written
	\sa CustomOperator::AddOutputPort, OperatorContext::GetOutputPort
	*/
	CRef GetOutputTarget() const;

	/*! Returns the OutputPort that represents the connection to the currently evaluating output
	target. In the case of a multi-output operator this object can be used to determine which output
	is currently being evaluated.

	When the operator is writing to a Parameter use the OutputPort::PutValue function on the returned
	OutputPort to set a new value on the parameter.

	\return OutputPort
	*/
	CRef GetOutputPort() const;

	/*! Returns the evaluation time.  An operator should not assume that the evaluation time is the
	same as the current scene time or that it is evaluated just once per frame.
	\return CTime Evaluation time
	*/
	CTime GetTime() const;

	/*! Returns the value of a given InputPort on the CustomOperator by index. If the connected object
	is a Parameter this will return the actual value of the %Parameter value, not the Parameter object.
	Otherwise it will return the object that represents the input object, and this object can be used
	to retrieve information about the state of the input object. For example if the operator is connected
	to a %Parameter of type double this will return a double. And if the operator is connected to a
	%Primitive this will return a Primitive.

	\note This function is a convenient and fast approach that hides the existence of InputPort objects.

 	\param in_port	Port index.  If the operator was created by a function like Parameter::AddCustomOp then
 		the index is easy to determine based on the order of the inputs in the input argument.  If the
 		operator was built manually then the index can be determined based on the order of calls to
 		CustomOperator::AddInputPort or CustomOperator::AddIOPort
	\param in_group PortGroup index.  Use this parameter for advanced operators that have more than one
		PortGroup.  Otherwise it can be assumed that the input is on the first group (group 0)
	\param in_instance  %PortGroup instance index. Use this parameter for advanced operators that support
		having more than one object connecting to a particular PortGroup.  Otherwise it is assumed that the
		input is on the first group instance.
	\return A CValue containing a reference to an %XSI Object or the value of a Parameter.
	\sa Operator::GetInputValue, Parameter::GetValue
	*/
	CValue GetInputValue(	LONG in_port,
							LONG in_group = 0,
							LONG in_instance = 0 ) const;

	/*! Returns the value of a given input port by name. This variation works based on the string names of
	the InputPort and PortGroup objects.
	\warning This function is slower the other signature, which works based on numeric index.

 	\param in_port	Port name.
	\param in_group PortGroup name.
	\param in_instance  %PortGroup instance index.
	\return A CValue containing a reference to an %XSI Object or the value of a Parameter.
	\sa Operator::GetInputValue, Parameter::GetValue
	*/
	CValue GetInputValue(	const CString&	in_port,
							const CString&	in_group = CString(),
							LONG			in_instance = 0 ) const;

	/*! Returns the current value of the specified Parameter, if any, of the CustomOperator. This function
	provides access only to parameters defined for the CustomOperator. For example, an Operator often has
	parameters that allow the user or the plug-in to configure the operator's behavior, such as "RecursionDepth",
	"UDivisions", "Height" or "CenterX".

	\note This function cannot be used to retrieve the value of Parameters on any other object in the scene.
	For that scenario the operator should connect to the Parameter (or object that owns the parameter) and use
	OperatorContext::GetInputValue to retrieve the %Parameter value.

	\param in_name scripting name of the parameter for which we want to get the value.
	\return The current value of the Parameter (boolean, double, string or other supported type)
	\return If the parameter is unable to retrieve the value, it will return an empty CValue.
	\return If the parameter value is a string and it has not been set then it will return an empty string.
	\sa ProjectItem::GetParameterValue, Parameter::GetValue
	*/
	CValue GetParameterValue( const CString& in_name )const;

	private:
	OperatorContext * operator&() const;
	OperatorContext * operator&();
};

};
#endif // __OPERATORCONTEXT_H__
