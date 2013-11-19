
#ifndef _MFnDistanceManip
#define _MFnDistanceManip
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
// CLASS:    MFnDistanceManip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnDistanceManip)
//
// Function set for a base manipulator - the DistanceManip
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MFnManip3D.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnDistanceManip)

/// DistanceManip function set (OpenMayaUI)
/**
MFnDistanceManip is the function set for distance manipulators.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MFnDistanceManip : public MFnManip3D
{
	declareDagMFn(MFnDistanceManip, MFnManip3D);

public:
	///
	MObject		create(MStatus *ReturnStatus = NULL);
	///
	MObject		create(const MString &manipName,
					   const MString &distanceName,
					   MStatus *ReturnStatus = NULL);
	///
	MStatus		connectToDistancePlug(MPlug &distancePlug);
	///
	MStatus		setStartPoint(const MPoint &point);
	///
	MStatus		setDirection(const MVector &vector);
	///
	MStatus		setDrawStart(bool state);
	///
	MStatus		setDrawLine(bool state);
	///
	MStatus		setScalingFactor(double scalingFactor);
	///
	bool		isDrawStartOn(MStatus *ReturnStatus = NULL) const;
	///
	bool		isDrawLineOn(MStatus *ReturnStatus = NULL) const;
	///
	double		scalingFactor(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	distanceIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	directionIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	startPointIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	currentPointIndex(MStatus *ReturnStatus = NULL) const;

protected:
// No protected members

private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnDistanceManip */
