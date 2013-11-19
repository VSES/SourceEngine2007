//*****************************************************************************
/*!
   \file xsi_trianglevertex.h
   \brief TriangleVertex and CTriangleVertexRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITRIANGLEVERTEX_H__
#define __XSITRIANGLEVERTEX_H__

#include <xsi_siobject.h>
#include <xsi_math.h>
#include <xsi_uv.h>
#include <xsi_vertexcolor.h>

namespace XSI {

//*****************************************************************************
/*! \class TriangleVertex xsi_trianglevertex.h
	\brief A TriangleVertex represents a Point of a Triangle object.

	\sa Triangle, CTriangleRefArray, CTriangleVertexRefArray, Geometry

	\eg
	\code
		using namespace XSI;
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		Triangle triangle(
			myCube.GetActivePrimitive().GetGeometry().GetTriangles().GetItem(0));
		TriangleVertex vertex0(triangle.GetPoints().GetItem(0));

		CVector3 pos(vertex0.GetPosition());

		app.LogMessage( L"Vertex 0: " +
			CValue(pos.GetX()).GetAsText() + L"," +
			CValue(pos.GetY()).GetAsText() + L"," +
			CValue(pos.GetZ()).GetAsText() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL TriangleVertex : public SIObject
{
public:
	/*! Default constructor. */
	TriangleVertex();

	/*! Default destructor. */
	~TriangleVertex();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	TriangleVertex(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	TriangleVertex(const TriangleVertex& in_obj);

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
	\return The new TriangleVertex object.
	*/
	TriangleVertex& operator=(const TriangleVertex& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new TriangleVertex object.
	*/
	TriangleVertex& operator=(const CRef& in_ref);

	/*! Returns the position of this TriangleVertex object.
	\return The TriangleVertex position.
	*/
	MATH::CVector3 GetPosition() const;

	/*! Returns the normal of this TriangleVertex object. Triangles that belong to
	the same polygon have the same normal values.
	\return The TriangleVertex normal.
	*/
	MATH::CVector3 GetNormal() const;

	/*! Returns the vertex color of this TriangleVertex object.
	\return The TriangleVertex color.
	*/
	CVertexColor GetColor() const;

	/*! Returns the UV coordinates this TriangleVertex object.
	\return The TriangleVertex UV coordinates.
	*/
	CUV GetUV() const;

	/*! Returns the index of this TriangleVertex object in the geometry.
	\return The TriangleVertex index.
	*/
	LONG GetIndex() const;

	private:
	TriangleVertex * operator&() const;
	TriangleVertex * operator&();
};

//*****************************************************************************
/*! \class CTriangleVertexRefArray xsi_trianglevertex.h
	\brief An array of TriangleVertex objects that represents all points of a Triangle object.

	\warning This specialized array is returned by Triangle::GetPoints, it is not meant
		to be created and modified in user-defined functions. If you want to add and remove
		arbitrary items to a collection, you must use a CRefArray instead.

	\sa Triangle::GetPoints, Triangle, Geometry

	\eg
	\code
		using namespace XSI;
		using namespace MATH;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		Triangle triangle(myCube.GetActivePrimitive().GetGeometry().GetTriangles().GetItem(0));
		CTriangleVertexRefArray vertices(triangle.GetPoints());

		CVertexColorArray colors(vertices.GetColorArray());
		LONG ulCount = colors.GetCount();

		for (LONG i=0; i<ulCount; i++ )
		{
			app.LogMessage( L"Vertex Color " +
				CValue((LONG)i).GetAsText() + L": " +
				CValue(colors[i].r).GetAsText() + L"," +
				CValue(colors[i].g).GetAsText() + L"," +
				CValue(colors[i].b).GetAsText() + L"," +
				CValue(colors[i].a).GetAsText() );
		}
	\endcode
*/
//*****************************************************************************
class SICPPSDKDECL CTriangleVertexRefArray : public CRefArray
{
	public:
	CTriangleVertexRefArray();

	/*! Copy constructor.
	\param in_array constant CTriangleVertexRefArray reference object.
	*/
	CTriangleVertexRefArray( const CTriangleVertexRefArray& in_array );

	/*! Returns a TriangleVertex object at a specified index in the array.
	\param in_index The index of the TriangleVertex.
	\return The Triangle object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the TriangleVertex object in the array matching the specified name.
	\param in_name The name of the TriangleVertex.
	\return The Triangle object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of TriangleVertex objects in the array.
	\return The number of TriangleVertex objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Returns an array of CVector3 objects representing the positions of all
	TriangleVertex objects in this array.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetPositionArray() const;

	/*! Returns an array of CVector3 objects representing the normals of all
	TriangleVertex objects in this array.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetNormalArray() const;

	/*! Returns an array of CVertexColor objects representing the colors of all
	TriangleVertex objects in this array.
	\return Array of CVertexColor objects.
	*/
	CVertexColorArray GetColorArray() const;

	/*! Returns an array of CUV objects representing the UV coordinates of all
	TriangleVertex objects in this array.
	\return Array of CUV objects.
	*/
	CUVArray GetUVArray() const;

	private:
	void* m_ptr;
};

};

#endif // __XSITRIANGLEVERTEX_H__
