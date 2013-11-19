//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef _PARAMETER_H_
#define _PARAMETER_H_
#include "stdafx.h"
#include "Control.h"
#include "Unit.h"
#include "BoundValueTable.h"
#include <string>
#include <vector>
//#include <math.h>
using namespace std;

namespace Sas
{
	class Effect;
	class Variable;
	class Parameter;
	class DataType;
	class Value;
	class Scalar;
	class Vector;
	class Matrix;
	class Array;
	class Object;
	class Struct;
	class IBoundValueTableListener;

	//class Annotation;
    //class ParameterElement;
	enum SASVARIABLE_CLASS;
	enum SASVARIABLE_TYPE;


	struct SASVARIABLE_DESC 
	{
		wstring Name;
		wstring Semantic;
		SASVARIABLE_CLASS Class;
		SASVARIABLE_TYPE Type;
		UINT Rows;
		UINT Columns;
		UINT Elements;
		UINT Annotations;
		UINT StructMembers;
		bool IsAnnotation;
		bool IsShared;

		bool IsArray() const { return Elements > 0; }
		bool IsStruct() const { return StructMembers > 0; }
	};

    struct Version
    {
        int Major;
        int Minor;
        int Revision;
    };

    struct GlobalVariableData
    {
        Version SasVersion;
        wstring Description;
        wstring Help;
        wstring Category;
        wstring Author;
        wstring Company;
        wstring Revision;
        wstring AuthoringSoftware;
    };

    enum VariableScope
    {
        VariableScope_Global = 1,
        VariableScope_NonGlobal = 2,
        VariableScope_Any = VariableScope_Global | VariableScope_NonGlobal
    };

    struct AnnotationTemplate
    {
        AnnotationTemplate()
        {
            ZeroMemory(this, sizeof(AnnotationTemplate) );
        }

        const WCHAR* Name;
        SASVARIABLE_CLASS Class;
        SASVARIABLE_TYPE Type;
        UINT Rows;
        UINT Columns;
        UINT Elements;

        VariableScope Scope;
        Value* pDefaultValue;
        bool IsRequired;
    };

    
   
    //-----------------------------------------------------------------------------
	class Variable
	{
	public:
		Variable(Effect* effect, Variable* parent = NULL, UINT index = 0 );

        virtual ~Variable();

		const SASVARIABLE_DESC& Desc() { return m_Desc; };

		Variable* GetAnnotation(UINT i) { return (i >= m_Annotations.size() ? NULL : m_Annotations[i]); }
		Variable* GetMember(UINT i) { return (i >= m_Members.size() ? NULL : m_Members[i]); }
		Variable* GetElement(UINT i) { return (i >= m_Elements.size() ? NULL : m_Elements[i]); }
		Variable* GetParent() { return m_Parent; }
		UINT GetIndex() { return m_Index; }
			

        Value* GetValue();
        HRESULT GetValueAndCast( Value& value );
        HRESULT SetValue( const Value& value );
        HRESULT SetValue( const DataType& DataType );


		Parameter* GetParameter();

	protected:
        Effect* m_Effect;
		Variable* m_Parent;
		UINT m_Index;


		Parameter* m_Parameter;

		SASVARIABLE_DESC m_Desc;
        Value* m_pValue; // Cached copy of the D3DXEffect parameter value

		vector<Variable*> m_Annotations;
        vector<Variable*> m_Members;
        vector<Variable*> m_Elements;


    protected:
        HRESULT CreateValue( Value** ppValueOut ) const;

        HRESULT GetValue_Internal( Value* pValue ) const;
        HRESULT GetStruct_Internal( Struct* pStruct ) const;
        HRESULT GetArray_Internal( Array* pArray ) const;

        HRESULT SetValue_Internal( Value* pValue );
        HRESULT SetStruct_Internal( Struct* pStruct );
        HRESULT SetArray_Internal( Array* pArray );


        virtual HRESULT GetScalar_Internal( Scalar* pScalar ) const = 0;
        virtual HRESULT GetVector_Internal( Vector* pVector ) const = 0;
        virtual HRESULT GetMatrix_Internal( Matrix* pMatrix ) const = 0;
        virtual HRESULT GetObject_Internal( Object* pObject ) const = 0;

        virtual HRESULT SetScalar_Internal( Scalar* pScalar ) = 0;
        virtual HRESULT SetVector_Internal( Vector* pVector ) = 0;
        virtual HRESULT SetMatrix_Internal( Matrix* pMatrix ) = 0;
        virtual HRESULT SetObject_Internal( Object* pObject ) = 0;

		//this defines special casts only avaiable on effect variables.
		//such as String-To-Texture
		virtual HRESULT SpecializedCasts( Value* pCached, const Value& value ) = 0;
	};

	
	//-----------------------------------------------------------------------------
	class Parameter : public IBoundValueTableListener
	{
    public:
        struct UI
        {
            Control Control;
            bool Visible;
            wstring Label;
            wstring Description;
            float Min;
            float Max;
            int Steps;
            float StepsPower;
            float Stride;
            vector<wstring> Enum;
        };

	public:
        Parameter( Variable* parent );
        virtual ~Parameter();

		Variable* GetVariable(){ return m_Variable; }

        //ParameterElement* GetElement(UINT i) const { return m_Elements[i]; }

        HRESULT Bind( BoundValueTable* pTable );

        static void ReleaseStaticResources();
        
        bool IsGlobalParameter() const { return m_isGlobalParameter; }

        const WCHAR* GetResourceAddress() const { return m_ResourceAddress.c_str(); }
        const WCHAR* GetBindAddress() const { return m_BindAddress.c_str(); }
        const BindAddressHandle GetBindAddressHandle() const { return m_BindAddressHandle; }

        virtual void OnValueChanged( BindAddressHandle handle, const Value* pNewValue );

	private:
        static void CreateAnnotationTemplates();
        static AnnotationTemplate CreateTemplate( const WCHAR* name, const WCHAR* declaration, VariableScope scope, Value* pDefaultValue, bool isRequired=false );
        static AnnotationTemplate CreateTemplate( const WCHAR* name, const WCHAR* declaration, VariableScope scope, DataType& defaultValue, bool isRequired=false );
    
        void ReadSasAnnotations();
        HRESULT ResolveSasAnnotation( const WCHAR* name, Value& valueOut );
        

    protected:   
		Variable* m_Variable;

        Unit m_Unit;
        bool m_Normalize;
        UI m_UI;

        bool m_isGlobalParameter;
        GlobalVariableData* m_GlobalData;

        wstring m_ResourceAddress;
        wstring m_BindAddress;
        BindAddressHandle m_BindAddressHandle;
        BoundValueTable* m_pTable;

      
        typedef map<const WCHAR*, AnnotationTemplate, StringCompareInsensitive> AnnotationTemplateMap;
	    static AnnotationTemplateMap s_AnnotationTemplates;
	};
}

#endif //_PARAMETER_H_