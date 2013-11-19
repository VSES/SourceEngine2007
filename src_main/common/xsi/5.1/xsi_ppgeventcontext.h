//*****************************************************************************
/*!
   \file xsi_ppgeventcontext.h
   \brief PPGEventContext class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPPGEVENTCONTEXT_H__
#define __XSIPPGEVENTCONTEXT_H__

#include <xsi_base.h>
#include <xsi_value.h>
#include <xsi_context.h>

namespace XSI {

class ProjectItem ;
class Parameter ;


//*****************************************************************************
/*! \class PPGEventContext xsi_ppgeventcontext.h
	\brief This object is provided as the context object in the %PPGEvent callback for a Plugin-based CustomProperty
	object (see PluginRegistrar::RegisterProperty).

	Using the information provided by this object, it is possible to implement PPG logic code using the C++ API.
	For information about implementing logic with scripting see PPGLayout::PutLogic and \xl PPG.

	The PPGEventContext::GetEventID function returns information about the specific event that occurred.  If the event
	is PPGEventContext::siOnInit then Context::GetSource returns the CustomProperty object that is being inspected.
	If the event is PPGEventContext::siParameterChange then Context::GetSource returns the Parameter that changed. If
	the event is PPGEventContext::siButtonClicked then call Context::GetAttribute to get the attribute "Button", which
	will return a CString with the script name of the Button that was clicked.  If the event is PPGEventContext::siTabChange
	then call Context::GetAttribute to get the attribute "Tab", which will return a CString with the label of the Tab
	that was clicked.

	To force the user interface to be redrawn, call Context::PutAttribute to set the attribute "Refresh" to true.  This
	is useful if the context code changes the underlying PPGLayout during its execution.  Similarly, it is possible to
	close the %Property Page by setting the attribute "Close".  For the equivalent functionality in scripting see
	\xl PPG.Refresh and \xl PPG.Close.

	The OnChange callback for parameters is only invoked when a user is currently inspecting the property page. However,
	it may be either the user who changed the value on the inspected page or a change initiated through scripting (excluding
	a change originating from a callback function, of course). For example, if %Parameter X is changed, and the callback code
	sets %Parameter Y to the same value as X, then the PPGEvent is not invoked a second time.

	\since 4.0
*/
//*****************************************************************************

class SICPPSDKDECL PPGEventContext : public Context
{
public:

	/*! Defines the possible actions that result in the callback function being called.
	*/
	enum PPGEvent
	{
		siUndefined         = 0,    /*!< Unknown event */
		siOnInit			= 1,	/*!< PPG has just been drawn */
		siParameterChange	= 3,	/*!< Value of a Parameter has changed */
		siButtonClicked		= 4,	/*!< The user clicked a button*/
		siTabChange         = 5     /*!< User changed tabs on a multi-tab PPG */
	};

	/*! Default constructor. */
	PPGEventContext();

	/*! Default destructor. */
	~PPGEventContext();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PPGEventContext(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PPGEventContext(const PPGEventContext& in_obj);

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
	\return The new PPGEventContext object.
	*/
	PPGEventContext& operator=(const PPGEventContext& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PPGEventContext object.
	*/
	PPGEventContext& operator=(const CRef& in_ref);

	/*! Returns information about what specific occurence in the user interface provoked the
	callback.  For additional information use Context::GetSource and Context::GetAttribute.
	*/
	PPGEvent GetEventID() const ;

	private:
	PPGEventContext * operator&() const;
	PPGEventContext * operator&();
};

};
#endif // __XSIPPGEVENTCONTEXT_H__
