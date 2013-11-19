
#ifndef _MFnKeyframeDeltaBlockAddRemove
#define _MFnKeyframeDeltaBlockAddRemove
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
// CLASS:    MFnKeyframeDeltaBlockAddRemove
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnKeyframeDeltaBlockAddRemove)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>

// *****************************************************************************

// DECLARATIONS
class MTimeArray;
class MDoubleArray; 
class MTime;

// *****************************************************************************

// CLASS DECLARATION (MFnKeyframeDeltaBlockAddRemove)

/// Function set for block add or removal of keys. 
/**
   Certain keyframe editing operations work on group of keys rather than 
   an single key. These operations require this function set to 
   retrieve changes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnKeyframeDeltaBlockAddRemove : public MFnKeyframeDelta 
{

	declareMFn(MFnKeyframeDeltaBlockAddRemove, MFnKeyframeDelta);

public:
	
	///
	enum DeltaType { 
		///
		kAdded,
		///
		kRemoved
	}; 
	
	///
	DeltaType	deltaType( MStatus *ReturnStatus = NULL ) const;
	///
	MTime		startTime( MStatus *ReturnStatus = NULL ) const;
	///
	MTime		endTime( MStatus *ReturnStatus = NULL ) const;
	///
	unsigned	numKeys( MStatus *ReturnStatus = NULL ) const;

	///
	void		getValues( MDoubleArray &values, MStatus *ReturnStatus = NULL ) const;
	///
	void		getTimes( MTimeArray &times, MStatus *ReturnStatus = NULL ) const;

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
#endif /* _MFnKeyframeDeltaBlockAddRemove */
