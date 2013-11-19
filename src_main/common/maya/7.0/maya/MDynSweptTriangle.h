
#ifndef _MDynSweptTriangle
#define _MDynSweptTriangle
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
// CLASS:    MDynSweptTriangle
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDynSweptTriangle)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MVector;
class MPoint;

// *****************************************************************************

// CLASS DECLARATION (MDynSweptTriangle)

/// Class for evaluating surfaces as triangles over time. (OpenMayaFX)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MDynSweptTriangle
{
public:
	///
					MDynSweptTriangle();
	///
					~MDynSweptTriangle();

	// Time-Dependent Access Methods
	//
	//
	///
	MVector			vertex( int vertexId, double t = 1.0 );

	///
	MVector			normal( double t = 1.0 );

	///
	MVector 		normalToPoint(double x, double y, double z, double t = 1.0);

	// Non-Time-Dependent Access Method
	//
	//
	///
	MVector			uvPoint( int vertexId );

	///
	double			area();

protected:
// No protected members

private:
	static const char* className();
	MDynSweptTriangle( void* );
	void * fData;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDynSweptTriangle */
