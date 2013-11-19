
#ifndef _MDeviceState
#define _MDeviceState
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
// CLASS:    MDeviceState
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDeviceState)
//
//	MDeviceState is a generic event class for input devices.
//	Input device classes (such as MPxMidiInputDevice) are responsible for
//	converting specific event types to an MDeviceState which Maya understands.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MString;
class THeventInputDevice;

// *****************************************************************************

// CLASS DECLARATION (MDeviceState)

/// Input device state (OpenMayaUI)
/**

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MDeviceState
{
public:
	///
	virtual ~MDeviceState();

	///
	int		devicePosition( const unsigned short int axis) const;
	///
	int		devicePosition( const MString & axisName ) const;
	///
	void	setDevicePosition( const int position,
							   const unsigned short int axis);
	///
	void	setDevicePosition( const int position,
							   const MString & axisName );

	///
	bool	buttonState( const unsigned short int button ) const;
	///
	bool	buttonState( const MString & buttonName ) const;
	///
	void	setButtonState( const bool state,
							const unsigned short int button );
	///
	void	setButtonState( const bool state,
							const MString & buttonName );

	///
	int		maxAxis() const;

	///
	bool	isNull();

protected:
// No protected members

private:
	MDeviceState( void * );
	void *   data();
	void * api_stateData;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDeviceState */
