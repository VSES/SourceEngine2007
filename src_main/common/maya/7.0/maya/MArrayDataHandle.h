
#ifndef _MArrayDataHandle
#define _MArrayDataHandle
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
// CLASS:    MArrayDataHandle
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MArrayDataHandle)
//
//	An MArrayDataHandle is a smart pointer into a data block (MDataBlock) that 
//  handles access to array data.  MArrayDataHandle supports attributes that
//  can have an array of data.
//
//  The array indices do not have to be continuous.  The array can be sparse.
//  Thus, an array could have four elements with the indicies 1, 6, 8, and 10.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <string.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>

// *****************************************************************************
// FORWARD DECLARATIONS
class MArrayDataBuilder;
// *****************************************************************************

// CLASS DECLARATION (MArrayDataHandle)

/// Data block handle for array data
/**
  An MArrayDataHandle is a smart pointer into an MDataBlock that
  handles array data.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MArrayDataHandle {

public:
	///
	MArrayDataHandle( const MDataHandle & in, MStatus * ReturnStatus = NULL );

	///
	MDataHandle       inputValue(MStatus *ReturnStatus = NULL);
	///
	MDataHandle       outputValue(MStatus *ReturnStatus = NULL);
	///
	MArrayDataHandle  inputArrayValue( MStatus * ReturnStatus = NULL );
	///
	MArrayDataHandle  outputArrayValue( MStatus * ReturnStatus = NULL );
	///
    MStatus           next();
	///
	unsigned          elementCount(MStatus *ReturnStatus = NULL);
	///
	unsigned          elementIndex(MStatus *ReturnStatus = NULL);
	///
    MStatus           jumpToElement( unsigned index ); 
	/// 
    MStatus           setClean();
	/// 
    MStatus           setAllClean();
	///
	MArrayDataBuilder builder(MStatus *ReturnStatus = NULL);
	///
	MStatus           set( MArrayDataBuilder &builder );

	///
    MArrayDataHandle( const MArrayDataHandle &other );
	///
	MArrayDataHandle& operator=( const MArrayDataHandle& other );

protected:
// No protected members

private:
	const char*		className() const;
	friend class MDataBlock;
	friend class MArrayDataBuilder;
	MArrayDataHandle( void* );
	MArrayDataHandle();
#ifdef Bits64_
	char data[40];
#else
	char data[20];
#endif
	bool fIsNull;
};

inline MArrayDataHandle::MArrayDataHandle( const MArrayDataHandle &other )
{
	memcpy( this, &other, sizeof(MArrayDataHandle) ); 
}

inline MArrayDataHandle& MArrayDataHandle::operator=(
											const MArrayDataHandle &other )
{
	memcpy( this, &other, sizeof(MArrayDataHandle) ); 
	return *this;
}

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MArrayDataHandle */
