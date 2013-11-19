#pragma once
#ifndef DXMGRAPHADAPTER_H
#define DXMGRAPHADAPTER_H
/*
class DXMNode;
class DXMNodeAdapter;
class DXMGraph;
*/

//inherit and customize me
class DXMGraphAdapter
{
	friend DXMGraph;
public:
	typedef CRBMap<DXMNodeAdapter*, bool >  DXMSyncMap;
	typedef CRBMapIterator<DXMSyncMap> DXMSyncIterator;


	DXMGraphAdapter();

	virtual ~DXMGraphAdapter();

	DXMGraph* GetGraph();
	DXMNode* FindNode(MObject& node);
	DXMNodeAdapter* FindNodeAdapter(MObject& node, const CStringA& signature);


	//entry name into the AdapterMap
	virtual const CStringA& Signature()= 0;

	//DXMGraph only tracks nodes where an adapters is interested in that type of node.
	virtual bool IsInterested( MObject& node )= 0;


	virtual bool GetCallbackState();

	virtual void OnNodeAdded( DXMNode* node )= 0;
	virtual void OnNodeRemoved( DXMNode* node )= 0;

	//undirty node adapters.
	//return the # of node adapters were cleaned durring the sync
	virtual UINT Synchronize();
	
	void AddSyncRequest(DXMNodeAdapter* nodeAdapter);
	void RemoveSyncRequest(DXMNodeAdapter* nodeAdapter);
	UINT GetSyncRequestCount();
	bool HasRecentSyncRequest();
	void ResetRecentSyncRequest();
protected:
	//USED BY DXMGraph
	virtual void Initialize(DXMGraph* owner);
	virtual void Destroy();
	virtual void SetCallbackState(bool Activate);

	void AddAdapterOnNode( DXMNode* node, DXMNodeAdapter* nodeAdapter );
	void RemoveAdapterOnNode( DXMNode* node, DXMNodeAdapter* nodeAdapter );



	DXMSyncMap SyncNodeAdapters;
	bool RecentSyncRequest;

	bool CallbackState;
	DXMGraph* Graph;

};

#endif