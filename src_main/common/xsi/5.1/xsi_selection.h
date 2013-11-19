//*****************************************************************************
/*!	\file xsi_selection.h
 	\brief Selection class declaration.

	© Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
	reserved. This file contains confidential and proprietary information of
	Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
	end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISELECTION_H__
#define __XSISELECTION_H__

#include <xsi_base.h>

namespace XSI {

class Filter;

//*****************************************************************************
/*! \class Selection xsi_selection.h
	\brief The selection object represents the global selection for the application.

	The user can change the global selection in the views, explorer, MCA (selection sub-panel in the main control
	panel), and through script commands (for example, see the \xl SelectObj, \xl AddToSelection,
	\xl RemoveFromSelection, \xl ToggleSelection, and \xl DeselectAll, commands etc.).

	\note Selection provides access to its members through Selection::GetItem which returns a reference to the
		specified item. For example, whether you select the whole sphere or only a subcomponent (like an edge),
		you get a CRef.

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		// creates some objects and adds them to the selection list
		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"myCube", myCube );

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"NurbsSurface", L"myGrid", myGrid );

		X3DObject myCylinder;
		root.AddGeometry( L"Cylinder", L"MeshSurface", L"myCylinder", myCylinder );

		Property myProp;
		myCylinder.AddProperty(L"Display Property", false, L"MyDisplayProp", myProp);

		Selection selection( app.GetSelection() );

		selection.Add( myCube );
		selection.Add( myGrid );
		selection.Add( myProp );

		// iterate over the selection
		LONG lCount = selection.GetCount();
		app.LogMessage( L"Selection count: " + CValue(lCount).GetAsText() );

		for (LONG i=0; i<lCount; i++ )
		{
			SIObject obj(selection[i]);
			app.LogMessage( L"Selection " + CValue(i).GetAsText() +
				L"= " + obj.GetName() );
		}

		// enumerate all polygon meshes
		CRefArray array(selection.GetArray());

		CRefArray meshRefArray;
		array.Filter(siPolyMeshType, CStringArray(), L"", meshRefArray );

		lCount = meshRefArray.GetCount();
		for (i=0; i<lCount; i++ )
		{
			SIObject obj(meshRefArray[i]);
			app.LogMessage( L"Mesh "+CValue(i).GetAsText()+
							L"= "+obj.GetName() );
		}

		// find the selected display property
		CRef propRef;
		array.Find( siDisplayType, propRef );

		SIObject obj(propRef);
		app.LogMessage( L"Display Property = " + obj.GetName() );
	\endcode

 */
//*****************************************************************************

class SICPPSDKDECL Selection : public CBase
{
public:
	/*! Default constructor. */
	Selection();

	/*! Default destructor. */
	~Selection();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Selection(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Selection(const Selection& in_obj);

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
	\return The new Selection object.
	*/
	Selection& operator=(const Selection& in_obj);

	/*! Creates an object from a reference object. The new object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Selection object.
	*/
	Selection& operator=(const CRef& in_ref);

	/*! Accesses elements at a given index.
	\param in_index CRef object
	\return A new reference object.
	*/
	CRef operator[]( LONG in_index );

	/*! Returns a CRef object contained in the list given an index number.
	\param in_index A number index
	\return A reference to the specified selection item.
	*/
	CRef GetItem( LONG in_index ) const;

	/*! Returns a CRef object contained in the list given an object string name.
	\param in_name The name of an object to retrieve.
	\return A reference to the specified selection item.
	*/
	CRef GetItem( const CString& in_name ) const;

	/*! Adds a reference to an object to the selection list.
	\param in_object A reference to the object to add.
	\param in_selType Specifies how the object is selected in hierarchy.
	\return CStatus::OK Success
	\return CStatus::Fail Failure
	*/
	CStatus Add( const CRef& in_object,
		siSelectMode in_selType = siSelectDefault );

	/*! Removes a item from the selection list.
	\param in_object A reference to object to remove.
	\param in_selType Specifies how the object is selected in hierarchy (node, branch, tree, model)
	\return CStatus::OK Success
	\return CStatus::Fail Failure
	*/
	CStatus Remove( const CRef& in_object,
		siSelectMode in_selType = siSelectDefault );

	/*! Deselects everything.
	\return CStatus::OK Success
	\return CStatus::Fail Failure
	*/
	CStatus Clear();

	/*! Returns the number of items in the Selection
	\return Number of items
	*/
	LONG GetCount() const;

	/*! Returns a CRefArray containing the selected objects.
	\return The selection list.
	*/
	CRefArray GetArray();

	/*! Sets the selection by parsing a given string.
	\param in_str Text string to parse. The string list is comma-separated. You may use wildcards in
		the string to match many object for example \c light*
	\return CStatus::OK success
	\return CStatus::Fail The string is either badly formatted or contains unresolvable object names.

	\eg Creates some objects and adds them to the selection list.
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"myCube", myCube );

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"NurbsSurface", L"myGrid", myGrid );

		X3DObject myCylinder;
		root.AddGeometry( L"Cylinder", L"MeshSurface", L"myCylinder", myCylinder );

		Selection sel = app.GetSelection();

		CString str;
		str = L"myCube";
		str += L",";
		str += L"myGrid";
		str += L",";
		str += L"myCylinder";

		sel.SetAsText( str );

		LONG lCount = sel.GetCount();
		app.LogMessage( L"Selection count: " + CValue(lCount).GetAsText() );
		//'INFO : "Selection count: 3"

		for (LONG i=0; i<lCount; i++ )
		{
			SIObject obj(sel[i]);
			app.LogMessage( L"Selection " + CValue(i).GetAsText() + L"= " +
				obj.GetFullName() );
		}

		//'INFO : "Selection 0= myCube"
		//'INFO : "Selection 1= myGrid"
		//'INFO : "Selection 2= myCylinder"
	\endcode
	*/
	CStatus SetAsText( const CString& in_str );

	/*! Returns the contents of the selection as a readable string.
	\return A readable string or an empty value if the string can't be expressed as a readable string.

	\eg Creates some objects and adds them to the selection list
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		X3DObject myCube;
		root.AddGeometry( L"Cube", L"MeshSurface", L"myCube", myCube );

		X3DObject myGrid;
		root.AddGeometry( L"Grid", L"NurbsSurface", L"myGrid", myGrid );

		X3DObject myCylinder;
		root.AddGeometry( L"Cylinder", L"MeshSurface", L"myCylinder", myCylinder );

		Selection sel = app.GetSelection();

		sel.Add( myCube );
		sel.Add( myGrid );
		sel.Add( myCylinder );

		CString strSel = sel.GetAsText();

		app.LogMessage( L"Selection as text: " +  strSel );
		// 'INFO : "Selection as text: myCube,myGrid,myCylinder"
	\endcode
	*/
	CString GetAsText();

	/*! Returns the currently selected filter from the selection.
	\return Current Filter object.
	\since 4.0

	\eg
	\code
		using namespace XSI;
		Application app;
		Selection sel = app.GetSelection();
		Filter currentFilter = sel.GetFilter();

		app.LogMessage( L"Selected filter: " +  currentFilter.GetName() );

	\endcode
	*/
	Filter GetFilter() const;

	private:
	Selection * operator&() const;
	Selection * operator&();
};

};

#endif // __XSISELECTION_H__
