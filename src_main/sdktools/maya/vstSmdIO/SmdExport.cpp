//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CSmdExport
//
//=============================================================================


#if _MSC_VER >= 1300
#pragma warning(disable : 4702)	// warning C4702: unreachable code
#endif //_MSV_VER >= 1300

// Standard includes
#include <assert.h>
#include <iomanip>


// Maya includes
#include <maya/MAnimControl.h>
#include <maya/MBoundingBox.h>
#include <maya/MDagPathArray.h>
#include <maya/MItDag.h>
#include <maya/MIteratorType.h>
#include <maya/MEulerRotation.h>
#include <maya/MFileIO.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnGeometryFilter.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnSet.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>


// Valve includes
#include "valveMaya.h"


// Local includes
#include "VProjectUtil.h"
#include "SmdExport.h"


// Local typedefs
typedef int int3[3];


//-----------------------------------------------------------------------------
// Returns the bind matrix for a given dag path, if it doesn't have a
// stored bind matrix, the current local matrix is returned
//-----------------------------------------------------------------------------
MMatrix GetBindMatrix(
	const MDagPath &mDagPath,
	const bool bindCurrent )
{
	if ( !bindCurrent )
	{
		MPlugArray mPlugArray;
		MPlug mPlug( MFnDagNode( mDagPath ).findPlug( "bindPose" ) );
		if ( !mPlug.isNull() && mPlug.connectedTo( mPlugArray, false, true ) && mPlugArray.length() )
		{
			const MPlug bindPosePlug( mPlugArray[ 0 ] );

			const MObject dagPoseObj( bindPosePlug.node() );
			const MFnDependencyNode dagPoseFn( dagPoseObj );

			const MObject xformMatrixObj( dagPoseFn.attribute( "xformMatrix" ) );

			MPlug xformMatrixPlug( dagPoseObj, xformMatrixObj );
			xformMatrixPlug.selectAncestorLogicalIndex( bindPosePlug.logicalIndex(), xformMatrixObj );

			MObject xformMatrixVal;

			if ( xformMatrixPlug.getValue( xformMatrixVal ) )
				return MFnMatrixData( xformMatrixVal ).matrix();
		}
	}

	return MFnDagNode( mDagPath ).transformationMatrix();
}


//-----------------------------------------------------------------------------
// Returns the world space bind matrix for a given dag path
//-----------------------------------------------------------------------------
MMatrix GetWorldBindMatrix(
	const MDagPath &mDagPath,
	const bool bindCurrent )
{
	if ( !bindCurrent )
	{
		MPlugArray mPlugArray;
		MPlug mPlug( MFnDagNode( mDagPath ).findPlug( "bindPose" ) );
		if ( !mPlug.isNull() && mPlug.connectedTo( mPlugArray, false, true ) && mPlugArray.length() )
		{
			const MPlug bindPosePlug( mPlugArray[ 0 ] );

			const MObject dagPoseObj( bindPosePlug.node() );
			const MFnDependencyNode dagPoseFn( dagPoseObj );

			const MObject worldMatrixObj( dagPoseFn.attribute( "worldMatrix" ) );

			MPlug xformMatrixPlug( dagPoseObj, worldMatrixObj );
			xformMatrixPlug.selectAncestorLogicalIndex( bindPosePlug.logicalIndex(), worldMatrixObj );

			MObject worldMatrixVal;

			if ( xformMatrixPlug.getValue( worldMatrixVal ) )
				return MFnMatrixData( worldMatrixVal ).matrix();
		}
	}

	return mDagPath.inclusiveMatrix();
}


//-----------------------------------------------------------------------------
// Purpose: Determine if a given Maya Dag node is visible
// Input:	i_mDagPath				The node to check for visibility
// Output:	i_templateAsInvisible	If true, template objects are considered to be invisible
//-----------------------------------------------------------------------------
bool CSmdExport::IsNodeVisible( const MDagPath &mDagPath, bool bTemplateAsInvisible ) const
{
	const MFnDagNode dagFn( mDagPath );

	// All intermediate objects are considered invisible
	if ( dagFn.isIntermediateObject() )
		return false;

	if ( OptExportInvisible() )
		return true;

	// visibility off == invisible!
	bool visibleVal( false );
	dagFn.findPlug( "visibility" ).getValue( visibleVal );
	if ( !visibleVal )
		return false;

	// If template objects are to be treated as invisible, template == invisible!
	if ( bTemplateAsInvisible )
	{
		bool templateVal( false );
		dagFn.findPlug( "template" ).getValue( templateVal );
		if ( templateVal )
			return false;
	}

	bool overrideVal( false );
	dagFn.findPlug("overrideEnabled").getValue( overrideVal );
	if ( overrideVal )
	{
		// visibility off == invisible!
		dagFn.findPlug("overrideVisibility").getValue( visibleVal );
		if ( !visibleVal )
			return false;

		// If template objects are to be treated as invisible, template == invisible!
		if ( bTemplateAsInvisible )
		{
			int displayTypeVal( 0 );
			dagFn.findPlug( "overrideDisplayType" ).getValue( displayTypeVal );
			if ( displayTypeVal == 1 )
				return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Determine if a given Maya Dag path is visible through
//			Simply makes sure the node and all of its ancestors are visible
// Input:	mDagPath				The node to check for visibility
// Output:	templateAsInvisible		If true, template objects are considered to be invisible
//-----------------------------------------------------------------------------
bool CSmdExport::IsPathVisible( MDagPath mDagPath, bool bTemplateAsInvisible ) const
{
	for ( ; mDagPath.length(); mDagPath.pop() ) 
	{
		if ( !IsNodeVisible( mDagPath, bTemplateAsInvisible ) )
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Remove the Maya namespace(s) from a given string
// Input:	mString					The string from which to remove any namespaces
// Output:	The string with any Maya namespaces removed
//-----------------------------------------------------------------------------
MString RemoveNameSpace(
	const MString &mString )
{
	const int cIndex( mString.rindex( ':' ) );
	if ( cIndex >= 0 )
	{
		const int l( mString.length() );
		if ( cIndex + 1 < l )
		{
			return mString.substring( cIndex + 1, l - 1 );
		}
	}

	return mString;
}


//-----------------------------------------------------------------------------
// CDagData Constructor
//-----------------------------------------------------------------------------
CDagData::CDagData(
	int index,
	const MDagPath &mDagPath,
	const CDagData *pDagParent,
	const bool bBindCurrent )
: m_index( index )
, m_mDagPath( mDagPath )
, m_pDagParent( pDagParent )
, m_bindMatrix( GetBindMatrix( mDagPath, bBindCurrent ) )
, m_worldBindMatrix( GetWorldBindMatrix( mDagPath, bBindCurrent ) )
{
}


//-----------------------------------------------------------------------------
// CMeshData Constructor
//-----------------------------------------------------------------------------
CMeshData::CMeshData(
	const MDagPath &mDagPath,
	const MDagPath &baseDagPath,
	const MObject &skinObj,
	const MObject &blendObj,
	const bool bBindCurrent )
: m_mDagPath( mDagPath )
, m_baseDagPath ( bBindCurrent ? mDagPath : baseDagPath )
, m_worldBindMatrix( GetWorldBindMatrix( m_mDagPath, bBindCurrent ) )
, m_skinObj( skinObj )
, m_blendObj( blendObj )
{
}


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CSmdExport::CSmdExport(
	uint type,
	uint options,
	uint version )
: m_version( version )
, m_debugMaterial( m_version > 1 ? "debug/debugempty" : "debugempty" )
, m_type( type )
, m_options( options )
, m_optUpAxis( 0 )
, m_optRotMat( MMatrix::identity )
, m_optPrettyPrint( ( options & kOptPrettyPrint ) != 0 )
, m_optWorld( ( options & kOptWorld ) != 0 )
, m_maxNameLen( 0 )
, m_fs( 0.0 )
, m_fe( 0.0 )
, m_fi( 1.0 )
{
	if ( options & kOptYUp )
	{
		m_optUpAxis = kOptYUp;
	}
	else if ( options & kOptZUp )
	{
		m_optUpAxis = kOptZUp;
	}
	else if ( options & kOptXUp )
	{
		m_optUpAxis = kOptXUp;
	}
	else
	{
		m_optUpAxis = kOptYUp;
	}

	if ( MGlobal::isYAxisUp() )
	{
		if ( m_optUpAxis == kOptXUp )
		{
			m_optRotMat = MEulerRotation( M_PI / 2.0, -M_PI / 2.0, 0.0 ).asMatrix();
		}
		else if ( m_optUpAxis == kOptZUp )
		{
			m_optRotMat = MEulerRotation( M_PI / 2.0, 0.0, 0.0 ).asMatrix();
		}
	}
	else
	{
		// Z Axis is up
		if ( m_optUpAxis == kOptXUp )
		{
			m_optRotMat = MEulerRotation( 0.0, -M_PI / 2.0, 0.0 ).asMatrix();
		}
		else if ( m_optUpAxis == kOptYUp )
		{
			m_optRotMat = MEulerRotation( -M_PI / 2.0, 0.0, 0.0 ).asMatrix();
		}
	}
}


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CSmdExport::~CSmdExport()
{
	for ( DagMap_t::iterator di( m_dagMap.begin() ); di != m_dagMap.end(); ++di )
	{
		delete di->second;
	}

	for ( MeshList_t::iterator mi( m_meshDataList.begin() ); mi != m_meshDataList.end(); ++mi )
	{
		delete *mi;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CSmdExport::DoIt(
	const MString &filename,
	const MSelectionList &exportList,
	MStringArray &result )
{
	if ( m_os.is_open() )
	{
		MGlobal::displayError( "File stream is already open, aborting export" );
		return MS::kFailure;
	}

	m_os.open( filename.asChar() );
	if ( !m_os )
	{
		MGlobal::displayError( MString( "Can't open \"" ) + filename + "\" for writing, aborting export" );
		return MS::kFailure;
	}

	OutputHeader();

	m_os.precision( 6 );
	m_os.setf( std::ios_base::fixed );

	m_os << "version " << m_version << "\n";

	CreateExportLists( exportList );

	OutputExportData();

	m_os.close();

	CVProjectUtil vp( filename.asChar(), CVProjectUtil::kFlagsDefault );

	if ( m_options & kOptMaterialVmt )
	{
		OutputVmts( vp, result );
	}

	if ( m_options & kOptQc )
	{
		unsigned int qcFlags(
			( m_options & kOptXUp ? CVProjectUtil::kQcXUp : 0 ) |
			( m_options & kOptYUp ? CVProjectUtil::kQcYUp : 0 ) |
			( m_options & kOptZUp ? CVProjectUtil::kQcZUp : 0 ) |
			( m_options & kOptQcCollapseBones ? CVProjectUtil::kQcCollapseBones : 0 ) |
			( m_options & kOptQcSkipBoneInBBox ? CVProjectUtil::kQcSkipBoneInBBox : 0 ) |
			( m_options & kOptQcCenterBonesOnVerts ? CVProjectUtil::kQcCenterBonesOnVerts : 0 ) 
			);

		if ( ( m_options & ( kOptXUp | kOptYUp | kOptZUp ) ) == 0 )
		{
			if ( MGlobal::isYAxisUp() )
			{
				qcFlags |= CVProjectUtil::kQcYUp;
			}
			else
			{
				qcFlags |= CVProjectUtil::kQcZUp;
			}
		}

		std::list< std::string > qcStuff;

		const std::string qc( vp.WriteQc( qcFlags, qcStuff ) );
		if ( !qc.empty() )
		{
			result.append( "qc" );
			result.append( qc.c_str() );
		}
	}

	if ( m_options & kOptRunStudiomdl )
	{
		vp.RunStudiomdl();
	}

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputVmts(
	CVProjectUtil &vp,
	MStringArray &result )
{
	const std::map< std::string, CMaterial >::const_iterator mEnd( m_materialList.end() );
	for ( std::map< std::string, CMaterial >::const_iterator mi( m_materialList.begin() ); mi != mEnd; ++mi )
	{
		if ( mi->second.m_name != m_debugMaterial )
		{
			if ( m_options & kOptMaterialSrc )
			{
				std::string tex( vp.CopyTextureToMaterialSrc( mi->second.m_path, mi->second.m_name ) );
				if ( !tex.empty() )
				{
					result.append( "tex" );
					result.append( tex.c_str() );
				}
			}
			if ( m_options & kOptMaterialVtf )
			{
				const std::string vtf( vp.ConvertMaterialSrcToVtf( mi->second.m_name ) );
				if ( !vtf.empty() )
				{
					result.append( "vtf" );
					result.append( vtf.c_str() );
				}
			}

			const std::string vmt( vp.WriteVmt( mi->second.m_name, "VertexLitGeneric", mi->second.m_transparent ) );
			if ( !vmt.empty() )
			{
				result.append( "vmt" );
				result.append( vmt.c_str() );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::SetNodeDelPrefix(
	const MString &nodePrefix )
{
	if ( nodePrefix.length() )
	{
		m_nodeDelPrefix = nodePrefix;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::SetNodeAddPrefix(
	const MString &nodePrefix )
{
	if ( nodePrefix.length() )
	{
		m_nodeAddPrefix = nodePrefix;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::SetFrameRange(
	double fs,
	double fe,
	double fi )
{
	m_fs = fs;
	m_fe = fe;
	m_fi = fi;

	if ( m_fi == 0 )
	{
		m_fi = 1;
	}

	if ( m_fi > 0 )
	{
		if ( m_fe < m_fs )
		{
			const double t( m_fs );
			m_fs = m_fe;
			m_fe = t;
		}
	}
	else if ( m_fi < 0 )
	{
		if ( m_fe > m_fs )
		{
			const double t( m_fs );
			m_fs = m_fe;
			m_fe = t;
		}
	}
}


//-----------------------------------------------------------------------------
// Make a preprocessing pass on the scene to find all of the meshes and
// skeleton nodes to export
//-----------------------------------------------------------------------------
void CSmdExport::CreateExportLists(
	const MSelectionList &masterExportList )
{
	m_nodeList.clear();
	m_meshList.clear();
	m_orderedDagList.clear();
	m_dagMap.clear();
	m_meshDataList.clear();

	MDagPath mDagPath;

	for ( MItSelectionList sIt( masterExportList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() == MItSelectionList::kDagSelectionItem )
		{
			if ( sIt.getDagPath( mDagPath ) )
			{
				PreprocessDag( mDagPath, false );
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputDagInDependentOrder(
	const MDagPath &mDagPath,
	MSelectionList &done )
{
	if ( !done.hasItem( mDagPath ) )
	{
		MDagPath pDagPath( mDagPath );
		pDagPath.pop();
		if ( pDagPath.length() && m_nodeList.hasItem( pDagPath ) && !done.hasItem( pDagPath ) )
		{
			OutputDagInDependentOrder( pDagPath, done );
		}

		assert( m_dagMap.find( mDagPath.partialPathName().asChar() ) == m_dagMap.end() );

		CDagData *pDagData( new CDagData( done.length(), mDagPath, GetParent( mDagPath ), OptBindCurrent() ) );
		m_dagMap[ mDagPath.partialPathName().asChar() ] = pDagData;
		m_orderedDagList.push_back( pDagData );
		done.add( mDagPath );

		OutputDag( pDagData );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputExportData()
{
	if ( m_nodeList.length() == 0 )
		return;

	MDagPath mDagPath;

	m_os << "nodes\n";

	m_orderedDagList.clear();
	m_orderedDagList.reserve( m_nodeList.length() );

	m_dagMap.clear();

	MSelectionList doneList;

	for ( MItSelectionList sIt( m_nodeList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.getDagPath( mDagPath ) )
		{
			OutputDagInDependentOrder( mDagPath, doneList );
		}
	}

	m_os << "end\n";

	m_os << "skeleton\n";

	if ( m_type & kSkeletalAnimation )
	{
		OutputSkeletalAnimation();
	}
	else
	{
		OutputBindSkeleton();
	}

	m_os << "end\n";

	if ( ( m_type & ( kModel | kPhysModel ) ) )
	{
		m_os << "triangles\n";

		const MeshList_t::const_iterator mEnd( m_meshDataList.end() );
		for ( MeshList_t::const_iterator mi( m_meshDataList.begin() ); mi != mEnd; ++mi )
		{
			OutputMesh( *mi );
		}

		m_os << "end\n";
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputBindSkeleton()
{
	if ( m_orderedDagList.empty() )
	{
		m_os << "// No Skeleton\n" ;
		m_os << "time 0\n";
		return;
	}

	m_os << "time 0\n";

	const DagDataList_t::const_iterator dEnd( m_orderedDagList.end() );
	if ( m_optWorld )
	{
		for ( DagDataList_t::const_iterator di( m_orderedDagList.begin() ); di != dEnd; ++di )
		{
			OutputTransform( ( *di )->Index(), ( *di )->WorldBindMatrix() * m_optRotMat ) << "\n";
		}
	}
	else
	{
		for ( DagDataList_t::const_iterator di( m_orderedDagList.begin() ); di != dEnd; ++di )
		{
			if ( ( *di )->HasParent() )
			{
				OutputTransform( ( *di )->Index(), ( *di )->BindMatrix() ) << "\n";
			}
			else
			{
				OutputTransform( ( *di )->Index(), ( *di )->BindMatrix() * m_optRotMat ) << "\n";
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputSkeletalAnimation()
{
	if ( m_orderedDagList.empty() )
	{
		m_os << "// No Skeleton\n" ;
		m_os << "time " << 0 << "\n";
		return;
	}

	const MTime savedCurrentTime( MAnimControl::currentTime() );

	const MTime::Unit uiUnit( MTime::uiUnit() );
	MTime tc( m_fs, uiUnit );
	const MTime te( m_fe, MTime::uiUnit() );

	MDagPath mDagPath;
	MIteratorType mIteratorType;
	mIteratorType.setFilterType( MFn::kDagNode );
	MItDag dIt;
	const CDagData *&rDagData = m_orderedDagList.front();
	const CDagData **ppDagDataBegin = &rDagData;
	const CDagData **ppDagDataEnd = ( ppDagDataBegin + m_orderedDagList.size() );
	const CDagData **ppDagData;
	uint index;


	for ( uint frame( 0 ); tc <= te; ++frame, tc = MTime( m_fs + m_fi * frame, uiUnit ) )
	{
		m_os << "time " << frame << "\n";

		MAnimControl::setCurrentTime( tc );

		// This is a hack to make the exporter simulate Maya's draw cycle
		// So that batch mode acts like GUI mode
		// This came up as a result of parentConstraints doing funny thing
		for ( dIt.reset( mIteratorType, NULL, NULL ); !dIt.isDone(); dIt.next() )
		{
			dIt.getPath( mDagPath );
			MFnDagNode( mDagPath ).boundingBox();
		}

		for ( index = 0, ppDagData = ppDagDataBegin; ppDagData != ppDagDataEnd; ++ppDagData, ++index )
		{
			if ( m_optWorld || !( ( *ppDagData )->HasParent() ) )
			{
				OutputTransform( index, ( *ppDagData )->InclusiveMatrix() * m_optRotMat ) << "\n";
			}
			else
			{
				OutputTransform( index, ( *ppDagData )->LocalMatrix() ) << "\n";

			}
		}
	}

	MAnimControl::setCurrentTime( savedCurrentTime );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputDag(
	const CDagData *pDagData )
{
	const int index( pDagData->Index() );
	const int parentIndex( m_optWorld ? -1 : pDagData->ParentIndex() );

	if ( m_optPrettyPrint )
	{
		m_os << std::setw( 3 ) << index << " \"";

		if ( m_nodeAddPrefix.length() )
		{
			m_os << m_nodeAddPrefix;
		}

		MString dagName( RemoveNameSpace( pDagData->Name() ) );
		if ( m_nodeDelPrefix.length() )
		{
			if ( strncmp( m_nodeDelPrefix.asChar(), dagName.asChar(), m_nodeDelPrefix.length() ) == 0 )
			{
				dagName = dagName.substring( m_nodeDelPrefix.length(), dagName.length() - 1 );
			}
		}
		const uint pad( m_maxNameLen - dagName.length() + 3 );

		m_os << dagName.asChar() << "\" " << std::setw( pad ) << parentIndex << "\n";
	}
	else
	{
		m_os << index << " \"";

		if ( m_nodeAddPrefix.length() )
		{
			m_os << m_nodeAddPrefix.asChar();
		}

		MString dagName( RemoveNameSpace( pDagData->Name() ) );
		if ( m_nodeDelPrefix.length() )
		{
			if ( strncmp( m_nodeDelPrefix.asChar(), dagName.asChar(), m_nodeDelPrefix.length() ) == 0 )
			{
				dagName = dagName.substring( m_nodeDelPrefix.length(), dagName.length() - 1 );
			}
		}
		m_os << dagName.asChar() << "\" " << parentIndex << "\n";
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const CDagData *CSmdExport::GetParent(
	const MDagPath &mDagPath ) const
{
	MDagPath pDagPath( mDagPath );
	pDagPath.pop();

	const DagMap_t::const_iterator pi( m_dagMap.find( pDagPath.partialPathName().asChar() ) );
	if ( pi != m_dagMap.end() )
		return dynamic_cast< const CDagData * >( pi->second );

	return NULL;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int CSmdExport::GetParentIndex(
	const MDagPath &mDagPath ) const
{
	const CDagData *pDagData( GetParent( mDagPath ) );
	if ( pDagData )
		return pDagData->Index();

	return -1;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::GetFaceSets( 
	const MDagPath &mDagPath,
	const MFnMesh &meshFn,
	MObjectArray &shaders,
	std::vector< MFnSingleIndexedComponent * > &faceSetFns )
{
	shaders.clear();
	{
		const std::vector< MFnSingleIndexedComponent * >::const_iterator fsEnd( faceSetFns.end() );
		std::vector< MFnSingleIndexedComponent * >::const_iterator fsi;
		for ( fsi = faceSetFns.end(); fsi != fsEnd; ++fsi )
		{
			delete( *fsi );
		}
	}
	faceSetFns.clear();

	MIntArray faceShaderIndices;

	meshFn.getConnectedShaders( mDagPath.instanceNumber(), shaders, faceShaderIndices );

	// Create shaders.length() + 1 MFnSingleIndexedComponents for each faceSet
	{
		const uint shaderEnd( shaders.length() );
		for ( uint shaderIndex( 0 ); shaderIndex <= shaderEnd; ++shaderIndex )
		{
			// faceSetFns[ 0 ] is for faces with no shading group assigned
			// faceSetFns[ n ] is for shader n - 1
			faceSetFns.push_back( new MFnSingleIndexedComponent );
			faceSetFns.back()->create( MFn::kMeshPolygonComponent );
		}
	}

	// Now loop through faceShaderIndices and construct the single indexed components
	// this way, it's guaranteed that every face is accounted for
	{
		const uint faceEnd( faceShaderIndices.length() );
		assert( faceEnd == static_cast< uint >( meshFn.numPolygons() ) );

		for ( uint faceIndex( 0 ); faceIndex != faceEnd; ++faceIndex )
		{
			faceSetFns[ faceShaderIndices[ faceIndex ] + 1 ]->addElement( faceIndex );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputMesh(
	const CMeshData *pMeshData )
{
	const MDagPath &mDagPath( pMeshData->DagPath() );

	const int parentIndex( GetParentIndex( mDagPath ) );
	const MMatrix xformMat( pMeshData->WorldBindMatrix() * m_optRotMat );

	MFnMesh meshFn( mDagPath );

	bool doubleSided( false );
	if ( OptDoubleSided() )
	{
		MPlug mPlug( meshFn.findPlug( "doubleSided" ) );
		if ( !mPlug.isNull() )
		{
			mPlug.getValue( doubleSided );
		}
	}

	// Build an MFnSingleIndexedComponent Object for each shader plus one for
	// faces which have no shader.  Since 'faceShaderIndices' will contain -1
	// for faces without a shading group assignment, offset the indices by
	// +1.  If Maya's lying about -1 and it's -2 or something, things will be bad

	MObjectArray shaders;
	std::vector< MFnSingleIndexedComponent * > faceSetFns;

	GetFaceSets( mDagPath, meshFn, shaders, faceSetFns );

	std::string targaName( m_debugMaterial );
	std::string targaPath;
	const size_t nFaceSets( faceSetFns.size() );

	MDagPathArray mDagPathArray;
	std::vector< uint > validInfluenceIndices;
	std::vector< int > influenceNodeIndices;

	const MObject &skinObj( pMeshData->SkinObj() );

	if ( !skinObj.isNull() )
	{
		const MFnSkinCluster skinFn( skinObj );
		skinFn.influenceObjects( mDagPathArray, NULL );
		validInfluenceIndices.reserve( mDagPathArray.length() );
		influenceNodeIndices.reserve( mDagPathArray.length() );

		// Remove any shapes from the influence object list
		const uint nInfluence( mDagPathArray.length() );
		for ( uint ii( 0 ); ii != nInfluence; ++ii )
		{
			const MDagPath &iDagPath( mDagPathArray[ ii ] );
			if ( iDagPath.hasFn( MFn::kShape ) )
				continue;

			const DagMap_t::const_iterator imi( m_dagMap.find( iDagPath.partialPathName().asChar() ) );
			if ( imi != m_dagMap.end() )
			{
				const CDagData *pDagData( reinterpret_cast< const CDagData *>( imi->second ) );
				if ( pDagData )
				{
					influenceNodeIndices.push_back( pDagData->Index() );
					validInfluenceIndices.push_back( ii );
				}
			}
		}

		const size_t validInfluenceCount( validInfluenceIndices.size() );
		static const int kMaxWeightsPerVertex( 3 );
		const size_t weightsPerVertex( validInfluenceCount > kMaxWeightsPerVertex ?
			kMaxWeightsPerVertex : validInfluenceCount );

		for ( uint fi( 0 ); fi != nFaceSets; ++fi )
		{
			const MFnSingleIndexedComponent &faceSetFn( *faceSetFns[ fi ] );
			if ( faceSetFn.isEmpty() )
				continue;	// Ignore empty

			MObject fileObj;
			MObject place2dTextureObj;
			bool transparent( false );

			if ( fi != 0 )
			{
				targaName = GetTargaName( targaPath, shaders[ fi - 1 ], fileObj, place2dTextureObj, transparent );
				if ( m_materialList.find( targaName ) == m_materialList.end() )
				{
					m_materialList[ targaName ] = CMaterial( targaPath, targaName, transparent );
				}
			}

			MObject faceSetObj( faceSetFn.object() );
			OutputSkinnedFaceSet(
				mDagPath, meshFn, xformMat, doubleSided,
				fileObj, place2dTextureObj, transparent,
				pMeshData, parentIndex,
				skinFn, weightsPerVertex, validInfluenceCount,
				validInfluenceIndices, influenceNodeIndices,
				faceSetObj, targaName );
		}
	}
	else
	{
		for ( uint fi( 0 ); fi != nFaceSets; ++fi )
		{
			const MFnSingleIndexedComponent &faceSetFn( *faceSetFns[ fi ] );
			if ( faceSetFn.isEmpty() )
				continue;	// Ignore empty

			MObject fileObj;
			MObject place2dTextureObj;
			bool transparent( false );

			if ( fi != 0 )
			{
				targaName = GetTargaName( targaPath, shaders[ fi - 1 ], fileObj, place2dTextureObj, transparent );
				if ( m_materialList.find( targaName ) == m_materialList.end() )
				{
					m_materialList[ targaName ] = CMaterial( targaPath, targaName, transparent );
				}
			}

			MObject faceSetObj( faceSetFn.object() );
			OutputFaceSet( meshFn, xformMat, doubleSided,
				fileObj, place2dTextureObj, transparent,
				pMeshData, parentIndex, faceSetObj, targaName );
		}
	}

	// Delete all of the MFnSingleIdexedComponents
	{
		const std::vector< MFnSingleIndexedComponent * >::const_iterator fsEnd( faceSetFns.end() );
		std::vector< MFnSingleIndexedComponent * >::const_iterator fsi;
		for ( fsi = faceSetFns.end(); fsi != fsEnd; ++fsi )
		{
			delete( *fsi );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputSkinnedFaceSet(
	const MDagPath &mDagPath,
	const MFnMesh &meshFn,
	const MMatrix &xformMat,
	bool doubleSided,
	const MObject &fileObj,
	const MObject &place2dTextureObj,
	bool /* transparent */,
	const CMeshData *pMeshData,
	int parentIndex,
	const MFnSkinCluster &skinFn,
	size_t weightsPerVertex,
	size_t validInfluenceCount,
	const std::vector< uint > &validInfluenceIndices,
	const std::vector< int > &influenceNodeIndices,
	MObject &faceSetObj,
	const std::string &targaName )
{
	MIntArray meshVertexIndices;
	MPointArray tpList;
	MIntArray tvList;

	MPoint ps[ 3 ];
	MVector ns[ 3 ];
	float2 uvs[ 3 ];

	int3 wIndices[ 3 ];
	float3 wValues[ 3 ];
	int nWeights[ 3 ];
	float wTotals[ 3 ];

	const bool warpUV( !( fileObj.isNull() || place2dTextureObj.isNull() ) );
	MPlug iuP;
	MPlug ivP;
	MPlug ouP;
	MPlug ovP;
	if ( warpUV )
	{
		MFnDependencyNode iuvFn( place2dTextureObj );
		MPlug iuvP( iuvFn.findPlug( "uvCoord" ) );
		iuP = iuvP.child( 0 );
		ivP = iuvP.child( 1 );

		MFnDependencyNode ouvFn( fileObj );
		MPlug ouvP( ouvFn.findPlug( "uvCoord" ) );
		ouP = ouvP.child( 0 );
		ovP = ouvP.child( 1 );
	}

	uint mInfluenceCount;
	MDoubleArray mWeights;

	const MDagPath &baseDagPath( pMeshData->BaseDagPath() );
	MItMeshVertex bvIt( baseDagPath );

	MItMeshVertex cvIt( mDagPath );
	int pvIndex;
	MItMeshPolygon cpIt( mDagPath );
	int pfIndex;

	assert( cpIt.count() == MItMeshPolygon( baseDagPath ).count() );

	for ( MItMeshPolygon bpIt( baseDagPath, faceSetObj ); !bpIt.isDone(); bpIt.next() )
	{
		if ( !bpIt.hasValidTriangulation() )
			continue;

		cpIt.setIndex( bpIt.index(), pfIndex );

		bpIt.getVertices( meshVertexIndices );
		const uint nVerts( meshVertexIndices.length() );

		int nTri( 0 );
		bpIt.numTriangles( nTri );
		for ( int ti( 0 ); ti < nTri; ++ti )
		{
			bpIt.getTriangle( ti, tpList, tvList );

			for ( uint tvi( 0 ); tvi != 3; ++tvi )
			{ 
				ps[ tvi ] = ( tpList[ tvi ] * xformMat );

				const int meshVertexIndex( tvList[ tvi ] );

				for ( uint faceVertexIndex( 0 ); faceVertexIndex != nVerts; ++faceVertexIndex )
				{
					if ( meshVertexIndex == meshVertexIndices[ faceVertexIndex ] )
					{
						float2 &uv( uvs[ tvi ] );
						MVector &n( ns[ tvi ] );
						int3 &wIndex( wIndices[ tvi ] );
						float3 &wValue( wValues[ tvi ] );
						int &nWeight( nWeights[ tvi ] );
						float &wTotal( wTotals[ tvi ] );

						uv[ 0 ] = 0.0f;
						uv[ 1 ] = 0.0f;
						wIndex[ 0 ] = 0;
						wIndex[ 1 ] = 0;
						wIndex[ 2 ] = 0;
						wValue[ 0 ] = 0.0f;
						wValue[ 1 ] = 0.0f;
						wValue[ 2 ] = 0.0f;
						nWeight = 0;
						wTotal = 0.0f;

						GetNormal( meshFn, cvIt, meshVertexIndex, cpIt, faceVertexIndex, n );
						n = n.transformAsNormal( xformMat ).normal();

						bpIt.getUV( faceVertexIndex, uv );
						if ( warpUV )
						{
							iuP.setValue( uv[ 0 ] );
							ivP.setValue( uv[ 1 ] );
							ouP.getValue( uv[ 0 ] );
							ovP.getValue( uv[ 1 ] );
						}

						// Get skin weights, discarding ones we cannot use (from shapes)
						// and only keep the 'weightsPerVertex (3)' biggest weights
						bvIt.setIndex( meshVertexIndex, pvIndex );
						skinFn.getWeights( mDagPath, bvIt.vertex(), mWeights, mInfluenceCount );
						for ( size_t vi( 0 ); vi != validInfluenceCount; ++vi )
						{
							const unsigned mInfluenceIndex( validInfluenceIndices[ vi ] );
							const float w( static_cast< float >( mWeights[ mInfluenceIndex ] ) );

							for ( int wi( static_cast< int >( weightsPerVertex ) - 1 ); wi >= 0; --wi )
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
									wIndex[ wi ] = influenceNodeIndices[ vi ];

									// Skip to next weight
									break;
								}
							}
						}

						for ( size_t wi( 0 ); wi != weightsPerVertex; ++wi )
						{
							if ( wValue[ wi ] > 0.0f )
							{
								++nWeight;
								wTotal += wValue[ wi ];
							}
						}

						break;
					}
				}
			}

			m_os << targaName << "\n";
			PrintVertex( parentIndex, ps[ 0 ], ns[ 0 ], uvs[ 0 ] ) << " ";
			PrintWeights( wTotals[ 0 ], weightsPerVertex, nWeights[ 0 ], wIndices[ 0 ], wValues[ 0 ] ) << "\n";
			PrintVertex( parentIndex, ps[ 1 ], ns[ 1 ], uvs[ 1 ] ) << " ";
			PrintWeights( wTotals[ 1 ], weightsPerVertex, nWeights[ 1 ], wIndices[ 1 ], wValues[ 1 ] ) << "\n";
			PrintVertex( parentIndex, ps[ 2 ], ns[ 2 ], uvs[ 2 ] ) << " ";
			PrintWeights( wTotals[ 2 ], weightsPerVertex, nWeights[ 2 ], wIndices[ 2 ], wValues[ 2 ] ) << "\n";

			if ( doubleSided )
			{
				m_os << targaName << "\n";
				PrintVertex( parentIndex, ps[ 2 ], -ns[ 2 ], uvs[ 2 ] ) << " ";
				PrintWeights( wTotals[ 2 ], weightsPerVertex, nWeights[ 2 ], wIndices[ 2 ], wValues[ 2 ] ) << "\n";
				PrintVertex( parentIndex, ps[ 1 ], -ns[ 1 ], uvs[ 1 ] ) << " ";
				PrintWeights( wTotals[ 1 ], weightsPerVertex, nWeights[ 1 ], wIndices[ 1 ], wValues[ 1 ] ) << "\n";
				PrintVertex( parentIndex, ps[ 0 ], -ns[ 0 ], uvs[ 0 ] ) << " ";
				PrintWeights( wTotals[ 0 ], weightsPerVertex, nWeights[ 0 ], wIndices[ 0 ], wValues[ 0 ] ) << "\n";
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::OutputFaceSet(
	const MFnMesh &meshFn,
	const MMatrix &xformMat,
	bool doubleSided,
	const MObject &fileObj,
	const MObject &place2dTextureObj,
	bool /* transparent */,
	const CMeshData *pMeshData,
	int parentIndex,
	MObject &faceSetObj,
	const std::string &targaName )
{
	MIntArray meshVertexIndices;
	MPointArray tpList;
	MIntArray tvList;

	const MDagPath &cDagPath( pMeshData->DagPath() );
	MItMeshVertex cvIt( cDagPath );
	MItMeshPolygon cpIt( cDagPath );
	int pfIndex;

	const MDagPath &baseDagPath( pMeshData->BaseDagPath() );

	MPoint ps[ 3 ];
	MVector ns[ 3 ];
	float2 uvs[ 3 ];

	const bool warpUV( !( fileObj.isNull() || place2dTextureObj.isNull() ) );
	MPlug iuP;
	MPlug ivP;
	MPlug ouP;
	MPlug ovP;
	if ( warpUV )
	{
		MFnDependencyNode iuvFn( place2dTextureObj );
		MPlug iuvP( iuvFn.findPlug( "uvCoord" ) );
		iuP = iuvP.child( 0 );
		ivP = iuvP.child( 1 );

		MFnDependencyNode ouvFn( fileObj );
		MPlug ouvP( ouvFn.findPlug( "uvCoord" ) );
		ouP = ouvP.child( 0 );
		ovP = ouvP.child( 1 );
	}

	assert( cpIt.count() == MItMeshPolygon( baseDagPath ).count() );

	for ( MItMeshPolygon bpIt( baseDagPath, faceSetObj ); !bpIt.isDone(); bpIt.next() )
	{
		if ( !bpIt.hasValidTriangulation() )
			continue;	// ???

		cpIt.setIndex( bpIt.index(), pfIndex );

		bpIt.getVertices( meshVertexIndices );
		const uint nVerts( meshVertexIndices.length() );

		int nTri( 0 );
		bpIt.numTriangles( nTri );

		for ( int ti( 0 ); ti < nTri; ++ti )
		{
			bpIt.getTriangle( ti, tpList, tvList );

			for ( uint tvi( 0 ); tvi != 3; ++tvi )
			{ 
				ps[ tvi ] = tpList[ tvi ] * xformMat;

				const int meshVertexIndex( tvList[ tvi ] );

				float2 &uv( uvs[ tvi ] );
				MVector &n( ns[ tvi ] );
				uv[0] = 0.0f;
				uv[1] = 0.0f;

				for ( uint faceVertexIndex( 0 ); faceVertexIndex != nVerts; ++faceVertexIndex )
				{
					if ( meshVertexIndex == meshVertexIndices[ faceVertexIndex ] )
					{
						GetNormal( meshFn, cvIt, meshVertexIndex, cpIt, faceVertexIndex, n );
						n = n.transformAsNormal( xformMat ).normal();

						bpIt.getUV( faceVertexIndex, uv );
						if ( warpUV )
						{
							iuP.setValue( uv[ 0 ] );
							ivP.setValue( uv[ 1 ] );
							ouP.getValue( uv[ 0 ] );
							ovP.getValue( uv[ 1 ] );
						}
						break;
					}
				}

			}

			m_os << targaName << "\n";
			PrintVertex( parentIndex, ps[ 0 ], ns[ 0 ], uvs[ 0 ] ) << "\n";
			PrintVertex( parentIndex, ps[ 1 ], ns[ 1 ], uvs[ 1 ] ) << "\n";
			PrintVertex( parentIndex, ps[ 2 ], ns[ 2 ], uvs[ 2 ] ) << "\n";

			if ( doubleSided )
			{
				m_os << targaName << "\n";
				PrintVertex( parentIndex, ps[ 2 ], -ns[ 2 ], uvs[ 2 ] ) << "\n";
				PrintVertex( parentIndex, ps[ 1 ], -ns[ 1 ], uvs[ 1 ] ) << "\n";
				PrintVertex( parentIndex, ps[ 0 ], -ns[ 0 ], uvs[ 0 ] ) << "\n";
			}
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::ostream &CSmdExport::PrintVertex(
	int parentIndex,
	const MPoint &p,
	const MVector &n,
	const float2 &uv )
{

	if ( m_optPrettyPrint )
	{
		m_os << std::setw( 3 ) << parentIndex << " " <<
			PrettyPrint( p.x ) << " " <<
			PrettyPrint( p.y ) << " " <<
			PrettyPrint( p.z ) << " " <<
			PrettyPrint( n.x ) << " " <<
			PrettyPrint( n.y ) << " " <<
			PrettyPrint( n.z ) << " " <<
			PrettyPrint( uv[ 0 ] ) << " " <<
			PrettyPrint( uv[ 1 ] );
	}
	else
	{
		m_os << parentIndex << " " <<
			p.x << " " << p.y << " " << p.z << " " <<
			n.x << " " << n.y << " " << n.z << " " <<
			uv[ 0 ] << " " << uv[ 1 ];
	}

	return m_os;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::ostream &CSmdExport::PrintWeights(
	float wTotal,
	size_t weightsPerVertex,
	int nWeights,
	const int3 &wIndex,
	const float3 &wValue )
{
	if ( wTotal != 0.0f )
	{
		if ( m_optPrettyPrint )
		{
			m_os << " " << std::setw( 3 ) << nWeights;
		}
		else
		{
			m_os << " " << nWeights;
		}

		for ( size_t wi( 0 ); wi != weightsPerVertex; ++wi )
		{
			float wCurrent = wValue[ wi ];
			if ( wCurrent > 0.0f )
			{
				if ( m_optPrettyPrint )
				{
					m_os <<
						" " << std::setw( 3 ) << wIndex[ wi ] <<
						" " << PrettyPrint( wCurrent / wTotal );
				}
				else
				{
					m_os <<
						" " << wIndex[ wi ] <<
						" " << ( wCurrent / wTotal );
				}
			}
		}
	}

	return m_os;
}


//-----------------------------------------------------------------------------
// Gets the normal for the specified vertex and face
//-----------------------------------------------------------------------------
void CSmdExport::GetNormal(
	const MFnMesh &meshFn,
	MItMeshVertex &vIt,
	int meshVertexIndex,	// Mesh relative vertex index
	MItMeshPolygon &pIt,
	int faceVertexIndex,	// Face relative vertex index
	MVector &n ) const
{
	MVectorArray nArray;
	bool smooth( true );

	int pvIndex( 0 );
	vIt.setIndex( meshVertexIndex, pvIndex );

	// Phys Objects are always smooth normals
	if ( ( m_type & kPhysModel ) == 0 )
	{
		MIntArray edgeList;
		vIt.getConnectedEdges( edgeList );
		const uint nEdge( edgeList.length() );
		for ( uint ei( 0 ); ei != nEdge; ++ei )
		{
			if ( !meshFn.isEdgeSmooth( edgeList[ ei ] ) )
			{
				smooth = false;
				break;
			}
		}
	}

	if ( smooth )
	{
		vIt.getNormals( nArray );

		n = MVector::zero;

		const uint nEnd( nArray.length() );
		for ( uint ni( 0 ); ni != nEnd; ++ni )
		{
			n += nArray[ ni ];
		}
		n.normalize();
	}
	else
	{
		pIt.getNormal( faceVertexIndex, n );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint CSmdExport::PreprocessDag(
	const MDagPath &mDagPath,
	bool force )
{
	// Don't process things twice
	if ( m_nodeList.hasItem( mDagPath ) || m_meshList.hasItem( mDagPath ) )
		return 1;

	// Don't export invisible stuff, unless we're asked to
	if ( !force && !IsNodeVisible( mDagPath ) )
		return 0;

	uint retVal( 0 );

	if ( mDagPath.hasFn( MFn::kTransform ) )
	{
		if ( mDagPath.hasFn( MFn::kJoint ) )
		{
			retVal = 1;
			m_nodeList.add( mDagPath, MObject::kNullObj, true );
			const uint nLen( RemoveNameSpace( MFnDagNode( mDagPath ).name() ).length() );
			if ( nLen > m_maxNameLen )
			{
				m_maxNameLen = nLen;
			}
		}

		uint nProcess( 0 );
		const uint nChildren( mDagPath.childCount() );
		for ( uint ci( 0 ); ci != nChildren; ++ci )
		{
			MDagPath cDagPath( mDagPath );
			cDagPath.push( mDagPath.child( ci ) );
			nProcess += PreprocessDag( cDagPath, force );
		}

		if ( retVal == 0 && nProcess )
		{
			retVal = 1;
			m_nodeList.add( mDagPath, MObject::kNullObj, true );
			const uint nLen( RemoveNameSpace( MFnDagNode( mDagPath ).name() ).length() );
			if ( nLen > m_maxNameLen )
			{
				m_maxNameLen = nLen;
			}
		}
	}
	else if ( mDagPath.hasFn( MFn::kMesh ) )
	{
		retVal = 1;
		PreprocessMesh( mDagPath );
	}
	else if ( MFnDagNode( mDagPath ).typeName() == "vstAttachment" )
	{
		short tV( 1 );
		MFnDagNode( mDagPath ).findPlug( "type" ).getValue( tV );
		// Type is one of: 0 - Bone + Attachment
		//                 1 - Just Attachment
		//                 2 - Just Bone
		if ( tV == 0 || tV == 2 )
		{
			retVal = 1;
		}
		else
		{
			retVal = 0;
		}
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdExport::PreprocessMesh(
	const MDagPath &mDagPath )
{
	if ( !IsNodeVisible( mDagPath ) )
		return;

	MSelectionList historyList;
	GetHistory( mDagPath, historyList );

	MObject skinObj;
	MObject blendObj;
	MObject latticeObj;
	MObject dObj;
	MObjectArray inputObjs;

	for ( MItSelectionList sIt( historyList ); !sIt.isDone(); sIt.next() )
	{
		if ( sIt.itemType() != MItSelectionList::kDNselectionItem )
			continue;

		if ( !sIt.getDependNode( dObj ) )
			continue;

		if ( !dObj.hasFn( MFn::kGeometryFilt ) )
			continue;

		if ( dObj.hasFn( MFn::kSkinClusterFilter ) )
		{
			skinObj = dObj;
			PreprocessSkinCluster( dObj );
		}
		else if ( dObj.hasFn( MFn::kBlendShape ) )
		{
			blendObj = dObj;
		}
		else if ( dObj.hasFn( MFn::kFFD ) )
		{
			latticeObj = dObj;
		}
	}

	if ( m_type & ( kModel | kPhysModel | kVertexAnimation ) )
	{
		AddPreprocessedMesh( mDagPath, skinObj, blendObj, latticeObj );
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint CSmdExport::AddPreprocessedMesh(
	const MDagPath &mDagPath,
	const MObject &skinObj,
	const MObject &blendObj,
	const MObject &latticeObj )
{
	assert( !m_meshList.hasItem( mDagPath ) );

	MDagPath baseDagPath( mDagPath );

	if ( latticeObj.isNull() )
	{
		const MObject &dObj( !blendObj.isNull() ? blendObj : skinObj );
		if ( !dObj.isNull() )
		{
			MObjectArray inputObjs;
			if ( MFnGeometryFilter( dObj ).getInputGeometry( inputObjs ) && inputObjs.length() )
			{
				MDagPath::getAPathTo( inputObjs[ 0 ], baseDagPath );
				if ( !( baseDagPath.isValid() && baseDagPath.length() ) )
				{
					baseDagPath = mDagPath;
				}
			}
			else
			{
				MGlobal::displayWarning( MString( "Cannot find the base geometry for deforming mesh " ) + mDagPath.partialPathName() );
			}
		}
	}

	m_meshList.add( mDagPath );
	m_meshDataList.push_back( new CMeshData( mDagPath, baseDagPath, skinObj, blendObj, OptBindCurrent() ) );

	return 1;
}


//-----------------------------------------------------------------------------
// Gets the input history for a given dag node via the mel command
//-----------------------------------------------------------------------------
void CSmdExport::PreprocessSkinCluster(
	const MObject &skinObj )
{
	MFnSkinCluster skinFn( skinObj );
	MDagPathArray mDagPathArray;

	MStatus mStatus;
	if ( skinFn.influenceObjects( mDagPathArray, &mStatus ) == 0 )
		return; // Can't get influence objects

	if ( !mStatus || mDagPathArray.length() == 0 )
		return; // Can't get influence objects

	const uint nDag( mDagPathArray.length() );
	for ( uint di( 0 ); di != nDag; ++di )
	{
		const MDagPath &mDagPath( mDagPathArray[ di ] );

		// Ignore geometric influence objects
		if ( mDagPath.hasFn( MFn::kShape ) )
			continue;

		// Make sure that any transform/joint this mesh is skinned to is exported
		PreprocessDag( mDagPath, true );
	}
}


//-----------------------------------------------------------------------------
// Gets the input history for a given dag node via the mel command
//-----------------------------------------------------------------------------
void CSmdExport::GetHistory(
	const MDagPath &mDagPath,
	MSelectionList &historyList )
{
	historyList.clear();

	MStringArray history;

	MGlobal::executeCommand( "listHistory -il 2 -pdo 1 \"" +
		mDagPath.fullPathName() + "\"", history );

	const uint nHistory( history.length() );
	for ( uint hi( 0 ); hi != nHistory; ++hi )
	{
		historyList.add( history[ hi ] );
	}
}


//-----------------------------------------------------------------------------
// Changes any \'s to /'s in place and then splits on /
//-----------------------------------------------------------------------------
uint CSmdExport::SplitPath(
	const MString &path,
	MStringArray &splitPath )
{
	char *pC( const_cast< char *>( path.asChar() ) );
	for ( const char *const pEnd( pC + path.length() ); pC != pEnd; ++pC )
	{
		if ( *pC == '\\' )
		{
			*pC = '/';
		}
	}

	splitPath.clear();
	path.split( '/', splitPath );

	return splitPath.length();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MString &CSmdExport::StripExtension(
	MString &path )
{
	const int ext( path.rindex( '.' ) - 1 );
	if ( ext >= 0 )
	{
		path = path.substring( 0, ext );
	}

	return path;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CSmdExport::PathFromProj(
	const MStringArray &p,
	uint pLen,
	const MStringArray &v,
	uint vLen,
	MString &path )
{
	if ( vLen && vLen > pLen )
	{
		for ( uint pi( 0 ); pi != vLen; ++pi )
		{
			const MString &vStr( v[ pi ] );
			const MString &pStr( p[ pi ] );
			if ( vStr != pStr && ( vStr != "game" || pStr != "content" ) )
				return false;
		}

		path = p[ vLen ];
		for ( uint pi( vLen + 1 ); pi != pLen; ++pi )
		{
			path = path + "/" + p[ pi ];
		}

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Figures out the targa name from the shading group
//-----------------------------------------------------------------------------
std::string CSmdExport::GetTargaName(
	std::string &targaPath,
	const MObject &shadingGroupObj,
	MObject &fileObj,
	MObject &place2dTextureObj,
	bool &transparent ) const
{
	targaPath.clear();

	MObject surfaceShaderObj( FindInputNode( shadingGroupObj, "surfaceShader" ) );
	if ( surfaceShaderObj.isNull() )
	{
		// Comments in the 'triangle' block seem to make SMD's parser barf :(
		//	return MString( "// Can't find surfaceShader node from shadingGroup\n//No input to " ) +
		//		MFnDependencyNode( shadingGroupObj ).name() + ".surfaceShader\ndebug/debugempty";
		return m_debugMaterial;
	}

	MObject tmpFileObj( FindInputNodeOfType( surfaceShaderObj, "color", "file" ) );
	if ( tmpFileObj.isNull() )
	{
		//	return MString( "// Can't find file texture node from surfaceShader\n// No input to " ) +
		//		MFnDependencyNode( surfaceShaderObj ).name() + ".color\ndebug/debugempty";
		return m_debugMaterial;
	}

	fileObj = tmpFileObj;

	MObject transObj( FindInputNodeOfType( surfaceShaderObj, "transparency", "file" ) );
	if ( fileObj == transObj )
	{
		transparent = true;
	}

	MObject tmpPlace2dTextureObj( FindInputNodeOfType( fileObj, "uvCoord", "place2dTexture" ) );
	if ( !tmpPlace2dTextureObj.isNull() )
	{
		MFnDependencyNode place2dTextureFn( tmpPlace2dTextureObj );

		MPlug uvCoordP( place2dTextureFn.findPlug( "uvCoord" ) );
		if ( !( uvCoordP.isNull() || uvCoordP.isConnected() || uvCoordP.isLocked() ) )
		{
			place2dTextureObj = tmpPlace2dTextureObj;
		}
	}

	MString fileTexture;
	int fLen( 0 );

	// There's a fileTexture node driving color
	const MFnDependencyNode fileFn( fileObj );
	const MPlug fileP( fileFn.findPlug( "fileTextureName" ) );
	fileP.getValue( fileTexture );
	fLen = fileTexture.length();
	if ( fLen == 0 )
	{
		//	return MString( "// No texture filename specified on file texture node\n// " ) +
		//		MFnDependencyNode( fileObj ).name() + ".fileTextureName is empty\ndebug/debugempty";
		return m_debugMaterial;
	}

	targaPath = fileTexture.asChar();

	MStringArray path;
	const uint pLen( SplitPath( StripExtension( fileTexture ), path ) );

	if ( pLen == 0 )
		return fileTexture.asChar();

	if ( m_version > 1 )
	{
		// Do relative texture name

		fileTexture.clear();

		MStringArray vproject;
		const uint vLen( SplitPath( MString( getenv( "VPROJECT" ) ), vproject ) );

		if ( PathFromProj( path, pLen, vproject, vLen, fileTexture ) )
			return fileTexture.asChar();

		fileTexture = path[ pLen - 1 ];
		for ( int pi( pLen - 2 ); pi >= 0; --pi )
		{
			if ( path[ pi ] == "materials" || path[ pi ] == "materialsrc" )
			{
				if ( fileTexture.length() )
					return fileTexture.asChar();
				else
					break;
			}

			fileTexture = path[ pi ] + "/" + fileTexture;
		}
	}

	return path[ pLen - 1 ].asChar();
}


//-----------------------------------------------------------------------------
// Finds a node in the history of a node by following the same named
// input until it gets to a node to the requested type or there's no
// more attributes to follow or the named attribute isn't connected
//-----------------------------------------------------------------------------
MObject CSmdExport::FindInputNodeOfType(
	const MObject &dstObj,
	MString dstPlugName,
	MString typeName )
{
	const MFnDependencyNode dstFn( dstObj );
	const MPlug dstPlug( dstFn.findPlug( dstPlugName ) );

	if ( dstPlug.isNull() )
		return MObject::kNullObj;

	MPlugArray mPlugArray;
	if ( !( dstPlug.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
		return  MObject::kNullObj;

	MPlug srcPlug( mPlugArray[ 0 ] );
	MObject srcObj( srcPlug.node() );
	MFnDependencyNode srcFn( srcObj );
	if ( srcFn.typeName() == typeName )
		return srcObj;

	return FindInputNodeOfType( srcObj, dstPlugName, typeName );
}


//-----------------------------------------------------------------------------
// Finds a node in the history of a node by following the named
// input
//-----------------------------------------------------------------------------
MObject CSmdExport::FindInputNode(
	const MObject &dstObj,
	MString dstPlugName )
{
	const MFnDependencyNode dstFn( dstObj );
	const MPlug dstPlug( dstFn.findPlug( dstPlugName ) );

	if ( dstPlug.isNull() )
		return MObject::kNullObj;

	MPlugArray mPlugArray;
	if ( !( dstPlug.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
		return  MObject::kNullObj;

	return mPlugArray[ 0 ].node();
}


//-----------------------------------------------------------------------------
// Prints a header on the SMD file
//-----------------------------------------------------------------------------
void CSmdExport::OutputHeader()
{
	MString melResult;
	MStringArray melArrayResult;

	MGlobal::executeCommand( "about -a", melResult );
	m_os << "// EDITOR=" << melResult.asChar() << " " << MGlobal::mayaVersion().asChar() << "\n";

	m_os << "// SCENE=" << MFileIO::currentFile().asChar() << "\n";

	MGlobal::executeCommand( "about -cd", melResult );
	m_os << "// DATE=" << melResult.asChar();

	MGlobal::executeCommand( "about -ct", melResult );
	m_os << " " << melResult.asChar() << "\n";

	char pTmpBuf[256];
	*pTmpBuf ='\0';
	DWORD dwSize( sizeof( pTmpBuf ) );

	GetComputerName( pTmpBuf, &dwSize);
	m_os << "// MACHINE=" << pTmpBuf << "\n";

	MGlobal::executeCommand( "strip( about( \"-osv\" ) )", melResult );
	m_os << "// OS=" << melResult.asChar() << "\n";

	const bool guiMode = MGlobal::mayaState() == MGlobal::kInteractive;

	if ( guiMode )
	{
		m_os << "// MAYAMODE=gui\n";
		MGlobal::executeCommand( "hardware -gfx", melArrayResult );
		m_os << "// GFX=" << melArrayResult[ 0 ].asChar() << "\n";
	}
	else
	{
		m_os << "// MAYAMODE=batch\n";
	}

	GetUserName( pTmpBuf, &dwSize);
	m_os << "// USERNAME=" << pTmpBuf << "\n";

	const char *pVProject = getenv( "VPROJECT" );
	if ( pVProject )
	{
		m_os << "// VPROJECT=" << pVProject << "\n";
	}

	m_os << "// UPAXIS=";

	if ( m_optUpAxis == kOptXUp )
	{
		m_os << "X";
	}
	else if ( m_optUpAxis == kOptYUp )
	{
		m_os << "Y";
	}
	else if ( m_optUpAxis == kOptZUp )
	{
		m_os << "Z";
	}
	else
	{
		m_os << "Unknown";
	}

	m_os << "\n";

	m_os << "// SMD Export Settings\n";

	m_os << "// TYPE=";
	if ( m_type & kModel )
	{
		m_os << "Model";
	}

	if ( m_type & kPhysModel )
	{
		m_os << "PhysicsModel";
	}

	if ( m_type & kSkeletalAnimation )
	{
		m_os << "Animation";
	}

	if ( m_type & kVertexAnimation )
	{
		m_os << "VertexAnimation";
	}

	m_os << "\n";

	if ( m_type & ( kSkeletalAnimation | kVertexAnimation ) )
	{
		m_os << "// FRAMERANGE=" <<
			MTime( m_fs, MTime::uiUnit() ) << "-" <<
			MTime( m_fe, MTime::uiUnit() ) << "x" <<
			MTime( m_fi, MTime::uiUnit() ) << "\n";

		m_os << "// FRAMERATE=" <<
			( 1.0 / MTime( 1.0, MTime::uiUnit() ).as( MTime::kSeconds ) ) <<
			" fps (Frames Per Second)\n";
	}

	m_os << "//\n\n";
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::ostream &CSmdExport::OutputTransform(
	int index,
	const MMatrix &m )
{
	if ( m_optPrettyPrint )
		return OutputPrettyTransform( index, m );

	return OutputPlainTransform( index, m );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::ostream &CSmdExport::OutputPrettyTransform(
	int index,
	const MMatrix &m )
{
	MEulerRotation r;
	r = m;
	const MVector &t( reinterpret_cast< const MVector & >( *( m[ 3 ] ) ) );

	m_os << std::setw( 3 ) << index << " " <<
		PrettyPrint( t.x ) << " " << PrettyPrint( t.y ) << " " << PrettyPrint( t.z ) << " " <<
		PrettyPrint( r.x ) << " " << PrettyPrint( r.y ) << " " << PrettyPrint( r.z );

	return m_os;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::ostream &CSmdExport::OutputPlainTransform(
	int index,
	const MMatrix &m )
{
	MEulerRotation r;
	r = m;
	const MVector &t( reinterpret_cast< const MVector & >( *( m[ 3 ] ) ) );

	m_os << std::setw( 3 ) << index << " " <<
		t.x << " " << t.y << " " << t.z << " " <<
		r.x << " " << r.y << " " << r.z;

	return m_os;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CSmdExport::PrettyPrint(
	const double &d )
{
	static int count( 0 );
	static char buf[ MAX_PATH ];

	*buf = '\0';
	_snprintf( buf, MAX_PATH - 1, "%11.6lf", d );
	buf[ sizeof( buf ) - 1 ] = '\0';

	if ( strchr( buf, '.' ) )
	{
		char *const pEnd( buf );
		for ( char *p( pEnd + strlen( buf ) - 1 ); p >= pEnd; --p )
		{
			if ( *p != '0' )
			{
				if ( *p == '.' )
				{
					*p = ' ';
					if ( ( p - pEnd ) >= 2 && *( p - 1 ) == '0' && *( p - 2 ) == '-' )
					{
						*( p - 2 ) = ' ';
						*( p - 1 ) = '0';
					}
				}
				break;
			}

			*p = ' ';
		}
	}

	return std::string( buf );
}