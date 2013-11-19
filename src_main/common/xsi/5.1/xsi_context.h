//*****************************************************************************
/*!
   \file xsi_context.h
   \brief Context class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSICONTEXT_H__
#define __XSICONTEXT_H__

#include <xsi_base.h>
#include <xsi_value.h>

namespace XSI {

class ProjectItem ;
class Parameter ;


//*****************************************************************************
/*! \class Context xsi_context.h
   \brief This is a general, flexible %Context object that is used for callbacks. Instances of this object are used to
   pass information to the C++ API callbacks that implement a PluginItem.

	The main reason context objects are used in %XSI is to ensure backward compatibility for %XSI plugins.  In future
	versions it will be possible to add additional information and functionality to the context object without changing
	the signature of the callback routines.

	In some cases a specialized class will be provided rather than a basic Context object.  Some of these classes derive
	from Context (as is the case with PPGEventContext) whereas others do not (for example PluginRegistrar).

	\sa PPGEventContext, UpdateContext, PluginRegistrar
	\since 4.0
*/
//*****************************************************************************

class SICPPSDKDECL Context : public CBase
{
public:

	/*! Default constructor. */
	Context();

	/*! Default destructor. */
	~Context();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Context(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Context(const Context& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object.
	\param in_obj constant class object.
	\return The new Context object.
	*/
	Context& operator=(const Context& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Context object.
	*/
	Context& operator=(const CRef& in_ref);

	/*! Returns the object that is the cause of the callback being fired. The type of object returned depends on
	the callback.  For example this returns a CustomProperty object in the context of a \c Define callback for a
	CustomProperty PluginItem, and it returns a PPGLayout object in the context of a \c DefineLayout callback.
	This specific information is documented along with the callbacks of each PluginItem.
	\sa PluginRegistrar::RegisterProperty
	\return A reference to the plug-in item.
	*/
	CRef GetSource() const ;

	/*! Returns an attribute on the context.  Attributes are used to provide additional context information above
	and beyond the object returned by Context::GetSource.  Supported attributes, if any, are documented along with
	the callbacks of each PluginItem.
	\param in_AttributeName The name of the attribute to get.
	\return The attribute value.
	*/
	CValue GetAttribute( const CString& in_AttributeName ) ;

	/*! Sets a single attribute value on the context.  This is a way to pass additional information or instructions
	to %XSI regarding the outcome of the callback. Supported attributes, if any, are documented along with the
	callbacks of each PluginItem.  If the attribute does not exist it is added, otherwise the existing value is
	replaced by the new value.
	\param in_AttributeName The name of the attribute to set.
	\param in_val The attribute value to set.
	\return CStatus::OK
	\return CStatus::Fail
	*/
	CStatus PutAttribute( const CString& in_AttributeName, const CValue& in_val ) ;

	/*! Sets an attribute value as an array of values on the context. If the attribute does not exist it is added,
	otherwise the existing value is replaced by the new value.
	\param in_AttributeName The name of the attribute to set.
	\param in_array The attribute values to set.
	\return CStatus::OK
	\return CStatus::Fail
	*/
	CStatus PutArrayAttribute( const CString& in_AttributeName, const CRefArray& in_array ) ;

	/*! Returns an attribute value as an array of values on the context.
	\param in_AttributeName The name of the attribute to get.
	\return The attribute values to get.
	*/
	CRefArray GetArrayAttribute( const CString& in_AttributeName ) const;

	/*! Returns the user data stored in the context.
	\return The raw user data.
	*/
	CValue		GetUserData();

	/*! Allows you to store a CValue inside the context object. This is called \a User \a Data because %XSI does not
	care about the content or meaning of the data.  This information is not persisted.  A common use for this feature
	is to store a pointer to a newly allocated C++ object during an \c Init callback, and then to use Context::GetUserData
	to retrieve the pointer when other callbacks are called.

	\warning In the case where a pointer is stored during the \c Init callback, the object should be destroyed during
	the \c Term callback.

	\param in_val User data to be set for the operator instance.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	 */
	CStatus 	PutUserData(const CValue& in_val);

	private:
	Context * operator&() const;
	Context * operator&();
};

};
#endif // __XSICONTEXT_H__
