//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of ValveMaya::CDmeImport
//
//=============================================================================

// Maya includes
#include <maya/MAnimControl.h>
#include <maya/MArgDatabase.h>
#include <maya/MColorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MEulerRotation.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnSet.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnTransform.h>
#include <maya/MIntArray.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MSelectionList.h>
#include <maya/MTime.h>
#include <maya/MTimeArray.h>
#include <maya/MQuaternion.h>
#include <maya/MVectorArray.h>

// Valve includes
#include "movieobjects/dmeanimationlist.h"
#include "movieobjects/dmechannel.h"
#include "movieobjects/dmedag.h"
#include "movieobjects/dmecombinationoperator.h"
#include "movieobjects/dmefaceset.h"
#include "movieobjects/dmejoint.h"
#include "movieobjects/dmematerial.h"
#include "movieobjects/dmemesh.h"
#include "movieobjects/dmemodel.h"
#include "tier1/utlqueue.h"
#include "valveMaya.h"

// Local includes
#include "DmeImport.h"



const char *const CDmeImport::m_sOptImportType = "it";
const char *const CDmeImport::m_sLongOptImportType = "importType";
const char *const CDmeImport::m_sOptSetTimeline = "st";
const char *const CDmeImport::m_sLongOptSetTimeline = "setTimeline";


// Local classes

//-----------------------------------------------------------------------------
// Purpose: Link a material name and path to a set of indices
//-----------------------------------------------------------------------------
class CShadingGroup
{
public:
	CShadingGroup()
	: m_materialName( "debugEmpty" )
	, m_materialPath( "debug/debugEmpty" )
	{}

	MString m_materialName;
	MString m_materialPath;
	MObject m_faceListObj;
};


//-----------------------------------------------------------------------------
// Constructor for CDmeImport
//-----------------------------------------------------------------------------
CDmeImport::CDmeImport(
	ValveMaya::CUndo *pUndo )
: m_pUndo( pUndo ? pUndo : new ValveMaya::CUndo )
, m_bOwnUndo( pUndo ? false : true )
{}


//-----------------------------------------------------------------------------
// Destructor for CDmeImport
//-----------------------------------------------------------------------------
CDmeImport::~CDmeImport()
{
	if ( m_pUndo && m_bOwnUndo )
	{
		delete m_pUndo;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MDagPath GetFistSpecifiedDag( const MArgDatabase &mArgDatabase, MStatus *pStatus = NULL )
{
	MSelectionList mSelectionList;
	if ( mArgDatabase.getObjects( mSelectionList ) )
	{
		for ( MItSelectionList sIt( mSelectionList ); !sIt.isDone(); sIt.next() )
		{
			if ( sIt.itemType() == MItSelectionList::kDagSelectionItem )
			{
				MDagPath mDagPath;
				if ( sIt.getDagPath( mDagPath ) )
				{
					if ( pStatus )
					{
						*pStatus = MS::kFailure;
					}

					return mDagPath;
				}
			}
		}
	}

	if ( pStatus )
	{
		*pStatus = MS::kFailure;
	}

	return MDagPath();
}


//-----------------------------------------------------------------------------
// Perform a conversion from Dme to Maya for a whole scene
//-----------------------------------------------------------------------------
MDagPath CDmeImport::DoIt(
	const MArgDatabase &mArgDatabase,
	CDmElement *pDmeRoot )
{
	// Just in case things are called twice, clear out any state
	// The same MDagModifier will be used for all calls
	m_mDagRoot = MDagPath();
	m_dmeModelStack.Clear();
	m_shadingGroups.Clear();

	m_type = 0U;
	if ( mArgDatabase.isFlagSet( m_sOptImportType ) )
	{
		MString importType;
		mArgDatabase.getFlagArgument( m_sOptImportType, 0, importType );
		if ( StringHasPrefix( importType.asChar(), "m" ) )
		{
			m_type |= kModel;
		}
		else if ( StringHasPrefix( importType.asChar(), "a" ) )
		{
			m_type |= kAnimation;
		}
	}

	Assert( pDmeRoot );

	CDmeDag *pModel = pDmeRoot->GetValueElement< CDmeDag >( "model" );
	CDmeDag *pSkeleton = pDmeRoot->GetValueElement< CDmeDag >( "skeleton" );
	CDmeCombinationOperator *pDmeComboOp = pDmeRoot->GetValueElement< CDmeCombinationOperator >( "combinationOperator" );
	CDmeAnimationList *pAnimationList = pDmeRoot->GetValueElement< CDmeAnimationList >( "animationList" );

	if ( ( m_type & kModel ) || ( !( m_type & kAnimation ) && pModel ) )
	{
		if ( pModel )
		{
			DmeDagToMayaDag( pModel );

			if ( pSkeleton && ( pSkeleton != pModel ) )
			{
				DmeDagToMayaDag( pSkeleton );
			}

			if ( pDmeComboOp )
			{
				// minfo << " * TODO: Import DmeComboOp: " << pDmeComboOp->GetName() << std::endl;
			}
		}
		else
		{
			merr << "vsDmxIO Import: -importType model specified but no model found to import" << std::endl;
		}
	}

	// Assign Materials
	for ( int i = 0; i < m_shadingGroups.GetNumStrings(); ++i )
	{
		// I think just m_shadingGroups[ i ] would also work but it's not clear that it would
		m_shadingGroups[ m_shadingGroups.String( i ) ].Create( m_pUndo );
	}


	if ( ( m_type & kAnimation ) || ( !( m_type & kModel ) && ( pAnimationList && pSkeleton ) ) )
	{
		if ( pAnimationList )
		{
			if ( pSkeleton )
			{
				double optFs = MAnimControl::minTime().as( MTime::uiUnit() );
				double optFo = 0.0;

				if ( mArgDatabase.isFlagSet( "fs" ) )
				{
					mArgDatabase.getFlagArgument( "fs", 0, optFs );
				}

				if ( mArgDatabase.isFlagSet( "fo" ) )
				{
					mArgDatabase.getFlagArgument( "fo", 0, optFo );
				}

				const bool optKfr(
					mArgDatabase.isFlagSet( "fo" ) ||
					mArgDatabase.isFlagSet( "kfr" ) );

				const MTime mTo( optKfr ? -optFo : optFs, MTime::uiUnit() );

				m_mDagRoot = GetFistSpecifiedDag( mArgDatabase );
				ProcessAnimation( m_mDagRoot, pSkeleton, pAnimationList, mTo, mArgDatabase.isFlagSet( m_sOptSetTimeline ) );
			}
			else
			{
				merr << "vsDmxIO Import: -importType animation specified but no skeleton found to import" << std::endl;
			}
		}
		else
		{
			merr << "vsDmxIO Import: -importType animation specified but no animation found to import" << std::endl;
		}
	}

	return m_mDagRoot;
}


//-----------------------------------------------------------------------------
// Convert the given DmeDag node (and its children) to the Maya equivalents
//-----------------------------------------------------------------------------
void CDmeImport::DmeDagToMayaDag(
	CDmeDag *pDmeDag,
	const MObject &parentObj )
{
	MString mayaNodeType( "transform" );

	CDmeModel *pDmeModel( NULL );

	// Figure out what type of Maya node to create for this node

	if ( pDmeDag->IsA( CDmeModel::GetStaticTypeSymbol() ) )
	{
		mayaNodeType = "vstInfo";
		pDmeModel = CastElement< CDmeModel >( pDmeDag );
		m_dmeModelStack.Push( pDmeModel );
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

	MObject thisObj( m_pUndo->DagModifier().createNode( mayaNodeType, parentObj ) );
	m_pUndo->DagModifierDoIt();

	if ( thisObj.isNull() )
	{
		merr << "Couldn't convert DmeDag node " << pDmeDag->GetName() << " into a Maya node" << std::endl;
		return;
	}

	m_pUndo->DagModifier().renameNode( thisObj, pDmeDag->GetName() );
	m_pUndo->DagModifierDoIt();

	// If this is the first Maya node created, then it's got to be the root
	if ( !m_mDagRoot.isValid() )
	{
		MDagPath::getAPathTo( thisObj, m_mDagRoot );
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
	DmeMeshToMayaMesh( CastElement< CDmeMesh >( pDmeShape ), thisObj );

	// TODO: Restore any extra recorded attributes

	// Handle any children of this node

	const int cEnd( pDmeDag->GetChildCount() );
	for ( int ci( 0 ); ci < cEnd; ++ci )
	{
		DmeDagToMayaDag( pDmeDag->GetChild( ci ), thisObj );
	}

	if ( pDmeModel )
	{
		// TODO - Bind

		Assert( m_dmeModelStack.Count() && m_dmeModelStack.Top() == pDmeModel );
		m_dmeModelStack.Pop();
	}
}


//-----------------------------------------------------------------------------
// Given a set of Dme normals, convert and set them to the Maya equivalent
//-----------------------------------------------------------------------------
void CDmeImport::DoNormals(
	CDmeVertexData *pDmeVertexData,
	MFnMesh &meshFn )
{
	// Get the Normal data from Dme
	const CUtlVector< int > &positionIndices( pDmeVertexData->GetVertexIndexData( CDmeVertexDataBase::FIELD_POSITION ) );
	if ( positionIndices.Count() == 0 )
		return;

	const CUtlVector< int > &normalIndices( pDmeVertexData->GetVertexIndexData( CDmeVertexDataBase::FIELD_NORMAL ) );
	if ( normalIndices.Count() != positionIndices.Count() )
		return;

	const CUtlVector< Vector > &normals( pDmeVertexData->GetNormalData() );
	if ( normals.Count() == 0 )
		return;

	// Scan through nornm
	// Set the normals
	const FieldIndex_t nIndex( pDmeVertexData->FindFieldIndex( "normals" ) );

	MVectorArray mNormalArray;
	if ( nIndex >= 0 )
	{
		const CDmrArray< Vector > normals( pDmeVertexData->GetVertexData( nIndex ) );

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
}

//-----------------------------------------------------------------------------
// Convert the given DmeMesh to a Maya Mesh
//-----------------------------------------------------------------------------
void CDmeImport::DmeMeshToMayaMesh( CDmeMesh *pDmeMesh,	const MObject &parentObj )
{
	// Need something to convert and a valid Maya parent

	if ( !pDmeMesh || parentObj.isNull() )
		return;

	CDmeVertexData *pBindVertexData( pDmeMesh->FindBaseState( "bind" ) );
	const FieldIndex_t pIndex( pBindVertexData->FindFieldIndex( "positions" ) );

	if ( pIndex < 0 )
	{
		merr << "No positions in DmeMesh " << pDmeMesh->GetName() << ", cannot convert to Maya" << std::endl;
		return;
	}

	// Suck the vertices out of Dme

	MFloatPointArray mVertexArray;
	{
		// Copy Dme Data to Maya format
		CDmeVertexData *pBindVertexData = pDmeMesh->FindBaseState( "bind" );
		const CUtlVector< Vector > &positions( pBindVertexData->GetPositionData() );

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

	const FieldIndex_t uvIndex( pBindVertexData->FindFieldIndex( "textureCoordinates" ) );

	MFloatArray muArray;
	MFloatArray mvArray;

	if ( uvIndex >= 0 )
	{
		const CDmrArray< Vector2D > uvs( pBindVertexData->GetVertexData( uvIndex ) );

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

	// Suck the colors out of Dme

	const FieldIndex_t cIndex( pBindVertexData->FindFieldIndex( "colors" ) );

	MColorArray mColorArray;
	if ( cIndex >= 0 )
	{
		const CDmrArray< Color > colors( pBindVertexData->GetVertexData( cIndex ) );

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

	// For storing material names
	char buf0[ MAX_PATH ];
	char buf1[ MAX_PATH ];

	{
		int faceCount( 0 );			// Number of faces & size of mPolygonCounts
		int faceVaryingCount( 0 );	// Sum of all the values in mPolygonCounts & size of mPolygonConnects

		// Make a quick pass to figure out how much data there is

		const uint fsEnd( pDmeMesh->FaceSetCount() );
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

		const CDmrArray<int> indices( pBindVertexData->GetIndexData( pIndex ) );

		if ( uvIndex >= 0 )
		{
			const CDmrArray<int> uvIndices( pBindVertexData->GetIndexData( uvIndex ) );
			muvIds.setLength( faceVaryingCount );

			Assert( static_cast<int>( muvIds.length() ) == faceVaryingCount );

			int *puvIds( &muvIds[ 0 ] );

			int dmeFaceIndex( 0 );

			for ( uint fsi( 0 ); fsi != fsEnd; ++fsi )
			{
				CDmeFaceSet *pDmeFaceSet( pDmeMesh->GetFaceSet( fsi ) );

				// Put a new shading group on the list
				CShadingGroup &shadingGroup = shadingGroups[ shadingGroups.AddToTail( CShadingGroup() ) ];

				// Record information for the shading group

				CDmeMaterial *pDmeMaterial = pDmeFaceSet->GetMaterial();
				if ( pDmeMaterial )
				{
					Q_StripExtension( pDmeMaterial->GetName(), buf0, sizeof( buf0 ) );
					Q_FixSlashes( buf0 );
					Q_StripTrailingSlash( buf0 );
					Q_FileBase( buf0, buf1, sizeof( buf1 ) );

					shadingGroup.m_materialName = buf1;

					Q_StripExtension( pDmeMaterial->GetMaterialName(), buf0, sizeof( buf0 ) );
					Q_FixSlashes( buf0 );
					Q_StripTrailingSlash( buf0 );
					Q_FixSlashes( buf0, '/' );

					shadingGroup.m_materialPath = buf0;
				}

				MIntArray materialFaces;
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
						*pFaceConnects++ = indices[ *pSrc ];
						*puvIds++ = uvIndices[ *pSrc ];
					}
				}

				// Create SingleIndexedComponent of Faces
				MFnSingleIndexedComponent sicFn;
				shadingGroup.m_faceListObj = sicFn.create( MFn::kMeshPolygonComponent );
				sicFn.addElements( materialFaces );
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
		merr << "Couldn't create Maya mesh from DmeMesh " << pDmeMesh->GetName() << std::endl;
		return;
	}

	m_pUndo->DagModifier().renameNode( meshObj, pDmeMesh->GetName() );
	m_pUndo->DagModifierDoIt();

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
			merr << "No UV set on " << meshFn.name() << std::endl;
		}
	}

	DoNormals( pBindVertexData, meshFn );

	// Add the shading groups

	{
		MDagPath meshDagPath;
		MDagPath::getAPathTo( meshObj, meshDagPath );

		const int sgEnd = shadingGroups.Count();
		for ( int sgi = 0; sgi < sgEnd; ++sgi )
		{
			CShadingGroup &sg = shadingGroups[ sgi ];
			CMultiShadingGroup &msg = m_shadingGroups[ sg.m_materialPath.asChar() ];
			msg.m_materialName = sg.m_materialName;
			msg.m_materialPath = sg.m_materialPath;
			msg.m_mSelectionList.add( meshDagPath, sg.m_faceListObj );
		}
	}
}

//-----------------------------------------------------------------------------
// Compare the name of a Maya Dag node to a DmeDag node, return true if they
// are the same, false otherwise
//-----------------------------------------------------------------------------
bool CompareMayaDmeDag( const MDagPath &mDagPath, const CDmeDag *pDmeDag )
{
	if ( !Q_stricmp( MFnDagNode( mDagPath ).name().asChar(), pDmeDag->GetName() ) )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
// Returns true if the passed DmeDag and MDagPath have a child with the same
// name, false otherwise.  If true, the passed MDagPath is modified to refer
// to the matching child
//-----------------------------------------------------------------------------
bool FindMatchingMayaChild( CDmeDag *pDmeDag, const MDagPath &mDagPath, MDagPath &cDagPath )
{
	MDagPath tDagPath;
	for ( uint i = 0; i < mDagPath.childCount(); ++i )
	{
		tDagPath = mDagPath;
		tDagPath.push( mDagPath.child( i ) );
		if ( CompareMayaDmeDag( tDagPath, pDmeDag ) )
		{
			cDagPath = tDagPath;
			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
class CMayaDmeAnimData
{
public:
	CMayaDmeAnimData( const MDagPath &mDagPath, CDmeDag *pDmeDag )
	: m_mDagPath( mDagPath )
	, m_pDmeDag( pDmeDag )
	{
	}

	bool IsValid() const { return m_pDmeDag && m_mDagPath.isValid() && m_mDagPath.length(); }

	const MDagPath m_mDagPath;		// Maya Dag
	CDmeDag *const m_pDmeDag;	// Dme Dag
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MatchDags( CUtlVector< CMayaDmeAnimData > &mayaDmeAnimDataList, const MDagPath &mDagPath, CDmeDag *pDmeDag )
{
	CDmeDag *pDmeDagChild;
	MDagPath cDagPath;

	bool bFoundMatch;

	for ( int i = pDmeDag->GetChildCount() - 1; i >= 0; --i )
	{
		pDmeDagChild = pDmeDag->GetChild( i );
		bFoundMatch = false;
		for ( uint j = 0; j < mDagPath.childCount(); ++j )
		{
			cDagPath = mDagPath;
			cDagPath.push( mDagPath.child( j ) );
			if ( CompareMayaDmeDag( cDagPath, pDmeDagChild ) )
			{
				mayaDmeAnimDataList.AddToTail( CMayaDmeAnimData( cDagPath, pDmeDagChild ) );
				MatchDags( mayaDmeAnimDataList, cDagPath, pDmeDagChild );
				bFoundMatch = true;
				break;
			}
		}

		if ( !bFoundMatch )
		{
			mayaDmeAnimDataList.AddToTail( CMayaDmeAnimData( MDagPath(), pDmeDagChild ) );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CDmeDag *FindFirstMatchingDmeChild( const MDagPath &mDagPath, CDmeDag *pDmeDag )
{
	const MString name( MFnDagNode( mDagPath ).name() );
	const char *const pName = name.asChar();

	CUtlQueue< CDmeDag * > bfq;	// Breadth First Queue
	bfq.Insert( pDmeDag );

	while ( bfq.Count() )
	{
		pDmeDag = bfq.RemoveAtTail();

		if ( !Q_stricmp( pName, pDmeDag->GetName() ) )
			return pDmeDag;

		for ( int i = 0; i < pDmeDag->GetChildCount(); ++i )
		{
			bfq.Insert( pDmeDag->GetChild( i ) );
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDmeImport::CreateTranslationCurves( const MDagPath &mDagPath, const CDmeVector3Log *pTLog, const MTime &mTimeOffset, MTime &minTime, MTime &maxTime )
{
	MObject xObj = m_pUndo->DagModifier().MDGModifier::createNode( "animCurveTL" );
	MObject yObj = m_pUndo->DagModifier().MDGModifier::createNode( "animCurveTL" );
	MObject zObj = m_pUndo->DagModifier().MDGModifier::createNode( "animCurveTL" );
	m_pUndo->DagModifierDoIt();

	m_pUndo->DagModifier().renameNode( xObj, MString( pTLog->GetName() ) + "x" );
	m_pUndo->DagModifier().renameNode( yObj, MString( pTLog->GetName() ) + "y" );
	m_pUndo->DagModifier().renameNode( zObj, MString( pTLog->GetName() ) + "z" );
	m_pUndo->DagModifierDoIt();

	const int nKeys = pTLog->GetKeyCount();

	MTimeArray times( nKeys, MTime() );
	MDoubleArray xVals( nKeys );
	MDoubleArray yVals( nKeys );
	MDoubleArray zVals( nKeys );
	for ( int i = 0; i < nKeys; ++i )
	{
		DmeTime_t t;
		times[ i ] = MTime( pTLog->GetKeyTime( i ).GetSeconds(), MTime::kSeconds ) + mTimeOffset;
		const Vector &v = pTLog->GetKeyValue( i );
		xVals[ i ] = v.x;
		yVals[ i ] = v.y;
		zVals[ i ] = v.z;
	}

	MFnAnimCurve xFn( xObj ); xFn.addKeys( &times, &xVals );
	MFnAnimCurve yFn( yObj ); yFn.addKeys( &times, &yVals );
	MFnAnimCurve zFn( zObj ); zFn.addKeys( &times, &zVals );

	const MFnDependencyNode depFn( mDagPath.transform() );

	m_pUndo->Connect( xFn.findPlug( "output" ), depFn.findPlug( "translateX" ), true );
	m_pUndo->Connect( yFn.findPlug( "output" ), depFn.findPlug( "translateY" ), true );
	m_pUndo->Connect( zFn.findPlug( "output" ), depFn.findPlug( "translateZ" ), true );

	minTime = times[ 0 ];
	maxTime = times[ nKeys - 1 ];
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDmeImport::CreateOrientationCurves( const MDagPath &mDagPath, const CDmeQuaternionLog *pOLog, const MTime &mTimeOffset, MTime &minTime, MTime &maxTime )
{
	MFnTransform tFn( mDagPath.transform() );
	MTransformationMatrix tm = tFn.transformation();
	MQuaternion iq;
	tm.setRotationQuaternion( iq.x, iq.y, iq.z, iq.w );
	const MMatrix im = tm.asMatrixInverse();
	iq = im;

//	const MMatrix im = tFn.transformationMatrix().inverse();
//	MQuaternion iq;
//	iq = im;

	// Figure out the matrix 
	MObject xObj = m_pUndo->DagModifier().MDGModifier::createNode( "animCurveTA" );
	MObject yObj = m_pUndo->DagModifier().MDGModifier::createNode( "animCurveTA" );
	MObject zObj = m_pUndo->DagModifier().MDGModifier::createNode( "animCurveTA" );
	m_pUndo->DagModifierDoIt();

	m_pUndo->DagModifier().renameNode( xObj, MString( pOLog->GetName() ) + "x" );
	m_pUndo->DagModifier().renameNode( yObj, MString( pOLog->GetName() ) + "y" );
	m_pUndo->DagModifier().renameNode( zObj, MString( pOLog->GetName() ) + "z" );
	m_pUndo->DagModifierDoIt();

	const int nKeys = pOLog->GetKeyCount();

	MEulerRotation::RotationOrder rotationOrder = MEulerRotation::RotationOrder( tFn.rotationOrder() - MTransformationMatrix::kXYZ );
	MEulerRotation oldEuler;
	oldEuler.reorder( rotationOrder );

	MTimeArray times( nKeys, MTime() );
	MDoubleArray xVals( nKeys );
	MDoubleArray yVals( nKeys );
	MDoubleArray zVals( nKeys );
	for ( int i = 0; i < nKeys; ++i )
	{
		DmeTime_t t;
		times[ i ] = MTime( pOLog->GetKeyTime( i ).GetSeconds(), MTime::kSeconds ) + mTimeOffset;
		const Quaternion &q = pOLog->GetKeyValue( i );
		MEulerRotation e = ( MQuaternion( q.x, q.y, q.z, q.w ) * iq ).asEulerRotation();
		e.reorderIt( rotationOrder );
		e = MEulerRotation::closestSolution( e, oldEuler );

		xVals[ i ] = e.x;
		yVals[ i ] = e.y;
		zVals[ i ] = e.z;

		oldEuler = e;
	}

	MFnAnimCurve xFn( xObj ); xFn.addKeys( &times, &xVals );
	MFnAnimCurve yFn( yObj ); yFn.addKeys( &times, &yVals );
	MFnAnimCurve zFn( zObj ); zFn.addKeys( &times, &zVals );

	const MFnDependencyNode depFn( mDagPath.transform() );

	m_pUndo->Connect( xFn.findPlug( "output" ), depFn.findPlug( "rotateX" ), true );
	m_pUndo->Connect( yFn.findPlug( "output" ), depFn.findPlug( "rotateY" ), true );
	m_pUndo->Connect( zFn.findPlug( "output" ), depFn.findPlug( "rotateZ" ), true );

	minTime = times[ 0 ];
	maxTime = times[ nKeys - 1 ];
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CDmeImport::ProcessAnimation(
	const MDagPath &mDagRoot,
	CDmeDag *pSkeleton,
	CDmeAnimationList *pAnimationList,
	const MTime &mTimeOffset,
	bool bSetTimeSlider )
{
	CDmeDag *pDmeDag = FindFirstMatchingDmeChild( mDagRoot, pSkeleton );

	if ( !pDmeDag )
		return;

	// List of all Dme -> Maya pairings in Dme depth first order
	CUtlVector< CMayaDmeAnimData > mayaDmeAnimDataList;

	// Add the first one
	mayaDmeAnimDataList.AddToTail( CMayaDmeAnimData( mDagRoot, pDmeDag ) );

	MatchDags( mayaDmeAnimDataList, mDagRoot, pDmeDag );

	bool bSetTimes = true;
	MTime minTime;
	MTime maxTime;
	MTime tmpMinTime;
	MTime tmpMaxTime;

	for ( int i = 0; i < mayaDmeAnimDataList.Count(); ++i )
	{
		const CMayaDmeAnimData &animData = mayaDmeAnimDataList[ i ];

		if ( !animData.IsValid() )
			continue;

		const MDagPath &mDagPath = animData.m_mDagPath;
		CDmeDag *pDmeDag = animData.m_pDmeDag;
		CDmeTransform *pDmeTransform = pDmeDag->GetTransform();
		if ( !pDmeTransform )
			continue;

		const CDmeVector3Log *pTLog = NULL;
		const CDmeQuaternionLog *pOLog = NULL;

		DmAttributeReferenceIterator_t j = g_pDataModel->FirstAttributeReferencingElement( pDmeTransform->GetHandle() );
		while ( j != DMATTRIBUTE_REFERENCE_ITERATOR_INVALID )
		{
			CDmAttribute *pAttribute = g_pDataModel->GetAttribute( j );
			CDmElement *pDmeParent = pAttribute->GetOwner();
			if ( pDmeParent && !Q_stricmp( pAttribute->GetName(), "toElement" ) )
			{
				CDmeChannel *pChannel = CastElement< CDmeChannel >( pDmeParent );
				if ( pChannel )
				{
					if ( !pTLog && !Q_stricmp( pChannel->GetValueString( "toAttribute" ), "position" ) )
					{
						pTLog = CastElement< CDmeVector3Log >( pChannel->GetLog() );
					}
					else if ( !pOLog && !Q_stricmp( pChannel->GetValueString( "toAttribute" ), "orientation" ) )
					{
						pOLog = CastElement< CDmeQuaternionLog >( pChannel->GetLog() );
					}
				}
			}
			j = g_pDataModel->NextAttributeReferencingElement( j );
		}

		if ( pTLog )
		{
			CreateTranslationCurves( mDagPath, pTLog, mTimeOffset, tmpMinTime, tmpMaxTime );

			if ( bSetTimes )
			{
				minTime = tmpMinTime;
				maxTime = tmpMaxTime;
				bSetTimes = false;
			}
			else {
				if ( tmpMinTime < minTime )
				{
					minTime = tmpMinTime;
				}

				if ( tmpMaxTime < maxTime )
				{
					maxTime = tmpMaxTime;
				}
			}
		}

		if ( pOLog )
		{
			CreateOrientationCurves( mDagPath, pOLog, mTimeOffset, tmpMinTime, tmpMaxTime );

			if ( bSetTimes )
			{
				minTime = tmpMinTime;
				maxTime = tmpMaxTime;
				bSetTimes = false;
			}
			else {
				if ( tmpMinTime < minTime )
				{
					minTime = tmpMinTime;
				}

				if ( tmpMaxTime < maxTime )
				{
					maxTime = tmpMaxTime;
				}
			}
		}
	}

	if ( bSetTimeSlider )
	{
		MAnimControl::setMinMaxTime( minTime, maxTime );
		MAnimControl::setAnimationStartEndTime( minTime, maxTime );
	}
}


//-----------------------------------------------------------------------------
// Create shading groups
//-----------------------------------------------------------------------------
MStatus CDmeImport::CMultiShadingGroup::Create( ValveMaya::CUndo *pUndo )
{
	// Find defaultTextureList
	MObject renderUtilityListObj;
	MObject shaderListObj;
	MObject textureListObj;

	for ( MItDependencyNodes dIt; !dIt.isDone(); dIt.next() )
	{
		MObject mObj = dIt.thisNode();
		switch ( mObj.apiType() )
		{
		case MFn::kRenderUtilityList:
			renderUtilityListObj = mObj;
			break;
		case MFn::kShaderList:
			shaderListObj = mObj;
			break;
		case MFn::kTextureList:
			textureListObj = mObj;
			break;
		}
	}

	// Create the vsVmtToTex node
	MObject vsVmtToTexObj = pUndo->DagModifier().MDGModifier::createNode( "vsVmtToTex" );
	if ( !( pUndo->DagModifierDoIt() && !vsVmtToTexObj.isNull() ) )
	{
		merr << "vsDmxIO: Couldn't Create vsVmtToTex Node For Material: " << m_materialPath << std::endl;
		return MS::kFailure;
	}
	pUndo->DagModifier().renameNode( vsVmtToTexObj, m_materialName + "_vmt" );
	pUndo->DagModifierDoIt();

	const MFnDependencyNode vsVmtToTexFn( vsVmtToTexObj );
	MPlug materialPathP = vsVmtToTexFn.findPlug( "materialPath" );
	if ( materialPathP.isNull() )
		return MS::kFailure;

	materialPathP.setValue( m_materialPath );

	// Create the file node
	MObject fileObj = pUndo->DagModifier().MDGModifier::createNode( "file" );
	pUndo->DagModifierDoIt();
	pUndo->DagModifier().renameNode( fileObj, m_materialName + "_file" );
	pUndo->DagModifierDoIt();

	const MFnDependencyNode fileFn( fileObj );
	MPlug mP( fileFn.findPlug( "mirrorV" ) );
	mP.setValue( true );

	MFnSet shadingEngineFn;
	shadingEngineFn.addMembers( m_mSelectionList );

	// Create the shading engine
	MStatus mStatus;
	MObject shadingEngineObj = shadingEngineFn.create( m_mSelectionList, MFnSet::kRenderableOnly, &mStatus );

	// Create the phong surface shader
	const MObject phongObj = pUndo->DagModifier().MDGModifier::createNode( "phong" );
	pUndo->DagModifierDoIt();
	pUndo->DagModifier().renameNode( phongObj, m_materialName + "_mat" );
	pUndo->DagModifierDoIt();

	const MFnDependencyNode phongFn( phongObj );

	// Hook everything up
	pUndo->Connect( vsVmtToTexFn.findPlug( "texturePath" ), fileFn.findPlug( "fileTextureName" ), true );
	pUndo->Connect( fileFn.findPlug( "outColor" ), phongFn.findPlug( "color" ), true );
	pUndo->Connect( phongFn.findPlug( "outColor" ), shadingEngineFn.findPlug( "surfaceShader" ), true );

	if ( !renderUtilityListObj.isNull() )
	{
		MPlug renderUtilityP = MFnDependencyNode( renderUtilityListObj ).findPlug( "utilities" );
		renderUtilityP = renderUtilityP.elementByLogicalIndex( ValveMaya::NextAvailable( renderUtilityP ) );
		pUndo->Connect( vsVmtToTexFn.findPlug( "message" ), renderUtilityP, true );
	}

	if ( !shaderListObj.isNull() )
	{
		MPlug shaderListP = MFnDependencyNode( shaderListObj ).findPlug( "shaders" );
		shaderListP = shaderListP.elementByLogicalIndex( ValveMaya::NextAvailable( shaderListP ) );
		pUndo->Connect( phongFn.findPlug( "message" ), shaderListP, true );
	}

	if ( !textureListObj.isNull() )
	{
		MPlug textureListP = MFnDependencyNode( textureListObj ).findPlug( "textures" );
		textureListP = textureListP.elementByLogicalIndex( ValveMaya::NextAvailable( textureListP ) );
		pUndo->Connect( fileFn.findPlug( "message" ), textureListP, true );
	}

	return MS::kSuccess;
}