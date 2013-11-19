
#ifndef _MPxCommand
#define _MPxCommand
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
// CLASS:    MPxCommand
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxCommand)
//
//	This is the proxy class for creating MEL commands through the API.
//
//	Each command is derived from this one, and must have a doIt method,
//	and optionally undoIt, and redoIt methods.
//
//	The doIt method should collect whatever information is required to
//	do the task, and store it in local class data.  It should finally
//	call redoIt to make the command happen.  The redoIt method should
//	do the actual work, using only the local class data.  The undoIt
//	method should undo the actual work, again using only the local
//	class data.
//
//	Maya contains support for infinite levels of undo.  If a command
//	written in a plug-in changes the state of anything in Maya, it
//	should implement undoIt and redoIt methods.  As well, if the doIt
//	method returns successfully, Maya will call the method's isUndoable
//	method immediately afterwards.  If that method returns true, the
//	instance of this class is retained and passed to Maya's undo
//	manager so that the undoIt and redoIt methods can be called when
//	appropriate.  If isUndoable returns false, the command instance is
//	destroyed right away.
//
//	So, for example, if a command supports both query and edit modes,
//	in query mode the command should set a flag so that the isUndoable
//	method returns false to prevent that command instance from being
//	retained by the undo manager.  In edit mode, where the state of
//	Maya is changed, isUndoable should return true to enable undo and
//	redo.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

class MFileObject;
class MArgList;
class MIntArray;
class MDoubleArray;
class MStringArray;
class MSyntax;

// *****************************************************************************

// CLASS DECLARATION (MPxCommand)

/// Base class for user commands
/**
  Base class for creating user defined Maya commands.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPxCommand  
{
public:
	///
						MPxCommand();
	///
	virtual 			~MPxCommand();
	/// Pure virtual
	virtual MStatus   	doIt( const MArgList& args ) = 0;
	///
	virtual MStatus   	undoIt( );
	///
	virtual MStatus   	redoIt( );
	///
	virtual bool		isUndoable() const;
	///
	virtual bool		hasSyntax() const;
	///
	MSyntax				syntax() const;
	///
	bool                isHistoryOn() const;
	///
	MString      		commandString() const;
	///
	MStatus            	setHistoryOn( bool state );
	///
	MStatus            	setCommandString( const MString & );


	///
	static void			displayInfo( const MString & theInfo );
	///
	static void			displayWarning( const MString & theWarning );
	///
	static void			displayError( const MString & theError );

	///
	static void			displayWarning( const MString & theWarning, bool wantDisplayLineNumber );
	///
	static void			displayError( const MString & theError, bool wantDisplayLineNumber );

	///
	enum MResultType {
		///
		kLong,
		///
		kDouble,
		///
		kString,
		///
		kNoArg
	};

	///
	static void         clearResult();

	///
	static void         setResult( int val );
	///
	static void         setResult( double val );
	///
	static void         setResult( bool val );
	///
	static void         setResult( const char* val );
	///
	static void         setResult( const MString& val );
	///
	static void         setResult( const MIntArray& val );
	///
	static void         setResult( const MDoubleArray& val );
	///
	static void         setResult( const MStringArray& val );

	///
	static void         appendToResult( int val );
	///
	static void         appendToResult( double val );
	///
	static void         appendToResult( bool val );
	///
	static void         appendToResult( const char* val );
	///
	static void         appendToResult( const MString& val );
	///
	static void         appendToResult( const MStringArray& val );

	///
	static MResultType  currentResultType();
	///
	static MStatus      getCurrentResult( int& val );
	///
	static MStatus      getCurrentResult( double& val );
	///
	static MStatus      getCurrentResult( MString& val );
	///
	static MStatus      getCurrentResult( MIntArray& val );
	///
	static MStatus      getCurrentResult( MDoubleArray& val );
	///
	static MStatus      getCurrentResult( MStringArray& val );

	/// OBSOLETE
	MStatus            	setUndoable( bool state );

protected:
	static const char*	className();
	void setData( void * ptr );
	void*				instance;
private:
	MString command;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxCommand */
