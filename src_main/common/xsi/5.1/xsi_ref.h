//*****************************************************************************
/*!
   \file xsi_ref.h
   \brief XSI namespace, CRef class, and CRefArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIREF_H__
#define __XSIREF_H__


#include "sicppsdk.h"
#include "xsi_string.h"
#include "xsi_decl.h"

/*! \namespace XSI
	\brief The XSI namespace defines a region where all API classes are declared.
	Namespaces are used to prevent global namespace pollution and name clashing.

	All API classes defined in the C++ API are assigned to the XSI namespace.
	Members of the XSI namespace can be identified explicitly with the symbol
	XSI for code clarity. To simplify the code however, namespace members can be
	referenced without the namespace symbol by using the C++ using directive.

	\eg Demonstrates use of namespace
	\code
		// namespace qualification
		XSI::CSIObject myObj;

		// CSIObject referenced without explicit qualification
		using namespace XSI;
		CSIObject myObj;
	\endcode
*/
namespace XSI {

class CStatus;
class CValueArray;

//*****************************************************************************
/*! \class CRef xsi_ref.h
	\brief This class is used to reference XSI objects. Each instance of an CRef
	maintains a link with only one single object at a time, it cannot refer to
	multiple objects. CRef has no base or derived class hierarchy and only
	contains methods for accessing the type of the underlying XSI object.

	CRef provides the core of the RTTI system to determine if an object is
	of a specific type, for example, to ensure that the compatibility between
	the reference and a given API class is valid by checking it against the
	siClassID enum. CRef instances are typically attached to API classes for
	manipulating XSI objects.

	A CRef object can be converted to its corresponding %XSI COM object, the
	resulting object can be used with the %XSI COM API. The inverse is also
	possible, you can create a CRef object from an existing %XSI COM object.
	See the XSI::ConvertObject functions for more details.

	\sa CRefArray, CBase

	\eg Demonstrates how to use the work with CRefs
	\code
		using namespace XSI;

		// Forward declaration
		void GetSomeObjects( CRefArray& io_array, const Application& in_app );


		Application app;

		CRefArray array;
		GetSomeObjects( array, app );

		// Find all X3DObjects in the scene and add them to the ref array
		CStringArray emptyArray;
		Model root( app.GetActiveSceneRoot() );

		array += root.FindChildren( L"", L"", emptyArray, true );

		// Add grid's parameters to ref array
		X3DObject grid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", grid );

		array += grid.GetParameters();

		for (LONG i=0; i<array.GetCount(); ++i )
		{
			app.LogMessage(L"");

			CRef ref(array[i]);
			app.LogMessage( CString(L">>Reference object class type: ") + ref.GetClassIDName() );

			if ( ref.IsA( siSIObjectID ) )
			{
				SIObject siobj(ref);
				app.LogMessage( CString(L"\tObject name: ") + siobj.GetName() );
			}

			if ( ref.IsA( siX3DObjectID ) )
			{
				X3DObject xobj(ref);
				app.LogMessage( CString(L"\tNumber of children : ") +
								CValue(xobj.GetChildren().GetCount()).GetAsText() );
			}

			if ( ref.IsA( siParameterID ) )
			{
				Parameter param(ref);
				app.LogMessage( CString(L"\tParameter's value : ") +
								param.GetValue().GetAsText() );
			}
		}


		// Fills an array with CRef objects
		void GetSomeObjects( CRefArray& io_array, const Application& in_app )
		{
			X3DObject myCube;
			X3DObject myCone;
			Material myPhong;

			Model root = in_app.GetActiveSceneRoot();

			root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );
			root.AddGeometry( L"Cone", L"MeshSurface", L"", myCone );
			myCone.AddMaterial(	L"Phong", false, L"", myPhong );

			// Add CRef items to the input array
			io_array.Add( myCube.GetActivePrimitive().GetRef() );
			io_array.Add( myCone.GetActivePrimitive().GetRef() );
			io_array.Add( myPhong.GetRef() );
		}

	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL CRef
{
public:
	/*! Default constructor. */
	CRef();

	/*! Construct a CRef object from another CRef object.
	\param in_xsiRef constant reference object.
	*/
	CRef(const CRef& in_xsiRef);

	/*! Destructor. */
	~CRef();

	/*! Returns the type of the object being referenced.
	\return The class type.
	*/
	siClassID GetClassID() const;

	/*! Returns true if a given class type is compatible with the underlying
 	reference object.
	\param in_classID Class type (the siClassID enum).
	\return true if the class is compatible, false otherwise.
 	*/
	bool IsA(siClassID in_classID) const;

	/*! Returns the type of the referenced object in text format.
	\return The text string.
	*/
	CString GetClassIDName() const;

	/*! Returns true if the referenced object is valid or false otherwise.
	\return true if valid, false otherwise.
	*/
	bool IsValid() const;

	/*! Bool converter operator that returns true if the object is valid or
	false otherwise.
	\return true if object is valid; false if not valid.
	\since 4.0
	*/
	operator bool() const;

	/*! Equality operator
	\param in_ref Reference object to compare.
	\return true if both CRef objects are identical, false otherwise.
	*/
	bool operator == (const CRef& in_ref) const;

	/*! Inequality operator
	\param in_ref Reference object to compare.
	\return true if objects are not identical, false otherwise.
	*/
	bool operator != (const CRef& in_ref ) const;

	/*! Assigns a CRef object to this one.
	\param in_ref A source Reference object
	\return A new reference object.
	*/
	CRef& operator = (const CRef& in_ref );

	/*! Sets CRef from an object's string path. Returns CStatus::Fail if the object's
	path is invalid.
	\param in_str String path to object for example, "Model.null.kine.local.posx"
	\return CStatus::OK success
	\return CStatus::InvalidArgument string is empty or contains invalid object path.
	\return CStatus::Fail failure
	*/
	CStatus Set(const CString& in_str);

	/*! Returns the object's path as a string.
	\return The object's path.
	*/
	CString GetAsText() const;

protected:
	void* GetPtr() const;

private:
	void* m_ptr;
};

//*************************************************************************
/*! \class CRefArray xsi_ref.h
	\brief The CRefArray class holds generic CRef objects and acts as a base class for
	other specialized CRef array classes (CEdgeRefArray, CFacetRefArray, etc...).
	Like CRef class, the CRefArray class also provides a runtime type information
	system to help determining the object type during execution time.

	The following example demonstrates several approaches for iterating the items
	of a CRefArray object. Although all of these methods below are more than
	adequate, the last one is the most effective.

	\eg Demonstrates how to use the CRefArray class
	\code
		using namespace XSI;

		// Forward declaration
		void GetSomeObjects( CRefArray& io_array, const Application& in_app );


		Application app;

		CRefArray array;
		GetSomeObjects( array, app );

		// Find all X3DObjects in the scene and adds them to the ref array
		CStringArray emptyArray;
		Model root = app.GetActiveSceneRoot();

		array += root.FindChildren( L"", L"", emptyArray, true );

		// Add grid's parameters to ref array
		X3DObject grid;
		root.AddGeometry( L"Grid", L"MeshSurface", L"", grid );

		array += grid.GetParameters();

		for ( LONG i=0; i<array.GetCount(); ++i )
		{
			app.LogMessage(L"");

			CRef ref(array[i]);
			app.LogMessage( CString(L">>Reference object class type: ") + ref.GetClassIDName() );

			if ( ref.IsA( siSIObjectID ) )
			{
				SIObject siobj(ref);
				app.LogMessage( CString(L"\tObject name: ") + siobj.GetName() );
			}

			if ( ref.IsA( siX3DObjectID ) )
			{
				X3DObject xobj(ref);
				app.LogMessage( L"\tNumber of children : " +
								CValue(xobj.GetChildren().GetCount()).GetAsText() );
			}

			if ( ref.IsA( siParameterID ) )
			{
				Parameter param(ref);
				app.LogMessage( L"\tParameter's value : " +
								param.GetValue().GetAsText() );
			}
		}



		// Fills an array with CRef objects
		void GetSomeObjects( CRefArray& io_array, const Application& in_app )
		{
			X3DObject myCube;
			X3DObject myCone;
			Material myPhong;

			Model root = in_app.GetActiveSceneRoot();

			root.AddGeometry( L"Cube", L"MeshSurface", L"", myCube );
			root.AddGeometry( L"Cone", L"MeshSurface", L"", myCone );
			myCone.AddMaterial(	L"Phong", false, L"", myPhong );

			// Add CRef items to the input array
			io_array.Add( myCube.GetActivePrimitive().GetRef() );
			io_array.Add( myCone.GetActivePrimitive().GetRef() );
			io_array.Add( myPhong.GetRef() );
		}

	\endcode
	\sa CEdgeRefArray, CFacetRefArray, CParameterRefArray, CPointRefArray,
		CPolygonFaceRefArray,  CPolygonNodeRefArray, CSampleRefArray, CSegmentRefArray,
		CTriangleRefArray,  CTriangleVertexRefArray, CVertexRefArray
*/
//*************************************************************************
class SICPPSDKDECL CRefArray
{
	public:

	/*! Constructs a CRefArray object and optionally initializes the vector
	to a known size.
	\param in_lSize Size of array, defaults to 0.
	*/
	CRefArray(LONG in_lSize=0);

	/*! Constructs a CRefArray object from another CRefArray object.
	\param in_refArray constant CRefArray object.
	*/
	CRefArray(const CRefArray& in_refArray);

	/*! Default destructor. */
	virtual ~CRefArray();

	/*! Assigns a CRefArray object to this one.
	\param in_refArray A constant CRefArray object.
	\return A new reference object.
	*/
	CRefArray& operator=( const CRefArray& in_refArray );

	/*! Appends the content of a CRefArray object to this one.
	\param in_refArray A constant CRefArray object.
	\return A new reference object.
	*/
	CRefArray& operator+=( const CRefArray& in_refArray );

	/*! Accessor to elements at a given index. This function can only be
	called by constant objects, the returned value is read-only.
	\param in_index constant CRef object
	\return A new reference object.
	*/
	const CRef& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index CRef object
	\return A new reference object.
	*/
	CRef& operator[]( LONG in_index );

	/*! Returns a CRef object contained in the list given an index number.
	\param in_index A number index
	\return A new CRef object.
	*/
	virtual CRef GetItem( LONG in_index ) const;

	/*! Returns a CRef object contained in the list given an object string name.
	\param in_name The name of an object to retrieve.
	\return A new CRef object.
	*/
	virtual CRef GetItem( const CString& in_name ) const;

	/*! Adds a CRef object at the end of this array.
	\param in_item CRef object.
	*/
	virtual void Add( const CRef& in_item );

	/*! Returns the number of items in CRefArray
	\return Size of the array
	*/
	virtual LONG GetCount() const;

	/*! Returns true if a given class type is compatible with the underlying
 	reference object.
	\param in_classID Class type.
	\return true if the class is compatible, false otherwise.
 	*/
	bool IsA(siClassID in_classID) const;

	/*! Returns the class type of this array
	\return siClassID Class type of this array.
	*/
	siClassID GetClassID() const;

	/*! Returns the type of the referenced object in text format.
	\return The text string.
	*/
	CString GetClassIDName() const;

	/*! Returns true if this array is a valid object or false otherwise.
	\return true if valid, false otherwise.
	*/
	virtual bool IsValid() const;

	/*! Bool converter operator that returns true if this array is a valid
	object or false otherwise.
	\return true if object is valid; false if not valid.
	\since 4.0
	*/
	operator bool() const;

	/*! Searches this array and return a new array of CRef objects that match a
	given set of criterias. If no objects are found, the function returns an
	empty array.
	\param in_strType Type of object e.g. ::siPolyMeshType, ::siBendOpType, etc..
	\param in_strFam Array of families e.g. ::siGroupFamily, ::si3DObjectFamily, etc...
	\param in_strObjName Object name
	\param io_array Array of objects found.
	\return CStatus::OK success
	\return CStatus::False Didn't find any objects meeting the criterias.
	\return CStatus::Fail failure
	*/
	CStatus Filter( const CString& in_strType, const CStringArray& in_strFam,
		const CString& in_strObjName, CRefArray& io_array ) const;

	/*! Searches this array and return the first object that matches a given
	type. If no object is found, the method returns an empty object.
	\param in_strType Type of object e.g. siPolyMeshType, siBendOpType, etc..
	\param io_obj Object found.
	\return CStatus::OK success
	\return CStatus::False Didn't find any objects meeting the criterias.
	\return CStatus::Fail failure
	 */
	CStatus Find( const CString& in_strType, CRef& io_obj ) const;

	/*! Sets from a CValueArray where each item can be a CString containing the object's path
	or a CRef object.
	\param in_valarray  string array of objects either as CString (object's path) or CRef objects.
	\return The number of objects set. Returns -1 on failure.
	*/
	LONG Set(const CValueArray& in_valarray);

	/*! Sets from a string array of object paths, where the item separator is a comma.
	If an empty string is passed then a count of 0 is returned.
	\param in_strarray string array of object paths for example, "null,null1,null2"
	\param in_sep single character representing item separator.
	\return The number of objects set. Returns -1 on failure.
	*/
	LONG Set(const CString& in_strarray, const CString& in_sep = L"," );

	/*! Removes a CRef from ref array.
	\param in_ref reference to remove
	\return The number of items removed
	*/
	LONG Remove(const CRef& in_ref );

	/*! Removes all items from ref array.
	 */
	void Clear(void);

	/*! Return object's string path
	 * \return The object's path.
	 */
	CString GetAsText() const;

	/*! Reallocates memory for the array, preserves its contents if new size is larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	\since 5.0
	*/
	CStatus Resize( LONG in_size );

	protected:
	void SetPtr( void* p=NULL, LONG id=siUnknownClassID );
	void* GetPtr() const;
	LONG GetID()const;

	private:
	CRefArray * operator&() const;
	CRefArray * operator&();

	LONG	m_id;
	mutable void*	m_ptr;
};

/*! Converts an %XSI Automation object to a CRef object.

\note Normally %XSI will automatically perform this conversion. For example when calling a custom
	Command implemented in C++ from scripting, any arguments which are objects will automatically
	be converted to the corresponding C++ API CRef-based object.

\param in_pAuto IDispatch pointer to a COM Automation object (see CValue::DataType)
\return The new CRef object
*/
SICPPSDKDECL CRef ConvertObject( void* in_pAuto );

/*! Converts a CRef object to an %XSI Automation object.  The returned pointer can be used to
communicate with the scripting version of the Object %Model.

The returned object pointer has been reference counted, and the caller is responsible for
calling IDispatch::Release().

\note The CComAPIHandler object is available as an alternative way to communicate with %XSI
	Automation objects.

\param in_obj A constant CRef object to convert.
\return IDispatch pointer to an XSI Automation object (see CValue::DataType).
*/
SICPPSDKDECL void* ConvertObject( const CRef& in_obj );

};

#endif // __XSIREF_H__
