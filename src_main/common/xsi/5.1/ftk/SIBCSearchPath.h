//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCSearchPath.h | Main header file for CSIBCSearchPath implementation
//***************************************************************************************

/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE .

COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __CSIBCSearchPath_H__
#define __CSIBCSearchPath_H__

//***************************************************************************************
// Includes
//***************************************************************************************
#include <SIBCArray.h>
#include <SIBCString.h>
#include <SIBCUtil.h>

//***************************************************************************************
// CSIBCSearchPath | deals with paths and filenames
//***************************************************************************************

//! Class dealing with paths and filenames.
/*!	This class provides an easy method for locating files within designated search paths
*/

class XSICOREEXPORT CSIBCSearchPath
{
	public:

		/*!	Default Constructor.
			Initializes a CSIBCSearchPath object.
		*/	
		CSIBCSearchPath();

		~CSIBCSearchPath();

		/*! Adds a search path to this object. Subsequent calls to CSIBCSearchPath::Search,
			and CSIBCSearchPath::GetFiles will look in this path for matching files.
			\param	in_Path	String containing the full path.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The search path was added successfully.
			</ul>

			\sa CSIBCSearchPath::GetPaths
			\sa CSIBCSearchPath::ClearPaths
		*/
		SI_Error AddPath( CSIBCString & in_Path);

		/*!	Clears all search paths associated with this object.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The search path was added successfully.
			</ul>

			\sa CSIBCSearchPath::AddPath
			\sa CSIBCSearchPath::GetPaths
		*/
		SI_Error ClearPaths();

		/*! Searches for a file named \p file, in the search paths added with CSIBCSearchPath::AddPath.
			\param	file		Filename to search for.
			\param	resultpath	Contains the full path and filename of the file, if found.
			\return	SI_Error	Error code indicating the success or failure of this command.

			<ul>Possible returns:
				<li>SI_SUCCESS - The file was located. \p resultpath contains the path and filename.
				<li>SI_FILE_NOT_FOUND - The file could not be found (\p resultpath is unchanged).
			</ul>

			\sa CSIBCSearchPath::GetFiles
			\sa CSIBCSearchPath::AddPath
			\sa CSIBCSearchPath::GetPaths
		*/
		SI_Error Search( CSIBCString &file, CSIBCString &resultpath );

		/*! Returns an array of all files matching \p in_Filter in the search paths for this object.
			This function will also return any directories matching the search filter.
			\param	in_Filter	The filename to search for (can include wildcards).
			\param	out_pFiles	Pointer to an array of strings to append the full paths and filenames
					of files matched to \p in_Filter in this object's search paths.	The strings added
					to this array must be freed by the caller.

			<ul>Possible returns:
				<li>SI_SUCCESS - The command was successful.
			</ul>

			\sa CSIBCSearchPath::GetDirectories
			\sa CSIBCSearchPath::Search
			\sa CSIBCSearchPath::AddPath
			\sa CSIBCSearchPath::GetPaths
		*/
		SI_Error GetFiles( CSIBCString &in_Filter, CSIBCArray < CSIBCString * >	*out_pFiles );

		
		/*! Function exactly like GetFiles, except that only directories matching the filter are returned.
			\param	in_Filter	The filename to search for (can include wildcards).
			\param	out_pFiles	Pointer to an array of strings to append the full paths of directories 
					matched to \p in_Filter in this object's search paths.	The strings added
					to this array must be freed by the caller.

			<ul>Possible returns:
				<li>SI_SUCCESS - The command was successful.
			</ul>

			\sa CSIBCSearchPath::GetFiles
			\sa CSIBCSearchPath::Search
			\sa CSIBCSearchPath::AddPath
			\sa CSIBCSearchPath::GetPaths
		*/
		SI_Error GetDirectories( CSIBCString& in_Filter, CSIBCArray < CSIBCString * > * out_pDirs );
			

		/*! Returns the array containing the search paths defined for this object.
			\return	CSIBCArray<CSIBCString*>&	Reference to the array containing the search paths
					for this object.
			\sa CSIBCSearchPath::AddPath
			\sa CSIBCSearchPath::ClearPaths
		*/
		CSIBCArray < CSIBCString * > &GetPaths() { return m_Paths; };	// Get the paths


	protected:


	private:
		CSIBCArray < CSIBCString * >	m_Paths;
		SI_Void							*m_pUnused;

};


#endif // CSIBCSearchPath
