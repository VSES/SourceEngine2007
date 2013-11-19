//*****************************************************************************
/*!
   \file xsi_argument.h
   \brief Argument class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIARGUMENT_H__
#define __XSIARGUMENT_H__

#include <xsi_siobject.h>
#include <xsi_value.h>

namespace XSI {

class ArgumentArray ; // Forward declaration
class ArgumentHandler ;

//*****************************************************************************
/*! \class Argument xsi_argument.h
   \brief The Argument object represents an argument (or \a parameter) of an %XSI Command. For example the
   \xl DeleteObj command takes 1 argument (\c InputObj).

	Argument objects are used in three circumstances in %XSI:

	\par For defining command arguments at creation time
		When a command is created, arguments are added to the Command object using ArgumentArray::Add,
		ArgumentArray::AddWithHandler or ArgumentArray::AddObjectArgument.  The value
		of the argument (see Argument::PutValue) is treated as the default value for that argument.
		This default value is persisted and is used as the argument value in the case that no value
		is specified at invocation time.

	\par For specifying argument values when invoking a command.
		When a command is invoked, Argument::PutValue is used to specify the value of the
		argument that will be passed to the command.  Or, when not specifically set
		the value will be determined based on the default or the ArgumentHandler associated
		with the argument.  From the C++ API the most common way of invoking a command
		is with Application::ExecuteCommand, in which case it is not necessary to deal with this
		object at all.

	\par For re-defining command arguments after installation
		It is also possible to change the definition of a custom command
		after it has been installed.  In this case Argument::PutValue is
		used to specify new default values for the command, and it is possible to
		add and remove arguments.  Then Command::Update is called to
		commit the changes.  (This case does not apply to custom commands implemented
		inside a self-installed plug-in).

	For more information and an example see the documentation for the Command object.
	\since 4.0
 */
//*****************************************************************************
class SICPPSDKDECL Argument : public SIObject
{
public:
	/*! Default constructor. */
	Argument();

	/*! Default destructor. */
	~Argument();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Argument(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Argument(const Argument& in_obj);

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
	\return The new Argument object.
	*/
	Argument& operator=(const Argument& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Argument object.
	*/
	Argument& operator=(const CRef& in_ref);

	/*! Returns the value of the argument.  This would be the default value from the command definition unless
	some alternative value has been set by calling Argument::PutValue.
	\return The value of the argument.
	\since 4.0
	*/
	CValue GetValue() const ;

	/*! Sets the value of an argument. If called while the command is being defined, (for example in the
	\e commandname_Init callback of a self-installed plug-in) then this establishes the default value of the
	argument which is persisted as part of the command definition.

	When you use Command::Execute to invoke a command, you can provide an actual value for this argument, which
	overrides the default value, but does not affect the persisted command definition.

	\warning The Value is erased if Argument::PutHandler is called to set an ArgumentHandler after the Value has
		been already set.

	\return CStatus::OK
	\return CStatus::Fail
	\sa ArgumentArray::Add
	\since 4.0
	*/
	CStatus PutValue(const CValue&)  ;

	/*! Returns the argument flags.  These flags indicate how the argument values are passed to or from the Command.
	Arguments for custom commands always use ::siArgumentInput.
	\return The the argument flags (one of the ::siArgumentFlags values).
	\since 4.0
	*/
	siArgumentFlags GetFlags() const ;

	/*! Returns the ArgumentHandler, if any, of the Argument.  Call ArgumentHandler::IsValid on
	the returned object to test whether there really is an ArgumentHandler.
	\note  This function should not be confused with Command::GetHandler, which returns the name
		of a script function.
	\return The new ArgumentHandler object.
	\since 4.0
	*/
	ArgumentHandler GetHandler() const ;

	/*! Sets the ArgumentHandler of the argument based on the desired type.
	\warning Calling this function flushes any existing value associated with the Argument (see
		Argument::PutValue).
	\param in_strHandler String from the ::siArgumentHandler enum, for example \c Frame, \c SingleObj
		or \c Collection.
	\return CStatus::OK
	\return CStatus::Fail
	\sa ArgumentArray::AddWithHandler
	\since 4.0
	*/
	CStatus PutHandler( const CString & in_strHandler ) ;

	/*! Sets the ArgumentHandler of the argument by using an existing ArgumentHandler object.
	\param in_Handler An instance of an ArgumentHandler.  %Pass an uninitialized ArgumentHandler object
		to remove any existing ArgumentHandler.
	\return CStatus::OK
	\return CStatus::Fail
	\sa ArgumentArray::AddWithHandler
	\since 4.0
	*/
	CStatus PutHandler( ArgumentHandler & in_Handler ) ;

	private:
	Argument * operator&() const;
	Argument * operator&();
};

//*****************************************************************************
/*! \class ArgumentArray xsi_argument.h
	This object is owned by a Command object and holds a collection of Argument objects.

	Each Argument has a unique name, which is useful for documentation purposes and to faciliate the
	ArgumentArray::GetItem function. However it is the order of the arguments in the collection which
	determines how they are passed to the command implementation.  For commands implemented with the
	C++ API all the arguments are passed together in a CValueArray.

	Even though the number of arguments defined must exactly match those expected by the implementation,
	it is not always necessary to specify each argument when invoking a command.  In that case a default
	value is passed, based on the definition or the action of an ArgumentHandler.

	This object can be used in two different ways:

	\li To define the arguments of a new custom command. The definition of the arguments is normally
		done in the \e commandname_Init callback of a self-installed plug-in, as shown in the example
		in the Command object documentation.

	\li To specify specific argument values when a command is executed (see Command::Execute).

	\warning This specialized array is returned by Command::GetArguments, it is not meant to be created
		and modified in user-defined functions. If you want to add and remove arbitrary items to a
		collection, you must use a CRefArray instead.

	\since 4.0
 */
//*****************************************************************************
class SICPPSDKDECL ArgumentArray : public CBase
{
	public:
	/*! Default constructor. */
	ArgumentArray();

	/*! Default destructor. */
	~ArgumentArray();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ArgumentArray(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ArgumentArray(const ArgumentArray& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return True if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object.
	\param in_obj constant class object.
	\return The new ArgumentArray object.
	*/
	ArgumentArray& operator=(const ArgumentArray& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ArgumentArray object.
	*/
	ArgumentArray& operator=(const CRef& in_ref);

	/*! Returns an Argument object at a specified index in the array.
	\param in_index The index of the argument.
	\return The specified Argument object.
	\since 4.0
	*/
	Argument GetItem( LONG in_index ) const;

	/*! Returns an Argument object based on its name.
	\param in_name The name of the argument.
	\return The specified Argument object.
	\since 4.0
	*/
	Argument GetItem( const CString& in_name ) const;

	/*! Accessor to elements at a given index.
	\param in_index The index of the argument
	\return The specified Argument object.
	\since 4.0
	*/
	Argument operator[]( LONG in_index );

	/*! Accessor to elements at a given index. This function can only be called by constant objects,
	the returned value is read-only.
	\param in_index The index of the argument
	\return The read-only Argument object.
	\since 4.0
	*/
	const Argument operator[]( LONG in_index ) const;

	/*! Returns the number of Argument objects in the array.
	\return The number of Argument objects.
	*/
	LONG GetCount() const;

	/*! Adds an argument to the Command definition. To add an argument that receives an object call,
	use ArgumentArray::AddObjectArgument instead. To use an ArgumentHandler use ArgumentArray::AddWithHandler
	instead.

	This function is only available on custom commands. Normally this is called during the definition of
	a command (for example during the \e commandname_Init callback of a self-installed plug-in).

	\param in_strName A name for the argument. The name chosen is not particularly important
		as long as it is unique in the ArgumentArray. However having a good argument name helps
		users understand how to use the Command.
	\param in_dftValue The argument default value (optional). Specifying a default value
		also serves the purpose of specifying the specific CValue type that is expected
		(See CValue::DataType). For example if the default value is a double then %XSI will
		attempt to convert any input argument to double before invoking the command.
		When not specified the argument will be passed in directly without conversion.
	\return The new Argument object.
	\since 4.0
	*/
	Argument Add(
				const CString& in_strName,
				const CValue& in_dftValue = CValue() ) ;

	/*! Adds an Argument to the Command definition which will receive an %XSI object as its value when
	the Command is invoked. Because the argument uses the ::siArgHandlerSingleObj ArgumentHandler
	it automatically handles conversion of strings that represent objects in the scene.  It also
	automatically converts from a scripting (COM-based) object to a CRef-based C++ object.  See the example
	in the documentation for the Command object.

	To receive a CValueArray containing multiple objects, use ArgumentArray::AddWithHandler with
	::siArgHandlerCollection, or ::siArgHandlerMarkedParameters.

	\param in_strName A name for the argument. The name chosen is not particularly important
		as long as it is unique in the ArgumentArray. However having a good argument name helps
		users understand how to use the Command.
	\return The new Argument object.
	\since 4.0
	*/
	Argument AddObjectArgument(
				const CString& in_strName ) ;

	/*! Adds an argument to the command definition that uses an ArgumentHandler.
	\param in_strName Name of the new argument
	\param in_strHandlerName Name of the ArgumentHandler, see the ::siArgumentHandler enum for
		a complete list of supported handlers.
	\param in_Default Normally %Argument Handlers do not require a default value, but some
		adjust their behavior if a string is provided.  For example if the string \c * is provided
		in conjunction with the \c Collection %Argument Handler then all the objects in the scene
		are sent to the command rather than the current selection.  This behavior is documented
		with the ::siArgumentHandler enum.
	\return The new Argument object.
	\sa Argument, ArgumentHandler
	\since 4.0
	*/
	Argument AddWithHandler(
				const CString& in_strName,
				const CString& in_strHandlerName,
				const CString& in_Default = CString()) ;

	/*! Alternative version of ArgumentArray::AddWithHandler that takes an existing ArgumentHandler
	object rather than the string name of the %ArgumentHandler.
	\param in_strName Name of the new argument
	\param in_Handler ArgumentHandler object.
	\param in_Default Normally %Argument Handlers do not require a default value, but some
		adjust their behavior if a string is provided.  For example if the string \c * is provided
		in conjunction with the \c Collection %Argument Handler then all the objects in the scene
		are sent to the command rather than the current selection.  This behavior is documented
		with the ::siArgumentHandler enum.
	\return The new Argument object.
	\sa Argument, ArgumentHandler
	\since 4.0
	*/
	Argument AddWithHandler(
				const CString& in_strName,
				ArgumentHandler& in_Handler,
				const CString& in_Default = CString()) ;

	/*! Appends a copy of an argument. Because the name must be unique for each ArgumentArray, the
	Argument that is being copied must belong to a different command than the one that it is being
	added to.  This function would normally not be used for custom commands implemented as self-installed
	plug-ins.
	\param in_src Argument to copy
	\return The new Argument object.
	\since 4.0
	*/
	Argument AddCopy( const Argument & in_src ) ;

	/*! Removes all arguments from the collection.  This function would normally not be used for
	custom commands implemented as self-installed plug-ins.
	\return CStatus::OK
	\return CStatus::Fail
	\sa Command::Update
	\since 4.0
	*/
	CStatus RemoveAll() ;

	/*! Removes a single argument from the collection.  This function would normally not be used for
	custom commands implemented as self-installed plug-ins.
	\param in_index 0-based index
	\return CStatus::OK
	\return CStatus::Fail
	\sa Command::Update
	\since 4.0
	*/
	CStatus Remove( LONG in_index ) ;

	/*! Removes a single argument from the collection.  This function would normally not be used for
	custom commands implemented as self-installed plug-ins.
	\param in_strArgName Name of the Argument
	\return CStatus::OK
	\return CStatus::Fail
	\sa Command::Update
	\since 4.0
	*/
	CStatus Remove( const CString& in_strArgName ) ;
} ;

};

#endif
