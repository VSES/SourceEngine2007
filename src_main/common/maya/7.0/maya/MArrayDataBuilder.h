
#ifndef _MArrayDataBuilder
#define _MArrayDataBuilder
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
// CLASS:    MArrayDataBuilder
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MArrayDataBuilder)
//
//  An MArrayDataBuilder is used to construct data for attributes/plugs that 
//  have an array of data.  An array builder is used with an MArrayDataHandle
//  for managing data.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>

// *****************************************************************************

// FORWARD DECLARATIONS
class MDataBlock; 

// *****************************************************************************

// CLASS DECLARATION (MArrayDataBuilder)

/// Array builder for arrays in data blocks
/**
 An MArrayDataBuilder is used to build array data for attributes/plugs
 that support arrays.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MArrayDataBuilder  
{

public:
    /// Obsolete ...
	MArrayDataBuilder ( const MObject & attribute, unsigned numElements,
						MStatus * ReturnStatus = NULL );

	/// 
	MArrayDataBuilder ( MDataBlock *, const MObject &attribute, unsigned, 
						MStatus * ReturnStatus = NULL ); 
	
    ///
	MDataHandle      addLast(MStatus * ReturnStatus = NULL);
    ///
	MDataHandle      addElement( unsigned index,
								 MStatus * ReturnStatus = NULL );
    ///
	MArrayDataHandle addLastArray(MStatus * ReturnStatus = NULL );
    ///
	MArrayDataHandle addElementArray( unsigned index,
									  MStatus * ReturnStatus = NULL  );
    /// 
	MStatus          removeElement( unsigned index );
    /// 
	unsigned         elementCount( MStatus * ReturnStatus = NULL ) const;
    /// 
	MStatus          growArray( unsigned amount );
    ///
	MStatus          setGrowSize( unsigned size );

    ///
    MArrayDataBuilder( const MArrayDataBuilder &other );
	///
	MArrayDataBuilder& operator=( const MArrayDataBuilder& other );
	///
    ~MArrayDataBuilder();

protected:
// No protected members

private: 
	void constructClass(MDataBlock *, const MObject &attribute, unsigned, 
						MStatus * ReturnStatus = NULL ); 
	const char*		className() const;
	friend class MArrayDataHandle;
	MArrayDataBuilder();
	MArrayDataBuilder( void* );
#ifdef Bits64_
	char data[48];
#else
	char data[28];
#endif
	bool fIsNull;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MArrayDataBuilder */
