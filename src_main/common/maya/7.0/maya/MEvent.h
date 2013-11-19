
#ifndef _MEvent
#define _MEvent
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
//
// CLASS:    MEvent
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MEvent)
//
// The MEvent class is used for querying system events such as mouse presses.
//
// Events are handled by an MPxContext derived class in which MEvents are passed
// and can be accessed.
// 
// Since Maya has default actions for several events, only a subset are
// available through the API.
// The events that can be accessed are:
//    buttonPress       - mouse button press event (left & middle only)
//    buttonRelease     - mouse buttons release event (left & middle only)
//    mouseDrag         - mouse button drag event (left & middle only)
//    hold              - mouse button hold event (left & middle only)
//    deleteEvent       - delete/backspace key event
//    commandCompletion - complete key event
// 
// Several modifiers for events are also accessible through the API.  Modifiers
// are actions that occur during an event.  For example, holding down the shift
// key while pressing a mouse button causes a button press event to occur with
// a shift modifier.
// 
// A modifier can be used to determine if two mouse events occur simulaneously.
// The second mouse event is registered as a modifier in the hold event of the 
// first mouse button.  So if you wanted to determine if both the left and
// middle buttons are pressed then you would query the modifier in the hold
// event of the first mouse button using the isModifierMiddleMouseButton() and
// isModifierLeftMouseButton() methods.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MDeviceState;

// *****************************************************************************

// CLASS DECLARATION (MEvent)

/// System event information (OpenMayaUI)
/**

System event information class.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MEvent  
{
public:

	///
    enum ModifierType {
		///
        shiftKey    = 1<<0,
		///
        controlKey  = 1<<2
    };

	///
	enum MouseButtonType {
		///
		kLeftMouse		= 1<<6,
		///
		kMiddleMouse	= kLeftMouse<<1
	};

public:
	///
    MEvent();
	///
    virtual ~MEvent();

	///
    MStatus         getPosition( short& x_pos, short& y_pos ) const;
	///
    MStatus         setPosition( short& x_pos, short& y_pos );
	///
    MStatus         getWindowPosition( short& x_pos, short& y_pos ) const;
	///
    MouseButtonType mouseButton( MStatus * ReturnStatus = NULL ) const;
	///
    bool            isModifierKeyRelease( MStatus * ReturnStatus = NULL ) const;
	///
    ModifierType    modifiers( MStatus * ReturnStatus = NULL ) const;
	///
    MStatus         setModifiers( ModifierType& modType );
	///
    bool            isModifierNone( MStatus * ReturnStatus = NULL ) const;
	///
    bool            isModifierShift( MStatus * ReturnStatus = NULL ) const;
	///
    bool            isModifierControl( MStatus * ReturnStatus = NULL ) const;
	///
    bool            isModifierLeftMouseButton( MStatus * ReturnStatus = NULL )
					const;
	///
    bool            isModifierMiddleMouseButton( MStatus * ReturnStatus = NULL )
					const;

protected:
// No protected members

private:
    static const char* className();


    MEvent( const void * );
    const void * fEventPtr;
    void * fModifier;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MEvent */
