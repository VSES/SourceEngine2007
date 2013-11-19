//*****************************************************************************
/*!
   \file xsi_siobject.h
   \brief SIObject class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISIOBJECT_H__
#define __XSISIOBJECT_H__

#include <xsi_base.h>

namespace XSI {

class Application;

//*****************************************************************************
/*! \class SIObject xsi_siobject.h
	\brief SIObject derives from CBase and acts as the base class for the %XSI Object %Model library. Unlike
	CBase, this class provides some high-level services such as object naming. It also provides accessors to
	the object parent and to the Application object.

	\sa Application

	\eg Converting between SIObject and CRef
	\code
		using namespace XSI;

		Application app;

		SIObject siObj(app);
		CString strName(L"MyName");
		siObj.PutName( strName );
		app.LogMessage( CValue(siObj.GetFullName() == strName).GetAsText() );
		app.LogMessage( CValue(siObj.GetName() == strName).GetAsText() );
		app.LogMessage( CValue(siObj.GetUniqueName() == strName).GetAsText() );
		app.LogMessage( CValue(siObj.GetType() == strName).GetAsText() );

		SIObject siObjCopy(siObj);
		app.LogMessage( CValue(siObj.IsEqualTo(siObjCopy)).GetAsText() );

		CRef refParent = siObj.GetParent();

		Command cmd;
		SIObject siObjCmd(cmd);
		SIObject siObjCmd2(siObjCmd);
		siObjCmd2.IsEqualTo(siObjCmd);

		SIObject siObjApp(app);
		CRef ref(siObjApp.GetRef());
		SIObject siObjFromRef(ref);
		siObjFromRef.IsEqualTo(ref);
	\endcode
 */
//*****************************************************************************

class SICPPSDKDECL SIObject : public CBase
{
public:
	/*! Default constructor. */
	SIObject();

	/*! Default destructor. */
	~SIObject();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	SIObject(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	SIObject(const SIObject& in_obj);

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
	\return The new SIObject object.
	*/
	SIObject& operator=(const SIObject& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new SIObject object.
	*/
	SIObject& operator=(const CRef& in_ref);

	/*! Returns the Application object.
	\return The XSI Application object.
	*/
	Application	GetApplication() const;

	/*! Returns the full name of the object.
	\return The full name of the object.
	*/
	CString	GetFullName() const;

	/*! Returns the name of the object.
	\return The name of the object.
	*/
	CString	GetName() const;

	/*! Sets the name of the object.
	\param in_name new name for the object.
	\return CStatus::OK success
	\return CStatus::Fail failure.
	*/
	CStatus	PutName(const CString& in_name);

	/* \deprecated Provided for backward compatibility only,
	*/
	CStatus	PutName(CString& in_name);

	/*! Returns the unique name of an object
	\return The unique name of the object.
	*/
	CString	GetUniqueName() const;

	/*! Returns the object type.
	\return The object type.
	*/
	CString	GetType() const;

	/*! Returns a reference  to the parent of the object. The objects that have no parent, like Application,
	return themselves as parent. If this method is called from the Update() context of a custom operator
	it will return an empty CRef.
	\return A reference to the parent of this object.
	*/
	CRef	GetParent() const;

	/*! Compares this object with a specified object and indicates whether they are the same object or not.
	\param in_ref The object to compare with .
	\return true if its equal, false otherwise.
	*/
	bool	IsEqualTo( const CRef& in_ref) const;

	/*! Returns the help file for this object.
	\return The full file path for the help file.
	\since 4.0
	*/
	CString GetHelp() const;

	/*! Returns the location path where this object is defined.
	\return The full path where this object is defined.
	\since 4.0
	*/
	CString GetOriginPath() const;

	/*! Returns the location path where this object is defined.
	\return Installation type.
	\since 4.0
	*/
	siInstallationPath GetOrigin() const;

	/*! Returns an array of user-defined categories.
	\return Array of categories (strings).
	\since 4.0
	*/
	CStringArray GetCategories() const;

	/*! Returns an array of objects nested under this object.
	\return Array of nested objects.
	\since 5.0.

	\eg This example traverses the hierarchy of a selected object and logs
		recursively all its nested objects.

	\code
		using namespace XSI;

		// declare function prototypes
		void VisitNestedObjects( SIObject& in_obj, LONG in_indent );
		void LogNestedObject(SIObject& in_obj, LONG in_indent )

		// iterates over the nested objects	of the selected object
		Application app;
		Selection sel = app.GetSelection();
		SIObject selObject(sel[0]);
		VisitNestedObjects( selObject, 0 );

		void VisitNestedObjects( SIObject& in_obj, LONG in_indent )
		{
			LogNestedObject(in_obj, in_indent);

			CRefArray nestees( in_obj.GetNestedObjects() );
			for (LONG i=0; i<nestees.GetCount(); i++)
			{
				SIObject obj(nestees[i]);
				VisitNestedObjects( obj, in_indent+1 );
			}
		}

		void LogNestedObject(SIObject& in_obj, LONG in_indent )
		{
			CString str(in_obj.GetClassIDName());
			str += L":";
			str += in_obj.GetName();

			if (in_obj.IsA(siParameterID))
			{
				str += L":";
				Parameter paramObj(in_obj);

				str += CString(paramObj.GetValue());
			}

			const char* pStr = str.GetAsciiString();
			char szBuffer[256];
			szBuffer[255] = '\0';
			sprintf( szBuffer, "%*c%s", in_indent, ' ', pStr );

			str.PutAsciiString( szBuffer );

			Application app;
			app.LogMessage( str );
		}
	\endcode
	*/
	CRefArray GetNestedObjects() const;

	private:
	SIObject * operator&() const;
	SIObject * operator&();
};

};

#endif // __XSISIOBJECT_H__
