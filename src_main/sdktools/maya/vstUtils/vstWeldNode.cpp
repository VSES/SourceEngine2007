//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: An example Maya locator node (definition)
//
//=============================================================================


// Maya includes
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>


// Valve includes
#include "valveMaya.h"
#include "VsMayaMPxFactory.h"
#include "valveMaya/Undo.h"


//=============================================================================
// CVstWeldNode
//=============================================================================
class CVstWeldNode : public CVsMayaMPxNode
{
public:
	CVstWeldNode();

	virtual ~CVstWeldNode();

	static MStatus Initialize();

	MStatus compute(
		const MPlug &mPlug,
		MDataBlock &mDataBlock );

public:
	static MObject m_iaWorldGeometry;

	static MObject m_iaWeldInput;

	static MObject m_iaType;
	static MObject m_iaInt;
	static MObject m_iaFloat0;
	static MObject m_iaFloat1;
	static MObject m_iaOffsetMatrix;
	static MObject m_iaInverseParentSpace;
	static MObject m_iaRotateOrder;

	static MObject m_oaWeldOutput;

	static MObject m_oaTranslate;
	static MObject m_oaTranslateX;
	static MObject m_oaTranslateY;
	static MObject m_oaTranslateZ;

	static MObject m_oaRotate;
	static MObject m_oaRotateX;
	static MObject m_oaRotateY;
	static MObject m_oaRotateZ;

	enum EWeldType
	{
		kMeshFace = 0
	};
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CVstWeldNode::m_iaWorldGeometry;

MObject CVstWeldNode::m_iaWeldInput;

MObject CVstWeldNode::m_iaType;
MObject CVstWeldNode::m_iaInt;
MObject CVstWeldNode::m_iaFloat0;
MObject CVstWeldNode::m_iaFloat1;
MObject CVstWeldNode::m_iaOffsetMatrix;
MObject CVstWeldNode::m_iaInverseParentSpace;
MObject CVstWeldNode::m_iaRotateOrder;

MObject CVstWeldNode::m_oaWeldOutput;

MObject CVstWeldNode::m_oaTranslate;
MObject CVstWeldNode::m_oaTranslateX;
MObject CVstWeldNode::m_oaTranslateY;
MObject CVstWeldNode::m_oaTranslateZ;

MObject CVstWeldNode::m_oaRotate;
MObject CVstWeldNode::m_oaRotateX;
MObject CVstWeldNode::m_oaRotateY;
MObject CVstWeldNode::m_oaRotateZ;


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXNODE(
	CVstWeldNode,
	vstWeld,
	0x0010e4cb,
	CVstWeldNode::Initialize,
	"Constrains a transform to the world space of specific face of a mesh.\n"
	"Simply select any number of transforms to weld and then select the mesh\n"
	"as the last thing selected or specified and then run the command"
);


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVstWeldNode::CVstWeldNode()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CVstWeldNode::~CVstWeldNode()
{
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstWeldNode::Initialize()
{
	MFnCompoundAttribute cFn;
	MFnEnumAttribute eFn;
	MFnGenericAttribute gFn;
	MFnMatrixAttribute mFn;
	MFnNumericAttribute nFn;
	MFnTypedAttribute tFn;
	MFnUnitAttribute uFn;

	m_iaWorldGeometry = gFn.create( "worldGeometry", "wgeo" );
	gFn.setHidden( true );
	gFn.setStorable( false );
	gFn.addAccept( MFnData::kMesh );
	gFn.addAccept( MFnData::kNurbsCurve );
	gFn.addAccept( MFnData::kNurbsSurface );

	m_iaType = eFn.create( "type", "typ", kMeshFace );
	eFn.addField( "meshFace", kMeshFace );

	m_iaInt = nFn.create( "int", "i", MFnNumericData::kInt );

	m_iaFloat0 = nFn.create( "float0", "f0", MFnNumericData::kDouble );

	m_iaFloat1 = nFn.create( "float1", "f1", MFnNumericData::kDouble );

    m_iaOffsetMatrix = mFn.create( "offsetMatrix", "omat" );

    m_iaInverseParentSpace = mFn.create( "inverseParentSpace", "ips" );

	m_iaRotateOrder = eFn.create( "rotateOrder", "ro", 0 );
	eFn.addField( "xyz", 0 );
	eFn.addField( "yzx", 1 );
	eFn.addField( "zxy", 2 );
	eFn.addField( "xzy", 3 );
	eFn.addField( "yxz", 4 );
	eFn.addField( "zyx", 5 );

	m_iaWeldInput = cFn.create( "weldInput", "wi" );
	cFn.setArray( true );
	cFn.addChild( m_iaType );
	cFn.addChild( m_iaInt );
	cFn.addChild( m_iaFloat0 );
	cFn.addChild( m_iaFloat1 );
	cFn.addChild( m_iaOffsetMatrix );
	cFn.addChild( m_iaInverseParentSpace );
	cFn.addChild( m_iaRotateOrder );

	m_oaTranslateX = uFn.create( "translateX", "tx", MFnUnitAttribute::kDistance );
	m_oaTranslateY = uFn.create( "translateY", "ty", MFnUnitAttribute::kDistance );
	m_oaTranslateZ = uFn.create( "translateZ", "tz", MFnUnitAttribute::kDistance );
	m_oaTranslate = nFn.create( "translate", "t", m_oaTranslateX, m_oaTranslateY, m_oaTranslateZ );

	m_oaRotateX = uFn.create( "rotateX", "rx", MFnUnitAttribute::kAngle );
	m_oaRotateY = uFn.create( "rotateY", "ry", MFnUnitAttribute::kAngle );
	m_oaRotateZ = uFn.create( "rotateZ", "rz", MFnUnitAttribute::kAngle );
	m_oaRotate = nFn.create( "rotate", "r", m_oaRotateX, m_oaRotateY, m_oaRotateZ );

	m_oaWeldOutput = cFn.create( "weldOutput", "wo" );
	cFn.setArray( true );
	cFn.addChild( m_oaTranslate );
	cFn.addChild( m_oaRotate );
	cFn.setUsesArrayDataBuilder( true );

	addAttribute( m_iaWorldGeometry );
	addAttribute( m_iaWeldInput );
	addAttribute( m_oaWeldOutput );

	attributeAffects( m_iaWorldGeometry, m_oaWeldOutput );
	attributeAffects( m_iaWeldInput, m_oaWeldOutput );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void GetMeshMatrix( MItMeshPolygon &pIt, int nMeshFaceIndex, MMatrix &geoMatrix  )
{
	if ( nMeshFaceIndex < static_cast< int >( pIt.count() ) )
	{
		MPointArray facePoints;

		int nOldIndex;
		pIt.setIndex( nMeshFaceIndex, nOldIndex );

		pIt.getPoints( facePoints, MSpace::kWorld );

		// Calculate cheezy coordinate system, if any edges are 0 length then all hell breaks loose
		MVector x = ( facePoints[ 2 ] - facePoints[ 0 ] ).normal();
		MVector z = ( facePoints[ 1 ] - facePoints[ 0 ] ).normal();
		MVector y = ( z ^ x ).normal();
		z = x ^ y;

		memcpy( const_cast< double * >( geoMatrix[ 0 ] ), &x[ 0 ], 3 * sizeof( double ) );
		memcpy( const_cast< double * >( geoMatrix[ 1 ] ), &y[ 0 ], 3 * sizeof( double ) );
		memcpy( const_cast< double * >( geoMatrix[ 2 ] ), &z[ 0 ], 3 * sizeof( double ) );

		MPoint center = pIt.center( MSpace::kWorld );

		memcpy( const_cast< double * >( geoMatrix[ 3] ), &center[ 0 ], 3 * sizeof( double ) );

		pIt.setIndex( nOldIndex, nMeshFaceIndex );
	}
	else
	{
		merr << "Mesh face index " << nMeshFaceIndex << " out of range, only " << pIt.count() << " faces on mesh" << std::endl;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ConnectIntWeld( vm::CUndo &undo, const MObject &weldNodeObj, const MDagPath &toWeldPath, int nType, int nIntValue, const MMatrix &offsetMatrix )
{
	MFnDependencyNode weldFn( weldNodeObj );
	MFnDagNode toWeldFn( toWeldPath );

	MPlug wiAP = weldFn.findPlug( CVstWeldNode::m_iaWeldInput );
	const uint nWeldIndex = vm::NextAvailable( wiAP );
	MPlug wiP = wiAP.elementByLogicalIndex( nWeldIndex );

	undo.SetAttr( wiP.child( CVstWeldNode::m_iaType ), nType );
	undo.SetAttr( wiP.child( CVstWeldNode::m_iaInt ), nIntValue );

	MFnMatrixData dFn;
	MObject mdObj = dFn.create( offsetMatrix );
	undo.SetAttr( wiP.child( CVstWeldNode::m_iaOffsetMatrix ), mdObj );

	undo.Connect( toWeldFn.findPlug( "parentInverseMatrix" ).elementByLogicalIndex( toWeldPath.instanceNumber() ), wiP.child( CVstWeldNode::m_iaInverseParentSpace ), true );
	undo.Connect( toWeldFn.findPlug( "rotateOrder" ), wiP.child( CVstWeldNode::m_iaRotateOrder ), true );

	MPlug woAP = weldFn.findPlug( CVstWeldNode::m_oaWeldOutput );
	MPlug woP = woAP.elementByLogicalIndex( nWeldIndex );
	MPlug tP = woP.child( CVstWeldNode::m_oaTranslate );

	undo.Connect( tP.child( CVstWeldNode::m_oaTranslateX ), toWeldFn.findPlug( "translateX" ) );
	undo.Connect( tP.child( CVstWeldNode::m_oaTranslateY ), toWeldFn.findPlug( "translateY" ) );
	undo.Connect( tP.child( CVstWeldNode::m_oaTranslateZ ), toWeldFn.findPlug( "translateZ" ) );

	MPlug rP = woP.child( CVstWeldNode::m_oaRotate );

	undo.Connect( rP.child( CVstWeldNode::m_oaRotateX ), toWeldFn.findPlug( "rotateX" ) );
	undo.Connect( rP.child( CVstWeldNode::m_oaRotateY ), toWeldFn.findPlug( "rotateY" ) );
	undo.Connect( rP.child( CVstWeldNode::m_oaRotateZ ), toWeldFn.findPlug( "rotateZ" ) );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CVstWeldNode::compute(
	const MPlug &mPlug,
	MDataBlock &mDataBlock )
{
	if ( mPlug == m_oaWeldOutput || mPlug == m_oaTranslate || mPlug == m_oaRotate ||
			mPlug == m_oaTranslateX || mPlug == m_oaTranslateY || mPlug == m_oaTranslateZ ||
			mPlug == m_oaRotateX || mPlug == m_oaRotateY || mPlug == m_oaRotateZ )
	{
		const MObject geoObj = mDataBlock.inputValue( m_iaWorldGeometry ).data();
		if ( geoObj.apiType() == MFn::kMeshData )
		{
			MStatus mStatus;

			MObject meshObj = mDataBlock.inputValue( m_iaWorldGeometry ).asMeshTransformed();
			MFnMesh meshFn( meshObj );
			MItMeshPolygon pIt( meshObj );
			MPointArray facePoints;

			MArrayDataHandle wiAH = mDataBlock.inputArrayValue( m_iaWeldInput );
			MArrayDataHandle woAH = mDataBlock.outputArrayValue( m_oaWeldOutput, &mStatus );
			MArrayDataBuilder woADB = woAH.builder( &mStatus );

			const int nWeldCount = wiAH.elementCount();
			for ( int i = 0; i < nWeldCount; ++i, wiAH.next() )
			{
				MDataHandle wiDH = wiAH.inputValue();

				const MMatrix &offsetMatrix = wiDH.child( m_iaOffsetMatrix ).asMatrix();
				const MMatrix &inverseParentSpace = wiDH.child( m_iaInverseParentSpace ).asMatrix();
				const MEulerRotation::RotationOrder rotationOrder = static_cast< MEulerRotation::RotationOrder >( wiDH.child( m_iaRotateOrder ).asShort() );
				MMatrix geoMatrix;

				switch ( wiDH.child( m_iaType ).asShort() )
				{
				case kMeshFace:
					{
						const int nMeshFaceIndex = wiDH.child( m_iaInt ).asInt();
						GetMeshMatrix( pIt, nMeshFaceIndex, geoMatrix );
					}
					break;
				default:
					merr << "Unknown Weld Type " << wiDH.child( m_iaType ).asShort() << std::endl;
					break;
				}

				const int nWeldIndex = wiAH.elementIndex();
				MDataHandle woDH = woADB.addElement( nWeldIndex );

				MTransformationMatrix L( inverseParentSpace * offsetMatrix * geoMatrix );

				woDH.child( m_oaTranslate ).set( L.getTranslation( MSpace::kWorld ) );
				MEulerRotation e = L.rotation().asEulerRotation();
				e.reorder( rotationOrder );
				woDH.child( m_oaRotate ).set( e.asVector() );
			}
		}
		else
		{
			merr << "Invalid .inputGeometry data of type: " << geoObj.apiTypeStr() << " found while computing " << mPlug.info() << std::endl;
			return MS::kFailure;
		}

		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}