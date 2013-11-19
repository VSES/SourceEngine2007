//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CDmeExport
//
//=============================================================================

#pragma warning(disable : 4702)     // warning C4702: unreachable code (xtree)

// Standard includes
#include <map>
#include <string>


// Maya includes
#include <maya/MDagPathArray.h>
#include <maya/MFileIO.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnAttribute.h>
#include <maya/MFnBlendShapeDeformer.h>
#include <maya/MFnComponentListData.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnGeometryFilter.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMeshData.h>
#include <maya/MFnPointArrayData.h>
#include <maya/MFnSet.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnTransform.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPointArray.h>
#include <maya/MQuaternion.h>
#include <maya/MVector.h>


// Valve includes
#include "movieobjects/dmeattachment.h"
#include "movieobjects/dmedag.h"
#include "movieobjects/dmecombinationoperator.h"
#include "movieobjects/dmefaceset.h"
#include "movieobjects/dmejoint.h"
#include "movieobjects/dmedccmakefile.h"
#include "movieobjects/dmematerial.h"
#include "movieobjects/dmemesh.h"
#include "movieobjects/dmemodel.h"
#include "tier2/tier2.h"
#include "filesystem.h"
#include "valveMaya.h"


// Local includes
#include "DmeExport.h"


//-----------------------------------------------------------------------------
// Less function for use with CUtlMap and CUtlString keys
//-----------------------------------------------------------------------------
bool UtlStringLessFunc( const CUtlString &lhs, const CUtlString &rhs )
{
	return ( Q_strcmp( lhs.Get(), rhs.Get() ) < 0 );
}


//=============================================================================
// CNormalData class, used by CDmeExport.  Contains normal data for a mesh
// along with maps for accessing the data.  Organizes it into Vertex &
// FaceVarying.
//=============================================================================


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CNormalData::CNormalData()
: m_softVertexNormalMap( DefLessFunc( int ) )
{
}


//-----------------------------------------------------------------------------
// Create a map of vertex indices to soft normals
// normals will contain the value of all of the smooth normals
// softVertexNormalMap will map the vertex id to the soft normal map
// Later on, if a vertex id cannot be found in this map that means
// it is a hard vertex and should be added to the normal list at the
// point in which it is being added to the face set
//-----------------------------------------------------------------------------
void CNormalData::GetNormalData(
	const MDagPath &baseMeshPath,
	bool allShared )
{
	MapSoftVertexNormals( baseMeshPath, allShared );

	m_faceIndexToFaceVaryingIndexMap.RemoveAll();
	m_faceVaryingIndexToDmeNormalIndexMap.RemoveAll();

	MVector mNormal;
	Vector dNormal;

	for ( MItMeshPolygon pIt( baseMeshPath ); !pIt.isDone(); pIt.next() )
	{
		m_faceIndexToFaceVaryingIndexMap.AddToTail( m_faceVaryingIndexToDmeNormalIndexMap.Count() );

		const int nPv( pIt.polygonVertexCount() );
		for ( int i( 0 ); i < nPv; ++i )
		{
			const int vIndex( pIt.vertexIndex( i ) );
			const CUtlMap< int, int >::IndexType_t sIndex( m_softVertexNormalMap.Find( vIndex ) );
			if ( m_softVertexNormalMap.IsValidIndex( sIndex ) )
			{
				m_faceVaryingIndexToDmeNormalIndexMap.AddToTail( m_softVertexNormalMap.Element( sIndex ) );
			}
			else
			{
				pIt.getNormal( i, mNormal );
				mNormal.normalize();	// Probably not necessary
				dNormal.x = mNormal.x;
				dNormal.y = mNormal.y;
				dNormal.z = mNormal.z;
				m_faceVaryingIndexToDmeNormalIndexMap.AddToTail( m_normals.AddToTail( dNormal ) );
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Given an existing CNormalData, get a new set of normal data from a matching
// MFnMesh with different vertex data but the same topology into a new
// CUtlVector
//-----------------------------------------------------------------------------
void CNormalData::GetNormalData(
	MObject &meshObj,
	CUtlVector< Vector > &normals ) const
{
	normals.RemoveAll();

	MVector mNormal;
	Vector dNormal;

	MVectorArray nA;
	for ( MItMeshVertex vIt( meshObj ); !vIt.isDone(); vIt.next() )
	{
		if ( m_softVertexNormalMap.IsValidIndex( m_softVertexNormalMap.Find( vIt.index() ) ) )
		{
			vIt.getNormals( nA );

			mNormal = MVector::zero;
			const uint nEnd( nA.length() );
			for ( uint ni( 0 ); ni != nEnd; ++ni )
			{
				mNormal += nA[ ni ];
			}
			mNormal.normalize();

			dNormal.x = mNormal.x;
			dNormal.y = mNormal.y;
			dNormal.z = mNormal.z;
			normals.AddToTail( dNormal );
		}
	}

	for ( MItMeshPolygon pIt( meshObj ); !pIt.isDone(); pIt.next() )
	{
		const int nPv( pIt.polygonVertexCount() );
		for ( int i( 0 ); i < nPv; ++i )
		{
			const int vIndex( pIt.vertexIndex( i ) );
			if ( !m_softVertexNormalMap.IsValidIndex( m_softVertexNormalMap.Find( vIndex ) ) )
			{
				pIt.getNormal( i, mNormal );
				mNormal.normalize();	// Probably not necessary
				dNormal.x = mNormal.x;
				dNormal.y = mNormal.y;
				dNormal.z = mNormal.z;
				normals.AddToTail( dNormal );
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Returns the index into the m_normals array for the give face and face
// relative vertex indices
//-----------------------------------------------------------------------------
int CNormalData::NormalDataIndex(
	int faceIndex,
	int faceVertexIndex ) const
{
	return m_faceVaryingIndexToDmeNormalIndexMap[ m_faceIndexToFaceVaryingIndexMap[ faceIndex ] + faceVertexIndex ];
}


//-----------------------------------------------------------------------------
// Adds the normal data in this class to the passed CDmeVertexData
//-----------------------------------------------------------------------------
int CNormalData::AddNormalData(
	CDmeVertexData *pDmeVertexData ) const
{
	const FieldIndex_t normalIndex( pDmeVertexData->CreateField( CDmeVertexDataBase::FIELD_NORMAL ) );
	pDmeVertexData->AddVertexData( normalIndex, m_normals.Count() );
	pDmeVertexData->SetVertexData( normalIndex, 0, m_normals.Count(), AT_VECTOR3, const_cast< Vector * >( m_normals.Base() ) );

	return normalIndex;
}


//-----------------------------------------------------------------------------
// Creates a map of vertex data indices to soft normals
// Clears out the normal data list (m_normals) and so all of the soft normals
// will be first in the data list
//-----------------------------------------------------------------------------
void CNormalData::MapSoftVertexNormals(
	const MDagPath &baseMeshPath,
	bool allShared )
{
	m_normals.RemoveAll();
	m_softVertexNormalMap.RemoveAll();

	const MFnMesh meshFn( baseMeshPath );

	MIntArray edgeList;
	uint eEnd;
	uint ei;
	bool smooth;
	MVector mNormal;
	Vector dNormal;
	MVectorArray nA;

	for ( MItMeshVertex vIt( baseMeshPath ); !vIt.isDone(); vIt.next() )
	{
		smooth = true;

		if ( !allShared )
		{
			vIt.getConnectedEdges( edgeList );
			for ( eEnd = edgeList.length(), ei = 0; ei != eEnd; ++ei )
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
			vIt.getNormals( nA );
			mNormal = MVector::zero;
			const uint nEnd( nA.length() );
			for ( uint ni( 0 ); ni != nEnd; ++ni )
			{
				mNormal += nA[ ni ];
			}
			mNormal.normalize();

			dNormal.x = mNormal.x;
			dNormal.y = mNormal.y;
			dNormal.z = mNormal.z;
			m_softVertexNormalMap.InsertOrReplace( vIt.index(), m_normals.AddToTail( dNormal ) );
		}
	}
}


//=============================================================================
// Class which stores all data and functions needed to convert a Maya scene
// to Dme
//=============================================================================


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CDmeExport::CDmeExport(
	uint type,
	const MString &pathname,
	const MString &dmObjectIdStr )
: m_type( type )
, m_ownDmeData( true )
, m_mayaToDmeDag( UtlStringLessFunc )
, m_exportInvisible( false )
, m_scratchVmt( false )
, m_optMergeUvs( true )
, m_optTriangulateBad( false )
, m_optWeightMap( false )
, m_optBindCurrent( false )
{
	m_dmFileId = g_pDataModel->FindOrCreateFileId( pathname.asChar() );
	m_pRoot = CreateElement< CDmElement >( "root", FileId() );

	GetRoot()->AddAttributeElement< CDmElement >( "vsDmxIO_exportTags" );

	DmObjectId_t dmObjectId;
	DmObjectId_t *pObjectId = NULL;
	if ( dmObjectIdStr.length() )
	{
		if ( UniqueIdFromString( &dmObjectId, dmObjectIdStr.asChar(), dmObjectIdStr.length() ) )
		{
			pObjectId = &dmObjectId;
		}
	}

	if ( type == CDmeExport::kSkeletalAnimation )
	{
		m_pMayaMakefile = CreateElement< CDmeMayaAnimationMakefile >( "vsDmxIO", FileId(), pObjectId );
		m_pMayaMakefileSource = m_pMayaMakefile->AddSource< CDmeSourceMayaAnimationFile >( MFileIO::currentFile().asChar() );
	}
	else
	{
		m_pMayaMakefile = CreateElement< CDmeMayaModelMakefile >( "vsDmxIO", FileId(), pObjectId );
		m_pMayaMakefileSource = m_pMayaMakefile->AddSource< CDmeSourceMayaModelFile >( MFileIO::currentFile().asChar() );
	}

	GetRoot()->SetValue( "makefile", m_pMayaMakefile );
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CDmeExport::~CDmeExport()
{
	if ( m_ownDmeData )
	{
		g_pDataModel->RemoveFileId( FileId() );
	}
}


//-----------------------------------------------------------------------------
// Someone else is going to clear the Dme data!
//-----------------------------------------------------------------------------
DmFileId_t CDmeExport::Adopt()
{
	m_ownDmeData = false;
	return FileId();
}


//-----------------------------------------------------------------------------
// Find a cached CDmeDag which corresponds to a Maya DagPath
//-----------------------------------------------------------------------------
CDmeDag *CDmeExport::FindDmeDag(
	const MDagPath &mDagPath ) const
{
	Index_t mayaToDmeIndex(
		m_mayaToDmeDag.Find( CUtlString( mDagPath.fullPathName().asChar() ) ) );

	if ( !m_mayaToDmeDag.IsValidIndex( mayaToDmeIndex ) )
		return NULL;

	return m_mayaToDmeDag[ mayaToDmeIndex ];
}


//-----------------------------------------------------------------------------
// Converts attributes on the node to Dme Attributes
//-----------------------------------------------------------------------------
void CDmeExport::MayaAttrToDmeAttr(
	const MPlug &mPlug,
	CDmElement *pDmeElement )
{
	if ( mPlug.isNull() )
	{
		const MFnAttribute aFn( mPlug.node() );
	}
}


//-----------------------------------------------------------------------------
// Converts attributes on the node to Dme Attributes
//-----------------------------------------------------------------------------
void CDmeExport::MayaAttrsToDme(
	MDagPath &mDagPath,
	CDmElement *pDmeElement )
{
	if ( mDagPath.hasFn( MFn::kJoint ) )
	{
		const MFnIkJoint jFn( mDagPath );
		double d[ 3 ];

		jFn.getPreferedAngle( d );
		if ( d[ 0 ] != 0.0 || d[ 1 ] != 0.0 || d[ 2 ] != 0.0 )
		{
			pDmeElement->SetValue( "maya_preferredAngle", Vector( d[ 0 ], d[ 1 ], d[ 2 ] ) );
		}

		jFn.getStiffness( d );
		if ( d[ 0 ] != 0.0 || d[ 1 ] != 0.0 || d[ 2 ] != 0.0 )
		{
			pDmeElement->SetValue( "maya_stiffness", Vector( d[ 0 ], d[ 1 ], d[ 2 ] ) );
		}
	}
}


//-----------------------------------------------------------------------------
// Makes a CDmeModel and adds it to the appropriate places
//-----------------------------------------------------------------------------
CDmeModel *CDmeExport::CreateDmeModel(
	const char *pDmeModelName,
	const char *pDmeModelTransformName,
	CDmeDag *pDmeDag )
{
	CDmeModel *pDmeModel( CreateElement< CDmeModel >( pDmeModelName , FileId() ) );

	if ( pDmeModel )
	{
		m_dmeModelStack.Push( pDmeModel );
		pDmeModel->GetTransform()->SetName( pDmeModelTransformName );
	}
	else
	{
		merr << "Couldn't create DmeModel named \"" << pDmeModelName << "\"" << std::endl;
	}

	return pDmeModel;
}


class CScopePopModel
{
public:
	CScopePopModel(
		CUtlStack< CDmeModel * > &dmeModelStack )
	: m_dmeModelStack( dmeModelStack )
	, m_pop ( false )
	{
	}

	~CScopePopModel()
	{
		if ( m_pop )
		{
			if ( m_dmeModelStack.Count() )
			{
				m_dmeModelStack.Pop();
			}
			else
			{
				merr << "Cannot pop empty DmeModel stack!!!" << std::endl;
			}
		}
	}

	CUtlStack< CDmeModel * > &m_dmeModelStack;
	bool m_pop;
};


//-----------------------------------------------------------------------------
// Purpose: Recursively convert the Maya Dag node into it's DME equivalent
// Input  : mDagPath		The Maya Dag Path to export
//			pDmeDagParent	The CDmeDag that will be the parent of the newly
//							created node, can be NULL
//			ppDmeDagRet		If supplied, it will contain the DmeDag node
//							that was created
//			force			Ignore checks like visibility, etc...
//-----------------------------------------------------------------------------
uint CDmeExport::ConvertDag(
	MDagPath &mDagPath,
	CDmeDag *pDmeDagParent,
	CDmeDag **ppDmeDagRet,
	bool force )
{
	uint retVal( 0 );

	if ( !IsDagOkToConvert( mDagPath, force, retVal ) )
		return retVal;

	if ( mDagPath.hasFn( MFn::kTransform ) )
		return ConvertTransform( mDagPath, pDmeDagParent, ppDmeDagRet, force );

	if ( mDagPath.hasFn( MFn::kShape ) )
		return ConvertShape( mDagPath, pDmeDagParent, ppDmeDagRet );

//	mwarn << "Transform apiType " << mDagPath.node().apiTypeStr()
//		<< " on " << mDagPath.partialPathName() << " is not handled, ignoring" << std::endl;

	return 0;
}


//-----------------------------------------------------------------------------
// Determines whether the passed maya dag node is one which should be converted
//-----------------------------------------------------------------------------
bool CDmeExport::IsDagOkToConvert(
	MDagPath &mDagPath,
	bool force,
	uint &retVal ) const
{
	// Discard known bad types
	switch ( mDagPath.apiType() )
	{
	case MFn::kManipulator3D:
		return false;
	default:	// Hushes compiler complaints re: cases not handled switching enum
		;
	}

	// Discard invisible if requested - Path visibility check can be slow....
	if ( MFnDagNode( mDagPath).isIntermediateObject() || !( force || m_exportInvisible || ValveMaya::IsNodeVisible( mDagPath, false ) ) )
		return false;

	// Don't do things twice
	if ( m_exportedList.hasItem( mDagPath ) )
	{
		mwarn << "Already processed " << mDagPath.fullPathName() << std::endl;
		retVal = 1;
		return false;
	}

	// No reason not to!
	return true;
}


//-----------------------------------------------------------------------------
// Converts a Maya transform type node into Dme
//-----------------------------------------------------------------------------
uint CDmeExport::ConvertTransform(
	const MDagPath &mDagPath,
	CDmeDag *pDmeDagParent,
	CDmeDag **ppDmeDagRet,
	bool force )
{
	CScopePopModel scopePopModel( m_dmeModelStack );

	uint retVal( 0 );


	const MFnTransform transformFn( mDagPath );
	const MFn::Type apiType( mDagPath.apiType() );
	const MString typeName( transformFn.typeName() );

	CDmeDag *pDmeDag( NULL );
	if ( apiType == MFn::kPluginTransformNode && typeName == "vstInfo" )
	{
		const MString nodeName( GetNodeName( mDagPath ) );
		pDmeDag = CreateDmeModel( nodeName.asChar(), nodeName.asChar() );
		scopePopModel.m_pop = true;
	}
	else  if ( mDagPath.hasFn( MFn::kJoint ) )
	{
		pDmeDag = CreateElement< CDmeJoint >(
			GetNodeName( mDagPath ).asChar(), FileId() );
	}
	else if ( mDagPath.hasFn( MFn::kShape ) )
	{
		pDmeDag = CreateElement< CDmeDag >(
			GetNodeName( mDagPath ).asChar(), FileId() );
	}
	else {
		pDmeDag = CreateElement< CDmeDag >(
			GetNodeName( mDagPath ).asChar(), FileId() );
	}

	if ( !pDmeDag )
	{
		merr << "Couldn't create Dme element for Maya node \"" <<
			mDagPath.partialPathName() << "\"" << std::endl;
		return retVal;
	}

	// Even though the transform might be thrown away we need the data
	// because of vstAttachment fiddling with it and then getting overwritten
	if ( apiType != MFn::kPluginTransformNode || typeName != "vstInfo" )
	{
		ConvertTransformData( mDagPath, apiType, pDmeDag );
	}

	retVal = ConvertTransformChildren( mDagPath, pDmeDag, ppDmeDagRet, force );
	if ( retVal == 0 )
		return retVal;	// I had no interesting children and I was of no interest

	// If we got this far, we either had some interesting children or we're a lone transform
	// so get all the transform information in local space

	m_mayaToDmeDag.Insert( CUtlString( mDagPath.fullPathName().asChar() ), pDmeDag );

	if ( pDmeDagParent )
	{
		// Only add it if it's not already a child...
		if ( pDmeDagParent->FindChild( pDmeDag ) < 0 )
		{
			pDmeDagParent->AddChild( pDmeDag );
		}
	}

	m_exportedList.add( mDagPath );

	if ( ppDmeDagRet )
	{
		*ppDmeDagRet = pDmeDag;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// Converts the data on a Maya transform into Dme
//-----------------------------------------------------------------------------
void CDmeExport::ConvertTransformData(
	const MDagPath &mDagPath,
	MFn::Type apiType,
	CDmeDag *pDmeDag )
{
	CDmeTransform *pDmeTransform( pDmeDag->GetTransform() );
	pDmeTransform->SetName( pDmeDag->GetName() );

	if ( apiType == MFn::kJoint )
	{
		const MFnIkJoint jointFn( mDagPath );

		MQuaternion r;
		r = jointFn.transformationMatrix();

		const MVector t( jointFn.getTranslation( MSpace::kTransform ) );

		pDmeTransform->SetOrientation( Quaternion( r.x, r.y, r.z, r.w ) );
		pDmeTransform->SetPosition( Vector( t.x, t.y, t.z ) );

		bool saveBind( IsInfluenceObject( mDagPath ) );
		if ( mDagPath.hasFn( MFn::kShape ) )
		{
			MDagPath sDagPath( mDagPath );
			sDagPath.extendToShapeDirectlyBelow( 0 );
			const MFnDagNode sDagFn( sDagPath );
			if ( sDagFn.typeName() != "vstHelperBone" )
			{
				saveBind = false;
			}
		}

		if ( saveBind )
		{
			SaveBindPose( jointFn, pDmeDag, pDmeTransform );
		}
	}
	else
	{
		const MFnTransform transformFn( mDagPath );

		MQuaternion q;
		transformFn.getRotation( q, MSpace::kTransform );
		pDmeTransform->SetOrientation( Quaternion( q.x, q.y, q.z, q.w ) );

		const MVector t( transformFn.getTranslation( MSpace::kTransform ) );
		pDmeTransform->SetPosition( Vector( t.x, t.y, t.z ) );

		if ( !mDagPath.hasFn( MFn::kShape ) && IsInfluenceObject( mDagPath ) )
		{
			SaveBindPose( transformFn, pDmeDag, pDmeTransform );
		}
	}
}


//-----------------------------------------------------------------------------
// Converts all of the children of a Maya transform type node
//-----------------------------------------------------------------------------
uint CDmeExport::ConvertTransformChildren(
	const MDagPath &mDagPath,
	CDmeDag *pDmeDag,
	CDmeDag **ppDmeDagRet,
	bool force )
{
	uint retVal( 1 );

	unsigned cEnd( mDagPath.childCount() );
	for ( unsigned ci( 0 ); ci != cEnd; ++ci )
	{
		MDagPath cDagPath( mDagPath );
		cDagPath.push( mDagPath.child( ci ) );
		retVal += ConvertDag( cDagPath, pDmeDag, ppDmeDagRet, force );
	}

	// If Maya says we have children, see if any of them were terribly interesting to Dme
	// Unless force was on
	if ( !force && cEnd && retVal == 1 )
	{
		// Don't keep this node because it's not useful for Dme purposes
		g_pDataModel->DestroyElement( pDmeDag->GetHandle() );
		retVal = 0;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// Find any valid deformers driving the mesh
//-----------------------------------------------------------------------------
uint CDmeExport::ConvertShape(
	const MDagPath &mDagPath,
	CDmeDag *pDmeDagParent,
	CDmeDag **ppDmeDagRet )
{
	switch ( mDagPath.apiType() )
	{
	case MFn::kMesh:
		return ConvertMesh( mDagPath, pDmeDagParent, ppDmeDagRet );
	case MFn::kPluginLocatorNode:
		{
			const MFnDagNode dagFn( mDagPath );
			const MString nodeType( dagFn.typeName() );
			if ( nodeType == "vstAttachment" )
				return ConvertAttachment( mDagPath, dagFn, pDmeDagParent );
		}
		break;
	case MFn::kPluginShape:
		{
			const MFnDagNode dagFn( mDagPath );
			const MString nodeType( dagFn.typeName() );
			if ( nodeType == "vstHelperBone" )
			{
				// TODO: Create vstHelperBone DMX element
				return 1;
			}
		}
		break;
	default:
//		mwarn << "Unhandled shape apiType " << mDagPath.node().apiTypeStr() <<
//			" on " << mDagPath.partialPathName() << ", ignoring" << std::endl;
		break;
	}

	return 0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint CDmeExport::ConvertAttachment(
	const MDagPath &mDagPath,
	const MFnDagNode &dagFn,
	CDmeDag *pDmeDagParent ) const
{
	MPlug tP( dagFn.findPlug( "type" ) );
	short tV( 1 );
	if ( !tP.isNull() )
	{
		tP.getValue( tV );
	}

	// Type is one of: 0 - Bone + Attachment
	//                 1 - Just Attachment
	//                 2 - Just Bone

	if ( tV == 2 )
		return 1;	// Tell things there was an interesting child so transform isn't discarded by Dmx but don't create DmeAttachment node

	CDmeAttachment *pDmeAttachment(
		CreateElement< CDmeAttachment >(
		GetNodeName( mDagPath ).asChar(),
		FileId() ) );

	pDmeAttachment->m_bIsRigid.Set( tV == 0 ? false : true );

	MPlug waP( dagFn.findPlug( "worldAligned" ) );
	bool waV( false );
	if ( !waP.isNull() )
	{
		waP.getValue( waV );
	}
	pDmeAttachment->m_bIsWorldAligned.Set( waV );
	pDmeDagParent->SetShape( pDmeAttachment );

	MPlug lpP( dagFn.findPlug( "localPosition" ) );
	MVector lp;
	lpP.child( 0 ).getValue( lp.x );
	lpP.child( 1 ).getValue( lp.y );
	lpP.child( 2 ).getValue( lp.z );

	if ( !lp.isEquivalent( MVector::zero ) )
	{
		CDmeTransform *pTransform( pDmeDagParent->GetTransform() );

		Vector v0( lp.x, lp.y, lp.z );
		Vector v1;
		VectorRotate( v0, pTransform->GetOrientation(), v1 );
		VectorAdd( v1, pTransform->GetPosition(), v0 );
		pTransform->SetPosition( v0 );
	}

	return 1;
}


//-----------------------------------------------------------------------------
// Find any valid deformers driving the mesh
//-----------------------------------------------------------------------------
void
CDmeExport::HandleDeformers(
	const MDagPath &mDagPath,
	MDagPath &baseDagPath,
	MObject &skinObj,
	MObject &blendObj ) const
{
	MSelectionList historyList;

	{
		MStringArray history;
		MGlobal::executeCommand( "listHistory -il 2 -pdo 1 \"" + mDagPath.fullPathName() + "\"", history );
		const uint hEnd( history.length() );
		for ( uint hi( 0 ); hi != hEnd; ++hi )
		{
			historyList.add( history[ hi ] );
		}
	}

	{
		MObject dObj;
		MObjectArray inputObjs;

		for ( MItSelectionList sIt( historyList ); !sIt.isDone(); sIt.next() )
		{
			if ( sIt.itemType() == MItSelectionList::kDNselectionItem )
			{
				if ( sIt.getDependNode( dObj ) && dObj.hasFn( MFn::kGeometryFilt ) )
				{
					if ( dObj.hasFn( MFn::kSkinClusterFilter ) )
					{
						if ( !blendObj.isNull() )
						{
							mwarn << mDagPath.partialPathName() <<
								" has a blendShape node in it's history (" <<
								MFnDependencyNode( blendObj ).name() <<
								"), before a skinning node (" <<
								MFnDependencyNode( dObj ).name() << ").  "
								"This is unsupported in Dme so the data flow will be reversed " <<
								"on export and the data will flow from skin -> blend rather than blend -> skin" << std::endl;
						}

						if ( !skinObj.isNull() )
						{
							mwarn << mDagPath.partialPathName() <<
								" has multiple skinning nodes in it's history (" <<
								MFnDependencyNode( skinObj ).name() << ", " << MFnDependencyNode( dObj ).name() <<
								"), using the nearest ancestor " << MFnDependencyNode( dObj ).name() << std::endl;
						}

						skinObj = dObj;

						if ( baseDagPath.length() == 0 )
						{
							if ( MFnGeometryFilter( skinObj ).getInputGeometry( inputObjs ) && inputObjs.length() )
							{
								MDagPath::getAPathTo( inputObjs[ 0 ], baseDagPath );
							}
						}
					}
					else if ( ( m_type & kPhysicsModel ) == 0 && dObj.hasFn( MFn::kBlendShape ) )
					{
						if ( !blendObj.isNull() )
						{
							mwarn << mDagPath.partialPathName() <<
								" has multiple blendShape nodes in it's history (" <<
								MFnDependencyNode( blendObj ).name() << ", " << MFnDependencyNode( dObj ).name() <<
								"), using the nearest ancestor " << MFnDependencyNode( dObj ).name() << std::endl;
						}

						blendObj = dObj;

						if ( baseDagPath.length() == 0 )
						{
							if ( MFnGeometryFilter( blendObj ).getInputGeometry( inputObjs ) && inputObjs.length() )
							{
								MDagPath::getAPathTo( inputObjs[ 0 ], baseDagPath );
							}
						}
					}
					else if ( dObj.hasFn( MFn::kTweak ) )
					{
						// Ignore for now but should probably check to see if they are empty before ignoring
					}
					else
					{
						mwarn << mDagPath.partialPathName() <<
							" has an unsupported node (" <<
							MFnDependencyNode( dObj ).name() <<
							" in its history of type (" <<
							dObj.apiTypeStr() << ") which is unsupported in Dme.  " <<
							"This node's effects will be ignored which may result in the exported data differing from Maya" << std::endl;
					}
				}
			}
		}
	}

	if ( m_optBindCurrent || baseDagPath.length() == 0 )
	{
		// No blendshape or skin history could be found
		// or the user said to...
		// Just use the current state
		baseDagPath = mDagPath;
	}
}


//-----------------------------------------------------------------------------
// Gets a blendShape delta from Maya from full geometry
//-----------------------------------------------------------------------------
void GetDeltaVertexValuesFromGeometry(
	CDmeMesh *pDmeMesh,
	const MObject &deltaMeshObj,
	CUtlVector< int > &vIndices,
	CUtlVector< Vector > &vDeltas )
{
	const MFnMesh ipgFn( deltaMeshObj );
	MFloatPointArray mp;
	ipgFn.getPoints( mp );
	const CDmeVertexData *pBindData( pDmeMesh->FindBaseState( "bind" ) );
	const CUtlVector< Vector > &bp( pBindData->GetPositionData() );

	vIndices.RemoveAll();

	// Sanity check...
	if ( bp.Count() >= static_cast< int >( mp.length() ) )
	{
		Vector d;
		const Vector *pBind( bp.Base() );
		const MFloatPoint *pMaya( &mp[ 0 ] );
		int vi( 0 );
		for ( const MFloatPoint *const pMayaEnd( pMaya + mp.length() ); pMaya != pMayaEnd; ++pMaya, ++pBind, ++vi )
		{
			d.x = pMaya->x - pBind->x;
			d.y = pMaya->y - pBind->y;
			d.z = pMaya->z - pBind->z;

			// Kind of a magic number but it's because of 16 bit compression of the delta values
			if ( fabs( d.x ) >= ( 1 / 4096.0f ) || fabs( d.y ) >= ( 1.0f / 4096.0f ) || fabs( d.z ) >= ( 1.0f / 4096.0f ) )
			{
				vIndices.AddToTail( vi );
				vDeltas.AddToTail( d );
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Gets the Maya blendShape vertexIndices for the given blendShape target
//-----------------------------------------------------------------------------
void GetDeltaVertexIndices(
	const MPlug &inputTargetItemP,	// inputTargetItem Plug
	CUtlVector< int > &vIndices )	// Vertex indices
{
	// Make sure this is empty
	vIndices.RemoveAll();

	// First see if the inputTarget has any components...

	// .inputComponentsTarget Plug
	const MPlug ictP( inputTargetItemP.child( 2 ) );
	// .inputComponentTargets Value
	MObject ictObj;

	ictP.getValue( ictObj );
	if ( ictObj.isNull() )
		return;

	MIntArray components;
	const MFnComponentListData ictFn( ictObj );
	const uint ictEnd( ictFn.length() );

	for ( uint icti( 0 ); icti != ictEnd; ++icti )
	{
		MFnSingleIndexedComponent( ictFn[ icti ] ).getElements( components );
		const uint cEnd( components.length() );
		vIndices.EnsureCapacity( vIndices.Count() + cEnd );
		for ( uint ci( 0 ); ci != cEnd; ++ci )
		{
			vIndices.AddToTail( components[ ci ] );
		}
	}
}


//-----------------------------------------------------------------------------
// Gets a blendShape delta from Maya from the Maya computed delta on the node
//-----------------------------------------------------------------------------
void GetDeltaVertexValuesFromDelta(
	CDmeMesh *pDmeMesh,
	const MString &deltaName,
	const MPlug &inputTargetItemP,
	CUtlVector< int > &vIndices,
	CUtlVector< Vector > &vDeltas )
{
	// Delta comes from the delta state
	GetDeltaVertexIndices( inputTargetItemP, vIndices );

	if ( vIndices.Count() == 0 )
	{
		return;
	}

	// .inputPointsTarget
	MPlug iptP( inputTargetItemP.child( 1 ) );

	MObject iptObj;
	iptP.getValue( iptObj );
	if ( iptObj.isNull() )
	{
		vIndices.RemoveAll();
		return;
	}

	const MFnPointArrayData iptFn( iptObj );
	const int iptEnd( static_cast< int >( iptFn.length() ) );

	if ( iptEnd == 0 )
	{
		vIndices.RemoveAll();
		return;
	}

	// minimum of the number of points and the number of components...
	vDeltas.EnsureCount( min( iptEnd, vIndices.Count() ) );

	// Copy the data from Maya to Dme
	Vector *pDst( vDeltas.Base() );
	const MPoint *pSrc( &iptFn[ 0 ] );

	for ( const Vector *const pEnd( pDst + vDeltas.Count() ); pDst != pEnd; ++pDst, ++pSrc )
	{
		pDst->x = pSrc->x;
		pDst->y = pSrc->y;
		pDst->z = pSrc->z;
	}
}


//-----------------------------------------------------------------------------
// Gets the non-zero Maya blendShape deltas for the given blendShape target
//-----------------------------------------------------------------------------
void GetDeltaVertexValues(
	CDmeMesh *pDmeMesh,
	const MString &deltaName,
	const MPlug &inputTargetItemP,
	CUtlVector< int > &vIndices,
	CUtlVector< Vector > &vDeltas )
{
	vDeltas.RemoveAll();

	// Check to see if the delta in Maya is coming from real geometry
	// or from a delta state already

	MPlug ipgP( inputTargetItemP.child( 0 ) );
	MObject ipgObj;
	ipgP.getValue( ipgObj );
	if ( !ipgObj.isNull() )
	{
		// Delta comes from real geometry
		GetDeltaVertexValuesFromGeometry( pDmeMesh, ipgObj, vIndices, vDeltas );
	}
	else
	{
		// Delta comes from a delta on the Maya blendShape node
		GetDeltaVertexValuesFromDelta( pDmeMesh, deltaName, inputTargetItemP, vIndices, vDeltas );
	}
}


//-----------------------------------------------------------------------------
// Remove zero deltas from the data
//-----------------------------------------------------------------------------
void RemoveZeroDeltas(
	CUtlVector< int > &vIndices,
	CUtlVector< Vector > &vDeltas )
{
	CUtlVector< int > remove;

	const int viEnd( vIndices.Count() );
	const int vdEnd( vDeltas.Count() );
	const int vidEnd( min( viEnd, vdEnd ) );

	const Vector *pd( vDeltas.Base() );
	const Vector *const pdEnd( pd + vidEnd );
	for ( int vidi( 0 ); pd != pdEnd; ++pd, ++vidi )
	{
		// Kind of a magic number but it's because of 16 bit compression of the delta values
		if ( fabs( pd->x ) < ( 1 / 4096.0f ) && fabs( pd->y ) < ( 1.0f / 4096.0f ) && fabs( pd->z ) < ( 1.0f / 4096.0f ) )
		{
			remove.AddToTail( vidi );
		}
	}

	const int rEnd( remove.Count() );
	if ( rEnd )
	{
		// First remove everything from vidEnd to the end of vIndices & vDeltas to avoid shifting more than required
		if ( viEnd > vidEnd )
		{
			vIndices.RemoveMultiple( vidEnd, viEnd - vidEnd );
		}

		if ( vdEnd > vidEnd )
		{
			vDeltas.RemoveMultiple( vidEnd, vdEnd - vidEnd );
		}

		// Now remove the extras working backwards to avoid shifting more than is necessary
		const int *const prBase( remove.Base() );
		for ( const int *pr( prBase + rEnd - 1 ); pr >= prBase; --pr )
		{
			vIndices.Remove( *pr );
			vDeltas.Remove( *pr );
		}
	}
}


void AddDelta(
	CDmeVertexDeltaData *pDmeDeltaData,
	MFnMesh &meshFn )
{
	const FieldIndex_t positionIndex( pDmeDeltaData->FindFieldIndex( CDmeVertexDeltaData::FIELD_POSITION ) );
	if ( positionIndex < 0 )
		return;

	const CDmrArray< int > positionIndices( pDmeDeltaData->GetIndexData( positionIndex ) );
	const CDmrArray< Vector > positionData( pDmeDeltaData->GetVertexData( positionIndex ) );

	int pIndex;
	MPoint mPoint;
	const int pEnd( positionIndices.Count() );
	for ( int pi( 0 ); pi < pEnd; ++pi )
	{
		pIndex = positionIndices[ pi ];
		meshFn.getPoint( pIndex, mPoint );
		meshFn.setPoint( pIndex, mPoint + reinterpret_cast< const MFloatVector & >( positionData[ pi ] ) );
	}
}


//-----------------------------------------------------------------------------
// Get the delta state normals from maya for a specific delta state
//-----------------------------------------------------------------------------
void CDmeExport::GetDeltaNormals(
	MFnMesh &meshFn,
	MObject &meshObj,
	const CNormalData &normalData,
	CDmeMesh *pDmeMesh,
	int nDeltaIndex,
	const CUtlVector<int> &dependentDeltaStates )
{
	// For each delta state, add in all of the position deltas that
	// contribute to this state plus this state to the base mesh
	const int dEnd( dependentDeltaStates.Count() );
	for ( int di( 0 ); di < dEnd; ++di )
	{
		AddDelta( pDmeMesh->GetDeltaState( dependentDeltaStates[ di ] ), meshFn );
	}
	AddDelta( pDmeMesh->GetDeltaState( nDeltaIndex ), meshFn );

	// Now ask Maya for the actual normals (so they are computed consistently)
	CUtlVector< Vector > deltaNormals;
	normalData.GetNormalData( meshObj, deltaNormals );

	// Set the Delta normals from the actual normals
	pDmeMesh->SetDeltaNormalDataFromActualNormals(
		nDeltaIndex, dependentDeltaStates,
		deltaNormals.Count(), deltaNormals.Base() );

#if defined( _DEBUG ) && 0
	// Debug!
	MDagModifier dm;
	MObject dmObj = dm.createNode( "mesh" );
	dm.doIt();

	dm.renameNode( dmObj, pDmeMesh->GetDeltaState( nDeltaIndex )->GetName() );
	dm.doIt();

	MFnMesh meshCopyFn;
	meshCopyFn.copy( meshObj, dmObj );
#endif // defined( _DEBUG )
}


//-----------------------------------------------------------------------------
// Get the delta state normals from maya for all delta states
//-----------------------------------------------------------------------------
void CDmeExport::GetDeltaNormals(
	const MDagPath &baseMeshPath,
	const CNormalData &normalData,
	CDmeMesh *pDmeMesh,
	CDmeCombinationOperator *pDmeComboOp )
{
	// Create a temp mesh to do computations on
	MFnMesh meshFn;
	MObject tmpMeshObj( MFnMeshData().create() );

	CUtlVector< CDmeMesh::DeltaComputation_t > deltaList;
	pDmeMesh->ComputeDependentDeltaStateList( deltaList );

	for ( int i = 0; i < deltaList.Count(); ++i )
	{
		// Copy the base state mesh into the temporary mesh
		MObject meshObj = meshFn.copy( baseMeshPath.node(), tmpMeshObj );

		GetDeltaNormals(
			meshFn, meshObj, normalData, pDmeMesh,
			deltaList[ i ].m_nDeltaIndex, deltaList[ i ].m_DependentDeltas );
	}
}


//-----------------------------------------------------------------------------
// Get the names of the blendShape targets by looking for aliasAttr's on
// the blendshape node and then specifically parsing out those that refer
// to .weight[*].  As far as I can tell this can only be done via mel
// as of Maya 7 because the type of attribute aliasAttr's is stored in
// is inaccessible via the API.  You'd think it would just be a string
// array but nope...
//-----------------------------------------------------------------------------
void
BuildWeightAliasMap(
	const MFnBlendShapeDeformer &blendFn,
	CUtlMap< int, MString > &aaMap )
{
	MStringArray aaList;
	MGlobal::executeCommand( MString( "aliasAttr -q \"" ) + blendFn.name() + "\";", aaList );
	if ( aaList.length() )
	{
		int wi;

		const uint aaEnd( aaList.length() );
		for ( uint aai( 1 ); aai < aaEnd; aai += 2 )
		{
			if ( sscanf( aaList[ aai ].asChar(), "weight[%d]", &wi ) == 1 )
			{
				aaMap.Insert( wi, aaList[ aai - 1 ] );
			}
		}
	}
}


//-----------------------------------------------------------------------------
// Figure out the name of this delta state and any attributes based on the name
//-----------------------------------------------------------------------------
void GetDeltaName(
	int deltaIndex,
	CUtlMap< int, MString > &aaMap,
	MString &deltaName,
	bool &control )
{
	// Figure out the delta name from the aliasAttr map and the delta index
	const CUtlMap< int, MString >::IndexType_t aai( aaMap.Find( deltaIndex ) );
	deltaName = aaMap.IsValidIndex( aai ) ?
		aaMap.Element( aai ) :
	MString( "weight[" ) + deltaIndex + "]";

	// If it hasn't any _'s in it's name it's a Control!
	control = ( deltaName.index( '_' ) < 0 );
}


//-----------------------------------------------------------------------------
// Find or create the CDmeCombinationOperator for this export session
//-----------------------------------------------------------------------------
CDmeCombinationOperator *CDmeExport::FindOrCreateDmeComboOp(
	const MString &name ) const
{
	CDmeCombinationOperator *pDmeComboOp(
		GetRoot()->GetValueElement< CDmeCombinationOperator >( "combinationOperator" ) );

	if ( !pDmeComboOp )
	{
		pDmeComboOp = CreateElement< CDmeCombinationOperator >( name.asChar(), GetRoot()->GetFileId() );
		GetRoot()->SetValue( "combinationOperator", pDmeComboOp );
	}

	return pDmeComboOp;
}


//-----------------------------------------------------------------------------
// Convert data in a Maya blendShape node to DmeVertexDeltaData
//-----------------------------------------------------------------------------
CDmeCombinationOperator *CDmeExport::DoBlendShape(
	const MDagPath &meshPath,
	const MDagPath &baseMeshPath,
	const MObject &blendShapeObj,
	const CNormalData &normalData,
	CDmeMesh *pDmeMesh ) const
{
	const MFnBlendShapeDeformer blendFn( blendShapeObj );

	// Sanity checks

	{
		MObjectArray baseObjs;
		blendFn.getBaseObjects( baseObjs );
		if ( baseObjs.length() > 1 )
		{
			mwarn << "Multiple base objects driving blend shape node \"" << blendFn.name() << "\", ignoring all but the first one" << std::endl;
		}
	}

	// Find or create the DmeComboOp for this Dme scene
	CDmeCombinationOperator *pDmeComboOp( FindOrCreateDmeComboOp( ValveMaya::RemoveNameSpace( blendFn.name() ) ) );

	if ( !pDmeComboOp )
	{
		return NULL;
	}

	// A map for looking up aliasAttr's on .weight for target states
	CUtlMap< int, MString > aaMap( 0, 0, DefLessFunc( int ) );
	BuildWeightAliasMap( blendFn, aaMap );

	MDoubleArray wArray;
	const MFnMesh baseMeshFn( baseMeshPath );
	wArray.setLength( baseMeshFn.numVertices() );

	// Loop through all of the targets on the blendShape node and suck out the relevant data
	MPlug itAP( blendFn.findPlug( "it" ) );										// .inputTarget
	const uint itEnd( itAP.evaluateNumElements() );
	for ( uint iti( 0 ); iti != itEnd; ++iti )
	{
		MPlug itP( itAP.elementByPhysicalIndex( iti ) );
		if ( itP.logicalIndex() == 0 )
		{
			MPlug itgAP( itP.child( 0 ) );										// .inputTarget.inputTargetGroup
			const uint itgEnd( itgAP.evaluateNumElements() );
			for ( uint itgi( 0 ); itgi != itgEnd; ++itgi )
			{
				MPlug itgP( itgAP.elementByPhysicalIndex( itgi ) );
				const int deltaIndex( itgP.logicalIndex() );					// The Maya index of the current delta state

				MPlug itiAP( itgP.child( 0 ) );									// .inputTarget.inputTargetGroup.inputTargetItem
				const uint itiEnd( itiAP.evaluateNumElements() );
				for ( uint itii( 0 ); itii != itiEnd; ++itii )
				{
					MPlug itiP( itiAP.elementByPhysicalIndex( itii ) );
					if ( itiP.logicalIndex() == 6000 )							// currently only interested in those at logicalIndex 6000
					{
						MString deltaName;										// The name of this delta state
						bool isSplit( false );									// Is this delta a Left/Right split? TODO: Look up split/dominance in Dme?
						bool isControl( false );								// Is this delta a control (input) or just a combination

						GetDeltaName( deltaIndex, aaMap, deltaName, isControl );

						CUtlVector< int > vIndices;								// The index of the vertex for each value in vDeltas
						CUtlVector< Vector > vDeltas;							// The deltas value of the vertex for this state

						GetDeltaVertexValues( pDmeMesh,							// Get the delta data from Maya
							deltaName, itiP, vIndices, vDeltas );

						if ( vIndices.Count() == 0 )							// If there's no delta data, ignore this delta state
						{
							continue;
						}

						if ( vIndices.Count() != vDeltas.Count() )
						{
							mwarn << "blendShape target \"" << 
								itiP.info() << "\" has " << vIndices.Count() << " components but " << vDeltas.Count() << " Deltas" << std::endl;
						}

						RemoveZeroDeltas( vIndices, vDeltas );					// Remove any zero deltas

						if ( m_optWeightMap )
						{
							wArray = MDoubleArray( baseMeshFn.numVertices(), 0.0 );
							int i( 0 );
							int index;
							double maxLen( 0.0 );
							for ( int j( 0 ); j != vIndices.Count(); ++j )
							{
								index = vIndices[ j ];
								while ( i != index )
								{
									++i;
								}

								wArray[ i ] = vDeltas[ j ].Length();
								if ( wArray[ i ] > maxLen )
								{
									maxLen = wArray[ i ];
								}
							}

							if ( maxLen > 0.0 )
							{
								for ( i = 0; i < static_cast< int >( wArray.length() ); ++i )
								{
									wArray[ i ] /= maxLen;
								}
							}

							MObject aObj( MFnTypedAttribute().create( deltaName + "Weight", deltaName + "Weight", MFnData::kDoubleArray ) );

							MDGModifier dgMod;
							dgMod.addAttribute( meshPath.node(), aObj );
							dgMod.doIt();

							MPlug mPlug( meshPath.node(), aObj );
							MFnDoubleArrayData dadFn;
							mPlug.setValue( dadFn.create( wArray ) );
						}

						const int vidEnd( min( vIndices.Count(), vDeltas.Count() ) );

						// Create and set the Dme version of the delta state
						if ( isControl )
						{
							pDmeComboOp->FindOrCreateControl( deltaName.asChar(), isSplit, true );
						}

						CDmeVertexDeltaData *pDmeVertexDeltaData( pDmeMesh->FindOrCreateDeltaState( deltaName.asChar() ) );
						pDmeVertexDeltaData->FlipVCoordinate( true );

						FieldIndex_t deltaPosIndex( pDmeVertexDeltaData->CreateField( CDmeVertexDeltaData::FIELD_POSITION ) );
						pDmeVertexDeltaData->AddVertexData( deltaPosIndex, vidEnd );

						pDmeVertexDeltaData->SetVertexData( deltaPosIndex, 0, vidEnd, AT_VECTOR3, vDeltas.Base() );
						pDmeVertexDeltaData->SetVertexIndices( deltaPosIndex, 0, vidEnd, vIndices.Base() );
					}
					else
					{
						mwarn << blendFn.name() << ": Ignoring != 6000 inbetween " <<
							itiP.logicalIndex() << " == " <<
							( ( itiP.logicalIndex() - 5000.0 ) / 1000.0 ) << std::endl;
					}
				}
			}
		}
		else
		{
			mwarn << blendFn.name() << ": Ignoring targets for blend shape input " <<
				itP.logicalIndex() << std::endl;
		}
	}

	GetDeltaNormals( baseMeshPath, normalData, pDmeMesh, pDmeComboOp );

	pDmeComboOp->AddTarget( pDmeMesh );

	return pDmeComboOp;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool GetMeshAttrDataInHistory( const MObject &nObj, const char *pAttrName, MSelectionList &visited, const int nVertexCount, float *pVertexData )
{
	// Check if we've already seen this node
	if ( visited.hasItem( nObj ) )
		return false;

	// Add to visited list
	visited.add( nObj );

	MFnDependencyNode nFn( nObj );

	MPlug aPlug = nFn.findPlug( pAttrName );

	if ( aPlug.isNull() )
	{
		MPlugArray connections;
		MPlugArray inPlugs;

		if ( nObj.hasFn( MFn::kMesh ) )
		{
			connections.append( nFn.findPlug( "inMesh" ) );
		}
		else
		{
			connections.append( nFn.findPlug( "inputGeometry" ) );
//			nFn.getConnections( connections );
		}

		for ( uint i = 0; i < connections.length(); ++i )
		{
			if ( connections[ i ].connectedTo( inPlugs, true, false ) && inPlugs.length() )
			{
				for ( uint j = 0; j < inPlugs.length(); ++j )
				{
					if ( GetMeshAttrDataInHistory( inPlugs[ j ].node(), pAttrName, visited, nVertexCount, pVertexData ) )
						return true;
				}
			}
		}

		return false;
	}

	MObject aObj = aPlug.attribute();

	// Not a typed attribute, ignore it
	if ( !aObj.hasFn( MFn::kTypedAttribute ) )
		return false;

	// Not doubleArray, ignore it
	const MFnTypedAttribute tFn( aObj );
	if ( tFn.attrType() != MFnData::kDoubleArray )
		return false;

	MObject avObj;
	aPlug.getValue( avObj );
	MFnDoubleArrayData avFn( avObj );

	// Not the same size as the number of vertices, ignore it
	if ( static_cast< int >( avFn.length() ) != nVertexCount )
		return false;

	const double *pSrc( &avFn[ 0 ] );
	for ( const float *const pVertexDataEnd( pVertexData + nVertexCount ); pVertexData != pVertexDataEnd; ++pVertexData, ++pSrc )
	{
		*pVertexData = *pSrc;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Convert Maya attributes to Dme vertex data as long as the following is true:
// * Dynamic maya attribute
// * doubleArray ( TODO: floatArray, colorArray, vectorArray, pointArray )
// * Same size as the number of vertices ( TODO: faceVarying )
//-----------------------------------------------------------------------------
void MayaVertexDataToDmeVertexData(
	const MDagPath &mDagPath,
	CDmeVertexData *pDmeVertexData,
	CDmeCombinationOperator *pDmeComboOp )
{
	const CUtlVector< int > &positionIndices( pDmeVertexData->GetVertexIndexData( CDmeVertexDataBase::FIELD_POSITION ) );
	const int nVertices( pDmeVertexData->GetPositionData().Count() );

	// Look for "speed" & "balance" maps if there is a combination operator attached
	const char *attrsToConvert[] =
	{
		"balance",
		"speed"
	};

	float *pVertexData = reinterpret_cast< float * >( alloca( nVertices * sizeof( float ) ) );
	for ( int i = 0; i < sizeof( attrsToConvert ) / sizeof( attrsToConvert[ 0 ] ); ++i )
	{
		MSelectionList visited;
		if ( GetMeshAttrDataInHistory( mDagPath.node(), attrsToConvert[ i ], visited, nVertices, pVertexData ) )
		{
			if ( !Q_strcmp( attrsToConvert[ i ], "balance" ) )
			{
				const CUtlVector<Vector> &positionData( pDmeVertexData->GetPositionData() );
				const float *pX( reinterpret_cast< const float * >( positionData.Base() ) );
				float *pData = pVertexData;
				for ( const float *const pDataEnd( pData + nVertices ); pData != pDataEnd; ++pData, pX += 3 )
				{
					if ( *pX > 0.0f )
					{
						// Left
						*pData = *pData * 0.5f;
					}
					else
					{
						// Right
						*pData = 1.0f - ( *pData * 0.5f );
					}
				}
			}
			else if ( pDmeComboOp && !Q_strcmp( attrsToConvert[ i ], "speed" ) )
			{
				pDmeComboOp->UsingLaggedData( true );
			}

			const FieldIndex_t vIndex( pDmeVertexData->CreateField< float >( attrsToConvert[ i ] ) );
			if ( vIndex < 0 )
				continue;

			// Add & set the data
			pDmeVertexData->AddVertexData( vIndex, nVertices );
			pDmeVertexData->SetVertexData( vIndex, 0, nVertices, AT_FLOAT, pVertexData );
			pDmeVertexData->SetVertexIndices( vIndex, 0, positionIndices.Count(), positionIndices.Base() );
		}
	}
	
	const MFnDagNode dagFn( mDagPath );
	for ( uint ai( dagFn.attributeCount() - 1 ); ai >= 0; --ai )
	{
		const MObject aObj( dagFn.attribute( ai ) );
		const MFnAttribute aFn( aObj );

		// Not dynamic - abort at this point
		if ( !aFn.isDynamic() )
			break;

		// Not a typed attribute, ignore it
		if ( !aObj.hasFn( MFn::kTypedAttribute ) )
			continue;

		// Not doubleArray, ignore it
		const MFnTypedAttribute tFn( aObj );
		if ( tFn.attrType() != MFnData::kDoubleArray )
			continue;

		MObject avObj;
		{
			MPlug aP( mDagPath.node(), aObj );
			aP.getValue( avObj );
		}
		MFnDoubleArrayData avFn( avObj );

		// Not the same size as the number of vertices, ignore it
		if ( static_cast< int >( avFn.length() ) != nVertices )
			continue;

		// Only convert attributes that start with "vs"
		const MString aN( aFn.name() );
		if ( aN.substring( 0, 1 ).toLowerCase() != "vs" )
			continue;

		const FieldIndex_t vIndex( pDmeVertexData->CreateField< float >( aFn.name().asChar() ) );
		if ( vIndex < 0 )
			continue;

		const double *pSrc( &avFn[ 0 ] );
		float *pDst( pVertexData );

		if ( pDmeComboOp && aN == "balance" )
		{
			const CUtlVector<Vector> &positionData( pDmeVertexData->GetPositionData() );
			const float *pX( reinterpret_cast< const float * >( positionData.Base() ) );
			for ( const float *const pDstEnd( pDst + nVertices ); pDst != pDstEnd; ++pDst, ++pSrc, pX += 3 )
			{
				if ( *pX > 0.0f )
				{
					// Left
					*pDst = *pSrc * 0.5f;
				}
				else
				{
					// Right
					*pDst = 1.0f - ( *pSrc * 0.5f );
				}
			}
		}
		else
		{
			if ( pDmeComboOp && aN == "speed" )
			{
				pDmeComboOp->UsingLaggedData( true );
			}

			for ( const float *const pDstEnd( pDst + nVertices ); pDst != pDstEnd; ++pDst, ++pSrc )
			{
				*pDst = *pSrc;
			}
		}

		// Add & set the data
		pDmeVertexData->AddVertexData( vIndex, nVertices );
		pDmeVertexData->SetVertexData( vIndex, 0, nVertices, AT_FLOAT, pVertexData );
		pDmeVertexData->SetVertexIndices( vIndex, 0, positionIndices.Count(), positionIndices.Base() );
	}
}


//-----------------------------------------------------------------------------
// Adds the position data from the passed MFnMesh function set to the
// passed DmeVertexData returning the field index where it is stored
//-----------------------------------------------------------------------------
int CDmeExport::AddPositionData(
	const MDagPath &meshDagPath,
	CDmeVertexData *pDmeVertexData )
{
	const MFnMesh meshFn( meshDagPath );

	MFloatPointArray mPoints;
	meshFn.getPoints( mPoints );
	const uint numVertices( mPoints.length() );

	// We have to put it into a temporary format for now - maybe can stuff it right into DME?
	CUtlVector< Vector > dPoints;
	dPoints.EnsureCount( numVertices );

	// Copy from Maya homogeneous to Dme cartesian
	for ( uint pi( 0 ); pi < numVertices; ++pi )
	{
		const MFloatPoint &p( mPoints[ pi ] );
		// Just ignore W, should be 1... but could divide by W...
		dPoints[ pi ] = Vector( p.x, p.y, p.z );
	}

	const FieldIndex_t positionIndex( pDmeVertexData->CreateField( CDmeVertexDataBase::FIELD_POSITION ) );
	pDmeVertexData->AddVertexData( positionIndex, numVertices );
	pDmeVertexData->SetVertexData( positionIndex, 0, numVertices, AT_VECTOR3, dPoints.Base() );

	return positionIndex;
}


//-----------------------------------------------------------------------------
// Adds the UV (Texture Coordinate) data from the passed MFnMesh function set
// to the passed DmeVertexData returning the field index where it is stored
//-----------------------------------------------------------------------------
int CDmeExport::AddUVData(
	const MDagPath &meshDagPath,
	const MString &uvSet,
	CUtlVector< Vector2D > &uvData,
	CUtlVector< int > &uvIndexMap ) const
{
	int uvOffset( uvData.Count() );

	const MFnMesh meshFn( meshDagPath );
	const uint numUVs( meshFn.numUVs( uvSet ) );

	if ( numUVs )
	{
		MFloatArray mUs;
		MFloatArray mVs;
		meshFn.getUVs( mUs, mVs, &uvSet );

		const float *pUSrc( &mUs[ 0 ] );
		const float *pVSrc( &mVs[ 0 ] );

		Vector2D uv;

		uvData.EnsureCapacity( uvData.Count() + numUVs );
		uvIndexMap.EnsureCount( numUVs );

		if ( m_optMergeUvs )
		{
			int uvDataCount;
			bool uniqueUv;
			for ( uint uvi( 0 ); uvi != numUVs; ++uvi, ++pUSrc, ++pVSrc )
			{
				uv.x = *pUSrc;
				uv.y = *pVSrc;

				uvDataCount = uvData.Count();
				uniqueUv = true;
				for ( int uvdi( 0 ); uvdi < uvDataCount; ++uvdi )
				{
					if ( uv == uvData[ uvdi ] )
					{
						uvIndexMap[ uvi ] = uvdi;
						uniqueUv = false;
						break;
					}
				}

				if ( uniqueUv )
				{
					uvIndexMap[ uvi ] = uvData.Count();
					uvData.AddToTail( uv );
				}
			}
		}
		else
		{
			for ( uint uvi( 0 ); uvi != numUVs; ++uvi, ++pUSrc, ++pVSrc )
			{
				uvIndexMap[ uvi ] = uvi;
				uvData.AddToTail( Vector2D( *pUSrc, *pVSrc ) );
			}
		}

		return uvOffset;
	}

	return -1;
}


//-----------------------------------------------------------------------------
// Adds the UV (Texture Coordinate) data from the passed MFnMesh function set
// to the passed DmeVertexData returning the field index where it is stored
//-----------------------------------------------------------------------------
int CDmeExport::AddColorData(
	const MDagPath &meshDagPath,
	CDmeVertexData *pDmeVertexData )
{
	const MFnMesh meshFn( meshDagPath );
	const uint numColors( meshFn.numColors() );

	if ( numColors )
	{
		MColorArray mColorArray;
		meshFn.getColors( mColorArray );

		const MColor *pColorSrc( &mColorArray[ 0 ] );

		CUtlVector< Color > dColors;
		dColors.EnsureCount( numColors );

		float r, g, b, a;

		Color *pColorDst( dColors.Base() );
		for ( const Color *const pColorEnd( pColorDst + numColors ); pColorDst != pColorEnd; ++pColorDst, ++pColorSrc )
		{
			pColorSrc->get( MColor::kRGB, r, g, b, a );
			pColorDst->SetColor(
				clamp( static_cast<int>( floor( r * 255.0f ) ), 0, 255 ),
				clamp( static_cast<int>( floor( g * 255.0f ) ), 0, 255 ),
				clamp( static_cast<int>( floor( b * 255.0f ) ), 0, 255 ),
				clamp( static_cast<int>( floor( a * 255.0f ) ), 0, 255 ) );
		}

		const FieldIndex_t colorIndex( pDmeVertexData->CreateField( CDmeVertexDataBase::FIELD_COLOR ) );
		pDmeVertexData->AddVertexData( colorIndex, numColors );
		pDmeVertexData->SetVertexData( colorIndex, 0, numColors, AT_COLOR, dColors.Base() );

		return colorIndex;
	}

	return -1;
}


//-----------------------------------------------------------------------------
// Purpose: Convert a Maya Mesh node into a DmeMesh node
// Input  : mDagPath		The Maya Dag Path to export
//			pDmeDagParent	The CDmeDag that will be the parent of the newly created
//							node, cannot be NULL
// Output : The CDmeMesh for the node that was converted, can be NULL
//-----------------------------------------------------------------------------
uint CDmeExport::ConvertMesh(
	const MDagPath &mDagPath,
	CDmeDag *pDmeDagParent,
	CDmeDag **ppDmeDagRet )
{
	// Don't export invisible meshes unless -exportInvisible is set
	// Don't export meshes unless we're exporting models or skeletal animation

	if ( !DoType( kModel | kPhysicsModel | kSkeletalAnimation ) ||
		MFnDagNode( mDagPath).isIntermediateObject() || !( m_exportInvisible || ValveMaya::IsNodeVisible( mDagPath, false ) ) )
		return 0;

	// If we are exporting skeletal animations but not models, don't actually convert the mesh
	// but inform our ancestors that the hierarchy up to this point should be kept around
	if ( !DoType( kModel | kPhysicsModel ) && DoType( kSkeletalAnimation ) )
		return 1;

	// If we have no parent, make one up!
	// This should call this routine again with this MDagPath
	// and with it's parent converted into a DmeModel
	if ( !pDmeDagParent )
	{
		mwarn << "No parent found for " << mDagPath.partialPathName() << ", making one up" << std::endl;
		MDagPath pDagPath( mDagPath );
		while ( pDagPath.isValid() && pDagPath.length() && !pDagPath.hasFn( MFn::kTransform ) )
		{
			pDagPath.pop();
		}

		if ( pDagPath.isValid() && pDagPath.length() && pDagPath.hasFn( MFn::kTransform ) )
			return ConvertDag( pDagPath, pDmeDagParent, ppDmeDagRet, true );
	}

	// Now set the shape for the passed in DmeDag
	if ( pDmeDagParent->GetShape() )
	{
		mwarn << "Dme parent " << pDmeDagParent->GetName() <<
		" already has a shape node " << pDmeDagParent->GetShape()->GetName() <<
		", cannot set " << mDagPath.fullPathName() << " as it's shape" << std::endl;
		return 0;
	}

	// Don't do things twice
	if ( m_exportedList.hasItem( mDagPath ) )
	{
		mwarn << "Already processed " << mDagPath.fullPathName() << std::endl;
		return 1;
	}

	// Create the CDmeMesh element
	CDmeMesh *pDmeMesh( CreateElement< CDmeMesh >( GetNodeName( mDagPath ).asChar(), FileId() ) );

	CDmeVertexData *pDmeVertexData( pDmeMesh->FindOrCreateBaseState( "bind" ) );
	pDmeVertexData->FlipVCoordinate( true );

	pDmeMesh->SetCurrentBaseState( "bind" );

	MDagPath baseMeshPath;
	MObject blendObj;
	MObject skinObj;

	HandleDeformers( mDagPath, baseMeshPath, skinObj, blendObj );

	const MFnMesh meshFn( baseMeshPath );
	const int numColors( meshFn.numColors() );
	const int numUVs( meshFn.numUVs() );

	CUtlVector< int > positionIndices;
	CUtlVector< int > normalIndices;
	CUtlVector< int > uvIndices;
	CUtlVector< int > colorIndices;

	{
		const int numFaceVertices( meshFn.numFaceVertices() );

		positionIndices.EnsureCapacity( numFaceVertices );
		normalIndices.EnsureCapacity( numFaceVertices );
		uvIndices.EnsureCapacity( numUVs ? numFaceVertices : 0 );
		colorIndices.EnsureCapacity( numColors ? numFaceVertices : 0 );
	}

	CNormalData normalData;
	normalData.GetNormalData( baseMeshPath, ( m_type & kPhysicsModel ) != 0 );

	CUtlVector< Vector2D > uvData;

	MayaFaceSetsToDmeFaceSets(
		mDagPath, baseMeshPath, normalData, pDmeMesh, positionIndices,
		normalIndices, uvIndices, uvData, colorIndices );

	const int numFaceVertices( positionIndices.Count() );
	Assert( normalIndices.Count() == numFaceVertices );

	// Add the vertex data
	pDmeVertexData->AddVertexIndices( numFaceVertices );

	// Add Position Data
	pDmeVertexData->SetVertexIndices(
		AddPositionData(
			baseMeshPath, pDmeVertexData), 0, numFaceVertices, positionIndices.Base() );

	// Add Normal Data
	pDmeVertexData->SetVertexIndices( normalData.AddNormalData( pDmeVertexData ), 0, numFaceVertices, normalIndices.Base() );

	// Add UV Data
	if ( uvData.Count() )
	{
		const FieldIndex_t uvIndex( pDmeVertexData->CreateField( CDmeVertexDataBase::FIELD_TEXCOORD ) );
		pDmeVertexData->AddVertexData( uvIndex, uvData.Count() );
		pDmeVertexData->SetVertexData( uvIndex, 0, uvData.Count(), AT_VECTOR2, uvData.Base() );
		pDmeVertexData->SetVertexIndices( uvIndex, 0, numFaceVertices, uvIndices.Base() );
	}

	// Field index for Colors, might not be used
	FieldIndex_t colorIndex( AddColorData( baseMeshPath, pDmeVertexData ) );
	if ( colorIndex >= 0 )
	{
		pDmeVertexData->SetVertexIndices( colorIndex, 0, numFaceVertices, colorIndices.Base() );
	}

	CDmeCombinationOperator *pDmeComboOp(
		blendObj.isNull() ?
		NULL :
		DoBlendShape( mDagPath, baseMeshPath, blendObj, normalData, pDmeMesh ) );

	MayaVertexDataToDmeVertexData( mDagPath, pDmeVertexData, pDmeComboOp );

	pDmeDagParent->SetShape( pDmeMesh );

	if ( !skinObj.isNull() )
	{
		// This mesh is skinned so put it on the appropriate lists
		m_bindList.add( mDagPath, MObject::kNullObj, true );
		m_mayaToDmeDag.Insert( CUtlString( mDagPath.fullPathName().asChar() ), pDmeDagParent );

		// See if there's an active dmeModel for this Mesh
		CDmeModel *pDmeModel( m_dmeModelStack.Count() ? m_dmeModelStack.Top() : NULL );

		if ( pDmeModel )
		{
			CDmAttribute *pDmeModelAttr = pDmeDagParent->AddAttributeElement< CDmeModel >( "__dmeModel_bind" );

			if ( pDmeModelAttr )
			{
				pDmeModelAttr->AddFlag( FATTRIB_DONTSAVE );
				pDmeModelAttr->SetValue( pDmeModel->GetHandle() );
			}
		}
	}

	m_exportedList.add( mDagPath );

	return 1;
}


//-----------------------------------------------------------------------------
// Purpose: Tag a given DmeDag node with extra attributes
//-----------------------------------------------------------------------------
void CDmeExport::AddTags(
	const MString &cmdLine ) const
{
	CDmAttribute *pExportTags =	GetRoot()->AddAttributeElement<CDmElement>( "vsDmxIO_exportTags" );

	CDmElement *pDmeElement( CreateElement<CDmElement>( "vsDmxIO_exportTags", FileId() ) );

	pExportTags->SetValue( pDmeElement->GetHandle() );

	MString melResult;
	MStringArray melArrayResult;

	MGlobal::executeCommand( "about -cd", melResult );
	pDmeElement->SetValue( "date", CUtlString( melResult.asChar() ) );

	MGlobal::executeCommand( "about -ct", melResult );
	pDmeElement->SetValue( "time", CUtlString( melResult.asChar() ) );

	char pTmpBuf[256];
	*pTmpBuf ='\0';
	DWORD dwSize( sizeof( pTmpBuf ) );

	GetUserName( pTmpBuf, &dwSize);
	pDmeElement->SetValue( "user", CUtlString( pTmpBuf ) );

	GetComputerName( pTmpBuf, &dwSize);
	pDmeElement->SetValue( "machine", CUtlString( pTmpBuf ) );

	MGlobal::executeCommand( "strip( about( \"-osv\" ) )", melResult );
	pDmeElement->SetValue( "os", CUtlString( melResult.asChar() ) );

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

		pDmeElement->SetValue( "cpu", CUtlString( cpuInfo.asChar() ) );
	}

	const bool guiMode = MGlobal::mayaState() == MGlobal::kInteractive;

	if ( guiMode )
	{
		pDmeElement->SetValue( "mayaMode", "gui" );
		MGlobal::executeCommand( "hardware -gfx", melArrayResult );
		pDmeElement->SetValue( "gfx", CUtlString( melArrayResult[ 0 ].asChar() ) );
	}
	else
	{
		pDmeElement->SetValue( "mayaMode", "batch" );
	}

	MGlobal::executeCommand( "about -a", melResult );
	pDmeElement->SetValue( "app", CUtlString( melResult.asChar() ) );

	pDmeElement->SetValue( "appVersion", CUtlString( MGlobal::mayaVersion().asChar() ) );

	pDmeElement->SetValue( "cmdLine", CUtlString( cmdLine.asChar() ) );
}


//-----------------------------------------------------------------------------
// Purpose: Data passed through the export process
//-----------------------------------------------------------------------------
void CDmeExport::Bind(
	CDmeModel *pDmeModelRoot )
{
	MDagPath mDagPath;

	for ( MItSelectionList sIt( m_bindList ); !sIt.isDone(); sIt.next() )
	{
		if ( !sIt.getDagPath( mDagPath ) )
			continue;

		CDmeDag *pDmeDag( CastElement<CDmeDag>( FindDmeDag( mDagPath ) ) );
		if ( !pDmeDag )
		{
			merr << "Trying to bind " << mDagPath.partialPathName() <<
				" but can't find corresponding DmeDag node" << std::endl;
			continue;
		}

		// Look for the model bind reference
		CDmeModel *pDmeModel(
			pDmeDag->GetValueElement< CDmeModel >( "__dmeModel_bind" ) );
		pDmeDag->RemoveAttribute( "__dmeModel_bind" );

		if ( !pDmeModel )
		{
			pDmeModel = pDmeModelRoot;
		}

		// The mesh attached to the shape
		CDmeMesh *pDmeMesh( CastElement<CDmeMesh>( pDmeDag->GetShape() ) );
		if ( !pDmeMesh )
		{
			merr << "Trying to bind " << mDagPath.partialPathName() <<
				" but no DmeMesh set as the shape of DmeDag " << pDmeDag->GetName() << std::endl;
			continue;
		}

		// TODO: FIx me!  I already found the skinCluster node via listHistory
		MObject skinClusterObj;

		// See if the mesh has history, and if it does, if it's a
		// skin cluster... if it's something else, warn the user.
		{
			MPlug imP( MFnDagNode( mDagPath ).findPlug( "inMesh" ) );
			if ( !imP.isNull() )
			{
				MPlugArray mPlugArray;
				if ( imP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() )
				{
					MObjectArray skinnedObjects;
					const MObject meshObj( mDagPath.node() );
					MObject skinObj;

					// It has history so look for MFnSkinCluster objects
					for ( MItDependencyNodes dIt( MFn::kSkinClusterFilter ); skinClusterObj.isNull() && !dIt.isDone(); dIt.next() )
					{
						skinObj = dIt.thisNode();
						MFnSkinCluster( skinObj ).getOutputGeometry( skinnedObjects );
						const uint soEnd( skinnedObjects.length() );
						for ( uint soi( 0 ); soi != soEnd; ++soi )
						{
							if ( meshObj == skinnedObjects[ soi ] )
							{
								skinClusterObj = skinObj;
								break;
							}
						}
					}
				}
			}
		}

		if ( skinClusterObj.isNull() )
		{
			mwarn << "No skin cluster found driving mesh " << mDagPath.partialPathName() << std::endl;
			continue;
		}

		// It seems to be connected to a Maya SkinCluster node so get the influences
		MFnSkinCluster skinFn( skinClusterObj );
		MDagPathArray mDagPathArray;
		MStatus mStatus;

			// Make sure that there is at least one thing influencing the mesh
		if ( ! ( skinFn.influenceObjects( mDagPathArray, &mStatus ) && mStatus && mDagPathArray.length() ) )
		{
			mwarn << "No influence objects attached to skin cluster " << skinFn.name() <<
				" driving mesh " << mDagPath.partialPathName() << std::endl;
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
				MDagPath sDagPath( iDagPath );
				sDagPath.extendToShapeDirectlyBelow( 0 );
				const MFnDagNode dagFn( sDagPath );
				const MString nodeType( dagFn.typeName() );
				if ( nodeType != "vstHelperBone" )
				{
					mwarn << "Ignoring geometric influence object \"" << iDagPath.partialPathName() << "\" " <<
						"driving skin cluster \"" << skinFn.name() << "\" " <<
						"while exporting skinning information for " << mDagPath.partialPathName() << std::endl;
					continue;
				}
			}

			// Find the joint in the DmeScene
			CDmeDag *pDmeDag( FindDmeDag( iDagPath ) );

			// If the joint couldn't be found, go back and export it by finding where it should
			// be in what is already exported and start exporting that hierarchy at that point
			if ( !pDmeDag )
			{
				// If the joint isn't found, look for it's parent
				MDagPath cDagPath( iDagPath );
				MDagPath pDagPath( cDagPath );
				pDagPath.pop();
				while ( pDagPath.length() )
				{
					CDmeDag *pDmeDagAncestor( FindDmeDag( pDagPath ) );
					if ( pDmeDagAncestor )
					{
						ConvertDag( cDagPath, pDmeDagAncestor, NULL, true );
						pDmeDag = FindDmeDag( iDagPath );
						break;
					}

					cDagPath = pDagPath;
					pDagPath.pop();
				}

				// If the joint still hasn't been found, just start exporting
				// it parented to the DmeModel
				if ( !pDmeDag )
				{
					Assert( m_dmeModelStack.Count() );
					ConvertDag( cDagPath, m_dmeModelStack.Top(), NULL, true );
					pDmeDag = FindDmeDag( iDagPath );
				}

				if ( !pDmeDag )
				{
					merr << "Couldn't find or export joint \"" << iDagPath.partialPathName() <<
						"\" that is a skin joint on \"" << mDagPath.partialPathName() <<
						"\", check for invisibility and ensure it's part of the group of nodes being exported" << std::endl;
				}
			}

			dmeJointIndices.AddToTail( pDmeModel->AddJoint( pDmeDag ) );

			validInfluenceIndices.AddToTail( i );
		}

		// Valid influence count (sans Influence shapes, only joints & transforms)
		const int validInfluenceCount( validInfluenceIndices.Count() );

		// Make sure there is at least one valid thing influencing the mesh
		if ( validInfluenceCount <= 0 )
		{
			mwarn << "No valid influence objects found while getting bind info from " << skinFn.name() <<
				" for " << mDagPath.partialPathName() << std::endl;
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

					mwarn << "Unweighted vertex " << mStringArray[ 0 ] << " on skinned mesh " << mDagPath.partialPathName() << std::endl;

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

		CDmeVertexData *pDmeVertexData( pDmeMesh->FindBaseState( "bind" ) );
		if ( pDmeVertexData )
		{
			pDmeVertexData->CreateJointWeightsAndIndices( weightsPerVertex, &jointWeight, &jointIndex );
			Assert( dmeWeights.Count() == weightsPerVertex * pDmeVertexData->GetPositionData().Count() );
			Assert( dmeIndices.Count() == weightsPerVertex * pDmeVertexData->GetPositionData().Count() );

			pDmeVertexData->AddVertexData( jointIndex, dmeIndices.Count() );
			pDmeVertexData->SetVertexData( jointIndex, 0, dmeIndices.Count(), AT_INT, dmeIndices.Base() );

			pDmeVertexData->AddVertexData( jointWeight, dmeWeights.Count() );
			pDmeVertexData->SetVertexData( jointWeight, 0, dmeWeights.Count(), AT_FLOAT, dmeWeights.Base() );
		}

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
				merr << "Cannot find or create \"bind\" vertex state on DmeModel " << pDmeModel->GetName() << std::endl;
				continue;
			}

			CDmaElementArray<CDmeTransform> &bindTransforms( pDmeTransformList->m_Transforms );

			Assert( bindTransforms.Count() == jEnd );

			for ( int ji( 0 ); ji < jEnd; ++ji )
			{
				CDmeTransform *pSrcTransform( pDmeModel->GetJointTransform( ji ) );
				CDmeTransform *pSrc( pSrcTransform->GetValueElement<CDmeTransform>( "__bind" ) );

				if ( pSrc )
				{
					bindTransforms.Set( ji, pSrc );
					pSrcTransform->RemoveAttribute( "__bind" );
				}
				else
				{
					if ( ji != 0 )
					{
						mwarn << "Cannot find __bind transform on " << pSrcTransform->GetName() << std::endl;
					}

					CDmeTransform *pDst( CreateElement<CDmeTransform>( pSrcTransform->GetName(), FileId() ) );

					bindTransforms.Set( ji, pDst );
				}
			}
		}

		CDmaElementArray<CDmeTransform> &bindTransforms( pDmeTransformList->m_Transforms );
		Assert( bindTransforms.Count() <= jEnd );

		for ( int ji( bindTransforms.Count() ); ji < jEnd; ++ji )
		{
			CDmeTransform *pSrcTransform( pDmeModel->GetJointTransform( ji ) );
			CDmeTransform *pSrc( pSrcTransform->GetValueElement<CDmeTransform>( "__bind" ) );

			if ( pSrc )
			{
				bindTransforms.AddToTail( pSrc );
				pSrcTransform->RemoveAttribute( "__bind" );
			}
			else
			{
				if ( ji != 0 )
				{
					mwarn << "Cannot find __bind transform on " << pSrcTransform->GetName() << std::endl;
				}

				CDmeTransform *pDst( CreateElement<CDmeTransform>( pSrcTransform->GetName(), FileId() ) );

				pDst->SetOrientation( pSrc->GetOrientation() );
				pDst->SetPosition( pSrc->GetPosition() );

				bindTransforms.AddToTail( pDst );
			}
		}
	}
}


void CDmeExport::SetFileName(
	const char *filename )
{
	m_pMayaMakefile->SetFileName( filename );
}


//-----------------------------------------------------------------------------
// Takes a Maya shading group and tries to deduce a Valve Material name
// based on some path searching kookiness...
// In the future this should be more robust with known and documented
// ways of assigning stuff in Maya to match up to the Source world
//-----------------------------------------------------------------------------
void CDmeExport::GetMaterialInfo(
	const MObject &shadingGroupObj,
	MString &meshName,
	MString &faceSetName,
	MString &materialName,
	MString &materialPath,
	MString &uvSet,
	MString &debugWhy ) const
{
	Assert( shadingGroupObj.apiType() == MFn::kShadingEngine );

	faceSetName = vm::RemoveNameSpace( MFnDependencyNode( shadingGroupObj ).name() );
	materialName = faceSetName + "Material";
	materialPath = "debug/debugEmpty";

	MObject fileObj;
	materialPath = vm::GetMaterialPath( shadingGroupObj, &fileObj, NULL, NULL, NULL, &debugWhy );

	if ( !fileObj.isNull() )
	{
		// Get the name of the UV set associated with this texture
		MGlobal::executeCommand( "getAttr( uvLink( \"-queryObject\", \"" + meshName + "\", \"-texture\", \"" + MFnDependencyNode( fileObj ).name() + "\" ) )", uvSet );
	}

	const int lastSlash = materialPath.rindex( '/' );
	if ( lastSlash < 0 )
	{
		materialName = materialPath + "Material";
	}
	else
	{
		materialName = materialPath.substring( lastSlash + 1, materialPath.length() - 1 ) + "Material";
	}
}


//-----------------------------------------------------------------------------
// Given an array of polygon relative indices for a face and the triangles
// making up the face, get the face relative indices for the triangle verts
//-----------------------------------------------------------------------------
void CreateFaceVertexMap(
	const MIntArray &verts,
	const MIntArray &triVerts,
	MIntArray &triVertsMap )
{
	const uint nTriVerts( triVerts.length() );
	triVertsMap.setLength( nTriVerts );
	const uint nVerts( verts.length() );

	for ( uint i( 0U ); i != nTriVerts; ++i )
	{
		bool found( false );
		for ( uint j( 0U ); j != nVerts; ++j )
		{
			if ( triVerts[ i ] == verts[ j ] )
			{
				triVertsMap[ i ] = j;
				found = true;
				break;
			}
		}

		if ( !found )
		{
			minfo << " NOT FOUND!!!" << std::endl;
		}
	}
}


//-----------------------------------------------------------------------------
// Converts a single face sets on a Maya mesh to the Dme equivalent
//-----------------------------------------------------------------------------
void CDmeExport::MayaFaceSetToDmeFaceSet(
	const MDagPath &meshPath,
	const MDagPath &baseMeshPath,
	const CNormalData &normalData,
	const MObject &mayaFaceSetObject,
	const MString &uvSet,
	CDmeFaceSet *pFaceSet,
	CUtlVector< int > &positionIndices,
	CUtlVector< int > &normalIndices,
	CUtlVector< int > &uvIndices,
	const CUtlVector< int > &uvIndexMap,
	int uvOffset,
	CUtlVector< int > &colorIndices ) const
{
	// A vector for Face Set indices
	CUtlVector< int > faceSets;

	uint vIndex;
	int uvIndex;

	MPointArray triPnts;
	MIntArray triVerts;
	MIntArray triVertsMap;
	MIntArray verts;

	uint pIndex;
	int lfvi;
	int colorIndex;

	for ( MItMeshPolygon pIt( baseMeshPath, const_cast< MObject & >( mayaFaceSetObject ) ); !pIt.isDone(); pIt.next() )
	{
		pIndex = pIt.index();

		if ( m_optTriangulateBad )
		{
			if ( !pIt.isConvex() || pIt.isHoled() )
			{
				if ( pIt.hasValidTriangulation() )
				{
					mwarn << baseMeshPath.partialPathName() << ".f[" << pIndex << "] is not a convex polygon without holes, triangulating" << std::endl;

					pIt.getVertices( verts );
					pIt.getTriangles( triPnts, triVerts );
					CreateFaceVertexMap( verts, triVerts, triVertsMap );

					const int nVerts( triVerts.length() );

					if ( uvOffset >= 0 && pIt.hasUVs( uvSet ) && pIt.hasColor() )
					{
						for ( int i( 0 ); i < nVerts; )
						{
							for ( uint j( 0U ); j != 3U; ++j, ++i )
							{
								lfvi = triVertsMap[ i ];
								vIndex = triVerts[ i ];

								faceSets.AddToTail( positionIndices.Count() );

								normalIndices.AddToTail( normalData.NormalDataIndex( pIndex, lfvi ) );
								positionIndices.AddToTail( vIndex );
								pIt.getUVIndex( lfvi, uvIndex, &uvSet );
								uvIndices.AddToTail( uvIndexMap[ uvIndex ] + uvOffset );
								pIt.getColorIndex( lfvi, colorIndex );
								colorIndices.AddToTail( colorIndex );
							}

							faceSets.AddToTail( -1 );
						}
					}
					else if ( uvOffset >= 0 && pIt.hasUVs( uvSet ) )
					{
						for ( int i( 0 ); i < nVerts; )
						{
							for ( uint j( 0U ); j != 3U; ++j, ++i )
							{
								lfvi = triVertsMap[ i ];
								vIndex = triVerts[ i ];

								faceSets.AddToTail( positionIndices.Count() );

								normalIndices.AddToTail( normalData.NormalDataIndex( pIndex, lfvi ) );
								positionIndices.AddToTail( vIndex );
								pIt.getUVIndex( lfvi, uvIndex, &uvSet );
								uvIndices.AddToTail( uvIndexMap[ uvIndex ] + uvOffset );
							}

							faceSets.AddToTail( -1 );
						}
					}
					else if ( pIt.hasColor() )
					{
						for ( int i( 0 ); i < nVerts; )
						{
							for ( uint j( 0U ); j != 3U; ++j, ++i )
							{
								lfvi = triVertsMap[ i ];
								vIndex = triVerts[ i ];

								faceSets.AddToTail( positionIndices.Count() );

								normalIndices.AddToTail( normalData.NormalDataIndex( pIndex, lfvi ) );
								positionIndices.AddToTail( vIndex );
								pIt.getColorIndex( lfvi, colorIndex );
								colorIndices.AddToTail( colorIndex );
							}

							faceSets.AddToTail( -1 );
						}
					}
					else
					{
						for ( int i( 0 ); i < nVerts; )
						{
							for ( uint j( 0U ); j != 3U; ++j, ++i )
							{
								lfvi = triVertsMap[ i ];
								vIndex = triVerts[ i ];

								faceSets.AddToTail( positionIndices.Count() );

								normalIndices.AddToTail( normalData.NormalDataIndex( pIndex, lfvi ) );
								positionIndices.AddToTail( vIndex );
							}

							faceSets.AddToTail( -1 );
						}
					}

					continue;
				}
				else
				{
					mwarn << baseMeshPath.partialPathName() << ".f[" << pIndex << "] is not a convex polygon without holes but doesn't have a valid triangulation, studiomdl result will likely be incorrect" << std::endl;
				}
			}
		}

		// lfv = localFaceVertex( Index | End )
		const int lfvEnd( pIt.polygonVertexCount() );

		if ( uvOffset >= 0 && pIt.hasUVs( uvSet ) && pIt.hasColor() )
		{
			// Vertices, normals, UVs & Colors!
			for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi )
			{
				vIndex = pIt.vertexIndex( lfvi );

				faceSets.AddToTail( positionIndices.Count() );

				normalIndices.AddToTail( normalData.NormalDataIndex( pIndex, lfvi ) );
				positionIndices.AddToTail( vIndex );
				pIt.getUVIndex( lfvi, uvIndex, &uvSet );
				uvIndices.AddToTail( uvIndexMap[ uvIndex ] + uvOffset );
				pIt.getColorIndex( lfvi, colorIndex );
				colorIndices.AddToTail( colorIndex );
			}
		}
		else if ( uvOffset >= 0 && pIt.hasUVs( uvSet ) )
		{
			for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi )
			{
				vIndex = pIt.vertexIndex( lfvi );

				faceSets.AddToTail( positionIndices.Count() );

				normalIndices.AddToTail( normalData.NormalDataIndex( pIndex, lfvi ) );
				positionIndices.AddToTail( vIndex );
				pIt.getUVIndex( lfvi, uvIndex, &uvSet );
				uvIndices.AddToTail( uvIndexMap[ uvIndex ] + uvOffset );
			}
		}
		else if ( pIt.hasColor() )
		{
			for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi )
			{
				vIndex = pIt.vertexIndex( lfvi );

				faceSets.AddToTail( positionIndices.Count() );

				normalIndices.AddToTail( normalData.NormalDataIndex( pIndex, lfvi ) );
				positionIndices.AddToTail( vIndex );
				pIt.getColorIndex( lfvi, colorIndex );
				colorIndices.AddToTail( colorIndex );
			}
		}
		else
		{
			// Just vertices & normals
			for ( int lfvi( 0 ); lfvi < lfvEnd; ++lfvi )
			{
				vIndex = pIt.vertexIndex( lfvi );

				faceSets.AddToTail( positionIndices.Count() );

				normalIndices.AddToTail( normalData.NormalDataIndex( pIndex, lfvi ) );
				positionIndices.AddToTail( vIndex );
			}
		}

		faceSets.AddToTail( -1 );
	}

	pFaceSet->AddIndices( faceSets.Count() );
	pFaceSet->SetIndices( 0, faceSets.Count(), faceSets.Base() );
}


//-----------------------------------------------------------------------------
//
// Map of uvOffsets & uvIndexMaps used in function below
//
//-----------------------------------------------------------------------------
class CUvInfo
{
public:
	CUvInfo()
	: m_uvOffset( -1 )
	{}

	CUvInfo( const CUvInfo &rhs )
	{
		m_uvIndexMap = rhs.m_uvIndexMap;
		m_uvOffset = rhs.m_uvOffset;
	}

	CUtlVector< int > m_uvIndexMap;
	int m_uvOffset;
};


//-----------------------------------------------------------------------------
//
// Converts all face sets on a Maya mesh to their Dme equivalents
//
//-----------------------------------------------------------------------------
void CDmeExport::MayaFaceSetsToDmeFaceSets(
	const MDagPath &meshPath,
	const MDagPath &baseMeshPath,
	const CNormalData &normalData,
	CDmeMesh *pDmeMesh,
	CUtlVector< int > &positionIndices,
	CUtlVector< int > &normalIndices,
	CUtlVector< int > &uvIndices,
	CUtlVector< Vector2D > &uvData,
	CUtlVector< int > &colorIndices ) const
{
	const MMatrix m( baseMeshPath.inclusiveMatrix() );

	MObjectArray shaders;
	MIntArray faceShaderIndices;

	MFnMesh meshFn( meshPath );
	meshFn.getConnectedShaders( meshPath.instanceNumber(), shaders, faceShaderIndices );

	// Build an MFnSingleIndexedComponent Object for each shader plus one for
	// faces which have no shader.  Since 'faceShaderIndices' will contain -1
	// for faces without a shading group assignment, offset the indices by
	// +1.  If Maya's lying about -1 and it's -2 or something, things will be bad

	CUtlVector< MFnSingleIndexedComponent * > faceSetFns;

	if ( ( m_type & kPhysicsModel ) != 0 )
	{
		// Put them all in faceSetFns[ 0 ] for no shading group!
		faceSetFns.AddToTail( new MFnSingleIndexedComponent );
		faceSetFns.Tail()->setCompleteData( meshFn.numPolygons() );
	}
	else
	{
		// Create shaders.length() + 1 MFnSingleIndexedComponents for each faceSet
		const uint shaderEnd( shaders.length() );
		for ( uint shaderIndex( 0 ); shaderIndex <= shaderEnd; ++shaderIndex )
		{
			// faceSetFns[ 0 ] is for faces with no shading group assigned
			// faceSetFns[ n ] is for shader n - 1
			faceSetFns.AddToTail( new MFnSingleIndexedComponent );
			faceSetFns.Tail()->create( MFn::kMeshPolygonComponent );
		}

		// Now loop through faceShaderIndices and construct the single indexed components
		// this way, it's guaranteed that every face is accounted for
		{
			const uint faceEnd( faceShaderIndices.length() );
			Assert( faceEnd == static_cast< uint >( meshFn.numPolygons() ) );

			for ( uint faceIndex( 0 ); faceIndex != faceEnd; ++faceIndex )
			{
				faceSetFns[ faceShaderIndices[ faceIndex ] + 1 ]->addElement( faceIndex );
			}
		}
	}

	std::map< std::string, CUvInfo > uvInfoMap;

	// Now loop through all of the face sets which aren't empty and
	// create a DmeFaceSet for them

	const uint faceSetFnsEnd( faceSetFns.Count() );
	for ( uint faceSetFnsIndex( 0 ); faceSetFnsIndex != faceSetFnsEnd; ++faceSetFnsIndex )
	{
		const MFnSingleIndexedComponent &faceSetFn( *faceSetFns[ faceSetFnsIndex ] );
		if ( faceSetFn.isEmpty() )
		{
			// Ignore empty
			continue;
		}

		MString faceSetName;
		MString materialName;
		MString materialPath;
		MString uvSet;
		MString debugWhy;

		meshFn.getCurrentUVSetName( uvSet );

		if ( faceSetFnsIndex == 0 )
		{
			// 0 is for faces without a shader...
			faceSetName = ( ( m_type & kPhysicsModel ) == 0 ) ? "Unassigned" : "Physics";
			materialName = faceSetName + "Material";
			materialPath = "debug/debugempty";
		}
		else
		{
			GetMaterialInfo( shaders[ faceSetFnsIndex - 1 ],
				meshPath.fullPathName(),
				faceSetName, materialName, materialPath, uvSet, debugWhy );
		}

		// Create the CDmeFaceSet
		CDmeFaceSet *pDmeFaceSet( CreateElement< CDmeFaceSet >( faceSetName.asChar(), FileId() ) );
		if ( !pDmeFaceSet )
		{
			Error( "Cannot create DmeFaceSet \"%s\"\n", faceSetName.asChar() );
			continue;
		}

		const CUtlVector< int > *pUvIndexMap( NULL );
		int uvOffset( 0 );
		std::map< std::string, CUvInfo >::const_iterator uvi( uvInfoMap.find( uvSet.asChar() ) );
		if ( uvi == uvInfoMap.end() )
		{
			CUvInfo &uvInfo( uvInfoMap[ uvSet.asChar() ] );

			uvInfo.m_uvOffset = AddUVData( baseMeshPath, uvSet, uvData, uvInfo.m_uvIndexMap );
			uvOffset = uvInfo.m_uvOffset;
			pUvIndexMap = &uvInfo.m_uvIndexMap;
		}
		else
		{
			const CUvInfo &uvInfo( uvi->second );
			uvOffset = uvInfo.m_uvOffset;
			pUvIndexMap = &uvInfo.m_uvIndexMap;
		}

		MayaFaceSetToDmeFaceSet( meshPath, baseMeshPath, normalData, faceSetFn.object(), uvSet, pDmeFaceSet,
			positionIndices, normalIndices, uvIndices, *pUvIndexMap, uvOffset, colorIndices );

		CDmeMaterial *pMaterial( CreateElement< CDmeMaterial >( materialName.asChar(), FileId() ) );
		if ( materialPath == "debug/debugempty" || debugWhy.length() )
		{
			pMaterial->SetValue( "debugWhy", debugWhy.asChar() );
		}

		pMaterial->SetMaterial( materialPath.asChar() );
		pDmeFaceSet->SetMaterial( pMaterial );
		pDmeMesh->AddFaceSet( pDmeFaceSet );
	}

	// Delete all of the MFnSingleIdexedComponents
	faceSetFns.PurgeAndDeleteElements();
}


//-----------------------------------------------------------------------------
// Save the bind pose for a transform or joint
//-----------------------------------------------------------------------------
void CDmeExport::SaveBindPose(
	const MFnDagNode &dagFn,
	CDmeDag *pDmeDag,
	CDmeTransform *pDmeTransform ) const
{
	CDmAttribute *pBindTransformAttr = pDmeTransform->AddAttributeElement<CDmeTransform>( "__bind" );

	pBindTransformAttr->AddFlag( FATTRIB_DONTSAVE );

	CDmeTransform *pBindTransform(
		CreateElement< CDmeTransform >( pDmeDag->GetName(), FileId() ) );

	pBindTransformAttr->SetValue( pBindTransform->GetHandle() );

	// Copy the current data to the bind pose initially

	pBindTransform->SetOrientation( pDmeTransform->GetOrientation() );
	pBindTransform->SetPosition( pDmeTransform->GetPosition() );

	// Find the bind pose data for the joint and save that information
	// If there's no explicit bind pose set, issue a warning and use the
	// current position as the bind pose

	// If the model is not skinned we do not need any bindPose information

	if ( m_optBindCurrent )
	{
		MPlug mPlug = dagFn.findPlug( "matrix" );

		MStatus mStatus;
		MObject xformMatrixVal;

		mStatus = mPlug.getValue( xformMatrixVal );
		if ( mStatus )
		{
			const MFnMatrixData xformMatrixFn( xformMatrixVal );
			const MTransformationMatrix bindPoseMat = xformMatrixFn.matrix();

			// Compiler cannot handle MQuaternion r = bindPoseMat.asRotateMatrix() ?!?!?!
			MQuaternion r;
			r = bindPoseMat.asRotateMatrix();

			const MVector t( bindPoseMat.getTranslation( MSpace::kTransform ) );

			pBindTransform->SetOrientation( Quaternion( r.x, r.y, r.z, r.w ) );
			pBindTransform->SetPosition( Vector( t.x, t.y, t.z ) );
		}
		else
		{
			mwarn << "Can't get value from maya attribute " << mPlug.info() << std::endl;
		}

		return;
	}

	MPlugArray mPlugArray;
	MPlug mPlug( dagFn.findPlug( "bindPose" ) );
	if ( !mPlug.isNull() && mPlug.connectedTo( mPlugArray, false, true ) && mPlugArray.length() )
	{
		if ( mPlugArray.length() > 1 )
		{
			mwarn << "Joint " << dagFn.name() << " has more than one bind pose, using first pose" << std::endl;
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
			mwarn << "Can't get value from maya attribute " << xformMatrixPlug.info() << std::endl;
		}
	}
	else
	{
		mwarn << "No bind pose set on " << dagFn.type() << " " << dagFn.name() << std::endl;
	}
}


//-----------------------------------------------------------------------------
// Checks whether the passed Maya node is a skinCluster influence object
//-----------------------------------------------------------------------------
bool CDmeExport::IsInfluenceObject(
	const MDagPath &mDagPath )
{
	MPlug wmAP( MFnDagNode( mDagPath ).findPlug( "worldMatrix" ) );
	if ( wmAP.isNull() )
		return false;

	MPlugArray mPlugArray;

	const unsigned wmEnd( wmAP.evaluateNumElements() );
	for ( unsigned wmi( 0 ); wmi != wmEnd; ++wmi )
	{
		if ( wmAP[ wmi ].connectedTo( mPlugArray, false, true ) && mPlugArray.length() > 0 )
		{
			const unsigned nPlugs( mPlugArray.length() );
			for ( unsigned pi( 0U ); pi != nPlugs; ++pi )
			{
				if ( mPlugArray[ pi ].node().apiType() == MFn::kSkinClusterFilter )
					return true;
			}
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
// Checks whether the passed Maya node is a skinCluster influence object
//-----------------------------------------------------------------------------
MString CDmeExport::GetNodeName(
	const MDagPath &mDagPath )
{
	const MFnDagNode dagFn( mDagPath );
	MStatus mStatus;

	MPlug mPlug( dagFn.findPlug( "vstName", false, &mStatus ) );
	if ( mStatus && !mPlug.isNull() )
	{
		MString vstName;
		if ( mPlug.getValue( vstName ) && vstName.length() )
		{
			return vstName;
		}
	}

	return ValveMaya::RemoveNameSpace( dagFn.name() );
}