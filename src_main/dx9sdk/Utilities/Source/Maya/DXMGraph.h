#pragma once
#ifndef DXMGRAPH_H
#define DXMGRAPH_H

class DXMNode;
class DXMNodeAdapter;
class DXMGraphAdapter;


//to use this
//Initialize(...)
//AddAdapters(...) ...
//SetCallbackState(true)
//RefreshNodes()
class DXMGraph
{
	friend DXMNode;
	friend DXMNodeAdapter;
	friend DXMGraphAdapter;
public:
	typedef CRBMap<CStringA, DXMGraphAdapter*, CStringElementTraits<CStringA> >  DXMAdapterMap;
	typedef CRBMapIterator<DXMAdapterMap> DXMAdapterIterator;

	typedef CRBMap<DXMNode*, DXMNode* > DXMNodeMap;
	typedef CRBMapIterator<DXMNodeMap> DXMNodeIterator;

	DXMGraph();
	virtual ~DXMGraph();


	virtual void Initialize(const MString& nodeAnchorLongName, const MString& nodeAnchorShortName);
	virtual void Destroy();


	//recursively set all Adapters and Nodes SetCallbackState to same value
	virtual void SetCallbackState(bool Activate);
	virtual bool GetCallbackState(){ return CallbackState; }


	DXMAdapterIterator GetAdapterIterator();
	DXMGraphAdapter* FindAdapter(const CStringA& signature);

	//After using these two function you should RefreshNodes()
	//because existing nodes might need to be effected by the adapter and 
	//some maya Dependency Nodes may not have been tracked at all since there
	//may have not been an "interested" adapter
	void AddAdapter(DXMGraphAdapter* adapter);
	void RemoveAdapter(DXMGraphAdapter* adapter, bool DeleteAdapter);

	DXMNodeIterator GetNodeIterator();
	DXMNode* FindNode( MObject& node);
	DXMNodeAdapter* FindNodeAdapter( MObject& node, const CStringA& sig);

	//for each node: remove and add
	void RegisterNodes();
	void DeregisterNodes();
	void RefreshNodes();

	bool IsNodeSelectable(MObject& testMe);

	virtual void OnNodeAdded( MObject & node); 
	static void DispatchNodeAdded( MObject & node, void* clientData );

	virtual void OnNodeRemoved( MObject & node);
	static void DispatchNodeRemoved( MObject & node, void* clientData );
	
	virtual void OnBeforeSceneChange();
	static void DispatchBeforeSceneChange( void* clientData );

	virtual void OnAfterSceneChange();
	static void DispatchAfterSceneChange( void* clientData );




	//these are the names to be used to anchor the nodes of this graph to the Maya nodes.
	const MString& GetNodeAnchorLongName(){ return NodeAnchorLongName; }
	const MString& GetNodeAnchorShortName(){ return NodeAnchorShortName; }

protected:

	//override me to used specialized DXMNode;
	virtual DXMNode* CreateNode();
	void RemoveNode(DXMNode* tracker, bool DeleteIt);


	MString NodeAnchorLongName;
	MString NodeAnchorShortName;
	
	DXMAdapterMap Adapters;
	DXMNodeMap Nodes;

	bool CallbackState;
	MCallbackId NodeAddedCID;
	MCallbackId NodeRemovedCID;
	MCallbackId BeforeSceneOpenCID;
	MCallbackId BeforeSceneNewCID;
	MCallbackId AfterSceneOpenCID;
	MCallbackId AfterSceneNewCID;


};




#endif