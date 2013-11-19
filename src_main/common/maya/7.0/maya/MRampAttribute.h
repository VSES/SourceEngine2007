#ifndef _MRampAttribute
#define _MRampAttribute
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
// CLASS:    MRampAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MRampAttribute)
//
// This class is used to manipulate ramp attributes.  You can add, delete, and set
// entries in both color and curve ramps.
// You can't create a ramp attribute from this class, you can only modifying existing 
// ramps.
//
// *****************************************************************************

#if defined __cplusplus

// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MDGContext.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

class MString;
class MStatus;
class MPlug;
class MColor;
class MFloatArray;
class MColorArray;
class MIntArray;
class MObject;

// CLASS DECLARATION (MRampAttribute)


#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

/// Ramp data attribute function wrapper class
/**
  Wrapper class for ramp data attributes.
*/

class OPENMAYA_EXPORT MRampAttribute 
{

public:

	///
	MRampAttribute ();
	///
	MRampAttribute (const MRampAttribute& other);

	///
	MRampAttribute (const MPlug& obj, MStatus* ReturnStatus = NULL);

	///
	MRampAttribute (const MObject& node, const MObject& attr, MStatus* ReturnStatus = NULL);

	///
	MRampAttribute&    operator =( const MRampAttribute & other );

	///
	~MRampAttribute ();

	///
	enum MInterpolation	{	
	///
		kLinear = 0,
	///
		kNone = 1,
	///
		kSpline = 2,
	///
		kSmooth = 3
	};

	///
	unsigned int getNumEntries (MStatus * returnStatus = NULL);
	///
	void getEntries (MIntArray& indexes, 
		MFloatArray& positions,
		MFloatArray& values, 
		MIntArray& interps, 
		MStatus * returnStatus = NULL);
	///
	void getEntries (MIntArray& indexes, 
		MFloatArray& positions, 
		MColorArray& colors, 
		MIntArray& interps, 
		MStatus * returnStatus = NULL);
	///
	void addEntries (MFloatArray& positions, 
		MColorArray& values, 
		MIntArray& interps, 
		MStatus * returnStatus = NULL);
	///
	void addEntries (MFloatArray& positions, 
		MFloatArray& values, 
		MIntArray& interps, 
		MStatus * returnStatus = NULL);
	///
	void deleteEntries (MIntArray& indexes, 
		MStatus * returnStatus = NULL);
	///
	void setColorAtIndex (MColor& color, unsigned int index, MStatus * returnStatus = NULL);
	///
	void setValueAtIndex (float value, unsigned int index, MStatus * returnStatus = NULL);
	///
	void setPositionAtIndex (float position, unsigned int index, MStatus * returnStatus = NULL);
	///
	void setInterpolationAtIndex (MRampAttribute::MInterpolation interp, unsigned int index, MStatus * returnStatus = NULL);
	///
	bool isColorRamp(MStatus * returnStatus = NULL);
	///
	bool isCurveRamp(MStatus * returnStatus = NULL);
	///
	void getColorAtPosition (float position, MColor& color, MStatus * returnStatus = NULL, MDGContext& = MDGContext::fsNormal);
	///
	void getValueAtPosition (float position, float& value, MStatus * returnStatus = NULL, MDGContext& = MDGContext::fsNormal);

	///
	static MObject createCurveRamp(MString& attrLongName, MString& attrShortName, MStatus* ReturnStatus = NULL);
	///
	static MObject createColorRamp(MString& attrLongName, MString& attrShortName, MStatus* ReturnStatus = NULL);

protected:
// No protected members


private:

	static const char* className();
	const void * fData;
	bool fOwn;


};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnRampDataAttribute */



