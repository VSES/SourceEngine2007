//*****************************************************************************
/*!
   \file xsi_utils.h
   \brief Utils class declaration.

   © Copyright 1998-2005 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSIUTIL_H__
#define __XSIUTIL_H__

//*****************************************************************************
//
// Defines
//
//*****************************************************************************

//*****************************************************************************
//
// Includes
//
//*****************************************************************************
#include "sicppsdk.h"
#include "xsi_string.h"

//*****************************************************************************
//
// Typedefs
//
//*****************************************************************************

namespace XSI {

//*****************************************************************************
/*! \class CUtils xsi_utils.h
	\brief This is an intrinsic object that provides general utility methods.
	\since Gemini

 */
//*****************************************************************************


class CValue;

class SICPPSDKDECL CUtils
{
public:

	/*! Resolves a path according to \xl Linktab rules.
	\param in_Path The path to be resolved.
	\return The resolved path.
	*/
	static CString ResolvePath( const CString &in_Path );

	/*! Determines whether a location is a workgroup, user, addon, factory,
	custom, or other type of location.
	\param in_strPath A full path name of a file or directory. The location does not need to exist.
	\return The ::siInstallationPath enum value that best describes the location.
	*/
	static LONG GetLocationType( const CString &in_strPath );

	/*! Returns true if %XSI is running on a 64-bit operating system.
	\return True if XSI is running on a 64-bit operating system, and False otherwise.
	*/
	static 	bool Is64BitOS();

	/*! Returns true if %XSI is running on Linux.
	\return True if XSI is running on Linux, and False otherwise.
	*/
	static 	bool IsLinuxOS();

	/*! Returns true if %XSI is running on Microsoft Windows.
	\return True if XSI is running on Microsoft Windows, and False otherwise.
	*/
	static 	bool IsWindowsOS();

	/*! Returns true if %XSI is running on a case-sensitive file system. For example, the
	file system on Microsoft Windows is case-insensitive, so Temp.txt and TEMP.TXT
	are the same file. Linux, however, has a case-sensitive file system. This
	function is provided to help write code that works properly on both platforms.
	\return True if the file system is case-sensitive, and False otherwise.
	*/
	static 	bool IsFileSystemCaseSensitive();

	/*! Returns true if the specified scripting language is installed.
	\param in_strLangOrFile The name of a scripting language or the name of a script file. Valid
		language names are \c VBScript, \c JScript, \c Python, and \c PerlScript. Valid script file
		names must have the extension \c .vbs (VBScript), \c .js (JScript), \c .pl (PerlScript),
		\c .py (Python), or \c .pys (Python).
	\return True if the specified scripting language is installed, and False otherwise.
	*/
	static 	bool IsScriptingLanguageInstalled( const CString &in_strLangOrFile );

	/*! Returns the translation of a string from the specified %XSI translation dictionary.
	A translation dictionary is a table that maps English strings to another language.

	Translation dictionaries are stored in \c .dict files. The translation dictionaries for a specific
	language are stored in \c .dict files in a subfolder of \c Application/Dictionary (in the Factory location).
	For example, the Japanese translation dictionaries are stored in the \c Application/Dictionary/jp folder.

	Each string can be referenced by its numeric ID or by the English version of the string.

	Some strings contain "%s" placeholders for string substitutions. For example, a message like
	"File c:\temp\myfile.txt not found" is stored in the dictionary as "File %s not found".
	The "%s" token is a placeholder for the file name. A string can contain up to three \c %s placeholders.

	When you call Translate, the optional arguments in_string1, in_string2, and in_string3
	specify the strings used to replace the placeholders. The number of optional strings supplied
	to Translate must match the number of placeholders. Other printf-type tokens, such as \c %d, are
	not supported and may cause a crash.

	\param in_ToTranslate The English string you want to translate. If the specified string is not found
		in the dictionary, or if the current language is English, you get back the same string you passed in.

		You can also pass in the numeric ID (an unsigned int) of a string in the dictionary. If a string with that
		ID cannot be found, you get back an empty string.

	\param in_dictionary The file name of the dictionary (without the \c .dict extension).
	\param in_string1 Optional. The string to substitute for the first \c %s in the dictionary string.
	\param in_string2 Optional. The string to substitute for the second \c %s in the dictionary string.
	\param in_string3 Optional. The string to substitute for the third \c %s in the dictionary string.
	\return The translation of the specified English string into the current language.

	\eg Demonstration of how to use the CUtils::Translate utility
	\code
		using namespace XSI;
		Application app;

		// You can't translate any arbitrary string, only
		// ones that are already in the dictionary.

		CString pixelRatio = CUtils::Translate( L"Pixel %s Ratio", L"XSIXSI", L"Pixel" ) ;
		app.LogMessage( L"Pixel Ratio translated is " + pixelRatio ) ;
		// Expected output: Pixel Ratio translated is Pixel Pixel Ratio

		// Load an XSI warning message, in English
		// it says "Do you want to remove it?"

		CValue valNumber((LONG) 333);
		CString errorMsg = CUtils::Translate( valNumber, L"XSIMSGCAT" ) ;
		app.LogMessage( errorMsg ) ;
		// Expected output: Do you want to remove it?

		// This is an error message that contains additional
		// context information

		// Dictionary contains: "31$Path %s has been changed to %s"
		valNumber =  (LONG)32;
		CString warningMsg = CUtils::Translate((LONG)32, L"XSIMSGCAT", L"C:\\out.txt", L"C:\\temp\\out.txt" );
		app.LogMessage( warningMsg, siWarningMsg) ;

		// Expected output: 31$Path C:\out.txt has been changed to C:\temp\out.txt
	\endcode

	*/

	static CString Translate( const CValue &in_ToTranslate, const CString &in_dictionary,
			const CString &in_string1 = CString(), const CString &in_string2 = CString(),
			const CString &in_string3 = CString());


	/*! Returns the correct path-separator character for the current platform. For example, Slash
	returns a forward slash ("/") for Linux and a backslash ("\") for Windows. This is useful
	for writing cross-platform code.
	\return A string with a single slash character.
	*/
	static CString Slash();

	/*! Builds a full path by concatenating one or more path fragments. Path fragments include
	drive letters, server names ("\\server"), device names, folder names, file names, and
	special symbols such as ".." and "~".

	Path fragments do not need to include a path separator character. BuildPath makes sure
	there is a single path separator character between each part of the path. For example,
	BuildPath( "users", "yanick" ) returns "users\yanick" on Windows and "users/yanick" on
	Linux. The path does not need to specify an existing file or folder.

	BuildPath makes it easier to write cross-platform code that deals with file
	and folder paths.

	\param in_segment1 The first part of a path (for example, "C:" or "/MyDisk").
	\param in_segment2 The second part of the path.
	\param in_segment3 An optional fragment to append to the path.
	\param in_segment4 An optional fragment to append to the path.
	\param in_segment5 An optional fragment to append to the path.
	\param in_segment6 An optional fragment to append to the path.
	\param in_segment7 An optional fragment to append to the path.
	\param in_segment8 An optional fragment to append to the path.
	\return A path composed from the specified fragments.
	*/
	static CString BuildPath
	(
		const CString &in_segment1,
		const CString &in_segment2,
		const CString &in_segment3 = CString(),
		const CString &in_segment4 = CString(),
		const CString &in_segment5 = CString(),
		const CString &in_segment6 = CString(),
		const CString &in_segment7 = CString(),
		const CString &in_segment8 = CString());

	/*! Makes sure that all directories in the specified path exist. For example, if the
	specified path is "c:\temp\a\b\c\out.txt" and a, b, and c do not exist, then EnsureFolderExists
	creates them.

	If the path is invalid or the user does not have sufficient permissions to create the
	directory, EnsureFolderExists returns False.

	\param in_Path A full (absolute) path, which can be either a network or local path.
	\param in_bHasFileName True if the path includes a file name. If False, EnsureFolderExists
 	interprets any file name as a directory name.
	\return True if successful and False otherwise.
	*/
	static bool EnsureFolderExists( const CString &in_Path, const bool in_bHasFileName );

private:
	CUtils(){};
};

};

#endif // __XSIPRIMITIVE_H__
