//----------------------------------------------------------------------------
// File: stdafx.h
//
// Copyright (c) Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef STRICT
#define STRICT
#endif

#ifndef WINVER
#define WINVER         0x0500
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0500 
#endif

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif
#pragma comment( lib, "d3d9.lib" )

#pragma warning( disable : 4100 ) // disable unreference formal parameter warnings for /W4 builds

#define _CRT_SECURE_NO_DEPRECATE
#define STRSAFE_NO_DEPRECATE

#include <windows.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <math.h>      
#include <limits.h>      
#include <stdio.h>
#include <WCHAR.h>

// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
#endif

// Direct3D includes
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <d3d10.h>
#include <d3dx10.h>

#ifndef STRSAFE_NO_DEPRECATE
#pragma deprecated("strncpy")
#pragma deprecated("wcsncpy")
#pragma deprecated("StringCchCopy")
#pragma deprecated("wcsncat")
#pragma deprecated("strncat")
#pragma deprecated("_tcsncat")
#endif

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

#include <stdio.h>
#include <conio.h>
#include <msxml2.h>


#ifndef APP_FULLNAME
#define APP_FULLNAME L"Microsoft (R) Mesh Convert Tool "
#endif
#define APP_NAME L"meshconvert"

//--------------------------------------------------------------------------------------
// Simple helper stack class
//--------------------------------------------------------------------------------------
template <class T>
class CStack
{
private:
    UINT m_MemorySize;
    UINT m_NumElements;
    T*	m_pData;

    bool EnsureStackSize( UINT64 iElements )
    {
        if( m_MemorySize > iElements )
            return true;

        T* pTemp = new T[ (size_t)(iElements*2 + 256) ];
        if( !pTemp )
            return false;

        if( m_NumElements )
        {
            CopyMemory( pTemp, m_pData, (size_t)(m_NumElements*sizeof(T)) );
        }
    
        if( m_pData ) delete []m_pData;
        m_pData = pTemp;
        return true;
    }

public:
    CStack() { m_pData = NULL; m_NumElements = 0; m_MemorySize = 0; }
    ~CStack() { if( m_pData ) delete []m_pData; }

    UINT GetCount() { return m_NumElements; }
    T GetAt( UINT i ) { return m_pData[i]; }
    T GetTop()
    {
        if( m_NumElements < 1 )
            return NULL;

        return m_pData[ m_NumElements-1 ];
    }

    T GetRelative( INT i )
    {
        INT64 iVal = m_NumElements-1 + i;
        if( iVal < 0 )
            return NULL;
        return m_pData[ iVal ];
    }

    bool Push( T pElem )
    {
        if(!EnsureStackSize( m_NumElements+1 ) )
            return false;
        
        m_pData[m_NumElements] = pElem;
        m_NumElements++;

        return true;
    }

    T Pop()
    {
        if( m_NumElements < 1 )
            return NULL;

        m_NumElements --;
        return m_pData[m_NumElements];
    }
};


#include "IntermediateMesh.h"
#include "LoaderXFile.h"
#include "Loader.h"

