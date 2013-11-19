
#ifndef _MFnDynSweptGeometryData
#define _MFnDynSweptGeometryData
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
// CLASS:    MFnDynSweptGeometryData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnDynSweptGeometryData)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnData.h>

// *****************************************************************************

// DECLARATIONS

class MDynSweptLine;
class MDynSweptTriangle;

// *****************************************************************************

// CLASS DECLARATION (MFnDynSweptGeometryData)

/// Swept Geometry function set for dependency node data (OpenMayaFX)
/**
	Access MDynSweptLine and MDynSweptTriangle dependency node data
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MFnDynSweptGeometryData : public MFnData
{

	declareMFn(MFnDynSweptGeometryData, MFnData);

public:
	///
	int					lineCount( MStatus* ReturnStatus = NULL ) const;
	///
	int					triangleCount( MStatus* ReturnStatus = NULL ) const;
	///
	MDynSweptLine		sweptLine( int index,
							MStatus* ReturnStatus = NULL ) const;
	///
	MDynSweptTriangle	sweptTriangle( int index,
							MStatus* ReturnStatus = NULL ) const;

	///
	MObject				create( MStatus* ReturnStatus=NULL );

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
#endif /* _MFnDynSweptGeometryData */
