#pragma once
#ifndef DAGNNODEADAPTER_H
#define DAGNNODEADAPTER_H


class DXMNodeDagAdapter : public DXMNodeAdapter
{
	friend DXMGraphDagAdapter;
	friend DXMGraphAdapter;
	friend DXMRoute;
public:
	typedef CRBMap<DXMRoute*, DXMRoute*>  DXMRouteMap;
	typedef CRBMapIterator<DXMRouteMap> DXMRouteIterator;

	enum DAGSYNCFLAGS
	{
		DAGSYNC_TRANSFORM= 1,
		DAGSYNC_VISIBILITY_OF_NODE= 2,
		DAGSYNC_VISIBILITY_OF_ROUTES= 4,
		//DAGSYNC_HIERARCHY= 8,
	};

	DWORD SyncFlags;


	~DXMNodeDagAdapter();

	virtual bool Synchronize();
	void SyncVisibilityOfNode();
	void SyncVisibilityOfRoutes();
	void SyncTransform();
	//void SyncHierarchy();
	void SyncMesh();

	bool IsExplicitlyInvisible(){ return Invisible; }
	D3DXMATRIX GetLocalMatrix(){ return LocalMatrix; }

	void AddMemberToRoutes(LPUNKNOWN pUnk);
	void RemoveMemberFromRoutes(LPUNKNOWN pUnk);


	virtual void SetCallbackState(bool DoCallbacks);

	virtual void OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug);
	static void DispatchAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData );

	DXMRouteIterator GetRouteIterator();
	DXMRoute* FindRoute(MDagPath& route);

	virtual const CStringA& Signature(){ return MySignature; }
	virtual const CStringA& OwnerSignature(){ return DXMGraphDagAdapter::MySignature; }

	static CStringA MySignature;

protected:
	//USED BY DXMGraphDagAdapter
	DXMNodeDagAdapter();
	//USED BY DXMNode


	virtual void Initialize(DXMNode* node);
	virtual void Destroy();

	//USED INTERNALLY AND BY DXMRoute
	DXMRoute* AddRoute(MDagPath& route);
	void RemoveRoute(DXMRoute* route, bool DeleteIt);
	void RemoveRoute(MDagPath& path, bool deleteIt);

	virtual DXMRoute* CreateRoute();


	DXMRouteMap		Routes;

	bool			Invisible;
	D3DXMATRIX		LocalMatrix;
	MCallbackId		AttributeChangedCID;
};



#endif