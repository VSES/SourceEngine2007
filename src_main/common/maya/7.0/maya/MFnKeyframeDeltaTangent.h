
#ifndef _MFnKeyframeDeltaTangent
#define _MFnKeyframeDeltaTangent
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
// ==========================================================================
//
// *****************************************************************************
//
// CLASS:    MFnKeyframeDeltaTangent
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnKeyframeDeltaTangent)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnKeyframeDelta.h>
#include <maya/MFnAnimCurve.h> 

// *****************************************************************************

// DECLARATIONS
class MTime; 

// *****************************************************************************

// CLASS DECLARATION (MFnKeyframeDeltaTangent)

/// Function set for changes to a key's tangent.
/**
Any changes to a key's tangent value is accessed through this function set.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnKeyframeDeltaTangent : public MFnKeyframeDelta 
{

	declareMFn(MFnKeyframeDeltaTangent, MFnKeyframeDelta);

public:

	/// 
	MFnAnimCurve::TangentType	previousTangentType( MStatus *ReturnStatus = NULL ) const;
	/// 
	MFnAnimCurve::TangentType	currentTangentType( MStatus *ReturnStatus = NULL ) const; 

	///
	MStatus		getPreviousPosition( float &x, float &y ) const;
	/// 
	MStatus		getCurrentPosition( float &x, float &y ) const; 
	///
	bool		isInTangent( MStatus *ReturnStatus = NULL  ) const;

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
#endif /* _MFnKeyframeDeltaTangent */
