//*****************************************************************************
/*!
   \file xsi_timerevent.h
   \brief TimerEvent class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITIMEREVENT_H__
#define __XSITIMEREVENT_H__

#include <xsi_eventinfo.h>

namespace XSI {


//*****************************************************************************
/*! \class TimerEvent xsi_timerevent.h
	\brief A TimerEvent represents a system timer and allows you to specify the interval and
	the delay time at which a timer elapses. Timers in XSI are independent of the timeline clock
	and can be used to produce recurrent work accurately. For example, you can implement an
	autosave mechanism with a TimerEvent.

	The TimerEvent class derives from EventInfo and can be managed like a regular XSI event.
	Use PluginRegistrar::RegisterTimerEvent to register them and Application::GetEventInfos to
	access them. Timer events can be suspended with muting and resumed by un-muting (see
	EventInfo::PutMute). You can reset a timer with TimerEvent.Reset.

	\since Gemini

	\eg Demonstrates how to create a TimerEvent object.

	\code
		using namespace XSI;

		// Function prototype for creating an event plug-in
		CString LoadTimerEventPluginHelper();

		// Load the sample timer event plug-in
		CString strEventPlugin = LoadTimerEventPluginHelper();

		// Iterate over the all event objects.
		Application app;
		CRefArray eventInfoArray = app.GetEventInfos();

		for (LONG i=0; i<eventInfoArray.GetCount(); i++ )
		{
			EventInfo eventInfo = eventInfoArray[i];

			CString strName = eventInfo.GetName();
			bool bTimerEvent = eventInfo.IsA( siTimerEvent );

			app.LogMessage( str + L":" + CString(bTimerEvent) );
		}

		// Unload the plugin and deletes it
		app.UnloadPlugin( strEventPlugin, true );

		CString LoadTimerEventPluginHelper()
		{
			// Loads the sample event plug-in if one is defined
			Application app;
			CString strEventPlugin = app.GetInstallationPath(siUserPath);
		#ifdef unix
			strEventPlugin += L"/Application/Plugins/TimerEventPlugin.js";
		#else
			strEventPlugin += L"\\Application\\Plugins\\TimerEventPlugin.js";
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
			eventWizard.PutParameterValue(L"PluginName", CString(L"MyTimerEventPlugin") );
			eventWizard.PutParameterValue(L"FileName", CString(L"TimerEventPlugin") );
			eventWizard.PutParameterValue(L"ScriptLanguage", CString(L"JScript") );

			// Tell the wizard to register the following timer event
			eventWizard.PutParameterValue(L"siOnTimer", true );
			eventWizard.PutParameterValue(L"siOnTimer", CString(L"MyOnTimerEvent") );

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

class SICPPSDKDECL TimerEvent : public EventInfo
{
public:
	/*! Default constructor. */
	TimerEvent();

	/*! Default destructor. */
	~TimerEvent();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	TimerEvent(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	TimerEvent(const TimerEvent& in_obj);

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
	\return The new TimerEvent object.
	*/
	TimerEvent& operator=(const TimerEvent& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new TimerEvent object.
	*/
	TimerEvent& operator=(const CRef& in_ref);

	/*! Returns the timer interval time in milliseconds.
	\return Interval time.
	*/
	LONG GetInterval() const;

	/*! Returns the timer delay value in milliseconds.
	\return Delay time.
	*/
	LONG GetDelay() const;

	/*! Stops and re-starts this TimerEvent with new timer values.

	\param in_interval This value specifies, in milliseconds, the amount of
		time to wait between each timer invocation. If the value is 0, the event
		timer is stopped (muted) after the first invocation. The timer can be
		re-started again by un-muting the event. If the value > 0, the timer
		will elapse indefinitely.

	\param in_delay The delay value, in milliseconds, is the amount of time
		to wait until the timer is first invoked. If the delay is set to 0 (default),
		the timer starts immediately.

	\return CStatus::OK Success.
	\return CStatus::Fail Failure.

	\sa EventInfo::PutMute
	*/
	CStatus Reset( LONG in_interval, LONG in_delay=0 );

	private:
	TimerEvent * operator&() const;
	TimerEvent * operator&();
};

};
#endif // __XSITIMEREVENT_H__
