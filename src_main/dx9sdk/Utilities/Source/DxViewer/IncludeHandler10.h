#pragma once
#include "d3d10.h"
#include "d3dx10.h"
#include <string>
#include "macros.h"
class IncludeHandler10 : public ID3D10Include
{
public:
    FRIEND_SINGLETON_DIR_H( IncludeHandler10 );

    STDMETHOD(Open)( D3D10_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pNumBytes );
    STDMETHOD(Close)( LPCVOID pData );

    std::wstring m_ContentDirectory;
};

DECLARE_SINGLETON_DIR_H( IncludeHandler10 );
