//*****************************************************************************
/*!
   \file xsi_edge.h
   \brief Edge and CEdgeRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIEDGE_H__
#define __XSIEDGE_H__

#include <xsi_segment.h>

namespace XSI {

class CVertexRefArray;
class CPolygonFaceRefArray;
class CEdgeRefArray;
class CPolygonNodeRefArray;
class CDoubleArray;
class CBoolArray;

//*****************************************************************************
/*! \class Edge xsi_edge.h
	\brief The Edge object gives access to the geometry edges of an X3DObject's primitive.

	Edges are the PolygonMesh-specifics of Segments.  You can reach edges from PolygonFace::GetEdges and PolygonMesh::GetEdges.

	\sa CEdgeRefArray, Segment, Vertex, PolygonMesh, Geometry

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		PolygonMesh mesh( myCube.GetActivePrimitive().GetGeometry() );

		Edge edge( mesh.GetEdges().GetItem(0) );
		app.LogMessage( L"This object is an: " + edge.GetClassIDName() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Edge : public Segment
{
public:
	/*! Default constructor. */
	Edge();

	/*! Default destructor. */
	~Edge();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Edge(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Edge(const Edge& in_obj);

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
	\return The new Edge object.
	*/
	Edge& operator=(const Edge& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Edge object.
	*/
	Edge& operator=(const CRef& in_ref);

	/*! Returns the vertices at the ends of this edge.
	\return An array of Vertex objects
	*/
	CVertexRefArray GetVertices() const;

	/*! Returns the PolygonNode objects adjacent to this edge.
	\return An array of PolygonNode objects
	*/
	CPolygonNodeRefArray GetNodes() const;

	/*! Returns an array of Vertex neighbors within a given distance.If the distance is 1, the 2 Vertex objects
	comprising the current Edge are returned.
	\param in_lDistance Value Represents the degree of neighborhood (eg., degree=2 for an edge means its adjacent
		vertices plus the adjacent vertices of the adjacent vertices).
	\return An array of Vertex objects
	*/
	CVertexRefArray GetNeighborVertices(LONG in_lDistance = 1) const;

	/*! Returns an array of Edge neighbors within a given distance.If the distance is 1 the Edge objects are returned
	in counter-clockwise order.
	\param in_lDistance Value Represents the degree of neighborhood (eg., degree=2 for an edge means its adjacent
		vertices plus the adjacent vertices of the adjacent vertices).
	\return An array of Edge objects
	*/
	CEdgeRefArray GetNeighborEdges(LONG in_lDistance = 1) const;

	/*! Returns an array of PolygonFace neighbors within a given distance.If the	distance is 1 the adjacent
	PolygonFace objects are returned (1 if border edge, 2 otherwise).
	\param in_lDistance Value representing the degree of neighborhood (eg., degree=2 for an edge means its
		adjacent vertices plus the adjacent vertices of the adjacent vertices).
	\return An array of PolygonFace objects
	*/
	CPolygonFaceRefArray GetNeighborPolygons(LONG in_lDistance = 1) const;

	/*! Returns an array of Edge objects that are adjacent to this Edge object, within a given distance.
	\param in_lDistance Value representing the degree of neighborhood (eg., degree=2 for an edge means its
		adjacent vertices plus the adjacent vertices of the adjacent vertices).
	\return An array of Edge objects
	*/
	CEdgeRefArray GrowNeighborEdges(LONG in_lDistance = 1) const;

	/*! Returns the crease value associated with this edge.
	\return The crease value between 0.0 and 10.0.
	\since 3.5.1
	*/
	double GetCrease()const;

	/*! Returns whether the edge is hard or not.
	\return True if the edge is hard, false if not.
	\since 3.5.1
	*/
	bool GetIsHard()const;

	/*! Returns whether the edge is a boundary or not.
	\return True if the edge is a boundary, false if not.
	\since 4.2
	*/
	bool GetIsBoundary()const;

	private:
	Edge * operator&() const;
	Edge * operator&();
};

//*****************************************************************************
/*! \class CEdgeRefArray xsi_edge.h
	\brief Array of Edge objects.

	\warning This specialized array is returned by PolygonFace::GetEdges and PolygonMesh::GetEdges,
		it is not meant to be created and modified in user-defined functions. If you want to add and remove arbitrary
		items to a collection, you must use a CRefArray instead.

	\sa PolygonMesh::GetEdges, PolygonFace::GetEdges

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		PolygonMesh mesh( myCube.GetActivePrimitive().GetGeometry() );

		CEdgeRefArray edges(mesh.GetEdges());

		app.LogMessage( L"Number of edges: " +
						CValue(edges.GetCount()).GetAsText() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL CEdgeRefArray : public CSegmentRefArray
{
	public:
	/*! Constructor.
	*/
	CEdgeRefArray();

	/*! Copy constructor.
	\param in_array constant CEdgeRefArray reference object.
	*/
	CEdgeRefArray( const CEdgeRefArray& in_array);

	/*! Returns the Edge object at the specified index in the array.
	\param in_index The index of the Edge.
	\return A reference to the specified Edge object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the Edge object in the array matching the specified name.
	\param in_name The name of the Edge.
	\return A reference to the specified Edge object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Gets the number of Edges in the array.
	\return The number of Edge objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Returns an array of Vertex neighbors within a given distance.
	\param in_lDistance Value Represents the degree of neighborhood (eg., degree=2 for an edge means its adjacent
		vertices plus the adjacent vertices of the adjacent vertices).
	\return An array of Vertex objects
	*/
	CVertexRefArray GetNeighborVertices(LONG in_lDistance = 1) const;

	/*! Returns an array of Edge neighbors within a given distance.
	\param in_lDistance Value representing the degree of neighborhood (eg., degree=2 for an edge means its adjacent
		vertices plus the adjacent vertices of the adjacent vertices).
	\return An array of Edge objects
	*/
	CEdgeRefArray GetNeighborEdges(LONG in_lDistance = 1) const;

	/*! Returns an array of PolygonFace neighbors within a given distance.
	\param in_lDistance Value representing the degree of neighborhood (eg., degree=2 for an edge means its adjacent
		vertices plus the adjacent vertices of the adjacent vertices).
	\return An array of PolygonFace objects
	*/
	CPolygonFaceRefArray GetNeighborPolygons(LONG in_lDistance = 1) const;

	/*! Returns an array of Edge objects on the geometry that are adjacent to the Edge objects contained in the array,
	within a given distance.
	\param in_lDistance Value representing the degree of neighborhood (eg., degree=2 for an edge means its adjacent
		vertices plus the adjacent vertices of the adjacent vertices).
	\return An array of Edge objects
	*/
	CEdgeRefArray GrowNeighborEdges(LONG in_lDistance = 1) const;

	/*! Returns an array of crease values for all edges contained in this edge array.
	\return An array of crease values (doubles).
	\since 3.5.1
	*/
	CDoubleArray GetCreaseArray() const;

	/*! Returns an array of boolean values indicating whether each edge in the array is hard or not.
	\return An array of boolean values.
	\since 3.5.1
	*/
	CBoolArray GetIsHardArray() const;
};

};

#endif // __XSIEDGE_H__
