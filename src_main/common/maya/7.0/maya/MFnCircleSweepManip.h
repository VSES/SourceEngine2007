
#ifndef _MFnCircleSweepManip
#define _MFnCircleSweepManip
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
// CLASS:    MFnCircleSweepManip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnCircleSweepManip)
//
// Function set for a base manipulator - the CircleSweepManip
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

class MPoint;
class MVector;
class MAngle;

// *****************************************************************************

// CLASS DECLARATION (MFnCircleSweepManip)

/// CircleSweepManip function set (OpenMayaUI)
/**
MFnCircleSweepManip is the function set for circle sweep manipulators.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MFnCircleSweepManip : public MFnManip3D
{
	declareDagMFn(MFnCircleSweepManip, MFnManip3D);

public:
	///
	MObject		create(MStatus *ReturnStatus = NULL);
	///
	MObject		create(const MString &manipName,
					   const MString &angleName,
					   MStatus *ReturnStatus = NULL);
	///
	MStatus		connectToAnglePlug(MPlug &anglePlug);
	///
	MStatus		setCenterPoint(const MPoint &centerPoint);
	///
	MStatus		setStartPoint(const MPoint &startPoint);
	///
	MStatus		setEndPoint(const MPoint &endPoint);
	///
	MStatus		setNormal(const MVector &normal);
	///
	MStatus		setRadius(double radius);
	///
	MStatus		setAngle(MAngle angle);
	///
	MStatus		setDrawAsArc(bool state);
	///
	MPoint		startPoint(MStatus *ReturnStatus = NULL) const;
	///
	MPoint		endPoint(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	centerIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	axisIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	angleIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	startCircleIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	endCircleIndex(MStatus *ReturnStatus = NULL) const;

protected:

private:

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnCircleSweepManip */
