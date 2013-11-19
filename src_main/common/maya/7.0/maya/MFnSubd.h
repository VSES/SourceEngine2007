
#ifndef _MFnSubd
#define _MFnSubd
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
// CLASS:    MFnSubd
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnSubd)
//
//	Function set for hierarchical Catmull-Clark subdivision surfaces.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>

// *****************************************************************************

// DECLARATIONS

class MUint64Array;
class MObjectArray;
class MDoubleArray;
class MVectorArray;
class MPointArray;
class MIntArray;
class MVector;

#if defined(OSMac_)
#include <maya/MPoint.h>
#else
class MPoint;
#endif

// Default tolerance
#define kMFnSubdTolerance 1.0e-3

// Point equivalent tolerance
#define kMFnSubdPointTolerance 1.0e-10

// *****************************************************************************

// CLASS DECLARATION (MFnSubd)

/// Subdivision surface function set
/**

*/
#if defined(_WIN32)
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnSubd : public MFnDagNode 
{
	declareDagMFn(MFnSubd, MFnDagNode);

public: 
    ///
    MObject createBaseMesh( bool reverseNormal,
							int numVertices, int numPolygons,
							const MPointArray& vertexArray,
							const MIntArray& polygonCounts,
							const MIntArray& polygonConnects,
							MObject parentOrOwner = MObject::kNullObj,
							MStatus* ReturnStatus = NULL );

	///
	MObject		copy( const MObject& source,
					  MObject parentOrOwner = MObject::kNullObj,
					  MStatus* ReturnStatus = NULL );

	///
	MObject		collapse( const int level,
						  bool makeNewNodeInvisible,
						  MStatus* ReturnStatus = NULL );

public:
	// Manual creation:

	///
	int		 vertexBaseMeshAdd( double x, double y, double z,
									MStatus* ReturnStatus = NULL );
	///
	MUint64		 polygonBaseMeshAdd( int vertexCount,
									 const int vertexIds[],
									 MStatus* ReturnStatus = NULL );
	///
	MUint64		 polygonBaseMeshAddWithUVs( int vertexCount,
											const int vertexIds[],
											const double uValues[],
											const double vValues[],
											MStatus* ReturnStatus = NULL );
public:
	///
	int		editsPending( MStatus* ReturnStatus = NULL ) const;
	///
	void		editsUpdateAll( MStatus* ReturnStatus = NULL );

public:
	///
	unsigned levelMaxCurrent( MStatus* ReturnStatus = NULL ) const;
	///
	unsigned levelMaxAllowed( MStatus* ReturnStatus = NULL ) const;
	///
	void		levelFullySubdivideTo( int level,
									   MStatus* ReturnStatus = NULL );
    ///
	unsigned    vertexCount( int level = 0,
							 MStatus* ReturnStatus = NULL) const;
public:
	///
	MUint64		vertexIdFromBaseVertexIndex( int baseVertexIndex,
										 MStatus* ReturnStatus = NULL) const;
	///
	int		vertexBaseIndexFromVertexId( MUint64 vertexId,
										 MStatus* ReturnStatus = NULL) const;

	///
	MStatus		vertexEditsGetAllNonBase( MUint64Array& vertexIds,
										  MVectorArray& edits,
							MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus		vertexEditsSetAllNonBase( const MUint64Array& vertexIds,
										  const MVectorArray& edits,
							MSpace::Space space = MSpace::kObject );
	///
	MStatus		vertexEditsClearAllNonBase();

	///
	MStatus		vertexPositionGet( MUint64 vertId, MPoint& position,
							MSpace::Space space = MSpace::kObject)const;
	///
	MStatus		vertexEditGet( MUint64 vertId, MVector& edit,
							MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus		vertexPositionGetNoEdit( MUint64 vertId, MPoint& position,
							MSpace::Space space = MSpace::kObject )const;
	///
	MStatus		vertexPositionSet( MUint64 vertId, const MPoint& position,
								   bool updateEdits,
								   MSpace::Space space = MSpace::kObject );
	///
	MStatus		vertexEditSet( MUint64 vertId, const MVector& edit,
							   bool updateEdits,
								MSpace::Space space = MSpace::kObject );
	///
	MStatus 	vertexBaseMeshGet( MPointArray& positions,
								MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus 	vertexBaseMeshSet( const MPointArray& positions,
								MSpace::Space space = MSpace::kObject );
	///
	MStatus 	vertexBaseMeshGetWithId( MPointArray& positions,
									 MUint64Array& vertexIds,
								MSpace::Space space = MSpace::kObject ) const;
	///
	MStatus 	vertexBaseMeshSetWithId( const MPointArray& positions,
										 const MUint64Array& vertexIds,
								MSpace::Space space = MSpace::kObject );
	///
	int		vertexValence( MUint64 vertId,
							   MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		vertexNormal( MUint64 vertId, MVector& normal ) const;
	///
	MStatus		vertexAdjacentVertices( MUint64 vertId,
										MUint64Array& vertList ) const;
	///
	MStatus		vertexIncidentEdges( MUint64 vertId,
									 MUint64Array& edgeList ) const;
	///
	MStatus		vertexIncidentPolygons( MUint64 vertId,
										MUint64Array& polyList ) const;
	///
	bool		vertexIsBoundary( MUint64 vertId,
								  MStatus* ReturnStatus = NULL ) const;
	///
	bool		vertexIsValid( MUint64 vertId,
							   MStatus* ReturnStatus = NULL ) const;
	///
	bool		vertexIsCreased( MUint64 vertId,
								 MStatus* ReturnStatus = NULL ) const;
	///
	bool		vertexCreaseRelevant( MUint64 vertId,
									  MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		vertexSetCrease( MUint64 vertId, bool creased ) const;
	///
	MStatus		vertexChildren( MUint64 vertId, MUint64Array& children ) const;

public:
	///
	MStatus		creasesGetAll( MUint64Array& vertexIds,
							   MUint64Array& edgeIds ) const;
	///
	MStatus		creasesSetAll( const MUint64Array& vertexIds,
							   const MUint64Array& edgeIds );
	///
	MStatus		creasesClearAll() const;
	///
	MStatus		updateAllEditsAndCreases();

public:
    ///
	unsigned    edgeCount( int level = 0,
						   MStatus* ReturnStatus = NULL ) const;
	///
	MUint64		edgeBetween( MUint64 vertex1, MUint64 vertex2,
							 MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		edgeVertices( MUint64 edge, MUint64& v1, MUint64& v2 ) const;
	///
	MStatus		edgeAdjacentPolygon( MUint64 edge, MUint64Array& polys ) const;
	///
	bool		edgeIsBoundary( MUint64 edge,
								MStatus* ReturnStatus = NULL ) const;
	///
	bool		edgeIsValid( MUint64 edgeId,
							 MStatus* ReturnStatus = NULL  ) const;
	///
	bool		edgeIsCreased( MUint64 edgeId,
							   MStatus* ReturnStatus = NULL  ) const;
	///
	bool		edgeCreaseRelevant( MUint64 edgeId,
									MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		edgeSetCrease( MUint64 edgeId, bool creased ) const;
	///
	MStatus		edgeChildren( MUint64 edgeId, MUint64Array& children ) const;

public:
    ///
	unsigned    polygonCount( int level = 0,
							  MStatus* ReturnStatus = NULL ) const;
    ///
	int        polygonCountMaxWithGivenBaseMesh( int level = 1,
								MStatus* ReturnStatus = NULL ) const;
	///
	unsigned	polygonVertexCount( MUint64 polyId,
									MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		polygonVertices( MUint64 polyId,
								 MUint64Array& vertIds ) const;
	///
	unsigned	polygonEdgeCount( MUint64 polyId,
								  MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		polygonEdges( MUint64 polyId,
							  MUint64Array& edgeIds ) const;
	///
	bool		polygonIsValid( MUint64 polyId,
							   MStatus* ReturnStatus = NULL ) const;
	///
	bool		polygonHasChildren( MUint64 polyId, MStatus* ReturnStatus = NULL ) const;
	///
	MStatus		polygonChildren( MUint64 polyId, MUint64Array& children ) const;
	///
	MStatus		polygonSubdivide( MUint64 polyId );

	///
	MStatus		polygonSetUseUVs( MUint64 polyId, bool useThem );
	///
	bool		polygonHasVertexUVs( MUint64 polyId,
									 MStatus* ReturnStatus = NULL );
	///
	MStatus		polygonGetVertexUVs( MUint64 polyId,
									 MDoubleArray& uValues,
									 MDoubleArray& vValues ) const;
	///
	MStatus		polygonSetVertexUVs( MUint64 polyId,
									 const MDoubleArray& uValues,
									 const MDoubleArray& vValues );
	///
	MStatus		polygonGetCenterUV( MUint64 polyId,
									double& u, double& v ) const;

public:
	///
	MStatus		evaluatePosition( MUint64 polyId, double u, double v,
								 bool uvNormalized, MPoint& ) const;
	///
	MStatus		evaluateNormal( MUint64 polyId, double u, double v,
								 bool uvNormalized, MVector& nrml ) const;
	///
	MStatus		evaluatePositionAndNormal( MUint64 polyId, double u, double v,
										   bool uvNormalized,
										   MPoint& pos, MVector& nrml ) const;
	///
	bool		getCubicSpline( MUint64 polyId, MPoint vertices[16],
								MStatus* ReturnStatus = NULL );

public:
	///
	MStatus		getConnectedSetsAndMembers( unsigned instanceNumber,
											MObjectArray & sets,
											MObjectArray & comps,
											bool renderableSetsOnly ) const;
	///
    MStatus getConnectedShaders( unsigned instanceNumber,
									 MObjectArray & shaders,
									 MUint64Array & faces,
									 MIntArray & indices ) const;
	///
	MObject tesselate(  bool uniform, int depth, int sample,
						MObject parentOrOwner = MObject::kNullObj,
						MStatus* ReturnStatus = NULL );

	///
	MObject tessellateViaNurbs( MObject parentOrOwner = MObject::kNullObj,
								MStatus* ReturnStatus = NULL );

	///
	MStatus convertToNurbs( MObjectArray& newNurbsSurfaces );

    ///
	MStatus updateSubdSurface(); 

public:
	///
	MStatus		 vertexBaseMeshAddWithIndex( double x, double y, double z,
											 int index );

protected:  
	virtual bool objectChanged( MFn::Type tp, MStatus* ReturnStatus );

private:
	void *f_subdRef;

};

#if defined(_WIN32)
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnSubd */
