#ifndef _MFnNurbsSurface
#define _MFnNurbsSurface
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
// CLASS:    MFnNurbsSurface
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnNurbsSurface)
//
// This is the function set for NURBS (non-uniform ration B-spline) surfaces.
//
// The U and V knot vectors for NURBS surfaces are of size
// {\bf (spansInU + 2*degreeInU -1)} and {\bf (spansInV + 2*degreeInV -1)}. 
// Note: spans = numCVs - degree.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MObject.h>
#include <maya/MTesselationParams.h>
#include <maya/MObjectArray.h>

// *****************************************************************************

// DECLARATIONS

class MPointArray;
class MDoubleArray; 
class MFloatArray;
class MIntArray;
class MPoint;
class MVector;
class MDagPath;
class MPtrBase;
class MDGModifier;
class MSelectionList;
class MTrimBoundaryArray;
class TnurbsCurvePtrArray;
struct ag_curve;

#define kMFnNurbsEpsilon  1.0e-3

// *****************************************************************************


typedef MStatus (MFnNurbsSurface::*setKnotsFnPtr)( unsigned, double ); 
typedef int (MFnNurbsSurface::*numKnotsFnPtr)( MStatus* ) const; 
                                                   

// CLASS DECLARATION (MFnNurbsSurface)

/// NURBS surface function set
/**

*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnNurbsSurface : public MFnDagNode 
{
	declareDagMFn(MFnNurbsSurface, MFnDagNode);

public:
    ///
	enum Form {
        ///
		kInvalid =0,
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
	MObject 	create(	const MPointArray &controlVertices,
						const MDoubleArray &uKnotSequences,
						const MDoubleArray &vKnotSequences,
						unsigned int degreeInU,
						unsigned int degreeInV,
						Form formU,
						Form formV,
						bool createRational,
						MObject parentOrOwner = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );
	///
	MObject     copy( const MObject &source,
					  MObject parentOrOwner = MObject::kNullObj,
					  MStatus* ReturnStatus = NULL );
    ///
	MObject 	cv( unsigned indexU, unsigned indexV, 
						MStatus * ReturnStatus = NULL );
    ///
	MObject		cvsInU( unsigned startIndex,
					    unsigned endIndex,
						unsigned rowIndex, 
						MStatus * ReturnStatus = NULL );
    ///
	MObject		cvsInV( unsigned startIndex,
						unsigned endIndex,
						unsigned rowIndex, 
						MStatus * ReturnStatus = NULL ); 
    ///
	MStatus		getCVs( MPointArray& array,
						MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus		setCVs( const MPointArray& array,
						MSpace::Space space = MSpace::kObject );
	///
	MStatus		getCV( unsigned indexU, unsigned indexV, MPoint& pnt,
						MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus		setCV( unsigned indexU, unsigned indexV, MPoint& pnt,
						MSpace::Space space = MSpace::kObject ) const;
	///
	Form		formInU( MStatus * ReturnStatus = NULL ) const;
    ///
	Form		formInV( MStatus * ReturnStatus = NULL ) const;
    ///
	bool		isBezier( MStatus * ReturnStatus = NULL ) const;
    ///
	bool		isUniform( MStatus * ReturnStatus = NULL ) const;
    ///
	bool		isKnotU( double param, MStatus * ReturnStatus = NULL ) const;
    ///
	bool		isKnotV( double param, MStatus * ReturnStatus = NULL ) const;
    ///
	bool		isParamOnSurface( double paramU, double paramV, 
						MStatus * ReturnStatus = NULL ) const;
    ///
	MStatus		getKnotDomain( double &startU, double &endU, double &startV, 
						double &endV ) const; 
    ///
	int			degreeU( MStatus * ReturnStatus = NULL ) const;
    ///
	int			degreeV( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numSpansInU( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numSpansInV( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numNonZeroSpansInU( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numNonZeroSpansInV( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numCVsInU( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numCVsInV( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numKnotsInU( MStatus * ReturnStatus = NULL ) const;
    ///
	int			numKnotsInV( MStatus * ReturnStatus = NULL ) const;
    ///
	MStatus		getKnotsInU( MDoubleArray &array ) const;
    ///
	MStatus		getKnotsInV( MDoubleArray &array ) const;  
    ///
	MStatus		setKnotsInU( const MDoubleArray &array, unsigned startIndex,
						unsigned endIndex );
    ///
	MStatus		setKnotsInV( const MDoubleArray &array, unsigned startIndex,
						unsigned endIndex );
    ///
	double		knotInU( unsigned index, MStatus * ReturnStatus = NULL ) const;
    ///
	double		knotInV( unsigned index, MStatus * ReturnStatus = NULL ) const;
    ///
	MStatus		setKnotInU( unsigned index, double param );
    ///
	MStatus		setKnotInV( unsigned index, double param );
    ///
	MStatus		removeKnotInU( double atThisParam, bool removeAll = false );
    ///
	MStatus		removeKnotInV( double atThisParam,  bool removeAll = false );
    ///
	MStatus		removeOneKnotInU( double atThisParam );
    ///
	MStatus		removeOneKnotInV( double atThisParam );
    ///
    MVector		normal( double paramInU, double paramInV, 
						MSpace::Space space = MSpace::kObject,
						MStatus * ReturnStatus = NULL ) const;
    ///
    MStatus		getTangents( double paramInU, double paramInV, 
						MVector &vectorInU, MVector &vectorInV,
						MSpace::Space space = MSpace::kObject) const; 
    ///
	double		area( double tolerance = kMFnNurbsEpsilon, 
						MStatus * ReturnStatus = NULL ) const;

    ///
	double		area( MSpace::Space space,
						double tolerance = kMFnNurbsEpsilon,						 
						MStatus * ReturnStatus = NULL ) const;

    ///
	MPoint		closestPoint( const MPoint &toThisPoint, 
						double * paramU = NULL,
						double * paramV = NULL,
						bool ignoreTrims = false,
						double tolerance = kMFnNurbsEpsilon, 
						MSpace::Space space = MSpace::kObject,
						MStatus * ReturnStatus = NULL ) const;
    ///
	bool		isPointOnSurface( const MPoint &point, 
						double tolerance = kMFnNurbsEpsilon, 
						MSpace::Space space = MSpace::kObject,
						MStatus * ReturnStatus = NULL ) const;
    /// OBSOLETE
	MStatus		getParamAtPoint( const MPoint & atThisPoint,
						double &paramU, double &paramV,
						MSpace::Space space = MSpace::kObject) const;
    ///
	MStatus		getParamAtPoint( const MPoint & atThisPoint,
						double &paramU, double &paramV,
						bool ignoreTrimBoundaries,
						MSpace::Space space = MSpace::kObject) const;
    ///
	MStatus		getPointAtParam( double paramU, double paramV, MPoint &point,
						MSpace::Space space = MSpace::kObject) const;
    ///
	double		distanceToPoint( const MPoint &pt, 
						MSpace::Space space = MSpace::kObject,
						MStatus * ReturnStatus = NULL ) const;
	///
	MObject		tesselate( MTesselationParams & parms =
						MTesselationParams::fsDefaultTesselationParams,
                        MObject parentOrOwner = MObject::kNullObj,
						MStatus * ReturnStatus = NULL );

	///
	bool		intersect( const MPoint &rayStartingPoint,
						const MVector &alongThisDirection,
						double &u, double &v,
						MPoint &intersectionData,
						double tolerance = kMFnNurbsEpsilon,
						MSpace::Space space = MSpace::kObject,
						bool calculateDistance = false,
						double * distance = NULL,
						bool calculateExactHit = false,
						bool* wasExactHit = NULL,
						MStatus* ReturnStatus = NULL ) const;
	///
	bool		intersect( const MPoint &rayStartingPoint,
						const MVector &alongThisDirection,
						MDoubleArray &u, MDoubleArray &v,
						MPointArray &intersectionData,
						double tolerance = kMFnNurbsEpsilon,
						MSpace::Space space = MSpace::kObject,
						bool calculateDistance = false,
						MDoubleArray * distances = NULL,
						bool calculateExactHit = false,
						bool* wasExactHit = NULL,
						MStatus* ReturnStatus = NULL ) const;

    ///
	bool		hasHistoryOnCreate( MStatus * ReturnStatus = NULL );
    ///
	MStatus		updateSurface();


    ///
	enum BoundaryType {
        ///
		kInvalidBoundary=0,
        ///
		kOuter,
        ///
		kInner,
        ///
		kSegment,
        ///
		kClosedSegment
	};
	///
	bool		isTrimmedSurface( MStatus * ReturnStatus = NULL );
	///
	unsigned	numRegions( MStatus * ReturnStatus = NULL );
	///
	bool		isFlipNorm( unsigned region, MStatus * ReturnStatus = NULL );
	///
	unsigned	numBoundaries( unsigned region, MStatus * ReturnStatus = NULL);
	///
	BoundaryType boundaryType( unsigned region, unsigned boundary,
						MStatus * ReturnStatus = NULL );
	///
	unsigned     numEdges( unsigned region, unsigned boundary,
    					MStatus * ReturnStatus = NULL );
	///
	MObjectArray edge( unsigned region, unsigned boundary, unsigned edge,
				 	 	bool paramEdge = false,
						MStatus * ReturnStatus = NULL );

	///
	bool        isPointInTrimmedRegion( double u, double v,
						MStatus * ReturnStatus = NULL );

	///
	MStatus		getTrimBoundaries(MTrimBoundaryArray& result,
    							  unsigned region,
    							  bool paramEdge = true);
	///
	MStatus		trimWithBoundaries(const MTrimBoundaryArray& mBoundaries,
    							   bool  flipNormal = false,
    							   double e_tol = 1e-3,
    							   double pe_tol = 1e-5);
	///
	MStatus 	projectCurve( MDagPath& curve, MVector * direction = NULL,
				 		bool constructionHistory = false );
	///
	MStatus		trim( MDoubleArray& locatorU, MDoubleArray& locatorV,
						bool constructionHistory = false );

	// Support methods needed to do patch level explicit uv assignment and
	// editing.

	///
	unsigned     numPatches( MStatus * ReturnStatus = NULL ) const;
	///
	unsigned     numPatchesInU( MStatus * ReturnStatus = NULL ) const;
	///
	unsigned     numPatchesInV( MStatus * ReturnStatus = NULL ) const;
	
	// Explicit uv related methods..
	//
	///
	int			numUVs( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus 	setUVs( const MFloatArray& uArray, const MFloatArray& vArray);
	///
	MStatus 	getUVs( MFloatArray& uArray, MFloatArray& vArray ) const;
	///
	MStatus		setUV( int uvId, float u, float v );
	///
	MStatus		getUV( int uvId, float & u, float & v ) const;
	///
	MStatus 	getPatchUV( int patchId, int cornerIndex, 
									float & u, float & v ) const;
	///
	MStatus 	getPatchUVs( int patchId, MFloatArray& uArray,
												MFloatArray& vArray) const;
	///
	MStatus 	getPatchUVid( int patchId, int cornerIndex, int &uvId ) const;
	///
	MStatus 	assignUV( int patchId, int cornerIndex, int uvId );
	///
	MStatus		assignUVs( const MIntArray& uvCounts, const MIntArray& uvIds );
	///
	MStatus		clearUVs( );
	///
	MStatus		getAssignedUVs( MIntArray& uvCounts, MIntArray& uvIds) const;

	/// OBSOLETE
	MObject		tesselate( MTesselationParams & parms,
						MStatus * ReturnStatus );


protected: 
	virtual bool objectChanged( MFn::Type, MStatus * );
	bool		getUVSetIndex( const MString * uvSetName,
								int & uvSet) const;
	bool		curveToMObjects(ag_curve*       crv,
    							MObjectArray&   curveArray,
    							MStatus         *ReturnStatus);
	bool		trimWithBoundariesInHistory(
								const TnurbsCurvePtrArray& boundaries,
    							bool  flipnorm,
    							double   e_tol,
    							double   pe_tol,
    							MStatus  *status);
private:
    
    // function used for both setKnotsInU() and setKnotsInV()
    
    MStatus setKnotsUsingFn( const MDoubleArray& array, unsigned startIndex,
					 unsigned endIndex, numKnotsFnPtr numKnotFn,
					 setKnotsFnPtr setKnotFn );

	inline void * updateGeomPtr() const;
	inline void * updateConstGeomPtr() const;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnNurbsSurface */
