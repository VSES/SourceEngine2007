//======= Copyright © 1996-2007, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVsVmt
//
//=============================================================================


// Maya includes
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnSet.h>
#include <maya/MFnStringData.h>
#include <maya/MGlobal.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MSelectionList.h>
#include <maya/MStringArray.h>
#include <maya/MSyntax.h>


// Valve includes
#include "valveMaya.h"
#include "valveMaya/Undo.h"
#include "VsMayaMPxFactory.h"
#include "filesystem.h"
#include "tier2/tier2.h"


//=============================================================================
//
//=============================================================================
class CVsVmtCmd : public CVsMayaMPxCommand
{
public:
	CVsVmtCmd();

	virtual ~CVsVmtCmd();

	// Inheritied from MPxCommand
	virtual MStatus doIt( const MArgList &mArgList );
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const { return m_undo.IsUndoable(); }
	virtual bool hasSyntax() const { return true; }

	// Inheritied from CVsMayaCommand
	void SpecifySyntax( MSyntax &mSyntax, vm::CMSyntaxHelp &help );

protected:
	void GetSpecifiedVsVmtNodes( const MSelectionList &iList, MSelectionList &oList );

	MString GetVmtPath();

	vm::CUndo m_undo;
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVsVmtCmd,
	vsVmt,
	"Creates vsVmt nodes" );


//-----------------------------------------------------------------------------
// Purpose: Creator
//-----------------------------------------------------------------------------
CVsVmtCmd::CVsVmtCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CVsVmtCmd::~CVsVmtCmd()
{
}


//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
//-----------------------------------------------------------------------------
static const char *const kOptHelp( "h" );
static const char *const kOptCreate( "c" );
static const char *const kOptName( "n" );
static const char *const kOptVmtPath( "v" );
static const char *const kOptBrowse( "b" );

void CVsVmtCmd::SpecifySyntax( MSyntax &mSyntax, vm::CMSyntaxHelp &help )
{
	help.AddFlag( mSyntax, kOptHelp, "help", "General",
		"Prints this information" );

	help.AddFlag( mSyntax, kOptCreate, "create", "Create",
		"Creates a new vsVmtToTex and assigns it to the current selection." );

	help.AddFlag( mSyntax, kOptName, "name", "Create",
		"Names the newly created shading group", MSyntax::kString );

	help.AddFlag( mSyntax, kOptVmtPath, "vmtPath", "Create",
		"Specifies either the relative or absolute the vmt material file to be associated\n"
		"  with the created vsVmt node", MSyntax::kString );

	help.AddFlag( mSyntax, kOptBrowse, "browse", "Create",
		"Brings up a file browser to browse to the vmt material file to be associated\n"
		"  with the created vsVmt node.  NOTE: -vmtPath overrides -browse." );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( true );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsVmtCmd::doIt( const MArgList &mArgList )
{
	m_undo.SetArgList( syntax(), mArgList );

	return redoIt();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsVmtCmd::undoIt()
{
	m_undo.Undo();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsVmtCmd::GetSpecifiedVsVmtNodes( const MSelectionList &iList, MSelectionList &oList )
{
	MObject mObj;

	for ( MItSelectionList sIt( iList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() == MItSelectionList::kDNselectionItem && sIt.getDependNode( mObj ) && MFnDependencyNode( mObj ).typeName() == "vsVmt" )
		{
			oList.add( mObj, true );
		}
	}

	MObject vsVmtObj;
	MDagPath mDagPath;
	MDagPath sDagPath;
	MSelectionList sList;

	for ( MItDependencyNodes dIt( MFn::kShadingEngine ); !dIt.isDone(); dIt.next() )
	{
		vsVmtObj = MObject::kNullObj;

		const MFnSet sFn( dIt.thisNode() );

		sFn.getMembers( sList, false );

		MPlugArray myPlugs;
		MPlugArray connectedPlugs;
		sFn.getConnections( myPlugs );
		for ( uint i = 0; i < myPlugs.length(); ++i )
		{
			if ( myPlugs[ i ].connectedTo( connectedPlugs, true, true ) && connectedPlugs.length() )
			{
				for ( uint j = 0; j < connectedPlugs.length(); ++j )
				{
					mObj = connectedPlugs[ j ].node();
					if ( MFnDependencyNode( mObj ).typeName() == "vsVmt" )
					{
						vsVmtObj = mObj;
						break;
					}
				}
			}
		}

		if ( vsVmtObj.isNull() )
			continue;

		for ( MItSelectionList iIt( iList ); !iIt.isDone(); iIt.next() )
		{
			if ( iIt.itemType() == MItSelectionList::kDagSelectionItem && iIt.getDagPath( mDagPath ) )
			{
				bool bFound = false;
				for ( MItSelectionList sIt( sList ); !sIt.isDone(); sIt.next() )
				{
					if ( sIt.itemType() == MItSelectionList::kDagSelectionItem && sIt.getDagPath( sDagPath ) && sDagPath == mDagPath )
					{
						oList.add( vsVmtObj, true );
						bFound = true;
						break;
					}
				}

				if ( !bFound )
				{
					uint nShapesCount = 0;
					mDagPath.numberOfShapesDirectlyBelow( nShapesCount );
					for ( uint i = 0; !bFound && i < nShapesCount; ++i )
					{
						MDagPath cDagPath( mDagPath );
						cDagPath.extendToShapeDirectlyBelow( i );

						for ( MItSelectionList sIt( sList ); !sIt.isDone(); sIt.next() )
						{
							if ( sIt.itemType() == MItSelectionList::kDagSelectionItem && sIt.getDagPath( sDagPath ) && sDagPath == cDagPath )
							{
								oList.add( vsVmtObj, true );
								bFound = true;
								break;
							}
						}
					}
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MString CVsVmtCmd::GetVmtPath()
{
	const MArgDatabase &mArgDatabase = m_undo.ArgDatabase();

	if ( !mArgDatabase.isFlagSet( kOptVmtPath ) )
		return MString( "debug/debugempty" );

	MString vmtPath;
	mArgDatabase.getFlagArgument( kOptVmtPath, 0, vmtPath );

	char buf0[ MAX_PATH ];
	char buf1[ MAX_PATH ];

	// Clean up the passed material name
	if ( !g_pFullFileSystem->FullPathToRelativePath( vmtPath.asChar(), buf0, sizeof( buf0 ) ) )
	{
		Q_strncpy( buf0, vmtPath.asChar(), sizeof( buf0 ) );
	}

	Q_FixSlashes( buf0, '/' );
	if ( !Q_strnicmp( "materials/", buf0, 10 ) )
	{
		Q_StripExtension( buf0 + 10, buf1, sizeof( buf1 ) );
	}
	else
	{
		Q_StripExtension( buf0, buf1, sizeof( buf1 ) );
	}
	Q_StripTrailingSlash( buf1 );

	return MString( buf1 );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsVmtCmd::redoIt()
{
	m_undo.Clear();

	const MArgDatabase &mArgDatabase = m_undo.ArgDatabase();

	if ( mArgDatabase.isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
		return MS::kSuccess;
	}

	if ( mArgDatabase.isEdit() )
	{
		if ( mArgDatabase.isFlagSet( kOptVmtPath ) )
		{
			MSelectionList optSelectionList;
			mArgDatabase.getObjects( optSelectionList );

			MSelectionList vsVmtList;
			GetSpecifiedVsVmtNodes( optSelectionList, vsVmtList );

			bool bFound = false;

			MObject sObj;
			for ( MItSelectionList sIt( vsVmtList ); !sIt.isDone(); sIt.next() )
			{
				if ( sIt.getDependNode( sObj ) && MFnDependencyNode( sObj ).typeName() == "vsVmt" )
				{
					MPlug vmtPathP = MFnDependencyNode( sObj ).findPlug( "vmtPath" );
					if ( !vmtPathP.isNull() )
					{
						m_undo.SetAttr( vmtPathP, MFnStringData().create( GetVmtPath() ) );
						bFound = true;
					}
				}
			}

			if ( bFound )
				return MS::kSuccess;

			merr << GetName() << ": -edit specified without any specified vsVmt nodes to edit" << std::endl;
		}
		else
		{
			merr << GetName() << ": -edit specified without any editing action specified" << std::endl;
		}

		return MS::kFailure;
	}

	if ( mArgDatabase.isFlagSet( kOptCreate ) )
	{
		if ( mArgDatabase.isFlagSet( kOptBrowse ) )
			return MGlobal::executeCommand( "vsVmtBrowser" );

		m_undo.SaveCurrentSelection();

		MSelectionList optSelectionList;
		mArgDatabase.getObjects( optSelectionList );

		if ( optSelectionList.length() == 0 )
		{
			merr << GetName() << ": Nothing specified to apply a material to" << std::endl;
			return MS::kFailure;
		}

		MStringArray mStringArray;
		optSelectionList.getSelectionStrings( mStringArray );

		MString vmtPath( "debug/debugempty" );
		if ( mArgDatabase.isFlagSet( kOptVmtPath ) )
		{
			mArgDatabase.getFlagArgument( kOptVmtPath, 0, vmtPath );
		}

		char buf0[ MAX_PATH ];
		char buf1[ MAX_PATH ];

		// Clean up the passed material name
		if ( !g_pFullFileSystem->FullPathToRelativePath( vmtPath.asChar(), buf0, sizeof( buf0 ) ) )
		{
			Q_strncpy( buf0, vmtPath.asChar(), sizeof( buf0 ) );
		}

		Q_FixSlashes( buf0, '/' );
		if ( !Q_strnicmp( "materials/", buf0, 10 ) )
		{
			Q_StripExtension( buf0 + 10, buf1, sizeof( buf1 ) );
		}
		else
		{
			Q_StripExtension( buf0, buf1, sizeof( buf1 ) );
		}
		Q_StripTrailingSlash( buf1 );
		vmtPath = buf1;

		MString optName;
		if ( mArgDatabase.isFlagSet( kOptName ) )
		{
			mArgDatabase.getFlagArgument( kOptName, 0, optName );
		}
		else if ( vmtPath == "debug/debugempty" )
		{
			optName = "vsVmt";
		}

		MString melCmd;
		melCmd += "{\n";
		if ( optName.length() )
		{
			melCmd += "  string $optName = \"";
			melCmd += optName;
			melCmd += "\";\n";
		}
		else
		{
			melCmd += "  string $optName = `basenameEx \"";
			melCmd += vmtPath;
			melCmd += "\"`;\n";
		}

		melCmd += "  string $material = `shadingNode -asShader -n $optName vsVmt`;\n";
		melCmd += "  setAttr -type \"string\" ( $material + \".vmtPath\" ) \"";
		melCmd += vmtPath;
		melCmd += "\";\n";
		melCmd += "  string $shadingGroup = `sets -renderable true -noSurfaceShader true -empty -name ( $optName + \"SG\" )`;\n";
		melCmd += "  connectAttr -f ( $material + \".outColor\" ) ( $shadingGroup + \".surfaceShader\" );\n";
		melCmd += "  sets -e -fe $shadingGroup";
		for ( uint ssi( 0 ); ssi != mStringArray.length(); ++ssi )
		{
			melCmd += " \"";
			melCmd += mStringArray[ ssi ];
			melCmd += "\"";
		}
		melCmd += ";\n";
		melCmd += "}\n";

		m_undo.DagModifier().commandToExecute( melCmd );
		m_undo.DagModifier().doIt();

		MSelectionList sList;
		MGlobal::getActiveSelectionList( sList );
		MStringArray sArray;
		sList.getSelectionStrings( sArray );
		setResult( sArray );

		return MS::kSuccess;
	}

	merr << GetName() << ": No action specified, nothing to do" << std::endl;
	return MS::kFailure;
}