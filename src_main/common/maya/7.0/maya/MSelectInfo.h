
#ifndef _MSelectInfo
#define _MSelectInfo
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
// CLASS:    MSelectInfo
//
//   MSelectInfo is used with user defined shape selection and is passed
//   as an argument to the MPxSurfaceShapeUI::select method.
//   This class encapsulates all the selection state information for
//   selected objects.
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MSelectInfo)
//
// *****************************************************************************
//
#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MDrawInfo.h>

// *****************************************************************************

// DECLARATIONS

class MSelectionMask;
class MPoint;
class MPointArray;
class MVector;
class MSelectionList;
class MMatrix;

// *****************************************************************************

// CLASS DECLARATION (MSelectInfo)

/// Selection state information used in MPxSurfaceShapeUI::select (OpenMayaUI)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MSelectInfo : public MDrawInfo
{
public:
	///
	MSelectInfo();
	///
	MSelectInfo( const MSelectInfo& in );
	///
	~MSelectInfo();

public:

	// Public accessor methods

	///
    M3dView			view();

    ///
	bool			singleSelection() const;
    ///
	bool			selectClosest() const;
	///
    bool			selectable( MSelectionMask & mask ) const;
	///
    bool			selectableComponent( bool displayed,
										 MSelectionMask & mask ) const;

	///
    bool			isRay() const;
	///
	MMatrix			getAlignmentMatrix() const;
	///
    void			getLocalRay( MPoint&, MVector & ) const;

	///
    bool			selectForHilite( const MSelectionMask & ) const;

	///
    bool			selectOnHilitedOnly() const;

    ///
	int				highestPriority() const;
    ///
	void			setHighestPriority( int );

	///
    void			addSelection( const MSelectionList &item,
								  const MPoint &point,
								  MSelectionList &list,
								  MPointArray &points,
								  const MSelectionMask & mask,
								  bool isComponent );

	///
    MDagPath		selectPath() const;


protected:
// No protected members

private:
	const char*	 className() const;
    MSelectInfo( void* in );
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MSelectInfo */
