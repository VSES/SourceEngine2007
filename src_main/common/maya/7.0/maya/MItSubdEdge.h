
#ifndef _MItSubdEdge
#define _MItSubdEdge
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
// CLASS:    MItSubdEdge
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItSubdEdge)
//
//	This class is the iterator for subdiv edges.
//
//	The iterator iterates over all edges of the subdiv surface.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MObject;
class MStatus;

// *****************************************************************************

// CLASS DECLARATION (MItSubdEdge)

/// Subdiv edge iterator
/**
  Iterates over subdiv edges
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MItSubdEdge
{
public:   
	///
	MItSubdEdge( MObject & subdObject, MStatus * ReturnStatus = NULL );
	///
	virtual ~MItSubdEdge();

	///
	MStatus		reset();
	///
	MStatus		next(); 
	///
	bool		isDone( MStatus * ReturnStatus = NULL );
	///
	bool isValid(MStatus * ReturnStatus = NULL) const;

	///
	int level(MStatus * ReturnStatus = NULL) const;
	///
	void setLevel( int level, MStatus * ReturnStatus = NULL );

	///
	MUint64		index( MStatus * ReturnStatus = NULL );

	///
	bool		isSharp( MStatus * ReturnStatus = NULL );
	///
	void		setSharpness( bool sharp, MStatus * ReturnStatus = NULL );
	///
	bool		isBoundary( MStatus * ReturnStatus = NULL );

protected:

private:
	static const char* 	className();
	void     *       f_it;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItSubdEdge */



