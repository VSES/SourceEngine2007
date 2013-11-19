//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_
#pragma once
#include "stdafx.h"
//#include "Parameter.h"
//#include "BoundValueTable.h"
#include "FilePath.h"

#include <vector>
using namespace std;

namespace Sas
{
	class Variable;
	class BoundValueTable;

    //-----------------------------------------------------------------------------
    class Effect
    {
    public:
        virtual ~Effect();
      
        const SourceInfo* GetCreation() const { return &m_Creation; }

        HRESULT Bind( BoundValueTable* pTable );
        
        virtual HRESULT LoadResources();
     
        Variable* GetVariable( const WCHAR* name );
        Variable* GetVariable( const char* name );

        Variable* GetGlobal() const { return m_pGlobal; }

 		virtual HRESULT LoadTexture( const WCHAR* file,  IUnknown** texture) = 0;

    protected:
 
		// Private constructor
        Effect()
        {
            m_pGlobal = NULL;
        }

        SourceInfo m_Creation;       
        vector<Variable*> m_Variables;
        Variable* m_pGlobal;
    };


}

#endif //_EFFECT_H_