
#ifndef _MGlobal
#define _MGlobal
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
// CLASS:    MGlobal
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MGlobal)
//
// MGlobal is a static class which provides access to Maya's model (3d
// graphical database).
//
// MGlobal provides methods for selection, 3d-views, setting the global
// time, adding to the DAG, and executing MEL commands from within the API.
//
// MGlobal also provides methods for controlling error logging in the API.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MSelectionMask.h>
#include <maya/MTransformationMatrix.h>

// *****************************************************************************

// DECLARATIONS

class MPoint;
class MString;
class MTime;
class MVector;
class MSelectionList;
class MDagPath;
class MObjectArray;
class MCommandResult;

// *****************************************************************************

// CLASS DECLARATION (MGlobal)

/// Static class providing common API global functions
/**
  Provide methods for selection, 3D-views, time, model manipulation and MEL
  commands.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MGlobal  
{
public:
	///
	static MString      mayaVersion ();

	///
	enum MMayaState	{
		/// running with a UI
		kInteractive,
		/// running without a UI
		kBatch,
		/// A standalone (MLibrary) application is running
		kLibraryApp,
		/// Running in baseUI mode
		kBaseUIMode
	};

	///
	static MMayaState   mayaState( MStatus * ReturnStatus = NULL );
	///
	static MStatus		getFunctionSetList( MObject forObject, 
											MStringArray & array );
	///
	static MStatus		getAssociatedSets( MSelectionList & list,
											MObjectArray & setArray );
    ///
    static MStatus		getSelectionListByName( const MString& name,
												MSelectionList &list );

    ///
    enum ListAdjustment { // Comments use active list as example.
        ///
        kReplaceList,   // Totally replace the active list with the given items.
        ///
		kXORWithList,   // If one of the given items is in the active list take
                        // it out, otherwise add it to the active list.
        ///
        kAddToList,     // Add the items to the list
        ///
        kRemoveFromList // Remove the items from the list
    };

    ///
	static MStatus	 	getActiveSelectionList( MSelectionList & dest );
	///
	static MStatus 		getLiveList( MSelectionList & dest );
    ///
	static MStatus 		getHiliteList( MSelectionList & dest );
    ///
	static MStatus 		setHiliteList( MSelectionList & src );
	///
	static MStatus	 	setActiveSelectionList( MSelectionList & src,
									   ListAdjustment=kReplaceList);
	///
	static MStatus 		setDisplayCVs( MSelectionList&, bool );
	//
	///
	static MStatus		selectCommand( MSelectionList & src,
									   ListAdjustment=kReplaceList);
    ///
    static MStatus		selectByName( const MString& name,
									  ListAdjustment=kAddToList );
    ///
    static MStatus		unselectByName( const MString& name );
    ///
    static MStatus		select( MObject& object, ListAdjustment=kAddToList );
    ///
    static MStatus		select( MDagPath& object, MObject& component,
								ListAdjustment=kAddToList );
    ///
    static MStatus		unselect( MObject& object );
    ///
    static MStatus		unselect( MDagPath& object, MObject& component );
    ///
    static MStatus		selectFromScreen( short& x_pos, short& y_pos,
                                           ListAdjustment=kAddToList );
    ///
    static MStatus		selectFromScreen( short& start_x, short& start_y,
                                          short& end_x, short& end_y,
                                          ListAdjustment=kAddToList );

	///
	enum MSelectionMode {
		///
		kSelectObjectMode,
		///
		kSelectComponentMode,
		///
		kSelectRootMode,
		///
		kSelectLeafMode,
		///
		kSelectTemplateMode
	};

	///
	static MSelectionMode selectionMode( MStatus * ReturnStatus = NULL );
	///
	static MStatus		  setSelectionMode( MSelectionMode mode );

    ///
	static MSelectionMask objectSelectionMask( MStatus * ReturnStatus = NULL );
    ///
	static MStatus		  setObjectSelectionMask( MSelectionMask& mask );
    ///
	static MSelectionMask componentSelectionMask( MStatus * ReturnStatus =
												  NULL );
    ///
	static MStatus		  setComponentSelectionMask( MSelectionMask& mask );
    ///
	static MSelectionMask animSelectionMask( MStatus * ReturnStatus = NULL );
    ///
	static MStatus		  setAnimSelectionMask( MSelectionMask& mask );
    ///
	static MSelectionMask miscSelectionMask( MStatus * ReturnStatus = NULL );
    ///
	static MStatus		  setMiscSelectionMask( MSelectionMask& mask );

    ///
	static MStatus		  clearSelectionList();
    ///
	static bool			  isSelected( MObject& object,
							  		  MStatus * ReturnStatus = NULL );

    ///
	static MStatus		viewFrame( const MTime & time );
	///
	static MStatus      viewFrame( double time);

	///
	static MStatus		sourceFile( const MString& fileName );
	///
	static MStatus 		executeCommandOnIdle( const MString& command,
										bool displayEnabled = false);
	///
	static MStatus 		executeCommand( const MString& command,
										bool displayEnabled = false,
										bool undoEnabled = false);
    ///
	static MStatus		executeCommand( const MString& command,
										MCommandResult& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	///
	static MStatus		executeCommand( const MString& command,
										int& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	///
	static MStatus		executeCommand( const MString& command,
										MIntArray& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	///
	static MStatus		executeCommand( const MString& command,
										double& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	///
	static MStatus		executeCommand( const MString& command,
										MDoubleArray& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	///
	static MStatus		executeCommand( const MString& command,
										MString& result,
										bool displayEnabled = false,
										bool undoEnabled = false );
	///
	static MStatus		executeCommand( const MString& command,
										MStringArray& result,
										bool displayEnabled = false,
										bool undoEnabled = false );

    ///
    static MStatus      addToModel( MObject & object, 
									  MObject & parent = MObject::kNullObj );
    ///
    static MStatus      addToModelAt( MObject&, const MVector& point,
								  const double scale[3]  = NULL,
								  const double rotation[3] = NULL,
								  MTransformationMatrix::RotationOrder order =
									  MTransformationMatrix::kXYZ );
    ///
    static MStatus      removeFromModel( MObject& );
    ///
    static MStatus      deleteNode( MObject& );
    ///
    static MStatus      setYAxisUp( bool rotateView = false );
    ///
    static MStatus      setZAxisUp( bool rotateView = false );
    ///
    static bool			isYAxisUp( MStatus* = NULL );
    ///
    static bool			isZAxisUp( MStatus* = NULL );
    ///
    static MVector      upAxis( MStatus* = NULL );

	///
	static void			displayInfo( const MString & theMessage );
	///
	static void			displayWarning( const MString & theWarning );
	///
	static void			displayError( const MString & theError );
	///
	static void			setDisableStow( bool state );
	///
	static bool			disableStow( );

//	Support for DEBUG/NDEBUG API
//
	///
	static MString		defaultErrorLogPathName( MStatus* ReturnStatus = NULL );
	///
	static MStatus		resetToDefaultErrorLogPathName();
	///
	static MStatus		setErrorLogPathName( const MString& logPathName );
	///
	static MString		errorLogPathName( MStatus* ReturnStatus = NULL );
	///
	static MStatus		startErrorLogging();
	///
	static MStatus		startErrorLogging( const MString& logPathName );
	///
	static bool			errorLoggingIsOn( MStatus* ReturnStatus = NULL );
	///
	static MStatus		stopErrorLogging();
	///
	static MStatus		closeErrorLog();
	///
	static MStatus		doErrorLogEntry( const MString& errorLogEntry );
	///
	static bool			isUndoing();
	///
	static bool			isRedoing();

protected:
	static const char* className();
private:
	~MGlobal();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MGlobal */
