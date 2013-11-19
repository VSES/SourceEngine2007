//*****************************************************************************
/*!
   \file xsi_project.h
   \brief Project class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIPROJECT_H__
#define __XSIPROJECT_H__

#include <xsi_siobject.h>

namespace XSI {

class Layer;
class Pass;
class Scene;

//*****************************************************************************
/*! \class Project xsi_project.h
	\brief The Project object represents the active XSI Project. It exposes information like the
	current scene (see Project::GetActiveScene).

	\sa Scene, Application
 */
//*****************************************************************************

class SICPPSDKDECL Project : public SIObject
{
public:
	/*! Default constructor. */
	Project();

	/*! Default destructor. */
	~Project();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	Project(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	Project(const Project& in_obj);

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
	\return The new Project object.
	*/
	Project& operator=(const Project& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new Project object.
	*/
	Project& operator=(const CRef& in_ref);

	/*! Return the currently active Scene object.
	\return The currently active scene.
	*/
	Scene 		GetActiveScene() const;

	/*! Not-implemented.
	\return Array of Scene objects (not implemented).
	*/
	CRefArray 	GetScenes() const;

	/*! Returns an array of reference to the properties installed on the Project.
	\return Array of references to Property objects.
	*/
	CRefArray	GetProperties() const;

	/*! Returns the path of the project.
	\return CString Path of the project.
	\since 5.0
	*/
	CString GetPath() const;

	private:
	Project * operator&() const;
	Project * operator&();

};

};

#endif // __XSIPROJECT_H__
