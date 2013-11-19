
#ifndef _MArgDatabase
#define _MArgDatabase
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
// CLASS:    MArgDatabase
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MArgDatabase)
//
// This class is used to parse and store all of the flags, arguments
// and objects which are passed to a command.  The MArgDatabase accepts
// an MSyntax object, which describes the format for a command, and uses
// it to parse the command arguments into a form which is easy to query.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MArgParser.h>

// *****************************************************************************

// DECLARATIONS

class MSyntax;
class MSelectionList;
class MStringArray;
class MArgList;
class MDistance;
class MAngle;
class MTime;

// *****************************************************************************

// CLASS DECLARATION (MArgDatabase)

/// Command argument list parser.
/**
This class parses argument lists based on a syntax object (MSyntax) 
which describes the format for a command.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MArgDatabase : public MArgParser
{
public:
	///
			MArgDatabase		();
	///
			MArgDatabase		(const MSyntax &syntax,
								 const MArgList &argList,
								 MStatus *ReturnStatus = NULL);
	///
	virtual ~MArgDatabase();

	///
	MStatus getFlagArgument		(const char *flag, unsigned index, 
								 MSelectionList &result) const;
	///
	MStatus	getCommandArgument	(unsigned index, MSelectionList &result) const;

	///
	MStatus	getObjects			(MSelectionList &result) const;

	// Redeclarations of overloaded methods from MArgParser
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 bool &result) const;
	MStatus	getFlagArgument		(const char *flag, unsigned index,
								 int &result) const;
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 double &result) const;
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 MString &result) const;
	MStatus getFlagArgument		(const char *flag, unsigned index, 
								 unsigned &result) const;
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 MDistance &result) const;
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 MAngle &result) const;
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 MTime &result) const;
	MStatus	getCommandArgument	(unsigned index, bool &result) const;
	MStatus	getCommandArgument	(unsigned index, int &result) const;
	MStatus	getCommandArgument	(unsigned index, double &result) const;
	MStatus	getCommandArgument	(unsigned index, MString &result) const;
	MStatus	getCommandArgument	(unsigned index, MDistance &result) const;
	MStatus	getCommandArgument	(unsigned index, MAngle &result) const;
	MStatus	getCommandArgument	(unsigned index, MTime &result) const;
	MStatus	getObjects			(MStringArray &result) const;

protected:
// No protected members

private:
	const char *className() const;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MArgDatabase */

