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
#include <maya/MItDag.h>
#include <maya/MSelectionList.h>

// Valve includes

#include "valveMaya.h"
#include "VsMayaMPxFactory.h"

class CVstInfoCmd : public CVsMayaMPxCommand
{
public:
	CVstInfoCmd();

	virtual ~CVstInfoCmd();

	// Inheritied from MPxCommand
	virtual MStatus doIt( const MArgList &i_mArgList );

	virtual MStatus undoIt();

	virtual MStatus redoIt();

	virtual bool isUndoable() const { return m_undoable; }

	virtual bool hasSyntax() const { return true; }

	// Inheritied from CVsMayaMPxCommand
	void SpecifySyntax( MSyntax &mSyntax, ValveMaya::CMSyntaxHelp &mSyntaxHelp );

protected:
	bool m_undoable;					// Is the command undoable or not
	MArgList m_mArgList;				// The stored arg list (for undo/redo)
	MDagModifier *m_mDagModifier;		// The dag modifier (for undo/redo)
	MSelectionList m_mSelectionList;	// The previous selection list (for undo/redo)
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstInfoCmd,
	vstInfo,
	"Creates and manipulates vstInfo nodes"
);


//-----------------------------------------------------------------------------
//
// Purpose: Creator
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

CVstInfoCmd::CVstInfoCmd()
: m_undoable(false)
, m_mDagModifier(NULL)
{
}


//-----------------------------------------------------------------------------
//
// Purpose: Destructor
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

CVstInfoCmd::~CVstInfoCmd()
{
}


//-----------------------------------------------------------------------------
//
// Purpose: MSyntax factory for the command
// Input  :
// Output : A Maya MSyntax class describing command's command line syntax
//
//-----------------------------------------------------------------------------

void CVstInfoCmd::SpecifySyntax( MSyntax &mSyntax, ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntax.addFlag( "h", "help" );
	mSyntax.addFlag( "c", "create" );
	mSyntax.addFlag( "n", "name", MSyntax::kString );
	mSyntax.addFlag( "s", "select" );
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

MStatus CVstInfoCmd::doIt( const MArgList &i_mArgList )
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

MStatus CVstInfoCmd::undoIt()
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

MStatus CVstInfoCmd::redoIt()
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
		static const char *const vstUtilsInfo[] =
		{
			"",
			" Valve Source Tools (vst) Info Command",
			"",
			"  The vstInfo command is used to create and manipulate vstInfo nodes.",
			"",
			" SWITCHES:",
			"",
			"  -h  | -help            - Print this information",
			"  -c  | -create          - Create a vstInfo node",
			"  -n  | -name <string>   - Used with -create, names the created node the specified <string>",
			"  -s  | -select          - Selects all vstInfo nodes in the scene",
			"",
			" EXAMPLES:",
			"",
			" string $vstInfo = `vstInfo -c -n bob`;",
			" print( \"Created Node: \" + $vstInfo + \"\\n\" );",
			""
		};

		for ( int li( 0 ); li != sizeof( vstUtilsInfo ) / sizeof( vstUtilsInfo[0] ); ++li )
		{
			MGlobal::displayInfo( vstUtilsInfo[li]  );
		}
	}
	else if ( mArgDatabase.isFlagSet( "create" ) )
	{
		// Get name command line arg
		MString optName;
		if ( mArgDatabase.isFlagSet( "name" ) )
		{
			mArgDatabase.getFlagArgument( "name", 0, optName );
		}

		m_undoable = true;
		m_mDagModifier = new MDagModifier;

		MObject tObj;	// Transform MObject
		MObject sObj;	// Shape MObject

		mStatus = vm::CreateDagNode(
			"vstInfo",
			optName.length() ? optName.asChar() : "vstInfo",
			MObject::kNullObj,
			&tObj,
			&sObj,
			m_mDagModifier);

		if ( !mStatus )
		{
			displayError( "Couldn't create vstInfo node" );
			m_mDagModifier->undoIt();
			delete m_mDagModifier;
			m_mDagModifier = NULL;
			m_undoable = false;

			return MS::kFailure;
		}

		MDagPath mDagPath;
		MDagPath::getAPathTo( tObj, mDagPath );
		MGlobal::select( mDagPath, MObject::kNullObj, MGlobal::kReplaceList );

		setResult( mDagPath.partialPathName() );
	}
	else if ( mArgDatabase.isFlagSet( "select" ) )
	{
		MSelectionList mSelectionList;
		MDagPath mDagPath;

		for ( MItDag dagIt; !dagIt.isDone(); dagIt.next() )
		{
			if ( MFnDependencyNode( dagIt.item() ).typeName() == "vstInfo" )
			{
				dagIt.getPath( mDagPath );
				mSelectionList.add( mDagPath, MObject::kNullObj, true );
			}
		}

		if ( mSelectionList.length() )
		{
			m_undoable = true;
			MGlobal::setActiveSelectionList( mSelectionList, MGlobal::kReplaceList );
		}
	}

	return MS::kSuccess;
}
