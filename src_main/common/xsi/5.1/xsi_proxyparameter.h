//*****************************************************************************
/*!
   \file xsi_proxyparameter.h
   \brief ProxyParameter class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPROXYPARAMETER_H__
#define __XSIPROXYPARAMETER_H__

#include <xsi_parameter.h>
#include <xsi_value.h>
#include <xsi_status.h>

namespace XSI {

//*****************************************************************************
/*! \class ProxyParameter xsi_proxyparameter.h
	\brief A ProxyParameter is a kind of Parameter that can be created on a CustomProperty
	which is "linked" with the value of another Parameter somewhere else in the Scene.

	Both the "Master Parameter" and the %ProxyParameter always have the same value and
	changing the value of the proxy parameter is equivalent to changing the value of the
	master parameter.

	\sa CustomProperty::AddProxyParameter

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		CustomProperty cpset;
		root.AddCustomProperty(L"MyCustomPSet", false, cpset );

		// For demonstration purposes,
		// get reference to rotx on the default scene camera

		CRef crefProxiedParam;
		crefProxiedParam.Set(L"Camera_Root.kine.local.rotx");

		// Create proxy parameter to this camera parameter
		ProxyParameter returnedProxyParam = cpset.AddProxyParameter(Parameter(crefProxiedParam)) ;

		// Changing the proxy parameter will update the value on the camera object
		returnedProxyParam.PutValue( 47.0 ) ;

		CString strRotX = Parameter( crefProxiedParam ).GetValue().GetAsText() ;
		app.LogMessage( L"After changing proxy param value the rot X is " + strRotX ) ;

		// You can remove a proxy parameter like this
		cpset.RemoveParameter( returnedProxyParam ) ;
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL ProxyParameter : public Parameter
{
public:
	/*! Default constructor. */
	ProxyParameter();

	/*! Default destructor. */
	~ProxyParameter();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ProxyParameter(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ProxyParameter(const ProxyParameter& in_obj);

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
	\return The new ProxyParameter object.
	*/
	ProxyParameter& operator=(const ProxyParameter& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new ProxyParameter object.
	*/
	ProxyParameter& operator=(const CRef& in_ref);

	/*! Returns the parameter that is referenced by a proxy parameter.  If this parameter
	is not a proxy parameter then an empty object is returned.
	\return The real Parameter that the ProxyParameter clones.
	*/
	Parameter GetMasterParameter() const;

	private:
	ProxyParameter * operator&() const;
	ProxyParameter * operator&();
};

};
#endif // __XSIPROXYPARAMETER_H__
