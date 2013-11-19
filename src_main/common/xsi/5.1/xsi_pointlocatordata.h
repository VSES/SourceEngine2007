//*****************************************************************************
/*!
  \file xsi_pointlocatordata.h
  \brief PointLocatorData class declaration.

  © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
  reserved. This file contains confidential and proprietary information of
  Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
  end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPOINTLOCATORDATA_H__
#define __XSIPOINTLOCATORDATA_H__

#include <xsi_base.h>
#include <xsi_ppgitem.h>
#include <xsi_value.h>
namespace XSI {

//*****************************************************************************
/*! \class PointLocatorData xsi_pointlocatordata.h
	\brief A %PointLocatorData object represents a collection of point locators. A point locator
	is a geometric surface coordinate, and represents a precise location on a geometry.

	%Point locators are topologically defined, so they are not dependent on the position
	or the deformation of the geometry (they "stick" to the surface). The actual data
	defining a point locator is abstracted and depends on the geometry type.

	%Point locators are a generalization of Point. As %points, point locators can
	be processed independently from the geometry type. Like point indices, point
	locators are not related to a particular geometry instance. You can query any
	geometry having the same topology with the same %PointLocatorData. For instance,
	you can use point locators to evaluate positions of an animated geometry at different times.

	Because point locators don't apply to a particular geometry instance, the %PointLocatorData
	object has no functionality by itself. Most methods related to %PointLocatorData are
	defined in Geometry, PolygonMesh and NurbsSurfaceMesh. %PointLocatorData can be created
	by the following methods:
		\li Geometry::GetClosestLocations
		\li Geometry::GetClosestLocationsWithinRadius
		\li Geometry::GetSurfacePointLocatorsFromPoints
		\li PolygonMesh::ConstructPointLocators
		\li NurbsSurfaceMesh::ConstructPointLocators

	\note %Point locators are currently only supported by NurbsSurfaceMesh and PolygonMesh objects.

	Other examples of %PointLocatorData usage can be found in various related methods such as
	PointLocatorData::Count and PolygonMesh::ConstructPointLocators. A JScript example of
	a scripted operator can be found at \xl %PointLocatorData .

	\sa Geometry, Geometry::GetClosestLocations, Geometry::GetClosestLocationsWithinRadius,
		Geometry::SetupClosestLocationQueries, Geometry::GetSurfacePointLocatorsFromPoints,
		Geometry::EvaluatePositions, Geometry::EvaluateNormals, Geometry::EvaluateClusterProperty,
		PolygonMesh::GetPolygonIndexArray, PolygonMesh::GetTriangleVertexIndexArray,
		PolygonMesh::GetTriangleNodeIndexArray, PolygonMesh::GetTriangleWeightArray,
		PolygonMesh::ConstructPointLocators, NurbsSurfaceMesh::GetSubSurfaceIndexArray,
		NurbsSurfaceMesh::GetNormalizedUVArray, NurbsSurfaceMesh::ConstructPointLocators

	\since 5.0

	\eg This example uses PointLocatorData to deform a polygon mesh based on
		the closest weight map value of a NURBS surface mesh.

	\code
		using namespace XSI;

		// Scene creation:
		//   Polygon mesh grid and a NURBS surface grid.
		//   On the NURBS grid, a weight map having a radial gradient

		Application app;
		Model root = app.GetActiveSceneRoot();
		X3DObject meshGridObj;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", meshGridObj );

		meshGridObj.PutParameterValue(L"subdivu", 24l);
		meshGridObj.PutParameterValue(L"subdivv", 24l);

		// We must freeze it, otherwise setting its position array will be forbidden:
		CValueArray args(3);
		CValue outArg;
		args[0] = meshGridObj.GetRef();
		app.ExecuteCommand(L"FreezeObj",args, outArg);

		Geometry meshGridGeom( meshGridObj.GetActivePrimitive().GetGeometry() );

		X3DObject NURBSGridObj;
		root.AddGeometry( L"Grid", L"NurbsSurface", L"", NURBSGridObj );
		NURBSGridObj.PutParameterValue(L"subdivu", 2l);
		NURBSGridObj.PutParameterValue(L"subdivv", 2l);

		NurbsSurfaceMesh NURBSGridGeom( NURBSGridObj.GetActivePrimitive().GetGeometry() );

		args.Clear();
		args.Resize(4);
		args[0] = CValue( CString(L"WeightMap") );
		args[1] = CValue( NURBSGridObj.GetRef() );
		args[2] = CValue(CString(L"MyWeightMap"));
		args[3] = CValue(CString(L"Weight Map Property"));
		app.ExecuteCommand( L"CreateWeightMap", args, outArg );
		ClusterProperty NURBSWeightMap(Cluster(NURBSGridGeom.GetClusters()[0]).GetProperties()[0]);

		args.Clear();
		args.Resize(2);
		args[0] = CValue(NURBSWeightMap.GetFullName() + L".weightmapop.type");
		args[1] = 5l ;
		app.ExecuteCommand( L"SetValue", args, outArg ) ;
		args[0] = CValue(NURBSWeightMap.GetFullName() + L".weightmapop.weight");
		args[1] = 1l ;
		app.ExecuteCommand( L"SetValue", args, outArg ) ;
		args[0] = CValue(NURBSWeightMap.GetFullName() + L".weightmapop.invert");
		args[1] = true ;
		app.ExecuteCommand( L"SetValue", args, outArg ) ;

		// Get the weight map again to have an updated copy
		ClusterProperty NURBSWeightMap2(Cluster(NURBSGridGeom.GetClusters()[0]).GetProperties()[0]);

		// Apply the deformation
		MATH::CVector3Array posArray = meshGridGeom.GetPoints().GetPositionArray();
		PointLocatorData pointLocators = NURBSGridGeom.GetClosestLocations(posArray.GetCount(), (double*)&posArray[0]);

		std::vector<float> data;
		data.resize(NURBSWeightMap2.GetValueSize()*pointLocators.GetCount());
		NURBSGridGeom.EvaluateClusterProperty(pointLocators, -1, NULL, NURBSWeightMap2.GetParent(), NURBSWeightMap2, &data.front());

		for(LONG i = 0; i < posArray.GetCount(); i++)
			posArray[i].PutY(posArray[i].GetY() + data[i]*5);

		meshGridGeom.GetPoints().PutPositionArray(posArray);
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL PointLocatorData : public CBase
{
public:
	/*! Default constructor. */
	PointLocatorData();

	/*! Default destructor. */
	~PointLocatorData();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PointLocatorData(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PointLocatorData(const PointLocatorData& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object.
	\param in_obj constant class object.
	\return The new PointLocatorData object.
	*/
	PointLocatorData& operator=(const PointLocatorData& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PointLocatorData object.
	*/
	PointLocatorData& operator=(const CRef& in_ref);

	/*! Returns the number of point locators contained in this object.
	\return Number of point locators.

	\eg This example uses PointLocatorData to find closest vertices on a polygon mesh
		within a specific radius, and then creates a cluster on these points.

	\code
		Application app;
		Model root = app.GetActiveSceneRoot();
		X3DObject meshGridObj;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", meshGridObj );
		PolygonMesh meshGridGeom( meshGridObj.GetActivePrimitive().GetGeometry() );

		CValueArray args(8);
		CValue outArg;
		args[0] = CValue(meshGridObj.GetRef());
		args[1] = CValue(L"0");
		args[2] = CValue(L"0");
		args[3] = CValue(L"-2");
		args[4] = CValue(L"siRelative");
		args[5] = CValue(L"siView");
		args[6] = CValue(L"siObj");
		args[7] = CValue(L"siXYZ");
		app.ExecuteCommand( L"Translate", args, outArg );

		// Set up to closest vertex search
		meshGridGeom.SetupClosestLocationQueries(siClosestVertexOrKnot, &meshGridObj.GetKinematics().GetGlobal().GetTransform());

		// Get all vertices within a radius of 2 units relatively to the world center
		MATH::CVector3 positionToQuery(0,0,0);
		PointLocatorData pointLocatorsWithinTwoUnits = meshGridGeom.GetClosestLocationsWithinRadius(positionToQuery, 2.0);

		CLongArray indices;

		for(LONG i = 0; i < pointLocatorsWithinTwoUnits.GetCount(); i++)
		{
			// Even though the point locators have be defined to exactly match the vertex
			// positions, the search data is returned as a position on a specific triangle,
			// just as any other use of the Point Locator. However the vertex that matches
			// can be determined because it will be weighted at 100%.
			LONG subTriangleVertices[3];
			meshGridGeom.GetTriangleVertexIndexArray(pointLocatorsWithinTwoUnits, 1, &i, subTriangleVertices);

			float subTriangleWeights[3];
			meshGridGeom.GetTriangleWeightArray(pointLocatorsWithinTwoUnits, 1, &i, subTriangleWeights);

			LONG vtxIdx = subTriangleVertices[0];
			if(subTriangleWeights[1] > subTriangleWeights[0] && subTriangleWeights[1] > subTriangleWeights[2])
				vtxIdx = subTriangleVertices[1];
			else if(subTriangleWeights[2] > subTriangleWeights[0] && subTriangleWeights[2] > subTriangleWeights[1])
				vtxIdx = subTriangleVertices[2];

			indices.Add(vtxIdx);
		}
		// Create a cluster on these vertices
		Cluster vtxCluster;
		meshGridGeom.AddCluster(siVertexCluster, L"ClosestVertices", indices, vtxCluster);
	\endcode
	*/
	LONG GetCount() const;

	private:
	PointLocatorData * operator&() const;
	PointLocatorData * operator&();
};

};
#endif // __XSIPOINTLOCATORDATA_H__
