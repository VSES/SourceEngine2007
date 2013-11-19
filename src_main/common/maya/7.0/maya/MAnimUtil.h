
#ifndef _MAnimUtil
#define _MAnimUtil
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
// CLASS:    MAnimUtil
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAnimUtil)
//
// MAnimUtil is a static class which provides helper methods for determining
// whether or not an object is animated, which attributes for the object are
// animated, and finally there is a method which will find all the animation
// curves for a given attribute.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MPlug;
class MPlugArray;
class MObjectArray;
class MSelectionList;
class MDagPath;

// *****************************************************************************

// CLASS DECLARATION (MAnimUtil)

/// Static class providing common animation helper methods (OpenMayaAnim)

class OPENMAYAANIM_EXPORT MAnimUtil
{
public:
	///
	static bool		isAnimated(	const MObject &node,
								bool checkParent = false,
								MStatus * ReturnStatus = NULL );
	///
	static bool		isAnimated(	const MDagPath &path,
								bool checkParent = false,
								MStatus * ReturnStatus = NULL );
	///
	static bool		isAnimated(	const MPlug &plug,
								bool checkParent = false,
								MStatus * ReturnStatus = NULL );
	///
	static bool		isAnimated(	const MSelectionList &selectionList,
								bool checkParent = false,
								MStatus * ReturnStatus = NULL );
	///
	static bool		findAnimatedPlugs(	const MObject &node,
										MPlugArray &animatedPlugs,
										bool checkParent = false,
										MStatus * ReturnStatus = NULL );
	///
	static bool		findAnimatedPlugs(	const MDagPath &path,
										MPlugArray &animatedPlugs,
										bool checkParent = false,
										MStatus * ReturnStatus = NULL );
	///
	static bool		findAnimatedPlugs(	const MSelectionList &selectionList,
										MPlugArray &animatedPlugs,
										bool checkParent = false,
										MStatus * ReturnStatus = NULL );
	///
	static bool		findAnimation(	const MPlug &plug,
									MObjectArray &animation,
									MStatus * ReturnStatus = NULL );
protected:
	static const char* className();
private:
	~MAnimUtil();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif
};

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAnimUtil */
