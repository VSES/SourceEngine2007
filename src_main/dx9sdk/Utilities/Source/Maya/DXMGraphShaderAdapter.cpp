#include "pch.h"

CStringA DXMGraphShaderAdapter::MySignature("DXMGraphShaderAdapter");

bool DXMGraphShaderAdapter::IsInterested( MObject& node )
{
	MFnDependencyNode depNode(node);

	return (depNode.typeId() ==  DirectXShader::id);
}


void DXMGraphShaderAdapter::OnNodeAdded( DXMNode* node )
{
	DXMNodeShaderAdapter* adapter= new DXMNodeShaderAdapter();
	AddAdapterOnNode(node, adapter);

}

void DXMGraphShaderAdapter::OnNodeRemoved( DXMNode* node )
{
	DXMNodeAdapter* adapter= node->FindAdapter(DXMNodeShaderAdapter::MySignature);
	if(adapter)
		RemoveAdapterOnNode(node, adapter);
}

