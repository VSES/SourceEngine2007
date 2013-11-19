//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVsSkinnerCmd
//
//=============================================================================

#pragma warning(disable : 4702)   // warning C4702: unreachable code

// Standard includes
#include <set>
#include <list>


// Maya includes
#include <maya/MAngle.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MDistance.h>
#include <maya/MGlobal.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnSet.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>

// Debug
#include <maya/MFnMesh.h>
#include <maya/MColorArray.h>


// Valve includes
#include "valveMaya.h"
#include "valveMaya/Undo.h"


// Local includes
#include "vsSkinnerNode.h"


class CVsSkinnerCmd : public CVsMayaMPxCommand
{
public:
	CVsSkinnerCmd();

	virtual ~CVsSkinnerCmd();

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
	MStatus DoCreate();

	void DoConnectGeometry(
		const MDagPath &skinnerPath,
		const MDagPath &gDagPath );

	MObject DoNewVolume(
		const MDagPath &skinnerPath,
		const MDagPath &pDagPath );

	MSelectionList DoNewVolumes(
		const MDagPath &skinnerPath,
		const MSelectionList &skeletonList );

	MStatus DoAttachMesh(
		const MDagPath &skinnerPath,
		const MDagPath &meshPath );

	MStatus DoDetachMesh(
		const MDagPath &mDagPath );

	MStatus DoLs();

	MDagPath GetSpecifiedMesh();

	static MStatus GetSpecifiedMeshes(
		const MSelectionList &iList,
		MSelectionList &oList );

	static MStatus FindMeshesInHierarchy(
		const MDagPath &mDagPath,
		MSelectionList &oList );

	static MStatus ConnectedToMesh(
		const MDagPath &iDagPath,
		MDagPath &oDagPath );

	MDagPath GetSpecifiedSkinnerNode();

	static MStatus GetSpecifiedSkinnerNodes(
		const MSelectionList &iList,
		MSelectionList &oList );

	static MStatus FindSkinnerNodesInHierarchy(
		const MDagPath &mDagPath,
		MSelectionList &oList );

	static MStatus IsSkinnerNode(
		const MObject &iObject );

	static MStatus IsSkinnerNode(
		const MDagPath &iDagPath );

	static MStatus ConnectedToSkinnerNode(
		const MDagPath &iDagPath,
		MDagPath &oDagPath );

	ValveMaya::CUndo m_undo;			// Undo manager
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVsSkinnerCmd,
	vsSkinner,
	"Creates and manipulates vsSkinner nodes" );


//-----------------------------------------------------------------------------
// Purpose: Creator
//-----------------------------------------------------------------------------
CVsSkinnerCmd::CVsSkinnerCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CVsSkinnerCmd::~CVsSkinnerCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
// Output : A Maya MSyntax class describing command's command line syntax
//-----------------------------------------------------------------------------

static const char *const kOptHelp( "h" );
static const char *const kOptCreate( "c" );
static const char *const kOptName( "n" );
static const char *const kOptNewVolume( "nv" );
static const char *const kOptFalloff( "f" );
static const char *const kOptStrength( "s" );
static const char *const kOptAttachMesh( "am" );
static const char *const kOptDetachMesh( "dm" );
static const char *const kOptBind( "b" );
static const char *const kOptLs( "ls" );
static const char *const kOptSelected( "sl" );
static const char *const kOptLong( "l" );


void CVsSkinnerCmd::SpecifySyntax(
	MSyntax &mSyntax,
	ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "General", "Prints usage information" );

	mSyntaxHelp.AddFlag( mSyntax, kOptCreate, "create", "Create",
		"Creates a new vsSkinner node" );

	mSyntaxHelp.AddFlag( mSyntax, kOptBind, "bind", "Create",
		"Binds the created node to either the specified joint hierarchy or the selected joints. Specifiy h or s for hierarchy or selected." );

	mSyntaxHelp.AddFlag( mSyntax, kOptName, "name", "Create",
		"Names the created vsSkinner node the specified name" );

	mSyntaxHelp.AddFlag( mSyntax, kOptNewVolume, "newVolume", "Edit",
		"Creates a new volume on the specified vsSkinner node using the specified transform as the parent.  The vsSkinner node must be specified first the parent transform second." );

	mSyntaxHelp.AddFlag( mSyntax, kOptFalloff, "falloff", "Edit",
		"Specifies the falloff of a created volume, defaults to 0.25",
		MSyntax::kDouble );

	mSyntaxHelp.AddFlag( mSyntax, kOptStrength, "strength", "Edit",
		"Specifies the strength of a created volume, defaults to 1.0",
		MSyntax::kDouble );

	mSyntaxHelp.AddFlag( mSyntax, kOptAttachMesh, "attachMesh", "Edit",
		"Attaches the input mesh to the node for previewing via the vsSkinner color set" );

	mSyntaxHelp.AddFlag( mSyntax, kOptDetachMesh, "detachMesh", "Edit",
		"Detaches the preview attributes of input mesh from the node" );

	mSyntaxHelp.AddFlag( mSyntax, kOptLs, "list", "Query",
		"List the vsSkinner nodes in the scene or if -sl/-selected is specified, derived from the specified nodes" );

	mSyntaxHelp.AddFlag( mSyntax, kOptSelected, "selected", "Query",
		"Modifies the behavior of -ls/-list so only the specified or selected nodes are taken into account" );

	mSyntaxHelp.AddFlag( mSyntax, kOptLong, "long", "Query",
		"Modifies the behavior of -ls/-list so long node pathnames are returned.  If -long isn't specified, the shortest unique pathname is returned." );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( false );
	mSyntax.enableQuery( false );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::doIt(
	const MArgList &mArgList )
{
	if ( m_undo.SetArgList( syntax(), mArgList ) )
		return redoIt();

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::undoIt()
{
	m_undo.Undo();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::redoIt()
{
	if ( m_undo.ArgDatabase().isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
		return MS::kSuccess;
	}

	m_undo.SaveCurrentSelection();

	if ( m_undo.ArgDatabase().isFlagSet( kOptCreate ) )
	{
		return DoCreate();
	}
	else if ( m_undo.ArgDatabase().isFlagSet( kOptNewVolume ) )
	{
		MSelectionList optSelectionList;
		m_undo.ArgDatabase().getObjects( optSelectionList );
		MSelectionList volumeList( DoNewVolumes( GetSpecifiedSkinnerNode(), optSelectionList ) );
		if ( volumeList.length() == 0 )
		{
			merr << "Couldn't create new volumes" << std::endl;
			return MS::kFailure;
		}
		MGlobal::setActiveSelectionList( volumeList, MGlobal::kReplaceList );
		MStringArray vA;
		volumeList.getSelectionStrings( vA );
		setResult( vA );

		return MS::kSuccess;
	}
	else if ( m_undo.ArgDatabase().isFlagSet( kOptAttachMesh ) )
	{
		return DoAttachMesh( GetSpecifiedSkinnerNode(), GetSpecifiedMesh() );
	}
	else if ( m_undo.ArgDatabase().isFlagSet( kOptDetachMesh ) )
	{
		return DoDetachMesh( GetSpecifiedSkinnerNode() );
	}
	else if ( m_undo.ArgDatabase().isFlagSet( kOptLs ) )
	{
		return DoLs();
	}

	merr << GetName() << ": No action specified" << std::endl;

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::DoCreate()
{
	// Get name command line arg
	MString optName;
	if ( m_undo.ArgDatabase().isFlagSet( kOptName ) )
	{
		m_undo.ArgDatabase().getFlagArgument( kOptName, 0, optName );
	}

	// Create vsSkinner node
	MObject stObj;			// Skinner transform object
	MObject vsSkinnerObj;	// Skinner shape object

	MDagModifier &mDagModifier( m_undo.DagModifier() );

	if ( vm::CreateDagNode(
		"vsSkinner",
		optName.length() ? optName.asChar() : GetName().asChar(),
		MObject ::kNullObj,
		&stObj,
		&vsSkinnerObj,
		&mDagModifier ) != MS::kSuccess )
	{
		displayError( MString( "Couldn't create " ) + GetName() + " transform node" );
		m_undo.Undo();

		return MS::kFailure;
	}

	MDagPath stPath;
	MDagPath::getAPathTo( stObj, stPath );
	setResult( stPath.partialPathName() );

	MDagPath skinnerPath;
	MDagPath::getAPathTo( vsSkinnerObj, skinnerPath );

	MSelectionList optSelectionList;
	m_undo.ArgDatabase().getObjects( optSelectionList );
	MSelectionList volumeList( DoNewVolumes( skinnerPath, optSelectionList ) );

	if ( volumeList.length() )
	{
		MGlobal::setActiveSelectionList( volumeList, MGlobal::kReplaceList );
	}
	else
	{
		MGlobal::select( stPath, MObject::kNullObj, MGlobal::kReplaceList );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// This is here for reference. It isn't currently used
//-----------------------------------------------------------------------------
void CVsSkinnerCmd::DoConnectGeometry(
	const MDagPath &skinnerPath,
	const MDagPath &gDagPath )
{
	MObject skinnerObj( skinnerPath.node() );
	const MFnDependencyNode vsSkinnerFn( skinnerObj );
	const MFnMesh meshFn( gDagPath );

	// Connect things up

	MPlug sP;
	MPlug dP;

	MDagModifier &mDagModifier( m_undo.DagModifier() );

	// Connect mesh.worldMatrix[#] vsSkinner.geometryWorldMatrix
	sP = meshFn.findPlug( "worldMatrix" );
	sP = sP.elementByLogicalIndex( gDagPath.instanceNumber() );
	dP = vsSkinnerFn.findPlug( "geometryWorldMatrix" );
	mDagModifier.connect( sP, dP );

	sP = meshFn.findPlug( "outMesh" );
	dP = vsSkinnerFn.findPlug( "inputGeometry" );
	mDagModifier.connect( sP, dP );

	mDagModifier.doIt();

	// Add the default volume
	MSelectionList emptyList;
	MGlobal::setActiveSelectionList( DoNewVolumes( skinnerPath, emptyList ), MGlobal::kReplaceList );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVsSkinnerCmd::DoNewVolume(
	const MDagPath &skinnerPath,
	const MDagPath &pDagPath )
{
	if ( !IsSkinnerNode( skinnerPath ) )
		return MObject::kNullObj;

	CVsSkinnerNode *pSkinnerNode( reinterpret_cast< CVsSkinnerNode * >( MFnDagNode( skinnerPath ).userNode() ) );

	double strength( 1.0 );
	if ( m_undo.ArgDatabase().isFlagSet( kOptStrength ) )
	{
		m_undo.ArgDatabase().getFlagArgument( kOptStrength, 0, strength );
	}

	double falloff( 0.25 );
	if ( m_undo.ArgDatabase().isFlagSet( kOptFalloff ) )
	{
		m_undo.ArgDatabase().getFlagArgument( kOptFalloff, 0, falloff );
	}

	return pSkinnerNode->NewVolume( strength, falloff, pDagPath, m_undo );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MSelectionList CVsSkinnerCmd::DoNewVolumes(
	const MDagPath &skinnerPath,
	const MSelectionList &skeletonList )
{
	MSelectionList retList;

	const bool optSelected( m_undo.ArgDatabase().isFlagSet( kOptSelected ) );

	MSelectionList optSelection;
	m_undo.ArgDatabase().getObjects( optSelection );

	// TODO: Maybe some fancier logic to only create volumes on joints that make sense?
	//		 Perhaps the ol' has children but no shapes gag?  Watch out for vstHelperBones!

	MDagPath mDagPath;
	for ( MItSelectionList sIt( optSelection ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() == MItSelectionList::kDagSelectionItem && sIt.getDagPath( mDagPath ) && mDagPath.hasFn( MFn::kTransform ) )
		{
			if ( optSelected )
			{
				MObject cObj( DoNewVolume( skinnerPath, mDagPath ) );
				if ( cObj.isNull() )
				{
					mwarn << "Couldn't create new volume on " << skinnerPath.partialPathName()
						<< " using " << mDagPath.partialPathName() << " as a parent" << std::endl;
				}
				else
				{
					retList.add( skinnerPath, cObj, true );
				}
			}
			else
			{
				MItDag dIt;
				for ( dIt.reset( mDagPath ); !dIt.isDone(); dIt.next() )
				{
					dIt.getPath( mDagPath );

					if ( mDagPath.childCount() )
					{
						uint nShapes( 0 );
						mDagPath.numberOfShapesDirectlyBelow( nShapes );

						if ( nShapes == 0U || mDagPath.hasFn( MFn::kJoint ) )
						{
							MObject cObj( DoNewVolume( skinnerPath, mDagPath ) );
							if ( cObj.isNull() )
							{
								mwarn << "Couldn't create new volume on " << skinnerPath.partialPathName()
									<< " using " << mDagPath.partialPathName() << " as a parent" << std::endl;
							}
							else
							{
								retList.add( skinnerPath, cObj, true );
							}
						}
					}
				}
			}
		}
	}

	return retList;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::DoAttachMesh(
	const MDagPath &skinnerPath,
	const MDagPath &meshPath )
{
	if ( !IsSkinnerNode( skinnerPath ) )
		return MS::kFailure;

	CVsSkinnerNode *pSkinnerNode( reinterpret_cast< CVsSkinnerNode * >( MFnDagNode( skinnerPath ).userNode() ) );
	return pSkinnerNode->AttachMesh( meshPath, m_undo.DagModifier() );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::DoDetachMesh(
	const MDagPath &mDagPath )
{
	if ( !IsSkinnerNode( mDagPath ) )
		return MS::kFailure;

	CVsSkinnerNode *pSkinnerNode( reinterpret_cast< CVsSkinnerNode * >( MFnDagNode( mDagPath ).userNode() ) );
	return pSkinnerNode->DetachMesh( m_undo.DagModifier() );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::DoLs()
{
	MSelectionList tmpList;

	if ( m_undo.ArgDatabase().isFlagSet( kOptSelected ) )
	{
		MSelectionList skinnerNodes;
		m_undo.ArgDatabase().getObjects( skinnerNodes );

		GetSpecifiedSkinnerNodes( skinnerNodes, tmpList );
	}
	else
	{
		MDagPath eDagPath;
		FindSkinnerNodesInHierarchy( eDagPath, tmpList );
	}

	const bool longPath( m_undo.ArgDatabase().isFlagSet( kOptLong ) );
	MStringArray result;

	MDagPath mDagPath;
	for ( MItSelectionList sIt( tmpList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.getDagPath( mDagPath ) )
		{
			result.append( longPath ? mDagPath.fullPathName() : mDagPath.partialPathName() );
		}
	}

	setResult( result );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MDagPath CVsSkinnerCmd::GetSpecifiedMesh()
{
	MSelectionList meshes;
	MSelectionList optSelectionList;

	m_undo.ArgDatabase().getObjects( optSelectionList );

	GetSpecifiedMeshes( optSelectionList, meshes );

	MDagPath mDagPath;

	if ( meshes.length() == 0U )
		return mDagPath;

	if ( meshes.length() > 1U )
	{
		meshes.getDagPath( 0U, mDagPath );

		mwarn << "Using mesh " << mDagPath.partialPathName() << ", ignoring extra meshes";
		if ( meshes.length() > 2U )
			mwarn << "s";
		mwarn << ":";

		for ( uint i( 1U ); i != meshes.length(); ++i )
		{
			meshes.getDagPath( i, mDagPath );
			mwarn << " " << mDagPath.partialPathName();
		}

		mwarn << std::endl;
	}

	meshes.getDagPath( 0U, mDagPath );
	return mDagPath;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::GetSpecifiedMeshes(
	const MSelectionList &iList,
	MSelectionList &oList )
{
	MStatus retVal( MS::kFailure );

	oList.clear();

	MDagPath mDagPath;

	for ( MItSelectionList sIt( iList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() == MItSelectionList::kDagSelectionItem && sIt.getDagPath( mDagPath ) )
		{
			if ( mDagPath.hasFn( MFn::kMesh ) )
			{
				mDagPath.extendToShapeDirectlyBelow( 0 );
				oList.add( mDagPath, MObject::kNullObj, true );
				retVal = MS::kSuccess;
			}
		}
	}

	MSelectionList tmpList;

	for ( MItSelectionList sIt( iList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() == MItSelectionList::kDagSelectionItem && sIt.getDagPath( mDagPath ) )
		{
			if ( FindMeshesInHierarchy( mDagPath, tmpList ) )
			{
				oList.merge( tmpList );
				retVal = MS::kSuccess;
			}
		}
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::FindMeshesInHierarchy(
	const MDagPath &iDagPath,
	MSelectionList &oList )
{
	MStatus retVal( MS::kFailure );

	MDagPath rDagPath( iDagPath );	// Root dag path
	while ( rDagPath.length() > 1U )
	{
		rDagPath.pop();
	}

	MDagPath mDagPath;
	MDagPath sDagPath;

	uint nShapes;

	MItDag dIt;
	if ( rDagPath.length() )
	{
		dIt.reset( rDagPath );
	}

	for ( ; !dIt.isDone(); dIt.next() )
	{
		if ( !dIt.getPath( mDagPath ) )
			continue;

		mDagPath.numberOfShapesDirectlyBelow( nShapes );
		for ( uint i( 0U ); i != nShapes; ++i )
		{
			sDagPath = mDagPath;
			sDagPath.extendToShapeDirectlyBelow( i );
			if ( sDagPath.hasFn( MFn::kMesh ) )
				continue;

			oList.add( sDagPath, MObject::kNullObj, true );
			retVal = MS::kSuccess;
		}

		if ( !ConnectedToMesh( mDagPath, sDagPath ) )
			continue;

		oList.add( sDagPath, MObject::kNullObj, true );
		retVal = MS::kSuccess;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// Returns the vsSkinner node if the passed node is connected to a vsSkinner node
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::ConnectedToMesh(
	const MDagPath &iDagPath,
	MDagPath &oDagPath )
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

					if ( mObj.apiType() == MFn::kMesh )
					{
						MDagPath::getAPathTo( mObj, oDagPath );
						return MS::kSuccess;
					}
				}
			}
		}
	}

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MDagPath CVsSkinnerCmd::GetSpecifiedSkinnerNode()
{
	MSelectionList skinnerNodes;
	MSelectionList optSelectionList;

	m_undo.ArgDatabase().getObjects( optSelectionList );

	GetSpecifiedSkinnerNodes( optSelectionList, skinnerNodes );

	MDagPath mDagPath;

	if ( skinnerNodes.length() == 0U )
		return mDagPath;

	if ( skinnerNodes.length() > 1U )
	{
		skinnerNodes.getDagPath( 0U, mDagPath );

		mwarn << "Using vsSkinnerNode " << mDagPath.partialPathName() << ", ignoring extra vsSkinnerNode";
		if ( skinnerNodes.length() > 2U )
			mwarn << "s";
		mwarn << ":";

		for ( uint i( 1U ); i != skinnerNodes.length(); ++i )
		{
			skinnerNodes.getDagPath( i, mDagPath );
			mwarn << " " << mDagPath.partialPathName();
		}

		mwarn << std::endl;
	}

	skinnerNodes.getDagPath( 0U, mDagPath );
	return mDagPath;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::GetSpecifiedSkinnerNodes(
	const MSelectionList &iList,
	MSelectionList &oList )
{
	MStatus retVal( MS::kFailure );

	oList.clear();

	MDagPath mDagPath;

	MSelectionList tmpList;

	for ( MItSelectionList sIt( iList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() == MItSelectionList::kDagSelectionItem && sIt.getDagPath( mDagPath ) )
		{
			if ( FindSkinnerNodesInHierarchy( mDagPath, tmpList ) )
			{
				oList.merge( tmpList );
				retVal = MS::kSuccess;
			}
		}
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::FindSkinnerNodesInHierarchy(
	const MDagPath &iDagPath,
	MSelectionList &oList )
{
	MStatus retVal( MS::kFailure );

	MDagPath rDagPath( iDagPath );	// Root dag path
	while ( rDagPath.length() > 1U )
	{
		rDagPath.pop();
	}

	MDagPath mDagPath;
	MDagPath sDagPath;

	uint nShapes;

	MItDag dIt;
	if ( rDagPath.length() )
	{
		dIt.reset( rDagPath );
	}

	for ( ; !dIt.isDone(); dIt.next() )
	{
		if ( !dIt.getPath( mDagPath ) )
			continue;

		mDagPath.numberOfShapesDirectlyBelow( nShapes );
		for ( uint i( 0U ); i != nShapes; ++i )
		{
			sDagPath = mDagPath;
			sDagPath.extendToShapeDirectlyBelow( i );
			if ( !IsSkinnerNode( sDagPath ) )
				continue;

			oList.add( sDagPath, MObject::kNullObj, true );
			retVal = MS::kSuccess;
		}

		if ( !ConnectedToSkinnerNode( mDagPath, sDagPath ) )
			continue;

		oList.add( sDagPath, MObject::kNullObj, true );
		retVal = MS::kSuccess;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::IsSkinnerNode(
	const MObject &iObject )
{
	if ( iObject.hasFn( MFn::kPluginShape ) && MFnDependencyNode( iObject ).typeName() == "vsSkinner" )
		return MS::kSuccess;

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::IsSkinnerNode(
	const MDagPath &iDagPath )
{
	if ( iDagPath.hasFn( MFn::kPluginShape ) && MFnDagNode( iDagPath ).typeName() == "vsSkinner" )
		return MS::kSuccess;

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// Returns the vsSkinner node if the passed node is connected to a vsSkinner node
//-----------------------------------------------------------------------------
MStatus CVsSkinnerCmd::ConnectedToSkinnerNode(
	const MDagPath &iDagPath,
	MDagPath &oDagPath )
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

					if ( IsSkinnerNode( mObj ) )
					{
						MDagPath::getAPathTo( mObj, oDagPath );
						return MS::kSuccess;
					}
				}
			}
		}
	}

	return MS::kFailure;
}