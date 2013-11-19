//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVsMasterCmd
//
//=============================================================================

// Maya includes
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <maya/MArgList.h>

#include "vsMayaCommand.h"

//-----------------------------------------------------------------------------
//
// Purpose: A new Maya command dervied from MPxCommand
//
//-----------------------------------------------------------------------------
class CVsMasterCmd : public CVsMayaCommand
{
public:
	CVsMasterCmd();
	virtual ~CVsMasterCmd();

	// Inherited from MPxCommand
	virtual MStatus doIt( const MArgList &argList );
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const { return m_bUndoable; }
	virtual bool hasSyntax() const { return true; }

	// Inherited from CVsMayaCommand
	virtual void SpecifySyntax( MSyntax &syntax );

protected:
	bool m_bUndoable;				// Is the command undoable or not
	MArgList m_argList;				// The stored arg list (for undo/redo)
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_COMMAND( CVsMasterCmd, "vsMaster" );


//-----------------------------------------------------------------------------
// Constructor, destructor
//-----------------------------------------------------------------------------
CVsMasterCmd::CVsMasterCmd() : m_bUndoable(false)
{
}

CVsMasterCmd::~CVsMasterCmd()
{
}


//-----------------------------------------------------------------------------
//
// Purpose: MSyntax factory for the command
// Input  :
// Output : A Maya MSyntax class describing command's command line syntax
//
//-----------------------------------------------------------------------------
void CVsMasterCmd::SpecifySyntax( MSyntax &syntax )
{
	syntax.addFlag( "h", "help" );
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
MStatus CVsMasterCmd::doIt( const MArgList &argList )
{
	// Simply save the argument list and then call redoIt()
	// If everything is done correctly the scene should be EXACTLY the
	// same each time doIt() or redoIt() is run so this is a reasonable
	// and general way to approach undo/redo, i.e. treat each redo like
	// it's being done for the very first time
	// There may be exceptions but this is a good structure to start with

	m_argList = argList;

	return redoIt();
}


//-----------------------------------------------------------------------------
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------
MStatus CVsMasterCmd::undoIt()
{
	if ( m_bUndoable )
	{
		// Undo it... whatever that takes but the scene should be restored
		//            to the exact same state as it was just before doIt()
		//            was called (this includes active selection, etc...)
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
MStatus CVsMasterCmd::redoIt()
{
	MStatus status;

	const MArgDatabase argDatabase( syntax(), m_argList, &status );
	if ( !status )
	{
		setResult( "Cannot parse command line" );
		return MS::kFailure;
	}

	if ( argDatabase.isFlagSet( "help" ) )
	{
		MGlobal::displayInfo( "-help!!!" );
	}

	return MS::kSuccess;
}
