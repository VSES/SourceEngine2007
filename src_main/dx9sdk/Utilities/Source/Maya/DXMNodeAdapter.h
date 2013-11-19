#pragma once
#ifndef DXMNodeADAPTER_H
#define DXMNodeADAPTER_H
/*
class DXMNode;
class DXMGraph;
class DXMGraphAdapter;
*/

//inherit and customize me
class DXMNodeAdapter
{
	friend DXMGraphAdapter;
	friend DXMNode;
public:

	virtual ~DXMNodeAdapter();

	DXMNode* GetNode();
	DXMGraph* GetGraph();
	DXMGraphAdapter* GetOwnerGraphAdapter();

	//entry name into the AdapterMap
	virtual const CStringA& Signature()= 0;
	virtual const CStringA& OwnerSignature()= 0;

	virtual bool GetCallbackState();


	virtual void OnNameChange(const MString & prevName);
	//virtual void OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug);

	//undirty DXMNodeAdapters.
	//return if now synchronized or false is not.
	//do not remove any node adapters from the DXMGraphAdapter::DirtyMap here...
	//do not delete other DXMNodeAdapters here
	virtual bool Synchronize(){ return true; }


protected:
	//USED BY DXMGraphAdapter
	DXMNodeAdapter();
	//USED BY DXMNode
	virtual void Initialize(DXMNode* node);
	virtual void Destroy();
	virtual void SetCallbackState(bool Activate);


	bool CallbackState;
	DXMNode* Node;
};

#endif