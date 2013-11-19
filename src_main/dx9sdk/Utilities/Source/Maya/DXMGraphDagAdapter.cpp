#include "pch.h"
	
CStringA DXMGraphDagAdapter::MySignature( "DXMGraphDagAdapter" );
	


DXMGraphDagAdapter::DXMGraphDagAdapter() : DXMGraphAdapter() 
{
	ChildAddedCID = 0;
	ChildRemovedCID = 0;
}

DXMGraphDagAdapter::~DXMGraphDagAdapter()
{
	DXCC_ASSERT( ChildAddedCID == 0 );
	DXCC_ASSERT( ChildRemovedCID == 0 );
}

void DXMGraphDagAdapter::Initialize(DXMGraph* owner)
{
	DXMGraphAdapter::Initialize(owner);
}

void DXMGraphDagAdapter::SetCallbackState(bool Activate)
{ 
	CallbackState= Activate;

	if(CallbackState && !ChildAddedCID)
	{
		MItDependencyNodes depItr( MFn::kWorld );
		MDagPath path= MDagPath::getAPathTo( depItr.item() );

		ChildAddedCID= MDagMessage::addChildAddedCallback( path, DXMGraphDagAdapter::DispatchChildAddedToWorld, this, NULL);
	}
	else if(!CallbackState && ChildAddedCID)
	{
		MMessage::removeCallback( ChildAddedCID );
		ChildAddedCID= 0;
	}

	if(CallbackState && !ChildRemovedCID)
	{
		MItDependencyNodes depItr( MFn::kWorld );
		MDagPath path= MDagPath::getAPathTo( depItr.item() );

		ChildRemovedCID= MDagMessage::addChildRemovedCallback ( path, DXMGraphDagAdapter::DispatchChildRemovedToWorld, this, NULL);
	}
	else if(!CallbackState && ChildRemovedCID)
	{
		MMessage::removeCallback( ChildRemovedCID );
		ChildRemovedCID= 0;
	}

	DXMGraphAdapter::SetCallbackState(Activate);
}

void DXMGraphDagAdapter::OnChildAddedToWorld( MDagPath &child)
{
	if( g_DebugBasic )
	{
		MString name= child.fullPathName();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	DXMRoute* route= AddRoute(child);
	if(route)
	{
		for(UINT i= 0; i < child.childCount(); i++)
		{
			MDagPath grandchild= child;
			grandchild.push(child.child(i));

			route->OnChildAdded(grandchild);
		}
	}

}

void DXMGraphDagAdapter::DispatchChildAddedToWorld( MDagPath &child, MDagPath &parent, void * clientData )
{
	DXMGraphDagAdapter* graphDagAdapter= (DXMGraphDagAdapter*)clientData;

	CManagerLock SceneLock;
	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	graphDagAdapter->OnChildAddedToWorld(child);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}

void DXMGraphDagAdapter::OnChildRemovedToWorld( MDagPath &child)
{
	if( g_DebugBasic )
	{
		MString name= child.fullPathName();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	RemoveRoute(child, true);
}

void DXMGraphDagAdapter::DispatchChildRemovedToWorld( MDagPath &child, MDagPath &parent, void * clientData )
{
	DXMGraphDagAdapter* graphDagAdapter= (DXMGraphDagAdapter*)clientData;

	CManagerLock SceneLock;
	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	graphDagAdapter->OnChildRemovedToWorld(child);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}

UINT DXMGraphDagAdapter::Synchronize()
{
	UINT result= DXMGraphAdapter::Synchronize();

	if(result > 0)
		g_PreviewPipeline.AccessRoot()->RecalcWorldsFromLocals();

	return result;
}

bool DXMGraphDagAdapter::IsInterested( MObject& node )
{
	return node.hasFn(MFn::kDagNode) && !node.hasFn(MFn::kWorld);
}

void DXMGraphDagAdapter::OnNodeAdded( DXMNode* node )
{
	DXMNodeDagAdapter* adapter= new DXMNodeDagAdapter();
	AddAdapterOnNode(node, adapter);
}

void DXMGraphDagAdapter::OnNodeRemoved( DXMNode* node )
{
	DXMNodeAdapter* adapter= node->FindAdapter(DXMNodeDagAdapter::MySignature);
	RemoveAdapterOnNode(node, adapter);
}

DXMRoute* DXMGraphDagAdapter::FindRoute(MDagPath& path)
{
	MObject node= path.node();
	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( FindNodeAdapter(node, DXMNodeDagAdapter::MySignature) );
	if(nodeDagAdapter)
	{
		return nodeDagAdapter->FindRoute(path);
	}

	return NULL;
}

DXMRoute* DXMGraphDagAdapter::AddRoute(MDagPath& path)
{
	MObject node= path.node();
	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( FindNodeAdapter(node, DXMNodeDagAdapter::MySignature) );
	if(nodeDagAdapter)
	{
		return nodeDagAdapter->AddRoute(path);
	}

	return NULL;
}

void DXMGraphDagAdapter::RemoveRoute(DXMRoute* route, bool deleteIt)
{
	route->GetNodeDagAdapter()->RemoveRoute(route, deleteIt);
}

void DXMGraphDagAdapter::RemoveRoute(MDagPath& path, bool deleteIt)
{
	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( FindNodeAdapter(path.node(), DXMNodeDagAdapter::MySignature) );
	if(nodeDagAdapter)
		nodeDagAdapter->RemoveRoute(path, deleteIt);
}

