
#ifndef _MPxSelectionContext
#define _MPxSelectionContext
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
// CLASS:    MPxSelectionContext
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxSelectionContext)
//
//  This is the base class for contexts.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MPxContext.h>
#include <maya/MPoint.h>

// *****************************************************************************

// DECLARATIONS

class MString;

// *****************************************************************************

// CLASS DECLARATION (MPxSelectionContext)

/// Base class for interative selection tools (OpenMayaUI)
/**

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MPxSelectionContext : public MPxContext
{
public:
	///
	MPxSelectionContext ();
	///
	virtual	~MPxSelectionContext ();

	///
	virtual MStatus		doPress ( MEvent & event );
	///
	virtual MStatus		doRelease ( MEvent & event );
	///
	virtual MStatus		doDrag ( MEvent & event );
	///
	virtual MStatus		doHold ( MEvent & event );
	///
	virtual MStatus 	helpStateHasChanged ( MEvent & );

	///
	virtual MStatus     addManipulator( const MObject & manipulator );
	///
	virtual MStatus     deleteManipulators();
	///
	MStatus				setImage( const MString & image, ImageIndex index );
	///
	MStatus				getImage( MString & image, ImageIndex index ) const;

protected:

	// Thise methods determine whether the tool will select
	// an object or operate on an object
	///
	bool        isSelecting();

	// Get the world space location of the selection point
	///
	MPoint 		startPoint();		// point where dragging started
	///
	MPoint 		lastDragPoint();	// preview drag point (both in WS)


	// Create an instance of a tool command for use in
	// this context.
	//
	///
	virtual MPxToolCommand *	newToolCommand();

	virtual const char*	className() const;

public:
	// Temporarily putting these virtual functions at the end
	///
	virtual void		abortAction();
	///
	virtual bool		processNumericalInput( const MDoubleArray &values,
											   const MIntArray &flags,
											   bool isAbsolute );
	///
	virtual bool		feedbackNumericalInput() const;
	///
	virtual MSyntax::MArgType	argTypeNumericalInput( unsigned index ) const;
	///
	virtual	void		getClassName( MString & name ) const;

private:

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxSelectionContext */
