#include "pch.h"

CStringA DXMNodeLayerAdapter::MySignature( "DXMNodeLayerAdapter" );



DXMNodeLayerAdapter::DXMNodeLayerAdapter() : DXMNodeAdapter() 
{
	AttributeChangedCID= 0;
	Invisible= true;

}

DXMNodeLayerAdapter::~DXMNodeLayerAdapter() 
{
	DXCC_ASSERT(AttributeChangedCID == 0);		
}

bool DXMNodeLayerAdapter::Synchronize()
{
	if(SyncFlags & LAYERSYNC_VISIBILITY)
	{
		SyncVisibility();
	}

	return SyncFlags == 0;
}


void DXMNodeLayerAdapter::Initialize(DXMNode* node)
{
	DXMNodeAdapter::Initialize(node);

	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(node->GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	SyncFlags= LAYERSYNC_VISIBILITY;
	GetOwnerGraphAdapter()->AddSyncRequest(this);

}


void DXMNodeLayerAdapter::Destroy()
{

	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(GetNode()->GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	DXMNodeAdapter::Destroy();
}

void DXMNodeLayerAdapter::SetCallbackState(bool DoCallbacks)
{
	DXMNodeAdapter::SetCallbackState(DoCallbacks);

	if(DoCallbacks && !AttributeChangedCID)
	{
		AttributeChangedCID= MNodeMessage::addAttributeChangedCallback( GetNode()->GetSite(), DXMNodeLayerAdapter::DispatchAttributeChanged, this, NULL);
	}
	else if(!DoCallbacks && AttributeChangedCID)
	{
		MMessage::removeCallback( AttributeChangedCID );
		AttributeChangedCID= 0;
	}
}

void DXMNodeLayerAdapter::OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug)
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



	static MString enabledStr("enabled");
	static MString displayTypeStr("displayType");
	static MString visibilityStr("visibility");


	MObject oAttribute= plug.attribute();
	MFnAttribute attribute(oAttribute);
	MString attributeStr= attribute.name();

	if( attributeStr == enabledStr  
	|| attributeStr == displayTypeStr  
	|| attributeStr == visibilityStr )
	{
		//SyncVisibility();
		SyncFlags |= LAYERSYNC_VISIBILITY;
		GetOwnerGraphAdapter()->AddSyncRequest(this);
	}
}


void DXMNodeLayerAdapter::SyncVisibility()
{
	DXMGraphDagAdapter* graphDagAdapter= reinterpret_cast<DXMGraphDagAdapter*>( GetGraph()->FindAdapter(DXMGraphDagAdapter::MySignature));

	MFnDependencyNode depNode(GetNode()->GetSite());

	MString layer = depNode.name();
	
	MStringArray members;
	MGlobal::executeCommand( MString("editDisplayLayerMembers -q -fullNames ") + layer , members );

	for(UINT i= 0; i < members.length(); i++)
	{
		MDagPath path;
		MSelectionList selection;

		LPCSTR memberStr= members[i].asChar();

		DXCHECK_MSTATUS( selection.add(members[i]) );
		DXCHECK_MSTATUS( selection.getDagPath(0, path) );

		DXMRoute* route= graphDagAdapter->FindRoute(path);
		if(route)
		{
			DXMNodeDagAdapter* nodeDagAdapter= route->GetNodeDagAdapter();
			
			nodeDagAdapter->SyncFlags |= DXMNodeDagAdapter::DAGSYNC_VISIBILITY_OF_NODE | DXMNodeDagAdapter::DAGSYNC_VISIBILITY_OF_ROUTES;
			nodeDagAdapter->GetOwnerGraphAdapter()->AddSyncRequest(nodeDagAdapter);
		}
	}

	SyncFlags = SyncFlags & ~LAYERSYNC_VISIBILITY;
}


//STATIC
void DXMNodeLayerAdapter::DispatchAttributeChanged( MNodeMessage::AttributeMessage msg,
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


