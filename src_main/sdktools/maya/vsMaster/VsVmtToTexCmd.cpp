//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVsVmtToTex
//
//=============================================================================

// Maya includes

#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>
#include <maya/MSyntax.h>
#include <maya/MVector.h>
#include <maya/MProgressWindow.h>

// Valve includes

#include "valveMaya.h"
#include "VsMayaMPxFactory.h"
#include "filesystem.h"
#include "tier2/tier2.h"
#include "tier1/utlbuffer.h"
#include "vstdlib/iprocessutils.h"
#include "KeyValues.h"
#include "p4lib/ip4.h"

// Local includes


class CVsVmtToTex : public CVsMayaMPxCommand
{
public:
	CVsVmtToTex();

	virtual ~CVsVmtToTex();

	// Inheritied from MPxCommand
	virtual MStatus doIt( const MArgList &mArgList );
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const { return m_undoable; }
	virtual bool hasSyntax() const { return true; }

	// Inheritied from CVsMayaCommand
	void SpecifySyntax( MSyntax &mSyntax, vm::CMSyntaxHelp &help );

	enum PathType
	{
		kUnknown,
		kVmt,
		kVtf,
		kTga
	};

	static PathType GetPathType( const char *const pExt );

	static MStatus VmtPathToMaterialPath( const MString &vmtPath, MString &materialPath );

	static MStatus CalcPaths(
		const MString &path,
		PathType &pathType,
		MString &materialPath,
		MString &vmtPath,
		MString &vtfPath,
		MString &tgaPath );

	static void GetGamePath(
		const MString &path,
		MString &gamePath );

protected:
	bool m_undoable;						// Is the command undoable or not
	MArgDatabase *m_mArgDatabase;			// The stored arg database (for undo/redo)
	MDagModifier *m_mDagModifier;			// The dag modifier (for undo/redo)
	MSelectionList m_mSelectionList;		// The previous selection list (for undo/redo)
};


// Statics

//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVsVmtToTex,
	vsVmtToTex,
	"Creates and manipulates vstVmtToTex nodes" );


//-----------------------------------------------------------------------------
//
// Purpose: Creator
// Input  :
// Output :
//
//-----------------------------------------------------------------------------
CVsVmtToTex::CVsVmtToTex() : m_undoable( false ), m_mArgDatabase( NULL ), m_mDagModifier( NULL )
{
}


//-----------------------------------------------------------------------------
//
// Purpose: Destructor
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

CVsVmtToTex::~CVsVmtToTex()
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
//
// Purpose: MSyntax factory for the command
// Input  :
// Output : A Maya MSyntax class describing command's command line syntax
//
//-----------------------------------------------------------------------------

static const char *const kOptHelp( "h" );
static const char *const kOptCreate( "c" );
static const char *const kOptName( "n" );
static const char *const kOptSelect( "s" );
static const char *const kOptAssign( "a" );
static const char *const kOptVmtPath( "v" );
static const char *const kOptBrowse( "b" );

void CVsVmtToTex::SpecifySyntax( MSyntax &mSyntax, vm::CMSyntaxHelp &help )
{
	help.AddFlag( mSyntax, kOptHelp, "help", "General",
		"Prints this information" );
	help.AddFlag( mSyntax, kOptSelect, "select", "General",
		"Selects all vsVmtToTex nodes in the scene" );
	help.AddFlag( mSyntax, kOptCreate, "create", "Create",
		"Creates a new vsVmtToTex and assigns it to the current selection." );
	help.AddFlag( mSyntax, kOptName, "name", "Create",
		"Names the newly created shading group", MSyntax::kString );
	help.AddFlag( mSyntax, kOptVmtPath, "vmtPath", "Create",
		"Specifies either the relative or absolute the vmt material file to be associated\n"
		"  with the created vsVmtToTex node", MSyntax::kString );
	help.AddFlag( mSyntax, kOptBrowse, "browse", "Create",
		"Brings up a file browser to browse to the vmt material file to be associated\n"
		"  with the created vsVmtToTex node.  Note, -vmtPath overrides -browse." );

	mSyntax.setObjectType( MSyntax::kSelectionList );
	mSyntax.useSelectionAsDefault( true );
	mSyntax.enableEdit( false );
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

MStatus CVsVmtToTex::doIt( const MArgList &mArgList )
{
	// Specifying maya nodes on the command line doesn't work just saving
	// a copy of the arg list... probably it keeps pointers to strings
	// or something stupid like that, so have to parse the command line
	// into an MArgDatabase here and since there's no way to clean out
	// one, create a new one

	MStatus mStatus( MS::kSuccess );
	MArgDatabase *mArgDatabase = new MArgDatabase( syntax(), mArgList, &mStatus );

	if ( mArgDatabase ) 
	{
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
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

MStatus CVsVmtToTex::undoIt()
{
	if ( m_undoable )
	{
		// Undo it... whatever that takes but the scene should be restored
		//            to the exact same state as it was just before doIt()
		//            was called (this includes active selection, etc...)

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
// Takes a path to a vmt file on disk and figures out a material path based on it
//-----------------------------------------------------------------------------
MStatus CVsVmtToTex::VmtPathToMaterialPath(
	const MString &vmtPath,
	MString &materialPath )
{
	char pTmpBuf0[ MAX_PATH ];
	char pTmpBuf1[ MAX_PATH ];

	MString ext;

	Q_strncpy( pTmpBuf0, vmtPath.asChar(), sizeof( pTmpBuf0 ) );
	Q_FixSlashes( pTmpBuf0 );
	if ( !g_pFullFileSystem->FullPathToRelativePath( pTmpBuf0, pTmpBuf1, sizeof( pTmpBuf1 ) ) )
	{
		merr << "Path is not under current VPROJECT(" << getenv( "VPROJECT" ) << ") search path" << std::endl;
		return MS::kFailure;
	}

	Q_FixSlashes( pTmpBuf0, '/' );
	Q_StripExtension( pTmpBuf0, pTmpBuf1, sizeof( pTmpBuf1 ) );

	MStringArray mStringArray;
	materialPath.clear();
	MString( pTmpBuf1 ).split( '/', mStringArray );
	for ( uint pi( 0 ); pi != mStringArray.length(); ++pi )
	{
		if ( ( mStringArray[ pi ] == "materials" ) || ( mStringArray[ pi ] == "materialsrc" ) )
		{
			if ( pi + 1 != mStringArray.length() )
			{
				materialPath = mStringArray[ pi + 1 ];
				for ( uint si( pi + 2 ); si < mStringArray.length(); ++si )
				{
					materialPath += "/";
					materialPath += mStringArray[ si ];
				}
			}
			break;
		}
	}

	if ( !materialPath.length() )
	{
		materialPath = "debug/debugempty";
		return MS::kFailure;
	}

	Q_FixSlashes( const_cast< char * >( materialPath.asChar() ), '/' );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Given an extension, returns a file type
//-----------------------------------------------------------------------------
CVsVmtToTex::PathType CVsVmtToTex::GetPathType(
		const char *const pExt )
{
	if ( !Q_stricmp( "vmt", pExt ) )
		return kVmt;

	if ( !Q_stricmp( "vtf", pExt ) )
		return kVtf;

	if ( !Q_stricmp( "tga", pExt ) )
		return kTga;

	return kUnknown;
}


//-----------------------------------------------------------------------------
// Changes /content/([^\/]*)/materialsrc/ to /game/\1/materials/
// And adjusts to the proper path separator
//-----------------------------------------------------------------------------
void CVsVmtToTex::GetGamePath(
	const MString &path,
	MString &gamePath )
{
	gamePath = path;
	Q_FixSlashes( const_cast< char * >( gamePath.asChar() ), '/' );
	MStringArray split;
	gamePath.split( '/', split );
	for ( uint i = 0U; i < split.length() - 2 ; ++i )
	{
		if ( !Q_stricmp( "content", split[ i ].asChar() ) && !Q_stricmp( "materialsrc", split[ i + 2 ].asChar() ) )
		{
			split[ i ] = "game";
			split[ i + 2 ] = "materials";
			gamePath = split[ 0 ];
			for ( uint j = 1U; j < split.length(); ++j )
			{
				gamePath += "/";
				gamePath += split[ j ];
			}
			break;
		}
	}

	Q_FixSlashes( const_cast< char *>( gamePath.asChar() ) );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
KeyValues *FindKey( KeyValues *pKeyValues, const char *pKeyString );


//-----------------------------------------------------------------------------
// Return a relative texture path based on the given vmt path
//-----------------------------------------------------------------------------
void VmtToTex(
	const MString &materialPath,
	const MString &textureId,
	MString &texturePath )
{
	texturePath = materialPath;

	MString vmtPath( "materials/" + materialPath + ".vmt" );
	Q_FixSlashes( const_cast< char * >( vmtPath.asChar() ) );

	CUtlBuffer utlBuf( 0, 0, CUtlBuffer::TEXT_BUFFER );
	if ( g_pFullFileSystem->ReadFile( vmtPath.asChar() , "game", utlBuf ) )
	{
		KeyValues *pKeyValues( new KeyValues( "" ) );
		if ( pKeyValues->LoadFromBuffer( vmtPath.asChar(), utlBuf ) )
		{
			KeyValues *pTextureKey = FindKey( pKeyValues, textureId.asChar() );
			const char *pTexturePath( NULL );

			if ( pTextureKey )
			{
				pTexturePath = pTextureKey->GetString();
			}

			if ( pTexturePath && *pTexturePath != '\0')
			{
				texturePath = pTexturePath;
				Q_FixSlashes( const_cast< char * >( texturePath.asChar() ) );
			}
			else
			{
				merr << "Can't find \"" << textureId << "\" In \"" << vmtPath << "\"" << std::endl;
			}
		}
		else
		{
			merr << "Can't open \"" << vmtPath << "\" from \"game\"" << std::endl;
		}

		pKeyValues->deleteThis();
	}
	else
	{
		merr << "Can't open \"" << vmtPath << "\"" << std::endl;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VmtPathFromRelativePath(
	const MString &materialPath,
	MString &vmtPath )
{
	vmtPath = "materials/" + materialPath + ".vmt";
	Q_FixSlashes( const_cast< char * >( vmtPath.asChar() ) );
	char pTmpBuf0[ MAX_PATH ];
	g_pFullFileSystem->RelativePathToFullPath( vmtPath.asChar(), "game", pTmpBuf0, sizeof( pTmpBuf0 ) );
	Q_FixSlashes( pTmpBuf0 );
	vmtPath = pTmpBuf0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void VtfPathFromRelativePath(
	const MString &materialPath,
	MString &vtfPath )
{
	vtfPath = "materials/" + materialPath + ".vtf";
	Q_FixSlashes( const_cast< char * >( vtfPath.asChar() ) );
	char pTmpBuf0[ MAX_PATH ];
	g_pFullFileSystem->RelativePathToFullPath( vtfPath.asChar(), "game", pTmpBuf0, sizeof( pTmpBuf0 ) );
	Q_FixSlashes( pTmpBuf0 );
	vtfPath = pTmpBuf0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TgaPathFromRelativePath(
	const MString &materialPath,
	MString &tgaPath )
{
	tgaPath = "materials/" + materialPath + ".vtf";
	Q_FixSlashes( const_cast< char * >( tgaPath.asChar() ) );

	char pTmpBuf0[ MAX_PATH ];
	char pTmpBuf1[ MAX_PATH ];
	g_pFullFileSystem->RelativePathToFullPath( tgaPath.asChar(), "game", pTmpBuf0, sizeof( pTmpBuf0 ) );
	Q_FixSlashes( pTmpBuf0, '/' );
	Q_StrSubst( pTmpBuf0, "/materials/", "/materialsrc/", pTmpBuf1, sizeof( pTmpBuf1 ) );
	Q_StrSubst( pTmpBuf1, "/game/", "/content/", pTmpBuf0, sizeof( pTmpBuf0 ) );
	Q_FixSlashes( pTmpBuf0 );
	Q_StripExtension( pTmpBuf0, pTmpBuf1, sizeof( pTmpBuf1 ) );
	Q_SetExtension( pTmpBuf1, ".tga", sizeof( pTmpBuf1 ) );
	tgaPath = pTmpBuf1;
}


//-----------------------------------------------------------------------------
// Takes a path to a vmt file on disk and figures out a material path based on it
//-----------------------------------------------------------------------------
MStatus CVsVmtToTex::CalcPaths(
	const MString &path,
	PathType &pathType,
	MString &materialPath,
	MString &vmtPath,
	MString &vtfPath,
	MString &tgaPath )
{
	MString gamePath;
	GetGamePath( path, gamePath );

	char pTmpBuf0[ MAX_PATH ];
	char pTmpBuf1[ MAX_PATH ];

	Q_strncpy( pTmpBuf0, gamePath.asChar(), sizeof( pTmpBuf0 ) );
	Q_FixSlashes( pTmpBuf0 );

	if ( !g_pFullFileSystem->FullPathToRelativePath( pTmpBuf0, pTmpBuf1, sizeof( pTmpBuf1 ) ) )
	{
		merr << "Path is not under current VPROJECT(" << getenv( "VPROJECT" ) << ") search path" << std::endl;
		return MS::kFailure;
	}

	const MString relativePath( pTmpBuf1 );

	Q_ExtractFileExtension( pTmpBuf0, pTmpBuf1, sizeof( pTmpBuf1 ) );

	pathType = GetPathType( pTmpBuf1 );
	if ( pathType == kUnknown )
	{
		merr << "Unknown path type \"" << pTmpBuf1 << "\"" << std::endl;
		return MS::kFailure;
	}

	const MString ext( pTmpBuf1 );

	Q_FixSlashes( pTmpBuf0, '/' );
	Q_StripExtension( pTmpBuf0, pTmpBuf1, sizeof( pTmpBuf1 ) );

	MStringArray mStringArray;
	materialPath.clear();
	MString( pTmpBuf1 ).split( '/', mStringArray );
	for ( uint pi( 0 ); pi != mStringArray.length(); ++pi )
	{
		if ( ( mStringArray[ pi ] == "materials" ) || ( mStringArray[ pi ] == "materialsrc" ) )
		{
			if ( pi + 1 != mStringArray.length() )
			{
				materialPath = mStringArray[ pi + 1 ];
				for ( uint si( pi + 2 ); si < mStringArray.length(); ++si )
				{
					materialPath += "/";
					materialPath += mStringArray[ si ];
				}
			}
			break;
		}
	}

	if ( !materialPath.length() )
	{
		materialPath = "debug/debugempty";
	}

	Q_FixSlashes( const_cast< char * >( materialPath.asChar() ), '/' );

	if ( pathType == kVmt )
	{
		MString texPath;
		VmtToTex( materialPath, "$basetexture", texPath );
		vmtPath = path;
		VtfPathFromRelativePath( texPath, vtfPath );
		TgaPathFromRelativePath( texPath, tgaPath );
		Q_FixSlashes( const_cast< char * >( vmtPath.asChar() ) );
	}
	else if ( pathType == kVtf )
	{
		vtfPath = path;
		Q_FixSlashes( const_cast< char * >( vtfPath.asChar() ) );
		VmtPathFromRelativePath( materialPath, vmtPath );
		TgaPathFromRelativePath( materialPath, tgaPath );
	}
	else
	{
		tgaPath = path;
		Q_FixSlashes( const_cast< char * >( tgaPath.asChar() ) );
		VmtPathFromRelativePath( materialPath, vmtPath );
		VtfPathFromRelativePath( materialPath, vtfPath );
	}

	return MS::kSuccess;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpewRetval_t SpewIgnoreFunc( SpewType_t, char const * )
{	
	return SPEW_CONTINUE;
}

//-----------------------------------------------------------------------------
// Adds a file to perforce if it's under the current client spec and isn't
// already in perforce
//-----------------------------------------------------------------------------
MStatus AddToPerforce(
	const MString &path )
{
	MStatus retVal;

	SpewOutputFunc_t oldSpewOutputFunc = GetSpewOutputFunc();
	SpewOutputFunc( SpewIgnoreFunc );

	char pTmpBuf0[ MAX_PATH ];
	if ( g_pFullFileSystem->RelativePathToFullPath( path.asChar(), "game", pTmpBuf0, sizeof( pTmpBuf0 ) ) == NULL )
	{
		Q_strncpy( pTmpBuf0, path.asChar(), sizeof( pTmpBuf0 ) );
	}

	if ( !p4->IsFileInPerforce( pTmpBuf0 ) )
	{
		if ( !p4->OpenFileForAdd( pTmpBuf0 ) )
		{
			SpewOutputFunc( oldSpewOutputFunc );
			merr << "Couldn't add \"" << pTmpBuf0 << "\" to perforce" << std::endl;
			retVal = MS::kFailure;
		}
		else
		{
			SpewOutputFunc( oldSpewOutputFunc );
		}

		minfo << p4->GetLastError();
		minfo.flush();
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// Runs vtex on a TGA file to turn it into a vtf file if the vtf file doesn't exist
//-----------------------------------------------------------------------------
MStatus RunVtex(
	const MString &tgaPath,
	MString &vtfPath )
{
	if ( !g_pFullFileSystem->FileExists( vtfPath.asChar() ) )
	{
		char pTmpBuf0[ 1024 * 1024 ];

		const MString cmd( "vtex -mkdir -nopause \"" + tgaPath + "\"" );

		MProgressWindow::reserve();
		MProgressWindow::setTitle( "Running vtex" );
		MProgressWindow::setProgressStatus( cmd );
		MProgressWindow::setProgressRange( 1, 10 );
		MProgressWindow::startProgress();

		ProcessHandle_t pHandle( g_pProcessUtils->StartProcess( cmd.asChar(), true ) );
		for ( int i = 0; !MProgressWindow::isCancelled() && !g_pProcessUtils->IsProcessComplete( pHandle ) && i < 100; ++i )
		{
			MProgressWindow::advanceProgress( 1 );
			Sleep( 200 );
		}

		if ( MProgressWindow::isCancelled() )
		{
			MProgressWindow::setProgressStatus( "Aborting vtex - user abort" );
			g_pProcessUtils->AbortProcess( pHandle );
			minfo << "vtex command aborted by user: " << cmd << std::endl;
			MProgressWindow::endProgress();
			return MS::kFailure;
		}

		if ( !g_pProcessUtils->IsProcessComplete( pHandle ) )
		{
			MProgressWindow::setProgressStatus( "Aborting vtex - process took too long" );
			g_pProcessUtils->AbortProcess( pHandle );
			minfo << "vtex command failed: " << cmd << std::endl;
			MProgressWindow::endProgress();
			return MS::kFailure;
		}

		MProgressWindow::endProgress();

		const int vtexExit( g_pProcessUtils->GetProcessExitCode( pHandle ) );
		if ( vtexExit != 0 )
		{
			mwarn << "Non-Zero exit code from vtex command: " << cmd << std::endl;
			g_pProcessUtils->GetProcessOutput( pHandle, pTmpBuf0, sizeof( pTmpBuf0 ) );
			merr << pTmpBuf0 << std::endl;
		}

		if ( g_pFullFileSystem->RelativePathToFullPath( vtfPath.asChar(), "game", pTmpBuf0, sizeof( pTmpBuf0 ) ) == NULL )
		{
			merr << "vtex command failed: " << cmd << std::endl;
			return MS::kFailure;
		}

		Q_FixSlashes( pTmpBuf0 );
		vtfPath = pTmpBuf0;

		if ( !g_pFullFileSystem->FileExists( vtfPath.asChar() ) )
		{
			merr << "vtex command failed: " << cmd << std::endl;
			return MS::kFailure;
		}
	}

	return AddToPerforce( vtfPath );
}


//-----------------------------------------------------------------------------
// Make a simple vmt file if one doesn't already exist
//-----------------------------------------------------------------------------
MStatus MakeVmt(
	const MString &materialPath,
	MString &vmtPath )
{
	if ( !g_pFullFileSystem->FileExists( vmtPath.asChar() ) )
	{
		KeyValues *pKeyValues = new KeyValues( "VertexLitGeneric", "$basetexture", materialPath.asChar() );

		MStatus retVal;

		if ( !pKeyValues->SaveToFile( g_pFullFileSystem, vmtPath.asChar() ) )
		{
			merr << "Couldn't write VMT file \"" << vmtPath << "\"" << std::endl;
			retVal = MS::kFailure;
		}

		pKeyValues->deleteThis();
	}

	return AddToPerforce( vmtPath );
}


//-----------------------------------------------------------------------------
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------
MStatus CVsVmtToTex::redoIt()
{
	MStatus mStatus;

	if ( !mStatus )
	{
		setResult( MString( "Cannot parse command line" ) + mStatus.errorString() );
		return MS::kFailure;
	}

	if ( m_mArgDatabase->isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( GetName(), GetDesc() );
	}
	else
	{
		if ( m_mArgDatabase->isFlagSet( "create" ) )
		{
			if ( m_mArgDatabase->isFlagSet( kOptBrowse ) )
				return MGlobal::executeCommand( "vsVmtBrowse" );

			// Save the current selection just in case we want to undo stuff
			MGlobal::getActiveSelectionList( m_mSelectionList );

			MSelectionList optSelectionList;
			m_mArgDatabase->getObjects( optSelectionList );

			if ( optSelectionList.length() == 0 )
			{
				merr << "Nothing specified to apply a material to" << std::endl;
				return MS::kFailure;
			}

			m_undoable = true;

			MStringArray mStringArray;
			optSelectionList.getSelectionStrings( mStringArray );

			MString inPath;
			if ( m_mArgDatabase->isFlagSet( kOptVmtPath ) )
			{
				m_mArgDatabase->getFlagArgument( kOptVmtPath, 0, inPath );
			}

			MString materialPath;

			{
				PathType pathType;
				MString vmtPath;
				MString vtfPath;
				MString tgaPath;

				if ( CalcPaths( inPath, pathType, materialPath, vmtPath, vtfPath, tgaPath ) != MS::kSuccess )
					return MS::kFailure;

				if ( pathType == kTga )
				{
					AddToPerforce( tgaPath );
					RunVtex( tgaPath, vtfPath );
					MakeVmt( materialPath, vmtPath );
				}
			}

			MString optName;
			if ( m_mArgDatabase->isFlagSet( kOptName ) )
			{
				m_mArgDatabase->getFlagArgument( kOptName, 0, optName );
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
				melCmd += materialPath;
				melCmd += "\"`;\n";
			}

			melCmd += "  string $vmtToTex = `shadingNode -asUtility -n ( $optName + \"_vmt\" ) vsVmtToTex`;\n";
			melCmd += "  setAttr -type \"string\" ( $vmtToTex + \".materialPath\" ) \"";
			melCmd += materialPath;
			melCmd += "\";\n";
			melCmd += "  string $file = `shadingNode -asTexture -n ( $optName + \"File\" ) file`;\n";
			melCmd += "  string $material = `shadingNode -asShader -n $optName lambert`;\n";
			melCmd += "  string $shadingGroup = `sets -renderable true -noSurfaceShader true -empty -name ( $optName + \"SG\" )`;\n";
			melCmd += "  connectAttr ( $vmtToTex + \".texturePath\" ) ( $file + \".fileTextureName\" );\n";
			melCmd += "  connectAttr ( $file + \".outColor\" ) ( $material + \".color\" );\n";
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

			MGlobal::executeCommand( melCmd, false, true );

			MSelectionList sList;
			MGlobal::getActiveSelectionList( sList );
			MStringArray sArray;
			sList.getSelectionStrings( sArray );
			setResult( sArray );
		}
		else if ( m_mArgDatabase->isFlagSet( "select" ) )
		{
			MSelectionList mSelectionList;
			MDagPath mDagPath;

			for ( MItDag dagIt; !dagIt.isDone(); dagIt.next() )
			{
				if ( MFnDependencyNode( dagIt.item() ).typeName() == GetName() )
				{
					dagIt.getPath( mDagPath );
					mSelectionList.add( mDagPath, MObject::kNullObj, true );
				}
			}

			if ( mSelectionList.length() )
			{
				m_undoable = true;
				// Save the current selection just in case we want to undo stuff
				MGlobal::getActiveSelectionList( m_mSelectionList );
				MGlobal::setActiveSelectionList( mSelectionList, MGlobal::kReplaceList );
			}
		}
		else
		{
			displayError( GetName() + ": No valid operation specified via command line arguments\n" );
		}
	}

	return MS::kSuccess;
}