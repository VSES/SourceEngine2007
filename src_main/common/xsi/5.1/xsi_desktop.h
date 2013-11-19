//*****************************************************************************
/*!
   \file xsi_desktop.h
   \brief Desktop class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIDESKTOP_H__
#define __XSIDESKTOP_H__

#include <xsi_siobject.h>
#include <xsi_value.h>
#include <xsi_status.h>

#pragma warning(disable:4251)

namespace XSI {

class Layout;

//*****************************************************************************
/*! \class Desktop xsi_desktop.h
	\brief The %Desktop provides the services for creating and accessing UI objects in XSI.

	\since 4.0

	\eg
	\code
		using namespace XSI;

		Application app;
		Desktop theDesktop = app.GetDesktop();
		app.LogMessage( L"Desktop name: " + theDesktop.GetName() );

		Layout activeLayout = theDesktop.GetActiveLayout();
		app.LogMessage( L"Active layout name: " + activeLayout.GetName() );

		app.LogMessage( L"Listing all loaded layouts" );
		CRefArray layoutArray = theDesktop.GetLayouts();
		for (LONG i=0; i<layoutArray.GetCount(); i++)
		{
			Layout layout = layoutArray[i];
			app.LogMessage( L"Layout name: " + layout.GetName() );
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL Desktop : public SIObject
{
public:
	/*! Default constructor. */
	Desktop();

	/*! Default destructor. */
	~Desktop();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Desktop(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Desktop(const Desktop& in_obj);

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
	\return The new Desktop object.
	*/
	Desktop& operator=(const Desktop& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Desktop object.
	*/
	Desktop& operator=(const CRef& in_ref);

	/*! Returns the current active Layout object.
	\return The active Layout,
	*/
	Layout GetActiveLayout() const;

	/*! Sets the Layout object as active.
	\param in_layout The new active layout object
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutActiveLayout( const Layout& in_layout );

	/*! Returns an array of Layout objects currently loaded in XSI.
	\return Array of Layout objects,
	*/
	CRefArray GetLayouts() const;

	/*! Redraws the user interface.  It does this by pausing the execution of the plug-in until all pending
	window messages have been processed. See also the \xl Refresh and \xl SceneRefresh commands.

	\warning Use this function with caution.  Calling it frequently can significantly slow down the execution
	of a plug-in. Redrawing the UI can have implications beyond just the pixels on the screen; for example,
	render regions may be recalculated and parts of the scene may be re-evaluated.
	*/
	void RedrawUI() ;

	/*! Disables the Win32 hook used by %XSI on Windows controls.

	On Windows, this Win32 hook can interfere with the appearance and behavior of controls in the custom display
	host and other custom user interfaces. The hook does not interfere with property pages and views.

	Disable the hook just before you create a custom UI. After you finish creating the custom UI, call
	Desktop::RestoreWin32ControlsHook to restore the hook.

	You do not need to disable the hook for property pages and views, or on Linux.

	Do not disable the controls hook at any other time.

	\warning %XSI will not function properly if you do not restore the controls hook.

	\since 5.0

	\eg Demonstrates suspending and restoring the Windows controls hook
	\warning This example uses the Win32 API and therefore does not run on Linux.

	\code
		void ShowFontDlg()
		{
			Application app ;

			LOGFONT lf;
			CHOOSEFONT cf ;
			DWORD rgbCurrent = 0 ;

			ZeroMemory(&cf, sizeof(cf));
			cf.lStructSize = sizeof (cf);

			// Use the XSI main window so that the Font
			// dialog is properly parented
			cf.hwndOwner = (HWND)app.GetDesktop().GetApplicationWindowHandle();

			cf.lpLogFont = &lf;
			cf.rgbColors = rgbCurrent;
			cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

			// Disable the Win32 hook so the
			// Font dialog is displayed properly
			app.GetDesktop().SuspendWin32ControlsHook() ;
			::ChooseFont(&cf) ; // This function is part of the Win32 API
			app.GetDesktop().RestoreWin32ControlsHook() ;
		}
	\endcode
	*/
	void SuspendWin32ControlsHook() ;

	/*! Restores the Win32 controls hook disabled by a previous call to Desktop::SuspendWin32ControlsHook.
	\since 5.0
	*/
	void RestoreWin32ControlsHook() ;

	/*! Returns the platform-specific top %XSI window handle. On Windows, you must cast the return value to
	an HWND. On Linux, you get an X window.
	\return Window handle.
	\sa ViewContext::GetParentWindowHandle
	\since 5.0
	*/
	void* GetApplicationWindowHandle();


	private:
	Desktop * operator&() const;
	Desktop * operator&();
};

};
#endif // __XSIDESKTOP_H__
