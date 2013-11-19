
#ifndef __MaxUtilities_h__
#define __MaxUtilities_h__
#pragma once

D3DXMATRIX& ConvertGMatrix(D3DXMATRIX& dst, const GMatrix& src);
GMatrix& ConvertD3dMatrix(GMatrix& dst, const D3DXMATRIX& src);

const char* MakeExportableName(const char* name, char* buf, int bufCount);

const char* ParamTypeName(int type);
bool FindMaterialParameter(Mtl* mtl, const TCHAR* paramName, IParamBlock2** opb, ParamID* opid);


#endif
