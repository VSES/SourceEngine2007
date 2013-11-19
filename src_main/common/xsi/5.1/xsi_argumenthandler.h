//*****************************************************************************
/*!
   \file xsi_argumenthandler.h
   \brief ArgumentHandler class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIARGUMENTHANDLER_H__
#define __XSIARGUMENTHANDLER_H__

#include <xsi_siobject.h>
#include <xsi_value.h>

namespace XSI {

//*****************************************************************************
/*! \class ArgumentHandler xsi_argumenthandler.h
   \brief An %argument handler is an object that helps determine the value that is passed to a Command.

	%Argument handlers come into play when a command is invoked without providing a explicit value for an
	argument, or when the argument that was provided requires some processing before it can be sent to the
	command.  For example the \c Collection argument handler can turn a string-based list of objects into
	a CValueArray representing those objects.

	Argument handlers are widely used on built-in commands, and can be specified for custom commands (see
	ArgumentArray::AddWithHandler). They make it easier to implement flexible custom commands, and reduce
	the amount of error handling necessary inside the command implementation.

	Once specified as part of the command definition they act behind the scenes during the process of the
	command invocation.  Neither the caller nor the implementation need to explicitly manipulate ArgumentHandler
	objects.

	\note Internally the state of an argument handler is stored in the value of the argument.
	\sa ArgumentArray::AddWithHandler, ArgumentArray::AddObjectArgument
	\since 4.0
 */
//*****************************************************************************
class SICPPSDKDECL ArgumentHandler : public SIObject
{
public:
	/*! Default constructor. */
	ArgumentHandler();

	/*! Default destructor. */
	~ArgumentHandler();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ArgumentHandler(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ArgumentHandler(const ArgumentHandler& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return True if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new ArgumentHandler object.
	*/
	ArgumentHandler& operator=(const ArgumentHandler& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ArgumentHandler object.
	*/
	ArgumentHandler& operator=(const CRef& in_ref);

	private:
	ArgumentHandler * operator&() const;
	ArgumentHandler * operator&();
};
};
#endif
