
#ifndef _MFnMotionPath
#define _MFnMotionPath
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
// CLASS:    MFnMotionPath
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnMotionPath)
//
//	This class is used for constructing and manipulating motion path
//	animation.
//
//	Motion path animation requires a curve (or surface) and one or more other
//	objects.  During the animation, the objects will be moved along the curve.
//
//	Setting "follow" for the motion path aligns the object(s) local axis to	the
//	tangent of the motion path. Banking can also be applied to objects.
//
//	Motion path markers are points along the path where the orientation and
//	position for the object(s) travelling along the path can be specified.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
class MDagPath;
class MDagPathArray;
class MDGModifier;
class MTime;

// *****************************************************************************

// CLASS DECLARATION (MFnMotionPath)

/// Motion path animation function set (OpenMayaAnim)
/**
  Construct and manipulate motion paths
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnMotionPath : public MFnDependencyNode
{

	declareMFn(MFnMotionPath, MFnDependencyNode);

public:
	///
	enum Axis {
		///
		kXaxis,
		///
		kYaxis,
		///
		kZaxis
	};
	///
	MObject		create( const MDagPath & pathObject,
						const MDagPath & objectToAnimate,
						MTime & timeStart, MTime & timeEnd,
						MDGModifier * modifier = NULL,
						MStatus * ReturnStatus = NULL );
	///
	MStatus		setPathObject( const MDagPath & pathObject,
								 MDGModifier * modifier = NULL );
	///
	MDagPath	pathObject( MStatus * ReturnStatus = NULL );
	///
	MStatus		addAnimatedObject( const MDagPath & objectToAnimate,
									MDGModifier * modifier = NULL );
	///
	MStatus		getAnimatedObjects( MDagPathArray & array );
	///
	MStatus		setFollow( bool on, MDGModifier * modifier = NULL );
	///
	bool		follow( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus		setFollowAxis( Axis axis );
	///
	Axis		followAxis( MStatus * ReturnStatus = NULL );
	///
	MStatus		setUpAxis( Axis axis );
	///
	Axis		upAxis( MStatus * ReturnStatus = NULL );
	///
	MStatus		setBank( bool bank );
	///
	bool		bank( MStatus * ReturnStatus  = NULL) const;
	///
	MStatus		setBankScale( double bankScale );
	///
	double		bankScale( MStatus * ReturnStatus = NULL );
	///
	MStatus		setBankThreshold( double bankThreshold );
	///
	double		bankThreshold( MStatus * ReturnStatus = NULL );
	///
	MStatus		setUseNormal( bool use );
	///
	bool		useNormal( MStatus * ReturnStatus = NULL );
	///
	MStatus		setInverseNormal( bool invert );
	///
	bool		inverseNormal( MStatus * ReturnStatus = NULL );
	///
	MStatus		setUStart( double start );
	///
	MStatus		setUEnd( double end );

	///
	double		uStart( MStatus * ReturnStatus = NULL );
	///
	double		uEnd( MStatus * ReturnStatus = NULL );
	///
	MStatus		setUTimeStart( MTime & start );
	///
	MStatus		setUTimeEnd( MTime & end );
	///
	MTime		uTimeStart( MStatus * ReturnStatus = NULL );
	///
	MTime		uTimeEnd( MStatus * ReturnStatus = NULL );
	///
	unsigned	numPositionMarkers( MStatus * ReturnStatus = NULL );
	///
	MObject		getPositionMarker( unsigned, MStatus * ReturnStatus = NULL );
	///
	unsigned	numOrientationMarkers( MStatus * ReturnStatus = NULL );
	///
	MObject		getOrientationMarker( unsigned, MStatus * ReturnStatus = NULL );

protected:
// No protected members

private:
    void *         getCurve();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnMotionPath */
