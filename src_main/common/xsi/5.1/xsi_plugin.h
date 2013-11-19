//*****************************************************************************
/*!	\file xsi_plugin.h
	\brief Plugin class declaration.

	© Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
	reserved. This file contains confidential and proprietary information of
	Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
	end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPLUGIN_H__
#define __XSIPLUGIN_H__

#include <xsi_siobject.h>

namespace XSI
{

//*****************************************************************************
/*! \class Plugin xsi_pluginitem.h
	\brief A Plugin object holds read-only information on a particular plug-in file registered in XSI.

	Plug-in objects are filled in with the PluginRegistrar object at startup and can be accessed with
	the Application::GetPlugins property.

	\since 4.0

	\eg Shows how to access information about the plug-ins that are currently registered in XSI

	\code
		using namespace XSI;

		Application app;

		CRefArray pluginArray = app.GetPlugins();

		for (LONG i=0; i<pluginArray.GetCount(); i++)
		{
			Plugin plugin(pluginArray[i]);

			app.LogMessage( L"***" + plugin.GetName() + L"***" );

			LONG minor, major;
			plugin.GetVersion( major, minor );

			app.LogMessage( L"version: " +
				CValue((LONG)major).GetAsText() + L":" +
				CValue((LONG)minor).GetAsText() );

			app.LogMessage( L"author: " + plugin.GetAuthor() );
			app.LogMessage( L"language: " + plugin.GetLanguage() );
			app.LogMessage( L"file name: " + plugin.GetFilename() );
			app.LogMessage( L"url: " + plugin.GetURL() );
			app.LogMessage( L"email: " + plugin.GetEmail() );
			app.LogMessage( L"help: " + plugin.GetHelp() );
			app.LogMessage( L"origin: " + CValue((LONG)plugin.GetOrigin()).GetAsText() );
			app.LogMessage( L"origin path: " + plugin.GetOriginPath() );
			app.LogMessage( L"loaded: " + CValue(plugin.IsLoaded()).GetAsText() );

			CString strCat;
			CStringArray strArray = plugin.GetCategories();
			for (LONG j=0; j<strArray.GetCount(); j++)
			{
				strCat += strArray[j]; strCat += L",";
			}
			app.LogMessage( L"category: " + strCat );

			CRefArray itemArray = plugin.GetItems();
			for (j=0; j<itemArray.GetCount(); j++)
			{
				PluginItem pluginItem(itemArray[j]);
				app.LogMessage( L"*** item name: " + pluginItem.GetName() );
				app.LogMessage( L"    item type: " + pluginItem.GetType() );
				app.LogMessage( L"    item help: " + pluginItem.GetHelp() );
				app.LogMessage( L"    item origin: " + CValue((LONG)pluginItem.GetOrigin()).GetAsText() );
				app.LogMessage( L"    item origin path: " + pluginItem.GetOriginPath() );

				CStringArray strArray = pluginItem.GetCategories();
				for (LONG j=0; j<strArray.GetCount(); j++)
				{
					strCat += strArray[j]; strCat += L",";
				}
				app.LogMessage( L"    item category: " + strCat );
			}
		}
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Plugin : public SIObject
{
	public:
	/*! Default constructor. */
	Plugin();

	/*! Default destructor. */
	~Plugin();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Plugin(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Plugin(const Plugin& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The new object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new Plugin object.
	*/
	Plugin& operator=(const Plugin& in_obj);

	/*! Creates an object from a reference object. The new object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Plugin object.
	*/
	Plugin& operator=(const CRef& in_ref);

	/*!	Returns the name of the plug-in's author.
	\return The name of the plug-in's author
	 */
	CString GetAuthor() const;

	/*! Returns the URL path that can be used to link to a help page for the
	plug-in (for example, the web site of the plug-in's author).
	\return The URL path.
	 */
	CString GetURL() const;

	/*! Returns the email address of the plug-in (for example, the email address
	of the plug-in's author).
	\return The email address.
	 */
	CString GetEmail() const;

	/*! Returns the plug-in version.
	\retval out_major Major version number.
	\retval out_minor Minor version number.
	\return CStatus OK if successful.
	 */
	CStatus GetVersion( LONG& out_major, LONG& out_minor ) const;

	/*!	Returns the name of the language used to implement the plug-in. \c VBScript,
	\c JScript, \c PerlScript, \c Python and \c C++ are all valid languages.
	\return The name of the implementation language.
	*/
	CString GetLanguage() const;

	/*!	Returns the filename of the plug-in.
	\return The filename of the plug-in.
	*/
	CString GetFilename() const;

	/*! Returns true if the plug-in is currently loaded in the application.
	\return true if the plug-in is currently loaded
	*/
	bool IsLoaded() const;

	/*!	Returns a CRefArray object containing the plug-in items (PluginItem) registered
	by this plug-in. A single Plugin object can contain multiple customizable items,
	such as custom menus, operators and filters.
	\return Array of PluginItem objects.
	 */
	CRefArray GetItems() const;

	private:
	Plugin * operator&() const;
	Plugin * operator&();
};

};

#endif // __XSIPlugin_H__
