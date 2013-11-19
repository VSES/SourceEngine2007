#ifndef _MPxData
#define _MPxData
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
// CLASS:    MPxData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxData)
//
// In Maya, both intrinsic and user-defined Maya Objects are registered
// and recognized by their type identifier (MTypeId).  Data which flows in
// the Dependency Graph (DG) is implemented as Maya objects, therefore,
// the type characteristics of DG Data are specified by the type id
// (MTypeId).  The type id is used to at run-time to determine how to
// create and destroy Maya Objects, and how they are to be input/output
// from/to files.
//
// User-defined Data has two parts.  One part is an internal Maya Data
// object of neutral type which is common to all user-defined types.  This
// common Data object implements the interface and behaviour
// characteristics required for user-defined Data to act as Data within
// Maya.  The second part is unique to each user-defined type and
// implements the type-specific behaviour.
//
// The Proxy Data (MPxData) class is the base class for user-defined Data
// types.  All user-defined Data that is to be passed between Nodes in the
// DG must be derived from MPxData.  MPxData transparently incorporates
// the common behaviour and defines the common interface required for DG
// Data.  When initialized with a unique type id, Data objects of classes
// derived from MPxData are recognized by Maya the same as built-in DG
// Data types, but are be able to implement user-specified behaviour.
//
// The MPxData class defines the interface for methods to read, write and
// assign Data objects of the the user-defined type.  User-defined types
// must override these methods to provide type-specific behaviour.
//
// MPxData also provides common methods for querying the type id and type
// name of the Data object.
//
// All user-defined DG Data types require an associated static creator
// function that returns a void pointer to a new instance of the data
// class.  This function is usually a static function within the user
// defined data type class.
//
// The registration of the new data type is performed by the
// MFnPlugin::registerData() which is invoked in the initializePlugin()
// function during Plug-in loading.  One of the most important thing that
// the registration does is it associates the type id with the data.
//
// Once a user-defined Data type based on MPxData has been defined and
// registered, use the Typed Attribute Function Set (MFnTypedAttribute) to
// create an Attribute of the user-defined type.  The Attribute may also
// be an multi-Attribute (array).  Use the DG Node Function Set
// (MFnDependencyNode) to add the Attribute to a user- defined Node.  This
// is usually done in the initialize() method of the Node creator.
//
// Data of a user-defined type on a Node is accessed in the same way as
// intrinsic Data types.  The Data is actually held in an Data Block
// (MDataBlock).  Use a Data Handle (MDataHandle) or Array Data Handle
// (MArrayDataHandle) to access the Data within a Data Block.  Use a Data
// Block to obtain a Data Handle or Array Data Handle for either an
// Attribute or the associated Plug on the Attribute.  The Data Handle or
// Array data handle can then be queried for the underlying Data object.
// The underlying Data object is a generic Maya Object (MObject) with a
// type id of the user-defined type.
//
// Use the Plug-in Data Function Set (MFnPluginData) to obtain an MPxData
// pointer which can be safely cast to a pointer of the user-defined type.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MIOStreamFwd.h>

// *****************************************************************************

// DECLARATIONS

class MString;
class MObject;
class MTypeId;
class MPlug;
class MArgList;

// *****************************************************************************

// CLASS DECLARATION (MPxData)

///  Base Class for User-defined Dependency Graph Data Types
/**

Read and write user-defined Data from both ASCII and binary formatted files.

Determine the type id and type name.

Access an instance of the creator for the user-defined type.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPxData  
{
public:
	///
	enum Type {
		///
		kData,
		///
		kGeometryData,
		///
		kLast
	};

	///
	MPxData();
	///
	virtual ~MPxData();
	///
	virtual MStatus			readASCII( const MArgList& argList,
									   unsigned& endOfTheLastParsedElement );
	///
	virtual MStatus			readBinary( IOS_REF(istream)& in, unsigned length );
	///
	virtual MStatus			writeASCII( IOS_REF(ostream)& out );
	///
	virtual MStatus			writeBinary( IOS_REF(ostream)& out );
	///
	virtual	void			copy( const MPxData& src) = 0;
	///
	virtual MTypeId         typeId() const = 0;
	///
	virtual MString         name() const = 0;

protected:
	friend class            MDataHandle;
	void*					instance;

private:
// No private members
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxData */
