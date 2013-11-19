
#ifndef _MCursor
#define _MCursor
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
// CLASS:    MCursor
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MCursor)
//
//  The MCursor class implements a cursor class, and is used to pass
//  all cursor arguments to Maya API methods.
//  The data passed to the MCursor constructor is in xbm format.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MCursor)

/// Manipulate Cursors. (OpenMayaUI)
/**
 Implement a cursor.
*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MCursor
{
public:
#ifdef _WIN32
	/// NT constructor
				MCursor( LPCSTR pszResource );
#else
	/// IRIX constructor
				MCursor(short width,
						short height,
						short hotSpotX,
						short hotSpotY,
						unsigned char * bits,
						unsigned char * mask );
#endif
	///
				MCursor(const MCursor& other);
	///
	        	~MCursor();

	///
	MCursor &	operator=(const MCursor &);
	///
	bool		operator==(const MCursor &) const;
	///
	bool		operator!=(const MCursor &) const;
	/// Maya default cursor, the left arrow.
	static		MCursor	defaultCursor;
	/// '+' cursor.
	static		MCursor	crossHairCursor;
	/// '+' cursor with double lines.
	static		MCursor	doubleCrossHairCursor;
	/// Wedge-shaped arrow pointing left.
	static		MCursor	editCursor;
	/// Pencil shape.
	static		MCursor	pencilCursor;
	/// Open hand shaped cursor.
	static		MCursor	handCursor;

protected:
// No protected members

private:
				MCursor();
				MCursor(const void *);
	const void*	apiData;
	const void* apiData2;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32


// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MCursor */
