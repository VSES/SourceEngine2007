#pragma once
#ifndef DXMROUTE_H
#define DXMROUTE_H



class DXMRoute
{
	friend DXMNodeDagAdapter;
public:
	typedef CRBMap<DXMRoute*, DXMRoute*>  DXMRouteMap;
	typedef CRBMapIterator<DXMRouteMap> DXMRouteIterator;

	IDXCCResource*	Resource;
	IDXCCFrame*		Frame;


	virtual ~DXMRoute();

	virtual bool GetCallbackState(){ return CallbackState; }

	const MDagPath& GetPath() { return Path; }

	DXMNodeDagAdapter* GetNodeDagAdapter() { return NodeDagAdapter; }
	DXMNodeDagAdapter* FindDagNodeAdapter(MObject& obj);

	void AddMember(LPUNKNOWN pUnk);
	void RemoveMember(LPUNKNOWN pUnk);

	virtual void SyncHierarchy();
	virtual void SyncTransform();
	virtual void SyncVisibility();
	virtual void SyncMesh();

	bool IsVisible(){ return Visible; }

	void GatherName();
	void GatherAnimation();

	DXMRouteIterator GetChildRouteIterator();
	DXMRoute* FindChild(const MDagPath& path);
	DXMRoute* GetParentRoute(){ return ParentRoute; }






	virtual void OnChildAdded( MDagPath &child);
	static void DispatchChildAdded( MDagPath &child, MDagPath &parent, void * clientData );

	virtual void OnChildRemoved( MDagPath &child);
	static void DispatchChildRemoved( MDagPath &child, MDagPath &parent, void * clientData );

	//static void DispatchInstanceAdded( MDagPath &child, MDagPath &parent, void * clientData );
	//static void DispatchInstanceRemoved( MDagPath &child, MDagPath &parent, void * clientData );

protected:
	//USED BY DXMNodeDagAdapter
	DXMRoute();
	virtual void Initialize(DXMNodeDagAdapter* nodeDagAdapter, MDagPath & path);
	virtual void Destroy();
	virtual void SetCallbackState(bool Activate);


	//INTERNAL	
	void CreateFrame();
	void DestroyFrame();
	//void UpdateMatrix();
	void Parent(DXMRoute* parent);
	void Unparent();


	
	MDagPath Path;
#ifdef DEBUG
	CStringA Name;
#endif

	DXMNodeDagAdapter* NodeDagAdapter;

	DXMRouteMap ChildRoutes;
	DXMRoute* ParentRoute;
	bool Visible;
	bool CallbackState;
	MCallbackId ChildAddedCID;
	MCallbackId ChildRemovedCID;
	//MCallbackId InstanceAddedCID;
	//MCallbackId InstanceRemovedCID;

};



#endif