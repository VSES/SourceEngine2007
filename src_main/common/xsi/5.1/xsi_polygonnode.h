//*****************************************************************************
/*!
   \file xsi_polygonnode.h
   \brief PolygonNode and CPolygonNodeRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPOLYGONNODE_H__
#define __XSIPOLYGONNODE_H__

#include <xsi_sample.h>
#include <xsi_math.h>

namespace XSI {

//*****************************************************************************
/*! \class PolygonNode xsi_polygonnode.h
	\brief The PolygonNode object gives access to the X3DObject's polygon node
	geometry. A polygon represents the edge junction of a polygon object.

	\sa PolygonFace::GetNodes

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		PolygonMesh mesh( myCube.GetActivePrimitive().GetGeometry(0) );
		PolygonFace face( mesh.GetPolygons().GetItem(0) );
		PolygonNode node( face.GetNodes().GetItem(0) );

		app.LogMessage( L"Index of Node 0: " +
						CValue(node.GetIndex()).GetAsText() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL PolygonNode : public Sample
{
public:
	/*! Default constructor. */
	PolygonNode();

	/*! Default destructor. */
	~PolygonNode();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	PolygonNode(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	PolygonNode(const PolygonNode& in_obj);

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
	\return The new PolygonNode object.
	*/
	PolygonNode& operator=(const PolygonNode& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new PolygonNode object.
	*/
	PolygonNode& operator=(const CRef& in_ref);

	/*! Returns the index of this PolygonNode object within the polygon face.
	\return The index of this PolygonNode object.
	*/
	LONG GetIndex() const;

	/*! Returns the normal of this PolygonNode object.
	\param io_bValid Returns true if the normal is valid.
	\return The normal expressed as a CVector3 object..
	*/
	MATH::CVector3 GetNormal(bool& io_bValid) const;

	private:
	PolygonNode * operator&() const;
	PolygonNode * operator&();
};

//*****************************************************************************
/*! \class CPolygonNodeRefArray xsi_polygonnode.h
	\brief An array of PolygonNode objects representing the edge junction in a polygon.

	\warning This specialized array is returned by Vertex::GetNodes, PolygonMesh::GetNodes,
		PolygonFace::GetNodes, and Edge::GetNodes, it is not meant to be
		created and modified in user-defined functions. If you want to add and remove
		arbitrary items to a collection, you must use a CRefArray instead.

	\sa Vertex::GetNodes, PolygonMesh::GetNodes, PolygonFace::GetNodes, Edge::GetNodes

	\eg
	\code
		using namespace XSI;
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"", myCone );

		PolygonMesh mesh( myCone.GetActivePrimitive().GetGeometry(0) );
		CPolygonNodeRefArray pnodes(mesh.GetNodes());

		LONG lCount = pnodes.GetCount();
		app.LogMessage( CString(L"Number of nodes: ") + CValue(lCount).GetAsText() );

		CVector3 norm;
		PolygonNode pnode;
		for ( LONG i=0; i<lCount; i++ )
		{
			bool bValid;
			pnode = pnodes[i];
			norm = pnode.GetNormal(bValid);

			app.LogMessage(
				L"Normal: (" + CString(CValue(bValid).GetAsText()) +
				L") " +
				CValue(norm.GetX()).GetAsText() + L"," +
				CValue(norm.GetY()).GetAsText() + L"," +
				CValue(norm.GetZ()).GetAsText() );
		}
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL CPolygonNodeRefArray : public CSampleRefArray
{
	public:
	CPolygonNodeRefArray();

	/*! Copy constructor.
	\param in_array constant CPolygonNodeRefArray reference object.
	*/
	CPolygonNodeRefArray( const CPolygonNodeRefArray& in_array );

	/*! Returns a PolygonNode reference object at a specified index in the array.
	\param in_index The index of the PolygonNode.
	\return A reference to the PolygonNode object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns a PolygonNode reference object matching the specified name in the array.
	\param in_name The name of the PolygonNode.
	\return A reference to the PolygonNode object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of PolygonNode objects in the array.
	\return The number of PolygonNode objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	 * \return true if valid, false otherwise.
	 */
	bool IsValid() const;

	/*! Returns an array of normals expressed with CVector3 objects.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetNormalArray() const;
};


};

#endif // __XSIPOLYGONNODE_H__
