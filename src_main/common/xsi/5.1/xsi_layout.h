//*****************************************************************************
/*!
   \file xsi_layout.h
   \brief Layout class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSILAYOUT_H__
#define __XSILAYOUT_H__

#include <xsi_uipersistable.h>
#include <xsi_value.h>
#include <xsi_status.h>

#pragma warning(disable:4251)

namespace XSI {

class View;

//*****************************************************************************
/*! \class Layout xsi_layout.h
	\brief The Layout object represents an instance of a screen layout in XSI and gives access
	to the views attached to that layout. The layout is read-only and cannot be edited or created
	with the SDK.

	You can access layouts via the Desktop: see Desktop::ActiveLayout and Desktop::Layouts.

	\sa Desktop::ActiveLayout, Desktop::Layouts
	\since 4.0

	\eg
	\code
		using namespace XSI;

		Application app;
		Desktop theDesktop = app.GetDesktop();

		app.LogMessage( L"List all loaded layouts" );
		CRefArray layoutArray = theDesktop.GetLayouts();
		for (LONG i=0; i<layoutArray.GetCount(); i++)
		{
			Layout layout = layoutArray[i];
			app.LogMessage( L"Layout name: " + layout.GetName() );
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL Layout : public UIPersistable
{
public:
	/*! Default constructor. */
	Layout();

	/*! Default destructor. */
	~Layout();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Layout(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Layout(const Layout& in_obj);

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
	\return The new Layout object.
	*/
	Layout& operator=(const Layout& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Layout object.
	*/
	Layout& operator=(const CRef& in_ref);

	/*! Returns an array of View objects currently attached to this layout.
	\return Array of View objects,
	*/
	CRefArray GetViews() const;

	/*! Creates a new View object. The new view is a temporary view and not attached to this layout,
	therefore saving the layout does not make the new view part of the layout either.

	\param in_strTypeName The type of view to create. Available types can be found
		in the \c .xsivw and \c .xsitb files under the %Application\\Views and
		%Application\\Toolbars folders located in the standard factory, user and
		workgroup locations.
	\param in_strViewName The name of the view to create.
	\retval out_newView The newly created view object.

	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus CreateView(const CString& in_strTypeName,
		const CString& in_strViewName, CRef& out_newView );

	/*! Creates a new View object given the path to its definition on disk.  This function is a low-level
	approach to dealing with Views, in most cases Layout::CreateView is the most convenient way to create
	floating views.

	This function will load any valid view file, even if the file is outside of the normal search locations
	inside the user, factory and workgroups directories. It will fail if there is an XML syntax error or
	other problem with the content of the view definition.

	\param in_strFilePath Complete path to a \c .xsitb or \c .xsivw file on disk.
	\param in_strViewName The name of the view to create.
	\retval io_newView Returns the new view.

	\return CStatus::OK success
	\return CStatus::Fail failure
	\since
	*/
	CStatus CreateViewFromDefinitionFile( const CString& in_strFilePath,
		const CString& in_strViewName, CRef& io_newView );


	private:
	Layout * operator&() const;
	Layout * operator&();
};

};
#endif // __XSILAYOUT_H__
