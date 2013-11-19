
#ifndef _MArgParser
#define _MArgParser
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
// CLASS:    MArgParser
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MArgParser)
//
// This class is used to parse and store all of the flags, arguments
// and objects which are passed to a command.  The MArgParser accepts
// an MSyntax object, which describes the format for a command, and uses
// it to parse the command arguments into a form which is easy to query.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

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

// CLASS DECLARATION (MArgParser)

/// Command argument list parser
/**
This class parses argument lists based on a syntax object (MSyntax) 
which describes the format for a command.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MArgParser
{
public:
	///
			MArgParser();
	///
			MArgParser			(const MSyntax &syntax,
								 const MArgList &argList,
								 MStatus *ReturnStatus = NULL);
	///
	virtual ~MArgParser();

	///
	bool	isFlagSet			(const char *flag, 
								 MStatus *ReturnStatus = NULL) const;
	///
	bool	isEdit				(MStatus *ReturnStatus = NULL) const;
	///
	bool	isQuery				(MStatus *ReturnStatus = NULL) const;
 
	///
	unsigned numberOfFlagsUsed	() const;
 	///
	unsigned numberOfFlagUses	(const char *flag) const;
	///
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 bool &result) const;
	///
	MStatus	getFlagArgument		(const char *flag, unsigned index,
								 int &result) const;
	///
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 double &result) const;
	///
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 MString &result) const;
	///
	MStatus getFlagArgument		(const char *flag, unsigned index, 
								 unsigned &result) const;
	///
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 MDistance &result) const;
	///
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 MAngle &result) const;
	///
	MStatus	getFlagArgument		(const char *flag, unsigned index, 
								 MTime &result) const;
	///
	MStatus getFlagArgumentPosition
								(const char *flag, unsigned i,
								 unsigned &position) const;
	///
	MStatus getFlagArgumentList	(const char *flag, unsigned i,
								 MArgList& args) const;

	///
	MStatus	getCommandArgument	(unsigned index, bool &result) const;
	///
	MStatus	getCommandArgument	(unsigned index, int &result) const;
	///
	MStatus	getCommandArgument	(unsigned index, double &result) const;
	///
	MStatus	getCommandArgument	(unsigned index, MString &result) const;
	///
	MStatus	getCommandArgument	(unsigned index, MDistance &result) const;
	///
	MStatus	getCommandArgument	(unsigned index, MAngle &result) const;
	///
	MStatus	getCommandArgument	(unsigned index, MTime &result) const;

	///
	MStatus	getObjects			(MStringArray &result) const;

protected:
	bool fOwn;
	void *apiData;

private:
	const char *className() const;

	MArgParser (void *);
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MArgParser */

