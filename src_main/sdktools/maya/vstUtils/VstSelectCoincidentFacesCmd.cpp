//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVstSelectCoincidentFacesCmd
//
//=============================================================================

// Standard includes


// Maya includes
#include <maya/MDistance.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MIntArray.h>
#include <maya/MItDag.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSelectionList.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>


// Valve includes
#include "valveMaya.h"
#include "valveMaya/Undo.h"
#include "VsMayaMPxFactory.h"


//=============================================================================
//
//=============================================================================
class CVstSelectCoincidentFacesCmd : public CVsMayaMPxCommand
{
public:
	CVstSelectCoincidentFacesCmd();

	virtual ~CVstSelectCoincidentFacesCmd();

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
	MStatus DoSelect();

	void GetSpecifiedMeshes(
		MSelectionList &meshList );

	ValveMaya::CUndo m_undo;
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstSelectCoincidentFacesCmd,
	vstSelectCoincidentFaces,
	"Selects all coincident faces in the specified meshes.\n"
	"Simply select the meshes or mesh faces you want to check for coincidence and run the command.\n"
	"Select as many as required on as many different meshes as required and run the command.\n"
	"Note that only faces on the same meshes are checked against each other for coincidence.\n" );


//-----------------------------------------------------------------------------
// Purpose: Creator
//-----------------------------------------------------------------------------
CVstSelectCoincidentFacesCmd::CVstSelectCoincidentFacesCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CVstSelectCoincidentFacesCmd::~CVstSelectCoincidentFacesCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
// Output : A Maya MSyntax class describing command's command line syntax
//-----------------------------------------------------------------------------

static const char *const kOptHelp( "h" );
static const char *const kOptTolerance( "t" );

void CVstSelectCoincidentFacesCmd::SpecifySyntax(
	MSyntax &mSyntax,
	ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "General", "Prints usage information" );

	mSyntaxHelp.AddFlag( mSyntax, kOptTolerance, "tolerance", "General",
		"Specifies the tolerance distance between vertices for them to be considered the same vertex",
		MSyntax::kDistance );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( false );
	mSyntax.enableQuery( false );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstSelectCoincidentFacesCmd::doIt( const MArgList &mArgList )
{
	if ( m_undo.SetArgList( syntax(), mArgList ) )
		return redoIt();

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstSelectCoincidentFacesCmd::undoIt()
{
	m_undo.Undo();
	m_undo.Clear();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstSelectCoincidentFacesCmd::redoIt()
{
	MStatus mStatus;

	if ( !mStatus )
	{
		setResult( MString( "Cannot parse command line" ) + mStatus.errorString() );
		return MS::kFailure;
	}

	const MArgDatabase &mArgDatabase( m_undo.ArgDatabase() );

	m_undo.SaveCurrentSelection();

	if ( mArgDatabase.isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
		return MS::kSuccess;
	}

	return DoSelect();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstSelectCoincidentFacesCmd::DoSelect()
{
	MSelectionList meshList;
	GetSpecifiedMeshes( meshList );

	MSelectionList coincidentList;

	MDagPath mDagPath;
	MObject cObj;

	MPointArray points;
	MIntArray iIndexes;
	MIntArray jIndexes;

	uint iCount;
	bool addI;
	bool same;
	bool foundVertex;

	double tolerance( MPoint_kTol );
	if ( m_undo.ArgDatabase().isFlagSet( kOptTolerance ) )
	{
		MDistance optTolerance;
		m_undo.ArgDatabase().getFlagArgument( kOptTolerance, 0U, optTolerance );
		tolerance = optTolerance.as( MDistance::internalUnit() );
	}

	for ( MItSelectionList sIt( meshList ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( mDagPath, cObj ) )
			continue;

		MFnSingleIndexedComponent sFn;
		MObject sObj( sFn.create( MFn::kMeshPolygonComponent ) );

		MFnMesh meshFn( mDagPath );
		meshFn.getPoints( points );

		if ( !sIt.hasComponents() )
		{
			const uint nFaces( meshFn.numPolygons() );
			for ( uint i( 0U ); i != nFaces; ++i )
			{
				meshFn.getPolygonVertices( i, iIndexes );
				iCount = iIndexes.length();

				addI = false;

				for ( uint j( i + 1 ); j < nFaces; ++j )
				{
					meshFn.getPolygonVertices( j, jIndexes );
					if ( jIndexes.length() == iCount )
					{
						same = true;

						for ( uint k( 0U ); k != iCount; ++k )
						{
							foundVertex = false;

							const MPoint &kPoint( points[ iIndexes[ k ] ] );

							for ( uint l( 0U ); l < iCount; ++l )
							{
								if ( kPoint.isEquivalent( points[ jIndexes[ l ] ], tolerance ) )
								{
									foundVertex = true;
									break;
								}
							}

							if ( !foundVertex )
							{
								same = false;
								break;
							}
						}

						if ( same )
						{
							addI = true;
							sFn.addElement( j );
						}
					}
				}

				if ( addI )
				{
					sFn.addElement( i );
				}
			}
		}
		else
		{
			MFnSingleIndexedComponent cFn( cObj );
			MIntArray cA;
			MFnSingleIndexedComponent( cObj ).getElements( cA );

			const uint nFaces( cA.length() );

			for ( uint i( 0U ); i != nFaces; ++i )
			{
				meshFn.getPolygonVertices( cA[ i ], iIndexes );
				iCount = iIndexes.length();

				addI = false;

				for ( uint j( i + 1U ); j < nFaces; ++j )
				{
					meshFn.getPolygonVertices( cA[ j ], jIndexes );
					if ( jIndexes.length() == iCount )
					{
						same = true;

						for ( uint k( 0U ); k != iCount; ++k )
						{
							foundVertex = false;

							const MPoint &kPoint( points[ iIndexes[ k ] ] );

							for ( uint l( 0U ); l < iCount; ++l )
							{
								if ( kPoint.isEquivalent( points[ jIndexes[ l ] ], tolerance ) )
								{
									foundVertex = true;
									break;
								}
							}

							if ( !foundVertex )
							{
								same = false;
								break;
							}
						}

						if ( same )
						{
							addI = true;
							sFn.addElement( cA[ j ] );
						}
					}
				}

				if ( addI )
				{
					sFn.addElement( cA[ i ] );
				}
			}
		}

		if ( sFn.elementCount() > 0 )
		{
			coincidentList.add( mDagPath, sObj );
		}
		else
		{
			MSelectionList tmpList;
			tmpList.add( mDagPath, cObj );
			MStringArray tmpA;
			tmpList.getSelectionStrings( tmpA );

			minfo << "No coincident faces on:";
			for ( uint i( 0U ); i != tmpA.length(); ++i )
			{
				minfo << " " << tmpA[ i ];
			}
			minfo << std::endl;
		}
	}

	if ( coincidentList.length() )
	{
		MGlobal::setActiveSelectionList( coincidentList );
		MStringArray tmpA;
		coincidentList.getSelectionStrings( tmpA );
		setResult( tmpA );
	}
	else
	{
		if ( meshList.length() > 0U )
		{
			minfo << "No coincident faces found" << std::endl;
		}
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstSelectCoincidentFacesCmd::GetSpecifiedMeshes(
	MSelectionList &meshList )
{
	meshList.clear();

	MSelectionList optSelectionList;
	m_undo.ArgDatabase().getObjects( optSelectionList );

	MDagPath mDagPath;
	MObject cObj;
	for ( MItSelectionList sIt( optSelectionList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() == MItSelectionList::kDagSelectionItem && sIt.getDagPath( mDagPath, cObj ) )
		{
			if ( mDagPath.hasFn( MFn::kMesh ) )
			{
				if ( sIt.hasComponents() || !cObj.isNull() )
				{
					meshList.add( mDagPath, cObj );
				}
				else
				{
					mDagPath.extendToShapeDirectlyBelow( 0U );
					meshList.add( mDagPath, MObject::kNullObj, true );
				}
			}
		}
	}

	if ( meshList.isEmpty() )
	{
		for ( MItDag dIt( MItDag::kDepthFirst, MFn::kMesh ); !dIt.isDone(); dIt.next() )
		{
			if ( dIt.getPath( mDagPath ) )
			{
				meshList.add( mDagPath, MObject::kNullObj, true );
			}
		}
	}
}