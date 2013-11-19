//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CVsDmxIOCommand
//
//=============================================================================

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Maya includes

#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MGlobal.h>
#include <maya/MEulerRotation.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSet.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnTransform.h>
#include <maya/MFnTransform.h>
#include <maya/MIntArray.h>
#include <maya/MItDag.h>
#include <maya/MItMeshFaceVertex.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MIteratorType.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MQuaternion.h>
#include <maya/MTransformationMatrix.h>

// Valve includes

#include "valveMaya.h"
#include "vsMayaCommand.h"

#include "datamodel/dmattribute.h"
#include "movieobjects/dmedag.h"
#include "movieobjects/dmefaceset.h"
#include "movieobjects/dmejoint.h"
#include "movieobjects/dmematerial.h"
#include "movieobjects/dmemesh.h"
#include "movieobjects/dmemodel.h"
#include "utlstack.h"

//-----------------------------------------------------------------------------
//
// Purpose: A new Maya command dervied from MPxCommand
//
//-----------------------------------------------------------------------------

class CVsDmxIOCmd : public CVsMayaCommand
{
public:
	CVsDmxIOCmd();

	virtual ~CVsDmxIOCmd();

	// Inherited from MPxCommand
	virtual MStatus doIt( const MArgList &mArgList );

	virtual MStatus undoIt();

	virtual MStatus redoIt();

	virtual bool isUndoable() const { return m_undoable; }

	virtual bool hasSyntax() const { return true; }

	// Inherited from CVsMayaCommand
	virtual void SpecifySyntax( MSyntax &syntax );

protected:
	MStatus Export( const MArgDatabase &mArgDatabase, CDmeDag **ppDmeDag = NULL );

	MStatus Import( const MArgDatabase &mArgDatabase, CDmeDag *pDmeDag = NULL );

protected:
	bool m_undoable;						// Is the command undoable or not
	MSelectionList m_undoSelectionList;		// The list of stuff that was selected before command
	MDagModifier *m_undoDagModifier;		// The Maya dag modifier for undo
	MArgList m_mArgList;					// The stored arg list (for undo/redo)

public:
	static const char *const s_name;		// Name of the command
	static vm::CMSyntaxHelp s_mSyntaxHelp;	// Keeps track of command line flags
};


//-----------------------------------------------------------------------------
// Hook the command so we can see it in Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_COMMAND( CVsDmxIOCmd, CVsDmxIOCmd::s_name );

//-----------------------------------------------------------------------------
//
// Statics
//
//-----------------------------------------------------------------------------

// Name of the command
const char *const CVsDmxIOCmd::s_name( "vsDmxIO" );

// Syntax Helper
vm::CMSyntaxHelp CVsDmxIOCmd::s_mSyntaxHelp;

//-----------------------------------------------------------------------------
//
// Purpose: Creator
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

CVsDmxIOCmd::CVsDmxIOCmd()
: m_undoable( false )
, m_undoDagModifier( NULL )
{
}


//-----------------------------------------------------------------------------
//
// Purpose: Destructor
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

CVsDmxIOCmd::~CVsDmxIOCmd()
{
	if ( m_undoDagModifier )
	{
		delete m_undoDagModifier;
	}
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
static const char *const kOptTime( "t" );
static const char *const kOptOutType( "ot" );
static const char *const kOptSelection( "sl" );
static const char *const kOptAllTypes( "at" );
static const char *const kOptExportInvisible( "ei" );

static const char *const kOptFilter( "fi" );

//-----------------------------------------------------------------------------
//
// Purpose: MSyntax factory for the command
// Input  :
// Output : A Maya MSyntax class describing command's command line syntax
//
//-----------------------------------------------------------------------------

void CVsDmxIOCmd::SpecifySyntax( MSyntax &mSyntax )
{
	s_mSyntaxHelp.Clear();

	{
		s_mSyntaxHelp.AddFlag( mSyntax, kOptHelp, "help", "General", "Print this information" );
		s_mSyntaxHelp.AddFlag( mSyntax, kOptFilename, "filename", "General", "On both import and export, use the specified filename", MSyntax::kString );
	}

	{
		s_mSyntaxHelp.AddFlag( mSyntax, kOptImport, "import", "Import", "Import a DMX file from the file specified by -filename" );
	}

	{
		s_mSyntaxHelp.AddFlag( mSyntax, kOptExport, "export", "Export", "Export a DMX file to the file specified by -filename" );
		s_mSyntaxHelp.AddFlag( mSyntax, kOptTime, "time", "Export",
			"Export over the selected time range specified as -time S[-E[xI]] where S is start time, E is end time, and I is time increment.  "
			"If none of these are specified then the current animation range settings are used" );

		MString outTypes( "<" );

		for ( int i( 0 ); i < g_pDataModel->GetSerializerCount();  ++i )
		{
			outTypes += " ";
			outTypes +=  g_pDataModel->GetSerializerName( i );
		}
		outTypes += " >";

		s_mSyntaxHelp.AddFlag( mSyntax, kOptOutType, "outType", "Export", MString ( MString( "Override the output file type where the specified type is one of " ) + outTypes ).asChar(), MSyntax::kString );

		s_mSyntaxHelp.AddFlag( mSyntax, kOptSelection, "selection", "Export", "Only export selected objects in the scene" );

		s_mSyntaxHelp.AddFlag( mSyntax, kOptAllTypes, "allTypes", "Export", "Export every node type specified, the normal behaviour is to only export data under a vstInfo (Valve Source Tools - Info node)" );

		s_mSyntaxHelp.AddFlag( mSyntax, kOptExportInvisible, "exportInvisible", "Export", "Normally invisible, intermediate & template nodes are ignored, specify this flag to cause them to be exported as well." );
	}

	{
		s_mSyntaxHelp.AddFlag( mSyntax, kOptFilter, "filter", "Filter", "Essentially do an export and then an import without a round trip to disk, all import & export options apply" );
	}

	mSyntax.useSelectionAsDefault( true );
	mSyntax.setObjectType( MSyntax::kSelectionList );
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

MStatus CVsDmxIOCmd::doIt( const MArgList &mArgList )
{
	// Simply save the argument list and then call redoIt()
	// If everything is done correctly the scene should be EXACTLY the
	// same each time doIt() or redoIt() is run so this is a reasonable
	// and general way to approach undo/redo, i.e. treat each redo like
	// it's being done for the very first time
	// There may be exceptions but this is a good structure to start with

	m_mArgList = mArgList;

	return redoIt();
}


//-----------------------------------------------------------------------------
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

MStatus CVsDmxIOCmd::undoIt()
{
	if ( m_undoable )
	{
		MGlobal::setActiveSelectionList( m_undoSelectionList );

		if ( m_undoDagModifier )
		{
			m_undoDagModifier->undoIt();
			delete m_undoDagModifier;
			m_undoDagModifier = NULL;
		}
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose:
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

MStatus CVsDmxIOCmd::redoIt()
{
	MStatus retVal;

	const MArgDatabase mArgDatabase( syntax(), m_mArgList, &retVal );

	if ( !retVal )
	{
		setResult( "Cannot parse command line" );
		return retVal;
	}

	if ( mArgDatabase.isFlagSet( kOptHelp ) )
	{
		s_mSyntaxHelp.PrintHelp( s_name,
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
		CDmeDag *pDmeDag( NULL );

		retVal = Export( mArgDatabase, &pDmeDag );

		if ( retVal && pDmeDag )
		{
			retVal = Import( mArgDatabase, pDmeDag );
		}
		else
		{
			retVal = MS::kFailure;
		}
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
// Purpose: Determine whether the passed dag node is a soft skinned mesh
//          via a Maya SkinCluster deformer
// Input  : i_mDagPath		The Maya Dag Path to check
// Output : true if it's a skinned mesh, false otherwise
//
//-----------------------------------------------------------------------------

bool IsSkinned(
	const MDagPath &i_mDagPath)
{
	MPlug imP( MFnDagNode( i_mDagPath ).findPlug( "inMesh" ) );
	if ( imP.isNull() )
		return false;

	MPlugArray mPlugArray;
	if ( !( imP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
		return false;

	const MObject skinClusterObj( mPlugArray[ 0 ].node() );
	if ( skinClusterObj.apiType() != MFn::kSkinClusterFilter )
		return false;

	return true;
}


//-----------------------------------------------------------------------------
//
// Purpose: Determine whether the passed dag node is a Maya SkinCluster
//          influence object
// Input  : i_mDagPath		The Maya Dag Path to check
// Output : true if it's an influence object, false otherwise
//
//-----------------------------------------------------------------------------

bool IsInfluenceObject(
	const MDagPath &i_mDagPath)
{
	MPlug wmAP( MFnDagNode( i_mDagPath ).findPlug( "worldMatrix" ) );
	if ( wmAP.isNull() )
		return false;

	MPlugArray mPlugArray;

	const unsigned wmEnd( wmAP.evaluateNumElements() );
	for ( unsigned wmi( 0 ); wmi != wmEnd; ++wmi )
	{
		if ( wmAP[ wmi ].connectedTo( mPlugArray, false, true ) && mPlugArray.length() > 0 )
		{
			if ( mPlugArray[ 0 ].node().apiType() == MFn::kSkinClusterFilter )
				return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
//
// Purpose: LessFunc for CUtlMap
//
//-----------------------------------------------------------------------------

bool UtlStringLessFunc( const CUtlString &i_a, const CUtlString &i_b )
{
	return ( Q_strcmp( i_a.Get(), i_b.Get() ) < 0 );
}


//-----------------------------------------------------------------------------
//
// Purpose: Data passed through the export process
//
//-----------------------------------------------------------------------------

struct CExportData
{
	CUtlStack<CDmeModel *> m_dmeModelStack;			// Currently active DME model (should only be 1)
	CUtlMap<CUtlString, CDmeDag *> m_mayaToDmeDag;	// Maps MDagPath::fullPathName to a DmeDag node
	MSelectionList m_bindList;						// A list of MDagPath's which have to be bound
	MSelectionList m_exportedList;					// A list of MDagPath's which have been exported
													// Maybe more efficient than the above map?
	bool m_exportInvisible;							// Should invisible nodes be exported?
	DmFileId_t m_dmFileId;							// Data Model FileId

	typedef CUtlMap<CUtlString, CDmeDag *>::IndexType_t Index_t;	// Index into the map

	CExportData()
	: m_mayaToDmeDag( UtlStringLessFunc )
	, m_exportInvisible( false )
	, m_dmFileId( g_pDataModel->FindOrCreateFileId( "<vsDmxIOFileId>" ) )
	{}

	CDmeDag *FindDmeDag(
		const MDagPath &i_mDagPath ) const
	{
		Index_t mayaToDmeIndex(
			m_mayaToDmeDag.Find( CUtlString( i_mDagPath.fullPathName().asChar() ) ) );

		if ( !m_mayaToDmeDag.IsValidIndex( mayaToDmeIndex ) )
			return NULL;

		return m_mayaToDmeDag[ mayaToDmeIndex ];
	}
};


//-----------------------------------------------------------------------------
//
// Purpose: Tag a given DmeDag node with extra attributes
//
//-----------------------------------------------------------------------------

void AddTags( CDmElement *i_pDmeElement,
			 const CExportData &i_exportData )
{
	IDmAttributeElement *pExportTags(
		i_pDmeElement->FindOrAddAttributeElement( "vsDmxIO_exportTags", CDmElement::GetStaticTypeSymbol() ) );

	CDmElement *pDmeElement( CreateElement<CDmElement>( "vsDmxIO_exportTags", i_exportData.m_dmFileId ) );

	pExportTags->SetValue( pDmeElement->GetHandle() );

	MString melResult;
	MStringArray melArrayResult;

	{
		IDmAttributeTyped<CUtlString> *pTag(
			pDmeElement->FindOrAddAttributeTyped<CUtlString>( "date" ) );
		MGlobal::executeCommand( "about -cd", melResult );
		pTag->SetValue( melResult.asChar() );
	}

	{
		IDmAttributeTyped<CUtlString> *pTag(
			pDmeElement->FindOrAddAttributeTyped<CUtlString>( "time" ) );
		MGlobal::executeCommand( "about -ct", melResult );
		pTag->SetValue( melResult.asChar() );
	}

	{
		char buf[256];
		DWORD dwSize( sizeof( buf ) );

		if ( GetUserName( buf, &dwSize) )
		{
			IDmAttributeTyped<CUtlString> *pTag(
				pDmeElement->FindOrAddAttributeTyped<CUtlString>( "user" ) );
			pTag->SetValue( buf );
		}

		if ( GetComputerName( buf, &dwSize) )
		{
			IDmAttributeTyped<CUtlString> *pTag(
				pDmeElement->FindOrAddAttributeTyped<CUtlString>( "machine" ) );
			pTag->SetValue( buf );
		}
	}

	{
		IDmAttributeTyped<CUtlString> *pTag(
			pDmeElement->FindOrAddAttributeTyped<CUtlString>( "os" ) );
		MGlobal::executeCommand( "strip( about( \"-osv\" ) )", melResult );

		pTag->SetValue( melResult.asChar() );
	}

	{
		MString cpuInfo;

		MGlobal::executeCommand( "hardware -npr", melArrayResult );
		cpuInfo += melArrayResult[ 0 ];
		cpuInfo += " x ";

		MGlobal::executeCommand( "hardware -cpu", melArrayResult );
		cpuInfo += melArrayResult[ 0 ];

		MGlobal::executeCommand( "hardware -mhz", melArrayResult );
		cpuInfo += " @ ";
		cpuInfo += melArrayResult[ 0 ];
		cpuInfo += " MHz";

		IDmAttributeTyped<CUtlString> *pTag(
			pDmeElement->FindOrAddAttributeTyped<CUtlString>( "cpu" ) );
		pTag->SetValue( cpuInfo.asChar() );
	}

	{
		MGlobal::executeCommand( "hardware -gfx", melArrayResult );

		IDmAttributeTyped<CUtlString> *pTag(
			pDmeElement->FindOrAddAttributeTyped<CUtlString>( "gfx" ) );
		pTag->SetValue( melArrayResult[ 0 ].asChar() );
	}

	{
		MGlobal::executeCommand( "about -a", melResult );

		IDmAttributeTyped<CUtlString> *pTag(
			pDmeElement->FindOrAddAttributeTyped<CUtlString>( "app" ) );
		pTag->SetValue( melResult.asChar() );
	}

	{
		IDmAttributeTyped<CUtlString> *pMayaVersionAttr(
			pDmeElement->FindOrAddAttributeTyped<CUtlString>( "appVersion" ) );

		pMayaVersionAttr->SetValue( MGlobal::mayaVersion().asChar() );
	}
}


//-----------------------------------------------------------------------------
//
// Purpose: Cast attributes ala CastElement, not sure why it doesn't exist
//          maybe I missed the proper way to do it
//
//-----------------------------------------------------------------------------

template< class T >
IDmAttributeTyped< T > *CastAttribute( IDmAttribute *pAttribute )
{
	if ( pAttribute && pAttribute->GetType() == CDmAttributeInfo<T>::AttributeType() )
		return static_cast< IDmAttributeTyped<T> * >( pAttribute );
	return NULL;
}


//-----------------------------------------------------------------------------
//
// Purpose: Prints any of the vsDmxIO_exportTags info in a scene
//
//-----------------------------------------------------------------------------

void PrintTags( CDmeDag *i_pDmeDag )
{
	if ( i_pDmeDag )
	{
		CUtlStack<CDmeDag *> dmeDagStack;
		dmeDagStack.Push( i_pDmeDag );

		do
		{
			CDmeDag *pDmeDag( dmeDagStack.Top() );
			dmeDagStack.Pop();

			CDmElement *pExportTags( pDmeDag->GetAttributeValueElement<CDmElement>( "vsDmxIO_exportTags" ) );
			if ( pExportTags )
			{
				for ( IDmAttribute *pTag( pExportTags->FirstAttribute() ); pTag; pTag = pTag->NextAttribute() )
				{
					const char *const pTagName( pTag->GetName() );

					if ( Q_strcmp( "name", pTagName ) && Q_strcmp( "type", pTagName ) && Q_strcmp( "id", pTagName ) )
					{
						IDmAttributeTyped<CUtlString> *pTagTyped( CastAttribute<CUtlString>( pTag ) );
						if ( pTagTyped )
						{
							minfo << pTagTyped->GetName() << ": " << pTagTyped->GetValue() << std::endl;
						}
					}
				}
			}

			for ( int ci( pDmeDag->GetChildCount() - 1 ); ci >= 0; --ci )
			{
				dmeDagStack.Push( pDmeDag->GetChild( ci ) );
			}
		}
		while ( dmeDagStack.Count() );
	}
}


//-----------------------------------------------------------------------------
//
// Purpose: Data passed through the export process
//
//-----------------------------------------------------------------------------

void Bind(
	const CExportData &i_exportData)
{
	MDagPath mDagPath;

	for ( MItSelectionList sIt( i_exportData.m_bindList ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( mDagPath ) )
			continue;

		CDmeDag *pDmeDag( CastElement<CDmeDag>( i_exportData.FindDmeDag( mDagPath ) ) );
		if ( !pDmeDag )
		{
			Error( "%s: Trying to bind %s but can't find corresponding DmeDag Node\n", CVsDmxIOCmd::s_name, mDagPath.fullPathName().asChar() );
			continue;
		}

		// Look for the model bind reference
		CDmeModel *pDmeModel( pDmeDag->GetAttributeValueElement< CDmeModel >( "__dmeModel_bind" ) );
		if ( !pDmeModel )
		{
			Error( "%s: Trying to bind %s but can't find corresponding DmeModel Node\n", CVsDmxIOCmd::s_name, mDagPath.fullPathName().asChar() );
			continue;
		}

		pDmeDag->RemoveAttribute( "__dmeModel_bind" );

		// The mesh attached to the shape
		CDmeMesh *pDmeMesh( CastElement<CDmeMesh>( pDmeDag->GetShape() ) );
		if ( !pDmeMesh )
		{
			Error( "%s: Trying to bind %s but no DmeMesh set as the Shape of DmeDag %s\n",
				CVsDmxIOCmd::s_name,
				mDagPath.fullPathName().asChar(),
				pDmeDag->GetName() );
			continue;
		}

		// The path to the MFnSkinCluster has already worked so only check at the end for problems

		MPlugArray mPlugArray;
		MFnDagNode( mDagPath ).findPlug( "inMesh" ).connectedTo( mPlugArray, true, false );

		const MObject skinClusterObj( mPlugArray[ 0 ].node() );
		if ( skinClusterObj.apiType() != MFn::kSkinClusterFilter )
		{
			Warning( "%s: Can't find Maya skin cluster driving mesh %s\n",
				CVsDmxIOCmd::s_name,
				mDagPath.fullPathName().asChar() );
			continue;
		}

		// It seems to be connected to a Maya SkinCluster node so get the influences
		MFnSkinCluster skinFn( skinClusterObj );
		MDagPathArray mDagPathArray;
		MStatus mStatus;

		// Make sure that there is at least one thing influencing the mesh
		if ( ! ( skinFn.influenceObjects( mDagPathArray, &mStatus ) && mStatus && mDagPathArray.length() ) )
		{
			Warning( "%s: No influence objects attached to skin cluster %s driving mesh %s\n",
				CVsDmxIOCmd::s_name,
				skinFn.name().asChar(),
				mDagPath.fullPathName().asChar() );
			continue;
		}

		// Since some influence objects might need to be removed, keep track of the valid influence indices
		CUtlVector< unsigned > validInfluenceIndices;
		validInfluenceIndices.EnsureCapacity( mDagPathArray.length() );

		// It shouldn't happen, but just in case the same joint is attempted to be added to the same DmeModel
		// twice, CDmeMode::AddJoint() will return the old index instead of just blindly adding it to the
		// end.  So, build 
		CUtlVector<int> dmeJointIndices;
		dmeJointIndices.EnsureCapacity( mDagPathArray.length() );

		// Remove any shapes from the influence object list
		for ( unsigned i( 0 ); i < mDagPathArray.length(); ++i )
		{
			const MDagPath &iDagPath( mDagPathArray[ i ] );
			if ( iDagPath.hasFn( MFn::kShape ) )
			{
				Warning( "%s: Ignoring geometric influence object %s driving skin cluster %s while exporting skinning information for %s\n",
					CVsDmxIOCmd::s_name,
					iDagPath.partialPathName().asChar(),
					skinFn.name().asChar(),
					mDagPath.partialPathName().asChar() );
				continue;
			}

			// Find the joint in the DmeScene
			CDmeDag *pDmeDag( i_exportData.FindDmeDag( iDagPath ) );

			if ( !pDmeDag )
			{
				// TODO: Convert that dag node as is...
				Warning( "%s: Trying to bind %s to %s but can't find corresponding DmeDag Node\n",
					CVsDmxIOCmd::s_name,
					iDagPath.fullPathName().asChar(),
					mDagPath.fullPathName().asChar() );
				continue;
			}

			dmeJointIndices.AddToTail( pDmeModel->AddJoint( pDmeDag ) );

			validInfluenceIndices.AddToTail( i );
		}

		// Valid influence count (sans Influence shapes, only joints & transforms)
		const int validInfluenceCount( validInfluenceIndices.Count() );

		// Make sure there is at least one valid thing influencing the mesh
		if ( validInfluenceCount <= 0 )
		{
			Warning( "%s: No valid influence objects found while getting bind info from %s for %s\n",
				CVsDmxIOCmd::s_name,
				skinFn.name().asChar(),
				mDagPath.partialPathName().asChar() );
			continue;
		}

		// Maximum of 3 weights per vertex
		// Usually Maya will have way more than 3 per vertex but it's possible that there are only 0, 1 or 2 influences
		static const int kMaxWeightsPerVertex( 3 );
		const int weightsPerVertex( validInfluenceCount > kMaxWeightsPerVertex ? kMaxWeightsPerVertex : validInfluenceCount );

		// The list of weights and indicies to attach to the DmeMesh
		// The indices correspond to values from 'dmeJointIndices'
		// Also, the things returned by Maya might have been excluded so
		// maya indices have to be filtered through 'validInfluenceIndices'
		CUtlVector< float > dmeWeights;
		dmeWeights.EnsureCapacity( mDagPathArray.length() * weightsPerVertex );

		CUtlVector< int > dmeIndices;
		dmeIndices.EnsureCapacity( mDagPathArray.length() * weightsPerVertex );

		for ( MItMeshVertex vIt( mDagPath ); !vIt.isDone(); vIt.next() )
		{
			// If these never get set, they default to a weight of 0 on vertex 0
			int wIndex[kMaxWeightsPerVertex] = { 0, 0, 0 };
			float wValue[kMaxWeightsPerVertex] = { 0.0f, 0.0f, 0.0f };

			unsigned mInfluenceCount( 0 );	// Maya influence count
			MDoubleArray mWeights;
			skinFn.getWeights( mDagPath, vIt.vertex(), mWeights, mInfluenceCount );
			Assert( static_cast<int>( mInfluenceCount ) >= validInfluenceCount );

			// Loop through each valid influence index
			for ( int vi( 0 ); vi != validInfluenceCount; ++vi )
			{
				const unsigned mInfluenceIndex( validInfluenceIndices[ vi ] );
				const float w( mWeights[ mInfluenceIndex ] );

				for ( int wi( weightsPerVertex - 1); wi >= 0; --wi )
				{
					if ( w >= wValue[ wi ] )
					{
						// Cascade down current values
						for ( int cwi( 0 ); cwi < wi; ++cwi )
						{
							wValue[ cwi ] = wValue[ cwi + 1 ];
							wIndex[ cwi ] = wIndex[ cwi + 1 ];
						}

						// Save higher value
						wValue[ wi ] = w;
						wIndex[ wi ] = dmeJointIndices[ vi ];

						// Skip to next weight
						break;
					}
				}
			}

			{
				// Total Weight
				float wTotal( 0.0f );

				// Normalize the weights
				for ( int wi( 0 ); wi < weightsPerVertex; ++wi )
				{
					wTotal += wValue[ wi ];
				}

				// Now add the data to the dme Arrays avoiding division by zero
				if ( wTotal != 0.0f )
				{
					for ( int wi( 0 ); wi < weightsPerVertex; ++wi )
					{
						dmeWeights.AddToTail( wValue[ wi ] / wTotal );
						dmeIndices.AddToTail( wIndex[ wi ] );
					}
				}
				else
				{
					// Warn about 0 weighted vertices
					MSelectionList mSelectionList;
					mSelectionList.add( mDagPath, vIt.vertex() );
					MStringArray mStringArray;
					mSelectionList.getSelectionStrings( 0, mStringArray );

					Warning( "%s: Unweighted vertex %s on skinned mesh\n",
						CVsDmxIOCmd::s_name,
						mStringArray[ 0 ].asChar() );

					for ( int wi( 0 ); wi < weightsPerVertex; ++wi )
					{
						dmeWeights.AddToTail( wValue[ wi ] );
						dmeIndices.AddToTail( wIndex[ wi ] );
					}
				}
			}
		}

		FieldIndex_t jointWeight;
		FieldIndex_t jointIndex;

		pDmeMesh->CreateJointWeightsAndIndices( weightsPerVertex, &jointWeight, &jointIndex );
		Assert( dmeWeights.Count() == weightsPerVertex * pDmeMesh->GetPositionData().Count() );
		Assert( dmeIndices.Count() == weightsPerVertex * pDmeMesh->GetPositionData().Count() );

		pDmeMesh->AddVertexData( jointIndex, dmeIndices.Count() );
		pDmeMesh->SetVertexData( jointIndex, 0, dmeIndices.Count(), AT_INT, dmeIndices.Base() );

		pDmeMesh->AddVertexData( jointWeight, dmeWeights.Count() );
		pDmeMesh->SetVertexData( jointWeight, 0, dmeWeights.Count(), AT_FLOAT, dmeWeights.Base() );

		CDmeTransformList *pDmeTransformList( pDmeModel->FindBaseState( "bind" ) );

		// If there are no joints in the dmeModel, just go on...
		const int jEnd( pDmeModel->GetJointTransformCount() );
		if ( jEnd == 0 )
		{
			continue;
		}

		// Find the bind list on the dmeModel, create it if necessary
		if ( !pDmeTransformList )
		{
			// Only interface to that... does redundant work
			pDmeModel->CaptureJointsToBaseState( "bind" );
			pDmeTransformList = pDmeModel->FindBaseState( "bind" );

			if ( !pDmeTransformList )
			{
				Error( "%s: Cannot find or create \"bind\" base state on DmeModel %s\n",
					CVsDmxIOCmd::s_name,
					pDmeModel->GetName() );
				continue;
			}

			CDmAttributeVarElementArray<CDmeTransform> &bindTransforms( pDmeTransformList->m_Transforms );

			Assert( bindTransforms.Count() == jEnd );

			for ( int ji( 0 ); ji < jEnd; ++ji )
			{
				CDmeTransform *pSrcTransform( pDmeModel->GetJointTransform( ji ) );
				CDmeTransform *pSrc( pSrcTransform->GetAttributeValueElement<CDmeTransform>( "__bind" ) );

				if ( pSrc )
				{
					bindTransforms.Set( ji, pSrc );
					pSrcTransform->RemoveAttribute( "__bind" );
				}
				else
				{
					if ( ji != 0 )
					{
						Warning( "%s: Cannot find __bind transform on %s\n",
							CVsDmxIOCmd::s_name,
							pSrcTransform->GetName() );
					}

					CDmeTransform *pDst( CreateElement<CDmeTransform>( pSrcTransform->GetName(), i_exportData.m_dmFileId ) );

					bindTransforms.Set( ji, pDst );
				}
			}
		}

		CDmAttributeVarElementArray<CDmeTransform> &bindTransforms( pDmeTransformList->m_Transforms );
		Assert( bindTransforms.Count() <= jEnd );

		for ( int ji( bindTransforms.Count() ); ji < jEnd; ++ji )
		{
			CDmeTransform *pSrcTransform( pDmeModel->GetJointTransform( ji ) );
			CDmeTransform *pSrc( pSrcTransform->GetAttributeValueElement<CDmeTransform>( "__bind" ) );

			if ( pSrc )
			{
				bindTransforms.AddToTail( pSrc );
				pSrcTransform->RemoveAttribute( "__bind" );
			}
			else
			{
				if ( ji != 0 )
				{
					Warning( "%s: Cannot find __bind transform on %s\n",
						CVsDmxIOCmd::s_name,
						pSrcTransform->GetName() );
				}

				CDmeTransform *pDst( CreateElement<CDmeTransform>( pSrcTransform->GetName(), i_exportData.m_dmFileId ) );

				pDst->SetOrientation( pSrc->GetOrientation() );
				pDst->SetPosition( pSrc->GetPosition() );

				bindTransforms.AddToTail( pDst );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
// Purpose: Return the minimum of two numbers while catching type problems
// Input  : a	A value
//			b	Another value
// Output : The smaller of the two values
//
//-----------------------------------------------------------------------------

template <typename T_t>
inline const T_t &v_min(
	const T_t &i_a,
	const T_t &i_b )
{
  return i_a < i_b ? i_a : i_b;
}


//-----------------------------------------------------------------------------
//
// Purpose: Return the maximum of two numbers while catching type problems
// Input  : a	A value
//			b	Another value
// Output : The larger of the two values
//
//-----------------------------------------------------------------------------

template <typename T_t>
inline const T_t &v_max(
	const T_t &i_a,
	const T_t &i_b )
{
	return i_a > i_b ? i_a : i_b;
}


//-----------------------------------------------------------------------------
//
// Purpose: Retrun a value clamped to a min & max range
// Input  : i_v		Value to be clamped
//			i_min	A value
//			i_max	Another value
// Output : The larger of the two values
//
//-----------------------------------------------------------------------------

template <typename T_t>
inline const T_t &v_clamp(
	const T_t &i_v,
	const T_t &i_min,
	const T_t &i_max )
{
	if ( i_v < i_min )
		return i_min;

	if ( i_v > i_max )
		return i_max;

	return i_v;
}


//-----------------------------------------------------------------------------
//
// Purpose: Convert a Maya Mesh node into a DmeMesh node
// Input  : i_mDagPath		The Maya Dag Path to export
//			i_pDmeDagParent	The CDmeDag that will be the parent of the newly created
//							node, cannot be NULL
// Output : The CDmeMesh for the node that was converted, can be NULL
//
//-----------------------------------------------------------------------------

void MayaMeshToDmeMesh(
	const MDagPath &i_mDagPath,
	CDmeDag *i_pDmeDagParent,
	CExportData &io_exportData)
{
	// Forward declaration;
	CDmeDag *MayaDagToDmeDag(
		MDagPath &i_mDagPath,
		CDmeDag *i_pDmeDagParent,
		CExportData &io_exportData);

	// If we have no parent, make one up!
	// This should call this routine again with this MDagPath
	// and with it's parent converted into a DmeModel
	if ( !i_pDmeDagParent )
	{
		Warning( "%s: No parent found for %s - Making one up!\n", CVsDmxIOCmd::s_name, i_mDagPath.partialPathName().asChar() );
		MDagPath pDagPath( i_mDagPath );
		while ( pDagPath.isValid() && pDagPath.length() && !pDagPath.hasFn( MFn::kTransform ) )
		{
			pDagPath.pop();
		}

		if ( pDagPath.isValid() && pDagPath.length() && pDagPath.hasFn( MFn::kTransform ) )
		{
			MayaDagToDmeDag( pDagPath, NULL, io_exportData );
			return;
		}
	}

	// Now set the shape for the passed in DmeDag
	if ( i_pDmeDagParent->GetShape() )
	{
		Warning( "%s: Dme parent %s already has a shape node %s while trying to set %s\n",
			CVsDmxIOCmd::s_name,
			i_pDmeDagParent->GetName(),
			i_pDmeDagParent->GetShape()->GetName(),
			i_mDagPath.fullPathName().asChar() );
		return;
	}

	// Don't do things twice
	if ( io_exportData.m_exportedList.hasItem( i_mDagPath ) )
	{
		Warning( "%s: Already processed %s\n",
			CVsDmxIOCmd::s_name,
			i_mDagPath.fullPathName().asChar() );
		return;
	}

	// Make sure we really have a mesh
	MStatus mStatus;
	MFnMesh meshFn( i_mDagPath, &mStatus );
	if ( !mStatus )
		return;

	// Create the CDmeMesh element
	CDmeMesh *pDmeMesh( CreateElement< CDmeMesh >( meshFn.name().asChar(), io_exportData.m_dmFileId ) );

	// Just some checks
	const int numVertices( meshFn.numVertices() );
	const int numNormals( meshFn.numNormals() );
	int numColors( meshFn.numColors() );
	int numUVs( meshFn.numUVs() );
	const int numFaceVertices( meshFn.numFaceVertices() );

	// A vector for position indices
	CUtlVector< int > positionIndices;
	positionIndices.EnsureCount( numFaceVertices );
	Assert( positionIndices.Count() == numFaceVertices );

	// A vector for normal indices
	CUtlVector< int > normalIndices;
	normalIndices.EnsureCount( numFaceVertices );
	Assert( normalIndices.Count() == numFaceVertices );

	// A vector for UV indices
	CUtlVector< int > uvIndices;
	uvIndices.EnsureCount( numUVs ? numFaceVertices : 0 );
	Assert( uvIndices.Count() == numFaceVertices );

	// A vector for Color indices
	CUtlVector< int > colorIndices;
	colorIndices.EnsureCount( numColors ? numFaceVertices : 0 );
	Assert( uvIndices.Count() == numFaceVertices );

	MObjectArray shadingGroups;
	MObjectArray faceComponents;

	if ( !meshFn.getConnectedSetsAndMembers(
		i_mDagPath.instanceNumber(), shadingGroups, faceComponents, false) )
	{
		faceComponents.append( MObject::kNullObj );
	}

	int globalFaceVertexIndex( 0 );

	for ( unsigned fci( 0 ); fci != faceComponents.length(); ++fci )
	{
		MString faceSetName;
		MString materialName;
		MString materialPath;

		if ( fci < shadingGroups.length() )
		{
			const MObject shadingGroupObj( shadingGroups[ fci ] );

			if ( shadingGroupObj.apiType() != MFn::kShadingEngine )
				continue;

			// Set defaults for now

			const MFnSet setFn( shadingGroupObj );
			faceSetName = setFn.name();

			materialName = faceSetName + "Material";
			materialPath = "debug/debugempty";

			// Get the surface shader

			MPlugArray mPlugArray;

			const MFnDependencyNode shadingGroupFn( shadingGroupObj );
			const MPlug surfaceShaderP( shadingGroupFn.findPlug( "surfaceShader" ) );
			if ( surfaceShaderP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() )
			{
				const MFnDependencyNode surfaceShaderFn( mPlugArray[ 0 ].node() );
				const MPlug colorP( surfaceShaderFn.findPlug( "color" ) );

				if ( colorP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() )
				{
					const MFnDependencyNode fileFn( mPlugArray[ 0 ].node() );
					const MPlug fileP( fileFn.findPlug( "fileTextureName" ) );
					MString file;
					fileP.getValue( file );

					const MString vproject( getenv( "VPROJECT" ) );

					const int fLen( file.length() );
					const int vLen( vproject.length() );

					// Just put some arbitrary limits on size...
					if ( fLen && fLen < 4096 && vLen && vLen < 4096 )
					{
						// Make sure the same path separator is being used everywhere
						// Maybe this is dangerous... dunno, don't think so

						char *const pf( static_cast< char * >( _alloca( fLen + 1 ) ) );
						Q_memcpy( pf, file.asChar(), fLen + 1 );
						char *const pfEnd( pf + fLen );

						for ( char *p( pf ); p != pfEnd;  ++p )
						{
							if ( *p == '\\' )
							{
								*p = '/';
							}
						}

						char *const pv( static_cast< char * >( _alloca( vLen + 1 ) ) );
						Q_memcpy( pv, vproject.asChar(), vLen + 1 );
						char *pvEnd( pv + vLen );

						for ( char *p( pv ); p != pvEnd;  ++p )
						{
							if ( *p == '\\' )
							{
								*p = '/';
							}
						}

						// Ok, now in vproject, make sure there are no /'s at the end
						for ( char *p( pvEnd - 1); p >= pv && *p == '/'; --p )
						{
							*p = '\0';
						}

						// Now split them into Maya string arrays and find the common stuff

						MStringArray fa;
						MString( pf ).split( '/', fa );
						MStringArray va;
						MString( pv ).split( '/', va );

						// See if things are how we expect them...  va should be something like:
						// ... / <somedir> / game / <somegame>
						// Where <somedir> / 
						// e.g.
						// pf: U:/dev/main/content/tf2/materialsrc/models/player/hvyweapon/mouth.tga
						// pv: D:/DEV/ValveGames/main/game/tf2
						//
						// Means vmt file is:
						//
						// D:/DEV/ValveGames/main/game/tf2/materials/models/player/hvyweapon/mouth.vmt

						const int faLen( fa.length() );
						const int vaLen( va.length() );

						if ( vaLen >= 3 && va[ vaLen - 2 ] == "game" )
						{
							const int vSomeDir( vaLen - 3 );
							int fSomeDir( 0 );
							for ( const int fSomeDirEnd( faLen - 1 ); fSomeDir < fSomeDirEnd; ++fSomeDir )
							{
								if ( fa[ fSomeDir ]  == va[ vSomeDir ] &&
										fa[ fSomeDir + 1 ] == "content" &&
										va[ vSomeDir + 1 ] == "game" &&
										fa[ fSomeDir + 3 ] == "materialsrc" )
								{
									const int fSomeGame( fSomeDir + 2 );
									const int vSomeGame( vSomeDir + 2 );

									MStringArray fsa;
									fa[ faLen - 1 ].split( '.', fsa );

									MString vmtPath( va[ 0 ] );

									for ( int vi( 1 ); vi <= vSomeGame; ++vi )
									{
										vmtPath += "/";
										vmtPath += va[ vi ];
									}

									vmtPath += "/materials";

									for ( int fi( fSomeGame + 2 ); fi < ( faLen - 1 ); ++fi )
									{
										vmtPath += "/";
										vmtPath += fa[ fi ];
									}

									vmtPath += "/";
									vmtPath += MString( fsa[ 0 ] + ".vmt" );

									FILE *fp = fopen( vmtPath.asChar(), "r" );
									if ( fp )
									{
										MStringArray vmta;

										char buf[ 1024 ];
										char tex[ 1024 ];

										while ( !feof( fp ) )
										{
											if ( fgets( buf, sizeof( buf ), fp ) )
											{
												// Remove any leading /'s or \'s from the material path
												// I think that's a good thing to do anyway...
												if ( sscanf( buf, " \"$basetexture\" %*[\"/\\]%[^\"]\" ", tex ) == 1 )
												{
													// Flip the backslashes forward
													for ( char *cp( tex ); *cp != '\0'; ++cp )
													{
														if ( *cp == '\\' )
														{
															*cp = '/';
														}
													}
													materialPath = tex;

													for ( char *cp( tex ); *cp != '\0'; ++cp )
													{
														if ( *cp == '/' )
														{
															*cp = '_';
														}
													}
													materialName = tex;

													break;
												}
											}
										}
										fclose( fp );
									}
									break;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			materialName = faceSetName + "Material";
			materialPath = "debug/debugempty";
		}

		// Add the face set stuff
		CDmeFaceSet *pFaceSet = CreateElement< CDmeFaceSet >(
			faceSetName.asChar(), io_exportData.m_dmFileId );
		if ( !pFaceSet )
			continue;

		// A vector for Face Set indices
		CUtlVector< int > faceSets;

		MObject fComp( faceComponents[ fci ] );
		for ( MItMeshPolygon pIt( i_mDagPath, fComp ); !pIt.isDone(); pIt.next() )
		{
			// MItMeshFaceVertex should be used but it can fail (crash) when asking for
			// the normalId when there aren't faceVarying normals assigned

			// lfv = localFaceVertex( Index | End )
			const int lfvEnd( pIt.polygonVertexCount() );

			if ( pIt.hasUVs() && pIt.hasColor() )
			{
				// Vertices, normals, UVs & Colors!
				for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi, ++globalFaceVertexIndex )
				{
					positionIndices[ globalFaceVertexIndex ] = pIt.vertexIndex( lfvi );
					normalIndices[ globalFaceVertexIndex ] = pIt.normalIndex( lfvi );
					pIt.getUVIndex( lfvi, uvIndices[ globalFaceVertexIndex ] );
					pIt.getColorIndex( lfvi, colorIndices[ globalFaceVertexIndex ] );
					faceSets.AddToTail( globalFaceVertexIndex );
				}
			}
			else if ( pIt.hasUVs() )
			{
				// Just vertices, normals & Uvs
				if ( numColors )
				{
					Warning(
						"%s: Face %d on %s ( %s.f[%d] ) has no UVs assigned but other faces in the mesh do."
						"  This is not supported in Dme, not exporting UVs (texture coordinates)\n",
						CVsDmxIOCmd::s_name,
						pIt.index(),
						i_mDagPath.partialPathName().asChar(),
						i_mDagPath.partialPathName().asChar(),
						pIt.index() );

					numColors = 0;
				}

				for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi, ++globalFaceVertexIndex )
				{
					positionIndices[ globalFaceVertexIndex ] = pIt.vertexIndex( lfvi );
					normalIndices[ globalFaceVertexIndex ] = pIt.normalIndex( lfvi );
					pIt.getUVIndex( lfvi, uvIndices[ globalFaceVertexIndex ] );
					faceSets.AddToTail( globalFaceVertexIndex );
				}
			}
			else if ( pIt.hasColor() )
			{
				// Just vertices, normals & colors
				if ( numUVs )
				{
					Warning(
						"%s: Face %d on %s ( %s.f[%d] ) has no UVs assigned but other faces in the mesh do."
						"  This is not supported in Dme, not exporting color\n",
						CVsDmxIOCmd::s_name,
						pIt.index(),
						i_mDagPath.partialPathName().asChar(),
						i_mDagPath.partialPathName().asChar(),
						pIt.index() );

					numUVs = false;
				}

				for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi, ++globalFaceVertexIndex )
				{
					positionIndices[ globalFaceVertexIndex ] = pIt.vertexIndex( lfvi );
					normalIndices[ globalFaceVertexIndex ] = pIt.normalIndex( lfvi );
					pIt.getColorIndex( lfvi, colorIndices[ globalFaceVertexIndex ] );
					faceSets.AddToTail( globalFaceVertexIndex );
				}
			}
			else
			{
				// Just vertices & normals
				for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi, ++globalFaceVertexIndex )
				{
					positionIndices[ globalFaceVertexIndex ] = pIt.vertexIndex( lfvi );
					normalIndices[ globalFaceVertexIndex ] = pIt.normalIndex( lfvi );
					faceSets.AddToTail( globalFaceVertexIndex );
				}
			}

			faceSets.AddToTail( -1 );
		}

		if ( faceSets.Count() )
		{
			// Add the face set data
			pFaceSet->AddIndices( faceSets.Count() );
			pFaceSet->SetIndices( 0, faceSets.Count(), faceSets.Base() );

			CDmeMaterial *pMaterial = CreateElement< CDmeMaterial >(
				materialName.asChar(),
				io_exportData.m_dmFileId );

			pMaterial->SetMaterial( materialPath.asChar() );

			pFaceSet->SetMaterial( pMaterial );

			// Add the face set to the mesh
			pDmeMesh->AddFaceSet( pFaceSet );
		}
		else
		{
			Warning(" * NOTHING FOR %s\n", materialName.asChar() );
		}
	}

	// Everything together should add up to this...
	Assert( globalFaceVertexIndex == numFaceVertices );

	// Ask CDmeMesh where to store the mesh vertices
	const FieldIndex_t positionIndex( pDmeMesh->CreateField<Vector>( "positions" ) );

	{
		// Do vertices

		// Get the data from Maya
		MFloatPointArray mPoints;
		meshFn.getPoints( mPoints );
		Assert( static_cast<unsigned>( numVertices ) == mPoints.length() );

		// We have to put it into a temporary format for now - maybe can stuff it right into DME?
		CUtlVector< Vector > dPoints;
		dPoints.EnsureCount( numVertices );

		// Copy from Maya homogeneous to Dme cartesian
		for ( int pi( 0 ); pi < numVertices; ++pi )
		{
			const MFloatPoint &p( mPoints[ pi ] );
			// Just ignore W, should be 1... but could divide by W...
			dPoints[ pi ] = Vector( p.x, p.y, p.z );
		}

		// Add & set the data
		pDmeMesh->AddVertexData( positionIndex, numVertices );
		pDmeMesh->SetVertexData( positionIndex, 0, numVertices, AT_VECTOR3, dPoints.Base() );
	}

	// Ask CDmeMesh where to store the mesh normals
	const FieldIndex_t normalIndex( pDmeMesh->CreateField<Vector>( "normals" ) );

	{
		// Do normals

		// Get the data from Maya
		MFloatVectorArray mNormals;
		meshFn.getNormals( mNormals );
		Assert( static_cast<unsigned>( numNormals ) == mNormals.length() );

		// Add & set the data
		pDmeMesh->AddVertexData( normalIndex, numNormals );
		pDmeMesh->SetVertexData( normalIndex, 0, numNormals, AT_VECTOR3, &mNormals[ 0 ] );
	}

	// Field index for UVs, might not be used
	FieldIndex_t uvIndex( 0 );

	if ( numUVs )
	{
		// Do UVs

		// Get the data from Maya
		MFloatArray mUs;
		MFloatArray mVs;
		meshFn.getUVs( mUs, mVs );

		const float *pUSrc( &mUs[ 0 ] );
		const float *pVSrc( &mVs[ 0 ] );

		// Convert them into the format Dme wants
		CUtlVector< Vector2D > dUVs;
		dUVs.EnsureCount( numUVs );

		Assert( numUVs == static_cast<int>( mUs.length() ) );
		Assert( numUVs == static_cast<int>( mVs.length() ) );
		Assert( numUVs == dUVs.Count() );

		const Vector2D *const pUVEnd( dUVs.Base() + numUVs );

		for ( Vector2D *pUVDst( dUVs.Base() ); pUVDst < pUVEnd; ++pUVDst, ++pUSrc, ++pVSrc )
		{
			pUVDst->x = *pUSrc;
			pUVDst->y = ( 1.0f - *pVSrc );	// V's are flipped this way in Dme...
			// Of course, this "fix" assumes UVs only run 0-1 but I'm guessing a lot more
			// would break if they didn't
		}

		// Ask CDmeMesh where to store the mesh UVs
		uvIndex = pDmeMesh->CreateField<Vector2D>( "textureCoordinates" );

		// Add & set the data
		pDmeMesh->AddVertexData( uvIndex, numUVs );
		pDmeMesh->SetVertexData( uvIndex, 0, numUVs, AT_VECTOR2, dUVs.Base() );
	}

	// Field index for Colors, might not be used
	FieldIndex_t colorIndex( 0 );

	if ( numColors )
	{
		// Do Colors

		// Get the data from Maya
		MColorArray mColorArray;
		meshFn.getColors( mColorArray );

		const MColor *pColorSrc( &mColorArray[ 0 ] );

		// Convert them into the format Dme wants
		CUtlVector< Color > dColors;
		dColors.EnsureCount( numColors );

		Assert( numColors == static_cast<int>( mColorArray.length() ) );
		Assert( numColors == dColors.Count() );

		const Color *const pColorEnd( dColors.Base() + numColors );

		float r, g, b, a;

		for ( Color *pColor( dColors.Base() ); pColor < pColorEnd; ++pColor, ++pColorSrc )
		{
			pColorSrc->get( MColor::kRGB, r, g, b, a );
			pColor->SetColor(
				v_clamp( static_cast<int>( floor( r * 255.0f ) ), 0, 255 ),
				v_clamp( static_cast<int>( floor( g * 255.0f ) ), 0, 255 ),
				v_clamp( static_cast<int>( floor( b * 255.0f ) ), 0, 255 ),
				v_clamp( static_cast<int>( floor( a * 255.0f ) ), 0, 255 ) );
		}

		// Ask CDmeMesh where to store the mesh colors
		colorIndex = pDmeMesh->CreateField<Color>( "colors" );

		// Add & set the data
		pDmeMesh->AddVertexData( colorIndex, numColors );
		pDmeMesh->SetVertexData( colorIndex, 0, numColors, AT_COLOR, dColors.Base() );
	}

	pDmeMesh->AddVertexIndices( numFaceVertices );
	pDmeMesh->SetVertexIndices( positionIndex, 0, numFaceVertices, positionIndices.Base() );
	pDmeMesh->SetVertexIndices( normalIndex, 0, numFaceVertices, normalIndices.Base() );

	if ( numUVs )
	{
		pDmeMesh->SetVertexIndices( uvIndex, 0, numFaceVertices, uvIndices.Base() );
	}

	if ( numColors )
	{
		pDmeMesh->SetVertexIndices( colorIndex, 0, numFaceVertices, colorIndices.Base() );
	}

	// Tell the parent it has a shape now
	i_pDmeDagParent->SetShape( pDmeMesh );

	if ( IsSkinned( i_mDagPath ) )
	{
		// This mesh is skinned so put it on the appropriate lists
		io_exportData.m_bindList.add( i_mDagPath, MObject::kNullObj, true );
		io_exportData.m_mayaToDmeDag.Insert( CUtlString( i_mDagPath.fullPathName().asChar() ), i_pDmeDagParent );

		// See if there's an active dmeModel for this Mesh
		CDmeModel *pDmeModel( io_exportData.m_dmeModelStack.Top() );

		if ( pDmeModel )
		{
			IDmAttributeElement *pDmeModelAttr(
				i_pDmeDagParent->FindOrAddAttributeElement(
				"__dmeModel_bind", CDmeModel::GetStaticTypeSymbol() ) );

			if ( pDmeModelAttr )
			{
				pDmeModelAttr->SetValue( pDmeModel->GetHandle() );
			}
		}

		// TODO: Figure out a good way to add on any missed bits... i.e. if the things
		//       that the mesh is skinned to are not exported, things aren't going to work
	}

	// Put this on the done list
	io_exportData.m_exportedList.add( i_mDagPath );
}


//-----------------------------------------------------------------------------
//
// Purpose: Recursively convert the Maya Dag node into it's DME equivalent
// Input  : i_mDagPath		The Maya Dag Path to export
//			i_pDmeDagParent	The CDmeDag that will be the parent of the newly created
//							node, can be NULL
// Output :
//
//-----------------------------------------------------------------------------

CDmeDag *MayaDagToDmeDag(
	MDagPath &i_mDagPath,
	CDmeDag *i_pDmeDagParent,
	CExportData &io_exportData)
{
	// Discard known bad types
	switch ( i_mDagPath.apiType() )
	{
	case MFn::kManipulator3D:
		return NULL;
	default:	// Just so compiler doesn't complain about unhandled cases when switching on an enum
		;
	}

	// Discard invisible if requested - Path visibility check can be slow....
	if ( !( io_exportData.m_exportInvisible || vm::IsPathVisible( i_mDagPath ) ) )
		return NULL;

	// Don't do things twice
	if ( io_exportData.m_exportedList.hasItem( i_mDagPath ) )
	{
		Warning( "%s: Already processed %s\n",
			CVsDmxIOCmd::s_name,
			i_mDagPath.fullPathName().asChar() );
		return NULL;
	}

	if ( i_mDagPath.hasFn( MFn::kTransform ) )
	{
		// If this is a transform, convert it and then convert all of its children
		MFnTransform transformFn( i_mDagPath );

		CDmeDag *pDmeDag( NULL );

		if ( i_mDagPath.apiType() == MFn::kPluginTransformNode && MFnDagNode( i_mDagPath ).typeName() == "vstInfo" )
		{
			pDmeDag = CreateElement< CDmeModel >( transformFn.name().asChar(), io_exportData.m_dmFileId );
			if ( pDmeDag )
			{
				io_exportData.m_dmeModelStack.Push( CastElement< CDmeModel >( pDmeDag ) );
			}

			// TODO: Swizzy extra stuff like dynamic attributes & identify it as a vstInfo node!
		}
		else if ( i_mDagPath.hasFn( MFn::kShape ) )
		{
			// Perhaps check to see if the shape is skinned?
			pDmeDag = CreateElement< CDmeDag >( transformFn.name().asChar(), io_exportData.m_dmFileId );
		}
		else  if ( i_mDagPath.hasFn( MFn::kJoint ) )
		{
			pDmeDag = CreateElement< CDmeJoint >( transformFn.name().asChar(), io_exportData.m_dmFileId );
		}
		else {
			pDmeDag = CreateElement< CDmeDag >( transformFn.name().asChar(), io_exportData.m_dmFileId );
		}

		Assert( pDmeDag );

		// Convert children
		unsigned cEnd( i_mDagPath.childCount() );
		for ( unsigned ci( 0 ); ci != cEnd; ++ci )
		{
			MDagPath cDagPath( i_mDagPath );
			cDagPath.push( i_mDagPath.child( ci ) );
			MayaDagToDmeDag( cDagPath, pDmeDag, io_exportData );
		}

		// If Maya says we have children, see if any of them were terribly interesting to Dme
		if ( cEnd && pDmeDag->GetChildCount() == 0 && !pDmeDag->GetShape() )
		{
			// Don't keep this node because it's not useful for Dme purposes
			g_pDataModel->DestroyElement( pDmeDag->GetHandle() );
			return NULL;
		}

		bool saveBind( false );

		if ( !i_mDagPath.hasFn( MFn::kShape ) && IsInfluenceObject( i_mDagPath ) )
		{
			// If this is an influence object to something that's soft skinned, save it for binding later
			io_exportData.m_mayaToDmeDag.Insert( CUtlString( i_mDagPath.fullPathName().asChar() ), pDmeDag );
			saveBind = true;
		}

		// If we got this far, we either had some interesting children or we're a lone transform
		// so get all the transform information in local space

		if (  i_mDagPath.apiType() != MFn::kPluginTransformNode ||
			MFnDagNode( i_mDagPath ).typeName() != "vstInfo" ) {
			CDmeTransform *pDmeTransform( pDmeDag->GetTransform() );
			pDmeTransform->SetName( ( MString( pDmeDag->GetName() ) + "_xform" ).asChar() );

			if ( i_mDagPath.apiType() == MFn::kJoint )
			{
				const MFnIkJoint jointFn( i_mDagPath );

				// Record the local transformation
				// NOTE: Because of what appears to be a bug in MTransformationMatrix
				//       the rotation is derived from the 4x4 matrix instead of the
				//       uncombined components of rotation to get the same value as
				//       the bind below

				{
					MQuaternion ro;
					jointFn.getOrientation( ro );

					MQuaternion r;
					r = jointFn.transformationMatrix();

					const MVector t( jointFn.getTranslation( MSpace::kTransform ) );

					pDmeTransform->SetOrientation( Quaternion( r.x, r.y, r.z, r.w ) );
					pDmeTransform->SetPosition( Vector( t.x, t.y, t.z ) );
				}

				if ( saveBind )
				{
					IDmAttributeElement *pBindTransformAttr(
						pDmeTransform->FindOrAddAttributeElement(
						"__bind", CDmeTransform::GetStaticTypeSymbol() ) );

					pBindTransformAttr->AddFlag( FATTRIB_DONTSAVE );

					CDmeTransform *pBindTransform(
						CreateElement< CDmeTransform >( pDmeDag->GetName(), io_exportData.m_dmFileId ) );

					pBindTransformAttr->SetValue( pBindTransform->GetHandle() );

					// Copy the current data to the bind pose initially

					pBindTransform->SetOrientation( pDmeTransform->GetOrientation() );
					pBindTransform->SetPosition( pDmeTransform->GetPosition() );

					// Find the bind pose data for the joint and save that information
					// If there's no explicit bind pose set, issue a warning and use the
					// current position as the bind pose

					// If the model is not skinned we do not need any bindPose information

					MPlugArray mPlugArray;
					MPlug mPlug( jointFn.findPlug( "bindPose" ) );
					if ( !mPlug.isNull() && mPlug.connectedTo( mPlugArray, false, true ) && mPlugArray.length() )
					{
						if ( mPlugArray.length() > 1 )
						{
							Warning( "%s: Joint %s has more than one bind pose, using first pose\n",
								CVsDmxIOCmd::s_name,
								i_mDagPath.partialPathName().asChar() );
						}

						const MPlug bindPosePlug( mPlugArray[ 0 ] );

						const MObject dagPoseObj( bindPosePlug.node() );
						const MFnDependencyNode dagPoseFn( dagPoseObj );

						const MObject xformMatrixObj( dagPoseFn.attribute( "xformMatrix" ) );

						MPlug xformMatrixPlug( dagPoseObj, xformMatrixObj );
						xformMatrixPlug.selectAncestorLogicalIndex( bindPosePlug.logicalIndex(), xformMatrixObj );

						MStatus mStatus;
						MObject xformMatrixVal;

						mStatus = xformMatrixPlug.getValue( xformMatrixVal );
						if ( mStatus )
						{
							const MFnMatrixData xformMatrixFn( xformMatrixVal );
							if ( xformMatrixFn.isTransformation() )
							{
								// NOTE: There's a bug in Maya 7.0 which always returns 0
								//       for MTransformationMatrix::rotateOrientation() :(

								const MTransformationMatrix bindPoseMat = xformMatrixFn.transformation();

								// Compiler cannot handle MQuaternion r = bindPoseMat.asRotateMatrix() ?!?!?!
								MQuaternion r;
								r = bindPoseMat.asRotateMatrix();

								const MVector t( bindPoseMat.getTranslation( MSpace::kTransform ) );

								pBindTransform->SetOrientation( Quaternion( r.x, r.y, r.z, r.w ) );
								pBindTransform->SetPosition( Vector( t.x, t.y, t.z ) );
							}
						}
						else
						{
							Warning( " %s: Can't get value from %s\n",
								CVsDmxIOCmd::s_name,
								xformMatrixPlug.info().asChar() );
						}
					}
					else
					{
						Warning( " %s: No bind pose set on joint %s\n",
							CVsDmxIOCmd::s_name,
							jointFn.name().asChar() );
					}
				}
			}
			else
			{
				MFnTransform transformFn( i_mDagPath );

				MQuaternion q;
				transformFn.getRotation( q, MSpace::kTransform );
				pDmeTransform->SetOrientation( Quaternion( q.x, q.y, q.z, q.w ) );

				const MVector t( transformFn.getTranslation( MSpace::kTransform ) );
				pDmeTransform->SetPosition( Vector( t.x, t.y, t.z ) );
			}
		}

		// If we have a parent, let it know we're ok
		if ( i_pDmeDagParent )
		{
			i_pDmeDagParent->AddChild( pDmeDag );
		}

		// Put this on the done list
		io_exportData.m_exportedList.add( i_mDagPath );

		return pDmeDag;
	}
	else if ( i_mDagPath.hasFn( MFn::kShape ) )
	{
		switch ( i_mDagPath.apiType() )
		{
		case MFn::kMesh:
			MayaMeshToDmeMesh( i_mDagPath, i_pDmeDagParent, io_exportData );
			return i_pDmeDagParent;
		default:
			Warning( "%s: Unhandled shape apiType %s on %s\n",
				CVsDmxIOCmd::s_name,
				i_mDagPath.node().apiTypeStr(),
				i_mDagPath.fullPathName().asChar() );
			break;
		}
	}
	else
	{
		Warning( "%s: Unhandled transform apiType %s on %s\n",
			CVsDmxIOCmd::s_name,
			i_mDagPath.node().apiTypeStr(),
			i_mDagPath.fullPathName().asChar() );
	}

	return NULL;
}


//-----------------------------------------------------------------------------
//
// Purpose: Export the specified bits of the maya scene into the specified file
// Input  : i_mArgDatabase	The command line arguments as passed
// Output : MS::kSuccess if ok, MS::kFailure otherwise
//
//-----------------------------------------------------------------------------

MStatus CVsDmxIOCmd::Export(
	const MArgDatabase &mArgDatabase,
	CDmeDag **ppDmeDag )
{
	MString optFilename;
	if ( !ppDmeDag && !mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename ) )
	{
		Error( "%s: No filename specified for export\n", s_name );
		return MS::kFailure;
	}

	MSelectionList optUserSelectionList;
	bool exportInvisible( mArgDatabase.isFlagSet( kOptExportInvisible ) );

	// List of Maya nodes to export (note that all children of these nodes are exported as well)
	MSelectionList toExportList;

	MDagPath mDagPath;

	if ( mArgDatabase.isFlagSet( kOptSelection ) )
	{
		// Get the user's specified selection of stuff to export
		if ( !mArgDatabase.getObjects( optUserSelectionList ) )
		{
			Error( "%s: Cannot get list of objects to export\n", s_name );
			return MS::kFailure;
		}
		else if ( optUserSelectionList.isEmpty() )
		{
			Error( "%s: -export -selection specified but nothing is selected!\n", s_name );
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
					if ( exportInvisible || vm::IsPathVisible( mDagPath ) )
					{
						optUserSelectionList.add( mDagPath, MObject::kNullObj, true );
					}
				}
			}
		}
	}

	if ( !mArgDatabase.isFlagSet( kOptAllTypes ) )
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
		Error( "%s: Cannot find anything to export\n", s_name );
		return MS::kFailure;
	}

	// Data kept track of during the export process
	CExportData exportData;

	// The node which is the ultimate ancestor of all of the nodes created on export
	CDmeDag *pScene( NULL );

	{
		// The first node returned from depth first conversion will be stored here
		// If only one node is returned ( which will normally be the case, one CDmeModel )
		// Then pScene will be assigned to pRoot before this block exits

		CDmeDag *pRoot( NULL );

		// Export what's in toExportList
		// If only one node is returned, use that as the root node of the Dme scene
		// If more than one is returned, group them all together in a dummy DmeDag node called "vsDmxIO Scene"

		for ( MItSelectionList sIt( toExportList, MFn::kDagNode ); !sIt.isDone(); sIt.next() )
		{
			if ( !sIt.getDagPath( mDagPath ) )
				continue;

			CDmeDag *pDmeDag( MayaDagToDmeDag( mDagPath, NULL, exportData ) );

			// No more DME Model
			exportData.m_dmeModelStack.Clear();

			if ( !pDmeDag )
				continue;

			if ( pRoot )
			{
				if ( pScene )
				{
					pScene->AddChild( pDmeDag );
				}
				else
				{
					pScene = CreateElement< CDmeDag >( "vsDmxIO Scene", exportData.m_dmFileId );

					pScene->AddChild( pRoot );
					pScene->AddChild( pDmeDag );
				}
			}
			else
			{
				pRoot = pDmeDag;
			}
		}

		if ( !pScene && pRoot )
		{
			pScene = pRoot;
		}
	}

	// Now go through the exportData and see if we need to do any binding!
	Bind( exportData );

	// Do the export
	MString optOutType( "xml" );
	if ( mArgDatabase.isFlagSet( kOptOutType ) )
	{
		mArgDatabase.getFlagArgument( kOptOutType, 0, optOutType );
		bool outTypeOk( false );
		for ( int i( 0 ); i < g_pDataModel->GetSerializerCount();  ++i )
		{
			 if ( optOutType == g_pDataModel->GetSerializerName( i ) )
			 {
				 outTypeOk = true;
				 break;
			 }
		}

		if ( !outTypeOk )
		{
			Warning( "%s: Specified -outType \"%s\" is invalid, using xml\n", s_name, optOutType.asChar() );
			optOutType = "xml";
		}
	}

	if ( pScene )
	{
		if ( ppDmeDag )
		{
			// See if we're doing a filter operation
			// If so, Dme data created will be cleared on import

			*ppDmeDag = pScene;
		}
		else
		{
			// Tag the top level node with extra junk
			AddTags( pScene, exportData );

			if ( !g_pDataModel->SaveToFile( optFilename.asChar(), NULL, optOutType.asChar(), pScene ) )
			{
				Error( "%s: Encountered an error writing file \"%s\"!\n", s_name, optFilename.asChar() );
				return MS::kFailure;
			}

			// Does this clear all of the data?!?!
			g_pDataModel->RemoveFileId( pScene->GetFileId() );

			setResult( MString( "Exported to \"" ) + optFilename + "\"" );
		}
	}
	else
	{
		Warning( "%s: Found nothing worth exporting\n", s_name );
		return MS::kFailure;
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose: Data required during import
//
//-----------------------------------------------------------------------------

class CImportData
{
public:
	MDagModifier &m_mDagModifier;					// A Maya Dag Modifier to build the scene
	CUtlStack<CDmeModel *> m_dmeModelStack;			// Currently active DME model (should only be 1)
	MDagPath m_mDagRoot;							// The Maya root Dag node of the converted DmeData

	CImportData(
		MDagModifier &mDagModifier )
	: m_mDagModifier( mDagModifier )
	{}
};


//-----------------------------------------------------------------------------
//
// Purpose: Link a material name and path to a set of indices
//
//-----------------------------------------------------------------------------

class CShadingGroup
{
public:
	MString m_materialName;
	MString m_materialPath;
	MIntArray m_faces;
};


//-----------------------------------------------------------------------------
//
// Purpose: Convert the given DmeMesh to a Maya Mesh
//
//-----------------------------------------------------------------------------

void DmeMeshToMayaMesh(
	CDmeMesh *pDmeMesh,
	MObject parentObj,
	CImportData &importData )
{
	// Need something to convert and a valid Maya parent

	if ( !pDmeMesh || parentObj.isNull() )
		return;

	const FieldIndex_t pIndex( pDmeMesh->FindFieldIndex( "positions" ) );

	if ( pIndex < 0 )
	{
		Error( "%s: No positions in DmeMesh %s, cannot convert to Maya\n",
			CVsDmxIOCmd::s_name,
			pDmeMesh->GetName() );
		return;
	}

	// Suck the vertices out of Dme

	MFloatPointArray mVertexArray;
	{
		// Copy Dme Data to Maya format
		const CUtlVector< Vector > &positions( pDmeMesh->GetPositionData() );

		mVertexArray.setLength( positions.Count() );

		Assert( static_cast<int>( mVertexArray.length() ) == positions.Count() );

		float *pDst( &mVertexArray[ 0 ].x );

		const Vector *pSrc( positions.Base() );
		const Vector *const pSrcEnd( pSrc + positions.Count() );
		for ( ; pSrc != pSrcEnd; ++pSrc )
		{
			*pDst++ = pSrc->x;
			*pDst++ = pSrc->y;
			*pDst++ = pSrc->z;
			*pDst++ = 1.0f;
		}
	}

	// Suck the texture coordinates out of Dme

	const FieldIndex_t uvIndex( pDmeMesh->FindFieldIndex( "textureCoordinates" ) );

	MFloatArray muArray;
	MFloatArray mvArray;

	if ( uvIndex >= 0 )
	{
		const CUtlVector< Vector2D > &uvs( pDmeMesh->GetVertexData<Vector2D>( uvIndex )->GetValue() );

		muArray.setLength( uvs.Count() );
		mvArray.setLength( uvs.Count() );

		Assert( static_cast<int>( muArray.length() ) == uvs.Count() );
		Assert( static_cast<int>( mvArray.length() ) == uvs.Count() );

		float *puDst( &muArray[ 0 ] );
		float *pvDst( &mvArray[ 0 ] );

		const Vector2D *pSrc( uvs.Base() );
		const Vector2D *const pSrcEnd( pSrc + uvs.Count() );
		for ( ; pSrc != pSrcEnd; ++pSrc )
		{
			*puDst++ = pSrc->x;
			*pvDst++ = pSrc->y;
		}
	}

	// Suck the normals out of Dme
	// TODO: Figure out which normals are or are not face varying?
	// Maybe don't suck it out like this but figure it out from the indices
	// and then build an array Maya likes and make the two calls

	const FieldIndex_t nIndex( pDmeMesh->FindFieldIndex( "normals" ) );

	MVectorArray mNormalArray;
	if ( nIndex >= 0 )
	{
		const CUtlVector< Vector > &normals( pDmeMesh->GetVertexData<Vector>( nIndex )->GetValue() );

		mNormalArray.setLength( normals.Count() );

		Assert( static_cast<int>( mNormalArray.length() ) == normals.Count() );

		double *pDst( &mNormalArray[ 0 ].x );

		const Vector *pSrc( normals.Base() );
		const Vector *const pSrcEnd( pSrc + normals.Count() );
		for ( ; pSrc != pSrcEnd; ++pSrc )
		{
			*pDst++ = pSrc->x;
			*pDst++ = pSrc->y;
			*pDst++ = pSrc->z;
		}
	}

	// Suck the colors out of Dme

	const FieldIndex_t cIndex( pDmeMesh->FindFieldIndex( "colors" ) );

	MColorArray mColorArray;
	if ( cIndex >= 0 )
	{
		const CUtlVector< Color > &colors( pDmeMesh->GetVertexData< Color >( cIndex )->GetValue() );

		mColorArray.setLength( colors.Count() );

		Assert( static_cast<int>( mColorArray.length() ) == colors.Count() );

		float *pDst( &mColorArray[ 0 ].r );

		const Color *pSrc( colors.Base() );
		const Color *const pSrcEnd( pSrc + colors.Count() );
		for ( ; pSrc != pSrcEnd; ++pSrc )
		{
			*pDst++ = static_cast<float>( pSrc->r() ) / 255.0f;
			*pDst++ = static_cast<float>( pSrc->g() ) / 255.0f;
			*pDst++ = static_cast<float>( pSrc->b() ) / 255.0f;
			*pDst++ = static_cast<float>( pSrc->a() ) / 255.0f;
		}
	}

	// Suck the topology out of Dme

	MIntArray mPolygonCounts;		// Number of vertices in each face, size is number of faces
	MIntArray mPolygonConnects;		// The vertex indices for each face, size is the faceVarying count

	// UV index information for Maya
	const MIntArray &muvCounts( mPolygonCounts );
	MIntArray muvIds;

	// Shading groups
	CUtlVector< CShadingGroup > shadingGroups;

	{
		int faceCount( 0 );			// Number of faces & size of mPolygonCounts
		int faceVaryingCount( 0 );	// Sum of all the values in mPolygonCounts & size of mPolygonConnects

		// Make a quick pass to figure out how much data there is

		const uint fsEnd( pDmeMesh->NumFaceSets() );
		for ( uint fsi( 0 ); fsi != fsEnd; ++fsi )
		{
			CDmeFaceSet *pDmeFaceSet( pDmeMesh->GetFaceSet( fsi ) );
			const int *pSrc( pDmeFaceSet->GetIndices() );
			const int *const pSrcEnd( pSrc + pDmeFaceSet->NumIndices() );
			for ( ; pSrc != pSrcEnd; ++pSrc )
			{
				if ( *pSrc < 0 )
				{
					++faceCount;
				}
				else
				{
					++faceVaryingCount;
				}
			}
		}

		// Resize the Maya arrays (Growing Maya arrays is painful)

		mPolygonCounts.setLength( faceCount );
		mPolygonConnects.setLength( faceVaryingCount );

		Assert( static_cast<int>( mPolygonCounts.length() ) == faceCount );
		Assert( static_cast<int>( mPolygonConnects.length() ) == faceVaryingCount );

		// TODO: Handle reordering faces so they match Maya originally
		//       because Dme requires faces to be reordered into material groups

		int *pFaceCounts( &mPolygonCounts[ 0 ] );		// This is safe unless Maya changes something... :)
		int *pFaceConnects( &mPolygonConnects[ 0 ] );	// This is safe unless Maya changes something... :)
		int faceVertexCount( 0 );

		const CUtlVector<int> &pIndices( pDmeMesh->GetIndexData( pIndex )->GetValue() );

		if ( uvIndex >= 0 )
		{
			const CUtlVector<int> &uvIndices( pDmeMesh->GetIndexData( uvIndex )->GetValue() );
			muvIds.setLength( faceVaryingCount );

			Assert( static_cast<int>( muvIds.length() ) == faceVaryingCount );

			int *puvIds( &muvIds[ 0 ] );

			int dmeFaceIndex( 0 );

			for ( uint fsi( 0 ); fsi != fsEnd; ++fsi )
			{
				CDmeFaceSet *pDmeFaceSet( pDmeMesh->GetFaceSet( fsi ) );

				// Put a new shading group on the list
				CShadingGroup &shadingGroup( shadingGroups[ shadingGroups.AddToTail( CShadingGroup() ) ] );

				// Record information for the shading group
				CDmeMaterial *pDmeMaterial( pDmeFaceSet->GetMaterial() );
				if ( pDmeMaterial )
				{
					shadingGroup.m_materialName = pDmeMaterial->GetName();
					shadingGroup.m_materialPath = pDmeMaterial->GetMaterialName();
				}

				MIntArray &materialFaces( shadingGroup.m_faces );
				materialFaces.setLength( pDmeFaceSet->NumIndices() );
				int *pMaterialFace( &materialFaces[ 0 ] );

				const int *pSrc( pDmeFaceSet->GetIndices() );
				const int *const pSrcEnd( pSrc + pDmeFaceSet->NumIndices() );
				for ( ; pSrc != pSrcEnd; ++pSrc )
				{
					if ( *pSrc < 0 )
					{
						*pFaceCounts++ = faceVertexCount;
						*pMaterialFace++ = dmeFaceIndex++;
						faceVertexCount = 0;
					}
					else
					{
						++faceVertexCount;
						*pFaceConnects++ = pIndices[ *pSrc ];
						*puvIds++ = uvIndices[ *pSrc ];
					}
				}
			}
		}
	}

	// Create the mesh
	// Maya doesn't really let an MDagModifier be used but that's ok because the
	// parent was made with an MDagModifier and when that is undone, it will
	// delete this too

	MFnMesh meshFn;
	MObject meshObj( meshFn.create(
		mVertexArray.length(),
		mPolygonCounts.length(),
		mVertexArray,
		mPolygonCounts,
		mPolygonConnects,
		parentObj ) );

	if ( meshObj.isNull() )
	{
		Error( "%s: Couldn't create Maya mesh from DmeMesh %s\n",
			CVsDmxIOCmd::s_name,
			pDmeMesh->GetName() );
		return;
	}

	importData.m_mDagModifier.renameNode( meshObj, pDmeMesh->GetName() );
	importData.m_mDagModifier.doIt( );

	// Add the UVs
	if ( muvIds.length() && muArray.length() && muArray.length() == mvArray.length() )
	{
		if ( meshFn.numUVSets() )
		{
			meshFn.clearUVs();
			meshFn.setUVs( muArray, mvArray );
			meshFn.assignUVs( muvCounts, muvIds );
		}
		else
		{
			Error( "%s: No UV set on \n", CVsDmxIOCmd::s_name );
		}
	}

	// Add the shading groups

	MDagPath meshDagPath;
	MDagPath::getAPathTo( meshObj, meshDagPath );

	const int sgEnd( shadingGroups.Count() );
	for ( int sgi( 0 ); sgi < sgEnd; ++sgi )
	{
		CShadingGroup &sg( shadingGroups[ sgi ] );

		MDGModifier &mDGModifier( ( MDGModifier & )( importData.m_mDagModifier ) );

		// Create the shadingEngine (shadingGroup), really just a Set node
		MObject shadingEngineObj( mDGModifier.createNode( "shadingEngine" ) );
		mDGModifier.doIt();

		mDGModifier.renameNode( shadingEngineObj, sg.m_materialName + "SG" );
		mDGModifier.doIt();

		// Add the faces to it
		MFnSingleIndexedComponent sicFn;
		MObject faceSetObj( sicFn.create( MFn::kMeshPolygonComponent ) );
		sicFn.addElements( sg.m_faces );

		MFnSet shadingEngineFn( shadingEngineObj );
		shadingEngineFn.addMember( meshDagPath, faceSetObj );

		// Create the phong surface shader
		const MObject phongObj( mDGModifier.createNode( "phong" ) );
		mDGModifier.doIt();

		mDGModifier.renameNode( phongObj, sg.m_materialName );
		mDGModifier.doIt();

		const MFnDependencyNode phongFn( phongObj );

		// Create the file node
		const MObject fileObj( mDGModifier.createNode( "file" ) );
		mDGModifier.doIt();

		mDGModifier.renameNode( fileObj, sg.m_materialName + "Tex" );
		mDGModifier.doIt();

		const MFnDependencyNode fileFn( fileObj );
		MPlug mP( fileFn.findPlug( "mirrorV" ) );
		mP.setValue( true );

		// TODO: Make a fancy node that updates this as required
		mP = fileFn.findPlug( "fileTextureName" );

		{
			const MString vproject( getenv( "VPROJECT" ) );
			const vLen( vproject.length() );

			// Just a little sanity
			if ( vLen < 4096 )
			{
				char *const pv( static_cast< char * >( _alloca( vLen + 1 ) ) );
				Q_memcpy( pv, vproject.asChar(), vLen + 1 );
				char *pvEnd( pv + vLen );

				for ( char *p( pv ); p != pvEnd;  ++p )
				{
					if ( *p == '\\' )
					{
						*p = '/';
					}
				}

				MStringArray vA;
				MString( pv ).split( '/', vA );

				if ( vA.length() )
				{
					MString texPath( vA[ 0 ] );
					for ( int vi( 1 ); vi < static_cast<int>( vA.length() ) - 2; ++vi )
					{
						texPath += "/";
						texPath += vA[ vi ];
					}

					texPath += "/content/";
					texPath += vA[ vA.length() - 1 ];
					texPath += "/materialsrc/";
					texPath += sg.m_materialPath;
					texPath += ".tga";

					mP.setValue( texPath );
				}
			}
		}

		// Now hook it all up!

		const MPlug fileSrcP( fileFn.findPlug( "outColor" ) );
		const MPlug phongDstP( phongFn.findPlug( "color" ) );

		mDGModifier.connect( fileSrcP, phongDstP );

		const MPlug phongSrcP( phongFn.findPlug( "outColor" ) );
		const MPlug groupDstP( shadingEngineFn.findPlug( "surfaceShader" ) );

		mDGModifier.connect( phongSrcP, groupDstP );

		mDGModifier.doIt();
	}
}


//-----------------------------------------------------------------------------
//
// Purpose: Convert the given DmeDag node (and its children) to a Maya scene
//
//-----------------------------------------------------------------------------

void DmeDagToMayaDag(
	CDmeDag *pDmeDag,
	MObject parentObj,
	CImportData &importData )
{
	MString mayaNodeType( "transform" );

	CDmeModel *pDmeModel( NULL );

	// Figure out what type of Maya node to create for this node

	if ( pDmeDag->IsA( CDmeModel::GetStaticTypeSymbol() ) )
	{
		mayaNodeType = "vstInfo";
		pDmeModel = CastElement< CDmeModel >( pDmeDag );
		importData.m_dmeModelStack.Push( pDmeModel );
	}
	else if ( pDmeDag->IsA( CDmeJoint::GetStaticTypeSymbol() ) )
	{
		mayaNodeType = "joint";
		// TODO: For joints:
		// * Connect parent inverse scale
		// * Save other things about the joints ( preferred angle )
		// * Do bind pose ( in Bind step I suppose... )
	}

	// Create the Maya node

	MDagModifier &mDagModifier( importData.m_mDagModifier );

	MObject thisObj( mDagModifier.createNode( mayaNodeType, parentObj ) );
	mDagModifier.doIt();

	if ( thisObj.isNull() )
	{
		Error( "%s: Couldn't convert DmeDag node %s into a Maya node\n",
			CVsDmxIOCmd::s_name,
			pDmeDag->GetName() );
		return;
	}

	mDagModifier.renameNode( thisObj, pDmeDag->GetName() );
	mDagModifier.doIt();

	// If this is the first Maya node created, then it's got to be the root
	if ( !importData.m_mDagRoot.isValid() )
	{
		MDagPath::getAPathTo( thisObj, importData.m_mDagRoot );
	}

	// Set the transformation information for this node
	CDmeTransform *pDmeTransform( pDmeDag->GetTransform() );
	const Vector p( pDmeTransform->GetPosition() );
	const Quaternion o( pDmeTransform->GetOrientation() );

	MFnTransform thisFn( thisObj );
	thisFn.setTranslation( MVector( p.x, p.y, p.z ), MSpace::kTransform );
	thisFn.setRotation( MQuaternion( o.x, o.y, o.z, o.w ), MSpace::kTransform );

	// Handle the shape of this node

	CDmeShape *pDmeShape( pDmeDag->GetShape() );
	DmeMeshToMayaMesh( CastElement< CDmeMesh >( pDmeShape ), thisObj, importData );

	// TODO: Restore any extra recorded attributes

	// Handle any children of this node

	const int cEnd( pDmeDag->GetChildCount() );
	for ( int ci( 0 ); ci < cEnd; ++ci )
	{
		DmeDagToMayaDag( pDmeDag->GetChild( ci ), thisObj, importData );
	}

	if ( pDmeModel )
	{
		// TODO - Bind

		Assert( importData.m_dmeModelStack.Count() && importData.m_dmeModelStack.Top() == pDmeModel );
		importData.m_dmeModelStack.Pop();
	}
}


//-----------------------------------------------------------------------------
//
// Purpose: Export the specified bits of the maya scene into the specified file
// Input  : mArgDatabase	The command line arguments as passed
// Output : MS::kSuccess if ok, MS::kFailure otherwise
//
//-----------------------------------------------------------------------------

MStatus CVsDmxIOCmd::Import(
	const MArgDatabase &mArgDatabase,
	CDmeDag *pDmeDag )
{
	if ( !pDmeDag )
	{
		// Do an import from a filename

		MString optFilename;
		if ( !mArgDatabase.getFlagArgument( kOptFilename, 0, optFilename ) )
		{
			Error( "%s: No filename specified for import\n", s_name );
			return MS::kFailure;
		}

		CDmElement *pDmeElement( NULL );

		g_pDataModel->RestoreFromFile( optFilename.asChar(), NULL, NULL, false, &pDmeElement );

		pDmeDag = CastElement< CDmeDag >( pDmeElement );
		if ( !pDmeDag )
		{
			Error( "%s: Couldn't load any DmeDag data from %s\n", s_name, optFilename.asChar() );
			return MS::kFailure;
		}
	}

	// Create a dag modifier for undo and the import data class
	m_undoDagModifier = new MDagModifier;
	CImportData importData( *m_undoDagModifier );

	// Convert the Dme data into Maya format
	DmeDagToMayaDag( pDmeDag, MObject::kNullObj, importData );

	// Clear all of the imported Dme data as it's usefulness has past
	g_pDataModel->RemoveFileId( pDmeDag->GetFileId() );

	// Just see what happened when the import was run
	if ( !importData.m_mDagRoot.isValid() )
	{
		Error( "%s: Nothing was converted when trying to import" );

		// undo just in case something wacky was created
		m_undoDagModifier->undoIt();
		delete m_undoDagModifier;
		m_undoDagModifier = NULL;
		return MS::kFailure;
	}

	// Enable undoing of all of this because it looks ok
	m_undoable = true;

	// Make things cute for the user
	MGlobal::getActiveSelectionList( m_undoSelectionList );
	MGlobal::select( importData.m_mDagRoot, MObject::kNullObj, MGlobal::kReplaceList );

	setResult( importData.m_mDagRoot.partialPathName() );

	return MS::kSuccess;
}