#include "pch.h"

//MFnPlugin should only be included once.  these define changes works around that problem
//this is the master #include of MFnPlugin all other will undef NT_PLUGIN and def MNoVersionString
#ifndef NT_PLUGIN
#define NT_PLUGIN
#endif 
#include <maya/MFnPlugin.h>


CMayaManager g_PreviewPipeline;
CRITICAL_SECTION DeviceAndViewerSection; 
HANDLE DeviceCreatedEvent; 
UINT_PTR CMayaManager::m_RebuildDirtyTimer;
WPARAM CMayaManager::m_RebuildDirty_MenuId;

bool g_DebugExtreme= false;
bool g_DebugBasic= false;

CMayaManager::CMayaManager()
{
	TimeChangedFlag= false;
};

CMayaManager::~CMayaManager()
{
}; 




void				
CMayaManager::UI_PressRebuildDirty() 
{ 
	PostMessage(M3dView::applicationShell(), WM_COMMAND, m_RebuildDirty_MenuId, (LPARAM)0); 
}

VOID CALLBACK 
CMayaManager::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if(idEvent==m_RebuildDirtyTimer)
	{
		if(g_PreviewPipeline.TagGraph.GetCallbackState())
		{
			bool SyncLayer= (g_PreviewPipeline.TagGraphLayerAdapter.GetSyncRequestCount() > 0) && !g_PreviewPipeline.TagGraphLayerAdapter.HasRecentSyncRequest();
			bool SyncDag= (g_PreviewPipeline.TagGraphDagAdapter.GetSyncRequestCount() > 0) && !g_PreviewPipeline.TagGraphDagAdapter.HasRecentSyncRequest();
			bool SyncMesh= (g_PreviewPipeline.TagGraphMeshAdapter.GetSyncRequestCount() > 0) && !g_PreviewPipeline.TagGraphMeshAdapter.HasRecentSyncRequest();				
			bool SyncShader= (g_PreviewPipeline.TagGraphShaderAdapter.GetSyncRequestCount() > 0) && !g_PreviewPipeline.TagGraphShaderAdapter.HasRecentSyncRequest();								
			bool SyncStdMtl= (g_PreviewPipeline.TagGraphStdMtlAdapter.GetSyncRequestCount() > 0) && !g_PreviewPipeline.TagGraphStdMtlAdapter.HasRecentSyncRequest();								

			if(SyncLayer || SyncDag || SyncMesh || SyncShader || SyncStdMtl || g_PreviewPipeline.TimeChangedFlag)
			{
				UI_PressRebuildDirty();
			}

			g_PreviewPipeline.TagGraphLayerAdapter.ResetRecentSyncRequest();
			g_PreviewPipeline.TagGraphDagAdapter.ResetRecentSyncRequest();
			g_PreviewPipeline.TagGraphMeshAdapter.ResetRecentSyncRequest();
			g_PreviewPipeline.TagGraphShaderAdapter.ResetRecentSyncRequest();
			g_PreviewPipeline.TagGraphStdMtlAdapter.ResetRecentSyncRequest();
		}
	}
}

void CMayaManager::UI_SetPreviewState(bool bPreview)
{
	MStatus stat= MS::kSuccess;

	if(bPreview != TagGraph.GetCallbackState())
	{
		TagGraph.SetCallbackState( bPreview );

		if(bPreview == true)//PREVIEWING
			MGlobal::executeCommand ("DXCCRebuildScene");
		else
			MGlobal::executeCommand ("DXCCRebuildDirty");
	}


	if( bPreview != UI_GetPreviewState() )
	{
		if(bPreview == true)//PREVIEWING
			MGlobal::executeCommand ("DirectX_SetPreviewState(1)");
		else
			MGlobal::executeCommand ("DirectX_SetPreviewState(0)");
	}

}

bool CMayaManager::UI_GetPreviewState()
{	
	int state= 0;
	MGlobal::executeCommand ("DirectX_GetPreviewState()",  state);

	return (bool)(state==0?false:true);
}


bool CMayaManager::UI_GetViewerStartupState()
{	
	int state= 0;
	MGlobal::executeCommand ("DirectX_GetViewerStartupState()",  state);

	return (bool)(state==0?false:true);
}


HRESULT CMayaManager::Destroy(MObject obj)
{
	MStatus stat = MS::kSuccess;
	MFnPlugin plugin(obj);

	MGlobal::displayInfo("DirectX Extensions for Maya - Uninitialization Beginning");

	KillTimer( NULL, m_RebuildDirtyTimer);
	m_RebuildDirtyTimer= NULL;
	m_RebuildDirty_MenuId= NULL;

	DXMNodeMeshAdapter::MPxMesh_outMesh= MObject::kNullObj;

    CManagerLock SceneLock;
	SceneWriteLock(true, SceneLock);

	TagGraph.SetCallbackState(false);
	TagGraph.DeregisterNodes();
	TagGraph.RemoveAdapter(&TagGraphLayerAdapter, false);
	TagGraph.RemoveAdapter(&TagGraphDagAdapter, false);
	TagGraph.RemoveAdapter(&TagGraphMeshAdapter, false);
	TagGraph.RemoveAdapter(&TagGraphShaderAdapter, false);
	TagGraph.RemoveAdapter(&TagGraphStdMtlAdapter, false);
	TagGraph.Destroy();

	DirectXShader::StaticDestroy();

	SceneWriteUnlock(SceneLock);

	UI_SaveOptions();
	
	g_Viewer.Destroy();

	g_Engine.Destroy();

	CManager::Destroy();

	CloseHandle(DeviceCreatedEvent);
	DeviceCreatedEvent= NULL;
	DeleteCriticalSection(&DeviceAndViewerSection);


	MMessage::removeCallback( CallbackId_TimeChanged);
	CallbackId_TimeChanged=0;

	MMessage::removeCallback( CallbackId_Exiting);
	CallbackId_Exiting=0;

	DXMUnitTests::DeregisterCommands( plugin );
	plugin.deregisterCommand(DXM_UIHELP);
	plugin.deregisterCommand(DXCC_PREVIEW_CHANGED);
	plugin.deregisterCommand(DXCC_EXPORT_SCENE);
	plugin.deregisterCommand(DXCC_EXPORT_OPTIONS);
	plugin.deregisterCommand(DXCC_REBUILD_SCENE);
	plugin.deregisterCommand(DXCC_REBUILD_DIRTY);
	plugin.deregisterCommand(DXCC_VIEW_FLOATING);
	plugin.deregisterCommand(DXCC_VIEW_CLOSE);
	plugin.deregisterCommand(DXCC_VIEW_FRONT);
	plugin.deregisterCommand(DXCC_VIEW_TOP);
	plugin.deregisterCommand(DXCC_VIEW_SIDE);

	plugin.deregisterNode( DirectXShader::id );
	plugin.deregisterFileTranslator(DXCC_EXPORTER);

	MGlobal::executeCommand("DirectX_RemoveMenu()");
	
	MGlobal::displayInfo("DirectX Extensions for Maya - Uninitialization Complete");


	return S_OK;
}



D3DXMATRIX& CMayaManager::PerspectiveCamera_GetView()
{
	return PerspectiveCamera_View;
}

D3DXMATRIX& CMayaManager::PerspectiveCamera_GetProjection()
{
	return PerspectiveCamera_Projection;
}

HRESULT CMayaManager::PerspectiveCamera_Synchronize()
{
    MDagPath MayaCamera;
    M3dView panel;
    for(UINT iView= 0; iView < M3dView::numberOf3dViews(); iView++)
    {
        D3DXMATRIX mCamera;
        M3dView::get3dView(iView, panel);
        panel.getCamera(MayaCamera);
        MayaCamera.pop();

		MString perspNameStr( "persp" );
		MString cameraNameStr = MayaCamera.partialPathName();
		
		cameraNameStr = cameraNameStr.substring(0, perspNameStr.length()-1 );
        const char* cameraName= cameraNameStr.asChar();

        if(cameraNameStr == perspNameStr )
        {
            MayaCamera.extendToShape();
            MFloatMatrix fView(MayaCamera.inclusiveMatrix().matrix );

            ConvertWorldMatrix(mCamera, fView);

            panel.getCamera(MayaCamera);
            MFnCamera fnMayaCamera(MayaCamera.node());

            MVector mUp= fnMayaCamera.upDirection();
            MVector mAt= fnMayaCamera.viewDirection();
            MPoint mEye= fnMayaCamera.eyePoint(MSpace::kWorld);
            
            D3DXVECTOR3 dxEye( (float)mEye.x, (float)mEye.y, (float)-mEye.z );
            D3DXVECTOR3 dxAt( (float)mAt.x, (float)mAt.y, (float)-mAt.z );
            D3DXVECTOR3 dxUp( (float)mUp.x, (float)mUp.y, (float)-mUp.z );
			D3DXVECTOR4 fEye;
            D3DXVECTOR4 fAt;
            D3DXVECTOR3 fUp;

            D3DXVec3Transform(&fEye, &dxEye,(D3DXMATRIX*)&mCamera);
            D3DXVec3Transform(&fAt, &dxAt,(D3DXMATRIX*)&mCamera);
            D3DXVec3TransformNormal(&fUp, &dxUp,(D3DXMATRIX*)&mCamera);
            
            
            D3DXMatrixLookAtLH(&PerspectiveCamera_View, 
                (D3DXVECTOR3*)&fEye,
                (D3DXVECTOR3*)&fAt,
                &fUp);          

            // Projection matrix
            float zNear = (float)fnMayaCamera.nearClippingPlane();
            float zFar = (float)fnMayaCamera.farClippingPlane();
            float hFOV = (float)fnMayaCamera.horizontalFieldOfView();
            float f = (float) (1.0f / (float) tan( hFOV / 2.0f ));

            ZeroMemory( &PerspectiveCamera_Projection, sizeof(PerspectiveCamera_Projection) );
            PerspectiveCamera_Projection._11 = f;
            PerspectiveCamera_Projection._22 = f;
            PerspectiveCamera_Projection._33 = (zFar+zNear) / (zFar-zNear);
            PerspectiveCamera_Projection._34 = 1.0f;
            PerspectiveCamera_Projection._43 = -2 * (zFar*zNear)/(zFar-zNear);
            
            break;
        }
    }
	return S_OK;
}




/*
void CMayaManager::Callback_SceneSave(void* clientData )
{

	CManagerLock SceneLock;
	g_PreviewPipeline.SceneReadLock(true, SceneLock);

	for(MItDependencyNodes iter; !iter.isDone(); iter.next())
	{
		MFnDependencyNode depNode(iter.item());
		if(depNode.typeId() == DirectXShader::id)
		{
			g_PreviewPipeline.Material_Save(depNode.object());
		}
	}

	g_PreviewPipeline.SceneReadUnlock(SceneLock);
}
*/

MStatus CMayaManager::BindViewerToPanel (const char* strView)
{
	//HRESULT hr= S_OK;
	HWND renderwnd= NULL;
	MDagPath MayaCamera;

	if(strView == NULL)
		strView= "";

	StringCchCopyA(m_ViewerBinding, MAX_PATH, strView);

	if(strView && (strView[0] != '\0'))
	{
		if(0 == lstrcmpiA(strView, "floating"))
		{
			g_Viewer.BindToWindow(NULL, true); 
		}
		else
		{
			M3dView ourView;
			M3dView::get3dView(0,ourView);

			for(UINT iView= 0; iView < M3dView::numberOf3dViews(); iView++)
			{
				M3dView::get3dView(iView, ourView);
				ourView.getCamera(MayaCamera);
				MayaCamera.pop();
				if(MayaCamera.partialPathName() == MString(strView))
				{
					renderwnd= (HWND)ourView.window();
					g_Viewer.BindToWindow(ourView.window(), true); 
					break;
				}
			}
		}
	}

//e_Exit:

	return MS::kSuccess;
}


void PrependPathToEnv(const TCHAR* path)
{
    DWORD oldPathLen = GetEnvironmentVariable(TEXT("PATH"), NULL, 0)+1;
    DWORD addPathLen = _tcslen(path)+1;   
    DWORD newPathLen = addPathLen + oldPathLen;    // one NULL turns into semicolon

    TCHAR* newPath = (TCHAR*)_alloca(sizeof(TCHAR)*newPathLen);
    if (newPath)
    {
        StringCchPrintf(newPath, newPathLen, TEXT("%s;"), path);
		for(UINT iC= 0; iC<addPathLen; iC++)
		{
			if(newPath[iC] == TEXT('\\'))
				newPath[iC] = '/';
		}

        GetEnvironmentVariable(TEXT("PATH"), newPath+addPathLen, newPathLen-addPathLen  );
        
		SetEnvironmentVariable(TEXT("PATH"), newPath);
    }
}

bool AddDxccDllPath(CStringA& ErrorMsg)
{
	ErrorMsg= "No Errors";

#define DXCC_PATH TEXT("..\\..\\..\\..\\x86\\")
#define DXCC_FILE TEXT("DXCC.DLL")


    HMODULE hMLL = GetModuleHandle(TEXT(THISFILENAME));
    TCHAR mllPath[MAX_PATH];
    DWORD mllPathLen = GetModuleFileName(hMLL, mllPath, MAX_PATH);

    if (!(mllPathLen != 0 && mllPathLen < MAX_PATH))
	{
		ErrorMsg= "Could not fetch this module's filename and length.";
		return false;
	}


    TCHAR fullPath[MAX_PATH];
    TCHAR* fileName;

    DWORD fullPathLen = GetFullPathName(mllPath, MAX_PATH, fullPath, &fileName);
    if (!(fullPathLen != 0 && fullPathLen < MAX_PATH && fileName))
	{
		ErrorMsg= "Could not fetch the full path of this module.";
		return false;
	}

    fileName[0] = 0;    // lop off file name from path

	//no need to add the current path at this time
	//we dont want local overrides at this time
    //PrependPathToEnv(fullPath);

	PathAppend(fullPath, DXCC_PATH);
	PrependPathToEnv(fullPath);

	HMODULE  hDXCC = LoadLibrary(DXCC_FILE);
	if(hDXCC == NULL)
	{
		ErrorMsg="DXCC.DLL could not be found";
		return false;
	}

	PathAppend(fullPath, DXCC_FILE);

    TCHAR dxccPath[MAX_PATH];
    DWORD dxccPathLen = GetModuleFileName(hDXCC, dxccPath, MAX_PATH);

	if( 0 != lstrcmpi( fullPath , dxccPath ) )
	{
		ErrorMsg.Format( "Incompatable DXCC.DLL was loaded: %S\nDesired DXCC.DLL is in path: %S", dxccPath, fullPath);
		return false;
	}


	ErrorMsg= "No Errors";
	return true;
}

void CMayaManager::DispatchTimeChanged( void* clientData )
{
	g_PreviewPipeline.TimeChangedFlag= true;
}


HRESULT CMayaManager::Create(MObject obj)
{
	HRESULT hr= S_OK;
	MStatus stat= MS::kSuccess;
	//MItDag FindRoot;
	D3DPRESENT_PARAMETERS pp = {0};
	HWND hShell;
	HMENU hMenu;
	int iMenu;
	MArgList args;
	const MString DirectXShader_UserClassify( "shader/surface" );

	

	MFnPlugin plugin(obj, "Microsoft", "6.0", "Any");

	MGlobal::displayInfo("DirectX Extensions for Maya - Initialization Beginning");


	MGlobal::displayInfo("DirectX Extensions for Maya - DXCC Initialization Beginning");
	CStringA pathChangeErr;
    if(!AddDxccDllPath(pathChangeErr))
	{
		MString errStr= MString("DirectX Extensions for Maya - Error loading DXCC: ") + pathChangeErr.GetString();
		MGlobal::displayInfo(errStr);
		MessageBoxA((HWND)M3dView::applicationShell(), errStr.asChar(), "DirectX Extensions for Maya - Critical Error", MB_ICONEXCLAMATION);
		DXCC_GOTO_EXIT(e_Exit, false);
	}

#if defined(DEBUG) | defined(_DEBUG)
	DXCCSetDebugPrintfACallback( MayaDebugPrintfACallback ) ;
	DXCCSetDebugPrintfWCallback( MayaDebugPrintfWCallback ) ;
	DXCCSetBreakPointCallback( DXCCBreakPointDefaultCallback ) ;
	//g_DebugBasic= true;
	//g_DebugExtreme= true;
#endif

	MGlobal::displayInfo("DirectX Extensions for Maya - DXCC Initialization Complete");


	MGlobal::displayInfo("DirectX Extensions for Maya - DXUT Initialization Beginning");
	if( NULL == DXUTGetD3DObject() )
	{
        if(DXCC_FAILED( DXUTInit( false, false, false ) ))
		{
			MString errStr= MString("DirectX Extensions for Maya - Error loading DXUT: Most likely caused by D3D or D3DX DLL miss-match.");
			MGlobal::displayInfo(errStr);
			MessageBoxA((HWND)M3dView::applicationShell(), errStr.asChar(), "DirectX Extensions for Maya - Critical Error", MB_ICONEXCLAMATION);
			DXCC_GOTO_EXIT(e_Exit, false);
		}


        DXCC_ASSERT( DXUTGetD3DObject() != NULL );
    }
	MGlobal::displayInfo("DirectX Extensions for Maya - DXUT Initialization Complete");


	//-----------------------INITIALIZATION ------------------------//

	hr= CManager::Create();
	if(DXCC_FAILED(hr))
	{
		MGlobal::displayError("DirectX Extensions for Maya - Error");
		MGlobal::displayError("Could not initialize base plugin manager");
		DXCC_GOTO_EXIT(e_Exit, false);
	}

	DeviceCreatedEvent= CreateEventA(NULL, FALSE, FALSE, "DXCCManager_DeviceCreated");
	InitializeCriticalSection(&DeviceAndViewerSection);


	MGlobal::displayInfo("DirectX Extensions for Maya - Engine/Device Initialization Beginning");
	g_Engine.Create(this);
	MGlobal::displayInfo("DirectX Extensions for Maya - Engine/Device Initialization Complete");

	SetEngine(&g_Engine);

	pp.BackBufferWidth = 640;
	pp.BackBufferHeight = 480;
	pp.BackBufferCount = 1;
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	pp.MultiSampleQuality = 0;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.hDeviceWindow = NULL;
	pp.Windowed = true;
	pp.EnableAutoDepthStencil = true;
	pp.AutoDepthStencilFormat = D3DFMT_D16;
	pp.FullScreen_RefreshRateInHz = 0;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	MGlobal::displayInfo("DirectX Extensions for Maya - Viewer Initialization Beginning");
	g_Viewer.Create( &g_PreviewPipeline, &pp );
	MGlobal::displayInfo("DirectX Extensions for Maya - Viewer Initialization Complete");


	MGlobal::displayInfo("DirectX Extensions for Maya - SyncGraph Initialization Beginning");
	TagGraph.Initialize("DirectXTag", "DxTag");
	TagGraph.AddAdapter(&TagGraphLayerAdapter);
	TagGraph.AddAdapter(&TagGraphDagAdapter);
	TagGraph.AddAdapter(&TagGraphMeshAdapter);
	TagGraph.AddAdapter(&TagGraphShaderAdapter);
	TagGraph.AddAdapter(&TagGraphStdMtlAdapter);
	TagGraph.SetCallbackState(true);
	TagGraph.RegisterNodes();
	MGlobal::displayInfo("DirectX Extensions for Maya - SyncGraph Initialization Complete");


	MGlobal::displayInfo("DirectX Extensions for Maya - DirectXShader Initialization Beginning");
	DirectXShader::StaticInitialize();
	stat= plugin.registerNode( 
		"DirectXShader", 
		DirectXShader::id, 
		DirectXShader::creator, 
		DirectXShader::initialize, 
		MPxNode::kDependNode, 
		&DirectXShader_UserClassify );
	DXCHECK_MSTATUS(stat);
	MGlobal::displayInfo("DirectX Extensions for Maya - DirectXShader Initialization Complete");

	MGlobal::displayInfo("DirectX Extensions for Maya - XFileTranslator Initialization Beginning");
	// register the translator
	stat = plugin.registerFileTranslator(DXCC_EXPORTER,	// name
										 NULL,			// icon
										 XFileTranslator::creator,	
										 NULL,			// script
										 NULL,
										 false);
	DXCHECK_MSTATUS(stat);
	MGlobal::displayInfo("DirectX Extensions for Maya - XFileTranslator Initialization Complete");

	
/*
	//DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_EXPORT_SELECTED,  DXCCExportSelectedCommand::creator ));
*/	
	CallbackId_Exiting= MSceneMessage::addCallback( MSceneMessage::kMayaExiting, CMayaManager::Callback_MayaExiting, NULL, NULL);
	CallbackId_TimeChanged= MEventMessage::addEventCallback ( "timeChanged", CMayaManager::DispatchTimeChanged, NULL, NULL);

	MGlobal::displayInfo("DirectX Extensions for Maya - MenuCommands Initialization Beginning");
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXM_UIHELP ,  DXMUiHelpCommand::creator ));
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_PREVIEW_CHANGED ,  DXCCPreviewChangedCommand::creator ));
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_EXPORT_SCENE,  DXCCExportSceneCommand::creator ));	
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_EXPORT_OPTIONS,  DXCCExportOptionsCommand::creator ));
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_REBUILD_SCENE,  DXCCRebuildSceneCommand::creator ));
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_REBUILD_DIRTY,  DCCRebuildDirtyCommand::creator ));	
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_VIEW_FLOATING,  DXCCFloatingViewCommand::creator ));
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_VIEW_CLOSE,  DXCCCloseViewCommand::creator ));
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_VIEW_FRONT,  DXCCFrontViewCommand::creator ));
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_VIEW_SIDE,  DXCCSideViewCommand::creator ));
	DXCHECK_MSTATUS(stat= plugin.registerCommand( DXCC_VIEW_TOP,  DXCCTopViewCommand::creator ));
	
	MGlobal::displayInfo("DirectX Extensions for Maya - MenuCommands Initialization Complete");

	//Menu Creation
	MGlobal::displayInfo("DirectX Extensions for Maya - Menu Initialization Beginning");
	MGlobal::executeCommand("eval( \"source DirectX\" )", true, true);
	MGlobal::displayInfo("DirectX Extensions for Maya - Menu Initialization Complete");


	//Menu Capture IDs
	MGlobal::displayInfo("DirectX Extensions for Maya - UpdateTimer Initialization Beginning");
	hShell= (HWND)M3dView::applicationShell();
	hMenu= GetMenu(hShell);
	for(iMenu= 0; iMenu < GetMenuItemCount(hMenu); iMenu++)
	{
		char szMenu[MAX_PATH];
		MENUITEMINFOA info;
		ZeroMemory(&info, sizeof(MENUITEMINFO));
		info.cbSize= sizeof(MENUITEMINFO);
		info.fMask= MIIM_STRING|MIIM_SUBMENU;
		info.dwTypeData= szMenu;
		info.cch= MAX_PATH;

		GetMenuItemInfoA(hMenu, iMenu, TRUE, &info);
		szMenu[MAX_PATH-1]= '\0';

		if(0 == lstrcmpiA(szMenu, "DirectX"))
		{
			HMENU hDirectXMenu= info.hSubMenu ;

			for(iMenu= 0; iMenu < GetMenuItemCount(hDirectXMenu); iMenu++)
			{
				ZeroMemory(&info, sizeof(MENUITEMINFO));
				info.cbSize= sizeof(MENUITEMINFO);
				info.fMask= MIIM_STRING|MIIM_SUBMENU;
				info.dwTypeData= szMenu;
				info.cch= MAX_PATH;

				GetMenuItemInfoA(hDirectXMenu, iMenu, TRUE, &info);
				szMenu[MAX_PATH-1]= '\0';

				if(0 == lstrcmpiA(szMenu, "Rebuild Dirty")) 
				{
					m_RebuildDirty_MenuId= (WPARAM)MAKELONG(GetMenuItemID(hDirectXMenu, iMenu), 0);
					m_RebuildDirtyTimer= SetTimer(NULL, NULL, 200, CMayaManager::TimerProc);
					break;
				}
			}
			break;
		}
	}
	MGlobal::displayInfo("DirectX Extensions for Maya - UpdateTimer Initialization Complete");

	MGlobal::displayInfo("DirectX Extensions for Maya - UI Options Loading Beginning");
	UI_LoadOptions();
	MGlobal::displayInfo("DirectX Extensions for Maya - UI Options Loading Complete");


	MGlobal::displayInfo("DirectX Extensions for Maya - State Query Command Registration Beginning");
	DXMUnitTests::RegisterCommands(plugin);
	MGlobal::displayInfo("DirectX Extensions for Maya - State Query Command Registration Complete");


	MGlobal::displayInfo("DirectX Extensions for Maya - Initialization Complete");

	

e_Exit:
	return hr;
}

void CMayaManager::Callback_MayaExiting( void* clientData )
{

    g_PreviewPipeline.Destroy(MObject::kNullObj);

	return;
}

void CMayaManager::Scene_TimeChanged_RouteUpdate()
{
	for( DXMGraph::DXMNodeIterator nodeItr= TagGraph.GetNodeIterator();
		!nodeItr.Done();
		nodeItr.Next())
	{
		DXMNode* node= NULL;
		nodeItr.GetValue( node );

		DXMNodeDagAdapter* dagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( node->FindAdapter( DXMNodeDagAdapter::MySignature ) );
		if(dagAdapter)
		{
			dagAdapter->SyncFlags |= DXMNodeDagAdapter::DAGSYNC_TRANSFORM;
			dagAdapter->GetOwnerGraphAdapter()->AddSyncRequest(dagAdapter);
		}

	}
}

void CMayaManager::Scene_TimeChanged_MeshUpdate()
{
	for( DXMGraph::DXMNodeIterator nodeItr= TagGraph.GetNodeIterator();
		!nodeItr.Done();
		nodeItr.Next())
	{
		DXMNode* node= NULL;
		nodeItr.GetValue( node );

		DXMNodeMeshAdapter* meshAdapter= reinterpret_cast<DXMNodeMeshAdapter*>( node->FindAdapter( DXMNodeMeshAdapter::MySignature ) );
		if(meshAdapter)
		{
			if(meshAdapter->Mesh->IsSkinned())
				meshAdapter->Mesh->DoSkinning();
		}

	}
}

void CMayaManager::Scene_Update(bool bForceUpdate)
{
	


	CManagerLock SceneLock;
	SceneWriteLock(true, SceneLock);
	{

		if(TimeChangedFlag)
			Scene_TimeChanged_RouteUpdate();

		TagGraphLayerAdapter.Synchronize();
		TagGraphDagAdapter.Synchronize();
		TagGraphShaderAdapter.Synchronize();
		TagGraphStdMtlAdapter.Synchronize();
		TagGraphMeshAdapter.Synchronize();

		if(TimeChangedFlag)
			Scene_TimeChanged_MeshUpdate();


		PerspectiveCamera_Synchronize();


		TimeChangedFlag= false;
	}
	SceneWriteUnlock(SceneLock);

	

	return;
}


void CMayaManager::Scene_PrepareForExport(bool names, bool animation)
{
	HRESULT hr= S_OK;

	if(!names && !animation)
		return;

	try
	{
		for( DXMGraph::DXMNodeIterator nodeItr= TagGraph.GetNodeIterator();
			!nodeItr.Done();
			nodeItr.Next())
		{
			DXMNode* node= NULL;
			nodeItr.GetValue( node );

			DXMNodeDagAdapter* dagAdapter= reinterpret_cast<DXMNodeDagAdapter*>( node->FindAdapter( DXMNodeDagAdapter::MySignature ) );
			if(dagAdapter)
			{
				for( DXMNodeDagAdapter::DXMRouteIterator routeItr= dagAdapter->GetRouteIterator();
					!routeItr.Done();
					routeItr.Next())
				{
					DXMRoute* route= NULL;
					routeItr.GetValue( route );

					if(names)
						route->GatherName();

					if(animation)
						route->GatherAnimation();
				}
			}
		}
	}
	catch (...)
	{
		DXCC_DPFA_ERROR("Unknown error gathering animation");
	}
//e_Exit:
	return;
}




HRESULT	
CMayaManager::GetExportOptions(bool forcePopup)
{
	HRESULT hr= S_OK;
	LPDXCCPROPERTYBAG pBag= NULL;
	UINT iProperty;
	char szSettingFile[MAX_PATH];
	MString mszAppDir;
	DXCCPROPERTY_KEY key= NULL;
	LPCVOID value= NULL;

	ZeroMemory(&ExportOpts, sizeof(DXCCSaveSceneArgs));

	MGlobal::executeCommand( "internalVar -userAppDir", mszAppDir, false, false );
	PathCombineA( szSettingFile, mszAppDir.asChar(), "DXCCExportOptions.xml" );

	if(DXCC_SUCCEEDED(DXCCLoadPropertyBagFromFile(szSettingFile, EXPORTER_OPTIONS_TYPENAME, EXPORTER_OPTIONS_VERSION, &pBag)))
	{
		key= pBag->GetKey(PROPERTY_POPUP);
		if(key)
		{
			pBag->GetPropertyValueAsData(key, &value);
			if(forcePopup || *(BOOL*)value)
			{
				DXCCShowPropertyBagDialog( pBag, EXPORTER_OPTIONS_TYPENAME, M3dView::applicationShell(), TRUE);
			}
		}

		DXCCSavePropertyBagAsFile( pBag, szSettingFile, EXPORTER_OPTIONS_TYPENAME, EXPORTER_OPTIONS_VERSION);
	}
	else
	{
		const BOOL True= true;
		const BOOL False= false;

		DXCCCreatePropertyBag(&pBag);
		pBag->AddProperty(PROPERTY_POPUP, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &True);
		pBag->AddProperty(PROPERTY_MESH, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &True);
		pBag->AddProperty(PROPERTY_ADJACENCIES, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &True);
		pBag->AddProperty(PROPERTY_MATERIALS, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &True);

		pBag->AddProperty(PROPERTY_EFFECTPATH_BOOL, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &False);
		pBag->AddProperty(PROPERTY_EFFECTPATH_TEXT, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_STRING, "");

		pBag->AddProperty(PROPERTY_TEXTUREPATH_BOOL, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &False);
		pBag->AddProperty(PROPERTY_TEXTUREPATH_TEXT, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_STRING, "");

		pBag->AddProperty(PROPERTY_RELATIVEPATH_BOOL, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &False);

		pBag->AddProperty(PROPERTY_ANIMATION, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &True);

		hr= DXCCShowPropertyBagDialog(pBag, EXPORTER_OPTIONS_TYPENAME, NULL, TRUE);
		if(DXCC_FAILED(hr))
			DXCC_GOTO_EXIT(e_Exit, FALSE);
	
		hr= DXCCSavePropertyBagAsFile( pBag, szSettingFile, EXPORTER_OPTIONS_TYPENAME, EXPORTER_OPTIONS_VERSION);
		if(DXCC_FAILED(hr))
			DXCC_GOTO_EXIT(e_Exit, FALSE);
	}
	
	key= pBag->GetKey(PROPERTY_MESH);
	pBag->GetPropertyValueAsData(key, &value);
	ExportOpts.NoMeshes= ! *(BOOL*)value;

	key= pBag->GetKey(PROPERTY_ADJACENCIES);
	pBag->GetPropertyValueAsData(key, &value);
	ExportOpts.NoMeshAdjacencies= ! *(BOOL*)value;

	key= pBag->GetKey(PROPERTY_MATERIALS);
	pBag->GetPropertyValueAsData(key, &value);
	ExportOpts.NoMeshMaterials= ! *(BOOL*)value;

	key= pBag->GetKey(PROPERTY_EFFECTPATH_BOOL);
	pBag->GetPropertyValueAsData(key, &value);
	ExportOpts.ReplaceEffectPaths= *(BOOL*)value;

	key= pBag->GetKey(PROPERTY_EFFECTPATH_TEXT);
	pBag->GetPropertyValueAsData(key, &value);
	StringCchCopyA(ExportOpts.ReplacementEffectPath, MAX_PATH, (LPCSTR)value);

	key= pBag->GetKey(PROPERTY_TEXTUREPATH_BOOL);
	pBag->GetPropertyValueAsData(key, &value);
	ExportOpts.ReplaceTexturePaths= *(BOOL*)value;

	key= pBag->GetKey(PROPERTY_TEXTUREPATH_TEXT);
	pBag->GetPropertyValueAsData(key, &value);
	StringCchCopyA(ExportOpts.ReplacementTexturePath, MAX_PATH, (LPCSTR)value);

	key= pBag->GetKey(PROPERTY_RELATIVEPATH_BOOL);
	pBag->GetPropertyValueAsData(key, &value);
	ExportOpts.RelativePaths= *(BOOL*)value;

	key= pBag->GetKey(PROPERTY_ANIMATION);
	pBag->GetPropertyValueAsData(key, &value);
	ExportOpts.NoAnimation= ! *(BOOL*)value;

e_Exit:
	DXCC_RELEASE(pBag);

	return hr;
}



HRESULT	
CMayaManager::UI_LoadOptions()
{
	HRESULT hr= S_OK;
	LPDXCCPROPERTYBAG pBag= NULL;
	char szSettingFile[MAX_PATH];
	MString mszAppDir;
	UINT iProperty;

	MGlobal::executeCommand( "internalVar -userAppDir", mszAppDir, false, false );
	PathCombineA( szSettingFile, mszAppDir.asChar(), "DXUIOptions.xml" );
	if(!DXCC_SUCCEEDED(DXCCLoadPropertyBagFromFile(szSettingFile, VIEWER_OPTIONS_TYPENAME, VIEWER_OPTIONS_VERSION, &pBag)))
	{
		UI_SaveOptions();

		DXCCCreatePropertyBag(&pBag);
	}


	{
		DXCCPROPERTY_KEY key;
		DXCCPROPERTY_DESC desc;
		LPCVOID value;


		key= pBag->GetKey(PROPERTY_VIEWEROPEN);
		desc= pBag->GetPropertyDesc(key);
		pBag->GetPropertyValueAsData(key, &value);
		if(key && *(BOOL*)value)
		{
			char PanelName[MAX_PATH];
			PanelName[0]= '\0';

			MGlobal::executeCommand ("DirectX_SetViewerStartupState(1)");

			key= pBag->GetKey(PROPERTY_VIEWERWND);
			desc= pBag->GetPropertyDesc(key);
			pBag->GetPropertyValueAsData(key, &value);
			if(key)
			{
			
				BindViewerToPanel((LPCSTR)value);

				if(0 == lstrcmpiA((LPCSTR)value, "floating"))
				{
					RECT WndRect = {0,0, 640, 480};

					key= pBag->GetKey(PROPERTY_VIEWERPOSL);
					desc= pBag->GetPropertyDesc(key);
					pBag->GetPropertyValueAsData(key, &value);
					if(key)
					{
						WndRect.left= *(INT*)value;
					}

					key= pBag->GetKey(PROPERTY_VIEWERPOST);
					desc= pBag->GetPropertyDesc(key);
					pBag->GetPropertyValueAsData(key, &value);
					if(key)
					{
						WndRect.top= *(INT*)value;
					}

					key= pBag->GetKey(PROPERTY_VIEWERPOSR);
					desc= pBag->GetPropertyDesc(key);
					pBag->GetPropertyValueAsData(key, &value);
					if(key)
					{
						WndRect.right= *(INT*)value;
					}

					key= pBag->GetKey(PROPERTY_VIEWERPOSB);
					desc= pBag->GetPropertyDesc(key);
					pBag->GetPropertyValueAsData(key, &value);
					if(key)
					{
						WndRect.bottom= *(INT*)value;
					}

					MoveWindow(g_Viewer.GetRenderWindow(),
						WndRect.left,
						WndRect.top,
						WndRect.right - WndRect.left,
						WndRect.bottom - WndRect.top,
						true);
				}
			}
		}

		key= pBag->GetKey(PROPERTY_RTPREVIEWING);
		desc= pBag->GetPropertyDesc(key);
		pBag->GetPropertyValueAsData(key, &value);
		if(key)
		{
			CStringA cmd;
			cmd.Format("menuItem -e -cb %d  $g_dxPreviewToggle", (*(BOOL*)value));
			
			MGlobal::executeCommand (cmd.GetString());
		}

		UI_SetPreviewState(UI_GetPreviewState());
	}

	return hr;
}


HRESULT	
CMayaManager::UI_SaveOptions()
{
	HRESULT hr= S_OK;
	LPDXCCPROPERTYBAG pBag= NULL;
	char szSettingFile[MAX_PATH];
	MString mszAppDir;

	MGlobal::executeCommand( "internalVar -userAppDir", mszAppDir, false, false );
	PathCombineA( szSettingFile, mszAppDir.asChar(), "DXUIOptions.xml" );
	if(DXCC_SUCCEEDED(DXCCCreatePropertyBag(&pBag)))
	{
		RECT Location; 
		GetWindowRect(g_Viewer.GetRenderWindow(), &Location);

		DXCCPROPERTY_KEY key;
		BOOL boolValue;
		INT intValue;

		pBag->AddProperty(PROPERTY_VIEWEROPEN, &key);
		boolValue= UI_GetViewerStartupState();
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &boolValue);

		pBag->AddProperty(PROPERTY_VIEWERWND, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_STRING, &m_ViewerBinding);

		pBag->AddProperty(PROPERTY_VIEWERPOSL, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_INT, &Location.left);

		pBag->AddProperty(PROPERTY_VIEWERPOST, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_INT, &Location.top);

		pBag->AddProperty(PROPERTY_VIEWERPOSR, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_INT, &Location.right);

		pBag->AddProperty(PROPERTY_VIEWERPOSB, &key);
		pBag->SetPropertyValueAsData(key, DXCCPD_INT, &Location.bottom);

		pBag->AddProperty(PROPERTY_RTPREVIEWING, &key);
		boolValue= UI_GetPreviewState();
		pBag->SetPropertyValueAsData(key, DXCCPD_BOOL, &boolValue);
	
		hr= DXCCSavePropertyBagAsFile(pBag, szSettingFile, VIEWER_OPTIONS_TYPENAME, VIEWER_OPTIONS_VERSION);
		if(DXCC_FAILED(hr))
			DXCC_GOTO_EXIT(e_Exit, FALSE);
	}

e_Exit:
	DXCC_RELEASE(pBag);

	return hr;
}

HRESULT	
CMayaManager::Scene_Export(
			const char* file,		// save file object
			const char* options,	// options string
			MPxFileTranslator::FileAccessMode mode)
{
 	HRESULT hr = S_OK;
	MStatus stat = MS::kSuccess;
	CManagerLock SceneLock;
	MString mszAppDir;
	
	CStringA filename(file);
	const char* pExtension= strrchr(file, '.');
	if(!pExtension)
	{
		filename.Append( ".x");

		if(PathFileExistsA( filename.GetString() ))
		{
			CStringA Msg("Would you like to write over the existing file?\n");
			Msg.Append( filename );
			
			if( IDYES != MessageBoxA((HWND)M3dView::applicationShell(), Msg.GetString(), "File Exists", MB_YESNOCANCEL ) )
			{
				return S_OK;
			}
		}
	} 
	else if(0 != _stricoll(".x", pExtension) )
	{
       MessageBoxA((HWND)M3dView::applicationShell(), "Cannot write over non- \".x\" file", "Incompatable File Type", 0 );
	   return S_OK;
	}


	hr= GetExportOptions(false);
	if (DXCC_FAILED(hr))
		DXCC_GOTO_EXIT(e_Exit, FALSE);

	StringCchCopyA(ExportOpts.RelativeToPath, MAX_PATH, filename.GetString());
	for(LPSTR str= ExportOpts.RelativeToPath; str != NULL && *str != '\0'; str++)
	{
		if(str[0] == '/')
			str[0] = '\\';
	}
	PathRemoveFileSpecA(ExportOpts.RelativeToPath);




	if(false == TagGraph.GetCallbackState())
		MGlobal::executeCommand ("DXCCRebuildScene");
	else
		MGlobal::executeCommand ("DXCCRebuildDirty");





	SceneWriteLock(true, SceneLock);

	

	DXCC_DPF(TEXT("Maya XFile Exporter"));
	DXCC_DPF(TEXT("Copyright (C) 1998-2000 Microsoft Corporation. All Rights Reserved."));
	DXCC_DPFA("Exporting to \"%s\"...", filename.GetString());

	Scene_PrepareForExport(true, !ExportOpts.NoAnimation);

	hr= DXCCSaveScene(
			filename.GetString(), 
			D3DXF_FILEFORMAT_TEXT,//D3DXF_FILEFORMAT_BINARY,//
			AccessManager(), 
			AccessRoot(), 
			NULL, 
			&ExportOpts);
	if (DXCC_FAILED(hr))
		DXCC_GOTO_EXIT(e_Exit, TRUE);

	

e_Exit:

	SceneWriteUnlock(SceneLock);

	return hr;
}

//
//HRESULT 
//MayaMeshHierarchy::SceneAddAnimationFixedStep()
//{
//	HRESULT	hr = S_OK;
//	MStatus stat = MS::kSuccess;
//
//	//MAnimControl mAnimCtrl;
//	MTime mtTick, mtOriginalTime;
//	LONG nTicks;
//	LPDXCCFRAME pITFrame= NULL;
//	LPDXCCFRAMEITERATOR pIT = NULL;
//	DWORD nFPS= MayaGetFPS();
//
//	hr= DXCCCreateFrameIterator(&pIT);
//	if(DXCC_FAILED(hr))
//		DXCC_GOTO_EXIT(e_Exit, TRUE);
//
//	mtOriginalTime = MAnimControl::currentTime();
//
//	nTicks= (LONG)(MAnimControl::maxTime()-MAnimControl::minTime()).value()+1;
//
//	//Initialize frames
//	
//	for(pIT->Reset(pRoot, DXCCITERATOR_DEPTHFIRST), pIT->Next(); !pIT->Done(); pIT->Next())
//	{
//		LPFRAMEUSERDATA pFrameUD;
//
//		hr= pIT->Get(&pITFrame);
//		if(DXCC_FAILED(hr))
//			DXCC_GOTO_EXIT(e_Exit, TRUE);
//
//		pFrameUD= FrameGetUserData(pITFrame, pDXCCManager);
//
//		//if(MAnimUtil::isAnimated(pFrameUD->dagPath))
//		if(pFrameUD->dagPath.hasFn(MFn::kTransform))
//		{
//			
//			LPDXCCANIMATIONSTREAM pAnimation= NULL;
//			hr= pITFrame->GetLocalAnimation(&pAnimation);
//			if(DXCC_FAILED(hr))
//				DXCC_GOTO_EXIT(e_Exit, TRUE);
//
//			hr= pAnimation->CreateTransformKeys(nFPS, D3DXPLAY_ONCE, nTicks);
//			if(DXCC_FAILED(hr))
//				DXCC_GOTO_EXIT(e_Exit, TRUE);
//
//			DXCC_RELEASE(pAnimation);
//		}
//
//		DXCC_RELEASE(pITFrame);
//	}
//
//	for (mtTick = MAnimControl::minTime(); mtTick <= MAnimControl::maxTime(); mtTick += 1)
//	{
//		LONG iTick= (LONG)mtTick.value() - (LONG)MAnimControl::minTime().value();
//
//		stat= MAnimControl::setCurrentTime(mtTick);
//		if(DXMFAIL(stat))
//			DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
//
//		for(pIT->Reset(pRoot, DXCCITERATOR_DEPTHFIRST), pIT->Next(); !pIT->Done(); pIT->Next())
//		{
//			LPFRAMEUSERDATA pFrameUD;
//
//			hr= pIT->Get(&pITFrame);
//			if(DXCC_FAILED(hr))
//				DXCC_GOTO_EXIT(e_Exit, TRUE);
//			
//			pFrameUD= FrameGetUserData(pITFrame, pDXCCManager);
//			if(pFrameUD->dagPath.hasFn(MFn::kTransform))
//			{
//				hr= FrameAddAnimationFixedStep(iTick, pITFrame);
//				if(DXCC_FAILED(hr))
//					DXCC_GOTO_EXIT(e_Exit, TRUE);
//			}
//
//			DXCC_RELEASE(pITFrame);
//		}
//	}
//
//e_Exit:
//	DXCC_RELEASE(pITFrame);
//	DXCC_RELEASE(pIT);
//
//	// reset current time
//	MAnimControl::setCurrentTime(mtOriginalTime);
//
//	return hr;
//}
//
//HRESULT 
//MayaMeshHierarchy::FrameAddAnimationFixedStep(
//	LONG iTick, 
//	LPDXCCFRAME _pFrame)
//{
//	HRESULT	hr = S_OK;
//	MStatus stat = MS::kSuccess;
//	MFnTransform fnTransform;
//	DXCCKEY_MATRIX KeyMatrix;
//	MMatrix MayaMatrix;
//	DWORD i, j;			// counters
//	LPDXCCANIMATIONSTREAM pAnimation= NULL;
//	DWORD nFPS= MayaGetFPS();
//	MDagPath FramePath;
//
//	LPFRAMEUSERDATA pFrameUD= FrameGetUserData(_pFrame, pDXCCManager);
//	//MDagPath::getAPathTo(pFrameUD->object, FramePath);
//	FramePath= pFrameUD->dagPath;
//
//	if(!FramePath.hasFn(MFn::kTransform))
//		DXCC_GOTO_EXIT( e_Exit, FALSE);
//
//	//if(MAnimUtil::isAnimated(pFrameUD->dagPath))
//	//	DXCC_GOTO_EXIT( e_Exit, FALSE);
//
//	hr= _pFrame->GetLocalAnimation(&pAnimation);
//	if(DXCC_SUCCEEDED(hr))
//	{
//		stat = fnTransform.setObject(FramePath);
//		if(DXMFAIL(stat))
//			DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
//
//
//		KeyMatrix.Time= (FLOAT)iTick;///(float)nFPS;
//
//		MayaMatrix= fnTransform.transformation().asMatrix();
//		for (i = 0; i < 4; i++)
//			for (j = 0; j < 4; j++)
//				KeyMatrix.Value[i * 4 + j]= (FLOAT)MayaMatrix(i, j);
//
//		hr= pAnimation->SetTransformKeyAsMatrix(iTick, &KeyMatrix);
//		if(DXCC_FAILED(hr))
//			DXCC_GOTO_EXIT(e_Exit, TRUE);
//	}
//
//e_Exit:
//	DXCC_RELEASE(pAnimation);
//	return hr;
//};
//
//
//HRESULT 
//MayaMeshHierarchy::SceneAddAnimationKeys()
//{
//	HRESULT	hr = S_OK;
//	MStatus stat = MS::kSuccess;
//	LPDXCCFRAME pITFrame= NULL;
//	LPDXCCFRAMEITERATOR pIT= NULL;
//	MAnimControl mAnimCtrl;
//
//	hr= DXCCCreateFrameIterator(&pIT);
//	if(DXCC_FAILED(hr))
//		DXCC_GOTO_EXIT(e_Exit, TRUE);
//
//
//	for(pIT->Reset(pRoot, DXCCITERATOR_DEPTHFIRST), pIT->Next(); !pIT->Done(); pIT->Next())
//	{
//		hr= pIT->Get(&pITFrame);
//		if(DXCC_FAILED(hr))
//			DXCC_GOTO_EXIT(e_Exit, TRUE);
//
//		hr= FrameAddAnimationKeys(pITFrame);
//		if(DXCC_FAILED(hr))
//			DXCC_GOTO_EXIT(e_Exit, TRUE);
//
//		DXCC_RELEASE(pITFrame);
//	}
//
//e_Exit:
//	DXCC_RELEASE(pITFrame);
//	DXCC_RELEASE(pIT);
//
//	return hr;
//}
//
//HRESULT 
//MayaMeshHierarchy::FrameAddAnimationKeys(
//	LPDXCCFRAME _pFrame)
//{
//	HRESULT	hr = S_OK;
//	MStatus stat = MS::kSuccess;
////
////	DWORD nFPS= MayaGetFPS();
////	MAnimControl mAnimCtrl;
////	MTime mtTick;
////	MTime mtOriginalTime;
////	MFnTransform fnTransform;
////	MPlugArray AnimPlugs;
////	UINT iPlug, iCurve, i, j;
////	BOOL* rgbKeyedFrames;
////	UINT iKey, nKeys, iTick, nTicks= (UINT)(MAnimControl::maxTime()-MAnimControl::minTime()).value()+1;
////	MMatrix MayaMatrix;
////	LPDXCCANIMATIONSTREAM pAnimation;
////	MDagPath FramePath;
////
////	LPFRAMEUSERDATA pFrameUD= FrameGetUserData(_pFrame);
////	//MDagPath::getAPathTo(pFrameUD->object, FramePath);
////	FramePath= pFrameUD->dagPath;
////
////	if(MAnimUtil::isAnimated(pFrameUD->dagPath))
////		DXCC_GOTO_EXIT( e_Exit, FALSE);
////
////	mtOriginalTime = mAnimCtrl.currentTime();
////
////	stat = fnTransform.setObject(FramePath);
////	if(DXMFAIL(stat))
////		DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
////
////	MAnimUtil::findAnimatedPlugs(FramePath, AnimPlugs, false, &stat);
////	if(DXMFAIL(stat))
////		DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
////
////	//COUNT AND FLAG THE KEYS
////	//MAYA IS MORE COMPLICATED THAN SRT SO WE WILL JUST CAPTURE THE WHOLE MATRIX ON ANY KEY
////	rgbKeyedFrames= (BOOL*)_alloca(sizeof(BOOL)*nTicks);
////	nKeys= 0;
////	for (nKeys= 0, iPlug = 0; iPlug < AnimPlugs.length(); iPlug++)
////	{
////		MObjectArray Curves;
////
////		MAnimUtil::findAnimation(AnimPlugs[iPlug], Curves, &stat);
////		if(DXMFAIL(stat))
////		{
////			DXCC_DPF(TEXT("Ignoring anim-plug because could not find animation."));
////			continue;
////		}
////
////		for (iCurve = 0; iCurve < Curves.length(); iCurve++)
////		{
////			MFnAnimCurve fnCurve(Curves[iCurve], &stat);
////			if(DXMFAIL(stat))
////				DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
////
////			for (iKey = 0; iKey < fnCurve.numKeys(); iKey++)
////			{
////				MTime mtTime = fnCurve.time(iKey, &stat);
////				if(DXMFAIL(stat))
////					DXCC_STATUS_EXIT(hr, E_FAIL, e_Exit, TRUE);
////
////				rgbKeyedFrames[(LONG)mtTime.value()]= TRUE;
////				nKeys++;
////			}
////		}
////	}
////
////	hr= DXCCCreateAnimationStreamForMatrices(nKeys, &pAnimation);
////	if(DXCC_FAILED(hr))
////		DXCC_GOTO_EXIT(e_Exit, TRUE);
////
////	hr= _pFrame->SetLocalAnimation(pAnimation);
////	if(DXCC_FAILED(hr))
////		DXCC_GOTO_EXIT(e_Exit, TRUE);
////
////	for(iKey= 0, iTick = 0; iTick < nTicks; iTick++)
////	{
////		if(rgbKeyedFrames[iTick] == TRUE)
////		{
////			LPDXCCKEY_MATRIX pKeyMatrix;
////			hr= pAnimation->GetMatrix(iKey, &pKeyMatrix);
////			if(DXCC_FAILED(hr))
////				DXCC_GOTO_EXIT(e_Exit, TRUE);
////
////			pKeyMatrix->Time= iTick/(float)nFPS ;
////
////			MayaMatrix= fnTransform.transformation().asMatrix();
////			for (i = 0; i < 4; i++)
////				for (j = 0; j < 4; j++)
////					pKeyMatrix->Value[i * 4 + j]= (FLOAT)MayaMatrix(i, j);
////
////
////			iKey++;
////		}
////	}
////
////e_Exit:
////	DXCC_RELEASE(pAnimation);
////	// reset current time
////	mAnimCtrl.setCurrentTime(mtOriginalTime);
//
//	return hr;
//};


