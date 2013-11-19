//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVstSmdIOCommand
//
//=============================================================================


// Maya includes
#include <maya/MAnimControl.h>
#include <maya/MGlobal.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTransform.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MQuaternion.h>
#include <maya/MTime.h>


// Local includes
#include "vstSmdIOCmd.h"
#include "SmdImport.h"
#include "SmdExport.h"
#include "QciExport.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVstSmdIOCmd::CVstSmdIOCmd()
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVstSmdIOCmd::~CVstSmdIOCmd()
{
}


//-----------------------------------------------------------------------------
// Factory
//-----------------------------------------------------------------------------
void *CVstSmdIOCmd::Factory()
{
	return new CVstSmdIOCmd;
}


//-----------------------------------------------------------------------------
// Purpose: Symbols for command line arguments to avoid typos
//-----------------------------------------------------------------------------
static const char *const kOptHelp( "h" );
static const char *const kOptFilename( "f" );

static const char *const kOptImport( "i" );
static const char *const kOptOrigin( "o" );
static const char *const kOptAngles( "a" );
static const char *const kOptVmf( "vmf" );
static const char *const kOptTextureArchive( "t" );
static const char *const kOptGame( "g" );
static const char *const kOptImportSkeleton( "is" );
static const char *const kOptImportType( "it" );

static const char *const kOptExport( "e" );
static const char *const kOptExportType( "et" );
static const char *const kOptFrameStart( "fs" );
static const char *const kOptFrameEnd( "fe" );
static const char *const kOptFrameInc( "fi" );
static const char *const kOptSelection( "sl" );
static const char *const kOptExportInvisible( "ei" );
static const char *const kOptUpAxis( "up" );
static const char *const kOptNodePrefix( "np" );
static const char *const kOptRemovePrefix( "rp" );
static const char *const kOptPrettyPrint( "pp" );
static const char *const kOptWorld( "w" );
static const char *const kOptDoubleSided( "ds" );
static const char *const kOptQci( "qci" );

static const char *const kOptMaterials( "m" );
static const char *const kOptMaterialSrc( "src" );
static const char *const kOptMaterialVtf( "vtf" );
static const char *const kOptMaterialVmt( "vmt" );
static const char *const kOptRelativeMaterials( "rm" );
static const char *const kOptQc( "qc" );
static const char *const kOptQcCollapseBones( "qcl" );
static const char *const kOptQcCenterBonesOnVerts( "qce" );
static const char *const kOptQcSkipBonesInBBox( "qsb" );
static const char *const kOptRunStudiomdl( "stu" );

static const char *const kOptParents( "p" );
static const char *const kOptBindCurrent( "bc" );

static const char *const kOptConfirm( "con" );


//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
// Output : A Maya MSyntax class describing command's command line syntax
//-----------------------------------------------------------------------------
MSyntax CVstSmdIOCmd::SyntaxFactory()
{
	MSyntax mSyntax;

	mSyntax.addFlag( kOptHelp, "help" );
	mSyntax.addFlag( kOptFilename, "filename", MSyntax::kString );
	mSyntax.addFlag( kOptGame, "game", MSyntax::kString );

	mSyntax.addFlag( kOptImport, "import" );
	mSyntax.addFlag( kOptOrigin, "origin", MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble );
	mSyntax.addFlag( kOptAngles, "angles", MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble );
	mSyntax.addFlag( kOptVmf, "valveMapFile" );
	mSyntax.addFlag( kOptTextureArchive, "textureArchive", MSyntax::kString );
	mSyntax.addFlag( kOptImportSkeleton, "importSkeleton", MSyntax::kBoolean );
	mSyntax.addFlag( kOptImportType, "importType", MSyntax::kString );

	mSyntax.addFlag( kOptExport, "export" );
	mSyntax.addFlag( kOptExportType, "exportType", MSyntax::kString );
	mSyntax.makeFlagMultiUse( kOptExportType );
	mSyntax.addFlag( kOptFrameStart, "frameStart", MSyntax::kDouble );
	mSyntax.addFlag( kOptFrameEnd, "frameEnd", MSyntax::kDouble );
	mSyntax.addFlag( kOptFrameInc, "frameIncrement", MSyntax::kDouble );
	mSyntax.addFlag( kOptSelection, "selection" );
	mSyntax.addFlag( kOptUpAxis, "upAxis", MSyntax::kString );
	mSyntax.addFlag( kOptNodePrefix, "nodePrefix", MSyntax::kString );
	mSyntax.addFlag( kOptRemovePrefix, "removePrefix", MSyntax::kString );
	mSyntax.addFlag( kOptExportInvisible, "exportInvisible" );
	mSyntax.addFlag( kOptPrettyPrint, "prettyPrint", MSyntax::kBoolean );
	mSyntax.addFlag( kOptWorld, "world" );
	mSyntax.addFlag( kOptDoubleSided, "doubleSided" );

	mSyntax.addFlag( kOptQci, "qciExport", MSyntax::kString );

	mSyntax.addFlag( kOptMaterials, "materials" );
	mSyntax.addFlag( kOptMaterialSrc, "materialSrc" );
	mSyntax.addFlag( kOptMaterialVtf, "materialVtf" );
	mSyntax.addFlag( kOptMaterialVmt, "materialVmt" );
	mSyntax.addFlag( kOptRelativeMaterials, "relativeMaterials" );

	mSyntax.addFlag( kOptQc, "qcCreate" );
	mSyntax.addFlag( kOptQcCollapseBones, "qcCollapseBones" );
	mSyntax.addFlag( kOptQcCenterBonesOnVerts, "qcCenterBonesOnVerts" );
	mSyntax.addFlag( kOptQcSkipBonesInBBox, "qcSkipBonesInBBox" );

	mSyntax.addFlag( kOptRunStudiomdl, "studiomdl" );

	mSyntax.addFlag( kOptParents, "parents" );

	mSyntax.addFlag( kOptBindCurrent, "bindCurrent" );

	mSyntax.useSelectionAsDefault( true );
	mSyntax.setObjectType( MSyntax::kSelectionList );

	return mSyntax;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void
CVstSmdIOCmd::PrintHelp()
{
	MGlobal::displayInfo(
"\n"
" vsSmdIO - Valve Source Smd Input/Output\n"
"\n"
" Command to export and import data to and from Valve SMD (VRT) files.\n "
" The Valve SMD File Translator is merely a front end to this command which\n"
" is always used to do the actual work.\n"
"\n"
" Command Line Switches:\n"
"\n"
" -h   | -help  . . . . . . . Print this information\n"
"\n"
" -f   | -filename  . . . . . On both import and export, use the specified filename\n"
"\n"
" -i   | -import  . . . . . . Import an SMD file from the file specified by -filename\n"
" -o   | -origin <x> <y> <z>  When importing, translate the resulting model by the specified position\n"
" -a   | -angles <x> <y> <z>  When importing, rotate the resulting model by the specified degree angles\n"
" -vmf | -valveMapFile . . .  Tell vstSmdIO that the import is to go along with a Valve Map file so the orientations are different\n"
" -t   | -textureArchive <$>  Archive all file textures into the specified directory\n"
" -is  | -importSkeleton <?>  Determines whether the skeleton will be imported.  By default only the model mesh is imported\n"
" -it  | -importType  . . . . What sort of data to import.  One of:\n"
"                             reference ( Model )              r, m\n"
"                             animation ( Skeletal Animation ) a, s\n"
"                             NOTE: Defaults to model.  Unlike export type, only 1 import type can be specified\n"
"\n"
" -e   | -export  . . . . . . Export Maya data to the DMX file specified by -filename\n"
" -et  | -exportType <?>  . . What sort of data to export.  One of:\n"
"                             reference ( Model )              r, m\n"
"                             physbox   ( Physics Model )      p\n"
"                             animation ( Skeletal Animation ) a, s\n"
"                             vta       ( Vertex Animation )   v [NOT IMPLEMENTED YET]\n"
"                             NOTE: Only the shortest unique sequence of characters to\n"
"                                   uniquely identify the type is necessary. e.g. -et a.\n"
"                                   The list of acceptable characters is listed at the end of each type.\n"
"                                   To get both a model and its animation in the same smd, specify\n"
"                                   -exportType twice, i.e. -et m -et a\n"
" -fs  | -frameStart <#> . . Specify the start time for animation export.  If not specified, the current active\n"
"                            animation start time for the scene will be used\n"
" -fe  | -frameEnd <#> . . . Specify the end range for animation export.  If not specified, the current active\n"
"                            animation end time for the scene will be used\n"
" -fi  | -frameIncrement <#> Specify the time increment for animation export.  If not specified, the current\n"
"                            active animation increment time for the scene will be used\n"
" -sl  | -selection  . . . . Only export selected objects in the scene\n"
" -up  | -upAxis <?> . . . . Export the data from Maya as if it oriented with the specified up axis,\n"
"                            one of X, Y or Z.  If not specified the default will be Y.  No matter what\n"
"                            the Maya up axis is set to, if you do -upAxis ? there needs to be a corresponding:\n"
"\n"
"                            $upaxis ?\n"
"\n"
"                            command in the .qc file.  Where ? is X, Y or Z.  Note that case is unimportant in\n"
"                            this tool and in .qc, i.e. x, y or z work\n"
" -ds  | -doubleSided  . . . If specified, all meshes in the scene which have .doubleSided true will be output\n"
"                            twice.  NOTE: Use with caution as .doubleSided defaults to true on Maya meshes\n"
" -w   | -world  . . . . . . Collapse the bone hierarchy into world space\n"
" -rp  | -removePrefix <$> . Removes the specified pre-existing node prefix before adding any specified node\n"
"                            prefix via -np on import or export\n"
" -np  | -nodePrefix <$> . . Prefix each node with the specified string.  By default the Maya node name is\n"
"                            used directly in the smd file.  If, for compatibility reasons, each node needs\n"
"                            a prefix this command switch can be used, e.g. -nodePrefix \"ValveBiped.\"\n"
"                            This works on import and export and occurs after any -rp command"
" -ei  | -exportInvisible  . Normally invisible, intermediate & template nodes are ignored, specify this flag\n"
"                            to cause them to be exported as well.\n"
" -rm  | -relativeMaterials  Use relative material paths.  Note that this requires a version of studiomdl\n"
"                            that came out of Valve main after June 14, 2006.  Use of this flag\n"
"                            makes $cdmaterials qc commands unnecessary"
" -pp  | -prettyPrint <!>  . By default SMD's will be pretty printed.  Specify -prettyPrint false to\n"
"                            revert back to old style\n"
" -qci | -qciFile <$>  . . . Export extra data such as attachments, etc... that smd cannot support into\n"
"                            the specified qci file."
" -bc  | -bindCurrent  . . . Use the current pose of the skeleton and mesh as the bind pose\n"
"\n"
" Automation Options:\n"
"\n"
" -m   | -materials  . . . . Do all of -materialSrc, -materialVtf, -materialVmt\n"
" -src | -materialSrc  . . . Copy any textures used on the exported model\n"
" -vtf | -materialVtf  . . . Run vtex on the textures to convert them to vtf files\n"
" -vmt | -materialVmt  . . . Output a simple vmt file (VertexLitGeneric with the texture as $basetexture) for each texture mapped material used\n"
" -qc  | -qcCreate . . . . . Create a simple qc file for compiling the model\n"
" -qcl | -qcCollapseBones  . Output $collapsebones command in qc\n"
" -qce | -qcCenterBonesOnVerts Output $centerbonesonverts command in qc\n"
" -qsb | -qcSkipBonesInBBox  Output $skipbonesinbbox command in qc\n"
"\n"
" NOTE: If any automation options are set the result returned from the command will\n"
"       be a string array of pairs of strings, type and the pathname.\n"
"\n"
);
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
MStatus CVstSmdIOCmd::doIt(
	const MArgList &mArgList )
{
	if ( m_undo.SetArgList( syntax(), mArgList ) )
		return redoIt();

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// Undo the command
//-----------------------------------------------------------------------------
MStatus CVstSmdIOCmd::undoIt()
{
	m_undo.Undo();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Re-do the command
//-----------------------------------------------------------------------------
MStatus CVstSmdIOCmd::redoIt()
{
	MStatus retVal;

	const MArgDatabase &mArgDatabase( m_undo.ArgDatabase() );

	if ( mArgDatabase.isFlagSet( kOptHelp ) )
	{
		PrintHelp();
	}
	else if ( mArgDatabase.isFlagSet( kOptExport ) )
	{
		retVal = DoExport( mArgDatabase );
	}
	else if ( mArgDatabase.isFlagSet( kOptImport ) )
	{
		retVal = DoImport( mArgDatabase );
	}
	else
	{
		MGlobal::displayError( "None of -help, -export or -import specified... nothing to do" );
		retVal = MS::kFailure;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// Purpose: Export the specified bits of the maya scene into the specified file
// Input  : i_mArgDatabase	The command line arguments as passed
// Output : MS::kSuccess if ok, MS::kFailure otherwise
//-----------------------------------------------------------------------------
MStatus CVstSmdIOCmd::DoExport(
	const MArgDatabase &mArgDatabase )
{
	MString optFilename;
	if ( mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename ) != MS::kSuccess || optFilename.length() == 0 )
	{
		MGlobal::displayError( "No filename specified for export" );
		return MS::kFailure;
	}

	MSelectionList optSelectionList;
	if ( GetOptSelection( mArgDatabase, optSelectionList ) != MS::kSuccess )
		return MS::kFailure;

	const uint exportType( GetExportType( mArgDatabase ) );

	const uint exportFlags( GetExportFlags( mArgDatabase ) );

	const uint version(
		mArgDatabase.isFlagSet( kOptRelativeMaterials ) &&
		exportType & ( CSmdExport::kModel | CSmdExport::kPhysModel | CSmdExport::kVertexAnimation ) ? 2 : 1 );

	CSmdExport smdExport( exportType, exportFlags, version );

	smdExport.SetNodeAddPrefix( GetNodeAddPrefix( mArgDatabase ) );
	smdExport.SetNodeDelPrefix( GetNodeDelPrefix( mArgDatabase ) );

	if ( exportType & CSmdExport::kAnimation )
	{
		double fs( 0.0 );
		double fe( 0.0 );
		double fi( 1.0 );

		GetExportFrameRange( mArgDatabase, fs, fe, fi );

		smdExport.SetFrameRange( fs, fe, fi );
	}

	MStringArray result;

	if ( smdExport.DoIt( optFilename, optSelectionList, result ) != MS::kSuccess )
		return MS::kFailure;

	if ( result.length() )
	{
		result.append( "smd" );
		result.append( optFilename );
	}
	else
	{
		result.append( MString( "Exported to " ) + optFilename );
	}

	if ( mArgDatabase.isFlagSet( kOptQci ) )
	{
		MString optQci;
		mArgDatabase.getFlagArgument( kOptQci, 0, optQci );
		MSelectionList qciSelectionList;
		if ( mArgDatabase.isFlagSet( kOptSelection ) )
		{
			mArgDatabase.getObjects( qciSelectionList );
		}

		MStringArray qciResult;
		CQciExport qciExport;
		if ( qciExport.DoIt( optQci, qciSelectionList, qciResult ) && qciResult.length() )
		{
			for ( uint i( 0 ); i != qciResult.length(); ++i )
			{
				result.append( qciResult[ i ] );
			}
		}
	}

	setResult( result );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Purpose: Export the specified bits of the maya scene into the specified file
// Input  : mArgDatabase	The command line arguments as passed
// Output : MS::kSuccess if ok, MS::kFailure otherwise
//-----------------------------------------------------------------------------
MStatus CVstSmdIOCmd::DoImport(
	const MArgDatabase &mArgDatabase )
{
	MString optFilename;
	if ( mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename ) != MS::kSuccess || optFilename.length() == 0 )
	{
		MGlobal::displayError( "No filename specified for import" );
		return MS::kFailure;
	}

	MString optGame;
	if ( mArgDatabase.isFlagSet( kOptGame ) )
	{
		mArgDatabase.getFlagArgument( kOptGame, 0, optGame );
	}

	MString optTextureArchive;
	if ( mArgDatabase.isFlagSet( kOptTextureArchive ) )
	{
		mArgDatabase.getFlagArgument( kOptTextureArchive, 0, optTextureArchive );
	}

	CQcData qcData;
	char fullPath[ MAX_PATH ];
	if ( !_fullpath( fullPath, optFilename.asChar(), sizeof( fullPath ) ) )
	{
		strncpy( fullPath, optFilename.asChar(), sizeof( fullPath ) );
	}
	qcData.GetQcData( fullPath );

	if ( mArgDatabase.isFlagSet( kOptUpAxis ) )
	{
		MString upAxis;
		mArgDatabase.getFlagArgument( kOptUpAxis, 0, upAxis );
		switch ( *upAxis.asChar() )
		{
		case 'x':
		case 'X':
			qcData.m_upAxis = 0;
			break;
		case 'y':
		case 'Y':
			qcData.m_upAxis = 1;
			break;
		case 'z':
		case 'Z':
		default:
			qcData.m_upAxis = 2;
			break;
		}
	}

	CSmdImport smdImport( optGame.asChar(), optTextureArchive.asChar() );
	if ( mArgDatabase.isFlagSet( kOptImportSkeleton ) && !mArgDatabase.isFlagSet( kOptVmf ) )
	{
		mArgDatabase.getFlagArgument( kOptImportSkeleton, 0, smdImport.m_optImportSkeleton );
	}
	smdImport.SetNodeAddPrefix( GetNodeAddPrefix( mArgDatabase ) );
	smdImport.SetNodeDelPrefix( GetNodeDelPrefix( mArgDatabase ) );

	if ( mArgDatabase.isFlagSet( kOptImportType ) )
	{
		MString optImportType;
		if ( mArgDatabase.getFlagArgument( kOptImportType, 0, optImportType ) && (
			*optImportType.asChar() == 'a' || *optImportType.asChar() == 'A' ||
			*optImportType.asChar() == 's' || *optImportType.asChar() == 'S' ) )
		{
			MSelectionList mSelectionList;
			mArgDatabase.getObjects( mSelectionList );
			MDagPath rootDagPath;
			if ( mSelectionList.length() && mSelectionList.getDagPath( 0, rootDagPath ) )
			{
				return smdImport.ImportAnimation( optFilename.asChar(), rootDagPath, qcData, m_undo );
			}
			else
			{
				merr << "Cannot import animation without the root of the skeleton is selected or specified" << std::endl;
				return MS::kFailure;
			}
		}
	}

	MTransformationMatrix topLevel;

	if ( mArgDatabase.isFlagSet( kOptOrigin ) )
	{
		MVector o;
		mArgDatabase.getFlagArgument( kOptOrigin, 0, o.x );
		mArgDatabase.getFlagArgument( kOptOrigin, 1, o.y );
		mArgDatabase.getFlagArgument( kOptOrigin, 2, o.z );

		topLevel.setTranslation( o, MSpace::kObject );
	}

	if ( mArgDatabase.isFlagSet( kOptAngles ) )
	{
		MVector a;
		if ( mArgDatabase.isFlagSet( kOptVmf ) )
		{
			// The angles are specified in Yaw Pitch Roll order ( YZX )
			// but they're still an XYZ rotation
			mArgDatabase.getFlagArgument( kOptAngles, 0, a.y );
			mArgDatabase.getFlagArgument( kOptAngles, 1, a.z );
			mArgDatabase.getFlagArgument( kOptAngles, 2, a.x );
		}
		else
		{
			mArgDatabase.getFlagArgument( kOptAngles, 0, a.x );
			mArgDatabase.getFlagArgument( kOptAngles, 1, a.y );
			mArgDatabase.getFlagArgument( kOptAngles, 2, a.z );
		}

		const MEulerRotation e( a.x / 180.0 * M_PI, a.y / 180.0 * M_PI, a.z / 180.0 * M_PI, MEulerRotation::kXYZ );
		topLevel.rotateBy( e.asQuaternion(), MSpace::kObject );
	}

	if ( mArgDatabase.isFlagSet( kOptVmf ) )
	{
		if ( qcData.m_upAxis == 1U )
		{
			topLevel.rotateBy( MEulerRotation( 90.0 / 180.0 * M_PI, 0.0, 90.0 / 180.0 * M_PI ).asQuaternion(), MSpace::kObject );
		}
		else
		{
			topLevel.rotateBy( MEulerRotation( 0.0, 0.0, 90.0 / 180.0 * M_PI ).asQuaternion(), MSpace::kObject );
		}
	}
	else
	{
		switch ( qcData.m_upAxis )
		{
		case 0U:	// X Up
			if ( MGlobal::isYAxisUp() )
			{
				topLevel.rotateBy( MEulerRotation( -M_PI / 2.0, M_PI / 2.0, 0.0 ).asQuaternion(), MSpace::kObject );
			}
			else
			{
				topLevel.rotateBy( MEulerRotation( 0.0, M_PI / 2.0, 0.0 ).asQuaternion(), MSpace::kObject );
			}
			break;
		case 1U:	// Y Up
			if ( MGlobal::isZAxisUp() )
			{
				topLevel.rotateBy( MEulerRotation( M_PI / 2.0, 0.0, 0.0 ).asQuaternion(), MSpace::kObject );
			}
			break;
		default:
		case 2U:	// Z Up
			if ( MGlobal::isYAxisUp() )
			{
				topLevel.rotateBy( MEulerRotation( -M_PI / 2.0, 0.0, 0.0 ).asQuaternion(), MSpace::kObject );
			}
			break;
		}
	}

	MDagPath mDagPath( smdImport.DoIt( optFilename.asChar(), qcData, topLevel, m_undo ) );

	if ( mDagPath.isValid() && mDagPath.length() )
	{
		if ( mArgDatabase.isFlagSet( kOptVmf ) )
		{
			MFnNumericAttribute nFn;
			MObject aObj( nFn.create( "yUp", "yUp", MFnNumericData::kBoolean, false ) );
			MDagPath sDagPath( mDagPath );
			sDagPath.extendToShapeDirectlyBelow( 0 );
			m_undo.DagModifier().addAttribute( sDagPath.node(), aObj );
			m_undo.DagModifierDoIt();
			MPlug aP( sDagPath.node(), aObj );

			if ( qcData.m_upAxis == 1U )
			{
				aP.setValue( true );
			}
			else
			{
				aP.setValue( false );
			}
		}

		m_undo.SaveCurrentSelection();

		MGlobal::select( mDagPath, MObject::kNullObj, MGlobal::kReplaceList );
		setResult( mDagPath.partialPathName() );

		m_undo.SaveCurrentSelection();
		return MS::kSuccess;
	}

	m_undo.Undo();

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// Get the starting list of objects to export
//-----------------------------------------------------------------------------
MStatus CVstSmdIOCmd::GetOptSelection(
	const MArgDatabase &mArgDatabase,
	MSelectionList &mSelectionList )
{
	if ( mArgDatabase.isFlagSet( kOptSelection ) )
	{
		// Get the user's specified selection of stuff to export
		if ( !mArgDatabase.getObjects( mSelectionList ) )
		{
			MGlobal::displayError( "Cannot get list of specified objects to export" );
			return MS::kFailure;
		}
		else if ( mSelectionList.isEmpty() )
		{
			MGlobal::displayError( "-export -selection specified but nothing is selected" );
			return MS::kFailure;
		}
	}
	else
	{
		MDagPath mDagPath;
		const bool exportInvisible( mArgDatabase.isFlagSet( kOptExportInvisible ) );

		for ( MItDag dagIt( MItDag::kBreadthFirst, MFn::kDagNode ); !dagIt.isDone() && dagIt.depth() <= 1; dagIt.next() )
		{
			if ( dagIt.depth() == 1)
			{
				if ( dagIt.getPath( mDagPath ) )
				{
					if ( exportInvisible || ValveMaya::IsPathVisible( mDagPath ) )
					{
						mSelectionList.add( mDagPath, MObject::kNullObj, true );
					}
				}
			}
		}
	}

	if ( mSelectionList.isEmpty() )
	{
		MGlobal::displayError( "Cannot find anything to export" );
		return MS::kFailure;
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint CVstSmdIOCmd::GetExportType(
	const MArgDatabase &mArgDatabase )
{
	uint retVal( 0 );

	if ( mArgDatabase.isFlagSet( kOptExportType ) )
	{
		MString optExportType;
		MArgList tmpArgList;
		const uint etEnd( mArgDatabase.numberOfFlagUses( kOptExportType ) );
		for ( uint eti( 0 ); eti != etEnd; ++eti )
		{
			mArgDatabase.getFlagArgumentList( kOptExportType, eti, tmpArgList );
			optExportType = tmpArgList.asString( 0 );

			if ( strnicmp( optExportType.asChar(), "r", 1 ) == 0 || strnicmp( optExportType.asChar(), "m", 1 ) == 0 )
			{
				retVal |= CSmdExport::kReference;
			}
			else if ( strnicmp( optExportType.asChar(), "p", 1 ) == 0 )
			{
				retVal |= CSmdExport::kPhysModel;
			}
			else if ( strnicmp( optExportType.asChar(), "a", 1 ) == 0 || strnicmp( optExportType.asChar(), "s", 1 ) == 0 )
			{
				retVal |= CSmdExport::kAnimation;
			}
			else if ( strnicmp( optExportType.asChar(), "v", 1 ) == 0 )
			{
				retVal |= CSmdExport::kVTA;
			}
			else
			{
				MGlobal::displayWarning( MString( "Cannot determine the type of export from -et " ) + optExportType + ", assume reference/model" );
			}
		}
	}

	if ( retVal == 0 )
	{
		retVal = CSmdExport::kReference;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint CVstSmdIOCmd::GetExportFlags(
	const MArgDatabase &mArgDatabase )
{
	uint retVal(
		( mArgDatabase.isFlagSet( kOptExportInvisible ) ? CSmdExport::kOptExportInvisible : 0 ) |
		( mArgDatabase.isFlagSet( kOptWorld ) ? CSmdExport::kOptWorld : 0 ) |
		( mArgDatabase.isFlagSet( kOptMaterials ) ? CSmdExport::kOptMaterials : 0 ) |
		( mArgDatabase.isFlagSet( kOptMaterialSrc ) ? CSmdExport::kOptMaterialSrc : 0 ) |
		( mArgDatabase.isFlagSet( kOptMaterialVtf ) ? CSmdExport::kOptMaterialVtf : 0 ) |
		( mArgDatabase.isFlagSet( kOptMaterialVmt ) ? CSmdExport::kOptMaterialVmt : 0 ) |
		( mArgDatabase.isFlagSet( kOptConfirm ) ? CSmdExport::kOptConfirm : 0 ) |
		( mArgDatabase.isFlagSet( kOptQc ) ? CSmdExport::kOptQc : 0 ) |
		( mArgDatabase.isFlagSet( kOptQcCollapseBones ) ? CSmdExport::kOptQcCollapseBones : 0 ) |
		( mArgDatabase.isFlagSet( kOptQcCenterBonesOnVerts ) ? CSmdExport::kOptQcCenterBonesOnVerts : 0 ) |
		( mArgDatabase.isFlagSet( kOptQcSkipBonesInBBox ) ? CSmdExport::kOptQcSkipBoneInBBox : 0 ) |
		( mArgDatabase.isFlagSet( kOptRunStudiomdl ) ? CSmdExport::kOptRunStudiomdl : 0 ) |
		( mArgDatabase.isFlagSet( kOptDoubleSided ) ? CSmdExport::kOptDoubleSided : 0 ) |
		( mArgDatabase.isFlagSet( kOptBindCurrent ) ? CSmdExport::kOptBindCurrent : 0 )
	);

	bool doPrettyPrint( true );
	if ( mArgDatabase.isFlagSet( kOptPrettyPrint ) )
	{
		mArgDatabase.getFlagArgument( kOptPrettyPrint, 0, doPrettyPrint );
	}

	if ( doPrettyPrint )
	{
		retVal |= CSmdExport::kOptPrettyPrint;
	}

	if ( mArgDatabase.isFlagSet( kOptUpAxis ) )
	{
		MString upAxis;
		mArgDatabase.getFlagArgument( kOptUpAxis, 0, upAxis );
		switch ( *upAxis.asChar() )
		{
		case 'x':
		case 'X':
			retVal |= CSmdExport::kOptXUp;
			break;
		case 'y':
		case 'Y':
			retVal |= CSmdExport::kOptYUp;
			break;
		case 'z':
		case 'Z':
			retVal |= CSmdExport::kOptZUp;
			break;
		default:
			MGlobal::displayWarning( MString( "Unknown up axis \"" ) + upAxis + "\" specified, assuming Y up" );
			retVal |= CSmdExport::kOptYUp;
			break;
		}
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstSmdIOCmd::GetExportFrameRange(
	const MArgDatabase &mArgDatabase,
	double &optFs,
	double &optFe,
	double &optFi )
{
	optFs = MAnimControl::minTime().as( MTime::uiUnit() );
	optFe = MAnimControl::maxTime().as( MTime::uiUnit() );
	optFi = MAnimControl::playbackBy();

	if ( mArgDatabase.isFlagSet( kOptFrameStart ) )
	{
		mArgDatabase.getFlagArgument( kOptFrameStart, 0, optFs );
	}

	if ( mArgDatabase.isFlagSet( kOptFrameEnd ) )
	{
		mArgDatabase.getFlagArgument( kOptFrameEnd, 0, optFe );
	}

	if ( mArgDatabase.isFlagSet( kOptFrameInc ) )
	{
		mArgDatabase.getFlagArgument( kOptFrameInc, 0, optFi );
	}

	if ( optFi > 0 )
	{
		if ( optFs > optFe )
		{
			// Swap start and end times
			const double tmpFrame( optFs );
			optFs = optFe;
			optFe = tmpFrame;
		}
	}
	else
	{
		// Time increment is negative, which isn't supported, so negate time increment & swap ranges and see what happens
		optFi *= -1.0;
		const double tmpFrame( optFs );
		optFs = optFe;
		optFe = tmpFrame;

		if ( optFs > optFe )
		{
			// Swap start and end times back..
			const double tmpFrame( optFs );
			optFs = optFe;
			optFe = tmpFrame;
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MString CVstSmdIOCmd::GetNodeAddPrefix(
	const MArgDatabase &mArgDatabase )
{
	MString retVal;

	if ( mArgDatabase.isFlagSet( kOptNodePrefix ) )
	{
		mArgDatabase.getFlagArgument( kOptNodePrefix, 0, retVal );
		char *pP( const_cast< char *>( retVal.asChar() ) );
		for ( const char *const pEnd( pP + retVal.length() ); pP != pEnd; ++pP )
		{
			if ( *pP == '\"' )
			{
				*pP = '_';
			}
		}
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MString CVstSmdIOCmd::GetNodeDelPrefix(
	const MArgDatabase &mArgDatabase )
{
	MString retVal;

	if ( mArgDatabase.isFlagSet( kOptRemovePrefix ) )
	{
		mArgDatabase.getFlagArgument( kOptRemovePrefix, 0, retVal );
		char *pP( const_cast< char *>( retVal.asChar() ) );
		for ( const char *const pEnd( pP + retVal.length() ); pP != pEnd; ++pP )
		{
			if ( *pP == '\"' )
			{
				*pP = '_';
			}
		}
	}

	return retVal;
}
