#pragma once
#ifndef DXMGRAPHMESHADAPTER_H
#define DXMGRAPHMESHADAPTER_H

class DXMGraphMeshAdapter : public DXMGraphAdapter
{
public:
	DXMGraphMeshAdapter() : DXMGraphAdapter() {}

	virtual bool IsInterested( MObject& node );

	virtual void OnNodeAdded( DXMNode* node );
	virtual void OnNodeRemoved( DXMNode* node );

	virtual const CStringA& Signature(){ return MySignature; }

	static CStringA MySignature;
};


#endif