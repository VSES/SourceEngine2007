#pragma once
#include "stdafx.h"
#include "d3d10.h"
#include "d3dx10.h"

#include "Parameter.h"
#include "Effect.h"



namespace Sas
{

	class Effect10 : public Effect
	{
	 public:
		virtual ~Effect10();

		static HRESULT FromD3DEffect( ID3D10Effect* pD3DEffect, const SourceInfo& sf, Effect10** ppEffectOut );
		static HRESULT FromSource( ID3D10Device* pDevice, const SourceInfo& sf, const D3D10_SHADER_MACRO* pDefines, ID3D10Include* pInclude, DWORD HlslFlags, DWORD FxFlags, Effect10** ppEffectOut );
	 	
		virtual HRESULT LoadTexture( const WCHAR* file,  IUnknown** texture);
	   
		ID3D10Effect* GetD3DEffect(){ return m_pD3DEffect; }

	protected:

		// Private constructor
		Effect10() : Effect()
		{
			m_pD3DEffect = NULL; 
		}
	     
		ID3D10Effect* m_pD3DEffect;
	};


	class Variable10 : public Variable
	{
	public:
		Variable10( ID3D10EffectVariable* pD3DEffectVariable, Effect* effect, Variable* parent = NULL, UINT index = 0);

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


		ID3D10EffectVariable* m_pD3DEffectVariable;
	};

}