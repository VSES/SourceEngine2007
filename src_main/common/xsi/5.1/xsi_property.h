//*****************************************************************************
/*!
   \file xsi_property.h
   \brief Property object class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPROPERTY_H__
#define __XSIPROPERTY_H__

#include <xsi_projectitem.h>
#include <xsi_status.h>

namespace XSI {

//*****************************************************************************
/*! \class Property xsi_property.h
	\brief The Property class represents the property of the following entities:
		\li X3DObject
		\li Model
		\li Null
		\li Project
		\li Group
		\li Scene
		\li Cluster

	Properties are created with SceneItem::AddProperty.

	\sa siBranchFlag, SceneItem::GetProperties, SceneItem::GetLocalProperties,
		Group::GetProperties, Scene::GetProperties, Project::GetProperties

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myNull;
		root.AddNull( L"", myNull );

		Property myProp;
		myNull.AddProperty(L"Custom_parameter_list", false, L"MyCustomParam", myProp);
		app.LogMessage( L"Null's custom property name: " +
						myProp.GetFullName() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Property : public ProjectItem
{
public:
	/*! Default constructor. */
	Property();

	/*! Default destructor. */
	~Property();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Property(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Property(const Property& in_obj);

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
	\return The new Property object.
	*/
	Property& operator=(const Property& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Property object.
	*/
	Property& operator=(const CRef& in_ref);

	/*! Returns true if only one object of this type is allowed on its parent.
	\return true if Singleton.
	*/
	bool GetSingleton() const;

	/*! Returns true if a property is applied to the branch of its parent.
	\note Only getting the branch property is operator compliant. You
	cannot change the branch property if you are inside an operator.
	\return true if property is applied on branch.
	*/
	bool GetBranch() const;

	/*! Sets the property branch state.
	\note Not implemented.
	\param in_bState Branch state
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus	PutBranch(bool in_bState);

	private:
	Property * operator&() const;
	Property * operator&();
};

};

#endif // __XSIPROPERTY_H__
