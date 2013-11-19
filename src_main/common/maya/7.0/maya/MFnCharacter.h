
#ifndef _MFnCharacter
#define _MFnCharacter
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
// CLASS:    MFnCharacter
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnCharacter)
//
//  MFnCharacter is the function set that is used for manipulating characters.
//  Characters in Maya are dependency nodes used to manage any set of keyable
//  attributes. The attributes contained in a character can be manipulated
//  using clips, scheduled in a nonlinear manner by a clipScheduler node.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MString.h>
#include <maya/MFnSet.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MObjectArray;
class MSelectionList;
class MDagPath;
class TsetCmd;
class Tstring;
class MDGModifier;

// *****************************************************************************

// CLASS DECLARATION (MFnCharacter)

/// Function Set for Characters
/**
    Function set for characters
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnCharacter : public MFnSet
{
	declareMFn(MFnCharacter, MFnSet);

public:
	///
	MStatus		attachSourceToCharacter( MObject& sourceClip,
										 MDGModifier& dgMod);
	///
	MStatus		attachInstanceToCharacter( MObject& instanceClip,
										   MDGModifier& dgMod);
	///
	MStatus		addCurveToClip( MObject& curve,
								MObject& sourceClip,
								MPlug& characterPlug,
								MDGModifier& dgMod);
	///
	MObject		createBlend( MObject& instancedClip1,
							 MObject& instancedClip2,
							 MObject& blendAnimCurve,
							 MDGModifier& dgMod,
							 MStatus *ReturnStatus = NULL);
	///
	bool		blendExists(MObject& instancedClip1,
							MObject& instancedClip2,
							MObject& blendResult);
	///
	bool		removeBlend(MObject& instancedClip1,
							MObject& instancedClip2,
							MDGModifier& dgMod,
							MStatus* ReturnStatus = NULL);

	///
	bool	getCharacterThatOwnsPlug(MPlug& plug, MObject& result) const;
	///
	MStatus getMemberPlugs(MPlugArray& result) const;
	///
	MStatus getSubCharacters(MSelectionList& result) const;
	///
	MObject getClipScheduler(MStatus * ReturnStatus = NULL);
	///
	int 	getScheduledClipCount(MStatus * ReturnStatus = NULL);
	///
	MObject	getScheduledClip(int index, MStatus * ReturnStatus = NULL);
	///
	int 	getSourceClipCount(MStatus * ReturnStatus = NULL);
	///
	MObject	getSourceClip(int index, MStatus * ReturnStatus = NULL);
	///
    int		getBlendCount(MStatus * ReturnStatus = NULL);
	///
    MObject getBlend(int index, MStatus * ReturnStatus = NULL);
	///
    MStatus getBlendClips(int index, MObject& clip1, MObject& clip2);

protected:
	virtual		Tstring setCommandString();
	virtual		TsetCmd* setCommand();
private:
// No private members
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnCharacter */
