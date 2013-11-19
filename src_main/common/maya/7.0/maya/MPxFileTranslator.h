#ifndef _MPxFileTranslator
#define _MPxFileTranslator
//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
//
// CLASS:    MPxFileTranslator
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxFileTranslator)
//
//  This class provides a base class from which one derives in order to
//  implement Maya "File Translator Plug-Ins."  A file translator plug-in
//  allows Maya to read or write 3rd party file formats.
//  
//  The identifyFile method is called whenever Maya's file save or restore
//  dialogs are run, and identifyFile uses the filename and first few bytes
//  of the file to determine whether it is of a type supported by the
//  translator.  If it is, then Maya will display the file with the "name"
//  and "icon" specified as arguments to MFnPlugin::registerFileTranslator.
//  
//  If an attempt is made to read the file, the "reader" method in the
//  derived class is called, if an attempt is made to write the file, the
//  "writer" method is called.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypes.h>
#include <maya/MFileObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MPxFileTranslator)

/// Base Class for creating Maya File Translators
/**
  This class provides the connection to Maya by which user written file
  translators can be added as plug-ins.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPxFileTranslator  
{

public:
	///
	enum MFileKind {
		/// Translator understands how to read/write this file
		kIsMyFileType,
		/// Translator is not best available to read/write this file
		kCouldBeMyFileType,
		/// Translator does not understand how to read/write this file
		kNotMyFileType
	};
	///
	enum FileAccessMode {
		/// Unknown file access mode
		kUnknownAccessMode,
		/// Import data into new scene
		kOpenAccessMode,
		/// Reference data into current scene
		kReferenceAccessMode,
		/// Import data into current scene
		kImportAccessMode,
		/// Save data
		kSaveAccessMode,
		/// Export data
		kExportAccessMode,
		/// Export active (selected) data
		kExportActiveAccessMode
	};
	///
						MPxFileTranslator ();
	///
	virtual				~MPxFileTranslator ();	
	///
	virtual MStatus		reader ( const MFileObject& file,
								 const MString& optionsString,
								 FileAccessMode mode);
	///
	virtual MStatus		writer ( const MFileObject& file,
								 const MString& optionsString,
								 FileAccessMode mode);
	///
	virtual bool		haveReadMethod () const;
	///
	virtual bool		haveWriteMethod () const;
	///
	virtual bool		haveNamespaceSupport () const;
	///
	virtual bool		haveReferenceMethod () const;
	///
	virtual MString     defaultExtension () const;
	///
	virtual MString     filter () const;
	///
	virtual bool        canBeOpened () const;
	///
	virtual MPxFileTranslator::MFileKind identifyFile (	const MFileObject& file,  
								const char* buffer, short size) const;
	///
	static MPxFileTranslator::FileAccessMode fileAccessMode();

protected:
// No protected members

private:
	void*				data;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxFileTranslator */
