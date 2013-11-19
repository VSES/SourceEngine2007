//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#pragma warning(disable: 4995)
#include <strsafe.h>
#include <shlwapi.h>
#include <shlobj.h>
#pragma warning(default: 4995)
#include "Effect.h"
#include "sas.h"
using namespace Sas;


//--------------------------------------------------------------------------------------
Effect::~Effect()
{
	Bind(NULL);
    for( UINT i=0; i < m_Variables.size(); i++ )
    {
        LOOPSAFE_DELETE( m_Variables[i], Variable );
    }
	m_Variables.clear();
}
//--------------------------------------------------------------------------------------
HRESULT Effect::Bind( BoundValueTable* pTable )
{
    HRESULT hr;
    HRESULT hrReturn = S_OK;

    for( UINT i=0; i < m_Variables.size(); i++ )
    {
		Variable* variable = m_Variables[i];
		if(variable)
		{
			Parameter* parameter = variable->GetParameter();
			if(parameter)
			{
				hr = parameter->Bind( pTable );
				if( FAILED(hr) )
					hrReturn = hr;
			}
		}
    }

    return hrReturn;
}
//--------------------------------------------------------------------------------------
HRESULT Effect::LoadResources()
{
    HRESULT hr;

    for( UINT i=0; i < m_Variables.size(); i++ )
    {
        Parameter* pParameter = m_Variables[i]->GetParameter();
        if( NULL == pParameter )
            continue;

        const WCHAR* resourcePath = pParameter->GetResourceAddress();
        if( resourcePath && wcslen(resourcePath) > 0 )
        {

			SourceInfo file;
			if(! Host::GetIntegrator()->FindFile(resourcePath, m_Creation.GetDirectoryPath(), file ) )
            {
                //TODO - warning string
                continue;
            }

            hr = pParameter->GetVariable()->SetValue( String(file.GetFilePath()) );
            if( FAILED(hr) )
            {
                //TODO - warning string
                continue;
            }

        }
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
Variable* Effect::GetVariable( const WCHAR* name )
{
    for( UINT i=0; i < m_Variables.size(); i++ )
    {
		Variable* var = m_Variables[i];
        if( 0 == _wcsicmp( name, var->Desc().Name.c_str() ) )
            return var;
    }

    return NULL;
}


//--------------------------------------------------------------------------------------
Variable* Effect::GetVariable( const char* name )
{
	wstring wstr;
	CharToWString(name, wstr);

    return GetVariable( wstr.c_str() );
}

