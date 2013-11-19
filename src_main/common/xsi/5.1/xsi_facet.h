//*****************************************************************************
/*!
   \file xsi_facet.h
   \brief Facet and CFacetRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIFACET_H__
#define __XSIFACET_H__

#include <xsi_siobject.h>
#include <xsi_math.h>
#include <xsi_longarray.h>

namespace XSI {

class CSegmentRefArray;
class CSampleRefArray;
class CPointRefArray;
class SubComponent;

//*****************************************************************************
/*! \class Facet xsi_facet.h
	\brief A facet is a generic concept used to describe a Geometry's PolygonFace, NurbsSurface or NurbsCurve.

	Having a generic term for this concept allows the user to write code to traverse an object geometry in a
	generic fashion without resorting to special case handling based on a specific geometry type.

	\sa CFacetRefArray, Point, Segment, Geometry, Geometry::GetFacets

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		CFacetRefArray facets( myCube.GetActivePrimitive().GetGeometry().GetFacets() );

		app.LogMessage( facets.GetClassIDName() );

		LONG lFacetCount = facets.GetCount();
		for ( LONG i=0; i<lFacetCount; i++ )
		{
			Facet facet(facets[i]);
			CSegmentRefArray segments( facet.GetSegments() );

			app.LogMessage( L"Facet.Index " +
							CValue(facet.GetIndex()).GetAsText() );

			LONG lSegmentCount = segments.GetCount();

			for (LONG j=0; j<lSegmentCount; j++)
			{
				Segment seg = segments[j];
				app.LogMessage( L"Facet.Segment.Index " +
								CValue(seg.GetIndex()).GetAsText() );
			}
		}
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Facet : public SIObject
{
public:
	/*! Default constructor. */
	Facet();

	/*! Default destructor. */
	~Facet();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Facet(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Facet(const Facet& in_obj);

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
	\return The new Facet object.
	*/
	Facet& operator=(const Facet& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Facet object.
	*/
	Facet& operator=(const CRef& in_ref);

	/*! Returns an array of Segment objects on this Facet object.
	\return Array of references to Segment objects.
	*/
	CSegmentRefArray GetSegments() const;

	/*! Returns an array of Point objects on this Facet object.
	\return Array of references to Point objects.
	*/
	CPointRefArray GetPoints() const;

	/*! Returns the number of points on this Facet object.
	\since 4.2
	*/
	LONG GetNbPoints()const;

	/*! Returns the index of this Facet object in the CFacetRefArray.
	\return The index of this facet in the geometry.
	*/
	LONG GetIndex() const;

	/*! Returns an array of Sample object on this Facet object.
	\return Array of references to Sample objects.
	*/
	CSampleRefArray GetSamples() const;

	/*! Creates a SubComponent object from this Facet object,
	\return The SubComponent object.
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the Facet objects of the underlying geometry as specifed by the navigation component type (::siNavigateComponentType).
	Navigation is circular -- for instance, navigating beyond the last component will return the first and vice versa.
	\param in_siNavigate Type of direction for navigating the Facet objects (one of the ::siNavigateComponentType values).
	\return The Facet object being visited.
	*/
	Facet Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	Facet * operator&() const;
	Facet * operator&();
};

//*****************************************************************************
/*! \class CFacetRefArray xsi_facet.h
	\brief An array of Facet objects representing all segments of the underlying Geometry object.

	\warning This specialized array is returned by Geometry::GetFacets, it is not meant to be created
		and modified in user-defined functions. If you want to add and remove arbitrary items to a
		collection, you must use a CRefArray instead.

	\sa Geometry::GetFacets

	\eg
	\code
		using namespace XSI;
		using namespace MATH;

		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		CFacetRefArray facets( myCube.GetActivePrimitive().GetGeometry().GetFacets() );

		CVector3Array normArray = facets.GetNormalArray();

		app.LogMessage( facets.GetClassIDName() );
		app.LogMessage( CValue(normArray.GetCount()).GetAsText() );

		for (LONG i=0; i<normArray.GetCount(); i++)
		{
			app.LogMessage( L"Normal " + CValue(i).GetAsText() );

			app.LogMessage(
				CValue(normArray[i].GetX()).GetAsText() + L"," +
				CValue(normArray[i].GetY()).GetAsText() + L"," +
				CValue(normArray[i].GetZ()).GetAsText() );
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL CFacetRefArray : public CRefArray
{
	public:
	/*! Constructor.
	*/
	CFacetRefArray();

	/*! Copy constructor.
	\param in_array constant CFacetRefArray reference object.
	*/
	CFacetRefArray( const CFacetRefArray& in_array);

	/*! Returns a Facet object at a specified index in the array.
	\param in_index The index of the Facet to get.
	\return A reference to the specified Facet object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns the Facet object in the array matching the specified name.
	\param in_name The name of the Facet to get.
	\return A reference to the specified Facet object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of Facet objects in the array.
	\return The number of Facet objects.
	*/
	LONG GetCount() const;

	/*! Returns whether this array is a valid object.
	\return True if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Returns an array of normals expressed with CVector3 objects.
	\return Array of CVector3 objects.
	*/
	MATH::CVector3Array GetNormalArray() const;

	/*! Returns an array of indices in this CFacetRefArray.
	\return Array of (LONG) indices.
	*/
	CLongArray GetIndexArray() const;

	/*! Creates a SubComponent object from this Facet array,
	\return A SubComponent object
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the Facet objects of the underlying geometry as specifed by the navigation component type (::siNavigateComponentType).
	An array of Facet objects corresponding to each Facet object visited is returned.  Navigation is circular -- for instance,
	navigating beyond the last component will return the first and vice versa.
	\param in_siNavigate Type of direction for navigating the Facet objects (one of the ::siNavigateComponentType values).
	\return An array of refernces to Facet objects.
	*/
	CFacetRefArray Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	void* m_ptr;
};

};

#endif // __XSIFACET_H__
