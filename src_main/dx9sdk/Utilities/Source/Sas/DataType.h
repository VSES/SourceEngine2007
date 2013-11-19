//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "unknwn.h"
#include "windows.h"

#include "host.h"
//#include "Value.h"
#include "FilePath.h"
#pragma warning(disable: 4995)
#include <string>
#pragma warning(default: 4995)

#ifndef _DataType_H_
#define _DataType_H_

//struct IUnknown;

namespace Sas
{
	//forward declaration
	class Effect;
    class Value;
    class Scalar;
    class Object;
 
    //-----------------------------------------------------------------------------
    class DataType
    {
    public:
        DataType( SASVARIABLE_TYPE type=SASVT_INVALID ) : m_Type(type) {}
        virtual ~DataType() {};

        SASVARIABLE_TYPE Type() const { return m_Type; }
        bool IsNumeric() const { return SASVT_BOOL == m_Type ||
                                        SASVT_INT == m_Type ||
                                        SASVT_FLOAT == m_Type; }
        // Pure virtual
        virtual DataType* Clone() const = 0;
        virtual HRESULT Cast( const DataType& type ) = 0;

        static bool Equal( const DataType& a, const DataType& b ); 
        static bool Equal( const DataType* pA, const DataType* pB )
        {
            // True if pointers are equal (including both NULL)
            if( pA == pB )
                return true;

            // False if one is NULL and the other isn't
            if( NULL == pA || NULL == pB )
                return false;
    
            return Equal( *pA, *pB );
        }

        bool GetBool() const;
        int GetInt() const;
        float GetFloat() const;
        const WCHAR* GetString() const;

    protected:
        SASVARIABLE_TYPE m_Type;
    };


    //-----------------------------------------------------------------------------
    class Bool : public DataType
    {
    public:
        Bool( bool value=false ) : DataType(SASVT_BOOL), m_Value(value) {}

        bool Get() const { return m_Value; }
        void Set( bool value ) { m_Value = value; }

        DataType* Clone() const { return new Bool(m_Value); }
        HRESULT Cast( const DataType& type );

        static bool Equal( const Bool& a, const Bool& b )
        {
            return ( a.Get() == b.Get() );
        }

    private:
        bool m_Value;
    };


    //-----------------------------------------------------------------------------
    class Int : public DataType
    {
    public:
        Int( int value=0 ) : DataType(SASVT_INT), m_Value(value) {}

        int Get() const { return m_Value; }
        void Set( int value ) { m_Value = value; }

        DataType* Clone() const { return new Int(m_Value); }
        HRESULT Cast( const DataType& type );

        static bool Equal( const Int& a, const Int& b )
        {
            return ( a.Get() == b.Get() );
        }

    private:
        int m_Value;
    };


    //-----------------------------------------------------------------------------
    class Float : public DataType
    {
    public:
        Float( float value=0 ) : DataType(SASVT_FLOAT), m_Value(value) {}

        float Get() const { return m_Value; }
        void Set( float value ) { m_Value = value; }

        DataType* Clone() const { return new Float(m_Value); }
        HRESULT Cast( const DataType& type );

        static bool Equal( const Float& a, const Float& b )
        {
            return ( a.Get() == b.Get() );
        }

    private:
        float m_Value;
    };


    //-----------------------------------------------------------------------------
    class String : public DataType
    {
    public:
        String() : DataType(SASVT_STRING) {}
        String( const WCHAR* value ) : DataType(SASVT_STRING) { Set(value); }
        String( const char* value ) : DataType(SASVT_STRING) { SetAnsi(value); }
        virtual ~String() { Set(NULL); }

        const WCHAR* Get() const { return m_Value.c_str(); }
        HRESULT Set( const WCHAR* value );
        HRESULT SetAnsi( const char* value );

        DataType* Clone() const { return new String(m_Value.c_str()); }
        HRESULT Cast( const DataType& type );

        static bool Equal( const String& a, const String& b )
        {
            if( NULL == a.Get() && NULL == b.Get() )
                return true;

            if( NULL == a.Get() || NULL == b.Get() )
                return false;

            return (0 == wcscmp(a.Get(), b.Get()) );
        }


    private:
		std::wstring m_Value; //WCHAR* m_Value;
    };


    //-----------------------------------------------------------------------------
	class Texture : public DataType
    {
    public:
        Texture( IUnknown* value = NULL) : DataType( SASVT_TEXTURE ),  m_Value(NULL) { Set(value); }
        virtual ~Texture() { Set(NULL); }

        IUnknown* Get() const { return m_Value; }

		void Set( IUnknown* value );

        DataType* Clone() const { return new Texture(m_Value); }

		virtual HRESULT Cast( const DataType& type );

        static bool Equal( const Texture& a, const Texture& b )
        {
            return ( a.Get() == b.Get() );
        }

    protected:
        IUnknown* m_Value;
    };


	template < SASVARIABLE_TYPE _TYPE_ >
	class TextureTemplate : public Texture
    {
    public:
        TextureTemplate( IUnknown* value = NULL )
		{
			m_Type = _TYPE_ ;
			m_Value = NULL; 
			Set(value); 
		}

        virtual DataType* Clone() const { return new TextureTemplate< _TYPE_ >(m_Value); }

		virtual HRESULT Cast( const DataType& type )
		{
			switch( type.Type() )
			{
			case _TYPE_ :
				Set( ((TextureTemplate< _TYPE_ >&)type).Get() );
				return S_OK;
			};

			return E_FAIL;
		}

        static bool Equal( const TextureTemplate< _TYPE_ >& a, const TextureTemplate< _TYPE_ >& b )
        {
            return ( a.Get() == b.Get() );
        }
    };


  	typedef TextureTemplate<SASVT_TEXTURE1D> Texture1D;
	typedef TextureTemplate<SASVT_TEXTURE2D> Texture2D;
	typedef TextureTemplate<SASVT_TEXTURE3D> Texture3D;
	typedef TextureTemplate<SASVT_TEXTURECUBE> TextureCube;
}

#endif //_DataType_H_
