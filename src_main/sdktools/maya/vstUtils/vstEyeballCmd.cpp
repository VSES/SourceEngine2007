//=============================================================================
//
// Purpose: Implementation of CVstHelperBoneCmd
//
//=============================================================================


// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"
#include "valveMaya/Undo.h"


// Local includes
#include "VstEyeballNode.h"


class CVstEyeballCmd : public CVsMayaMPxCommand
{
public:
	CVstEyeballCmd();

	virtual ~CVstEyeballCmd();

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

	// New
	MStatus DoCreate();

protected:
	ValveMaya::CUndo m_undo;
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVstEyeballCmd,
	vstEyeball,
	"Creates and manipulates vstEyeball nodes." );


//-----------------------------------------------------------------------------
// Purpose: Creator
//-----------------------------------------------------------------------------
CVstEyeballCmd::CVstEyeballCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CVstEyeballCmd::~CVstEyeballCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
// Output : A Maya MSyntax class describing command's command line syntax
//-----------------------------------------------------------------------------

static const char *const kOptHelp( "h" );
static const char *const kOptCreate( "c" );
static const char *const kOptName( "n" );

void CVstEyeballCmd::SpecifySyntax(
	MSyntax &mSyntax,
	ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "General", "Prints usage information" );

	mSyntaxHelp.AddFlag( mSyntax, kOptCreate, "create", "Create", "Creates a new vstEyeball node" );

	mSyntaxHelp.AddFlag( mSyntax, kOptName, "name", "Create", "Names the created vstEyeball node the specified name", MSyntax::kString );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( false );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstEyeballCmd::doIt( const MArgList &mArgList )
{
	m_undo.SetArgList( syntax(), mArgList );

	return redoIt();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstEyeballCmd::undoIt()
{
	m_undo.Undo();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstEyeballCmd::redoIt()
{
	m_undo.Clear();
	const MArgDatabase &mArgDatabase = m_undo.ArgDatabase();
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

	merr << GetName() << ": No action specified, nothing to do" << std::endl;

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstEyeballCmd::DoCreate()
{
	MString optName;
	if ( m_undo.ArgDatabase().isFlagSet( kOptName ) )
	{
		m_undo.ArgDatabase().getFlagArgument( kOptName, 0, optName );
	}

	MObject etObj;	// Eyeball Transform
	MObject esObj;	// Eyeball Shape

	if ( vm::CreateDagNode(
		"vstEyeball",
		optName.length() ? optName.asChar() : GetName().asChar(),
		MObject::kNullObj,
		&etObj,
		&esObj,
		&m_undo.DagModifier() ) != MS::kSuccess )
	{
		merr << "Couldn't create: " << GetName() << " node" << std::endl;
		m_undo.Undo();
		return MS::kFailure;
	}

	MDagPath etPath;
	MDagPath::getAPathTo( etObj, etPath );
	MGlobal::select( etPath, MObject::kNullObj, MGlobal::kReplaceList );
	setResult( etPath.partialPathName() );

	return MS::kSuccess;
}