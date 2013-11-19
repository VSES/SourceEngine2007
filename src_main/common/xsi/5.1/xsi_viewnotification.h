//*****************************************************************************
/*!
   \file xsi_viewnotification.h
   \brief Contains the notification structures used by custom views:
   			\li CSelectionChangeNotification
   			\li CValueChangeNotification
   			\li CTimeChangeNotification
   			\li CWindowNotification
   			\li CObjectAddedNotification
   			\li CObjectRemovedNotification

   © Copyright 1998-2004 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSI_VIEWNOTIF_H__
#define __XSI_VIEWNOTIF_H__

#include "sicppsdk.h"
#include "xsi_status.h"
#include "xsi_ref.h"
#include "xsi_string.h"
#include "xsi_longarray.h"
#include "xsi_value.h"

namespace XSI {

//*****************************************************************************
/*! \class CSelectionChangeNotification xsi_viewnotification.h
	Holds information about a Selection change Event.
	When a Custom Display is being hosted by the Custom Display Host, this
	notification will be sent to that custom display whenever the
	selection changes in the scene.

	\sa ViewContext, Custom Display Host
 */
//*****************************************************************************
class SICPPSDKDECL CSelectionChangeNotification
{
public:

	/*! Default constructor. */
	CSelectionChangeNotification();

	/*! Default destructor. */
	~CSelectionChangeNotification();

	/*! Returns a CRefArray holding the new selection list
	\return CRefArray The new selection list
	*/
	const CRefArray&	GetSelectionList() const;

private:

	CSelectionChangeNotification (const CSelectionChangeNotification &);
	CSelectionChangeNotification * operator&() const;
	CSelectionChangeNotification * operator&();

	void*	m_pImpl;

};

//*****************************************************************************
/*! \class CValueChangeNotification xsi_viewnotification.h
	Holds information about a value change Event.
	When a Custom Display is being hosted by the Custom Display Host, this
	notification will be sent to that custom display whenever a value has
	changed in the scene.
	\sa ViewContext
 */
//*****************************************************************************
class SICPPSDKDECL CValueChangeNotification
{
public:

	/*! Default constructor. */
	CValueChangeNotification();

	/*! Default destructor. */
	~CValueChangeNotification();

	/*! Returns a CRef that is the object on which a value has changed
	\return CRef The object on which the selection has changed
	*/
	const CRef&	GetOwner() const;

	/*! Returns a a string that represents the component that has changed
	\return CString The component path
	*/
	const CString&	GetComponentName() const;

private:

	CValueChangeNotification (const CValueChangeNotification &);
	CValueChangeNotification * operator&() const;
	CValueChangeNotification * operator&();

	void*	m_pImpl;
};

//*****************************************************************************
/*! \class CTimeChangeNotification xsi_viewnotification.h
	\brief Holds information about a time change Event
	\sa ViewContext
 */
//*****************************************************************************
class SICPPSDKDECL CTimeChangeNotification
{
public:

	/*! Default constructor. */
	CTimeChangeNotification();

	/*! Default destructor. */
	~CTimeChangeNotification();

	/*! Returns the Event time
	\return double The Event time (in seconds)
	*/
	const double	GetTime() const;

	/*! Returns the time state. These are bitwise flags of siTimeState values
	\return LONG The time change state
	\sa siTimeState
	*/
	const LONG		GetState() const;

private:

	CTimeChangeNotification (const CTimeChangeNotification &);
	CTimeChangeNotification * operator&() const;
	CTimeChangeNotification * operator&();

	void*	m_pImpl;

};

//*****************************************************************************
/*! \class CWindowNotification xsi_viewnotification.h
	\brief Holds information about a change in window position
	\sa ViewContext
 */
//*****************************************************************************
class SICPPSDKDECL CWindowNotification
{
public:

	/*! Default constructor. */
	CWindowNotification();

	/*! Default destructor. */
	~CWindowNotification();

	/*! Returns the coordinates of the window in screen space
	*/
	void	GetPosition(INT &x, INT &y, INT &width, INT &height) const;

	/*! Returns the state of the window for which the notification was sent
	\return siWindowChangeState the window state
	*/
	XSI::siWindowChangeState 	GetWindowState ( ) const;


private:

	CWindowNotification (const CWindowNotification &);
	CWindowNotification * operator&() const;
	CWindowNotification * operator&();

	void*	m_pImpl;

};

//*****************************************************************************
/*! \class CObjectAddedNotification xsi_viewnotification.h
	Holds information about a new object added to the scene.
	Whenever a new object is created or added to a scene, this notification
	is sent to the custom display to notify them.
	\sa ViewContext
 */
//*****************************************************************************
class SICPPSDKDECL CObjectAddedNotification
{
public:

	/*! Default constructor. */
	CObjectAddedNotification();

	/*! Default destructor. */
	~CObjectAddedNotification();

	/*! Returns a CRef that is the object added to the scene
	\return CRef The object added to the scene
	*/
	const CRef&	GetObjectAdded() const;

private:

	CObjectAddedNotification (const CObjectAddedNotification &);
	CObjectAddedNotification * operator&() const;
	CObjectAddedNotification * operator&();

	void*	m_pImpl;
};

//*****************************************************************************
/*! \class CObjectRemovedNotification xsi_viewnotification.h
	Holds information about a object being removed from the scene.
	Whenever an object is remove or deleted from the scene, this notification
	is sent to the custom display to notify them.
	\sa ViewContext
 */
//*****************************************************************************
class SICPPSDKDECL CObjectRemovedNotification
{
public:

	/*! Default constructor. */
	CObjectRemovedNotification();

	/*! Default destructor. */
	~CObjectRemovedNotification();

	/*! Returns a string that is the fullname of the object that was removed
	\return CString A string that is the fullname of the object that was removed
	*/
	const CString&	GetObjectName() const;

	/*! Returns a flag indicating the mode in which the object was deleted
	\return siBranchFlag The branch selection flag
	*/
	const siBranchFlag	GetBranchFlag() const;



private:

	CObjectRemovedNotification (const CObjectRemovedNotification &);
	CObjectRemovedNotification * operator&() const;
	CObjectRemovedNotification * operator&();

	void*	m_pImpl;
};

}


#endif //__XSI_VIEWNOTIF_H__
