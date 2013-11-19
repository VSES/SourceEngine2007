
#ifndef _MFnNurbsCurve
#define _MFnNurbsCurve
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
// CLASS:    MFnNurbsCurve
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnNurbsCurve)
//
// Function set for NURBS curves.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MPointArray;
class MDoubleArray;
class MVector;
class MPoint;
class MDagPath;
class MPtrBase;
class TnurbsCurve;

#define kMFnNurbsEpsilon  1.0e-3

// *****************************************************************************

// CLASS DECLARATION (MFnNurbsCurve)

/// NURBS curve function set
/**
  Manipulate NURBS curve objects
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnNurbsCurve : public MFnDagNode 
{

	declareDagMFn(MFnNurbsCurve, MFnDagNode);

public: 
    ///
	enum Form {
        ///
		kInvalid=0,
        ///
		kOpen,
        ///
		kClosed, 
        ///
		kPeriodic,
        ///
		kLast
	};
    ///
	MObject  	create(	const MPointArray &controlVertices,
						const MDoubleArray &knotSequences,
						unsigned int degree,
						Form agForm,
						bool create2D,
						bool createRational,
						MObject & parentOrOwner = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
	///
	MObject		create( const MObjectArray& sources,
						MObject & parentOrOwner = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
	///
	MObject     copy (const MObject &source,
					  MObject &parentOrOwner = MObject::kNullObj,
					  MStatus* ReturnStatus = NULL);
	///
	MStatus		reverse(bool constructionHistory = false);

    ///
	MObject  	cv( unsigned index, MStatus * ReturnStatus = NULL ) const;
    ///
	MStatus		getCV( unsigned index, MPoint &pt,
						MSpace::Space space = MSpace::kObject ) const;
    ///
	MStatus		setCV( unsigned index, const MPoint &pt,
						MSpace::Space space = MSpace::kObject );
    ///
	Form		form( MStatus * ReturnStatus = NULL ) const;
    ///
	int			degree( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numCVs( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numSpans( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numKnots( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus		getKnotDomain( double &start, double &end ) const;
    ///
	MStatus		getKnots( MDoubleArray &array ) const;
    ///
    MStatus     setKnots( const MDoubleArray &array, unsigned startIndex,
                          unsigned endIndex );
    ///
	MStatus		setKnot( unsigned index, double param );
    ///
	MObject  	cvs( unsigned startIndex, unsigned endIndex,
						MStatus * ReturnStatus = NULL ) const;
	///
	MStatus		getCVs( MPointArray &array,
						MSpace::Space space = MSpace::kObject ) const;
    ///
	MStatus		setCVs( const MPointArray &array,
						MSpace::Space space = MSpace::kObject );
    ///
	double		knot( unsigned index, MStatus * ReturnStatus = NULL ) const;
    ///
	MStatus		removeKnot( double atThisParam, bool removeAll = false );
    ///
	bool		isPointOnCurve( const MPoint &point,
								double tolerance = kMFnNurbsEpsilon,
								MSpace::Space space = MSpace::kObject,
								MStatus * ReturnStatus = NULL ) const;
    ///
	MStatus		getPointAtParam( double param, MPoint &point,
								MSpace::Space space = MSpace::kObject
								) const;
    ///
	MStatus		getParamAtPoint( const MPoint & atThisPoint, double &param,
								 MSpace::Space space = MSpace::kObject )
								 const;
    ///
	bool		isParamOnCurve( double param,
								MStatus * ReturnStatus = NULL ) const;
    ///
	MVector		normal( double param,
						MSpace::Space space = MSpace::kObject,
						MStatus * ReturnStatus = NULL ) const;
    ///
	MVector		tangent( double param,
						 MSpace::Space space = MSpace::kObject,
						 MStatus * ReturnStatus = NULL ) const;
    ///
	bool		isPlanar( MVector * planeNormal = NULL,
							MStatus * ReturnStatus = NULL ) const;
    ///
	MPoint		closestPoint( const MPoint &toThisPoint, 
								double * param = NULL,
								double tolerance = kMFnNurbsEpsilon,
								MSpace::Space space = MSpace::kObject,
								MStatus * ReturnStatus = NULL ) const;
    ///
	double		distanceToPoint( const MPoint &pt,
								MSpace::Space space = MSpace::kObject,
								MStatus * ReturnStatus = NULL ) const;
    ///
	double		area( double tolerance = kMFnNurbsEpsilon,
							MStatus * ReturnStatus = NULL ) const;
    ///
	double		length( double tolerance = kMFnNurbsEpsilon,
							MStatus * ReturnStatus = NULL ) const;
    ///
	double		findParamFromLength( double partLength,
							MStatus * ReturnStatus = NULL ) const;
    ///
	bool        hasHistoryOnCreate( MStatus * ReturnStatus = NULL );
    ///
	MStatus     updateCurve();
protected:  
	MObject		createMObject(MObject & parentOrOwner,
    						  TnurbsCurve * newCurve,
    						  MStatus * ReturnStatus);

	virtual bool objectChanged( MFn::Type, MStatus * );
private:
	inline void * updateGeomPtr() const;
	inline void * updateConstGeomPtr() const;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnNurbsCurve */
