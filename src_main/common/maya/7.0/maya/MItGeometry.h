
#ifndef _MItGeometry
#define _MItGeometry
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
// CLASS:    MItGeometry
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItGeometry)
//
//	This class is the iterator class for iterating over the points/CVs/vertices
//  of a geometry.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MPoint;
class MDataHandle;
class MDagPath;

// *****************************************************************************

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

// CLASS DECLARATION (MItGeometry)

/// Iterator class for geometry data.
/**
  Iterate over points/CV's/vertices of a geometry shape such as a mesh,
  nurbs surface, nurbs curve, subdivision surface or lattice.
*/
class OPENMAYA_EXPORT MItGeometry
{
public:
	///
	MItGeometry( const MDagPath& dagPath,
				 MStatus * ReturnStatus = NULL);
    ///
    MItGeometry( const MDagPath& dagPath,
                 MObject & component,
                 MStatus * ReturnStatus = NULL );
    ///
    MItGeometry( MObject& dagObject,
                 MStatus * ReturnStatus = NULL );
    ///
    MItGeometry( MDataHandle& dataHandle,
                 unsigned int groupId,
                 bool readOnly = true,
                 MStatus * ReturnStatus = NULL );
    ///
    MItGeometry( MDataHandle& dataHandle,
                 bool readOnly = true,
                 MStatus * ReturnStatus = NULL );
    ///
    virtual ~MItGeometry();
    ///
    bool        isDone( MStatus * ReturnStatus = NULL ) const;
    ///
    MStatus     next();
    ///
    MPoint      position( MSpace::Space space = MSpace::kObject,
                          MStatus * ReturnStatus = NULL ) const;
    ///
    MStatus     setPosition( const MPoint& point,
                             MSpace::Space space = MSpace::kObject  );
    ///
    int	    index( MStatus * ReturnStatus = NULL ) const;
	///
	MObject		component( MStatus * ReturnStatus = NULL ) const;
	///
	int		count( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus		reset( );

protected:
// No protected members

private:
	// internal : called by the dataHandle constructors to set up the iterator
	//
	void 		createIterator(MDataHandle& dataHandle,
							   bool useComponents,
							   void* comps = NULL,
							   unsigned int groupId = 0,
							   bool readOnly = true,
							   MStatus* status = NULL);
	
private:
	void *      f_it;
    void *      f_path;
    void *      f_matrix;
    void *      f_clist;
	bool        f_readOnly;
	bool        f_own;
    
    static const char* className();    
	MItGeometry( void * ptr );
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItGeometry */
