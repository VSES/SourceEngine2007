//*****************************************************************************
/*!
   \file xsi_geometryaccessor.h
   \brief CGeometryAccessor class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIGEOMACCESSOR_H__
#define __XSIGEOMACCESSOR_H__

#include <xsi_siobject.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_transformation.h>

#pragma warning(disable:4251)

namespace XSI {

class CBitArray;
class CLongArray;
class CDoubleArray;
class CFloatArray;
class CRefArray;

//*****************************************************************************
/*! \class CGeometryAccessor xsi_geometryaccessor.h
	\brief The mesh geometry accessor provides an optimized access to the data and cluster property values of a geometry
	mesh object. CGeometryAccessor is suitable for data export application as it allows access to geometry data more quickly
	and efficiently than the regular geometry API classes.

	CGeometryAccessor lets you access the regular geometry data as defined on a mesh or you can configure CGeometryAccessor
	to subdivide the data in order to obtain a higher level of resolution on the geometry. Be careful, however: the higher
	the resolution the longer the access time will be.

	With CGeometryAccessor you can access regular geometry data such as:
		\li Polygons
		\li Vertices
		\li Nodes
		\li Edges
		\li Normals
		\li Triangles

	\note Triangles are generated from the mesh polygons in the exact same order.

	You can also access quickly all materials and cluster property values used by the mesh. The cluster property supported are:
		\li UVs
		\li User normals
		\li Vertex colors
		\li Shape keys
		\li Weight maps
		\li Envelope weights

	\note User data map values can also be accessed directly with CGeometryAccessor.

	The Material objects returned by CGeometryAccessor::GetMaterials can be used to optimize the access to the UV and vertex
	color values defined on these materials. With this shortcut you tackle only the materials used by the mesh which saves you
	from processing unused materials during your export operations.

	\sa PolygonMesh::GetGeometryAccessor, CClusterPropertyBuilder
	\since 5.0

	\eg Enumerates polygon vertex indices

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", mySphere );

		PolygonMesh m = mySphere.GetActivePrimitive().GetGeometry();

		// gets a geometry accessor object with default parameters
		CGeometryAccessor ga = m.GetGeometryAccessor();

		// get the number of vertices for each polygons
		CLongArray pvCount;
		ga.GetPolygonVerticesCount(pvCount);

		// get the vertex indices
		CLongArray vIndices;
		ga.GetVertexIndices(vIndices);

		// log all polygon vertex indices
		LONG nPolyCount = ga.GetPolygonCount();
		for (LONG i=0, nOffset=0; i<nPolyCount; i++)
		{
			CString strIndices;

			for ( LONG j=0; j<pvCount[i]; j++ )
			{
				strIndices += CString(vIndices[nOffset+j]);
			}
			nOffset += pvCount[i];
			app.LogMessage( CString(i) + L":" + strIndices );
		}
		\endcode

		\b Example2 : Get the UV values for all triangles of a geometry
		\code
		using namespace XSI;

		// Forward declaration
		PolygonMesh CreateMeshWithUVs( );

		// setup a mesh with some UV values
		PolygonMesh m = CreateMeshWithUVs( );

		// gets a geometry accessor object from the new mesh
		CGeometryAccessor ga = m.GetGeometryAccessor( );

		// gets all UVs on the mesh
		CRefArray uvs = ga.GetUVs( );

		// works with the first UV set on the geometry
		// UV values are stored as a flat list of float values grouped in
		// triplets (i.e. UVW), first triplet being the values at node 0, etc...
		ClusterProperty uvProp = uvs[ 0 ];
		CFloatArray uvValues;
		uvProp.GetValues( uvValues );

		// retrieve all node indices per triangle on the geometry
		// nodes are grouped in triplets, first triplet being the nodes for
		// triangle 0, etc...
		CLongArray triNodeIds;
		ga.GetTriangleNodeIndices( triNodeIds );

		// loops over triangles and log their UV values
		Application app;
		LONG triID = 0;
		for ( LONG i=0; i<triNodeIds.GetCount( ); i+=3 )
		{
			app.LogMessage( L"Triangle " + CString( triID++ ) );

			// loop over each triangle node, use the node id to index into the
			// UV value array
			for ( LONG j=0; j<3; j++ )
			{
				LONG nodeId = triNodeIds[ i+j ];

				// shift by 3 since the datum size for a UV value is 3 (i.e. UVW)
				LONG uvIdx = nodeId*3;
				float u = uvValues[ uvIdx + 0 ];
				float v = uvValues[ uvIdx + 1 ];
				float w = uvValues[ uvIdx + 2 ];

				CString strUVW;
				strUVW += CString( u ) + L" " + CString( v ) + L" " + CString( w );

				app.LogMessage( L"   Node " + CString( nodeId ) + L": " + strUVW );
			}
		}

		PolygonMesh CreateMeshWithUVs( )
		{
			Application app;
			Model root = app.GetActiveSceneRoot( );

			X3DObject myGrid;
			root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

			PolygonMesh m = myGrid.GetActivePrimitive( ).GetGeometry( );

			CClusterPropertyBuilder cpb = m.GetClusterPropertyBuilder( );

			// add UV properties on the grid
			ClusterProperty cp = cpb.AddUV( );

			float uvws[] =
			{
				0.1f, 0.1f, 0.1f,
				0.2f, 0.2f, 0.2f,
				0.3f, 0.3f, 0.3f,
				0.4f, 0.4f, 0.4f,
				0.5f, 0.5f, 0.5f,
				0.6f, 0.6f, 0.6f,
				0.7f, 0.7f, 0.7f,
				0.8f, 0.8f, 0.8f
			};

			// set UV values for the node ids stored in nodeIds
			LONG nodeIds[ ] = {0,1,2,3,8,9,10,11};

			// to demonstrate chunking we set the values 4 at a time
			LONG nChunk = 4;

			// set the first 4 UVs
			cp.SetValues( nodeIds, uvws, nChunk );

			// set the last ones
			// shift nodeIds with a nChunk factor
			// shift uvws with a nChunk*3 factor, 3 being the datum size of a UV (i.e. UVW)
			cp.SetValues( &nodeIds[ nChunk ], &uvws[ nChunk*3 ], nChunk );

			return m;
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL CGeometryAccessor : public CBase
{
public:
	/*! Default constructor. */
	CGeometryAccessor();

	/*! Default destructor. */
	~CGeometryAccessor();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	CGeometryAccessor(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	CGeometryAccessor(const CGeometryAccessor& in_obj);

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
	\return The new CGeometryAccessor object.
	*/
	CGeometryAccessor& operator=(const CGeometryAccessor& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new CGeometryAccessor object.
	*/
	CGeometryAccessor& operator=(const CRef& in_ref);

	/*! Returns the local transformation of the underlying PolygonMesh object.
	\return The transformation.
	*/
	MATH::CTransformation GetTransform() const;

	/*! Returns the number of polygons of the geometry.
	\return Polygon count.
	*/
	LONG GetPolygonCount() const;

	/*! Returns an array containing the number of vertices for each polygon	in the geometry.
	\retval out_array Array containing the vertex count for each polygon.
	\return CStatus::OK
	*/
	CStatus GetPolygonVerticesCount(CLongArray& out_array) const;

	/*! Returns the polygon indices corresponding to each element in the array of materials returned by
	CGeometryAccessor::GetMaterials. The array also indicates which Polygon uses which Material.
	\retval out_matIndices Array of Material indices.
  	\return CStatus::OK Success.

	\sa CGeometryAccessor::GetMaterials, CGeometryAccessor::GetPolygonMaterialIndicesByMaterial,
		PolygonMesh::GetGeometryAccessor

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", mySphere );

		PolygonMesh m = mySphere.GetActivePrimitive().GetGeometry();

		CLongArray pIndices(3);
		pIndices[0] = 0;
		pIndices[1] = 5;
		pIndices[2] = 25;

		Cluster myCls;
		m.AddCluster(siPolygonCluster, L"",	pIndices, myCls );

		Material clsMat;
		myCls.AddMaterial( L"Phong", false, L"MyPhong", clsMat );

		// gets a geometry accessor object with default parameters
		CGeometryAccessor ga = m.GetGeometryAccessor();

		// get the material objects used by the mesh
		CRefArray materials = ga.GetMaterials();

		// get the material indices used by each polygon
		CLongArray pmIndices;
		ga.GetPolygonMaterialIndices(pmIndices);

		// log all polygon material indices
		LONG nPolyCount = pmIndices.GetCount();
		for (LONG i=0; i<nPolyCount; i++)
		{
			Material mat(materials[ pmIndices[i] ]);
			app.LogMessage( CString(i) + L":" + mat.GetFullName() );
		}
	\endcode
	*/
	CStatus GetPolygonMaterialIndices(CLongArray& out_matIndices) const;

	/*! Returns a bit array that matches the number of polygons on the geometry where each true bit indicates
	that the corresponding Polygon uses the specified Material.

	\param in_refMat Material to look for.
	\retval out_polyIndices Bit array matching the specified polygon indices.
  	\return CStatus::OK

  	\sa CGeometryAccessor::GetPolygonMaterialIndices, CGeometryAccessor::GetMaterials,
  		PolygonMesh::GetGeometryAccessor

	\eg
	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject mySphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", mySphere );

		PolygonMesh m = mySphere.GetActivePrimitive().GetGeometry();

		// add cluster with 3 polygons
		CLongArray pIndices(3);
		pIndices[0] = 25;
		pIndices[1] = 30;
		pIndices[2] = 50;

		Cluster myCls;
		m.AddCluster(siPolygonCluster, L"",	pIndices, myCls );

		Material clsMat;
		myCls.AddMaterial( L"Phong", false, L"MyPhong", clsMat );

		// gets a tessellator object with default parameters
		CGeometryAccessor ga = m.GetGeometryAccessor();

		// get the polygon indices using material clsMat
		CBitArray polyIndices;
		ga.GetPolygonMaterialIndicesByMaterial(clsMat, polyIndices);

		// log all polygon indices using this material
		app.LogMessage( L"Log all polygons using material: " + clsMat.GetName() );
		LONG nPolyCount = polyIndices.GetTrueCount();
		LONG nIdx = polyIndices.GetIterator();
		while (polyIndices.GetNextTrueBit(nIdx))
		{
			app.LogMessage( CString(nIdx) );
		}

		// gets a tessellator object to get the subdivided polygons
		CGeometryAccessor gaSubd = m.GetGeometryAccessor(
			siConstructionModeModeling,
			siCatmullClark,
			1);

		gaSubd.GetPolygonMaterialIndicesByMaterial(clsMat, polyIndices);

		app.LogMessage( L"Log all subdivided polygons (level 1) using material: " + clsMat.GetName() );
		nIdx = polyIndices.GetIterator();
		while (polyIndices.GetNextTrueBit(nIdx))
		{
			app.LogMessage( CString(nIdx) );
		}
	\endcode
	*/
	CStatus GetPolygonMaterialIndicesByMaterial(const CRef& in_refMat,
		CBitArray& out_polyIndices) const;

	/*! Returns the number of vertices in the geometry.
	\return Vertex count.
	*/
	LONG GetVertexCount() const;

	/*! Returns an array containing the vertex indices for each Polygon. The vertices are stored as a flat list
	and ordered by polygons, first group being the vertices of polygon 0, etc.

	The array is formatted as: \code {poly0<v0,v1..vN>, poly1<v0,v1..vN>... polyN<v0,v1..vN>} \endcode

	\retval out_array Array of vertex indices.
	\return CStatus::OK Success.
	*/
	CStatus GetVertexIndices(CLongArray& out_array) const;

	/*! Returns an array of double values containing the vertex positions (xyz) of the entire geometry. The positions
	are stored as a flat list and ordered by polygons, first group being the positions of polygon 0, etc.

	The array is formatted as: \code {x0,y0,z0, ... xN,yN,zN} \endcode

	\retval out_array Array of vertex positions.
	\return CStatus::OK Success.
	*/
	CStatus GetVertexPositions(CDoubleArray& out_array) const;

	/*! Returns an array of float values containing the crease value for each vertex of the geometry.
	\retval out_array Array of crease values.
	\return CStatus::OK Success.
	*/
	CStatus GetVertexCreaseValues(CFloatArray& out_array) const;

	/*! Returns the number of edges in the Geometry.
	\return Edge count.
	*/
	LONG GetEdgeCount() const;

	/*! Returns an array containing the edge indices for each polygon. The indices are stored as a flat list and ordered
	by polygons, first group being the edge indices of polygon 0, etc.

	The array is formatted as: \code {p0<e0,e1,..eN>, p1<e0,e1,..eN>... pN<e0,e1,..eN>} \endcode

	\retval out_array Array of edge indices.
	\return CStatus::OK Success.
	*/
	CStatus GetEdgeIndices(CLongArray& out_array) const;

	/*! Returns an array of float values containing the crease value for each edge of the geometry.
	\retval out_array Array of edge crease values.
	\return CStatus::OK Success.
	*/
	CStatus GetEdgeCreaseValues(CFloatArray& out_array) const;

	/*! Returns a bit array of values indicating whether each edge in the geometry is hard or not.
	\retval out_array Array of bit fields.
	\return CStatus::OK Success.
	*/
	CStatus GetHardEdges(CBitArray& out_array) const;

	/*! Returns the number of polygon nodes in the geometry.
	\return Node count.
	*/
	LONG GetNodeCount() const;

	/*! Returns an array containing the node indices for each polygon in the geometry. The nodes are stored as a flat list
	and ordered by polygons, first group being the nodes of polygon 0, etc.

	The array is formatted as: \code {poly0<n0,n1..nN>, poly1<n0,n1..nN>... polyN<n0,n1..nN>} \endcode

	\retval out_array Array of node indices.
	\return CStatus::OK Success.
	*/
	CStatus GetNodeIndices(CLongArray& out_array) const;

	/*! Returns an array of float values containing the node normals (xyz) for the entire geometry. The normals are stored
	as a flat list and ordered by nodes, first group being the normals of node 0, etc.

	The array is formatted as: \code {x0,y0,z0, ... xN,yN,zN} \endcode

	\note The array may contain user normal values if user normal properties exist on the geometry. In this case the
		corresponding geometric normal values are hidden by the user normal values. You may use CGeometryAccessor::GetUserNormals
		if you wish to access only the user normal values.

	\retval out_array Array of node normal values.
	\return CStatus::OK Success.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		PolygonMesh m = myCube.GetActivePrimitive().GetGeometry();
		CGeometryAccessor ga = m.GetGeometryAccessor();
		
		CFloatArray nodeArray;
		ga.GetNodeNormals(nodeArray);
		
		for (LONG i=0; i<nodeArray.GetCount(); i+=3)
		{
			float x = nodeArray[i+0];
			float y = nodeArray[i+1];
			float z = nodeArray[i+2];
				
			app.LogMessage( CString(x) + L" " + CString(y) + L" " + CString(z) );
		}		
	\endcode
	
	*/
	CStatus GetNodeNormals(CFloatArray& out_array) const;

	/*! Returns the number of triangles in the geometry.
	\return Triangle count.
	*/
	LONG GetTriangleCount() const;

	/*! Returns an array containing the vertex indices for each triangulated polygon. The vertex indices are stored in the array
	as a flat list of values grouped in triplets, the first triplet being the vertices of triangle 0, etc.

	The array is formatted as: \code {t0<v0,v1,v2>, t1<v0,v1,v2>... tN<v0,v1,v2>} \endcode

	\retval out_array Array of triangle vertex indices.
	\return CStatus::OK Success.
	*/
	CStatus GetTriangleVertexIndices(CLongArray& out_array) const;

	/*! Returns an array containing the node indices for each triangulated polygon. The node indices are stored in the array as a
	flat list of values and grouped in triplets, first triplet being the nodes for triangle 0, etc.

	The array is formatted as: \code {t0<n0,n1,n2>, t1<n0,n1,n2>... tN<n0,n1,n2>} \endcode

	\retval out_array Array of triangle node indices, formatted as:
	\return CStatus::OK Success.
	*/
	CStatus GetTriangleNodeIndices(CLongArray& out_array) const;

	/*! Returns the index of the polygon that was used to generate the triangle specified with \c in_tindex. This function is best
	used when you want to find the polygon index of a specific triangle. However, you should consider using
	CGeometryAccessor::GetPolygonTriangleIndexArray instead if you need to find all polygon triangle indices in the entire geometry.

	\param in_tindex A triangle index.
	\retval out_pindex The polygon index used for generating the input triangle index.
	\return CStatus::OK Valid polygon index.
	\return CStatus::False Invalid triangle index used.

	\sa CGeometryAccessor::GetPolygonTriangleIndexArray, CGeometryAccessor::GetTriangleVertexIndexArray,
		CGeometryAccessor::GetTriangleNodeIndexArray
	*/
	CStatus GetPolygonTriangleIndex( LONG in_tindex, LONG& out_pindex ) const;

	/*! Returns the indices for the polygons used to generate all triangles of the geometry. The array size is equal to
	CGeometryAccessor::GetTriangleCount and contains all the polygon indices for the entire geometry. The array index matches the
	triangle index and the array value	corresponds to the polygon used for generating that triangle. For example, for a 2x2 grid,
	the array would be \code [0,0,1,1,2,2,3,3] \endcode

	The information returned is similar to what CGeometryAccessor::GetPolygonTriangleIndex returns, but	it contains information
	relating to all triangles of the geometry.

	\retval out_array Array of triangle indices.
	\return CStatus::OK Success.

	\sa CGeometryAccessor::GetPolygonTriangleIndex, CGeometryAccessor::GetTriangleVertexIndexArray,
		CGeometryAccessor::GetTriangleNodeIndexArray
	*/
	CStatus GetPolygonTriangleIndices( CLongArray& out_array) const;

	/*! Returns an array of ClusterProperty objects connected to the geometry.
	\param in_type Type of cluster properties to query.
  	\return CRefArray Array of references to cluster properties
	\sa CGeometryAccessor::GetMaterials, CGeometryAccessor::GetUserDataMaps, ClusterProperty
	*/
	CRefArray GetClusterProperties( siClusterPropertyType in_type ) const;

	/*! Finds the ClusterProperty object connected to the geometry matching the specified name.
	\param in_type Type of cluster properties to query.
	\param in_nName The property name.
  	\return CRef A reference to the cluster property object.
	\sa CGeometryAccessor::GetClusterProperties, ClusterProperty
	*/
	CRef GetClusterProperty( siClusterPropertyType in_type,	const CString& in_nName ) const;

	/*! Returns an array of UV property objects connected to the clusters of this geometry. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperties.
  	\return CRefArray Array of references to UV properties.
	\sa CGeometryAccessor::GetClusterProperties, ClusterProperty
	*/
	CRefArray GetUVs() const;

	/*! Finds the UV property object connected to the geometry which matches the specified name.  This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperty.
	\param in_nName The property name.
 	\return CRef A reference to the cluster property object.
	\sa CGeometryAccessor::GetClusterProperty, ClusterProperty
	*/
	CRef GetUV( const CString& in_nName ) const;

	/*! Returns an array of color vertex property objects connected to the clusters of this geometry. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperties.
  	\return CRefArray Array of references to color vertex properties.
	\sa CGeometryAccessor::GetClusterProperties, ClusterProperty
	*/
	CRefArray GetVertexColors() const;

	/*! Finds the vertex color property object connected to the geometry which matches the specified name. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperty.
	\param in_nName The property name.
  	\return CRef A reference to the cluster property object.
	\sa CGeometryAccessor::GetClusterProperty, ClusterProperty
	*/
	CRef GetVertexColor( const CString& in_nName ) const;

	/*! Returns an array of user normal property objects connected to the clusters of this geometry. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperties.
	\return CRefArray Array of references to user normal properties.
	\sa CGeometryAccessor::GetClusterProperties, CGeometryAccessor::GetNodeNormals, ClusterProperty
	*/
	CRefArray GetUserNormals() const;

	/*! Finds the user normal property object connected to the geometry which matches the specified name. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperty.
	\param in_nName The property name.
  	\return CRef A reference to the cluster property object.
	\sa CGeometryAccessor::GetClusterProperty, ClusterProperty
	*/
	CRef GetUserNormal( const CString& in_nName ) const;

	/*! Returns an array of EnvelopeWeight property objects connected to the clusters of this geometry. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperties.
  	\return CRefArray Array of references to EnvelopeWeight properties.
	\sa CGeometryAccessor::GetClusterProperties, EnvelopeWeight
	*/
	CRefArray GetEnvelopeWeights() const;

	/*! Finds the EnvelopeWeight property object connected to the geometry which matches the specified name. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperty.
	\param in_nName The property name.
	\return CRef A reference to the cluster property object.
	\sa CGeometryAccessor::GetClusterProperty, EnvelopeWeight
	*/
	CRef GetEnvelopeWeight( const CString& in_nName ) const;

	/*! Returns an array of shape key property objects connected to the clusters of this geometry. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperties.
  	\return CRefArray Array of references to shape key properties.
	\sa CGeometryAccessor::GetClusterProperties, ClusterProperty
	*/
	CRefArray GetShapeKeys() const;

	/*! Finds the shape key property object connected to the geometry which matches the specified name. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperty.
	\param in_nName The property name.
  	\return CRef A reference to the cluster property object.
	\sa CGeometryAccessor::GetClusterProperty, ClusterProperty
	*/
	CRef GetShapeKey( const CString& in_nName ) const;

	/*! Returns an array of weight map property objects connected to the clusters of this geometry. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperties.
  	\return CRefArray Array of references to weight map properties.
	\sa CGeometryAccessor::GetClusterProperties, ClusterProperty
	*/
	CRefArray GetWeightMaps() const;

	/*! Finds the weight map property object connected to the geometry which matches the specified name. This function is a more convenient
	alternative to using CGeometryAccessor::GetClusterProperty.
	\param in_nName The property name.
  	\return CRef A reference to the cluster property object.
	\sa CGeometryAccessor::GetClusterProperty, ClusterProperty
	*/
	CRef GetWeightMap( const CString& in_nName ) const;

	/*! Returns an array of UserDataMap objects connected to the geometry which matches the specified cluster type.
	\param in_type Type of cluster to query (one of the ::siClusterType values).
  	\return CRefArray Array of references to user data map properties.
	\sa UserDataMap
	*/
	CRefArray GetUserDataMaps( siClusterType in_type ) const;

	/*! Returns an array of all Material objects assigned to the geometry.
  	\return CRefArray Array of references to material properties.
	\sa Material.GetConnectedClusterProperties

	\eg
	\code
		Application app;
		Model root = app.GetActiveSceneRoot();

		// create a cube
		X3DObject myCube;
		root.AddGeometry( L"Cube",L"MeshSurface",L"" ,myCube );

		PolygonMesh mesh = myCube.GetActivePrimitive().GetGeometry();

		Material myMaterial;
		myCube.AddMaterial(L"Phong",false,L"",myMaterial);

		// add a polygon cluster to the cube
		Cluster myCls;
		CLongArray vIndices(4);
		vIndices[0] = 0;
		vIndices[1] = 1;
		vIndices[2] = 2;
		vIndices[3] = 3;

		mesh.AddCluster(siPolygonCluster, L"", vIndices, myCls );

		Material myClsMat;
		myCls.AddMaterial(L"Lambert",false,L"",myClsMat);

		// list all materials on the geometry
		CGeometryAccessor ga = mesh.GetGeometryAccessor();
		CRefArray mats = ga.GetMaterials();

		LONG nCount = mats.GetCount();
		for ( LONG i=0; i<nCount; i++ )
		{
			Material mat(mats[i]);
			app.LogMessage( L"Material " + CString(i) + L": " + mat.GetName() );
		}
	\endcode
	*/
	CRefArray GetMaterials() const;

	/*! Returns an array of clusters on the geometry matching the specified type.
	\param in_clsType Type of cluster requested (one of the ::siClusterType values).
  	\return CRefArray Array of references to clusters.
	\sa Cluster
	*/
	CRefArray GetClusters( siClusterType in_clsType ) const;

	/*!	Returns an array of all clusters defined in the geometry.
  	\return CRefArray Array of references to clusters.
	\sa Cluster
	*/
	CRefArray GetClusters() const;

	private:
	CGeometryAccessor * operator&() const;
	CGeometryAccessor * operator&();
};

};
#endif // __XSIGEOMACCESSOR_H__
