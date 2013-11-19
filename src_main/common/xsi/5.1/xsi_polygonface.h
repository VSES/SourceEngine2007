//*****************************************************************************
/*!
   \file xsi_polygonface.h
   \brief PolygonFace and CPolygonFaceRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPOLYGONFACE_H__
#define __XSIPOLYGONFACE_H__

#include <xsi_facet.h>
#include <xsi_longarray.h>
#include <xsi_pointlocatordata.h>

namespace XSI {

class CPolygonNodeRefArray;
class CVertexRefArray;
class CEdgeRefArray;
class CPolygonFaceRefArray;

//*****************************************************************************
/*! \class PolygonFace xsi_polygonface.h
	\brief The PolygonFace object gives access to the geometry polygons of an X3DObject's primitive.
	\sa PolygonMesh.GetPolygons(), CPolygonFaceArray

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		PolygonMesh mesh( myCube.GetActivePrimitive().GetGeometry() );
		PolygonFace face( mesh.GetPolygons().GetItem(0) );

		app.LogMessage(L"Number of nodes: " + CValue(face.GetNodes().GetCount()).GetAsText() );
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL PolygonFace : public Facet
{
public:
	/*! Default constructor. */
	PolygonFace();

	/*! Default destructor. */
	~PolygonFace();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PolygonFace(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PolygonFace(const PolygonFace& in_obj);

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
	\return The new PolygonFace object.
	*/
	PolygonFace& operator=(const PolygonFace& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PolygonFace object.
	*/
	PolygonFace& operator=(const CRef& in_ref);

	/*! Returns an array of PolygonNode objects on this PolygonFace object.
	\return Array of PolygonNode objects.
	*/
	CPolygonNodeRefArray GetNodes() const;

	/*! Returns an array of Vertex objects on this PolygonFace object.
	\return Array of Vertex objects.
	*/
	CVertexRefArray GetVertices() const;

	/*! Returns an array of Edge objects on this PolygonFace object.
	\return Array of Edge objects.
	*/
	CEdgeRefArray GetEdges() const;

	/*! Returns an array of Vertex neighbors within a given distance.If the
	distance is 1 all Vertex objects comprising the current PolygonFace are
	returned in counter-clockwise order.

	\param in_lDistance Value representing the degree of neighborhood
		(eg: degree=2 for a polygon means its adjacent vertices plus the adjacent
		vertices of the adjacent polygons).

	\return Array of Vertex objects
	*/
	CVertexRefArray GetNeighborVertices(LONG in_lDistance = 1) const;

	/*! Returns an array of Edge neighbors within a given distance. If the
	distance is 1 all Edge objects comprising the current PolygonFace are
	returned in counter-clockwise order.

	\param in_lDistance Value representing the degree of neighborhood
		(eg: degree=2 for a polygon means its adjacent vertices plus the adjacent
		vertices of the adjacent polygons).

	\return Array of Edge objects.
	*/
	CEdgeRefArray GetNeighborEdges(LONG in_lDistance = 1) const;

	/*! Returns an array of PolygonFace neighbors within a given distance. If
	the	distance is 1 all PolygonFace objects that share an Edge with the
	current PolygonFace are returned in counter-clockwise order.

	\param in_lDistance Value representing the degree of neighborhood
		(eg: degree=2 for a polygon means its adjacent vertices plus the adjacent
		vertices of the adjacent polygons).

	\return Array of PolygonFace objects.
	*/
	CPolygonFaceRefArray GetNeighborPolygons(LONG in_lDistance = 1) const;

	/*! Returns an array of PolygonFace objects that are adjacent to this
	PolygonFace object, within a given distance.

	\param in_lDistance Value representing the degree of neighborhood
		(eg: degree=2 for a polygon means its adjacent vertices plus the adjacent
		vertices of the adjacent polygons).

	\return Array of PolygonFace objects.
	*/
	CPolygonFaceRefArray GrowNeighborPolygons(LONG in_lDistance = 1) const;

	/*! Returns a packed array of triangulation descriptions corresponding
	to the tesselation of the polygon. The polygon is tesselated into
	(Facet::NbPoints - 2) triangles, and 3 indices in the range (0..Facet::NbPoints-1)
	are returned for each triangle. Each index triplet describes a subtriangle
	of the polygon, and the indices correspond to the vertex or node ordering within
	the polygon.

	Notice that the triangulation of a polygon may change accordingly to the
	deformation of the geometry.

	\return Array of packed triangle subindices triplets.

	\eg This example demonstrates how to describe the triangulation of some polygons.

	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject meshSphereObj;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", meshSphereObj );
		PolygonMesh meshSphereGeom( meshSphereObj.GetActivePrimitive().GetGeometry() );

		CPolygonFaceRefArray polygons = meshSphereGeom.GetPolygons();

		LONG i;
		for(i = 0; i < polygons.GetCount(); i++)
		{
			PolygonFace polygon = polygons[i];
			LONG nbTriangles = polygon.GetNbPoints()-2;

			CLongArray polygonNeiVtxIndices = polygon.GetVertices().GetIndexArray();
			CLongArray polygonNeiNodIndices = polygon.GetNodes().GetIndexArray();
			CLongArray polygonTriSubIndices = polygon.GetTriangleSubIndexArray();

			LONG j;
			for(j = 0; j < nbTriangles; j++)
			{
				app.LogMessage(L"SubTriangle " + CString(CValue(j)) + L" of Polygon " + CString(CValue(i)) + L" correspond to vertices (" +
					CString(CValue(polygonNeiVtxIndices[polygonTriSubIndices[j*3]])) + L", " +
					CString(CValue(polygonNeiVtxIndices[polygonTriSubIndices[j*3+1]])) + L", " +
					CString(CValue(polygonNeiVtxIndices[polygonTriSubIndices[j*3+2]])) + L") and nodes (" +
					CString(CValue(polygonNeiNodIndices[polygonTriSubIndices[j*3]])) + L", " +
					CString(CValue(polygonNeiNodIndices[polygonTriSubIndices[j*3+1]])) + L", " +
					CString(CValue(polygonNeiNodIndices[polygonTriSubIndices[j*3+2]])) + L").");
			}
		}
		// Expected results:
		//INFO : SubTriangle 0 of Polygon 0 correspond to vertices (9, 2, 0) and nodes (0, 1, 2).
		//INFO : SubTriangle 0 of Polygon 1 correspond to vertices (2, 9, 10) and nodes (3, 4, 5).
		//INFO : SubTriangle 1 of Polygon 1 correspond to vertices (2, 10, 3) and nodes (3, 5, 6).
		//INFO : SubTriangle 0 of Polygon 2 correspond to vertices (3, 10, 11) and nodes (7, 8, 9).
		//INFO : SubTriangle 1 of Polygon 2 correspond to vertices (3, 11, 4) and nodes (7, 9, 10).
		//etc.
	\endcode
	*/
	CLongArray GetTriangleSubIndexArray() const;

	private:
	PolygonFace * operator&() const;
	PolygonFace * operator&();
};

//*****************************************************************************
/*! \class CPolygonFaceRefArray xsi_polygonface.h
	\brief An array of PolygonFace objects representing the polygons in a polygon mesh.

	\warning This specialized array is returned by PolygonMesh::GetPolygons, it is not meant to be
		created and modified in user-defined functions. If you want to add and remove
		arbitrary items to a collection, you must use a CRefArray instead.

	\sa PolygonMesh::GetPolygons

	\eg

	\code
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		PolygonMesh mesh( myCube.GetActivePrimitive().GetGeometry() );
		CPolygonFaceRefArray polygons( mesh.GetPolygons() );

		CVector3Array normArray = polygons.GetNormalArray();

		app.LogMessage( polygons.GetClassIDName() );
		app.LogMessage( CValue((LONG)normArray.GetCount()).GetAsText() );

		for (LONG i=0; i<normArray.GetCount(); ++i)
		{
			app.LogMessage(L"Normal " + CValue((LONG)i).GetAsText() );
			app.LogMessage(
				CValue(normArray[i].GetX()).GetAsText() + L"," +
				CValue(normArray[i].GetY()).GetAsText() + L"," +
				CValue(normArray[i].GetZ()).GetAsText() );
		}
	\endcode
*/
//*****************************************************************************
class SICPPSDKDECL CPolygonFaceRefArray : public CFacetRefArray
{
	public:
	CPolygonFaceRefArray();

	/*! Copy constructor.
	\param in_array constant CPolygonFaceRefArray reference object.
	*/
	CPolygonFaceRefArray( const CPolygonFaceRefArray& in_array );

	/*! Returns a PolygonFace object at a specified index in the array.
	\param in_index The index of the PolygonFace.
	\return A reference to a PolygonFace object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the PolygonFace object matching the specified name.
	\param in_name The name of the PolygonFace.
	\return A reference to a PolygonFace object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of PolygonFace objects in the array.
	\return The number of PolygonFace objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Returns an array of normals expressed with CVector3 objects.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetNormalArray() const;

	/*! Returns an array of Vertex neighbors within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
		(eg: degree=2 for a polygon means its adjacent vertices plus the adjacent
		vertices of the adjacent polygons).
	\return Array of Vertex objects
	*/
	CVertexRefArray GetNeighborVertices(LONG in_lDistance = 1) const;

	/*! Returns an array of Edge neighbors within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
		(eg: degree=2 for a polygon means its adjacent vertices plus the adjacent
		vertices of the adjacent polygons).
	\return Array of Edge objects
	*/
	CEdgeRefArray GetNeighborEdges(LONG in_lDistance = 1) const;

	/*! Returns an array of PolygonFace neighbors within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
		(eg: degree=2 for a polygon means its adjacent vertices plus the adjacent
		vertices of the adjacent polygons).
	\return Array of PolygonFace objects
	*/
	CPolygonFaceRefArray GetNeighborPolygons(LONG in_lDistance = 1) const;

	/*! Returns an array of PolygonFace objects that are adjacent to each
	PolygonFace objects of this array, within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
		(eg: degree=2 for a polygon means its adjacent vertices plus the adjacent
		vertices of the adjacent polygons).
	\return Array of PolygonFace objects
	*/
	CPolygonFaceRefArray GrowNeighborPolygons(LONG in_lDistance = 1) const;

	/*! Returns an array of normals for all PolygonNode objects in the array. The
	normals are expressed with CVector3 objects.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetPolygonNodeNormalArray() const;

	/*! Returns an array containing the number of PolygonNode objects and the
	corresponding PolygonNode indices.for each PolygonFace in the array. The
	information is stored in the array as follows:
	\code [Number of PNode for PFace(0), PNode indices, ... Number of PNode for PFace(N), PNode indices] \endcode
	\return Array of values.
	\since 5.0
	*/
	CLongArray GetPolygonNodePolygonFaceIndexArray() const;
};

};

#endif // __XSIPOLYGONFACE_H__
