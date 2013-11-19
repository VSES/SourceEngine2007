#ifndef _MBoundingBox
#define _MBoundingBox
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
// CLASS:    MBoundingBox
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MBoundingBox)
//
//	This class provides an implementation of a bounding box.  A bounding box
//  describes a volume in space that bounds a piece of geometry.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MPoint.h>

// PDB
#if defined(_WIN32)
#undef min
#undef max
#endif // _WIN32

// *****************************************************************************

// DECLARATIONS 

class MMatrix;

// *****************************************************************************

// CLASS DECLARATION (MBoundingBox)

/// implementation of a 3D bounding box
/**
  This class implements a 3D bounding box
*/
#if defined(_WIN32)
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MBoundingBox  
{
public:
	///
    MBoundingBox();  
	///
	MBoundingBox( const MBoundingBox & src );  
	///
	MBoundingBox( const MPoint &corner1, const MPoint &corner2 );

	///
	~MBoundingBox();

	///
	void	clear();

	///  
    void	transformUsing ( const MMatrix &matrix );
	///
	void	expand( const MPoint & point );
	/// 
	void	expand( const MBoundingBox & box );
	
	/// 
	bool	contains( const MPoint & point ) const;
	///
	double	width() const;
	///
	double	height() const;
	///
	double	depth() const;
	///
	MPoint	center() const;
	///
	MPoint	min() const;
	///
	MPoint	max() const;

	///
	MBoundingBox & operator=( const MBoundingBox & other );

protected:
// No protected members

private: 
#if defined(OSMac_CFM_)
	char data[52];
#else
	double data[6];
#endif
};

#if defined(_WIN32)
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MBoundingBox */
