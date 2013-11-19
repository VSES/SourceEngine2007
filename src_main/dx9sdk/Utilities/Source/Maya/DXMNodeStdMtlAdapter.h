#pragma once
#ifndef DXMNODESTDMTLADAPTER_H
#define DXMNODESTDMTLADAPTER_H


class DXMNodeStdMtlAdapter : public DXMNodeAdapter
{
public:
	enum SHADERSYNCFLAGS
	{
		SHADERSYNC_ALL= 1,
	};
	DWORD SyncFlags;



	DXMNodeStdMtlAdapter();
	virtual ~DXMNodeStdMtlAdapter();

	virtual const CStringA& Signature(){ return MySignature; }
	virtual const CStringA& OwnerSignature(){ return DXMGraphStdMtlAdapter::MySignature; }

	static CStringA MySignature;
	
	virtual void Initialize(DXMNode* node);
	virtual void Destroy();
	virtual void SetCallbackState(bool DoCallbacks);
	virtual bool Synchronize();

	//LPDXCCRESOURCE Resource;
	//LPDXCCSHADERPROPERTIES Shader;
    //LPDXCCPROPERTYBAG Properties;//propertybag from Shader->GetProperties;
	D3DXMATERIAL material;
	char texFile[MAX_PATH+1];

	void CreateShaderProps();
    void DestroyShaderProps();
	
	virtual void OnAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug);
	static void DispatchAttributeChanged( MNodeMessage::AttributeMessage msg, MPlug& plug, MPlug& otherPlug, void* clientData );

protected:

	MCallbackId			AttributeChangedCID;

};






#endif