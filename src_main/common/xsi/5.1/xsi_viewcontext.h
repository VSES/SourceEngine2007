//*****************************************************************************
/*!
   \file xsi_viewcontext.h
   \brief Custom View Context object class.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIVIEWCONTEXT_H__
#define __XSIVIEWCONTEXT_H__

#include <xsi_base.h>
#include <xsi_value.h>

namespace XSI {

//*****************************************************************************
/*! \class ViewContext xsi_viewcontext.h
	\brief The ViewContext object serves 2 functions for Custom Displays:
		\li Provides the parent window handle in which Custom Displays should create it's child windows
		\li Provides information about a notification sent by XSI.

	\sa CTimeChangeNotification, CValueChangeNotification, CSelectionChangeNotification

	\eg
	\code
		void	MyCustomDisplay_Init (XSI::CRef in_pViewCtx)
		{
			// cast the CRef into a ViewContext object
			XSI::ViewContext l_vViewContext = in_pViewCtx;

			// make sure it is valid
			assert ( l_vViewContext.IsValid() );

			// Get the parent window handle. We will use this to
			// create our own child window.
			HWND l_hParent = (HWND)l_vViewContext.GetParentWindowHandle();

			// Create our window as a child of the XSI parent window
			HWND l_hMyWindow = CreateDialog(  MyDllInstance,
											MAKEINTRESOURCE(IDD_MYCUSTOMWINDOW),
											l_hParent,
											(DLGPROC)MyWindowProc);
		}

		void	MyCustomDisplay_Notify ( XSI::CRef in_pViewContext )
		{
			// cast the CRef into a ViewContext object
			XSI::ViewContext l_vViewContext = in_pViewCtx;

			// make sure it is valid
			assert ( l_vViewContext.IsValid() );

			XSI::siEventID in_eNotifcation;
			void*	in_pData;

			l_vViewContext.GetNotificationData ( in_eNotifcation, &in_pData );

			switch ( in_eNotifcation )
			{
				case siOnSelectionChange:
				{
					XSI::CSelectionChangeNotification* l_pSelection = (XSI::CSelectionChangeNotification*)in_pData;

					XSI::CString	l_szMessage;
					l_szMessage = L"XSI_SELECTION_CHANGED_CV";
					l_szMessage += L" [ ";

					//
					// Build a string of all objects that are in the selection list
					//

					for (INT c=0;c<l_pSelection->GetSelectionList().GetCount();c++)
					{
						XSI::CRef&	l_pCRef = l_pSelection->GetSelectionList().GetItem(c);

						XSI::SIObject mySIObject ( l_pCRef );

						XSI::CString	Name = mySIObject.GetFullName();

						if ( c != 0 )
							l_szMessage += L",";

						l_szMessage += Name;;

					}

					l_szMessage += L" ]";

					//
					// And print it
					//

					char *l_szMessChar = new char [ l_szMessage.Length() + 1 ];
					W2AHelper ( l_szMessChar, l_szMessage.GetWideString() );
					OutputDebugString ( l_szMessChar );
					delete [] l_szMessChar;

					break;
				}

				case siOnTimeChange:
				{
					XSI::CTimeChangeNotification* l_pTimeChange = (XSI::CTimeChangeNotification*)in_pData;

					XSI::CString	l_szMessage;
					l_szMessage = L"XSI_TIME_CHANGE_CV";

					char *l_szMessChar = new char [ l_szMessage.Length() + 1 ];
					DSW2AHelper ( l_szMessChar, l_szMessage.GetWideString() );
					OutputDebugString ( l_szMessChar );
					delete [] l_szMessChar;

					break;
				}

				case siOnValueChange:
				{
					XSI::CString	l_szMessage;
					l_szMessage = L"XSI_SET_VALUE_CV ";

					XSI::CValueChangeNotification* l_pData = (XSI::CValueChangeNotification*) in_pData;

					XSI::CString l_csCIDName = l_pData->GetOwner().GetClassIDName();

					l_szMessage += l_csCIDName;
					l_szMessage += L" [ ";
					l_szMessage += l_pData->GetComponentName();
					l_szMessage += L" ]";

					char *l_szMessChar = new char [ l_szMessage.Length() + 1 ];
					DSW2AHelper ( l_szMessChar, l_szMessage.GetWideString() );
					OutputDebugString ( l_szMessChar );
					delete [] l_szMessChar;

					break;

				}

			}
		}
	\endcode

*/
//*****************************************************************************

class SICPPSDKDECL ViewContext : public CBase
{
public:

		/*! Default constructor. */
	ViewContext();

	/*! Default destructor. */
	~ViewContext();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	ViewContext(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	ViewContext(const ViewContext& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object.
	\param in_obj constant class object.
	\return The new Context object.
	*/
	ViewContext& operator=(const ViewContext& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Context object.
	*/
	ViewContext& operator=(const CRef& in_ref);


	/*! Returns the user data stored in the Context.
	\return The raw user data.
	*/
	CValue		GetUserData();

	/*! Allows you to store a CValue inside the context object. This is called "User Data" because %XSI does
	not care about the content or meaning of the data.  This information is not persisted.  A common use for
	this feature is to store a pointer to a newly allocated C++ object during an "Init" callback,and then to
	use Context::GetUserData to retrieve the pointer when other callbacks are made. (In this scenario the object
	should be destroyed in the "Term" callback).

	\param in_val user data to be set for the operator instance.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	 */
	CStatus 	PutUserData(const CValue& in_val);

	/*! Returns the platform-specific parent window handle. For Windows, this must be cast to a HWND.
	\note This function is not necessary on Linux, where ViewContext::SetXWindowTopLevel should be used instead.
	\return Window handle.
	*/
	void*		GetParentWindowHandle();

	/*! Returns the data associated with a notification event
	\retval	out_eEventID	the event that was fired
	\retval	out_pData		the associated data
	 */
	CStatus 	GetNotificationData(siEventID& out_eEventID, void** out_pData);

	/*! Sets the Custom Display flags
	\param in_lFlags The flags
	\sa siCustomDisplayFlags
	 */
	CStatus 	SetFlags(LONG in_lFlags);


	/*! Specifies the top level X window for the Custom Display.
	\note This is only relevant on Linux.  This is needed so that %XSI can parent the Custom Display properly inside
		the View that hosts the Custom Display.
	\param in_pHandle The pointer to the X Window handle
	\return CStatus::OK Success
	\return CStatus::Fail Not a valid Window or problem with X client.
	 */
	CStatus		SetXWindowTopLevel ( void* in_pHandle );

	/*! Returns the top level widget from %XSI. This is necessary in order to create your custom display
	as a child of this widget.
	\return void* the Widget handle
	 */
	void*		GetTopLevelWidget ();


	private:
	ViewContext * operator&() const;
	ViewContext * operator&();
};

};
#endif // __XSICONTEXT_H__
