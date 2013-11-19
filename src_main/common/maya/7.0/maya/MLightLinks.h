#ifndef _MLightLinks
#define _MLightLinks
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
// CLASS:    MLightLinks
//
// ***************************************************************************
//
// CLASS DESCRIPTION (MLightLinks)
//
// MLightLinks is a class which provides access to Maya's light linking 
// information.  It parses the information expressed by the light linker nodes
// in the scene, and provides a simple queryable interface for it.
//
// ***************************************************************************

#if defined __cplusplus

// ***************************************************************************
// INCLUDED HEADER FILES

#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MIOStream.h>

// ***************************************************************************
// CLASS DECLARATION (MLightLinks)

/// Class providing read-only Light Linking API functions (OpenMayaRender)
/**
*/
class OPENMAYARENDER_EXPORT MLightLinks
{
public:

	///
	MLightLinks();

	///
	~MLightLinks();

	///
	MStatus parseLinks( const MObject& linkNode = MObject::kNullObj, 
						bool verbose = false, 
						IOS_REF(ostream) *stream = NULL,
						bool useIgnore = false );

	///
	MStatus getLinkedLights( const MDagPath& path, 
							 const MObject& component, 
							 MDagPathArray& lights );

	///
	MStatus getLinkedObjects( const MDagPath& light, 
							  MSelectionList& objects );

	///
	MStatus getIgnoredLights( const MDagPath& path, 
							  const MObject& component, 
							  MDagPathArray& lights );

	///
	MStatus getIgnoredObjects( const MDagPath& light, 
							   MSelectionList& objects );

private:
	void *f_linkUtil;
};


// ***************************************************************************
#endif /* __cplusplus */
#endif /* _MLightLinks */
