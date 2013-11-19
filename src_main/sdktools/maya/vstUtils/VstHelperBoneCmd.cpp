//=============================================================================
//
// Purpose: Implementation of CVstHelperBoneCmd
//
//=============================================================================

#pragma warning(disable : 4702)   // warning C4702: unreachable code

// Standard includes
#include <set>
#include <list>


// Maya includes
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MAngle.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MDistance.h>
#include <maya/MEulerRotation.h>
#include <maya/MGlobal.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnSet.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>


#include <fstream>


// Valve includes
#include "valveMaya.h"
#include "valveMaya/Undo.h"


// Local includes
#include "VstHelperBoneNode.h"


class CVstHelperBoneCmd : public CVsMayaMPxCommand
{
public:
	CVstHelperBoneCmd();

	virtual ~CVstHelperBoneCmd();

	// Inherited from MPxCommand
	virtual MStatus doIt(
		const MArgList &mArgList );

	virtual MStatus undoIt();

	virtual MStatus redoIt();

	virtual bool isUndoable() const { return m_undoable; }

	virtual bool hasSyntax() const { return true; }

	// Inherited from CVsMayaCommand
	virtual void SpecifySyntax(
		MSyntax &mSyntax,
		ValveMaya::CMSyntaxHelp &mSyntaxHelp);

protected:
	MStatus DoCreate();

	MStatus DoFixOld();

	MStatus DoNewTrigger( MAngle tolerance = MAngle( 0.0 ) );

	MStatus DoCopyTrigger(
		int triggerIndex );

	MStatus DoDeleteTrigger(
		int triggerIndex );

	MStatus DoUpdateTrigger(
		int triggerIndex );

	MStatus DoSnapControl(
		int triggerIndex );

	MStatus DoExport(
		const MString &filename );

	MStatus DoImport(
		const MString &filename );

	static void GetControlDagPath(
		const MDagPath &helperBonePath,
		MDagPath &controlPath );

	MSelectionList &GetSpecifiedHelperBones(
		MSelectionList &helperBoneList );

	static uint GetNextAvailableLogicalIndex(
		MPlug &mPlug );

	void CVstHelperBoneCmd::GetDefaultTriggerValues(
		const MDagPath &helperBoneShapePath,
		MVector &t,
		MVector &r,
		MVector &a );

	void SetTriggerValues(
		const MPlug &mPlug,
		const MVector *t,
		const MVector *r,
		const MVector *a );

	void CopyTriggerValues(
		const MPlug &srcP,
		MPlug &dstP );

	void CreateShadingGroup(
		const MDagPath &mDagPath );

	ValveMaya::CUndo m_undo;

	bool m_undoable;					// Is the command undoable or not
	std::list< MString > m_plugs;		// Plugs created for removal (for undo/redo)
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstHelperBoneCmd,
	vstHelperBone,
	"Creates and manipulates vstHelperBone nodes." );


//-----------------------------------------------------------------------------
// Purpose: Creator
//-----------------------------------------------------------------------------
CVstHelperBoneCmd::CVstHelperBoneCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CVstHelperBoneCmd::~CVstHelperBoneCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
// Output : A Maya MSyntax class describing command's command line syntax
//-----------------------------------------------------------------------------

static const char *const kOptHelp( "h" );
static const char *const kOptCreate( "c" );
static const char *const kOptFixOld( "fix" );
static const char *const kOptName( "n" );
static const char *const kOptNoCreateTrigger( "nct" );

static const char *const kOptNewTrigger( "nt" );
static const char *const kOptCopyTrigger( "ct" );
static const char *const kOptDeleteTrigger( "dt" );
static const char *const kOptUpdateTrigger( "ut" );

static const char *const kOptFilename( "f" );
static const char *const kOptSelected( "sl" );
static const char *const kOptExport( "e" );
static const char *const kOptImport( "i" );

static const char *const kOptTolerance( "tol" );
static const char *const kOptAxis( "a" );
static const char *const kOptTranslate( "t" );
static const char *const kOptRotate( "r" );

static const char *const kOptNodePrefix( "np" );
static const char *const kOptRemovePrefix( "rp" );

static const char *const kOptSnapControl( "s" );

void CVstHelperBoneCmd::SpecifySyntax(
	MSyntax &mSyntax,
	ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "General", "Prints usage information" );

	mSyntaxHelp.AddFlag( mSyntax, kOptCreate, "create", "Create",
		"Creates a helper bone node.\n"
		"Note that the actual transform/joint to be controlled by this helper bone node\n"
		"must already exist.\n"
		"\n"
		"  1. \"Control\" - The bone which is compared to the triggers\n"
		"  2. \"Helper\"  - The bone which is driven by the helper bone\n"
		"\n"
		"  Given a hierarchy like & the export command below:\n"
		"\n"
		"  + bit_upperArm\n"
		"    + bip_lowerArm\n"
		"    + hlp_bicep\n"
		"\n"
		"  mel> vstHelperBone -c bip_lowerArm hlp_bicep\n"
		"\n"
		"  The .vrd entry would look like:\n"
		"\n"
		"  <helper> (HELPER) (HELPER_PARENT) (CONTROL_PARENT) (CONTROL)\n"
		"\n"
		"  So in this case, it will be something like:\n"
		"\n"
		"  <helper> hlp_bicep bip_upperArm bip_upperArm bip_lowerArm\n" );

	mSyntaxHelp.AddFlag( mSyntax, kOptFixOld, "fixOld", "General",
		"Fixes old helper bone scenes.  Simply reconnects the parentSpace attribute"
		"properly.\n" );

	mSyntaxHelp.AddFlag( mSyntax, kOptName, "name", "Create",
		"Names the created helper bone shape the specified name",
		MSyntax::kString );

	mSyntaxHelp.AddFlag( mSyntax, kOptNoCreateTrigger, "noCreateTrigger", "Create",
		"Doesn't create the default trigger when creating a new vstHelperBone" );

	mSyntaxHelp.AddFlag( mSyntax, kOptNewTrigger, "newTrigger", "Edit",
		"Adds a new trigger.  Trigger parameters are set via other command line flags." );

	mSyntaxHelp.AddFlag( mSyntax, kOptCopyTrigger, "copyTrigger", "Edit",
		"Adds a new trigger by copying the values from the specified trigger",
		MSyntax::kLong );

	mSyntaxHelp.AddFlag( mSyntax, kOptDeleteTrigger, "deleteTrigger", "Edit",
		"Deletes the specified trigger",
		MSyntax::kLong );

	mSyntaxHelp.AddFlag( mSyntax, kOptUpdateTrigger, "updateTrigger", "Edit",
		"Updates the specified trigger",
		MSyntax::kLong );

	mSyntaxHelp.AddFlag( mSyntax, kOptTolerance, "tolerance", "Edit",
		"Specifies the tolerance for the new trigger when -newTrigger or -copyTrigger are used",
		MSyntax::kAngle );

	mSyntaxHelp.AddFlag( mSyntax, kOptAxis, "axis", "Edit",
		"Specifies the x, y, z Euler rotation for the axis of the new trigger when -newTrigger\n"
		"or -copyTrigger are used",
		MSyntax::kAngle, MSyntax::kAngle, MSyntax::kAngle );

	mSyntaxHelp.AddFlag( mSyntax, kOptTranslate, "translate", "Edit",
		"Specifies the x, y, z translation applied to the helper bone when the trigger is active for the new trigger when -newTrigger or -copyTrigger are used",
		MSyntax::kDistance, MSyntax::kDistance, MSyntax::kDistance );

	mSyntaxHelp.AddFlag( mSyntax, kOptRotate, "rotate", "Edit",
		"Specifies the x, y, z Euler rotation values applied to the helper bone when the trigger is active for the new trigger when -newTrigger or -copyTrigger are used",
		MSyntax::kAngle, MSyntax::kAngle, MSyntax::kAngle );

	mSyntaxHelp.AddFlag( mSyntax, kOptSnapControl, "snapControl", "General",
		"Snaps the control bone to the center of the specified trigger",
		MSyntax::kLong );

	mSyntaxHelp.AddFlag( mSyntax, kOptFilename, "filename", "Import/Export",
		"The .vrd filename to use for import or export",
		MSyntax::kString );

	mSyntaxHelp.AddFlag( mSyntax, kOptSelected, "selected", "Import/Export",
		"Exports only selected helper bones, otherwise all of the helper bones in the scene are exported." );

	mSyntaxHelp.AddFlag( mSyntax, kOptExport, "export", "Import/Export",
		"Exports the specified helper bones to the .vrd file specified by -filename" );

	mSyntaxHelp.AddFlag( mSyntax, kOptImport, "import", "Import/Export",
		"Imports the helper bones from the .vrd file specified by -filename - not implemented" );

	mSyntaxHelp.AddFlag( mSyntax, kOptNodePrefix, "nodePrefix", "Import/Export",
		"Adds a prefix to the node names",
		MSyntax::kString );

	mSyntaxHelp.AddFlag( mSyntax, kOptRemovePrefix, "removePrefix", "Import/Export",
		"Removes a prefix from the node names.  This is done before any prefix is added",
		MSyntax::kString );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( false );
}


//-----------------------------------------------------------------------------
// Purpose: When this command is executed by Maya for the first time,
//          a new instance of the command class is created and this function
//          is called with the specified command line arguments
// Input  : The command line arguments specified by the user
// Output : MStatus indicating success or failure of the command
//          In order for the command to be kept in the undo stack
//          the return value must be MStatus::kSuccess
//          Also the virtual isUndoable() has to return true for the
//          command to be kept in the undo stack for later undoing
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::doIt( const MArgList &mArgList )
{
	MStatus mStatus( m_undo.SetArgList( syntax(), mArgList ) );
	if ( mStatus )
		return redoIt();

	return mStatus;
}


//-----------------------------------------------------------------------------
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

MStatus CVstHelperBoneCmd::undoIt()
{
	// Execute all of the mel command type thingies
	const std::list< MString >::const_iterator pEnd( m_plugs.end() );
	for ( std::list< MString >::const_iterator pi( m_plugs.begin() ); pi != pEnd; ++pi )
	{
		MGlobal::executeCommand( *pi, true, false );
	}

	m_undo.Undo();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::redoIt()
{
	MStatus mStatus;

	if ( !mStatus )
	{
		setResult( MString( "Cannot parse command line" ) + mStatus.errorString() );
		return MS::kFailure;
	}

	m_undo.Clear();
	const MArgDatabase &mArgDatabase( m_undo.ArgDatabase() );
	m_undo.SaveCurrentSelection();

	if ( mArgDatabase.isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
		return MS::kSuccess;
	}
	else if ( mArgDatabase.isFlagSet( kOptCreate ) )
	{
		return DoCreate();
	}
	else if ( mArgDatabase.isFlagSet( kOptFixOld ) )
	{
		return DoFixOld();
	}
	else if ( mArgDatabase.isFlagSet( kOptNewTrigger ) )
	{
		return DoNewTrigger();
	}
	else if ( mArgDatabase.isFlagSet( kOptCopyTrigger ) )
	{
		int triggerIndex( 0 );
		mArgDatabase.getFlagArgument( kOptCopyTrigger, 0, triggerIndex );
		return DoCopyTrigger( triggerIndex );
	}
	else if ( mArgDatabase.isFlagSet( kOptDeleteTrigger ) )
	{
		int triggerIndex( 0 );
		mArgDatabase.getFlagArgument( kOptDeleteTrigger, 0, triggerIndex );
		return DoDeleteTrigger( triggerIndex );
	}
	else if ( mArgDatabase.isFlagSet( kOptUpdateTrigger ) )
	{
		int triggerIndex( 0 );
		mArgDatabase.getFlagArgument( kOptUpdateTrigger, 0, triggerIndex );
		return DoUpdateTrigger( triggerIndex );
	}
	else if ( mArgDatabase.isFlagSet( kOptSnapControl ) )
	{
		int triggerIndex( 0 );
		mArgDatabase.getFlagArgument( kOptSnapControl, 0, triggerIndex );
		return DoSnapControl( triggerIndex );
	}
	else if ( mArgDatabase.isFlagSet( kOptExport ) )
	{
		MString optFilename;
		if ( !mArgDatabase.isFlagSet( kOptFilename ) )
		{
			merr << "No -filename specified for -export" << std::endl;
			return MS::kFailure;
		}

		if ( mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename ) != MS::kSuccess )
		{
			merr << "Can't get -filename specified for -export" << std::endl;
			return MS::kFailure;
		}

		return DoExport( optFilename );
	}
	else if ( mArgDatabase.isFlagSet( kOptImport ) )
	{
		MString optFilename;
		if ( !mArgDatabase.isFlagSet( kOptFilename ) )
		{
			merr << "No -filename specified for -import" << std::endl;
			return MS::kFailure;
		}

		if ( mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename ) != MS::kSuccess )
		{
			merr << "Can't get -filename specified for -import" << std::endl;
			return MS::kFailure;
		}

		return DoImport( optFilename );
	}

	merr << GetName() << ": No action specified" << std::endl;

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoFixOld()
{
	MDagPath mDagPath;
	MSelectionList helperBoneList;

	if ( m_undo.ArgDatabase().isFlagSet( kOptSelected ) )
	{
		if ( GetSpecifiedHelperBones( helperBoneList ).length() == 0 )
		{
			merr << "No helper bones specified to fix" << std::endl;
			return MS::kFailure;
		}
	}
	else
	{
		for ( MItDag dIt( MItDag::kDepthFirst, MFn::kPluginShape ); !dIt.isDone(); dIt.next() )
		{
			if ( dIt.getPath( mDagPath ) && MFnDagNode( mDagPath ).typeName() == "vstHelperBone" )
			{
				mDagPath.extendToShapeDirectlyBelow( 0U );
				helperBoneList.add( mDagPath, MObject::kNullObj, true );
			}
		}
	}

	if ( helperBoneList.length() == 0 )
	{
		merr << "No helper bones found to fix" << std::endl;
		return MS::kFailure;
	}

	for ( MItSelectionList sIt( helperBoneList ); !sIt.isDone(); sIt.next() )
	{
		sIt.getDagPath( mDagPath );
		CVstHelperBoneNode *pHelperBone( reinterpret_cast< CVstHelperBoneNode * >( MFnDagNode( mDagPath ).userNode() ) );
		if ( pHelperBone )
		{
			MFnDagNode hFn( mDagPath );

			MPlug csP = hFn.findPlug( CVstHelperBoneNode::m_iaControlWorldSpace );
			MPlug psP = hFn.findPlug( CVstHelperBoneNode::m_iaControlParentWorldSpace );
			if ( csP.isNull() || psP.isNull() )
				continue;

			MPlugArray pA;
			if ( csP.connectedTo( pA, true, false ) && pA.length() )
			{
				MFnDependencyNode cFn( pA[ 0 ].node() );
				MPlug sP = cFn.findPlug( "parentMatrix" ).elementByLogicalIndex( pA[ 0 ].logicalIndex() );
				if ( m_undo.Connect( sP, psP, true ) )
				{
					MString result = "connectAttr";
					result += " ";
					result += sP.info();
					result += " ";
					result += psP.info();
					setResult( result );
				}
				else
				{
					mwarn << "Couldn't connect " << sP.info() << " to " << psP.info() << std::endl;
				}
			}
		}
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoCreate()
{
	MDagPath mDagPath;
	MSelectionList optSelectionList;

	// Validate specified items to whole dag nodes
	{
		MSelectionList tmpSelectionList;
		m_undo.ArgDatabase().getObjects( tmpSelectionList );
		for ( MItSelectionList sIt( tmpSelectionList, MFn::kDagNode ); !sIt.isDone(); sIt.next() )
		{
			if ( sIt.getDagPath( mDagPath ) )
			{
				optSelectionList.add( mDagPath, MObject::kNullObj, true );
			}
		}
	}

	const uint selectedCount( optSelectionList.length() );
	// Error if there aren't at least three
	if ( selectedCount < 2 )
	{
		displayError( GetName() + " need at least two dag nodes specified or selected when -create is used" );
		return MS::kFailure;
	}

	MDagPath cDagPath;	// Control path
	MDagPath htDagPath;	// Helper Transform path

	if ( optSelectionList.getDagPath( 0, cDagPath ) != MS::kSuccess )
	{
		merr << GetName() << ": Can only handle specified or selected dag objects when -create is used, 1st object specified is not a Maya dag object" << std::endl;
		return MS::kFailure;
	}

	if ( optSelectionList.getDagPath( 1, htDagPath ) != MS::kSuccess )
	{
		merr << GetName() << ": Can only handle specified or selected dag objects when -create is used, 2nd object specified is not a Maya dag object" << std::endl;
		return MS::kFailure;
	}

	// Get name command line arg
	MString optName;
	if ( m_undo.ArgDatabase().isFlagSet( kOptName ) )
	{
		m_undo.ArgDatabase().getFlagArgument( kOptName, 0, optName );
	}

	MDagModifier &mDagModifier( m_undo.DagModifier() );

	// Create the helper bone locator's transform
	MObject hsObj;

	if ( vm::CreateDagNode(
		"vstHelperBone",
		optName.length() ? optName.asChar() : GetName().asChar(),
		htDagPath.node(),
		&hsObj,
		NULL,
		&mDagModifier ) != MS::kSuccess )
	{
		displayError( MString( "Couldn't create " ) + GetName() + " transform node" );
		m_undo.Undo();
		return MS::kFailure;
	}

	// Create a 'pairBlend' node to make Maya's manipulators happy
	// Stupid maya
	MObject pbObj( mDagModifier.MDGModifier::createNode( "pairBlend" ) );
	if ( mDagModifier.doIt() != MS::kSuccess )
	{
		displayError( MString( "Couldn't create " ) + GetName() + "'s pairBlend node" );
		m_undo.Undo();
		return MS::kFailure;
	}

	if ( optName.length() )
	{
		mDagModifier.renameNode( pbObj, optName + "_pairBlend" );
		mDagModifier.doIt();
	}

	MFnDependencyNode pbFn( pbObj );
	{
		// Set initial values on pairBlend node
		// Don't need to undo these as the node itself will be deleted on undo
		MPlug pbP;

		pbP = pbFn.findPlug( "currentDriver" );
		pbP.setValue( 1 );
		pbP.setLocked( true );

		pbP = pbFn.findPlug( "translateXMode" );
		pbP.setValue( 1 );
		pbP.setLocked( true );

		pbP = pbFn.findPlug( "translateYMode" );
		pbP.setValue( 1 );
		pbP.setLocked( true );

		pbP = pbFn.findPlug( "translateZMode" );
		pbP.setValue( 1 );
		pbP.setLocked( true );

		pbP = pbFn.findPlug( "rotateOrder" );
		pbP.setValue( 0 );
		pbP.setLocked( true );

		pbP = pbFn.findPlug( "rotateMode" );
		pbP.setValue( 1 );
		pbP.setLocked( true );

		pbP = pbFn.findPlug( "rotInterpolation" );
		pbP.setValue( 1 );
		pbP.setLocked( true );
	}

	MFnTransform htFn( htDagPath );			// Helper Transform
	const MFnDependencyNode hsFn( hsObj );	// Helper Shape
	const MFnTransform cFn( cDagPath );		// Trigger Transform

	MPlug sP;
	MPlug dP;

	/*
	// Create the default trigger
	if ( false && !m_undo.ArgDatabase().isFlagSet( kOptNoCreateTrigger ) )
	{
		sP = hsFn.findPlug( "trigger" );
		sP = sP.elementByLogicalIndex( 0 );

		// Child 0 - Tolerance
		sP.child( 0 ).setValue( 90.0 / 180.0 * M_PI );

		// Child 1 - Axis
		MMatrix m( cDagPath.inclusiveMatrix() * cDagPath.exclusiveMatrixInverse() );
		MEulerRotation te;
		te = m;
		sP.child( 1 ).child( 0 ).setValue( te.x );
		sP.child( 1 ).child( 1 ).setValue( te.y );
		sP.child( 1 ).child( 2 ).setValue( te.z );
	}
	*/

	// Grab bind data from the helper bone node
	{
		sP = htFn.findPlug( "matrix" );
		MObject mObj;
		sP.getValue( mObj );
		const MTransformationMatrix tm( MFnMatrixData( mObj ).matrix() );

		dP = hsFn.findPlug( "baseTranslate" );
		const MVector bt( tm.translation( MSpace::kTransform ) );
		dP.child( 0 ).setValue( bt.x );
		dP.child( 1 ).setValue( bt.y );
		dP.child( 2 ).setValue( bt.z );

		dP = hsFn.findPlug( "baseRotate" );
		sP = htFn.findPlug( "rotate" );
		MVector br;
		sP.child( 0 ).getValue( br.x );
		sP.child( 1 ).getValue( br.y );
		sP.child( 2 ).getValue( br.z );
		dP.child( 0 ).setValue( br.x );
		dP.child( 1 ).setValue( br.y );
		dP.child( 2 ).setValue( br.z );

		MPlugArray pAi;
		htFn.getConnections( pAi );
		if ( pAi.length() )
		{
			MPlugArray pAj;
			const uint iEnd( pAi.length() );
			for ( uint ii( 0U ); ii != iEnd; ++ii )
			{
				if ( pAi[ ii ].partialName() == "is" )
					continue;

				pAj.clear();
				if ( pAi[ ii ].connectedTo( pAj, true, false ) && pAj.length() )
				{
					const uint jEnd( pAj.length() );
					for ( uint ji( 0U ); ji != jEnd; ++ji )
					{
						mDagModifier.disconnect( pAj[ ji ], pAi[ ii ] );
					}
				}
			}
		}
		mDagModifier.doIt();

	}

	// Figure out the default direction of the trigger draw axis which should point
	// from the trigger towards its first child
	{
		const uint nChildren = cDagPath.childCount();
		for ( uint i = 0; i != nChildren; ++i )
		{
			MDagPath tcDagPath( cDagPath );
			tcDagPath.push( cDagPath.child( i ) );
			const MMatrix lcMatrix( tcDagPath.inclusiveMatrix() * tcDagPath.exclusiveMatrixInverse() );
			MStatus mStatus;
			MPlug taP( hsFn.findPlug( "triggerDrawAxis", &mStatus ) );
			if ( mStatus && !taP.isNull() )
			{
				MVector da( lcMatrix[ 3 ] );
				da.normalize();
				taP.child( 0 ).setValue( da.x );
				taP.child( 1 ).setValue( da.y );
				taP.child( 2 ).setValue( da.z );
			}
			break;
		}
	}

	// control.parentMatrix[ instance ] -> vstHelperBone.triggerSpace
	{
		m_undo.Connect(
			cFn.findPlug( "parentMatrix" ).elementByLogicalIndex( cDagPath.instanceNumber() ),
			hsFn.findPlug( CVstHelperBoneNode::m_iaControlParentWorldSpace ) );
	}

	// control.worldMatrix[ instance ] -> vstHelperBone.triggerSpace
	{
		m_undo.Connect(
			cFn.findPlug( "worldMatrix" ).elementByLogicalIndex( cDagPath.instanceNumber() ),
			hsFn.findPlug( CVstHelperBoneNode::m_iaControlWorldSpace ) );
	}

	// control.rotateAxis -> vstHelperBone.controlRotateAxis
	{
		m_undo.Connect( cFn.findPlug( "rotateAxis" ), hsFn.findPlug( CVstHelperBoneNode::m_iaControlRotateAxis ) );
	}

	// control.rotateAxis -> vstHelperBone.controlRotateAxis
	if ( cDagPath.hasFn( MFn::kJoint ) )
	{
		m_undo.Connect( cFn.findPlug( "jointOrient" ), hsFn.findPlug( CVstHelperBoneNode::m_iaControlJointOrient ) );
	}

	// control bind

	// vstHelperBone.translate -> pairBlend.inTranslate1
	sP = hsFn.findPlug( "translate" );
	dP = pbFn.findPlug( "inTranslate1" );
	if ( !sP.isNull() && !dP.isNull() )
	{
		mDagModifier.connect( sP.child( 0 ), dP.child( 0 ) );
		mDagModifier.connect( sP.child( 1 ), dP.child( 1 ) );
		mDagModifier.connect( sP.child( 2 ), dP.child( 2 ) );
	}
	else
	{
		merr << GetName() << ": Couldn't find & connect " << hsFn.name() << ".translate -> " << pbFn.name() << ".inTranslate1" << std::endl;
	}

	// vstHelperBone.rotate -> pairBlend.inRotate1
	sP = hsFn.findPlug( "rotate" );
	dP = pbFn.findPlug( "inRotate1" );
	if ( !sP.isNull() && !dP.isNull() )
	{
		mDagModifier.connect( sP.child( 0 ), dP.child( 0 ) );
		mDagModifier.connect( sP.child( 1 ), dP.child( 1 ) );
		mDagModifier.connect( sP.child( 2 ), dP.child( 2 ) );
	}
	else
	{
		merr << GetName() << ": Couldn't find & connect " << hsFn.name() << ".rotate -> " << pbFn.name() << ".inRotate1" << std::endl;
	}

	// pairBlend.outTranslate -> transform.translate
	sP = pbFn.findPlug( "outTranslate" );
	dP = htFn.findPlug( "translate" );
	if ( !sP.isNull() && !dP.isNull() )
	{
		mDagModifier.connect( sP.child( 0 ), dP.child( 0 ) );
		mDagModifier.connect( sP.child( 1 ), dP.child( 1 ) );
		mDagModifier.connect( sP.child( 2 ), dP.child( 2 ) );
	}
	else
	{
		merr << GetName() << ": Couldn't find & connect " << pbFn.name() << ".outTranslate -> " << htDagPath.partialPathName() << ".translate" << std::endl;
	}

	// pairBlend.outRotate -> transform.rotate
	sP = pbFn.findPlug( "outRotate" );
	dP = htFn.findPlug( "rotate" );
	if ( !sP.isNull() && !dP.isNull() )
	{
		mDagModifier.connect( sP.child( 0 ), dP.child( 0 ) );
		mDagModifier.connect( sP.child( 1 ), dP.child( 1 ) );
		mDagModifier.connect( sP.child( 2 ), dP.child( 2 ) );
	}
	else
	{
		merr << GetName() << ": Couldn't find & connect " << pbFn.name() << ".outRotate -> " << htDagPath.partialPathName() << ".rotate" << std::endl;
	}

	if ( mDagModifier.doIt() != MS::kSuccess )
	{
		displayWarning( MString( GetName() ) + ": Couldn't connect everything when creating" );
	}

	// Create the default trigger
	if ( !m_undo.ArgDatabase().isFlagSet( kOptNoCreateTrigger ) )
	{
		DoNewTrigger( MAngle( 90.0, MAngle::kDegrees ) );
	}

	// Assign the shading group
	CreateShadingGroup( htDagPath );

	MGlobal::select( htDagPath, MObject::kNullObj, MGlobal::kReplaceList );
	setResult( htDagPath.partialPathName() );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MSelectionList &CVstHelperBoneCmd::GetSpecifiedHelperBones(
	MSelectionList &helperBoneList )
{
	MDagPath mDagPath;
	MSelectionList optSelectionList;

	{
		MSelectionList tmpSelectionList;
		m_undo.ArgDatabase().getObjects( tmpSelectionList );
		for ( MItSelectionList sIt( tmpSelectionList, MFn::kDagNode ); !sIt.isDone(); sIt.next() )
		{
			if ( sIt.getDagPath( mDagPath ) )
			{
				optSelectionList.add( mDagPath, MObject::kNullObj, true );
			}
		}
	}

	helperBoneList.clear();
	for ( MItSelectionList sIt( optSelectionList, MFn::kPluginShape ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( mDagPath ) )
			continue;

		mDagPath.extendToShapeDirectlyBelow( 0U );

		if ( MFnDagNode( mDagPath ).typeName() == "vstHelperBone" )
		{
			helperBoneList.add( mDagPath, MObject::kNullObj, true );
		}
	}

	return helperBoneList;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoNewTrigger( MAngle tolerance /* = MAngle( 0.0 ) */ )
{
	MSelectionList helperBoneList;
	if ( GetSpecifiedHelperBones( helperBoneList ).length() == 0 )
	{
		merr << "No helper bones specified to add a new trigger to" << std::endl;
		return MS::kFailure;
	}

	MDagPath helperBonePath;
	for ( MItSelectionList sIt( helperBoneList ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( helperBonePath ) )
			continue;

		const MFnDagNode helperBoneFn( helperBonePath );
		MPlug tAP( helperBoneFn.findPlug( "trigger" ) );
		if ( tAP.isNull() )
			continue;

		const uint nextAvailableLogicalIndex( GetNextAvailableLogicalIndex( tAP ) );
		MPlug tP( tAP.elementByLogicalIndex( nextAvailableLogicalIndex ) );
		if ( tP.isNull() )
			return MS::kFailure;

		m_plugs.push_front( MString(" removeMultiInstance \"" ) + tP.partialName( true, true, true, false, true, true ) + "\"" );

		MVector t;
		MVector r;
		MVector a;
		GetDefaultTriggerValues( helperBonePath, t, r, a );

		// If this isn't done, Maya won't actually add a new element
		tP.child( 0U ).setValue( tolerance );

		SetTriggerValues( tP, &t, &r, &a );

		appendToResult( tP.partialName( true, false, false, false, true, true ) );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoCopyTrigger(
	int triggerIndex )
{
	MSelectionList helperBoneList;
	if ( GetSpecifiedHelperBones( helperBoneList ).length() == 0 )
	{
		merr << "No helper bones specified to add a new trigger to" << std::endl;
		return MS::kFailure;
	}

	MDagPath helperBonePath;
	for ( MItSelectionList sIt( helperBoneList ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( helperBonePath ) )
			continue;

		const MFnDagNode helperBoneFn( helperBonePath );
		MPlug tAP( helperBoneFn.findPlug( "trigger" ) );
		if ( tAP.isNull() )
			continue;

		MPlug srcP;
		MIntArray lIndices;
		tAP.getExistingArrayAttributeIndices( lIndices );
		for ( uint ti( 0U ); ti != lIndices.length(); ++ti )
		{
			if ( lIndices[ ti ] == triggerIndex )
			{
				srcP = tAP.elementByLogicalIndex( triggerIndex );
				break;
			}
		}

		if ( srcP.isNull() )
		{
			merr << "Source trigger index " << triggerIndex << " out of range for copy from " << tAP.info() << std::endl;
			return MS::kFailure;
		}

		const uint nextAvailableLogicalIndex( GetNextAvailableLogicalIndex( tAP ) );
		MPlug dstP( tAP.elementByLogicalIndex( nextAvailableLogicalIndex ) );
		if ( dstP.isNull() )
			return MS::kFailure;

		m_plugs.push_front( MString(" removeMultiInstance \"" ) + dstP.partialName( true, true, true, false, true, true ) + "\"" );

		CopyTriggerValues( srcP, dstP );

		appendToResult( dstP.partialName( true, false, false, false, true, true ) );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// When deleting, shift all elements down to main consistency between
// logical and physical indices because otherwise it's a pain in mel
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoDeleteTrigger(
	int triggerIndex )
{
	MSelectionList helperBoneList;
	if ( GetSpecifiedHelperBones( helperBoneList ).length() == 0 )
	{
		merr << "No helper bones specified to add a new trigger to" << std::endl;
		return MS::kFailure;
	}

	MDagPath helperBonePath;
	for ( MItSelectionList sIt( helperBoneList ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( helperBonePath ) )
			continue;

		const MFnDagNode helperBoneFn( helperBonePath );
		MPlug tAP( helperBoneFn.findPlug( "trigger" ) );
		if ( tAP.isNull() )
			continue;

		MPlug delP;
		MPlug tP;
		const int nPhysicalElements( tAP.numElements() );
		for ( int ti( 0U ); ti < nPhysicalElements; ++ti )
		{
			tP = tAP.elementByPhysicalIndex( ti );
			if ( tP.isNull() )
				continue;

			if ( static_cast< int >( tP.logicalIndex() ) != ti )
			{
				merr << "Cannot delete element, logical & physical index mismatch for physical index " << ti << " is plug " << tP.info() << std::endl;
				return MS::kFailure;
			}

			if ( ti == triggerIndex )
			{
				delP = tP;
			}
		}

		if ( delP.isNull() )
		{
			merr << "Trigger index " << triggerIndex << " out of range for delete from " << tAP.info() << std::endl;
			return MS::kFailure;
		}

		appendToResult( delP.partialName( true, false, false, false, true, true ) );

		MString cmd;
		cmd += "{\n";
		cmd += "  string $attr = \"" + helperBonePath.partialPathName() + "." + tAP.partialName( false ) + "\";\n";
		cmd += "  int $i;\n";
		cmd += "  int $j;\n";
		cmd += MString( "  for ( $i = " ) + triggerIndex + MString( "; $i < " ) + ( nPhysicalElements - 1 ) + MString( "; ++$i )\n" );
		cmd += "  {\n";
		cmd += "    $j = $i + 1;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].tolerance\" ) `getAttr ( $attr + \"[ \" + $j + \" ].tolerance\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerDraw\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerDraw\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerAxisX\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerAxisX\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerAxisY\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerAxisY\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerAxisZ\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerAxisZ\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerTranslateX\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerTranslateX\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerTranslateY\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerTranslateY\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerTranslateZ\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerTranslateZ\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerRotateX\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerRotateX\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerRotateY\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerRotateY\" )`;\n";
		cmd += "    setAttr ( $attr + \"[ \" + $i + \" ].triggerRotateZ\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerRotateZ\" )`;\n";
		cmd += "    setAttr -type \"string\" ( $attr + \"[ \" + $i + \" ].triggerName\" ) `getAttr ( $attr + \"[ \" + $j + \" ].triggerName\" )`;\n";
		cmd += "  }\n";
		cmd += MString( "  removeMultiInstance ( $attr + \"[ " ) + ( nPhysicalElements - 1 ) + MString( " ]\" );\n" );
		cmd += "}\n";

		MGlobal::executeCommand( cmd, false, true );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoUpdateTrigger(
	int triggerIndex )
{
	MSelectionList helperBoneList;
	if ( GetSpecifiedHelperBones( helperBoneList ).length() == 0 )
	{
		merr << "No helper bones specified to add update triggers on" << std::endl;
		return MS::kFailure;
	}

	MDagPath helperBonePath;
	for ( MItSelectionList sIt( helperBoneList ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( helperBonePath ) )
			continue;

		const MFnDagNode helperBoneFn( helperBonePath );
		MPlug tAP( helperBoneFn.findPlug( "trigger" ) );
		if ( tAP.isNull() )
			continue;

		MPlug tP;
		MIntArray lIndices;
		tAP.getExistingArrayAttributeIndices( lIndices );
		for ( uint ti( 0U ); ti != lIndices.length(); ++ti )
		{
			if ( lIndices[ ti ] == triggerIndex )
			{
				tP = tAP.elementByLogicalIndex( triggerIndex );
				break;
			}
		}

		if ( tP.isNull() )
		{
			merr << "Trigger index " << triggerIndex << " out of range for update of " << tAP.info() << std::endl;
			return MS::kFailure;
		}

		MVector t;
		MVector r;
		MVector a;
		GetDefaultTriggerValues( helperBonePath, t, r, a );

		SetTriggerValues( tP, &t, &r, NULL );

		appendToResult( tP.partialName( true, false, false, false, true, true ) );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoSnapControl(
	int triggerIndex )
{
	MSelectionList helperBoneList;
	if ( GetSpecifiedHelperBones( helperBoneList ).length() == 0 )
	{
		merr << "No helper bones specified to snap control bones on" << std::endl;
		return MS::kFailure;
	}

	MDagPath helperBonePath;
	for ( MItSelectionList sIt( helperBoneList ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( helperBonePath ) )
		{
			merr << "Couldn't find a helper bone for snappin' " << std::endl;
			return MS::kFailure;
		}

		const MFnDagNode helperBoneFn( helperBonePath );
		MPlug tAP( helperBoneFn.findPlug( "trigger" ) );
		if ( tAP.isNull() )
		{
			merr << "Couldn't find trigger data for snappin' " << helperBonePath.partialPathName() << std::endl;
			return MS::kFailure;
		}

		MDagPath controlPath;
		GetControlDagPath( helperBonePath, controlPath );

		if ( !controlPath.isValid() || !controlPath.length() )
		{
			merr << "Couldn't find path to control bone for snappin'" << std::endl;
			return MS::kFailure;
		}

		MQuaternion tq;
		MPlug tP( tAP.elementByLogicalIndex( triggerIndex ) );
		if ( !tP.isNull() )
		{
			MEulerRotation te;
			tP.child( 1U ).child( 0U ).getValue( te.x );
			tP.child( 1U ).child( 1U ).getValue( te.y );
			tP.child( 1U ).child( 2U ).getValue( te.z );
			tq = te.asQuaternion();
		}

		MQuaternion raq;
		MPlug raP = helperBoneFn.findPlug( CVstHelperBoneNode::m_iaControlRotateAxis );
		if ( !raP.isNull() )
		{
			MEulerRotation rae;
			raP.child( 0 ).getValue( rae.x );
			raP.child( 1 ).getValue( rae.y );
			raP.child( 2 ).getValue( rae.z );
			raq = rae.asQuaternion();
		}

		MQuaternion joq;
		MPlug joP = helperBoneFn.findPlug( CVstHelperBoneNode::m_iaControlJointOrient );
		if ( !joP.isNull() )
		{
			MEulerRotation joe;
			joP.child( 0 ).getValue( joe.x );
			joP.child( 1 ).getValue( joe.y );
			joP.child( 2 ).getValue( joe.z );
			joq = joe.asQuaternion();
		}

		MQuaternion q = raq * tq * joq;
		MEulerRotation e = q.asEulerRotation();
		double x = e.x * 180.0 / M_PI;
		double y = e.y * 180.0 / M_PI;
		double z = e.z * 180.0 / M_PI;

		MFnTransform cFn( controlPath );
		MPlug craP = cFn.findPlug( "rotateAxis" );
		if ( !craP.isNull() )
		{
			MEulerRotation crae;
			craP.child( 0 ).getValue( crae.x );
			craP.child( 1 ).getValue( crae.y );
			craP.child( 2 ).getValue( crae.z );
			q = crae.asQuaternion().inverse() * q;
		}

		e = q.asEulerRotation();
		x = e.x * 180.0 / M_PI;
		y = e.y * 180.0 / M_PI;
		z = e.z * 180.0 / M_PI;

		if ( controlPath.hasFn( MFn::kJoint ) )
		{
			MPlug cjoP = cFn.findPlug( "jointOrient" );
			MEulerRotation cjoe;
			cjoP.child( 0 ).getValue( cjoe.x );
			cjoP.child( 1 ).getValue( cjoe.y );
			cjoP.child( 2 ).getValue( cjoe.z );
			q = q * cjoe.asQuaternion().inverse();
		}

		e = q.asEulerRotation();
		x = e.x * 180.0 / M_PI;
		y = e.y * 180.0 / M_PI;
		z = e.z * 180.0 / M_PI;

		MFnTransform( controlPath ).setRotation( q, MSpace::kTransform );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoExport(
	const MString &filename )
{
	MDagPath mDagPath;
	MSelectionList helperBoneList;

	MString addPrefix;
	if ( m_undo.ArgDatabase().isFlagSet( kOptNodePrefix ) )
	{
		m_undo.ArgDatabase().getFlagArgument( kOptNodePrefix, 0, addPrefix );
	}

	MString delPrefix;
	if ( m_undo.ArgDatabase().isFlagSet( kOptRemovePrefix ) )
	{
		m_undo.ArgDatabase().getFlagArgument( kOptRemovePrefix, 0, delPrefix );
	}

	if ( m_undo.ArgDatabase().isFlagSet( kOptSelected ) )
	{
		if ( GetSpecifiedHelperBones( helperBoneList ).length() == 0 )
		{
			merr << "No helper bones specified to export" << std::endl;
			return MS::kFailure;
		}
	}
	else
	{
		for ( MItDag dIt( MItDag::kDepthFirst, MFn::kPluginShape ); !dIt.isDone(); dIt.next() )
		{
			if ( dIt.getPath( mDagPath ) && MFnDagNode( mDagPath ).typeName() == "vstHelperBone" )
			{
				mDagPath.extendToShapeDirectlyBelow( 0U );
				helperBoneList.add( mDagPath, MObject::kNullObj, true );
			}
		}
	}

	if ( helperBoneList.length() == 0 )
	{
		merr << "No helper bones found to export" << std::endl;
		return MS::kFailure;
	}

	std::ofstream vrd( filename.asChar() );

	if ( !vrd )
	{
		merr << "Can't open " << filename << " for writing" << std::endl;
		return MS::kFailure;
	}

	for ( MItSelectionList sIt( helperBoneList ); !sIt.isDone(); sIt.next() )
	{
		sIt.getDagPath( mDagPath );
		CVstHelperBoneNode *pHelperBone( reinterpret_cast< CVstHelperBoneNode * >( MFnDagNode( mDagPath ).userNode() ) );
		pHelperBone->DoExport( vrd, addPrefix, delPrefix );
	}

	vrd.close();

	setResult( "Exported to " + filename );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneCmd::DoImport(
	const MString &filename )
{
	merr << "Import not implemented" << std::endl;
	return MS::kFailure;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstHelperBoneCmd::GetControlDagPath(
	const MDagPath &helperBonePath,
	MDagPath &controlPath )
{
	controlPath = MDagPath();

	MPlug tsP( MFnDagNode( helperBonePath ).findPlug( "triggerSpace" ) );
	if ( tsP.isNull() )
		return;

	MPlugArray mPlugArray;
	if ( tsP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() )
	{
		MDagPath::getAPathTo( mPlugArray[ 0 ].node(), controlPath );
		return;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint CVstHelperBoneCmd::GetNextAvailableLogicalIndex(
	MPlug &mPlug )
{
	uint retVal( 0U );

	if ( mPlug.isNull() || !mPlug.isArray() )
		return retVal;

	MIntArray lIndices;
	MStatus mStatus;
	if ( mPlug.getExistingArrayAttributeIndices( lIndices, &mStatus ) == 0U || mStatus != MS::kSuccess )
		return retVal;

	// I'd assume that these things are sorted but it's not documented as such
	std::set< uint > lIndexSet;

	const uint lEnd( lIndices.length() );
	for ( uint li( 0U ); li != lEnd; ++li )
	{
		lIndexSet.insert( lIndices[ li ] );
	}

	const std::set< uint >::const_iterator sEnd( lIndexSet.end() );
	for ( std::set< uint >::const_iterator si( lIndexSet.begin() ); si != sEnd; ++si, ++retVal )
	{
		if ( *si != retVal )
			return retVal;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstHelperBoneCmd::GetDefaultTriggerValues(
	const MDagPath &helperBoneShapePath,
	MVector &t,
	MVector &r,
	MVector &a )
{
	const MObject nObj( helperBoneShapePath.transform() );

	const MFnDependencyNode depFn( nObj );
	MPlug tP( depFn.findPlug( "translate" ) );
	if ( !tP.isNull() )
	{
		tP.child( 0U ).getValue( t.x );
		tP.child( 1U ).getValue( t.y );
		tP.child( 2U ).getValue( t.z );

		MPlug bP( MFnDagNode( helperBoneShapePath ).findPlug( "baseTranslate" ) );
		if ( !bP.isNull() )
		{
			MVector b;
			bP.child( 0U ).getValue( b.x );
			bP.child( 1U ).getValue( b.y );
			bP.child( 2U ).getValue( b.z );

			t = t - b;
		}
	}

	MPlug rP( depFn.findPlug( "rotate" ) );
	if ( !rP.isNull() )
	{
		rP.child( 0U ).getValue( r.x );
		rP.child( 1U ).getValue( r.y );
		rP.child( 2U ).getValue( r.z );

		MPlug bP( MFnDagNode( helperBoneShapePath ).findPlug( "baseRotate" ) );
		if ( !bP.isNull() )
		{
			MVector b;
			bP.child( 0U ).getValue( b.x );
			bP.child( 1U ).getValue( b.y );
			bP.child( 2U ).getValue( b.z );

			MMatrix rM( MEulerRotation( r ).asMatrix() );
			MMatrix biM( MEulerRotation( b ).asMatrix().inverse() );
			rM *= biM;

			MEulerRotation e;
			e = rM;
			r = e.asVector();
			if ( r.isEquivalent( MVector::zero ) )
			{
				r = MVector::zero;
			}
		}
	}

	if ( m_undo.ArgDatabase().isFlagSet( kOptTranslate ) )
	{
		MDistance distance;
		for ( uint i( 0U ); i < 3U; ++i )
		{
			if ( m_undo.ArgDatabase().getFlagArgument( kOptTranslate, i, distance ) )
			{
				t[ i ] = distance.asCentimeters();
			}
		}
	}

	if ( m_undo.ArgDatabase().isFlagSet( kOptRotate ) )
	{
		MAngle angle;
		for ( uint i( 0U ); i < 3U; ++i )
		{
			if ( m_undo.ArgDatabase().getFlagArgument( kOptRotate, i, angle ) )
			{
				r[ i ] = angle.asRadians();
			}
		}
	}

	MDagPath controlPath;
	GetControlDagPath( helperBoneShapePath, controlPath );
	if ( controlPath.isValid() && controlPath.length() )
	{
		MFnDagNode hFn( helperBoneShapePath );

		MQuaternion raq;
		MPlug raP = hFn.findPlug( CVstHelperBoneNode::m_iaControlRotateAxis );
		if ( !raP.isNull() )
		{
			MEulerRotation rae;
			raP.child( 0 ).getValue( rae.x );
			raP.child( 1 ).getValue( rae.y );
			raP.child( 2 ).getValue( rae.z );
			raq = rae.asQuaternion();
			raq.invertIt();
		}

		MQuaternion joq;
		MPlug joP = hFn.findPlug( CVstHelperBoneNode::m_iaControlJointOrient );
		if ( !joP.isNull() )
		{
			MEulerRotation joe;
			joP.child( 0 ).getValue( joe.x );
			joP.child( 1 ).getValue( joe.y );
			joP.child( 2 ).getValue( joe.z );
			joq = joe.asQuaternion();
			joq.invertIt();
		}

		MQuaternion tq;
		MFnTransform( controlPath ).getRotation( tq, MSpace::kObject );
		a = tq.asEulerRotation().asVector();
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstHelperBoneCmd::SetTriggerValues(
	const MPlug &mPlug,
	const MVector *pT,
	const MVector *pR,
	const MVector *pA )
{
	if ( mPlug.isNull() )
		return;

	if ( m_undo.ArgDatabase().isFlagSet( kOptTolerance ) )
	{
		MPlug tP( mPlug.child( 0U ) );
		Assert( !tP.isNull() );
		MAngle tolerance;
		if ( m_undo.ArgDatabase().getFlagArgument( kOptTolerance, 0, tolerance ) )
		{
			tP.setValue( tolerance );
		}
	}

	if ( pA )
	{
		MPlug aP( mPlug.child( 1U ) );
		Assert( !aP.isNull() );
		MAngle angle;

		if ( m_undo.ArgDatabase().isFlagSet( kOptAxis ) )
		{
			for ( uint i( 0U ); i < 3U; ++i )
			{
				if ( m_undo.ArgDatabase().getFlagArgument( kOptAxis, i, angle ) )
				{
					aP.child( i ).setValue( angle );
				}
			}
		}
		else
		{
			for ( uint i( 0U ); i < 3U; ++i )
			{
				aP.child( i ).setValue( ( *pA )[ i ] );
			}
		}
	}

	if ( pT )
	{
		MPlug tP( mPlug.child( 2U ) );
		Assert( !tP.isNull() );
		MDistance distance;

		if ( m_undo.ArgDatabase().isFlagSet( kOptTranslate ) )
		{
			for ( uint i( 0U ); i < 3U; ++i )
			{
				if ( m_undo.ArgDatabase().getFlagArgument( kOptTranslate, i, distance ) )
				{
					tP.child( i ).setValue( distance );
				}
			}
		}
		else
		{
			for ( uint i( 0U ); i < 3U; ++i )
			{
				tP.child( i ).setValue( ( *pT )[ i ] );
			}
		}
	}

	if ( pR )
	{
		MPlug rP( mPlug.child( 3U ) );
		Assert( !rP.isNull() );
		MAngle angle;

		if ( m_undo.ArgDatabase().isFlagSet( kOptRotate ) )
		{
			for ( uint i( 0U ); i < 3U; ++i )
			{
				if ( m_undo.ArgDatabase().getFlagArgument( kOptRotate, i, angle ) )
				{
					rP.child( i ).setValue( angle );
				}
			}
		}
		else
		{
			for ( uint i( 0U ); i < 3U; ++i )
			{
				rP.child( i ).setValue( ( *pR )[ i ] );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstHelperBoneCmd::CopyTriggerValues(
	const MPlug &srcP,
	MPlug &dstP )
{
	if ( srcP.isNull() || dstP.isNull() )
		return;

	MAngle angle;
	MDistance distance;

	for ( uint ci( 0U ); ci != srcP.numChildren(); ++ci )
	{
		if ( ci == 0U )
		{
			srcP.child( ci ).getValue( angle );
			dstP.child( ci ).setValue( angle );
		}
		else if ( ci == 2U )
		{
			for ( uint i( 0U ); i < 3U; ++i )
			{
				srcP.child( ci ).child( i ).getValue( distance );
				dstP.child( ci ).child( i ).setValue( distance );
			}
		}
		else
		{
			for ( uint i( 0U ); i < 3U; ++i )
			{
				srcP.child( ci ).child( i ).getValue( angle );
				dstP.child( ci ).child( i ).setValue( angle );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstHelperBoneCmd::CreateShadingGroup(
	const MDagPath &mDagPath )
{
	for ( MItDependencyNodes dIt( MFn::kShadingEngine ); !dIt.isDone(); dIt.next() )
	{
		MObject mObj = dIt.thisNode();
		MFnDependencyNode depFn( mObj );
		if ( depFn.name() == "vstHelperBoneSG" )
		{
			MPlug mP( depFn.findPlug(" vstHelperBoneShader" ) );
			if ( !mP.isNull() || true )
			{
				bool v( false );
				mP.getValue( v );
				if ( v || true )
				{
					MFnSet sgFn( mObj );
					MSelectionList mSelectionList;
					mSelectionList.add( mDagPath );
					sgFn.addMembers( mSelectionList );
					return;
				}
			}
		}
	}

	MStatus mStatus;

	MFnSet sgFn;
	MSelectionList mSelectionList;
	mSelectionList.add( mDagPath );
	MObject sgObj = sgFn.create( mSelectionList, MFnSet::kRenderableOnly, &mStatus );

	MDagModifier &mDagModifier( m_undo.DagModifier() );
	MObject ssObj = mDagModifier.MDGModifier::createNode( "lambert" );
	mDagModifier.doIt();

	mDagModifier.renameNode( sgObj, "helperBoneSG" );
	mDagModifier.renameNode( ssObj, "helperBone" );
	mDagModifier.doIt();

	MObject sgAttrObj = MFnNumericAttribute().create( "vstHelperBoneShader", "vhbs", MFnNumericData::kBoolean, false );
	mDagModifier.addAttribute( sgObj, sgAttrObj );
	mDagModifier.doIt();
	MPlug mP( sgObj, sgAttrObj );
	if ( !mP.isNull() )
	{
		mP.setValue( true );
	}

	const MFnDependencyNode ssFn( ssObj );

	MPlug srcP;
	MPlug dstP;

	// Connecting shading group to surface shader
	srcP = ssFn.findPlug( "outColor", &mStatus );
	if ( mStatus && !srcP.isNull() )
	{
		dstP = sgFn.findPlug( "surfaceShader", &mStatus );
		if ( mStatus && !dstP.isNull() )
		{
			MPlugArray mPlugArray;
			if ( dstP.connectedTo( mPlugArray, true, false ) )
			{
				for ( uint i( 0 ); i != mPlugArray.length(); ++i )
				{
					mDagModifier.disconnect( mPlugArray[ i ], dstP );
				}
			}
			mDagModifier.connect( srcP, dstP );
		}
	}
	mDagModifier.doIt();

	// Create Ramp
	MObject rObj = mDagModifier.MDGModifier::createNode( "ramp" );
	mDagModifier.doIt();
	mDagModifier.renameNode( rObj, "helperBoneRamp" );
	mDagModifier.doIt();

	const MFnDependencyNode rFn( rObj );

	MPlug ceAP( rFn.findPlug( "colorEntryList" ) );
	MPlug ceP;

	ceP = ceAP.elementByLogicalIndex( 0 );	// Black
	ceP.child( 0 ).setValue( 0.01 );
	ceP.child( 1 ).child( 0 ).setValue( 0 );
	ceP.child( 1 ).child( 1 ).setValue( 0 );
	ceP.child( 1 ).child( 2 ).setValue( 0 );

	ceP = ceAP.elementByLogicalIndex( 1 );	// Blue
	ceP.child( 0 ).setValue( 0.02 );
	ceP.child( 1 ).child( 0 ).setValue( 0 );
	ceP.child( 1 ).child( 1 ).setValue( 0 );
	ceP.child( 1 ).child( 2 ).setValue( 1 );

	ceP = ceAP.elementByLogicalIndex( 2 );	// Cyan
	ceP.child( 0 ).setValue( 0.25 );
	ceP.child( 1 ).child( 0 ).setValue( 0 );
	ceP.child( 1 ).child( 1 ).setValue( 1 );
	ceP.child( 1 ).child( 2 ).setValue( 1 );

	ceP = ceAP.elementByLogicalIndex( 3 );	// Green
	ceP.child( 0 ).setValue( 0.5 );
	ceP.child( 1 ).child( 0 ).setValue( 0 );
	ceP.child( 1 ).child( 1 ).setValue( 1 );
	ceP.child( 1 ).child( 2 ).setValue( 0 );

	ceP = ceAP.elementByLogicalIndex( 4 );	// Yellow
	ceP.child( 0 ).setValue( 0.75 );
	ceP.child( 1 ).child( 0 ).setValue( 1 );
	ceP.child( 1 ).child( 1 ).setValue( 1 );
	ceP.child( 1 ).child( 2 ).setValue( 0 );

	ceP = ceAP.elementByLogicalIndex( 5 );	// Red
	ceP.child( 0 ).setValue( 0.99 );
	ceP.child( 1 ).child( 0 ).setValue( 1 );
	ceP.child( 1 ).child( 1 ).setValue( 0 );
	ceP.child( 1 ).child( 2 ).setValue( 0 );

	// Connect Ramp To Surface Shader
	srcP = rFn.findPlug( "outColor" );
	dstP = ssFn.findPlug( "color" );
	if ( !srcP.isNull() && !dstP.isNull() )
	{
		mDagModifier.connect( srcP, dstP );
		mDagModifier.doIt();
	}
}