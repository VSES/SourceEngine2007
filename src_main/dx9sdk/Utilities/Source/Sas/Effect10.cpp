#include "stdafx.h"
#include "Effect10.h"
#include "Sas.h"
#include "Parameter.h"

using namespace Sas;

//--------------------------------------------------------------------------------------
Effect10::~Effect10()
{
    SAFE_RELEASE( m_pD3DEffect );
    Effect::~Effect();
}

//--------------------------------------------------------------------------------------
HRESULT Effect10::FromD3DEffect( ID3D10Effect* pD3DEffect, const SourceInfo& sf, Effect10** ppEffectOut )
{
    HRESULT hr = S_OK;
    Effect10* pNew = NULL;

    if( NULL == pD3DEffect || 
        NULL == ppEffectOut )
        return E_INVALIDARG;

    // Clear the output parameter
    *ppEffectOut = NULL;

    // Try to allocate new memory
    pNew = new Effect10();
    if( NULL == pNew )
    {
        hr = E_OUTOFMEMORY;
        goto LFail;
    }

    // Instance members
    pNew->m_pD3DEffect = pD3DEffect;
    pNew->m_pD3DEffect->AddRef();


    pNew->m_Creation = sf;

    D3D10_EFFECT_DESC EffectDesc;
    // Gather the parameters
    hr = pD3DEffect->GetDesc( &EffectDesc );
    if( FAILED(hr) )
        goto LFail;
    
    for( UINT i=0; i < EffectDesc.GlobalVariables; i++ )
    {
        ID3D10EffectVariable* pfxVariable = pD3DEffect->GetVariableByIndex( i );
        if( NULL == pfxVariable )
        {
            pNew->m_Variables.push_back(NULL);
            continue;
        }

        Variable* pParam = new Variable10(pfxVariable, pNew, NULL, i);
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
HRESULT Effect10::FromSource( ID3D10Device* pDevice, const SourceInfo& sf, const D3D10_SHADER_MACRO* pDefines, ID3D10Include* pInclude, DWORD HlslFlags, DWORD FxFlags, Effect10** ppEffectOut )
{
    HRESULT hr;

    if( NULL == ppEffectOut )
        return E_INVALIDARG;

    // Clear the output parameter
    *ppEffectOut = NULL;

    // Attempt to create the D3DXEffect
    ID3D10Effect* pD3DEffect = NULL;
    ID3D10Blob* pErrors = NULL;


    if(sf.GetSourceType() == SOURCEINFO_RESOURCE)
    {
        hr = D3DX10CreateEffectFromResource( 
                sf.GetResourceModule(), 
                sf.GetResourceName(), 
                sf.GetResourceName(), 
                pDefines, 
                pInclude,
                "fx_4_0",
                HlslFlags, 
                FxFlags,
                pDevice,
                NULL,
                NULL,
                &pD3DEffect,
                &pErrors,
                NULL );
    }
    else if(sf.GetSourceType() == SOURCEINFO_FILE)
    {

        hr = D3DX10CreateEffectFromFile( 
                sf.GetFilePath(), 
                pDefines, 
                pInclude,
                "fx_4_0",
                HlslFlags, 
                FxFlags, 
                pDevice, 
                NULL, 
                NULL, 
                &pD3DEffect, 
                &pErrors,
                NULL );
    }

    if( pErrors )
        Sas::Host::OutAnsi( FAILED(hr)?LEVEL_ERROR:LEVEL_WARNING, "%s",(LPSTR)pErrors->GetBufferPointer() );

    if( FAILED(hr) )
        goto e_Exit;

    D3D10_EFFECT_DESC desc;
    pD3DEffect->GetDesc(&desc);
    if(desc.Techniques <= 0)
    {
        Sas::Host::Out( LEVEL_ERROR, L"Effect does not have a Technique10." );
        hr = E_FAIL;
        goto e_Exit;
    }


    // Now that we have a D3DXEffect we can create a Sas::Effect
    hr = FromD3DEffect( pD3DEffect, sf, ppEffectOut );

e_Exit:
    SAFE_RELEASE(pErrors);
    SAFE_RELEASE( pD3DEffect );

    if(SUCCEEDED(hr))
        Sas::Host::Out( LEVEL_STATUS, L"Loading effect '%s'", sf.GetDescription() );
    else
        Sas::Host::Out( LEVEL_ERROR, L"Could not load effect '%s'", sf.GetDescription() );

    return hr;
}



//--------------------------------------------------------------------------------------
Variable10::Variable10( ID3D10EffectVariable* pD3DEffectVariable, Effect* effect, Variable* parent, UINT index) : Variable(effect, parent, index )
{
    m_pD3DEffectVariable = pD3DEffectVariable;

    //ZeroMemory( &m_Desc, sizeof(m_Desc) );
    D3D10_EFFECT_VARIABLE_DESC VarDesc;
    m_pD3DEffectVariable->GetDesc( &VarDesc );

    CharToWString(VarDesc.Name, m_Desc.Name);
    CharToWString(VarDesc.Semantic, m_Desc.Semantic);


    D3D10_EFFECT_TYPE_DESC TypeDesc;
    ID3D10EffectType* pType = m_pD3DEffectVariable->GetType();
    pType->GetDesc( &TypeDesc );



    if(TypeDesc.Elements > 0)
        m_Desc.Class = SASVC_ARRAY;
    else
    {
        switch( TypeDesc.Class )
        {
        case D3D10_SVC_SCALAR:
            m_Desc.Class = SASVC_SCALAR; break;
        case D3D10_SVC_VECTOR:
            m_Desc.Class = SASVC_VECTOR; break;
        case D3D10_SVC_MATRIX_ROWS:
            m_Desc.Class = SASVC_MATRIX; break;
        case D3D10_SVC_MATRIX_COLUMNS:
            m_Desc.Class = SASVC_MATRIX; break;
        case D3D10_SVC_OBJECT:
            m_Desc.Class = SASVC_OBJECT; break;
        case D3D10_SVC_STRUCT:
            m_Desc.Class = SASVC_STRUCT; break;
        default:
            m_Desc.Class = SASVC_INVALID; break;
        };
    }

    switch( TypeDesc.Type )
    {
    case D3D10_SVT_BOOL:
        m_Desc.Type = SASVT_BOOL; break;
    case D3D10_SVT_INT:
        m_Desc.Type = SASVT_INT; break;
    case D3D10_SVT_FLOAT:
        m_Desc.Type = SASVT_FLOAT; break;
    case D3D10_SVT_STRING:
        m_Desc.Type = SASVT_STRING; break;
    case D3D10_SVT_TEXTURE:
        m_Desc.Type = SASVT_TEXTURE; break;
    case D3D10_SVT_TEXTURE1D:
        m_Desc.Type = SASVT_TEXTURE1D; break;
    case D3D10_SVT_TEXTURE2D:
        m_Desc.Type = SASVT_TEXTURE2D; break;
    case D3D10_SVT_TEXTURE3D:
        m_Desc.Type = SASVT_TEXTURE3D; break;
    case D3D10_SVT_TEXTURECUBE:
        m_Desc.Type = SASVT_TEXTURECUBE; break;
    default:
        m_Desc.Type = SASVT_INVALID; break;
    };

    m_Desc.Rows = TypeDesc.Rows;
    m_Desc.Columns = TypeDesc.Columns;
    m_Desc.Elements = TypeDesc.Elements;
    m_Desc.Annotations = VarDesc.Annotations;
    m_Desc.StructMembers = TypeDesc.Members;
    m_Desc.IsAnnotation = (0 != (VarDesc.Flags & D3D10_EFFECT_VARIABLE_ANNOTATION));
    m_Desc.IsShared = (0 != (VarDesc.Flags & D3D10_EFFECT_VARIABLE_POOLED));


    // Get the associated annotations
    for( UINT i=0; i < m_Desc.Annotations; i++ )
    {
        ID3D10EffectVariable* pAnnotation = m_pD3DEffectVariable->GetAnnotationByIndex( i );
        m_Annotations.push_back( new Variable10( pAnnotation, effect, this, i ) );
    }

    // Get the elements
    for( UINT i=0; i < m_Desc.Elements; i++ )
    {
        ID3D10EffectVariable* pElement = m_pD3DEffectVariable->GetElement( i );
        m_Elements.push_back( new Variable10( pElement, effect, this, i ) );
    }

    if( m_Desc.Elements == 0 ) 
    {
        for( UINT i=0; i < m_Desc.StructMembers; i++ )
        {
            ID3D10EffectVariable* pMember = m_pD3DEffectVariable->GetMemberByIndex( i );
            m_Members.push_back( new Variable10( pMember, effect, this, i ) );
        }
    }

};

//--------------------------------------------------------------------------------------
HRESULT Variable10::GetObject_Internal( Object* pObject ) const
{
    HRESULT hr;

    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_STRING:
        {
            const char* x;

            hr = m_pD3DEffectVariable->AsString()->GetString(&x);
            if( FAILED(hr) )
                return hr;

            String temp;
            hr = temp.SetAnsi( x );
            if( FAILED(hr) )
                return hr;

            hr = pObject->Set( temp );
            if( FAILED(hr) )
                return hr;

            return hr;
        }

        //------------------------------------------------------------------------------
        case SASVT_TEXTURE:
        case SASVT_TEXTURE1D:
        case SASVT_TEXTURE2D:
        case SASVT_TEXTURE3D:
        case SASVT_TEXTURECUBE:
        {
            ID3D10ShaderResourceView* pResource = NULL;
            hr = m_pD3DEffectVariable->AsShaderResource()->GetResource( &pResource );
            if( FAILED(hr) )
                return hr;

            switch( m_Desc.Type )
            {
            case SASVT_TEXTURE:
                hr = pObject->Set( Texture(pResource) );
                break;
            case SASVT_TEXTURE1D:
                hr = pObject->Set( Texture1D(pResource) );
                break;
            case SASVT_TEXTURE2D:
                hr = pObject->Set( Texture2D(pResource) );
                break;
            case SASVT_TEXTURE3D:
                hr = pObject->Set( Texture3D(pResource) );
                break;
            case SASVT_TEXTURECUBE:
                hr = pObject->Set( TextureCube(pResource) );
                break;
            };

            SAFE_RELEASE(pResource);

            if( FAILED(hr) )
                return hr;

            return hr;
        }
    }

    return E_FAIL;
}


//--------------------------------------------------------------------------------------
HRESULT Variable10::GetScalar_Internal( Scalar* pScalar ) const
{
    HRESULT hr;

    
    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            BOOL x;

            hr = m_pD3DEffectVariable->AsScalar()->GetBool( &x );
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

            hr = m_pD3DEffectVariable->AsScalar()->GetInt( &x );
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

            hr = m_pD3DEffectVariable->AsScalar()->GetFloat( &x );
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
HRESULT Variable10::GetVector_Internal( Vector* pVector ) const
{
    HRESULT hr;

    switch( m_Desc.Type )
    {

        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            BOOL x[4];

            hr = m_pD3DEffectVariable->AsVector()->GetBoolVector( x );
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

            hr = m_pD3DEffectVariable->AsVector()->GetIntVector( x );
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

            hr = m_pD3DEffectVariable->AsVector()->GetFloatVector( x );
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
HRESULT Variable10::GetMatrix_Internal( Matrix* pMatrix ) const
{
    HRESULT hr;

    switch( m_Desc.Type )
    {
/*
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            BOOL x[16];

            hr = m_pD3DEffectVariable->AsMatrix()->GetMatrix( x );
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

            hr = m_pD3DEffectVariable->AsMatrix()->GetMatrix( x );
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
*/

        //------------------------------------------------------------------------------
        case SASVT_FLOAT:
        {
            float x[16];

            hr = m_pD3DEffectVariable->AsMatrix()->GetMatrix( x );
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





HRESULT Variable10::SpecializedCasts( Value* pCached, const Value& value )
{
    HRESULT hr = S_OK;

    switch( Desc().Type )
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
HRESULT Variable10::SetObject_Internal( Object* pObject )
{
    HRESULT hr = S_OK;

    DataType* pData= pObject->Get();
    if( NULL == pData )
        return E_FAIL;
            
    switch( m_Desc.Type )
    {
        case SASVT_STRING:
            return hr;
/*
        //------------------------------------------------------------------------------
        case SASVT_STRING:
        {
            if(pData->Type() == SASVT_STRING)
                return E_FAIL;


            std::string str;
            WCharToString( pData->GetString(), str);

            // Attempt to set the value, but don't return the result until
            // the temporary string has been released
            hr = m_pD3DEffectVariable->AsString()->SetString( str.c_str() );

            return hr;
        }
*/
        //------------------------------------------------------------------------------
        case SASVT_TEXTURE:
        case SASVT_TEXTURE1D:
        case SASVT_TEXTURE2D:
        case SASVT_TEXTURE3D:
        case SASVT_TEXTURECUBE:
        {
            if(pData->Type() == SASVT_STRING)
                return E_FAIL;

            IUnknown* pTexture = ((Texture*)pData)->Get();
          
            hr = m_pD3DEffectVariable->AsShaderResource()->SetResource( (ID3D10ShaderResourceView*) pTexture );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
    }

    return E_FAIL;
}


//--------------------------------------------------------------------------------------
HRESULT Variable10::SetScalar_Internal( Scalar* pScalar )
{
    HRESULT hr;

    DataType* pType= pScalar->Get();
    if( NULL == pType )
        return E_FAIL;
            
             
    switch( m_Desc.Type )
    {
        //------------------------------------------------------------------------------
        case SASVT_BOOL:
        {
            hr = m_pD3DEffectVariable->AsScalar()->SetBool( pType->GetBool() );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
            
        //------------------------------------------------------------------------------
        case SASVT_INT:
        {
            hr = m_pD3DEffectVariable->AsScalar()->SetInt( pType->GetInt() );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }

        //------------------------------------------------------------------------------
        case SASVT_FLOAT:
        {
            hr = m_pD3DEffectVariable->AsScalar()->SetFloat( pType->GetFloat() );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
    }

    return E_FAIL;
}


//--------------------------------------------------------------------------------------
HRESULT Variable10::SetVector_Internal( Vector* pVector )
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

            hr = m_pD3DEffectVariable->AsVector()->SetBoolVector( x );
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


            hr = m_pD3DEffectVariable->AsVector()->SetIntVector( x );
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


            hr = m_pD3DEffectVariable->AsVector()->SetFloatVector( x );
            if( FAILED(hr) )
                return hr;

            return S_OK;
        }
    }

    return E_FAIL;
}



//--------------------------------------------------------------------------------------
HRESULT Variable10::SetMatrix_Internal( Matrix* pMatrix )
{
    HRESULT hr;

    switch( m_Desc.Type )
    {
/*
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

            hr = m_pD3DEffectVariable->AsMatrix()->SetMatrix( x );
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

            hr = m_pD3DEffectVariable->AsMatrix()->SetMatrix( x );
            if( FAILED(hr) )
                return hr;


            return S_OK;
        }
*/

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

            hr = m_pD3DEffectVariable->AsMatrix()->SetMatrix( x );
            if( FAILED(hr) )
                return hr;


            return S_OK;
        }
    }

    return E_FAIL;
}





//--------------------------------------------------------------------------------------
HRESULT Effect10::LoadTexture( const WCHAR* file,  IUnknown** texture)
{
    HRESULT hr = S_OK;
    ID3D10Device* pDevice = NULL;
    hr = m_pD3DEffect->GetDevice( &pDevice );
    if(FAILED(hr))
        return hr;

    hr = D3DX10CreateShaderResourceViewFromFileW(
            pDevice,
            file,
            NULL,
            NULL,
            (ID3D10ShaderResourceView**)texture,
            NULL );

    SAFE_RELEASE(pDevice);

    return hr;
}
