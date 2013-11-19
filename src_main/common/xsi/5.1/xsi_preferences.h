//*****************************************************************************
/*!
   \file xsi_preferences.h
   \brief Preferences class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPREFERENCES_H__
#define __XSIPREFERENCES_H__

#include <xsi_value.h>
#include <xsi_siobject.h>

namespace XSI {

//*****************************************************************************
/*! \class Preferences xsi_preferences.h
	\brief The Preferences object can be used to manage preferences in XSI.

	Each preference is in a category which is represented by a node under the Preferences container in the
	Application. To identify a preference the category must be specified as \c category.preference. For
	example, \c "scripting.language".

	There are 2 kinds of preferences: native and custom. Native preferences come with XSI.
	They can be viewed with the File->%Preferences dialog. By default, no preferences file is created
	until you change the value of a preference, thereby changing the location of preference to \c user.
	When XSI starts it reads all \c .xsipref files located under the \c /Data/Preferences directory
	of the Factory, Workgroup and User locations (in that order), so if the value exists in both the factory
	and the user locations, the value in the user file will be used.

	\note Each time a user changes the value of a preference it is saved in the .xsipref under the user location.

	For custom preferences, you can create new preferences in a \c .xsipref file and you can use them in
	scripting. All loaded preferences are listed on the \b Preferences property page. You can also create a
	new category with UI that appears under the Preferences node.

	\since 4.0

   	\eg
	\code
		using namespace XSI;
		Application app;
		Preferences preferences = app.GetPreferences();

		CString languagepref ;
		preferences.GetPreferenceValue( L"scripting.language", languagepref );

		app.LogMessage( languagepref );

	\endcode

	\eg
	\code
		using namespace XSI;
		Application app;
		Preferences preferences = app.GetPreferences();

		CRefArray l_array;
		l_array = prefMgr.GetCategories();

		for (int i=0; i< l_array.GetCount(); i++)
		{
			if ( l_array[i].GetName() == L"scripting")
			{

			}
		}

	\endcode
 */
//*****************************************************************************
class SICPPSDKDECL Preferences : public SIObject
{
public:
	/*! Default constructor. */
	Preferences();

	/*! Default destructor. */
	~Preferences();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Preferences(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Preferences(const Preferences& in_obj);

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
	\return The new Preferences object.
	*/
	Preferences& operator=(const Preferences& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Preferences object.
	*/
	Preferences& operator=(const CRef& in_ref);

	/*! Returns the categories collection.
	\return The categories array. This is a list of Property objects which hold a set of preferences.
	*/
	CRefArray GetCategories( ) const;

	/*! Returns the value of the specified preference.
	\param in_sPrefName The name of the preference including the category: \c category.preference name
	\retval out_value The value returned.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus GetPreferenceValue( const CString &in_sPrefName, CValue &out_value);

	/*! Sets the value of the specified preference.
	\param in_sPrefName The name of the preference including the category: \c category.preference name
	\param in_value The value to set
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus SetPreferenceValue( const CString &in_sPrefName, const CValue &in_value);

	/*! Imports a set of preferences into %XSI. All values are saved in the .xsipref under the user location.
	The file can contain preferences from any category. The file must be in the .xsipref file format.
	\param in_sFile The file to import.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Import( const CString &in_sFile );

	/*! Exports one or more categories of preferences into a .xsipref file.
	\param in_sFile The file to export.
	\param in_sCategories The categories to export
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus Export( const CString &in_sFile, const CString &in_sCategories );

	/*! The category or the preference to restore to the default value.
	It can also be a specific preference.
	\param in_sPrefName The category or preference to restore.
	\return CStatus::OK success
	\return CStatus::Fail failure
	*/
	CStatus RestoreDefault( const CString &in_sPrefName );

	private:
	Preferences * operator&() const;
	Preferences * operator&();
};

};

#endif // __XSIPREFERENCES_H__
