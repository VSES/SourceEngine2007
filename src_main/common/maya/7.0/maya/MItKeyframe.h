
#ifndef _MItKeyframe
#define _MItKeyframe
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
// CLASS:    MItKeyframe
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItKeyframe)
//
//	In Maya Animation Curves (Anim Curve) are implemented as Dependency Graph
//	(DG) Nodes.  Each Node has multiple ordered and indexed keyframes.
//
//	The Keyframe Iterator provides methods for iterating over, querying and
//	editing the keyframes of a Anim Curve Node.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFn.h>
#include <maya/MStatus.h>
#include <maya/MTime.h>

// *****************************************************************************

// DECLARATIONS

class MObject;
class MPtrBase;

// *****************************************************************************

// CLASS DECLARATION (MItKeyframe)

///  Keyframe Iterator (OpenMayaAnim)
/**

Iterate over the keyframes of a particular Anim Curve Node, and query
and edit the keyframe to which the iterator points.

Determine the time and value of the keyframe, as well as the x,y values
and type of the tangent to the curve entering (in tangent) and leaving
(out tangent) the keyframe.

Set the time and value of the keyframe, and the type of the tangents.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MItKeyframe {
public:
	///
	enum TangentType {
		///
		kTangentGlobal = 0,
		///
		kTangentFixed,
		///
		kTangentLinear,
		///
		kTangentFlat,
		///
		kTangentSmooth,
		///
		kTangentStep,
		///
		kTangentSlow,
		///
		kTangentFast,
		///
		kTangentClamped,
		///
		kTangentPlateau,
        ///
        kTangentStepNext
	};
	///
	MItKeyframe( MObject & animCurveNode, MStatus * ReturnStatus = NULL );
	///
	~MItKeyframe();
	///
	MStatus     reset( MObject & animCurveNode );
	///
	MStatus     reset();
	///
	MStatus     next();
	///
	bool        isDone( MStatus * ReturnStatus = NULL );
	///
	MTime       time( MStatus * ReturnStatus = NULL );
	///
	MStatus     setTime( MTime time );
	///
	double      value( MStatus * ReturnStatus = NULL );
	///
	MStatus     setValue( double value );
	///
	TangentType inTangentType( MStatus * ReturnStatus = NULL );
	///
	TangentType outTangentType( MStatus * ReturnStatus = NULL );
	///
	MStatus     setInTangentType( TangentType tangentType );
	///
	MStatus     setOutTangentType( TangentType tangentType );
	///
	MStatus     getTangentOut( float &x, float &y );
	///
	MStatus     getTangentIn( float &x, float &y );
	///
	bool		tangentsLocked( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus		setTangentsLocked( bool locked );

protected:
// No protected members

private:
	static const char* className();
    MPtrBase *        f_ptr;
	unsigned          f_index;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItKeyframe */
