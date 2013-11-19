#pragma once
#ifndef FxAttributeCreator_H
#define FxAttributeCreator_H


class FxAttributeCreator : public IDXCCEnumEffectParameters
{
public:
	HRESULT Run(FxInternal* myShader);

	STDMETHOD(PreEnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& parameter, BOOL& GoDeeper, BOOL& DoEnum, BOOL& DoPost);
	STDMETHOD(EnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& parameter, BOOL& GoDeeper, BOOL& DoPost);
	STDMETHOD(PostEnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& parameter);

private:

	FxInternal* Shader;
	int arrayCount;
	MObject RootAttribute;
	CAtlList<MObject> oAttributeStack;
	
};

#endif