#pragma once
#ifndef FxAttributeFiller_H
#define FxAttributeFiller_H

class FxAttributeFiller : public IDXCCEnumEffectParameters
{
public:
	HRESULT Run(FxInternal* myShader, LPDXCCSHADERPROPERTIES pRestoreProps, LPCSTR RestoreMsg);

	STDMETHOD(PreEnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& parameter, BOOL& GoDeeper, BOOL& DoEnum, BOOL& DoPost);
	STDMETHOD(EnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& parameter, BOOL& GoDeeper, BOOL& DoPost);
	STDMETHOD(PostEnumParameter)(THIS_  LPD3DXEFFECT pEffect, DXCCEffectPath& parameter);
private:

	bool FillScript( DXCCEffectPath& parameter, MPlug& plug );
	bool FillPlug(DXCCEffectPath& parameter, MPlug& plug );

	FxInternal* Shader;
	
	CComPtr<IDXCCPropertyBag> Restore;
	
	CStringA Script;

	BOOL SasUiVisible;
	MString SasUiControl;
	MString SasUiLabel;
	float SasUiMin;
	float SasUiMax;
	int SasUiSteps;
};


#endif