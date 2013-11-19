#include "StdAfx.h"

#include "PipelineEngine.h"
#include "MaxPipeline.h"
#include "MaxUtilities.h"
#include "PipelineLock.h"
#include "Engine.h"

#include <crtdbg.h>

#define DXMATERIAL_CLASS_ID Class_ID(0xed995e4, 0x6133daf2)
#define DXMATERIAL_DYNAMIC_UI Class_ID(0xef12512, 0x11351ed1)

CMaxPipeline ThePipeline;
CRITICAL_SECTION DeviceAndViewerSection; 
HANDLE DeviceCreatedEvent; 

static bool DelayRecalcs = false;

typedef std::map<int, int> NodeIndexMap;
typedef std::vector<UINT> uints;

class VertexSplitMap
{
    struct VertSplitList
    {
        UINT FirstSplit;
        UINT LastSplit;

        VertSplitList()
        {
            FirstSplit = LastSplitMarker;
            LastSplit = LastSplitMarker;
        }
    };

    typedef std::vector<VertSplitList> VertSplitLists;

    VertSplitLists HostVerts;   // [0 ... NumHostVerts]

    uints PointReps;       // [0 ... NumSplitVerts]    
    uints SplitVerts;      // [0 ... NumSplitVerts]

public:

    const static UINT LastSplitMarker = 0xFFFFFFFF;

    void SetNumHostVerts(UINT numHostVerts)
    {
        HostVerts.resize(numHostVerts);
        PointReps.resize(0);
        SplitVerts.resize(0);
    }

    void AddSplitVert(UINT hostVert)
    {
        assert(hostVert < HostVerts.size());

        UINT newSplitVert = SplitVerts.size();
        
        VertSplitList& hv = HostVerts[hostVert];

        if (hv.FirstSplit == LastSplitMarker)
        {
            hv.FirstSplit = newSplitVert;
        }

        if (hv.LastSplit != LastSplitMarker)
        {
            SplitVerts[hv.LastSplit] = newSplitVert;
        }

        hv.LastSplit = newSplitVert;

        PointReps.push_back(hv.FirstSplit);
        SplitVerts.push_back(LastSplitMarker);
    }

    int NumTotalSplitVerts() const
    {
        return SplitVerts.size();
    }

    const UINT* GetPointReps() const
    {
        return &(PointReps[0]);
    }

    UINT GetFirstSplitVert(UINT hostVert, UINT* iter) const
    {
        assert(hostVert < HostVerts.size());
        *iter = HostVerts[hostVert].FirstSplit;
        return GetNextSplitVert(iter);
    }

    UINT GetNextSplitVert(UINT* iter) const
    {
        assert((*iter < SplitVerts.size()) || (*iter == LastSplitMarker));

        UINT SplitVert = *iter;

        if (SplitVert != LastSplitMarker)
        {
            *iter = SplitVerts[SplitVert];
        }
        else
        {
            *iter = LastSplitMarker;
        }

        return SplitVert;
    }
};

static D3DXMATERIAL DefaultMaterial =
{
    {
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 0.0f, 0.0f, 0.0f, 0.0f },
        0.0f
    },
    NULL
};

DWORD BuildBoneMap(IGameSkin* gs, NodeIndexMap& bones)
{
    int numBones = 0;

    for (int v=0; v<gs->GetNumOfSkinnedVerts(); v++)
    {
        int numVertBones = (gs->GetVertexType(v) == IGameSkin::IGAME_RIGID)? 1 : gs->GetNumberOfBones(v);

        for (int b=0; b<numVertBones; b++)
        {
            int id = gs->GetBoneID(v, b);

            NodeIndexMap::iterator bi = bones.find(id);
            if (bi == bones.end())
            {
                bones.insert(bones.begin(), NodeIndexMap::value_type(id, numBones));
                numBones++;
            }
        }
    }

    return numBones;
}

void Dpf(const char* fmt, ...)
{
#if 0
	char buf[255]; 
	va_list ap; 
	va_start(ap, fmt); 
	if (SUCCEEDED(StringCbVPrintfA(buf, sizeof(buf), fmt, ap))) 
	{ 
		OutputDebugString(buf);
		OutputDebugString("\n");
	} 
	va_end(ap); 
#endif
}

#define _V(x) {HRESULT hr = x; if(FAILED(hr)){ Dpf("%s(%d): %08X! %s", __FILE__, __LINE__, hr, #x);}}


RefResult CMaxPipeline::ReferenceTracker::NotifyRefChanged(Interval interval, RefTargetHandle target, PartID& partId, RefMessage msg)
{
    return REF_SUCCEED;
}

CMaxPipeline::CMaxPipeline()
{
    RefTracker.SetPipeline(this);
}

CMaxPipeline::~CMaxPipeline()
{
}

HRESULT CMaxPipeline::Create()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF|_CRTDBG_CHECK_ALWAYS_DF);

    HRESULT hr = S_OK;

    if (SUCCEEDED(hr = CPipeline::Create()))
    {
	    MaxCore = GetCOREInterface();

        DeviceCreatedEvent= CreateEventA(NULL, FALSE, FALSE, "DXCCManager_DeviceCreated");
	    InitializeCriticalSection(&DeviceAndViewerSection);

        Engine = new CEngine();
        Engine->Create(this);
        SetEngine(Engine);

        //RegisterCallbacks();
    }

    return hr;
}

HRESULT CMaxPipeline::Destroy()
{
    HRESULT hr = S_OK;

    ResetData();

    if (SUCCEEDED(hr = CPipeline::Destroy()))
    {
        Engine->Destroy();
        delete Engine;
        SetEngine(NULL);

	    MaxCore = NULL;

        //UnregisterCallbacks();
    }

    return hr;
}

void CMaxPipeline::ResetData()
{
	ThePipeline.SceneWriteLock(true, SyncLock);

    TextureTrackers::iterator tt;
    while ((tt = Textures.begin()) != Textures.end())
    {
        UntrackTexture(tt);
    }

    MaterialTrackers::iterator et;
    while ((et = Materials.begin()) != Materials.end())
    {
        UntrackMaterial(et);
    }

    MeshTrackers::iterator mt;
    while ((mt = Meshes.begin()) != Meshes.end())
    {
        UntrackNodeMesh(mt);
    }

    FrameTrackers::iterator ft;
    while ((ft = Frames.begin()) != Frames.end())
    {
        UntrackNodeFrame(ft);
    }

    CreateRoot();

    ThePipeline.SceneWriteUnlock(SyncLock);
}

void CMaxPipeline::SyncData(bool forceUpdate, bool selectedOnly)
{
    BeginSync(selectedOnly);
    SyncScene(forceUpdate);
    EndSync();
}

void CMaxPipeline::BeginSync(bool selectedOnly)
{
	ThePipeline.SceneWriteLock(true, SyncLock);

    if (ThisSyncTick == 0)
    {
	    ThisSyncTick = GetTickCount();

        ConversionMgr = GetConversionManager();
        ConversionMgr->SetCoordSystem(IGameConversionManager::IGAME_D3D);

	    Scene = GetIGameInterface();
        Scene->InitialiseIGame(selectedOnly);
	    Scene->SetStaticFrame(0);
    }
}

void CMaxPipeline::BeginSync()
{
	ThePipeline.SceneWriteLock(true, SyncLock);

    if (ThisSyncTick == 0)
    {
	    ThisSyncTick = GetTickCount();

        ConversionMgr = GetConversionManager();
        ConversionMgr->SetCoordSystem(IGameConversionManager::IGAME_D3D);

	    Scene = NULL;
    }
}

void CMaxPipeline::BeginSync(INode* node)
{
	ThePipeline.SceneWriteLock(true, SyncLock);

    if (ThisSyncTick == 0)
    {
	    ThisSyncTick = GetTickCount();

        ConversionMgr = GetConversionManager();
        ConversionMgr->SetCoordSystem(IGameConversionManager::IGAME_D3D);

	    Scene = GetIGameInterface();
        Scene->InitialiseIGame(node, true);
	    Scene->SetStaticFrame(0);
    }
}

void CMaxPipeline::EndSync()
{
	if (Scene)
	{
		Scene->ReleaseIGame();
		Scene = NULL;
	}

	ConversionMgr = NULL;

	ThisSyncTick = 0;

	ThePipeline.SceneWriteUnlock(SyncLock);
}


void CMaxPipeline::SyncScene(bool forceUpdate)
{
    Dpf("Begin Sync Scene");

    for(int n=0; n<Scene->GetTopLevelNodeCount(); n++)
	{
        IGameNode* node = Scene->GetTopLevelNode(n);
		SyncGameNodeTree(forceUpdate, node);
	}

    Dpf("End Sync Scene");
}


void CMaxPipeline::SyncGameNodeTree(bool forceUpdate, IGameNode* node)
{
    SyncGameNode(forceUpdate, node);

	for (int c=0; c<node->GetChildCount(); c++)
	{
        IGameNode* child = node->GetNodeChild(c);
		SyncGameNodeTree(forceUpdate, child);
	}
}

void CMaxPipeline::SyncGameNode(bool forceUpdate, IGameNode* node)
{
    if (node->IsGroupOwner())
    {
        return;
    }

    SyncGameNodeFrame(forceUpdate, node);

    IGameObject* go = node->GetIGameObject();
    if (go)
    {
        SyncGameNodeGameObject(forceUpdate, node, go);
    }

    IGameMaterial* gm = node->GetNodeMaterial();
    if (gm)
    {
//        SyncGameNodeMaterial(forceUpdate, node, gm);
    }
}

void CMaxPipeline::UntrackNode(INode* node)
{
    // untrack in reverse ourder of tracking

//    UntrackNodeMaterials(node);
    UntrackNodeMesh(node);
    UntrackNodeFrame(node);
}

void CMaxPipeline::UntrackNodeFrame(INode* node)
{
    FrameTrackers::iterator ft = Frames.find(node->GetHandle());
    UntrackNodeFrame(ft);
}

void CMaxPipeline::UntrackNodeFrame(FrameTrackers::iterator ft)
{
    if (ft != Frames.end())
    {
        // reparent children to scene root
        IDXCCFrame* sr = AccessRoot();
        UINT nc = ft->second.Item->NumChildren();
        for (UINT c=nc; c>0; c--)
        {
            ComPtr<IDXCCFrame> cf;
            if (SUCCEEDED(ft->second.Item->GetChild(c-1, cf)))
            {
                cf->SetParent(NULL, DelayRecalcs);
                sr->AddChild(cf, DelayRecalcs);
            }
		}       

        // unparent ourselves.
        ft->second.Item->SetParent(NULL, DelayRecalcs);

        ComPtr<IDXCCResource> res;
        if (SUCCEEDED(AccessManager()->FindResourceByPointer(ft->second.Item, NULL, res)))
        {
            AccessManager()->RemoveResource(res->GetHandle());
        }

        Frames.erase(ft);
    }
}

void CMaxPipeline::SyncGameNodeFrame(bool forceUpdate, IGameNode* node)
{
    FrameTrackers::iterator trk = Frames.find(node->GetNodeID());
    
    TCHAR* name = node->GetName();

    if (trk == Frames.end())
    {
        ComPtr<IDXCCFrame> frame;

        if (SUCCEEDED(CreateFrame(node, frame)))
        {
            Frames.insert(Frames.begin(), FrameTrackers::value_type(node->GetNodeID(), FrameTracker(frame)));
        }
    }
    else
    {
        FrameTracker& ft = trk->second;

        if (forceUpdate || ft.IsExtStale())
        {
            UpdateNodeFrameParent(node->GetMaxNode(), ft.Item);

            UpdateGameNodeFrameMatrix(node, node->GetNodeParent(), ft.Item);

            UpdateNodeFrameAnimation(node, ft.Item);
        }
    }
}

HRESULT CMaxPipeline::CreateFrame(IGameNode* node, IDXCCFrame** ext)
{
    HRESULT hr = S_OK;

    IDXCCFrame* f;

    if (SUCCEEDED(hr = DXCCCreateFrame(&f)))
    {
        ComPtr<IDXCCResource> res;

		if (SUCCEEDED(hr = AccessManager()->CreateResource(f, IID_IDXCCFrame, TRUE, res)))
        {
            char nameBuf[256];

            MakeExportableName(node->GetName(), nameBuf, sizeof(nameBuf)/sizeof(nameBuf[0]));
            
            if (SUCCEEDED(hr = res->SetName(nameBuf)))
            {
                IGameNode* pn = node->GetNodeParent();

                UpdateNodeFrameParent(node->GetMaxNode(), f);

                UpdateGameNodeFrameMatrix(node, pn, f);

                UpdateNodeFrameAnimation(node, f);

                *ext = f;
            }
        }
    }

    return hr;
}

void CMaxPipeline::UpdateGameNodeFrameMatrix(IGameNode* node, IGameNode* pnode, IDXCCFrame* frame)
{
    D3DXMATRIX dom;
    GMatrix om = node->GetObjectTM();
    ConvertGMatrix(dom, om);

    D3DXMATRIX dwm;
    GMatrix wm = node->GetWorldTM();
    ConvertGMatrix(dwm, wm);

    frame->SetLocalMatrix(&dom, DelayRecalcs);
}

void CMaxPipeline::UpdateNodeFrameName(INode* node)
{
    assert(node != NULL);

    FrameTrackers::iterator ft = Frames.find(node->GetHandle());
    if (ft != Frames.end())
    {
        ComPtr<IDXCCResource> res;

        if (SUCCEEDED(AccessManager()->FindResourceByPointer(ft->second.Item, NULL, res)))
        {
            res->SetName(node->GetName());
        }
    }
}

void CMaxPipeline::UpdateNodeFrameAnimation(IGameNode* node, IDXCCFrame* frame)
{
    const UINT samplesPerSecond = 30;
  
    ComPtr<IDXCCAnimationStream> anim;

    if (SUCCEEDED(frame->GetLocalAnimation(anim)))
    {
        UINT ticksPerFrame = GetTicksPerFrame();
        UINT framesPerSecond = GetFrameRate();
        UINT ticksPerSample = (ticksPerFrame * framesPerSecond) / samplesPerSecond;

        Interval interval = MaxCore->GetAnimRange();
        UINT range = interval.End() - interval.Start();
        UINT numSamples = range / ticksPerSample;

        if ((range % ticksPerSample) != 0)
        {
            numSamples++;
        }

        if (SUCCEEDED(anim->CreateTransformKeys(samplesPerSecond, D3DXPLAY_ONCE, numSamples)))
        {
            UINT k = 0;
            for (TimeValue tick = interval.Start(); tick <= interval.End(); tick += ticksPerSample, k++)
            {
                GMatrix tm;
                GMatrix fm = node->GetWorldTM(tick);

                IGameNode* pn = node->GetNodeParent();
                if (pn)
                {
                    GMatrix pfm = pn->GetWorldTM(tick);
                    Matrix3 ipfm = pfm.ExtractMatrix3();
                    ipfm.Invert();

                    tm = fm * GMatrix(ipfm);
                }
                else
                {
                    tm = fm;
                }

                
                DXCCKEY_MATRIX key;
                key.Time = (float)k; 
                ConvertGMatrix(key.Value, tm);

                _V(anim->SetTransformKeyAsMatrix(k, &key));
            }

            if ((range % ticksPerSample) != 0)
            {
                GMatrix tm;
                GMatrix fm = node->GetWorldTM(tick);

                IGameNode* pn = node->GetNodeParent();
                if (pn)
                {
                    GMatrix pfm = pn->GetWorldTM(tick);
                    Matrix3 ipfm = pfm.ExtractMatrix3();
                    ipfm.Invert();

                    tm = fm * GMatrix(ipfm);
                }
                else
                {
                    tm = fm;
                }

                
                DXCCKEY_MATRIX key;
                key.Time = (float)k; 
                ConvertGMatrix(key.Value, tm);

                _V(anim->SetTransformKeyAsMatrix(k, &key));
            }
        }
    }
}

void CMaxPipeline::UpdateNodeFrameParent(INode* node)
{
    FrameTrackers::iterator ft = Frames.find(node->GetHandle());
    if (ft != Frames.end())
    {
        UpdateNodeFrameParent(node, ft->second.Item);
    }
}

void CMaxPipeline::UpdateNodeFrameParent(INode* node, IDXCCFrame* frame)
{
    if (frame)
    {
        IDXCCFrame* pf = AccessRoot();
        INode* pn = node->GetParentNode();
        if (pn)
        {
            FrameTrackers::iterator pt = Frames.find(pn->GetHandle());
            if (pt != Frames.end())
            {
                pf = pt->second.Item;
            }
        }

        pf->AddChild(frame, DelayRecalcs);
    }
}

void CMaxPipeline::SyncGameNodeGameObject(bool forceUpdate, IGameNode* node, IGameObject* go)
{
    switch (go->GetIGameType())
    {
    case IGameObject::IGAME_LIGHT: 
        //SyncGameNodeLight(forceUpdate, node, go);
        break;
	case IGameObject::IGAME_MESH: 
        SyncGameNodeMesh(forceUpdate, node, (IGameMesh*)go);
        break;
	case IGameObject::IGAME_CAMERA: 
        //SyncGameNodeCamera(forceUpdate, node, go);
        break;
	case IGameObject::IGAME_HELPER: 
        //SyncGameNodeDummy(forceUpdate, node, go);
        break;
	case IGameObject::IGAME_BONE: 
        //SyncGameNodeMesh(forceUpdate, node, go);
        break;
	//case IGameObject::IGAME_SPLINE: break;
	//case IGameObject::IGAME_IKCHAIN: break;
    }
}

void CMaxPipeline::SyncGameNodeMesh(bool forceUpdate, IGameNode* node, IGameMesh* gm)
{
    MeshTrackers::iterator trk = Meshes.find(node->GetNodeID());
    
    if (trk == Meshes.end())
    {
        ComPtr<IDXCCMesh> Mesh;

        if (SUCCEEDED(CreateMesh(node, gm, Mesh)))
        {
            Meshes.insert(Meshes.begin(), MeshTrackers::value_type(node->GetNodeID(), MeshTracker(Mesh)));
        }
    }
    else
    {
        MeshTracker& mt = trk->second;

        if (forceUpdate || mt.IsExtStale())
        {
            UpdateGameNodeMesh(mt);
        }
    }
}

HRESULT CMaxPipeline::CreateMesh(IGameNode* node, IGameMesh* gm, IDXCCMesh** ext)
{
    assert(gm);
    assert(node);
    assert(ext);

    const bool captureSkin = false; // disable skinning, it is not prime-time yet.

    gm->SetUseWeightedNormals();
    gm->SetCreateOptimizedNormalList();

    if (!gm->InitializeData())
    {
        return E_OUTOFMEMORY;
    }

    ComPtr<IDXCCDeclaration> decl;
    bool hasNormals, hasBinormals, hasTangents, hasColor0Verts, hasColor1Verts, hasTexVerts;
    int numTCSet = 0;
    char tcBuf[64];

    if (FAILED(DXCCCreateDeclaration(decl)))
    {
        return E_FAIL;
    }

    _V(decl->InsertPositionElement(decl->NumElements()));

    if (hasNormals = (gm->GetNumberOfNormals() != 0))
        _V(decl->InsertNormalElement(decl->NumElements()));

    if (hasColor0Verts = (gm->GetNumberOfColorVerts() != 0))
        _V(decl->InsertDiffuseElement(decl->NumElements()));

    if (hasColor1Verts = (gm->GetNumberOfIllumVerts() != 0))
        _V(decl->InsertSpecularElement(decl->NumElements()));

    if (hasTexVerts = (gm->GetNumberOfTexVerts() != 0))
        _V(decl->InsertTexcoordElement(decl->NumElements(), numTCSet++, 2, "TexVerts"));

    for (int tc=0; tc<gm->GetActiveMapChannelNum().Count(); tc++)
    {
        StringCchPrintfA(tcBuf, sizeof(tcBuf), "Map %d", tc+1);
        _V(decl->InsertTexcoordElement(decl->NumElements(), numTCSet++, 3, tcBuf));
    }

    if (hasBinormals = (gm->GetNumberOfBinormals() != 0))
        _V(decl->InsertBinormalElement(decl->NumElements()));

    if (hasTangents = (gm->GetNumberOfTangents() != 0))
        _V(decl->InsertTangentElement(decl->NumElements()));

    HRESULT hr = S_OK;

    ComPtr<IDirect3DDevice9> Device;
	AccessEngine()->GetD3DDevice(Device);

    int numFaces = gm->GetNumberOfFaces();

    ComPtr<IDXCCMesh> mesh;

    if (SUCCEEDED(hr = DXCCCreateMesh(numFaces, numFaces*3, Device, decl->GetElements(), mesh)))
    {
        ComPtr<IDXCCResource> res;

        if (SUCCEEDED(hr = AccessManager()->CreateResource(mesh, IID_IDXCCMesh, TRUE, res)))
        {
            char nameBuf[256];
            MakeExportableName(node->GetName(), nameBuf, sizeof(nameBuf)/sizeof(nameBuf[0]));

            _V(res->SetName(nameBuf));

            FrameTrackers::iterator ft = Frames.find(node->GetNodeID());

            ComPtr<IDXCCFrame> frame;

            if (ft != Frames.end())
            {
                frame = ft->second.Item;
            }

            bool isInstance = false; // TODO:  how do we do this?
            IGameSkin* gs = captureSkin? gm->GetIGameSkin() : NULL;
            DWORD numBones = 0;
            NodeIndexMap bonemap;   

	        if (captureSkin && gs)
	        {
                numBones = BuildBoneMap(gs, bonemap);
		        _V(mesh->CreateSkin(numBones));
	        }
	        else
	        {
		        _V(mesh->RemoveSkin());
	        }


            int maxMtls = 0;
            _V(mesh->SetAttributeBoundMaterial(0, &DefaultMaterial));

            if (node->GetNodeMaterial())
            {
                Mtl* mtl = node->GetNodeMaterial()->GetMaxMaterial();
                if (mtl)
                {
                    if (mtl->IsMultiMtl())
                    {
                        maxMtls = mtl->NumSubMtls();
                        for (int m=0; m<maxMtls; m++)
                        {
                            Mtl* smtl = mtl->GetSubMtl(m);

                            SyncMaterial(false, node, smtl);
                            _V(mesh->SetAttributeBoundShader(m, FindMaterialShader(smtl->GetName().data())));
                            _V(mesh->SetAttributeBoundMaterial(m, FindMaterialValues(smtl->GetName().data())));
                        }
                    }
                    else
                    {
                        maxMtls = 1;
                        SyncMaterial(false, node, mtl);
                        _V(mesh->SetAttributeBoundShader(0, FindMaterialShader(mtl->GetName().data())));
                        _V(mesh->SetAttributeBoundMaterial(0, FindMaterialValues(mtl->GetName().data())));
                    }
                }
            }

            VertexSplitMap splits;

            if (SUCCEEDED(hr = mesh->LockFaces()))
            {
                ComPtr<IDXCCVertexBundler> mdata;

                if (SUCCEEDED(hr = mesh->GetVertexBundler(mdata)) && SUCCEEDED(hr = mdata->LockAll()))
                {
                	DXCCFLOAT4 f4;

                    splits.SetNumHostVerts(gm->GetNumberOfVerts());

                    for (int f=0; f<numFaces; f++)
                    {
                        FaceEx* face = gm->GetFace(f);
                        
                        Point3 faceNormal = gm->GetNormal(f);

                        // TODO: swap verts 1 and 2 if necessary (det and dot)

                        for (int v=0; v<3; v++)
                        {
                            int outVert = splits.NumTotalSplitVerts();

                            Point3 p = gm->GetVertex(face->vert[v], true);
                            _V(mdata->SetPosition(outVert, f4.Encode(&p.x, 3)));

                            if (hasNormals)
                            {
                                Point3 n = gm->GetNormal(face->norm[v]);
                                _V(mdata->SetNormal(outVert, f4.Encode(&n.x, 3)));
                            }

                            if (hasBinormals)
                            {
                                Point3 bn = gm->GetBinormal(face->norm[v]);
                                _V(mdata->SetBinormal(outVert, f4.Encode(&bn.x, 3)));
                            }

                            if (hasTangents)
                            {
                                Point3 tn = gm->GetTangent(face->norm[v]);
                                _V(mdata->SetTangent(outVert, f4.Encode(&tn.x, 3)));
                            }

                            if (hasColor0Verts)
                            {
                                Point3 c = gm->GetColorVertex(face->color[v]);
                                f4.Encode(&c.x, 3);
                                f4.value[3] = gm->GetAlphaVertex(face->alpha[v]);

                                _V(mdata->SetDiffuse(outVert, &f4));
                            }
                            if (hasColor1Verts)
                            {
                                float c = gm->GetIllumVertex(face->color[v]);
                                f4.value[0] = c;
                                f4.value[1] = c;
                                f4.value[2] = c;
                                f4.value[3] = 1.0f;

                                _V(mdata->SetSpecular(outVert, &f4));
                            }

                            if (hasTexVerts)
                            {
                                Point2 uv = gm->GetTexVertex(face->texCoord[v]);
                                _V(mdata->SetTexcoord(outVert, 0, f4.Encode(&uv.x, 2)));
                            }

                            if (tc > 1)
                            {
                                Tab<int> maps = gm->GetActiveMapChannelNum();

                                for (int tc=0; tc<maps.Count(); tc++)
                                {
                                    Point3 uvw = gm->GetMapVertex(maps[tc], face->texCoord[v]);
                                    _V(mdata->SetTexcoord(outVert, tc+1, f4.Encode(&uvw.x, 3)));
                                }
                            }




                            _V(mesh->SetFaceVertex(f, v, outVert));
                            
                            splits.AddSplitVert(face->vert[v]); 
                        }

                        _V(mesh->SetFaceAttribute(f, maxMtls? face->matID % maxMtls : 0));
                    }

                    mdata->UnlockAll();
                }

                mesh->UnlockFaces();
            }

            if (SUCCEEDED(hr))
            {
                if (captureSkin && gs)
                {
                    D3DXMATRIX meshMat;

                    mesh->SetSkinParent(frame); 

                    NodeIndexMap::iterator bbeg = bonemap.begin();
                    NodeIndexMap::iterator bend = bonemap.end();
                    NodeIndexMap::iterator bi;

                    for (bi=bbeg; bi!=bend; ++bi)
                    {
                        int boneId = bi->first;
                        INode* bone = MaxCore->GetINodeByHandle(boneId);

                        int boneIdx = bi->second;

                        Dpf("Bone %d: %s", boneIdx, bone->GetName());

                        FrameTrackers::iterator ft = Frames.find(boneId);    // find bone's frame tracker by id
                        if (ft == Frames.end())
                        {
                            SyncGameNode(false, Scene->GetIGameNode(boneId));
                            ft = Frames.find(boneId);
                        }

                        if (ft != Frames.end())
                        {
                            mesh->SetBoneFrame(boneIdx, ft->second.Item, TRUE);     // set i'th bone's frame based on the bonemap
                        }
                    }

                    int numSkinVerts = gs->GetNumOfSkinnedVerts();
                    for (int sv=0; sv<numSkinVerts; sv++)
                    {
                        bool isRigid = (gs->GetVertexType(sv) == IGameSkin::IGAME_RIGID);

                        int numBones = isRigid? 1 : gs->GetNumberOfBones(sv);

                        for (int b=0; b<numBones; b++)
                        {
                            int nodeId = gs->GetBoneID(sv, b);
                            int boneIdx = bonemap[nodeId];

                            float weight = isRigid? 1.0f : gs->GetWeight(sv, b);

                            UINT vi;
                            for(UINT v=splits.GetFirstSplitVert(sv, &vi); v!=VertexSplitMap::LastSplitMarker; v=splits.GetNextSplitVert(&vi))
                            {
                                mesh->SetBoneInfluence(boneIdx, v, weight);
                            }
                        }
                    }

                    Dpf("SkinVerts %d", numSkinVerts);
                }

                _V(mesh->ConvertPointRepsToAdjacency((const DWORD*)splits.GetPointReps()))

				D3DXWELDEPSILONS epsilon;
				ZeroMemory(&epsilon, sizeof(epsilon));
				epsilon.Binormal = 1.0e-6f;
				epsilon.Tangent = 1.0e-6f;

        	    _V(mesh->RemoveDuplicateVertices(&epsilon, NULL, NULL));

        	    _V(mesh->Optimize(D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_VERTEXCACHE, NULL, NULL));

                frame->AddMember(mesh);

                *ext = mesh;
                mesh->AddRef();
                hr = S_OK;
            }
        }
    }

    return hr;
}


void CMaxPipeline::UpdateGameNodeMesh(MeshTracker& ft)
{
}



void CMaxPipeline::UntrackNodeMesh(INode* node)
{
    MeshTrackers::iterator ft = Meshes.find(node->GetHandle());
    UntrackNodeMesh(ft);
}

void CMaxPipeline::UntrackNodeMesh(MeshTrackers::iterator ft)
{
    if (ft != Meshes.end())
    {
        ComPtr<IDXCCResource> res;
        if (SUCCEEDED(AccessManager()->FindResourceByPointer(ft->second.Item, NULL, res)))
        {
            AccessManager()->RemoveResource(res->GetHandle());
        }

        Meshes.erase(ft);
    }
}

void CMaxPipeline::SyncMaterial(bool forceUpdate, IGameNode* node, Mtl* mtl)
{
    TSTR& name = mtl->GetName();
    MaterialTrackers::iterator trk = FindMaterial(name);
    
    if (trk == Materials.end())
    {
        MaxMaterial Material;

        if (SUCCEEDED(CreateMaterial(node, mtl, &Material)))
        {
            TCHAR key[255];
            StringCchCopy(key, 255, mtl->GetName().data());            
            _tcslwr(key);

            Materials.insert(Materials.begin(), MaterialTrackers::value_type(key, MaterialTracker(Material)));
        }
    }
    else
    {
        MaterialTracker& mt = trk->second;

        if (forceUpdate || mt.IsExtStale())
        {
            UpdateMaterial(mt);
        }
    }
}

HRESULT CMaxPipeline::CreateMaterial(IGameNode* node, Mtl* mtl, MaxMaterial* out)
{
    HRESULT hr = E_FAIL;

    if (mtl->ClassID() == DXMATERIAL_CLASS_ID)
    {
        IDxMaterial* dxMtl = (IDxMaterial*)mtl->GetInterface(IDXMATERIAL_INTERFACE);
        if (dxMtl)
        {
            IParamBlock2* pb2;
            ParamID pid;

            if (FindMaterialParameter(mtl, "effectFile", &pb2, &pid))
            {
                TCHAR* effectFile;
                Interval iv;

                if (pb2->GetValue(pid, 0, effectFile, iv))
                {
                    //ComPtr<ID3DXEffect> effect;
                    ComPtr<IDXCCShaderProperties> shader;
                    ComPtr<ID3DXBuffer> errors;
                    //ComPtr<IDirect3DDevice9> d3d;

                    //AccessEngine()->GetD3DDevice(d3d);

                    DXCCCreateShaderProperties( shader );

					ComPtr<IDXCCPropertyBag> pShaderBag;
					shader->GetProperties(pShaderBag);

                    //if (SUCCEEDED(hr = D3DXCreateEffectFromFile(d3d, effectFile, NULL, NULL, 0, NULL, effect, errors)))
                    {
                        ComPtr<IDXCCResource> res;

                       //if (SUCCEEDED(hr = AccessManager()->CreateResource(shader, IID_IDXCCShaderProperties, TRUE, res)))
                       //if (SUCCEEDED(hr = AccessManager()->CreateResource(effect, IID_ID3DXEffect, TRUE, res)))
                       {
                            char nameBuf[256];
                            MakeExportableName(mtl->GetName().data(), nameBuf, sizeof(nameBuf)/sizeof(nameBuf[0]));

                            DXCCPROPERTY_KEY key;
                            pShaderBag->AddProperty( "DXCCResourcePath", &key );
                            pShaderBag->SetPropertyValueAsData(key, DXCCPD_STRING, nameBuf );
                            //if (SUCCEEDED(hr = res->SetName(nameBuf)) && SUCCEEDED(hr = res->SetResourcePath(effectFile)))
                            {
                                IParamBlock2* fxParams = mtl->GetParamBlock(0);
                                for (int p=0; p<fxParams->NumParams(); p++)
                                {
                                    SetShaderFromParamBlock( shader/*effect*/, fxParams, fxParams->IndextoID(p) );
                                }

                                out->Shader= shader;//Effect = effect;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
    {
        StdMat* stdMat = (StdMat*)mtl;

        Color amb = stdMat->GetAmbient(0);
        Color dif = stdMat->GetDiffuse(0);
        Color spc = stdMat->GetSpecular(0);
        float pow = stdMat->GetShininess(0);
        Color emm = stdMat->GetSelfIllumColor(0);
        float alpha = stdMat->GetOpacity(0);

        TCHAR* texFile = NULL;
        Texmap* diftex = stdMat->GetSubTexmap(ID_DI);

        if (diftex && (diftex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00)))
        {
            BitmapTex* bmp = (BitmapTex*)diftex;
            texFile = bmp->GetMapName();
        }

        D3DXMATERIAL d3dMat =
        {
            {
                { dif.r, dif.g, dif.b, alpha },
                { amb.r, amb.g, amb.b, 1.0f },
                { spc.r, spc.g, spc.b, 1.0f },
                { emm.r, emm.g, emm.b, 1.0f },
                pow
            },
            NULL
        };

        memcpy(&out->Material, &d3dMat, sizeof(out->Material));
        if (texFile)
        {
            StringCchCopy(out->TextureName, MAX_PATH, texFile);
            out->Material.pTextureFilename = texFile;
        }

        hr = S_OK;
    }

    return hr;
}

void CMaxPipeline::UpdateMaterial(MaterialTracker& ft)
{
}

void CMaxPipeline::UntrackMaterial(const TCHAR* materialName)
{
    UntrackMaterial(FindMaterial(materialName));
}

void CMaxPipeline::UntrackMaterial(MaterialTrackers::iterator ft)
{
    if (ft != Materials.end())
    {
        ComPtr<IDXCCResource> res;

        if (SUCCEEDED(AccessManager()->FindResourceByPointer(ft->second.Item.Shader /*Effect*/, NULL, res)))
        {
            AccessManager()->RemoveResource(res->GetHandle());
        }

        Materials.erase(ft);
    }
}

CMaxPipeline::MaterialTrackers::iterator CMaxPipeline::FindMaterial(const TCHAR* materialName)
{
    TCHAR key[255]; 
    StringCchCopy(key, 255, materialName);
    _tcslwr(key);

    return Materials.find(key);
}

LPDXCCSHADERPROPERTIES /*ID3DXEffect* */ CMaxPipeline::FindMaterialShader(const TCHAR* materialName)
{
    MaterialTrackers::iterator mt = FindMaterial(materialName);
    if (mt != Materials.end())
    {
        return mt->second.Item.Shader;//Effect;
    }
    else
    {
        return NULL;
    }
}

D3DXMATERIAL* CMaxPipeline::FindMaterialValues(const TCHAR* materialName)
{
    MaterialTrackers::iterator mt = FindMaterial(materialName);
    if (mt != Materials.end())
    {
        return &(mt->second.Item.Material);
    }
    else
    {
        return NULL;
    }
}

bool CMaxPipeline::SetShaderFromParamBlock(LPDXCCSHADERPROPERTIES shader, /*ID3DXEffect* fx,*/ IParamBlock2* pb, ParamID pid)
{
    ParamDef& def = pb->GetParamDef(pid);

	ComPtr<IDXCCPropertyBag> pShaderBag;
	shader->GetProperties(pShaderBag);

    /*
	D3DXHANDLE ph = fx->GetParameterByName(0, def.int_name);
    if (ph == NULL)
    {
        return false;
    }
	*/

    Dpf("%d %s (%s)", pid, def.int_name, ParamTypeName(def.type));

    Interval iv;

    switch (def.type)
    {
    case TYPE_FLOAT: //  "float";
    case TYPE_ANGLE: //  "angle";
	case TYPE_PCNT_FRAC: //  "pcntfrac";
	case TYPE_WORLD: //  "world";
	case TYPE_COLOR_CHANNEL: //  "colorchannel";
	case TYPE_TIMEVALUE: //  "timevalue";
        {
            float v;
            if (pb->GetValue(pid, 0, v, iv))
            {
                DXCCPROPERTY_KEY key;
                pShaderBag->AddProperty( def.int_name , &key);
                pShaderBag->SetPropertyValueAsData( key, DXCCPD_FLOAT, &v);
                //_V(fx->SetFloat(ph, v));
                return true;
            }
        }
        break;

    case TYPE_INT: //  "int";
	case TYPE_INDEX: //  "index";
	case TYPE_RADIOBTN_INDEX: //  "radiobuttonindex";
        {
            int v;
            if (pb->GetValue(pid, 0, v, iv))
            {
                DXCCPROPERTY_KEY key;
                pShaderBag->AddProperty( def.int_name , &key);
                pShaderBag->SetPropertyValueAsData( key, DXCCPD_INT, &v);
                //_V(fx->SetInt(ph, v));
                return true;
            }
        }
        break;

    case TYPE_RGBA: //  "rgba";
    case TYPE_POINT3: //  "point3";
    case TYPE_HSV: //  "hsv";
        {
            Point3 v;
            if (pb->GetValue(pid, 0, v, iv))
            {
                DXCCPROPERTY_KEY key;
                pShaderBag->AddProperty( def.int_name , &key);
                pShaderBag->SetPropertyValueAsData( key, DXCCPD_FLOAT3, &v);
                //_V(fx->SetFloatArray(ph, &v.x, 3));
                return true;
            }
        }
        break;

    case TYPE_POINT4: //  "point4";
	case TYPE_FRGBA: //  "frgba";
        {
            Point4 v;
            if (pb->GetValue(pid, 0, v, iv))
            {
                DXCCPROPERTY_KEY key;
                pShaderBag->AddProperty( def.int_name , &key);
                pShaderBag->SetPropertyValueAsData( key, DXCCPD_FLOAT4, &v);
                //_V(fx->SetFloatArray(ph, &v.x, 4));
                return true;
            }
        }
        break;

    case TYPE_BOOL: //  "bool";
        {
            int v;
            if (pb->GetValue(pid, 0, v, iv))
            {
                DXCCPROPERTY_KEY key;
                pShaderBag->AddProperty( def.int_name , &key);
                pShaderBag->SetPropertyValueAsData( key, DXCCPD_BOOL, &v);
               // _V(fx->SetBool(ph, v));
                return true;
            }
        }
        break;

	case TYPE_MATRIX3: //  "matrix3";
        {
            Matrix3 m;
            if (pb->GetValue(pid, 0, m, iv))
            {
                GMatrix gm(m);
                D3DXMATRIX v;
                ConvertGMatrix(v, gm);

                DXCCPROPERTY_KEY key;
                pShaderBag->AddProperty( def.int_name , &key);
                pShaderBag->SetPropertyValueAsData( key, DXCCPD_FLOAT4X4, &v);
                //_V(fx->SetFloatArray(ph, (const FLOAT*)&v, 16));
                return true;
            }
        }
        break;

    case TYPE_BITMAP: //  "bitmap"; // todo
        {
            PBBitmap* v;
            if (pb->GetValue(pid, 0, v, iv))
            {
                
                DXCCPROPERTY_KEY key;
                pShaderBag->AddProperty( def.int_name , &key);
                pShaderBag->SetPropertyValueAsData( key, DXCCPD_STRING, v->bi.Name() );
                /*
                SyncTexture(false, v);
                TextureTrackers::iterator trk = FindTexture(v->bi.Name());
                if (trk != Textures.end())
                {
                    _V(fx->SetTexture(ph, trk->second.Item));
                }
                */
                return true;
            }
        }
        break;

    case TYPE_FILENAME: //  "filename";
    case TYPE_STRING: //  "string";
        {
            TCHAR* v;
            if (pb->GetValue(pid, 0, v, iv))
            {
                DXCCPROPERTY_KEY key;
                pShaderBag->AddProperty( def.int_name , &key);
                pShaderBag->SetPropertyValueAsData( key, DXCCPD_STRING, v );
                //_V(fx->SetString(ph, v));
                return true;
            }
        }
        break;


    //case TYPE_FLOAT_TAB: //  "floattab";
	//case TYPE_INT_TAB: //  "inttab";
	//case TYPE_RGBA_TAB: //  "rgbatab";
	//case TYPE_POINT3_TAB: //  "point3tab";
	//case TYPE_BOOL_TAB: //  "booltab";
	//case TYPE_ANGLE_TAB: //  "angletab";
	//case TYPE_PCNT_FRAC_TAB: //  "pcntfractab";
	//case TYPE_WORLD_TAB: //  "worldtab";
	//case TYPE_STRING_TAB: //  "stringtab";
	//case TYPE_FILENAME_TAB: //  "filenametab";
	//case TYPE_HSV_TAB: //  "hsvtab";
	//case TYPE_COLOR_CHANNEL_TAB: //  "colorchanneltab";
	//case TYPE_TIMEVALUE_TAB: //  "timevaluetab";
	//case TYPE_RADIOBTN_INDEX_TAB: //  "radiobuttonindextab";
	//case TYPE_BITMAP_TAB: //  "bitmaptab";
	//case TYPE_INDEX_TAB: //  "indextab";
	//case TYPE_MATRIX3_TAB: //  "matrix3tab";
	//case TYPE_POINT4_TAB: //  "point4tab";
	//case TYPE_FRGBA_TAB: //  "frgbatab";
    }

    return false;
}


void CMaxPipeline::SyncTexture(bool forceUpdate, PBBitmap* bmp)
{
    const TCHAR* name = bmp->bi.Name();
    TextureTrackers::iterator trk = FindTexture(name);
    
    if (trk == Textures.end())
    {
        ComPtr<IDirect3DBaseTexture9> Texture;

        if (SUCCEEDED(CreateTexture(bmp, Texture)))
        {
            TCHAR key[255];
            StringCchCopy(key, 255, name);            
            _tcslwr(key);

            Textures.insert(Textures.begin(), TextureTrackers::value_type(key, TextureTracker(Texture)));
        }
    }
    else
    {
        TextureTracker& mt = trk->second;

        if (forceUpdate || mt.IsExtStale())
        {
            UpdateTexture(mt);
        }
    }
}

HRESULT CMaxPipeline::CreateTexture(PBBitmap* bmp, IDirect3DBaseTexture9** otex)
{
    HRESULT hr = S_OK;

    const TCHAR* fileName = bmp->bi.Name();
    D3DXIMAGE_INFO info;
    ComPtr<IDirect3DDevice9> d3d;
    ComPtr<IDirect3DTexture9> tex2d;
    ComPtr<IDirect3DVolumeTexture9> tex3d;
    ComPtr<IDirect3DCubeTexture9> texcube;

    AccessEngine()->GetD3DDevice(d3d);

    if (SUCCEEDED(hr = D3DXGetImageInfoFromFileA(fileName, &info)))
    {
        ComPtr<IDXCCResource> res;

	    switch (info.ResourceType)
	    {

	    case D3DRTYPE_TEXTURE:
            if (SUCCEEDED(hr = D3DXCreateTextureFromFileA(d3d, fileName, tex2d)))
		    {
			    if (SUCCEEDED(hr = AccessManager()->CreateResource(tex2d, IID_IDirect3DBaseTexture9, TRUE, res)))
                {
                    *otex = tex2d;
                    tex2d->AddRef();

    			    _V(hr = res->SetResourcePath(fileName));
                }
		    }
		    break;

        case D3DRTYPE_VOLUMETEXTURE:
            if (SUCCEEDED(hr = D3DXCreateVolumeTextureFromFileA(d3d, fileName, tex3d)))
		    {
			    if (SUCCEEDED(hr = AccessManager()->CreateResource(tex3d, IID_IDirect3DBaseTexture9, TRUE, res)))
                {
                    *otex = tex3d;
                    tex3d->AddRef();

                    _V(hr = res->SetResourcePath(fileName));
                }
		    }
		    break;

        case D3DRTYPE_CUBETEXTURE:
            if (SUCCEEDED(hr = D3DXCreateCubeTextureFromFileA(d3d, fileName, texcube)))
		    {
			    if (SUCCEEDED(hr = AccessManager()->CreateResource(texcube, IID_IDirect3DBaseTexture9, TRUE, res)))
                {
                    *otex = texcube;
                    texcube->AddRef();

                    _V(hr = res->SetResourcePath(fileName));
                }
		    }
		    break;
	    
        }
    }

    return hr;
}

void CMaxPipeline::UpdateTexture(TextureTracker& ft)
{
}

void CMaxPipeline::UntrackTexture(const TCHAR* textureName)
{
    UntrackTexture(FindTexture(textureName));
}

void CMaxPipeline::UntrackTexture(TextureTrackers::iterator tt)
{
    if (tt != Textures.end())
    {
        ComPtr<IDXCCResource> res;

        if (SUCCEEDED(AccessManager()->FindResourceByPointer(tt->second.Item, NULL, res)))
        {
            AccessManager()->RemoveResource(res->GetHandle());
        }      
    
        Textures.erase(tt);
    }
}

CMaxPipeline::TextureTrackers::iterator CMaxPipeline::FindTexture(const TCHAR* fileName)
{
    TCHAR key[255]; 
    StringCchCopy(key, 255, fileName);
    _tcslwr(key);

    return Textures.find(key);
}


void CMaxPipeline::RegisterCallbacks()
{
    RegisterNotification(CMaxPipeline::OnMaxSystemPostReset, this, NOTIFY_SYSTEM_POST_RESET);
    RegisterNotification(CMaxPipeline::OnMaxSystemPostNew, this, NOTIFY_SYSTEM_POST_NEW);
    RegisterNotification(CMaxPipeline::OnMaxFilePostOpen, this, NOTIFY_FILE_POST_OPEN);
    RegisterNotification(CMaxPipeline::OnMaxFilePostMerge, this, NOTIFY_FILE_POST_MERGE);
    RegisterNotification(CMaxPipeline::OnMaxBitmapChanged, this, NOTIFY_BITMAP_CHANGED);
    RegisterNotification(CMaxPipeline::OnMaxPostImport, this, NOTIFY_POST_IMPORT);

    RegisterNotification(CMaxPipeline::OnMaxNodeRenamed, this, NOTIFY_NODE_RENAMED);
    RegisterNotification(CMaxPipeline::OnMaxNodeCreated, this, NOTIFY_NODE_CREATED);
    RegisterNotification(CMaxPipeline::OnMaxNodeLinkedOrUnlinked, this, NOTIFY_NODE_LINKED);
    RegisterNotification(CMaxPipeline::OnMaxNodeLinkedOrUnlinked, this, NOTIFY_NODE_UNLINKED);
    RegisterNotification(CMaxPipeline::OnMaxNodePostMtl, this, NOTIFY_NODE_POST_MTL);
    RegisterNotification(CMaxPipeline::OnMaxNodesPreDelete, this, NOTIFY_SEL_NODES_PRE_DELETE);

    RegisterGlobalReference(&RefTracker);
}

void CMaxPipeline::UnregisterCallbacks()
{
    UnRegisterGlobalReference(&RefTracker);

    UnRegisterNotification(CMaxPipeline::OnMaxSystemPostReset, this, NOTIFY_SYSTEM_POST_RESET);
    UnRegisterNotification(CMaxPipeline::OnMaxSystemPostNew, this, NOTIFY_SYSTEM_POST_NEW);
    UnRegisterNotification(CMaxPipeline::OnMaxFilePostOpen, this, NOTIFY_FILE_POST_OPEN);
    UnRegisterNotification(CMaxPipeline::OnMaxFilePostMerge, this, NOTIFY_FILE_POST_MERGE);
    UnRegisterNotification(CMaxPipeline::OnMaxBitmapChanged, this, NOTIFY_BITMAP_CHANGED);
    UnRegisterNotification(CMaxPipeline::OnMaxPostImport, this, NOTIFY_POST_IMPORT);

    UnRegisterNotification(CMaxPipeline::OnMaxNodeRenamed, this, NOTIFY_NODE_RENAMED);
    UnRegisterNotification(CMaxPipeline::OnMaxNodeCreated, this, NOTIFY_NODE_CREATED);
    UnRegisterNotification(CMaxPipeline::OnMaxNodeLinkedOrUnlinked, this, NOTIFY_NODE_LINKED);
    UnRegisterNotification(CMaxPipeline::OnMaxNodeLinkedOrUnlinked, this, NOTIFY_NODE_UNLINKED);
    UnRegisterNotification(CMaxPipeline::OnMaxNodePostMtl, this, NOTIFY_NODE_POST_MTL);
    UnRegisterNotification(CMaxPipeline::OnMaxNodesPreDelete, this, NOTIFY_SEL_NODES_PRE_DELETE);
}

void CMaxPipeline::OnMaxSystemPostReset(void *param, NotifyInfo *info)	    //NOTIFY_SYSTEM_POST_RESET
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    mp->ResetData();
}

void CMaxPipeline::OnMaxSystemPostNew(void *param, NotifyInfo *info)		//NOTIFY_SYSTEM_POST_NEW
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    mp->ResetData();
}

void CMaxPipeline::OnMaxFilePostOpen(void *param, NotifyInfo *info)		//NOTIFY_FILE_POST_OPEN
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    mp->ResetData();
    mp->SyncData(false, false);
}

void CMaxPipeline::OnMaxFilePostMerge(void *param, NotifyInfo *info)		//NOTIFY_FILE_POST_MERGE
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    mp->ResetData();
    mp->SyncData(false, false);
}

void CMaxPipeline::OnMaxPostImport(void *param, NotifyInfo *info)			//NOTIFY_POST_IMPORT
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    mp->ResetData();
    mp->SyncData(false, false);
}

void CMaxPipeline::OnMaxNodeRenamed(void *param, NotifyInfo *info)		    //NOTIFY_NODE_RENAMED
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    struct RenameData
    {
        TCHAR* OldName;
        TCHAR* NewName;
    } * data = (RenameData*)info->callParam;

    mp->BeginSync();
    mp->UpdateNodeFrameName(mp->MaxCore->GetINodeByName(data->NewName));
    mp->EndSync();
}

void CMaxPipeline::OnMaxNodeCreated(void *param, NotifyInfo *info)		    //NOTIFY_NODE_CREATED
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    INode* node = (INode*)info->callParam;

    mp->BeginSync(node);
    mp->SyncGameNodeTree(false, mp->Scene->GetIGameNode(node));
    mp->EndSync();
}

void CMaxPipeline::OnMaxNodeLinkedOrUnlinked(void *param, NotifyInfo *info)			//NOTIFY_NODE_LINKED, NOTIFY_NODE_UNLINKED
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    INode* node = (INode*)info->callParam;

    mp->BeginSync();
    mp->UpdateNodeFrameParent(node);
    mp->EndSync();
}

void CMaxPipeline::OnMaxNodePostMtl(void *param, NotifyInfo *info)		    //NOTIFY_NODE_POST_MTL
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    INode* node = (INode*)info->callParam;
}

void CMaxPipeline::OnMaxNodesPreDelete(void *param, NotifyInfo *info)	    //NOTIFY_SEL_NODES_POST_DELETE
{
    CMaxPipeline* mp = (CMaxPipeline*)param;
    Tab<INode*>* nodes = (Tab<INode*>*)info->callParam;

    mp->BeginSync();

    for (int c=0; c<nodes->Count(); c++)
    {
        mp->UntrackNode((*nodes)[c]);
    }

    mp->EndSync();
}


void CMaxPipeline::OnMaxBitmapChanged(void *param, NotifyInfo *info)		//NOTIFY_BITMAP_CHANGED	
{
    const TCHAR* fileName = (const TCHAR*)info->callParam;
}

