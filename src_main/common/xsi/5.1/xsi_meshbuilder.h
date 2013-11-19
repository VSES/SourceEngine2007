//*****************************************************************************
/*!
   \file xsi_meshbuilder.h
   \brief CMeshBuilder and CMeshBuilder::CErrorDescriptor class declarations.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIMESHBUILDER_H__
#define __XSIMESHBUILDER_H__

#include <xsi_base.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_longarray.h>

#pragma warning(disable:4251)

namespace XSI {

//*****************************************************************************
/*! \class CMeshBuilder xsi_meshbuilder.h
	\brief CMeshBuilder allows you to create polygon meshes from an ordered array of
	vertex coordinates and polygon connections (polygon to vertices). You can
	build the vertices and polygons by providing all the data at once or you can
	build them incrementally by providing the data in chunks.

	Building a mesh incrementally is especially convenient when you have to deal with
	very large data sets, allowing you to minimize the memory required for storing such
	data sets.

	Although you can undo the storing CMeshBuilder operation (ie., the whole creation of
	the new mesh), the individual operations for setting the vertex and polygon values
	are not undoable. For performance reasons it is not recommended to use undo for
	building a mesh as this will duplicate the data in memory.

	\warning If you choose to build the mesh with undo disabled, the current undo stack
	is flushed before proceeding with the operation. This is important in order to
	protect data integrity.

	CMeshBuilder also provides the functionality for setting the material used by each
	polygon of the mesh with only one function call (CMeshBuilder::SetPolygonsMaterial).
	The SetPolygonsMaterial function manages creating clusters, which can also be undone.
	However, while you can undo the cluster creation, you cannot undo the operations for
	setting the cluster element values.

	CMeshBuilder can only be used from existing X3DObject instances. Therefore, the newly
	created mesh will replace the current one on the X3DObject instance.

	\since 5.0

	\eg Creates a 2x2 grid mesh with the CMeshBuilder class.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		// Create the grid from an empty mesh object
		X3DObject myObj;
		CMeshBuilder msBuilder;
		root.AddPolygonMesh( L"MyMesh", myObj, msBuilder );

		// Add the vertices of the grid
		double myVertexPositionArray[27] =
		{
			-4, 0, -4,
			-4, 0,  0,
			-4, 0,  4,
			 0, 0, -4,
			 0, 0,  0,
			 0, 0,  4,
			 4, 0, -4,
			 4, 0,  0,
			 4, 0,  4
		};

		// Add the vertices with positions
		msBuilder.AddVertices( 9, myVertexPositionArray );

		// Add the polygons of the grid
		LONG pPolyVertexCounts[4] = {4,4,4,4};
		LONG pVtxIndices[16] =
		{
			0, 1, 4, 3,
			1, 2, 5, 4,
			3, 4, 7, 6,
			4, 5, 8, 7
		};

		msBuilder.AddPolygons( 4, pPolyVertexCounts, pVtxIndices );

		// Generate the new grid with undo disabled
		CMeshBuilder::CErrorDescriptor err = msBuilder.Build(false);
		if (err.GetCode()==CStatus::Fail)
		{
			app.LogMessage( L"Error generating the mesh: " + err.GetDescription() );
		}

	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL CMeshBuilder : public CBase
{
public:

	//*************************************************************************
	/*! \class CErrorDescriptor xsi_meshbuilder.h
		\brief This class reports the error status returned by CMeshBuilder::Build. If
		the mesh builder fails, the faulty indices are reported by this object.

		\sa CMeshBuilder::Build
		\since
	*/
	//*************************************************************************

	class SICPPSDKDECL CErrorDescriptor : public CStatus
	{
		public:

		/*! Constructs a CMeshBuilder::CErrorDescriptor
		*/
		CErrorDescriptor(CStatus::Code in_code);

		/*! Default destructor
		*/
		~CErrorDescriptor();

		/*! The faulty polygon indices on the last mesh built with CMeshBuild.
		The array is empty if the mesh was built successfully.
		*/
		CLongArray m_polygonIndices;

		/*! The faulty vertex indices on the last mesh built with CMeshBuild.
		The array is empty if the mesh was built successfully.
		*/
		CLongArray m_vertexIndices;

		/*! The faulty edge indices on the last mesh built with CMeshBuild.
		The array is empty if the mesh was built successfully.
		*/
		CLongArray m_edgeIndices;

		/*! The faulty node indices on the last mesh built with CMeshBuild.
		The array is empty if the mesh was built successfully.
		*/
		CLongArray m_nodeIndices;
	};

	/*! Default constructor. */
	CMeshBuilder();

	/*! Default destructor. */
	~CMeshBuilder();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	CMeshBuilder(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	CMeshBuilder(const CMeshBuilder& in_obj);

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
	\return The new CMeshBuilder object.
	*/
	CMeshBuilder& operator=(const CMeshBuilder& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new CMeshBuilder object.
	*/
	CMeshBuilder& operator=(const CRef& in_ref);

	/*! Adds the specified number of vertices to the underlying mesh. You can either specify the
	array of positions for these vertices as input to this function or use the
	CMeshBuilder::SetVertexPositions function to specify them later.

	\param in_nVertices Number of vertices to add. One vertex is a 3 components value (xyz).
	\param in_pPos Optional array of size in_nVertices*3 containing the vertex positions (xyz).
		If this array is omitted, all vertex positions are set to 0.

	\return CStatus
	*/
	CStatus AddVertices( LONG in_nVertices, const double* in_pPos=0 );

	/*! Sets the positions of the mesh vertices by identifying which vertices to set and what the
	new values will be. This method allows you to incrementally set vertex values to minimize memory
	usage, but you can also specify the positions all at once when you add the vertices to the mesh.

	\param in_nVertices Number of vertices to set. One vertex is a 3 components value (xyz).
	\param in_pPos Array of size in_nVertices*3 containing the vertex positions (xyz)
	\param in_nOffset Specifies the starting vertex index in the mesh.

	\return CStatus

	\eg Creates a cube with the CMeshBuilder class and demonstrates how to set the vertex
		positions after adding the polygons to the mesh.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot();

		// Creates the cube from an empty mesh
		X3DObject myObj;
		CMeshBuilder msBuilder;
		root.AddPolygonMesh( L"MyCube", myObj, msBuilder );

		// Add empty vertices to the cube without setting the positions
		msBuilder.AddVertices( 8, NULL );

		// Add polygons to the cube
		LONG pPolyVertexCounts[6] = {4,4,4,4,4,4};
		LONG pVtxIndices[24] =
		{
			0, 2, 3, 1,
			0, 1, 5, 4,
			0, 4, 6, 2,
			1, 3, 7, 5,
			2, 6, 7, 3,
			4, 5, 7, 6
		};

		msBuilder.AddPolygons( 6, pPolyVertexCounts, pVtxIndices );

		// Now set the positions one at a time
		double myCubeVertices[24] =
		{
			-4, -4, -4,
			 4, -4, -4,
			-4,  4, -4,
			 4,  4, -4,
			-4, -4,  4,
			 4, -4,  4,
			-4,  4,  4,
			 4,  4,  4
		};

		for (LONG i=0, nOffset=0; i<8; i++, nOffset++ )
		{
			msBuilder.SetVertexPositions(
				1,							// nb vertices
				myCubeVertices+(nOffset*3), // positions
				nOffset						// vertex index to set
			);
		}

		// Generate the new cube
		CMeshBuilder::CErrorDescriptor err = msBuilder.Build(false);
		if (err==CStatus::Fail)
		{
			app.LogMessage( L"Error generating the mesh: " + err.GetDescription() );
		}

	\endcode
	*/
	CStatus SetVertexPositions( LONG in_nVertices, const double* in_pPos, LONG in_nOffset=0 );

	/*! Adds polygons to a mesh by describing how many polygons to add and how many vertices (and
	their indices) each new polygon will have.

	\param in_nPolygons Number of polygons to add.
	\param in_pPolyVertexCounts Array of size in_nPolygons containing the vertex count of each polygon to add.
	\param in_pVtxIndices Array containing the vertex indices for each polygon to add. This array describes how
		each polygon is connected to the mesh. The Array size should be the sum of all items of in_pPolyVertexCounts.

	\return CStatus::OK Success
	\return CStatus::Unexpected Vertex positions are not set on the mesh.
	All vertex positions must be added with CMeshBuilder::AddVertices or
	CMeshBuilder::SetVertexPositions before defining the polygons.
	*/
	CStatus AddPolygons(
		LONG in_nPolygons,
		const LONG* in_pPolyVertexCounts,
		const LONG* in_pVtxIndices );

	/*! Adds triangle polygons to a mesh. The function assumes the polygons to add are triangles. Therefore the caller
	doesn't need to allocate an array for storing the number of vertices per polygon as for CMeshBuilder::AddPolygons.

	\param in_nTriangles Number of triangles to add.
	\param in_pVtxIndices Array containing the vertex indices for each polygon to add. This array describes how
		each triangle is connected to the mesh. The Array size should be in_nTriangles * 3.
	\return CStatus::OK
	\since 5.0
	*/
	CStatus AddTriangles( LONG in_nTriangles, const LONG* in_pVtxIndices );

	/*! Sets the edge crease value on the specified polygon.
	\param in_nPolyID polygon index to set.
	\param in_nPolyEdgeIndex Edge to set (ids are ccw and local to the polygon)
	\param in_value crease value.

	\return CStatus
	*/
	CStatus SetEdgeCreaseValue( LONG in_nPolyID, LONG in_nPolyEdgeIndex, float in_value );

	/*! Sets the edge on the specified polygon as hard or smooth.
	\param in_nPolyID polygon index to set.
	\param in_nPolyEdgeIndex Edge to set (ids are ccw and local to the polygon)
	\param in_bFlag set edge to hard if true (default) or smooth it if false.

	\return CStatus
	*/
	CStatus SetHardEdge( LONG in_nPolyID, LONG in_nPolyEdgeIndex, bool in_bFlag=true );

	/*! Sets the vertex crease value for the specified vertex.
	\param in_nVtxID Index of the vertex to set.
	\param in_value The crease value.
	\return CStatus
	*/
	CStatus SetVertexCreaseValue( LONG in_nVtxID, float in_value );

	/*! Assigns the specified material to the set of polygons matching the specified indices. This function
	selects existing clusters or creates new ones based on the input material by using the following heuristics:

		\li If a cluster exists with the specified material (in_refMat), that cluster is selected.

		\li If there is no cluster set with the specified material (in_refMat), a new cluster is
			created and is set with the specified material.

		\li If no clusters exist on the mesh, a new cluster is created and assigned the specified material (in_refMat).

	\note This function can be called only after the mesh is built.

	\param in_refMat Material to assign to the polygons
	\param in_nSize Size of the in_pPolyIndices array
	\param in_pPolyIndices Array of polygon indices

	\return CStatus::OK Success
	\return CStatus::False The function cannot assign more than one material per polygon. Therefore the operation
		ends before completion when a duplicated polygon is detected.
	\return CStatus::Fail Operation didn't succeed.

	\sa CMeshBuilder::Build

	\eg Demonstrates how to set the material for each face of a cube.

	\code
		using namespace XSI;

		Application app;
		Model root = app.GetActiveSceneRoot( );

		// Get a mesh builder from an empty mesh to create the new cube
		X3DObject myCube;
		CMeshBuilder msBuilder;
		root.AddPolygonMesh( L"MyCube", myCube, msBuilder );

		// Add vertices
		double myCubeVertices[ 24 ] =
		{
			-4, -4, -4,
			4, -4, -4,
			-4,  4, -4,
			4,  4, -4,
			-4, -4,  4,
			4, -4,  4,
			-4,  4,  4,
			4,  4,  4
		};

		msBuilder.AddVertices( 8, myCubeVertices );

		// Appends the polygons
		LONG pPolyVertexCounts[ 6 ] = { 4,4,4,4,4,4 };
		LONG pVtxIndices[ 24 ] =
		{
			0, 2, 3, 1,
			0, 1, 5, 4,
			0, 4, 6, 2,
			1, 3, 7, 5,
			2, 6, 7, 3,
			4, 5, 7, 6
		};

		msBuilder.AddPolygons( 6, pPolyVertexCounts, pVtxIndices );

		// Generate the new cube with undo disabled
		CMeshBuilder::CErrorDescriptor err = msBuilder.Build( false );
		if (err==CStatus::Fail)
		{
			app.LogMessage( L"Error generating the mesh: " + err.GetDescription() );
		}

		// Create 3 different materials
		Scene scene = app.GetActiveProject( ).GetActiveScene( );
		MaterialLibrary matlib( scene.GetActiveMaterialLibrary( ) );

		Material myMat1 = matlib.CreateMaterial( L"Phong", L"myMat1" );
		Shader sh = myMat1.GetShaders( ).GetItem( 0 );
		Parameter p = sh.GetParameters( ).GetItem( L"diffuse" );
		p.PutParameterValue( L"red", 1L );
		p.PutParameterValue( L"green", 0L );
		p.PutParameterValue( L"blue", 0L );

		Material myMat2 = matlib.CreateMaterial( L"Phong", L"myMat2" );
		sh = myMat2.GetShaders( ).GetItem( 0 );
		p = sh.GetParameters( ).GetItem( L"diffuse" );
		p.PutParameterValue( L"red", 0L );
		p.PutParameterValue( L"green", 1L );
		p.PutParameterValue( L"blue", 0L );

		Material myMat3 = matlib.CreateMaterial( L"Phong", L"myMat3" );
		sh = myMat3.GetShaders( ).GetItem( 0 );
		p = sh.GetParameters( ).GetItem( L"diffuse" );
		p.PutParameterValue( L"red", 0L );
		p.PutParameterValue( L"green", 0L );
		p.PutParameterValue( L"blue", 1L );

		// Set polygon 4, 5 with the red material
		LONG poly45[] = {4,5};
		msBuilder.SetPolygonsMaterial( myMat1, 2, poly45 );

		// Set polygon 2, 3 with the green material
		LONG poly23[] = {2,3};
		msBuilder.SetPolygonsMaterial( myMat2, 2, poly23 );

		// Set polygon 0, 1 with the blue material
		LONG poly01[] = {0,1};
		msBuilder.SetPolygonsMaterial( myMat3, 2, poly01 );

	\endcode
	*/
	CStatus SetPolygonsMaterial(
		const CRef&	in_refMat,
		LONG		in_nSize,
		const LONG* in_pPolyIndices );

	/*! Assigns a specific material to a set of polygons via a polygon cluster. This function can only be
	called once the mesh is built.

	The following heuristics are used by the function for selecting the polygon cluster to use for setting
	the polygon's material. The selected cluster is returned with io_clsName:

		\li If the named cluster (io_clsName) exists and is set with the specified material (in_refMat),
			that cluster is selected.

		\li If the named cluster (io_clsName) exists but its material doesn't match the specified material
			(in_refMat) or has no material, a new cluster is created and is assigned the specified material.

		\li If no clusters exist on the mesh, a new cluster is created and assigned the specified material
			(in_refMat).

		\li If io_clsName is empty, the first cluster found on the mesh and set with the specified material
			(in_refMat) is selected.

		\li If io_clsName is empty and no cluster is found with the specified material (in_refMat), a new
			cluster is created and assigned the specified material.

	\param in_refMat Material to assign to the polygons
	\param in_nSize Size of the in_pPolyIndices array
	\param in_pPolyIndices Array of polygon indices
	\param io_clsName Name of the cluster to use for setting the polygons. The function returns the name of the selected cluster with io_clsName.

	\return CStatus::OK Success
	\return CStatus::False The function cannot assign more than one material per polygon. Therefore the
		operation ends before completion when a duplicated polygon is detected.
	\return CStatus::Fail Operation didn't succeed.

	\sa CMeshBuilder::Build
	*/
	CStatus SetPolygonsMaterial(
		const CRef& in_refMat,
		LONG in_nSize,
		const LONG* in_pPolyIndices,
		CString& io_clsName );

	/*! Validates the new mesh created from the vertices and polygon data and swaps the underlying X3DObject's mesh with the new one.
	The existing mesh, clusters and properties on the X3DObject are destroyed. An error descriptor is returned to inform the caller
	whether or not the mesh was created correctly.

	If the mesh creation fails, the error object is set with the faulty indices, one array per geometry type (polygon, vertex, edge
	and node). The operation of building a mesh can be undone but that comes with a cost since mesh geometries can get quite large.

	\note It is recommended to disable the undo when building a mesh.

	\param in_bUndoRequired If true, CMeshBuilder saves the data for both new and original meshes for undo. If the current undo stack
		level is 0, the operation is not undoable. If you pass false, the current undo stack is set to 0. Therefore all undo events
		currently on the stack are lost. The undo stack is set back to the previous level after the Build function returns.

	\return CErrorDescriptor The returned object indicates whether the mesh was built successfully or not. It also contains
		information on the faulty components.
	*/
	CErrorDescriptor Build(bool in_bUndoRequired);

	private:
	CMeshBuilder * operator&() const;
	CMeshBuilder * operator&();
};

};

#endif // __XSIMESHBUILDER_H__
