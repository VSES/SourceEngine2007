//======= Copyright © 1996-2006, Valve Corporation, All rights reserved. ======
//
// Purpose: Implementation of CSmdImport
//
//=============================================================================

#if _MSC_VER >= 1300
#pragma warning(disable : 4511)	// warning C4511: copy constructor could not be generated
#pragma warning(disable : 4512)	// warning C4512: assignment operator could not be generated
#pragma warning(disable : 4127)	// warning C4127: conditional expression is constant
#pragma warning(disable : 4701)	// warning C4701: local variable 'result' may be used without having been initialized
#pragma warning(disable : 4702)	// warning C4702: unreachable code
#pragma warning(disable : 4996) // warning C4966: 'foo' was declared deprecated
#endif //_MSV_VER >= 1300


// Standard includes
#include <io.h>
#include <fstream>


// Boost
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>


// Maya includes
#include <maya/MAnimControl.h>
#include <maya/MEulerRotation.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFn.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MFnComponentListData.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnSet.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnTransform.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MProgressWindow.h>
#include <maya/MQuaternion.h>
#include <maya/MSelectionList.h>
#include <maya/MTimeArray.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MVector.h>


// Valve includes
#include "valveMaya.h"


// Local includes
#include "SmdImport.h"


//-----------------------------------------------------------------------------
// Copy Data from STL container to a Maya container
//-----------------------------------------------------------------------------
void Copy(
	const std::deque< MFloatVector > &src,
	MFloatPointArray &dst )
{
	dst.setLength( src.size() );
	const std::deque< MFloatVector >::const_iterator sEnd( src.end() );
	std::deque< MFloatVector >::const_iterator si( src.begin() );
	for ( uint di( 0U ); si != sEnd; ++si, ++di )
	{
		dst[ di ] = MFloatPoint( si->x, si->y, si->z );
	}
}


//-----------------------------------------------------------------------------
// Copy Data from STL container to a Maya container
//-----------------------------------------------------------------------------
template < class T_t, class U_t >
void Copy(
	const T_t &src,
	U_t &dst )
{
	dst.setLength( src.size() );
	const T_t::const_iterator sEnd( src.end() );
	T_t::const_iterator si( src.begin() );
	for ( uint di( 0U ); si != sEnd; ++si, ++di )
	{
		dst[ di ] = *si;
	}
}


//-----------------------------------------------------------------------------
// Copy Data from STL container to a Maya container
//-----------------------------------------------------------------------------
template < class T_t >
void Copy(
	const T_t &src,
	MFloatArray &u,
	MFloatArray &v )
{
	u.setLength( src.size() );
	v.setLength( src.size() );
	const T_t::const_iterator sEnd( src.end() );
	T_t::const_iterator si( src.begin() );
	for ( uint di( 0U ); si != sEnd; ++si, ++di )
	{
		u[ di ] = si->u;
		v[ di ] = si->v;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MPlug GetNextDefaultRenderUtilitiesPlug()
{
	MItDependencyNodes dIt( MFn::kRenderUtilityList );
	if ( dIt.isDone() )
		return MPlug();

	MPlug uAP( MFnDependencyNode( dIt.thisNode() ).findPlug( "utilities" ) );
	if ( uAP.isNull() )
		return MPlug();

	// This should return the -nextAvailable index
	// Assuming logical elements are sorted...
	return uAP.elementByLogicalIndex( uAP.elementByPhysicalIndex( uAP.evaluateNumElements() - 1 ).logicalIndex() + 1 );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MPlug GetNextDefaultShaderPlug()
{
	MItDependencyNodes dIt( MFn::kShaderList );
	if ( dIt.isDone() )
		return MPlug();

	MPlug uAP( MFnDependencyNode( dIt.thisNode() ).findPlug( "shaders" ) );
	if ( uAP.isNull() )
		return MPlug();

	// This should return the -nextAvailable index
	// Assuming logical elements are sorted...
	return uAP.elementByLogicalIndex( uAP.elementByPhysicalIndex( uAP.evaluateNumElements() - 1 ).logicalIndex() + 1 );
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MPlug GetNextDefaultTexturePlug()
{
	MItDependencyNodes dIt( MFn::kTextureList );
	if ( dIt.isDone() )
		return MPlug();

	MPlug uAP( MFnDependencyNode( dIt.thisNode() ).findPlug( "textures" ) );
	if ( uAP.isNull() )
		return MPlug();

	// This should return the -nextAvailable index
	// Assuming logical elements are sorted...
	return uAP.elementByLogicalIndex( uAP.elementByPhysicalIndex( uAP.evaluateNumElements() - 1 ).logicalIndex() + 1 );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CreateFileNode(
	const std::string &textureName,
	MDagModifier &mDagModifier )
{
	MObject resultObj = mDagModifier.MDGModifier::createNode( "file" );
	mDagModifier.doIt();

	if ( resultObj.isNull() )
		return resultObj;

	MFnDependencyNode resultFn( resultObj );
	resultFn.findPlug( "fileTextureName" ).setValue( textureName.c_str() );

	const MPlug srcP( resultFn.findPlug( "message" ) );
	const MPlug dstP( GetNextDefaultTexturePlug() );

	if ( !srcP.isNull() && !dstP.isNull() )
	{
		mDagModifier.connect( srcP, dstP );
		mDagModifier.doIt();
	}

	return resultObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdImport::PurgeUnusedBones( 
	CUtlVector< CNodeData > &nodeList,
	MSelectionList &rootNodeList,
	MDagModifier &mDagModifier )
{
	bool removed( true );

	while ( removed )
	{
		removed = false;
		const int nCount( nodeList.Count() );
		for ( int i( 0 ); i < nCount; ++i )
		{
			CNodeData &nodeData( nodeList[ i ] );
			if ( nodeData.Valid() && nodeData.m_dagPath.childCount() == 0 && !nodeData.m_skinned )
			{
				Assert( nodeData.m_dagPath.childCount() == 0 );

				MDagPath tmpDagPath;
				for ( uint j( 0 ); j != rootNodeList.length(); ++j )
				{
					if ( rootNodeList.getDagPath( j, tmpDagPath ) && tmpDagPath == nodeData.m_dagPath )
					{
						rootNodeList.remove( j );
						break;
					}
				}

				mDagModifier.deleteNode( nodeData.m_dagPath.node() );
				nodeData.Reset();
				removed = true;
			}
		}

		if ( removed )
		{
			mDagModifier.doIt();
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject GetConnectedNode(
	const MPlug &mPlug )
{
	MPlugArray mPlugArray;
	if ( mPlug.connectedTo( mPlugArray, true, true ) && mPlugArray.length() )
	{
		return mPlugArray[ 0 ].node();
	}

	return MObject::kNullObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CSmdImport::SkinMesh(
	const MDagPath &origMeshDagPath,
	ValveMaya::CUndo &undo )
{
	// Duplicate the mesh and parent it under the original transform
	// rename it with the suffix "Orig" and mark as .intermediateObject

	const MObject origMeshObj( origMeshDagPath.node() );
	MFnMesh origMeshFn( origMeshDagPath );
	const MString meshName( origMeshFn.name() );

	const MObject newTransformObj( origMeshFn.duplicate() );
	const MObject newMeshObj( MFnDagNode( newTransformObj ).child( 0 ) );
	MFnDagNode newMeshFn( newMeshObj );
	newMeshFn.setIntermediateObject( true );

	MDagModifier &dagMod( undo.DagModifier() );

	dagMod.reparentNode( newMeshObj, origMeshFn.parent( origMeshDagPath.instanceNumber() ) );
	dagMod.doIt();

	dagMod.deleteNode( newTransformObj );
	dagMod.doIt();
	dagMod.renameNode( newMeshObj, meshName + "Orig" );
	dagMod.doIt();

	// Lock the mesh transform thingies
	const MFnDependencyNode origTransformFn( origMeshFn.parent( origMeshDagPath.instanceNumber() ) );

	// Create the 'skinCluster' node
	const MObject skinObj( dagMod.MDGModifier::createNode( "skinCluster" ) );
	dagMod.doIt();
	const MFnDependencyNode skinFn( skinObj );
	const MString skinName( skinFn.name() );

	MObject om_iogObj( origMeshFn.findPlug( "instObjGroups" ).attribute() );
	MObject om_ogObj( origMeshFn.findPlug( "objectGroups" ).attribute() );
	MPlug om_ogAP( origMeshFn.findPlug( "objectGroups" ) );
	MPlug om_ogP( origMeshFn.findPlug( "objectGroups" ) );
	MPlug om_ogclP( origMeshFn.findPlug( "objectGrpCompList" ) );
	MObject om_ogclObj( om_ogclP.attribute() );
	MPlug om_ogiP( origMeshFn.findPlug( "objectGroupId" ) );
	MObject om_ogiObj( om_ogiP.attribute() );
	MPlug om_ogcP( newMeshFn.findPlug( "objectGrpColor" ) );

	om_ogAP.selectAncestorLogicalIndex( 0, om_iogObj );
	om_ogclP.selectAncestorLogicalIndex( 0, om_iogObj );
	om_ogiP.selectAncestorLogicalIndex( 0, om_iogObj );

	MIntArray ogis;
	om_ogAP.getExistingArrayAttributeIndices( ogis );

	MPlug worldMeshP( newMeshFn.findPlug( "worldMesh" ).elementByLogicalIndex( origMeshDagPath.instanceNumber() ) );

	for ( uint i( 0 ); i != ogis.length(); ++i )
	{
		MPlug om_ogP( om_ogAP.elementByPhysicalIndex( i ) );

		om_ogiP = om_ogP.child( om_ogiObj );
		om_ogclP = om_ogP.child( om_ogclObj );

		// Get 'groupId' node
		const MObject groupIdObj( GetConnectedNode( om_ogiP ) );
		if ( groupIdObj.isNull() )
			continue;
		const MFnDependencyNode groupIdFn( groupIdObj );

		// Create 'groupParts' node
		const MObject groupPartsObj( dagMod.MDGModifier::createNode( "groupParts" ) );
		dagMod.doIt();

		const MFnDependencyNode groupPartsFn( groupPartsObj );
		groupPartsFn.findPlug( "isHistoricallyInteresting" ).setValue( false );

		MObject cldObj;
		om_ogclP.getValue( cldObj );
		groupPartsFn.findPlug( "inputComponents" ).setValue( cldObj );

		// Connect groupId.groupId -> groupParts.groupId
		undo.Connect(
			groupIdFn.findPlug( "groupId" ),
			groupPartsFn.findPlug( "groupId" ) );

		// Connect worldMeshP -> groupParts.inputGeometry
		undo.Connect( worldMeshP, groupPartsFn.findPlug( "inputGeometry" ) );

		worldMeshP = groupPartsFn.findPlug( "outputGeometry" );
	}

	// Create tweak stuff

	{
		// Create tweak 'groupId' node
		const MObject groupIdObj( dagMod.MDGModifier::createNode( "groupId" ) );
		dagMod.doIt();
		dagMod.renameNode( groupIdObj, "tweakGroupId" );
		dagMod.doIt();
		const MFnDependencyNode groupIdFn( groupIdObj );
		groupIdFn.findPlug( "isHistoricallyInteresting" ).setValue( false );

		const uint objectGroupIndex( ValveMaya::NextAvailable( om_ogAP ) );
		om_ogiP.selectAncestorLogicalIndex( 0, om_iogObj );
		om_ogiP.selectAncestorLogicalIndex( objectGroupIndex, om_ogObj );

		undo.Connect( groupIdFn.findPlug( "groupId" ), om_ogiP );

		// Create 'groupParts' node
		const MObject groupPartsObj( dagMod.MDGModifier::createNode( "groupParts" ) );
		dagMod.doIt();
		dagMod.renameNode( groupPartsObj, "tweakGroupParts" );
		dagMod.doIt();
		const MFnDependencyNode groupPartsFn( groupPartsObj );
		groupPartsFn.findPlug( "isHistoricallyInteresting" ).setValue( false );
		MFnSingleIndexedComponent cFn;
		MObject cObj( cFn.create( MFn::kMeshVertComponent ) );
		cFn.setComplete( true );
		MFnComponentListData cldFn;
		MObject cldObj( cldFn.create() );
		cldFn.add( cObj );
		groupPartsFn.findPlug( "inputComponents" ).setValue( cldObj );

		// Connect groupId.groupId -> groupParts.groupId
		undo.Connect(
			groupIdFn.findPlug( "groupId" ),
			groupPartsFn.findPlug( "groupId" ) );

		// Connect newMesh.worldMesh[ # ] -> groupParts.inputGeometry
		undo.Connect(
			worldMeshP,
			groupPartsFn.findPlug( "inputGeometry" ) );

		worldMeshP = groupPartsFn.findPlug( "outputGeometry" );

		// Create tweak node
		MObject tweakObj( dagMod.MDGModifier::createNode( "tweak" ) );
		const MFnDependencyNode tweakFn( tweakObj );
		MObject tweakInputObj( tweakFn.findPlug( "input" ).attribute() );
		MPlug tigP( tweakFn.findPlug( "inputGeometry" ) );
		tigP.selectAncestorLogicalIndex( 0, tweakInputObj );
		MPlug tgiP( tweakFn.findPlug( "groupId" ) );
		tgiP.selectAncestorLogicalIndex( 0, tweakInputObj );

		undo.Connect(
			worldMeshP,
			tigP );

		undo.Connect(
			groupIdFn.findPlug( "groupId" ),
			tgiP );

		MObject vlistObj( tweakFn.findPlug( "vlist" ) );
		MPlug vertexP( tweakFn.findPlug( "vertex" ) );
		MObject vertexObj( vertexP.attribute() );
		vertexP.selectAncestorLogicalIndex( 0, vlistObj );
		vertexP.selectAncestorLogicalIndex( 0, vertexObj );

		undo.Connect(
			vertexP,
			origMeshFn.findPlug( "tweakLocation" ) );

		worldMeshP = tweakFn.findPlug( "outputGeometry" );
		worldMeshP = worldMeshP.elementByLogicalIndex( 0 );

		// Create 'objectSet' node
		const MObject setObj( dagMod.MDGModifier::createNode( "objectSet" ) );
		dagMod.doIt();
		dagMod.renameNode( setObj, "tweakSet" );
		const MFnDependencyNode setFn( setObj );
		setFn.findPlug( "isHistoricallyInteresting" ).setValue( false );
		setFn.findPlug( "verticesOnlySet" ).setValue( true );

		// Connect mesh.instObjectGroups[ 0 ].objectGroups[ objectGroupIndex ] -> set.dagSetMembers[ 0 ]
		om_ogP.selectAncestorLogicalIndex( 0, om_iogObj );
		om_ogP.selectAncestorLogicalIndex( objectGroupIndex, om_ogObj );
		undo.Connect(
			om_ogP,
			setFn.findPlug( "dagSetMembers" ).elementByLogicalIndex( 0 ) );

		// Connect skinCluster->message set.usedBy[0]
		undo.Connect(
			tweakFn.findPlug( "message" ),
			setFn.findPlug( "usedBy" ).elementByLogicalIndex( 0 ) );

		// Connect set.memberWireframeColor -> mesh.instObjGroups[0].objectGroups[0].objectGrpColor
		om_ogcP.selectAncestorLogicalIndex( 0, om_iogObj );
		om_ogcP.selectAncestorLogicalIndex( objectGroupIndex, om_ogObj );
		undo.Connect(
			setFn.findPlug( "memberWireframeColor" ),
			om_ogcP );

	}

	{
		// Create skin node

		// Create 'groupId' node
		const MObject groupIdObj( dagMod.MDGModifier::createNode( "groupId" ) );
		dagMod.doIt();
		dagMod.renameNode( groupIdObj, skinName + "GroupId" );
		dagMod.doIt();
		const MFnDependencyNode groupIdFn( groupIdObj );
		groupIdFn.findPlug( "isHistoricallyInteresting" ).setValue( false );

		uint objectGroupIndex( ValveMaya::NextAvailable( om_ogAP ) );
		om_ogiP.selectAncestorLogicalIndex( 0, om_iogObj );
		om_ogiP.selectAncestorLogicalIndex( objectGroupIndex, om_ogObj );

		undo.Connect(
			groupIdFn.findPlug( "groupId" ),
			om_ogiP );

		// Create 'groupParts' node
		const MObject groupPartsObj( dagMod.MDGModifier::createNode( "groupParts" ) );
		dagMod.doIt();
		dagMod.renameNode( groupPartsObj, skinName + "GroupParts" );
		dagMod.doIt();
		const MFnDependencyNode groupPartsFn( groupPartsObj );
		groupPartsFn.findPlug( "isHistoricallyInteresting" ).setValue( false );
		MFnSingleIndexedComponent cFn;
		MObject cObj( cFn.create( MFn::kMeshVertComponent ) );
		cFn.setComplete( true );
		MFnComponentListData cldFn;
		MObject cldObj( cldFn.create() );
		cldFn.add( cObj );
		groupPartsFn.findPlug( "inputComponents" ).setValue( cldObj );

		// Create 'objectSet' node
		const MObject setObj( dagMod.MDGModifier::createNode( "objectSet" ) );
		dagMod.doIt();
		dagMod.renameNode( setObj, skinName + "Set" );
		const MFnDependencyNode setFn( setObj );
		setFn.findPlug( "isHistoricallyInteresting" ).setValue( false );
		setFn.findPlug( "verticesOnlySet" ).setValue( true );

		// Connect mesh.instObjectGroups[ 0 ].objectGroups[ objectGroupIndex ] -> set.dagSetMembers[ 0 ]
		om_ogP.selectAncestorLogicalIndex( 0, om_iogObj );
		om_ogP.selectAncestorLogicalIndex( objectGroupIndex, om_ogObj );
		undo.Connect(
			om_ogP,
			setFn.findPlug( "dagSetMembers" ).elementByLogicalIndex( 0 ) );

		// Connect skinCluster->message set.usedBy[0]
		undo.Connect(
			skinFn.findPlug( "message" ),
			setFn.findPlug( "usedBy" ).elementByLogicalIndex( 0 ) );

		MPlug skinP( skinFn.findPlug( "message" ) );
		if ( skinP.isNull() )
			return MObject::kNullObj;

		// Set some values on skin node
		skinFn.findPlug( "geomMatrix" ).setValue( MFnMatrixData().create( origMeshDagPath.inclusiveMatrix() ) );
		skinFn.findPlug( "maintainMaxInfluences" ).setValue( true );
		skinFn.findPlug( "maxInfluences" ).setValue( 3 );

		// Connect newMesh.worldMesh[ # ] -> groupParts.inputGeometry
		undo.Connect(
			worldMeshP,
			groupPartsFn.findPlug( "inputGeometry" ) );

		// Connect groupParts.outputGeometry -> skin.input[ 0 ].inputGeometry
		MObject inputAttrObj( skinFn.findPlug( "input" ) );
		MPlug igP( skinFn.findPlug( "inputGeometry" ) );
		igP.selectAncestorLogicalIndex( 0, inputAttrObj );
		undo.Connect(
			groupPartsFn.findPlug( "outputGeometry" ),
			igP );

		// Connect skin.outputGeometry[ 0 ] -> origMesh.inMesh
		undo.Connect(
			skinFn.findPlug( "outputGeometry" ).elementByLogicalIndex( 0 ),
			origMeshFn.findPlug( "inMesh" ) );

		// Connect groupId.groupId -> groupParts.groupId
		undo.Connect(
			groupIdFn.findPlug( "groupId" ),
			groupPartsFn.findPlug( "groupId" ) );

		// Connect groupId.groupId -> skin.input[ 0 ].groupId
		MPlug giP( skinFn.findPlug( "groupId" ) );
		giP.selectAncestorLogicalIndex( 0, inputAttrObj );
		undo.Connect(
			groupIdFn.findPlug( "groupId" ),
			giP );

		// Connect set.memberWireframeColor -> mesh.instObjGroups[0].objectGroups[0].objectGrpColor
		om_ogcP.selectAncestorLogicalIndex( 0, om_iogObj );
		om_ogcP.selectAncestorLogicalIndex( objectGroupIndex, om_ogObj );
		undo.Connect(
			setFn.findPlug( "memberWireframeColor" ),
			om_ogcP );
	}

	return skinObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdImport::SkinMeshes(
	CUtlVector< CNodeData > &nodeList,
	std::map< int, CPolygonData > &polygonDataMap,
	ValveMaya::CUndo &undo )
{
	const int nNodes( nodeList.Count() );

	const std::map< int, CPolygonData >::const_iterator pEnd( polygonDataMap.end() );
	for ( std::map< int, CPolygonData >::iterator pi( polygonDataMap.begin() ); pi != pEnd; ++pi )
	{
		CPolygonData &polygonData( pi->second );

		const std::set< int > &influenceObjects( polygonData.m_influenceObjects );
		if ( influenceObjects.size() )
		{
			// Create the skin cluster node for the mesh
			const MDagPath &mDagPath( polygonData.m_mDagPath );
			MStringArray melResult;

			const MObject skinObj( SkinMesh( mDagPath, undo ) );

			const MObject dagPoseObj( undo.DagModifier().MDGModifier::createNode( "dagPose" ) );
			undo.DagModifierDoIt();
			undo.DagModifier().renameNode( dagPoseObj, "bindPose" );
			undo.DagModifierDoIt();

			const MFnDependencyNode dagPoseFn( dagPoseObj );
			dagPoseFn.findPlug( "bindPose" ).setValue( true );
			MPlug bpwmAP( dagPoseFn.findPlug( "worldMatrix" ) );
			MPlug bpxmAP( dagPoseFn.findPlug( "xformMatrix" ) );
			MPlug bpmAP( dagPoseFn.findPlug( "members" ) );
			MPlug bppAP( dagPoseFn.findPlug( "parents" ) );
			MPlug bpwP( dagPoseFn.findPlug( "world" ) );

			if ( !skinObj.isNull() )
			{
				const MFnDependencyNode skinFn( skinObj );

				// Connect up all of the influence objects
				MPlug pbmAP( skinFn.findPlug( "bindPreMatrix" ) );
				MPlug mAP( skinFn.findPlug( "matrix" ) );
				MPlug dAP( skinFn.findPlug( "dropoff" ) );
				MPlug lwAP( skinFn.findPlug( "lockWeights" ) );

				int influenceIndex( 0 );
				const std::set< int >::const_iterator sEnd( influenceObjects.end() );
				for ( std::set< int >::const_iterator si( influenceObjects.begin() ); si != sEnd; ++si )
				{
					if ( *si >= nNodes )
					{
						merr << "Can't find node " << *si << " to skin " << mDagPath.partialPathName() << std::endl;
						continue;
					}

					CNodeData &nodeData( nodeList[ *si ] );
					if ( !nodeData.Valid() )
					{
						merr << "Can't find node " << *si << " to skin " << mDagPath.partialPathName() << std::endl;
						continue;
					}

					nodeData.m_skinned = true;
					nodeData.m_influenceIndex = influenceIndex;

					const MDagPath &iDagPath( nodeData.m_dagPath );
					const MFnDagNode iFn( iDagPath );

					iFn.findPlug( "useObjectColor" ).setValue( true );
					iFn.findPlug( "objectColor" ).setValue( influenceIndex % 8 );

					MPlug matP( iFn.findPlug( "matrix" ) );
					MObject matObj;
					matP.getValue( matObj );

					bpxmAP.elementByLogicalIndex( influenceIndex ).setValue( MFnMatrixData().create( MFnMatrixData( matObj ).transformation() ) );
					MPlug wmP( iFn.findPlug( "worldMatrix" ).elementByLogicalIndex( iDagPath.instanceNumber() ) );
					MPlug bpP( iFn.findPlug( "bindPose" ) );
					MPlug mP( iFn.findPlug( "message" ) );

					MObject wmObj;
					wmP.getValue( wmObj );
					bpP.setValue( MFnMatrixData().create( MFnMatrixData( wmObj ).matrix() ) );

					undo.Connect( bpP, bpwmAP.elementByLogicalIndex( influenceIndex ) );
					undo.Connect( mP, bpmAP.elementByLogicalIndex( influenceIndex ) );
					if ( nodeData.m_parentIndex >= 0 )
					{
						const CNodeData &parentNodeData( nodeList[ nodeData.m_parentIndex ] );
						if ( parentNodeData.m_influenceIndex >= 0 )
						{
							undo.Connect( bpmAP.elementByLogicalIndex( nodeList[ nodeData.m_parentIndex ].m_influenceIndex ), bppAP.elementByLogicalIndex( influenceIndex ) );
						}
						else
						{
							merr << "Bad Parent" << std::endl;
							undo.Connect( bpwP, bppAP.elementByLogicalIndex( influenceIndex ) );
						}
					}
					else
					{
						undo.Connect( bpwP, bppAP.elementByLogicalIndex( influenceIndex ) );
					}

					pbmAP.elementByLogicalIndex( influenceIndex ).setValue( MFnMatrixData().create( iDagPath.inclusiveMatrixInverse() ) );
					dAP.elementByLogicalIndex( influenceIndex ).setValue( 4.0 );	// ???
					undo.Connect( wmP, mAP.elementByLogicalIndex( influenceIndex ) );

					MObject aObj( MFnNumericAttribute().create( "lockInfluenceWeights", "liw", MFnNumericData::kBoolean, false ) );
					undo.DagModifier().addAttribute( iDagPath.node(), aObj );
					undo.DagModifierDoIt();

					undo.Connect( MPlug( iDagPath.node(), aObj ), lwAP.elementByLogicalIndex( influenceIndex ) );

					// TODO: Dag Pose for each influenceObject
					++influenceIndex;
				}

				{
					MPlug wlP( skinFn.findPlug( "weightList") );
					MObject wlObj( wlP.attribute() );
					MPlug wP( skinFn.findPlug( "weights") );
					MObject wObj( wP.attribute() );
					MPlug weP;

					const CUtlVector< CVertexWeights > &weightList( polygonData.m_weightList );
					const int nWeightList( weightList.Count() );

					MProgressWindow::reserve();
					MProgressWindow::setTitle( "Smd Import " );
					MProgressWindow::setProgressStatus( "Skin: Setting Weights" );
					MProgressWindow::setProgressMin( 0 );
					MProgressWindow::setProgressMax( nWeightList - 1 );
					MProgressWindow::startProgress();

					int nWeights;
					for ( int i( 0 ); i < nWeightList; ++i )
					{
						MProgressWindow::setProgress( i );
						wP.selectAncestorLogicalIndex( i, wlObj );
						const CVertexWeights &weights( weightList[ i ] );
						nWeights = weights.Count();
						for ( int j( 0 ); j < nWeights; ++j )
						{
							weP = wP.elementByLogicalIndex( nodeList[ weights.InfluenceIndex( j ) ].m_influenceIndex );
							weP.setValue( weights.NormalizedWeight( j ) );
						}
					}

					MProgressWindow::endProgress();
				}
			}

			MFnTransform origTransformFn( mDagPath.transform() );

			origTransformFn.findPlug( "tx" ).setLocked( true );
			origTransformFn.findPlug( "ty" ).setLocked( true );
			origTransformFn.findPlug( "tz" ).setLocked( true );
			origTransformFn.findPlug( "rx" ).setLocked( true );
			origTransformFn.findPlug( "ry" ).setLocked( true );
			origTransformFn.findPlug( "rz" ).setLocked( true );
			origTransformFn.findPlug( "sx" ).setLocked( true );
			origTransformFn.findPlug( "sy" ).setLocked( true );
			origTransformFn.findPlug( "sz" ).setLocked( true );
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdImport::CreateShadingGroups(
	const CQcData &qcData,
	const std::string &smdPath,
	const std::map< int, CPolygonData > &polygonDataMap,
	const std::map< std::string, CShadingGroup > &shadingGroupMap,
	ValveMaya::CUndo &undo )
{
	MIntArray mIntArray;
	MStatus mStatus;

	boost::smatch match;

	MDagModifier &mDagModifier( undo.DagModifier() );

	const std::map< std::string, CShadingGroup >::const_iterator sgEnd( shadingGroupMap.end() );
	for ( std::map< std::string, CShadingGroup >::const_iterator sgi( shadingGroupMap.begin() ); sgi != sgEnd; ++sgi )
	{
		MFnSet sgFn;

		const std::map< int, std::deque< int > > &componentMap( sgi->second.m_componentListMap );
		const std::map< int, std::deque< int > >::const_iterator cEnd( componentMap.end() );
		for ( std::map< int, std::deque< int > >::const_iterator ci( componentMap.begin() ); ci != cEnd; ++ci )
		{
			const std::map< int, CPolygonData >::const_iterator pi( polygonDataMap.find( ci->first ) );
			if ( pi == polygonDataMap.end() )
			{
				merr << "Can't find dagPath" << std::endl;
				continue;
			}

			const MDagPath &mDagPath( pi->second.m_mDagPath );
			if ( !mDagPath.isValid() || !mDagPath.length() )
			{
				merr << "Invalid dagPath found" << std::endl;
				continue;
			}

			Copy( ci->second, mIntArray );
			MFnSingleIndexedComponent cFn;
			MObject cObj = cFn.create( MFn::kMeshPolygonComponent );
			cFn.addElements( mIntArray );

			MSelectionList mSelectionList;
			mSelectionList.add( mDagPath, cObj );

			if ( ci != componentMap.begin() )
			{
				sgFn.addMembers( mSelectionList );
				continue;
			}

			MObject sgObj = sgFn.create( mSelectionList, MFnSet::kRenderableOnly, &mStatus );

			const std::string &materialPath( sgi->second.m_materialPath );
			std::string materialName( materialPath );
			if ( boost::regex_search( materialPath, match, boost::regex( "([^\\\\/]+)$" ) ) )
			{
				materialName = std::string( match[ 1 ].first, match[ 1 ].second );
			}

			MObject ssObj = mDagModifier.MDGModifier::createNode( "lambert" );
			mDagModifier.doIt();

			mDagModifier.renameNode( sgObj, MString( ( materialName + "SG" ).c_str() ) );
			mDagModifier.renameNode( ssObj, MString( ( materialName ).c_str() ) );
			mDagModifier.doIt();

			const MFnDependencyNode ssFn( ssObj );

			// Connecting shading group to surface shader
			MPlug srcP( ssFn.findPlug( "outColor", &mStatus ) );
			if ( mStatus && !srcP.isNull() )
			{
				MPlug dstP( sgFn.findPlug( "surfaceShader", &mStatus ) );
				if ( mStatus && !dstP.isNull() )
				{
					MPlugArray mPlugArray;
					if ( dstP.connectedTo( mPlugArray, true, false ) )
					{
						for ( uint i( 0 ); i != mPlugArray.length(); ++i )
						{
							mDagModifier.disconnect( mPlugArray[ i ], dstP );
						}
					}
					mDagModifier.connect( srcP, dstP );
				}
			}

			mDagModifier.doIt();

			undo.Connect( ssFn.findPlug( "message" ), GetNextDefaultShaderPlug() );

			const std::string texturePath( TexturePathFromMaterialPath( qcData, smdPath, materialPath ) );
			const std::string tgaPath( TargaPathFromTexturePath( qcData, smdPath, texturePath ) );

			MObject fObj = CreateFileNode( tgaPath, mDagModifier );
			MFnDependencyNode fFn( fObj );
			mDagModifier.connect( fFn.findPlug( "outColor" ), ssFn.findPlug( "color" ) );
			mDagModifier.doIt();
		}
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::ostream &operator<<(
	ostream &os,
	const CUv &uv )
{
	os << uv.u << ", " << uv.v;
	return os;
}


//-----------------------------------------------------------------------------
// It's expected that T & U are stl container classes and that the thing that
// they contain is the same and there's an operator==() defined for it
//-----------------------------------------------------------------------------
template < class T_t, class U_t >
void GetUniqueData(
	const T_t &data,					// The data to find the unique values of
	U_t &uniqueData,					// The unique versions of the data
	std::vector< uint > &dataMap )		// Map non-unique indexes to unique indexes
{
	uniqueData.clear();
	uniqueData.reserve( data.size() );
	dataMap.reserve( data.size() );

	U_t::const_iterator vi;
	U_t::const_iterator vBegin;
	U_t::const_iterator vEnd;

	const T_t::const_iterator ivBegin( data.begin() );
	const T_t::const_iterator ivEnd( data.end() );
	for ( T_t::const_iterator ivi( ivBegin ); ivi != ivEnd; ++ivi )
	{
		vBegin = uniqueData.begin();
		for ( vi = vBegin, vEnd = uniqueData.end(); vi != vEnd; ++vi )
		{
			if ( *ivi == *vi )
			{
				dataMap.push_back( std::distance( vBegin, vi ) );
				break;
			}
		}

		if ( vi == vEnd )
		{
			dataMap.push_back( uniqueData.size() );
			uniqueData.push_back( *ivi );
		}
	}

	Assert( data.size() == dataMap.size() );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CQcData::ParseQc(
	const std::string &smdPath,
	const std::string &qcPath )
{
	if ( _access( qcPath.c_str(), 04 ) == 0 )
	{
		try
		{
			const boost::regex upaxis( "^\\s*\\$upaxis\\s+([^\\s]+)\\s*$", boost::regex::icase );
			const boost::regex scale( "^\\s*\\$scale\\s+([^\\s]+)\\s*$", boost::regex::icase );
			const boost::regex cdmaterials( "^\\s*\\$cdmaterials\\s+([^\\s]+)\\s*$", boost::regex::icase );

			boost::smatch match;

			std::string buf;
			std::ifstream ifs( qcPath.c_str() );

			while ( std::getline( ifs, buf ) )
			{
				if ( boost::regex_match( buf, match, upaxis ) )
				{
					const std::string axis( match[ 1 ].first, match[ 1 ].second );
					if ( strchr( axis.c_str(), 'y' ) || strchr( axis.c_str(), 'Y' ) )
					{
						m_upAxis = 1U;
					}
					else if ( strchr( axis.c_str(), 'x' ) || strchr( axis.c_str(), 'X' ) )
					{
						m_upAxis = 0U;
					}
					else
					{
						m_upAxis = 2U;
					}
				}
				else if ( boost::regex_match( buf, match, scale ) )
				{
					const std::string mStr( match[ 1 ].first, match[ 1 ].second );
					m_scale = strtod( mStr.c_str(), NULL );
				}
				else if ( boost::regex_match( buf, match, cdmaterials ) )
				{
					m_cdmaterials.push_back( std::string( match[ 1 ].first, match[ 1 ].second ) );
				}
			}

			return true;
		}
		catch ( ... )
		{
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CQcData::GetQcData(
	const std::string &smdPath )
{
	try
	{
		// Look for same thing named with a .qc extension
		std::string qcPath( boost::regex_replace( smdPath, boost::regex( "\\.[^\\.]*$" ), ".qc" ) );
		if ( _access( qcPath.c_str(), 04 ) == 0 )
			return ParseQc( smdPath, qcPath );

		// Do the '_reference' crap
		qcPath = boost::regex_replace( qcPath, boost::regex( "([^/]+)_reference(\\.[^\\.]*)?$", boost::regex::icase ), "$1$2" );

		if ( _access( qcPath.c_str(), 04 ) == 0 )
			return ParseQc( smdPath, qcPath );

		qcPath = boost::regex_replace( qcPath, boost::regex( "(\\.[^\\.]*)$" ), "_reference$1" );

		if ( _access( qcPath.c_str(), 04 ) == 0 )
			return ParseQc( smdPath, qcPath );

		// Look for any *.qc file in the same directory as the smd that contains the smd pathname

		boost::smatch match;
		if ( boost::regex_match( smdPath, match, boost::regex( "^(.+[\\\\/])([^\\\\/]+)$" ) ) )
		{
			const std::string path( match[ 1 ].first, match[ 1 ].second );
			const std::string file( match[ 2 ].first, match[ 2 ].second );

			struct _finddata_t qcFile;
			long hFile;

			/* Find first .c file in current directory */
			if( ( hFile = _findfirst( ( path + "*.qc" ).c_str(), &qcFile ) ) == -1L )
			{
				merr << "No *.c files in current directory!" << std::endl;
			}
			else
			{
				/* Find the rest of the .c files */
				do {
					std::ifstream ifs( ( path + qcFile.name ).c_str() );
					std::string buf;
					boost::regex smdRegex( std::string( "\"" ) + file + "\"", boost::regex::icase );
					while ( std::getline( ifs, buf ) )
					{
						if ( boost::regex_search( buf, smdRegex ) )
						{
							_findclose( hFile );
							return ParseQc( smdPath, path + qcFile.name );
						}
					}
				} while( _findnext( hFile, &qcFile ) == 0 );

				_findclose( hFile );
			}
		}
	}
	catch ( const std::exception &e )
	{
		merr << "Exception: " << e.what() << std::endl;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Create a Maya mesh from the data from the SMD
//
// TODO: Merge non-unique vertices, normals & UVs
//-----------------------------------------------------------------------------
MObject CSmdImport::CreateMesh(
	const MObject &parentObj,
	const std::string &smdPath,
	const MString &name,
	CPolygonData &polygonData,
	MSelectionList &rootNodeList,
	MDagModifier &mDagModifier )
{
	const std::deque< int > &i_polygonCounts( polygonData.m_polygonCounts );
	const std::deque< int > &i_polygonConnects( polygonData.m_polygonConnects );
	const std::deque< MFloatVector > &i_vertexArray( polygonData.m_vertexArray );
	const std::deque< int > &i_uvIds( polygonData.m_uvIds );
	const std::deque< CUv > &uvs( polygonData.m_uvArray );
	const std::deque< MFloatVector > &i_normalArray( polygonData.m_normalArray );

	MIntArray polygonCounts;
	MIntArray polygonConnects;
	MFloatPointArray vertices;
	MIntArray uvIds;
	MFloatArray us;
	MFloatArray vs;
	MVectorArray normals;

	Copy( i_polygonCounts, polygonCounts );
	Copy( i_polygonConnects, polygonConnects );
	Copy( i_vertexArray, vertices );
	Copy( i_uvIds, uvIds );
	Copy( uvs, us, vs );
	Copy( i_normalArray, normals );

	MObject tObj = mDagModifier.createNode( "transform" );
	mDagModifier.doIt();

	if ( tObj.isNull() )
	{
		merr << "Creation of mesh parent failed" << std::endl;
		return tObj;
	}

	if ( !parentObj.isNull() )
	{
		MDagPath pDagPath;
		MDagPath::getAPathTo( parentObj, pDagPath );

		const MMatrix pMatInv( pDagPath.inclusiveMatrixInverse() );
		double m[ 4 ][ 4 ];
		pMatInv.get( m );
		const MFloatMatrix pFloatMatInv( m );

		MString newName( MFnDependencyNode( parentObj ).name() );
		mDagModifier.renameNode( parentObj, newName + "_parent" );
		mDagModifier.doIt();

		mDagModifier.renameNode( tObj, newName );
		mDagModifier.doIt();

		MObject grandParentObj;
		if ( pDagPath.length() > 1 )
		{
			MDagPath gpDagPath( pDagPath );
			gpDagPath.pop();
			grandParentObj = gpDagPath.node();
		}

		mDagModifier.reparentNode( tObj, grandParentObj );
		mDagModifier.doIt();

		MDagPath tDagPath;
		MDagPath::getAPathTo( tObj, tDagPath );

		const MFnTransform pFn( pDagPath );

		MVector t;
		MPlug tsP( pFn.findPlug( "t" ) );
		tsP.child( 0 ).getValue( t.x );
		tsP.child( 1 ).getValue( t.y );
		tsP.child( 2 ).getValue( t.z );

		MVector r;
		MPlug rsP( pFn.findPlug( "r" ) );
		rsP.child( 0 ).getValue( r.x );
		rsP.child( 1 ).getValue( r.y );
		rsP.child( 2 ).getValue( r.z );

		const MFnTransform tFn( tDagPath );

		MPlug tdP( tFn.findPlug( "t" ) );
		tdP.child( 0 ).setValue( t.x );
		tdP.child( 1 ).setValue( t.y );
		tdP.child( 2 ).setValue( t.z );

		MPlug rdP( tFn.findPlug( "r" ) );
		rdP.child( 0 ).setValue( r.x );
		rdP.child( 1 ).setValue( r.y );
		rdP.child( 2 ).setValue( r.z );

		MVector g;
		rdP.child( 0 ).getValue( g.x );
		rdP.child( 1 ).getValue( g.y );
		rdP.child( 2 ).getValue( g.z );

		for ( uint i( 0 ); i < pDagPath.childCount(); ++i )
		{
			mDagModifier.reparentNode( pDagPath.child( i ), tObj );
			mDagModifier.doIt();
		}

		if ( rootNodeList.hasItem( pDagPath ) )
		{
			MDagPath tmpDagPath;

			for ( uint i( 0 ); i < rootNodeList.length(); ++i )
			{
				rootNodeList.getDagPath( i, tmpDagPath );
				if ( tmpDagPath == pDagPath )
				{
					rootNodeList.remove( i );
					break;
				}
			}
		}
		mDagModifier.deleteNode( parentObj );

		if ( tDagPath.length() == 1 )
		{
			rootNodeList.add( tDagPath );
		}

		const uint nVertices( vertices.length() );
		for ( uint i( 0 ); i < nVertices; ++i )
		{
			vertices[ i ] *= pFloatMatInv;
		}

		const uint nNormals( normals.length() );
		for ( uint i( 0 ); i < nNormals; ++i )
		{
			normals[ i ] = normals[ i ].transformAsNormal( pMatInv );
		}
	}
	else
	{
		mDagModifier.renameNode( tObj, name );
		mDagModifier.doIt();
	}

	MFnMesh meshFn;
	MObject meshObj = meshFn.create( vertices.length(), polygonCounts.length(), vertices, polygonCounts, polygonConnects, tObj );

	if ( meshObj.isNull() )
	{
		merr << "Creation of mesh failed" << std::endl;
		return tObj;
	}

	mDagModifier.renameNode( meshObj, MFnDependencyNode( tObj ).name() + "Shape" );
	mDagModifier.doIt();

	MStatus mStatus;

	Assert( us.length() == vs.length() );
	for ( uint i( 0U ); i != uvIds.length(); ++i )
	{
		Assert( uvIds[ i ] < ( static_cast< int >( us.length() ) ) );
	}

	mStatus = meshFn.clearUVs();
	mStatus = meshFn.setUVs( us, vs );
	mStatus = meshFn.assignUVs( polygonCounts, uvIds );
	if ( !mStatus )
	{
		merr << "UV Assignment failed on: " << meshFn.name() << std::endl;
	}

	MIntArray faceList;
	faceList.setLength( polygonCounts.length() );
	for ( uint i( 0 ); i != faceList.length(); ++i )
	{
		faceList[ i ] = static_cast< int >( i );
	}

	MIntArray normalIds( normals.length() );
	Assert( normals.length() == i_polygonConnects.size() );
	const uint nFaceVaryingVertices( normals.length() );
	for ( uint i( 0U ); i != nFaceVaryingVertices; ++i )
	{
		normalIds[ i ] = i;
	}

	meshFn.setFaceVertexNormals( normals, faceList, normalIds );

	return tObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CSmdImport::CSmdImport(
	const std::string &game,
	const std::string &textureArchive )
: m_optImportSkeleton( false )
, m_optAutoStripPrefix( true )
, m_optImportAnimation( false )
{
	try
	{
		m_textureArchive = boost::regex_replace( textureArchive, boost::regex( "[\\\\/]+" ), "/" );

		std::string vproject( getenv( "VPROJECT" ) );
		m_vproject = boost::regex_replace( vproject, boost::regex( "[\\\\/]+" ), "/" );
		m_vproject = boost::regex_replace( m_vproject, boost::regex( "/+$" ), "" );

		if ( !game.empty() )
		{
			std::string tmp_vproject;
			if ( boost::algorithm::all( game, boost::algorithm::is_any_of( "\\/" ) ) )
			{
				tmp_vproject = boost::regex_replace( game, boost::regex( "[\\\\/]+" ), "/" );
				tmp_vproject = boost::regex_replace( tmp_vproject, boost::regex( "/+$" ), "" );
			}
			else
			{
				tmp_vproject = boost::regex_replace( m_vproject, boost::regex( "[^/]+/*$" ), game );
			}
			if ( _access( ( tmp_vproject + "/gameinfo.txt" ).c_str(), 04 ) == 0 )
			{
				m_vproject = tmp_vproject;
			}
		}

		std::string gameinfo( m_vproject + "/gameinfo.txt" );

		if ( _access( gameinfo.c_str(), 04 ) == 0 )
		{
			std::ifstream ifs( gameinfo.c_str() );
			std::string buf;

			enum ParserState
			{
				kPreSearchPaths,
				kPostSearchPaths
			};

			ParserState parserState( kPreSearchPaths );

			const boost::regex searchPath( "^\\s*SearchPaths\\s*$", boost::regex::icase );
			const boost::regex keyValue( "^\\s*([^\\s]+)\\s+([^\\s]+)\\s*$" );
			const boost::regex closeBrace( "^\\s*\\}\\s*$" );
			const boost::regex game( "^\"?Game\"?$", boost::regex::icase );
			boost::smatch match;
			while ( std::getline( ifs, buf ) )
			{
				if ( parserState == kPreSearchPaths )
				{
					if ( boost::algorithm::find_regex( buf, searchPath ) )
					{
						parserState = kPostSearchPaths;
					}
				}
				else
				{
					if ( boost::regex_match( buf, match, keyValue ) )
					{
						const std::string key( match[ 1 ].first, match[ 1 ].second );
						if ( boost::regex_match( key, game ) )
						{
							std::string val( match[ 2 ].first, match[ 2 ].second );
							val = boost::regex_replace( val, boost::regex( "\\|gameinfo_path\\|", boost::regex::icase ), m_vproject + "/" );
							val = boost::regex_replace( val, boost::regex( "^[\"']" ), "" );
							val = boost::regex_replace( val, boost::regex( "[\"']$" ), "" );
							val = boost::regex_replace( val, boost::regex( "[\\\\/]+" ), "/" );
							val = boost::regex_replace( val, boost::regex( "/\\.$" ), "" );
							val = boost::regex_replace( val, boost::regex( "/+$" ), "" );
							if ( !boost::algorithm::contains( val, "/" ) )
							{
								val = boost::regex_replace( m_vproject, boost::regex( "[^/]+/*$" ), val );
							}
							m_gamePath.push_back( val );
							m_contentPath.push_back( boost::regex_replace( val, boost::regex( "/game/" ), "/content/" ) );
						}
					}
					else if ( boost::regex_match( buf, closeBrace ) )
					{
						break;
					}
				}
			}
		}
		else
		{
			m_gamePath.push_back( m_vproject );
			m_contentPath.push_back( boost::regex_replace( m_vproject, boost::regex( "/game/" ), "/content/" ) );
			mwarn << "Can't open " << gameinfo << ", only find targa files in " << m_contentPath.back() << "/materialsrc" << std::endl;
		}
	}
	catch ( ... )
	{
	}
}


#define MAX_WEIGHTS_PER_VERTEX 3


//=============================================================================
//
//=============================================================================
class CVertexWeight
{
public:
	uint m_boneIndex;
	float m_weight;

	inline bool operator==(
		const CVertexWeight &rhs ) const
	{
		return m_boneIndex == rhs.m_boneIndex && m_weight == rhs.m_weight;
	}

	inline bool operator!=(
		const CVertexWeight &rhs ) const
	{
		return m_boneIndex != rhs.m_boneIndex || m_weight != rhs.m_weight;
	}
};


//=============================================================================
//
//=============================================================================
class CVertex
{
public:
	MFloatVector m_vertex;
	MFloatVector m_normal;
	CUv m_uv;
	uint m_nWeights;
	CVertexWeight m_vertexWeights[ MAX_WEIGHTS_PER_VERTEX ];

	inline bool operator==(
		const CVertex &rhs ) const
	{
		if ( m_vertex != rhs.m_vertex ||
			m_normal != rhs.m_normal ||
			m_uv != rhs.m_uv ||
			m_nWeights != rhs.m_nWeights )
			return false;

		for ( uint i( 0U ); i != m_nWeights; ++i )
		{
			if ( m_vertexWeights[ i ] != rhs.m_vertexWeights[ i ] )
				return false;
		}

		return true;
	}

	inline void Reset()
	{
		m_nWeights = 0U;
	}
};


//=============================================================================
//
//=============================================================================
class CTriangle
{
public:
	uint m_nVertices;
	CVertex m_vertices[ 3 ];

	bool Valid() const;

	inline void Reset()
	{
		m_nVertices = 0U;
		m_vertices[ 0U ].Reset();
		m_vertices[ 1U ].Reset();
		m_vertices[ 2U ].Reset();
	}
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CTriangle::Valid() const
{
	// A triangle is valid if all three vertices are unique

	const uint nVerticesMinusOne( m_nVertices - 1U );
	for ( uint i( 0U ); i != nVerticesMinusOne; ++i )
	{
		for ( uint j( i + 1U ); j != m_nVertices; ++j )
		{
			if ( m_vertices[ i ].m_vertex == m_vertices[ j ].m_vertex )
				return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template < class T_t, class U_t, class V_t >
int AddUniqueVertex(
	const T_t &vertex,
	U_t &vertices,
	V_t &polygonConnects )
{
	uint vIndex( 0U );
	const U_t::const_iterator vEnd( vertices.end() );
	for ( U_t::const_iterator vi( vertices.begin() ); vi != vEnd; ++vi, ++vIndex )
	{
		if ( *vi == vertex )
		{
			polygonConnects.push_back( vIndex );
			return vIndex;
		}
	}

	vIndex = vertices.size();

	polygonConnects.push_back( vIndex );
	vertices.push_back( vertex );
	return vertices.size() - 1;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
enum ParserState
{
	kUnknown,
	kPreamble,
	kGeneral,
	kNodes,
	kSkeleton,
	kTriangles
};


//-----------------------------------------------------------------------------
// Tests whether the passed buffer is an all whitespace line or not
//-----------------------------------------------------------------------------
inline const char *
ParserSkipSpace(
	const char *pBuf )
{
	// Skip to first non-whitespace character
	for ( ;; )
	{
		if ( !isspace( *pBuf ) )
			break;

		++pBuf;
	}

	return pBuf;
}


//-----------------------------------------------------------------------------
// Tests whether the passed buffer is an all whitespace line or not
//-----------------------------------------------------------------------------
bool CSmdImport::ParserIsBlankLine(
	const char *pBuf )
{
	for ( const char *pChar( pBuf ); *pChar; ++pChar )
	{
		if ( !isspace( *pChar ) )
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
// Returns true the specified buffer is a comment line
// meaning that it starts with // (with optional white space preceeding the //)
//-----------------------------------------------------------------------------
bool IsCommentLine(
	const char *pBuf )
{
	pBuf = ParserSkipSpace( pBuf );

	if ( *pBuf && *pBuf == '/' && *( pBuf + 1 ) == '/' )
	{
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool HandleQcHints(
	const char *pBuf,
	CQcData &qcData )
{
	if ( !IsCommentLine( pBuf ) )
		return false;

	char key[ 512 ];
	char val[ 512 ];

	if ( sscanf( pBuf, "// %512s=%512s", key, val ) == 2 )
	{
		if ( Q_stricmp( key, "UPAXIS" ) == 0 )
		{
			if ( strpbrk( val, "xX" ) )
			{
				qcData.m_upAxis = 0U;
			}
			else if ( strpbrk( val, "yY" ) )
			{
				qcData.m_upAxis = 1U;
			}
			else if ( strpbrk( val, "zZ" ) )
			{
				qcData.m_upAxis = 2U;
			}
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CSmdImport::ParserHandleVersion(
	const char *pBuf,
	int *version )
{
	pBuf = ParserSkipSpace( pBuf );

	if ( !( *pBuf && Q_strnicmp( pBuf, "version", 7 ) == 0 ) )
		return false;

	if ( version )
	{
		*version = strtol( pBuf + 7, NULL, 0 );	// Skip past "version"
	}

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool HandleSectionStart(
	const char *pBuf,
	const char *pSectionId )
{
	pBuf = ParserSkipSpace( pBuf );

	if ( *pBuf && Q_stricmp( pBuf, pSectionId ) == 0 )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdImport::ParserGetNodeName(
	const char *pBuf,
	MString &name ) const
{
	const char *pStart( ParserSkipSpace( pBuf ) );

	while ( *pStart && *pStart == '\"' )
	{
		++pStart;
	}

	const char *pEnd( pStart + Q_strlen( pStart ) - 1 );

	while ( pEnd >= pStart )
	{
		if ( !isspace( *pEnd ) && *pEnd != '\"' )
			break;

		--pEnd;
	}

	if ( m_optAutoStripPrefix )
	{
		const char *pDot( strchr( pStart, '.' ) );
		if ( pDot )
		{
			pStart = pDot + 1;
		}
	}

	if ( m_nodeDelPrefix.length() && strncmp( pStart, m_nodeDelPrefix.asChar(), m_nodeDelPrefix.length() ) == 0 )
	{
		pStart = pStart + m_nodeDelPrefix.length();
	}

	if ( pStart <= pEnd )
	{
		name = MString( pStart, pEnd - pStart + 1 );
	}
	else
	{
		name = "Unknown";
	}

	if ( m_nodeAddPrefix.length() )
	{
		name = m_nodeAddPrefix + name;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CSmdImport::ParserHandleSkeletonLine(
	const char *pBuf,
	int &nId,
	MString &name,
	int &pId ) const
{
	pBuf = ParserSkipSpace( pBuf );

	char tmpBuf[ 512 ];
	if ( sscanf( pBuf, "%d %512s %d", &nId, tmpBuf, &pId ) == 3 )
	{
		ParserGetNodeName( tmpBuf, name );
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ParserFindNodeUnderRoot(
	const MDagPath &rootDagPath,
	const MString &name,
	MDagPath &foundDagPath )
{
	MDagPath mDagPath;

	MItDag dIt;
	for ( dIt.reset( rootDagPath ); !dIt.isDone(); dIt.next() )
	{
		dIt.getPath( mDagPath );
		if ( dIt.getPath( mDagPath ) && MFnDagNode( mDagPath ).name() == name )
		{
			foundDagPath = mDagPath;
			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ParserHandleTime(
	const char *pBuf,
	int &time )
{
	if ( sscanf( pBuf, "time %d", &time ) == 1 )
		return true;

	return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CSmdImport::ParserHandleSample(
	const char *pBuf,
	CUtlVector< CNodeData > &nodeList )
{
	int nId;
	MFloatVector p;
	MFloatVector o;

	if ( sscanf( pBuf, "%d %f %f %f %f %f %f", &nId, &p.x, &p.y, &p.z, &o.x, &o.y, &o.z ) != 7 )
		return false;

	if ( nId >= nodeList.Count() || !nodeList[ nId ].Valid() )
		return false;

	CNodeData &nodeData( nodeList[ nId ] );
	nodeData.m_positions.AddToTail( p );
	nodeData.m_orientations.AddToTail( o );

	return true;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MStatus CSmdImport::ImportAnimation(
	const std::string &i_smdPath,
	const MDagPath &rootDagPath,
	CQcData &qcData,
	ValveMaya::CUndo &undo )
{
	const std::string smdPath( GetSmdPath( i_smdPath ) );
	qcData.GetQcData( smdPath );

	std::ifstream ifs( smdPath.c_str() );
	if ( !ifs )
	{
		merr << "Cannot open smd file " << smdPath << std::endl;
		return MS::kFailure;
	}

	uint lineno( 0U );
	ParserState parserState( kPreamble );

	CUtlVector< CNodeData > nodeList;
	nodeList.EnsureCapacity( 512 );	

	int time;
	CUtlVector< int > times;
	int nId;
	int pId;
	MString name;
	MDagPath mDagPath;

	std::string buf;
	const char *pBuf;
	while ( std::getline( ifs, buf ) )
	{
		++lineno;
		pBuf = buf.c_str();

		if ( ParserIsBlankLine( pBuf ) )
			continue;

		if ( parserState == kPreamble )
		{
			if ( HandleQcHints( pBuf, qcData ) )
				continue;

			if ( ParserHandleVersion( pBuf ) )
			{
				parserState = kGeneral;
				continue;
			}

			mwarn << "Line " << lineno << ": Invalid line found during preamble, only expecting comment, blank line or version= line" << std::endl;
			mwarn << "Line " << lineno << ": Invalid: " << pBuf << std::endl;
		}

		else if ( parserState == kGeneral )
		{
			if ( HandleSectionStart( pBuf, "nodes" ) )
			{
				parserState = kNodes;
			}
			else if ( HandleSectionStart( pBuf, "skeleton" ) )
			{
				parserState = kSkeleton;
			}
			else if ( HandleSectionStart( pBuf, "triangles" ) )
			{
				parserState = kTriangles;
			}
		}
		else if ( parserState == kNodes )
		{
			if ( HandleSectionStart( pBuf, "end" ) )
			{
				parserState = kGeneral;
				continue;
			}

			if ( !ParserHandleSkeletonLine( pBuf, nId, name, pId ) )
				continue;

			if ( !ParserFindNodeUnderRoot( rootDagPath, name, mDagPath ) )
				continue;

			nodeList.EnsureCount( nId + 1 );
			CNodeData &nodeData( nodeList[ nId ] );

			if ( nodeData.Valid() )
			{
				merr << "Line " << lineno << ": Duplicate node id " << nId << "(" << name << ")" << " found, already " << nodeData.m_dagPath.partialPathName() << std::endl;
				continue;
			}

			nodeData.m_dagPath = mDagPath;
		}
		else if ( parserState == kSkeleton )
		{
			if ( HandleSectionStart( pBuf, "end" ) )
			{
				parserState = kGeneral;
				continue;
			}

			if ( ParserHandleTime( pBuf, time ) )
			{
				times.AddToTail( time );
				continue;
			}

			if ( ParserHandleSample( pBuf, nodeList ) )
				continue;

//			merr << "Line " << lineno << ": Can't handle sample: " << pBuf << std::endl;
		}
		else if ( parserState == kTriangles )
		{
			if ( HandleSectionStart( pBuf, "end" ) )
			{
				parserState = kGeneral;
				continue;
			}
		}
		else
		{
			mwarn << "Line " << lineno << ": Unknown Parser State Ignoring: " << buf << std::endl;
			parserState = kGeneral;
		}
	}

	CreateAnimationCurves( times, nodeList, undo );

	return MS::kSuccess;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MObject CreateAndConnectAnimCurve(
	const MPlug &mPlug,
	ValveMaya::CUndo &undo )
{
	MObject cObj;

	MDGModifier dgMod;
	MStatus mStatus;
	if ( MFnUnitAttribute( mPlug.attribute() ).unitType() == MFnUnitAttribute::kAngle )
	{
		cObj = undo.DagModifier().MDGModifier::createNode( "animCurveTA", &mStatus );
	}
	else
	{
		cObj = undo.DagModifier().MDGModifier::createNode( "animCurveTL", &mStatus );
	}

	undo.DagModifierDoIt();

	const MFnDependencyNode cFn( cObj );

	undo.Connect( cFn.findPlug( "output" ), mPlug, true );

	return cObj;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CopyValues(
	const CUtlVector< MFloatVector > &src,
	MDoubleArray &dst,
	int index )
{
	const int nSrc( src.Count() );
	dst.setLength( nSrc );

	const float *pSrc( &src.Base()->x + index );
	for ( int i( 0 ); i < nSrc; ++i, pSrc += 3 )
	{
		dst[ i ] = *pSrc;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdImport::CreateAnimationCurves(
	const CUtlVector< int > &times,
	const CUtlVector< CNodeData > &nodeList,
	ValveMaya::CUndo &undo )
{
	MTimeArray timeArray;
	const int nTimes( times.Count() );
	timeArray.setLength( nTimes );
	const MTime::Unit uiUnit( MTime::uiUnit() );
	for ( int i( 0 ); i < nTimes; ++i )
	{
		timeArray[ i ] = MTime( static_cast< double >( times[ i ] ), uiUnit );
	}

	MDoubleArray values;
	values.setLength( nTimes );

	const int nNodes( nodeList.Count() );
	for ( int i( 0 ); i < nNodes; ++i )
	{
		const CNodeData &nodeData( nodeList[ i ] );
		if ( nodeData.Valid() && nodeData.m_positions.Count() == nTimes && nodeData.m_orientations.Count() == nTimes )
		{
			const MDagPath &mDagPath( nodeData.m_dagPath );
			const MFnDagNode dagFn( mDagPath );

			CopyValues( nodeData.m_positions, values, 0 );
			MFnAnimCurve( CreateAndConnectAnimCurve( dagFn.findPlug( "tx" ), undo ) ).addKeys( &timeArray, &values );

			CopyValues( nodeData.m_positions, values, 1 );
			MFnAnimCurve( CreateAndConnectAnimCurve( dagFn.findPlug( "ty" ), undo ) ).addKeys( &timeArray, &values );

			CopyValues( nodeData.m_positions, values, 2 );
			MFnAnimCurve( CreateAndConnectAnimCurve( dagFn.findPlug( "tz" ), undo ) ).addKeys( &timeArray, &values );

			CopyValues( nodeData.m_orientations, values, 0 );
			MFnAnimCurve( CreateAndConnectAnimCurve( dagFn.findPlug( "rx" ), undo ) ).addKeys( &timeArray, &values );

			CopyValues( nodeData.m_orientations, values, 1 );
			MFnAnimCurve( CreateAndConnectAnimCurve( dagFn.findPlug( "ry" ), undo ) ).addKeys( &timeArray, &values );

			CopyValues( nodeData.m_orientations, values, 2 );
			MFnAnimCurve( CreateAndConnectAnimCurve( dagFn.findPlug( "rz" ), undo ) ).addKeys( &timeArray, &values );
		}
	}

	MAnimControl::setMinMaxTime( timeArray[ 0 ], timeArray[ nTimes - 1 ] );
	MAnimControl::setAnimationStartEndTime( timeArray[ 0 ], timeArray[ nTimes - 1 ] );
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MDagPath CSmdImport::DoIt(
	const std::string &i_smdPath,
	CQcData &qcData,
	const MTransformationMatrix &topLevelMat,
	ValveMaya::CUndo &undo )
{
	const std::string smdPath( GetSmdPath( i_smdPath ) );
	qcData.GetQcData( smdPath );

	std::ifstream ifs( smdPath.c_str() );
	if ( !ifs )
	{
		merr << "Cannot open smd file " << smdPath << std::endl;
		return MDagPath();
	}

	MDagModifier &mDagModifier( undo.DagModifier() );

	try {
		const boost::regex blankLine( "^\\s*$" );
		const boost::regex comment( "^\\s*\\/\\/.*$" );
		const boost::regex infoComment( "^\\s*\\/\\/\\s*([^=]+)=\\s*(.*)\\s*$" );
		const boost::regex triangleMaterial( "^\\s*(.*)\\s*$" );

		char tmpBuf[ 1024 ];
		int version = -1;
		uint lineno( 0U );
		ParserState parserState( kPreamble );

		std::map< int, CPolygonData > polygonDataMap;
		std::map< std::string, CShadingGroup > shadingGroupMap;
		std::map< std::string, CShadingGroup >::iterator currentShadingGroup( shadingGroupMap.end() );

		std::vector< std::string > triangleData;

		CUtlVector< CNodeData > nodeList;
		nodeList.EnsureCapacity( 512 );		// Reserve space for 512 of them

		int nId;
		int pId;
		MFloatVector p;
		MFloatVector n;
		CUv uv;

		MSelectionList rootNodeList;

		CTriangle triangle;
		triangle.Reset();

		int badPreambleCount( 0 );

		std::string buf;
		boost::smatch match;
		while ( std::getline( ifs, buf ) )
		{
			++lineno;

			// Blank lines & comments can be in any state
			if ( boost::regex_match( buf, blankLine ) )
			{
				continue;
			}

			if ( boost::regex_match( buf, comment ) )
			{
				if ( parserState == kPreamble && boost::regex_match( buf, match, infoComment ) )
				{
					const std::string key( match[ 1 ].first, match[ 1 ].second );
					if ( _stricmp( key.c_str(), "UPAXIS" ) == 0 )
					{
						const std::string value( match[ 2 ].first, match[ 2 ].second );
						if ( strchr( value.c_str(), 'y' ) || strchr( value.c_str(), 'Y' ) )
						{
							qcData.m_upAxis = 1U;
						}
						else if ( strchr( value.c_str(), 'z' ) || strchr( value.c_str(), 'Z' ) )
						{
							qcData.m_upAxis = 2U;
						}
						else if ( strchr( value.c_str(), 'x' ) || strchr( value.c_str(), 'X' ) )
						{
							qcData.m_upAxis = 0U;
						}
					}
				}
				continue;
			}

			// Only new thing allowed in pre-am
			if ( parserState == kPreamble )
			{
				if ( sscanf( buf.c_str(), "%1024s %d", tmpBuf, &version ) == 2 && _stricmp( tmpBuf, "version" ) == 0 )
				{
					parserState = kGeneral;
				}
				else
				{
					mwarn << "Line " << lineno << ": Invalid: " << buf << std::endl;
					mwarn << "Line " << lineno << ": Invalid line found during preamble, expecting comment, blank line or version= line" << std::endl;
					++badPreambleCount;

					if ( badPreambleCount > 10 )
					{
						merr << i_smdPath << " doesn't seem to be an SMD file.  Aborting import" << std::endl;
					}
				}
			}
			else if ( parserState == kGeneral )
			{
				if ( sscanf( buf.c_str(), "%1024s", tmpBuf ) == 1 && _stricmp( tmpBuf, "nodes" ) == 0 )
				{
					parserState = kNodes;
				}
				else if ( sscanf( buf.c_str(), "%1024s", tmpBuf ) == 1 && _stricmp( tmpBuf, "skeleton" ) == 0 )
				{
					parserState = kSkeleton;
				}
				else if ( sscanf( buf.c_str(), "%1024s", tmpBuf ) == 1 && _stricmp( tmpBuf, "triangles" ) == 0 )
				{
					parserState = kTriangles;
				}
			}
			else if ( parserState == kNodes )
			{
				if ( sscanf( buf.c_str(), "%1024s", tmpBuf ) == 1 && _stricmp( tmpBuf, "end" ) == 0 )
				{
					parserState = kGeneral;
				}
				else
				{
					char tmpBuf[ 1024 ];

					if ( m_optImportSkeleton && sscanf( buf.c_str(), "%d %1024s %d", &nId, tmpBuf, &pId ) == 3 )
					{
						nodeList.EnsureCount( nId + 1 );
						CNodeData &nodeData( nodeList[ nId ] );

						if ( nodeData.Valid() )
						{
							merr << "Line " << lineno << ": Duplicate node id " << nId << "(" << tmpBuf << ")" << " found, already " << nodeData.m_dagPath.partialPathName() << std::endl;
						}
						else
						{
							MObject pObj;
							if ( pId >= 0 )
							{
								if ( pId < nodeList.Count() )
								{
									CNodeData &parentNodeData( nodeList[ pId ] );
									if ( parentNodeData.Valid() )
									{
										pObj = parentNodeData.m_dagPath.node();
										nodeData.m_parentIndex = pId;
									}
								}

								if ( pObj.isNull() )
								{
									merr << "Line " << lineno << ": Can't find node parent " << pId << " for node " << nId << std::endl;
								}
							}

							MObject jObj( mDagModifier.createNode( "joint", pObj ) );
							mDagModifier.doIt();

							if ( jObj.isNull() )
							{
								merr << "Line " << lineno << ": Can't create joint " << tmpBuf << std::endl;
							}
							else
							{
								if ( !pObj.isNull() )
								{
									mDagModifier.connect( MFnDependencyNode( pObj ).findPlug( "scale" ), MFnDependencyNode( jObj ).findPlug( "inverseScale" ) );
									mDagModifier.doIt();
								}

								char *pNodename( tmpBuf );
								while ( *pNodename && *pNodename == '\"' )
								{
									++pNodename;
								}

								for ( int nodenameLen( Q_strlen( pNodename ) ); *( pNodename + nodenameLen - 1 ) == '\"'; --nodenameLen )
								{
									*( pNodename + nodenameLen - 1 ) = '\0';
								}

								if ( m_optAutoStripPrefix )
								{
									char *pDot = strchr( pNodename, '.' );
									if ( pDot )
									{
										pNodename = pDot + 1;
									}
								}

								if ( m_nodeDelPrefix.length() && strncmp( pNodename, m_nodeDelPrefix.asChar(), m_nodeDelPrefix.length() ) == 0 )
								{
									pNodename = pNodename + m_nodeDelPrefix.length();
								}

								if ( m_nodeAddPrefix.length() )
								{
									MString newNodeName( m_nodeAddPrefix + pNodename );
									mDagModifier.renameNode( jObj, newNodeName );
								}
								else
								{
									mDagModifier.renameNode( jObj, pNodename );
								}
								mDagModifier.doIt();

								MDagPath jDagPath;
								MDagPath::getAPathTo( jObj, jDagPath );
								nodeData.m_dagPath = jDagPath;
								if ( jDagPath.length() == 1 )
								{
									rootNodeList.add( jDagPath );
								}
							}
						}
					}
					else
					{
//						mwarn << "Line " << lineno << ": Unknown \"node\" line: " << buf << std::endl;
					}
				}
			}
			else if ( parserState == kSkeleton )
			{
				if ( sscanf( buf.c_str(), "%1024s", tmpBuf ) == 1 && _stricmp( tmpBuf, "end" ) == 0 )
				{
					parserState = kGeneral;
				}
				else if ( sscanf( buf.c_str(), "%d %f %f %f %f %f %f", &nId, &p.x, &p.y, &p.z, &n.x, &n.y, &n.z ) == 7 )
				{
					if ( m_optImportSkeleton )
					{
						if ( nId < nodeList.Count() && nodeList[ nId ].Valid() )
						{
							const MFnDagNode dagFn( nodeList[ nId ].m_dagPath );
							MPlug tP( dagFn.findPlug( "t" ) );
							tP.child( 0 ).setValue( p.x );
							tP.child( 1 ).setValue( p.y );
							tP.child( 2 ).setValue( p.z );
							MPlug rP( dagFn.findPlug( "r" ) );
							rP.child( 0 ).setValue( n.x );
							rP.child( 1 ).setValue( n.y );
							rP.child( 2 ).setValue( n.z );
						}
						else
						{
							merr << "Line " << lineno << ": Can't find node " << nId << " to set bind state on " << std::endl;
						}
					}
				}
				else
				{
					// Just ignore for now...
				}
			}
			else if ( parserState == kTriangles )
			{
				if ( sscanf( buf.c_str(), "%1024s", tmpBuf ) == 1 && _stricmp( tmpBuf, "end" ) == 0 )
				{
					triangle.Reset();
					parserState = kGeneral;
				}
				else if ( sscanf( buf.c_str(), "%d %f %f %f %f %f %f %f %f", &nId, &p.x, &p.y, &p.z, &n.x, &n.y, &n.z, &uv.u, &uv.v ) == 9 )
				{
					if ( triangle.m_nVertices == 3U )
					{
						mwarn << "Line " << lineno << ": Too many vertices in triangle" << std::endl;
						continue;
					}

					Assert( triangle.m_nVertices < 3 );
					CVertex &vertex( triangle.m_vertices[ triangle.m_nVertices++ ] );
					vertex.m_vertex = p;
					vertex.m_normal = n;
					vertex.m_uv = uv;

					if ( m_optImportSkeleton )
					{
						boost::algorithm::trim_left( buf );
						boost::algorithm::split( triangleData, buf, boost::algorithm::is_space(), boost::algorithm::token_compress_on );
						uint tEnd( triangleData.size() - 1 );
						if ( tEnd > 10 )
						{
							bool renormalize( false );

							int id( -1 );
							double weight( 0.0f );
							for ( uint ti( 10U ); ti < tEnd; ++ti )
							{
								id = strtol( triangleData[ ti ].c_str(), NULL, 0 );
								++ti;
								weight = strtod( triangleData[ ti ].c_str(), NULL );

								if ( id >= 0 )
								{
									if ( vertex.m_nWeights == 3 )
									{
										for ( uint i( 0U ); i != 3U; ++i )
										{
											CVertexWeight &vertexWeight( vertex.m_vertexWeights[ i ] );
											if ( weight > vertexWeight.m_weight )
											{
												vertexWeight.m_weight = weight;
												vertexWeight.m_boneIndex = id;
											}
										}

										renormalize = true;
									}
									else
									{
										Assert( vertex.m_nWeights < 3 );
										CVertexWeight &vertexWeight( vertex.m_vertexWeights[ vertex.m_nWeights++ ] );
										vertexWeight.m_boneIndex = id;
										vertexWeight.m_weight = weight;
									}
								}
								else
								{
									mwarn << "Line: " << lineno << ": Ignoring unknown joint id for vertex weight" << std::endl;
								}
							}

							if ( renormalize )
							{
								float totalWeight( 0.0f );

								for ( uint i( 0U ); i != 3U; ++i )
								{
									CVertexWeight &vertexWeight( vertex.m_vertexWeights[ i ] );
									totalWeight += vertexWeight.m_weight;
								}

								for ( uint i( 0U ); i != 3U; ++i )
								{
									CVertexWeight &vertexWeight( vertex.m_vertexWeights[ i ] );
									vertexWeight.m_weight /= totalWeight;
								}
							}
						}
					}

					if ( triangle.m_nVertices != 3U )
						continue;

					if ( !triangle.Valid() )
						continue;

					CPolygonData &polygonData( polygonDataMap[ nId ] );
					std::set< int > &influenceObjects( polygonData.m_influenceObjects );
					int vIndex;

					currentShadingGroup->second.m_componentListMap[ nId ].push_back( polygonData.m_polygonCounts.size() );
					polygonData.m_polygonCounts.push_back( triangle.m_nVertices );

					for ( uint i( 0U ); i != 3U; ++i )
					{
						const CVertex &vertex( triangle.m_vertices[ i ] );

						vIndex = AddUniqueVertex( vertex.m_vertex, polygonData.m_vertexArray, polygonData.m_polygonConnects );
						AddUniqueVertex( vertex.m_uv, polygonData.m_uvArray, polygonData.m_uvIds );

						polygonData.m_normalArray.push_back( vertex.m_normal );

						if ( vertex.m_nWeights )
						{
							polygonData.m_weightList.EnsureCount( vIndex + 1 );
							CVertexWeights &vertexWeights( polygonData.m_weightList[ vIndex ] );

							for ( uint j( 0U ); j != vertex.m_nWeights; ++j  )
							{
								const CVertexWeight &vertexWeight( vertex.m_vertexWeights[ j ] );
								if ( vertexWeights.AddWeight( vIndex, vertexWeight.m_boneIndex, vertexWeight.m_weight ) )
								{
									influenceObjects.insert( vertexWeight.m_boneIndex );
								}
							}
						}
					}

					triangle.Reset();
				}
				else if ( boost::regex_match( buf, match, triangleMaterial ) )
				{
					std::string materialPath( match[ 1 ].first, match[ 1 ].second );
					materialPath = boost::regex_replace( materialPath, boost::regex( "\\..*$" ), "" );
					if ( currentShadingGroup == shadingGroupMap.end() || currentShadingGroup->second.m_materialPath != materialPath )
					{
						currentShadingGroup = shadingGroupMap.find( materialPath );
						if ( currentShadingGroup == shadingGroupMap.end() )
						{
							std::pair< std::map< std::string, CShadingGroup >::iterator, bool > insertResult =
								shadingGroupMap.insert( std::pair< std::string, CShadingGroup >( materialPath, CShadingGroup() ) );
							currentShadingGroup = insertResult.first;
							if ( currentShadingGroup == shadingGroupMap.end() )
							{
								merr << "Line " << lineno << ": Couldn't store shading group information material " << materialPath << std::endl;
								continue;
							}
							currentShadingGroup->second.m_materialPath = materialPath;
						}
					}
					triangle.Reset();
				}
				else
				{
					mwarn << "Line " << lineno << ": Unexpected data in triangles: " << buf << std::endl;
					triangle.Reset();
				}
			}
			else
			{
				mwarn << "Line " << lineno << ": Unknown Parser State Ignoring: " << buf << std::endl;
				parserState = kGeneral;
			}
		}

		// Get the name of the smd
		MString name( "unknown" );
		if ( boost::regex_search( smdPath, match, boost::regex( "([^\\\\/\\.]+)(\\..*)?$" ) ) )
		{
			name = std::string( match[ 1 ].first, match[ 1 ].second ).c_str();
		}

		{
			const std::map< int, CPolygonData >::const_iterator pEnd( polygonDataMap.end() );
			for ( std::map< int, CPolygonData >::iterator pi( polygonDataMap.begin() ); pi != pEnd; ++pi )
			{
				MObject parentObj;
				if ( nodeList.Count() > pi->first )
				{
					const CNodeData &nodeData( nodeList[ pi->first ] );
					if ( nodeData.Valid() )
					{
						parentObj = nodeData.m_dagPath.node();
					}
				}

				MObject tObj = CreateMesh( parentObj, smdPath, name, pi->second, rootNodeList, mDagModifier );
				if ( tObj.isNull() )
				{
					merr << "Line " << lineno << ": Couldn't create mesh" << std::endl;
					continue;
				}

				MDagPath &mDagPath( pi->second.m_mDagPath );
				MDagPath::getAPathTo( tObj, mDagPath );

				if ( parentObj.isNull() )
				{
					rootNodeList.add( mDagPath, MObject::kNullObj, true );
				}
			}

			CreateShadingGroups( qcData, smdPath, polygonDataMap, shadingGroupMap, undo );

			MDagPath retPath;

			if ( rootNodeList.length() == 0 )
			{
				merr << "No data created from smd import" << std::endl;
				return retPath;
			}

			if ( rootNodeList.length() == 1 )
			{
				rootNodeList.getDagPath( 0, retPath );
			}
			else
			{
				MObject rObj( mDagModifier.createNode( "transform" ) );
				mDagModifier.doIt();
				if ( rObj.isNull() )
				{
					merr << "Couldn't create group node for imported smd data" << std::endl;
					return MDagPath();
				}

				mDagModifier.renameNode( rObj, name );

				MDagPath mDagPath;
				for ( MItSelectionList sIt( rootNodeList, MFn::kDagNode ); !sIt.isDone(); sIt.next() )
				{
					if ( sIt.getDagPath( mDagPath ) )
					{
						mDagModifier.reparentNode( mDagPath.node(), rObj );
					}
				}
				mDagModifier.doIt();

				MDagPath::getAPathTo( rObj, retPath );
			}

			if ( !topLevelMat.isEquivalent( MMatrix::identity ) )
			{
				if (	!retPath.hasFn( MFn::kTransform ) ||
						retPath.hasFn( MFn::kJoint ) ||
						(
							retPath.hasFn( MFn::kTransform ) &&
							!MFnTransform( retPath ).transformation().isEquivalent( MTransformationMatrix::identity )
						)
					)
				{
					MObject rObj( mDagModifier.createNode( "transform" ) );
					mDagModifier.doIt();
					mDagModifier.renameNode( rObj, name );
					mDagModifier.reparentNode( retPath.node(), rObj );
					mDagModifier.doIt();
					MDagPath::getAPathTo( rObj, retPath );
				}

				MFnTransform( retPath ).set( topLevelMat );
			}

			if ( m_optImportSkeleton )
			{
				SkinMeshes( nodeList, polygonDataMap, undo );
				PurgeUnusedBones( nodeList, rootNodeList, mDagModifier );
			}

			return retPath;
		}
	}
	catch ( std::exception &e )
	{
		mwarn << "Exception " << e.what() << std::endl;
		return MDagPath();
	}

	return MDagPath();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CSmdImport::TexturePathFromMaterialPath(
	const CQcData &qcData,
	const std::string &smdPath,
	const std::string &materialPath,
	const std::string &textureType )
{
	// See if it's cached
	{
		const std::map< std::string, std::string >::const_iterator ci( m_materialPathToTexturePathCache.find( materialPath ) );
		if ( ci != m_materialPathToTexturePathCache.end() )
			return ci->second;
	}

	std::string vmtPath;

	const std::list< std::string >::const_iterator mEnd( qcData.m_cdmaterials.end() );
	std::list< std::string >::const_iterator mi;

	const std::list< std::string >::const_iterator pEnd( m_gamePath.end() );
	std::list< std::string >::const_iterator pi;
	for ( pi = m_gamePath.begin(); vmtPath.empty() && pi != pEnd; ++pi )
	{
		std::string tmpVmtPath( *pi + "/materials/" + materialPath + ".vmt" );
		if ( _access( tmpVmtPath.c_str(), 04 ) == 0 )
		{
			vmtPath = tmpVmtPath;
			break;
		}

		for ( mi = qcData.m_cdmaterials.begin(); mi != mEnd; ++mi )
		{
			tmpVmtPath = *pi + "/materials/" + *mi + "/" + materialPath + ".vmt";

			if ( _access( tmpVmtPath.c_str(), 04 ) == 0 )
			{
				vmtPath = tmpVmtPath;
				break;
			}
		}
	}

	if ( vmtPath.empty() )
	{
		std::string tmpMaterialPath( boost::regex_replace( smdPath, boost::regex( "[\\\\/][^\\\\/]*$" ), "" ) );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "[\\\\/]+" ), "/" );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "^.*/game/[^/]*/" ), "" );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "^.*/content/[^/]*/" ), "" );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "^[^:]:/" ), "" );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "/$" ), "" );

		for ( pi = m_gamePath.begin(); pi != pEnd; ++pi )
		{
			std::string tmpVmtPath( *pi + "/materials/" + tmpMaterialPath + "/" + materialPath + ".vmt" );
			if ( _access( tmpVmtPath.c_str(), 04 ) == 0 )
			{
				vmtPath = tmpVmtPath;
				break;
			}
		}
	}

	if ( vmtPath.empty() )
	{
		std::string tmpMaterialPath( boost::regex_replace( smdPath, boost::regex( "\\.[^\\.]*$" ), "" ) );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "[\\\\/]+" ), "/" );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "^.*/game/[^/]*/" ), "" );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "^.*/content/[^/]*/" ), "" );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "^[^:]:/" ), "" );
		tmpMaterialPath = boost::regex_replace( tmpMaterialPath, boost::regex( "/$" ), "" );

		for ( pi = m_gamePath.begin(); pi != pEnd; ++pi )
		{
			std::string tmpVmtPath( *pi + "/materials/" + tmpMaterialPath + "/" + materialPath + ".vmt" );
			if ( _access( tmpVmtPath.c_str(), 04 ) == 0 )
			{
				vmtPath = tmpVmtPath;
				break;
			}
		}
	}

	if ( vmtPath.empty() )
		return vmtPath;

	if ( _access( vmtPath.c_str(), 04 ) == 0 )
	{
		std::ifstream ifs( vmtPath.c_str() );
		std::string buf;

		const boost::regex keyValue( "^\\s*[^\\s]+\\s+([^\\s]+)\\s*$" );
		boost::smatch match;
		while ( std::getline( ifs, buf ) )
		{
			if ( boost::algorithm::icontains( buf, textureType ) )
			{
				std::string val;
				if ( boost::regex_match( buf, match, keyValue ) )
				{
					val = std::string( match[ 1 ].first, match[ 1 ].second );
				}

				if ( val.empty() )
					continue;

				val = boost::regex_replace( val, boost::regex( "^[\"']" ), "" );
				val = boost::regex_replace( val, boost::regex( "[\"']$" ), "" );
				val = boost::regex_replace( val, boost::regex( "[\\\\/]+" ), "/" );
				val = boost::regex_replace( val, boost::regex( "^/+" ), "" );
				val = boost::regex_replace( val, boost::regex( "/+$" ), "" );

				m_materialPathToTexturePathCache[ materialPath ] = val;

				return val;
			}
		}
	}

	return "";
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CSmdImport::TargaPathFromTexturePath(
	const CQcData &qcData,
	const std::string &smdPath,
	const std::string &texturePath )
{
	// See if it's cached
	const std::map< std::string, std::string >::const_iterator ci( m_texturePathToTargaPathCache.find( texturePath ) );
	if ( ci != m_texturePathToTargaPathCache.end() )
		return ci->second;

	std::string tgaPath;

	const std::list< std::string >::const_iterator mEnd( qcData.m_cdmaterials.end() );
	std::list< std::string >::const_iterator mi;

	const std::list< std::string >::const_iterator pEnd( m_contentPath.end() );
	std::list< std::string >::const_iterator pi;

	for ( pi = m_contentPath.begin(); pi != pEnd; ++pi )
	{
		tgaPath = *pi + "/materialsrc/" + texturePath + ".tga";
		if ( _access( tgaPath.c_str(), 04 ) == 0 )
		{
			tgaPath = ArchiveTexture( tgaPath );
			m_texturePathToTargaPathCache[ texturePath ] = tgaPath;
			return tgaPath;
		}
	}

	for ( pi = m_contentPath.begin(); pi != pEnd; ++pi )
	{
		for ( mi = qcData.m_cdmaterials.begin(); mi != mEnd; ++mi )
		{
			tgaPath = *pi + "/materialsrc/" + *mi + "/" + texturePath + ".tga";

			if ( _access( tgaPath.c_str(), 04 ) == 0 )
			{
				tgaPath = ArchiveTexture( tgaPath );
				m_texturePathToTargaPathCache[ texturePath ] = tgaPath;
				return tgaPath;
			}
		}
	}

	std::string tmpTexturePath( boost::regex_replace( smdPath, boost::regex( "\\.[^\\.]*$" ), "" ) );
	tmpTexturePath = boost::regex_replace( tmpTexturePath, boost::regex( "[\\\\/]+" ), "/" );
	tmpTexturePath = boost::regex_replace( tmpTexturePath, boost::regex( "^.*/game/[^/]*/" ), "" );
	tmpTexturePath = boost::regex_replace( tmpTexturePath, boost::regex( "^[^:]:/" ), "" );
	tmpTexturePath = boost::regex_replace( tmpTexturePath, boost::regex( "/$" ), "" );

	for ( pi = m_contentPath.begin(); pi != pEnd; ++pi )
	{
		tgaPath = *pi + "/materialsrc/" + tmpTexturePath + ".tga";
		if ( _access( tgaPath.c_str(), 04 ) == 0 )
		{
			tgaPath = ArchiveTexture( tgaPath );
			m_texturePathToTargaPathCache[ texturePath ] = tgaPath;
			return tgaPath;
		}
	}

	return "";
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CSmdImport::GetSmdPath(
	const std::string &i_smdPath ) const
{
	try
	{
		if ( _access( i_smdPath.c_str(), 04 ) == 0 )
			return i_smdPath;

		std::string smdPath( boost::regex_replace( i_smdPath, boost::regex( "[\\\\/]+" ), "/" ) );
		smdPath = boost::regex_replace( smdPath, boost::regex( "\\.[^\\.]*$" ), ".smd" );

		if ( _access( smdPath.c_str(), 04 ) == 0 )
			return smdPath;

		smdPath = boost::regex_replace( smdPath, boost::regex( "([^/]+)_reference(\\.[^\\.]*)?$", boost::regex::icase ), "$1$2" );

		if ( _access( smdPath.c_str(), 04 ) == 0 )
			return smdPath;

		smdPath = boost::regex_replace( smdPath, boost::regex( "(\\.[^\\.]*)$" ), "_reference$1" );

		if ( _access( smdPath.c_str(), 04 ) == 0 )
			return smdPath;

		std::string fullSmdPath;

		const std::list< std::string >::const_iterator pEnd( m_contentPath.end() );
		for ( std::list< std::string >::const_iterator pi( m_contentPath.begin() ); pi != pEnd; ++pi )
		{
			smdPath = boost::regex_replace( i_smdPath, boost::regex( "[\\\\/]+" ), "/" );
			smdPath = boost::regex_replace( smdPath, boost::regex( "\\.[^\\.]*$" ), ".smd" );
			fullSmdPath = *pi + "/" + smdPath;

			if ( _access( fullSmdPath.c_str(), 04 ) == 0 )
				return fullSmdPath;

			smdPath = boost::regex_replace( smdPath, boost::regex( "([^/\\.]+)_reference(\\.[^\\.]*)?$", boost::regex::icase ), "$1$2" );
			fullSmdPath = *pi + "/" + smdPath;

			if ( _access( fullSmdPath.c_str(), 04 ) == 0 )
				return fullSmdPath;

			smdPath = boost::regex_replace( smdPath, boost::regex( "(\\.[^\\.]*)$" ), "_reference$1" );
			fullSmdPath = *pi + "/" + smdPath;

			if ( _access( fullSmdPath.c_str(), 04 ) == 0 )
				return fullSmdPath;
		}
	}
	catch ( const std::exception &e )
	{
		merr << "Error: " << e.what() << std::endl;
	}

	return i_smdPath;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
std::string CSmdImport::ArchiveTexture(
	const std::string &tgaPath )
{
	if ( !m_textureArchive.empty() )
	{
		const std::string srcPath( boost::regex_replace( tgaPath, boost::regex( "[\\\\/]+" ), "/" ) );
		boost::smatch match;

		if ( boost::regex_search( srcPath, match, boost::regex( "([^/]+)$" ) ) )
		{
			const std::string srcFile( match[ 1 ].first, match[ 1 ].second );
			const std::string dstPath( m_textureArchive + "/" + srcFile );

			MString melCmd;
			melCmd += "{\n";
			melCmd += "  string $c = \"copy \\\"";
			melCmd += srcPath.c_str();
			melCmd += "\\\" \\\"";
			melCmd += dstPath.c_str();
			melCmd += "\\\"\";\n";
			melCmd += "  $c = `substituteAllString $c \"/\" \"\\\\\"`;\n";
			melCmd += "  system $c;\n";
			melCmd += "}\n";

			MGlobal::executeCommand( melCmd );

			return dstPath;
		}
	}

	return tgaPath;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void CSmdImport::SetNodeDelPrefix(
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
void CSmdImport::SetNodeAddPrefix(
	const MString &nodePrefix )
{
	if ( nodePrefix.length() )
	{
		m_nodeAddPrefix = nodePrefix;
	}
}