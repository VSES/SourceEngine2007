
#ifndef _MDeviceChannel
#define _MDeviceChannel
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
// CLASS:    MDeviceChannel
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDeviceChannel)
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MDeviceState;
class MString;

// *****************************************************************************

// CLASS DECLARATION (MDeviceChannel)

/// Input device channel (OpenMayaUI)
/**


*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MDeviceChannel
{
public:
	///
	MDeviceChannel( const MString&, MDeviceChannel* = NULL, int = -1 );

	///
	~MDeviceChannel();


	///
	MString 			name() const;
	///
	MString 			longName() const;

	///
	int				axisIndex() const;

	///
	bool     			hasChildren() const;
	///
	MDeviceChannel	 	parent() const;
	///
	MDeviceChannel 		childByIndex( int );
	///
	int					numChildren() const;

protected:
	// no protected members

private:
	friend class MPxMidiInputDevice;
	MDeviceChannel( void * );
	void * data;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDeviceChannel */
