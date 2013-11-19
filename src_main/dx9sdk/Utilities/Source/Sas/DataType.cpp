//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "DataType.h"
#include "Sas.h"
using namespace Sas;


void Texture::Set( IUnknown* value )
{
	SAFE_RELEASE(m_Value);
	m_Value = value;
	if( value )
		value->AddRef();
}

//-----------------------------------------------------------------------------
bool DataType::Equal( const DataType& a, const DataType& b )
{
    if( a.Type() != b.Type() )
        return false;

    switch( a.Type() )
    {
        case SASVT_BOOL: return Bool::Equal( (Bool&)a, (Bool&)b );
        case SASVT_INT: return Int::Equal( (Int&)a, (Int&)b );
        case SASVT_FLOAT: return Float::Equal( (Float&)a, (Float&)b );
        case SASVT_STRING: return String::Equal( (String&)a, (String&)b );
        case SASVT_TEXTURE: return Texture::Equal( (Texture&)a, (Texture&)b );
        case SASVT_TEXTURE1D: return Texture1D::Equal( (Texture1D&)a, (Texture1D&)b );
        case SASVT_TEXTURE2D: return Texture2D::Equal( (Texture2D&)a, (Texture2D&)b );
        case SASVT_TEXTURE3D: return Texture3D::Equal( (Texture3D&)a, (Texture3D&)b );
        case SASVT_TEXTURECUBE: return TextureCube::Equal( (TextureCube&)a, (TextureCube&)b );
    }

    return false;
}


//-----------------------------------------------------------------------------
bool DataType::GetBool() const
{
    HRESULT hr;
    Bool b;
    
    hr = b.Cast( *this );
    if( FAILED(hr) )
        return 0;

    return b.Get();
}


//-----------------------------------------------------------------------------
int DataType::GetInt() const
{
    HRESULT hr;
    Int i;
    
    hr = i.Cast( *this );
    if( FAILED(hr) )
        return 0;

    return i.Get();
}


//-----------------------------------------------------------------------------
float DataType::GetFloat() const
{
    HRESULT hr;
    Float f;
    
    hr = f.Cast( *this );
    if( FAILED(hr) )
        return 0;

    return f.Get();
}


//-----------------------------------------------------------------------------
const WCHAR* DataType::GetString() const
{
    if( SASVT_STRING != this->Type() )
        return NULL;

    return ((String*)this)->Get();
}


/*
//-----------------------------------------------------------------------------
HRESULT Void::Cast( const DataType& type )
{
	switch( type.Type() )
	{
	case SASVT_INVALID:
		return S_OK;
	}

	return E_FAIL;
}
*/

//-----------------------------------------------------------------------------
HRESULT Bool::Cast( const DataType& type )
{
	switch( type.Type() )
	{
	case SASVT_BOOL:
		m_Value = ((Bool&)type).Get();
		return S_OK;

	case SASVT_INT:
		m_Value = ( 0 != ((Int&)type).Get() );
		return S_OK;

	case SASVT_FLOAT:
		m_Value = ( 0 != ((Float&)type).Get() );
		return S_OK;
	}

	return E_FAIL;
}


//-----------------------------------------------------------------------------
HRESULT Int::Cast( const DataType& type )
{
	switch( type.Type() )
	{
	case SASVT_BOOL:
		m_Value = ((Bool&)type).Get() ? 1 : 0;
		return S_OK;

	case SASVT_INT:
		m_Value = ((Int&)type).Get();
		return S_OK;

	case SASVT_FLOAT:
		m_Value = (int)((Float&)type).Get();
		return S_OK;
	}

	return E_FAIL;
}


//-----------------------------------------------------------------------------
HRESULT Float::Cast( const DataType& type )
{
	switch( type.Type() )
	{
	case SASVT_BOOL:
		m_Value = ((Bool&)type).Get() ? 1.0f : 0.0f;
		return S_OK;

	case SASVT_INT:
		m_Value = (float)((Int&)type).Get();
		return S_OK;

	case SASVT_FLOAT:
		m_Value = ((Float&)type).Get();
		return S_OK;
	}

	return E_FAIL;
}


//-----------------------------------------------------------------------------
HRESULT String::Cast( const DataType& type )
{
	switch( type.Type() )
	{
	case SASVT_STRING:
		return Set( ((String&)type).Get() );
	}

	return E_FAIL;
}
HRESULT String::Set( const WCHAR* value )
{
	m_Value = ( value ? value : L"" );
	return S_OK;
}


//-----------------------------------------------------------------------------
HRESULT String::SetAnsi( const char* value )
{
	CharToWString(value, m_Value);
	return S_OK;
}
//-----------------------------------------------------------------------------

HRESULT Texture::Cast( const DataType& type )
{
	switch( type.Type() )
	{
	case SASVT_TEXTURE:
		Set( ((Texture&)type).Get() );
		return S_OK;
	case SASVT_TEXTURE1D:
		Set( ((Texture1D&)type).Get() );
		return S_OK;
	case SASVT_TEXTURE2D:
		Set( ((Texture2D&)type).Get() );
		return S_OK;
	case SASVT_TEXTURE3D:
		Set( ((Texture3D&)type).Get() );
		return S_OK;
	case SASVT_TEXTURECUBE:
		Set( ((TextureCube&)type).Get() );
		return S_OK;
	}

	return E_FAIL;
}
