//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVstExampleCommand
//
//=============================================================================

// Maya includes

#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>

// Valve includes

#include "valveMaya.h"
#include "VsMayaMpxFactory.h"


//-----------------------------------------------------------------------------
//
// Purpose: A new Maya command dervied from MPxCommand
//
//-----------------------------------------------------------------------------
class CVstExampleCmd : public CVsMayaMPxCommand
{
public:
	CVstExampleCmd();
	virtual ~CVstExampleCmd();

	// Inherited from MPxCommand
	virtual MStatus doIt( const MArgList &i_mArgList );
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const { return m_undoable; }
	virtual bool hasSyntax() const { return true; }

	// Inherited from CVsMayaMPxCommand
	virtual void SpecifySyntax( MSyntax &syntax, vm::CMSyntaxHelp &help );

protected:
	bool m_undoable;					// Is the command undoable or not
	MArgList m_mArgList;				// The stored arg list (for undo/redo)
	MDagModifier *m_mDagModifier;		// The dag modifier (for undo/redo)
	MSelectionList m_mSelectionList;	// The previous selection list (for undo/redo)

public:
	static const char *const s_name;	// Name of the command
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstExampleCmd,
	vstExample,
	"Example VST Command" );


//-----------------------------------------------------------------------------
//
// Statics
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CVstExampleCmd::CVstExampleCmd() : m_undoable(false), m_mDagModifier(NULL)
{
}


CVstExampleCmd::~CVstExampleCmd()
{
}


//-----------------------------------------------------------------------------
//
// Purpose: MSyntax factory for the command
// Input  :
// Output : A Maya MSyntax class describing command's command line syntax
//
//-----------------------------------------------------------------------------
void CVstExampleCmd::SpecifySyntax( MSyntax &mSyntax, vm::CMSyntaxHelp &help )
{
	mSyntax.addFlag( "h", "help" );
	mSyntax.addFlag( "c", "create", MSyntax::kString );
	mSyntax.addFlag( "n", "name", MSyntax::kString );
}


//-----------------------------------------------------------------------------
//
// Purpose: When this command is executed by Maya for the first time,
//          a new instance of the command class is created and this function
//          is called with the specified command line arguments
// Input  : The command line arguments specified by the user
// Output : MStatus indicating success or failure of the command
//          In order for the command to be kept in the undo stack
//          the return value must be MStatus::kSuccess
//          Also the virtual isUndoable() has to return true for the
//          command to be kept in the undo stack for later undoing
//
//-----------------------------------------------------------------------------

MStatus CVstExampleCmd::doIt( const MArgList &i_mArgList )
{
	// Simply save the argument list and then call redoIt()
	// If everything is done correctly the scene should be EXACTLY the
	// same each time doIt() or redoIt() is run so this is a reasonable
	// and general way to approach undo/redo, i.e. treat each redo like
	// it's being done for the very first time
	// There may be exceptions but this is a good structure to start with

	m_mArgList = i_mArgList;

	return redoIt();
}


//-----------------------------------------------------------------------------
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

MStatus CVstExampleCmd::undoIt()
{
	if ( m_undoable )
	{
		// Undo it... whatever that takes but the scene should be restored
		//            to the exact same state as it was just before doIt()
		//            was called (this includes active selection, etc...)

		if ( m_mDagModifier )
		{
			m_mDagModifier->undoIt();
			delete m_mDagModifier;
			m_mDagModifier = NULL;
		}

		MGlobal::setActiveSelectionList( m_mSelectionList );
		m_mSelectionList.clear();
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

MStatus CVstExampleCmd::redoIt()
{
	MStatus mStatus;

	// Save the current selection just in case we want to undo stuff

	MGlobal::getActiveSelectionList( m_mSelectionList );

	const MArgDatabase mArgDatabase( syntax(), m_mArgList, &mStatus );
	if ( !mStatus )
	{
		setResult( "Cannot parse command line" );
		return MS::kFailure;
	}

	if ( mArgDatabase.numberOfFlagsUsed() == 0 || mArgDatabase.isFlagSet( "help" ) )
	{
		MGlobal::displayInfo( "" );
		MGlobal::displayInfo( " Valve Source Tools Example Maya Plugin Command" );
		MGlobal::displayInfo( "" );
		MGlobal::displayInfo( " -h  | -help          - Display this information" );
		MGlobal::displayInfo( " -c  | -create <type> - Create a node from this plugin where <type> is one of 'locator' or 'transform'" );
		MGlobal::displayInfo( " -n  | -name <string> - When creating a node, name it <string>" );
		MGlobal::displayInfo( "" );
	}
	else
	{
		// Get common arguments
		MString optName;
		if ( mArgDatabase.isFlagSet( "name" ) )
		{
			mArgDatabase.getFlagArgument( "name", 0, optName );
		}

		if ( mArgDatabase.isFlagSet( "create" ) )
		{
			MString createType;
			mArgDatabase.getFlagArgument( "create", 0, createType );
			if ( _strnicmp( "locator", createType.asChar(), 1 ) == 0 )
			{
				// Create an example locator node

				m_undoable = true;
				m_mDagModifier = new MDagModifier;

				MObject tObj;	// Transform MObject
				MObject sObj;	// Shape MObject

				mStatus = vm::CreateDagNode( 
					"vstExampleLocator",
					optName.length() ? optName.asChar() : "vstExampleLocator",
					MObject::kNullObj,
					&tObj,
					&sObj,
					m_mDagModifier);

				if ( !mStatus )
				{
					displayError( "Couldn't create locator" );
					m_mDagModifier->undoIt();
					delete m_mDagModifier;
					m_mDagModifier = NULL;
					m_undoable = false;

					return mStatus;
				}

				MDagPath mDagPath;
				MDagPath::getAPathTo( tObj, mDagPath );
				MGlobal::select( mDagPath, MObject::kNullObj, MGlobal::kReplaceList );

				setResult( mDagPath.partialPathName() );
			}
			else if ( _strnicmp( "transform", createType.asChar(), 1 ) == 0 )
			{
				// Create an example transform node

				m_undoable = true;
				m_mDagModifier = new MDagModifier;

				MObject tObj;	// Transform MObject

				mStatus = vm::CreateDagNode( 
					"vstExampleTransform",
					optName.length() ? optName.asChar() : "vstExampleTransform",
					MObject::kNullObj,
					&tObj,
					NULL,
					m_mDagModifier);

				if ( !mStatus )
				{
					displayError( "Couldn't create transform" );
					m_mDagModifier->undoIt();
					delete m_mDagModifier;
					m_mDagModifier = NULL;
					m_undoable = false;

					return mStatus;
				}

				MDagPath mDagPath;
				MDagPath::getAPathTo( tObj, mDagPath );
				MGlobal::select( mDagPath, MObject::kNullObj, MGlobal::kReplaceList );

				setResult( mDagPath.partialPathName() );
			}
			else
			{
				setResult( MString( "Cannot create Vst Example node of type: " ) + createType );
				return MS::kFailure;
			}
		}
	}

	return MS::kSuccess;
}
