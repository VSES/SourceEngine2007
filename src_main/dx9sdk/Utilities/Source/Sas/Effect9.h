#pragma once
#include "stdafx.h"
#include "d3d9.h"
#include "d3dx9.h"

#include "Parameter.h"
#include "Effect.h"

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
    LPD3DXBUFFER*                   ppCompilationErrors);


namespace Sas
{

	class Effect9 : public Effect
	{
	 public:
		virtual ~Effect9();

		static HRESULT FromD3DXEffect( ID3DXEffect* pD3DXEffect, const SourceInfo& sf, Effect9** ppEffectOut );
		static HRESULT FromSource( IDirect3DDevice9* pDevice, const SourceInfo& sf, const D3DXMACRO* pDefines, ID3DXInclude* pInclude, DWORD flags, Effect9** ppEffectOut );

		HRESULT OnLostDevice();
		HRESULT OnResetDevice();
	 	
		virtual HRESULT LoadTexture( const WCHAR* file,  IUnknown** texture);
	   
		ID3DXEffect* GetD3DXEffect(){ return m_pD3DXEffect; }

	protected:

		// Private constructor
		Effect9() : Effect()
		{
			m_pD3DXEffect = NULL; 
		}
	     
		ID3DXEffect* m_pD3DXEffect;
	};


	class Variable9 : public Variable
	{
	public:
		Variable9( ID3DXEffect* pD3DXEffect, D3DXHANDLE handle, Effect* effect, Variable* parent = NULL, UINT index = 0);

	protected:
        virtual HRESULT GetScalar_Internal( Scalar* pScalar ) const;
        virtual HRESULT GetVector_Internal( Vector* pVector ) const;
        virtual HRESULT GetMatrix_Internal( Matrix* pMatrix ) const;
        virtual HRESULT GetObject_Internal( Object* pObject ) const;

        virtual HRESULT SetScalar_Internal( Scalar* pScalar );
        virtual HRESULT SetVector_Internal( Vector* pVector );
        virtual HRESULT SetMatrix_Internal( Matrix* pMatrix );
        virtual HRESULT SetObject_Internal( Object* pObject );

		virtual HRESULT SpecializedCasts( Value* pCached, const Value& value );


		ID3DXEffect* m_pD3DXEffect;
		D3DXHANDLE m_Handle;
	};

}