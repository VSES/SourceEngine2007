#include "pch.h"

CStringA DXMNodeDagAdapter::MySignature( "DXMNodeDagAdapter" );



DXMNodeDagAdapter::DXMNodeDagAdapter() : DXMNodeAdapter() 
{
	AttributeChangedCID= 0;
	Invisible= true;
	D3DXMatrixIdentity( &LocalMatrix );

}

DXMNodeDagAdapter::~DXMNodeDagAdapter() 
{
	DXCC_ASSERT(AttributeChangedCID == 0);		
	DXCC_ASSERT(Routes.GetCount() == 0);		

	//DXMNodeAdapter::~DXMNodeAdapter();
}

bool DXMNodeDagAdapter::Synchronize()
{

	bool bSyncMesh= false;
/*
	if(SyncFlags & DAGSYNC_HIERARCHY)
	{
		SyncHierarchy();
		bSyncMesh= true;
	}
*/
	if(SyncFlags & DAGSYNC_TRANSFORM)
		SyncTransform();
	
	if(SyncFlags & DAGSYNC_VISIBILITY_OF_NODE)
	{
		SyncVisibilityOfNode();
		bSyncMesh= true;
	}

	if((SyncFlags & DAGSYNC_VISIBILITY_OF_ROUTES) ||  (SyncFlags & DAGSYNC_VISIBILITY_OF_NODE))
	{
		SyncVisibilityOfRoutes();
		bSyncMesh= true;
	}

	if(bSyncMesh)
		SyncMesh();

	return SyncFlags == 0;
}



DXMRoute* DXMNodeDagAdapter::CreateRoute()
{ 
	return new DXMRoute(); 
}


void DXMNodeDagAdapter::Initialize(DXMNode* node)
{
	DXMNodeAdapter::Initialize(node);

	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(node->GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	MDagPathArray dagPathArray;
	MFnDagNode dagNode(node->GetSite());
	dagNode.getAllPaths(dagPathArray);
	for(UINT index= 0; index < dagPathArray.length(); index++)
	{
		AddRoute(dagPathArray[index]);
	}

	SyncFlags= DAGSYNC_TRANSFORM | DAGSYNC_VISIBILITY_OF_NODE | DAGSYNC_VISIBILITY_OF_ROUTES;
	GetOwnerGraphAdapter()->AddSyncRequest(this);

}

DXMNodeDagAdapter::DXMRouteIterator DXMNodeDagAdapter::GetRouteIterator()
{ 
	return DXMRouteIterator(&Routes); 
}

void DXMNodeDagAdapter::Destroy()
{

	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(GetNode()->GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	for(POSITION position= Routes.GetHeadPosition();
		position != NULL;)
	{
		DXMRoute* route= Routes.GetValueAt(position);
		Routes.GetNext(position);

		RemoveRoute(route, true);
	}

	DXMNodeAdapter::Destroy();
}

void DXMNodeDagAdapter::SetCallbackState(bool DoCallbacks)
{
	DXMNodeAdapter::SetCallbackState(DoCallbacks);

	if(DoCallbacks && !AttributeChangedCID)
	{
		AttributeChangedCID= MNodeMessage::addAttributeChangedCallback( GetNode()->GetSite(), DXMNodeDagAdapter::DispatchAttributeChanged, this, NULL);
	}
	else if(!DoCallbacks && AttributeChangedCID)
	{
		MMessage::removeCallback( AttributeChangedCID );
		AttributeChangedCID= 0;
	}
}

DXMRoute* DXMNodeDagAdapter::AddRoute(MDagPath& path)
{

	if( g_DebugExtreme )
	{
		MString name= MFnDependencyNode(GetNode()->GetSite()).name();
		MString pathName= path.fullPathName();
		DXCC_DPFA_REPORT( "(This: %s) (Path: %s)", name.asChar(), pathName.asChar() );
	}

	if(FindRoute(path))
		return NULL;

	DXCC_ASSERT( path.node() == GetNode()->GetSite() );

	DXMRoute* route= CreateRoute();
	Routes.SetAt(route, route);

	route->Initialize(this, path);

	return route;
}

DXMRoute* DXMNodeDagAdapter::FindRoute(MDagPath& path)
{
	DXCC_ASSERT( path.node() == GetNode()->GetSite() );

	for(POSITION position= Routes.GetHeadPosition();
		position != NULL;
		Routes.GetNext(position))
	{
		DXMRoute* route= Routes.GetValueAt(position);
		if(route->GetPath() == path)
			return route;
	}

	return NULL;
}

void DXMNodeDagAdapter::RemoveRoute(DXMRoute* route, bool DeleteIt)
{
	if(Routes.RemoveKey(route))
	{

		if( g_DebugExtreme )
		{
			MString name= route->GetPath().fullPathName();
			DXCC_DPFA_REPORT("%s", name.asChar());
		}
	
		route->Destroy();

		if(DeleteIt)
			delete route;
	}
}

void DXMNodeDagAdapter::RemoveRoute(MDagPath& path, bool deleteIt)
{
	DXMRoute* route= FindRoute(path);
	if(route)
		RemoveRoute(route, deleteIt);
}

void DXMNodeDagAdapter::OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug)
{
	msg= (MNodeMessage::AttributeMessage)(msg & ~MNodeMessage::kIncomingDirection);
	msg= (MNodeMessage::AttributeMessage)(msg & ~MNodeMessage::kOtherPlugSet);

	
	if( g_DebugExtreme )
	{
		MString msgString;
		MString plugName= plug.name();

		switch(msg)
		{
		// a connection has been made to an attribute of this node
		case MNodeMessage::kConnectionMade:
			msgString= "kConnectionMade";
			break;
		case MNodeMessage::kConnectionBroken:
			msgString= "kConnectionBroken";
			break;
		case MNodeMessage::kAttributeEval:
			msgString= "kAttributeEval";
			break;
		case MNodeMessage::kAttributeSet:
			msgString= "kAttributeSet";
			break;
		case MNodeMessage::kAttributeLocked:
			msgString= "kAttributeLocked";
			break;
		case MNodeMessage::kAttributeUnlocked:
			msgString= "kAttributeUnlocked";
			break;
		case MNodeMessage::kAttributeAdded:
			msgString= "kAttributeAdded";
			break;
		case MNodeMessage::kAttributeRemoved:
			msgString= "kAttributeRemoved";
			break;
		case MNodeMessage::kAttributeRenamed:
			msgString= "kAttributeRenamed";
			break;
		case MNodeMessage::kAttributeKeyable:
			msgString= "kAttributeKeyable";
			break;
		case MNodeMessage::kAttributeUnkeyable:
			msgString= "kAttributeUnkeyable";
			break;
		case MNodeMessage::kAttributeArrayAdded:
			msgString= "kAttributeArrayAdded";
			break;
		case MNodeMessage::kAttributeArrayRemoved:
			msgString= "kAttributeArrayRemoved";
			break;
		default:
			DXCC_ASSERT(FALSE);
			break;
		};

		DXCC_DPFA_REPORT( "(Plug: %s) (MSG: %s)", plugName.asChar(), msgString.asChar() );
	}

	if( msg != MNodeMessage::kAttributeSet )
		return;

	//Get the highest plug so we dont have to compare as much
	while( !plug.parent().isNull() )
		plug= plug.parent();

	MObject attribute= plug.attribute();

	if( attribute == MPxTransform::translate  
	|| attribute == MPxTransform::rotate  
	|| attribute == MPxTransform::scale )
	{
		//SyncTransform();
		SyncFlags |= DAGSYNC_TRANSFORM;
		GetOwnerGraphAdapter()->AddSyncRequest(this);
	}
	else if(attribute == MPxTransform::visibility
	|| attribute == MPxTransform::intermediateObject
	|| attribute == MPxTransform::isTemplated  
	|| attribute == MPxTransform::ghosting
	|| attribute == MPxTransform::drawOverride)
	{
		//SyncVisibility();
		SyncFlags |= DAGSYNC_VISIBILITY_OF_NODE | DAGSYNC_VISIBILITY_OF_ROUTES;
		GetOwnerGraphAdapter()->AddSyncRequest(this);
	}
	//we serperate this from the other SyncTransform because these are less commonly edited.
	else if( attribute == MPxTransform::rotateOrder  
	|| attribute == MPxTransform::shear  
	|| attribute == MPxTransform::rotatePivot  
	|| attribute == MPxTransform::rotatePivotTranslate   
	|| attribute == MPxTransform::scalePivot  
	|| attribute == MPxTransform::scalePivotTranslate  
	|| attribute == MPxTransform::rotateAxis   
	|| attribute == MPxTransform::rotateQuaternion)
	{
		//SyncTransform();
		SyncFlags |= DAGSYNC_TRANSFORM;
		GetOwnerGraphAdapter()->AddSyncRequest(this);
	}
 

}

void DXMNodeDagAdapter::SyncTransform()
{
	MFnTransform fnTransform(GetNode()->GetSite());
	MMatrix matrix= fnTransform.transformationMatrix(); 
	ConvertLocalMatrix(LocalMatrix, matrix);

	for(DXMNodeDagAdapter::DXMRouteIterator routeItr= GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route= NULL;
		routeItr.GetValue(route);

		route->SyncTransform();
	}

	SyncFlags = SyncFlags & ~DAGSYNC_TRANSFORM;

}




void DXMNodeDagAdapter::SyncVisibilityOfNode()
{
	MObject obj= GetNode()->GetSite();

	bool visVal;
	MPlug visPlug(obj, MPxTransform::visibility);
	visPlug.getValue(visVal);

	bool intermediateVal;
	MPlug intermediatePlug(obj, MPxTransform::intermediateObject);
	intermediatePlug.getValue(intermediateVal);

	bool temlateVal;
	MPlug temlatePlug(obj, MPxTransform::isTemplated);
	temlatePlug.getValue(temlateVal);


	bool ghostPlug;
	MPlug ghostVal(obj, MPxTransform::ghosting);
	ghostVal.getValue(ghostPlug);

	MPlug drawOverridePlug(obj, MPxTransform::drawOverride);

	bool overrideEnabledVal;
	MPlug overrideEnabledPlug= drawOverridePlug.child(MPxTransform::overrideEnabled);
	overrideEnabledPlug.getValue(overrideEnabledVal);

	int overrideDisplayTypeVal;
	MPlug overrideDisplayTypePlug= drawOverridePlug.child(MPxTransform::overrideDisplayType);
	overrideDisplayTypePlug.getValue(overrideDisplayTypeVal);

	bool overrideVisibilityVal;
	MPlug overrideVisibilityPlug= drawOverridePlug.child(MPxTransform::overrideVisibility );
	overrideVisibilityPlug.getValue(overrideVisibilityVal);


	if(overrideEnabledVal)
		Invisible = !(overrideDisplayTypeVal == 0 && overrideVisibilityVal == true);
	else
		Invisible = !( visVal && !intermediateVal && !temlateVal && !ghostPlug );
	

	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(GetNode()->GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("(This: %s) (Invisible: %d)", name.asChar(), Invisible);
	}

	SyncFlags = SyncFlags & ~DAGSYNC_VISIBILITY_OF_NODE;
}



void DXMNodeDagAdapter::SyncVisibilityOfRoutes()
{
	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(GetNode()->GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT(name.asChar());
	}

	for(DXMNodeDagAdapter::DXMRouteIterator routeItr= GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route;
		routeItr.GetValue(route);

		route->SyncVisibility();
	}


	SyncFlags = SyncFlags & ~DAGSYNC_VISIBILITY_OF_ROUTES;
}
/*
void DXMNodeDagAdapter::SyncHierarchy()
{
	for(DXMNodeDagAdapter::DXMRouteIterator routeItr= GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route;
		routeItr.GetValue(route);

		route->SyncHierarchy();
	}

	SyncFlags = SyncFlags & ~DAGSYNC_HIERARCHY;
}
*/
void DXMNodeDagAdapter::SyncMesh()
{

	DXMNodeMeshAdapter* meshAdapter= reinterpret_cast<DXMNodeMeshAdapter*>( GetNode()->FindAdapter( DXMNodeMeshAdapter::MySignature ) );
	if(meshAdapter)
	{
		meshAdapter->UpdateVisibility();
	}

	for(DXMNodeDagAdapter::DXMRouteIterator routeItr= GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route;
		routeItr.GetValue(route);

		route->SyncMesh();
	}
}


//STATIC
void DXMNodeDagAdapter::DispatchAttributeChanged( MNodeMessage::AttributeMessage msg,
	MPlug & plug,
	MPlug & otherPlug,
	void* clientData )
{


	DXMNodeDagAdapter* adapter= (DXMNodeDagAdapter*)clientData;

	CManagerLock SceneLock;

	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	adapter->OnAttributeChanged(msg,plug,otherPlug);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}


void DXMNodeDagAdapter::AddMemberToRoutes(LPUNKNOWN pUnk)
{
	for( DXMRoute::DXMRouteIterator routeItr= GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route= NULL;
		routeItr.GetValue(route);
		
		route->AddMember(pUnk);
	}
}

void DXMNodeDagAdapter::RemoveMemberFromRoutes(LPUNKNOWN pUnk)
{
	for( DXMRoute::DXMRouteIterator routeItr= GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route= NULL;
		routeItr.GetValue(route);
		
		route->RemoveMember(pUnk);
	}
}

