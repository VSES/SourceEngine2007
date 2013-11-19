
#ifndef _MFileIO
#define _MFileIO
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
// CLASS:    MFileIO
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFileIO)
//
//	The MFileIO class provides a set of global functions that can be used to
//	load and store scene elements into maya files.  Methods are also provided
//	to import elements from other files into the current scene, and export
//	elements of the current scene into files.  As well, references to other
//	files can be created and removed.
//	
//	This class implements only the most common operations from the full set
//	possible with the MEL command "file."  See the documentation of this
//	command for a complete listing of all the possiblities.  If more control
//	is required than provided by this class, a MEL command can be constructed
//	according to the instructions for the "file" command, and issued via the
//	executeCommand method of the MGlobal class.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MPlugArray.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFileIO)

/// I/O operations on scene files.
/**
 Methods for opening, saving, importing, exporting and referencing files.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFileIO  
{

public:
	///
						MFileIO ();
	///
						MFileIO ( const MString& fileName );
	///
	virtual				~MFileIO ();
	///
	static MString		currentFile ();
	///
	static MStatus		setCurrentFile ( const MString& fileName );
	///
	static MString		fileType();
	///
	static MStatus		getFileTypes ( MStringArray& types );
	///
	static MStatus		newFile ( bool force = false );
	///
	static MStatus		open ( const MString& fileName,
							   const char* type = NULL,
							   bool force = false );
	///
	static MStatus		save ( bool force = false );
	///
	static MStatus		saveAs ( const MString& fileName,
							     const char* type = NULL,
								 bool force = false );
	///
	static MStatus		import ( const MString& fileName,
								 const char* type = NULL,
								 bool  preserveReferences = false,
								 const char* nameSpace = NULL);
	///
	static MStatus		exportSelected ( const MString& fileName,
								 const char* type = NULL,
								 bool preserveReferences = false);
	///
	static MStatus		exportAll ( const MString& fileName,
								 const char* type = NULL,
								 bool preserveReferences = false);
	///
	static MStatus		exportAnimFromReference ( const MString& fileName, const char* type = NULL);
	///
	static MStatus		exportAnimFromReference ( const MString& fileName, const MString& refNodeName, const char* type = NULL);
	///
	static MStatus		exportSelectedAnimFromReference ( const MString& fileName, const char* type = NULL);
	///
	static MStatus		exportSelectedAnimFromReference ( const MString& fileName, const MString& refNodeName, const char* type = NULL);
	///
	static MStatus		exportAnim ( const MString& fileName, const char* type = NULL);
	///
	static MStatus		exportSelectedAnim ( const MString& fileName, const char* type = NULL);
	///
	static MStatus		exportAsReference ( const MString& fileName, const char* type = NULL);
	///
	static MStatus		getReferences ( MStringArray& references );
	///
	static MStatus		getReferenceNodes ( const MString &fileName,
											MStringArray& nodes );
	///
	static MStatus		getReferenceConnectionsMade ( const MString &fileName,
													  MStringArray& connections );
	///
	static MStatus		getReferenceConnectionsBroken(
							const MString&	fileName,
							MStringArray&	connections
						);
	///
	static MStatus		getReferenceConnectionsBroken(
							const MString&	fileName,
							MPlugArray&		srcPlugs,
							MPlugArray&		destPlugs
						);
	///
	static MStatus		getReferenceConnectionsBroken(
							const MObject&	node,
							MStringArray&	connections,
							bool			append = false,
							bool			removeDuplicates = true
						);
	///
	static MStatus		getReferenceConnectionsBroken(
							const MObject&	node,
							MPlugArray&		srcPlugs,
							MPlugArray&		destPlugs,
							bool			append = false,
							bool			removeDuplicates = true
						);
	///
	static MStatus		reference ( const MString& fileName, bool deferReference = false, bool lockReference = false );
	///
	static MStatus		removeReference ( const MString& fileName );
	///
	static bool			isReadingFile();
	///
	static bool			isWritingFile();
	///
	static bool			mustRenameToSave(MStatus *ReturnStatus = NULL);
	///
	static MStatus		setMustRenameToSave(bool);
	///
	static MString		mustRenameToSaveMsg(MStatus *ReturnStatus = NULL);
	///
	static MStatus		setMustRenameToSaveMsg(MString &);
	///
	static MString		beforeOpenFilename(MStatus * ReturnStatus = NULL);
	///
	static MString		beforeImportFilename(MStatus * ReturnStatus = NULL);
	///
	static MString		beforeSaveFilename(MStatus * ReturnStatus = NULL);
	///
	static MString		beforeExportFilename(MStatus * ReturnStatus = NULL);
	///
	static MString		beforeReferenceFilename(MStatus * ReturnStatus = NULL);

	///
	static MString		getLastTempFile(MStatus * ReturnStatus = NULL);
	///
	static bool			getErrorStatus(MStatus * ReturnStatus = NULL);
	///
	static MStatus		resetError();

	///
	static MString		loadReference(MString& referenceFileName, MStatus *returnStatus = NULL);
	///
	static MString		loadReferenceByNode(MString& referenceNodeName, MStatus *returnStatus = NULL);
	///
	static MString		loadReferenceByNode(MObject& referenceNode, MStatus *returnStatus = NULL);

	///
	static MString		unloadReference(MString& referenceFileName, MStatus *returnStatus = NULL);
	///
	static MString		unloadReferenceByNode(MString& referenceNodeName, MStatus *returnStatus = NULL);
	///
	static MString		unloadReferenceByNode(MObject& referenceNode, MStatus *returnStatus = NULL);

	static MString		getReferenceFileByNode(MObject& referenceNode, MStatus *returnStatus = NULL);
	///
	static MStatus		cleanReference(MString& referenceNodeName, const char* editCommand = NULL, MStatus *returnStatus = NULL);
	///
	static MStatus		saveReference(MString& referenceNodeName, MStatus *returnStatus = NULL);

protected:
// No protected members 
	
private:
	static const char* 	className();

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFileIO */
