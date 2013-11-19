//*****************************************************************************
/*!
   \file xsi_geometry.h
   \brief Geometry class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIGEOMETRY_H__
#define __XSIGEOMETRY_H__

#include <xsi_siobject.h>
#include <xsi_math.h>
#include <xsi_longarray.h>
#include <xsi_doublearray.h>
#include <xsi_cluster.h>
#include <xsi_clusterproperty.h>
#include <xsi_pointlocatordata.h>
#include <xsi_vector3.h>

namespace XSI {

class CFacetRefArray;
class CSegmentRefArray;
class CPointRefArray;
class CSampleRefArray;
class CRefArray;
class CTriangleRefArray;
class MATH::CTransformation;

//*****************************************************************************
/*! \class Geometry xsi_geometry.h
	\brief The Geometry class gives access to a X3DObject's geometry. Geometry is the base class for specific geometry
	classes such as PolygonMesh and NurbsSurfaceMesh.

	\sa Cluster, ClusterProperty, Facet, Segment, Point, NurbsSurfaceMesh, NurbsSurface, PolygonMesh,
		PolygonFace, Edge, Vertex

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		Geometry geom( myGrid.GetActivePrimitive().GetGeometry(0) );

		CPointRefArray points( geom.GetPoints() );

		app.LogMessage( L"Number of points: " +
						CValue(points.GetCount()).GetAsText() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Geometry : public SIObject
{
public:
	/*! Default constructor. */
	Geometry();

	/*! Default destructor. */
	~Geometry();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Geometry(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Geometry(const Geometry& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The newly created object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new Geometry object.
	*/
	Geometry& operator=(const Geometry& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Geometry object.
	*/
	Geometry& operator=(const CRef& in_ref);

	/*! Returns an array of Facet objects on this geometry.
	\return Array of references to Facet objects
	\sa Facet, Geometry::GetPoints, Geometry::GetSegments, Geometry
	*/
	CFacetRefArray		GetFacets() const;

	/*! Returns an array of Segment objects on this geometry.
	\return Array of references to Segment objects
	\sa Segment, Geometry::GetPoints, Geometry::GetFacets, Geometry
	*/
	CSegmentRefArray	GetSegments() const;

	/*! Returns an array of Point objects on this geometry.
	\return Array of references to Point objects.
	\sa Point
	*/
	CPointRefArray		GetPoints() const;

	/*! Returns an array of Sample objects on this geometry.
	\return Array of references to Sample objects.
	\sa Sample, Facet::GetSamples
	*/
	CSampleRefArray		GetSamples() const;

	/*! Returns an array of Cluster objects on this geometry.
	\return Array of references to Cluster objects.
	\sa Cluster
	*/
	CRefArray			GetClusters() const;

	/*! Returns an array of Triangle objects on this geometry.
	\return Array of references to Triangle objects.
	\sa Facet, Geometry::GetPoints, Geometry::GetSegments, Geometry::GetFacets,
	*/
	CTriangleRefArray	GetTriangles() const;

	/*! Creates and adds a partial or complete cluster to this geometry.
	\param in_type Type of cluster, such as ::siVertexCluster, ::siEdgeCluster, etc.  See the ::ClusterTypes enum for a complete list.
	\param in_name Name of the new cluster
	\param in_indices Array of cluster indices.  If an empty array is sent then a complete cluster is created. A complete cluster
		represents all the elements of the geometry and will remain complete even as new elements are added to the geometry. It is
		not possible to remove elements from such a cluster.
	\param io_cluster Newly created Cluster
	\return CStatus::OK success
	\return CStatus::Fail failure
	\sa ::ClusterTypes, Cluster
	*/
	CStatus	AddCluster(	const CString& in_type,
						const CString& in_name,
						const CLongArray& in_indices,
						Cluster& io_cluster);

	/*! Creates and adds an empty cluster to this geometry.
	\param in_type Type of cluster
	\param in_name Name of the new cluster
	\param io_cluster Newly created Cluster
	\return CStatus::OK success
	\return CStatus::Fail failure
	\sa ClusterTypes, Cluster
	*/
	CStatus	AddEmptyCluster(
						const CString& in_type,
						const CString& in_name,
						Cluster& io_cluster);

	/*! Creates a PointLocatorData containing surface locations corresponding to the specified Points of the geometry (or all Points
	if not specified).

	In the case of a NurbsSurfaceMesh, there is no direct relationship between the points (control vertices) and the surface. In that
	case, the returned point locators correspond to the surface locations most influenced by each input control vertex.

	\note The returned point locators can be evaluated on any %Geometry instance having the same topology.

	\param in_nbPoints Number of points indices passed in in_pPoints. To specify all points in geometry, use \c -1. In that case,
		the \c in_pPoints argument will be ignored.
	\param in_pPoints %Point indices (not used if in_pPoints is -1)
	\return A new PointLocatorData object (CRef::IsValid == false if failed)

	\sa PolygonMesh::ConstructPointLocators, NurbsSurfaceMesh::ConstructPointLocators
	\since 5.0

	\eg This example uses point locators to place a null positioned and oriented to a surface location corresponding to each
		control vertex of the NURBS sphere.

	\code
		void CreateNullsAtPointLocations( X3DObject& inObj, const PointLocatorData& inPointLocators )
		{
			Geometry geom( inObj.GetActivePrimitive().GetGeometry() );

			std::vector<double> posData, normData;
			posData.resize(inPointLocators.GetCount()*3);
			normData.resize(inPointLocators.GetCount()*3);

			geom.EvaluatePositions(inPointLocators, -1, 0, &posData.front());
			geom.EvaluateNormals(inPointLocators, siInterpolatedVertexGeometricNormals, -1, 0, &normData.front());

			MATH::CVector3 trans;
			MATH::CRotation rot;

			LONG i;
			for (i = 0; i < (LONG)posData.size(); i+=3)
			{
				Null nullObj;
				inObj.AddNull(L"",nullObj);

				trans.Set(posData[i], posData[i+1], posData[i+2]);
				nullObj.PutLocalTranslation(trans);

				trans.Set(normData[i], normData[i+1], normData[i+2]);
				rot.SetFromXYZAxes( trans, trans, trans );
				nullObj.PutLocalRotation(rot);
			}
		}

		void main()
		{
			Application app;
			Model root = app.GetActiveSceneRoot();

			X3DObject meshSphereObj;
			root.AddGeometry( L"Sphere", L"MeshSurface", L"", meshSphereObj );
			PolygonMesh meshSphereGeom( meshSphereObj.GetActivePrimitive().GetGeometry() );

			PointLocatorData pointLocatorsFromSpherePoints = meshSphereGeom.GetSurfacePointLocatorsFromPoints();

			CreateNullsAtPointLocations(meshSphereObj, pointLocatorsFromSpherePoints);
		}
	\endcode
	*/
	PointLocatorData GetSurfacePointLocatorsFromPoints
	(
		LONG in_nbPoints = -1,
		const LONG* in_pPoints = NULL
	) const;

	/*! Creates a PointLocatorData object containing the closest surface locations from the specified input positions. By default,
	input positions have to be defined in the object's local space reference.

	Some aspects of the closest locations computation such as the reference pose or the method (closest surface, closest vertex or
	knot, ...) can be set up using Geometry::SetupClosestLocationQueries. This setup will affect all subsequent calls to
	Geometry::GetClosestLocations and to Geometry::GetClosestLocationsWithinRadius.

	When you call this function an acceleration cache is automatically created. See Geometry::SetupClosestLocationQueries for
	more information.

	\note The "closest distance" relationship may change relative to the spatial reference of the geometry and the input
		positions. See Geometry::SetupClosestLocationQueries for more information.

	\tip The returned point locators can be evaluated on any %Geometry instance having the same topology.

	\param in_nbPositions Number of positions (XYZ triplets) specified in in_pPositions
	\param in_pPositions Positions (packed XYZ) from which the spatial search will be initiated.
	\return A new PointLocatorData object containing in_nbPositions point locators (CRef::IsValid == false if failed)

	\sa Geometry::GetClosestLocationsWithinRadius, Geometry::SetupClosestLocationQueries
	\since 5.0

	\eg Demonstrates using the PointLocatorData to find the closest mesh sphere vertex from a Null

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();
		Null nullObj;
		root.AddNull( L"myNull", nullObj );

		CValueArray args(8);
		CValue outArg;
		args[0] = CValue(nullObj.GetRef());
		args[1] = CValue(L"-2.0");
		args[2] = CValue(L"8.0");
		args[3] = CValue(L"5.0");
		args[4] = CValue(L"siRelative");
		args[5] = CValue(L"siView");
		args[6] = CValue(L"siObj");
		args[7] = CValue(L"siXYZ");
		app.ExecuteCommand( L"Translate", args, outArg );

		X3DObject meshSphereObj;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", meshSphereObj );
		PolygonMesh meshSphereGeom( meshSphereObj.GetActivePrimitive().GetGeometry() );

		PointLocatorData closestPointLocator = meshSphereGeom.GetClosestLocations(1, (double*)&(nullObj.GetKinematics().GetGlobal().GetTransform().GetTranslation()));

		double pos[3];
		LONG triVtx[3];
		float triWei[3];

		meshSphereGeom.EvaluatePositions(closestPointLocator, -1, 0, pos);
		meshSphereGeom.GetTriangleVertexIndexArray(closestPointLocator, -1, 0, triVtx);
		meshSphereGeom.GetTriangleWeightArray(closestPointLocator, -1, 0, triWei);

		app.LogMessage(CString(L"The closest position on the mesh sphere from the Null is (")
				+ CString(CValue(pos[0])) + L", " + CString(CValue(pos[1])) + L", " + CString(CValue(pos[2])) + L")");
		app.LogMessage(CString(L"which corresponds to the triangle made of vertices (")
				+ CString(CValue(triVtx[0])) + L", " + CString(CValue(triVtx[1])) + L", " + CString(CValue(triVtx[2])) + L").");
		app.LogMessage(CString(L"The barycentric weight relatively to each triangle vertex is (")
				+ CString(CValue(triWei[0])) + L", " + CString(CValue(triWei[1])) + L", " + CString(CValue(triWei[2])) + L").");
		//INFO : The closest position on the mesh sphere from the Null is (-0.695969, 3.28837, 1.85179)
		//INFO : which corresponds to the triangle made of vertices (14, 21, 22).
		//INFO : The barycentric weight relatively to each triangle vertex is (0.347985, 0.121569, 0.530446).
	\endcode

	\eg Demonstrates using the PointLocatorData to shrink-wrap a sphere to a cube, and then push the sphere along the cube's normals.

	\code
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject meshCubeObj;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", meshCubeObj );
		PolygonMesh meshCubeGeom( meshCubeObj.GetActivePrimitive().GetGeometry() );

		X3DObject meshSphereObj;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", meshSphereObj );
		PolygonMesh meshSphereGeom( meshSphereObj.GetActivePrimitive().GetGeometry() );

		meshSphereObj.PutParameterValue(L"subdivu", 24l);
		meshSphereObj.PutParameterValue(L"subdivv", 24l);

		// We must freeze it, otherwise setting its position array will be forbidden:
		CValueArray args(3);
		CValue outArg;
		args[0] = meshSphereObj.GetRef();
		app.ExecuteCommand(L"FreezeObj",args, outArg);

		MATH::CVector3Array posArray = meshSphereGeom.GetPoints().GetPositionArray();
		PointLocatorData sphereOnCubePointLocators = meshCubeGeom.GetClosestLocations(posArray.GetCount(), (double*)&posArray[0]);

		MATH::CVector3Array normArray;
		normArray.Resize(posArray.GetCount());

		meshCubeGeom.EvaluatePositions(sphereOnCubePointLocators, -1, 0, (double*)&posArray[0]);
		meshCubeGeom.EvaluateNormals(sphereOnCubePointLocators, siInterpolatedVertexGeometricNormals, -1, 0, (double*)&normArray[0]);

		LONG i;
		for (i = 0; i < posArray.GetCount(); i++)
		{
			posArray[i].PutX(posArray[i].GetX()+normArray[i].GetX()*3);
			posArray[i].PutY(posArray[i].GetY()+normArray[i].GetY()*3);
			posArray[i].PutZ(posArray[i].GetZ()+normArray[i].GetZ()*3);
		}
		meshSphereGeom.GetPoints().PutPositionArray(posArray);
	\endcode
	*/

	PointLocatorData GetClosestLocations
	(
		LONG			in_nbPositions,
		const double*	in_pPositions
	) const;

	/*! Constructs a PointLocatorData containing closest surface locations from the input position within a search sphere of a specific
	radius. By default, the input positions have to be defined in the object's local space reference. Optionally, the search can be
	restricted to return a maximum number of locations. In order to avoid volumetric restrictions, simply specify a very large radius.

	Some aspects of the closest locations's computation such as the reference pose or the method (closest surface, closest vertex or
	knot, ...) can be set up using Geometry::SetupClosestLocationQueries. This setup will affect all subsequent calls to
	GetClosestLocationsWithinRadius and to Geometry::GetClosestLocations.

	If the search was set up with Geometry::SetupClosestLocationQueries to be a closest surface or smoothed closest surface search, then
	it returns one point locator per connected island for PolygonMeshes. Such connected islands are defined so that you cannot walk
	between 2 returned point locators without leaving the search radius (this walking is through polygon adjacency, not vertex adjacency).
	For NurbsSurfaceMesh geometries, it returns at most one point locator.

	When you call this function an acceleration cache is automatically created. See Geometry::SetupClosestLocationQueries for more
	information.

	\note The "closest distance" relationship may change relative to the spatial reference of the geometry and the input positions.
		See Geometry::SetupClosestLocationQueries for more information.

	\tip The returned point locators can be evaluated on any %Geometry instance having the same topology.

	\param in_position Position from which the spatial search will be initiated.
	\param in_radius Maximum distance (cutoff) from in_PositionToSearchFrom
	\param in_nbToSearch Maximum number of point locators to find (no maximum if -1)
	\return A new PointLocatorData object containing a  point locators (.IsValid() == false if failed)

	\sa Geometry::GetClosestLocations, Geometry::SetupClosestLocationQueries
	\since 5.0

	\eg For an example using this function, see PointLocatorData::GetCount.
	*/
	PointLocatorData GetClosestLocationsWithinRadius
	(
		const MATH::CVector3&		in_position,
		double				in_radius,
		LONG				in_nbToSearch = -1
	) const;

	/*! Sets up how subsequent calls to Geometry::GetClosestLocations and Geometry::GetClosestLocationsWithinRadius will operate.

	An acceleration cache is built either on the first call to this function or to Geometry::GetClosestLocations or
	Geometry::GetClosestLocationsWithinRadius. The cache assumes that the geometry is not deformed during all subsequent calls
	to Geometry::GetClosestLocations and Geometry::GetClosestLocationsWithinRadius. If that is not the case, you should get the
	geometry again or force a rebuild of the acceleration cache by calling this function, otherwise you may get wrong results.

	\note In general setting up the closest location calls with the right transformation is important since the "closest distance"
		relationship may change relative to the spatial reference of the geometry and the input positions. For instance, if you
		copy closest locations of an object's points onto another one, you will generally have different results if you scale both
		of them non-uniformly or if you add some shearing to the pose. In such case, computing the closest locations in local space
		will lead to different results from computing the closest locations in global space.

	\tip Specifying a subset of facets as targets of the search allows faster searches and a lower memory usage for the acceleration cache.

	\param in_method Method to use when searching the closest location (see ::siClosestLocationMethod for possible values).
	\param in_pTransfo Reference transformation in which the input positions will be interpreted, and in which the search
		will be executed.
	\param in_nbFacetsToRestrictSearch Number of facets on which the search will be restricted (or -1 for no restriction)
	\param in_pFacetsToRestrictSearch Facet indices (size = in_nbFacetsToRestrictSearch). Not used if
		\c in_nbFacetsToRestrictSearch is set to -1.
	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa Geometry::GetClosestLocations, Geometry::GetClosestLocationsWithinRadius
	\since 5.0

	\eg Demonstrates using the PointLocatorData to shrink-wrap a sphere on only two faces of a cube
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject meshCubeObj;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", meshCubeObj );
		PolygonMesh meshCubeGeom( meshCubeObj.GetActivePrimitive().GetGeometry() );

		X3DObject meshSphereObj;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", meshSphereObj );
		PolygonMesh meshSphereGeom( meshSphereObj.GetActivePrimitive().GetGeometry() );

		meshSphereObj.PutParameterValue(L"subdivu", 24l);
		meshSphereObj.PutParameterValue(L"subdivv", 24l);

		// We must freeze it, otherwise setting its position array will be forbidden:
		CValueArray args(3);
		CValue outArg;
		args[0] = meshSphereObj.GetRef();
		app.ExecuteCommand(L"FreezeObj",args, outArg);

		LONG facets[2] = {2,3};
		meshCubeGeom.SetupClosestLocationQueries(siClosestSurface,0,2,facets);

		MATH::CVector3Array posArray = meshSphereGeom.GetPoints().GetPositionArray();
		PointLocatorData sphereOnCubePointLocators = meshCubeGeom.GetClosestLocations(posArray.GetCount(), (double*)&posArray[0]);
		meshCubeGeom.EvaluatePositions(sphereOnCubePointLocators, -1, 0, (double*)&posArray[0]);
		meshSphereGeom.GetPoints().PutPositionArray(posArray);
	\endcode
	*/
	CStatus	SetupClosestLocationQueries
	(
		siClosestLocationMethod in_method = siClosestSurface,
		MATH::CTransformation* in_pTransfo = NULL,
		LONG in_nbFacetsToRestrictSearch = -1,
		const LONG* in_pFacetsToRestrictSearch = NULL
	);

	/*! Evaluates the positions at the surface locations defined by the input point locator data. The returned positions
	are in the geometry's local space reference.

	\param in_ptLocators Contains the point locations to be evaluated.
	\param in_nbPointLocatorsIndices Number of point locators to be evaluated (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be evaluated (not used if in_nbPointLocatorsIndices is -1)
	\retval out_pPositions Returns evaluated positions' packed XYZ. Size must be \c in_ptLocators::GetCount()*3 if
		\c in_nbPointLocatorsIndices is set to -1; otherwise, size must be \c in_nbPointLocatorsIndices*3 .
	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa Geometry::EvaluateNormals, Geometry::EvaluateClusterProperty
	\since 5.0

	\eg For an example using this function, see Geometry::GetClosestLocations.
	*/
	CStatus EvaluatePositions
	(
		const PointLocatorData& in_ptLocators,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		double* out_pPositions
	) const;

	/*! Evaluates the surface normals at the surface locations defined by the input point locator data. The returned normals are in
	the geometry's local space reference. In the case of PolygonMesh, various computation methods are available.

	\param in_ptLocators Contains the point locations to be evaluated.
	\param in_ComputationMethod Used method in order to compute the normals (only revelant to PolygonMesh geometries)
	\param in_nbPointLocatorsIndices Number of point locators to be evaluated (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be evaluated (not used if in_nbPointLocatorsIndices is -1)
	\retval out_pNormals Returns evaluated normals' packed XYZ. Size must be \c in_ptLocators::GetCount()*3 if
		\c in_nbPointLocatorsIndices is set to -1; otherwise, size must be \c in_nbPointLocatorsIndices*3
	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa Geometry::EvaluatePositions, Geometry::EvaluateClusterProperty
	\since 5.0

	\eg For an example using this function, see Geometry::GetClosestLocations.
	*/
	CStatus EvaluateNormals
	(
		const PointLocatorData& in_ptLocators,
		siNormalComputationMethod in_ComputationMethod,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		double* out_pNormals
	) const;

	/*! Evaluates the cluster property data at the surface locations defined by the point locator data.

	The size of the returned array depends on the cluster property type, which can be queried using ClusterProperty::GetValueSize.

	If a point locator doesn't correspond exactly to a component defining a value for the cluster property (for example,
	sampled point for UVs), then the value will be interpolated between adjacent components. In the case of NurbsSurfaceMesh,
	the returned values will be smoothed according to the NURBS surface mathematical equation since some properties are defined
	on control vertices (for example, weight maps) while point locators are defined on the surface.

	If a point locator corresponds to a discontinuity in property space, the returned value will be an arbitrary one among the
	many values corresponding to that location.

	\note The parent cluster is required in addition to the cluster property even if it is unambiguous to get the parent cluster
		from the cluster property. The reason is that, in the context of a custom operator, both the cluster and the cluster
		property must come from the value of an input port, otherwise evaluation errors will occur.

	\param in_ptLocators Contains the point locations to be evaluated.
	\param in_nbPointLocatorsIndices Number of point locators to be evaluated (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be evaluated (not used if in_nbPointLocatorsIndices is -1)
	\param in_parentCluster The parent cluster of the cluster property to be evaluated.
	\param in_clusterProperty The cluster property to be evaluated.
	\retval out_pData Returns evaluated cluster property's packed data. Size must be
		\c in_ptLocators::GetCount() \c * \c in_clusterProperty::GetValueSize() if \c in_nbPointLocatorsIndices is -1;
		otherwise, \c in_nbPointLocatorsIndices \c * \c in_clusterProperty::GetValueSize().
	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa Geometry::EvaluatePositions, Geometry::EvaluateNormals
	\since 5.0

	\eg For an example using this function, see PointLocatorData.
	*/
	CStatus EvaluateClusterProperty
	(
		const PointLocatorData& in_ptLocators,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		const Cluster& in_parentCluster,
		const ClusterProperty& in_clusterProperty,
		float* out_pData
	) const;


	/*! Calculates and returns bounding box information
	\retval out_centerx  Returns the x coordinate of the center of the bounding box.
	\retval out_centery  Returns the y coordinate of the center of the bounding box.
	\retval out_centerz  Returns the z coordinate of the center of the bounding box.
	\retval out_extentx  Returns the extent of the bounding box in the X axis.
	\retval out_extenty  Returns the extent of the bounding box in the Y axis.
	\retval out_extentz  Returns the extent of the bounding box in the Z axis.
	\param in_XfoObjectToBBoxSpace  Contains a transform (if desired) between the object and the global coordinate axes. This
		can be used to more tightly orient a bounding box around the object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\sa Geometry::GetBoundingSphere, Geometry::GetBoundingCapsule
	\since 5.0
	*/
	CStatus GetBoundingBox
	(
	   double                & out_centerx,
	   double                & out_centery,
	   double                & out_centerz,
	   double                & out_extentx,
	   double                & out_extenty,
	   double                & out_extentz,
	   const MATH::CTransformation & in_XfoObjectToBBoxSpace
	) const;


	/*! Calculates and returns bounding sphere information
	\retval out_centerx  Returns the x coordinate of the center of the bounding sphere.
	\retval out_centery  Returns the y coordinate of the center of the bounding sphere.
	\retval out_centerz  Returns the z coordinate of the center of the bounding sphere.
	\retval out_radius  Returns the radius of the bounding sphere.
	\param in_centerMethod Specifies the technique used to calculate the center of the bounding sphere
	\param in_XfoObjectToBSphereSpace  Contains a transform (if desired) between the object and the global coordinate axes. This
		can be used to more tightly orient a bounding sphere around the object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	\sa Geometry::GetBoundingBox, Geometry::GetBoundingCapsule
	\since 5.0
	*/
	CStatus GetBoundingSphere
	(
	   double                      & out_centerx,
	   double                      & out_centery,
	   double                      & out_centerz,
	   double                      & out_radius,
	   siVolumeCenterMethod          in_centerMethod,
	   const MATH::CTransformation & in_XfoObjectToBSphereSpace
    );

	/*! Calculates and returns bounding capsule (swept sphere) information
	\retval out_centerx  Returns the x coordinate of the center of the bounding capsule.
	\retval out_centery  Returns the y coordinate of the center of the bounding capsule.
	\retval out_centerz  Returns the z coordinate of the center of the bounding capsule.
	\retval out_length  Returns the length of the bounding capsule (not including the radii of the capping hemispheres).
	\retval out_radius  Returns the cylindrical radius of the bounding capsule.
	\param in_centerMethod Specifies the technique used to calculate the center of the bounding capsule
	\param in_axisMethod Specifies the technique used to calculate the long axis of the bounding capsule
	\param in_XfoObjectToBCapsuleSpace  Contains a transform (if desired) between the object and the global coordinate axes. This
		can be used to more tightly orient a bounding capsule around the object
	\return CStatus::OK success
	\return CStatus::Fail failure
	\sa Geometry::GetBoundingBox, Geometry::GetBoundingSphere
	\since 5.0
	*/
	CStatus GetBoundingCapsule
	(
	    double                        & out_centerx,
		double                        & out_centery,
		double                        & out_centerz,
		double                        & out_length,
		double                        & out_radius,
		siVolumeCenterMethod            in_centerMethod,
		siBoundingCapsuleMethod         in_axisMethod,
		const MATH::CTransformation   & in_XfoObjectToBCapsuleSpace
	) ;


	private:
	Geometry * operator&() const;
	Geometry * operator&();
};

};

#endif // __XSIGEOMETRY_H__
