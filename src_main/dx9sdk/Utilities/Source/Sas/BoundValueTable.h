//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#ifndef _BOUNDVALUETABLE_H_
#define _BOUNDVALUETABLE_H_
#include "BindAddress.h"
//#include "Value.h"
//#include <vector>
//#include <algorithm>
using namespace std;

namespace Sas
{
	class Value;
	class Scalar;
	class Vector;
	class Matrix;
	class Array;
	class Struct;
	class DataType;
	class Float;
	class Int;
	class Bool;
	class String;
	class Object;

    //TODO - The BoundValueTable needs links to the children so that it can inform
    // about updates to values which appear only in the parent


    class IBoundValueTableListener
    {
    public:
        virtual void OnValueChanged( Sas::BindAddressHandle handle, const Sas::Value* pNewValue ) = 0;    
    };


    // Note: The BoundValueTable manages pValue memory allocation/deallocation
    struct Binding
    {
        Binding( const Value* pV=NULL ) : pValue(pV), bPromoteToParent(false) {}
        ~Binding();

        void CallListeners( BindAddressHandle handle );
        void AddListener( IBoundValueTableListener* pListener );
        void RemoveListener( IBoundValueTableListener* pListener );
        void RemoveAllListeners();

		bool bPromoteToParent;
        const Value* pValue;
        vector< IBoundValueTableListener* > Listeners;
    };


    class BoundValueTable
	{
	public:
        ~BoundValueTable() { Clear(); }

		BoundValueTable( BoundValueTable* pParent=NULL ) : m_pParent(pParent) { Clear(); }
		BoundValueTable* CreateChild() { return new BoundValueTable(this); }

		BoundValueTable* GetParent() const { return m_pParent; }

		HRESULT PromoteToParent( BindAddressHandle handle );
		HRESULT PromoteToParent( const WCHAR* strBindAddress );
		HRESULT DemoteFromParent( BindAddressHandle handle );
		HRESULT DemoteFromParent( const WCHAR* strBindAddress);

        const Value* GetValue( BindAddressHandle handle, bool searchChain=true ) const;
        const Value* GetValue( const WCHAR* strBindAddress, bool searchChain=true ) const;

        float GetFloat( BindAddressHandle handle, bool searchChain=true, HRESULT* pHR=NULL );
		float GetFloat( const WCHAR* strBindAddress, bool searchChain=true, HRESULT* pHR=NULL );

        HRESULT CastValue( BindAddressHandle handle, Value* pValue, bool searchChain=true, const WCHAR** pStrError=NULL );
		HRESULT CastValue( const WCHAR* strBindAddress, Value* pValue, bool searchChain=true, const WCHAR** pStrError=NULL );

		HRESULT SetValue( BindAddressHandle handle, const Value* pValue );
		HRESULT SetValue( const WCHAR* strBindAddress, const Value* pValue );
        
        HRESULT SetValue( BindAddressHandle handle, const Value& value );
        HRESULT SetValue( const WCHAR* strBindAddress, const Value& value );
        
        HRESULT SetValue( BindAddressHandle handle, const DataType& DataType );
        HRESULT SetValue( const WCHAR* strBindAddress, const DataType& DataType );
		
        HRESULT SetFloat( BindAddressHandle handle, float f );
        HRESULT SetFloat( const WCHAR* strBindAddress, float f);

        HRESULT SetInt( BindAddressHandle handle, int n );
        HRESULT SetInt( const WCHAR* strBindAddress, int n );

        HRESULT SetVector( BindAddressHandle handle, const D3DXVECTOR4* pV );
        HRESULT SetVector( const WCHAR* strBindAddress, const D3DXVECTOR4* pV );

        HRESULT SetMatrix( BindAddressHandle handle, const D3DXMATRIX* pM );
        HRESULT SetMatrix( const WCHAR* strBindAddress, const D3DXMATRIX* pM );

        void AddListener( BindAddressHandle handle, IBoundValueTableListener* pListener );
        void RemoveListener( BindAddressHandle handle, IBoundValueTableListener* pListener );
        void RemoveAllListeners( BindAddressHandle handle );

		void Clear();

        void Print();
		
		std::wstring Name;
	private:
		bool GetOrAddBinding(  Binding** result, BindAddressHandle handle, bool OkToAdd);
		bool GetBinding( const Binding** result, BindAddressHandle handle) const;

		BoundValueTable* m_pParent;
        vector<Binding> m_Bindings;
	};
}

#endif //_BOUNDVALUETABLE_H_