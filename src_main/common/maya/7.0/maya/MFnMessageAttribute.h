
#ifndef _MFnMessageAttribute
#define _MFnMessageAttribute
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
// CLASS:    MFnMessageAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnMessageAttribute)
//
// Function set for message attributes.  A message attribute is a
// dependency node attribute that does not transmit data.  Message
// attributes only exist to formally declare relationships between nodes.
// By connecting two nodes via message attributes, a relationship between
// those nodes is expressed.  Message attribute connections can be
// traversed in the same manner as any other connection in the dependency
// graph.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

class MString;

// CLASS DECLARATION (MFnMessageAttribute)

/// Message attribute function set
/**
  Function set for message attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnMessageAttribute : public MFnAttribute 
{

	declareMFn(MFnMessageAttribute, MFnAttribute);

public:
	///
	MObject    create( const MString& fullName,
					   const MString& briefName,
					   MStatus* ReturnStatus = NULL );

protected:
// No protected members

private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnMessageAttribute */



