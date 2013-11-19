#pragma once
#ifndef FXSHADER_H
#define FXSHADER_H

class FxInternal;

class DirectXShader : public MPxNode
{
public:

						DirectXShader(){};
	virtual				~DirectXShader(){};
	
	static  void *		creator();
	virtual void		postConstructor(); 

	static  MStatus		initialize();

	virtual MStatus		compute( const MPlug&, MDataBlock& );
    virtual bool		setInternalValue( const MPlug& plug, const MDataHandle& dataIn);
	//virtual bool		getInternalValue( const MPlug& plug, MDataHandle& dataIn);

	//DXCC STATICS
	static MObject  	aLoading;
	static MObject  	aFile;
	static MObject  	aScript;

	//MAYA STATICS
	static MTypeId  	id;// The IFF type id
	static MObject  	aColor;// Surface color
	static MObject  	aColorR;// Red component of surface color
	static MObject  	aColorG;// Green component of surface color
	static MObject  	aColorB;// Blue component of surface color
	static MObject  	aOutColor;// Output color
	static MObject  	aOutColorR;// Red component of output color
	static MObject  	aOutColorG;// Green component of output color
	static MObject  	aOutColorB;// Blue component of output color
	static MObject  	aNormalCamera;// Surface normal
	static MObject  	aNormalCameraX;// X component of surface normal
	static MObject  	aNormalCameraY;// Y component of surface normal
	static MObject  	aNormalCameraZ;// Z component of surface normal

	static MString		AnchorShortName;
	static MString		AnchorLongName;

	static MString		RootLongName;
	static MString		RootShortName;

	static MCallbackId	SceneOpenedCID;
	static MCallbackId	NodeRemovedCID;
	static void			StaticInitialize();
	static void			StaticDestroy();
	static void			DispatchNodeRemoved(MObject & node, void* clientData );

	static void			DispatchSceneOpened(void* clientData );
	static void			LoadLegacySaves();

	static CRBMap<FxInternal*, FxInternal*> ActiveFx;


	static HRESULT CreateShaderProperties(MObject& obj, bool bMakeRes, LPDXCCRESOURCE* ppRes, LPDXCCSHADERPROPERTIES* ppShaderProp);

protected:
	static void Internal_CreateShaderProperties_RecursePlugs(FxInternal* fxInternal, MPlug& plug, LPDXCCPROPERTYBAG pBag);
	static void Internal_CreateShaderProperties_DoPlugExport(FxInternal* fxInternal, const MPlug& plug, D3DXHANDLE& handle, LPDXCCPROPERTYCOLLECTION pCollection, DXCCPROPERTY_KEY key);
};

#endif