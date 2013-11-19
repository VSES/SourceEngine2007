#include "pch.h"

CStringA DXMNodeShaderAdapter::MySignature("DXMNodeShaderAdapter");

DXMNodeShaderAdapter::DXMNodeShaderAdapter() : DXMNodeAdapter() 
{
	Resource= NULL;
	Shader= NULL;
    Properties= NULL;

	AttributeChangedCID= 0;
}

DXMNodeShaderAdapter::~DXMNodeShaderAdapter() 
{
	DXCC_ASSERT(AttributeChangedCID == 0);		

	//DXMNodeAdapter::~DXMNodeAdapter();
}

void 
DXMNodeShaderAdapter::Initialize(DXMNode* node) 
{  
	DXMNodeAdapter::Initialize(node); 
	
	SyncFlags |= SHADERSYNC_ALL;
	GetOwnerGraphAdapter()->AddSyncRequest(this);
} 

void 
DXMNodeShaderAdapter::Destroy() 
{
	DestroyShaderProps();

	DXMNodeAdapter::Destroy();
}
		
	
bool 
DXMNodeShaderAdapter::Synchronize()
{ 
	CreateShaderProps(); 
	SyncFlags = SyncFlags & ~SHADERSYNC_ALL;


	for(MItDependencyGraph FindEngine(GetNode()->GetSite(), MFn::kShadingEngine, MItDependencyGraph::kDownstream );
		!FindEngine.isDone();
		FindEngine.next())
	{
		MObject dgShadingEngine= FindEngine.thisNode();

		if(!dgShadingEngine.isNull())
		{
			for(MItDependencyGraph FindMesh(dgShadingEngine, MFn::kMesh, MItDependencyGraph::kUpstream);
				!FindMesh.isDone();
				FindMesh.next())
			{
				MObject dgMesh= FindMesh.thisNode();
				if(!dgMesh.isNull())
				{
					DXMNode* meshNode= GetGraph()->FindNode(dgMesh);
					if(meshNode)
					{
						DXMNodeMeshAdapter* meshAdapter= reinterpret_cast<DXMNodeMeshAdapter*>( meshNode->FindAdapter( DXMNodeMeshAdapter::MySignature ) );
						if(meshAdapter)
						{
							meshAdapter->SyncFlags |= DXMNodeMeshAdapter::MESHSYNC_ALL;
							meshAdapter->GetOwnerGraphAdapter()->AddSyncRequest(meshAdapter);
						}
					}

				}
			}
		}
	}

	return true; 
}

void DXMNodeShaderAdapter::SetCallbackState(bool DoCallbacks)
{
	DXMNodeAdapter::SetCallbackState(DoCallbacks);

	if(DoCallbacks && !AttributeChangedCID)
	{
		AttributeChangedCID= MNodeMessage::addAttributeChangedCallback( GetNode()->GetSite(), DXMNodeShaderAdapter::DispatchAttributeChanged, this, NULL);
	}
	else if(!DoCallbacks && AttributeChangedCID)
	{
		MMessage::removeCallback( AttributeChangedCID );
		AttributeChangedCID= 0;
	}
}

void DXMNodeShaderAdapter::OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug)
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
	

/*
	MObject attribute= plug.attribute();
	if( attribute == DirectXShader::aLoading )
	{
		bool loading;
		plug.getValue(loading);
		if(!loading)
		{
			SyncFlags |= SHADERSYNC_ALL;
			GetOwnerGraphAdapter()->AddSyncRequest(this);

		}
	}
*/
	if(msg == MNodeMessage::kAttributeSet)
	{
		SyncFlags |= SHADERSYNC_ALL;
		GetOwnerGraphAdapter()->AddSyncRequest(this);
	}
}

//STATIC
void DXMNodeShaderAdapter::DispatchAttributeChanged( MNodeMessage::AttributeMessage msg,
	MPlug & plug,
	MPlug & otherPlug,
	void* clientData )
{
	CManagerLock SceneLock;

	g_PreviewPipeline.SceneWriteLock(true, SceneLock);


	DXMNodeShaderAdapter* adapter= (DXMNodeShaderAdapter*)clientData;
	
	DXCC_ASSERT( adapter->GetNode()->GetSite() == plug.node() );
		
	adapter->OnAttributeChanged(msg,plug,otherPlug);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}


void 
DXMNodeShaderAdapter::CreateShaderProps()
{
    DestroyShaderProps();

	if(DXCC_SUCCEEDED( DirectXShader::CreateShaderProperties(GetNode()->GetSite(), true, &Resource, &Shader) ) )
	{
		Shader->GetProperties(&Properties);
	}
}

void 
DXMNodeShaderAdapter::DestroyShaderProps()
{

	if(Resource)
	{
		g_PreviewPipeline.AccessManager()->RemoveResource(Resource->GetHandle());
		DXCC_RELEASE(Resource);
	}
    DXCC_RELEASE(Shader);
    DXCC_RELEASE(Properties);
}

