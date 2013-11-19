//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "BoundValueTable.h"
#include "Value.h"

//debug
#include "Parameter.h"

using namespace std;
using namespace Sas;

HRESULT BoundValueTable::PromoteToParent( const WCHAR* strBindAddress ){ return PromoteToParent( BindAddressHelper::StringToHandle(strBindAddress) ); }
HRESULT BoundValueTable::DemoteFromParent( const WCHAR* strBindAddress){ return DemoteFromParent( BindAddressHelper::StringToHandle(strBindAddress) ); }


const Value* BoundValueTable::GetValue( const WCHAR* strBindAddress, bool searchChain ) const { return GetValue( BindAddressHelper::StringToHandle(strBindAddress), searchChain ); }

float BoundValueTable::GetFloat( const WCHAR* strBindAddress, bool searchChain, HRESULT* pHR ) { return GetFloat( BindAddressHelper::StringToHandle(strBindAddress), searchChain, pHR ); }

HRESULT BoundValueTable::CastValue( const WCHAR* strBindAddress, Value* pValue, bool searchChain, const WCHAR** pStrError ) { return CastValue( BindAddressHelper::StringToHandle(strBindAddress), pValue, searchChain, pStrError ); }

HRESULT BoundValueTable::SetValue( const WCHAR* strBindAddress, const Value* pValue ) { return SetValue( BindAddressHelper::StringToHandle(strBindAddress), pValue ); }

HRESULT BoundValueTable::SetValue( BindAddressHandle handle, const Value& value ) { return SetValue( handle, &value ); }
HRESULT BoundValueTable::SetValue( const WCHAR* strBindAddress, const Value& value ) { return SetValue( BindAddressHelper::StringToHandle(strBindAddress), value ); }

HRESULT BoundValueTable::SetValue( const WCHAR* strBindAddress, const DataType& DataType ) { return SetValue( BindAddressHelper::StringToHandle(strBindAddress), DataType ); }

HRESULT BoundValueTable::SetFloat( BindAddressHandle handle, float f ) { return SetValue( handle, Scalar(Float(f)) ); }
HRESULT BoundValueTable::SetFloat( const WCHAR* strBindAddress, float f) { return SetFloat( BindAddressHelper::StringToHandle(strBindAddress), f ); }

HRESULT BoundValueTable::SetInt( BindAddressHandle handle, int n ) { return SetValue( handle, Scalar(Int(n)) ); }
HRESULT BoundValueTable::SetInt( const WCHAR* strBindAddress, int n ) { return SetInt( BindAddressHelper::StringToHandle(strBindAddress), n ); }

HRESULT BoundValueTable::SetVector( BindAddressHandle handle, const D3DXVECTOR4* pV ) { return SetValue( handle, Vector(pV) ); }
HRESULT BoundValueTable::SetVector( const WCHAR* strBindAddress, const D3DXVECTOR4* pV ) { return SetVector( BindAddressHelper::StringToHandle(strBindAddress), pV ); }

HRESULT BoundValueTable::SetMatrix( BindAddressHandle handle, const D3DXMATRIX* pM ) { return SetValue( handle, Matrix(pM) ); }
HRESULT BoundValueTable::SetMatrix( const WCHAR* strBindAddress, const D3DXMATRIX* pM ) { return SetMatrix( BindAddressHelper::StringToHandle(strBindAddress), pM ); }

//--------------------------------------------------------------------------------------
void Binding::CallListeners( BindAddressHandle handle )
{
    for( UINT i=0; i < Listeners.size(); i++ )
    {
        Listeners[i]->OnValueChanged( handle, pValue );
    }
}


//--------------------------------------------------------------------------------------
void Binding::AddListener( IBoundValueTableListener* pListener )
{
    // Don't store the listener twice
    vector<IBoundValueTableListener*>::iterator it;
    it = find(Listeners.begin(), Listeners.end(), pListener);
    if( it != Listeners.end() )
        return;

    Listeners.push_back(pListener);
}


//--------------------------------------------------------------------------------------
void Binding::RemoveListener( IBoundValueTableListener* pListener )
{
    // Find the listener in the list
    vector<IBoundValueTableListener*>::iterator it;
    it = find(Listeners.begin(), Listeners.end(), pListener);
    if( it == Listeners.end() )
        return;
	else
		Listeners.erase( it );
}


//--------------------------------------------------------------------------------------
void Binding::RemoveAllListeners()
{
    Listeners.clear();
}


//--------------------------------------------------------------------------------------
const Value* BoundValueTable::GetValue ( BindAddressHandle handle, bool searchChain ) const 
{
    const Value* pReturnValue = NULL;

    const Binding* binding = NULL;
	if( GetBinding(&binding, handle) )
	{
		if(!binding->bPromoteToParent)
		{
			pReturnValue = m_Bindings[handle].pValue;
			if( pReturnValue )
				return pReturnValue;
		}
	}

 
    // Check for a multiple address equivalent (which is the default for valid out-of-bounds scenarios)
    // Under expected usage, these values should only appear in the top-most defaults
    // table since if the value is found here, the parent will not be checked.
    BindAddressInfo info;
    if( SUCCEEDED( BindAddressHelper::GetInfo( handle, &info ) ) )
    {
        if( -1 != info.MultipleHandle )
        {
            const Value* pDefaultValue = GetValue( info.MultipleHandle, false );
            if( pDefaultValue )
                return pDefaultValue;
        }
    }
		
    // Otherwise, if not searching the chain or no parent exists the search is complete, return NULL
	if( !searchChain || m_pParent == NULL )
		return NULL;

	// Tail recursion
	return m_pParent->GetValue(handle, true);
}

HRESULT BoundValueTable::PromoteToParent( BindAddressHandle handle )
{
    Binding* binding = NULL;
	if(!GetOrAddBinding(&binding, handle, false))
		return E_FAIL;

	binding->bPromoteToParent = true;

	// Re-add the listeners in order to propagate them up the chain
    for( UINT i=0; i < binding->Listeners.size(); i++ )
    {
        // This will recurse up the chain
        if( m_pParent )
            m_pParent->AddListener( handle, binding->Listeners[i] );
    }

	return S_OK;
}

HRESULT BoundValueTable::DemoteFromParent( BindAddressHandle handle )
{
    Binding* binding = NULL;
	if(!GetOrAddBinding(&binding, handle, false))
		return E_FAIL;

	binding->bPromoteToParent = false;

	for( UINT i=0; i < binding->Listeners.size(); i++ )
    {
        // This will recurse up the chain
        if( m_pParent )
            m_pParent->RemoveListener( handle, binding->Listeners[i] );
    }

	return S_OK;
}

//--------------------------------------------------------------------------------------
HRESULT BoundValueTable::SetValue( BindAddressHandle handle, const Value* pValue )
{
    Binding* binding = NULL;
	if(!GetOrAddBinding(&binding, handle, false))
		return E_FAIL;

	if(binding->bPromoteToParent)
	{
		BindAddressInfo info;
		BindAddressHelper::GetInfo( handle, &info );
		Sas::Host::Out( Sas::LEVEL_ERROR, L"Cannot set value on BindAddress that is promoted to parent, %s", info.StrAddress.c_str() );
		return E_FAIL;
	}

	const Value*& pOldValue = binding->pValue;

    // If the current value is equal to the value being set, nothing needs to be done
    if( Value::Equal( pOldValue, pValue ) )
        return S_OK;
    
    // The old value needs to be deallocated before proceeding with the rest of the set.
	SAFE_DELETE(pOldValue);

    // Clone and store the new value
	Value* pNewValue = NULL;
	if( NULL != pValue )
	{
		pNewValue = pValue->Clone();
		if( NULL == pNewValue )
			return E_OUTOFMEMORY;
	}

    // Set the value
	binding->pValue = pNewValue;
    binding->CallListeners( handle );
    
	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT BoundValueTable::SetValue( BindAddressHandle handle, const DataType& DataType )
{
    switch( DataType.Type() )
    {
        case SASVT_BOOL:
        case SASVT_INT:
        case SASVT_FLOAT:
            return SetValue( handle, Scalar( DataType ) );

        default:
            return SetValue( handle, Object( DataType ) );
    }  
}


//--------------------------------------------------------------------------------------
// Search the current BoundValueTable, then search the parent, and so on...
// Return the first value found, null if no values found
//--------------------------------------------------------------------------------------
HRESULT BoundValueTable::CastValue( BindAddressHandle handle, Value* pValue, bool searchChain, const WCHAR** pStrError )
{
	if( NULL == pValue )
	{
		if( pStrError )
			*pStrError = L"BoundValueTable::CastValue() - Null argument: pValue";

		return E_INVALIDARG;
	}

    // Search for the value
	const Value* pStoredValue = GetValue( handle, searchChain );
	if( NULL == pStoredValue )
    {
        if( pStrError )
			*pStrError = L"BoundValueTable::CastValue() - No value found at that address";

        return E_FAIL;
    }

	// Attempt the cast
    return pValue->Cast( *pStoredValue );
}


//--------------------------------------------------------------------------------------
float BoundValueTable::GetFloat( BindAddressHandle handle, bool searchChain, HRESULT* pHR )
{
	HRESULT hr;
	float returnValue = 0.0f;
    Scalar floatScalar( SASVT_FLOAT );

    // Attempt to get the value (regardless of type), return 0.0f on error
	const Value* pValue = GetValue( handle, searchChain );
	
	// If no value was found, return 0.0f
	if( NULL == pValue )
    {
        hr = E_FAIL;
		goto LCleanReturn;
    }

	// Attempt to convert the value to a float
	hr = floatScalar.Cast( *pValue );
    if( FAILED(hr) )
        goto LCleanReturn;

	// No errors
    returnValue = ((Float*)floatScalar.Get())->Get();
	hr = S_OK;

LCleanReturn:
    if( pHR ) 
        *pHR = hr;

	return returnValue;
}


//--------------------------------------------------------------------------------------

bool BoundValueTable::GetOrAddBinding( Binding** result, BindAddressHandle handle, bool OkToAdd)
{
	if(result == NULL)
		return false;

    // Negative handles are invalid
	if( handle < 0 )
		return false;
	
	if( !OkToAdd && (handle >= (int)m_Bindings.size()) )
		return false;

	// Extend the 0-based list of values to accomodate all handles (indices) up to and
	// including the new handle
	while(handle >= (int)m_Bindings.size())
	{
		m_Bindings.push_back( Binding() );
	}

	*result = &m_Bindings[handle];

	return true;
}

bool BoundValueTable::GetBinding( const Binding** result, BindAddressHandle handle) const
{
	if(result == NULL)
		return false;

    // Negative handles are invalid
	if( handle < 0 )
		return false;

	if( handle >= (int)m_Bindings.size())
		return false;

	*result = &m_Bindings[handle];

	return true;
}

void BoundValueTable::AddListener( BindAddressHandle handle, IBoundValueTableListener* pListener )
{
    Binding* binding = NULL;
	if(!GetOrAddBinding(&binding, handle, true))
		return;

    binding->AddListener( pListener );

    // If the listener is placed upon a NULL value, then the listener will automatically
    // be propagated up the parent chain until a value is found
    if( binding->bPromoteToParent && m_pParent )
        m_pParent->AddListener( handle, pListener );
}


//--------------------------------------------------------------------------------------
void BoundValueTable::RemoveListener( BindAddressHandle handle, IBoundValueTableListener* pListener )
{
    Binding* binding = NULL;
	if(!GetOrAddBinding(&binding, handle, false))
		return;

	binding->RemoveListener( pListener );

	if(binding->bPromoteToParent && m_pParent)
        m_pParent->RemoveListener( handle, pListener );
}


//--------------------------------------------------------------------------------------
void BoundValueTable::RemoveAllListeners( BindAddressHandle handle )
{
    Binding* binding = NULL;
	if(!GetOrAddBinding(&binding, handle, false))
		return;
	
	if(binding->bPromoteToParent && m_pParent)
	{
		for( vector<IBoundValueTableListener*>::iterator itr = binding->Listeners.begin(); 
			itr != binding->Listeners.end();
			itr++ )
		{
			m_pParent->RemoveListener( handle, *itr );
		}
	}

	binding->RemoveAllListeners();
}

Binding::~Binding() 
{ 
	RemoveAllListeners(); 
}

//--------------------------------------------------------------------------------------
void BoundValueTable::Clear() 
{
    // Bindings are passed by value and use shallow copying, so they don't manage their
    // own memory beyond the listeners
	for( UINT i=0; i < m_Bindings.size(); i++ )
    {
        SAFE_DELETE( m_Bindings[i].pValue );
		RemoveAllListeners(i);
    }

    // Clearing the bindings list will call the Binding destructor, which cleans up 
    // listeners
	m_Bindings.clear(); 
}


//--------------------------------------------------------------------------------------
void BoundValueTable::Print()
{
    WCHAR buffer[32];
    
    OutputDebugString( L"BoundValueTable---------------\n" );
    OutputDebugString( L"Bindings: " );
	StringCchPrintf(buffer, 10, L"%d", (int)m_Bindings.size() );
    OutputDebugString( buffer );
    OutputDebugString( L"\n" );

    for( UINT i=0; i < m_Bindings.size(); i++ )
    {
        OutputDebugString( BindAddressHelper::HandleToString(i) );
        OutputDebugString( L" Listeners: " );
		StringCchPrintf(buffer, 10, L"%d", (int)(m_Bindings[i].Listeners.size()) );
        OutputDebugString( buffer );
        OutputDebugString( L"\n" );
    }
}

