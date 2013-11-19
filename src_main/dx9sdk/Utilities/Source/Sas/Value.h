//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include "DataType.h"
//#include <vector>
using namespace std;

namespace Sas
{
    // Forward declarations
    class DataType;
        

    //--------------------------------------------------------------------------------------
    class Value
    {
    public:
        Value( SASVARIABLE_CLASS eClass=SASVC_INVALID, SASVARIABLE_TYPE eType=SASVT_INVALID ) : 
            m_Class(eClass), 
            m_Type(eType) {};

        virtual ~Value();

        
        // Virtual functions
        virtual Value* Clone() const = 0;
        virtual HRESULT Cast( const Value& value ) = 0; 
        virtual bool IsNumeric() const { return false; }

        // TODO - A Value is valid if and only if:
        // 1) All the contained Values are valid
        // 2) All the contained DataTypes pointers are set to instances with types
        //    that match the Value's type
        virtual bool IsValid() const { return false; }
        
        SASVARIABLE_CLASS Class() const { return m_Class; }
        SASVARIABLE_TYPE Type() const { return m_Type; }
        
        virtual HRESULT Cast( const DataType& DataType ); // Wraps as a value and calls Cast()

        static bool Equal( const Value& a, const Value& b );
        static bool Equal( const Value* pA, const Value* pB )
        {
            // True if pointers are equal (including both NULL)
            if( pA == pB )
                return true;

            // False if one is NULL and the other isn't
            if( NULL == pA || NULL == pB )
                return false;
    
            return Equal( *pA, *pB );
        }


    protected:
        static HRESULT Set_Internal_V_V( Value*& pOld, const Value* pNew );
        static HRESULT Set_Internal_V_PT( Value*& pOld, const DataType* pNew );
        static HRESULT Set_Internal_PT_V( DataType*& pOld, const Value* pNew );
        static HRESULT Set_Internal_PT_PT( DataType*& pOld, const DataType* pNew );
     
        static HRESULT Cast_Internal_V_V( Value*& pDest, const Value* pSrc );
        static HRESULT Cast_Internal_V_PT( Value*& pDest, const DataType* pSrc );
        static HRESULT Cast_Internal_PT_V( DataType*& pDest, const Value* pSrc );
        static HRESULT Cast_Internal_PT_PT( DataType*& pDest, const DataType* pSrc );
        
     
    private:
        SASVARIABLE_CLASS m_Class;
        SASVARIABLE_TYPE m_Type;
    };


    //--------------------------------------------------------------------------------------
    class Object : public Value
    {
    public:
        Object( SASVARIABLE_TYPE type, DataType* pValue=NULL );

        Object( const DataType& value );

        virtual ~Object();

        virtual Value* Clone() const;

        virtual HRESULT Cast( const Value& value );
        
        virtual bool IsNumeric() const;
        
        static bool Equal( Object& a, Object& b );

        DataType* Get() const;
        HRESULT Set( const DataType* pDataType );
        HRESULT Set( const DataType& DataType );

        const WCHAR* GetString();

    private:
        DataType* m_pDataType;
    };


    //--------------------------------------------------------------------------------------
    class Scalar : public Value
    {
    public:
        Scalar( SASVARIABLE_TYPE type, DataType* pValue=NULL );
        Scalar( const DataType& value );

        virtual ~Scalar();

        virtual Value* Clone() const 
        { 
            return new Scalar( Type(), m_pDataType );
        }

        virtual HRESULT Cast( const Value& value );
        
        virtual bool IsNumeric() const;
        
        static bool Equal( Scalar& a, Scalar& b );

        DataType* Get() const;
        HRESULT Set( const DataType* pDataType );
        HRESULT Set( const DataType& DataType );
        
        float GetFloat();

        int GetInt();

        bool GetBool();

    private:
        DataType* m_pDataType;
    };


    //--------------------------------------------------------------------------------------
    class Vector : public Value
    {
    public:
        Vector( SASVARIABLE_TYPE type, UINT length ) : 
            Value(SASVC_VECTOR, type), 
            m_Length(length) 
        { 
            ZeroMemory( m_pDataTypes, sizeof(m_pDataTypes) );
        }

        Vector( const D3DXVECTOR4* pVector ) :
            Value(SASVC_VECTOR, SASVT_FLOAT),
            m_Length(4)
        {
            ZeroMemory( m_pDataTypes, sizeof(m_pDataTypes) );
            Set( pVector );
        }

        Vector( const D3DXVECTOR3* pVector ) :
            Value(SASVC_VECTOR, SASVT_FLOAT),
            m_Length(3)
        {
            ZeroMemory( m_pDataTypes, sizeof(m_pDataTypes) );
            Set( pVector );
        }

        Vector( const D3DXVECTOR2* pVector ) :
            Value(SASVC_VECTOR, SASVT_FLOAT),
            m_Length(2)
        {
            ZeroMemory( m_pDataTypes, sizeof(m_pDataTypes) );
            Set( pVector );
        }

        Vector( D3DCOLORVALUE color ) :
            Value(SASVC_VECTOR, SASVT_FLOAT),
            m_Length(4)
        {
            ZeroMemory( m_pDataTypes, sizeof(m_pDataTypes) );
            Set( color );
        }

        virtual ~Vector();

        virtual Value* Clone() const;
        virtual HRESULT Cast( const Value& value );
        
        UINT Length() const { return m_Length; }
        
        static bool Equal( Vector& a, Vector& b );
        
        DataType* Get( UINT index ) const 
        { 
            if( index >= Length() ) 
                return NULL;
 
            return m_pDataTypes[index];
        }
        
        HRESULT Set( UINT index, const DataType* pDataType ) 
        { 
            if( index >= Length() ) 
                return E_INVALIDARG;
 
            return Set_Internal_PT_PT( m_pDataTypes[index], pDataType );
        }

        HRESULT Set( UINT index, const DataType& DataType ) { return Set( index, &DataType ); }

        HRESULT Set( UINT index, bool value );
        HRESULT Set( UINT index, int value );
        HRESULT Set( UINT index, float value );

        HRESULT Set( const D3DXVECTOR4* pVector );

        HRESULT Set( const D3DXVECTOR3* pVector );

        HRESULT Set( const D3DXVECTOR2* pVector );

        HRESULT Set( D3DCOLORVALUE color );

        void SetAll( DataType* pDataType )
        {
            for( UINT i=0; i < Length(); i++ )
            {
                Set( i, pDataType );
            }
        }
        
    private:
        UINT m_Length;
        DataType* m_pDataTypes[4];
    };


    //--------------------------------------------------------------------------------------
    class Matrix : public Value
    {
    public:
        Matrix( SASVARIABLE_TYPE type, UINT rows, UINT columns ) : 
            Value(SASVC_MATRIX, type), 
            m_Rows(rows), 
            m_Columns(columns) 
        { 
            ZeroMemory( m_pDataTypes, sizeof(m_pDataTypes) );
        }

        Matrix( const D3DXMATRIX* pMatrix ) :
            Value(SASVC_MATRIX, SASVT_FLOAT),
            m_Rows(4),
            m_Columns(4)
        {
            ZeroMemory( m_pDataTypes, sizeof(m_pDataTypes) );
            Set( pMatrix );
        }

        virtual ~Matrix();

        virtual Value* Clone() const;
        virtual HRESULT Cast( const Value& value );
        
        UINT Rows() const { return m_Rows; }
        UINT Columns() const { return m_Columns; }

        static bool Equal( Matrix& a, Matrix& b );

        DataType* Get( UINT row, UINT column ) const 
        { 
            if( row >= Rows() || column >= Columns() )
                return NULL;

            return m_pDataTypes[row][column];
        }
       
        HRESULT Set( UINT row, UINT column, const DataType* pDataType )
        {
            if( row >= Rows() || column >= Columns() )
                return NULL;

            return Set_Internal_PT_PT( m_pDataTypes[row][column], pDataType );
        }

        HRESULT Set( UINT row, UINT column, const DataType& DataType ) { return Set(row, column, &DataType); }
        
        HRESULT Set( UINT row, UINT column, float f );

        HRESULT Set( const D3DXMATRIX* pMatrix )
        {
            HRESULT hr;

            if( NULL == pMatrix )
                return E_INVALIDARG;
            
            hr = Set( 0, 0, pMatrix->_11 );  if( FAILED(hr) ) return hr;
            hr = Set( 0, 1, pMatrix->_12 );  if( FAILED(hr) ) return hr;
            hr = Set( 0, 2, pMatrix->_13 );  if( FAILED(hr) ) return hr;
            hr = Set( 0, 3, pMatrix->_14 );  if( FAILED(hr) ) return hr;

            hr = Set( 1, 0, pMatrix->_21 );  if( FAILED(hr) ) return hr;
            hr = Set( 1, 1, pMatrix->_22 );  if( FAILED(hr) ) return hr;
            hr = Set( 1, 2, pMatrix->_23 );  if( FAILED(hr) ) return hr;
            hr = Set( 1, 3, pMatrix->_24 );  if( FAILED(hr) ) return hr;

            hr = Set( 2, 0, pMatrix->_31 );  if( FAILED(hr) ) return hr;
            hr = Set( 2, 1, pMatrix->_32 );  if( FAILED(hr) ) return hr;
            hr = Set( 2, 2, pMatrix->_33 );  if( FAILED(hr) ) return hr;
            hr = Set( 2, 3, pMatrix->_34 );  if( FAILED(hr) ) return hr;

            hr = Set( 3, 0, pMatrix->_41 );  if( FAILED(hr) ) return hr;
            hr = Set( 3, 1, pMatrix->_42 );  if( FAILED(hr) ) return hr;
            hr = Set( 3, 2, pMatrix->_43 );  if( FAILED(hr) ) return hr;
            hr = Set( 3, 3, pMatrix->_44 );  if( FAILED(hr) ) return hr;

            return S_OK;
        }


        void SetAll( DataType* pDataType )
        {
            for( UINT r=0; r < Rows(); r++ )
            {
                for( UINT c=0; c < Columns(); c++ )
                {
                    Set( r, c, pDataType );
                }
            }
        }

    private:
        UINT m_Rows;
        UINT m_Columns;
        DataType* m_pDataTypes[4][4];
    };


    //--------------------------------------------------------------------------------------
    class Array : public Value
    {
    public:
        Array( SASVARIABLE_TYPE type, UINT length ) : Value(SASVC_ARRAY, type)
        { 
            for( UINT i=0; i < length; i++ )
            {
                m_Values.push_back(NULL);
            }
        }

        virtual ~Array();

        virtual Value* Clone() const;

        virtual HRESULT Cast( const Value& value ); 
        

        UINT Length() const { return (UINT)m_Values.size(); }
        
        static bool Equal( Array& a, Array& b )
        {
            if( a.Type() != b.Type() )
                return false;

            if( a.Length() != b.Length() )
                return false;

            for( UINT i=0; i < a.Length(); i++ )
            {
                Value* pA = a.Get(i);
                Value* pB = b.Get(i);

                if( !Value::Equal( pA, pB ) )
                    return false;
            }

            return true;
        }
        
        Value* Get( UINT index ) const 
        { 
            if( index >= Length() )
                return NULL;

            return m_Values[index]; 
        }

        HRESULT Set( UINT index, const Value* pValue )
        {
            if( index >= Length() )
                return E_INVALIDARG;

            return Set_Internal_V_V( m_Values[index], pValue );
        }

        HRESULT Set( UINT index, const Value& value ) { return Set(index, &value); }
        
        void SetAll( Value* pValue )
        {
            for( UINT i=0; i < Length(); i++ )
            {
                Set( i, pValue );
            }
        }

        HRESULT Set( UINT index, bool value );
        HRESULT Set( UINT index, int value );
        HRESULT Set( UINT index, float value );
        
		HRESULT Set( bool* value );
        HRESULT Set( int* value );
        HRESULT Set( float* value );

        int GetInt( UINT index ) const;
        

    private:
        vector<Value*> m_Values;
    };
  

    //--------------------------------------------------------------------------------------
    class Struct : public Value
    {
    public:
        Struct( UINT length ) : Value(SASVC_STRUCT, SASVT_INVALID)
        { 
            for( UINT i=0; i < length; i++ )
            {
                m_Values.push_back(NULL);
            }
        }

        virtual ~Struct();

        virtual Value* Clone() const;
        virtual HRESULT Cast( const Value& value );
        

        UINT Length() const { return (UINT)m_Values.size(); }

        static bool Equal( Struct& a, Struct& b )
        {
            if( a.Type() != b.Type() )
                return false;

            if( a.Length() != b.Length() )
                return false;

            for( UINT i=0; i < a.Length(); i++ )
            {
                Value* pA = a.Get(i);
                Value* pB = b.Get(i);

                if( !Value::Equal( pA, pB ) )
                    return false;
            }

            return true;
        }


        Value* Get( UINT index ) const 
        { 
            if( index >= Length() )
                return NULL;

            return m_Values[index]; 
        }

        HRESULT Set( UINT index, Value* pValue )
        {
            if( index >= Length() )
                return E_INVALIDARG;

            return Set_Internal_V_V( m_Values[index], pValue );
        }

        HRESULT Set( UINT index, Value& value ) { return Set( index, &value ); }
        
        HRESULT Set( UINT index, const DataType* pDataType ) 
        { 
            if( index >= Length() ) 
                return E_INVALIDARG;
 
            return Set_Internal_V_PT( m_Values[index], pDataType );
        }

        HRESULT Set( UINT index, const DataType& DataType ) { return Set( index, &DataType ); }


        void SetAll( Value* pValue )
        {
            for( UINT i=0; i < Length(); i++ )
            {
                Set( i, pValue );
            }
        }

    private:
        vector<Value*> m_Values;
    };
}