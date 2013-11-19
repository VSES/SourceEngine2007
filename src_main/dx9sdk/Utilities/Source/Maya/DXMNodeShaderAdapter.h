#pragma once
#ifndef DXMNODESHADERADAPTER_H
#define DXMNODESHADERADAPTER_H


class DXMNodeShaderAdapter : public DXMNodeAdapter
{
public:
	enum SHADERSYNCFLAGS
	{
		SHADERSYNC_ALL= 1,
	};
	DWORD SyncFlags;



	DXMNodeShaderAdapter();
	virtual ~DXMNodeShaderAdapter();

	virtual const CStringA& Signature(){ return MySignature; }
	virtual const CStringA& OwnerSignature(){ return DXMGraphShaderAdapter::MySignature; }

	static CStringA MySignature;
	
	virtual void Initialize(DXMNode* node);
	virtual void Destroy();
	virtual void SetCallbackState(bool DoCallbacks);
	virtual bool Synchronize();

	LPDXCCRESOURCE Resource;
	LPDXCCSHADERPROPERTIES Shader;
    LPDXCCPROPERTYBAG Properties;//propertybag from Shader->GetProperties;

	void CreateShaderProps();
    void DestroyShaderProps();
	
	virtual void OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug);
	static void DispatchAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData );

protected:

	MCallbackId			AttributeChangedCID;

};






#endif