#pragma once
#ifndef FxInternal_H
#define FxInternal_H

class FxInternal : public DXMAnchor
{
private:

public:
	FxInternal();

	virtual ~FxInternal();


	void Destroy();
	
	LPD3DXEFFECT Effect;

	bool		EffectLoad(LPCSTR filename, LPDXCCSHADERPROPERTIES pRestoreProps, LPCSTR RestoreMsg);
	void		EffectUnload();
	bool		EffectFillPlug(MPlug& plug);

	D3DXHANDLE	DecodeHandle(const CStringA& handleStr);
	MPlug		DecodePlug(const CStringA& plugStr);
	CStringA	EncodePlug(const MPlug& plug);
	CStringA	TranscodeHandleToPlug(const CStringA& plug);
	CStringA	TranscodePlugToHandle(const CStringA& handle);

	MPlug		GetValuePlug(MPlug& plug);

	void		LoadLegacy();

};


MString FxParameterGetUiLabel(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter);
BOOL FxParameterGetUiVisible(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter);
MString FxParameterGetUiControl(LPD3DXEFFECT pEffect, DXCCEffectPath& parameter);

#endif
