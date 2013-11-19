#include "pch.h"

CStringA DXMNodeMeshAdapter::MySignature( "DXMNodeMeshAdapter" );

MObject DXMNodeMeshAdapter::MPxMesh_outMesh = MObject::kNullObj;

CAtlArray<DWORD>				DXMNodeMeshAdapter::g_D3DPointReps;
CAtlArray<VertexD3DToMayaType>	DXMNodeMeshAdapter::g_VertexD3DToMaya;
CAtlArray<VertexMayaToD3DType>	DXMNodeMeshAdapter::g_VertexMayaToD3D;


DXMNodeMeshAdapter::~DXMNodeMeshAdapter() 
{
	DXCC_ASSERT(Resource == NULL);
	DXCC_ASSERT(Mesh == NULL);		
	DXCC_ASSERT(AttributeChangedCID == 0);		

	//DXMNodeAdapter::~DXMNodeAdapter();
}

DXMNodeMeshAdapter::DXMNodeMeshAdapter() : DXMNodeAdapter() 
{
	Mesh= NULL;
	Resource= NULL;
	AnyRoutesVisible= false;
	SyncFlags= 0;
	AttributeChangedCID= 0;
}

void DXMNodeMeshAdapter::UpdateVisibility()
{
	DXMNodeDagAdapter* nodeDagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( GetNode()->FindAdapter(DXMNodeDagAdapter::MySignature) );
	if(!nodeDagAdapter)
		return;

	bool newVis= false;
	for(DXMNodeDagAdapter::DXMRouteIterator routeItr= nodeDagAdapter->GetRouteIterator();
		!routeItr.Done();
		routeItr.Next())
	{
		DXMRoute* route= NULL;
		routeItr.GetValue(route);

		if(route->IsVisible())
		{
			newVis= true;
			break;
		}
	}

	if(AnyRoutesVisible != newVis)
	{
		AnyRoutesVisible= newVis;

		if(AnyRoutesVisible)
		{
			SyncFlags |= MESHSYNC_ALL;
			GetOwnerGraphAdapter()->AddSyncRequest(this);
		}
		else
		{
			DestroyMesh();
		}

		if( g_DebugBasic )
		{
			MFnDependencyNode depNode(GetNode()->GetSite());
			MString name= depNode.name();
			DXCC_DPFA_REPORT("(This: %s) (Visible: %d)", name.asChar(), AnyRoutesVisible);
		}
	}
}

void DXMNodeMeshAdapter::Initialize(DXMNode* node)
{
	DXMNodeAdapter::Initialize(node);

	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(node->GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	if(MPxMesh_outMesh.isNull())
	{
		MFnDependencyNode depNode(node->GetSite());
		MPxMesh_outMesh= depNode.attribute("outMesh");
		DXCC_ASSERT(!MPxMesh_outMesh.isNull());
	}

	UpdateVisibility();

	SyncFlags |= MESHSYNC_ALL;
	GetOwnerGraphAdapter()->AddSyncRequest(this);

}

void DXMNodeMeshAdapter::Destroy()
{

	if( g_DebugBasic )
	{
		MFnDependencyNode depNode(GetNode()->GetSite());
		MString name= depNode.name();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}

	DestroyMesh();

	DXMNodeAdapter::Destroy();
}

void DXMNodeMeshAdapter::DestroyMesh()
{
	DXMNodeDagAdapter* dagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( GetNode()->FindAdapter(DXMNodeDagAdapter::MySignature) );
	if(dagAdapter)
	{
		dagAdapter->RemoveMemberFromRoutes(Mesh);
	}

	if(Resource)
	{
		g_PreviewPipeline.AccessManager()->RemoveResource(Resource->GetHandle());
		DXCC_RELEASE(Resource);
	}
	DXCC_RELEASE(Mesh);
}



void DXMNodeMeshAdapter::SetCallbackState(bool DoCallbacks)
{
	DXMNodeAdapter::SetCallbackState(DoCallbacks);

	if(DoCallbacks && !AttributeChangedCID)
	{
		AttributeChangedCID= MNodeMessage::addAttributeChangedCallback( GetNode()->GetSite(), DXMNodeMeshAdapter::DispatchAttributeChanged, this, NULL);
	}
	else if(!DoCallbacks && AttributeChangedCID)
	{
		MMessage::removeCallback( AttributeChangedCID );
		AttributeChangedCID= 0;
	}
}


void DXMNodeMeshAdapter::OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug)
{
	msg= (MNodeMessage::AttributeMessage)(msg & ~MNodeMessage::kIncomingDirection);
	msg= (MNodeMessage::AttributeMessage)(msg & ~MNodeMessage::kOtherPlugSet);

	if(g_DebugExtreme)
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

	//if we are not visible we are not updating
	if(!AnyRoutesVisible)
		return;

	//Get the highest plug so we dont have to compare as much
	MPlug rootPlug= plug;
	while( !rootPlug.parent().isNull() )
		rootPlug= rootPlug.parent();


	if( plug.attribute() == MPxMesh_outMesh 
		|| (rootPlug.attribute() == MPxTransform::instObjGroups 
			&& (msg == MNodeMessage::kAttributeSet 
				|| msg == MNodeMessage::kAttributeArrayAdded
				|| msg == MNodeMessage::kAttributeArrayRemoved
				|| msg == MNodeMessage::kConnectionMade
				|| msg == MNodeMessage::kConnectionBroken
				|| msg == MNodeMessage::kAttributeAdded
				|| msg == MNodeMessage::kAttributeRemoved)
			)
		)
	{
		SyncFlags |= MESHSYNC_ALL;
		GetOwnerGraphAdapter()->AddSyncRequest(this);
	}
}

bool DXMNodeMeshAdapter::Synchronize()
{
	if(SyncFlags & MESHSYNC_ALL)
		SyncMesh();
	
	return SyncFlags == 0; 
}


void DXMNodeMeshAdapter::SyncMesh()
{
	DestroyMesh();
	ExtractMesh();

	DXMNodeDagAdapter* dagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( GetNode()->FindAdapter(DXMNodeDagAdapter::MySignature) );
	if(dagAdapter)
	{
		dagAdapter->AddMemberToRoutes(Mesh);
	}

	SyncFlags = SyncFlags & ~MESHSYNC_ALL;
}

//STATIC
void DXMNodeMeshAdapter::DispatchAttributeChanged( MNodeMessage::AttributeMessage msg,
	MPlug & plug,
	MPlug & otherPlug,
	void* clientData )
{


	DXMNodeMeshAdapter* adapter= (DXMNodeMeshAdapter*)clientData;

	CManagerLock SceneLock;

	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	adapter->OnAttributeChanged(msg,plug,otherPlug);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}


bool	
DXMNodeMeshAdapter::FindSkinMesh(const MFnMesh& mesh, MFnSkinCluster& skinCluster, MDagPathArray& boneArray)
{
	HRESULT hr = S_OK;
	MStatus stat = MS::kSuccess;

	bool bFoundSkin= false;

	MPlug inPlug=mesh.findPlug("inMesh", &stat);
	DXCHECK_MSTATUS( stat );

	MItDependencyGraph SkinFinder(	inPlug, 
									MFn::kSkinClusterFilter, 
									MItDependencyGraph::kUpstream,
									//MItDependencyGraph::kBreadthFirst,
									//MItDependencyGraph::kNodeLevel, 
									MItDependencyGraph::kDepthFirst,
									MItDependencyGraph::kPlugLevel, 
									&stat);
	if(DXMSUCCESS(stat))
	{
		for(; !SkinFinder.isDone(); SkinFinder.next() )
		{
			skinCluster.setObject(SkinFinder.thisNode());

			skinCluster.influenceObjects( boneArray, &stat);
			DXCHECK_MSTATUS(stat);

			return true;
		}
	}

	return false;
}

HRESULT	
DXMNodeMeshAdapter::ExtractMesh()
{
	HRESULT hr= S_OK;
	MStatus stat = MS::kSuccess;
	
	

	MFnSkinCluster skinCluster;
	MDagPathArray boneArray;


	MObjectArray Shaders;
	MIntArray Attributes; 

	MFnMesh fnMesh;


	stat = fnMesh.setObject(GetNode()->GetSite());//dagTag.node());
	DXCHECK_MSTATUS(stat);

	//AND ATTRIBUTE BUFFER

	UINT instNum= 0;//dagTag.instanceNumber();
	stat= fnMesh.getConnectedShaders(instNum, Shaders, Attributes);
	DXCHECK_MSTATUS(stat);

	UINT NumShaders= Shaders.length();
	UINT NumAttributes= Attributes.length();

	bool AddDefaults = false;
	for(UINT iA = 0; iA < Attributes.length(); iA++)
	{
		int attrib = Attributes[iA];
		if(attrib < 0 || attrib >= Attributes.length() )
		{
			AddDefaults = true;
			Attributes[iA] = NumShaders;
		}
	}

	FindSkinMesh(fnMesh, skinCluster, boneArray);


	GatherMesh(fnMesh, Attributes, skinCluster, boneArray.length() );


	if(!skinCluster.object().isNull())
	{
		hr= GatherSkin(fnMesh, skinCluster, boneArray);
		DXCC_CHECK_HRESULT(hr);

		hr= Mesh->DoSkinning();
		DXCC_CHECK_HRESULT(hr);
	}


	hr= GatherMaterials(Shaders, AddDefaults, Mesh);
	DXCC_CHECK_HRESULT(hr);

	D3DXWELDEPSILONS epsilon;
	ZeroMemory(&epsilon, sizeof(epsilon));
	epsilon.Binormal = 1.0e-6f;
	epsilon.Tangent = 1.0e-6f;

	hr= Mesh->RemoveDuplicateVertices(&epsilon, NULL, NULL);
	DXCC_CHECK_HRESULT(hr);

	hr= Mesh->Optimize( D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT, NULL, NULL);
	DXCC_CHECK_HRESULT(hr);

	g_PreviewPipeline.TriggerViewerEvents.OnMeshChange(Mesh);


	

	return hr;
}

void	
DXMNodeMeshAdapter::GatherMesh(MFnMesh& mesh, MIntArray& ShaderAttributes, MFnSkinCluster& skinCluster, UINT boneCount)
{
	HRESULT hr = S_OK;
	MStatus stat = MS::kSuccess;
	MPlug inPlug;	
	MObject inObj;

	if(!skinCluster.object().isNull())
	{
		MFnMesh preSkinMesh;
		inPlug= skinCluster.findPlug( "input" , &stat);

		UINT logicalIndex= skinCluster.indexForOutputShape(mesh.object());
		inPlug= inPlug.elementByLogicalIndex( logicalIndex );
		inPlug= inPlug.child( 0 );
	}
	else
	{
		inPlug= mesh.findPlug( "outMesh" , &stat);
	}
	
	inPlug.getValue( inObj );

	DXCC_ASSERT(!inObj.isNull());
	GatherMesh_Internal(inObj, ShaderAttributes, boneCount);
}


void
DXMNodeMeshAdapter::GatherMesh_Internal(MObject& obj, MIntArray& ShaderAttributes , UINT boneCount)
{
	HRESULT hr= S_OK;
	MStatus stat = MS::kSuccess;
	LPDXCCVERTEXBUNDLER dxVertexBundler= NULL;

	

	MFnMesh fnMesh(obj);
	
	//DXCC_ASSERT(!fnMesh.object().isNull());

	MItMeshPolygon itMeshPoly(MObject::kNullObj);
	MItMeshVertex itMeshVert(MObject::kNullObj);

	DCCMeshInfo MeshInfo;
	UINT iVertexD3D=0;
	UINT iTriangleD3D= 0;
	FLOAT fDeterminant=1;
	DXCCFLOAT4 f4;

	hr= CreateMesh(obj, MeshInfo);
	DXCC_CHECK_HRESULT(hr);

	if(boneCount > 0)
	{
		hr= Mesh->CreateSkin(boneCount);
		DXCC_CHECK_HRESULT(hr);
	}

	hr= Mesh->LockFaces();
	DXCC_CHECK_HRESULT(hr);

	hr= Mesh->GetVertexBundler(&dxVertexBundler);
	DXCC_CHECK_HRESULT(hr);

	hr= dxVertexBundler->LockAll();
	if(DXCC_FAILED(hr))
		DXCC_GOTO_EXIT(e_Exit, TRUE);
		
	stat = itMeshPoly.reset(obj);
	DXCHECK_MSTATUS(stat);


	stat = itMeshVert.reset(obj);
	DXCHECK_MSTATUS(stat);


	//PolygonLoop
	iVertexD3D=0;
	iTriangleD3D=0;
	for (int iPolygonMaya = 0; 
		!itMeshPoly.isDone(); 
		iPolygonMaya++, itMeshPoly.next())
	{
		MVector			PolygonNormalMaya;
		D3DXVECTOR3		PolygonNormalD3D;
		int				cPolyonTrianglesMaya;
		UINT			cPolyonVerticesMaya= itMeshPoly.polygonVertexCount(&stat);
		DXCHECK_MSTATUS(stat);

		int attrib = ShaderAttributes[iPolygonMaya];
		//already forced within range//if(attrib < 0) attrib= defaultAttribute; 

		stat= itMeshPoly.getNormal(PolygonNormalMaya, MSpace::kObject); 
		DXCHECK_MSTATUS(stat);

		PolygonNormalMaya.z= -PolygonNormalMaya.z;

		f4.Encode(&PolygonNormalMaya.x, 3);
		f4.Decode(&PolygonNormalD3D);

		stat= itMeshPoly.numTriangles(cPolyonTrianglesMaya);
		DXCHECK_MSTATUS(stat);

		//PolygonTriangleLoop
		for (int iPolyonTriangleMaya = 0; 
			iPolyonTriangleMaya < cPolyonTrianglesMaya; 
			iPolyonTriangleMaya++, iTriangleD3D++)
		{
			MPointArray			TrianglePointsMaya;
			MIntArray			TriangleVertexListMaya; //these are local to polygon
			int					swap;
			//D3DXPLANE			d3dxPlane;
			//D3DXVECTOR3		v0,v1,v2;
			//FLOAT				fDot;
			//BOOL				bReverse= FALSE;

			stat= itMeshPoly.getTriangle(iPolyonTriangleMaya,  
										 TrianglePointsMaya, //these are local to polygon
										 TriangleVertexListMaya, 
										 MSpace::kObject ); 	
			DXCHECK_MSTATUS(stat);			


			if(TriangleVertexListMaya.length() != 3)
			{
				DXCC_DPF_ERROR(TEXT("Polygon(%d) Triangle(%d) Size != 3"), iPolygonMaya, iPolyonTriangleMaya);
				DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
			}

			//FIX WINDING ORDER
			swap= TriangleVertexListMaya[1];
			TriangleVertexListMaya[1]= TriangleVertexListMaya[2];
			TriangleVertexListMaya[2]=swap;

			//TriangleVertexLoop
			for(int iTriangleVertex= 0; 
				iTriangleVertex < 3; 
				iTriangleVertex++)
			{
				int iVertexMaya= TriangleVertexListMaya[iTriangleVertex];;
				int iVertexD3D= (iTriangleD3D*3)+iTriangleVertex;

				////BEGIN//EXTRACT VERTEX///////////////////////////////////////////

				stat= itMeshVert.setIndex(iVertexMaya, swap);
				DXCHECK_MSTATUS(stat);

				g_VertexD3DToMaya[iVertexD3D].MayaMaterial= attrib;
				g_VertexD3DToMaya[iVertexD3D].MayaVertex= iVertexMaya;	
				if(g_VertexMayaToD3D[iVertexMaya].D3DFirstPointRep == UNUSED32)
				{//link to D3DFirstPointRep&D3DLastPointRep if not hooked up
					g_VertexMayaToD3D[iVertexMaya].D3DFirstPointRep= iVertexD3D;
					g_VertexMayaToD3D[iVertexMaya].D3DLastPointRep= iVertexD3D;

					g_D3DPointReps[iVertexD3D]=  iVertexD3D;

					g_VertexD3DToMaya[iVertexD3D].D3DNextPointRep= UNUSED32;	
				}
				else
				{//link to D3DNextPointRep&D3DLastPointRep
					DWORD iPrevPointRepD3D = g_VertexMayaToD3D[iVertexMaya].D3DLastPointRep;

					g_VertexD3DToMaya[iPrevPointRepD3D].D3DNextPointRep= iVertexD3D;	
					g_VertexD3DToMaya[iVertexD3D].D3DNextPointRep= UNUSED32;	
					g_VertexMayaToD3D[iVertexMaya].D3DLastPointRep= iVertexD3D;

					g_D3DPointReps[iVertexD3D]= g_VertexMayaToD3D[iVertexMaya].D3DFirstPointRep;
				}


				MeshInfo.VertexInfo.position= itMeshVert.position ( MSpace::kObject, &stat); 
				DXCHECK_MSTATUS(stat);			
				
				MeshInfo.VertexInfo.position.z= -MeshInfo.VertexInfo.position.z;

				hr= dxVertexBundler->SetPosition(iVertexD3D, f4.Encode(&MeshInfo.VertexInfo.position.x, 4));
				DXCC_CHECK_HRESULT(hr);

				stat= itMeshVert.getNormal(MeshInfo.VertexInfo.normal, iPolygonMaya, MSpace::kObject); 
				if(DXMFAIL(stat))
				{
					MeshInfo.VertexInfo.normal= PolygonNormalMaya;
					stat= MStatus::kSuccess;
				}

				MeshInfo.VertexInfo.normal.z= -MeshInfo.VertexInfo.normal.z;

				hr= dxVertexBundler->SetNormal(iVertexD3D, f4.Encode(&MeshInfo.VertexInfo.normal.x, 3));
				DXCC_CHECK_HRESULT(hr);


				for(UINT iUV= 0; iUV < MeshInfo.VertexInfo.UVNames.length(); iUV++)
				{
					float2 uvPoint;
					if(DXMSUCCESS(itMeshVert.getUV(iPolygonMaya, uvPoint,  &MeshInfo.VertexInfo.UVNames[iUV])))
					{
						MeshInfo.VertexInfo.Ucoords[iUV]= uvPoint[0];
						MeshInfo.VertexInfo.Vcoords[iUV]= 1.0f-uvPoint[1];
					}
					else
					{
						MeshInfo.VertexInfo.Ucoords[iUV]= 0.0f;
						MeshInfo.VertexInfo.Vcoords[iUV]= 0.0f;
					}

					hr= dxVertexBundler->SetTexcoord(iVertexD3D, iUV, f4.Encode(&D3DXVECTOR2(MeshInfo.VertexInfo.Ucoords[iUV], MeshInfo.VertexInfo.Vcoords[iUV])));
					DXCC_CHECK_HRESULT(hr);
				}

				if(itMeshVert.hasColor())
				{
					if(!DXMFAIL(itMeshVert.getColor(MeshInfo.VertexInfo.color, iPolygonMaya)))
					{
						hr= dxVertexBundler->SetDiffuse(iVertexD3D, f4.Encode(&MeshInfo.VertexInfo.color.r, 4));
						DXCC_CHECK_HRESULT(hr);
					}
				}

				////END//EXTRACT VERTEX///////////////////////////////////////////

				hr= Mesh->SetFaceVertex(iTriangleD3D, iTriangleVertex, iVertexD3D);
				DXCC_CHECK_HRESULT(hr);
			}

			hr= Mesh->SetFaceAttribute(iTriangleD3D, attrib );
			DXCC_CHECK_HRESULT(hr);

		}//END//PolygonTriangleLoop
	}//END//PolygonLoop

	hr= dxVertexBundler->UnlockAll();
	DXCC_CHECK_HRESULT(hr);

	hr= Mesh->UnlockFaces();
	DXCC_CHECK_HRESULT(hr);

	hr= Mesh->ConvertPointRepsToAdjacency(g_D3DPointReps.GetData());
	DXCC_CHECK_HRESULT(hr);

	hr= Mesh->ComputeTangent( 0, 0, 0, 0);
	DXCC_CHECK_HRESULT(hr);

e_Exit:
	

	DXCC_RELEASE(dxVertexBundler);

	//return hr; 
}


HRESULT	
DXMNodeMeshAdapter::GatherMaterials( 
	MObjectArray& Shaders,  //shadingEngine nodes
	bool AddDefaults,
	LPDXCCMESH pMesh)
{
	HRESULT hr= S_OK;
	MStatus stat= MS::kSuccess;


	//SHADER
	if(Shaders.length() > 0)
	{
		for(UINT iShader= 0; iShader < Shaders.length(); iShader++) //shadingEngine nodes
		{
			MFnDependencyNode shadingEngine(Shaders[iShader]);
			MPlug ShaderPlug;
			MPlugArray ShaderPlugArray;

			BOOL bFoundShader= false;
			BOOL bFoundMtl= false;

			//TODO displacement or volume shaders too!
			ShaderPlug = shadingEngine.findPlug("surfaceShader", &stat); 
			if (DXMSUCCESS(stat) && !ShaderPlug.isNull()) //we have to find the plug but it doesnt have to be connected
			{
				ShaderPlug.connectedTo(ShaderPlugArray, true, false); //the t
	
				if (ShaderPlugArray.length() == 1) 
				{
					MFnDependencyNode shader(ShaderPlugArray[0].node());
					int iHandle;
					DXCCHANDLE hHandle;
					
					DXMNode *shaderNode= this->GetGraph()->FindNode(ShaderPlugArray[0].node());
					if(shaderNode)
					{
						DXMNodeStdMtlAdapter* mtlAdapter= reinterpret_cast<DXMNodeStdMtlAdapter*>( shaderNode->FindAdapter(DXMNodeStdMtlAdapter::MySignature) );
						if(mtlAdapter)
						{
							pMesh->SetAttributeBoundMaterial(iShader, &mtlAdapter->material);
							bFoundMtl= true;
						}

						DXMNodeShaderAdapter* shaderAdapter= reinterpret_cast<DXMNodeShaderAdapter*>( shaderNode->FindAdapter(DXMNodeShaderAdapter::MySignature) );
						if(shaderAdapter && shaderAdapter->Shader)
						{
							pMesh->SetAttributeBoundShader(iShader, shaderAdapter->Shader);
							bFoundShader= true;
						}


					}
				}
			}

			if(!bFoundShader)
			{
				hr= pMesh->SetAttributeBoundShader(iShader, NULL);
				DXCC_CHECK_HRESULT(hr);
			}

			if(!bFoundMtl)
			{
				hr= pMesh->SetAttributeBoundMaterial(iShader, NULL);
				DXCC_CHECK_HRESULT(hr);
			}

		}

		//for polygons without a material
		if(AddDefaults)
		{
			hr= pMesh->SetAttributeBoundShader(Shaders.length(), NULL);
			DXCC_CHECK_HRESULT(hr);

			hr= pMesh->SetAttributeBoundMaterial(Shaders.length(), NULL);
			DXCC_CHECK_HRESULT(hr);
		}


	}
	else
	{
		hr= pMesh->SetAttributeBoundShader(0, NULL);
		DXCC_CHECK_HRESULT(hr);

		hr= pMesh->SetAttributeBoundMaterial(0, NULL);
		DXCC_CHECK_HRESULT(hr);
	}


	return hr;
}



HRESULT 
DXMNodeMeshAdapter::GatherMeshInfo(MObject& obj, DCCMeshInfo& MayaMeshInfo)
{
	HRESULT hr= S_OK;
	MStatus stat = MS::kSuccess;

	

	MFnMesh fnMesh(obj);

	//CALCULATE TRIANGLES AND VERTICES
	MItMeshPolygon itMeshPoly(MObject::kNullObj);




	stat = itMeshPoly.reset(obj);
	DXCHECK_MSTATUS(stat);

	for (; !itMeshPoly.isDone(); itMeshPoly.next())
	{
		int cPolyTriCount;

		stat= itMeshPoly.numTriangles(cPolyTriCount);
		DXCHECK_MSTATUS(stat);

		MayaMeshInfo.cD3DTriangles+= cPolyTriCount;
	}
	//END//CALCULATE TRIANGLES AND VERTICES

	MayaMeshInfo.cD3DVertices= MayaMeshInfo.cD3DTriangles*3;

	hr= GatherVertexInfo(obj, MayaMeshInfo.VertexInfo);
	DXCC_CHECK_HRESULT(hr);

	

//e_Exit:
	return hr;
}

HRESULT 
DXMNodeMeshAdapter::GatherVertexInfo(MObject& obj, DCCVertexInfo& MayaVertexInfo)
{
	HRESULT hr= S_OK;
	MStatus stat = MS::kSuccess;

	

	MFnMesh fnMesh(obj);


	//CALCULATE UV SET QUANTITY
	MayaVertexInfo.cUVs= fnMesh.numUVSets(&stat);
	DXCHECK_MSTATUS(stat);


	stat= fnMesh.getUVSetNames ( MayaVertexInfo.UVNames ); 
	DXCHECK_MSTATUS(stat);


	stat= MayaVertexInfo.Ucoords.setLength(MayaVertexInfo.cUVs);
	DXCHECK_MSTATUS(stat);


	stat= MayaVertexInfo.Vcoords.setLength(MayaVertexInfo.cUVs);
	DXCHECK_MSTATUS(stat);


	

//e_Exit:
	return hr;

}

HRESULT	
DXMNodeMeshAdapter::CreateMesh(MObject& obj, DCCMeshInfo &MayaMeshInfo)
{
	HRESULT hr= S_OK;
	MStatus stat = MS::kSuccess;

	MFnMesh fnMesh(obj);

	LPDXCCDECLARATION	dxDeclaration= NULL; 
	LPDIRECT3DDEVICE9	dxDevice= NULL;

	

	hr= GatherMeshInfo(obj, MayaMeshInfo);
	DXCC_CHECK_HRESULT(hr);

	if(MayaMeshInfo.cD3DVertices > g_D3DPointReps.GetCount())
	{
		g_D3DPointReps.SetCount(MayaMeshInfo.cD3DVertices);
		g_VertexD3DToMaya.SetCount(MayaMeshInfo.cD3DVertices);
	}
	new((VertexD3DToMayaType*)g_VertexD3DToMaya.GetData()) VertexD3DToMayaType[MayaMeshInfo.cD3DVertices];//fill minimum needed
	FillMemory((DWORD*)g_D3DPointReps.GetData(), MayaMeshInfo.cD3DVertices*sizeof(DWORD), 0xff);//fill minimum needed

	if(fnMesh.numVertices() > (int)g_VertexMayaToD3D.GetCount())
	{
		g_VertexMayaToD3D.SetCount(fnMesh.numVertices());
	}
	new((VertexMayaToD3DType*)g_VertexMayaToD3D.GetData()) VertexMayaToD3DType[fnMesh.numVertices()];


	hr= DXCCCreateDeclaration(&dxDeclaration);
	DXCC_CHECK_HRESULT(hr);

	hr= dxDeclaration->InsertPositionElement(dxDeclaration->NumElements());
	DXCC_CHECK_HRESULT(hr);
	hr= dxDeclaration->InsertNormalElement(dxDeclaration->NumElements());
	DXCC_CHECK_HRESULT(hr);
	hr= dxDeclaration->InsertDiffuseElement(dxDeclaration->NumElements());
	DXCC_CHECK_HRESULT(hr);
	for(UINT iTex= 0; iTex < MayaMeshInfo.VertexInfo.cUVs; iTex++)
	{
		hr=dxDeclaration->InsertTexcoordElement(dxDeclaration->NumElements(), iTex, 2, MayaMeshInfo.VertexInfo.UVNames[iTex].asChar() );
		DXCC_CHECK_HRESULT(hr);
	}

	//placed at the bottom so that we map to FVF
	hr= dxDeclaration->InsertTangentElement(dxDeclaration->NumElements());
	DXCC_CHECK_HRESULT(hr);
	hr= dxDeclaration->InsertBinormalElement(dxDeclaration->NumElements());
	DXCC_CHECK_HRESULT(hr);

	hr= g_PreviewPipeline.AccessEngine()->GetD3DDevice(&dxDevice);
	DXCC_CHECK_HRESULT(hr);

		
	DXCC_ASSERT(Mesh == NULL && Resource == NULL);

	hr= DXCCCreateMesh(MayaMeshInfo.cD3DTriangles, MayaMeshInfo.cD3DVertices, dxDevice, dxDeclaration->GetElements(), &Mesh);
	DXCC_CHECK_HRESULT(hr);


	hr= g_PreviewPipeline.AccessManager()->CreateResource((LPUNKNOWN)Mesh, IID_IDXCCMesh, TRUE, &Resource );
	DXCC_CHECK_HRESULT(hr);
	
	
	MString name= MFnDependencyNode( GetNode()->GetSite() ).name() + "_Mesh";
	hr= Resource->SetName(name.asChar());
	DXCC_CHECK_HRESULT(hr);
	


	

	DXCC_RELEASE(dxDevice);
	DXCC_RELEASE(dxDeclaration);

	return hr;
}



HRESULT	
DXMNodeMeshAdapter::GatherSkin(MFnMesh& fnMesh, MFnSkinCluster& skinCluster, MDagPathArray& boneArray)
{
	HRESULT hr = S_OK;
	MStatus stat = MS::kSuccess;

	if(skinCluster.object().isNull())
		return S_OK;

	if(boneArray.length() == 0)
		return S_OK;

	

	MDagPath preskinDP= MDagPath::getAPathTo(fnMesh.object(), NULL);
	DXCC_ASSERT(preskinDP.isValid());
	DXMNodeDagAdapter* preskinNodeAdapter= reinterpret_cast<DXMNodeDagAdapter*>( GetGraph()->FindNodeAdapter(fnMesh.object(), DXMNodeDagAdapter::MySignature) );
	DXCC_ASSERT(preskinNodeAdapter);
	DXMRoute* preskinRoute= preskinNodeAdapter->FindRoute( preskinDP );
	DXCC_ASSERT(preskinRoute);

	MDagPath postskinDP= MDagPath::getAPathTo(GetNode()->GetSite(), NULL);
	DXCC_ASSERT(preskinDP.isValid());
	DXMNodeDagAdapter* postskinNodeAdapter= reinterpret_cast<DXMNodeDagAdapter*>( GetNode()->FindAdapter( DXMNodeDagAdapter::MySignature ) );
	DXCC_ASSERT(postskinNodeAdapter);
	DXMRoute* postskinRoute= postskinNodeAdapter->FindRoute( postskinDP );
	DXCC_ASSERT(postskinRoute);

	DXCC_ASSERT( preskinRoute && postskinRoute && postskinRoute->GetParentRoute());


	hr= Mesh->SetSkinParent(postskinRoute->GetParentRoute()->Frame);
	DXCC_CHECK_HRESULT(hr);



	MPlug PlugToBindMesh = skinCluster.findPlug("geomMatrix", &stat);
	DXCHECK_MSTATUS(stat);
	MObject	MObjectToBindMesh;
	stat = PlugToBindMesh.getValue(MObjectToBindMesh);
	DXCHECK_MSTATUS(stat);
	MFnMatrixData	MatrixDataToBindMesh(MObjectToBindMesh);
	MMatrix MatrixToMesh= MatrixDataToBindMesh.matrix();


	MPlug PlugToInvBindArray = skinCluster.findPlug("bindPreMatrix", &stat);
	DXCHECK_MSTATUS(stat);

	UINT count= boneArray.length();
	for(UINT index= 0; index < count; index++)
	{
		MSelectionList	SelectionInfo;
		MFloatArray		Weights;
	
		MDagPath boneDP= boneArray[index];
		UINT iBone= skinCluster.indexForInfluenceObject(boneDP, NULL);



		DXMNodeDagAdapter* dagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( GetGraph()->FindNodeAdapter( boneDP.node(), DXMNodeDagAdapter::MySignature) );
		if(!dagAdapter)
			continue;

		DXMRoute* route= dagAdapter->FindRoute( boneDP );
		if(!route)
			continue;
	
		hr= Mesh->SetBoneFrame(iBone, route->Frame, FALSE);
		DXCC_CHECK_HRESULT(hr);

		MPlug PlugToInvBind= PlugToInvBindArray.elementByLogicalIndex(iBone);
		MObject			MObjectToInvBind;
		stat = PlugToInvBind.getValue(MObjectToInvBind);
		DXCHECK_MSTATUS(stat);
		MFnMatrixData	MatrixDataToInvBind(MObjectToInvBind);
		DXCHECK_MSTATUS(stat);
		MMatrix MatrixToInvBind= MatrixDataToInvBind.matrix();
		//MatrixToInvBind= MatrixToInvBind.inverse();
		//MatrixToInvBind= MatrixToInvBind.transpose();
		//MatrixToInvBind= MatrixToInvBind.inverse();


		MMatrix MatrixOfBoneOffset= MatrixToMesh * MatrixToInvBind;


		D3DXMATRIX		matBoneOffset;
		ConvertLocalMatrix(matBoneOffset, MatrixOfBoneOffset);


		hr= Mesh->SetBoneOffsetMatrix(iBone, &matBoneOffset);
		if(DXCC_FAILED(hr)) 
			DXCC_GOTO_EXIT(e_Exit, TRUE);

		stat = skinCluster.getPointsAffectedByInfluence(boneDP, SelectionInfo, Weights);
		if(DXMSUCCESS(stat))
		{

			UINT iWeight = 0;
			UINT numInfluenceComponents= SelectionInfo.length();
			for(UINT iInfluenceComponents= 0; iInfluenceComponents < numInfluenceComponents; iInfluenceComponents++)
			{
				MDagPath dagMesh;
				MObject component;

				//get mesh and vertices from selection data
				stat = SelectionInfo.getDagPath(iInfluenceComponents, dagMesh, component);
				
				MFnSingleIndexedComponent Component(component);
				if(postskinDP == dagMesh)
				{
					int numComponents= Component.elementCount();
					for(int iComp= 0; iComp < numComponents; iComp++, iWeight++)
					{
						int iVertexMaya= Component.element(iComp, &stat);
						DXCC_ASSERT(iWeight < Weights.length());
						float fWeight= Weights[iWeight];

						for(DWORD iVertexD3D= g_VertexMayaToD3D[iVertexMaya].D3DFirstPointRep;
							iVertexD3D != UNUSED32;
							iVertexD3D= g_VertexD3DToMaya[iVertexD3D].D3DNextPointRep)
						{
							Mesh->SetBoneInfluence(iBone, iVertexD3D, fWeight); 
						}
					}
				}
				else
				{
					iWeight+= Component.elementCount();
				}

			}//END//WEIGHTS
		}
	}

e_Exit:
	;

	

	return hr;
}
