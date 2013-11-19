//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVstWeldCmd
//
//=============================================================================


// Maya includes
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>


// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"
#include "valveMaya/Undo.h"


//-----------------------------------------------------------------------------
// Utility functions found in the node .cpp file
//-----------------------------------------------------------------------------
void GetMeshMatrix( MItMeshPolygon &pIt, int nMeshFaceIndex, MMatrix &geoMatrix  );
void ConnectIntWeld( vm::CUndo &undo, const MObject &weldNodeObj, const MDagPath &toWeldPath, int nType, int nIntValue, const MMatrix &offsetMatrix );


//=============================================================================
//
//=============================================================================
class CVstWeldCmd : public CVsMayaMPxCommand
{
public:
	CVstWeldCmd();

	virtual ~CVstWeldCmd();

	// Inherited from MPxCommand
	virtual MStatus doIt(
		const MArgList &mArgList );

	virtual MStatus undoIt();

	virtual MStatus redoIt();

	virtual bool isUndoable() const { return m_undo.IsUndoable(); }

	virtual bool hasSyntax() const { return true; }

	// Inherited from CVsMayaCommand
	virtual void SpecifySyntax(
		MSyntax &mSyntax,
		ValveMaya::CMSyntaxHelp &mSyntaxHelp);

protected:
	MObject DoCreate();

	static MStatus IsWeldNode( const MObject &iObject );

	static MObject ConnectedToWeldNode( const MDagPath &iDagPath );

	MObject GetWeldNodeFromMesh( const MDagPath &weldToPath );

	MStatus WeldMeshFace( const MDagPath &weldToPath, const MObject &weldToObj, const MSelectionList &toWeldList );

	ValveMaya::CUndo m_undo;			// Undo manager
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstWeldCmd,
	vstWeld,
	"Creates and manipulates vstWeld nodes" );


//-----------------------------------------------------------------------------
// Purpose: Creator
//-----------------------------------------------------------------------------
CVstWeldCmd::CVstWeldCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CVstWeldCmd::~CVstWeldCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
//-----------------------------------------------------------------------------
static const char *const kOptHelp( "h" );
static const char *const kOptName( "n" );


void CVstWeldCmd::SpecifySyntax(
	MSyntax &mSyntax,
	ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "General", "Prints usage information" );

	mSyntaxHelp.AddFlag( mSyntax, kOptName, "name", "Create",
		"If a vstWeld node is created, this will name the vstWeld node" );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( false );
	mSyntax.enableQuery( false );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstWeldCmd::doIt(
	const MArgList &mArgList )
{
	if ( m_undo.SetArgList( syntax(), mArgList ) )
		return redoIt();

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstWeldCmd::undoIt()
{
	m_undo.Undo();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstWeldCmd::redoIt()
{
	if ( m_undo.ArgDatabase().isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
		return MS::kSuccess;
	}

	m_undo.SaveCurrentSelection();

	MSelectionList mSelectionList;
	m_undo.ArgDatabase().getObjects( mSelectionList );

	MSelectionList cleanList;

	for ( MItSelectionList sIt( mSelectionList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() == MItSelectionList::kDagSelectionItem )
		{
			MDagPath mDagPath;
			if ( sIt.hasComponents() )
			{
				MObject cObj;
				sIt.getDagPath( mDagPath, cObj );
				cleanList.add( mDagPath, cObj );
			}
			else
			{
				sIt.getDagPath( mDagPath );
				cleanList.add( mDagPath );
			}
		}
	}

	if ( cleanList.length() < 2 )
	{
		merr << GetName() << " needs at least two things selected or specified" << std::endl;
		return MS::kFailure;
	}

	MDagPath weldToPath;
	MObject weldToObj;

	cleanList.getDagPath( cleanList.length() - 1, weldToPath, weldToObj );
	cleanList.remove( cleanList.length() - 1 );

	uint nShapesBelowCount = 0;
	weldToPath.numberOfShapesDirectlyBelow( nShapesBelowCount );
	for ( uint i = 0; i < nShapesBelowCount; ++i )
	{
		MDagPath tmpDagPath = weldToPath;
		tmpDagPath.extendToShapeDirectlyBelow( 0 );
		if ( !MFnDagNode( tmpDagPath ).isIntermediateObject() )
		{
			weldToPath.extendToShapeDirectlyBelow( i );
			break;
		}
	}

	if ( weldToPath.hasFn( MFn::kMesh ) )
	{
		WeldMeshFace( weldToPath, weldToObj, cleanList );
	}
	else
	{
		merr << GetName() << ": Don't know how to weld anything to a " << weldToPath.node().apiTypeStr() << " - " << weldToPath.partialPathName() << std::endl;
		return MS::kFailure;
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstWeldCmd::DoCreate()
{
	MDagModifier &mDagModifier = m_undo.DagModifier();
	MObject vstWeldNode = mDagModifier.MDGModifier::createNode( "vstWeld" );
	mDagModifier.doIt();

	if ( m_undo.ArgDatabase().isFlagSet( kOptName ) )
	{
		MString optName;
		m_undo.ArgDatabase().getFlagArgument( kOptName, 0, optName );
		mDagModifier.renameNode( vstWeldNode, optName );
	}

	setResult( MFnDependencyNode( vstWeldNode ).name() );

	return vstWeldNode;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstWeldCmd::IsWeldNode(
	const MObject &iObject )
{
	if ( iObject.hasFn( MFn::kPluginDependNode ) && MFnDependencyNode( iObject ).typeName() == "vstWeld" )
		return MS::kSuccess;

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// Returns the vsSkinner node if the passed node is connected to a vsSkinner node
//-----------------------------------------------------------------------------
MObject CVstWeldCmd::ConnectedToWeldNode(
	const MDagPath &iDagPath )
{
	MPlugArray pA;
	MPlugArray pA1;

	if ( MFnDagNode( iDagPath ).getConnections( pA ) && pA.length() )
	{
		MObject mObj;

		const uint np( pA.length() );
		for ( uint i( 0U ); i != np; ++i )
		{
			if ( pA[ i ].connectedTo( pA1, true, true ) && pA1.length() )
			{
				const uint np1( pA1.length() );
				for ( uint j( 0U ); j != np1; ++j )
				{
					mObj = pA1[ j ].node();

					if ( IsWeldNode( mObj ) )
					{
						return mObj;
					}
				}
			}
		}
	}

	return MObject::kNullObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstWeldCmd::GetWeldNodeFromMesh( const MDagPath &weldToPath )
{
	MObject weldNode = ConnectedToWeldNode( weldToPath );

	if ( weldNode.isNull() )
	{
		weldNode = DoCreate();

		m_undo.Connect( MFnDagNode( weldToPath ).findPlug( "worldMesh" ).elementByLogicalIndex( weldToPath.instanceNumber() ), MFnDependencyNode( weldNode ).findPlug( "worldGeometry" ), true );
		m_undo.DagModifier().doIt();
	}

	return weldNode;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstWeldCmd::WeldMeshFace( const MDagPath &weldToPath, const MObject &weldToObj, const MSelectionList &toWeldList )
{
	if ( !weldToObj.isNull() && !weldToObj.hasFn( MFn::kMeshPolygonComponent ) )
	{
		merr << GetName() << ": Can't weld to mesh components of type " << weldToObj.apiTypeStr() << " on mesh " << weldToPath.partialPathName() << std::endl;
		return MS::kFailure;
	}

	int nFaceIndex = 0;

	if ( !weldToObj.isNull() )
	{
		MFnSingleIndexedComponent sFn( weldToObj );
		if ( sFn.elementCount() < 1 )
		{
			merr << GetName() << ": Invalid polygon face list specified for mesh " << weldToPath.partialPathName() << std::endl;
			return MS::kFailure;
		}

		nFaceIndex = sFn.element( 0 );
	}

	MObject weldNode = GetWeldNodeFromMesh( weldToPath );

	if ( weldNode.isNull() )
	{
		merr << GetName() << ": Couldn't create vstWeld node while welding to " << weldToPath.partialPathName() << std::endl;
		return MS::kFailure;
	}

	MFnDependencyNode weldFn( weldNode );

	MFnMesh weldToFn( weldToPath );
	MItMeshPolygon pIt( weldToPath );

	for ( MItSelectionList sIt( toWeldList ); !sIt.isDone(); sIt.next() )
	{
		MDagPath toWeldPath;
		sIt.getDagPath( toWeldPath );
		while ( toWeldPath.length() && toWeldPath.isValid() && !toWeldPath.hasFn( MFn::kTransform ) )
		{
			toWeldPath.pop();
		}

		if ( toWeldPath.length() <= 0 || !toWeldPath.isValid() )
		{
			merr << GetName() << ": Invalid path of a thing to weld while welding to mesh " << weldToPath.partialPathName() << std::endl;
			continue;
		}

		MFnTransform tFn( toWeldPath );
		MPoint tPoint( tFn.getTranslation( MSpace::kWorld ) );
		MPoint cPoint;

		if ( weldToObj.isNull() )
		{
			weldToFn.getClosestPoint( tPoint, cPoint, MSpace::kWorld, &nFaceIndex );
		}

		if ( nFaceIndex < 0 || nFaceIndex >= static_cast< int >( pIt.count() ) )
		{
			merr << GetName() << ": Face index " << nFaceIndex << " is out of range for mesh " << weldToPath.partialPathName() << std::endl;
			continue;
		}

		MMatrix PL = toWeldPath.inclusiveMatrix();
		MMatrix geoMatrix;

		GetMeshMatrix( pIt, nFaceIndex, geoMatrix );

		MMatrix O = PL * geoMatrix.inverse();

		ConnectIntWeld( m_undo, weldNode, toWeldPath, 0 /* Magic... enum in vstWeldNode.cpp */, nFaceIndex, O );
	}

	return MS::kSuccess;
}