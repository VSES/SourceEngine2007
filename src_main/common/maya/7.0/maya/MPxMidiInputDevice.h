
#ifndef _MPxMidiInputDevice
#define _MPxMidiInputDevice
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
// CLASS:    MPxMidiInputDevice
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxMidiInputDevice)
//
//  Base class for user-defined midi input devices.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#ifdef SGI

#include <dmedia/midi.h>
#endif // SGI
#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MDeviceState;
class MDeviceChannel;
class MString;

// *****************************************************************************

// CLASS DECLARATION (MPxMidiInputDevice)

/// Midi input device (OpenMayaUI)
/**

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MPxMidiInputDevice
{
public:
	///
	MPxMidiInputDevice();
	///
	virtual ~MPxMidiInputDevice();

	///
	virtual	MStatus			openDevice();
	///
	virtual	void			closeDevice();

	///
	virtual	void			nameAxes();
	///
	virtual void			nameButtons();
	///
	virtual	MDeviceState* 	deviceState();
	///
#ifdef SGI
	virtual	MDeviceState* 	deviceState( MDevent& );
#endif // SGI

	///
	virtual MStatus 		sendMessage(	const char* const messageType,
											const char* const messageParams );
	///
	virtual char* 			getMessage(	const char* const messageType,
										char* messageResponse );

	///
	virtual void 			doButtonEvents( bool = true );
	///
	virtual void 			doMovementEvents( bool = true );

	///
	MPxMidiInputDevice( void * init );

protected:
	///
	MStatus		setNamedButton( MString &, unsigned short );
	///
	MStatus		addChannel( MDeviceChannel & );
	///
	MStatus 	setDegreesOfFreedom( int freedom );
	///
	MStatus 	setNumberOfButtons( int buttons );

private:
	void   setData( void* );
	virtual const char*	className() const;
	void * 	data;
	int 	degreesOfFreedom;
	int 	numberOfButtons;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxMidiInputDevice */
