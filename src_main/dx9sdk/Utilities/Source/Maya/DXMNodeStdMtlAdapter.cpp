#include "pch.h"

CStringA DXMNodeStdMtlAdapter::MySignature("DXMNodeStdMtlAdapter");

DXMNodeStdMtlAdapter::DXMNodeStdMtlAdapter() : DXMNodeAdapter() 
{
	material.pTextureFilename = texFile;
	AttributeChangedCID= 0;
}

DXMNodeStdMtlAdapter::~DXMNodeStdMtlAdapter() 
{
	DXCC_ASSERT(AttributeChangedCID == 0);		

	//DXMNodeAdapter::~DXMNodeAdapter();
}

void 
DXMNodeStdMtlAdapter::Initialize(DXMNode* node) 
{  
	DXMNodeAdapter::Initialize(node); 
	
	SyncFlags |= SHADERSYNC_ALL;
	GetOwnerGraphAdapter()->AddSyncRequest(this);
} 

void 
DXMNodeStdMtlAdapter::Destroy() 
{
	DestroyShaderProps();

	DXMNodeAdapter::Destroy();
}
		
	
bool 
DXMNodeStdMtlAdapter::Synchronize()
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

void DXMNodeStdMtlAdapter::SetCallbackState(bool DoCallbacks)
{
	DXMNodeAdapter::SetCallbackState(DoCallbacks);

	if(DoCallbacks && !AttributeChangedCID)
	{
		AttributeChangedCID= MNodeMessage::addAttributeChangedCallback( GetNode()->GetSite(), DXMNodeStdMtlAdapter::DispatchAttributeChanged, this, NULL);
	}
	else if(!DoCallbacks && AttributeChangedCID)
	{
		MMessage::removeCallback( AttributeChangedCID );
		AttributeChangedCID= 0;
	}
}

void DXMNodeStdMtlAdapter::OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug)
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
	

	SyncFlags |= SHADERSYNC_ALL;
	GetOwnerGraphAdapter()->AddSyncRequest(this);

}

//STATIC
void DXMNodeStdMtlAdapter::DispatchAttributeChanged( MNodeMessage::AttributeMessage msg,
	MPlug & plug,
	MPlug & otherPlug,
	void* clientData )
{
	CManagerLock SceneLock;

	g_PreviewPipeline.SceneWriteLock(true, SceneLock);


	DXMNodeStdMtlAdapter* adapter= (DXMNodeStdMtlAdapter*)clientData;
	
	DXCC_ASSERT( adapter->GetNode()->GetSite() == plug.node() );
		
	adapter->OnAttributeChanged(msg,plug,otherPlug);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}

void 
DXMNodeStdMtlAdapter::CreateShaderProps()
{
    DestroyShaderProps();


	MFnDependencyNode depNode( GetNode()->GetSite() );

	float r,g,b,a;
	MPlug cR,cG,cB,cA;

	//AMBIENT

	r= 0.2f;
	g= 0.2f;
	b= 0.2f;

	cR = depNode.findPlug( "ambientColorR");
	cG = depNode.findPlug( "ambientColorG");
	cB = depNode.findPlug( "ambientColorB");

	cR.getValue(r);
	cG.getValue(g);
	cB.getValue(b);

	material.MatD3D.Ambient.r = r;
	material.MatD3D.Ambient.g = g;
	material.MatD3D.Ambient.b = b;
	material.MatD3D.Ambient.a = 1.0f;

	//DIFFUSE
	float diffuse= 1.0f	;
	r= 0.8f;
	g= 0.8f;
	b= 0.8f;

	bool UseOutColor= false;
	MPlug color = depNode.findPlug( "color");
	if(color.isNull())
	{
		color = depNode.findPlug( "outColor");
		UseOutColor= true;
	}

	bool texFound= false;
	MPlugArray dst;
	if( !color.isNull() && color.connectedTo(dst, true, false) )
	{
		MPlug tex= dst[0];

		MObject oNode = tex.node();

		if( oNode.hasFn( MFn::kFileTexture ) )
		{
			MFnDependencyNode texNode( oNode );
			MPlug file= texNode.findPlug( "fileTextureName" );
			MString fileStr;
			file.getValue( fileStr );
			StringCchCopyA( material.pTextureFilename, MAX_PATH+1, fileStr.asChar() );

			texFound= true;
		}
	}

	if( !color.isNull() && !texFound )
	{
		MPlug cDiffuse = depNode.findPlug( "diffuse");

		if(!UseOutColor)
		{
			cR = depNode.findPlug( "colorR");
			cG = depNode.findPlug( "colorG");
			cB = depNode.findPlug( "colorB");
		}
		else
		{
			cR = depNode.findPlug( "outColorR");
			cG = depNode.findPlug( "outColorG");
			cB = depNode.findPlug( "outColorB");
		}

		cDiffuse.getValue(diffuse);
		cR.getValue(r);
		cG.getValue(g);
		cB.getValue(b);
	}

	material.MatD3D.Diffuse.r = diffuse * r;
	material.MatD3D.Diffuse.g = diffuse * g;
	material.MatD3D.Diffuse.b = diffuse * b;
	material.MatD3D.Diffuse.a = 1.0f;


	//EMISSIVE

	r= 0.0f;
	g= 0.0f;
	b= 0.0f;

	bool UseOutIncandescence = false;
	MPlug incandescence = depNode.findPlug( "incandescence");
	if(incandescence.isNull())
	{
		incandescence = depNode.findPlug( "outGlowColor");
		UseOutIncandescence= true;
	}

	if(!UseOutIncandescence)
	{
		cR = depNode.findPlug( "incandescenceR");
		cG = depNode.findPlug( "incandescenceG");
		cB = depNode.findPlug( "incandescenceB");
	}
	else
	{
		cR = depNode.findPlug( "outGlowColorR");
		cG = depNode.findPlug( "outGlowColorG");
		cB = depNode.findPlug( "outGlowColorB");
	}


	cR.getValue(r);
	cG.getValue(g);
	cB.getValue(b);

	material.MatD3D.Emissive.r = r;
	material.MatD3D.Emissive.g = g;
	material.MatD3D.Emissive.b = b;
	material.MatD3D.Emissive.a = 1.0f;


	//SPECULAR

	float reflectivity = 1.0f;
	r= 1.0f;
	g= 1.0f;
	b= 1.0f;

	MPlug cReflect = depNode.findPlug( "reflectivity");
	cR = depNode.findPlug( "specularColorR");
	cG = depNode.findPlug( "specularColorG");
	cB = depNode.findPlug( "specularColorB");

	cReflect.getValue(reflectivity);
	cR.getValue(r);
	cG.getValue(g);
	cB.getValue(b);

	material.MatD3D.Specular.r = reflectivity*r;
	material.MatD3D.Specular.g = reflectivity*g;
	material.MatD3D.Specular.b = reflectivity*b;
	material.MatD3D.Specular.a = 1.0f;


	//ALPHAs

	r= 0.0f;
	g= 0.0f;
	b= 0.0f;
	a= 1.0f;

	bool UseOutTransparency = false;
	MPlug transparency = depNode.findPlug( "transparency");
	if(transparency.isNull())
	{
		color = depNode.findPlug( "outTransparency");
		UseOutTransparency= true;
	}

	if(!UseOutTransparency)
	{
		cR = depNode.findPlug( "transparencyR");
		cG = depNode.findPlug( "transparencyG");
		cB = depNode.findPlug( "transparencyB");
	}
	else
	{
		cR = depNode.findPlug( "outTransparencyR");
		cG = depNode.findPlug( "outTransparencyG");
		cB = depNode.findPlug( "outTransparencyB");
	}

	cA = depNode.findPlug( "materialAlphaGain");

	cR.getValue(r);
	cG.getValue(g);
	cB.getValue(b);
	cA.getValue(a);

	material.MatD3D.Ambient.a = material.MatD3D.Diffuse.a = material.MatD3D.Specular.a = a * ( 1.0f -(r+g+b) );



}

void 
DXMNodeStdMtlAdapter::DestroyShaderProps()
{
    ZeroMemory( &material.MatD3D, sizeof(material.MatD3D) );
	material.pTextureFilename[0] = '\0';
}
