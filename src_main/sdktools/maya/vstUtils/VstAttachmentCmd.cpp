//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVstAttachmentCmd
//
//=============================================================================

// Maya includes

#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>

// Valve includes

#include "valveMaya.h"
#include "VsMayaMPxFactory.h"

// Local includes


class CVstAttachmentCmd : public CVsMayaMPxCommand
{
public:
	CVstAttachmentCmd();

	virtual ~CVstAttachmentCmd();

	// Inheritied from MPxCommand
	virtual MStatus doIt( const MArgList &mArgList );

	virtual MStatus undoIt();

	virtual MStatus redoIt();

	virtual bool isUndoable() const { return m_undoable; }

	virtual bool hasSyntax() const { return true; }

	// Inheritied from CVsMayaCommand
	void SpecifySyntax( MSyntax &mSyntax, ValveMaya::CMSyntaxHelp &mSyntaxHelp);

protected:
	MStatus DoHelp();
	MStatus DoCreate();
	MStatus DoSelect();

	bool m_undoable;					// Is the command undoable or not
	MArgDatabase *m_mArgDatabase;		// The stored arg database (for undo/redo)
	MDagModifier *m_mDagModifier;		// The dag modifier (for undo/redo)
	MSelectionList m_mSelectionList;	// The previous selection list (for undo/redo)
};

//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstAttachmentCmd,
	vstAttachment,
	"Creates vstAttachment nodes which are nodes that act as attachments in the game" );


//-----------------------------------------------------------------------------
// Creator
//-----------------------------------------------------------------------------
CVstAttachmentCmd::CVstAttachmentCmd()
: m_undoable( false )
, m_mArgDatabase( NULL )
, m_mDagModifier( NULL )
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVstAttachmentCmd::~CVstAttachmentCmd()
{
	if ( m_mArgDatabase )
	{
		delete m_mArgDatabase;
		m_mArgDatabase = NULL;
	}

	if ( m_mDagModifier )
	{
		delete m_mDagModifier;
		m_mDagModifier = NULL;
	}
}


//-----------------------------------------------------------------------------
// Fill in the Maya MSyntax class describing command's command line syntax
//-----------------------------------------------------------------------------
static const char *const kOptHelp( "h" );
static const char *const kOptCreate( "c" );
static const char *const kOptName( "n" );
static const char *const kOptParent( "p" );

void CVstAttachmentCmd::SpecifySyntax(
	MSyntax &mSyntax,
	ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "General",
		"Prints this information");

	mSyntaxHelp.AddFlag( mSyntax, kOptCreate, "create", "Create",
		"Creates a new vstAttachment locator" );

	mSyntaxHelp.AddFlag( mSyntax, kOptName, "name", "Create",
		"When creating a new vstAttachment locator, give it the specified name",
		MSyntax::kString );

	mSyntaxHelp.AddFlag( mSyntax, kOptParent, "parent", "Create",
		"When creating a new vstAttachment locator parent it under the first dag node specified" );

	mSyntax.enableEdit( false );
	mSyntax.enableQuery( false );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
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
MStatus CVstAttachmentCmd::doIt(
	const MArgList &mArgList )
{
	MStatus mStatus;
	MArgDatabase *mArgDatabase( new MArgDatabase( syntax(), mArgList, &mStatus ) );

	if ( mArgDatabase ) {
		if ( mStatus )
		{
			m_mArgDatabase = mArgDatabase;
			mStatus = redoIt();
		}
		else
		{
			delete mArgDatabase;
		}
	}

	return mStatus;
}


//-----------------------------------------------------------------------------
// Called when the user undoes the command
//-----------------------------------------------------------------------------
MStatus CVstAttachmentCmd::undoIt()
{
	if ( m_undoable )
	{
		MGlobal::setActiveSelectionList( m_mSelectionList );
		m_mSelectionList.clear();

		if ( m_mDagModifier )
		{
			m_mDagModifier->undoIt();
			delete m_mDagModifier;
			m_mDagModifier = NULL;
		}
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Called in general to actually do the work of the command
//-----------------------------------------------------------------------------
MStatus CVstAttachmentCmd::redoIt()
{
	if ( m_mArgDatabase->isFlagSet( kOptHelp ) )
	{
		return DoHelp();
	}
	else if ( m_mArgDatabase->isFlagSet( "create" ) )
	{
		return DoCreate();
	}
	else if ( m_mArgDatabase->isFlagSet( "select" ) )
	{
		return DoSelect();
	}

	merr << "No valid operation specified via command line arguments" << std::endl;

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// Prints help
//-----------------------------------------------------------------------------
MStatus CVstAttachmentCmd::DoHelp()
{
	GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Creates a vstAttachment Locator
//-----------------------------------------------------------------------------
MStatus CVstAttachmentCmd::DoCreate()
{
	MDagModifier *mDagModifier( new MDagModifier );

	if ( !mDagModifier )
	{
		merr << "Can't create new MDagModifier" << std::endl;
		return MS::kFailure;
	}

	MString optName( "vstAttachment" );
	if ( m_mArgDatabase->isFlagSet( kOptName ) )
	{
		m_mArgDatabase->getFlagArgument( kOptName, 0, optName );
	}

	// Create the helper bone locator's transform
	MObject xObj = mDagModifier->createNode( "transform" );
	mDagModifier->doIt();

	if ( xObj.isNull() )
	{
		merr << "Can't create new transform node" << std::endl;
		return MS::kFailure;
	}

	// name the shape & the transform the same thing
	mDagModifier->renameNode( xObj, optName );
	mDagModifier->doIt();

	MObject vstAttachmentObj = mDagModifier->createNode( "vstAttachment", xObj );

	if ( vstAttachmentObj.isNull() )
	{
		merr << "Can't create new vstAttachment node" << std::endl;
		mDagModifier->undoIt();
		return MS::kFailure;
	}

	// name the shape & the transform the same thing
	mDagModifier->renameNode( vstAttachmentObj, MFnDependencyNode( xObj ).name() );
	mDagModifier->doIt();

	m_undoable = true;
	m_mDagModifier = mDagModifier;

	if ( m_mArgDatabase->isFlagSet( kOptParent ) )
	{
		MSelectionList mSelectionList;
		m_mArgDatabase->getObjects( mSelectionList );
		for ( MItSelectionList sIt( mSelectionList, MFn::kDagNode ); !sIt.isDone(); sIt.next() )
		{
			MDagPath mDagPath;
			if ( sIt.getDagPath( mDagPath ) )
			{
				m_mDagModifier->reparentNode( xObj, mDagPath.node() );
				m_mDagModifier->doIt();
				break;
			}
		}
	}

	// Save the current selection just in case we want to undo stuff
	MGlobal::getActiveSelectionList( m_mSelectionList );

	MDagPath xDagPath;
	MDagPath::getAPathTo( xObj, xDagPath );
	MGlobal::select( xDagPath, MObject::kNullObj, MGlobal::kReplaceList );
	setResult( xDagPath.partialPathName() );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Selects all vstAttachment nodes in the scene
//-----------------------------------------------------------------------------
MStatus CVstAttachmentCmd::DoSelect()
{
	MSelectionList mSelectionList;
	MDagPath mDagPath;

	for ( MItDag dagIt; !dagIt.isDone(); dagIt.next() )
	{
		if ( MFnDependencyNode( dagIt.item() ).typeName() == "vstAttachment" )
		{
			dagIt.getPath( mDagPath );
			mSelectionList.add( mDagPath, MObject::kNullObj, true );
		}
	}

	if ( mSelectionList.length() )
	{
		// Save the current selection just in case we want to undo stuff
		MGlobal::getActiveSelectionList( m_mSelectionList );
		MGlobal::setActiveSelectionList( mSelectionList, MGlobal::kReplaceList );
		m_undoable = true;
	}

	return MS::kSuccess;
}