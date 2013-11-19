//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================

// Standard includes
#include <math.h>

// Maya includes
#include <maya/MEulerRotation.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPoint.h>
#include <maya/MQuaternion.h>
#include <maya/MVector.h>

#include <maya/MGlobal.h>

// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"

#include "mathlib/mathlib.h"

// Local includes

class CVstAimNode : public CVsMayaMPxNode
{
public:
	CVstAimNode();

	virtual ~CVstAimNode();

	static MStatus Initialize();

	MStatus compute(
		const MPlug &mPlug,
		MDataBlock &mDataBlock );

public:
	static MObject m_iaRotateOrder;

	static MObject m_iaAim;
	static MObject m_iaUp;

	static MObject m_iaParentSpace;
	static MObject m_iaAimSpace;
	static MObject m_iaParentSpaceTranslation;

	static MObject m_oaLocalRotation;
	static MObject m_oaLocalRotationX;
	static MObject m_oaLocalRotationY;
	static MObject m_oaLocalRotationZ;

};


INSTALL_MAYA_MPXNODE(
	CVstAimNode,
	vstAim,
	0x0010e4c4,
	CVstAimNode::Initialize,
	"An aim constraint node which orients up based on what the user specified up vector is doing in the parent of the node being aimed.  "
	"This node is implemented in the Source engine as an <aimconstraint> helper bone."
);

// Statics

MObject CVstAimNode::m_iaRotateOrder;

MObject CVstAimNode::m_iaAim;
MObject CVstAimNode::m_iaUp;

MObject CVstAimNode::m_iaParentSpace;
MObject CVstAimNode::m_iaAimSpace;
MObject CVstAimNode::m_iaParentSpaceTranslation;

MObject CVstAimNode::m_oaLocalRotation;
MObject CVstAimNode::m_oaLocalRotationX;
MObject CVstAimNode::m_oaLocalRotationY;
MObject CVstAimNode::m_oaLocalRotationZ;

//-----------------------------------------------------------------------------
//
// Purpose: Constructor
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

CVstAimNode::CVstAimNode()
{
}


//-----------------------------------------------------------------------------
//
// Purpose: Destructor
// Input  :
// Output :
//
//-----------------------------------------------------------------------------

CVstAimNode::~CVstAimNode()
{
}


//-----------------------------------------------------------------------------
//
// Purpose: Initialize the node, add static attributes, etc...
// Input  :
// Output : MStatus::kSuccess if everything was ok
//
//-----------------------------------------------------------------------------

MStatus CVstAimNode::Initialize()
{
	MFnEnumAttribute eFn;
	MFnMatrixAttribute mFn;
	MFnNumericAttribute nFn;
	MFnTypedAttribute tFn;
	MFnUnitAttribute uFn;

	m_iaRotateOrder = eFn.create( "rotateOrder", "ro", 0 );
	eFn.addField( "xyz", 0 );
	eFn.addField( "yzx", 1 );
	eFn.addField( "zxy", 2 );
	eFn.addField( "xzy", 3 );
	eFn.addField( "yxz", 4 );
	eFn.addField( "zyx", 5 );
	eFn.setHidden( true );

	// NOTE: The default value specified here doesn't work, it ends up being 0
    m_iaAim = nFn.create( "aim", "a",
      nFn.create( "aimX", "ax", MFnNumericData::kDouble,  0.0 ),
      nFn.create( "aimY", "ay", MFnNumericData::kDouble, -1.0 ),
      nFn.create( "aimZ", "az", MFnNumericData::kDouble,  0.0 ) );

	// Have to actually set them like this
	nFn.setObject( nFn.child( 1 ) );
	nFn.setDefault( -1.0 );

	// Ditto
    m_iaUp = nFn.create( "up", "u",
      nFn.create( "upX", "ux", MFnNumericData::kDouble,  0.0 ),
      nFn.create( "upY", "uy", MFnNumericData::kDouble,  1.0 ),
      nFn.create( "upZ", "uz", MFnNumericData::kDouble,  0.0 ) );

	// Ditto
	nFn.setObject( nFn.child( 1 ) );
	nFn.setDefault( 1.0 );

    m_iaParentSpace = mFn.create( "parentSpace", "psp" );
	mFn.setHidden( true );

    m_iaAimSpace = mFn.create( "aimSpace", "asp" );
	mFn.setHidden( true );

    m_iaParentSpaceTranslation = nFn.create( "translate", "t",
      uFn.create( "translateX", "tx", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "translateY", "ty", MFnUnitAttribute::kDistance, 0.0 ),
      uFn.create( "translateZ", "tz", MFnUnitAttribute::kDistance, 0.0 ) );
	nFn.setHidden( true );

    m_oaLocalRotation = nFn.create( "rotate", "r",
      uFn.create( "rotateX", "rx", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "rotateY", "ry", MFnUnitAttribute::kAngle, 0.0 ),
      uFn.create( "rotateZ", "rz", MFnUnitAttribute::kAngle, 0.0 ) );
	nFn.setWritable( false );
	nFn.setHidden( true );

	m_oaLocalRotationX = nFn.child( 0 );
	m_oaLocalRotationY = nFn.child( 1 );
	m_oaLocalRotationZ = nFn.child( 2 );

	addAttribute( m_iaRotateOrder );

	addAttribute( m_iaAim );
	addAttribute( m_iaUp );

	addAttribute( m_iaParentSpace );
	addAttribute( m_iaAimSpace );
	addAttribute( m_iaParentSpaceTranslation );

	addAttribute( m_oaLocalRotation );

	attributeAffects( m_iaRotateOrder, m_oaLocalRotation );

	attributeAffects( m_iaAim, m_oaLocalRotation );
	attributeAffects( m_iaUp, m_oaLocalRotation );

	attributeAffects( m_iaParentSpace, m_oaLocalRotation );
	attributeAffects( m_iaAimSpace, m_oaLocalRotation );
	attributeAffects( m_iaParentSpaceTranslation, m_oaLocalRotation );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
// Purpose: Initialize the node, add static attributes, etc...
// Input  :
// Output : MStatus::kSuccess if everything was ok
//
//-----------------------------------------------------------------------------

MStatus CVstAimNode::compute(
	const MPlug &mPlug,
	MDataBlock &mDataBlock )
{
	if ( mPlug == m_oaLocalRotation ||
			mPlug == m_oaLocalRotationX ||
			mPlug == m_oaLocalRotationY ||
			mPlug == m_oaLocalRotationZ )
	{
		// Get data from Maya
		const MVector m_userAimVector( reinterpret_cast<MVector &>(
			mDataBlock.inputValue( m_iaAim ).asDouble3() ).normal() );

		const MVector m_userUpVector( reinterpret_cast<MVector &>(
			mDataBlock.inputValue( m_iaUp ).asDouble3() ).normal() );

		// Get the parent space matrix
		const MMatrix &m_parentSpace( mDataBlock.inputValue( m_iaParentSpace ).asMatrix() );

		// Get the local position
		const MPoint m_aimLocalPosition( mDataBlock.inputValue( m_iaParentSpaceTranslation ).asVector() );

		// Get the aim space matrix
		const MMatrix &m_aimAtSpace( mDataBlock.inputValue( m_iaAimSpace ).asMatrix() );

		{
			// Valve Version
			// The rotation of the bone to aim it properly
			Quaternion boneRotation;

			// Guaranteed to be unit length
			const Vector userAimVector( m_userAimVector.x, m_userAimVector.y, m_userAimVector.z );

			// Guaranteed to be unit length
			const Vector userUpVector( m_userUpVector.x, m_userUpVector.y, m_userUpVector.z );

			// The worldspace matrix of the parent of the bone
			const matrix3x4_t parentSpace(
				m_parentSpace[0][0], m_parentSpace[1][0], m_parentSpace[2][0], m_parentSpace[3][0],
				m_parentSpace[0][1], m_parentSpace[1][1], m_parentSpace[2][1], m_parentSpace[3][1],
				m_parentSpace[0][2], m_parentSpace[1][2], m_parentSpace[2][2], m_parentSpace[3][2] );

			const Vector aimLocalPosition( m_aimLocalPosition.x, m_aimLocalPosition.y, m_aimLocalPosition.z );
			Vector aimWorldPosition;
			VectorTransform( aimLocalPosition, parentSpace, aimWorldPosition );

			// The worldspace matrix of the bone to aim at
			const matrix3x4_t aimAtSpace(
				m_aimAtSpace[0][0], m_aimAtSpace[1][0], m_aimAtSpace[2][0], m_aimAtSpace[3][0],
				m_aimAtSpace[0][1], m_aimAtSpace[1][1], m_aimAtSpace[2][1], m_aimAtSpace[3][1],
				m_aimAtSpace[0][2], m_aimAtSpace[1][2], m_aimAtSpace[2][2], m_aimAtSpace[3][2] );

			Vector aimAtWorldPosition;
			MatrixGetColumn( aimAtSpace, 3, aimAtWorldPosition );

			Vector aimVector;
			VectorSubtract( aimAtWorldPosition, aimWorldPosition, aimVector );
			VectorNormalizeFast( aimVector );

			Vector axis;
			CrossProduct( userAimVector, aimVector, axis );
			VectorNormalizeFast( axis );
			float angle( acosf( DotProduct( userAimVector, aimVector ) ) );
			Quaternion aimRotation;
			AxisAngleQuaternion( axis, RAD2DEG( angle ), aimRotation );

			if ( ( 1.0f - fabs( DotProduct( userUpVector, userAimVector ) ) ) > FLT_EPSILON )
			{
				matrix3x4_t aimRotationMatrix;
				QuaternionMatrix( aimRotation, aimRotationMatrix );

				Vector tmpV;

				Vector tmp_pUp;
				VectorRotate( userUpVector, aimRotationMatrix, tmp_pUp );
				VectorScale( aimVector, DotProduct( aimVector, tmp_pUp ), tmpV );
				Vector pUp;
				VectorSubtract( tmp_pUp, tmpV, pUp );
				VectorNormalizeFast( pUp );

				Vector tmp_pParentUp;
				VectorRotate( userUpVector, parentSpace, tmp_pParentUp );
				VectorScale( aimVector, DotProduct( aimVector, tmp_pParentUp ), tmpV );
				Vector pParentUp;
				VectorSubtract( tmp_pParentUp, tmpV, pParentUp );
				VectorNormalizeFast( pParentUp );

				angle = acos( DotProduct( pUp, pParentUp ) );
				CrossProduct( pUp, pParentUp, axis );
				VectorNormalizeFast( axis );
				Quaternion upRotation;
				AxisAngleQuaternion( axis, RAD2DEG( angle ), upRotation );

				QuaternionMult( upRotation, aimRotation, boneRotation );
			}
			else
			{
				boneRotation = aimRotation;
			}

			// The world matrix of the bone to change
			matrix3x4_t boneMatrix;
			QuaternionMatrix( boneRotation, aimWorldPosition, boneMatrix );

			MMatrix bm;
			MQuaternion bq( boneRotation.x, boneRotation.y, boneRotation.z, boneRotation.w );
			bm = bq;
			bm = bm * m_parentSpace.inverse();
			bq = bm;

			// Convert it back to Maya

			MEulerRotation m_aimEuler;
			m_aimEuler = bq;

			// See if the rotation order is not XYZ
			const MEulerRotation::RotationOrder m_rotationOrder( static_cast< MEulerRotation::RotationOrder >(
				mDataBlock.inputValue( m_iaRotateOrder ).asShort() ) );

			if ( m_aimEuler.order != m_rotationOrder )
			{
				m_aimEuler.reorderIt( m_rotationOrder );
			}

			// Set the value in Maya
			mDataBlock.outputValue( m_oaLocalRotation ).set( m_aimEuler.x, m_aimEuler.y, m_aimEuler.z );
		}

		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}