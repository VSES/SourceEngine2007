#include "pch.h"

CStringA DXMGraphStdMtlAdapter::MySignature("DXMGraphStdMtlAdapter");

MString gSurfaceShader("shader/surface");

bool DXMGraphStdMtlAdapter::IsInterested( MObject& node )
{
	MFnDependencyNode depNode(node);

	return /*(depNode.typeId() != DirectXShader::id) && */
		(MFnDependencyNode::classification(depNode.typeName()) == gSurfaceShader);
}


void DXMGraphStdMtlAdapter::OnNodeAdded( DXMNode* node )
{
	DXMNodeStdMtlAdapter* adapter= new DXMNodeStdMtlAdapter();
	AddAdapterOnNode(node, adapter);

}

void DXMGraphStdMtlAdapter::OnNodeRemoved( DXMNode* node )
{
	DXMNodeAdapter* adapter= node->FindAdapter(DXMGraphStdMtlAdapter::MySignature);
	if(adapter)
		RemoveAdapterOnNode(node, adapter);
}

