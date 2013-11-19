//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================

// Maya includes
#include <maya/MAngle.h>
#include <maya/MArgDatabase.h>
#include <maya/MAttributeIndex.h>
#include <maya/MAttributeSpecArray.h>
#include <maya/MAttributeSpec.h>
#include <maya/MBoundingBox.h>
#include <maya/MColor.h>
#include <maya/MDagPathArray.h>
#include <maya/MDistance.h>
#include <maya/MDrawData.h>
#include <maya/MEulerRotation.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MMaterial.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MPxGeometryIterator.h>
#include <maya/MQuaternion.h>
#include <maya/MSelectionList.h>
#include <maya/MSelectionMask.h>
#include <maya/MVector.h>


// Valve includes
#include "valveMaya.h"
#include "mathlib/mathlib.h"


// Local Includes
#include "vsSkinnerNode.h"


// Get rid of min() & max() macros as they interfere with
// MBoundingBox::min() & MBoundingBox::max() 
#if defined min
#undef min
#endif // defined min

#if defined max
#undef max
#endif // defined max


// Define template versions, which they should have been all along...
template < class T_t >
inline T_t min( const T_t &a, const T_t &b )
{
	return a < b ? a : b;
}


template < class T_t >
inline T_t max( const T_t &a, const T_t &b )
{
	return a > b ? a : b;
}


//=============================================================================
//
//=============================================================================
class CVsSkinnerUI : public MPxSurfaceShapeUI
{
public:
	CVsSkinnerUI();

	virtual void getDrawRequests(
		const MDrawInfo &mDrawInfo,
		bool objectAndActiveOnly,
		MDrawRequestQueue &mDrawRequestQueue );

	virtual void draw(
		const MDrawRequest &mDrawRequest,
		M3dView &m3dView ) const;

	virtual bool select(
		MSelectInfo &MSelectInfo,
		MSelectionList &mSelectionList,
		MPointArray &worldSpaceSelectPts ) const;

protected:
	bool SelectComponents(
		MSelectInfo &mSelectInfo,
		MSelectionList &mSelectionList,
		MPointArray &worldSpaceSelectPts ) const;

	void DoDraw(
		const MDagPath &mDagPath,
		M3dView &m3dView,
		short drawToken,
		const MDrawRequest *pDrawRequest = NULL ) const;

	void DrawUnitSphere() const;

	void DrawVolumes(
		M3dView &m3dView,
		const MDrawRequest *pDrawRequest = NULL ) const;

	enum {
		kDrawBoundingBox,
		kDrawVertices,
		kDrawWireframe,
		kDrawWireframeOnShaded,
		kDrawSmoothShaded,
		kDrawFlatShaded,
		kLastToken
	};
};


INSTALL_MAYA_MPXSHAPENODE(
	CVsSkinnerNode,
	vsSkinner,
	0x0010e4c7,
	CVsSkinnerNode::Initialize,
	CVsSkinnerUI,
	"Valve volume skinning node"
);


//-----------------------------------------------------------------------------
// Statics
//-----------------------------------------------------------------------------

MObject CVsSkinnerNode::m_iaVolume;

MObject CVsSkinnerNode::m_iaVolumeParentBindWorldMatrix;
MObject CVsSkinnerNode::m_iaVolumeWorldMatrix;
MObject CVsSkinnerNode::m_iaStrength;
MObject CVsSkinnerNode::m_iaFalloff;

MObject CVsSkinnerNode::m_iaVolumeParentWorldMatrix;

MObject CVsSkinnerNode::m_iaGeometryWorldMatrix;
MObject CVsSkinnerNode::m_iaGeometry;

MObject CVsSkinnerNode::m_caVolumeData;

MObject CVsSkinnerNode::m_caGeometryWeights;

MObject CVsSkinnerNode::m_oaGeometry;

MObject CVsSkinnerNode::m_oaWeightList;
MObject CVsSkinnerNode::m_oaWeights;


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVsSkinnerNode::CVsSkinnerNode()
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVsSkinnerNode::~CVsSkinnerNode()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsSkinnerNode::postConstructor()
{
	// Allow a shading group to be connected
	setRenderable( true );
}


//-----------------------------------------------------------------------------
// Initialize the node, add static attributes, etc...
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::Initialize()
{
	MFnCompoundAttribute cFn;
	MFnGenericAttribute gFn;
	MFnMatrixAttribute mFn;
	MFnNumericAttribute nFn;
	MFnTypedAttribute tFn;
	MFnUnitAttribute uFn;

	m_iaVolumeParentBindWorldMatrix = mFn.create( "volumeParentBindWorldMatrix", "vpbm" );

	m_iaVolumeParentBindWorldMatrix = mFn.create( "volumeParentBindWorldMatrix", "vpbm" );

	m_iaVolumeWorldMatrix = mFn.create( "volumeWorldMatrix", "vwm" );

	m_iaStrength = nFn.create( "strength", "str", MFnNumericData::kDouble, 0.0 );
	nFn.setSoftMin( -1.0 );
	nFn.setSoftMax(  1.0 );

	m_iaFalloff = nFn.create( "falloff", "fof", MFnNumericData::kDouble, 0.0 );
	nFn.setSoftMin( -1.0 );
	nFn.setSoftMax(  1.0 );

	m_iaVolume = cFn.create( "volume", "vol" );
	cFn.addChild( m_iaVolumeParentBindWorldMatrix );
	cFn.addChild( m_iaVolumeWorldMatrix );
	cFn.addChild( m_iaStrength );
	cFn.addChild( m_iaFalloff );
	cFn.setArray( true );

	m_iaVolumeParentWorldMatrix = mFn.create( "volumeParentWorldMatrix", "vpwm" );
	mFn.setArray( true );

	m_iaGeometryWorldMatrix = mFn.create( "geometryWorldMatrix", "gwm" );
	mFn.setHidden( true );

	m_caVolumeData = nFn.create( "ca_volumeData", "card", MFnNumericData::kBoolean, false );
	nFn.setHidden( true );
	nFn.setStorable( false );

	m_caGeometryWeights = nFn.create( "ca_meshWeights", "camw", MFnNumericData::kBoolean, false );
	nFn.setHidden( true );
	nFn.setStorable( false );

	m_iaGeometry = tFn.create( "inputGeometry", "igeo", MFnData::kMesh );

	m_oaGeometry = tFn.create( "outputGeometry", "ogeo", MFnData::kMesh );
	tFn.setDisconnectBehavior( MFnAttribute::kDelete );
	tFn.setStorable( false );

	addAttribute( m_iaVolume );
	addAttribute( m_iaVolumeParentWorldMatrix );
	addAttribute( m_iaGeometryWorldMatrix );
	addAttribute( m_iaGeometry );
	addAttribute( m_caVolumeData );
	addAttribute( m_caGeometryWeights );
	addAttribute( m_oaGeometry );

	m_oaWeights = nFn.create( "skinWeights", "sw", MFnNumericData::kDouble, 0.0 );
	nFn.setKeyable( true );
	nFn.setArray( true );
	nFn.setReadable( true );
	nFn.setUsesArrayDataBuilder( true );

	m_oaWeightList = cFn.create( "skinWeightList", "swl" );
	cFn.setArray( true );
	cFn.addChild( m_oaWeights );
	cFn.setReadable( true );
	cFn.setUsesArrayDataBuilder( true );

	addAttribute( m_oaWeightList );

	attributeAffects( m_iaVolume, m_caVolumeData );
	attributeAffects( m_iaGeometryWorldMatrix, m_caVolumeData );
	attributeAffects( mControlPoints, m_caVolumeData );
	attributeAffects( mControlValueX, m_caVolumeData );
	attributeAffects( mControlValueY, m_caVolumeData );
	attributeAffects( mControlValueZ, m_caVolumeData );

	attributeAffects( m_iaVolume, m_caGeometryWeights );
	attributeAffects( m_iaGeometryWorldMatrix, m_caGeometryWeights );
	attributeAffects( mControlPoints, m_caGeometryWeights );
	attributeAffects( mControlValueX, m_caGeometryWeights );
	attributeAffects( mControlValueY, m_caGeometryWeights );
	attributeAffects( mControlValueZ, m_caGeometryWeights );
	attributeAffects( m_caVolumeData, m_caGeometryWeights );
	attributeAffects( m_iaGeometry, m_caGeometryWeights );

	attributeAffects( m_iaVolume, m_oaGeometry );
	attributeAffects( m_iaGeometryWorldMatrix, m_oaGeometry );
	attributeAffects( mControlPoints, m_oaGeometry );
	attributeAffects( mControlValueX, m_oaGeometry );
	attributeAffects( mControlValueY, m_oaGeometry );
	attributeAffects( mControlValueZ, m_oaGeometry );
	attributeAffects( m_caVolumeData, m_oaGeometry );
	attributeAffects( m_iaGeometry, m_oaGeometry );
	attributeAffects( m_caGeometryWeights, m_oaGeometry );

	attributeAffects( m_iaVolume, m_oaWeightList );
	attributeAffects( m_iaGeometryWorldMatrix, m_oaWeightList );
	attributeAffects( mControlPoints, m_oaWeightList );
	attributeAffects( mControlValueX, m_oaWeightList );
	attributeAffects( mControlValueY, m_oaWeightList );
	attributeAffects( mControlValueZ, m_oaWeightList );
	attributeAffects( m_caVolumeData, m_oaWeightList );
	attributeAffects( m_iaGeometry, m_oaWeightList );

	attributeAffects( m_iaVolume, m_oaWeights );
	attributeAffects( m_iaGeometryWorldMatrix, m_oaWeights );
	attributeAffects( mControlPoints, m_oaWeights );
	attributeAffects( mControlValueX, m_oaWeights );
	attributeAffects( mControlValueY, m_oaWeights );
	attributeAffects( mControlValueZ, m_oaWeights );
	attributeAffects( m_caVolumeData, m_oaWeights );
	attributeAffects( m_iaGeometry, m_oaWeights );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose: Initialize the node, add static attributes, etc...
// Input  :
// Output : MStatus::kSuccess if everything was ok
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::compute(
	const MPlug &mPlug,
	MDataBlock &mDataBlock )
{
	minfo << " * Compute: " << mPlug.info() << std::endl;

	if ( mPlug == m_caVolumeData )
	{
		return ComputeVolumeData( mDataBlock );
	}
	else if ( mPlug == m_caGeometryWeights )
	{
		return ComputeGeometryWeights( mDataBlock );
	}
	else if ( mPlug == m_oaGeometry )
	{
		return ComputeGeometry( mDataBlock );
	}
	else if ( mPlug == m_oaWeightList || mPlug == m_oaWeights )
	{
		return ComputeWeightList( mDataBlock );
	}

	return MS::kUnknownParameter;
}


//-----------------------------------------------------------------------------
// Purpose: Return the bounding box for the node, Maya uses this to determine
//			if the node needs to be drawn and for selection
//-----------------------------------------------------------------------------
MBoundingBox CVsSkinnerNode::boundingBox() const
{
	SyncVolumeData();

	MDagPath mDagPath;
	MDagPath::getAPathTo( thisMObject(), mDagPath );
	MBoundingBox bbox( m_wbbox );
	bbox.transformUsing( mDagPath.inclusiveMatrixInverse() );

	return m_wbbox;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVsSkinnerNode::match(
	const MSelectionMask &mask,
	const MObjectArray &cList ) const
{
	Assert( 0 );
	minfo << " * Match" << std::endl;

	// Don't know what to do for the first thing
	if ( cList.length() == 0 )
		return mask.intersects( MSelectionMask::kSelectMeshes );

	for ( uint i( 0 ); i != cList.length(); ++i )
	{
		if ( cList[ i ].apiType() == MFn::kMeshVertComponent && mask.intersects( MSelectionMask::kSelectMeshVerts ) )
		{
			minfo << "   + " << true << std::endl;
			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MPxSurfaceShape::MatchResult CVsSkinnerNode::matchComponent(
	const MSelectionList &iList,
	const MAttributeSpecArray &specArray,
	MSelectionList &oList )
{
	const MAttributeSpec volumeSpec( specArray[ 0 ] );

	if ( specArray.length() != 1 || volumeSpec.dimensions() <= 0 ||
		!( !Q_stricmp( volumeSpec.name().asChar(), "v" ) ||
		!Q_stricmp( volumeSpec.name().asChar(), "vol" ) ||
		!Q_stricmp( volumeSpec.name().asChar(), "volume" ) ||
		!Q_stricmp( volumeSpec.name().asChar(), "vtx" ) ||
		!Q_stricmp( volumeSpec.name().asChar(), "vertex" ) ) )
		return MPxSurfaceShape::matchComponent( iList, specArray, oList );

	int lBound( 0 );
	int uBound( 0 );

	MAttributeIndex aIndex( volumeSpec[ 0 ] );
	if ( aIndex.hasLowerBound() )
	{
		aIndex.getLower( lBound );
	}
	else
	{
		aIndex.getValue( lBound );
	}

	if ( aIndex.hasUpperBound() )
	{
		aIndex.getUpper( uBound );
	}
	else
	{
		aIndex.getValue( uBound );
	}

	if ( uBound < lBound )
	{
		int tBound( uBound );
		uBound = lBound;
		lBound = tBound;
	}

	MObject thisObj( thisMObject() );
	MPlug vAP( thisObj, m_iaVolume );
	MIntArray vIndices;
	vAP.getExistingArrayAttributeIndices( vIndices );
	MPlug wP;
	MPlugArray pA;

	MPxSurfaceShape::MatchResult retVal( MPxSurfaceShape::kMatchInvalidAttributeIndex );

	MFnSingleIndexedComponent cFn;
	MObject cObj( cFn.create( MFn::kMeshVertComponent ) );

	for ( int i( lBound ); i <= uBound; ++i )
	{
		for ( uint j( 0U ); j != vIndices.length(); ++j )
		{
			if ( vIndices[ j ] == i )
			{
				cFn.addElement( i );
				retVal = MPxSurfaceShape::kMatchOk;
				wP = vAP.elementByLogicalIndex( i ).child( m_iaVolumeWorldMatrix );
				if ( wP.connectedTo( pA, true, false ) && pA.length() )
				{
					MDagPath lDagPath;
					MDagPath::getAPathTo( pA[ 0 ].node(), lDagPath );
					if ( lDagPath.length() > 1U )
					{
						lDagPath.pop();
						oList.add( lDagPath, MObject::kNullObj, true );
					}
				}
				break;
			}
		}
	}

	if ( retVal == MPxSurfaceShape::kMatchOk )
	{
		MDagPath mDagPath;
		iList.getDagPath( 0, mDagPath );
		oList.add( mDagPath, cObj, true );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
// Just ensures that the local class array, m_volumeData is up to date by
// triggering a compute cycle if any of the inputs are dirty
//-----------------------------------------------------------------------------
void CVsSkinnerNode::SyncVolumeData() const
{
	MObject skinnerObj( thisMObject() );
	MPlug caVolumeDataP( skinnerObj, CVsSkinnerNode::m_caVolumeData );
	bool caVolumeDataV;
	caVolumeDataP.getValue( caVolumeDataV );
}


//-----------------------------------------------------------------------------
// Just ensures that the local class array, m_volumeData is up to date by
// triggering a compute cycle if any of the inputs are dirty
//-----------------------------------------------------------------------------
void CVsSkinnerNode::SyncGeometryWeights() const
{
	MObject skinnerObj( thisMObject() );
	MPlug caGeometryDataP( skinnerObj, CVsSkinnerNode::m_caGeometryWeights );
	bool caGeometryDataV;
	caGeometryDataP.getValue( caGeometryDataV );
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVsSkinnerNode::DrawBoundingBox() const
{
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	MPoint n( m_wbbox.min() );
	MPoint x( m_wbbox.max() );
	glBegin( GL_QUAD_STRIP );
	{
		glVertex3d( x.x, n.y, n.z );
		glVertex3d( n.x, n.y, n.z );
		glVertex3d( x.x, x.y, n.z );
		glVertex3d( n.x, x.y, n.z );
		glVertex3d( x.x, x.y, x.z );
		glVertex3d( n.x, x.y, x.z );
		glVertex3d( x.x, n.y, x.z );
		glVertex3d( n.x, n.y, x.z );
		glVertex3d( x.x, n.y, n.z );
		glVertex3d( n.x, n.y, n.z );
	}
	glEnd();

	for ( int i( 0 ); i < m_volumeData.Count(); ++i )
	{
		const MPoint n( m_volumeData[ i ].m_bbox.min() );
		const MPoint x( m_volumeData[ i ].m_bbox.max() );

		glBegin( GL_QUAD_STRIP );
		{
			glVertex3d( x.x, n.y, n.z );
			glVertex3d( n.x, n.y, n.z );
			glVertex3d( x.x, x.y, n.z );
			glVertex3d( n.x, x.y, n.z );
			glVertex3d( x.x, x.y, x.z );
			glVertex3d( n.x, x.y, x.z );
			glVertex3d( x.x, n.y, x.z );
			glVertex3d( n.x, n.y, x.z );
			glVertex3d( x.x, n.y, n.z );
			glVertex3d( n.x, n.y, n.z );
		}
		glEnd();
	}
}


//-----------------------------------------------------------------------------
// This function assumes that Maya's MArrayDataHandles, although sparse,
// are in order
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::ComputeVolumeData(
	MDataBlock &mDataBlock )
{
	const MMatrix &geometryWorldMatrix( mDataBlock.inputValue( m_iaGeometryWorldMatrix ).asMatrix() );


	MArrayDataHandle vADH( mDataBlock.inputArrayValue( m_iaVolume ) );
	const uint nVolume( vADH.elementCount() );
	m_volumeData.EnsureCount( nVolume );

	m_wbbox.clear();
	uint vLogicalIndex;

	MDataHandle vDH;
	for ( uint vPhysicalIndex( 0U ); vPhysicalIndex != nVolume; ++vPhysicalIndex, vADH.next() )
	{
		vLogicalIndex = vADH.elementIndex();

		vDH = vADH.inputValue();
		VolumeData_s &volumeData( m_volumeData[ vPhysicalIndex ] );
		volumeData.m_jointLogicalIndex = vLogicalIndex;
		volumeData.m_volumeLogicalIndex = vLogicalIndex;
		volumeData.m_worldMatrix = vDH.child( m_iaVolumeWorldMatrix ).asMatrix();
		volumeData.m_volumePosition = MPoint( volumeData.m_worldMatrix[ 3 ] );
		volumeData.m_geometryToVolumeMatrix = geometryWorldMatrix * volumeData.m_worldMatrix.inverse();
		volumeData.m_strength = vDH.child( m_iaStrength ).asDouble();
		volumeData.m_falloff = vDH.child( m_iaFalloff ).asDouble();
		volumeData.m_bbox = MBoundingBox( MPoint( -1.0f, -1.0f, -1.0f ), MPoint( 1.0f, 1.0f, 1.0f ) );
		volumeData.m_bbox.transformUsing( volumeData.m_worldMatrix );
		m_wbbox.expand( volumeData.m_bbox );
	}

	mDataBlock.setClean( m_caVolumeData );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsSkinnerNode::CSkinWeight::AddWeight(
	uint volumeLogicalIndex,
	uint jointLogicalIndex,
	float weight )
{
	++m_requestedCount;

	if ( m_count )
	{
		const uint countMinusOne( m_count - 1 );

		CJointWeight &jointWeight( m_weights[ countMinusOne ] );
		if ( weight > jointWeight.m_weight )
		{
			swap( volumeLogicalIndex, jointWeight.m_volumeLogicalIndex );
			swap( jointLogicalIndex, jointWeight.m_jointLogicalIndex );
			swap( weight, jointWeight.m_weight );
		}

		bool swapped( true );
		while ( swapped )
		{
			swapped = false;

			for ( uint i( countMinusOne ); i != 0; --i )
			{
				for ( uint j( i ); j != 0; --j )
				{
					CJointWeight &jw0( m_weights[ i ] );
					CJointWeight &jw1( m_weights[ i - 1 ] );
					if ( jw0.m_weight > jw1.m_weight )
					{
						swapped = true;
						swap( jw0.m_volumeLogicalIndex, jointWeight.m_volumeLogicalIndex );
						swap( jw0.m_jointLogicalIndex, jointWeight.m_jointLogicalIndex );
						swap( jw0.m_weight, jw1.m_weight );
					}
				}
			}
		}
	}

	if ( m_count < kMaxWeightPerVertex )
	{
		CJointWeight &jointWeight( m_weights[ m_count++ ] );
		jointWeight.m_volumeLogicalIndex = volumeLogicalIndex;
		jointWeight.m_jointLogicalIndex = jointLogicalIndex;
		jointWeight.m_weight = weight;
	}
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::ComputeGeometryWeights(
	MDataBlock &mDataBlock )
{
	ComputeVolumeData( mDataBlock );

	MPoint p;
	MVector v;
	double l;
	float s;	// Strength
	float f;	// Falloff
	float w;

	const int nVolumes( m_volumeData.Count() );
	MObject mObj( mDataBlock.inputValue( m_iaGeometry ).asMesh() );
	MItMeshVertex vIt( mObj );

	const int nVertices( vIt.count() );

	m_skinWeights.EnsureCount( nVertices );

	for ( int vIndex( 0 ); !vIt.isDone(); vIt.next(), ++vIndex )
	{
		CSkinWeight &skinWeight( m_skinWeights[ vIndex ] );
		skinWeight.Reset();

		p = vIt.position();
		for ( int i( 0 ); i < nVolumes; ++i )
		{
			const VolumeData_s &volumeData( m_volumeData[ i ] );
			v = p * volumeData.m_geometryToVolumeMatrix;
			l = v.length();
			s = volumeData.m_strength;
			f = volumeData.m_falloff;

			if ( f < 0.0 )
			{
				if ( l < 1.0f )
				{
					f = f + 1.0f;
					w = RemapValClamped( l, 0.0f, 1.0f, s, s * f );
					if ( w != 0.0 )
					{
						skinWeight.AddWeight( volumeData.m_volumeLogicalIndex, volumeData.m_jointLogicalIndex, w );
					}
				}
			}
			else
			{
				w = RemapValClamped( l, 1.0f, f + 1.0f, s, 0.0 );
				if ( w != 0.0 )
				{
					skinWeight.AddWeight( volumeData.m_volumeLogicalIndex, volumeData.m_jointLogicalIndex, w );
				}
			}
		}
	}

	mDataBlock.setClean( m_caGeometryWeights );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::ComputeWeightList(
	MDataBlock &mDataBlock ) const
{
	mDataBlock.inputValue( m_caGeometryWeights );

	MObject thisNode = thisMObject();
	MPlug wPlug(thisNode, m_oaWeights ); 

	MStatus status;
	MDataBlock &block( mDataBlock );

	static uint cCount( 0 );

	// Write into aWeightList
	uint i;
	uint j;
	uint nWeights;
	float totalWeight;

	const uint nSkinWeights( m_skinWeights.Count() );
	for ( i = 0; i != nSkinWeights; ++i )
	{
		status = wPlug.selectAncestorLogicalIndex( i, m_oaWeightList );
		MDataHandle wHandle = wPlug.constructHandle(block);
		MArrayDataHandle arrayHandle(wHandle, &status);
		MArrayDataBuilder arrayBuilder = arrayHandle.builder(&status);

		const CSkinWeight &skinWeight( m_skinWeights[ i ] );
		totalWeight = skinWeight.TotalWeight();
		nWeights = skinWeight.m_count;
		for ( j = 0; j != nWeights; ++j )
		{
			const CJointWeight &jointWeight( skinWeight.m_weights[ j ] );
			MDataHandle handle = arrayBuilder.addElement( jointWeight.m_jointLogicalIndex, &status );
			handle.set( static_cast< double >( jointWeight.m_weight / totalWeight ) );
		}
		status = arrayHandle.set(arrayBuilder);
		wPlug.setValue(wHandle);
		wPlug.destructHandle(wHandle);
	}

	mDataBlock.setClean( m_oaWeightList );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// XXX TODO - Just show the active volumes' weight
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::ComputeGeometry(
	MDataBlock &mDataBlock )
{
	mDataBlock.inputValue( m_caGeometryWeights );

	MDataHandle ogDH( mDataBlock.outputValue( m_oaGeometry ) );
	ogDH.copy( mDataBlock.inputValue( m_iaGeometry ) );

	MObject ogObj( ogDH.asMesh() );
	MFnMesh meshFn( ogObj );

	const uint nWeights( m_skinWeights.Count() );

	MColorArray colors( nWeights, MColor( 0.0f, 0.0f, 0.0f ) );
	MIntArray colorIds( nWeights );

	for ( uint i( 0U ); i != nWeights; ++i )
	{
		CSkinWeight &skinWeight( m_skinWeights[ i ] );
		const uint &count( skinWeight.m_count );
		const float weight( skinWeight.TotalWeight() );

		if ( count > 3 )
		{
			if ( weight > 0 )
			{
				colors[ i ] = MColor( weight, weight, 0.0f );
			}
			else
			{
				colors[ i ] = MColor( -weight, 0.0f, -weight );
			}

		}
		else
		{
			if ( weight > 0 )
			{
				colors[ i ] = MColor( 0.0f, weight, 0.0f );
			}
			else
			{
				colors[ i ] = MColor( 0.0f, 0.0f, -weight );
			}
		}
		colorIds[ i ] = i;
	}

	meshFn.setVertexColors( colors, colorIds );

	mDataBlock.setClean( m_oaGeometry );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Returns an MPlug corresponding to the bind world matrix for the passed
// transform.  If the transform is connected to a dagPose node which is
// a bind pose dagPose node then it will return the matrix from that node
// otherwise it will return the world space matrix of the node itself
// for the instance of the dag path
//-----------------------------------------------------------------------------
MPlug GetWorldMatrixPlug(
	const MDagPath &tDagPath,
	bool bind )
{
	const MFnDagNode tFn( tDagPath );

	if ( bind )
	{
		MPlug mP( tFn.findPlug( "message" ) );
		MPlugArray pA;
		if ( mP.connectedTo( pA, false, true ) )
		{
			const uint nP( pA.length() );
			for ( uint i( 0 ); i != nP; ++i )
			{
				MObject nObj( pA[ i ].node() );
				if ( !nObj.hasFn( MFn::kDagPose ) )
					continue;

				const MFnDependencyNode nFn( nObj );
				bool bpV( false );
				nFn.findPlug( "bindPose" ).getValue( bpV );
				if ( !bpV )
					continue;

				minfo << " * BIND POSE for " << tDagPath.partialPathName() << " is on " << nFn.name() << std::endl;

				MPlug wmP( nFn.findPlug( "worldMatrix" ) );
				return wmP.elementByLogicalIndex( pA[ i ].logicalIndex() );
			}
		}
	}

	MPlug wmP( tFn.findPlug( "worldMatrix" ) );
	return wmP.elementByLogicalIndex( tDagPath.isInstanced() ? tDagPath.instanceNumber() : 0 );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsSkinnerNode::AttachTransformToVolume(
	const MPlug &volumePlug,
	const MDagPath &transformPath,
	const MObject &skinnerBoneObj,
	const MDagPath &editTransformPath,
	const MDagPath &editShapePath,
	ValveMaya::CUndo &undo ) const
{
	Assert( !volumePlug.isNull() );
	Assert( transformPath.isValid() && transformPath.length() );
	Assert( !skinnerBoneObj.isNull() );
	Assert( editTransformPath.isValid() && editTransformPath.length() );
	Assert( editShapePath.isValid() && editShapePath.length() );

	const MFnDependencyNode sbFn( skinnerBoneObj );

	MPlug twbmP( GetWorldMatrixPlug( transformPath, true ) );

	// transform.worldMatrix -> skinnerBone.parentWorldMatrix (NOTE: Will get bind matrix if exists)
	undo.Connect( twbmP, sbFn.findPlug( "parentWorldMatrix" ), true );

	// transform.worldMatrix -> skinner.parentBindWorldMatrix
	undo.Connect( twbmP, volumePlug.child( m_iaVolumeParentBindWorldMatrix ), true );

	// transform|childTransform.worldMatrix -> skinnerBone.childWorldMatrix (NOTE: Will get bind matrix if exists)
	if ( transformPath.childCount() == 1 )
	{
		MDagPath childPath( transformPath );
		childPath.push( transformPath.child( 0 ) );
		if ( childPath.hasFn( MFn::kTransform ) )
		{
			undo.Connect( GetWorldMatrixPlug( childPath, true ), sbFn.findPlug( "childWorldMatrix" ) );
		}
	}

	// transform.worldMatrix -> skinner.parentBindWorldMatrix
	MPlug vpwmP( thisMObject(), m_iaVolumeParentWorldMatrix );
	vpwmP = vpwmP.elementByLogicalIndex( volumePlug.logicalIndex() );
	undo.Connect( GetWorldMatrixPlug( transformPath, false ), vpwmP, true );

	// skinnerBone.translate ..etc.. -> editTransform.translate ..etc..
	const MFnDagNode etFn( editTransformPath );
	undo.Connect( sbFn.findPlug( "translate" ), etFn.findPlug( "translate" ), true );
	undo.Connect( sbFn.findPlug( "rotate" ), etFn.findPlug( "rotate" ), true );
	undo.Connect( sbFn.findPlug( "scale" ), etFn.findPlug( "scale" ), true );
	undo.Connect( sbFn.findPlug( "shear" ), etFn.findPlug( "shear" ), true );

	// editShape.worldMatrix -> skinner.volumeWorldMatrix
	undo.Connect( GetWorldMatrixPlug( editShapePath, false ), volumePlug.child( m_iaVolumeWorldMatrix ), false );
}


//-----------------------------------------------------------------------------
// Create a new volume on the node
//-----------------------------------------------------------------------------
MObject CVsSkinnerNode::NewVolume(
	double strength,
	double falloff,
	const MDagPath &parent,
	ValveMaya::CUndo &undo ) const
{
	MObject thisObj( thisMObject() );

	MPlug vP( thisObj, m_iaVolume );
	const uint nextAvailable( ValveMaya::NextAvailable( vP ) );
	vP = vP.elementByLogicalIndex( nextAvailable );

	vP.child( m_iaFalloff ).setValue( falloff );
	vP.child( m_iaStrength ).setValue( strength );

	MDagPath mDagPath;
	MDagPath::getAPathTo( thisObj, mDagPath );
	MDagModifier &dagMod( undo.DagModifier() );

	MObject dObj( dagMod.MDGModifier::createNode( "vsSkinnerBone" ) );
	dagMod.doIt();

	// TODO: Name things after the parent

	MString pName( MFnDagNode( parent ).name() );

	dagMod.renameNode( dObj, MString( "vdp_" ) + static_cast< int >( nextAvailable ) );
	dagMod.doIt();

	MObject tObj( dagMod.createNode( "transform", mDagPath.transform() ) );
	dagMod.doIt();

	dagMod.renameNode( tObj, pName + MString( "Parent" ) );
	dagMod.doIt();

	MObject ltObj( dagMod.createNode( "transform", tObj ) );
	dagMod.doIt();

	dagMod.renameNode( ltObj, pName );
	dagMod.doIt();

	MObject lsObj( dagMod.createNode( "locator", ltObj ) );
	dagMod.doIt();

	dagMod.renameNode( lsObj, pName + "Shape" );
	dagMod.doIt();

	MFnDependencyNode dFn( dObj );

	MFnDependencyNode tFn( tObj );
	tFn.findPlug( "intermediateObject" ).setValue( true );
	tFn.findPlug( "inheritsTransform" ).setValue( false );
	tFn.findPlug( "rotateOrder" ).setLocked( true );
	MPlug tP( tFn.findPlug( "rotateAxis" ) );
	tP.setLocked( true );
	tP.child( 0 ).setLocked( true );
	tP.child( 1 ).setLocked( true );
	tP.child( 2 ).setLocked( true );

	MFnDependencyNode ltFn( ltObj );
	ltFn.findPlug( "intermediateObject" ).setValue( true );

	MFnDependencyNode lsFn( lsObj );
	lsFn.findPlug( "intermediateObject" ).setValue( true );

	MDagPath tPath;
	MDagPath::getAPathTo( tObj, tPath );
	MDagPath lsPath;
	MDagPath::getAPathTo( lsObj, lsPath );
	AttachTransformToVolume( vP, parent, dObj, tPath, lsPath, undo );

	MFnSingleIndexedComponent sFn;
	MObject sObj( sFn.create( MFn::kMeshVertComponent ) );	// .vtx[ # ]
	sFn.addElement( nextAvailable );

	ScaleVolumeToJoint( vP, undo );

	return sObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::ScaleVolumeToJoint(
	MPlug &vP,
	ValveMaya::CUndo &undo ) const
{
	MObject thisObj( thisMObject() );
	MPlug sP( thisObj, m_iaVolumeParentWorldMatrix );
	sP = sP.elementByLogicalIndex( vP.logicalIndex() );

	MPlugArray pA;
	if ( !( sP.connectedTo( pA, true, false ) && pA.length() == 1U ) )
		return MS::kFailure;

	MDagPath pDagPath;
	MDagPath::getAPathTo( pA[ 0 ].node(), pDagPath );

	if ( pDagPath.length() == 0U )
		return MS::kFailure;

	const uint nChildren( pDagPath.childCount() );

	if ( nChildren == 0U )
		return MS::kSuccess;

	sP = vP.child( m_iaVolumeWorldMatrix );
	if ( !( sP.connectedTo( pA, true, false ) && pA.length() == 1U ) )
		return MS::kFailure;

	MDagPath wDagPath;
	MDagPath::getAPathTo( pA[ 0 ].node(), wDagPath );

	if ( wDagPath.length() <= 1U )
		return MS::kFailure;

	MFnTransform wFn( wDagPath.transform() );

	int cIndex( -1 );
	// See if it's a 1 child node after helper bones are filtered out

	for ( uint i( 0U ); i != nChildren; ++i )
	{
		MDagPath cDagPath( pDagPath );
		cDagPath.push( pDagPath.child( i ) );

		if ( !cDagPath.hasFn( MFn::kTransform ) )
			continue;

		const uint nGrandChildren( cDagPath.childCount() );
		if ( nGrandChildren == 1U && !cDagPath.child( 0U ).hasFn( MFn::kTransform ) )
			continue;

		if ( cIndex >= 0 )
			return MS::kSuccess;
		cIndex = i;
	}

	if ( cIndex < 0 )
		return MS::kSuccess;

	MFnTransform cFn( pDagPath.child( cIndex ) );
	MVector wTranslate( cFn.getTranslation( MSpace::kTransform ) );
	wTranslate /= 2.0;
	wFn.setTranslation( wTranslate, MSpace::kTransform );
	const double wLen( wTranslate.length() );
	wTranslate.normalize();

	double s[ 3 ] = { 1.0, 1.0, 1.0 };

	if ( !wTranslate.isEquivalent( MVector::zero ) )
	{
		double axes[][3] =
		{
			{  1.0,  0.0,  0.0 },
			{  0.0,  1.0,  0.0 },
			{  0.0,  0.0,  1.0 },
			{ -1.0,  0.0,  0.0 },
			{  0.0, -1.0,  0.0 },
			{  0.0,  0.0, -1.0 }
		};

		int axis( 0 );
		double dotProduct( MVector( axes[ axis ] ) * wTranslate );
		for ( uint i( axis + 1 ); i != 6; ++i )
		{
			double tmpDotProduct( MVector( axes[ i ] ) * wTranslate );
			if ( tmpDotProduct > dotProduct )
			{
				axis = i;
				dotProduct = tmpDotProduct;
			}
		}

		s[ axis % 3 ] = wLen;
		s[ ( axis + 1 ) % 3 ] = wLen / 2.0f;
		s[ ( axis + 2 ) % 3 ] = wLen / 2.0f;
	}

	wFn.setScale( s );

	return MS::kSuccess;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::AttachMesh(
	const MDagPath &meshPath,
	MDagModifier &mDagModifier )
{
	// Figure out which mesh needs to be attached, detached
	MObject thisObj( thisMObject() );
	MFnDependencyNode vsSkinnerFn( thisObj );

	// Figure out if the mesh needs to be duplicated or not
	const MFnDagNode oldMeshFn( meshPath );

	MPlug inMeshP( oldMeshFn.findPlug( "inMesh" ) );
	if ( !inMeshP.isConnected() )
	{
		// Duplicate the mesh and parent it under the same transform as the original
		MObject newTransformObj( oldMeshFn.duplicate() );
		MObject newMeshObj( MFnDagNode( newTransformObj ).child( 0 ) );
		MFnDagNode newMeshFn( newMeshObj );
		newMeshFn.setIntermediateObject( true );
		mDagModifier.connect( newMeshFn.findPlug( "outMesh" ), inMeshP );
		mDagModifier.doIt();
		mDagModifier.reparentNode( newMeshObj, oldMeshFn.parent( meshPath.instanceNumber() ) );
		mDagModifier.doIt();
		mDagModifier.deleteNode( newTransformObj );
		mDagModifier.doIt();
		mDagModifier.renameNode( newMeshObj, oldMeshFn.name() + "Copy" );
		mDagModifier.doIt();
	}

	MPlugArray mPlugArray;

	inMeshP = oldMeshFn.findPlug( "inMesh" );
	if ( !( inMeshP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
	{
		merr << "No history on " << meshPath.fullPathName() <<
			" can't attach to vsSkinner node" << vsSkinnerFn.name() << std::endl;
		return MS::kFailure;
	}

	MObject ccsObj( ValveMaya::AddColorSetToMesh( "vsSkinner", meshPath, mDagModifier ) );

	inMeshP = oldMeshFn.findPlug( "inMesh" );
	if ( !( inMeshP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
	{
		merr << "No history on " << meshPath.fullPathName() <<
			" can't attach to vsSkinner node" << vsSkinnerFn.name() << std::endl;
		return MS::kFailure;
	}

	// Find out if anything is already connected to m_iaGeometry
	MPlug igP( thisObj, m_iaGeometry );
	MPlugArray igPA;
	if ( igP.connectedTo( igPA, true, false ) && igPA.length() )
	{
		mDagModifier.disconnect( igPA[ 0 ], igP );
	}

	mDagModifier.disconnect( mPlugArray[ 0 ], inMeshP );
	mDagModifier.connect( mPlugArray[ 0 ], igP );
	mDagModifier.connect( MPlug( thisObj, m_oaGeometry ), inMeshP );
	mDagModifier.doIt();

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Detach a mesh that was connected in exactly the way as above
//-----------------------------------------------------------------------------
MStatus CVsSkinnerNode::DetachMesh(
	MDagModifier &mDagModifier )
{
	// Figure out which mesh needs to be attached, detached
	MObject thisObj( thisMObject() );
	MFnDependencyNode vsSkinnerFn( thisObj );

	MPlug gwmP( thisObj, m_iaGeometryWorldMatrix );
	MPlugArray mPlugArray;
	if ( !( gwmP.connectedTo( mPlugArray, true, false ) && mPlugArray.length() ) )
		return MS::kFailure;

	MObject meshObj( mPlugArray[ 0 ].node() );
	MDagPath mDagPath;
	MDagPathArray pathArray;
	MDagPath::getAllPathsTo( meshObj, pathArray );
	if ( mPlugArray[ 0 ].logicalIndex()  < pathArray.length() )
	{
		mDagPath = pathArray[ mPlugArray[ 0 ].logicalIndex() ];
	}
	else
	{
		mDagPath = pathArray[ 0 ];
	}

	// Get input geometry
	MPlug igP( thisObj, m_iaGeometry );
	MPlugArray igPA;
	if ( !igP.connectedTo( igPA, true, false ) && igPA.length() )
		return MS::kFailure;

	// See if the node driving the input geometry is a createColorSet node making a vsSkinner colorSet
	MObject ccsObj( igPA[ 0 ].node() );
	if ( ccsObj.apiType() != MFn::kCreateColorSet )
		return MS::kFailure;

	MString csnV;
	MFnDependencyNode ccsFn( ccsObj );
	ccsFn.findPlug( "colorSetName" ).getValue( csnV );
	if ( Q_strnicmp( "vsSkinner", csnV.asChar(), 9 ) )
		return MS::kFailure;

	MPlug ccsigP( ccsFn.findPlug( "inputGeometry" ) );
	MPlugArray ccsigPA;
	if ( !( ccsigP.connectedTo( ccsigPA, true, false ) && ccsigPA.length() ) )
		return MS::kFailure;

	MPlug ogP( thisObj, m_oaGeometry );
	MPlugArray ogPA;
	if ( !( ogP.connectedTo( ogPA, false, true ) && ogPA.length() == 1 ) )
		return MS::kFailure;

	MObject omObj( ogPA[ 0 ].node() );
	MFnDependencyNode omFn( omObj );
	MPlug omP( omFn.findPlug( "outMesh" ) );
	if ( omP.isNull() )
		return MS::kFailure;

	// Disconnect & Remove the createColorSet node
	mDagModifier.disconnect( igPA[ 0 ], igP );
	mDagModifier.disconnect( ccsigPA[ 0 ], ccsigP );
	mDagModifier.doIt();

	mDagModifier.deleteNode( ccsObj );
	mDagModifier.doIt();

	// Disconnect the outGeometry from the vsSkinner node
	mDagModifier.disconnect( ogP, ogPA[ 0 ] );
	mDagModifier.doIt();

	// Connect mesh output geometry to the vsSkinner node
	mDagModifier.connect( omP, igP );
	mDagModifier.doIt();

	// Connect the input geometry into the createColorSet to the mesh
	mDagModifier.connect( ccsigPA[ 0 ], ogPA[ 0 ] );
	mDagModifier.doIt();

	// Query the output to sync things up
	bool junk;
	MPlug( thisObj, m_caVolumeData ).getValue( junk );

	// Now see if the upstream geometry node should be deleted
	MObject ugObj( ccsigPA[ 0 ].node() );
	if ( ugObj.apiType() == MFn::kMesh )
	{
		MFnMesh ugFn( ugObj );
		if ( ugFn.isIntermediateObject() && !ugFn.findPlug( "inMesh" ).isConnected() && ugFn.name() == omFn.name() + "Copy" )
		{
			mDagModifier.deleteNode( ugObj );
			mDagModifier.doIt();
		}
	}

	return MS::kSuccess;
}


//=============================================================================
//
// CVsSkinnerUI
//
//=============================================================================


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVsSkinnerUI::CVsSkinnerUI()
{
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVsSkinnerUI::getDrawRequests(
	const MDrawInfo &mDrawInfo,
	bool objectAndActiveOnly,
	MDrawRequestQueue &mDrawRequestQueue )
{
	// Object and component color defines
	//
#define LEAD_COLOR				18	// green
#define ACTIVE_COLOR			15	// white
#define ACTIVE_AFFECTED_COLOR	8	// purple
#define DORMANT_COLOR			4	// blue
#define HILITE_COLOR			17	// pale blue
#define DORMANT_VERTEX_COLOR	8	// purple
#define ACTIVE_VERTEX_COLOR		16	// yellow

	MDrawData mDrawData;
	CVsSkinnerNode *pSkinner( reinterpret_cast< CVsSkinnerNode * >( surfaceShape() ) );
	MDrawRequest mDrawRequest( mDrawInfo.getPrototype( *this ) );
	getDrawData( pSkinner, mDrawData );
	mDrawRequest.setDrawData( mDrawData );

	const M3dView::DisplayStyle  displayStyle( mDrawInfo.displayStyle() );
	const M3dView::DisplayStatus displayStatus( mDrawInfo.displayStatus() );

	switch ( displayStyle )
	{
	case M3dView::kBoundingBox:
		mDrawRequest.setToken( kDrawBoundingBox );
		mDrawRequestQueue.add( mDrawRequest );
		break;
	case M3dView::kWireFrame:
		{
			mDrawRequest.setToken( kDrawWireframe );

			const M3dView::ColorTable activeColorTable( M3dView::kActiveColors );
			const M3dView::ColorTable dormantColorTable( M3dView::kDormantColors );

			switch ( displayStatus )
			{
			case M3dView::kLead:
				mDrawRequest.setColor( LEAD_COLOR, activeColorTable );
				break;
			case M3dView::kActive:
				mDrawRequest.setColor( ACTIVE_COLOR, activeColorTable );
				break;
			case M3dView::kActiveAffected:
				mDrawRequest.setColor( ACTIVE_AFFECTED_COLOR, activeColorTable );
				break;
			case M3dView::kDormant:
				mDrawRequest.setColor( DORMANT_COLOR, dormantColorTable );
				break;
			case M3dView::kHilite:
				mDrawRequest.setColor( HILITE_COLOR, activeColorTable );
				break;
			default:
				break;
			}

			mDrawRequestQueue.add( mDrawRequest );
		}
		break;
	case M3dView::kGouraudShaded:
	case M3dView::kFlatShaded:
		{
			// Create the smooth shaded draw request
			//
			mDrawRequest.setToken( kDrawSmoothShaded );

			// Need to get the material info
			//
			MDagPath path = mDrawInfo.multiPath();	// path to your dag object 
			M3dView view = mDrawInfo.view();; 		// view to draw to
			MMaterial material = MPxSurfaceShapeUI::material( path );

			// Evaluate the material and if necessary, the texture.
			//
			if ( ! material.evaluateMaterial( view, path ) )
			{
				merr << "Couldn't evaluate material" << std::endl;
			}

			if ( material.materialIsTextured() )
			{
				material.evaluateTexture( mDrawData );
			}

			mDrawRequest.setMaterial( material );

			// request.setDisplayStyle( appearance );

			bool materialTransparent = false;
			
			if ( material.getHasTransparency( materialTransparent ) )
			{
				mDrawRequest.setIsTransparent( true );
			}

			mDrawRequestQueue.add( mDrawRequest );

			// create a draw request for wireframe on shaded if
			// necessary.
			//
			if ( displayStatus == M3dView::kActive ||
				displayStatus == M3dView::kLead ||
				displayStatus == M3dView::kHilite )
			{
				MDrawRequest wireRequest = mDrawInfo.getPrototype( *this );
				wireRequest.setDrawData( mDrawData );
				wireRequest.setToken( kDrawWireframeOnShaded );
				wireRequest.setDisplayStyle( M3dView::kWireFrame );

				M3dView::ColorTable activeColorTable = M3dView::kActiveColors;

				switch ( displayStatus )
				{
				case M3dView::kLead :
					wireRequest.setColor( LEAD_COLOR, activeColorTable );
					break;
				case M3dView::kActive :
					wireRequest.setColor( ACTIVE_COLOR, activeColorTable );
					break;
				case M3dView::kHilite :
					wireRequest.setColor( HILITE_COLOR, activeColorTable );
					break;
				default :	
					break;
				}

				mDrawRequestQueue.add( wireRequest );
			}
			break;
		}
	default:
		mDrawRequestQueue.add( mDrawRequest );
		break;
	}

	if ( !objectAndActiveOnly )
	{
		// Inactive Components
		if ( displayStyle == M3dView::kPoints || displayStatus == M3dView::kHilite )
		{
			MDrawRequest vertexRequest( mDrawInfo.getPrototype( *this ) );
			vertexRequest.setDrawData( mDrawData );
			vertexRequest.setToken( kDrawVertices );
			vertexRequest.setColor( DORMANT_VERTEX_COLOR, M3dView::kActiveColors );

			mDrawRequestQueue.add( vertexRequest );
		}

		// Active components
		if ( pSkinner->hasActiveComponents() )
		{
			MDrawRequest vertexRequest( mDrawInfo.getPrototype( *this ) );
			vertexRequest.setDrawData( mDrawData );
			vertexRequest.setToken( kDrawVertices );
			vertexRequest.setColor( ACTIVE_VERTEX_COLOR, M3dView::kActiveColors );

			MObjectArray cList( pSkinner->activeComponents() );
			MObject cObj( cList[ 0 ] );	// TODO: Filter List?
			vertexRequest.setComponent( cObj );

			mDrawRequestQueue.add( vertexRequest );
		}
	}
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVsSkinnerUI::draw(
	const MDrawRequest &mDrawRequest,
	M3dView &m3dView ) const
{
	// Make sure all of the data is computated
	CVsSkinnerNode *pSkinner( reinterpret_cast< CVsSkinnerNode * >( surfaceShape() ) );
	pSkinner->SyncGeometryWeights();

	m3dView.beginGL();

	DoDraw( mDrawRequest.multiPath(), m3dView, mDrawRequest.token(), &mDrawRequest );

	m3dView.endGL();
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVsSkinnerUI::DoDraw(
	const MDagPath &mDagPath,
	M3dView &m3dView,
	short drawToken,
	const MDrawRequest *pDrawRequest ) const
{
	const MMatrix inclusiveMatrixInverse( mDagPath.inclusiveMatrixInverse() );

	GLint matrixMode( GL_MODELVIEW );
	glGetIntegerv( GL_MATRIX_MODE, &matrixMode );
	glMatrixMode( GL_MODELVIEW );

	glPushMatrix();
	{
		glMultMatrixd( &inclusiveMatrixInverse.matrix[ 0 ][ 0 ] );

		bool turnOnLighting( false );

		if ( glIsEnabled( GL_LIGHTING ) )
		{
			turnOnLighting = true;
			glDisable( GL_LIGHTING );
		}

		if ( drawToken == kDrawBoundingBox )
		{
			CVsSkinnerNode *pSkinner( reinterpret_cast< CVsSkinnerNode * >( surfaceShape() ) );
			pSkinner->DrawBoundingBox();
		}
		else if ( drawToken == kDrawVertices )
		{
			DrawVolumes( m3dView, pDrawRequest );
		}
		else
		{
			DrawVolumes( m3dView );
		}

		if ( turnOnLighting )
		{
			glEnable( GL_LIGHTING );
		}
	}
	glPopMatrix();

	glMatrixMode( matrixMode );
}


//-----------------------------------------------------------------------------
// Do user screen selection of the shape
//-----------------------------------------------------------------------------
bool CVsSkinnerUI::select(
	MSelectInfo &mSelectInfo,
	MSelectionList &mSelectionList,
	MPointArray &worldSpaceSelectPts ) const
{
	bool retVal( false );

	// Make sure all of the data is computated
	CVsSkinnerNode *pSkinner( reinterpret_cast< CVsSkinnerNode * >( surfaceShape() ) );
	pSkinner->SyncGeometryWeights();

	M3dView m3dView( mSelectInfo.view() );

	if ( mSelectInfo.displayStatus() == M3dView::kHilite )
	{
		return SelectComponents( mSelectInfo, mSelectionList, worldSpaceSelectPts );
	}

	short drawToken( 0 );
	switch ( m3dView.displayStyle() ) {
	case M3dView::kBoundingBox:
		drawToken = kDrawBoundingBox;
		break;
	case M3dView::kWireFrame:
		drawToken = kDrawWireframe;
		break;
	case M3dView::kPoints:
		drawToken = kDrawBoundingBox;
		break;
	case M3dView::kGouraudShaded:
		drawToken = kDrawSmoothShaded;
		break;
	case M3dView::kFlatShaded:
		drawToken = kDrawFlatShaded;
		break;
	default:
		break;
	}

	uint selectBuf[ 4 ];

	m3dView.beginGL();

	m3dView.beginSelect( selectBuf, 4 );
	m3dView.initNames();
	m3dView.pushName( 0 );

	DoDraw( mSelectInfo.selectPath(), m3dView, drawToken );

	if ( m3dView.endSelect() > 0  )
	{
		retVal = true;

		MPoint wPoint(0.0, 0.0, 0.0, 1.0);

		if ( mSelectInfo.singleSelection() )
		{
			// Determine the depth of the selection:
			const double depth( static_cast< double >( selectBuf[ 1 ] ) / ( 65536.0 * 65536.0 - 1.0 ) );

			// Get the current GL Model View matrix:
			MMatrix mvmat;
			glGetDoublev(GL_MODELVIEW_MATRIX, &mvmat.matrix[ 0 ][ 0 ] );

			// Get the current GL Projection matrix:
			MMatrix pmat;
			glGetDoublev(GL_PROJECTION_MATRIX, &pmat.matrix[ 0 ][ 0 ] );

			// Determine where the NDC (Normalized Device Coordinates, Screen Space [ 0, 1 ]
			// point (0,0,depth) is in world space:
			const MMatrix ipmvmat( ( mvmat * pmat ).inverse() );
			wPoint = MPoint( ipmvmat[ 2 ] ) * ( depth + depth - 1.0 ) + MPoint( ipmvmat[ 3 ] );
		}

		MSelectionList mSelectionItem;
		mSelectionItem.add( mSelectInfo.selectPath() );
		mSelectInfo.addSelection( mSelectionItem, wPoint, mSelectionList, worldSpaceSelectPts, MSelectionMask::kSelectNurbsSurfaces, false );
	}

	m3dView.endGL();

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CVsSkinnerUI::SelectComponents(
	MSelectInfo &mSelectInfo,
	MSelectionList &mSelectionList,
	MPointArray &worldSpaceSelectPts ) const
{
	bool retVal( false );

	M3dView m3dView( mSelectInfo.view() );
	m3dView.beginGL();

	GLint matrixMode( GL_MODELVIEW );
	glGetIntegerv( GL_MATRIX_MODE, &matrixMode );
	glMatrixMode( GL_MODELVIEW );

	CVsSkinnerNode *pSkinner( reinterpret_cast< CVsSkinnerNode * >( surfaceShape() ) );
	MObject skinnerObj( pSkinner->thisMObject() );
	MPlug vAP( skinnerObj, CVsSkinnerNode::m_iaVolume );
	MPlug wP;
	MPlugArray pA;

	MFnSingleIndexedComponent cFn;
	MObject cObj( cFn.create( MFn::kMeshVertComponent ) );

	MSelectionList sList;

	glPushMatrix();
	{
		const MMatrix inclusiveMatrixInverse( mSelectInfo.selectPath().inclusiveMatrixInverse() );
		glMultMatrixd( &inclusiveMatrixInverse.matrix[ 0 ][ 0 ] );

		const CUtlVector< CVsSkinnerNode::VolumeData_s > &volumeData( pSkinner->m_volumeData );
		const int nVolume( volumeData.Count() );

		for ( int i = 0; i < nVolume; ++i )
		{
			const CVsSkinnerNode::VolumeData_s &volumeData( volumeData[ i ] );

			m3dView.beginSelect();
			{
				glPushMatrix();
				glMultMatrixd( &volumeData.m_worldMatrix.matrix[ 0 ][ 0 ] );
				DrawUnitSphere();
				glPopMatrix();
			}

			if ( m3dView.endSelect() )
			{
				retVal = true;
				cFn.addElement( volumeData.m_volumeLogicalIndex );
				wP = vAP.elementByLogicalIndex( volumeData.m_volumeLogicalIndex ).child( CVsSkinnerNode::m_iaVolumeWorldMatrix );
				if ( wP.connectedTo( pA, true, false ) && pA.length() )
				{
					MDagPath lDagPath;
					MDagPath::getAPathTo( pA[ 0 ].node(), lDagPath );
					if ( lDagPath.length() > 1U )
					{
						lDagPath.pop();
						sList.add( lDagPath, MObject::kNullObj, true );
					}
				}
			}
		}
	}
	glPopMatrix();

	glMatrixMode( matrixMode );

	m3dView.endGL();

	if ( retVal )
	{
		sList.add( mSelectInfo.selectPath(), cObj );
		mSelectInfo.addSelection( sList, MPoint::origin, mSelectionList, worldSpaceSelectPts, MSelectionMask( MSelectionMask::kSelectComponentsMask ), true );
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsSkinnerUI::DrawUnitSphere() const
{
	// TODO: Make a table

	glBegin( GL_LINE_LOOP );
	for ( int i = 0; i < 360; i += 10 )
	{
		glVertex3f( 0.0f, cos( i / 180.0 * M_PI ), sin( i / 180.0 * M_PI ) );
	}
	glEnd();

	glBegin( GL_LINE_LOOP );
	for ( int i = 0; i < 360; i += 10 )
	{
		glVertex3f( cos( i / 180.0 * M_PI ), 0.0f, sin( i / 180.0 * M_PI ) );
	}
	glEnd();

	glBegin( GL_LINE_LOOP );
	for ( int i = 0; i < 360; i += 10 )
	{
		glVertex3f( cos( i / 180.0 * M_PI ), sin( i / 180.0 * M_PI ), 0.0f );
	}
	glEnd();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsSkinnerUI::DrawVolumes(
	M3dView &m3dView,
	const MDrawRequest *pDrawRequest ) const
{
	CVsSkinnerNode *pSkinner( reinterpret_cast< CVsSkinnerNode * >( surfaceShape() ) );

	const CUtlVector< CVsSkinnerNode::VolumeData_s > &volumeData( pSkinner->m_volumeData );
	const int nVolume( volumeData.Count() );

	char numBuf[ 32 ];

	if ( pDrawRequest )
	{
		MObject cObj( pDrawRequest->component() );
		MFnSingleIndexedComponent cFn( cObj );
		const int nElements( cFn.elementCount() );
		uint logicalIndex;

		int j( -1 );
		for ( int i( 0 ); i < nElements; ++i )
		{
			logicalIndex = cFn.element( i );
			for ( ++j; j < nVolume; ++j )
			{
				const CVsSkinnerNode::VolumeData_s &volumeData( volumeData[ j ] );
				if ( volumeData.m_volumeLogicalIndex == logicalIndex )
				{
					glPushMatrix();
					glMultMatrixd( &volumeData.m_worldMatrix.matrix[ 0 ][ 0 ] );
					DrawUnitSphere();
					glPopMatrix();
					Q_snprintf( numBuf, sizeof( numBuf ), "%d", logicalIndex );
					m3dView.drawText( numBuf, volumeData.m_volumePosition );
					break;
				}
			}
		}
	}
	else
	{
		for ( int i = 0; i < nVolume; ++i )
		{
			const CVsSkinnerNode::VolumeData_s &volumeData( volumeData[ i ] );
			glPushMatrix();
			glMultMatrixd( &volumeData.m_worldMatrix.matrix[ 0 ][ 0 ] );
			DrawUnitSphere();
			glPopMatrix();
		}
	}
}