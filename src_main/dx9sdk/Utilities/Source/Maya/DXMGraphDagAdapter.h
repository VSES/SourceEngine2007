#pragma once
#ifndef DXMGRAPHDAGADAPTER_H
#define DXMGRAPHDAGADAPTER_H

/*
class DXMNode;
class DXMNodeAdapter;

class DXMGraph
class DXMGraphAdapter;


class DXMNodeDagAdapter;
*/

class DXMRoute;

class DXMGraphDagAdapter : public DXMGraphAdapter
{
public:
	DXMGraphDagAdapter();
	virtual ~DXMGraphDagAdapter();

	virtual void Initialize(DXMGraph* owner);
	virtual void SetCallbackState(bool Activate);


	virtual bool IsInterested( MObject& node );

	virtual UINT Synchronize();

	virtual void OnNodeAdded( DXMNode* node );
	virtual void OnNodeRemoved( DXMNode* node );

	DXMRoute* FindRoute(MDagPath& path);
	DXMRoute* AddRoute(MDagPath& path);
	void RemoveRoute(DXMRoute* route, bool deleteIt);
	void RemoveRoute(MDagPath& path, bool deleteIt);


	//FOR MFn::kWorld
	virtual void OnChildAddedToWorld( MDagPath &child);
	static void DispatchChildAddedToWorld( MDagPath &child, MDagPath &parent, void * clientData );
	//FOR MFn::kWorld
	virtual void OnChildRemovedToWorld( MDagPath &child);
	static void DispatchChildRemovedToWorld( MDagPath &child, MDagPath &parent, void * clientData );



	virtual const CStringA& Signature(){ return MySignature; }

	static CStringA MySignature;
	MCallbackId ChildAddedCID;
	MCallbackId ChildRemovedCID;

};


#endif