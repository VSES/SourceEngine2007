#include "pch.h"

void DXMGraphAdapter::Destroy()
{ 
	SetCallbackState(false); 

	for(DXMGraph::DXMNodeIterator itrNodes= Graph->GetNodeIterator();
		!itrNodes.Done();
		itrNodes.Next())
	{
		DXMNode* node= NULL;
		itrNodes.GetValue(node);

		for(DXMNode::DXMAdapterIterator nodeAdapterItr= node->GetAdapterIterator();
			!nodeAdapterItr.Done();
			nodeAdapterItr.Next())
		{
			DXMNodeAdapter* nodeAdapter=  NULL;
			nodeAdapterItr.GetValue(nodeAdapter);

			if( nodeAdapter->OwnerSignature() == Signature())
			{
				nodeAdapter->GetNode()->RemoveAdapter(nodeAdapter, true);
			}
		}
	}

	Graph= NULL; 
}

UINT DXMGraphAdapter::GetSyncRequestCount()
{
	return SyncNodeAdapters.GetCount();
} 

bool DXMGraphAdapter::HasRecentSyncRequest()
{
	return RecentSyncRequest;
}

void DXMGraphAdapter::ResetRecentSyncRequest()
{
	RecentSyncRequest= false;
}

void DXMGraphAdapter::AddSyncRequest(DXMNodeAdapter* nodeAdapter)
{
	RecentSyncRequest= true;
	SyncNodeAdapters.SetAt(nodeAdapter, false);
}

void DXMGraphAdapter::RemoveSyncRequest(DXMNodeAdapter* nodeAdapter)
{
	SyncNodeAdapters.RemoveKey(nodeAdapter);
}

UINT DXMGraphAdapter::Synchronize()
{
	UINT result= 0;
	bool synced;
	for(POSITION position =  SyncNodeAdapters.GetHeadPosition();
		position!=NULL;
		SyncNodeAdapters.GetNext(position))
	{
		DXMNodeAdapter* nodeAdapter= NULL;
		nodeAdapter= SyncNodeAdapters.GetKeyAt(position);
			
		synced= nodeAdapter->Synchronize();

		if(synced)
		{
			SyncNodeAdapters.SetValueAt(position, synced );
			result++;
		}
	}

	if(result > 0)
	{
		//CLEAN OUT SYNCHRONIZED NODE ADAPTERS
		//we dont do this above because of potential cascading effects in on Synchronize()
		for(POSITION position =  SyncNodeAdapters.GetHeadPosition();
			position!=NULL;
			( (!synced) ? SyncNodeAdapters.GetNext(position) : NULL ) )
		{
			synced= SyncNodeAdapters.GetValueAt(position);
			
			if(synced)
			{
				POSITION removePos= position;
				SyncNodeAdapters.GetNext(position);

				SyncNodeAdapters.RemoveAt(removePos);
			}
		}
	}

	return result;
}


DXMGraphAdapter::DXMGraphAdapter()
{ 
	Graph= NULL; 
	CallbackState= false; 
}

DXMGraphAdapter::~DXMGraphAdapter()
{
	DXCC_ASSERT( SyncNodeAdapters.GetCount() == 0);
}


DXMGraph* DXMGraphAdapter::GetGraph()
{ 
	return Graph; 
}
DXMNode* DXMGraphAdapter::FindNode(MObject& node)
{ 
	return Graph->FindNode(node); 
}

DXMNodeAdapter* DXMGraphAdapter::FindNodeAdapter(MObject& obj, const CStringA& signature)
{ 
	DXMNode* node= FindNode(obj);
    if(node)
		return node->FindAdapter(signature);
	
	return NULL;
}

void DXMGraphAdapter::Initialize(DXMGraph* owner)
{ 
	Graph= owner; 
	RecentSyncRequest= false;
	SetCallbackState(owner->GetCallbackState()); 
}

void DXMGraphAdapter::SetCallbackState(bool Activate)
{ 
	CallbackState= Activate; 
}

bool DXMGraphAdapter::GetCallbackState()
{
	return CallbackState; 
}


void DXMGraphAdapter::AddAdapterOnNode( DXMNode* node, DXMNodeAdapter* nodeAdapter )
{
	if(node && nodeAdapter)
		node->AddAdapter(nodeAdapter);
}

void DXMGraphAdapter::RemoveAdapterOnNode( DXMNode* node, DXMNodeAdapter* nodeAdapter )
{
	if(node && nodeAdapter)
		node->RemoveAdapter(nodeAdapter, true);
}

