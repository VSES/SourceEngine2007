#include "windows.h"
#pragma warning(disable: 4995)
#include <strsafe.h>
#include <shlwapi.h>
#include <shlobj.h>
#pragma warning(default: 4995)

#include "dxut.h"
#include "resource.h"
#include "FileSystem.h"
#include "IncludeHandler9.h"


DECLARE_SINGLETON_DIR_CPP( IncludeHandler9 , m_ContentDirectory );



//--------------------------------------------------------------------------------------
HRESULT IncludeHandler9::Open( D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileNameA, LPCVOID pParentData, LPCVOID *ppData, UINT *pNumBytes )
{
    HRESULT hr = S_OK;
    HGLOBAL hglobal = NULL;
    HANDLE hFile = NULL;

    if( 0 == lstrcmpiA( pFileNameA, "sas/sas.fxh" ) ||
        0 == lstrcmpiA( pFileNameA, "sas\\sas.fxh" ) ||
        0 == lstrcmpiA( pFileNameA, "sas\\\\sas.fxh" ) ||
        0 == lstrcmpiA( pFileNameA, "sas.fxh" ) )
    {
        HRSRC hrsrc;
        BYTE* pBytesSrc;
            
        hrsrc = FindResource( NULL, MAKEINTRESOURCE(IDR_SASFXH), RT_RCDATA );
        if( hrsrc == NULL )
        { hr = E_FAIL; goto LFail; }

        hglobal = LoadResource( NULL, hrsrc );
        if( hglobal == NULL )
        { hr = E_FAIL; goto LFail; }

        pBytesSrc = (BYTE*)LockResource( hglobal );
        if( pBytesSrc == NULL )
        { hr = E_FAIL; goto LFail; }

        *pNumBytes = SizeofResource( NULL, hrsrc );
        *ppData = (void*)new BYTE[*pNumBytes];
        if( *ppData == NULL )
        { hr = E_OUTOFMEMORY; goto LFail; }

        memcpy( (void*)*ppData, pBytesSrc, *pNumBytes );
    }
    else
    {
        WCHAR pFileNameW[MAX_PATH] = L"";
        StringCchPrintfW(pFileNameW, MAX_PATH, L"%S", pFileNameA);

        DXV_FIND_DATA findData;

        DXVGetFileFinder().ClearSearchPaths();

        const TCHAR* c = m_ContentDirectory.c_str();
        while (c && *c)
        {
            // skip ws
            while (*c && iswspace(*c)) c++;
            if (*c == 0) break;

            // mark name start, find semicolon delimiter or end-of-string
            const TCHAR* ns = c;
            while (*c && *c != L';') c++;

            // remove trailing whitespace
            const TCHAR* es = c-1;
            while (es > ns && iswspace(*es)) es--;

            int length = ((int)(es - ns)) + 1 + 1;  // +1 for nul, +1 because es is on the last character.
            TCHAR* statement = (TCHAR*) alloca( sizeof( TCHAR ) * length );
            memcpy(statement, ns, sizeof( TCHAR ) * length );
            statement[length-1] = TEXT('\0');

            DXVGetFileFinder().AddSearchPath(statement);

            if (*c == L';') 
                c++;
        }

        if( DXVGetFileFinder().FindFile(findData, pFileNameW, L"" ) )
        {
            DWORD bytesRead;

            hFile = CreateFile( findData.FullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL );
            if( INVALID_HANDLE_VALUE == hFile )
            { hr = E_FAIL; goto LFail; }

            *pNumBytes = findData.FileData.nFileSizeLow;
            *ppData = (void*)new BYTE[*pNumBytes];
            if( *ppData == NULL )
            { hr = E_OUTOFMEMORY; goto LFail; }

            if( 0 == ReadFile( hFile, (void*)*ppData, *pNumBytes, &bytesRead, NULL ) )
            { hr = E_FAIL; goto LFail; }
        }
    }

LFail:

    DXVGetFileFinder().ClearSearchPaths();

    CloseHandle(hFile);
    FreeResource(hglobal);
    return hr;
}

//--------------------------------------------------------------------------------------
HRESULT IncludeHandler9::Close( LPCVOID pData )
{
    BYTE* pBytes = (BYTE*)pData;
    SAFE_DELETE_ARRAY( pBytes );
    return S_OK;
}
