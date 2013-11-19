
#ifndef _MDataBlock
#define _MDataBlock
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
// CLASS:    MDataBlock
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDataBlock)
//
//   An MDataBlock provides storage for the data being received by or sent by
//   the node.  It is the data for the plugs and attributes of the node.  The
//   data block is only valid during the compute method of the depend node,
//   and should not be stored.
//
//   Access to the data in an MDataBlock is done using an MDataHandle or an
//   MArrayDataHandle.
//  
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
class MObject;
class MPlug;
class MDGContext;
// *****************************************************************************

// CLASS DECLARATION (MDataBlock)

/// Dependency node data block
/**
  The storage for the data of all of a node's plugs and attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MDataBlock  
{

public:

    ///
	virtual		     ~MDataBlock ();
    ///
	MDataHandle      inputValue ( const MPlug & plug, 
								        MStatus * ReturnStatus = NULL );
    ///
	MDataHandle      inputValue ( const MObject & attribute, 
								        MStatus * ReturnStatus = NULL ); 
    ///
	MDataHandle      outputValue ( const MPlug & plug, 
								        MStatus * ReturnStatus = NULL );
    ///
	MDataHandle      outputValue ( const MObject & attribute, 
								        MStatus * ReturnStatus = NULL );
    ///
	MArrayDataHandle inputArrayValue ( const MPlug & plug, 
								        MStatus * ReturnStatus = NULL );
    ///
	MArrayDataHandle inputArrayValue ( const MObject & attribute, 
								        MStatus * ReturnStatus = NULL );
    ///
	MArrayDataHandle outputArrayValue ( const MPlug & plug, 
										MStatus * ReturnStatus = NULL );
    ///
	MArrayDataHandle outputArrayValue ( const MObject & attribute, 
										MStatus * ReturnStatus = NULL );
    ///
	MStatus          setClean ( const MPlug & plug );
    ///
	MStatus          setClean ( const MObject & attribute );

    ///
	bool			isClean ( const MPlug & plug );
    ///
	bool			isClean ( const MObject & attribute, 
							  MStatus* ReturnStatus=NULL );

	///
	MDGContext       context ( MStatus * ReturnStatus = NULL );
	///
	MStatus          setContext ( const MDGContext & ctx );

protected:
// No protected members

private:
	const char*		 className() const;

	friend class MArrayDataBuilder; 

	friend class MPxNode;
	friend class MPlug;
	MDataBlock( void * ptr ); 
	void * data;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDataBlock */
