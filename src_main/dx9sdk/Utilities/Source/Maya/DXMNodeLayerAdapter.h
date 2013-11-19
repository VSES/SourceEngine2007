#pragma once
#ifndef LAYERNODEADAPTER_H
#define LAYERNODEADAPTER_H


class DXMNodeLayerAdapter : public DXMNodeAdapter
{
	friend DXMGraphLayerAdapter;
	friend DXMGraphAdapter;
public:

	enum DAGSYNCFLAGS
	{
		LAYERSYNC_VISIBILITY= 1,
	};

	DWORD SyncFlags;


	~DXMNodeLayerAdapter();

	virtual bool Synchronize();
	void SyncVisibility();


	virtual void SetCallbackState(bool DoCallbacks);

	virtual void OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug);
	static void DispatchAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData );

	virtual const CStringA& Signature(){ return MySignature; }
	virtual const CStringA& OwnerSignature(){ return DXMGraphLayerAdapter::MySignature; }

	static CStringA MySignature;

protected:
	//USED BY DXMGraphLayerAdapter
	DXMNodeLayerAdapter();
	//USED BY DXMNode

	virtual void Initialize(DXMNode* node);
	virtual void Destroy();

	bool			Invisible;
	MCallbackId		AttributeChangedCID;
};



#endif