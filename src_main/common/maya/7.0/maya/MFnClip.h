
#ifndef _MFnClip
#define _MFnClip
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
// CLASS:    MFnClip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnClip)
//
// MFnClip is the function set for clips.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
// *****************************************************************************

// DECLARATIONS
class MObject;
class MTime;
class MDGModifier;
class MIntArray;

// *****************************************************************************

// CLASS DECLARATION (MFnClip)

/// clip function set (OpenMayaAnim)
/**
MFnClip is the function set for creating, querying and
editing animation clips.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnClip : public MFnDependencyNode
{
	declareMFn(MFnClip, MFnDependencyNode);

public:
	///
	MObject		createSourceClip ( const MTime& sourceStart,
								   const MTime& sourceDuration,
								   MDGModifier& dgMod,
								   MStatus * ReturnStatus = NULL );
	///
	MObject		createInstancedClip ( MObject& sourceClip,
									  const MTime& start,
									  MDGModifier& dgMod,
									  MStatus * ReturnStatus = NULL,
									  bool absolute = 1,
									  double postCycle = 0.0,
									  double weight = 1.0,
									  double scale = 1.0,
			  						  double preCycle = 0.0 );
	///
	bool 		isInstancedClip(MStatus *ReturnStatus = NULL);
	///
	bool		isPose(MStatus *ReturnStatus = NULL);
	///
	MObject 	sourceClip(MStatus *ReturnStatus = NULL);
	///
	double		getPreCycle(MStatus *ReturnStatus = NULL);
	///
	double		getPostCycle(MStatus *ReturnStatus = NULL);
	/// OBSOLETE
	double		getCycle(MStatus *ReturnStatus = NULL);
	///
	double		getWeight(MStatus *ReturnStatus = NULL);
	///
	double 		getScale(MStatus *ReturnStatus = NULL);
	/// OBSOLETE
	bool 		getAbsolute(MStatus *ReturnStatus = NULL);
	///
	bool 		getEnabled(MStatus *ReturnStatus = NULL);
	///
	MTime		getStartFrame(MStatus *ReturnStatus = NULL);
	///
	MTime		getSourceStart(MStatus *ReturnStatus = NULL);
	///
	MTime		getSourceDuration(MStatus *ReturnStatus = NULL);
	///
	MStatus		getMemberAnimCurves(MObjectArray& curves,
									MPlugArray& associatedAttrs);
	///
	MStatus		getAbsoluteChannelSettings(MIntArray& absoluteChannels);
	///
	int			getTrack(MStatus *ReturnStatus = NULL);
	///
	MStatus		setPoseClip(bool state, MDGModifier* mod = NULL);
	///
	MStatus		setPreCycle(double cycle, MDGModifier* mod = NULL);
	///
	MStatus		setPostCycle(double cycle, MDGModifier* mod = NULL);
	/// OBSOLETE
	MStatus		setCycle(double cycle, MDGModifier* mod = NULL);
	///
	MStatus		setWeight(double wt, MDGModifier* mod = NULL);
	///
	MStatus		setScale(double scale, MDGModifier* mod = NULL);
	/// OBSOLETE
	MStatus		setAbsolute(bool abs, MDGModifier* mod = NULL);
	///
	MStatus		setEnabled(bool val, MDGModifier* mod = NULL);
	///
	MStatus		setStartFrame(const MTime& start, MDGModifier* mod = NULL);
	///
	MStatus		setSourceData(const MTime& start,const MTime& duration,
							  MDGModifier* mod = NULL);
	///
	MStatus		setTrack(int index, MDGModifier* mod = NULL);
	///
	MStatus		setAbsoluteChannelSettings(const MIntArray& absoluteChannels,
										   MDGModifier* mod = NULL);
protected:
// No protected members

private:

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnClip */
