#include "pch.h"

DXMGraph::DXMGraph()
{
	CallbackState= false;
	NodeAddedCID= 0;
	NodeRemovedCID= 0; 
	BeforeSceneOpenCID= 0; 
	BeforeSceneNewCID= 0; 
	AfterSceneOpenCID= 0; 
	AfterSceneNewCID= 0; 
}

DXMGraph::~DXMGraph()
{
	DXCC_ASSERT( BeforeSceneOpenCID == 0 );
	DXCC_ASSERT( BeforeSceneNewCID == 0 );
	DXCC_ASSERT( AfterSceneOpenCID == 0 );
	DXCC_ASSERT( AfterSceneNewCID == 0 );
	DXCC_ASSERT( NodeRemovedCID == 0 );
	DXCC_ASSERT( NodeAddedCID == 0 );
	DXCC_ASSERT( Adapters.GetCount() == 0 );
	DXCC_ASSERT( Nodes.GetCount() == 0 );
}

DXMNode* DXMGraph::CreateNode()
{ 
	return new DXMNode(); 
}

void DXMGraph::Initialize(const MString& nodeAnchorLongName, const MString& nodeAnchorShortName)
{
	NodeAnchorLongName= nodeAnchorLongName;
	NodeAnchorShortName= nodeAnchorShortName;
}

DXMGraph::DXMAdapterIterator DXMGraph::GetAdapterIterator()
{ 
	return DXMAdapterIterator(&Adapters); 
}

DXMGraph::DXMNodeIterator DXMGraph::GetNodeIterator()
{ 
	return DXMNodeIterator(&Nodes); 
}

void DXMGraph::Destroy()
{
	SetCallbackState(false);

	for( DXMAdapterIterator adapterItr= GetAdapterIterator();
		!adapterItr.Done(); 
		adapterItr.First())//always take the first one since we are deleting it.
	{
		DXMGraphAdapter* adapter= NULL;
		adapterItr.GetValue(adapter);

		RemoveAdapter(adapter, true);
	}

	DXCC_ASSERT(Adapters.GetCount() == 0);

	for( DXMNodeIterator trackerItr= GetNodeIterator();
		!trackerItr.Done(); 
		trackerItr.First())//always take the first one since we are deleting it.
	{
		DXMNode* tracker= NULL;
		trackerItr.GetValue(tracker);

		RemoveNode(tracker, true);
	}

	DXCC_ASSERT(Nodes.GetCount() == 0);
}


void DXMGraph::SetCallbackState(bool DoCallbacks)
{
	POSITION position;

	CallbackState= DoCallbacks;

	for(position = Nodes.GetHeadPosition();
		position != NULL;
		Nodes.GetNext(position))
	{
		DXMNode* tracker= Nodes.GetValueAt(position);

		tracker->SetCallbackState(DoCallbacks); 
	}

	for( position = Adapters.GetHeadPosition();
		position != NULL;
		Adapters.GetNext(position))
	{
		DXMGraphAdapter* adapter= Adapters.GetValueAt(position);

		adapter->SetCallbackState(DoCallbacks); 
	}

	if(CallbackState && !NodeAddedCID)
	{
		NodeAddedCID= MDGMessage::addNodeAddedCallback( DXMGraph::DispatchNodeAdded, kDefaultNodeType, this, NULL); 
	}
	else if(!CallbackState && NodeAddedCID)
	{
		MMessage::removeCallback( NodeAddedCID );
		NodeAddedCID= 0;
	}

	if(CallbackState && !NodeRemovedCID)
	{
		NodeRemovedCID= MDGMessage::addNodeRemovedCallback( DXMGraph::DispatchNodeRemoved, kDefaultNodeType, this, NULL); 
	}
	else if(!CallbackState && NodeRemovedCID)
	{
		MMessage::removeCallback( NodeRemovedCID );
		NodeRemovedCID= 0;
	}

	if(CallbackState && !BeforeSceneOpenCID)
	{
		BeforeSceneOpenCID= MSceneMessage::addCallback(MSceneMessage::kBeforeOpen , DXMGraph::DispatchBeforeSceneChange, this, NULL); 
	}
	else if(!CallbackState && BeforeSceneOpenCID)
	{
		MMessage::removeCallback( BeforeSceneOpenCID );
		BeforeSceneOpenCID= 0;
	}

	if(CallbackState && !BeforeSceneNewCID)
	{
		BeforeSceneNewCID= MSceneMessage::addCallback(MSceneMessage::kBeforeNew , DXMGraph::DispatchBeforeSceneChange, this, NULL); 
	}
	else if(!CallbackState && BeforeSceneNewCID)
	{
		MMessage::removeCallback( BeforeSceneNewCID );
		BeforeSceneNewCID= 0;
	}


	if(CallbackState && !AfterSceneOpenCID)
	{
		AfterSceneOpenCID= MSceneMessage::addCallback(MSceneMessage::kSceneUpdate , DXMGraph::DispatchAfterSceneChange, this, NULL); 
	}
	else if(!CallbackState && AfterSceneOpenCID)
	{
		MMessage::removeCallback( AfterSceneOpenCID );
		AfterSceneOpenCID= 0;
	}


	if(CallbackState && !AfterSceneNewCID)
	{
		AfterSceneNewCID= MSceneMessage::addCallback(MSceneMessage::kAfterNew , DXMGraph::DispatchAfterSceneChange, this, NULL); 
	}
	else if(!CallbackState && AfterSceneNewCID)
	{
		MMessage::removeCallback( AfterSceneNewCID );
		AfterSceneNewCID= 0;
	}


}

DXMNodeAdapter* DXMGraph::FindNodeAdapter( MObject& obj, const CStringA& sig)
{
	DXMNode* node= FindNode(obj);
	if(node)
		return node->FindAdapter( sig );

	return NULL;
}

DXMNode* DXMGraph::FindNode(MObject& obj)
{
	return reinterpret_cast<DXMNode*>( DXMAnchor::GetAnchor(obj, GetNodeAnchorShortName()) );
}

void DXMGraph::RemoveNode(DXMNode* node, bool DeleteIt)
{
	DXCC_ASSERT(node);

	for(POSITION position = Adapters.GetHeadPosition();
		position != NULL;
		Adapters.GetNext(position))
	{
		DXMGraphAdapter* adapter= Adapters.GetValueAt(position);

		if(adapter->IsInterested(node->GetSite()))
			adapter->OnNodeRemoved(node);
	}

	node->Destroy();

	if(DeleteIt)
		delete node;
}
	
void DXMGraph::RefreshNodes() 
{ 
	//register auto deregisters
	RegisterNodes();
};

void DXMGraph::DeregisterNodes()
{
	CManagerLock SceneLock;
	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	for(DXMNodeIterator nodeItr= GetNodeIterator();
		!nodeItr.Done();
		nodeItr.First())//keep grabbing first since we are deleting them...
	{
		DXMNode* node= NULL;
		nodeItr.GetValue(node);

		RemoveNode(node, true);
	}


	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}

void DXMGraph::RegisterNodes()
{
	CManagerLock SceneLock;
	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	DeregisterNodes();

	for(MItDependencyNodes itDepNodes;
		!itDepNodes.isDone();
		itDepNodes.next())
	{
		OnNodeAdded( itDepNodes.item() );
	}

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}


void DXMGraph::OnBeforeSceneChange()
{
	DeregisterNodes();
}

void DXMGraph::DispatchBeforeSceneChange( void* clientData )
{
	DXMGraph* Graph= (DXMGraph*)clientData;
	
	CManagerLock SceneLock;

	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	Graph->OnBeforeSceneChange();

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}



void DXMGraph::OnAfterSceneChange()
{
	MGlobal::executeCommand ("DXCCRebuildScene");

}

void DXMGraph::DispatchAfterSceneChange( void* clientData )
{
	DXMGraph* Graph= (DXMGraph*)clientData;
	
	Graph->OnAfterSceneChange();
}


//STATIC
void DXMGraph::DispatchNodeAdded( MObject& node, void* clientData )
{
	if(!node.hasFn(MFn::kDependencyNode))
		return;

	DXMGraph* Graph= (DXMGraph*)clientData;

	CManagerLock SceneLock;

	if(node.hasFn(MFn::kDependencyNode))

	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	Graph->OnNodeAdded(node);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}

bool DXMGraph::IsNodeSelectable(MObject& testMe)
{
	MString name;
	if(testMe.hasFn(MFn::kDagNode))
	{
		MFnDagNode dagNode(testMe);
		name= dagNode.fullPathName();
	}
	else if(testMe.hasFn(MFn::kDependencyNode))
	{
		MFnDependencyNode depNode(testMe);
		name= depNode.name();
	}
	else
		return false;

#ifdef DEBUG
	LPCSTR nameStr= name.asChar();
#endif

	MSelectionList selectable;
	selectable.add(name);

	MObject obj;
	selectable.getDependNode(0, obj);

	return !obj.isNull();
}

void DXMGraph::OnNodeAdded( MObject& node )
{

	if(!IsNodeSelectable(node))
		return;

	DXMNode* tracker= NULL;
	bool bInterested= false;
	for(POSITION position = Adapters.GetHeadPosition();
		position != NULL;
		Adapters.GetNext(position))
	{
		DXMGraphAdapter* adapter= Adapters.GetValueAt(position);
		
		if(adapter->IsInterested(node))
		{
			if(bInterested == false)
			{
				bInterested= true;

				tracker= CreateNode();

				Nodes.SetAt(tracker, tracker);

				tracker->Initialize(this, node);
			}

			adapter->OnNodeAdded(tracker);
		}
	}
}


//STATIC
void DXMGraph::DispatchNodeRemoved( MObject& node, void* clientData )
{

	CManagerLock SceneLock;

	g_PreviewPipeline.SceneWriteLock(true, SceneLock);


	DXMGraph* Graph= (DXMGraph*)clientData;
	Graph->OnNodeRemoved(node);


	g_PreviewPipeline.SceneWriteUnlock(SceneLock);

}

void DXMGraph::OnNodeRemoved( MObject& node)
{
	DXMNode* tracker= FindNode(node);
	if(tracker)
	{
		RemoveNode(tracker, true);
	}
}

void DXMGraph::AddAdapter(DXMGraphAdapter* adapter) 
{ 
	DXCC_ASSERT(!Adapters.Lookup(adapter->Signature()));
	if( g_DebugExtreme )
	{
		DXCC_DPFA_REPORT("Signature: %s", adapter->Signature());
	}

	Adapters.SetAt(adapter->Signature(), adapter); 

	adapter->Initialize(this);
}


DXMGraphAdapter* DXMGraph::FindAdapter(const CStringA& signature)
{
	DXMGraphAdapter* result= NULL;
	Adapters.Lookup(signature, result);
	return result;
}

void DXMGraph::RemoveAdapter(DXMGraphAdapter* adapter, bool DeleteAdapter)
{
	DXMAdapterMap::CPair* pair= Adapters.Lookup(adapter->Signature());
	
	DXCC_ASSERT(pair);

	adapter->Destroy();

    Adapters.RemoveAt(pair);

	if( g_DebugExtreme )
	{
		DXCC_DPFA_REPORT("Signature: %s", adapter->Signature());
	}

	if(DeleteAdapter)
		delete adapter;

}



