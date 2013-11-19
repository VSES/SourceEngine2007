
#ifndef _MFnKeyframeDeltaMove
#define _MFnKeyframeDeltaMove
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
//
// *****************************************************************************
//
// CLASS:    MFnKeyframeDeltaMove
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnKeyframeDeltaMove)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>

// *****************************************************************************

// DECLARATIONS
class MTime;

// *****************************************************************************

// CLASS DECLARATION (MFnKeyframeDeltaMove)

/// Function set for change in keyframe value or time.
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnKeyframeDeltaMove : public MFnKeyframeDelta 
{

	declareMFn(MFnKeyframeDeltaMove, MFnKeyframeDelta);

public:

	/// 
	MTime		previousTime( MStatus * ReturnStatus = NULL ) const;
	///
	MTime		currentTime( MStatus * ReturnStatus = NULL ) const;
	///
	double		previousValue( MStatus * ReturnStatus = NULL ) const; 
	///
	double		currentValue( MStatus * ReturnStatus = NULL ) const;	

	///
	unsigned	previousIndex( MStatus * ReturnStatus = NULL ) const;
	///
	virtual unsigned keyIndex( MStatus * ReturnStatus = NULL ) const; 

protected:
	// No protected.

private:
	// No private. 
};

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnKeyframeDeltaMove */
