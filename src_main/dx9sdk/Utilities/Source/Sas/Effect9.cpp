#include "stdafx.h"
#include "Effect9.h"
#include "Sas.h"
#include "Parameter.h"

using namespace Sas;

//--------------------------------------------------------------------------------------
Effect9::~Effect9()
{
    SAFE_RELEASE( m_pD3DXEffect );
    Effect::~Effect();

}

//--------------------------------------------------------------------------------------
HRESULT Effect9::FromD3DXEffect( ID3DXEffect* pD3DXEffect, const SourceInfo& sf, Effect9** ppEffectOut )
{
    HRESULT hr = S_OK;
    Effect9* pNew = NULL;

    if( NULL == pD3DXEffect || 
        NULL == ppEffectOut )
        return E_INVALIDARG;

    // Clear the output parameter
    *ppEffectOut = NULL;

    // Try to allocate new memory
    pNew = new Effect9();
    if( NULL == pNew )
    {
        hr = E_OUTOFMEMORY;
        goto LFail;
    }

    // Instance members
    pNew->m_pD3DXEffect = pD3DXEffect;
    pNew->m_pD3DXEffect->AddRef();


    pNew->m_Creation = sf;

    D3DXEFFECT_DESC EffectDesc;
    // Gather the parameters
    hr = pD3DXEffect->GetDesc( &EffectDesc );
    if( FAILED(hr) )
        goto LFail;
    
    for( UINT i=0; i < EffectDesc.Parameters; i++ )
    {
        D3DXHANDLE hParameter = pD3DXEffect->GetParameter( NULL, i );
        if( NULL == hParameter )
        {
            pNew->m_Variables.push_back(NULL);
            continue;
        }

        Variable* pParam = new Variable9(pD3DXEffect, hParameter, pNew, NULL, i);
        if( NULL == pParam )
        {
            hr = E_OUTOFMEMORY;
            goto LFail;
        }

        pNew->m_Variables.push_back( pParam );

        // Check for the global parameter
        if( pParam->GetParameter()->IsGlobalParameter() )
        {
            // If this is the second global parameter found, ignore it
            if( NULL != pNew->m_pGlobal )
                Sas::Host::Out( LEVEL_ERROR, L"Multiple global parameters found. Ignoring all but the first occurance" );
            else
                pNew->m_pGlobal = pParam;
        }
    }

    // If no global parameter was found, the effect is by definition
    // not Sas-complaint
    if( NULL == pNew->m_pGlobal )
    {
        Sas::Host::Out( LEVEL_ERROR, L"'%s' - Global parameter not found. Effect is not SAS-compliant.", sf.GetDescription() );
        hr = E_FAIL;
        goto LFail;
    }


LFail:
    if(FAILED(hr))
    {
        SAFE_DELETE( pNew );
    }

    *ppEffectOut = pNew;
    return hr;
}




//--------------------------------------------------------------------------------------
HRESULT Effect9::FromSource( IDirect3DDevice9* pDevice, const SourceInfo& sf, const D3DXMACRO* pDefines, ID3DXInclude* pInclude, DWORD flags, Effect9** ppEffectOut )
{
    HRESULT hr;
    hr = S_OK;
    if( NULL == ppEffectOut )
        return E_INVALIDARG;

    // Clear the output parameter
    *ppEffectOut = NULL;

    // Attempt to create the D3DXEffect
    ID3DXEffect* pD3DXEffect = NULL;
    ID3DXBuffer* pErrors = NULL;

    if(sf.GetSourceType() == SOURCEINFO_RESOURCE)
    {
        hr = SASCreateEffectFromResource(
                pDevice,
                sf.GetResourceModule(),
                sf.GetResourceName(),
                sf.GetResourceType(),
                pDefines,
                pInclude,
                flags,
                NULL,
                &pD3DXEffect,
                &pErrors );
    }
    else if(sf.GetSourceType() == SOURCEINFO_FILE)
    {
        hr = D3DXCreateEffectFromFile( 
                pDevice, 
                sf.GetFilePath(), 
                pDefines, 
                pInclude, 
                flags, 
                NULL, 
                &pD3DXEffect, 
                &pErrors );
        if( hr == E_NOTIMPL)
            hr = D3DXCreateEffectFromFile( 
                    pDevice, 
                    sf.GetFilePath(), 
                    pDefines, 
                    pInclude, 
                    (flags | D3DXSHADER_USE_LEGACY_D3DX9_31_DLL), 
                    NULL, 
                    &pD3DXEffect, 
                    &pErrors );
    }

    if( pErrors )
        Sas::Host::OutAnsi( FAILED(hr)?LEVEL_ERROR:LEVEL_WARNING, "%s", (LPSTR)pErrors->GetBufferPointer() );

    if( FAILED(hr) )
        goto e_Exit;

    D3DXHANDLE technique = NULL;
    hr= pD3DXEffect->FindNextValidTechnique(NULL, &technique);
    if( FAILED(hr) )
    {
        Sas::Host::Out( LEVEL_ERROR, L"No technique in the effect is valid on your hardware." );
        goto e_Exit;
    }
    
    pD3DXEffect->SetTechnique(technique);

    // Now that we have a D3DXEffect we can create a Sas::Effect
    hr = FromD3DXEffect( pD3DXEffect, sf, ppEffectOut );

e_Exit:
    SAFE_RELEASE(pErrors);
    SAFE_RELEASE( pD3DXEffect );

    if(SUCCEEDED(hr))
        Sas::Host::Out( LEVEL_STATUS, L"Loading effect '%s'", sf.GetDescription() );
    else
        Sas::Host::Out( LEVEL_ERROR, L"Could not load effect '%s'", sf.GetDescription() );

    return hr;
}

HRESULT WINAPI
SASCreateEffectFromResource(
    LPDIRECT3DDEVICE9               pDevice,
    HMODULE							module, 
    LPCWSTR							resName, 
    LPCWSTR							resType,
    CONST D3DXMACRO*                pDefines,
    LPD3DXINCLUDE                   pInclude,
    DWORD                           Flags,
    LPD3DXEFFECTPOOL                pPool,
    LPD3DXEFFECT*                   ppEffect,
    LPD3DXBUFFER*                   ppCompilationErrors)
{
    HRESULT hr = S_OK;
    HRSRC hSrc= FindResource(module, resName, resType);
    if(hSrc == NULL)
        return E_FAIL;

    HGLOBAL hGlobal = LoadResource(module, hSrc);
    if(hGlobal)
    {
        DWORD InMemorySize = SizeofResource(module, hSrc);
        void* pInMemory = LockResource(hGlobal);
        
        hr = D3DXCreateEffect( pDevice, pInMemory, InMemorySize, pDefines, pInclude,Flags, pPool, ppEffect, ppCompilationErrors );
        if (hr == E_NOTIMPL)
            hr = D3DXCreateEffect( pDevice, pInMemory, InMemorySize, pDefines, pInclude,(Flags|D3DXSHADER_USE_LEGACY_D3DX9_31_DLL), pPool, ppEffect, ppCompilationErrors );

        FreeResource(hGlobal);
    }
 
    return hr;
}




//--------------------------------------------------------------------------------------
HRESULT Effect9::OnLostDevice()
{
    if( NULL == m_pD3DXEffect )
        return S_OK;

    return m_pD3DXEffect->OnLostDevice();
}


//--------------------------------------------------------------------------------------
HRESULT Effect9::OnResetDevice()
{
    if( NULL == m_pD3DXEffect )
        return S_OK;

    return m_pD3DXEffect->OnResetDevice();
}




//--------------------------------------------------------------------------------------
Variable9::Variable9( ID3DXEffect* pD3DXEffect, D3DXHANDLE handle, Effect* effect, Variable* parent, UINT index) 
: Variable(effect, parent, index ) 
{
    m_pD3DXEffect = pD3DXEffect;
    m_Handle = handle;

    //ZeroMemory( &m_Desc, sizeof(m_Desc) );
    D3DXPARAMETER_DESC desc;
    m_pD3DXEffect->GetParameterDesc( handle, &desc );

    CharToWString(desc.Name, m_Desc.Name);
    CharToWString(desc.Semantic, m_Desc.Semantic);

    if(desc.Elements > 0)
        m_Desc.Class = SASVC_ARRAY;
    else
    {
        switch( desc.Class )
        {
        case D3DXPC_SCALAR:
            m_Desc.Class = SASVC_SCALAR; break;
        case D3DXPC_VECTOR:
            m_Desc.Class = SASVC_VECTOR; break;
        case D3DXPC_MATRIX_ROWS:
            m_Desc.Class = SASVC_MATRIX; break;
        case D3DXPC_MATRIX_COLUMNS:
            m_Desc.Class = SASVC_MATRIX; break;
        case D3DXPC_OBJECT:
            m_Desc.Class = SASVC_OBJECT; break;
        case D3DXPC_STRUCT:
            m_Desc.Class = SASVC_STRUCT; break;
        default:
            m_Desc.Class = SASVC_INVALID; break;
        };
    }

    switch( desc.Type )
    {
    case D3DXPT_BOOL:
        m_Desc.Type = SASVT_BOOL; break;
    case D3DXPT_INT:
        m_Desc.Type = SASVT_INT; break;
    case D3DXPT_FLOAT:
        m_Desc.Type = SASVT_FLOAT; break;
    case D3DXPT_STRING:
        m_Desc.Type = SASVT_STRING; break;
    case D3DXPT_TEXTURE:
        m_Desc.Type = SASVT_TEXTURE; break;
    case D3DXPT_TEXTURE1D:
        m_Desc.Type = SASVT_TEXTURE1D; break;
    case D3DXPT_TEXTURE2D:
        m_Desc.Type = SASVT_TEXTURE2D; break;
    case D3DXPT_TEXTURE3D:
        m_Desc.Type = SASVT_TEXTURE3D; break;
    case D3DXPT_TEXTURECUBE:
        m_Desc.Type = SASVT_TEXTURECUBE; break;
    default:
        m_Desc.Type = SASVT_INVALID; break;
    };

    m_Desc.Rows = desc.Rows;
    m_Desc.Columns = desc.Columns;
    m_Desc.Elements = desc.Elements;
    m_Desc.Annotations = desc.Annotations;
    m_Desc.StructMembers = desc.StructMembers;
    m_Desc.IsAnnotation = (0 != (desc.Flags & D3DX_PARAMETER_ANNOTATION));
    m_Desc.IsShared = (0 != (desc.Flags & D3DX_PARAMETER_SHARED));


    // Get the associated annotations
    for( UINT i=0; i < m_Desc.Annotations; i++ )
    {
        D3DXHANDLE hAnnotation = m_pD3DXEffect->GetAnnotation( m_Handle, i );
        m_Annotations.push_back( new Variable9( m_pD3DXEffect, hAnnotation, effect, this, i ) );
    }

    // Get the elements
    for( UINT i=0; i < m_Desc.Elements; i++ )
    {
        D3DXHANDLE hElement = m_pD3DXEffect->GetParameterElement( m_Handle, i );
        m_Elements.push_back( new Variable9( m_pD3DXEffect, hElement, effect, this, i ) );
    }

    if( m_Desc.Elements == 0 ) 
    {
        for( UINT i=0; i < m_Desc.StructMembers; i++ )
        {
            D3DXHANDLE hMember = m_pD3DXEffect->GetParameter( m_Handle, i );
            m_Members.push_back( new Variable9( m_pD3DXEffect, hMember, effect, this, i ) );
        }
    }

};

//--------------------------------------------------------------------------------------
HRESULT Variable9::GetObject_Internal( Object* pObject ) const
{
    HRESULT hr;

    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_STRING:
        {
            const char* x;

            hr = m_pD3DXEffect->GetString( m_Handle, &x );
            if( FAILED(hr) )
                return hr;

            String temp;
            hr = temp.SetAnsi( x );
            if( FAILED(hr) )
                return hr;

            hr = pObject->Set( temp );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }

        //------------------------------------------------------------------------------
        case SASVT_TEXTURE:
        {
            IDirect3DBaseTexture9* pTexture = NULL;
            hr = m_pD3DXEffect->GetTexture( m_Handle, &pTexture );
            if( FAILED(hr) )
                return hr;

            hr = pObject->Set( Texture(pTexture) );

            SAFE_RELEASE(pTexture);

            if( FAILED(hr) )
                return hr;

            return S_OK;
        }

        //------------------------------------------------------------------------------
        case SASVT_TEXTURE1D:
        {
            IDirect3DTexture9* pTexture = NULL;
            hr = m_pD3DXEffect->GetTexture( m_Handle, (IDirect3DBaseTexture9**)&pTexture );
            if( FAILED(hr) )
                return hr;

            hr = pObject->Set( Texture1D(pTexture) );

            SAFE_RELEASE(pTexture);

            if( FAILED(hr) )
                return hr;

            return S_OK;
        }

        //------------------------------------------------------------------------------
        case SASVT_TEXTURE2D:
        {
            IDirect3DTexture9* pTexture = NULL;
            hr = m_pD3DXEffect->GetTexture( m_Handle, (IDirect3DBaseTexture9**)&pTexture );
            if( FAILED(hr) )
                return hr;

            hr = pObject->Set( Texture2D(pTexture) );

            SAFE_RELEASE(pTexture);

            if( FAILED(hr) )
                return hr;

            return S_OK;
        }

        //------------------------------------------------------------------------------
        case SASVT_TEXTURE3D:
        {
            IDirect3DVolumeTexture9* pTexture = NULL;
            hr = m_pD3DXEffect->GetTexture( m_Handle, (IDirect3DBaseTexture9**)&pTexture );
            if( FAILED(hr) )
                return hr;

            hr = pObject->Set( Texture3D(pTexture) );

            SAFE_RELEASE(pTexture);

            if( FAILED(hr) )
                return hr;

            return S_OK;
        }

        //------------------------------------------------------------------------------
        case SASVT_TEXTURECUBE:
        {
            IDirect3DCubeTexture9* pTexture = NULL;
            hr = m_pD3DXEffect->GetTexture( m_Handle, (IDirect3DBaseTexture9**)&pTexture );
            if( FAILED(hr) )
                return hr;

            hr = pObject->Set( TextureCube(pTexture) );

            SAFE_RELEASE(pTexture);

            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
        
    }

    return E_FAIL;
}


//--------------------------------------------------------------------------------------
HRESULT Variable9::GetScalar_Internal( Scalar* pScalar ) const
{
    HRESULT hr;

    
    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            BOOL x;

            hr = m_pD3DXEffect->GetBool( m_Handle, &x );
            if( FAILED(hr) )
                return hr;

            hr = pScalar->Set( Bool(0 != x) );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
            
        //------------------------------------------------------------------------------
        case SASVT_INT:
        {
            int x;

            hr = m_pD3DXEffect->GetInt( m_Handle, &x );
            if( FAILED(hr) )
                return hr;

            hr = pScalar->Set( Int(x) );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }

        //------------------------------------------------------------------------------
        case SASVT_FLOAT:
        {
            float x;

            hr = m_pD3DXEffect->GetFloat( m_Handle, &x );
            if( FAILED(hr) )
                return hr;

            hr = pScalar->Set( Float(x) );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
    }

    return E_FAIL;
}


//--------------------------------------------------------------------------------------
HRESULT Variable9::GetVector_Internal( Vector* pVector ) const
{
    HRESULT hr;

    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            BOOL x[4];

            hr = m_pD3DXEffect->GetBoolArray( m_Handle, x, 4 );
            if( FAILED(hr) )
                return hr;

            for( UINT i=0; i < m_Desc.Columns; i++ )
            {
                hr = pVector->Set( i, Bool(0 != x[i]) );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }
          

        //------------------------------------------------------------------------------
        case SASVT_INT:
        {
            int x[4];

            hr = m_pD3DXEffect->GetIntArray( m_Handle, x, 4 );
            if( FAILED(hr) )
                return hr;

            for( UINT i=0; i < m_Desc.Columns; i++ )
            {
                hr = pVector->Set( i, Int(x[i]) );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }


        //------------------------------------------------------------------------------
        case SASVT_FLOAT:
        {
            float x[4];

            hr = m_pD3DXEffect->GetFloatArray( m_Handle, x, 4 );
            if( FAILED(hr) )
                return hr;

            for( UINT i=0; i < m_Desc.Columns; i++ )
            {
                hr = pVector->Set( i, Float(x[i]) );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }
    }

    return E_FAIL;
}

//--------------------------------------------------------------------------------------
HRESULT Variable9::GetMatrix_Internal( Matrix* pMatrix ) const
{
    HRESULT hr;

    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            BOOL x[16];

            hr = m_pD3DXEffect->GetBoolArray( m_Handle, x, 16 );
            if( FAILED(hr) )
                return hr;

            int index=0;
            for( UINT r=0; r < m_Desc.Rows; r++ )
            {
                for( UINT c=0; c < m_Desc.Columns; c++ )
                {
                    hr = pMatrix->Set( r, c, Bool(0 != x[index]) );
                    if( FAILED(hr) )
                        return hr;

                    index++;
                }
            }

            return S_OK;
        }
          

        //------------------------------------------------------------------------------
        case SASVT_INT:
        {
            int x[16];

            hr = m_pD3DXEffect->GetIntArray( m_Handle, x, 16 );
            if( FAILED(hr) )
                return hr;

            int index=0;
            for( UINT r=0; r < m_Desc.Rows; r++ )
            {
                for( UINT c=0; c < m_Desc.Columns; c++ )
                {
                    hr = pMatrix->Set( r, c, Int( x[index] ) );
                    if( FAILED(hr) )
                        return hr;

                    index++;
                }
            }

            return S_OK;
        }


        //------------------------------------------------------------------------------
        case SASVT_FLOAT:
        {
            float x[16];

            hr = m_pD3DXEffect->GetFloatArray( m_Handle, x, 16 );
            if( FAILED(hr) )
                return hr;

            int index=0;
            for( UINT r=0; r < m_Desc.Rows; r++ )
            {
                for( UINT c=0; c < m_Desc.Columns; c++ )
                {
                    hr = pMatrix->Set( r, c, Float( x[index] ) );
                    if( FAILED(hr) )
                        return hr;

                    index++;
                }
            }

            return S_OK;
        }
    }

    return E_FAIL;
}





HRESULT Variable9::SpecializedCasts( Value* pCached, const Value& value )
{
    HRESULT hr = S_OK;

    SASVARIABLE_TYPE type = Desc().Type;
    switch( type )
    {
        case SASVT_TEXTURE:
        case SASVT_TEXTURE1D:
        case SASVT_TEXTURE2D:
        case SASVT_TEXTURE3D:
        case SASVT_TEXTURECUBE:
        {
            if( value.Type() == SASVT_STRING )
            {
                IUnknown* pTexture = NULL;
                String* strValue = (String*) ((Object&)value).Get();
                const WCHAR* str = strValue->Get();

                if(str != NULL && str[0] != L'\0' )
                {
                    SourceInfo si;
                    if(! Sas::Host::GetIntegrator()->FindFile( str , m_Effect->GetCreation()->GetDirectoryPath(), si) )
                        return E_FAIL;

                    hr = m_Effect->LoadTexture(si.GetFilePath(), (IUnknown**)&pTexture);	
                    if( FAILED(hr) )
                    {
                        Sas::Host::Out( Sas::LEVEL_WARNING, L"Unable to cast string into texture: %s", si.GetFilePath() );
                        SAFE_RELEASE(pTexture);
                        return hr;
                    }
                }

                switch(Desc().Type)
                {
                    case SASVT_TEXTURE:
                        ((Object*)pCached)->Set( Texture(pTexture) );
                        break;
                    case SASVT_TEXTURE1D:
                        ((Object*)pCached)->Set( Texture1D(pTexture) );
                        break;
                    case SASVT_TEXTURE2D:
                        ((Object*)pCached)->Set( Texture2D(pTexture) );
                        break;
                    case SASVT_TEXTURE3D:
                        ((Object*)pCached)->Set( Texture3D(pTexture) );
                        break;
                    case SASVT_TEXTURECUBE:
                        ((Object*)pCached)->Set( TextureCube(pTexture) );
                        break;
                }
                SAFE_RELEASE(pTexture);
                return S_OK;
            }
        }
    }

    return E_NOTIMPL;//we don't support that cast;
}



//--------------------------------------------------------------------------------------
HRESULT Variable9::SetObject_Internal( Object* pObject )
{
    HRESULT hr = S_OK;

    DataType* pData= pObject->Get();
    if( NULL == pData )
        return E_FAIL;
            
    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_STRING:
        {
            if(pData->Type() == SASVT_STRING)
                return E_FAIL;


            char* str = NULL;

            const WCHAR* wstr;
            wstr = pData->GetString();
            if( wstr )
            {
                // Convert to ansi
                int size = (int)wcslen(wstr)+1;
                str = new char[ size ];
                if( NULL == str )
                    return E_OUTOFMEMORY;

                WideCharToMultiByte( CP_ACP, 0, wstr, -1, str, size, NULL, NULL );
            }

            // Attempt to set the value, but don't return the result until
            // the temporary string has been released
            hr = m_pD3DXEffect->SetString( m_Handle, str );

            SAFE_DELETE_ARRAY(str);
            return hr;
        }

        //------------------------------------------------------------------------------
        case SASVT_TEXTURE:
        case SASVT_TEXTURE1D:
        case SASVT_TEXTURE2D:
        case SASVT_TEXTURE3D:
        case SASVT_TEXTURECUBE:
        {
            if(pData->Type() == SASVT_STRING)
                return E_FAIL;
          
            hr = m_pD3DXEffect->SetTexture( m_Handle, (IDirect3DBaseTexture9*) (((Texture*)pData)->Get()) );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
    }

    return E_FAIL;
}


//--------------------------------------------------------------------------------------
HRESULT Variable9::SetScalar_Internal( Scalar* pScalar )
{
    HRESULT hr;

    DataType* pType;

    pType = pScalar->Get();
    if( NULL == pType )
        return E_FAIL;
            
             
    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            hr = m_pD3DXEffect->SetBool( m_Handle, pType->GetInt() ); // Cast to BOOL
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
            
        //------------------------------------------------------------------------------
        case SASVT_INT:
        {
            hr = m_pD3DXEffect->SetInt( m_Handle, pType->GetInt() );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }

        //------------------------------------------------------------------------------
        case SASVT_FLOAT:
        {
            hr = m_pD3DXEffect->SetFloat( m_Handle, pType->GetFloat() );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
    }

    return E_FAIL;
}


//--------------------------------------------------------------------------------------
HRESULT Variable9::SetVector_Internal( Vector* pVector )
{
    HRESULT hr;

    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            BOOL x[4] = {0};

            for( UINT i=0; i < m_Desc.Columns; i++ )
            {
                DataType* pType = pVector->Get(i);
                if( pType )
                {
                    x[i] = pType->GetInt(); // Cast to BOOL
                }
            }


            hr = m_pD3DXEffect->SetBoolArray( m_Handle, x, pVector->Length() );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
          

        //------------------------------------------------------------------------------
        case SASVT_INT:
        {
            int x[4] = {0};

            for( UINT i=0; i < m_Desc.Columns; i++ )
            {
                DataType* pType = pVector->Get(i);
                if( pType )
                {
                    x[i] = pType->GetInt(); 
                }
            }


            hr = m_pD3DXEffect->SetIntArray( m_Handle, x, pVector->Length() );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }


        //------------------------------------------------------------------------------
        case SASVT_FLOAT:
        {
            float x[4] = {0};

            for( UINT i=0; i < m_Desc.Columns; i++ )
            {
                DataType* pType = pVector->Get(i);
                if( pType )
                {
                    x[i] = pType->GetFloat(); 
                }
            }


            hr = m_pD3DXEffect->SetFloatArray( m_Handle, x, pVector->Length() );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
    }

    return E_FAIL;
}



//--------------------------------------------------------------------------------------
HRESULT Variable9::SetMatrix_Internal( Matrix* pMatrix )
{
    HRESULT hr;

    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            BOOL x[16];

            int index=0;
            for( UINT r=0; r < m_Desc.Rows; r++ )
            {
                for( UINT c=0; c < m_Desc.Columns; c++ )
                {
                    DataType* pType = pMatrix->Get(r, c);
                    if( pType )
                    {
                        x[index] = pType->GetInt(); // Cast to BOOL
                    }

                    index++;
                }
            }

            hr = m_pD3DXEffect->SetBoolArray( m_Handle, x, 16 );
            if( FAILED(hr) )
                return hr;


            return S_OK;
        }
          

        //------------------------------------------------------------------------------
        case SASVT_INT:
        {
            int x[16];

            int index=0;
            for( UINT r=0; r < m_Desc.Rows; r++ )
            {
                for( UINT c=0; c < m_Desc.Columns; c++ )
                {
                    DataType* pType = pMatrix->Get(r, c);
                    if( pType )
                    {
                        x[index] = pType->GetInt(); 
                    }

                    index++;
                }
            }

            hr = m_pD3DXEffect->SetIntArray( m_Handle, x, 16 );
            if( FAILED(hr) )
                return hr;


            return S_OK;
        }


        //------------------------------------------------------------------------------
        case SASVT_FLOAT:
        {
            float x[16];

            int index=0;
            for( UINT r=0; r < m_Desc.Rows; r++ )
            {
                for( UINT c=0; c < m_Desc.Columns; c++ )
                {
                    DataType* pType = pMatrix->Get(r, c);
                    if( pType )
                    {
                        x[index] = pType->GetFloat();
                    }

                    index++;
                }
            }

            hr = m_pD3DXEffect->SetFloatArray( m_Handle, x, 16 );
            if( FAILED(hr) )
                return hr;


            return S_OK;
        }
    }

    return E_FAIL;
}





//--------------------------------------------------------------------------------------
HRESULT Effect9::LoadTexture( const WCHAR* file,  IUnknown** texture)
{
    HRESULT hr = S_OK;
    LPDIRECT3DDEVICE9 pDevice = NULL;
    hr = m_pD3DXEffect->GetDevice( &pDevice );
    if(FAILED(hr))
        return hr;


    D3DXIMAGE_INFO info;
    hr = D3DXGetImageInfoFromFile(file, &info);
    if(FAILED(hr))
        return hr;


    switch( info.ResourceType )
    {
    case  D3DRTYPE_TEXTURE:
        {
            hr = D3DXCreateTextureFromFile( 
                pDevice, 
                file, 
                (IDirect3DTexture9**)texture );
        }
        break;
    case  D3DRTYPE_VOLUMETEXTURE:
        {
            hr = D3DXCreateVolumeTextureFromFile( 
                pDevice, 
                file, 
                (IDirect3DVolumeTexture9**)texture );
        }
        break;
    case  D3DRTYPE_CUBETEXTURE:
        {
            hr = D3DXCreateCubeTextureFromFile( 
                pDevice, 
                file, 
                (IDirect3DCubeTexture9**)texture );
        }
        break;
    }

    SAFE_RELEASE(pDevice);

    return hr;
}
