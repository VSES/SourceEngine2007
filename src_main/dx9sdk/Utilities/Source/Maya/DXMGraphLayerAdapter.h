#pragma once
#ifndef DXMGRAPHLAYERADAPTER_H
#define DXMGRAPHLAYERADAPTER_H


class DXMGraphLayerAdapter : public DXMGraphAdapter
{
public:
	DXMGraphLayerAdapter();
	virtual ~DXMGraphLayerAdapter();

	virtual void Initialize(DXMGraph* owner);
	virtual void SetCallbackState(bool Activate);


	virtual bool IsInterested( MObject& node );

	virtual UINT Synchronize();

	virtual void OnNodeAdded( DXMNode* node );
	virtual void OnNodeRemoved( DXMNode* node );
	virtual const CStringA& Signature(){ return MySignature; }

	static CStringA MySignature;
};


#endif