//*****************************************************************************
/*!
   \file xsi_menu.h
   \brief Custom Menu class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIMENU_H__
#define __XSIMENU_H__

#include <xsi_menuitem.h>
#include <xsi_status.h>

namespace XSI {

//*****************************************************************************
/*! \class Menu xsi_menu.h
	\brief Represents a custom menu in %XSI and provides the basic support for creating them.

	The most important features of custom menus are:

	\li Built on demand through a callback function when the end-user opens the menu.

	\li Can be attached to %XSI native menus (aka hosting menus) of specific views.

	\li Can be created either as a dynamic or static menu: a dynamic menu is always
		rebuilt before the menu opens whereas static menus are only built once.

	\li Can be displayed either as a submenu or as a flat list of items in the hosting menu.

	\li Can be set with a Filter for validating the menu items against the target/selected
		object(s) before opening the menu.

	\li The location where custom menus can be attached are identified with anchor points,
		see ::siMenuAnchorPoints for a complete list.

	\li Can be shared by multiple views, the same menu can be registered in different views
		with different anchor points and display options. This maximizes the code reusability
		of custom menus in %XSI.

	Custom menus are self-installed plug-ins and are integrated in %XSI by dropping the plug-in file
	in the %Application/Plugins folder.

	\note %XSI doesn't allow you to create a custom menu by running a script from the scripting editor:
		you must implement a menu plug-in.

	A custom menu must be registered at startup time with PluginRegistrar::RegisterMenu. The registration
	function doesn't load the custom menu explicitly in memory; %XSI loads the menu only when it is requested
	by the user.

	You must implement a callback function in order to build a menu and add menu items, this function is
	called by %XSI when the user opens the menu. The signature of this callback function is the following:

	\code
	CStatus MyMenu_Init( const XSI::CRef& in_context )
	\endcode

	\par in_context
	The Context object containing the menu to build. The function name must be formatted as
	\code {plugin_item_name}_Init \endcode where \c plugin_item_name is the string passed to
	PluginRegistrar::RegisterMenu to identify the menu plug-in item.

	\note The function return value is not required by %XSI.

	\sa MenuItem, \xl cus_menus

	\since 4.0

	\eg Example showing a simple custom menu plugin.

	\code
		CStatus XSILoadPlugin( PluginRegistrar& in_reg )
		{
			// register plugin information
			in_reg.PutAuthor( L"Softimage Corp." );
			in_reg.PutName( L"Deform menu plugin" );

			// set the version number of this plugin
			in_reg.PutVersion( 1, 0 );

			// install a dynamic custom menu in the spatial section of the
			// toolbar panel deform menu
			bool bDynamic = true;
			bool bDisplayAsSubmenu = true;

			in_reg.RegisterMenu( siMenuToolbarDeformSpatialID, L"Custom",
				bDisplayAsSubmenu, bDynamic );

			return CStatus::OK;
		}

		// This is the callback function used for building the menuItem. Since the
		// menu is dynamic, the function is called each time the menu opens.
		CStatus Custom_Init( XSI::CRef& in_ref )
		{
			// retrieve the menu object to build
			Menu menu = Context( in_ref ).GetSource();

			// set the menu caption
			menu.PutName( L"&Custom Deform" );

			// attach the Twist command to the first menu item
			MenuItem menuItem;
			menu.AddCommandItem( L"&Twist", L"Twist", menuItem );

			// adds a separator
			menu.AddSeparatorItem();

			// adds other menu items and attach a function callback
			menu.AddItem( L"Custom &1", siMenuItem, menuItem );
			menuItem.PutCallback( L"OnMenuItem" );

			menu.AddCallbackItem( L"Custom &2", L"OnMenuItem", menuItem );
			menu.AddCallbackItem( L"Custom &3", L"OnMenuItem", menuItem );

			menu.AddSeparatorItem();

			Menu subMenu = menuItem;
			menu.AddItem( L"&Sub menu", siMenuItemSubmenu, menuItem );
			subMenu.AddCommandItem( L"Sub&divide", L"Subdivide", menuItem );

			// set the submenu menu with a polygon filter, the menu will be enabled
			// only if polygon mesh objects are selected
			subMenu.PutFilter( L"Polygon" );
			return CStatus::OK;
		}

		// This callback function is called by XSI when the menu item 1 or 2 are
		// selected
		XSI::CStatus OnMenuItem( XSI::CRef& in_ref )
		{
			Context ctxt = in_ref;
			MenuItem menuItem = ctxt.GetSource();

			Application app;
			app.LogMessage( menuItem.GetName() + L":" + menuItem.GetCallback() );

			return CStatus::OK;
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL Menu : public MenuItem
{
public:
	/*! Default constructor. */
	Menu();

	/*! Default destructor. */
	~Menu();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Menu(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Menu(const Menu& in_obj);

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
	\return The new Menu object.
	*/
	Menu& operator=(const Menu& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Menu object.
	*/
	Menu& operator=(const CRef& in_ref);

	/*! Adds a menu item at the end of the menu. This function doesn't attach a callback or
	command to the newly created menu item. You must call the proper function to do so
	(ie., Menu::AddCommandItem or Menu::AddCallbackItem).

	\param in_strLabel The menu item label
	\param in_pStyle The menu item style.
	\retval out_menuItem The new MenuItem object.

	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddItem(
		const CString&			in_strLabel,
		const siMenuItemStyle	in_pStyle,
		MenuItem&				out_menuItem );

	/*! Adds a menu item at the end of the menu and attaches a command. The command is fired
	when the menu item is selected.

	You can attach any native or custom commands you want to a menu item. If your menu is attached
	to a contextual menu and one of your command's arguments takes the selected objects by default,
	then the current selected object(s) is passed in to your command. The target object under the
	cursor is also passed in as part of the selected objects. However if no objects are selected
	then only the target is passed in.

	\param in_strLabel The menu item label
	\param in_strCmd The name of a command.
	\retval out_menuItem The new MenuItem object.

	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddCommandItem(
		const CString& in_strLabel,
		const CString& in_strCmd,
		MenuItem& out_menuItem );

	/*! Adds a menu item at the end of the menu and attaches a callback function. The callback is
	fired when the menu item is selected.

	If your menu is attached to a contextual menu, the currently selected objects are passed into
	your callback. The target object under the cursor is also passed in as part of the selected objects.
	However if no objects are selected then only the target is passed in. The objects can be retrieved
	through the Context::GetAttributeValue function using the \c Target attribute. The selected/target
	objects are not passed into the callback of a custom menu item attached to a regular menu.

	\param in_strLabel The menu item label
	\param in_strCallback The name of the callback.
	\retval out_menuItem The new MenuItem object.

	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddCallbackItem(
		const CString& in_strLabel,
		const CString& in_strCallback,
		MenuItem& out_menuItem );

	/*! Adds a separator at the end of the menu.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus AddSeparatorItem();

	private:
	Menu * operator&() const;
	Menu * operator&();
};

};
#endif // __XSIMENU_H__
