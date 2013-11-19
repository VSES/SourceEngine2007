//*****************************************************************************
/*!
   \file xsi_segment.h
   \brief Segment and CSegmentRefArray class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISEGMENT_H__
#define __XSISEGMENT_H__

#include <xsi_siobject.h>
#include <xsi_math.h>
#include <xsi_longarray.h>

namespace XSI {

class CPointRefArray;
class CSampleRefArray;
class SubComponent;
class Segment;

//*****************************************************************************
/*! \class Segment xsi_segment.h
	\brief A Segment is a generic concept used to describe a PolygonMesh's Edge.

	Having a generic term for this concept allows the user to write code to traverse an object's geometry
	in a generic fashion without resorting to special case handling based on a specific geometry type.

	\sa CSegmentRefArray, Point, Facet, Geometry

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		CSegmentRefArray segments(
			myCube.GetActivePrimitive().GetGeometry().GetSegments() );

		LONG lCount = segments.GetCount();
		for ( LONG i=0; i<lCount; i++ )
		{
			Segment segment(segments[i]);

			app.LogMessage( L"Segment.Index " +
				CValue(segment.GetIndex()).GetAsText() );
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL Segment : public SIObject
{
public:
	/*! Default constructor. */
	Segment();

	/*! Default destructor. */
	~Segment();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Segment(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Segment(const Segment& in_obj);

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
	\return The new Segment object.
	*/
	Segment& operator=(const Segment& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Segment object.
	*/
	Segment& operator=(const CRef& in_ref);

	/*! Returns an array of Point objects on this Segment object.
	\return Array of Point objects.
	*/
	CPointRefArray GetPoints() const;

	/*! Returns the index of this Segment object in the geometry's segment array.
	\return The Segment index.
	*/
	LONG GetIndex() const;

	/*! Returns an array of Sample object on this Segment object.
	\return Array of Sample objects.
	*/
	CSampleRefArray GetSamples() const;

	/*! Creates a SubComponent object from this Segment object,
	\return A SubComponent object
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the Segment objects of the underlying geometry as specifed by
	the navigation component type.  Navigation is circular -- for instance,
	navigating beyond the last component will return the first and vice versa.
	\param in_siNavigate Type of direction for navigating the Segment objects.
	\return The Segment object being visited.
	*/
	Segment Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	Segment * operator&() const;
	Segment * operator&();
};

//*****************************************************************************
/*! \class CSegmentRefArray xsi_segment.h
	\brief An array of Segment objects that represents all the segment sub-components in the Geometry.

	\warning This specialized array is returned by Geometry::GetSegments and Facet::GetSegments,
		it is not meant to be created and modified in user-defined functions. If you want to add
		and remove arbitrary items to a collection, you must use a CRefArray instead.

	\sa Geometry::GetSegments, Facet::GetSegments,  Geometry, Segment

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		CSegmentRefArray segments(
			myCube.GetActivePrimitive().GetGeometry().GetSegments() );

		CLongArray indexArray = segments.GetIndexArray();


		for (LONG i=0; i< indexArray.GetCount(); ++i )
		{
			app.LogMessage( L"Segment.Index " +
							CValue(indexArray[i]).GetAsText() );
		}
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL CSegmentRefArray : public CRefArray
{
	public:
	CSegmentRefArray();

	/*! Copy constructor.
	\param in_array constant CSegmentRefArray reference object.
	*/
	CSegmentRefArray( const CSegmentRefArray& in_array );

	/*! Returns a reference to the Segment object at the specified index in the array.
	\param in_index The index of the Segment to get.
	\return A a reference to the segment.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns a reference to the Segment object specified by name.
	\param in_name The name of the Segment to get.
	\return A a reference to the segment.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of Segment objects in the array.
	\return The number of Segment objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Returns an array of normals expressed as CVector3 objects.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetNormalArray() const;

	/*! Returns an array of indices in the geometry's Segment array.
	\return Array of indices.
	*/
	CLongArray GetIndexArray() const;

	/*! Creates a SubComponent object from this Segment array.
	\return A SubComponent object
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the Segment objects of the underlying geometry as specifed by the navigation component
	type. An array of Segment objects corresponding to each Sample object visited is returned.  Navigation
	is circular -- for instance, navigating beyond the last component will return the first and vice versa.
	\param in_siNavigate Type of direction for navigating the Segment objects.
	\return An array of Segment objects.
	*/
	CSegmentRefArray Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	void* m_ptr;
};

};

#endif // __XSISEGMENT_H__
