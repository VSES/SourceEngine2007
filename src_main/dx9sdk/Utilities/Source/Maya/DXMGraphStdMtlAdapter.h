#pragma once
#ifndef DXMGRAPHSTDMTLADAPTER_H
#define DXMGRAPHSTDMTLADAPTER_H

class DXMGraphStdMtlAdapter : public DXMGraphAdapter
{
public:
	DXMGraphStdMtlAdapter() : DXMGraphAdapter() {}

	virtual bool IsInterested( MObject& node );

	virtual void OnNodeAdded( DXMNode* node );
	virtual void OnNodeRemoved( DXMNode* node );

	virtual const CStringA& Signature(){ return MySignature; }

	static CStringA MySignature;
};


#endif