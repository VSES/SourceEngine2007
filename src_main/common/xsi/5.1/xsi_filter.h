//*****************************************************************************
/*!	\file xsi_filter.h
	\brief Filter class declaration.

	© Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
	reserved. This file contains confidential and proprietary information of
	Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
	end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIFILTER_H__
#define __XSIFILTER_H__

#include <xsi_siobject.h>

namespace XSI {

//*****************************************************************************
/*! \class Filter xsi_filter.h
	\brief The Filter object represents an instance of an %XSI filter. Filters are used by %XSI interactive tools and
	OM objects for validating the use of a set of objects in a given context. %XSI already defines a set of native
	filters available to users and can be extended with custom filter objects.

	Custom filters are defined with a plug-in which can be either compiled or scripted as described in \a Distributing
	\a Customized \a Items (see \xl cus_filters in the \a SDK \a Customization guide for the detailed workflow).

	Like native filters, custom filters can also be used with other base filter functions defined in the SDK such as
	\xl SIFilter, \xl X3DObjectCollection.Filter, etc.

	\since 4.0

	\eg Demonstrates how to work with filters in XSI

	\code
		#include <xsi_application.h>
		#include <xsi_filter.h>
		#include <xsi_model.h>

		using namespace XSI;

		// Helper functions for logging filter operations results
		static TestFilter( const CString& in_strFilter, const CRef& in_ref )
		{
			Application app;
			Filter filter = app.GetFilters().GetItem( in_strFilter );

			bool bApplicable = filter.IsApplicable(in_ref);
			bool bMatch = filter.Match(in_ref);

			app.LogMessage( L"*** Filter: " + filter.GetName() );
			app.LogMessage( L"   Target: " + SIObject(in_ref).GetName() );
			app.LogMessage( L"   Applicable: " + CValue(bApplicable).GetAsText() );
			app.LogMessage( L"   Match: " + CValue(bMatch).GetAsText() );
		}

		static TestFilter( const CString& in_strFilter, const CString& in_strTest, const CRefArray& in_array )
		{
			Application app;
			Filter filter = app.GetFilters().GetItem( in_strFilter );

			bool bApplicable = filter.IsApplicable(in_array);
			bool bMatch = filter.Match(in_array);
			CRefArray subArray = filter.Subset(in_array);

			app.LogMessage( L"*** Filter: " + filter.GetName() );
			app.LogMessage( L"   Target: " + in_strTest + L" count: " + CValue((LONG)in_array.GetCount()).GetAsText() );
			app.LogMessage( L"   Applicable: " + CValue(bApplicable).GetAsText() );
			app.LogMessage( L"   Match: " + CValue(bMatch).GetAsText() );
			app.LogMessage( L"   Subset: " + CValue(subArray.GetCount()).GetAsText() );
		}

		// Display all filters defined in XSI
		Application app;

		CRefArray filterArray = app.GetFilters();

		for (LONG i=0; i<filterArray.GetCount(); i++)
		{
			Filter filter(filterArray[i]);
			app.LogMessage( filter.GetName() );
		}

		// Sample filter tests
		Model root = app.GetActiveSceneRoot();

		X3DObject sphere;
		root.AddGeometry( L"Sphere", L"MeshSurface", L"", sphere );

		TestFilter( L"Object", root );
		TestFilter( L"PolygonMesh", sphere );

		CRefArray array(root.GetChildren());
		CString strTest(root.GetName() + L" children");
		TestFilter( L"Object", strTest, array );
		TestFilter( L"Light", strTest, array );
		TestFilter( L"Camera", strTest, array );
		TestFilter( L"PolygonMesh", strTest, array );

		array = root.GetProperties();
		strTest = root.GetName() + L" properties";
		TestFilter( L"Object", strTest, array );
		TestFilter( L"Light", strTest, array );

	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Filter : public SIObject
{
public:
	/*! Default constructor. */
	Filter();

	/*! Default destructor. */
	~Filter();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Filter(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Filter(const Filter& in_obj);

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID class type.
	\return true if the class is compatible, false otherwise.
	*/
	bool IsA( siClassID in_ClassID) const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Creates an object from another object. The new object is set to
	empty if the input object is not compatible.
	\param in_obj constant class object.
	\return The new Filter object.
	*/
	Filter& operator=(const Filter& in_obj);

	/*! Creates an object from a reference object. The new object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Filter object.
	*/
	Filter& operator=(const CRef& in_ref);

	/*! Returns true if the input object meets the filter requirements.
	\param in_obj The object to filter.
	\return true if it matches or false otherwise.
	 */
	bool Match( const CRef& in_obj ) const;

	/*! Returns true if each object in the array matches the filter requirements. This is similar to the
	other Filter::Match function.
    \param in_array Array of objects to filter.
	\return true if all objects in the array match the filter or false otherwise.
	 */
	bool Match( const CRefArray& in_array ) const;

	/*! Returns true if the filter is compatible with an input object. This function is used to quickly
	determine whether a filter can be used on a given type of object.
	\param in_obj Object to test.
	\return true if the input object is applicable or false otherwise.
	*/
	bool IsApplicable( const CRef& in_obj ) const;

	/*! Returns true if the filter is compatible with each object of the input array. This function is
	similar to the other Filter::IsApplicable function and is used to quickly determine whether a filter
	can be used on a given type of object.
	\param in_array Array of objects to filter.
	\return true if all objects in the array match the filter or false otherwise.
	*/
	bool IsApplicable( const CRefArray& in_array ) const;

	/*! Returns an array of objects built from the input array. All objects matching the filter requirements
	are returned through the output array.
	\param in_array Array of objects to filter in order to build the subset.
	\return Array of objects that match the filter criterias.
	*/
	CRefArray Subset( const CRefArray& in_array ) const;

	private:
	Filter * operator&() const;
	Filter * operator&();
};

};

#endif // __XSIFilter_H__
