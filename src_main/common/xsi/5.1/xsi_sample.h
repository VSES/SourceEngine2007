//*****************************************************************************
/*!
   \file xsi_sample.h
   \brief Sample and CSampleRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISAMPLE_H__
#define __XSISAMPLE_H__

#include <xsi_siobject.h>
#include <xsi_longarray.h>

namespace XSI {

class SubComponent;
class Sample;

//*****************************************************************************
/*! \class Sample xsi_sample.h
	\brief A Sample is a generic concept used to describe a PolygonNode or NurbsSample.

	Having a generic term for this concept allows the user to write code to traverse an object's geometry
	in a generic fashion without resorting to special case handling based on a specific geometry type.

	\sa Facet::GetSamples, Segment::GetSamples, Point::GetSamples, Geometry::GetSamples, CSampleRefArray

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		CSampleRefArray samples(
			myCube.GetActivePrimitive().GetGeometry().GetSamples() );

		LONG lCount = samples.GetCount();
		for ( LONG i=0; i<lCount; i++ )
		{
			Sample sample(samples[i]);

			app.LogMessage( L"Sample.Index " +
							CValue(sample.GetIndex()).GetAsText() );
		}
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Sample : public SIObject
{
public:
	/*! Default constructor. */
	Sample();

	/*! Default destructor. */
	~Sample();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Sample(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Sample(const Sample& in_obj);

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
	\return The new Sample object.
	*/
	Sample& operator=(const Sample& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Sample object.
	*/
	Sample& operator=(const CRef& in_ref);

	/*! Returns the index of this Sample object relative to its Facet object.
	\return The sample's index.
	*/
	LONG GetIndex() const;

	/*! Creates a SubComponent object from this Sample object,
	\return A SubComponent object
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the Sample objects of the underlying geometry as specifed by
	the navigation component type.  Navigation is circular -- for instance,
	navigating beyond the last component will return the first and vice versa.
	\param in_siNavigate Type of direction for navigating the Sample objects.
	\return The Sample object being visited.
	*/
	Sample Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	Sample * operator&() const;
	Sample * operator&();
};

//*****************************************************************************
/*! \class CSampleRefArray xsi_sample.h
	\brief Array of Sample objects that represent the samples in the Geometry.

	\warning This specialized array is returned by Facet::GetSamples, Segment::GetSamples
		and Point::GetSamples, it is not meant to be created and modified in user-defined
		functions. If you want to add and remove arbitrary items to a collection, you must
		use a CRefArray instead.

	\sa Facet::GetSamples, Segment::GetSamples, Point::GetSamples

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );

		CSampleRefArray samples( myCube.GetActivePrimitive().GetGeometry().GetSamples() );

		CLongArray indexArray = samples.GetIndexArray();

		for (LONG i=0; i< indexArray.GetCount(); ++i )
		{
			app.LogMessage( L"Sample.Index " +
							CValue(indexArray[i]).GetAsText() );
		}
	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL CSampleRefArray : public CRefArray
{
	public:
	CSampleRefArray();

	/*! Copy constructor.
	\param in_array constant CSampleRefArray reference object.
	*/
	CSampleRefArray( const CSampleRefArray& in_array );

	/*! Returns a reference to the Sample object at the specified index in the array.
	\param in_index The index of the Sample.
	\return The Sample object.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns a reference to the Sample object specified by name.
	\param in_name The name of the Sample to get.
	\return The Sample object.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Returns the number of Sample objects in the array.
	\return The number of Sample objects.
	*/
	LONG GetCount() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Returns an array of indices of all Sample objects relative to the Facet object.
	\return Array of indices.
	*/
	CLongArray GetIndexArray() const;

	/*! Creates a SubComponent object from this Sample array,
	\return A SubComponent object
	*/
	SubComponent GetSubComponent() const;

	/*! Navigates the Sample objects of the underlying geometry as specifed by
	the navigation component type. An array of Sample objects corresponding to
	each Sample object visited is returned.  Navigation is circular -- for instance,
	navigating beyond the last component will return the first and vice versa.
	\param in_siNavigate Type of direction for navigating the Sample objects.
	\return An array of Sample objects.
	*/
	CSampleRefArray Navigate(siNavigateComponentType in_siNavigate) const;

	private:
	void* m_ptr;
};

};

#endif // __XSISAMPLE_H__
