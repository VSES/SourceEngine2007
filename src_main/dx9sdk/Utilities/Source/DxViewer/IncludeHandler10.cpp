#include "IncludeHandler10.h"
#include "IncludeHandler9.h"

DECLARE_SINGLETON_DIR_CPP( IncludeHandler10 , m_ContentDirectory );

//--------------------------------------------------------------------------------------
HRESULT IncludeHandler10::Open( D3D10_INCLUDE_TYPE IncludeType, LPCSTR pFileNameA, LPCVOID pParentData, LPCVOID *ppData, UINT *pNumBytes )
{
    return DXVGetIncludeHandler9(m_ContentDirectory.c_str()).Open(
            ((IncludeType == D3D10_INCLUDE_SYSTEM) ? D3DXINC_SYSTEM : D3DXINC_LOCAL),
            pFileNameA,
            pParentData,
            ppData,
            pNumBytes);
}

//--------------------------------------------------------------------------------------
HRESULT IncludeHandler10::Close( LPCVOID pData )
{
    return DXVGetIncludeHandler9(m_ContentDirectory.c_str()).Close( pData );
}
