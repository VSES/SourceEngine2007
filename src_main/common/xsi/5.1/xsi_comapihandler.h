//*****************************************************************************
/*!	\file xsi_comapihandler.h
 	\brief CComAPIHandler class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __CCOMAPIHANDLER__H_
#define __CCOMAPIHANDLER__H_

#include <xsi_ref.h>
#include <xsi_string.h>
#include <xsi_value.h>
#include <xsi_status.h>

namespace XSI {

//*****************************************************************************
/*! \class CComAPIHandler xsi_comapihandler.h
 	\brief The CComAPIHandler class implements a C++ wrapper for calling automation objects (scripting objects) or
 	objects implemented using the COM API.

	Automation objects can be created from C++ using CComAPIHandler::CreateInstance. The handler provides access to an
	automation object’s methods and properties via CComAPIHandler::Call, CComAPIHandler::GetProperty and CComAPIHandler::PutProperty.

	\sa ::ConvertObject

	\eg
	\code
		using namespace XSI;
		CStatus st;

		CComAPIHandler xsiuitoolkit;

		xsiuitoolkit.CreateInstance( L"XSI.UIToolkit");

		CValue return = xsiuitoolkit.GetProperty( L"ProgressBar" );

		// The return contains a pointer to the progressbar
		// that has had its reference count incremented, the destructor of CValue
		// takes care of dereferencing the pointer.
		CComAPIHandler progressbar( return );

		// set min/max
		progressbar.PutProperty( L"Minimum", (LONG)1 );
		progressbar.PutProperty( L"Maximum", (LONG)200 );
		progressbar.PutProperty( L"Value", (LONG)195 );
		progressbar.PutProperty( L"Caption", CValue(L"My Caption") );

		return = progressbar.GetProperty( L"Value" );

		// show the progress bar
		progressbar.PutProperty( L"Visible", true );

		while ( (progressbar.GetProperty( L"CancelPressed" ) == false) &&
			((LONG)return < 200))
		{
			// your code here

			progressbar.Call( L"Increment", return );
		}

		// Just for good measure. If you put break points in the
		// code sometimes the progress dialog doesn't go away.
		progressbar.PutProperty( L"Visible", false );

	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL CComAPIHandler
{
public:
	enum InvokeFlag
	{
		Method,
		PropertyGet,
		PropertyPut,
		PropertyPutRef
	};

	/*! Default constructor. */
	CComAPIHandler();

	/*! Default destructor. */
	virtual ~CComAPIHandler();

	/*! Constructor.
	\param in_val Contains a reference to a COM object or an XSI object.

	\eg Demonstrates how it is possible to access properties exposed on XSI objects using the COM (Scripting)
		interface rather than the C++ API.  This approach is important for accessing any method or property
		that is available in the COM API but not the C++ API.

	\code
		using namespace XSI ;
		Application app ;
		Model root = app.GetActiveSceneRoot();

		X3DObject arc;
		root.AddGeometry( L"Arc", L"NurbsCurve", L"AnArc", arc ) ;

		// Construct a CComAPIHandler representing the newly created arc object
		CComAPIHandler omArc( arc.GetRef() );

		// Now we can use the COM API (e.g. the scripting interface)
		// to modify the object.  In this case we change the name property.
		omArc.PutProperty( L"Name", L"MyArc" ) ;

		// Retrieve the object name using the Name property
		CValue nameFromCOMAPI = omArc.GetProperty( L"Name" ) ;
		app.LogMessage( L"COM SIObject::Name property returns: " + nameFromCOMAPI.GetAsText() ) ;

		// In this case the same property is available directly from the C++ API
		// so we didn't really need to use CComAPIHandler
		CString nameFromCppAPI = arc.GetName() ;
		app.LogMessage( L"C++ API SIObject::GetName returns: " + nameFromCppAPI ) ;

		// Expected results:
		//'INFO : COM SIObject::Name property returns: MyArc
		//'INFO : C++ API SIObject::GetName returns: MyArc
	\endcode
	*/
	CComAPIHandler( const CValue& in_val );

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	CComAPIHandler(const CComAPIHandler& in_obj);

	/*! Assignment operator
	\param in_obj CComAPIHandler to copy.
	*/
	CComAPIHandler& operator=( const CComAPIHandler& in_obj);

	/*! Creates an automation object and attaches it to the CComAPIHandler object. If you try to create an
	automation object that doesn't exist an error (CStatus::Fail) occurs.
	\param in_name \c ProgID of the COM object to create. The ProgID is a string found in the Registry, that
		maps to the \c CLSID of the object.  For example \c XSI.UIToolkit and \c Scripting.FileSystemObject.
	\return CStatus::OK	success
	\return CStatus::Fail other failure
	*/
	CStatus CreateInstance( const CString& in_name );

	/*! Attaches an automation object or %XSI object to the CComAPIHandler object. The CValue can be set either
	with a CRef object or with a pointer to a COM object.   CComAPIHandler takes care of handling the reference
	counting on the underlying COM object.
	\param in_val CValue to attach
	\return CStatus::OK	Success
	\return CStatus::InvalidArgument Invalid referance object
	\return CStatus::Fail Other failure
	*/
	CStatus Attach( const CValue& in_val );

	/*! Detaches an automation object pointer from the CComAPIHandler object.
	\return CStatus::OK Success
	\return CStatus::Fail Other failure
	*/
	CStatus Detach(void);

	/*! Returns the property value.
	\param in_name The name of the property.
	\return The property value.
	*/
	CValue GetProperty(const CString& in_name ) const;

	/*! Sets the property value. If you try to set a read-only property, an error (CStatus::AccessDenied) occurs.
	If you try to call a method that doesn't exist an error (CStatus::MemberNotFound) occurs.
	\param in_name The name of the property.
	\param in_newval The new property value.
	\return CStatus::OK Success
	\return CStatus::Fail Other failure
	\return CStatus::AccessDenied Trying to set a read-only parameter.
	\return CStatus::MemberNotFound Property does not exist.
	*/
	CStatus PutProperty( const CString& in_name, const CValue& in_newval ) const;

	/*! Calls the object method specified by name. If you try to call a method that doesn't exist an error
	(CStatus::MemberNotFound) occurs.
	\param in_name The name of the method.
	\param io_return The return value.
	\param in_args Array of arguments passed through to the method to call,
	\return CStatus::OK	Success
	\return CStatus::Fail Other failure
	\return CStatus::AccessDenied Trying to set a read-only parameter.
	\return CStatus::MemberNotFound	Method does not exist.
	*/
	CStatus Call(	const CString& in_name,
					CValue& io_return,
					const CValueArray& in_args = CValueArray() ) const;

	/*! Calls the object method specified by name. This version is convenient for calling a method that takes one argument.
	If you try to call a method that doesn't exist an error (CStatus::MemberNotFound) occurs.
	\param in_name The name of the property.
	\param io_return The return value.
	\param in_arg The argument passed through to the method.
	\return CStatus::OK Success
	\return CStatus::Fail Other failure
	\return CStatus::AccessDenied Trying to set a read-only parameter.
	\return CStatus::MemberNotFound Method does not exist.
	*/
	CStatus Call(	const CString& in_name,
					CValue& io_return,
					const CValue& in_arg ) const;

	/*! Calls the object method specified by name. This version is convenient for calling a method that takes two arguments.
	If you try to call a method that doesn't exist an error (CStatus::MemberNotFound) occurs.
	\param in_name The name of the property.
	\param io_return The return value.
	\param in_arg The first argument passed through to the method.
	\param in_arg1 The second argument passed through to the method.
	\return CStatus::OK	Success
	\return CStatus::Fail Other failure
	\return CStatus::AccessDenied Trying to set a read-only parameter.
	\return CStatus::MemberNotFound	Method does not exist.
	*/
	CStatus Call(	const CString& in_name,
					CValue& io_return,
					const CValue& in_arg,
					const CValue& in_arg1 ) const;

	/*! Calls the object method specified by name. This version is convenient for calling a method that takes three arguments.
	If you try to call a method that doesn't exist an error (CStatus::MemberNotFound) occurs.
	\param in_name The name of the property.
	\param io_return The return value.
	\param in_arg The first argument passed through to the method.
	\param in_arg1 The second argument passed through to the method.
	\param in_arg2 The third argument passed through to the method.
	\return CStatus::OK	Success
	\return CStatus::Fail Other failure
	\return CStatus::AccessDenied Trying to set a read-only parameter.
	\return CStatus::MemberNotFound	Method does not exist.
	*/
	CStatus Call(	const CString& in_name,
					CValue& io_return,
					const CValue& in_arg,
					const CValue& in_arg1,
					const CValue& in_arg2 ) const;

	/*! Calls the object method specified by name. This version is convenient for calling a method that takes four arguments.
	If you try to call a method that doesn't exist an error (CStatus::MemberNotFound) occurs.
	\param in_name The name of the property.
	\param io_return The return value.
	\param in_arg The first argument passed through to the method.
	\param in_arg1 The second argument passed through to the method.
	\param in_arg2 The third argument passed through to the method.
	\param in_arg3 The fourth argument passed through to the method.
	\return CStatus::OK	Success
	\return CStatus::Fail Other failure
	\return CStatus::AccessDenied Trying to set a read-only parameter.
	\return CStatus::MemberNotFound	Method does not exist.
	*/
	CStatus Call(	const CString& in_name,
					CValue& io_return,
					const CValue& in_arg,
					const CValue& in_arg1,
					const CValue& in_arg2,
					const CValue& in_arg3 ) const;

	/*! Calls the object method or property specified by name, in the context specified by wFlags. If you try to call a
	method/property that doesn't exist an error (CStatus::MemberNotFound) occurs.
	\param in_name The name of the property or method to invoke
	\param in_flags invoke flags
	\param io_return The return value.
	\param in_args Array of arguments for the method to call,
	\return CStatus::OK	Success
	\return CStatus::Fail Other failure
	\return CStatus::AccessDenied Trying to set a read-only parameter.
	\return CStatus::MemberNotFound	Method or property does not exist.
	*/
	CStatus Invoke(	const CString& in_name,
					LONG in_flags,
					CValue& io_return,
					const CValueArray& in_args = CValueArray() ) const;

	/*! Returns a reference to the automation object attached to the CComAPIHandler.
	\return A reference to an automation object.
	*/
	CValue GetRef(void) const;

private:
	void* m_ptr; // pointer to COM object
};
};
#endif // __CCOMAPIHANDLER__H_
