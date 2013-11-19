//*****************************************************************************
/*!
   \file xsi_eventinfo.h
   \brief EventInfo class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIEVENTINFO_H__
#define __XSIEVENTINFO_H__

#include <xsi_siobject.h>

namespace XSI {


//*****************************************************************************
/*! \class EventInfo xsi_eventinfo.h
	\brief The EventInfo class contains the information on a particular event such as its mute value, event handler, etc.

	You access the name and the type of the event with SIObject::GetName and SIObject::GetType. Use EventInfo::PutMute to
	enable or disable events. EventInfo objects are created with Application::GetEventInfos.

	\sa Application::GetEventInfos
	\since 5.0

	\eg Demonstrates how to create EventInfo objects and to access event information.

	\code
		using namespace XSI;

		// Function prototype for creating an event plug-in
		CString LoadEventPluginHelper();

		// Load the sample event plug-in
		CString strEventPlugin = LoadEventPluginHelper();

		// Iterate over the EventInfo Objects
		Application app;
		CRefArray eventInfoArray = app.GetEventInfos();

		for (LONG i=0; i<eventInfoArray.GetCount(); i++ )
		{
			EventInfo eventInfo = eventInfoArray[i];

			LONG lToken = eventInfo.GetToken();
			app.LogMessage( L"event token identifier: " + CString(lToken) );

			CString str = eventInfo.GetName();
			app.LogMessage( L"event name: " + str );

			str = eventInfo.GetType();
			app.LogMessage( L"event type: " + str );

			str = eventInfo.GetHandler();
			app.LogMessage( L"event handler: " + str );

			str = eventInfo.GetFileName();
			app.LogMessage( L"event handler file name: " + str );

			str = eventInfo.GetLanguage();
			app.LogMessage( L"event handler implementation language: " + str );

			bool bMute = eventInfo.GetMute();
			app.LogMessage( L"event is muted: " + CString(bMute) );
		}

		// Unload the plugin and deletes it
		app.UnloadPlugin( strEventPlugin, true );

		CString LoadEventPluginHelper()
		{
			// Loads the sample event plug-in if one is defined
			Application app;
			CString strEventPlugin = app.GetInstallationPath(siUserPath);
		#ifdef unix
			strEventPlugin += L"/Application/Plugins/SimpleEventPlugin.js";
		#else
			strEventPlugin += L"\\Application\\Plugins\\SimpleEventPlugin.js";
		#endif

			Plugin myEventPlugin = app.LoadPlugin( strEventPlugin );

			if (myEventPlugin.IsValid())
			{
				return strEventPlugin;
			}

			// Gets the event wizard property
			CommandArray cmdArray = app.GetCommands();
			Command cmd = cmdArray.GetItem( L"CreateEventWizard" );

			CValue retVal;
			cmd.Execute(retVal);

			CustomProperty eventWizard(retVal);

			// Set the plug-in info
			eventWizard.PutParameterValue(L"PluginName", CString(L"MySimpleEventPlugin") );
			eventWizard.PutParameterValue(L"FileName", CString(L"SimpleEventPlugin") );
			eventWizard.PutParameterValue(L"ScriptLanguage", CString(L"JScript") );

			// Tell the wizard to register the following events
			eventWizard.PutParameterValue(L"siOnKeyUp", true );
			eventWizard.PutParameterValue(L"siOnKeyUpName", CString(L"MyOnKeyUpEvent") );
			eventWizard.PutParameterValue(L"siOnKeyDown", true );
			eventWizard.PutParameterValue(L"siOnKeyDownName", CString(L"MyOnKeyDownEvent") );

			// For the purpose of this example we use internal commands that are
			// part of the Event Wizard implementation to generate a plug-in
			// containing the new events

			cmd = cmdArray.GetItem( L"EventWizardGenerateCode" );

			cmd.Execute(retVal);

			// Load the plug-in
			app.LoadPlugin( strEventPlugin );

			return strEventPlugin;
		}
  	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL EventInfo : public SIObject
{
public:
	/*! Default constructor. */
	EventInfo();

	/*! Default destructor. */
	~EventInfo();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	EventInfo(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	EventInfo(const EventInfo& in_obj);

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
	\return The new EventInfo object.
	*/
	EventInfo& operator=(const EventInfo& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new EventInfo object.
	*/
	EventInfo& operator=(const CRef& in_ref);

	/*! Returns the connection token for this event. Once an event is bound to an event handler function, %XSI creates
	a connection token that uniquely identifies this connection.
	\return Token identifier (LONG).
	*/
	LONG GetToken() const;

	/*! Returns the name of the event handler function used for responding to the event.
	\return Handler name as CString.
	*/
	CString GetHandler() const;

	/*! Returns the name of the file implementing the event handler function.
	\return Handler file name as a CString.
	*/
	CString GetFileName() const;

	/*! Returns the language name used to implementing the event handler function. VBScript, JScript, PerlScript,
	Python and C++ are all valid languages.
	\return Language name as a CString.
	*/
	CString GetLanguage() const;

	/*! Enables or disables the event. When set to true the event is muted (disabled). All handlers connected to a muted
	event are simply ignored by %XSI when the event is fired.
	\param in_bVal True to disable the event, false to enables it
	\return CStatus::OK Success.
	*/
	CStatus PutMute(bool in_bVal);

	/*! Returns whether this event is muted.
	\return True for muted (disabled)
	*/
	bool GetMute() const;

	private:
	EventInfo * operator&() const;
	EventInfo * operator&();
};

};
#endif // __XSIEVENTINFO_H__
