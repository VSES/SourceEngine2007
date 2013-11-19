#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include <string>

class IncludeHandler9 : public ID3DXInclude
{
public:
    FRIEND_SINGLETON_DIR_H( IncludeHandler9 );

    STDMETHOD(Open)( D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pNumBytes );
    STDMETHOD(Close)( LPCVOID pData );

    std::wstring m_ContentDirectory;
};

DECLARE_SINGLETON_DIR_H( IncludeHandler9 );
