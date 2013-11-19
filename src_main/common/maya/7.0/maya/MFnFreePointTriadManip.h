
#ifndef _MFnFreePointTriadManip
#define _MFnFreePointTriadManip
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
// CLASS:    MFnFreePointTriadManip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnFreePointTriadManip)
//
// Function set for a base manipulator - the FreePointTriadManip.
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

// CLASS DECLARATION (MFnFreePointTriadManip)

/// FreePointTriadManip function set (OpenMayaUI)
/**
MFnFreePointTriadManip is the function set for free point triad manipulators.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MFnFreePointTriadManip : public MFnManip3D
{
	declareDagMFn(MFnFreePointTriadManip, MFnManip3D);

public:
	///
	enum ManipPlane {
		/// Y-Z Plane
		kYZPlane = 0,
		/// X-Z Plane
		kXZPlane,
		/// X-Y Plane
		kXYPlane,
		/// View Plane
		kViewPlane
	};

	///
	MObject		create(MStatus *ReturnStatus = NULL);
	///
	MObject		create(const MString &manipName,
					   const MString &pointName,
					   MStatus *ReturnStatus = NULL);
	///
	MStatus		connectToPointPlug(MPlug &pointPlug);
	///
	MStatus		setDrawAxes(bool state);
	///
	MStatus		setSnapMode(bool state);
	///
	MStatus		setKeyframeAll(bool state);
	///
	MStatus		setDrawArrowHead(bool state);
	///
	MStatus		setGlobalTriadPlane(ManipPlane whichPlane);
	///
	MStatus		setPoint(const MPoint& pointValue);
	///
	bool		isDrawAxesOn(MStatus *ReturnStatus = NULL) const;
	///
	bool		isSnapModeOn(MStatus *ReturnStatus = NULL) const;
	///
	bool		isKeyframeAllOn(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	pointIndex(MStatus *ReturnStatus = NULL) const;

protected:

private:

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnFreePointTriadManip */
