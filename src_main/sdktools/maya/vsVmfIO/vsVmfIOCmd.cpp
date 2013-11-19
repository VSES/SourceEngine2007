//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVsVmfCmd
//
//=============================================================================

#pragma warning(disable : 4702)   // warning C4702: unreachable code

// Standard includes
#include <string>
#include <map>
#include <io.h>


// Maya includes
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MObjectArray.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MVector.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>


// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"
#include "filesystem.h"
#include "tier2/tier2.h"
#include "utlbuffer.h"
#include "KeyValues.h"


// Local Includes
#include "VmfImport.h"


class CVsVmfCmd : public CVsMayaMPxCommand
{
public:
	CVsVmfCmd();

	virtual ~CVsVmfCmd();

	// Inheritied from MPxCommand
	virtual MStatus doIt( const MArgList &mArgList );

	virtual MStatus undoIt();

	virtual MStatus redoIt();

	virtual bool isUndoable() const { return m_undoable; }

	virtual bool hasSyntax() const { return true; }

	// Inheritied from CVsMayaCommand
	void SpecifySyntax( MSyntax &mSyntax, ValveMaya::CMSyntaxHelp &mSyntaxHelp);

protected:
	MStatus ImportVmf(
		const MString &filename );

	static void CVsVmfCmd::SetVmfImportOpts(
		CVmfImport &vmfImport,
		const MArgDatabase &mArgDatabase );

	bool m_undoable;					// Is the command undoable or not
	MArgDatabase *m_pArgDatabase;		// The stored arg database (for undo/redo)
	MDagModifier *m_pDagModifier;		// The dag modifier (for undo/redo)
	MObjectArray m_undoObjs;			// List of MObjects created outside of MDagModifier
	MSelectionList m_mSelectionList;	// The previous selection list (for undo/redo)
};

//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVsVmfCmd,
	vsVmfIO,
	"Imports Valve Map Files (vmf) into Maya... maybe someday export" );


//-----------------------------------------------------------------------------
// Purpose: Creator
//-----------------------------------------------------------------------------

CVsVmfCmd::CVsVmfCmd()
: m_undoable( false )
, m_pArgDatabase( NULL )
, m_pDagModifier( NULL )
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------

CVsVmfCmd::~CVsVmfCmd()
{
	if ( m_pArgDatabase )
	{
		delete m_pArgDatabase;
		m_pArgDatabase = NULL;
	}

	if ( m_pDagModifier )
	{
		delete m_pDagModifier;
		m_pDagModifier = NULL;
	}
}


//-----------------------------------------------------------------------------
// MSyntax factory for the command
//-----------------------------------------------------------------------------

static const char *const kOptHelp( "h" );
static const char *const kOptImport( "i" );
static const char *const kOptFilename( "f" );
static const char *const kOptBakeUVs( "b" );
static const char *const kOptImportTools( "it" );
static const char *const kOptImportDev( "id" );
static const char *const kOptTextureArchive( "t" );
static const char *const kOptImportBrushes( "ib" );
static const char *const kOptImportLights( "il" );
static const char *const kOptImportModels( "im" );

void CVsVmfCmd::SpecifySyntax( MSyntax &mSyntax, ValveMaya::CMSyntaxHelp &mSyntaxHelp )
{
	mSyntaxHelp.Clear();

	mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "General", "Prints this information" );
	mSyntaxHelp.AddFlag( mSyntax, kOptImport, "import", "Import", "Imports a VMF file" );
	mSyntaxHelp.AddFlag( mSyntax, kOptFilename, "filename", "Import", "The VMF file to import", MSyntax::kString );
	mSyntaxHelp.AddFlag( mSyntax, kOptBakeUVs, "bakeUVs", "Import", "Bake the UVs into the polygons generated from the brushes, otherwise a projection node is used to apply the texture.  Default is true.", MSyntax::kBoolean );
	mSyntaxHelp.AddFlag( mSyntax, kOptImportTools, "importTools", "Import", "Bring in brush sides that have a material of \"TOOLS/*\". Default is false.", MSyntax::kBoolean );
	mSyntaxHelp.AddFlag( mSyntax, kOptImportDev, "importDev", "Import", "Bring in brush sides that have a material of *\"DEV/*\". Default is false.", MSyntax::kBoolean );
	mSyntaxHelp.AddFlag( mSyntax, kOptImportBrushes, "importBrushes", "Import", "Bring in brushes. Default is true.", MSyntax::kBoolean );
	mSyntaxHelp.AddFlag( mSyntax, kOptImportLights, "importLights", "Import", "Bring in lights. Default is true.", MSyntax::kBoolean );
	mSyntaxHelp.AddFlag( mSyntax, kOptImportModels, "importModels", "Import", "Bring in models. Default is true.", MSyntax::kBoolean );
	mSyntaxHelp.AddFlag( mSyntax, kOptTextureArchive, "textureArchive", "Import", "Copy all of the textures to a specified directory and refer to them from there", MSyntax::kString );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( false );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsVmfCmd::doIt(
	const MArgList &mArgList )
{
	MStatus mStatus( MS::kSuccess );
	MArgDatabase *mArgDatabase = new MArgDatabase( syntax(), mArgList, &mStatus );

	if ( mArgDatabase ) {
		if ( mStatus )
		{
			m_pArgDatabase = mArgDatabase;
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
//
//-----------------------------------------------------------------------------
MStatus CVsVmfCmd::undoIt()
{
	if ( m_undoable )
	{
		MGlobal::setActiveSelectionList( m_mSelectionList );
		m_mSelectionList.clear();

		if ( m_pDagModifier )
		{
			m_pDagModifier->undoIt();
			delete m_pDagModifier;
			m_pDagModifier = NULL;
		}

		for ( int i = m_undoObjs.length() - 1; i >= 0; --i )
		{
			MGlobal::deleteNode( m_undoObjs[ i ] );
		}
		m_undoObjs.clear();
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsVmfCmd::redoIt()
{
	if ( m_pArgDatabase->isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName().asChar(), GetDesc().asChar() );
		return MS::kSuccess;
	}

	if ( m_pArgDatabase->isFlagSet( kOptImport ) )
	{
		if ( !m_pArgDatabase->isFlagSet( kOptFilename ) )
		{
			merr << "No -filename specified with -import" << std::endl;
			return MS::kFailure;
		}

		MString optFilename;
		m_pArgDatabase->getFlagArgument( kOptFilename, 0, optFilename );

		if ( _access( optFilename.asChar(), 04 ) != 0 )
		{
			merr << optFilename << " doesn't exist or isn't readable" << std::endl;
			return MS::kFailure;
		}

		m_undoable = true;
		MGlobal::getActiveSelectionList( m_mSelectionList );
		m_pDagModifier = new MDagModifier;

		CVmfImport vmfImport;

		SetVmfImportOpts( vmfImport, *m_pArgDatabase );

		return vmfImport.DoIt( optFilename, m_pDagModifier, &m_undoObjs );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Sets internal options in the CVmfImport class instanced based on command
// line arguments
//-----------------------------------------------------------------------------
void CVsVmfCmd::SetVmfImportOpts(
	CVmfImport &vmfImport,
	const MArgDatabase &mArgDatabase )
{
	vmfImport.m_optBakeUVs = true;
	if ( mArgDatabase.isFlagSet( kOptBakeUVs ) )
	{
		mArgDatabase.getFlagArgument( kOptBakeUVs, 0, vmfImport.m_optBakeUVs );
	}

	vmfImport.m_optImportTools = false;
	if ( mArgDatabase.isFlagSet( kOptImportTools ) )
	{
		mArgDatabase.getFlagArgument( kOptBakeUVs, 0, vmfImport.m_optImportTools );
	}

	vmfImport.m_optImportDev = false;
	if ( mArgDatabase.isFlagSet( kOptImportDev ) )
	{
		mArgDatabase.getFlagArgument( kOptImportDev, 0, vmfImport.m_optImportDev );
	}

	vmfImport.m_optImportBrushes = true;
	if ( mArgDatabase.isFlagSet( kOptImportBrushes ) )
	{
		mArgDatabase.getFlagArgument( kOptImportBrushes, 0, vmfImport.m_optImportBrushes );
	}

	vmfImport.m_optImportLights = true;
	if ( mArgDatabase.isFlagSet( kOptImportLights ) )
	{
		mArgDatabase.getFlagArgument( kOptImportLights, 0, vmfImport.m_optImportLights );
	}

	vmfImport.m_optImportModels = true;
	if ( mArgDatabase.isFlagSet( kOptImportModels ) )
	{
		mArgDatabase.getFlagArgument( kOptImportModels, 0, vmfImport.m_optImportModels );
	}

	vmfImport.m_optTextureArchive.clear();
	if ( mArgDatabase.isFlagSet( kOptTextureArchive ) )
	{
		mArgDatabase.getFlagArgument( kOptTextureArchive, 0, vmfImport.m_optTextureArchive );
		char buf0[ MAX_PATH ];
		Q_strcpy( buf0, vmfImport.m_optTextureArchive.asChar() );
		Q_FixSlashes( buf0 );
		Q_FixSlashes( buf0, '/' );
		vmfImport.m_optTextureArchive = buf0;

		if ( vmfImport.m_optTextureArchive.length() && _access( vmfImport.m_optTextureArchive.asChar(), 02 ) != 0 )
		{
			Q_mkdir( const_cast< char * >( vmfImport.m_optTextureArchive.asChar() ) );
		}

		if ( _access( buf0, 04 ) != 0 )
		{
			vmfImport.m_optTextureArchive.clear();
		}
	}
}