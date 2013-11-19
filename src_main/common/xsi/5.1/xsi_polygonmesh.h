//*****************************************************************************
/*!
   \file xsi_polygonmesh.h
   \brief PolygonMesh class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPOLYGONMESH_H__
#define __XSIPOLYGONMESH_H__

#include <xsi_geometry.h>
#include <xsi_clusterproperty.h>
#include <xsi_pointlocatordata.h>

namespace XSI {

class CVertexRefArray;
class CEdgeRefArray;
class CPolygonFaceRefArray;
class CPolygonNodeRefArray;
class CGeometryAccessor;
class CMeshBuilder;
class CClusterPropertyBuilder;

//*****************************************************************************
/*! \class PolygonMesh xsi_polygonmesh.h
	\brief A PolygonMesh is a kind of Geometry.

	We can navigate the PolygonMesh using the generic terms such as Facet, Segment or Point or we can choose to
	navigate using terms that are specific to the PolygonMesh such as PolygonFace, Edge and Vertex. There are some
	functions that do not fit with the generic terms such as PolygonNode, which are specific to PolygonMesh, this
	is why there are specific terms as well as generic terms.

	\sa Primitive::GetGeometry, X3DObject::AddPolygonMesh

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		PolygonMesh mesh( myGrid.GetActivePrimitive().GetGeometry(0) );

		CPolygonFaceRefArray polygonArray( mesh.GetPolygons() );

		CValue val( polygonArray.GetCount() );
		app.LogMessage( CString(L"Number of polygons: ") + val.GetAsText() );

	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL PolygonMesh : public Geometry
{
public:
	/*! Default constructor. */
	PolygonMesh();

	/*! Default destructor. */
	~PolygonMesh();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PolygonMesh(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PolygonMesh(const PolygonMesh& in_obj);

	/*! Copy constructor.
	\param in_obj Geometry object.
	*/
	PolygonMesh(const Geometry& in_obj);

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
	\return The new PolygonMesh object.
	*/
	PolygonMesh& operator=(const PolygonMesh& in_obj);

	/*! Creates a Polygon object from a Geometry object. The newly created object is
	set to empty if the input Geometry object is not compatible.
	\param in_geom constant class object.
	\return The new PolygonMesh object.
	*/
	PolygonMesh& operator=(const Geometry& in_geom);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PolygonMesh object.
	*/
	PolygonMesh& operator=(const CRef& in_ref);

	/*! Returns an array of of all Vertex objects on this PolygonMesh object.
	\return Array of Vertex objects.
	*/
	CVertexRefArray GetVertices() const;

	/*! Returns an array of all Edge objects on this PolygonMesh object.
	\return Array of Edge objects.
	*/
	CEdgeRefArray GetEdges() const;

	/*! Returns an array of all PolygonFace objects on this PolygonMesh object.
	\return Array of PolygonFace objects.
	*/
	CPolygonFaceRefArray GetPolygons() const;

	/*! Returns an array of all PolygonNode objects on this PolygonMesh object.
	\return Array of PolygonNode objects.
	*/
	CPolygonNodeRefArray GetNodes() const;

	/*! Sets the PolygonMesh from a raw data set, this is only available from
	custom operators. If this function is used on a polygon mesh with some
	clusters and a topology change is performed, the caller is responsible for
	updating the clusters.

	\param in_vertices Array representing the polygon vertices. Each vertex is
		expressed with a CVector3 structure.
	\param in_polygonDescr An ordered array of polygon definitions, each polygon
		is defined by a list of elements, the first element of a polygon definition
		must be set with the number of indices for that polygon. The ordering of
		vertices must respect a ccw ordering to get out going normals (right-hand
		rule). For instance, an array of polygons with 4 indices each is formatted
		as {4,0,1,4,3,4,1,2,5,4...}

	\return CStatus::OK success
	\return CStatus::AccessDenied function used outside of a plug-in operator context.

  	\eg

	\code
		using namespace XSI;
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"", myCone );

		PolygonMesh meshCube( myCube.GetActivePrimitive().GetGeometry() );

		//Getting the polygon mesh description
		CVector3Array	vertices;
		CLongArray		polygonData;
		meshCube.Get(vertices,polygonData);

		//Freezing the geometry
		CValueArray args(3);
		CValue outArg;
		args[0] = myCone.GetRef();
		app.ExecuteCommand(L"FreezeObj",args, outArg);

		PolygonMesh meshCone( myCone.GetActivePrimitive().GetGeometry() );

		//converting the cone to a cube.
		meshCone.Set(vertices,polygonData);
	\endcode
	*/
	CStatus Set( const MATH::CVector3Array& in_vertices,
			const CLongArray& in_polygonDescr );

	/*! Returns a complete data set description of a polygon mesh.

	\param io_vertices Array representing the polygon vertices. Each vertex is
		expressed with a CVector3 structure.
	\param io_polygonDescr An ordered array of polygon definitions, each polygon
		is defined by a list of elements, the first element of a polygon definition
		must be set with the number of indices for that polygon. The ordering of
		vertices must respect a ccw ordering to get out going normals (right-hand
		rule). For instance, an array of polygons with 4 indices each is formatted
		as {4,0,1,4,3,4,1,2,5,4...}

	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Get( MATH::CVector3Array& io_vertices, CLongArray& io_polygonDescr ) const;

	/*! Sets the current color at vertices property used by the polygon mesh's material.
	Using the default argument clears the current vertex color property.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutCurrentVertexColor( const ClusterProperty& = ClusterProperty());

	/*! Returns the current vertex color property used by the polygon mesh's material.
	When you first create a polygon mesh it inherits a material but doesn't automatically
	have a vertex color. You need to explicitly set the vertex color to be used by the
	material (this then becomes the current vertex color). The current color at vertices property
	may not be set, in which case the first vertex color property found will be returned.
	If the polygon mesh has no vertex color properties then
	an empty ClusterProperty API class will be returned. Use the IsValid() method
	to check if the cluster property is valid.
	\return ClusterProperty
	*/
	ClusterProperty GetCurrentVertexColor( void ) const;

	/*! Adds a new vertex color property to this polygon mesh object.
	\param name name of new vertex color.
	\return The vertex color property as a ClusterProperty.
	*/
	ClusterProperty AddVertexColor( const CString& name = CString() );

	/*! Returns an array of the vertex color properties installed on this polygon mesh object.
	\return Array of vertex color properties.
	*/
	CRefArray GetVertexColors( void ) const;

	/*! Returns a geometry accessor object which gives access to the mesh and
	subdivided geometry data. 
	Note: The CGeometryAccessor object is not accessible from an operator.

	\param in_mode The construction mode is used to access a version of the geometry with 
		specific deforms. The geometry positions you get depends on the mode you pass in, 
		for instance, when in_mode is set to ::siConstructionModeModeling, you get the 
		original geometry positions. This mode is typically used in export applications where 
		geometry, shape and envelope positions are exported separately. Other modes of interest 
		(see ::siConstructionMode) include:

		\li \c siConstructionModePrimaryShape: Combines the geometry positions with the shape 
			deformation.

		\li \c siConstructionModeAnimation: Combines the geometry positions with the shape and 
			envelope deformation altogether.

		\li \c siConstructionModeSecondaryShape: Combines the geometry positions with the shape 
			and envelope deformation and the deforms installed above the envelope such as the 
			move point operators. This mode is typically used for plotting the final results of 
			shape and envelope deformation.

		\li \c siConstructionModeDefault: Uses the current construction mode set in XSI.

	\param in_type The algorithm used for subdividing the geometry.
	\param in_subdLevel The level of subdivision of the geometry. Defaults to 0.
	\param in_bUseLoopForTriangles This argument is specific to the subdivision
		and provides a more precise subdivision when set to true. The argument is
		ignored when in_subdLevel is 0. Defaults to false.
	\param in_bUseDiscontinuity Allows computing the discontinuity of normals
		when accessing the normal values of the geometry. If this flag is false the
		resulting normals are smoothed. Defaults to true.
	\param in_discontinuityAngle Specifies the angle of discontinuity when
		accessing the normal values on the geometry. This argument is ignored if
		in_bUseDiscontinuity is false. Defaults to 60.0.

	\return CGeometryAccessor object.
	\since 5.0
	*/
	CGeometryAccessor GetGeometryAccessor
	(
		siConstructionMode		in_mode = siConstructionModeModeling,
		siSubdivisionRuleType	in_type = siCatmullClark,
		LONG					in_subdLevel = 0,
		bool					in_bUseLoopForTriangles = false,
		bool					in_bUseDiscontinuity = true,
		double					in_discontinuityAngle = 60.0
	) const;

	/*! Returns a mesh builder object.
	\return CMeshBuilder object.
	\since 5.0
	*/
	CMeshBuilder GetMeshBuilder() const;

	/*! Returns a cluster property builder object.
	\return CClusterPropertyBuilder object.
	\since 5.0
	*/
	CClusterPropertyBuilder GetClusterPropertyBuilder() const;

	/*! Returns the polygon indices on which point locators are defined.

	The position within the polygon can be queried with PolygonMesh::GetTriangleVertexIndexArray,
	PolygonMesh::GetTriangleNodeIndexArray and PolygonMesh::GetTriangleWeightArray.

	Notice that this information is part of polygon mesh point locator's definition,
	and depends on the topology only (won't change if the geometry is deformed).

	\param in_ptLocators Contains the point locations to be queried.
	\param in_nbPointLocatorsIndices Number of point locators to be queried (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be queried (not used if in_nbPointLocatorIndices is -1)
	\retval out_pIndices Returned polygon indices. Size must be in_ptLocators.GetCount() if
		in_nbPointLocatorIndices is -1, in_nbPointLocatorIndices otherwise.

	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa PolygonMesh::GetTriangleVertexIndexArray, PolygonMesh::GetTriangleNodeIndexArray,
		PolygonMesh::GetTriangleWeightArray, PolygonMesh::ConstructPointLocators
	\since 5.0

	\eg This example topologically describes the point locators resulting from the
		shrink-wrapping of a cube onto a mesh sphere.

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

		MATH::CVector3Array posArray = meshCubeGeom.GetPoints().GetPositionArray();
		PointLocatorData cubeOnSpherePointLocators = meshSphereGeom.GetClosestLocations(posArray.GetCount(), (double*)&posArray[0]);

		LONG i;
		for(i = 0; i < cubeOnSpherePointLocators.GetCount(); i++)
		{
			LONG polygon;
			meshSphereGeom.GetPolygonIndexArray(
							cubeOnSpherePointLocators,
							1, &i, &polygon);
			LONG nodes[3];
			meshSphereGeom.GetTriangleNodeIndexArray(
							cubeOnSpherePointLocators,
							1, &i, nodes);
			LONG vertices[3];
			meshSphereGeom.GetTriangleVertexIndexArray(
							cubeOnSpherePointLocators,
							1, &i, vertices);
			float weights[3];
			meshSphereGeom.GetTriangleWeightArray(
							cubeOnSpherePointLocators,
							1, &i, weights);

			app.LogMessage(L"Point locator " + CString(CValue(i)) + L" is on polygon " + CString(CValue(polygon)) + L",");
			app.LogMessage(L"on a subtriangle described by nodes ("
						+ CString(CValue(nodes[0])) + L", " + CString(CValue(nodes[1])) + L", " + CString(CValue(nodes[2])) + L")" +
						L" or by vertices ("
						+ CString(CValue(vertices[0])) + L", " + CString(CValue(vertices[1])) + L", " + CString(CValue(vertices[2])) + L")");
			app.LogMessage(L"and having barycentric coordinates ("
						+ CString(CValue(weights[0])) + L", " + CString(CValue(weights[1])) + L", " + CString(CValue(weights[2])) + L")");
			app.LogMessage(L"");
		}
		// Expected results:
		//INFO : Point locator 0 is on polygon 58,
		//INFO : on a subtriangle described by nodes (217, 219, 220) or by vertices (52, 4, 53)
		//INFO : and having barycentric coordinates (0.617317, 0, 0.382683)
		//INFO :
		//INFO : Point locator 1 is on polygon 42,
		//INFO : on a subtriangle described by nodes (157, 159, 160) or by vertices (38, 46, 39)
		//INFO : and having barycentric coordinates (0.617317, 0, 0.382683)
		//INFO :
		//INFO : Point locator 2 is on polygon 61,
		//INFO : on a subtriangle described by nodes (229, 231, 232) or by vertices (55, 7, 56)
		//INFO : and having barycentric coordinates (0.382683, 0, 0.617317)
		//etc.
	\endcode
	*/
	CStatus GetPolygonIndexArray
	(
		const PointLocatorData& in_ptLocators,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		LONG* out_pIndices
	) const;

	/*! Returns the vertex indices of the polygon subtriangle on which the point locators
	are defined.

	Each vertex index triplet is guaranteed to correspond to one polygon (the one returned
	by PolygonMesh::GetPolygonIndexArray). The barycentric position within the triangle can
	be queried with PolygonMesh::GetTriangleWeightArray.

	\warning This information may in some cases vary between geometry instances having
	the same topology but different deformations. Since the point locators are defining
	a precise position on the surface, and since deformations can dynamically change the
	triangulation within a polygon, it can happen that a point locator is remapped to a
	different subtriangle (both barycentric weights and subtriangle indices can vary).

	\param in_ptLocators Contains the point locations to be queried.
	\param in_nbPointLocatorsIndices Number of point locators to be queried (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be queried (not used if in_nbPointLocatorIndices is -1)
	\retval out_pIndices Returned triangle vertex triplets. Size must be 3*in_ptLocators.GetCount() if
		in_nbPointLocatorIndices is -1, 3*in_nbPointLocatorIndices otherwise.

	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa PolygonMesh::GetPolygonIndexArray, PolygonMesh::GetTriangleNodeIndexArray,
		PolygonMesh::GetTriangleWeightArray, PolygonMesh::ConstructPointLocators
	\since 5.0

	\eg An example using this method can be found in PolygonMesh::GetPolygonIndexArray and
		Geometry::GetClosestLocations methods' description.
	*/
	CStatus GetTriangleVertexIndexArray
	(
		const PointLocatorData& in_ptLocators,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		LONG* out_pIndices
	) const;

	/*! Returns the node indices of the polygon subtriangle on which the point locators
	are defined.

	Each node index triplet is guaranteed to correspond to one polygon (the one returned
	by PolygonMesh::GetPolygonIndexArray). The barycentric position within the triangle
	can be queried with PolygonMesh::GetTriangleWeightArray.

	\warning This information may in some cases vary between geometry instances having
	the same topology but different deformations. Since the point locators are defining
	a precise position on the surface, and since deformations can dynamically change the
	triangulation within a polygon, it can happen that a point locator is remapped to a
	different subtriangle (both barycentric weights and subtriangle indices can vary).

	\param in_ptLocators Contains the point locations to be queried.
	\param in_nbPointLocatorsIndices Number of point locators to be queried (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be queried (not used if in_nbPointLocatorIndices is -1)
	\retval out_pIndices Returned triangle node triplets. Size must be 3*in_ptLocators.GetCount() if
		in_nbPointLocatorIndices is -1, 3*in_nbPointLocatorIndices otherwise.

	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa PolygonMesh::GetPolygonIndexArray, PolygonMesh::GetTriangleNodeIndexArray,
		PolygonMesh::GetTriangleWeightArray, PolygonMesh::ConstructPointLocators
	\since 5.0

	\eg An example using this method can be found in PolygonMesh::GetPolygonIndexArray and
		Geometry::GetClosestLocations methods' description.
	*/
	CStatus GetTriangleNodeIndexArray
	(
		const PointLocatorData& in_ptLocators,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		LONG* out_pIndices
	) const;

	/*! Returns the barycentric weight triplets describing the position within the polygon
	subtriangle on which the point locators are defined.

	The components defining the subtriangle queried with methods PolygonMesh::GetTriangleVertexIndexArray
	and PolygonMesh::GetTriangleNodeIndexArray.

	Notice that this information may exceptionally vary between geometry instances having the
	same topology but different deformations. Since the point locators are defining a precise
	position on the surface, and since deformations can dynamically change the triangulation
	within a polygon, it can happen that a point locator is remapped to a different subtriangle
	(both barycentric weights and subtriangle indices can vary).

	\param in_ptLocators Contains the point locations to be queried.
	\param in_nbPointLocatorsIndices Number of point locators to be queried (-1 if all)
	\param in_pPointLocatorsIndices %Point locator indices to be queried (not used if in_nbPointLocatorIndices is -1)
	\retval out_pWeights Returned barycentric weight triplets. Size must be 3*in_ptLocators.GetCount() if
		in_nbPointLocatorIndices is -1, 3*in_nbPointLocatorIndices otherwise.

	\return CStatus::OK success
	\return CStatus::Fail failure

	\sa PolygonMesh::GetPolygonIndexArray, PolygonMesh::GetTriangleNodeIndexArray,
		PolygonMesh::GetTriangleWeightArray, PolygonMesh::ConstructPointLocators
	\since 5.0

	\eg An example using this method can be found in PolygonMesh::GetPolygonIndexArray and
		Geometry::GetClosestLocations methods' description.
	*/
	CStatus GetTriangleWeightArray
	(
		const PointLocatorData& in_ptLocators,
		LONG in_nbPointLocatorsIndices, //-1 if all
		const LONG* in_pPointLocatorsIndices, // 0 if all
		float* out_pWeights
	) const;

	/*! Creates a PointLocatorData from %PolygonMesh specific topological information.
	Polygon indices, vertex indices and subtriangle barycentric weights (normalized
	and positive) are required in order to define each point locator. The vertex
	indices of a subtriangle must all be part of the corresponding input polygon.

	In order to have more predictable results, it is recommended to specify the
	subtriangles of vertices corresponding to the actual triangulation of the polygons.
	Actual polygon triangulation can be retrieved with PolygonFace::TriangleSubIndexArray.

	Notice that the returned point locators can be evaluated on any %PolygonMesh instance
	having the same topology.

	\param in_nbPointLocators Number of points locators to be constructed
	\param in_pPolygonIndices Polygon indices (size must be in_nbPoints)
	\param in_pSubTriangleVertexIndices Vertex indices on the polygon (size must be in_nbPoints*3)
	\param in_pSubTriangleWeights Triangle barycentric weights (size must be in_nbPoints*3)

	\return A new PointLocatorData object
	\return IsValid() == false if failed

	\sa PolygonMesh::GetPolygonIndexArray, PolygonMesh::GetTriangleVertexIndexArray,
		PolygonMesh::GetTriangleNodeIndexArray, PolygonMesh::GetTriangleWeightArray,
		Geometry::GetSurfacePointLocatorsFromPoints, NurbsSurfaceMesh::ConstructPointLocators
	\since 5.0

	\eg This example creates point locators at random surface locations,
		and positions a Null at each of these point locators.

	\code
		using namespace XSI;

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

		float GetNormalizedRandom(){return float(rand())/RAND_MAX;}

		LONG RandInRange(LONG in_bound)
		{
			float fRand = GetNormalizedRandom()*0.99999f;
			return (LONG)(in_bound*fRand);
		}

		void main()
		{
			//	Even if these point locators are simply randomly constructed,
			//	this example shows how to do it in the most accurate way,
			//	which is to specify subtriangles which correspond to the actual
			//	polygon triangulation.
			Application app;
			Model root = app.GetActiveSceneRoot();

			X3DObject meshSphereObj;
			root.AddGeometry( L"Sphere", L"MeshSurface", L"", meshSphereObj );
			PolygonMesh meshSphereGeom( meshSphereObj.GetActivePrimitive().GetGeometry() );

			LONG nbPtLocators = 50;

			std::vector<LONG> polygonIndexArray;
			std::vector<LONG> subTriangleVertexArray;
			std::vector<float> subTriangleWeightArray;

			polygonIndexArray.resize(nbPtLocators);
			subTriangleVertexArray.resize(nbPtLocators*3);
			subTriangleWeightArray.resize(nbPtLocators*3);

			CPolygonFaceRefArray polygons = meshSphereGeom.GetPolygons();

			LONG i;
			for(i = 0; i < nbPtLocators; i++)
			{
				polygonIndexArray[i] = RandInRange(polygons.GetCount());

				PolygonFace polygon = polygons[polygonIndexArray[i]];

				LONG subTri = RandInRange(polygon.GetNbPoints()-2);
				CLongArray polygonNeiVtxIndices = polygon.GetVertices().GetIndexArray();

				CLongArray triangleSubIndices = polygon.GetTriangleSubIndexArray();
				subTriangleVertexArray[i*3] = polygonNeiVtxIndices[triangleSubIndices[subTri*3]];
				subTriangleVertexArray[i*3+1] = polygonNeiVtxIndices[triangleSubIndices[subTri*3+1]];
				subTriangleVertexArray[i*3+2] = polygonNeiVtxIndices[triangleSubIndices[subTri*3+2]];

				float weights[3] = {GetNormalizedRandom(), GetNormalizedRandom(), GetNormalizedRandom()};
				float wsum = weights[0]+weights[1]+weights[2];
				subTriangleWeightArray[i*3] = weights[0]/wsum;
				subTriangleWeightArray[i*3+1] = weights[0]/wsum;
				subTriangleWeightArray[i*3+2] = weights[0]/wsum;
			}
			PointLocatorData randomPointLocators = meshSphereGeom.ConstructPointLocators(nbPtLocators, &polygonIndexArray.front(), &subTriangleVertexArray.front(), &subTriangleWeightArray.front());

			CreateNullsAtPointLocations(meshSphereObj, randomPointLocators);
		}
	\endcode
	*/
	PointLocatorData ConstructPointLocators
	(
		LONG					in_nbPointLocators,
		const LONG*				in_pPolygonIndices,
		const LONG*				in_pSubTriangleVertexIndices,
		const float*			in_pSubTriangleWeights
	) const;

	private:
	PolygonMesh * operator&() const;
	PolygonMesh * operator&();
};

};

#endif // __XSIPOLYGONMESH_H__
