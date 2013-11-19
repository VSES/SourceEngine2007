//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Value.h"
using namespace Sas;


HRESULT Vector::Set( const D3DXVECTOR4* pVector )
{
    HRESULT hr;

    if( NULL == pVector )
        return E_INVALIDARG;
    
    hr = Set( 0, pVector->x );  if( FAILED(hr) ) return hr;
    hr = Set( 1, pVector->y );  if( FAILED(hr) ) return hr;
    hr = Set( 2, pVector->z );  if( FAILED(hr) ) return hr;
    hr = Set( 3, pVector->w );  if( FAILED(hr) ) return hr;

    return S_OK;
}

HRESULT Vector::Set( const D3DXVECTOR3* pVector )
{
    HRESULT hr;

    if( NULL == pVector )
        return E_INVALIDARG;
    
    hr = Set( 0, pVector->x );  if( FAILED(hr) ) return hr;
    hr = Set( 1, pVector->y );  if( FAILED(hr) ) return hr;
    hr = Set( 2, pVector->z );  if( FAILED(hr) ) return hr;

    return S_OK;
}

HRESULT Vector::Set( const D3DXVECTOR2* pVector )
{
    HRESULT hr;

    if( NULL == pVector )
        return E_INVALIDARG;
    
    hr = Set( 0, pVector->x );  if( FAILED(hr) ) return hr;
    hr = Set( 1, pVector->y );  if( FAILED(hr) ) return hr;

    return S_OK;
}

HRESULT Vector::Set( D3DCOLORVALUE color )
{
    HRESULT hr;

    D3DXCOLOR d3dxcolor(color.r, color.g, color.b, color.a);

    hr = Set( 0, d3dxcolor.r );
    hr = Set( 1, d3dxcolor.g );
    hr = Set( 2, d3dxcolor.b );
    hr = Set( 3, d3dxcolor.a );

    return S_OK;
}


Object::Object( SASVARIABLE_TYPE type, DataType* pValue ) : 
    Value(SASVC_OBJECT, type), 
    m_pDataType(NULL) 
{
    Set(pValue);
}

Object::Object( const DataType& value ) : 
    Value(SASVC_OBJECT, value.Type()),
    m_pDataType(NULL) 
{ 
    Set(&value); 
}
bool Object::IsNumeric() const { return (m_pDataType && m_pDataType->IsNumeric()); }

bool Object::Equal( Object& a, Object& b )
{
    if( a.Type() != b.Type() )
        return false;

    DataType* pA = a.Get();
    DataType* pB = b.Get();

    return DataType::Equal( pA, pB );
}

DataType* Object::Get() const { return m_pDataType; }
HRESULT Object::Set( const DataType* pDataType ) { return Set_Internal_PT_PT(m_pDataType, pDataType); }
HRESULT Object::Set( const DataType& DataType ) { return Set(&DataType); }

const WCHAR* Object::GetString() 
{ 
    DataType* pPT = Get();
    if( NULL == pPT )
        return NULL;
    
    return pPT->GetString(); 
}

Value* Object::Clone() const 
{ 
    return new Object( Type(), m_pDataType );
}

//-----------------------------------------------------------------------------




Scalar::Scalar( SASVARIABLE_TYPE type, DataType* pValue ) : 
    Value(SASVC_SCALAR, type), 
    m_pDataType(NULL) 
{
    Set(pValue);
}

Scalar::Scalar( const DataType& value ) : 
    Value(SASVC_SCALAR, value.Type()),
    m_pDataType(NULL) 
{ 
    Set(&value); 
}

bool Scalar::IsNumeric() const { return (m_pDataType && m_pDataType->IsNumeric()); }

 bool Scalar::Equal( Scalar& a, Scalar& b )
{
    if( a.Type() != b.Type() )
        return false;

    DataType* pA = a.Get();
    DataType* pB = b.Get();

    return DataType::Equal( pA, pB );
}

DataType* Scalar::Get() const { return m_pDataType; }
HRESULT Scalar::Set( const DataType* pDataType ) { return Set_Internal_PT_PT(m_pDataType, pDataType); }
HRESULT Scalar::Set( const DataType& DataType ) { return Set(&DataType); }

float Scalar::GetFloat() 
{ 
    DataType* pPT = Get();
    if( NULL == pPT )
        return 0;
    
    return pPT->GetFloat(); 
}

int Scalar::GetInt() 
{ 
    DataType* pPT = Get();
    if( NULL == pPT )
        return 0;
    
    return pPT->GetInt(); 
}

bool Scalar::GetBool()
{
    DataType* pPT = Get();
    if( NULL == pPT )
        return false;

    return pPT->GetBool();
}
//-----------------------------------------------------------------------------
bool Vector::Equal( Vector& a, Vector& b )
{
    if( a.Type() != b.Type() )
        return false;

    if( a.Length() != b.Length() )
        return false;

    for( UINT i=0; i < a.Length(); i++ )
    {
        DataType* pA = a.Get(i);
        DataType* pB = b.Get(i);

        if( !DataType::Equal( pA, pB ) )
            return false;
    }

    return true;
}

HRESULT Vector::Set( UINT index, bool value ) { return Set( index, &Bool(value) ); }
HRESULT Vector::Set( UINT index, int value ) { return Set( index, &Int(value) ); }
HRESULT Vector::Set( UINT index, float value ) { return Set( index, &Float(value) ); } 

//-----------------------------------------------------------------------------

bool Matrix::Equal( Matrix& a, Matrix& b )
{
    if( a.Type() != b.Type() )
        return false;

    if( a.Rows() != b.Rows() ||
        a.Columns() != b.Columns() )
        return false;

    for( UINT r=0; r < a.Rows(); r++ )
    {
        for( UINT c=0; c < a.Columns(); c++ )
        {
            DataType* pA = a.Get(r, c);
            DataType* pB = b.Get(r, c);

            if( !DataType::Equal( pA, pB ) )
                return false;
        }
    }

    return true;
}
 
HRESULT Matrix::Set( UINT row, UINT column, float f ) { return Set(row, column, Float(f)); }


//-----------------------------------------------------------------------------

HRESULT Array::Set( UINT index, bool value ) { return Set( index, &Scalar(Bool(value)) ); }
HRESULT Array::Set( UINT index, int value ) { return Set( index, &Scalar(Int(value)) ); }
HRESULT Array::Set( UINT index, float value ) { return Set( index, &Scalar(Float(value)) ); } 
HRESULT Array::Set( bool* value ) 
{ 
	HRESULT hr = S_OK;
	for( UINT i = 0; i < Length() && SUCCEEDED(hr) ; i++ )
		hr = Set( i, &Scalar(Bool(value[i])) ); 
	return hr;
}

HRESULT Array::Set( int* value ) 
{ 
	HRESULT hr = S_OK;
	for( UINT i = 0; i < Length() && SUCCEEDED(hr) ; i++ )
		hr = Set( i, &Scalar(Int(value[i])) ); 
	return hr;
}

HRESULT Array::Set( float* value ) 
{ 
	HRESULT hr = S_OK;
	for( UINT i = 0; i < Length() && SUCCEEDED(hr) ; i++ )
		hr = Set( i, &Scalar(Float(value[i])) ); 
	return hr;
} 

int Array::GetInt( UINT index ) const
{
    Value* pValue = Get(index);
    if( NULL == pValue ||
        SASVT_INT != pValue->Type() )
    {
        return 0;
    }

    return ((Scalar*)pValue)->GetInt();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Value::~ Value()
{

}

Object::~Object() 
{ 
	Set(NULL); 
}

Scalar::~Scalar() 
{ 
	Set(NULL); 
}

Vector::~Vector() 
{ 
	SetAll(NULL); 
}

Matrix::~Matrix() 
{ 
	SetAll(NULL); 
}

Array::~Array() 
{ 
    SetAll(NULL); 
    m_Values.clear(); 
}

Struct::~Struct() 
{ 
    SetAll(NULL); 
    m_Values.clear();
}

//-----------------------------------------------------------------------------
bool Value::Equal( const Value& a, const Value& b )
{
    if( a.Class() != b.Class() )
        return false;

    switch( a.Class() )
    {
        case SASVC_SCALAR: return Scalar::Equal( (Scalar&)a, (Scalar&)b );
        case SASVC_VECTOR: return Vector::Equal( (Vector&)a, (Vector&)b );
        case SASVC_MATRIX: return Matrix::Equal( (Matrix&)a, (Matrix&)b );
        case SASVC_OBJECT: return Object::Equal( (Object&)a, (Object&)b );
        case SASVC_STRUCT: return Struct::Equal( (Struct&)a, (Struct&)b );
        case SASVC_ARRAY: return Array::Equal( (Array&)a, (Array&)b );
    }

    return false;
}

//-----------------------------------------------------------------------------
HRESULT Value::Set_Internal_PT_PT( DataType*& pOld, const DataType* pNew )
{
 	SAFE_DELETE(pOld);

	if( pNew )
	{
		pOld = pNew->Clone();
        if( NULL == pOld )
            return E_OUTOFMEMORY;
	}

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT Value::Set_Internal_PT_V( DataType*& pOld, const Value* pNew )
{   
    if( pNew == NULL )
        return Set_Internal_PT_PT( pOld, NULL );

     switch( pNew->Class() )
    {
        case SASVC_SCALAR:
            return Set_Internal_PT_PT( pOld, ((Scalar*)pNew)->Get() );

        case SASVC_OBJECT:
            return Set_Internal_PT_PT( pOld, ((Object*)pNew)->Get() );
    }

	return E_INVALIDARG;
}


//-----------------------------------------------------------------------------
HRESULT Value::Set_Internal_V_V( Value*& pOld, const Value* pNew )
{
	SAFE_DELETE(pOld);

	if( pNew )
	{
		pOld = pNew->Clone();
        if( NULL == pOld )
            return E_OUTOFMEMORY;
	}

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT Value::Set_Internal_V_PT( Value*& pOld, const DataType* pNew )
{   
    if( pNew == NULL )
        return Set_Internal_V_V( pOld, NULL );

    Scalar scalar( *pNew );
    Object object( *pNew );
                    
    switch( pNew->Type() )
    {
        case SASVT_INVALID:
            return E_INVALIDARG;

        case SASVT_BOOL:
        case SASVT_INT:
        case SASVT_FLOAT:
            return Set_Internal_V_V( pOld, &scalar );

        default:
            return Set_Internal_V_V( pOld, &object );
    }
}

//--------------------------------------------------------------------------------------
HRESULT Value::Cast_Internal_PT_V( DataType*& pDest, const Value* pSrc )
{   
    if( pSrc == NULL )
        return Cast_Internal_PT_PT( pDest, NULL );

     switch( pSrc->Class() )
    {
        case SASVC_SCALAR:
            return Cast_Internal_PT_PT( pDest, ((Scalar*)pSrc)->Get() );

        case SASVC_OBJECT:
            return Cast_Internal_PT_PT( pDest, ((Object*)pSrc)->Get() );
    }

	return E_INVALIDARG;
}


//--------------------------------------------------------------------------------------
HRESULT Value::Cast_Internal_PT_PT( DataType*& pDest, const DataType* pSrc )
{   
    HRESULT hr;

	if( NULL == pSrc )
	{
		// If the source value is null, deallocate the current value (if any)
		SAFE_DELETE(pDest);
	}
	else
	{
		// Else, either clone or cast
		if( NULL == pDest )
		{
			// No current basic type set. Simply clone a new allocation
			pDest = pSrc->Clone();
			if( NULL == pDest )
				return E_OUTOFMEMORY;
		}
		else
		{
			// Attempt to cast into the current basic type
			hr = pDest->Cast( *pSrc );
			if( FAILED(hr) )
				return hr;
		}
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT Value::Cast_Internal_V_V( Value*& pDest, const Value* pSrc )
{   
    HRESULT hr;

	if( NULL == pSrc )
	{
		// If the source value is null, deallocate the current value (if any)
		SAFE_DELETE(pDest);
	}
	else
	{
		// Else, either clone or cast
		if( NULL == pDest )
		{
			// No current basic type set. Simply clone a new allocation
			pDest = pSrc->Clone();
			if( NULL == pDest )
				return E_OUTOFMEMORY;
		}
		else
		{
			// Attempt to cast into the current basic type
			hr = pDest->Cast( *pSrc );
			if( FAILED(hr) )
				return hr;
		}
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT Value::Cast_Internal_V_PT( Value*& pDest, const DataType* pSrc )
{   
    if( pSrc == NULL )
        return Cast_Internal_V_V( pDest, NULL );

    Scalar scalar( *pSrc );
    Object object( *pSrc );
                    
    switch( pSrc->Type() )
    {
        case SASVT_INVALID:
            return E_INVALIDARG;

        case SASVT_BOOL:
        case SASVT_INT:
        case SASVT_FLOAT:
            return Cast_Internal_V_V( pDest, &scalar );

        default:
            return Cast_Internal_V_V( pDest, &object );
    }
}


//--------------------------------------------------------------------------------------
HRESULT Value::Cast( const DataType& DataType )
{
    switch( DataType.Type() )
    {
    case SASVT_BOOL:
    case SASVT_INT:
    case SASVT_FLOAT:
        return Cast( Scalar(DataType) );

    default:
        return Cast( Object(DataType) );
    }
};


//--------------------------------------------------------------------------------------
HRESULT Object::Cast( const Value& value )
{
	switch( value.Class() )
	{
		case SASVC_OBJECT: 
        {
            return Cast_Internal_PT_PT( m_pDataType, ((Object&)value).Get() );
        }
	}
	
	// Can not cast from specified type
	return E_FAIL;
};


//--------------------------------------------------------------------------------------
HRESULT Scalar::Cast( const Value& value )
{
	switch( value.Class() )
	{
		case SASVC_SCALAR: return Cast_Internal_PT_PT( m_pDataType, ((Scalar&)value).Get() );
		case SASVC_VECTOR: return Cast_Internal_PT_PT( m_pDataType, ((Vector&)value).Get(0) );
		case SASVC_MATRIX: return Cast_Internal_PT_PT( m_pDataType, ((Matrix&)value).Get(0,0) );
		case SASVC_ARRAY: 
		{
			Value* innerValue = ((Array&)value).Get(0);
			return Cast( *innerValue );
		}
        case SASVC_STRUCT:
		{
			// Find the first numeric member
			Struct& s = (Struct&)value;
			for( UINT i=0; i < s.Length(); i++ )
			{
				Value* pValue = s.Get(i);
				if( NULL == pValue )
					continue;

				if( pValue->IsNumeric() )
				{
					// Found one. Recurse to complete the cast
					return Cast( *pValue );
				}
			}

			// Not found
			return E_FAIL;
		}
	}
	
	// Can not cast from specified type
	return E_FAIL;
};


//--------------------------------------------------------------------------------------
Value* Vector::Clone() const
{
    HRESULT hr;

    Vector* pNewVector = new Vector( Type(), Length() );
    if( NULL == pNewVector )
        return NULL;

    for( UINT i=0; i < Length(); i++ )
    {
        hr = pNewVector->Set( i, Get(i) );
        if( FAILED(hr) )
        {
            // TODO - Error string
            SAFE_DELETE(pNewVector);
            return NULL;
        }
    }

    return pNewVector;
}


//--------------------------------------------------------------------------------------
HRESULT Vector::Cast( const Value& value )
{
    HRESULT hr;

	switch( value.Class() )
	{
		case SASVC_SCALAR: 
        {
            Scalar& scalar = (Scalar&)value;

            // Duplicate to all components
            for( UINT i=0; i < m_Length; i++ )
            {
                hr = Cast_Internal_PT_PT( m_pDataTypes[i], scalar.Get() );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }

		case SASVC_VECTOR: 
        {
            Vector& vector = (Vector&)value;

            // Invalid if the destination contains more elements than the source
            if( Length() > vector.Length() )
                return E_FAIL;

            // Duplicate left-most components
            for( UINT i=0; i < m_Length; i++ )
            {
                hr = Cast_Internal_PT_PT( m_pDataTypes[i], vector.Get(i) );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }

        case SASVC_ARRAY:
        {
            Array& array = (Array&)value;

            // Invalid if the destination contains more elements than the source
            if( Length() > array.Length() )
                return E_FAIL;

            // Duplicate left-most components
            for( UINT i=0; i < m_Length; i++ )
            {
                hr = Cast_Internal_PT_V( m_pDataTypes[i], array.Get(i) );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }

        case SASVC_MATRIX:
        {
            Matrix& matrix = (Matrix&)value;

            // Invalid if the matrix contains a different number of elements than the vector
            if( Length() != matrix.Rows() * matrix.Columns() )
                return E_FAIL;

            // Duplicate components
            UINT i=0;
            for( UINT iRow=0; iRow < matrix.Rows(); iRow++ )
            {
                for( UINT iColumn=0; iColumn < matrix.Columns(); iColumn++ )
                {
                    hr = Cast_Internal_PT_PT( m_pDataTypes[i++], matrix.Get(iRow, iColumn) );
                    if( FAILED(hr) )
                        return hr;
                }
            }

            return S_OK;
        }

        case SASVC_STRUCT:
		{
			// Find the first numeric member
			Struct& strct = (Struct&)value;

            // Struct must be at least the size of the desintation vector
            if( strct.Length() < Length() )
                return E_FAIL;

            // All elements up to the size of the vector must be numeric
			for( UINT i=0; i < strct.Length(); i++ )
			{
				Value* pValue = strct.Get(i);
				if( NULL == pValue )
					return E_FAIL;

				if( !pValue->IsNumeric() )
                    return E_FAIL;

                Scalar tempScalar( pValue->Type() );
				hr = tempScalar.Cast( *pValue );
                if( FAILED(hr) )
                    return hr;

                hr = Cast_Internal_PT_PT( m_pDataTypes[i], tempScalar.Get() );
                if( FAILED(hr) )
                    return hr;
			}

			return S_OK;
		}
	}
	
	// Can not cast from specified type
	return E_FAIL;
};


//--------------------------------------------------------------------------------------
Value* Matrix::Clone() const
{
    HRESULT hr;

    Matrix* pNew = new Matrix( Type(), Rows(), Columns() );
    if( NULL == pNew )
    {
        // TODO - Error string
        return NULL;
    }

    for( UINT r=0; r < Rows(); r++ )
    {
        for( UINT c=0; c < Columns(); c++ )
        {
            hr = pNew->Set( r, c, Get(r, c) );
            if( FAILED(hr) )
            {
                // TODO - Error string
                SAFE_DELETE(pNew);
                return NULL;
            }
        }
    }

    return pNew;
}


//--------------------------------------------------------------------------------------
HRESULT Matrix::Cast( const Value& value )
{
    HRESULT hr;

	switch( value.Class() )
	{
		case SASVC_SCALAR: 
        {
            Scalar& scalar = (Scalar&)value;

            // Duplicate to all components
            for( UINT r=0; r < Rows(); r++ )
            {
                for( UINT c=0; c < Columns(); c++ )
                {
                    hr = Cast_Internal_PT_PT( m_pDataTypes[r][c], scalar.Get() );
                    if( FAILED(hr) )
                        return hr;
                }
            }

            return S_OK;
        }

		case SASVC_VECTOR: 
        {
            Vector& vector = (Vector&)value;

            // Invalid if the destination contains a different number of elements than the source
            if( Rows() * Columns() > vector.Length() )
                return E_FAIL;

            // Duplicate left-most components
            int index=0;
            for( UINT r=0; r < Rows(); r++ )
            {
                for( UINT c=0; c < Columns(); c++ )
                {
                    hr = Cast_Internal_PT_PT( m_pDataTypes[r][c], vector.Get(index) );
                    if( FAILED(hr) )
                        return hr;

                    index++;
                }
            }

            return S_OK;
        }

        case SASVC_MATRIX:
        {
            Matrix& matrix = (Matrix&)value;

            // Invalid if the destination matrix contains more elements than the source
            // along either dimension
            if( Rows() > matrix.Rows() ||
                Columns() > matrix.Columns() )
                return E_FAIL;

            // Duplicate components
            for( UINT r=0; r < Rows(); r++ )
            {
                for( UINT c=0; c < Columns(); c++ )
                {
                    hr = Cast_Internal_PT_PT( m_pDataTypes[r][c], matrix.Get(r, c) );
                    if( FAILED(hr) )
                        return hr;
                }
            }

            return S_OK;
        }

        case SASVC_STRUCT:
		{
			Struct& strct = (Struct&)value;

            // Struct must be at least the size of the desintation 
            if( strct.Length() < Rows() * Columns() )
                return E_FAIL;

            // All elements up to the size of the matrix must be numeric
            int index=0;
            for( UINT r=0; r < Rows(); r++ )
            {
                for( UINT c=0; c < Columns(); c++ )
                {
				    Value* pValue = strct.Get(index);
				    if( NULL == pValue )
					    return E_FAIL;

				    if( !pValue->IsNumeric() )
                        return E_FAIL;

                    hr = Cast_Internal_PT_V( m_pDataTypes[r][c], pValue );
                    if( FAILED(hr) )
                        return hr;

                    index++;
                }
			}

			return S_OK;
		}
	}
	
	// Can not cast from specified type
	return E_FAIL;
};


//--------------------------------------------------------------------------------------
Value* Array::Clone() const
{
    HRESULT hr;

    Array* pNewArray = new Array( Type(), Length() );
    if( NULL == pNewArray )
    {
        // TODO - Error string
        return NULL;
    }

    for( UINT i=0; i < Length(); i++ )
    {
        hr = pNewArray->Set( i, Get(i) );
        if( FAILED(hr) )
        {
            // TODO - Error string
            SAFE_DELETE(pNewArray);
            return NULL;
        }
    }

    return pNewArray;
}


//--------------------------------------------------------------------------------------
HRESULT Array::Cast( const Value& value )
{
    HRESULT hr;

	switch( value.Class() )
	{
		case SASVC_SCALAR: 
        {
            Scalar& scalar = (Scalar&)value;

            // Duplicate to all components
            for( UINT i=0; i < Length(); i++ )
            {
                hr = Cast_Internal_V_PT( m_Values[i], scalar.Get() );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }

		case SASVC_VECTOR: 
        {
            Vector& vector = (Vector&)value;

            // Invalid if the destination contains more elements than the source
            if( Length() > vector.Length() )
                return E_FAIL;

            // Duplicate left-most components
            for( UINT i=0; i < Length(); i++ )
            {
                hr = Cast_Internal_V_PT( m_Values[i], vector.Get(i) );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }

        case SASVC_MATRIX:
        {
            Matrix& matrix = (Matrix&)value;

            // Invalid if the matrix contains a different number of elements than the vector
            if( Length() != matrix.Rows() * matrix.Columns() )
                return E_FAIL;

            // Duplicate components
            UINT index=0;
            for( UINT iRow=0; iRow < matrix.Rows(); iRow++ )
            {
                for( UINT iColumn=0; iColumn < matrix.Columns(); iColumn++ )
                {
                    hr = Cast_Internal_V_PT( m_Values[index], matrix.Get(iRow, iColumn) );
                    if( FAILED(hr) )
                        return hr;

                    index++;
                }
            }

            return S_OK;
        }

        case SASVC_STRUCT:
		{
			// Find the first numeric member
			Struct& strct = (Struct&)value;

            // Struct must be at least the size of the desintation vector
            if( strct.Length() < Length() )
                return E_FAIL;

            // All elements up to the size of the vector must be numeric
			for( UINT i=0; i < strct.Length(); i++ )
			{
				Value* pValue = strct.Get(i);
				if( NULL == pValue )
					return E_FAIL;

				if( !pValue->IsNumeric() )
                    return E_FAIL;

                Scalar tempScalar( pValue->Type() );
				hr = tempScalar.Cast( *pValue );
                if( FAILED(hr) )
                    return hr;

                hr = Cast_Internal_V_PT( m_Values[i], tempScalar.Get() );
                if( FAILED(hr) )
                    return hr;
			}

			return S_OK;
		}
	}
	
	// Can not cast from specified type
	return E_FAIL;
};


//--------------------------------------------------------------------------------------
HRESULT Struct::Cast( const Value& value )
{
    HRESULT hr;

    switch( value.Class() )
	{
		case SASVC_SCALAR: 
        {
            Scalar& scalar = (Scalar&)value;

            // Valid if and only if the destination structure contains only numeric
            // elements
            for( UINT i=0; i < Length(); i++ )
            {
                Value* pValue = Get(i);
                if( !pValue || !pValue->IsNumeric() )
                    return E_FAIL;
            }
            
            // Duplicate to all components
            for( UINT i=0; i < Length(); i++ )
            {   
                hr = Cast_Internal_V_V( m_Values[i], &scalar );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }

		case SASVC_VECTOR: 
        {
            Vector& vector = (Vector&)value;

            // Invalid if the destination contains more elements than the source
            if( Length() > vector.Length() )
                return E_FAIL;

            // Valid if and only if the destination structure contains only numeric
            // elements
            for( UINT i=0; i < Length(); i++ )
            {
                Value* pValue = Get(i);
                if( !pValue || !pValue->IsNumeric() )
                    return E_FAIL;
            }

            // Duplicate left-most components
            for( UINT i=0; i < Length(); i++ )
            {
                Scalar scalar( *vector.Get(i) );
                scalar.Set( vector.Get(i) );
                hr = Cast_Internal_V_V( m_Values[i], &scalar );
                if( FAILED(hr) )
                    return hr;
            }

            return S_OK;
        }

        case SASVC_MATRIX:
        {
            Matrix& matrix = (Matrix&)value;

            // Invalid if the matrix contains a different number of elements than the vector
            if( Length() != matrix.Rows() * matrix.Columns() )
                return E_FAIL;

            // Duplicate components
            UINT i=0;
            for( UINT iRow=0; iRow < matrix.Rows(); iRow++ )
            {
                for( UINT iColumn=0; iColumn < matrix.Columns(); iColumn++ )
                {
                    Scalar scalar( *matrix.Get(iRow, iColumn) );
                    hr = Cast_Internal_V_V( m_Values[i++], &scalar );
                    if( FAILED(hr) )
                        return hr;
                }
            }

            return S_OK;
        }

        case SASVC_STRUCT:
		{
			Struct& strct = (Struct&)value;

            // The destination structure must not be larger than the source structure
            if( Length() > strct.Length() )
                return E_FAIL;

            // Attempt a cast on the elements
			for( UINT i=0; i < Length(); i++ )
			{
				hr = Cast_Internal_V_V( m_Values[i], strct.Get(i) );
                if( FAILED(hr) )
                    return hr;
			}

			return S_OK;
		}
	}
	
	// Can not cast from specified type
	return E_FAIL;
}


//--------------------------------------------------------------------------------------
Value* Struct::Clone() const
{
    HRESULT hr;

    Struct* pNew = new Struct( Length() );
    if( NULL == pNew )
    {
        // TODO - Error string
        return NULL;
    }

    for( UINT i=0; i < Length(); i++ )
    {
        hr = pNew->Set( i, Get(i) );
        if( FAILED(hr) )
        {
            // TODO - Error string
            SAFE_DELETE(pNew);
            return NULL;
        }
    }

    return pNew;
}