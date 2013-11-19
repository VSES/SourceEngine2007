//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVsDmxIOCommand
//
//=============================================================================

#include <time.h>

// Maya includes
#include <maya/MArgList.h>
#include <maya/MAnimControl.h>
#include <maya/MArgDatabase.h>
#include <maya/MBoundingBox.h>
#include <maya/MItDag.h>
#include <maya/MIteratorType.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MTime.h>

// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"
#include "movieobjects/dmeanimationlist.h"
#include "movieobjects/dmechannel.h"
#include "movieobjects/dmeclip.h"
#include "movieobjects/dmedag.h"
#include "movieobjects/dmedccmakefile.h"
#include "movieobjects/dmelog.h"
#include "movieobjects/dmemodel.h"
#include "movieobjects/dmevertexdata.h"
#include "movieobjects/dmecombinationoperator.h"
#include "movieobjects/dmeselection.h"
#include "movieobjects/dmobjserializer.h"
#include "tier1/utlsymbol.h"
#include "filesystem.h"
#include "tier2/tier2.h"

// Local includes
#include "DmeImport.h"
#include "DmeExport.h"


#define DEFAULT_FILE_FORMAT "model"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CDmeMayaData
{
public:
	CDmeMayaData(
		CDmeDag *pDmeDag,
		const MDagPath &mDagPath )
	: m_pDmeDag( pDmeDag )
	, m_mDagPath( mDagPath )
	, m_pDmePositionLog( NULL )
	, m_pDmeOrientationLog( NULL )
	, m_animatedRoot( false )
	{}
	
	const MDagPath m_mDagPath;
	CDmeDag *m_pDmeDag;
	CDmeVector3Log *m_pDmePositionLog;
	CDmeQuaternionLog *m_pDmeOrientationLog;
	bool m_animatedRoot;
};


//-----------------------------------------------------------------------------
// A new Maya command dervied from MPxCommand
//-----------------------------------------------------------------------------
class CVsDmxIOCmd : public CVsMayaMPxCommand
{
public:
	CVsDmxIOCmd();

	virtual ~CVsDmxIOCmd();

	// Inherited from MPxCommand
	virtual MStatus doIt( const MArgList &mArgList );
	virtual MStatus undoIt();
	virtual MStatus redoIt();
	virtual bool isUndoable() const { return m_undo.IsUndoable(); }
	virtual bool hasSyntax() const { return true; }

	// Inherited from CVsMayaMPxCommand
	virtual void SpecifySyntax( MSyntax &syntax, vm::CMSyntaxHelp &help );

protected:
	MStatus Export( const MArgDatabase &mArgDatabase, CDmElement **ppRoot = NULL );

	MStatus Import( const MArgDatabase &mArgDatabase, CDmElement *pRoot = NULL );

	CDmeAnimationList *ExportSkeletonAnimation(
		const MArgDatabase &mArgDatabase,
		const CUtlVector< CDmeMayaData * > &dmeMayaRootList,
		CDmElement *pDmeRoot,
		double optFs,
		double optFe,
		double optFi,
		double optFo,
		bool optKfr,
		const MDagPath &optExportRelativePath );

protected:
	ValveMaya::CUndo m_undo;				// The undo thingy
	MString m_cmdLine;

private:
	static void CVsDmxIOCmd::AddExtraJoints(
		CDmeDag *pDmeDag );

	static CUtlSymbolTable &GetAvailableEncodings( CUtlSymbolTable &encodings );
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXCOMMAND(
	CVsDmxIOCmd,
	vsDmxIO,
	"Command to export and import data to and from Valve DMX files.  "
	"The Valve DMX FIle Translator is merely a front end to this command which is always used to do the actual work."
);


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVsDmxIOCmd::CVsDmxIOCmd()
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVsDmxIOCmd::~CVsDmxIOCmd()
{
}


//-----------------------------------------------------------------------------
//
// Purpose: Symbols for command line arguments to avoid typos
//
//-----------------------------------------------------------------------------

static const char *const kOptHelp( "h" );
static const char *const kOptFilename( "f" );

static const char *const kOptImport( "i" );

static const char *const kOptExport( "e" );
static const char *const kOptExportType( "et" );
static const char *const kOptExportRelative( "er" );
static const char *const kOptAnimationName( "an" );
static const char *const kOptFrameStart( "fs" );
static const char *const kOptFrameEnd( "fe" );
static const char *const kOptFrameInc( "fi" );
static const char *const kOptOutType( "ot" );
static const char *const kOptSelection( "sl" );
static const char *const kOptExportInvisible( "ei" );
static const char *const kOptScratchMaterial( "sm" );
static const char *const kOptMakefileObjectId( "mi" );
static const char *const kOptAscii( "asc" );
static const char *const kOptBinary( "bin" );
static const char *const kOptVstOnly( "vst" );
static const char *const kOptMergeUvs( "muv" );

static const char *const kOptFilter( "flt" );
static const char *const kOptFrameOffset( "fo" );
static const char *const kOptKeepFrameRange( "kfr" );
static const char *const kOptComboMerge( "cm" );
static const char *const kOptTriangulate( "t" );
static const char *const kOptWeightMap( "wm" );

static const char *const kOptBindCurrent( "bc" );

//-----------------------------------------------------------------------------
// Purpose: MSyntax factory for the command
// Output : A Maya MSyntax class describing command's command line syntax
//-----------------------------------------------------------------------------
void CVsDmxIOCmd::SpecifySyntax( MSyntax &mSyntax, vm::CMSyntaxHelp &help )
{
	help.Clear();

	{
		help.AddFlag( mSyntax, kOptHelp, "help", "General", "Print this information" );
		help.AddFlag( mSyntax, kOptFilename, "filename", "General", "On both import and export, use the specified filename", MSyntax::kString );
	}

	{
		help.AddFlag( mSyntax, kOptImport, "import", "Import", "Import a DMX file from the file specified by -filename" );

		help.AddFlag( mSyntax, CDmeImport::m_sOptImportType, CDmeImport::m_sLongOptImportType, "Import",
			"What sort of data to import.  One of [ model, animation ].  Defaults to model."
			"NOTE: Only the shortest unique sequence of characters to uniquely identify the type is necessary. e.g. -it m.",
			MSyntax::kString );

		help.AddFlag( mSyntax, CDmeImport::m_sOptSetTimeline, CDmeImport::m_sLongOptSetTimeline, "Import",
			"If specified, the time slider will be set to match the imported animation" );
	}

	{
		help.AddFlag( mSyntax, kOptExport, "export", "Export", "Export Maya data to the DMX file specified by -filename" );

		help.AddFlag( mSyntax, kOptExportType, "exportType", "Export",
			"What sort of data to export.  One of [ model, skeletalAnimation, physicsModel ].  "
			"NOTE: Only the shortest unique sequence of characters to uniquely identify the type is necessary. e.g. -et s.  "
			"Defaults to model.  Can be specified multiple times.",
			MSyntax::kString );

		mSyntax.makeFlagMultiUse( kOptExportType );

		help.AddFlag( mSyntax, kOptFrameStart, "frameStart", "Export",
			"Specify the start time for animation export.  If not specified, the current active animation start time for the scene will be used",
			MSyntax::kDouble );

		help.AddFlag( mSyntax, kOptFrameEnd, "frameEnd", "Export",
			"Specify the end range for animation export.  If not specified, the current active animation end time for the scene will be used",
			MSyntax::kDouble );

		help.AddFlag( mSyntax, kOptFrameInc, "frameIncrement", "Export",
			"Specify the time increment for animation export.  If not specified, the current active animation increment time for the scene will be used",
			MSyntax::kDouble );

		help.AddFlag( mSyntax, kOptFrameOffset, "frameOffset", "Export",
			"If specified, the actual Maya frame will have this value added to it on export.  Defaults to 0"
			"NOTE: If -frameOffset is specified then it implies -keepFrameRange is specified in that\n"
			"      frame offset is subtracted from the Maya frame range and that is exported.",
			MSyntax::kDouble );

		help.AddFlag( mSyntax, kOptKeepFrameRange, "keepFrameRange", "Export",
			"Normally the frame range from Maya is automatically shifted to begin at frame/time 0 on export.\n"
			"If this option is specified then the frame range will be exported as-is from Maya.\n"
			"NOTE: If -frameOffset is specified then it implies -keepFrameRange is specified in that\n"
			"      frame offset is subtracted from the Maya frame range and that is exported." );

		help.AddFlag( mSyntax, kOptScratchMaterial, "scratchMaterial", "Export",
			"If just a plain Maya surface shader is assigned without a texture map driving\n"
			"the color then a scratch vmt file will be created and those faces will be shaded\n"
			"with a solid color" );

		help.AddFlag( mSyntax, kOptAnimationName, "animationName", "Export",
			"Override the animation name for a vstInfo node.  Specify the vstInfo node name first and then the name of the animation second",
			MSyntax::kString,
			MSyntax::kString );

		help.AddFlag( mSyntax, kOptMergeUvs, "mergeUvs", "Export",
			"Merges any non-unique UV coordinates on export.  Default is true.  If false, UVs are exported verbatim",
			MSyntax::kBoolean );

		help.AddFlag( mSyntax, kOptMakefileObjectId, "makefileObjectId", "Export",
			"Specify the DmObjectId for the DmeMakefile that will be created",
			MSyntax::kString );

		help.AddFlag( mSyntax, kOptTriangulate, "triangulate", "Export",
			"If true, triangulate any polygonal faces that are not convex, have holes, are starlike or lamina" );

		help.AddFlag( mSyntax, kOptAscii, "ascii", "Export",
			"A shortcut for -outType keyvalues2" );

		help.AddFlag( mSyntax, kOptBinary, "binary", "Export",
			"A shortcut for -outType binary" );

		help.AddFlag( mSyntax, kOptWeightMap, "weightMap", "Export",
			"Create a weight map for each delta state on the mesh" );

		MString outTypes( "<" );

		if ( g_pDataModel )
		{
			for ( int i( 0 ); i < g_pDataModel->GetEncodingCount();  ++i )
			{
				outTypes += " ";
				outTypes +=  g_pDataModel->GetEncodingName( i );
			}
			outTypes += " >";
		}

		help.AddFlag( mSyntax, kOptOutType, "outType", "Export", MString ( MString( "Override the output file type where the specified type is one of " ) + outTypes ).asChar(), MSyntax::kString );

		help.AddFlag( mSyntax, kOptSelection, "selection", "Export", "Only export selected objects in the scene.  NOTE: If -selection is specified, -exportInvisible is also implied." );

		help.AddFlag( mSyntax, kOptVstOnly, "vstOnly", "Export", "Only export data under a vstInfo (Valve Source Tools - Info node)." );

		help.AddFlag( mSyntax, kOptExportInvisible, "exportInvisible", "Export", "Normally invisible, intermediate & template nodes are ignored, specify this flag to cause them to be exported as well." );

		help.AddFlag( mSyntax, kOptComboMerge, "comboMerge", "Export",
			"Merge the first DmeCombinationOperator found in the specified path with the exported data replacing any existing DmeCombination operator",
			MSyntax::kString );

		help.AddFlag( mSyntax, kOptExportRelative, "exportRelative", "Export",
			"Exports the model or animation as if the specified dag node was the origin",
			MSyntax::kSelectionItem );
	}

	{
		help.AddFlag( mSyntax, kOptFilter, "filter", "Filter", "Essentially do an export and then an import without a round trip to disk, all import & export options apply" );
	}

	help.AddFlag( mSyntax, kOptBindCurrent, "bindCurrent", "Export", "Use the current pose as the bind pose when exporting" );

	mSyntax.useSelectionAsDefault( true );
	mSyntax.setObjectType( MSyntax::kSelectionList );
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
MStatus CVsDmxIOCmd::doIt(
	const MArgList &mArgList )
{
	m_cmdLine = "vsDmxIO ";

	const uint aEnd( mArgList.length() );
	for ( uint ai( 0 ); ai != aEnd; ++ai )
	{
		MString arg( mArgList.asString( ai ) );
		if ( arg.index( ' ' ) >= 0 || arg.index( '\t' ) >= 0 || arg.index( '\n' ) >= 0 )
		{
			arg = MString( "\"" ) + arg + "\"";
		}
		m_cmdLine += " ";
		m_cmdLine += arg;
	}
	m_cmdLine += ";";

	m_undo.SetArgList( syntax(), mArgList );
	return redoIt();
}


//-----------------------------------------------------------------------------
// Undo the command
//-----------------------------------------------------------------------------
MStatus CVsDmxIOCmd::undoIt()
{
	m_undo.Undo();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Re-do the command
//-----------------------------------------------------------------------------
MStatus CVsDmxIOCmd::redoIt()
{
	MStatus retVal;

	const MArgDatabase &mArgDatabase = m_undo.ArgDatabase();

	if ( mArgDatabase.isFlagSet( kOptHelp ) )
	{
		GetSyntaxHelp()->PrintHelp( "vsDmxIO",
			" Imports and exports DMX files to and from Maya" );
	}
	else if ( mArgDatabase.isFlagSet( kOptExport ) )
	{
		retVal = Export( mArgDatabase );
	}
	else if ( mArgDatabase.isFlagSet( kOptImport ) )
	{
		retVal = Import( mArgDatabase );
	}
	else if ( mArgDatabase.isFlagSet( kOptFilter ) )
	{
		CDmElement *pRoot( NULL );

		retVal = Export( mArgDatabase, &pRoot );

		if ( retVal && pRoot )
		{
			retVal = Import( mArgDatabase, pRoot );
		}
		else
		{
			retVal = MS::kFailure;
		}
	}
	else
	{
		merr << "No operation specified for vsDmxIO, must be one of -export, -import or -filter" << std::endl;
		return MS::kFailure;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// Exports skeletal animation
//-----------------------------------------------------------------------------
CDmeAnimationList *CVsDmxIOCmd::ExportSkeletonAnimation(
	const MArgDatabase &mArgDatabase,
	const CUtlVector< CDmeMayaData * > &dmeMayaRootList,
	CDmElement *pDmeRoot,
	double optFs,
	double optFe,
	double optFi,
	double optFo,
	bool optKfr,
	const MDagPath &optExportRelativePath )
{
	if ( dmeMayaRootList.Count() == 0 )
		return NULL;

	// Name the animation - TOOD: Support embedded frame ranges
	// Algorithm for naming animation
	//
	// 1. First use the name of the vstInfo node ( the skeleton name in this case )
	// 2. If there's an animName name string attribute on the Maya node, rename it to that
	// 3. If the user specified an animation name on the command line and it matches
	//    the name of the node being exported, use that

	MString animName( CastElement< CDmeModel >( dmeMayaRootList.Head()->m_pDmeDag ) ? dmeMayaRootList.Head()->m_pDmeDag->GetName() : "vsDmxIO Animation" );

	{
		MPlug animNameP( MFnDagNode( dmeMayaRootList.Head()->m_mDagPath ).findPlug( "animName" ) );
		if ( !animNameP.isNull() )
		{
			MString tmpAnimName;
			animNameP.getValue( tmpAnimName );
			if ( tmpAnimName.length() )
			{
				animName = tmpAnimName;
			}
		}
	}

	if ( mArgDatabase.isFlagSet( kOptAnimationName ) )
	{
		MString optVstInfoName;
		mArgDatabase.getFlagArgument( kOptAnimationName, 0, optVstInfoName );

		const MDagPath &mDagPath( dmeMayaRootList.Head()->m_mDagPath );

		if ( mDagPath.fullPathName() == optVstInfoName ||
			ValveMaya::RemoveNameSpace( mDagPath.fullPathName() ) == optVstInfoName ||
			mDagPath.partialPathName() == optVstInfoName ||
			ValveMaya::RemoveNameSpace( mDagPath.partialPathName() ) == optVstInfoName ||
			ValveMaya::RemoveNameSpace( MFnDagNode( mDagPath ).name() ) == optVstInfoName )
		{
			MString optAnimName;
			mArgDatabase.getFlagArgument( kOptAnimationName, 1, optAnimName );
			if ( optAnimName.length() )
			{
				animName = optAnimName;
			}
		}
	}

	CDmeAnimationList *pAnimationList( CreateElement< CDmeAnimationList >( animName.asChar(), pDmeRoot->GetFileId() ) );
	CDmeChannelsClip *pAnimation( CreateElement< CDmeChannelsClip >( animName.asChar(), pDmeRoot->GetFileId() ) );

	const double secondsPerFrame( MTime( 1.0, MTime::uiUnit() ).as( MTime::kSeconds ) );
	pAnimation->SetValue< int >( "frameRate", static_cast< int >( 1.0 / secondsPerFrame ) );

	pAnimationList->AddAnimation( pAnimation );

	CUtlVector< CDmeMayaData * > dmeMayaDataList;

	{
		// Create the Dme animation structure
		// TODO: Come up with spiffier names

		const int dmeMayaRootEnd( dmeMayaRootList.Count() );
		for ( int dmeMayaRootIndex( 0 ); dmeMayaRootIndex < dmeMayaRootEnd; ++dmeMayaRootIndex )
		{
			CUtlStack< CDmeMayaData * > dmeMayaDataStack;
			const CDmeMayaData *pDmeMayaData( dmeMayaRootList[ dmeMayaRootIndex ] );
			CDmeMayaData *pChildDmeMayaData( new CDmeMayaData( pDmeMayaData->m_pDmeDag, pDmeMayaData->m_mDagPath ) );
			pChildDmeMayaData->m_animatedRoot = true;
			dmeMayaDataStack.Push( pChildDmeMayaData );

			// Walk the DmeDag and find the corresponding Maya nodes...
			// Ignore extra Maya nodes ( nodes in Maya but not in Dme )
			// Warn about missing Maya nodes ( nodes in Dme but not in Maya )
			CUtlVector< int > mayaChildren;

			do
			{
				CDmeMayaData *pDmeMayaData( dmeMayaDataStack.Top() );
				dmeMayaDataStack.Pop();

				const MDagPath &mDagPath( pDmeMayaData->m_mDagPath );
				CDmeDag *pDmeDag( pDmeMayaData->m_pDmeDag );

				// Create the Dme channels for position & orientation
				// on the current node
				// TODO: Detect if this Maya dag node is, in fact, animated!
				// Never animate dmeModel

				bool animated( true );

				CDmeShape *pDmeShape( pDmeDag->GetShape() );
				if ( pDmeShape && pDmeShape->GetValue< int >( "jointCount" ) )
				{
					// Don't animate the transform of shape nodes which are skinned
					animated = false;
				}
				else if ( pDmeDag->IsA( CDmeModel::GetStaticTypeSymbol() ) )
				{
					// Don't animate vstInfo nodes ever
					animated = false;
				}

				// TODO: Don't animate things that aren't in the jointTransform list of the model

				if ( animated )
				{
					CDmeTransform *pDmeTransform( pDmeDag->GetTransform() );

					CDmeChannel *pDmePositionChannel(
						CreateElement< CDmeChannel >(
						( MString( vm::RemoveNameSpace( pDmeTransform->GetName() ) ) + "_p" ).asChar(), pDmeRoot->GetFileId() ) );
					pAnimation->m_Channels.AddToTail( pDmePositionChannel );
					pDmePositionChannel->SetOutput( pDmeTransform, "position" );
					pDmeMayaData->m_pDmePositionLog = pDmePositionChannel->CreateLog< Vector >();
					pDmeMayaData->m_pDmePositionLog->SetValueThreshold( 1.0e-6 );

					CDmeChannel *pDmeOrientationChannel(
						CreateElement< CDmeChannel >(
						( MString( pDmeTransform->GetName() ) + "_o" ).asChar(), pDmeRoot->GetFileId() ) );
					pAnimation->m_Channels.AddToTail( pDmeOrientationChannel );
					pDmeOrientationChannel->SetOutput( pDmeTransform, "orientation" );
					pDmeMayaData->m_pDmeOrientationLog = pDmeOrientationChannel->CreateLog< Quaternion >();
					pDmeMayaData->m_pDmeOrientationLog->SetValueThreshold( 1.0e-6 );

					dmeMayaDataList.AddToTail( pDmeMayaData );
				}

				// Cache the Maya child count to make searching marginally faster when Maya nodes are found
				{
					int mCi( mDagPath.childCount() - 1 );
					for ( mayaChildren.SetCount( mCi + 1 ); mCi >= 0; --mCi )
					{
						mayaChildren[ mCi ] = mCi;
					}
				}

				// Put the children onto the stack for processing
				for ( int dCi( pDmeDag->GetChildCount() - 1 ); dCi >= 0; --dCi )
				{
					CDmeDag *pDmeDagChild( pDmeDag->GetChild( dCi ) );
					const char *const pDmeDagChildName( pDmeDagChild->GetName() );

					// Look for this Dme node as a child of the current Maya Dag
					for ( int mCi( mayaChildren.Count() - 1 ); mCi >= 0; --mCi )
					{
						const MObject mChildObj( mDagPath.child( mayaChildren[ mCi ] ) );
						if ( Q_strcmp( vm::RemoveNameSpace( MFnDependencyNode( mChildObj ).name() ).asChar(), pDmeDagChildName ) == 0 )
						{
							MDagPath mDagPathChild( mDagPath );
							mDagPathChild.push( mDagPath.child( mayaChildren[ mCi ] ) );
							CDmeMayaData *pChildDmeMayaData( new CDmeMayaData( pDmeDagChild, mDagPathChild ) );
							if ( !animated && pDmeMayaData->m_animatedRoot )
							{
								pChildDmeMayaData->m_animatedRoot = true;
							}
							dmeMayaDataStack.Push( pChildDmeMayaData );
							mayaChildren.Remove( mCi );
							break;
						}
					}

					if ( dmeMayaDataStack.Top()->m_pDmeDag != pDmeDagChild )
					{
						Warning( "vsDmxIO: Cannot find a child of Maya node %s to match Dme child %s of DmeDag %s.  No animation data will be exported for that Dme child node or any of its descendents.\n",
							mDagPath.partialPathName().asChar(),
							pDmeDagChild->GetName(),
							pDmeDag->GetName() );
					}
				}

				if ( !animated )
				{
					// Don't need it, isn't animated
					delete pDmeMayaData;
				}
			}
			while ( dmeMayaDataStack.Count() );
		}
	}

	{
		// Just cause it's going to be used a lot
		const MTime::Unit uiUnit( MTime::uiUnit() );

		// Starting time in Dme time units ( 10ths of a millisecond )
		const DmeTime_t dTs( static_cast< int >( 10.0 * MTime( optFs + ( optKfr ? optFo : -optFs ), MTime::uiUnit() ).as( MTime::kMilliseconds ) ) );
		// Current time (Dme)
		DmeTime_t dTc( dTs );
		// Current time (Maya)
		MTime mTc( optFs, uiUnit );
		// Ending time (Maya)
		const MTime mTe( optFe, uiUnit );
		// Time offset (Maya)
		const MTime mTo( optKfr ? optFo : -optFs, uiUnit );

		CDmeMayaData **ppDmeMayaDataStart = dmeMayaDataList.Base();
		CDmeMayaData **ppDmeMayaDataEnd = ppDmeMayaDataStart + dmeMayaDataList.Count();
		CDmeMayaData **ppDmeMayaData;
		const CDmeMayaData *pDmeMayaData;

		MQuaternion o;
		const double *p;

		const MTime savedCurrentTime( MAnimControl::currentTime() );
		pAnimation->SetStartTime( dTs );

		MDagPath mDagPath;
		MIteratorType mIteratorType;
		mIteratorType.setFilterType( MFn::kDagNode );
		MItDag dIt;

		if ( optExportRelativePath.isValid() && optExportRelativePath.length() )
		{
			for ( int fi( 1 ); mTc <= mTe; ++fi )
			{
				MAnimControl::setCurrentTime( mTc );

				// This is a hack to make the exporter simulate Maya's draw cycle
				// So that batch mode acts like GUI mode
				// This came up as a result of parentConstraints doing funny thing
				for ( dIt.reset( mIteratorType, NULL, NULL ); !dIt.isDone(); dIt.next() )
				{
					dIt.getPath( mDagPath );
					MFnDagNode( mDagPath ).boundingBox();
				}

				// Dme Time is 10ths of a millisecond
				dTc = DmeTime_t( static_cast< int >( 10.0 * ( mTc + mTo ).as( MTime::kMilliseconds ) ) );

				for ( ppDmeMayaData = ppDmeMayaDataStart; ppDmeMayaData != ppDmeMayaDataEnd; ++ppDmeMayaData )
				{
					pDmeMayaData = *ppDmeMayaData;
					MMatrix m( MFnDagNode( pDmeMayaData->m_mDagPath ).transformationMatrix() );
					if ( pDmeMayaData->m_animatedRoot )
					{
						m = m * optExportRelativePath.inclusiveMatrixInverse();
					}
					o = m;
					p = m[ 3 ];

					pDmeMayaData->m_pDmeOrientationLog->SetKey( dTc, Quaternion( o.x, o.y, o.z, o.w ) );
					pDmeMayaData->m_pDmePositionLog->SetKey( dTc, Vector( *p, *( p + 1 ), *( p + 2 ) ) );
				}

				mTc = MTime( optFs + optFi * fi, uiUnit );
			}
		}
		else
		{
			for ( int fi( 1 ); mTc <= mTe; ++fi )
			{
				MAnimControl::setCurrentTime( mTc );

				// This is a hack to make the exporter simulate Maya's draw cycle
				// So that batch mode acts like GUI mode
				// This came up as a result of parentConstraints doing funny thing
				for ( dIt.reset( mIteratorType, NULL, NULL ); !dIt.isDone(); dIt.next() )
				{
					dIt.getPath( mDagPath );
					MFnDagNode( mDagPath ).boundingBox();
				}

				// Dme Time is 10ths of a millisecond
				dTc = DmeTime_t( static_cast< int >( 10.0 * ( mTc + mTo ).as( MTime::kMilliseconds ) ) );

				for ( ppDmeMayaData = ppDmeMayaDataStart; ppDmeMayaData != ppDmeMayaDataEnd; ++ppDmeMayaData )
				{
					pDmeMayaData = *ppDmeMayaData;
					const MMatrix m( MFnDagNode( pDmeMayaData->m_mDagPath ).transformationMatrix() );
					o = m;
					p = m[ 3 ];

					pDmeMayaData->m_pDmeOrientationLog->SetKey( dTc, Quaternion( o.x, o.y, o.z, o.w ) );
					pDmeMayaData->m_pDmePositionLog->SetKey( dTc, Vector( *p, *( p + 1 ), *( p + 2 ) ) );
				}

				mTc = MTime( optFs + optFi * fi, uiUnit );
			}
		}

		pAnimation->SetDuration( dTc - dTs );
		MAnimControl::setCurrentTime( savedCurrentTime );
	}

	// Clean up allocated data
	dmeMayaDataList.PurgeAndDeleteElements();

	return pAnimationList;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsDmxIOCmd::AddExtraJoints( CDmeDag *pDmeDag )
{
	// A stack for keeping track of the current DmeModel
	CDmeModel *pDmeModel( NULL );
	CDmeModel *pDmeModelCheck( NULL );

	// A stack for a depth first traversal of the Dme scene
	CUtlStack< CDmeDag * > dmeDagStack;
	dmeDagStack.Push( pDmeDag );

	CDmeTransformList *pDmeTransformList( NULL );

	matrix3x4_t mat;

	do
	{
		pDmeDag = dmeDagStack.Top();
		dmeDagStack.Pop();

		// Queue up the children of this thing for next iteration
		for ( int ci( pDmeDag->GetChildCount() - 1 ); ci >= 0; --ci )
		{
			dmeDagStack.Push( pDmeDag->GetChild( ci ) );
		}

		pDmeModelCheck = CastElement< CDmeModel >( pDmeDag );

		if ( pDmeModelCheck )
		{
			// This is the new DmeModel
			pDmeModel = pDmeModelCheck;
			pDmeTransformList = pDmeModel->FindBaseState( "bind" );
			if ( !pDmeTransformList )
			{
				pDmeModel->CaptureJointsToBaseState( "bind" );
				pDmeTransformList = pDmeModel->FindBaseState( "bind" );
			}
		}
		else if ( pDmeModel )
		{
			if ( pDmeTransformList )
			{
				const int numJoints( pDmeTransformList->GetTransformCount() );

				if ( pDmeModel->AddJoint( pDmeDag ) >= numJoints )
				{
					// Just added a new one..
					CDmeTransform *pSrc( pDmeDag->GetTransform() );
					CDmeTransform *pDst( CreateElement<CDmeTransform>( pSrc->GetName(), pDmeTransformList->GetFileId() ) );
					pSrc->GetTransform( mat );
					pDst->SetTransform( mat );
					pDmeTransformList->m_Transforms.AddToTail( pDst );
				}
			}
			else
			{
				Error( "%s: No DmeTransformList on DmeModel %s\n",
					CVsMayaMPxCommandDecorator< CVsDmxIOCmd >::Name().asChar(),
					pDmeModel->GetName() );
			}
		}
	}
	while ( dmeDagStack.Count() );
}


//-----------------------------------------------------------------------------
// Loads a Dme file
//-----------------------------------------------------------------------------
CDmElement *LoadDmeFile(
	const MString &filename )
{
	CDmElement *pRoot = NULL;
	g_pDataModel->RestoreFromFile( filename.asChar(), NULL, NULL, &pRoot );

	if ( !pRoot )
	{
		merr << "Can't load Dme file " << filename << std::endl;
		return NULL;
	}

	return pRoot;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmeCombinationOperator *FindComboOp( CDmElement *pRoot )
{
	if ( !pRoot )
		return NULL;

	// First look for the first CDmeCombinationOperator in pRoot
	for ( DmElementHandle_t hElement = g_pDataModel->FirstAllocatedElement();
		hElement != DMELEMENT_HANDLE_INVALID;
		hElement = g_pDataModel->NextAllocatedElement( hElement ) )
	{
		CDmeCombinationOperator *pDmeComboOp = GetElement< CDmeCombinationOperator >( hElement );
		if ( pDmeComboOp && pDmeComboOp->GetFileId() == pRoot->GetFileId())
			return pDmeComboOp;
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// Merges the CDmeCombinationOperator from one file into the passed scene
//-----------------------------------------------------------------------------
void
ComboMerge(
	const MString &comboFile,
	CDmElement *pRoot )
{
	CDmeCombinationOperator *pDst = FindComboOp( pRoot );
	CDmeCombinationOperator *pSrc = FindComboOp( LoadDmeFile( comboFile ) );

	if ( !pSrc || !pDst )
	{
		merr << "Can't merge DmeComboOp from " << comboFile << ": Couldn't find DmeCombinationOperators" << std::endl;
		return;
	}

	pDst->CopyControls( pSrc );
}

//-----------------------------------------------------------------------------
// Purpose: Export the specified bits of the maya scene into the specified file
// Input  : i_mArgDatabase	The command line arguments as passed
// Output : MS::kSuccess if ok, MS::kFailure otherwise
//-----------------------------------------------------------------------------
MStatus CVsDmxIOCmd::Export( const MArgDatabase &mArgDatabase, CDmElement **ppDmeDag )
{
	MString optFilename;

	if ( ppDmeDag )
	{
		optFilename = "<vsDmxIO>";
	}
	else
	{
		mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename );
	}

	MSelectionList optUserSelectionList;
	bool exportInvisible( mArgDatabase.isFlagSet( kOptExportInvisible ) || mArgDatabase.isFlagSet( kOptSelection ) );

	// List of Maya nodes to export (note that all children of these nodes are exported as well)
	MSelectionList toExportList;

	MDagPath mDagPath;

	if ( mArgDatabase.isFlagSet( kOptSelection ) )
	{
		// Get the user's specified selection of stuff to export
		if ( !mArgDatabase.getObjects( optUserSelectionList ) )
		{
			Error( "vsDmxIO: Cannot get list of objects to export\n" );
			return MS::kFailure;
		}
		else if ( optUserSelectionList.isEmpty() )
		{
			Error( "vsDmxIO: -export -selection specified but nothing is selected!\n" );
			return MS::kFailure;
		}
	}
	else
	{
		// Iterate over the Dag to find stuff to export!
		for ( MItDag dagIt( MItDag::kBreadthFirst, MFn::kDagNode ); !dagIt.isDone() && dagIt.depth() <= 1; dagIt.next() )
		{
			if ( dagIt.depth() == 1)
			{
				if ( dagIt.getPath( mDagPath ) )
				{
					if ( !MFnDagNode( mDagPath ).isIntermediateObject() && ( exportInvisible || vm::IsPathVisible( mDagPath, true ) ) )
					{
						optUserSelectionList.add( mDagPath, MObject::kNullObj, true );
					}
				}
			}
		}
	}

	if ( mArgDatabase.isFlagSet( kOptVstOnly ) )
	{
		// Only export stuff under a 'vstInfo' node

		for ( MItSelectionList sIt( optUserSelectionList ); !sIt.isDone(); sIt.next() )
		{
			if ( sIt.getDagPath( mDagPath ) )
			{
				MDagPath oldestAncestor;

				// Find the oldest ancestor of the specified node that is of type vstInfo
				while ( mDagPath.isValid() && mDagPath.length() )
				{
					if ( mDagPath.hasFn( MFn::kPluginTransformNode ) && MFnDagNode( mDagPath ).typeName() == "vstInfo" )
					{
						oldestAncestor = mDagPath;
					}

					mDagPath.pop();
				}

				if ( oldestAncestor.isValid() && ( exportInvisible || vm::IsPathVisible( oldestAncestor ) ) )
				{
					toExportList.add( oldestAncestor, MObject::kNullObj, true );
				}
			}
		}
	}
	else
	{
		toExportList = optUserSelectionList;
	}

	if ( toExportList.isEmpty() )
	{
		Error( "vsDmxIO: Cannot find anything to export\n" );
		return MS::kFailure;
	}

	if ( optFilename.length() == 0 )
	{
		// Look for the first vstInfo node and grab the filename from the .exportName attribute
		MDagPath infoPath;
		for ( MItSelectionList sIt( toExportList, MFn::kDagNode ); !sIt.isDone(); sIt.next() )
		{
			sIt.getDagPath( infoPath );
			const MFnDagNode dagFn( infoPath );
			if ( dagFn.typeName() == "vstInfo"  )
			{
				MPlug epP = dagFn.findPlug( "exportName" );
				if ( !epP.isNull() )
				{
					MString epS;
					epP.getValue( epS );
					if ( epS.length() )
					{
						optFilename = epS;
						break;
					}
				}
			}
		}
	}

	if ( optFilename.length() == 0 )
	{
		Error( "vsDmxIO: No filename specified for export and cannot find a vstInfoNode.exportPath attribute\n" );
		return MS::kFailure;
	}

	optFilename = optFilename.expandEnvironmentVariablesAndTilde();

	uint exportType( 0 );

	if ( mArgDatabase.isFlagSet( kOptExportType ) )
	{
		MString optExportType;
		MArgList tmpArgList;
		const uint etEnd( mArgDatabase.numberOfFlagUses( kOptExportType ) );
		for ( uint eti( 0 ); eti != etEnd; ++eti )
		{
			mArgDatabase.getFlagArgumentList( kOptExportType, eti, tmpArgList );
			optExportType = tmpArgList.asString( 0 );

			if ( Q_strncasecmp( optExportType.asChar(), "m", 1 ) == 0 )
			{
				exportType |= CDmeExport::kModel;
			}
			else if ( Q_strncasecmp( optExportType.asChar(), "a", 1 ) == 0 || Q_strncasecmp( optExportType.asChar(), "s", 1 ) == 0 )
			{
				exportType |= CDmeExport::kSkeletalAnimation;
			}
			else if ( Q_strncasecmp( optExportType.asChar(), "p", 1 ) == 0 )
			{
				exportType |= CDmeExport::kPhysicsModel;
			}
			else
			{
				Warning( "vsDmxIO: Cannot determine type of export from -et %s, assuming model\n",
					optExportType.asChar() );
			}
		}
	}

	if ( exportType == 0 )
	{
		exportType = CDmeExport::kModel;
	}

	// Tell Dme not to make things undoable while this variable exists
	CDisableUndoScopeGuard guard;

	// Data kept track of during the export process
	MString optDmObjectId;
	if ( mArgDatabase.isFlagSet( kOptMakefileObjectId ) )
	{
		mArgDatabase.getFlagArgument( kOptMakefileObjectId, 0, optDmObjectId );
	}

	CDmeExport exportData( exportType, optFilename, optDmObjectId );
	exportData.m_scratchVmt = mArgDatabase.isFlagSet( kOptScratchMaterial );
	exportData.m_exportInvisible = exportInvisible;
	exportData.m_optWeightMap = mArgDatabase.isFlagSet( kOptWeightMap );
	if ( mArgDatabase.isFlagSet( kOptMergeUvs ) )
	{
		mArgDatabase.getFlagArgument( kOptMergeUvs, 0, exportData.m_optMergeUvs );
	}

	if ( mArgDatabase.isFlagSet( kOptTriangulate ) )
	{
		exportData.m_optTriangulateBad = true;
	}

	exportData.m_optBindCurrent = mArgDatabase.isFlagSet( kOptBindCurrent );

	// Set the list of things selected
	if ( mArgDatabase.isFlagSet( kOptSelection ) )
	{
		MStringArray mStringArray;
		optUserSelectionList.getSelectionStrings( mStringArray );
		for ( uint si( 0 ); si != mStringArray.length(); ++si )
		{
			exportData.m_pMayaMakefileSource->m_RootDCCObjects.AddToTail( mStringArray[ si ].asChar() );
		}
	}

	// These are all of the top level nodes exported from Maya (no ancestors)
	CDmeModel *pDmeModelLast( NULL );
	CDmeModel *pDmeModelRoot( NULL );

	CUtlVector< CDmeMayaData * > dmeMayaDataList;

	// Export what's in toExportList
	// This should result in what's already been exported...
	for ( MItSelectionList sIt( toExportList, MFn::kDagNode ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( mDagPath ) )
			continue;

		CDmeDag *pDmeDag( NULL );
		exportData.ConvertDag( mDagPath, pDmeModelRoot, &pDmeDag, false );

		Assert( exportData.m_dmeModelStack.Count() == 0 );

		if ( pDmeDag )
		{
			dmeMayaDataList.AddToTail( new CDmeMayaData( pDmeDag, mDagPath ) );

			if ( pDmeModelRoot )
			{
				Assert( pDmeModelLast == NULL );
				if ( pDmeModelRoot->FindChild( pDmeDag ) < 0 )
				{
					// Only add it if it's not already a child...
					pDmeModelRoot->AddChild( pDmeDag );
				}
			}
			else
			{
				CDmeModel *pDmeModel( CastElement< CDmeModel >( pDmeDag ) );

				if ( pDmeModel && pDmeModelLast == NULL )
				{
					pDmeModelLast = pDmeModel;
				}
				else
				{
					pDmeModelRoot = CreateElement< CDmeModel >( "vsDmxIO Scene", exportData.FileId() );
					if ( pDmeModelLast != NULL )
					{
						// Only add it if it's not already a child...
						if ( pDmeModelRoot->FindChild( pDmeModelLast ) < 0 )
						{
							pDmeModelRoot->AddChild( pDmeModelLast );
						}
						pDmeModelLast = NULL;
					}
					if ( pDmeModelRoot->FindChild( pDmeDag ) < 0 )
					{
						// Only add it if it's not already a child...
						pDmeModelRoot->AddChild( pDmeDag );
					}
				}
			}
		}
	}

	if ( pDmeModelRoot == NULL && pDmeModelLast != NULL )
	{
		pDmeModelRoot = pDmeModelLast;
	}

	if ( pDmeModelRoot == NULL )
	{
		mwarn << "Found nothing to export" << std::endl;
		return MS::kFailure;
	}

	// Now go through the exportData and see if we need to do any binding!
	exportData.Bind( pDmeModelRoot );

	// Loop through all CDmeModel's in the scene and make sure every descendent
	// joint/transform that isn't already in the jointIndices array is added
	// to it's closest CDmeModel ancestor
	AddExtraJoints( pDmeModelRoot );

	if ( exportType & ( CDmeExport::kModel | CDmeExport::kPhysicsModel ) )
	{
		exportData.GetRoot()->SetValue( "model", pDmeModelRoot );
	}
	exportData.GetRoot()->SetValue( "skeleton", pDmeModelRoot );

	double optFs( MAnimControl::minTime().as( MTime::uiUnit() ) );
	double optFe( MAnimControl::maxTime().as( MTime::uiUnit() ) );
	double optFi( MAnimControl::playbackBy() );
	double optFo( 0.0 );

	// Get arguments for Skeletal animation and export skeletal animation
	if ( exportType & CDmeExport::kSkeletalAnimation )
	{
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

		if ( mArgDatabase.isFlagSet( kOptFrameOffset ) )
		{
			mArgDatabase.getFlagArgument( kOptFrameOffset, 0, optFo );
		}

		const bool optKfr(
			mArgDatabase.isFlagSet( kOptFrameOffset ) ||
			mArgDatabase.isFlagSet( kOptKeepFrameRange ) );

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

		MDagPath optExportRelativePath;
		if ( mArgDatabase.isFlagSet( kOptExportRelative ) )
		{
			MSelectionList optExportRelative;
			mArgDatabase.getFlagArgument( kOptExportRelative, 0, optExportRelative );
			if ( optExportRelative.length() )
			{
				optExportRelative.getDagPath( 0, optExportRelativePath );
			}
		}

		CDmeAnimationList *pDmeAnimationList( ExportSkeletonAnimation( mArgDatabase, dmeMayaDataList, exportData.GetRoot(), optFs, optFe, optFi, optFo, optKfr, optExportRelativePath ) );

		if ( pDmeAnimationList )
		{
			exportData.m_pMayaMakefileSource->m_FrameStart.Set( optFs );
			exportData.m_pMayaMakefileSource->m_FrameEnd.Set( optFe );
			exportData.m_pMayaMakefileSource->m_FrameIncrement.Set( optFi );
			// TODO: Add more options to DCC makefile thingy :(
			exportData.m_pMayaMakefileSource->SetValue( "cmdLine", CUtlString( m_cmdLine.asChar() ) );

			exportData.GetRoot()->SetValue( "animationList", pDmeAnimationList );
		}
	}

	if ( ppDmeDag )
	{
		// See if we're doing a filter operation
		// If so, Dme data created will be cleared on import
		// And we don't need to do extra junk like makefile's

		*ppDmeDag = pDmeModelRoot;
		exportData.Adopt();
	}
	else
	{
		// Tag the top level node with extra junk
		exportData.AddTags( m_cmdLine );

		exportData.m_pMayaMakefileSource->m_ExportType.Set( ( exportType & CDmeExport::kSkeletalAnimation ) ? 1 : 0 );

		if ( !( exportType & CDmeExport::kSkeletalAnimation ) )
		{
			// Set current time and increment of 0 for model exports since right now, things are not optional
			exportData.m_pMayaMakefileSource->m_FrameStart.Set( MAnimControl::currentTime().as( MTime::uiUnit() ) );
			exportData.m_pMayaMakefileSource->m_FrameEnd.Set( MAnimControl::currentTime().as( MTime::uiUnit() ) );
			exportData.m_pMayaMakefileSource->m_FrameIncrement.Set( 0.0 );
		}

		// Write the data to disk
#if defined( _DEBUG )
		const char *const defOutType( "keyvalues2" );
#else // defined( _DEBUG )
		const char *const defOutType( g_pDataModel->GetDefaultEncoding( DEFAULT_FILE_FORMAT ) );
#endif // defined( _DEBUG )

		MString optOutType( defOutType );

		if ( mArgDatabase.isFlagSet( kOptBinary ) )
		{
			optOutType = "binary";
		}
		else if ( mArgDatabase.isFlagSet( kOptAscii ) )
		{
			optOutType = "keyvalues2";
		}
		else if ( mArgDatabase.isFlagSet( kOptOutType ) )
		{
			mArgDatabase.getFlagArgument( kOptOutType, 0, optOutType );
			if ( !GetAvailableEncodings( CUtlSymbolTable() ).Find( optOutType.asChar() ).IsValid() )
			{
				mwarn << "vsDmxIO: Specified -outType \"" << optOutType << "\" is invalid, using \"" << defOutType << "\"" << std::endl;
				optOutType = defOutType;
			}
		}

		exportData.SetFileName( optFilename.asChar() );

		if ( mArgDatabase.isFlagSet( kOptComboMerge ) )
		{
			MString optComboMergeFile;
			mArgDatabase.getFlagArgument( kOptComboMerge, 0, optComboMergeFile );
			ComboMerge( optComboMergeFile, exportData.GetRoot() );
		}

		if ( !g_pDataModel->SaveToFile( optFilename.asChar(), NULL, optOutType.asChar(), DEFAULT_FILE_FORMAT, exportData.GetRoot() ) )
		{
			Error( "vsDmxIO: Encountered an error writing file \"%s\"!\n", optFilename.asChar() );
			return MS::kFailure;
		}

		// This is just to override the weird behaviour in FIleSystem which
		// makes all filenames lowercase on WIN32 which is wacky
		// because WIN32 filesystems are case insensitive
		rename( optFilename.asChar(), optFilename.asChar() );

		setResult( MString( "Exported to \"" ) + optFilename + "\"" );
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Purpose: Export the specified bits of the maya scene into the specified file
// Input  : mArgDatabase	The command line arguments as passed
// Output : MS::kSuccess if ok, MS::kFailure otherwise
//-----------------------------------------------------------------------------
MStatus CVsDmxIOCmd::Import(
	const MArgDatabase &mArgDatabase,
	CDmElement *pMemoryRoot )
{
	CDmElement *pRoot = pMemoryRoot;
	MString fileError;

	if ( !pRoot )
	{
		// Do an import from a filename

		MString optFilename;
		if ( !mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename ) )
		{
			Error( "vsDmxIO: No filename specified for import\n" );
			return MS::kFailure;
		}

		optFilename = optFilename.expandEnvironmentVariablesAndTilde();

		if ( !Q_strcasecmp( optFilename.asChar() + optFilename.length() - 4, ".obj" ) )
		{
			minfo << " Importing: " << optFilename << " as OBJ" << std::endl;
			pRoot = CDmObjSerializer().ReadOBJ( optFilename.asChar() );

			if ( !pRoot )
			{
				merr << "Couldn't read OBJ: " << optFilename << std::endl;
				return MS::kFailure;
			}
		}
		else
		{
			g_pDataModel->RestoreFromFile( optFilename.asChar(), NULL, NULL, &pRoot );

			if ( !pRoot )
			{
				Error( "vsDmxIO: Dme load of \"%s\" failed during import\n", optFilename.asChar() );
				return MS::kFailure;
			}

			fileError = MString( " from file \"" ) + optFilename + "\"";
		}
	}

	m_undo.SaveCurrentSelection();

	// Create a dag modifier for undo and the import data class
	CDmeImport importer( &m_undo );

	MDagPath mDagRoot = importer.DoIt( mArgDatabase, pRoot );

	// Clear all of the imported Dme data as it's usefulness has past
	g_pDataModel->RemoveFileId( pRoot->GetFileId() );

	// Just see what happened when the import was run
	if ( !mDagRoot.isValid() )
	{
		merr << "vsDmxIO: Nothing was converted when trying to import" << std::endl;

		return MS::kFailure;
	}

	MGlobal::select( mDagRoot, MObject::kNullObj, MGlobal::kReplaceList );

	setResult( mDagRoot.partialPathName() );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Fills the passed CUtlVector with the names of the available serializers
// Returns a const reference to the passed CUtlVector
//-----------------------------------------------------------------------------
CUtlSymbolTable &CVsDmxIOCmd::GetAvailableEncodings( CUtlSymbolTable &encodings )
{
	const int nEncodings( g_pDataModel->GetEncodingCount() );
	for ( int i( 0 ); i < nEncodings;  ++i )
	{
		encodings.AddString( g_pDataModel->GetEncodingName( i ) );
	}

	return encodings;
}