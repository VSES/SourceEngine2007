
#ifndef _MAnimCurveClipboard
#define _MAnimCurveClipboard
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
// CLASS:    MAnimCurveClipboard
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAnimCurveClipboard)
//
//	The clipboard is a list of MAnimCurveClipboardItems (i.e. an
//	MAnimCurveClipboardItemArray).
//
//	The items on the clipboard must be ordered.  In the case where the
//	clipboard info represents animation of a hierarchy, the order in which
//	items appear in the clipboard is reliant on a depth-first-iteration from
//	the root or the hierarchy.  This ordering, is essential to properly match
//	up hierarchies of objects
//
//	As an example, consider that animation from the following hierarchy
//	is placed into the clipboard.  (Dag Objects with "/" for sides
//	are animated, as in the Hypergraph)
//	        ____
//	       / A /
//	       /___/\_______
//	         |\_____    \__
//	        ____   _|__   _|__
//	       | B |  / E /  | F |
//	       |___|  /___/  |___|
//	         |             |\________
//	         ____          ____      _|__
//	        / C /         / G /     / H /
//	        /___/         /___/     /___/
//	         |
//	        ____
//	       / D /
//	       /___/
//
//	In this example, the object labelled "C" has translate{X,Y,Z} animated,
//	while all the others only have one animated attribute (excluding objects
//	B and F, which have no animated attributes).
//
//	Using notation where A(r,c,a) represents the animCurve driving object
//	"A", which is at row "r" in its subhierarchy, has "c" children, and
//	"a" animated attributes these items would appear in the clipboard in this
//	order (i.e. depth-first with each object's attributes explicitly indexed
//	before continuing down the heirarchy):
//
//	A(0,3,0) , B(1,1,0) , C(2,1,0) , C(2,1,1) , C(2,1,2) , D(3,0,0) ,
//	E(1,0,0) , F(1,2,0) , G(2,0,0) , H(2,0,0)
//
//	For example C(2,1,2) would mean that the object C resides on the second
//	level of the subheirarchy and has one child.  The last "2" is simply
//	used as an index to count the number of animated attributes on this
//	object.
//
//	Multiple objects can be represented on the clipboard in this manner.  In
//	the example above, if we had a separate second object with no children,
//	"J", it would appear at the end of the array as J(0,0,0).
//
//	Note that although B and F contain no animation data themselves, they
//	must still be placed on the clipboard as placeholders to preserve the
//	heirarchy information.  A placeholder object is defined by a NULL value
//	for the MAnimCurveClipboardItem's animCurve.
//
//	There is a special clipboard that remains static.  It can be accessed by
//	MAnimCurveClipboard::theAPIClipboard().
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MAnimCurveClipboardItemArray.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MAnimCurveClipboard)

/// Control over the animation clipboard (OpenMayaAnim)
/**
	This class provide access to the animation or API clipboard which are used
	to hold on to anim curves during cut/copy/paste operations.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MAnimCurveClipboard
{
public:
	///
										MAnimCurveClipboard();
	///
										~MAnimCurveClipboard();

	///
	// For accessing the static API clipboard
	static MAnimCurveClipboard &		theAPIClipboard();
	///
	MStatus		set( const MAnimCurveClipboard &cb );
	///
	MStatus		set( const MAnimCurveClipboardItemArray &clipboardItemArray );
	///
	MStatus		set( const MAnimCurveClipboardItemArray &clipboardItemArray,
					 const MTime &startTime, const MTime &endTime,
					 const float &startUnitlessInput,
					 const float &endUnitlessInput );
	///
	MStatus		clear ();

	///
	bool		isEmpty( MStatus * ReturnStatus = NULL ) const;
	///
	const MAnimCurveClipboardItemArray 	clipboardItems( MStatus *
												ReturnStatus = NULL) const;
	///
	MTime		startTime( MStatus * ReturnStatus = NULL ) const;
	///
	MTime		endTime( MStatus * ReturnStatus = NULL ) const;
	///
	float		startUnitlessInput( MStatus * ReturnStatus = NULL ) const;
	///
	float		endUnitlessInput( MStatus * ReturnStatus = NULL ) const;


protected:
// No protected members

private:
	void *							fClipboard;

	static MAnimCurveClipboard		fsAPIClipboard;

	MAnimCurveClipboard& operator = (const MAnimCurveClipboard&) const;
	MAnimCurveClipboard& operator = (const MAnimCurveClipboard&);
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAnimCurveClipboard */
