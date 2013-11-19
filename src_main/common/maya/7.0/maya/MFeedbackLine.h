
#ifndef _MFeedbackLine
#define _MFeedbackLine
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
// CLASS:    MFeedbackLine
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFeedbackLine)
//
//  The MFeedbackLine is used to display information back to the user.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MStatus;
class MString;

// CLASS DECLARATION (MFeedbackLine)

/// Feedback line (OpenMayaUI)
/**
This class provides methods for displaying information to the user.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32


class OPENMAYAUI_EXPORT MFeedbackLine {
public:
	///
	static MStatus	setFormat		(const MString &format);
	///
	static MStatus	setTitle		(const MString &title);
	///
	static MStatus	setValue		(short index, double value);
	///
	static void		clear			();
	///
	static bool		showFeedback	();
	///
	static void		setShowFeedback	(bool showFeedback);

private:
	static const char *className();
};


#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFeedbackLine */
