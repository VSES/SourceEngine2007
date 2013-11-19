
#ifndef __MaxPipeline_h__
#define __MaxPipeline_h__

#pragma once

#include "Pipeline.h"
#include "PipelineLock.h"
#include "ItemTracker.h"

struct MaxMaterial
{
    //ComPtr<ID3DXEffect> Effect;
    ComPtr<IDXCCShaderProperties> Shader;
    D3DXMATERIAL Material;
    char TextureName[MAX_PATH];

    MaxMaterial()
    {
        Material.pTextureFilename = TextureName;
        TextureName[0] = 0;
    }

    MaxMaterial(LPDXCCSHADERPROPERTIES shader) //ID3DXEffect* fx)
    {
        Shader= shader;//Effect = fx;

        Material.pTextureFilename = TextureName;
        TextureName[0] = 0;
    }
};

class CMaxPipeline : public CPipeline
{
public:
	CMaxPipeline();
	virtual ~CMaxPipeline(); 

	virtual HRESULT Create();
	virtual HRESULT Destroy();

    void ResetData();
    void SyncData(bool forceUpdate, bool selectedOnly);
	
protected: // types

    typedef ItemTracker<ComPtr<IDXCCFrame> > FrameTracker;
	typedef std::map<int, FrameTracker> FrameTrackers;
    
    typedef ItemTracker<ComPtr<IDXCCMesh> > MeshTracker;
	typedef std::map<int, MeshTracker> MeshTrackers;

    typedef ItemTracker<MaxMaterial> MaterialTracker;
	typedef std::map<tstring, MaterialTracker> MaterialTrackers;

    typedef ItemTracker<ComPtr<IDirect3DBaseTexture9> > TextureTracker;
	typedef std::map<tstring, TextureTracker> TextureTrackers;

    class ReferenceTracker : public GlobalReferenceMaker
    {
        CMaxPipeline* Pipeline;
    public:

        void SetPipeline(CMaxPipeline* pipeline)
        {
            Pipeline = pipeline;
        }

        RefResult NotifyRefChanged(Interval interval, RefTargetHandle target, PartID& partId, RefMessage msg);
    };

protected: // methods

    void BeginSync();   // do not init IGameScene
    void BeginSync(bool selectedOnly);
    void BeginSync(INode* node);
    void EndSync();

    void SyncScene(bool forceUpdate);
    void SyncGameNodeTree(bool forceUpdate, IGameNode* node);
    void SyncGameNode(bool forceUpdate, IGameNode* node);
    void SyncGameNodeGameObject(bool forceUpdate, IGameNode* node, IGameObject* go);

    void UntrackNode(INode* node);
    
    void SyncGameNodeFrame(bool forceUpdate, IGameNode* node);
    HRESULT CreateFrame(IGameNode* node, IDXCCFrame** ext);
    void UpdateGameNodeFrameMatrix(IGameNode* node, IGameNode* pnode, IDXCCFrame* frame);
    void UpdateNodeFrameAnimation(IGameNode* node, IDXCCFrame* frame);
    void UpdateNodeFrameName(INode* node);
    void UpdateNodeFrameParent(INode* node);
    void UpdateNodeFrameParent(INode* node, IDXCCFrame* frame);
    void UntrackNodeFrame(FrameTrackers::iterator ft);
    void UntrackNodeFrame(INode* node);

    void SyncGameNodeMesh(bool forceUpdate, IGameNode* node, IGameMesh* gm);
    HRESULT CreateMesh(IGameNode* node, IGameMesh* gm, IDXCCMesh** ext);
    void UpdateGameNodeMesh(MeshTracker& ft);
    void UntrackNodeMesh(INode* node);
    void UntrackNodeMesh(MeshTrackers::iterator);

    void SyncMaterial(bool forceUpdate, IGameNode* node, Mtl* mtl);
    HRESULT CreateMaterial(IGameNode* node, Mtl* mtl, MaxMaterial* out);
    void UpdateMaterial(MaterialTracker& ft);
    void UntrackMaterial(const TCHAR* materialName);
    void UntrackMaterial(MaterialTrackers::iterator);
    MaterialTrackers::iterator FindMaterial(const TCHAR* materialName);
    LPDXCCSHADERPROPERTIES /*ID3DXEffect* */ FindMaterialShader(const TCHAR* materialName);
    D3DXMATERIAL* FindMaterialValues(const TCHAR* materialName);
    bool SetShaderFromParamBlock(LPDXCCSHADERPROPERTIES shader, /*ID3DXEffect* fx, */ IParamBlock2* pb, ParamID pid);

    void SyncTexture(bool forceUpdate, PBBitmap* bmp);
    HRESULT CreateTexture(PBBitmap* bmp, IDirect3DBaseTexture9** tex);
    void UpdateTexture(TextureTracker& ft);
    void UntrackTexture(const TCHAR* textureName);
    void UntrackTexture(TextureTrackers::iterator);
    TextureTrackers::iterator FindTexture(const TCHAR* fileName);

    void RegisterCallbacks();
    void UnregisterCallbacks();

	static void OnMaxSystemPostReset(void *param, NotifyInfo *info);	    //NOTIFY_SYSTEM_POST_RESET
	static void OnMaxSystemPostNew(void *param, NotifyInfo *info);		//NOTIFY_SYSTEM_POST_NEW
	static void OnMaxFilePostOpen(void *param, NotifyInfo *info);		//NOTIFY_FILE_POST_OPEN
	static void OnMaxFilePostMerge(void *param, NotifyInfo *info);		//NOTIFY_FILE_POST_MERGE
	static void OnMaxBitmapChanged(void *param, NotifyInfo *info);		//NOTIFY_BITMAP_CHANGED	
	static void OnMaxPostImport(void *param, NotifyInfo *info);			//NOTIFY_POST_IMPORT

	static void OnMaxNodeRenamed(void *param, NotifyInfo *info);		    //NOTIFY_NODE_RENAMED
	static void OnMaxNodeCreated(void *param, NotifyInfo *info);		    //NOTIFY_NODE_CREATED
	static void OnMaxNodeLinkedOrUnlinked(void *param, NotifyInfo *info);			//NOTIFY_NODE_LINKED, NOTIFY_NODE_UNLINKED
	static void OnMaxNodePostMtl(void *param, NotifyInfo *info);		    //NOTIFY_NODE_POST_MTL
	static void OnMaxNodesPreDelete(void *param, NotifyInfo *info);	    //NOTIFY_SEL_NODES_PRE_DELETE

	static VOID CALLBACK OnTimer(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);	// WM_TIMER

protected: // Data

	class CViewer* Viewer;
	class CEngine* Engine;

	DXCCSaveSceneArgs ExportOptions;

	FrameTrackers Frames;
	MeshTrackers Meshes;
    MaterialTrackers Materials;
    TextureTrackers Textures;

    ReferenceTracker RefTracker;

	// Begin stuff only valid in BeginSync/EndSync clause
	//
	CPipelineLock SyncLock;
	DWORD ThisSyncTick;	// GetTickCount at the start of the current sync
	Interface* MaxCore;
	IGameScene* Scene;
	IGameConversionManager* ConversionMgr;
	//
	// End stuff only valid in BeginSync/EndSync clause

};

extern CMaxPipeline ThePipeline;
extern CRITICAL_SECTION DeviceAndViewerSection; 
extern HANDLE DeviceCreatedEvent; 

#endif