
#ifndef _MAnimCurveClipboardItem
#define _MAnimCurveClipboardItem

//
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
// CLASS:    MAnimCurveClipboardItem
//
//
//******************************************************************************
//
// CLASS DESCRIPTION (MAnimCurveClipboardItem)
//
//  This class provides a wrapper to the clipboard item used to hold
//  on to cut/copy/paste information
//
//******************************************************************************

#if defined __cplusplus

//******************************************************************************
// INCLUDED HEADER HILES
//******************************************************************************


#include <maya/MFnAnimCurve.h>
#include <maya/MObject.h>


//******************************************************************************
// DECLARATIONS
//******************************************************************************

//	CLASS DECLARATION (MAnimCurveClipboardItem)

///	Wrapper for a clipboard item. (OpenMayaAnim)
/**
	This class provides a wrapper to the clipboard item used to hold
	on to cut/copy/paste information
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MAnimCurveClipboardItem
{
public:
	///
					MAnimCurveClipboardItem();
	///
					MAnimCurveClipboardItem( const MAnimCurveClipboardItem &r);
	///
					~MAnimCurveClipboardItem();

	///
	const MObject		animCurve( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus				getAddressingInfo( unsigned &rowCount,
										   unsigned &childCount,
										   unsigned &attributeCount) const;
	///
	const MString &		fullAttributeName( MStatus * ReturnStatus=NULL ) const;
	///
	const MString &		leafAttributeName( MStatus * ReturnStatus=NULL ) const;
	///
	const MString &		nodeName( MStatus * ReturnStatus=NULL ) const;
	///
	MFnAnimCurve::AnimCurveType	animCurveType( MStatus
												* ReturnStatus=NULL ) const;
	///
	MStatus				setAnimCurve( const MObject & curve );
	///
	MStatus				setAddressingInfo( unsigned rowCount,
										   unsigned childCount,
										   unsigned attributeCount );
	///
	MStatus				setNameInfo( const MString & nodeName,
									 const MString & fullName,
									 const MString & leafName );
	///
	MAnimCurveClipboardItem &operator =  (const MAnimCurveClipboardItem &from);
	///
	bool	 				operator == (const MAnimCurveClipboardItem &rhs) const;

protected:
// No protected members

private:
	void *							fCurve;
	MFnAnimCurve::AnimCurveType		fAnimCurveType;

	unsigned						fRowCount;
	unsigned						fChildCount;
	unsigned						fAttrCount;
	MString							fFullAttrName;
	MString							fLeafAttrName;
	MString							fNodeName;
	static const char*				className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

//******************************************************************************
#endif /* __cplusplus */
#endif /* _MAnimCurveClipboardItem */
