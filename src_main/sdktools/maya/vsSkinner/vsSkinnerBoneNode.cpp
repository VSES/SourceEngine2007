//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================

// Maya includes
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MVectorArray.h>


// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"
#include "mathlib/mathlib.h"


//=============================================================================
//
//=============================================================================
class CVsSkinnerBoneNode : public CVsMayaMPxNode
{
public:
	CVsSkinnerBoneNode();

	virtual ~CVsSkinnerBoneNode();

	virtual void postConstructor();

	static MStatus Initialize();

	virtual MStatus compute(
		const MPlug &mPlug,
		MDataBlock &mDataBlock );

public:
	static MObject m_iaParentWorldMatrix;

	static MObject m_iaChildWorldMatrix;

	static MObject m_oaAxis;

	static MObject m_oaWorldMatrix;

    static MObject m_oaTranslate;
    static MObject m_oaTranslateX;
    static MObject m_oaTranslateY;
    static MObject m_oaTranslateZ;

    static MObject m_oaRotate;
    static MObject m_oaRotateX;
    static MObject m_oaRotateY;
    static MObject m_oaRotateZ;

    static MObject m_oaScale;
    static MObject m_oaScaleX;
    static MObject m_oaScaleY;
    static MObject m_oaScaleZ;

    static MObject m_oaShear;
    static MObject m_oaShearX;
    static MObject m_oaShearY;
    static MObject m_oaShearZ;
};


INSTALL_MAYA_MPXNODE(
	CVsSkinnerBoneNode,
	vsSkinnerBone,
	0x0010e4c8,
	CVsSkinnerBoneNode::Initialize,
	"Valve volume skinning helper bone node"
);


//-----------------------------------------------------------------------------
// Statics
//-----------------------------------------------------------------------------
MObject CVsSkinnerBoneNode::m_iaParentWorldMatrix;

MObject CVsSkinnerBoneNode::m_iaChildWorldMatrix;

MObject CVsSkinnerBoneNode::m_oaAxis;

MObject CVsSkinnerBoneNode::m_oaWorldMatrix;

MObject CVsSkinnerBoneNode::m_oaTranslate;
MObject CVsSkinnerBoneNode::m_oaTranslateX;
MObject CVsSkinnerBoneNode::m_oaTranslateY;
MObject CVsSkinnerBoneNode::m_oaTranslateZ;

MObject CVsSkinnerBoneNode::m_oaRotate;
MObject CVsSkinnerBoneNode::m_oaRotateX;
MObject CVsSkinnerBoneNode::m_oaRotateY;
MObject CVsSkinnerBoneNode::m_oaRotateZ;

MObject CVsSkinnerBoneNode::m_oaScale;
MObject CVsSkinnerBoneNode::m_oaScaleX;
MObject CVsSkinnerBoneNode::m_oaScaleY;
MObject CVsSkinnerBoneNode::m_oaScaleZ;

MObject CVsSkinnerBoneNode::m_oaShear;
MObject CVsSkinnerBoneNode::m_oaShearX;
MObject CVsSkinnerBoneNode::m_oaShearY;
MObject CVsSkinnerBoneNode::m_oaShearZ;


//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
CVsSkinnerBoneNode::CVsSkinnerBoneNode()
{
}


//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
CVsSkinnerBoneNode::~CVsSkinnerBoneNode()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CVsSkinnerBoneNode::postConstructor()
{
	setExistWithoutInConnections( false );
	setExistWithoutOutConnections( false );
}


//-----------------------------------------------------------------------------
// Initialize the node, add static attributes, etc...
//-----------------------------------------------------------------------------
MStatus CVsSkinnerBoneNode::Initialize()
{
	MFnEnumAttribute eFn;
	MFnMatrixAttribute mFn;
	MFnNumericAttribute nFn;
	MFnUnitAttribute uFn;

	m_iaParentWorldMatrix = mFn.create( "parentWorldMatrix", "pwm" );

	m_iaChildWorldMatrix = mFn.create( "childWorldMatrix", "cwm" );
	mFn.setDefault( MMatrix::identity );

	m_oaAxis = eFn.create( "axis", "a", 0 );
	eFn.addField( "x", 0 );
	eFn.addField( "y", 1 );
	eFn.addField( "z", 2 );
	eFn.setWritable( false );

	m_oaWorldMatrix = mFn.create( "worldMatrix", "wm" );
	mFn.setWritable( false );

	m_oaTranslate = nFn.create( "translate", "t",
		uFn.create( "translateX", "tx", MFnUnitAttribute::kDistance ),
		uFn.create( "translateY", "ty", MFnUnitAttribute::kDistance ),
		uFn.create( "translateZ", "tz", MFnUnitAttribute::kDistance ) );
	m_oaTranslateX = nFn.child( 0U );
	m_oaTranslateY = nFn.child( 1U );
	m_oaTranslateZ = nFn.child( 2U );
	nFn.setWritable( false );

	m_oaRotate = nFn.create( "rotate", "r",
		uFn.create( "rotateX", "rx", MFnUnitAttribute::kAngle ),
		uFn.create( "rotateY", "ry", MFnUnitAttribute::kAngle ),
		uFn.create( "rotateZ", "rz", MFnUnitAttribute::kAngle ) );
	m_oaRotateX = nFn.child( 0U );
	m_oaRotateY = nFn.child( 1U );
	m_oaRotateZ = nFn.child( 2U );
	nFn.setWritable( false );

	m_oaScale = nFn.create( "scale", "s",
		nFn.create( "scaleX", "sx", MFnNumericData::kDouble, 1.0 ),
		nFn.create( "scaleY", "sy", MFnNumericData::kDouble, 1.0 ),
		nFn.create( "scaleZ", "sz", MFnNumericData::kDouble, 1.0 ) );
	m_oaScaleX = nFn.child( 0U );
	m_oaScaleY = nFn.child( 1U );
	m_oaScaleZ = nFn.child( 2U );
	nFn.setWritable( false );

	m_oaShear = nFn.create( "shear", "sh",
		nFn.create( "shearX", "shx", MFnNumericData::kDouble, 0.0 ),
		nFn.create( "shearY", "shy", MFnNumericData::kDouble, 0.0 ),
		nFn.create( "shearZ", "shz", MFnNumericData::kDouble, 0.0 ) );
	m_oaShearX = nFn.child( 0U );
	m_oaShearY = nFn.child( 1U );
	m_oaShearZ = nFn.child( 2U );
	nFn.setWritable( false );

	addAttribute( m_iaParentWorldMatrix );

	addAttribute( m_iaChildWorldMatrix );

	addAttribute( m_oaAxis );

	addAttribute( m_oaWorldMatrix );

	addAttribute( m_oaTranslate );
	addAttribute( m_oaRotate );
	addAttribute( m_oaScale );
	addAttribute( m_oaShear );

	attributeAffects( m_iaParentWorldMatrix, m_oaAxis );
	attributeAffects( m_iaChildWorldMatrix, m_oaAxis );

	attributeAffects( m_iaParentWorldMatrix, m_oaWorldMatrix );
	attributeAffects( m_iaChildWorldMatrix, m_oaWorldMatrix );

	attributeAffects( m_iaParentWorldMatrix, m_oaTranslate );
	attributeAffects( m_iaChildWorldMatrix, m_oaTranslate );

	attributeAffects( m_iaParentWorldMatrix, m_oaRotate );
	attributeAffects( m_iaChildWorldMatrix, m_oaRotate );

	attributeAffects( m_iaParentWorldMatrix, m_oaScale );
	attributeAffects( m_iaChildWorldMatrix, m_oaScale );

	attributeAffects( m_iaParentWorldMatrix, m_oaShear );
	attributeAffects( m_iaChildWorldMatrix, m_oaShear );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
// Compute the value of outgoing attributes based on the value of input attributes
//-----------------------------------------------------------------------------
MStatus CVsSkinnerBoneNode::compute(
	const MPlug &mPlug,
	MDataBlock &mDataBlock )
{
	const MMatrix &pwm( mDataBlock.inputValue( m_iaParentWorldMatrix ).asMatrix() );
	const MMatrix cm( mDataBlock.inputValue( m_iaChildWorldMatrix ).asMatrix() * pwm.inverse() );

	double axes[][3] =
	{
		{  1.0,  0.0,  0.0 },
		{  0.0,  1.0,  0.0 },
		{  0.0,  0.0,  1.0 },
		{ -1.0,  0.0,  0.0 },
		{  0.0, -1.0,  0.0 },
		{  0.0,  0.0, -1.0 }
	};

	MMatrix m;

	short axis( 0 );

	MVector ct( cm[ 3 ] );
	if ( ct.isEquivalent( MVector::zero ) )
	{
		axis = 1;
		m = pwm;
	}
	else
	{
		ct.normalize();

		double dotProduct( MVector( axes[ axis ] ) * ct );
		for ( uint i( axis + 1 ); i != 6; ++i )
		{
			double tmpDotProduct( MVector( axes[ i ] ) * ct );
			if ( tmpDotProduct > dotProduct )
			{
				axis = i;
				dotProduct = tmpDotProduct;
			}
		}

		MVectorArray coordsys( 3 );

		if ( axis < 3 )
		{
			coordsys[ axis % 3 ] = ct;
		}
		else
		{
			coordsys[ axis % 3 ] = -ct;
		}

		if ( abs( coordsys[ axis % 3 ] * MVector( pwm[ ( axis + 2 ) % 3 ] ) ) < 0.001 )
		{
			coordsys[ ( axis + 1 ) % 3 ] = MVector( axes[ ( axis + 1 ) % 3 ] );
			coordsys[ ( axis + 2 ) % 3 ] = MVector( axes[ ( axis + 2 ) % 3 ] );
		}
		else
		{
			coordsys[ ( axis + 1 ) % 3 ] = ( MVector( pwm[ ( axis + 2 ) % 3 ] ) ^ coordsys[ axis % 3 ] ).normal();
			coordsys[ ( axis + 2 ) % 3 ] = coordsys[ axis % 3 ] ^ MVector( pwm[ ( axis + 1 ) % 3 ] );
		}

		m.matrix[ 0 ][ 0 ] = coordsys[ 0 ].x;
		m.matrix[ 0 ][ 1 ] = coordsys[ 0 ].y;
		m.matrix[ 0 ][ 2 ] = coordsys[ 0 ].z;
		m.matrix[ 1 ][ 0 ] = coordsys[ 1 ].x;
		m.matrix[ 1 ][ 1 ] = coordsys[ 1 ].y;
		m.matrix[ 1 ][ 2 ] = coordsys[ 1 ].z;
		m.matrix[ 2 ][ 0 ] = coordsys[ 2 ].x;
		m.matrix[ 2 ][ 1 ] = coordsys[ 2 ].y;
		m.matrix[ 2 ][ 2 ] = coordsys[ 2 ].z;

		m = m * pwm;
	}

	mDataBlock.outputValue( m_oaAxis ).set( static_cast< short >( axis % 3 ) );

	mDataBlock.outputValue( m_oaWorldMatrix ).set( m );

	const MTransformationMatrix tm( m );

	mDataBlock.outputValue( m_oaTranslate ).set( tm.getTranslation( MSpace::kTransform ) );
	mDataBlock.outputValue( m_oaRotate ).set( tm.eulerRotation().asVector() );

	double s[ 3 ];

	tm.getScale( s, MSpace::kTransform );
	mDataBlock.outputValue( m_oaScale ).set( s[ 0 ], s[ 1 ], s[ 2 ] );

	tm.getShear( s, MSpace::kTransform );
	mDataBlock.outputValue( m_oaShear ).set( s[ 0 ], s[ 1 ], s[ 2 ] );

	return MS::kSuccess;
}