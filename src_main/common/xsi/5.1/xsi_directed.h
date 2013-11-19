//*****************************************************************************
/*!
   \file xsi_directed.h
   \brief Directed class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIDIRECTED_H__
#define __XSIDIRECTED_H__

#include <xsi_x3dobject.h>
#include <xsi_status.h>

namespace XSI {

//*****************************************************************************
/*! \class Directed xsi_directed.h
	\brief An object representing the interest of a Camera or Light.

	A Directed object
	cannot be created directly, it acts as a X3DObject class for Camera and Light objects.

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Light spot;
		root.AddLight( L"Spot", true, L"MySpotLight", spot );
		X3DObject interest = spot.GetInterest();
		app.LogMessage( CString(L"interest: ") + interest.GetName() );

		X3DObject myCone;
		root.AddGeometry( L"Cone", L"MeshSurface", L"", myCone );
		spot.PutInterest( myCone );
		interest = spot.GetInterest();
		app.LogMessage( L"interest: " + interest.GetName() );
	\endcode
*/
//*****************************************************************************

class SICPPSDKDECL Directed : public X3DObject
{
public:
	/*! Default constructor. */
	Directed();

	/*! Default destructor. */
	~Directed();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Directed(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Directed(const Directed& in_obj);

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
	\return The new Directed object.
	*/
	Directed& operator=(const Directed& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Directed object.
	*/
	Directed& operator=(const CRef& in_ref);

	/*! Returns the object's interest.
	\return The new interest object.

	\eg
	\code
		 using namespace XSI;
		 Application app;

		 Model root = app.GetActiveSceneRoot();
		 Light spot = root.AddLight( CString(L"Spot"), true, CString(L"MySpotLight");

		 X3DObject interest = spot.GetInterest();
		 app.LogMessage( CString(L"interest: ") + oInterest.Name() );

		 X3DObject myCone = root.AddGeometry( CString(L"Cone"), CString(L"MeshSurface"),
		 spot.PutInterest( myCone );

		 interest = spot.GetInterest();
		 app.LogMessage( CString(L"interest: ") + oInterest.Name() );
	 \endcode
	*/
	X3DObject GetInterest() const;

	/*! Sets the object's interest.
	\param in_obj The new interest object
	\return CStatus::OK Success
	\return CStatus::Fail Failure
	*/
	CStatus	PutInterest(const X3DObject& in_obj );

	private:
	Directed * operator&() const;
	Directed * operator&();
};

};

#endif // __XSIDIRECTED_H__
