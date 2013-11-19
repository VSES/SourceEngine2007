#include "pch.h"
	
CStringA DXMGraphLayerAdapter::MySignature( "DXMGraphLayerAdapter" );
	


DXMGraphLayerAdapter::DXMGraphLayerAdapter() : DXMGraphAdapter() 
{
}

DXMGraphLayerAdapter::~DXMGraphLayerAdapter()
{
}

void DXMGraphLayerAdapter::Initialize(DXMGraph* owner)
{
	DXMGraphAdapter::Initialize(owner);
}

void DXMGraphLayerAdapter::SetCallbackState(bool Activate)
{ 
	CallbackState= Activate;

	DXMGraphAdapter::SetCallbackState(Activate);
}


UINT DXMGraphLayerAdapter::Synchronize()
{
	UINT result= DXMGraphAdapter::Synchronize();

	return result;
}

bool DXMGraphLayerAdapter::IsInterested( MObject& node )
{
	return node.hasFn(MFn::kDisplayLayer);
}

void DXMGraphLayerAdapter::OnNodeAdded( DXMNode* node )
{
	DXMNodeLayerAdapter* adapter= new DXMNodeLayerAdapter();
	AddAdapterOnNode(node, adapter);
}

void DXMGraphLayerAdapter::OnNodeRemoved( DXMNode* node )
{
	DXMNodeAdapter* adapter= node->FindAdapter(DXMNodeLayerAdapter::MySignature);
	RemoveAdapterOnNode(node, adapter);
}


