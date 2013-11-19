
#ifndef _MFnKeyframeDeltaAddRemove
#define _MFnKeyframeDeltaAddRemove
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
// CLASS:    MFnKeyframeDeltaAddRemove
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnKeyframeDeltaAddRemove)
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

// CLASS DECLARATION (MFnKeyframeDeltaAddRemove)

/// Function set for the addition or removal keys on a curve.
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnKeyframeDeltaAddRemove : public MFnKeyframeDelta 
{

	declareMFn(MFnKeyframeDeltaAddRemove, MFnKeyframeDelta);

public:
	///
	enum DeltaType { 
		///
		kAdded, 
		///
		kRemoved,
		///
		kReplaced
	};

	///
	DeltaType	deltaType( MStatus *ReturnStatus = NULL ) const;
	///
	double		value( MStatus *ReturnStatus = NULL ) const;
	///
	MTime		time( MStatus *ReturnStatus = NULL ) const;
	///
	virtual unsigned keyIndex( MStatus * ReturnStatus = NULL ) const; 
	///
	double		replacedValue( MStatus *ReturnStatus = NULL ) const;

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
#endif /* _MFnKeyframeDeltaAddRemove */
