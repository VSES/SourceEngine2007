#pragma once
#ifndef DXMGRAPHSHADERADAPTER_H
#define DXMGRAPHSHADERADAPTER_H

class DXMGraphShaderAdapter : public DXMGraphAdapter
{
public:
	DXMGraphShaderAdapter() : DXMGraphAdapter() {}

	virtual bool IsInterested( MObject& node );

	virtual void OnNodeAdded( DXMNode* node );
	virtual void OnNodeRemoved( DXMNode* node );

	virtual const CStringA& Signature(){ return MySignature; }

	static CStringA MySignature;
};


#endif