//*****************************************************************************
/*!
   \file xsi_null.h
   \brief Null class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSINULL_H__
#define __XSINULL_H__

#include <xsi_x3dobject.h>

namespace XSI {

class Material;

//*****************************************************************************
/*! \class Null xsi_null.h
	\brief The Null object is a specialized X3DObject object and represents a point
	in space with no dimensions. Nulls can be used to set constraints,
	organize objects into hierarchies, and so on.

	\sa Light

	\eg
	\code
		using namespace XSI;
		Application app;
		Model root = app.GetActiveSceneRoot();

		Null myNull;
		root.AddNull( L"MyNull", myNull );

		app.LogMessage( myNull.GetFullName() );
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL Null : public X3DObject
{
public:
	/*! Default constructor. */
	Null();

	/*! Default destructor. */
	~Null();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Null(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Null(const Null& in_obj);

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
	\return The new Null object.
	*/
	Null& operator=(const Null& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Null object.
	*/
	Null& operator=(const CRef& in_ref);

	private:
	Null * operator&() const;
	Null * operator&();
};

};

#endif // __XSINULL_H__
