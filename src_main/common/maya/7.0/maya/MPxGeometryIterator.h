
#ifndef _MPxGeometryIterator
#define _MPxGeometryIterator
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
// CLASS:    MPxGeometryIterator
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxGeometryIterator)
//
//  This is the base class for user defined geometry iterators.
//  Geometry iterator allow iterating over components in geometry
//  in a geometry independent manner. This base class defines
//  the interface to be used maya when a generic component iteration
//  is required.
//
//  This class is used in conjunction with MPxSurfaceShape to provide
//  an iterator for components in a user defined shape.
//  The main methods to override in this class are point and set point.
//  The reset, isDone, and next methods have a default implementation
//  and should only be overridden if the component being iterator on
//  is not a single indexed component type.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS
 
class MPxSurfaceShape;
class MPoint;
class MObjectArray;
class TcomponentList;

// *****************************************************************************

// CLASS DECLARATION (MPxGeometryIterator)

/// the base class for user defined geometry iterators
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPxGeometryIterator
{
public:
    ///
	MPxGeometryIterator( void * userGeometry, MObjectArray & components );
	///
	MPxGeometryIterator( void * userGeometry, MObject & components );
    ///
	virtual ~MPxGeometryIterator();

	/////////////////////////////////////////////////////////////////////////
	// Methods to overload

	// Item by item traversal methods
	//
	///
	virtual bool			isDone() const;
	///
	virtual void			next();
	///
	virtual void			reset();

	// component for Item 
	///
	virtual void			component( MObject &component );

	// Item's point info access methods
	//
	///
	virtual bool			hasPoints() const;
	///
	virtual int				iteratorCount() const;
	///
	virtual MPoint			point() const;
	///
	virtual void			setPoint( const MPoint & ) const;
	///
	virtual int				setPointGetNext( MPoint & );
	///
	virtual int				index() const;


	///
	virtual bool			hasNormals() const;

	///
	virtual int				indexUnsimplified() const;

	////////////////////////////////////////////////////////////////////////

	// Methods to access the iterator values.
	// These values are used when iterating over all components
	// of a shape when there are no components specified.
	///
	int						currentPoint() const;
	///
	void					setCurrentPoint( int );
	///
	int						maxPoints() const;
	///
	void					setMaxPoints( int );

		

	//	Method to (optionally) set object (to allow tweaking of history)
	//
	///
	void					setObject( MPxSurfaceShape & );

	///
	void*					geometry() const;
	
protected:
	  
private:
	static const char*	    className();
	void * instance;
	TcomponentList			*compList;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxGeometryIterator */
