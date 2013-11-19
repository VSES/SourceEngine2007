
#ifndef _MFnPointOnCurveManip
#define _MFnPointOnCurveManip
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
// CLASS:    MFnPointOnCurveManip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnPointOnCurveManip)
//
// Function set for a base manipulator - the PointOnCurveManip.
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

// *****************************************************************************

// CLASS DECLARATION (MFnPointOnCurveManip)

/// PointOnCurveManip function set (OpenMayaUI)
/**
MFnPointOnCurveManip is the function set for point on curve manipulators.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MFnPointOnCurveManip : public MFnManip3D
{
	declareDagMFn(MFnPointOnCurveManip, MFnManip3D);

public:
	///
	MObject		create(MStatus *ReturnStatus = NULL);
	///
	MObject		create(const MString &manipName,
					   const MString &paramName,
					   MStatus *ReturnStatus = NULL);
	///
	MStatus		connectToCurvePlug(MPlug &curvePlug);
	///
	MStatus		connectToParamPlug(MPlug &paramPlug);
	///
	MStatus		setDrawCurve(bool state);
	///
	MStatus		setParameter(double parameter);
	///
	bool		isDrawCurveOn(MStatus *ReturnStatus = NULL) const;
	///
	double		parameter(MStatus *ReturnStatus = NULL) const;
	///
	MPoint		curvePoint(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	curveIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	paramIndex(MStatus *ReturnStatus = NULL) const;

protected:

private:

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnPointOnCurveManip */
