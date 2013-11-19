//====== Copyright © 1996-2006, Valve Corporation, All rights reserved. =======
//
// Purpose: 
//
//=============================================================================

// Maya includes
#include <maya/MGlobal.h>

// Valve includes
#include "VsMayaMPxFactory.h"

//-----------------------------------------------------------------------------
// Governs drag and drop behavior vsVmtToTex & fileTexture nodes
//-----------------------------------------------------------------------------

class CVsVmtToTexBehavior : public CVsMayaMPxDragAndDropBehavior
{
public:
	virtual bool shouldBeUsedFor(
		MObject &sourceNode,
		MObject &destinationNode,
		MPlug &sourcePlug,
		MPlug &destinationPlug );

	virtual MStatus connectNodeToNode(
		MObject &sourceNode, 
		MObject &destinationNode,
		bool force );

	virtual MStatus connectNodeToAttr(
		MObject &sourceNode,
		MPlug &destinationPlug,
		bool force );

	virtual MStatus connectAttrToNode(
		MPlug &sourceNode, 
		MObject &destinationNode,
		bool force );

	virtual MStatus connectAttrToAttr(
		MPlug &sourceNode,
		MPlug &destinationPlug,
		bool force );
};


//-----------------------------------------------------------------------------
// Register with Maya
//-----------------------------------------------------------------------------
INSTALL_MAYA_MPXDRAGANDDROPBEHAVIOR(
	CVsVmtToTexBehavior,
	vsVmtToTexBehavior,
	"Manages drag and dropping vsVmtToTex nodes and file (fileTexture) nodes" );


//-----------------------------------------------------------------------------
// Return true is this MPxDragAndDropBehavior will handle the connection
// between the two given nodes
//-----------------------------------------------------------------------------
bool CVsVmtToTexBehavior::shouldBeUsedFor(
	MObject &srcObj, 
	MObject &dstObj,
	MPlug &/* srcPlug */,
	MPlug &/* dstPlug */ )
{
	if ( MFnDependencyNode( srcObj ).typeName() == "vsVmtToTex" && dstObj.hasFn( MFn::kFileTexture ) )
	{
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------
// Connect two plugs
//-----------------------------------------------------------------------------
static MStatus ConnectAttr(
	const MPlug &srcPlug, 
	const MPlug &destPlug, 
	bool force)
{
	if ( !srcPlug.isNull() && !destPlug.isNull() )
	{
		MString cmd( "connectAttr " );
		if ( force )
		{
			cmd += "-f ";
		}
		cmd += srcPlug.name() + " " + destPlug.name();
		return MGlobal::executeCommand( cmd );
	}

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// Handle the connection between the VsVmtToTex node and the fileTexture node
//-----------------------------------------------------------------------------

MStatus CVsVmtToTexBehavior::connectNodeToNode(
	MObject &srcObj, 
	MObject &dstObj, 
	bool force )
{
	const MFnDependencyNode srcFn( srcObj );

	if ( srcFn.typeName() == "vsVmtToTex" && dstObj.hasFn( MFn::kFileTexture ) )
	{
		const MFnDependencyNode dstFn( dstObj );
		return ConnectAttr(
			srcFn.findPlug( "texturePath" ),
			dstFn.findPlug( "fileTextureName" ),
			force );
	}

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// If dragging a vsVmtToTex node onto a fileTexture node, connect up stuff
//-----------------------------------------------------------------------------
MStatus CVsVmtToTexBehavior::connectNodeToAttr(
	MObject &srcObj,
	MPlug &dstPlug,
	bool force )
{
	const MFnDependencyNode srcFn( srcObj );

	if ( srcFn.typeName() == "vsVmtToTex" && dstPlug.node().hasFn( MFn::kFileTexture ) && dstPlug.attribute().hasFn( MFn::kStringData ) )
	{
		return ConnectAttr( srcFn.findPlug( "texturePath" ), dstPlug, force );
	}

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// If dragging a vsVmtToTex node onto a fileTexture node, connect up stuff
//-----------------------------------------------------------------------------
MStatus CVsVmtToTexBehavior::connectAttrToNode( 
	MPlug &srcPlug,
	MObject &dstObj,
	bool force )
{
	const MFnDependencyNode srcFn( srcPlug.node() );
	if ( srcFn.typeName() == "vsVmtToTex" && srcPlug.attribute().hasFn( MFn::kStringData) && dstObj.hasFn( MFn::kFileTexture ) )
	{
		const MFnDependencyNode dstFn( dstObj );
		return ConnectAttr( srcPlug, dstFn.findPlug( "fileTextureName" ), force );
	}

	return MS::kFailure;
}


//-----------------------------------------------------------------------------
// If dragging a vsVmtToTex node onto a fileTexture node, connect up stuff
//-----------------------------------------------------------------------------
MStatus CVsVmtToTexBehavior::connectAttrToAttr( 
	MPlug &srcPlug, 
	MPlug &dstPlug, 
	bool force )
{
	return ConnectAttr( srcPlug, dstPlug, force );
}