
#ifndef _MTypeId
#define _MTypeId
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
// CLASS:    MTypeId
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MTypeId)
//
// In Maya, both intrinsic and user-defined Maya Objects are registered and
// recognized by their type identifier or type id.  The basis of the type id
// system is a tag which is used at run-time to determine how to create and
// destroy Maya Objects, and how they are to be input/output from/to files.
// These tag-based identifiers are implemented by the class MTypeId.
// 
// Use the MTypeId class to create, copy and query Maya Object type
// identifiers.
// 
// It is very important to note that these ids are written into the Maya
// binary file format.  So, once an id is assigned to a node or data type it
// can {\em never} be changed while any existing Maya file contains an
// instance of that node or data type.  If a change is made, such files will
// become unreadable.
// 
// Thus, even though we provide a range of reserved ids that you can use for
// internal plug-ins, Alias highly recommends that you obtain a
// globally unique id range (see below) and use ids from this range for all
// your plug-ins, even internal ones.  This can prevent significant headaches
// later if the plans for your plug-ins change.
// 
// There are 2 forms of the constructor for this class that can be used
// depending on whether the plug-in id is internal or globally unique.
// 
// For plug-ins that will forever be internal to your site use the constructor
// that takes a single unsigned parameter.  The numeric range 0 - 0x7ffff
// (524288 ids) has been reserved for such plug-ins.
// 
// The example plug-ins provided by Alias in the plug-in development
// kit will use ids in the range 0x80000 - 0xfffff (524288 ids).  If you
// customize one of these example plug-ins, you should change the id to avoid
// future conflicts.
// 
// Plug-ins that are intended to be shared between sites will need to have a
// globally unique id.  The Alias Support department will provide
// such id's in blocks of 256.  You will be assigned one or more 24-bit
// prefixes.  Once this has been obtained, used the MTypeId constructor that
// takes 2 unsigned parameters.  The prefix goes in the first parameter,
// and you are responsible for managing the allocation of the 256 ids that
// go into the second parameter.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MTypeId)

/// Manage Maya Object type identifiers
/**
  Create, copy, and query Maya Object type identifiers.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MTypeId  
{
public:
	///
				MTypeId();
	///
				~MTypeId();

	///
				MTypeId( unsigned id );
	///
				MTypeId( unsigned prefix, unsigned id );

	///
				MTypeId( const MTypeId& src );
	///
	MTypeId&	operator=( const MTypeId& rhs );
	///
	bool		operator==( const MTypeId& rhs ) const;
	///
	bool		operator!=( const MTypeId& rhs ) const;
	///
	unsigned	id( MStatus * ReturnStatus = NULL ) const;

protected:
// No protected members

private:
	union {
		unsigned fId;
		unsigned char fTag[4];
	};

	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MTypeId */
