//*****************************************************************************
/*!
   \file xsi_menuitem.h
   \brief MenuItem class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIMENUITEM_H__
#define __XSIMENUITEM_H__

#include <xsi_uiobject.h>
#include <xsi_status.h>
#include <xsi_decl.h>

namespace XSI {

//*****************************************************************************
/*! \class MenuItem xsi_menuitem.h
	\brief The MenuItem object represents an individual item on a custom menu.

	Here are some of the features:

		\li You can attach a command or a callback function to a custom menu item and fire them when you click on the menu item.
		\li %Menu items set with a command are greyed out if the command is disabled.
		\li %Menu items will show the origin of the command or callback ([u] for user, [w] for workgroup).
		\li %Menu items that point to a command will show the hotkey associated with the command.

	A menu item can be defined as a standard menu item on which you can attach a command or callback function

	\note If for some reasons you assign both a command and a callback to your menu item, the command will
		have precedence over the callback when the menu item is selected).

	You can also create a menu item as a separator or as a sub menu. See the Menu object documentation for more details about
	adding menu items to a custom menu.

	\since 4.0
*/
//*****************************************************************************

class SICPPSDKDECL MenuItem : public UIObject
{
public:
	/*! Default constructor. */
	MenuItem();

	/*! Default destructor. */
	~MenuItem();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	MenuItem(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	MenuItem(const MenuItem& in_obj);

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
	\return The new MenuItem object
	*/
	MenuItem& operator=(const MenuItem& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new MenuItem object.
	*/
	MenuItem& operator=(const CRef& in_ref);

	/*! Sets the callback function for this menu item. The callback function is called when the item is
	selected and is executed by %XSI as a single undo event. You can always undo/redo the action of a menu
	item callback function provided your function calls undoable commands or object model methods. The
	callback function must have the following signature:

	\code
	CStatus MyFunction( const XSI::CRef& in_context )
	\endcode

	\par in_context
	The Context object containing the MenuItem object for which the function is called.

	The menu item that points to a callback has the following caption: \code item_name [origin] \endcode
	where \c item_name is the name of the menu item and \c origin the origin of the callback (\c [u] for
	user, \c [w] for workgroup).

	\param in_strCallback Callback function name.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutCallback( const CString& in_strCallback);

	/*! Returns the callback function for this menu item.
	\return The callback function name.
	*/
	CString GetCallback() const;

	/*! Sets the command for this menu item. You can attach any native or custom commands you want to a
	menu item. If your menu is attached to a contextual menu and one of your command's arguments takes the
	selected objects by default, then the current selected object(s) is passed into your command. The target
	object under the cursor is also passed in as part of the selected objects. However if no objects are
	selected then only the target is passed in.

	When a command is attached to a menu item, the caption is set as follows: \code item_name [origin] hotkey \endcode
	where \c item_name is the name of the menu item, \c origin the origin of the command (\c [u] for user,
	\c [w] for workgroup) and \c hotkey the hotkey associated with the command.

	The menu item will be greyed out if the command is disabled.

	\param in_strCommand Command name.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutCommand( const CString& in_strCommand );

	/*! Returns the style of this menu item.
	\return The menu item style.
	*/
	siMenuItemStyle GetStyle() const;

	/*! Sets the filter for this menu item. XSI uses the filter for validating themenu item or an entire menu
	against the target or selected objects before it gets displayed.

	If the filter criteria are not satisfied then the menu item is greyed out. Menus are removed if attached
	to a contextual menu.

	\param in_strFilter Filter name.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\since 5.0
	*/
	CStatus PutFilter( const CString& in_strFilter );

	/*! Returns the filter attached to this menu item.
	\return Reference to the Filter object.
	\since 5.0
	*/
	CRef GetFilter() const;

	/*! Returns true if the menu item is enabled.
	\return True if enabled.
	\since 5.0
	*/
	bool IsEnabled() ;

	/*! Enables or disables a menu item. Menu objects can also be enabled or disabled which means all menu items
	defined in a menu are affected. This property is typically used for updating a menu item based on a specific
	context. Enabled by default.

	\param in_bFlag True enables the menu item, false disables it.
	\return CStatus::OK Success.
	\return CStatus::Fail Failure.
	\since 5.0
	*/
	CStatus PutEnabled( bool in_bFlag ) ;

	private:
	MenuItem * operator&() const;
	MenuItem * operator&();
};

};
#endif // __XSIMENUITEM_H__
