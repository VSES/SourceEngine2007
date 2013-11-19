#pragma once
#ifndef MANAGERIMPL_H
#define MANAGERIMPL_H

class DirectXShader;
class CMayaManager;

#define DX_LONG_ATTRIB_CALLBACKID	"DXCC_NodeAttribute_CallbackID"
#define DX_SHORT_ATTRIB_CALLBACKID	"DXca"
#define DX_LONG_NAME_CALLBACKID		"DXCC_NodeName_CallbackID"
#define DX_SHORT_NAME_CALLBACKID	"DXcn"
#define DX_LONG_PARENT_CALLBACKID	"DXCC_NodeParent_CallbackID"
#define DX_SHORT_PARENT_CALLBACKID	"DXcp"


#define PROPERTY_RTPREVIEWING	"Realtime Previewing"
#define PROPERTY_VIEWEROPEN		"Viewer Restore"
#define PROPERTY_VIEWERWND		"Viewer WindowStyle"
#define PROPERTY_VIEWERPOSL		"Viewer Position Left"
#define PROPERTY_VIEWERPOST		"Viewer Position Top"
#define PROPERTY_VIEWERPOSR		"Viewer Position Right"
#define PROPERTY_VIEWERPOSB		"Viewer Position Bottom"

#define PROPERTY_POPUP "Popup On Export"
#define PROPERTY_MESH "Meshes"
#define PROPERTY_ADJACENCIES "Adjacencies"
#define PROPERTY_MATERIALS "Materials"
#define PROPERTY_EFFECTPATH_BOOL "Replace Effect Path"
#define PROPERTY_EFFECTPATH_TEXT "New Effect Path"
#define PROPERTY_TEXTUREPATH_BOOL "Replace Texture Path"
#define PROPERTY_TEXTUREPATH_TEXT "New Texture Path"
#define PROPERTY_RELATIVEPATH_BOOL "Make Relative Paths"	
#define PROPERTY_ANIMATION "Animation"	

#define VIEWER_OPTIONS_TYPENAME "Viewer Options"
#define VIEWER_OPTIONS_VERSION 3
#define EXPORTER_OPTIONS_TYPENAME "Export Options"
#define EXPORTER_OPTIONS_VERSION 2

///////////////////////////////////////////////////////////////////////////////////////
// EXTERNS
///////////////////////////////////////////////////////////////////////////////////////
extern CMayaManager g_PreviewPipeline;
extern CRITICAL_SECTION DeviceAndViewerSection; 
extern HANDLE DeviceCreatedEvent; 

extern bool g_DebugExtreme;
extern bool g_DebugBasic;


class CMayaManager : public CManager
{
	friend DirectXShader;
public:
						CMayaManager();
	virtual				~CMayaManager(); 

	virtual HRESULT		Create(MObject obj);
	virtual HRESULT		Destroy(MObject obj);

	static void			Callback_MayaExiting( void* clientData );

	void				Scene_Update(bool bForceUpdate);
	HRESULT				Scene_Export(const char* file, const char* options, MPxFileTranslator::FileAccessMode mode);
	void				Scene_PrepareForExport(bool names, bool animation);
	void 				Scene_TimeChanged_RouteUpdate();
	void 				Scene_TimeChanged_MeshUpdate();


	D3DXMATRIX& 		PerspectiveCamera_GetView();
	D3DXMATRIX& 		PerspectiveCamera_GetProjection();
	HRESULT 			PerspectiveCamera_Synchronize(); 

	static bool			UI_GetPreviewState();
	static bool			UI_GetViewerStartupState();
	static bool			UI_GetSkinState();
	static void			UI_PressRebuildDirty();

	void			    UI_SetPreviewState(bool bPreview);
	HRESULT				UI_LoadOptions();
	HRESULT				UI_SaveOptions();

	HRESULT				GetExportOptions(bool forcePopup);

	MStatus				BindViewerToPanel(const char* strView);

	//static void		Callback_SceneSave(void* clientData );

	static void			DispatchTimeChanged(void* clientData);

	static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

////////////////////////VARIABLES/////////////////////
	bool m_RecalcWorldsFromLocals;

	static UINT_PTR m_RebuildDirtyTimer;
	static WPARAM	m_RebuildDirty_MenuId;


	DXMGraph TagGraph;
	DXMGraphLayerAdapter TagGraphLayerAdapter;
	DXMGraphDagAdapter TagGraphDagAdapter;
	DXMGraphMeshAdapter TagGraphMeshAdapter;
	DXMGraphShaderAdapter TagGraphShaderAdapter;
	DXMGraphStdMtlAdapter TagGraphStdMtlAdapter;
	bool TimeChangedFlag;

	DXCCSaveSceneArgs ExportOpts;
	CHAR m_ViewerBinding[MAX_PATH];

	MCallbackId CallbackId_Exiting;
	MCallbackId CallbackId_TimeChanged;

	//MCallbackId CallbackId_Initialized;//kSceneUpdate
	//MCallbackId CallbackId_Reloading;//kSceneUpdate
	//MCallbackId CallbackId_AfterOpen;
	//MIntArray	CallbackId_PreviewArray;
	//MIntArray	CallbackId_NodeArray;

	CMayaEngine g_Engine;
	CMayaViewer g_Viewer;

	D3DXMATRIX PerspectiveCamera_View;
	D3DXMATRIX PerspectiveCamera_Projection;

protected:


};



#endif