#ifndef _MDGContext
#define _MDGContext
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
// CLASS:    MDGContext
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDGContext)
//
// This class provides methods for accessing dependency graph contexts.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MObject;
class MTime;

// *****************************************************************************

// CLASS DECLARATION (MDGContext)

/// Dependency graph context class
/**
  Control the way in which dependency nodes are evaluated.
*/
#if defined(_WIN32)
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MDGContext  
{
public:

	// Normal
	///
	MDGContext( );

	// Timed
	///
	MDGContext( const MTime & when );

	///
	MDGContext( const MDGContext& in );

	///
	~MDGContext();



    // Method for determining whether the context is the "normal" one,
	// ie. the one used for normal evaluation
	///
	bool     	isNormal( MStatus * ReturnStatus = NULL ) const;

	///
	MStatus 	getTime( MTime & );

	///
	MDGContext&	operator =( const MDGContext& other );


	// Default context "Normal"
	///
	static		MDGContext	fsNormal;


protected:
// No protected members

private:
	const void * data;
	bool fOwn;
	friend class MPlug;
	friend class MDataBlock;
	const char* className() const;
	MDGContext( const void* );
};

#if defined(_WIN32)
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDGContext */
