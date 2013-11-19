//*****************************************************************************
/*!
   \file xsi_filereference.h
   \brief FileReference class declaration.

   © Copyright 1998-2003 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************
#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIFILEREFERENCE_H__
#define __XSIFILEREFERENCE_H__

#include <xsi_siobject.h>

namespace XSI {

class CValue;
//*****************************************************************************
/*! \class FileReference xsi_filereference.h
	\brief The %FileReference class represents a reference to a file on disk.

	When the user specifies the location of a file in a property page (for example, specifying the image file to use
	on a texture map) the string location is represented by the %FileReference. This class uses two levels of path:
		\li the user path
		\li the resolved path

	You can access the user path using the FileReference::GetPath function and it can contain environment variables
	and be relative to a project.

	You can access the resolved path using either FileReference::GetResolvedPath or the FileReference::GetUNCPath
	which represents the actual full path (as a local or \xl UNCPath).

	\sa Model::GetExternalFiles, Scene::GetExternalFiles

	\since 5.0
 */
//*****************************************************************************
class SICPPSDKDECL FileReference : public SIObject
{
public:
	/*! Default constructor. */
	FileReference();

	/*! Default destructor. */
	~FileReference();

	/*! Constructor.
	\param in_ref constant reference object.
	*/
	FileReference(const CRef& in_ref);

	/*! Copy constructor.
	\param in_obj constant class object.
	*/
	FileReference(const FileReference& in_obj);

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
	\return The new FileReference object.
	*/
	FileReference& operator=(const FileReference& in_obj);

	/*! Creates an object from a reference object. The newly created object is
	set to empty if the input reference object is not compatible.
	\param in_ref constant class object.
	\return The new FileReference object.
	*/
	FileReference& operator=(const CRef& in_ref);

	/*! Returns the name of the item.
	\return The resolved path of the file (see FileReference::GetResolvedPath).
	*/
	CString GetName() const ;

	/*! Returns the user path of the file object. The user path can contain environment variables specified
	with either \c $ or \c \% (for example, \c $USERNAME on Linux and \c \%USERNAME% on Windows).

	This function supports relative paths (relative to the project, the factory or the user location).
	\return The user path.
	\sa \xl Linktab
	*/
	CString GetPath();

	/*! Sets the user path.  The user path can contain environment variables specified
	with either \c $ or \c \% (for example, \c $USERNAME on Linux and \c \%USERNAME% on Windows).

	This function supports relative paths (relative to the project, the factory or the user location).
	\param in_Path The user path to set.
	\return Success or failure of the operation
	\sa \xl Linktab
	*/
	CStatus PutPath(const CString &in_Path);

	/*! Returns the type of file. The different types include models, pictures, audio, actions, scripts,
	simulation, queries, and synoptic.
	\note The type is also used as the directory name when copying files locally to the project.
	\return The file type.
	*/
	CString GetFileType() const;

	/*! Returns the resolved path of this file object. If the path is on a local drive or is on a shared network
	resource mapped to a drive letter, the local path or drive letter is returned; otherwise, the \xl UNCPath
	is returned.
	\note In the case of an image sequence, the resolved path will not be a valid file but rather the symbolic
	name for the sequence. For example: \code		c:\images\mysequence.[1..10].pic \endcode
	\tip To get a specific file use FileReference::GetFileInSequence and FileReference::GetNumberOfFilesInSequence.
	\return The resolved path.
	*/
	CString GetResolvedPath() const;

	/*! Checks whether the file is valid or not.
	\return True if the file exists
	*/
	bool FileExists() const;

	/*! Returns the resolved path of this file object. If the path is on a local drive, the local path is returned;
	otherwise, the \xl UNCPath is returned.
	* \return The \xl UNCPath.
	*/
	CString GetUNCPath()const;

	/*! Returns the GUID of the path. The GUID is a unique identifier for the dynamic life of a path. It should be
	used to retrieve a handle on the path object when dealing with the external file list.
	\note The GUID may change if the filename changes or when reloading the scene.
	\return The GUID as a CString.
	*/
	CString GetGUID()const;

	/*! Returns the list of objects within the scene that reference this file path. Usually there is only one owner:
	the Parameter containing the Source (the audio, image or model file, etc).
	\return Array of references to the owners.
	*/
	CRefArray GetOwners() const;

	/*! Returns the number of files that this sequence of files contains. If the sequence consists of only one file,
	this property returns 1.

	A file sequence is a set of multiple files with the same base name, such as \c myfile.1.pic, \c myfile.2.pic, etc.
	For example, when rendering to disk, %XSI creates file sequences based on a base filename that you provide in the
	\a Render \a Options property page and when browsing the output folder, %XSI displays file sequences as a single
	entity with an annotation like: \c myfile[1..2].pic.

	\return The number of files in the file sequence.
	\since 5.0
	*/
	ULONG GetNumberOfFilesInSequence()const;

	/*! Returns the resolved path of the specified file in the file sequence.

	A file sequence is a set of multiple files with the same base name, such as \c myfile.1.pic, \c myfile.2.pic, etc.
	For example, when rendering to disk, %XSI creates file sequences based on a base filename that you provide in the
	\a Render \a Options property page and when browsing the output folder, %XSI displays file sequences as a single
	entity with an annotation like: \c myfile.[1..2].pic.

	\param in_iIndex The zero-based index of the desired file.
	\note If the index greater than the number of files in the sequence, an error is returned

	\return The resolved path if the file is not a sequence and the specified index is \c 0
	\return Error if the specified index is greater than \c 0
	\return Empty path and an error if the specified index is out of bounds
	\since 5.0
	*/
	CString GetFileInSequence( ULONG in_iIndex)const;

	private:
	FileReference * operator&() const;
	FileReference * operator&();
};

}

#endif // __XSIFILEREFERENCE_H__
