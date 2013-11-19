//*****************************************************************************
/*!
   \file xsi_point.h
   \brief Point and CPointRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPOINT_H__
#define __XSIPOINT_H__

#include <xsi_siobject.h>
#include <xsi_math.h>
#include <xsi_longarray.h>

namespace XSI {

class CSampleRefArray;
class SubComponent;
class Point;
class CPointRefList;

//*****************************************************************************
/*! \class Point xsi_point.h
	\brief A Point is a generic concept used to describe a Geometry's Vertex or ControlPoint.

	Having a generic term for this concept allows the user to
	write code to traverse an objects geometry in a generic fashion without
	resorting to special case handling based on a specific geometry type.

	\sa CPointRefArray, Segment, Facet, Geometry

	\eg
	\code
		using namespace XSI;
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		Geometry geom = myGrid.GetActivePrimitive().GetGeometry(0);

		Point point(geom.GetPoints().GetItem(0) );

		CVector3 pos( point.GetPosition() );

		app.LogMessage( CValue(pos.GetX()).GetAsText() + L"," +
				CValue(pos.GetY()).GetAsText() + L"," +
				CValue(pos.GetZ()).GetAsText() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Point : public SIObject
{
public:
	/*! Default constructor. */
	Point();

	/*! Default destructor. */
	~Point();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Point(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Point(const Point& in_obj);

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
	\return The new Point object.
	*/
	Point& operator=(const Point& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Point object.
	*/
	Point& operator=(const CRef& in_ref);

	/*! Returns the position of this Point object.
	\return The position of this Point object.
	*/
	MATH::CVector3 GetPosition() const;

	/*! Sets the position of this Point object.
	\warning Not implemented.
	\param in_vect CVector3 object.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus PutPosition(const MATH::CVector3& in_vect);

	/*! Returns the normal of this Point object.
	\param io_bValid Returns true if the normal is valid.
	\return The normal expressed as a CVector3 object.
	*/
	MATH::CVector3 GetNormal(bool& io_bValid) const;

	/*! Returns the index of the Point in the geometry's Point array.
	\return The Point index.
	*/
	LONG GetIndex() const;

	/*! Returns an array of Sample objects for this Point..
	\return Array of Sample objects.
	*/
	CSampleRefArray GetSamples() const;

	/*! Creates a SubComponent object from this Point object.
	\return The SubComponent object.
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the Point objects of the underlying geometry as specifed by
	the navigation component type.  Navigation is circular -- for instance,
	navigating beyond the last component will return the first and vis versa.

	\param in_siNavigate Type of direction for navigating the Facet objects.
	\return The Point object being visited.
	*/
	Point Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	Point * operator&() const;
	Point * operator&();
};

//*****************************************************************************
/*! \class CPointRefArray xsi_point.h
	\brief An array of Point objects that represent the points in the Geometry.

	\warning This specialized array is returned by Geometry::GetPoints, it is not meant to be
		created and modified in user-defined functions. If you want to add and remove
		arbitrary items to a collection, you must use a CRefArray instead.

	\eg

	\code
		using namespace XSI;
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", myGrid );

		Geometry geom = myGrid.GetActivePrimitive().GetGeometry(0);

		CPointRefArray points( geom.GetPoints() );

		CValue val(points.GetCount());
		app.LogMessage( CString(L"Number of points: ") + val.GetAsText() );

		for(LONG i = 0; i < points.GetCount();  ++i)
		{
			Point pt(points[i]);
			CVector3 pos( pt.GetPosition() );

			app.LogMessage(
				CValue(pos.GetX()).GetAsText() + L"," +
				CValue(pos.GetY()).GetAsText() + L"," +
				CValue(pos.GetZ()).GetAsText() );
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL CPointRefArray : public CRefArray
{
	public:
	CPointRefArray();

	/*! Copy onstructor.
	\param in_array constant CPointRefArray reference object.
	*/
	CPointRefArray( const CPointRefArray& in_array );

	/*! Returns a reference to the Point object at a specified index in the array.
	\param in_index The index of the Point.
	\return The Point reference object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns a reference to the Point object matching the specified index.
	\param in_name The name of the Point.
	\return The Point reference object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of Point objects in the array.
	\return The number of Point objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Returns an array of positions expressed with CVector3 objects.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetPositionArray() const;

	/*! Sets the positions as an array of CVector3 objects. For non-custom
	operators this property can only be set if the object has been frozen.
	\param in_points An array of CVector3.
	\return CStatus::OK success
	*/
	CStatus PutPositionArray(const MATH::CVector3Array& in_points);

	/*! Returns an array of normals expressed with CVector3 objects.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetNormalArray() const;

	/*! Return an array of indices in the geometry Point array
	\return An array of indices.
	*/
	CLongArray GetIndexArray() const;

	/*! Creates a SubComponent object from this Point array,
	\return A SubComponent object
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the Point objects of the underlying geometry as specifed by
	the navigation component type. The function returns an array of Point
	objects corresponding to each Point object visited. Navigation is circular
	-- for instance, navigating beyond the last component will return the first
	and vis versa.
	\param in_siNavigate Type of direction for navigating the Point objects.
	\return An array of Point objects.
	*/
	CPointRefArray Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	void* m_ptr;
};

};

#endif // __XSIPOINT_H__
