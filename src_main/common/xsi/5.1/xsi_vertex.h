//*****************************************************************************
/*!
   \file xsi_vertex.h
   \brief The Vertex and CVertexRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIVERTEX_H__
#define __XSIVERTEX_H__

#include <xsi_point.h>

namespace XSI {

class CPolygonNodeRefArray;
class CVertexRefArray;
class CEdgeRefArray;
class CPolygonFaceRefArray;
class CDoubleArray;

//*****************************************************************************
/*! \class Vertex xsi_vertex.h
	\brief The Vertex object gives access to the geometry vertices of an X3DObject's Primitive. A Vertex is the object
	PolygonMesh specifics of the Point object.

	You can access vertices from Edge::GetVertices, PolygonFace::GetVertices and PolygonMesh::GetVertices.

	\sa CVertexRefArray, Point, Edge, PolygonMesh, Geometry

	\eg
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		PolygonMesh mesh(myCube.GetActivePrimitive().GetGeometry());
		Vertex vertex0(mesh.GetVertices().GetItem(0));

		app.LogMessage( L"Number of nodes for Vertex 0: " +
			CValue(vertex0.GetNodes().GetCount()).GetAsText() );
	\endcode

*/
//*****************************************************************************

class SICPPSDKDECL Vertex : public Point
{
public:
	/*! Default constructor. */
	Vertex();

	/*! Default destructor. */
	~Vertex();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Vertex(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Vertex(const Vertex& in_obj);

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
	\return The new Vertex object.
	*/
	Vertex& operator=(const Vertex& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Vertex object.
	*/
	Vertex& operator=(const CRef& in_ref);

	/*! Returns the PolygonNode objects adjacent to this Vertex.
	\return Array of PolygonNode objects.
	*/
	CPolygonNodeRefArray GetNodes() const;

	/*! Returns an array of Vertex neighbors within a given distance. If the
	distance is 1 the Vertex are returned in counter-clockwise order.
	\param in_lDistance Value representing the degree of neighborhood
	\return Array of Vertex objects.
	*/
	CVertexRefArray GetNeighborVertices(LONG in_lDistance = 1) const;

	/*! Returns an array of Edge neighbors within a given distance.If the
	distance is 1 all Edge objects for which one of the extremities is the
	current Vertex are returned in counter-clockwise order.
	\param in_lDistance Value representing the degree of neighborhood
	\return Array of Edge objects.
	*/
	CEdgeRefArray GetNeighborEdges(LONG in_lDistance = 1) const;

	/*! Returns an array of PolygonFace neighbors within a given distance.If
	the	distance is 1 all PolygonFace for which one of the Vertex objects is
	the current Vertex are returned in counter-clockwise order.
	\param in_lDistance Value representing the degree of neighborhood
	\return Array of PolygonFace objects.
	*/
	CPolygonFaceRefArray GetNeighborPolygons(LONG in_lDistance = 1) const;

	/*! Returns an array of Vertex neighbors adjacent to this Vertex, within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
	\return Array of Vertex objects.
	*/
	CVertexRefArray GrowNeighborVertices(LONG in_lDistance = 1) const;

	/*! Returns the crease value associated with this vertex.
	\return The crease value between 0.0 and 10.0.
	\since 3.5.1
	*/
	double GetCrease()const;

	/*! Returns whether the control point is a boundary or not.
	\return true if the control point is a boundary, false if not.
	\since 4.2
	*/
	bool GetIsBoundary()const;

	private:
	Vertex * operator&() const;
	Vertex * operator&();
};

//*****************************************************************************
/*! \class CVertexRefArray xsi_vertex.h
	\brief Represents an array of Vertex objects.

	\warning This specialized array is returned by PolygonMesh::GetVertices, PolygonFace::GetVertices
		and Edge::GetVertices, it is not meant to be created and modified in user-defined functions.
		If you want to add and remove arbitrary items to a collection, you must use a CRefArray instead.

	\sa PolygonMesh::GetVertices, PolygonFace::GetVertices, Edge::GetVertices

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		PolygonMesh mesh(myGrid.GetActivePrimitive().GetGeometry());
		CVertexRefArray vertices(mesh.GetVertices());

		LONG lCount = vertices.GetCount();
		for (LONG i=0; i<lCount; i++ )
		{
			Vertex vertex(vertices.GetItem(i));

			CVertexRefArray neighbors(vertex.GetNeighborVertices(1));
			app.LogMessage( L"Number of neighbor vertices for Vertex " +
				CValue(i).GetAsText() + L": " +
				CValue(neighbors.GetCount()).GetAsText() );
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL CVertexRefArray : public CPointRefArray
{
	public:
	CVertexRefArray();

	/*! Copy constructor.
	\param in_array constant CVertexRefArray reference object.
	*/
	CVertexRefArray( const CVertexRefArray& in_array );

	/*! Returns a Vertex object at a specified index in the array.
	\param in_index The index of the Vertex.
	\return CRef Vertex object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the Vertex object in the array matching the specified name.
	\param in_name The name of the Vertex.
	\return CRef Vertex object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of Vertex objects in the array.
	\return LONG Number of Vertex objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	 * \return bool True if valid, false otherwise.
	 */
	bool IsValid() const;

	/*! Returns an array of Vertex neighbors for all vertices contained in this array, within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
	\return CVertexRefArray An array of Vertex objects
	*/
	CVertexRefArray GetNeighborVertices(LONG in_lDistance = 1) const;

	/*! Returns an array of Edge neighbors for all vertices contained in this array, within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
	\return CEdgeRefArray An array of Edge objects
	*/
	CEdgeRefArray GetNeighborEdges(LONG in_lDistance = 1) const;

	/*! Returns an array of PolygonFace neighbors for all vertices contained in this array, within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
	\return CPolygonFaceRefArray An array of PolygonFace objects
	*/
	CPolygonFaceRefArray GetNeighborPolygons(LONG in_lDistance = 1) const;

	/*! Returns an array of adjacent Vertex neighbors for all vertices contained in this array, within a given distance.
	\param in_lDistance Value representing the degree of neighborhood
	\return CVertexRefArray An array of Vertex objects
	*/
	CVertexRefArray GrowNeighborVertices(LONG in_lDistance = 1) const;

	/*! Returns an array of crease values for all vertices contained in this vertex array.
	\return CDoubleArray An array of crease value.
	\since 3.5.1
	*/
	CDoubleArray GetCreaseArray() const;
};

};

#endif // __XSIVERTEX_H__
