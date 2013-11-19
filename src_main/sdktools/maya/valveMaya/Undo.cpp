//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Maya Undo helper
//
//=============================================================================


// Maya includes
#include <maya/MGlobal.h>
#include <maya/MFnTransform.h>
#include <maya/MPlugArray.h>


// Valve includes
#include <ValveMaya/Undo.h>


namespace ValveMaya
{

//=============================================================================
//
//
// CUndo - Main Undo Class
//
//
//=============================================================================


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CUndo::CUndo()
: m_pArgDatabase( NULL )
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CUndo::~CUndo()
{
	if ( m_pArgDatabase )
	{
		delete m_pArgDatabase;
		m_pArgDatabase = NULL;
	}

	Clear();
}


//-----------------------------------------------------------------------------
// Clear everything out of the undo stack but leave the arg database intact
// this is for a lot of undo/redo cycles
//-----------------------------------------------------------------------------
void CUndo::Clear()
{
	while ( m_undoStack.Count() )
	{
		delete m_undoStack.Top();
		m_undoStack.Pop();
	}
}


//-----------------------------------------------------------------------------
// Set the MArgList
//-----------------------------------------------------------------------------
MStatus CUndo::SetArgList(
	const MSyntax &mSyntax,
	const MArgList &mArgList )
{
	MStatus mStatus;
	if ( m_pArgDatabase )
	{
		delete m_pArgDatabase;
		m_pArgDatabase = NULL;
	}

	m_pArgDatabase = new MArgDatabase( mSyntax, mArgList, &mStatus );
	if ( !mStatus )
	{
		if ( m_pArgDatabase )
		{
			delete m_pArgDatabase;
			m_pArgDatabase = NULL;
		}
	}

	return mStatus;
}


//-----------------------------------------------------------------------------
// Get the current MArgDatabase
//-----------------------------------------------------------------------------
const MArgDatabase &CUndo::ArgDatabase()
{
	if ( !m_pArgDatabase )
	{
		m_pArgDatabase = new MArgDatabase;
	}

	return *m_pArgDatabase;
}


//-----------------------------------------------------------------------------
// Push a current selection undo operation onto the undo stack
//-----------------------------------------------------------------------------
void CUndo::SaveCurrentSelection()
{
	Push( new CUndoOpSelection );
}


//-----------------------------------------------------------------------------
// Return a shared pointer to the dag modifier on the top of the undo stack
// If the undo operation at the top of the stack is a dag modifier operation
// use it, otherwise create a new dag modifier operation, push and return it
//-----------------------------------------------------------------------------
MDagModifier &CUndo::DagModifier()
{
	CUndoOpDagModifier *pUndoDagMod( NULL );

	if ( m_undoStack.Count() )
	{
		pUndoDagMod = dynamic_cast< CUndoOpDagModifier * >( m_undoStack.Top() );
	}

	if ( !pUndoDagMod )
	{
		Push( new CUndoOpDagModifier );
		pUndoDagMod = dynamic_cast< CUndoOpDagModifier * >( m_undoStack.Top() );
	}

	return pUndoDagMod->m_mDagModifier;
}


//-----------------------------------------------------------------------------
// If the undo operation on the top of the undo stack is a dag modifier
// operation, call the doIt() method on it, otherwise do nothing
//-----------------------------------------------------------------------------
MStatus CUndo::DagModifierDoIt()
{
	if ( m_undoStack.Count() )
	{
		CUndoOpDagModifier *pUndoDagMod( dynamic_cast< CUndoOpDagModifier * >( m_undoStack.Top() ) );

		if ( pUndoDagMod )
			return pUndoDagMod->m_mDagModifier.doIt();
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Connects two plugs, forcibly disconnecting any existing connection to the
// destination plug
//-----------------------------------------------------------------------------
MStatus CUndo::Connect(
	const MPlug &srcP,
	const MPlug &dstP,
	bool force )
{
	MDagModifier &dagMod( DagModifier() );

	if ( force )
	{
		MPlugArray pA;
		if ( dstP.connectedTo( pA, true, false ) && pA.length() )
		{
			dagMod.disconnect( pA[ 0 ], dstP );
			dagMod.doIt();
		}
	}

	dagMod.connect( srcP, dstP );
	return dagMod.doIt();
}


//-----------------------------------------------------------------------------
// If the given value is different from the current value, change the
// value and push a CUndoOpSetAttr instance onto the undo stack
// Return true if the value was actually changed or not
//-----------------------------------------------------------------------------
bool CUndo::SetAttr(
	MPlug &mPlug,
	MObject &val )
{
	MObject oldVal;
	mPlug.getValue( oldVal );

	if ( val == oldVal )
		return false;

	Push( new CUndoOpSetAttr( mPlug, val ) );
	return true;
}


//-----------------------------------------------------------------------------
// If the given value is different from the current value, change the
// value and push a CUndoOpSetAttr instance onto the undo stack
// Return true if the value was actually changed or not
//-----------------------------------------------------------------------------
bool CUndo::SetAttr(
	MPlug &mPlug,
	double val )
{
	double oldVal( 0.0 );
	mPlug.getValue( oldVal );

	if ( val == oldVal )
		return false;

	Push( new CUndoOpSetAttr( mPlug, val ) );
	return true;
}


//-----------------------------------------------------------------------------
// Lock or unlock the given plug
// Return true if the lock state of the plug was changed
//-----------------------------------------------------------------------------
bool CUndo::Lock(
	MPlug &mPlug,
	bool lock )
{
	const bool isLocked( mPlug.isLocked() );
	if ( lock == isLocked )
		return false;

	Push( new CUndoOpLock( mPlug, lock ) );
	return true;
}


//-----------------------------------------------------------------------------
// Push the given shared pointer onto the undo operation stack
//-----------------------------------------------------------------------------
void CUndo::Push(
	CUndoOp *pUndoOp )
{
	if ( pUndoOp )
	{
		m_undoStack.Push( pUndoOp );
	}
}


//-----------------------------------------------------------------------------
// Undo all of the operations on the undo stack and clean out the stack
//-----------------------------------------------------------------------------
bool CUndo::IsUndoable() const
{
	return( m_undoStack.Count() != 0 );
}


//-----------------------------------------------------------------------------
// Undo all of the operations on the undo stack and clean out the stack
//-----------------------------------------------------------------------------
void CUndo::Undo()
{
	while ( m_undoStack.Count() )
	{
		m_undoStack.Top()->Undo();
		delete m_undoStack.Top();
		m_undoStack.Pop();
	}
}


//=============================================================================
//
//
// CUndoOpSetAttr: Undo stack member for setting attributes
//
//
//=============================================================================

//-----------------------------------------------------------------------------
// Construct from a plug and an MObject value
//-----------------------------------------------------------------------------
CUndoOpSetAttr::CUndoOpSetAttr(
	MPlug &mPlug,
	MObject &mObjectVal )
: m_mPlug( mPlug )
, m_numericVal( 0.0 )
, m_numeric( false )
{
	m_mPlug.getValue( m_mObjectVal );

	const bool locked( m_mPlug.isLocked() );

	if ( locked )
	{
		m_mPlug.setLocked( false );
	}

	m_mPlug.setValue( mObjectVal );

	if ( locked )
	{
		m_mPlug.setLocked( true );
	}
}


//-----------------------------------------------------------------------------
// Construct from a plug and a numeric value
//-----------------------------------------------------------------------------
CUndoOpSetAttr::CUndoOpSetAttr(
  MPlug &mPlug,
  double numericVal )
: m_mPlug( mPlug )
, m_numeric( true )
{
	m_mPlug.getValue( m_numericVal );

	const bool locked( m_mPlug.isLocked() );

	if ( locked )
	{
		m_mPlug.setLocked( false );
	}

	m_mPlug.setValue( numericVal );

	if ( locked )
	{
		m_mPlug.setLocked( true );
	}
}


//-----------------------------------------------------------------------------
// Undo SetAttr
//-----------------------------------------------------------------------------
void CUndoOpSetAttr::Undo()
{
	const bool locked( m_mPlug.isLocked() );

	if ( locked )
	{
		m_mPlug.setLocked( false );
	}

	if ( m_numeric )
	{
		m_mPlug.setValue( m_numericVal );
	}
	else
	{
		m_mPlug.setValue( m_mObjectVal );
	}

	if ( locked )
	{
		m_mPlug.setLocked( true );
	}
}


//=============================================================================
//
//
// CUndoOpSelection: Undo stack member for changing selection
//
//
//=============================================================================


//-----------------------------------------------------------------------------
// Constructor, save current active selection
//-----------------------------------------------------------------------------
CUndoOpSelection::CUndoOpSelection()
{
	MGlobal::getActiveSelectionList( m_mSelectionList );
}


//-----------------------------------------------------------------------------
// Undo, restored saved selection
//-----------------------------------------------------------------------------
void
CUndoOpSelection::Undo()
{
	MGlobal::setActiveSelectionList( m_mSelectionList, MGlobal::kReplaceList );
}


//=============================================================================
//
//
// CUndoOpLock: Undo stack member for locking and unlocking attributes
//
//
//=============================================================================


//-----------------------------------------------------------------------------
// Constructor, save plug's lock state
//-----------------------------------------------------------------------------
CUndoOpLock::CUndoOpLock(
	MPlug &mPlug,
	bool lock )
: m_mPlug( mPlug )
, m_locked( mPlug.isLocked() )
{
	m_mPlug.setLocked( lock );
}


//-----------------------------------------------------------------------------
// Constructor, save plug's lock state
//-----------------------------------------------------------------------------
void CUndoOpLock::Undo()
{
	m_mPlug.setLocked( m_locked );
}


//=============================================================================
//
//
// CUndoOpResetRestPosition: Undo stack member for locking and unlocking attributes
//
//
//=============================================================================


//-----------------------------------------------------------------------------
// Constructor, save plug's lock state
//-----------------------------------------------------------------------------
CUndoOpResetRestPosition::CUndoOpResetRestPosition(
	const MDagPath &mDagPath )
: m_mDagPath( mDagPath )
, m_matrix( MFnTransform( mDagPath ).restPosition( NULL ) )
{
	MFnTransform tFn( m_mDagPath );
	tFn.clearRestPosition();
	tFn.resetFromRestPosition();
}


//-----------------------------------------------------------------------------
// Constructor, save plug's lock state
//-----------------------------------------------------------------------------
void CUndoOpResetRestPosition::Undo()
{
	MFnTransform tFn( m_mDagPath );
	tFn.clearRestPosition();
	tFn.setRestPosition( m_matrix );
	tFn.resetFromRestPosition();
}


}