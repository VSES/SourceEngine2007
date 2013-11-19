#include "pch.h"


DXMRoute::DXMRoute()
{ 
	Frame= NULL;
	Resource= NULL;
	NodeDagAdapter= NULL; 
	ParentRoute= NULL;
	Visible= false;//true;
	CallbackState= false; 
	ChildAddedCID= 0;
	ChildRemovedCID= 0;
	//InstanceAddedCID= 0;
	//InstanceRemovedCID= 0;
}

DXMRoute::~DXMRoute()
{
	DXCC_ASSERT(Frame == NULL);
	DXCC_ASSERT(Resource == NULL);		
	DXCC_ASSERT(ChildAddedCID == 0);		
	DXCC_ASSERT(ChildRemovedCID == 0);		
}

void DXMRoute::SyncTransform()
{
	if( g_DebugBasic )
	{
		MString name= Path.fullPathName();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}	

	D3DXMATRIX localMatrix= GetNodeDagAdapter()->GetLocalMatrix();
	Frame->SetLocalMatrix(&localMatrix, true);
}


void DXMRoute::CreateFrame()
{
	HRESULT hr;

	DestroyFrame();

	if( g_DebugBasic )
	{
		MString name= Path.fullPathName();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}	

	DXCCCreateFrame(&Frame);

	g_PreviewPipeline.AccessManager()->CreateResource(Frame, IID_IDXCCFrame, true, &Resource);
}

void 
DXMRoute::GatherName()
{
	try
	{
		MString name= Path.partialPathName();
		CAtlArray<char> newName;
		newName.SetCount( name.length() + 2 );//one for a numbered start and one for a null terminator

		DXCCConvertStringToXFileObjectName( name.asChar(), newName.GetData(), newName.GetCount() );

		Resource->SetName( newName.GetData() );
	}
	catch( ... )
	{
		DXCC_DPFA_REPORT("unhandled exception most likely a memory exhaustion" );
	}
}

void 
DXMRoute::GatherAnimation()
{
	if( Path.hasFn(MFn::kIkHandle)
	|| Path.hasFn(MFn::kIkEffector)
	|| Path.hasFn(MFn::kManipulator3D) )
		return;

	HRESULT hr= S_OK;
	MStatus stat = MS::kSuccess;

	LPDXCCANIMATIONSTREAM pAnim= NULL;

	MFnDependencyNode depNode(Path.node());
	MPlug matrixPlug;

	UINT minTick= (UINT)MAnimControl::minTime().as(MTime::uiUnit());
	UINT maxTick= (UINT)MAnimControl::maxTime().as(MTime::uiUnit());
	UINT cTicks= maxTick-minTick;
	UINT nFPS= MayaGetFPS();

	DXCC_DPFA_REPORT("GATHERING ANIMATION ON DAGPATH: [TYPE: %s] %s", depNode.typeName().asChar(), Path.fullPathName().asChar());

	matrixPlug= depNode.findPlug("matrix", &stat);//xformMatrix
	if(DXMFAIL(stat) ||  matrixPlug.isNull())
		DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, FALSE);

	hr= Frame->GetLocalAnimation(&pAnim);
	if(DXCC_FAILED(hr))
		DXCC_GOTO_EXIT(e_Exit, TRUE);

	hr= pAnim->CreateTransformKeys(nFPS, D3DXPLAY_ONCE, cTicks);
	if(DXCC_FAILED(hr))
		DXCC_GOTO_EXIT(e_Exit, TRUE);
	
	for(UINT iTick= 0; iTick < cTicks; iTick++)
	{
		MObject matrixObject= MObject::kNullObj;

		DXCCKEY_MATRIX matrixKey;
		matrixKey.Time = (float)(minTick+(float)iTick);

		MTime time(matrixKey.Time, MTime::uiUnit());

		MDGContext timeContext(time);

		stat= matrixPlug.getValue(matrixObject, timeContext);
		if(DXMSUCCESS(stat) && !matrixObject.isNull())
		{
			MFnMatrixData matrixData(matrixObject, &stat);

			ConvertLocalMatrix(matrixKey.Value, matrixData.matrix());
		}
		else
		{
			D3DXMatrixIdentity(&matrixKey.Value);
		}

		hr= pAnim->SetTransformKeyAsMatrix(iTick, &matrixKey);
		if(DXCC_FAILED(hr))
			DXCC_GOTO_EXIT(e_Exit, TRUE);
	}

e_Exit:
	DXCC_RELEASE(pAnim);
}



DXMRoute* DXMRoute::FindChild(const MDagPath& path)
{
	for(POSITION position= ChildRoutes.GetHeadPosition();
		position!= NULL;
		position= ChildRoutes.GetHeadPosition())//we keep grabbing the first one since they are being deleted
	{
		DXMRoute* childRoute= ChildRoutes.GetValueAt(position);
		
		if(childRoute->GetPath() == path)
			return childRoute;
	}

	return NULL;
}


void DXMRoute::DestroyFrame()
{
	if(Frame)
	{
		if( g_DebugBasic )
		{
			MString name= Path.fullPathName();
			DXCC_DPFA_REPORT("%s", name.asChar());
		}	

		for(POSITION position= ChildRoutes.GetHeadPosition();
			position!= NULL;
			position= ChildRoutes.GetHeadPosition())//we keep grabbing the first one since they are being deleted
		{
			DXMRoute* childRoute= ChildRoutes.GetValueAt(position);
			
			childRoute->GetNodeDagAdapter()->RemoveRoute(childRoute, true);
		}
		ChildRoutes.RemoveAll();

		Unparent();

		Frame->SetParent(NULL, FALSE);

		DXCC_ASSERT(Frame->Root());
		DXCC_ASSERT(Frame->NumChildren() == 0);
		
		DXCC_RELEASE(Frame);
	}

	if(Resource)
	{
		g_PreviewPipeline.AccessManager()->RemoveResource(Resource->GetHandle());
		DXCC_RELEASE(Resource);
	}
}

DXMRoute::DXMRouteIterator DXMRoute::GetChildRouteIterator()
{ 
	return DXMRouteIterator(&ChildRoutes); 
}

void DXMRoute::Initialize(
	DXMNodeDagAdapter* nodeDagAdapter, 
	MDagPath & path)
{

	NodeDagAdapter= nodeDagAdapter; 
	Path= path; 
	MString name= path.fullPathName();
#ifdef DEBUG
	Name= name.asChar();
#endif

	if( g_DebugBasic )
	{
		DXCC_DPFA_REPORT("%s", name.asChar());
	}



	CreateFrame();


	SetCallbackState(nodeDagAdapter->GetCallbackState()); 

	SyncHierarchy();

	DXMNodeDagAdapter* dagAdapter= GetNodeDagAdapter();
	dagAdapter->SyncFlags |= DXMNodeDagAdapter::DAGSYNC_TRANSFORM | DXMNodeDagAdapter::DAGSYNC_VISIBILITY_OF_ROUTES; /*DXMNodeDagAdapter::DAGSYNC_HIERARCHY | */
	dagAdapter->GetOwnerGraphAdapter()->AddSyncRequest(dagAdapter);
}

void DXMRoute::SyncHierarchy()
{
	if( g_DebugBasic )
	{
		MString name= Path.fullPathName();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}	


	DXMGraphDagAdapter* GraphDagAdapter= reinterpret_cast<DXMGraphDagAdapter*>(GetNodeDagAdapter()->GetOwnerGraphAdapter());
	//0: World
	if( Path.length() > 1)
	{


		MDagPath parentPath= Path;
		parentPath.pop();

		DXMRoute* parentRoute= GraphDagAdapter->FindRoute(parentPath);
		if(parentRoute)
			Parent(parentRoute);
		else
			Unparent();

	}
	else
	{
		Unparent();
	}

	if(Path.childCount() > 0)
	{
		int cChild= Path.childCount();
		for(int iChild= 0; iChild < cChild; iChild++)
		{
			MDagPath childPath= Path;
			childPath.push( Path.child(iChild) );

			DXMRoute* childRoute= GraphDagAdapter->FindRoute(childPath);
			if(childRoute)
				childRoute->Parent(this);
		}
	}

	//SyncMesh();
}

void DXMRoute::SyncMesh()
{
	if( g_DebugBasic )
	{
		MString name= Path.fullPathName();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}	

	//ADD/REMOVE MESH
	DXMNodeMeshAdapter* meshAdapter= reinterpret_cast<DXMNodeMeshAdapter*>( GetNodeDagAdapter()->GetNode()->FindAdapter( DXMNodeMeshAdapter::MySignature ) );
	if(meshAdapter)
	{
		if(meshAdapter->Mesh)
		{
			if(Visible)
				AddMember(meshAdapter->Mesh);
			else
				RemoveMember(meshAdapter->Mesh);
		}
		else
		{
			meshAdapter->SyncFlags |= DXMNodeMeshAdapter::MESHSYNC_ALL;
			meshAdapter->GetOwnerGraphAdapter()->AddSyncRequest(meshAdapter);
		}
	}
}

void DXMRoute::Destroy()
{ 
	SetCallbackState(false); 

	DestroyFrame();

	if( g_DebugBasic )
	{
		MString name= Path.fullPathName();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}
}

void DXMRoute::Parent(
	DXMRoute* parent)
{

	Unparent();
	if(parent)
	{

		if( g_DebugExtreme )
		{
			MString name= GetPath().fullPathName();
			MString parentName= parent->GetPath().fullPathName();
			DXCC_DPFA_REPORT("(This: %s) to (Parent: %s)", name.asChar(), parentName.asChar());
		}

		ParentRoute= parent;
		ParentRoute->ChildRoutes.SetAt(this, this);

		Frame->SetParent(parent->Frame, false);

	}
}

void DXMRoute::Unparent()
{
	if(ParentRoute)
	{

		if( g_DebugExtreme )
		{
			MString name= GetPath().fullPathName();
			MString parentName= ParentRoute->GetPath().fullPathName();
			DXCC_DPFA_REPORT("(This: %s) to (Parent: %s)", name.asChar(), parentName.asChar());
		}

		ParentRoute->ChildRoutes.RemoveKey(this);
		ParentRoute= NULL;

		Frame->SetParent(NULL, false);
	}

    Frame->SetParent(g_PreviewPipeline.AccessRoot(), false);

}

void DXMRoute::SetCallbackState(bool Activate)
{ 
	CallbackState= Activate;

	if(CallbackState && !ChildAddedCID)
	{
		ChildAddedCID= MDagMessage::addChildAddedCallback( Path, DXMRoute::DispatchChildAdded, this, NULL);
	}
	else if(!CallbackState && ChildAddedCID)
	{
		MMessage::removeCallback( ChildAddedCID );
		ChildAddedCID= 0;
	}

	if(CallbackState && !ChildRemovedCID)
	{
		ChildRemovedCID= MDagMessage::addChildRemovedCallback ( Path, DXMRoute::DispatchChildRemoved, this, NULL);
	}
	else if(!CallbackState && ChildRemovedCID)
	{
		MMessage::removeCallback( ChildRemovedCID );
		ChildRemovedCID= 0;
	}

/*
	if(Path.instanceNumber() == 0)
	{
		if(CallbackState && !InstanceAddedCID)
		{
			InstanceAddedCID= MDagMessage::addInstanceAddedCallback ( Path, DXMRoute::DispatchInstanceAdded, this, NULL);
		}
		else if(!CallbackState && InstanceAddedCID)
		{
			MMessage::removeCallback( InstanceAddedCID );
			InstanceAddedCID= 0;
		}

		if(CallbackState && !InstanceRemovedCID)
		{
			InstanceRemovedCID= MDagMessage::addInstanceRemovedCallback ( Path, DXMRoute::DispatchInstanceRemoved, this, NULL);
		}
		else if(!CallbackState && InstanceRemovedCID)
		{
			MMessage::removeCallback( InstanceRemovedCID );
			InstanceRemovedCID= 0;
		}	
	}
*/
}
/*
void DXMRoute::DispatchInstanceAdded( MDagPath &child, MDagPath &parent, void * clientData )
{
	DXMRoute* route= (DXMRoute*)clientData;

	if( g_DebugExtreme )
	{
		MString name= route->GetPath().fullPathName();
		MString parentName= parent.fullPathName();
		MString childName= child.fullPathName();
		DXCC_DPFA_REPORT( "(This: %s) (Parent: %s) (Child: %s)",name.asChar(), parentName.asChar(), childName.asChar() );
}}

void DXMRoute::DispatchInstanceRemoved( MDagPath &child, MDagPath &parent, void * clientData )
{
	DXMRoute* route= (DXMRoute*)clientData;

	if( g_DebugExtreme )
	{
		MString name= route->GetPath().fullPathName();
		MString parentName= parent.fullPathName();
		MString childName= child.fullPathName();
		DXCC_DPFA_REPORT( "(This: %s) (Parent: %s) (Child: %s)",name.asChar(), parentName.asChar(), childName.asChar() );
	}
}
*/

DXMNodeDagAdapter* DXMRoute::FindDagNodeAdapter(MObject& sibling)
{
	DXMNode* node= GetNodeDagAdapter()->GetGraph()->FindNode(sibling);
	if(node)
	{
		return reinterpret_cast<DXMNodeDagAdapter*>( node->FindAdapter(DXMNodeDagAdapter::MySignature) );
	}
	return NULL;
}
	
void DXMRoute::OnChildAdded( MDagPath &child)
{
	OnChildRemoved(child);

	DXMNodeDagAdapter* childNodeDagAdapter= FindDagNodeAdapter(child.node());
	if(childNodeDagAdapter)
	{
		DXMRoute* route= childNodeDagAdapter->AddRoute(child);
		DXCC_ASSERT(route);

		for(UINT i= 0; i < child.childCount(); i++)
		{
			MDagPath grandchild= child;
			grandchild.push(child.child(i));

			route->OnChildAdded(grandchild);
		}
	}
}

void DXMRoute::DispatchChildAdded( MDagPath &child, MDagPath &parent, void * clientData )
{
	DXMRoute* route= (DXMRoute*)clientData;

	DXCC_ASSERT(parent == route->GetPath());

	CManagerLock SceneLock;
	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	route->OnChildAdded(child);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}

void DXMRoute::OnChildRemoved(MDagPath &child)
{
	DXMNodeDagAdapter* childNodeDagAdapter= FindDagNodeAdapter(child.node());
	if(childNodeDagAdapter)
	{
		childNodeDagAdapter->RemoveRoute(child, true);
	}

}

void DXMRoute::DispatchChildRemoved( MDagPath &child, MDagPath &parent, void * clientData )
{
	DXMRoute* route= (DXMRoute*)clientData;


	DXCC_ASSERT(parent == route->GetPath());

	CManagerLock SceneLock;
	g_PreviewPipeline.SceneWriteLock(true, SceneLock);

	route->OnChildRemoved(child);

	g_PreviewPipeline.SceneWriteUnlock(SceneLock);
}

void DXMRoute::SyncVisibility()
{
	if( g_DebugBasic )
	{
		MString name= Path.fullPathName();
		DXCC_DPFA_REPORT("%s", name.asChar());
	}	

	bool ParentVisible;
	if(ParentRoute)
		ParentVisible= ParentRoute->IsVisible();
	else
		ParentVisible= true;

	bool NodeInvisible= GetNodeDagAdapter()->IsExplicitlyInvisible();

	bool ComputedVisibility= ParentVisible && !NodeInvisible;

	if(Visible != ComputedVisibility)
	{
		Visible= ComputedVisibility;

		for(DXMRoute::DXMRouteIterator routeItr= GetChildRouteIterator();
			!routeItr.Done();
			routeItr.Next())
		{
			DXMRoute* route= NULL;
			routeItr.GetValue(route);

			route->SyncVisibility();
		}


		SyncMesh();
	}
}

void DXMRoute::AddMember(LPUNKNOWN pUnk)
{
	if(!IsVisible())
		return;

	UINT index;
	if(!DXCC_SUCCEEDED(Frame->FindMemberByPointer(pUnk, &index)))
	{
		if( g_DebugBasic )
		{
			MString name= Path.fullPathName();
			DXCC_DPFA_REPORT("%s", name.asChar());
		}	

		Frame->AddMember(pUnk);
	}
}

void DXMRoute::RemoveMember(LPUNKNOWN pUnk)
{
	UINT index;
	if(DXCC_SUCCEEDED(Frame->FindMemberByPointer(pUnk, &index)))
	{
		if( g_DebugBasic )
		{
			MString name= Path.fullPathName();
			DXCC_DPFA_REPORT("%s", name.asChar());
		}	

		Frame->RemoveMember(index);
	}
}

