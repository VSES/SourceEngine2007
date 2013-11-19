
#ifndef _MSyntax
#define _MSyntax
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
// CLASS:    MSyntax
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MSyntax)
//
//  This class is used to specify flags and arguments passed to commands.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MSyntax)

/// Syntax for Commands.
/**
This class is used to specify flags and arguments passed to commands.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32


class OPENMAYA_EXPORT MSyntax {
public:

	// argument type
	///
	enum MArgType {
		///
		kInvalidArgType,
		///
		kNoArg,
		///
		kBoolean,
		/// 
		kLong,
		///
		kDouble,
		///
		kString,
		///
		kUnsigned,
		///
		kDistance,
		///
		kAngle,
		///
		kTime,
		///
		kSelectionItem,
		///
		kLastArgType
	};

	// Object list type enum
	///
	enum MObjectFormat { 
		///
		kInvalidObjectFormat,
		///
		kNone,
		///
		kStringObjects,
		///
		kSelectionList,
		///
		kLastObjectFormat
	};

	///
		    MSyntax ();
	///
		    MSyntax ( const MSyntax& other );
	///
	virtual ~MSyntax();
	///
	MSyntax &operator=(const MSyntax &rhs);

	///
	MStatus	addFlag					(const char *shortName,
									 const char *longName,
									 MArgType argType1 = kNoArg,
									 MArgType argType2 = kNoArg,
									 MArgType argType3 = kNoArg,
									 MArgType argType4 = kNoArg,
									 MArgType argType5 = kNoArg,
									 MArgType argType6 = kNoArg);

	///
	MStatus	makeFlagMultiUse		(const char *flag);

	///
	MStatus	addArg					(MArgType arg);
	
	///
	void	useSelectionAsDefault	(bool useSelectionList = false);

	///
	MStatus	setObjectType			(MObjectFormat objectFormat, 
									 unsigned minimumObjects = 0);
	///
	MStatus	setObjectType			(MObjectFormat objectFormat, 
									 unsigned minimumObjects, 
									 unsigned maximumObjects);

	///
	void	setMinObjects			(unsigned minimumObjectCount);
	///
	void	setMaxObjects			(unsigned maximumObjectCount);
	///
	void	enableQuery				(bool supportsQuery = true);
	///
	void	enableEdit				(bool supportsEdit = true);

	///
	unsigned	minObjects			() const;
	///
	unsigned	maxObjects			() const;
	///
	bool		canQuery			() const;
	///
	bool		canEdit				() const;

protected:
// No protected members

private:
	const char *className() const;

	MSyntax(void *);
	MSyntax(const void *);
	bool fOwn;

	void * apiData;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

#endif /* __cplusplus */
#endif /* _MSyntax */
