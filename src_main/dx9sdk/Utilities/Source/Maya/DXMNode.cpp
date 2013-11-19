#include "pch.h"

DXMNode::DXMNode()
{
	Graph= NULL;

	CallbackState= false;
	NameChangeCID= 0;
	//AttributeChangedCID= 0;
}

DXMNode::~DXMNode()
{
	DXCC_ASSERT( NameChangeCID == 0);
	//DXMAnchor::~DXMAnchor();
}

DXMNode* DXMNode::FindSiblingNode(MObject& obj)
{
	return Graph->FindNode(obj);
}

void DXMNode::SetCallbackState(bool DoCallbacks)
{
	CallbackState= DoCallbacks;

	for(POSITION position = Adapters.GetHeadPosition();
		position != NULL;
		Adapters.GetNext(position))
	{
		DXMNodeAdapter* adapter= Adapters.GetValueAt(position);

		adapter->SetCallbackState(DoCallbacks); 
	}

	if(DoCallbacks && !NameChangeCID)
	{
		NameChangeCID= MNodeMessage::addNameChangedCallback( GetSite(), DXMNode::DispatchNameChange, this, NULL);
	}
	else if(!DoCallbacks && NameChangeCID)
	{
		MMessage::removeCallback( NameChangeCID );
		NameChangeCID= 0;
	}
/*
	if(DoCallbacks && !AttributeChangedCID)
	{
		AttributeChangedCID= MNodeMessage::addAttributeChangedCallback( GetSite(), DXMNode::DispatchAttributeChanged, this, NULL);
	}
	else if(!DoCallbacks && AttributeChangedCID)
	{
		MMessage::removeCallback( AttributeChangedCID );
		AttributeChangedCID= 0;
	}
*/
}
	
DXMNode::DXMAdapterIterator DXMNode::GetAdapterIterator()
{ 
	return DXMAdapterIterator(&Adapters); 
}

void DXMNode::Initialize(DXMGraph* graph, MObject& obj)
{
	DXCC_ASSERT(graph);
	DXCC_ASSERT(!obj.isNull());

	MFnDependencyNode depNode(obj);
	Graph = graph;

	if( g_DebugBasic )
	{
		MString name= depNode.name();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	DXMAnchor::AddAnchor(obj, Graph->GetNodeAnchorLongName(), Graph->GetNodeAnchorShortName(), this);


	SetCallbackState(Graph->GetCallbackState()); 
}

void DXMNode::Destroy()
{
	MFnDependencyNode depNode(GetSite());

	if( g_DebugBasic )
	{
		MString name= depNode.name();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	SetCallbackState(false);

	for( DXMAdapterIterator adapterItr= GetAdapterIterator();
		!adapterItr.Done();
		adapterItr.First())//keep pulling the first one out and deleting it.
	{
		DXMNodeAdapter* adapter= NULL;
		adapterItr.GetValue(adapter);

		RemoveAdapter(adapter, true);
	}

	DXMAnchor::Destroy();

	Graph->Nodes.RemoveKey(this);
	Graph= NULL;
	
}

//STATIC
void DXMNode::DispatchNameChange( MObject & obj, const MString & prevName, void* clientData )
{

	DXMNode* tracker= (DXMNode*)clientData;

	DXCC_ASSERT(tracker->GetSite() == obj);

	CManagerLock SceneLock;

	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	tracker->OnNameChange(prevName);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}

void DXMNode::OnNameChange( const MString & prevName )
{

	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(GetSite());
		MString newName= depNode.name();
		DXCC_DPFA_REPORT("(Old: %s) (New: %s)", prevName.asChar(), newName.asChar());
	}

	for(POSITION position = Adapters.GetHeadPosition();
		position != NULL;
		Adapters.GetNext(position))
	{
		DXMNodeAdapter* adapter= Adapters.GetValueAt(position);

		adapter->OnNameChange(prevName);
	}
}
/*
//STATIC
void DXMNode::DispatchAttributeChanged( MNodeMessage::AttributeMessage msg,
	MPlug & plug,
	MPlug & otherPlug,
	void* clientData )
{
	DXMNode* tracker= (DXMNode*)clientData;

	tracker->OnAttributeChanged(msg,plug,otherPlug);
}

void DXMNode::OnAttributeChanged( MNodeMessage::AttributeMessage msg,
	MPlug & plug,
	MPlug & otherPlug)
{
	if( g_DebugExtreme )
	{
		MString plugPath= plug.name();
		DXCC_DPFA_REPORT("DGNODE AttrChange (%s)", plugPath.asChar());
	}

	for(POSITION position = Adapters.GetHeadPosition();
		position != NULL;
		Adapters.GetNext(position))
	{
		DXMNodeAdapter* adapter= Adapters.GetValueAt(position);

		adapter->OnAttributeChanged(msg,plug,otherPlug);
	}
}
*/

void DXMNode::AddAdapter(DXMNodeAdapter* adapter) 
{ 
	DXCC_ASSERT( !Adapters.Lookup(adapter->Signature()));

	if( g_DebugExtreme )
	{
		MFnDependencyNode depNode(GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("(This: %s) (Signature: %s)", name.asChar(), adapter->Signature());
	}

	Adapters.SetAt(adapter->Signature(), adapter); 

	adapter->Initialize(this);
}

DXMNodeAdapter* DXMNode::FindAdapter(const CStringA& signature)
{
	DXMNodeAdapter* result= NULL;
	Adapters.Lookup(signature, result);
	return result;
}

void DXMNode::RemoveAdapter(DXMNodeAdapter* adapter, bool DeleteAdapter)
{
	DXCC_ASSERT(adapter);

	if( g_DebugExtreme )
	{
		MFnDependencyNode depNode(GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("(This: %s) (Signature: %s)", name.asChar(), adapter->Signature());
	}

	DXMAdapterMap::CPair* pair= Adapters.Lookup(adapter->Signature());

	DXCC_ASSERT(pair);

	adapter->Destroy();

	Adapters.RemoveAt(pair);

	if(DeleteAdapter)
		delete adapter;

}



