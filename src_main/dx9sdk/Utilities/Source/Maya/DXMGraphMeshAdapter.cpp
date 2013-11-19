#include "pch.h"

CStringA DXMGraphMeshAdapter::MySignature( "DXMGraphMeshAdapter" );


bool DXMGraphMeshAdapter::IsInterested( MObject& node )
{
	return node.hasFn(MFn::kMesh);
}

void DXMGraphMeshAdapter::OnNodeAdded( DXMNode* node )
{
	DXMNodeMeshAdapter* adapter= new DXMNodeMeshAdapter();
	AddAdapterOnNode(node, adapter);

}

void DXMGraphMeshAdapter::OnNodeRemoved( DXMNode* node )
{
	DXMNodeAdapter* adapter= node->FindAdapter(DXMGraphMeshAdapter::MySignature);
	if(adapter)
		RemoveAdapterOnNode(node, adapter);
}

