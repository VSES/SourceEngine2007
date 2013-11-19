//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================

// Standard includes
#include <math.h>


// Maya includes
#include <maya/MIOStream.h>
#include <maya/MAngle.h>
#include <maya/MColor.h>
#include <maya/MDagPathArray.h>
#include <maya/MDistance.h>
#include <maya/MDrawData.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MMaterial.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MSelectionList.h>
#include <maya/MSelectionMask.h>
#include <maya/MVector.h>
#include <maya/MPx3dModelView.h>


// Valve includes
#include "valveMaya.h"


// Local includes
#include "VstHelperBoneNode.h"


// Get rid of stupid min & max MACROS?!?!?!
#if defined min
#undef min
#endif // defined min

#if defined max
#undef max
#endif // defined max

// Make template functions

template < typename T >
T min( const T &a, const T &b )
{
	return a < b ? a : b;
}

template < typename T >
T max( const T &a, const T &b )
{
	return a > b ? a : b;
}

// Local includes

//=============================================================================
//
//=============================================================================
class CVstHelperBoneUI : public MPxSurfaceShapeUI
{
public:
	CVstHelperBoneUI();

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
	void DoDraw(
		const MDagPath &mDagPath,
		M3dView &m3dView,
		short drawToken,
		const MDrawRequest *pDrawRequest = NULL ) const;

	static void DrawArrow(
		const float sphereRadius );

	static void DrawMatrix();

	static void DrawAxis();

	static void DrawArrowhead();

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
	CVstHelperBoneNode,
	vstHelperBone,
	0x0010e4c3,
	CVstHelperBoneNode::Initialize,
	CVstHelperBoneUI,
	"Valve helper bones"
);

// Statics

MObject CVstHelperBoneNode::m_iaControlRotateAxis;

MObject CVstHelperBoneNode::m_iaControlJointOrient;

MObject CVstHelperBoneNode::m_iaControlParentWorldSpace;

MObject CVstHelperBoneNode::m_iaControlWorldSpace;

MObject CVstHelperBoneNode::m_iaTriggerDrawAxis;

MObject CVstHelperBoneNode::m_iaBaseTranslate;
MObject CVstHelperBoneNode::m_iaBaseRotate;

MObject CVstHelperBoneNode::m_caMatrices;

MObject CVstHelperBoneNode::m_iaMaxRadius;

MObject CVstHelperBoneNode::m_iaTrigger;
MObject CVstHelperBoneNode::m_iaTriggerDraw;
MObject CVstHelperBoneNode::m_iaTriggerTolerance;
MObject CVstHelperBoneNode::m_iaTriggerAxis;
MObject CVstHelperBoneNode::m_iaTriggerTranslate;
MObject CVstHelperBoneNode::m_iaTriggerRotate;
MObject CVstHelperBoneNode::m_iaTriggerName;

MObject CVstHelperBoneNode::m_caTriggerData;

MObject CVstHelperBoneNode::m_oaTranslate;
MObject CVstHelperBoneNode::m_oaTranslateX;
MObject CVstHelperBoneNode::m_oaTranslateY;
MObject CVstHelperBoneNode::m_oaTranslateZ;

MObject CVstHelperBoneNode::m_oaRotate;
MObject CVstHelperBoneNode::m_oaRotateX;
MObject CVstHelperBoneNode::m_oaRotateY;
MObject CVstHelperBoneNode::m_oaRotateZ;

MObject CVstHelperBoneNode::m_iaSubdivisons;
MObject CVstHelperBoneNode::m_oaNumTriangles;

MObject CVstHelperBoneNode::m_iaWireframeOnShaded;
MObject CVstHelperBoneNode::m_iaDrawWireframe;


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVstHelperBoneNode::CVstHelperBoneNode()
: m_maxRadius( 1.0 )
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVstHelperBoneNode::~CVstHelperBoneNode()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstHelperBoneNode::postConstructor()
{
	// Allow a shading group to be connected
	setRenderable( true );
}

//-----------------------------------------------------------------------------
//
// Purpose: Initialize the node, add static attributes, etc...
// Input  :
// Output : MStatus::kSuccess if everything was ok
//
//-----------------------------------------------------------------------------

MStatus CVstHelperBoneNode::Initialize()
{
	MFnCompoundAttribute cFn;
	MFnEnumAttribute eFn;
	MFnMatrixAttribute mFn;
	MFnNumericAttribute nFn;
	MFnTypedAttribute tFn;
	MFnUnitAttribute uFn;

    m_iaControlRotateAxis = nFn.create( "controlRotateAxis", "cra",
      uFn.create( "controlRotateAxisX", "crax", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "controlRotateAxisY", "cray", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "controlRotateAxisZ", "craz", MFnUnitAttribute::kAngle, 0.0 ) );
	nFn.setReadable( false );

    m_iaControlJointOrient = nFn.create( "controlJointOrient", "cjo",
      uFn.create( "controlJointOrientX", "cjox", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "controlJointOrientY", "cjoy", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "controlJointOrientZ", "cjoz", MFnUnitAttribute::kAngle, 0.0 ) );
	nFn.setReadable( false );

	m_iaControlParentWorldSpace = mFn.create( "parentSpace", "ps" );
	mFn.setHidden( true );
	mFn.setReadable( false );

    m_iaControlWorldSpace = mFn.create( "triggerSpace", "trs" );
	mFn.setHidden( true );
	mFn.setReadable( false );

    m_iaTriggerDrawAxis = nFn.create( "triggerDrawAxis", "tda",
      uFn.create( "triggerDrawAxisX", "tdax", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "triggerDrawAxisY", "tday", MFnUnitAttribute::kDistance, 1.0 ),
      uFn.create( "triggerDrawAxisZ", "tdaz", MFnUnitAttribute::kDistance, 0.0 ) );

	MFnUnitAttribute( nFn.child( 1 ) ).setDefault( MDistance( 1.0 ) );

    m_iaBaseTranslate = nFn.create( "baseTranslate", "bt",
      uFn.create( "baseTranslateX", "btx", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "baseTranslateY", "bty", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "baseTranslateZ", "btz", MFnUnitAttribute::kDistance, 0.0 ) );
	nFn.setReadable( false );

    m_iaBaseRotate = nFn.create( "baseRotate", "br",
      uFn.create( "baseRotateX", "brx", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "baseRotateY", "bry", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "baseRotateZ", "brz", MFnUnitAttribute::kAngle, 0.0 ) );
	nFn.setReadable( false );

	m_caMatrices = nFn.create( "caMatrices", "camt", MFnNumericData::kBoolean, false );
	nFn.setHidden( true );
	nFn.setStorable( false );
	nFn.setWritable( false );
	nFn.setConnectable( false );

	m_iaMaxRadius = uFn.create( "maxRadius", "maxr", MFnUnitAttribute::kDistance, 1.0 );
	uFn.setKeyable( true );
	uFn.setMin( 0.1 );
	uFn.setSoftMax( 5.0 );

	// Trigger Data

	m_iaTriggerTolerance = uFn.create( "tolerance", "tl", MFnUnitAttribute::kAngle, 0.0 );
	uFn.setSoftMin( MAngle( 0.0, MAngle::kDegrees ).asRadians() );
	uFn.setSoftMax( MAngle( 90.0, MAngle::kDegrees ).asRadians() );

	m_iaTriggerDraw = nFn.create( "triggerDraw", "td", MFnNumericData::kBoolean, true );

    m_iaTriggerAxis = nFn.create( "triggerAxis", "ta",
      uFn.create( "triggerAxisX", "tax", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "triggerAxisY", "tay", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "triggerAxisZ", "taz", MFnUnitAttribute::kAngle, 0.0 ) );

    m_iaTriggerTranslate = nFn.create( "triggerTranslate", "tt",
      uFn.create( "triggerTranslateX", "ttx", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "triggerTranslateY", "tty", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "triggerTranslateZ", "ttz", MFnUnitAttribute::kDistance, 0.0 ) );

    m_iaTriggerRotate = nFn.create( "triggerRotate", "tr",
      uFn.create( "triggerRotateX", "trx", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "triggerRotateY", "try", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "triggerRotateZ", "trz", MFnUnitAttribute::kAngle, 0.0 ) );

	m_iaTriggerName = tFn.create( "triggerName", "tn", MFnData::kString, MFnStringData().create() );

	m_iaTrigger = cFn.create( "trigger", "trg" );
	cFn.setArray( true );
	cFn.setIndexMatters( false );
	cFn.addChild( m_iaTriggerTolerance );
	cFn.addChild( m_iaTriggerAxis );
	cFn.addChild( m_iaTriggerTranslate );
	cFn.addChild( m_iaTriggerRotate );
	cFn.addChild( m_iaTriggerDraw );
	cFn.addChild( m_iaTriggerName );

	m_caTriggerData = nFn.create( "caTriggerData", "catd", MFnNumericData::kBoolean, false );
	nFn.setHidden( true );
	nFn.setStorable( false );
	nFn.setWritable( false );
	nFn.setConnectable( false );

    m_oaTranslate = nFn.create( "translate", "t",
      uFn.create( "translateX", "tx", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "translateY", "ty", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "translateZ", "tz", MFnUnitAttribute::kDistance, 0.0 ) );
	nFn.setReadable( true );
	nFn.setWritable( false );
	nFn.setConnectable( true );
	nFn.setStorable( false );
	nFn.setCached( false );
	nFn.setKeyable( false );

	m_oaTranslateX = nFn.child( 0 );
	m_oaTranslateY = nFn.child( 1 );
	m_oaTranslateZ = nFn.child( 2 );

    m_oaRotate = nFn.create( "rotate", "r",
      uFn.create( "rotateX", "rx", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "rotateY", "ry", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "rotateZ", "rz", MFnUnitAttribute::kAngle, 0.0 ) );
	nFn.setReadable( true );
	nFn.setWritable( false );
	nFn.setConnectable( true );
	nFn.setStorable( false );
	nFn.setCached( false );
	nFn.setKeyable( false );

	m_oaRotateX = nFn.child( 0 );
	m_oaRotateY = nFn.child( 1 );
	m_oaRotateZ = nFn.child( 2 );

	m_iaSubdivisons = nFn.create( "subdivisions", "subd", MFnNumericData::kInt, 4 );
	nFn.setMin( 1 );
	nFn.setSoftMax( 10 );
	nFn.setKeyable( true );

	m_oaNumTriangles = nFn.create( "numTriangles", "numt", MFnNumericData::kInt, 0 );
	nFn.setHidden( true );
	nFn.setStorable( false );
	nFn.setWritable( false );
	nFn.setConnectable( false );

	m_iaDrawWireframe = eFn.create( "drawWireframe", "dwf", 1 );
	eFn.addField( "None", kNone );
	eFn.addField( "Backface", kBack );
	eFn.addField( "Frontface", kFront );
	eFn.addField( "Both", kBoth );
	eFn.setKeyable( true );

	m_iaWireframeOnShaded = eFn.create( "wireOnShaded", "wfos", 0 );
	eFn.addField( "false", 0 );
	eFn.addField( "true", 1 );
	eFn.setKeyable( true );

	addAttribute( m_iaControlRotateAxis );

	addAttribute( m_iaControlJointOrient );

	addAttribute( m_iaControlParentWorldSpace );

	addAttribute( m_iaControlWorldSpace );

	addAttribute( m_iaTriggerDrawAxis );

	addAttribute( m_iaBaseTranslate );

	addAttribute( m_iaBaseRotate );

	addAttribute( m_caMatrices );

	addAttribute( m_iaMaxRadius );

	addAttribute( m_iaTrigger );

	addAttribute( m_caTriggerData );

	addAttribute( m_oaTranslate );
	addAttribute( m_oaRotate );

	addAttribute( m_iaSubdivisons );
	addAttribute( m_oaNumTriangles );

	addAttribute( m_iaDrawWireframe );
	addAttribute( m_iaWireframeOnShaded );

	attributeAffects( m_iaControlRotateAxis, m_caMatrices );
	attributeAffects( m_iaControlJointOrient, m_caMatrices );
	attributeAffects( m_iaControlRotateAxis, m_caMatrices );
	attributeAffects( m_iaControlJointOrient, m_caMatrices );
	attributeAffects( m_iaControlParentWorldSpace, m_caMatrices );
	attributeAffects( m_iaControlWorldSpace, m_caMatrices );
	attributeAffects( m_iaMaxRadius, m_caMatrices );
	attributeAffects( m_iaBaseTranslate, m_caMatrices );
	attributeAffects( m_iaBaseRotate, m_caMatrices );
	attributeAffects( m_iaTriggerDrawAxis, m_caMatrices );

	attributeAffects( m_iaControlRotateAxis, m_caTriggerData );
	attributeAffects( m_iaControlJointOrient, m_caTriggerData );
	attributeAffects( m_iaTrigger, m_caTriggerData );

	attributeAffects( m_iaControlRotateAxis, m_oaTranslate );
	attributeAffects( m_iaControlJointOrient, m_oaTranslate );
	attributeAffects( m_iaControlParentWorldSpace, m_oaTranslate );
	attributeAffects( m_iaControlWorldSpace, m_oaTranslate );
	attributeAffects( m_caMatrices, m_oaTranslate );
	attributeAffects( m_iaMaxRadius, m_oaTranslate );
	attributeAffects( m_iaBaseTranslate, m_oaTranslate );
	attributeAffects( m_iaBaseRotate, m_oaTranslate );
	attributeAffects( m_iaTrigger, m_oaTranslate );
	attributeAffects( m_caTriggerData, m_oaTranslate );
	attributeAffects( MPxNode::state, m_oaTranslate );

	attributeAffects( m_iaControlRotateAxis, m_oaRotate );
	attributeAffects( m_iaControlJointOrient, m_oaRotate );
	attributeAffects( m_iaControlParentWorldSpace, m_oaRotate );
	attributeAffects( m_iaControlWorldSpace, m_oaRotate );
	attributeAffects( m_caMatrices, m_oaRotate );
	attributeAffects( m_iaMaxRadius, m_oaRotate );
	attributeAffects( m_iaBaseTranslate, m_oaRotate );
	attributeAffects( m_iaBaseRotate, m_oaRotate );
	attributeAffects( m_iaTrigger, m_oaRotate );
	attributeAffects( m_caTriggerData, m_oaRotate );
	attributeAffects( MPxNode::state, m_oaRotate );

	attributeAffects( m_iaSubdivisons, m_oaNumTriangles );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose: Initialize the node, add static attributes, etc...
// Input  :
// Output : MStatus::kSuccess if everything was ok
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneNode::compute(
	const MPlug &mPlug,
	MDataBlock &mDataBlock )
{
	if ( mPlug == m_caTriggerData )
	{
		// Get the trigger data
		MArrayDataHandle tADH( mDataBlock.inputArrayValue( m_iaTrigger ) );
		const uint tEnd( tADH.elementCount() );

		// Only go until the logical and physical indexes do not match
		// For this usage, physical & logical must be the same, any logical indexes out of range are invalid

		// TODO: This could be optimised to only copy dirty data by checking the dirty status of the
		//       element attributes

		const MQuaternion rotateAxis( MEulerRotation( mDataBlock.inputValue( m_iaControlRotateAxis ).asVector() ).asQuaternion() );
		const MQuaternion jointOrient( MEulerRotation( mDataBlock.inputValue( m_iaControlJointOrient ).asVector() ).asQuaternion() );

		for ( uint ti( 0U ); ti == tADH.elementIndex() && ti != tEnd; ++ti, tADH.next() )
		{
			MDataHandle tDH( tADH.inputValue() );

			m_triggers.EnsureCount( ti + 1 );
			CHelperBoneTrigger &t( m_triggers[ ti ] );

			t.m_draw = tDH.child( m_iaTriggerDraw ).asBool();
			t.m_tolerance = tDH.child( m_iaTriggerTolerance ).asAngle().asRadians();
			t.m_invTolerance = t.m_tolerance == 0.0 ? 0.0 : 1.0 / t.m_tolerance;
			t.m_trigger = rotateAxis * MEulerRotation( tDH.child( m_iaTriggerAxis ).asVector() ).asQuaternion() * jointOrient;
			t.m_pos = tDH.child( m_iaTriggerTranslate ).asVector();
			t.m_quat = MEulerRotation( tDH.child( m_iaTriggerRotate ).asVector() ).asQuaternion();
			t.m_name = tDH.child( m_iaTriggerName ).asString();
		}

		for ( int j( m_triggers.Count() - 1 ); j >= 0 && j >= static_cast< int >( tEnd ); --j )
		{
			m_triggers.Remove( j );
		}

		mDataBlock.setClean( m_caTriggerData );

		return MS::kSuccess;
	}
	else if ( mPlug == m_caMatrices )
	{
		m_maxRadius = mDataBlock.inputValue( m_iaMaxRadius ).asDistance().as( MDistance::internalUnit() );

		// Get The Parent World Space Matrix
		const MMatrix &parentMatrix( mDataBlock.inputValue( m_iaControlParentWorldSpace ).asMatrix() );
		MQuaternion q;
		q = parentMatrix;
		m_parentRotateMatrix = q.asMatrix();

		MMatrix inverseParentMatrix = parentMatrix.inverse();
		// Get The Control World Space Matrix
		m_triggerWorldMatrix = mDataBlock.inputValue( m_iaControlWorldSpace ).asMatrix();
		m_triggerWorldTranslateMatrix[ 3 ][ 0 ] = m_triggerWorldMatrix[ 3 ][ 0 ];
		m_triggerWorldTranslateMatrix[ 3 ][ 1 ] = m_triggerWorldMatrix[ 3 ][ 1 ];
		m_triggerWorldTranslateMatrix[ 3 ][ 2 ] = m_triggerWorldMatrix[ 3 ][ 2 ];

		m_triggerLocalMatrix = m_triggerWorldMatrix * inverseParentMatrix;
		q = m_triggerLocalMatrix;
		m_triggerLocalMatrixQuat.x = q.x;
		m_triggerLocalMatrixQuat.y = q.y;
		m_triggerLocalMatrixQuat.z = q.z;
		m_triggerLocalMatrixQuat.w = q.w;

		m_baseTranslate = mDataBlock.inputValue( m_iaBaseTranslate ).asVector();
		m_baseRotate = mDataBlock.inputValue( m_iaBaseRotate ).asVector();
		m_baseRotateQuat = MEulerRotation( m_baseRotate ).asQuaternion();
		m_triggerDrawAxis = mDataBlock.inputValue( m_iaTriggerDrawAxis ).asVector();

		MPoint n( -m_maxRadius, -m_maxRadius, -m_maxRadius, 1.0 );
		MPoint x(  m_maxRadius,  m_maxRadius,  m_maxRadius, 1.0 );

		m_wbbox = MBoundingBox( n, x );
		m_wbbox.transformUsing( m_parentRotateMatrix * m_triggerWorldTranslateMatrix );

		mDataBlock.setClean( m_caMatrices );

		return MS::kSuccess;
	}
	else if (
		mPlug == m_oaTranslate ||
		mPlug == m_oaTranslateX ||
		mPlug == m_oaTranslateY ||
		mPlug == m_oaTranslateZ ||
		mPlug == m_oaRotate ||
		mPlug == m_oaRotateX ||
		mPlug == m_oaRotateY ||
		mPlug == m_oaRotateZ )
	{
		MObject thisObj( thisMObject() );

		// Ensure the matrices are cached
		mDataBlock.inputValue( m_caMatrices );

		// Ensure the trigger data is cached
		mDataBlock.inputValue( m_caTriggerData ).asBool();

		const int nTriggers( m_triggers.Count() );
		if ( nTriggers <= 0 || mDataBlock.inputValue( MPxNode::state ).asShort() == 1 )
		{
			// No triggers...
			// Copy base values to output... all done!
			mDataBlock.outputValue( m_oaTranslate ).set( m_baseTranslate );
			mDataBlock.outputValue( m_oaRotate ).set( m_baseRotate );

			return MS::kSuccess;
		}

		// Classic Helper Bone Behavior
		//  * Base position is added to each trigger state
		//  * If no triggers are active then trigger 0 is used
		//  * Weighted quaternions are simply added together instead of in log space

		float scale = ComputeTriggerWeights( m_triggerLocalMatrixQuat );

		if ( scale <= 0.001 )
		{
			// Use trigger 0
			const CHelperBoneTrigger &trigger( m_triggers[ 0 ] );

			MVector pos( trigger.m_pos + m_baseTranslate );

			mDataBlock.outputValue( m_oaTranslate ).set( pos );
			MQuaternion q( m_baseRotateQuat * trigger.m_quat );
			mDataBlock.outputValue( m_oaRotate ).set( q.asEulerRotation().asVector() );
		}
		else
		{
			scale = 1.0 / scale;

			Quaternion quat;
			Vector pos;

			quat.Init( 0, 0, 0, 0);
			pos.Init();

			for ( int ti( 0 ); ti < nTriggers; ++ti )
			{
				CHelperBoneTrigger &trigger( m_triggers[ ti ] );
				if ( trigger.m_weight )
				{
					float s = trigger.m_weight * scale;
					MQuaternion q( m_baseRotateQuat * trigger.m_quat );
					const Quaternion triggerQuat( q.x, q.y, q.z, q.w );

					QuaternionAlign( triggerQuat, quat, quat );

					quat.x = quat.x + s * triggerQuat.x;
					quat.y = quat.y + s * triggerQuat.y;
					quat.z = quat.z + s * triggerQuat.z;
					quat.w = quat.w + s * triggerQuat.w;
					pos.x = pos.x + s * ( trigger.m_pos.x + m_baseTranslate.x );
					pos.y = pos.y + s * ( trigger.m_pos.y + m_baseTranslate.y );
					pos.z = pos.z + s * ( trigger.m_pos.z + m_baseTranslate.z );
				}
			}

			MVector p( pos.x, pos.y, pos.z );
			MQuaternion q( quat.x, quat.y, quat.z, quat.w );

			mDataBlock.outputValue( m_oaTranslate ).set( p );
			mDataBlock.outputValue( m_oaRotate ).set( q.asEulerRotation().asVector() );
		}

		return MS::kSuccess;
	}
	else if ( mPlug == m_oaNumTriangles )
	{
		return ComputeConeData( mDataBlock );
	}

	return MS::kUnknownParameter;
}


//-----------------------------------------------------------------------------
// Purpose: Return the bounding box for the node, Maya uses this to determine
//			if the node needs to be drawn and for selection
//-----------------------------------------------------------------------------
MBoundingBox CVstHelperBoneNode::boundingBox() const
{
	// TODO:	This isn't quite correct... the bounding box in world space is
	//			correct but removing the inverse of the world matrix doesn't
	//			seem to do the trick.  Transforming the bounding box does
	//			an even worse job..  Seems like it still gets affected by the
	//			local space transform on this node, although worldSpace is
	//			supposed to include that...

	const MObject thisObj( thisMObject() );

	{
		MPlug mPlug( thisObj, m_caMatrices );
		bool junkVal;
		mPlug.getValue( junkVal );
	}

	MObject mObj;
	{
		MPlug mPlug( thisObj, worldInverseMatrix );
		mPlug = mPlug.elementByLogicalIndex( 0 );
		mPlug.getValue( mObj );
	}

	const MMatrix &wiMat( MFnMatrixData( mObj ).matrix() );

	MPoint n( m_wbbox.min() );
	MPoint x( m_wbbox.max() );

	n *= wiMat;
	x *= wiMat;

	MBoundingBox bbox( n, x );

	return bbox;
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
//
//-----------------------------------------------------------------------------
MString DoPrefixStuff(
	const MString &name,
	const MString &addPrefix,
	const MString &delPrefix )
{
	MString retVal( RemoveNameSpace( name ) );

	if ( delPrefix.length() )
	{
		if ( Q_strncmp( retVal.asChar(), delPrefix.asChar(), delPrefix.length() ) )
		{
			retVal = retVal.substring( delPrefix.length(), retVal.length() - 1 );
		}
	}

	if ( addPrefix.length() )
	{
		retVal = addPrefix + retVal;
	}

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneNode::DoExport(
	std::ostream &os,
	const MString &addPrefix,
	const MString &delPrefix )
{
	MObject helperBoneObj( thisMObject() );

	MPlug tAP( helperBoneObj, m_iaTrigger );
	if ( tAP.isNull() )
		return MS::kFailure;

	MDagPath helperShapePath;
	MDagPath::getAPathTo( helperBoneObj, helperShapePath );

	MDagPath helperPath( helperShapePath );
	helperPath.pop();

	MDagPath helperParentPath( helperPath );
	helperParentPath.pop();

	if ( !( helperShapePath.length() || helperPath.length() || helperParentPath.length() ) )
	{
		merr << "Can't create VRD - Can't figure out dag path to helper bone \"" << MFnDependencyNode( thisMObject() ).name().asChar() << "\" and its parent" << std::endl;
		return MS::kFailure;
	}

	const uint nTriggers( tAP.evaluateNumElements() );
	if ( nTriggers == 0U )
	{
		merr << "Can't create VRD - No triggers on helper bone: " << helperShapePath.partialPathName().asChar() << std::endl;
		return MS::kFailure;
	}

	MDagPath controlPath;
	MDagPath controlParentPath;

	{
		MPlug csP( helperBoneObj, m_iaControlWorldSpace );
		MPlugArray pA;
		if ( csP.connectedTo( pA, true, false ) && pA.length() == 1U )
		{
			MObject controlNode = pA[ 0 ].node();
			MDagPathArray controlPaths;
			if ( MDagPath::getAllPathsTo( controlNode, controlPaths ) && controlPaths.length() > pA[ 0 ].logicalIndex() )
			{
				controlPath = controlPaths[ pA[ 0 ].logicalIndex() ];
				controlParentPath = controlPath;
				controlParentPath.pop();
				if ( !( controlPath.length() || controlParentPath.length() ) )
				{
					mwarn << "Can't create VRD - Can't figure out path of control bone and its parent: " << MFnDependencyNode( controlNode ).name().asChar() << std::endl;
					return MS::kFailure;
				}
			}
			else
			{
				mwarn << "Can't create VRD - Can't figure out parent of control bone: " << MFnDependencyNode( controlNode ).name().asChar() << std::endl;
				return MS::kFailure;
			}
		}
		else
		{
			mwarn << "Can't create VRD - No control bone connected to helper bone: " << helperShapePath.partialPathName().asChar() << "." << csP.partialName() << std::endl;
			return MS::kFailure;
		}
	}

	// <helper> helper helperParent controlParent control

	os << "<helper>";
	os << " " << DoPrefixStuff( MFnDagNode( helperPath ).name(), addPrefix, delPrefix ).asChar();
	os << " " << DoPrefixStuff( MFnDagNode( helperParentPath ).name(), addPrefix, delPrefix ).asChar();
	os << " " << DoPrefixStuff( MFnDagNode( controlParentPath ).name(), addPrefix, delPrefix ).asChar();
	os << " " << DoPrefixStuff( MFnDagNode( controlPath ).name(), addPrefix, delPrefix ).asChar();
	os << "\n";

	MDistance btx, bty, btz;
	MPlug btP( helperBoneObj, m_iaBaseTranslate );
	btP.child( 0 ).getValue( btx );
	btP.child( 1 ).getValue( bty );
	btP.child( 2 ).getValue( btz );

	os << "<basepos> " << btx.value() << " " << bty.value() << " " << btz.value() << "\n";

	MAngle rx, ry, rz;

	const MFnDagNode hFn( helperShapePath );
	MPlug raP( hFn.findPlug( m_iaControlRotateAxis ) );
	if ( !raP.isNull() )
	{
		raP.child( 0 ).getValue( rx );
		raP.child( 1 ).getValue( ry );
		raP.child( 2 ).getValue( rz );
		if ( rx.value() != 0.0 || ry.value() != 0.0 || rz.value() != 0.0 )
		{
			os << "<rotateaxis> " << rx.asDegrees() << " " << ry.asDegrees() << " " << rz.asDegrees() << "\n";
		}
	}

	MPlug joP( hFn.findPlug( m_iaControlJointOrient ) );
	if ( !joP.isNull() )
	{
		joP.child( 0 ).getValue( rx );
		joP.child( 1 ).getValue( ry );
		joP.child( 2 ).getValue( rz );
		if ( rx.value() != 0.0 || ry.value() != 0.0 || rz.value() != 0.0 )
		{
			os << "<jointorient> " << rx.asDegrees() << " " << ry.asDegrees() << " " << rz.asDegrees() << "\n";
		}
	}

	for ( uint i( 0U ); i != nTriggers; ++i )
	{
		MPlug tP( tAP.elementByPhysicalIndex( i ) );

		MAngle tolerance;
		tP.child( m_iaTriggerTolerance ).getValue( tolerance );

		MAngle ax, ay, az;
		MPlug taP( tP.child( m_iaTriggerAxis ) );
		taP.child( 0 ).getValue( ax );
		taP.child( 1 ).getValue( ay );
		taP.child( 2 ).getValue( az );

		MAngle rx, ry, rz;
		MPlug trP( tP.child( m_iaTriggerRotate ) );
		trP.child( 0 ).getValue( rx );
		trP.child( 1 ).getValue( ry );
		trP.child( 2 ).getValue( rz );

		MDistance tx, ty, tz;
		MPlug ttP( tP.child( m_iaTriggerTranslate ) );
		ttP.child( 0 ).getValue( tx );
		ttP.child( 1 ).getValue( ty );
		ttP.child( 2 ).getValue( tz );

		MString tName;
		MPlug tnP( tP.child( m_iaTriggerName ) );
		tnP.getValue( tName );

		os << "<trigger>";
		os << " " << tolerance.asDegrees();
		os << " " << ax.asDegrees() << " " << ay.asDegrees() << " " << az.asDegrees();
		os << " " << rx.asDegrees() << " " << ry.asDegrees() << " " << rz.asDegrees();
		os << " " << tx.value() << " " << ty.value() << " " << tz.value();
		if ( tName.length() )
		{
			os << " " << tName.asChar();
		}
		os << "\n";
	}

	os << "\n";

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstHelperBoneNode::DrawBoundingBox() const
{
	const MPoint n( m_wbbox.min() );
	const MPoint x( m_wbbox.max() );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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


//-----------------------------------------------------------------------------
// Compute the vertices and normals for the current cone settings
// The cone's point is at the origin and opens out along the
// positive Z axis and ends in a unit circle in the XY plane one unit from
// the origin in Z
//-----------------------------------------------------------------------------
MStatus CVstHelperBoneNode::ComputeConeData(
	MDataBlock &mDataBlock )
{
	const uint nSubdivisions(
		static_cast< uint >( clamp( mDataBlock.inputValue( m_iaSubdivisons ).asInt(), 0, 100 ) ) );
	const uint nCirclePoints( nSubdivisions * 4 );
	const float fCirclePoints( nCirclePoints );

	m_unitCirclePoints.setLength( nCirclePoints );
	m_unitNormalPoints.setLength( nCirclePoints );
	for ( uint i( 0U ); i != nCirclePoints; ++i )
	{
		const float p( i / fCirclePoints * 2.0 * M_PI );
		m_unitCirclePoints[ i ] = MFloatVector( cos( p ), 1.0, sin( p ) );

		const float n( ( i + 0.5f ) / fCirclePoints * 2.0 * M_PI );
		m_unitNormalPoints[ i ] = MFloatVector( cos( n ), 1.0, sin( n ) );
	}

	mDataBlock.outputValue( m_oaNumTriangles ).set( static_cast< int >( nCirclePoints ) );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Draw a cone.  The cone's point is at the origin and opens out along the
// positive Z axis and ends in a unit circle in the XY plane one unit from
// the origin in Z
//-----------------------------------------------------------------------------
void CVstHelperBoneNode::DrawCone(
	float coneHalfAngle,
	float sphereRadius,
	float weight ) const
{
	float r;
	float h;
	float nr;
	float nh;

	if ( coneHalfAngle == static_cast< float >( M_PI / 2.0 ) )
	{
		r = sphereRadius;
		h = 0.0f;
		nr = 0.0;
		nh = 1.0f;
	}
	else
	{
		const float ss( sphereRadius * sphereRadius );

		const float t( tanf( coneHalfAngle ) );
		const float ts( t * t );
		r = sqrtf( ( ts * ss ) / ( 1.0f + ts ) );
		h = sqrtf( ss - ( r * r ) );

		const float nt( tanf( M_PI / 2.0f - coneHalfAngle ) );
		const float nts( nt * nt );
		nr = sqrtf( nts / ( 1.0f + nts ) );
		nh = sqrtf( 1.0f - ( nr * nr ) );
	}

	const uint nVertices( m_unitCirclePoints.length() );

	MFloatVector *pUnitCirclePoints( reinterpret_cast< MFloatVector * >( alloca( nVertices * sizeof( MFloatVector ) ) ) );
	MFloatVector *pNormals( reinterpret_cast< MFloatVector * >( alloca( nVertices * sizeof( MFloatVector ) ) ) );

	for ( uint i( 0U ); i != nVertices; ++i )
	{
		const MFloatVector &ps( m_unitCirclePoints[ i ] );

		MFloatVector &pd( pUnitCirclePoints[ i ] );
		pd.x = ps.x * r;
		pd.y = ps.y * h;
		pd.z = ps.z * r;

		const MFloatVector &ns( m_unitNormalPoints[ i ] );
		MFloatVector &nd( pNormals[ i ] );
		nd.x = ns.x * nr;
		nd.y = ns.y * -nh;
		nd.z = ns.z * nr;
	}

	glTexCoord2f( 0.0, clamp( weight, 0.01, 0.99 ) );

	glBegin( GL_TRIANGLES );
	for ( uint i( 0U ); i != nVertices; ++i )
	{
		const MFloatVector &v1( pUnitCirclePoints[ i ] );
		const MFloatVector &v2( pUnitCirclePoints[ ( i + 1 ) % nVertices ] );
		const MFloatVector &n( pNormals[ i ] );

		glNormal3fv( &n.x );
		glVertex3fv( &MFloatVector::zero.x );
		glVertex3fv( &v1.x );
		glVertex3fv( &v2.x );
	}
	glEnd();
}


//-----------------------------------------------------------------------------
// Returns true if the user has specified that this node should be rendered
// in wireframe on shaded mode
//-----------------------------------------------------------------------------
bool CVstHelperBoneNode::WireframeOnShaded() const
{
	MObject thisObj( thisMObject() );
	MPlug mPlug( thisObj, m_iaWireframeOnShaded );
	bool retVal;
	mPlug.getValue( retVal );

	return retVal;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float CVstHelperBoneNode::ComputeTriggerWeights( const Quaternion &controlQuat )
{
	Quaternion quat;
	float scale( 0.0f );

	const int nTriggerCount = m_triggers.Count();
	for ( int ti = 0; ti < nTriggerCount; ++ti )
	{
		CHelperBoneTrigger &trigger( m_triggers[ ti ] );
		if ( trigger.m_tolerance == 0.0 )
		{
			trigger.m_weight = 0.0;
		}
		else
		{
			const Quaternion triggerQuat( trigger.m_trigger.x, trigger.m_trigger.y, trigger.m_trigger.z, trigger.m_trigger.w );
			float dot = fabs( QuaternionDotProduct( triggerQuat, controlQuat ) );
			dot = clamp( dot, -1.0f, 1.0f );
			trigger.m_weight = 1 - ( 2 * acos( dot ) * trigger.m_invTolerance );
			trigger.m_weight = max( 0.0, trigger.m_weight );
			scale += trigger.m_weight;
		}
	}

	return scale;
}


//=============================================================================
//
// CVstHelperBoneUI
//
//=============================================================================


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVstHelperBoneUI::CVstHelperBoneUI()
{
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstHelperBoneUI::getDrawRequests(
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
	CVstHelperBoneNode *pHelperBone( reinterpret_cast< CVstHelperBoneNode * >( surfaceShape() ) );
	MDrawRequest mDrawRequest( mDrawInfo.getPrototype( *this ) );
	getDrawData( pHelperBone, mDrawData );
	mDrawRequest.setDrawData( mDrawData );

	const M3dView::DisplayStyle  displayStyle( mDrawInfo.displayStyle() );
	const M3dView::DisplayStatus displayStatus( mDrawInfo.displayStatus() );

	const M3dView::ColorTable activeColorTable( M3dView::kActiveColors );
	const M3dView::ColorTable dormantColorTable( M3dView::kDormantColors );

	switch ( displayStyle )
	{
	case M3dView::kBoundingBox:
		mDrawRequest.setToken( kDrawBoundingBox );
		mDrawRequestQueue.add( mDrawRequest );
		break;
	case M3dView::kWireFrame:
		{
			mDrawRequest.setToken( kDrawWireframe );

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
				displayStatus == M3dView::kHilite ||
				pHelperBone->WireframeOnShaded() )
			{
				MDrawRequest wireRequest = mDrawInfo.getPrototype( *this );
				wireRequest.setDrawData( mDrawData );
				wireRequest.setToken( kDrawWireframeOnShaded );
				wireRequest.setDisplayStyle( M3dView::kWireFrame );

				switch ( displayStatus )
				{
				case M3dView::kLead :
					wireRequest.setColor( LEAD_COLOR, activeColorTable );
					break;
				case M3dView::kActive :
					wireRequest.setColor( ACTIVE_COLOR, activeColorTable );
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
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstHelperBoneUI::draw(
	const MDrawRequest &mDrawRequest,
	M3dView &m3dView ) const
{
	m3dView.beginGL();

	DoDraw( mDrawRequest.multiPath(), m3dView, mDrawRequest.token(), &mDrawRequest );

	m3dView.endGL();
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstHelperBoneUI::DoDraw(
	const MDagPath &mDagPath,
	M3dView &m3dView,
	short drawToken,
	const MDrawRequest *pDrawRequest ) const
{
	// Make sure all of the data is computated
	CVstHelperBoneNode *pHelperBone( reinterpret_cast< CVstHelperBoneNode * >( surfaceShape() ) );
	MObject helperBoneObj( pHelperBone->thisMObject() );
	short drawWireframe( CVstHelperBoneNode::kNone );

	{
		bool junkVal;

		MPlug mPlug( helperBoneObj, CVstHelperBoneNode::m_caMatrices );
		mPlug.getValue( junkVal );

		mPlug.setAttribute( CVstHelperBoneNode::m_caTriggerData );
		mPlug.getValue( junkVal );

		int numTriangles;
		mPlug.setAttribute( CVstHelperBoneNode::m_oaNumTriangles );
		mPlug.getValue( numTriangles );

		mPlug.setAttribute( CVstHelperBoneNode::m_iaDrawWireframe );
		mPlug.getValue( drawWireframe );
	}

	const MMatrix inclusiveMatrixInverse( mDagPath.inclusiveMatrixInverse() );
	const MVector &tda( pHelperBone->m_triggerDrawAxis );
	const MMatrix cltm( MQuaternion( tda.rotateTo( MVector( 0.0, 1.0, 0.0 ) ) ).asMatrix() );

	glPushAttrib( GL_CURRENT_BIT | GL_POLYGON_BIT | GL_MATRIX_MODE );
	{
		glMatrixMode( GL_MODELVIEW );

		glPushMatrix();
		{
			// Take this thing to world space from the current local space
			glMultMatrixd( &inclusiveMatrixInverse.matrix[ 0 ][ 0 ] );

			bool turnOffPolygonOffsetFill( false );
			bool turnOffTexture2D( false );
			bool lightingWasOn( ( glIsEnabled( GL_LIGHTING ) ? true : false ) );
			bool lightingIsOn( lightingWasOn );

			if ( drawToken == kDrawBoundingBox )
			{
				if ( lightingIsOn )
				{
					glDisable( GL_LIGHTING );
					lightingIsOn = false;
				}

				pHelperBone->DrawBoundingBox();
			}
			else
			{
				if ( drawToken == kDrawSmoothShaded || drawToken == kDrawFlatShaded )
				{
					if ( !glIsEnabled( GL_POLYGON_OFFSET_FILL ) )
					{
						turnOffPolygonOffsetFill = true;
						glEnable( GL_POLYGON_OFFSET_FILL );
					}

					switch ( drawWireframe )
					{
					case CVstHelperBoneNode::kFront:
						glPolygonMode( GL_FRONT, GL_LINE );
						glPolygonMode( GL_BACK, GL_FILL );
						break;
					case CVstHelperBoneNode::kBack:
						glPolygonMode( GL_FRONT, GL_FILL );
						glPolygonMode( GL_BACK, GL_LINE );
						break;
					case CVstHelperBoneNode::kBoth:
						glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
						break;
					default:
						glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
						break;
					}

					if ( pDrawRequest )
					{
						MMaterial material( pDrawRequest->material() );
						material.setMaterial( pDrawRequest->multiPath(), pDrawRequest->isTransparent() );
						if ( material.materialIsTextured() )
						{
							if ( !glIsEnabled( GL_TEXTURE_2D ) )
							{
								turnOffTexture2D = true;
								glEnable( GL_TEXTURE_2D );
							}
							material.applyTexture( m3dView, pDrawRequest->drawData() );
						}
					}
				}
				else
				{
					glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					if ( lightingIsOn )
					{
						glDisable( GL_LIGHTING );
						lightingIsOn = false;
					}
				}

				const double &maxRadius( pHelperBone->m_maxRadius );

				glPushMatrix();
				{
					glMultMatrixd( &pHelperBone->m_triggerWorldMatrix.matrix[ 0 ][ 0 ] );
					glMultMatrixd( &cltm.matrix[ 0 ][ 0 ] );
					DrawArrow( maxRadius * 1.5 );
				}
				glPopMatrix();

				const MMatrix drawMat( pHelperBone->m_parentRotateMatrix * pHelperBone->m_triggerWorldTranslateMatrix );
				glMultMatrixd( &drawMat.matrix[ 0 ][ 0 ] );

				const int nTriggers( pHelperBone->m_triggers.Count() );
				for ( int ti( 0 ); ti < nTriggers; ++ti )
				{
					const CHelperBoneTrigger &trigger( pHelperBone->m_triggers[ ti ] );

					if ( !trigger.m_draw )
						continue;

					glPushMatrix();
					{
						const MMatrix tm( trigger.m_trigger.asMatrix() );

						glMultMatrixd( &tm.matrix[ 0 ][ 0 ] );

						glMultMatrixd( &cltm.matrix[ 0 ][ 0 ] );

						DrawArrow( maxRadius );

						pHelperBone->DrawCone( trigger.m_tolerance, maxRadius, trigger.m_weight );
					}

					glPopMatrix();
				}

				if ( turnOffPolygonOffsetFill )
				{
					glDisable( GL_POLYGON_OFFSET_FILL );
				}

				if ( turnOffTexture2D )
				{
					glDisable( GL_TEXTURE_2D );
				}
			}

			if ( lightingWasOn && lightingIsOn )
			{
				glEnable( GL_LIGHTING );
			}
		}
		glPopMatrix();
	}
	glPopAttrib();
}

//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
bool CVstHelperBoneUI::select(
	MSelectInfo &mSelectInfo,
	MSelectionList &mSelectionList,
	MPointArray &worldSpaceSelectPts ) const
{
	bool retVal( false );

	M3dView m3dView( mSelectInfo.view() );

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

	// Get the current GL Model View matrix:
	MMatrix mvmat;
	glGetDoublev(GL_MODELVIEW_MATRIX, &mvmat.matrix[ 0 ][ 0 ] );

	// Get the current GL Projection matrix:
	MMatrix pmat;
	glGetDoublev(GL_PROJECTION_MATRIX, &pmat.matrix[ 0 ][ 0 ] );

	if ( m3dView.endSelect() > 0  )
	{
		retVal = true;

		MPoint wPoint(0.0, 0.0, 0.0, 1.0);

		if ( mSelectInfo.singleSelection() ) {
			// Determine the depth of the selection:
			const double depth( static_cast< double >( selectBuf[ 1 ] ) / ( 65536.0 * 65536.0 - 1.0 ) );

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
void CVstHelperBoneUI::DrawArrow(
	float sphereRadius )
{
	glBegin( GL_LINE_STRIP );
	{
		glVertex3f(  0.0, 0.0, 0.0 );
		glVertex3f(  0.0, 1.5 * sphereRadius, 0.0 );
		glVertex3f(  0.1, 1.2 * sphereRadius, 0.0 );
		glVertex3f( -0.1, 1.2 * sphereRadius, 0.0 );
		glVertex3f(  0.0, 1.5 * sphereRadius, 0.0 );
	}
	glEnd();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstHelperBoneUI::DrawAxis()
{
	glPushAttrib( GL_CURRENT_BIT );

	// Draw Coordinate Axes
	glPushMatrix();
	glColor3f( 1.0f, 0.0f, 0.0f );
	glRotatef(  90.0f,  0.0f,  0.0f, -1.0f );
	DrawArrowhead();
	glPopMatrix();

	glPushMatrix();
	glColor3f( 0.0f, 1.0f, 0.0f );
	DrawArrowhead();
	glPopMatrix();

	glPushMatrix();
	glColor3f( 0.0f, 0.0f, 1.0f );
	glRotatef(  90.0f,  1.0f,  0.0f,  0.0f );
	DrawArrowhead();
	glPopMatrix();

	glPopAttrib();
}


//-----------------------------------------------------------------------------
// Purpose: Draw the locator...
//-----------------------------------------------------------------------------
void CVstHelperBoneUI::DrawArrowhead()
{
	static const float r2( sqrtf( 2.0f ) / 2.0f );

	glTranslatef( 0.0f, 1.0f, 0.0f );
	glScalef(  0.05f, 0.25f, 0.05f );

	glBegin( GL_TRIANGLE_FAN );

	// Tip of arrowhead
	glNormal3f(  0.0f,  0.0f,  0.0f );
	glVertex3f(  0.0f,  0.0f,  0.0f );

	// Base of arrowhead (unit circle points)
	glNormal3f(  1.0f, -1.0f,  0.0f );
	glVertex3f(  1.0f, -1.0f,  0.0f );

	glNormal3f(  r2,   -1.0f,  r2 );
	glVertex3f(  r2,   -1.0f,  r2 );

	glNormal3f(  0.0f, -1.0f,  1.0f );
	glVertex3f(  0.0f, -1.0f,  1.0f );

	glNormal3f(  -r2,  -1.0f,  r2 );
	glVertex3f(  -r2,  -1.0f,  r2 );

	glNormal3f( -1.0f, -1.0f,  0.0f );
	glVertex3f( -1.0f, -1.0f,  0.0f );

	glNormal3f( -r2,   -1.0f, -r2 );
	glVertex3f( -r2,   -1.0f, -r2 );

	glNormal3f(  0.0f, -1.0f, -1.0f );
	glVertex3f(  0.0f, -1.0f, -1.0f );

	glNormal3f(  r2,   -1.0f, -r2 );
	glVertex3f(  r2,   -1.0f, -r2 );

	glNormal3f(  1.0f, -1.0f,  0.0f );
	glVertex3f(  1.0f, -1.0f,  0.0f );

	glEnd();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVstHelperBoneUI::DrawMatrix()
{
	static const float colors[][3] =
	{
		{	1.0f,	0.0f,	0.0f },	// X: Red 
		{	0.0f,	1.0f,	0.0f },	// Y: Green
		{	0.0f,	0.0f,	1.0f }	// Z: Blue
	};

	static int color( 0 );

	const bool lighting( glIsEnabled( GL_LIGHTING ) != 0 );
	if ( lighting )
	{
		glDisable( GL_LIGHTING );
	}

	glBegin( GL_LINES );

	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 10.0f, 0.0f, 0.0f );

	glColor3f( 0.0f, 1.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 10.0f, 0.0f );

	glColor3f( 0.0f, 0.0f, 1.0f );
	glVertex3f( 0.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, 10.0f );

	glEnd();

	if ( lighting )
	{
		glEnable( GL_LIGHTING );
	}

	color = ( ( color + 1 ) % 3 );
}