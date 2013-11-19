//*****************************************************************************
/*!
   \file xsi_base.h
   \brief CBase class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIBASE_H__
#define __XSIBASE_H__

#include "sicppsdk.h"
#include "xsi_decl.h"
#include "xsi_ref.h"
#include "xsi_status.h"
#include "xsi_string.h"

namespace XSI {

//*****************************************************************************
/*! \class CBase xsi_base.h
	\brief CBase is the base class of all API classes and provides access to %XSI objects through generic
	CRef objects. The class defines a runtime type information sytem (RTTI) and CRef attachment support for
	API classes.

	The RTTI is a mechanism that allows API classes to determine the type of reference objects during program
	execution. The API classes must be attached to compatible CRef objects in order to operate on %XSI objects
	(CRef). A CRef object can only map to one %XSI object at a time, an API class can only have one API class
	type. However, a CRef object can be compatible with many API classes. This compatibility model follows the
	hierarchy class, thus an CRef object of type ::siPolygonMeshID would be compatible with PolygonMesh,
	Geometry and SIObject API classes.

	CRef objects expose the following functions for querying its underlying object class type and to determine
	its compatibility with a given API class: CRef::GetClassID and CRef::IsA.

	CBase provides functions similar to CRef to determine if an API class is compatible with a given %XSI object
	type or a specific CRef instance:

	\code
		virtual bool IsA( siClassID in_ClassID ) const;
		bool IsA( CRef& in_ref ) const;
	\endcode

	All API classes derive from CBase class. The virtual CBase::IsA function must be implemented by the derived classes.

	\eg Demonstrates the use of the RTTI system by walking through a list of objects and getting the
		geometry of each primitive.

	\code
		using namespace XSI;

		// Forward declaration
		void GetSomeObjects( CRefArray& io_array, const Application& in_app );


		Application app;

		CRefArray array;
		GetSomeObjects( array, app );

		Geometry geom;
		Primitive prim;
		for ( LONG i=0; i<array.GetCount(); ++i )
		{
			if ( array[i].IsA(siPrimitiveID) )
			{
				prim.SetObject(array[i]);
				geom = prim.GetGeometry();

				app.LogMessage( geom.GetName() + L" is a valid Geometry object: " + CValue(geom.IsValid()).GetAsText() );
				app.LogMessage( L"Number of points on " + geom.GetName() + L": " + CValue(geom.GetPoints().GetCount()).GetAsText() );
			}
		}

		//INFO : PolygonMesh is a valid Geometry object: true
		//INFO : Number of points on PolygonMesh: 8
		//INFO : PolygonMesh is a valid Geometry object: true
		//INFO : Number of points on PolygonMesh: 26


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

	API classes can also accept any kind of object without knowing how to operate on it. In such a case if the
	reference object passed to CBase::SetObject is incompatible then the API class will be put in an invalid state
	and all its functions would return an error.

	\eg Demonstrates how the code continues working even though the reference object is incompatible
	\code
		using namespace XSI;
		Application app;

		CRefArray array;
		GetSomeObjects( array, app );		// use function defined in Example 1

		Geometry geom;
		Primitive prim;
		for (LONG i = 0; i < array.GetCount(); ++i )
		{
			prim.SetObject(array[i]);
			geom = prim.GetGeometry();

			SIObject obj(array[i]);
			app.LogMessage( obj.GetName() + L" is a valid Geometry object: " +
				CValue(geom.IsValid()).GetAsText() );
			app.LogMessage( L"Number of points on " + obj.GetName() + L": " +
				CValue(geom.GetPoints().GetCount()).GetAsText() );
		}
	\endcode

 */
//*****************************************************************************
class SICPPSDKDECL CBase
{
public:
	/*! Default constructor. */
	CBase();

	/*! Default destructor. */
	virtual ~CBase();

	/*! Returns the type of the API class in a text string,
	\return The class type in a text string..
	*/
	CString GetClassIDName() const;

	/*! Returns the type of the API class.
	\return The class type.
	*/
	virtual siClassID GetClassID() const = 0;

	/*! Returns true if a given class type is compatible with this API class.
	\param in_ClassID Class type.
	\return True if the class is compatible, false otherwise.
	*/
	virtual bool IsA( siClassID in_ClassID ) const;

	/*! Returns true if a given CRef object is compatible with this API class.
	\param in_ref CRef object.
	\return True if the class is compatible, false otherwise.
	*/
	bool IsA( const CRef& in_ref ) const;

	/*! Sets this API class object with a reference object.
	\param in_obj CRef object.
	\return CStatus::OK success
	\return CStatus::Fail Failure
	*/
	CStatus SetObject( const CRef& in_obj );

	/*! Returns true if the object is valid.
	\return True of the object is valid; false otherwise.
	*/
	bool IsValid() const;


	/*! Resets this API class by deleting its CRef object.
	*/
	void ResetObject();

	/*! Converts from CBase to a CRef reference object..
	\return A reference to a CRef object.
	*/
	operator CRef&();


	/*! Equality operator
	\param in_obj API class object to compare.
	\return true if both API class objects are identical, false otherwise.
	\since 4.0
	*/
	bool operator == ( const CBase& in_obj ) const;

	/*! Inequality operator
	\param in_obj API class object to compare.
	\return true if the objects are not identical, false otherwise.
	\since 4.0
	*/
	bool operator != ( const CBase& in_obj ) const;

	/*! Converts from CBase to a constant CRef reference object.
	\return A constant reference to a CRef object.
	*/
	const CRef& GetRef() const;

protected:
	void	SetPtr( void* );
	void	SetRef( const CRef& in_ref );
	void	Clear();
	void*	GetPtr(void) const;

private:
	CBase & operator=( const CBase & );
	CBase * operator&() const;
	CBase * operator&();

	CRef m_ref;
};

};

#endif // __XSIBASE_H__




