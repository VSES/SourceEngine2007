#pragma once
#ifndef DXMNODE_H
#define DXMNODE_H

class DXMNode : public DXMAnchor
{
	friend DXMGraph;
	friend DXMGraphAdapter;
public:
	
	typedef CRBMap<CStringA, DXMNodeAdapter*, CStringElementTraits<CStringA> >  DXMAdapterMap;
	typedef CRBMapIterator<DXMAdapterMap> DXMAdapterIterator;

	virtual ~DXMNode();

	DXMGraph* GetGraph(){ return Graph; }
	DXMNode* FindSiblingNode(MObject& node);

	//MObject GetObject();

	virtual bool GetCallbackState(){ return CallbackState; }

	virtual void OnNameChange(const MString & prevName);
	static void DispatchNameChange( MObject& node, const MString & prevName,  void* clientData );

	//virtual void OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug);
	//static void DispatchAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData );

	DXMNodeAdapter* FindAdapter(const CStringA& signature);
	DXMAdapterIterator GetAdapterIterator();

protected:

	//USED BY DXMGraph
	DXMNode();
	virtual void Initialize(DXMGraph* graph, MObject& node);
	virtual void Destroy();
	virtual void SetCallbackState(bool Activate);

	//USED BY DXMGraphAdapter
	void RemoveAdapter(DXMNodeAdapter* adapter, bool DeleteAdapter);
	void AddAdapter(DXMNodeAdapter* adapter);


	DXMAdapterMap	Adapters;

	DXMGraph*		Graph;



	//MObjectHandle	ObjectHandle;

	bool			CallbackState;
	MCallbackId		NameChangeCID;
	//MCallbackId		AttributeChangedCID;


};



#endif