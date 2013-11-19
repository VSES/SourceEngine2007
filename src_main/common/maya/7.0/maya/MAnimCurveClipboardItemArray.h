
#ifndef _MAnimCurveClipboardItemArray
#define _MAnimCurveClipboardItemArray
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
// CLASS:    MAnimCurveClipboardItemArray
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAnimCurveClipboardItemArray)
//
//  This class implements an array of MAnimCurveClipboardItems.  Common
// 	convenience functions are available, and the implementation is compatible
// 	with the internal Maya implementation so that it can be passed efficiently
// 	between plugins and internal maya data structures.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MAnimCurveClipboardItem.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MAnimCurveClipboardItemArray)

///  Array of MAnimCurveClipboardItem data type (OpenMayaAnim)
/**
  Implement an array of MAnimCurveClipboardItem data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MAnimCurveClipboardItemArray
{

public:
    ///
					MAnimCurveClipboardItemArray();
    ///
					MAnimCurveClipboardItemArray(
									const MAnimCurveClipboardItemArray& other );
    ///
					MAnimCurveClipboardItemArray(
									const MAnimCurveClipboardItem src[],
									unsigned count );
    ///
					~MAnimCurveClipboardItemArray();
    ///
 	const MAnimCurveClipboardItem&		operator[]( unsigned index ) const;
    ///
 	MAnimCurveClipboardItem&			operator[]( unsigned index );
    ///
 	MStatus			set( const MAnimCurveClipboardItem& element,
						 unsigned index );
    ///
 	unsigned		length() const;
    ///
 	MStatus			remove( unsigned index );
    ///
 	MStatus			insert( const MAnimCurveClipboardItem & element,
							unsigned index );
    ///
 	MStatus			append( const MAnimCurveClipboardItem & element );
    ///
 	MStatus			clear();
    ///
	MStatus			get( MAnimCurveClipboardItem array[] ) const;
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;
	///
	bool			isValid( unsigned & failedIndex ) const;

protected:
// No protected members

private:
	bool							validate( unsigned int & index,
											  unsigned int rowCount ) const;
 	MAnimCurveClipboardItemArray&	operator = (
										const MAnimCurveClipboardItemArray&)
										const;
 	MAnimCurveClipboardItemArray&	operator = (
										const MAnimCurveClipboardItemArray&);
 	void*							fArray;
	static const char*				className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAnimCurveClipboardItemArray */
