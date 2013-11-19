//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Parameter.h"
#include "Effect.h"
#include "Host.h"

using namespace Sas;


map<const WCHAR*, AnnotationTemplate, StringCompareInsensitive> Parameter::s_AnnotationTemplates;

//--------------------------------------------------------------------------------------
Variable::~Variable()
{
	vector<Variable*>::iterator itr;


	for(itr = m_Elements.begin(); itr != m_Elements.end(); itr++)
		LOOPSAFE_DELETE( (*itr), Variable );
	m_Elements.clear();

	for(itr = m_Members.begin(); itr != m_Members.end(); itr++)
		LOOPSAFE_DELETE( (*itr), Variable );
	m_Members.clear();

	for(itr = m_Annotations.begin(); itr != m_Annotations.end(); itr++)
		LOOPSAFE_DELETE( (*itr), Variable );
	m_Annotations.clear();

	LOOPSAFE_DELETE( m_Parameter, Parameter);


    SAFE_DELETE( m_pValue );
}


//--------------------------------------------------------------------------------------
Value* Variable::GetValue()
{
    HRESULT hr;

    // If the value doesn't yet exist, create it
    if( NULL == m_pValue )
    {
        hr = CreateValue( &m_pValue );
        if( FAILED(hr) )
        {
            //TODO: Probably out of memory
            return NULL;
        }
    }
    
    hr = GetValue_Internal( m_pValue );
    if( FAILED(hr) )
    {
        //TODO: Error string
        return NULL;
    }

    return m_pValue;
}


//--------------------------------------------------------------------------------------
HRESULT Variable::GetValueAndCast( Value& value )
{
	HRESULT hr = S_OK;
    Value* pCached = GetValue();
    if( NULL == pCached )
        return E_FAIL;

    hr = value.Cast( *pCached );
    if( FAILED(hr) )
		hr = SpecializedCasts( pCached, value );

	return hr;
}


//--------------------------------------------------------------------------------------
HRESULT Variable::SetValue( const Value& value )
{
    HRESULT hr;

    // Get the value
    Value* pCached = GetValue();
    if( NULL == pCached )
        return E_FAIL;

    // Cast the passed value into the cached value
    hr = pCached->Cast( value );
    if( FAILED(hr) )
	{
		hr = SpecializedCasts( pCached, value );
		if( FAILED(hr) )
			return hr;
	}

    // Attempt to push the cached value back into the effect
    return SetValue_Internal( pCached );
};


//--------------------------------------------------------------------------------------
HRESULT Variable::SetValue( const DataType& DataType )
{
    switch( DataType.Type() )
    {
        case SASVT_BOOL:
        case SASVT_INT:
        case SASVT_FLOAT:
            return SetValue( Scalar( DataType ) );

        default:
            return SetValue( Object( DataType ) );
    }  
};


//--------------------------------------------------------------------------------------
HRESULT Variable::CreateValue( Value** ppValueOut ) const
{
    if( NULL == ppValueOut )
        return E_INVALIDARG;

    // Determine which type of value to create to contain this value
    if( m_Desc.Elements > 0 )
    {
        *ppValueOut = new Array( m_Desc.Type, m_Desc.Elements );
        if( NULL == *ppValueOut )
            return E_OUTOFMEMORY;

        return S_OK;
    }

    switch( m_Desc.Class )
    {
        case SASVC_SCALAR:
        {
            *ppValueOut = new Scalar( m_Desc.Type );
            if( NULL == *ppValueOut )
                return E_OUTOFMEMORY;

            return S_OK;
        }
        case SASVC_VECTOR:
        {
            *ppValueOut = new Vector( m_Desc.Type, m_Desc.Columns );
            if( NULL == *ppValueOut )
                return E_OUTOFMEMORY;

            return S_OK;
        }

        case SASVC_MATRIX:
        {
            *ppValueOut = new Matrix( m_Desc.Type, m_Desc.Rows, m_Desc.Columns );
            if( NULL == *ppValueOut )
                return E_OUTOFMEMORY;

            return S_OK;
        }

        case SASVC_OBJECT:
        {
            *ppValueOut = new Object( m_Desc.Type );
            if( NULL == *ppValueOut )
                return E_OUTOFMEMORY;

            return S_OK;
        }

        case SASVC_STRUCT:
        {
            *ppValueOut = new Struct( m_Desc.StructMembers );
            if( NULL == *ppValueOut )
                return E_OUTOFMEMORY;

            return S_OK;
        }
    }

    return E_FAIL;
}


//--------------------------------------------------------------------------------------
HRESULT Variable::GetValue_Internal( Value* pValue ) const
{
    if( NULL == pValue )
        return E_INVALIDARG;

    switch( pValue->Class() )
    {
        case SASVC_OBJECT:
            return GetObject_Internal( (Object*)pValue );

        case SASVC_SCALAR:
            return GetScalar_Internal( (Scalar*)pValue );

        case SASVC_VECTOR:
            return GetVector_Internal( (Vector*)pValue );

        case SASVC_ARRAY:
            return GetArray_Internal( (Array*)pValue );

        case SASVC_MATRIX:
            return GetMatrix_Internal( (Matrix*)pValue );

        case SASVC_STRUCT:
            return GetStruct_Internal( (Struct*)pValue );
    }

    return E_FAIL;
}





//--------------------------------------------------------------------------------------
HRESULT Variable::SetValue_Internal( Value* pValue )
{
    if( NULL == pValue )
        return E_INVALIDARG;

    switch( pValue->Class() )
    {
        case SASVC_OBJECT:
            return SetObject_Internal( (Object*)pValue );

        case SASVC_SCALAR:
            return SetScalar_Internal( (Scalar*)pValue );

        case SASVC_VECTOR:
            return SetVector_Internal( (Vector*)pValue );

        case SASVC_ARRAY:
            return SetArray_Internal( (Array*)pValue );

        case SASVC_MATRIX:
            return SetMatrix_Internal( (Matrix*)pValue );

        case SASVC_STRUCT:
            return SetStruct_Internal( (Struct*)pValue );
    }

    return E_FAIL;
}




//--------------------------------------------------------------------------------------
HRESULT Variable::SetArray_Internal( Array* pArray )
{
    HRESULT hr;

    for( UINT i=0; i < m_Elements.size(); i++ )
    {
		Variable* element = m_Elements[i];

		Value* pValue = pArray->Get(i);
        if( pValue == NULL )
            return E_FAIL;

        hr = element->SetValue( *pValue );
        if ( FAILED(hr) )
            return hr;
    }

    return S_OK;    
}


//--------------------------------------------------------------------------------------
HRESULT Variable::SetStruct_Internal( Struct* pStruct )
{
    HRESULT hr;

    for( UINT i=0; i < m_Members.size(); i++ )
    {
		Variable* member= m_Members[i];

        Value* pValue = pStruct->Get(i);
        if( NULL == pValue )
            return E_FAIL;

        hr = member->SetValue( *pValue );
        if ( FAILED(hr) )
            return hr;
    }

    return S_OK;    
}

//--------------------------------------------------------------------------------------
HRESULT Variable::GetArray_Internal( Array* pArray ) const
{
    HRESULT hr;

    for( UINT i=0; i < m_Elements.size(); i++ )
    {
		Variable* element = m_Elements[i];

        hr = pArray->Set( i, element->GetValue() );
        if ( FAILED(hr) )
            return hr;
    }

    return S_OK;    
}


//--------------------------------------------------------------------------------------
HRESULT Variable::GetStruct_Internal( Struct* pStruct ) const
{
    HRESULT hr;

    for( UINT i=0; i < m_Members.size(); i++ )
    {
		Variable* member = m_Members[i];

 
        hr = pStruct->Set( i, member->GetValue() );
        if ( FAILED(hr) )
            return hr;
    }

    return S_OK;    
}

Variable::Variable(Effect* effect, Variable* parent, UINT index ) 
{ 
	m_Parent = parent;
	m_Index = index;
	m_Effect = effect; 
	m_pValue = NULL;

	m_Parameter = NULL;
	//GetParameter();
}
//--------------------------------------------------------------------------------------
Parameter* Variable::GetParameter()
{
	if( m_Desc.IsAnnotation == false )
	{
		if(m_Parameter == NULL)
		{//create on first use.
			m_Parameter = new Parameter(this);
		}
	}
	return m_Parameter;
};
//--------------------------------------------------------------------------------------
Parameter::Parameter( Variable* variable )
{
	m_Variable = variable;
    m_BindAddressHandle = -1;
    m_pTable = NULL;
    m_isGlobalParameter = false;
	m_GlobalData = NULL;
	
	// If the static list of annotation templates hasn't been created yet, create it here
	if( 0 == s_AnnotationTemplates.size() )
		CreateAnnotationTemplates();

	if(m_Variable->GetParent() == NULL)
	{
		if( 0 == StrCmpIW( L"SasGlobal", m_Variable->Desc().Semantic.c_str() ) )
			m_isGlobalParameter = true;

		// Determine the value of all the SAS annotations
		ReadSasAnnotations();
	}
	else if(m_Variable->GetParent()->Desc().IsStruct())
	{
		m_BindAddress = wstring( m_Variable->GetParent()->GetParameter()->GetBindAddress() ) + m_Variable->Desc().Name;
		m_BindAddressHandle = BindAddressHelper::StringToHandle( m_BindAddress.c_str() );
	}
	else if(m_Variable->GetParent()->Desc().IsArray())
	{
		// Replace all occurances of [*] with [<index>]
		wstring address = m_Variable->GetParent()->GetParameter()->GetBindAddress();
        

		wstring::size_type index = (UINT)address.find( L"[*]");
		if( string::npos != index )
		{
			WCHAR buffer[32] = {0};
			StringCchPrintf(buffer, 10, L"%d", m_Variable->GetIndex());

			address.replace( index+1, 1, buffer );

			m_BindAddress = address;
			m_BindAddressHandle = BindAddressHelper::StringToHandle( m_BindAddress.c_str() );
		}
	}
}


//--------------------------------------------------------------------------------------
Parameter::~Parameter()
{
    Bind( NULL );
	SAFE_DELETE(m_GlobalData);
}


//--------------------------------------------------------------------------------------
void Parameter::ReleaseStaticResources()
{
    AnnotationTemplateMap::iterator it;
    for( it=s_AnnotationTemplates.begin(); it != s_AnnotationTemplates.end(); it++ )
    {
        SAFE_DELETE( (*it).second.pDefaultValue );
    }

    s_AnnotationTemplates.clear();
}


//--------------------------------------------------------------------------------------
HRESULT Parameter::Bind( BoundValueTable* pTable )
{
	if(pTable == m_pTable)
		return S_OK;


    HRESULT hr = S_OK;
    HRESULT hrReturn = S_OK;

    BindAddressInfo info;
    BindAddressHelper::GetInfo( GetBindAddressHandle(), &info );
    if( BindAddress_Singular == info.Type )
    {
        // If an old binding exists, remove the listeners
        if( m_pTable )
            m_pTable->RemoveListener( info.Handle, this );

        // If establishing a new binding, set up listeners
        if( pTable )
        {
            pTable->AddListener( info.Handle, this );

            // Set the parameter to the current value of the binding
            const Value* pValue = pTable->GetValue( GetBindAddressHandle(), true );
            if( pValue )
                m_Variable->SetValue( *pValue );
        }
    }
    else if( BindAddress_Multiple == info.Type )
    {
        for( UINT i=0; i < m_Variable->Desc().Elements; i++ )
        {
            Variable* pElement = m_Variable->GetElement(i);
			if(pElement)
			{
				hr = pElement->GetParameter()->Bind( pTable );
				if( FAILED(hr) )
					hrReturn = hr;
			}
        }
    }

    for( UINT i=0; i < m_Variable->Desc().StructMembers; i++ )
    {
        Variable* pMember = m_Variable->GetMember(i);
		if(pMember)
		{
			hr = pMember->GetParameter()->Bind( pTable );
			if( FAILED(hr) )
				hrReturn = hr;
		}
    }


    m_pTable = pTable;
    return hrReturn;
}


//--------------------------------------------------------------------------------------
AnnotationTemplate Parameter::CreateTemplate( const WCHAR* name, const WCHAR* declaration, VariableScope scope, Value* pDefaultValue, bool isRequired )
{
    AnnotationTemplate newTemplate;
    ZeroMemory( &newTemplate, sizeof(newTemplate) );

    newTemplate.Name = name;
    newTemplate.Scope = scope;
    newTemplate.IsRequired = isRequired;

    if( pDefaultValue )
        newTemplate.pDefaultValue = pDefaultValue->Clone();

    
    if( 0 == _wcsicmp( L"bool", declaration ) )
    {
        newTemplate.Class = SASVC_SCALAR;
        newTemplate.Type = SASVT_BOOL;
        newTemplate.Rows = 1;
        newTemplate.Columns = 1;
        newTemplate.Elements = 0;
    }
    else if( 0 == _wcsicmp( L"float", declaration ) )
    {
        newTemplate.Class = SASVC_SCALAR;
        newTemplate.Type = SASVT_FLOAT;
        newTemplate.Rows = 1;
        newTemplate.Columns = 1;
        newTemplate.Elements = 0;
    }
    else if( 0 == _wcsicmp( L"int", declaration ) )
    {
        newTemplate.Class = SASVC_SCALAR;
        newTemplate.Type = SASVT_INT;
        newTemplate.Rows = 1;
        newTemplate.Columns = 1;
        newTemplate.Elements = 0;
    }
    else if( 0 == _wcsicmp( L"int3", declaration ) )
    {
        newTemplate.Class = SASVC_VECTOR;
        newTemplate.Type = SASVT_INT;
        newTemplate.Rows = 1;
        newTemplate.Columns = 3;
        newTemplate.Elements = 0;
    }
    else if( 0 == _wcsicmp( L"string", declaration ) )
    {
        newTemplate.Class = SASVC_OBJECT;
        newTemplate.Type = SASVT_STRING;
        newTemplate.Rows = 0;
        newTemplate.Columns = 0;
        newTemplate.Elements = 0;
    }
    else
    {
        //TODO: Error, a standard annotation type was not found
    }

    return newTemplate;
}

//--------------------------------------------------------------------------------------
AnnotationTemplate Parameter::CreateTemplate( const WCHAR* name, const WCHAR* declaration, VariableScope scope, DataType& defaultValue, bool isRequired )
{ 
    switch( defaultValue.Type() )
    {
    case SASVT_BOOL:
    case SASVT_INT:
    case SASVT_FLOAT:
        return CreateTemplate( name, declaration, scope, &Scalar(defaultValue), isRequired );

    default:
        return CreateTemplate( name, declaration, scope, &Object(defaultValue), isRequired );
    }
}


//--------------------------------------------------------------------------------------
void Parameter::CreateAnnotationTemplates()
{
    // Required annotations
    s_AnnotationTemplates[ L"SasVersion" ] = CreateTemplate( L"SasVersion", L"int3", VariableScope_Global, NULL, true );
    
    // GlobalParameter annotations
    s_AnnotationTemplates[ L"SasEffectDescription" ] = CreateTemplate( L"SasEffectDescription", L"string", VariableScope_Global, String("") );  
    s_AnnotationTemplates[ L"SasEffectHelp" ] = CreateTemplate( L"SasEffectHelp", L"string", VariableScope_Global, String("") );
    s_AnnotationTemplates[ L"SasEffectCategory" ] = CreateTemplate( L"SasEffectCategory", L"string", VariableScope_Global, String("") );
    s_AnnotationTemplates[ L"SasEffectAuthor" ] = CreateTemplate( L"SasEffectAuthor", L"string", VariableScope_Global, String("") );
    s_AnnotationTemplates[ L"SasEffectCompany" ] = CreateTemplate( L"SasEffectCompany", L"string", VariableScope_Global, String("") );
    s_AnnotationTemplates[ L"SasEffectRevision" ] = CreateTemplate( L"SasEffectRevision", L"string", VariableScope_Global, String("") );
    s_AnnotationTemplates[ L"SasEffectAuthoringSoftware" ] = CreateTemplate( L"SasEffectAuthoringSoftware", L"string", VariableScope_Global, String("") );
    
    // Parameter annotations
    s_AnnotationTemplates[ L"SasBindAddress" ] = CreateTemplate( L"SasBindAddress", L"string", VariableScope_Any, String("") );
    s_AnnotationTemplates[ L"SasUnits" ] = CreateTemplate( L"SasUnits", L"string", VariableScope_Any, String("none") );
    s_AnnotationTemplates[ L"SasNormalize" ] = CreateTemplate( L"SasNormalize", L"bool", VariableScope_Any, Bool(false) );
    s_AnnotationTemplates[ L"SasResourceAddress" ] = CreateTemplate( L"SasResourceAddress", L"string", VariableScope_Any, String("") );

    // User interface
    s_AnnotationTemplates[ L"SasUiLabel" ] = CreateTemplate( L"SasUiLabel", L"string", VariableScope_Any, String("") );
    s_AnnotationTemplates[ L"SasUiDescription" ] = CreateTemplate( L"SasUiDescription", L"string", VariableScope_Any, String("") );
    s_AnnotationTemplates[ L"SasUiVisible" ] = CreateTemplate( L"SasUiVisible", L"bool", VariableScope_Any, Bool(true) );
    s_AnnotationTemplates[ L"SasUiControl" ] = CreateTemplate( L"SasUiControl", L"string", VariableScope_Any, String("None") );
    s_AnnotationTemplates[ L"SasUiMax" ] = CreateTemplate( L"SasUiMax", L"float", VariableScope_Any, Float(FLT_MAX) );
    s_AnnotationTemplates[ L"SasUiMin" ] = CreateTemplate( L"SasUiMin", L"float", VariableScope_Any, Float(-FLT_MAX) );
    s_AnnotationTemplates[ L"SasUiSteps" ] = CreateTemplate( L"SasUiSteps", L"int", VariableScope_Any, Int(0) );
    s_AnnotationTemplates[ L"SasUiStepsPower" ] = CreateTemplate( L"SasUiStepsPower", L"float", VariableScope_Any, Float(1) );
    s_AnnotationTemplates[ L"SasUiStride" ] = CreateTemplate( L"SasUiStride", L"float", VariableScope_Any, Float(1) );
    s_AnnotationTemplates[ L"SasUiEnum" ] = CreateTemplate( L"SasUiEnum", L"string", VariableScope_Any, String("") );  
}


//--------------------------------------------------------------------------------------
void Parameter::ReadSasAnnotations()
{
    HRESULT hr;
    Object str(SASVT_STRING);

    if( this->IsGlobalParameter() )
    {
		m_GlobalData = new GlobalVariableData;

        Array version(SASVT_INT, 3);
        hr = ResolveSasAnnotation( L"SasVersion", version );
        if( SUCCEEDED(hr) )
        {
            m_GlobalData->SasVersion.Major = version.GetInt(0);
            m_GlobalData->SasVersion.Minor = version.GetInt(1);
            m_GlobalData->SasVersion.Revision = version.GetInt(2);
        }         

        hr = ResolveSasAnnotation( L"SasEffectDescription", str );
        if( SUCCEEDED(hr) )
            m_GlobalData->Description = str.GetString();
        
        hr = ResolveSasAnnotation( L"SasEffectHelp", str );
        if( SUCCEEDED(hr) )
            m_GlobalData->Help = str.GetString();

        hr = ResolveSasAnnotation( L"SasEffectCategory", str );
        if( SUCCEEDED(hr) )
            m_GlobalData->Category = str.GetString();

        hr = ResolveSasAnnotation( L"SasEffectAuthor", str );
        if( SUCCEEDED(hr) )
            m_GlobalData->Author = str.GetString();

        hr = ResolveSasAnnotation( L"SasEffectCompany", str );
        if( SUCCEEDED(hr) )
            m_GlobalData->Company = str.GetString();

        hr = ResolveSasAnnotation( L"SasEffectRevision", str );
        if( SUCCEEDED(hr) )
            m_GlobalData->Revision = str.GetString();
        
        hr = ResolveSasAnnotation( L"SasEffectAuthoringSoftware", str );
        if( SUCCEEDED(hr) )
            m_GlobalData->AuthoringSoftware = str.GetString();
    }
    
    hr = ResolveSasAnnotation( L"SasBindAddress", str );
    if( SUCCEEDED(hr) )
    {
        // Validate the bind address
        BindAddressInfo info;
        hr = BindAddressHelper::GetInfo( str.GetString(), &info );
        if( FAILED(hr) || BindAddress_Invalid == info.Type )
        {
            // TODO - error output "Invalid SasBindAddress on parameter ____. Defaulting to SasBindAddress = "" "
            m_BindAddress = L"";
            m_BindAddressHandle = -1;
        }
        else
        {
            m_BindAddress = str.GetString();
            m_BindAddressHandle = BindAddressHelper::StringToHandle( m_BindAddress.c_str() );
        }
    }

    hr = ResolveSasAnnotation( L"SasUnits", str );
    if( SUCCEEDED(hr) )
    {
        m_Unit = UnitHelper::AbbreviationToUnit( str.GetString() );
        if( Unit_Invalid == m_Unit )
        {
            // TODO -- error "Unrecognized unit ____ on parameter ____. Defaulting to SasUnits = "none"
            m_Unit = Unit_None;
        }
    }
    
    Scalar b(SASVT_BOOL);
    hr = ResolveSasAnnotation( L"SasNormalize", b );
    if( SUCCEEDED(hr) )
        m_Normalize = b.GetBool();
    
    hr = ResolveSasAnnotation( L"SasResourceAddress", str );
    if( SUCCEEDED(hr) )
	{
        m_ResourceAddress =  str.GetString();
		if(!m_ResourceAddress.empty())
			m_Variable->SetValue( Object( String( m_ResourceAddress.c_str() ) ) );
	}


    hr = ResolveSasAnnotation( L"SasUiLabel", str );
    if( SUCCEEDED(hr) )
        m_UI.Label = str.GetString();

    hr = ResolveSasAnnotation( L"SasUiDescription", str );
    if( SUCCEEDED(hr) )
        m_UI.Description = str.GetString();

    hr = ResolveSasAnnotation( L"SasUiVisible", b );
    if( SUCCEEDED(hr) )
        m_UI.Visible = b.GetBool();


    hr = ResolveSasAnnotation( L"SasUiControl", str );
    if( SUCCEEDED(hr) )
    {
        bool found = false;
        const WCHAR* controlName = str.GetString();
		if(controlName)
		{
			for( Control c = (Control)0; c < Control_END; c = (Control)((int)c+1) )
			{
				if( 0 == _wcsicmp( controlName, ControlStrings[ (int)c ] ) )
				{
					found = true;
					m_UI.Control = c;
					break;
				}
			}
		}

        if( !found )
        {
            Host::Out( LEVEL_ERROR, L"'%s' - Unknown control type '%s', Defaulting to SasControlType = \"none\"", this->GetVariable()->Desc().Name.c_str(), controlName );
            m_UI.Control = Control_None;
        }
    }
    
    Scalar f(SASVT_FLOAT);
    hr = ResolveSasAnnotation( L"SasUiMin", f );
    if( SUCCEEDED(hr) )
        m_UI.Min = f.GetFloat();

    hr = ResolveSasAnnotation( L"SasUiMax", f );
    if( SUCCEEDED(hr) )
        m_UI.Max = f.GetFloat();

    Scalar n(SASVT_INT);
    hr = ResolveSasAnnotation( L"SasUiSteps", n );
    if( SUCCEEDED(hr) )
        m_UI.Steps = n.GetInt();

    hr = ResolveSasAnnotation( L"SasUiStepsPower", f );
    if( SUCCEEDED(hr) )
        m_UI.StepsPower = f.GetFloat();

    hr = ResolveSasAnnotation( L"SasUiStride", f );
    if( SUCCEEDED(hr) )
        m_UI.Stride = f.GetFloat();

    /* TODO
    // SasUiEnum
    string uiEnum = (SasString)ResolveValue( "SasUiEnum" );
    m_ui.Enum = uiEnum.Split( new char[] {','} );
    for( int i=0; i < m_ui.Enum.Length; i++ )
    {
        m_ui.Enum[i] = m_ui.Enum[i].Trim();
    }
    */
}


//--------------------------------------------------------------------------------------
HRESULT Parameter::ResolveSasAnnotation( const WCHAR* Name, Value& valueOut )
{
    // Find the associated template
    AnnotationTemplateMap::iterator it = s_AnnotationTemplates.find( Name );
    if( s_AnnotationTemplates.end() == it )
    {
        Host::Out( LEVEL_INTERNALERROR, L"Annotation '%s' is either not part of the SAS standard or is missing its template", Name );
        return E_INVALIDARG;
    }

    // Found the template
    AnnotationTemplate& Template = (*it).second;

    // See if that particular annotation exists on this parameter
    Variable* pAnnotation = NULL;
    for( UINT i=0; i < m_Variable->Desc().Annotations; i++ )
    {
        if( 0 == _wcsicmp( Name, m_Variable->GetAnnotation(i)->Desc().Name.c_str() ) )
        {
            // Found the annotation
            pAnnotation =m_Variable->GetAnnotation(i);
            break;
        }
    }

    if( pAnnotation )
    {
        // Make sure the annotation appears at the correct scope and is correctly typed
        if( IsGlobalParameter() && !(VariableScope_Global & Template.Scope) )
        {
            Host::Out( LEVEL_ERROR, L"'%s' - The standard Annotation '%s' is not allowed on the global parameter", this->GetVariable()->Desc().Name.c_str(), Name );
            return E_FAIL;
        }
        
        if( !IsGlobalParameter() && !(VariableScope_NonGlobal & Template.Scope) )
        {
            Host::Out( LEVEL_ERROR, L"'%s' - The standard Annotation '%s' is allowed on the global parameter only", this->GetVariable()->Desc().Name.c_str(), Name );
            return E_FAIL;
        }

        if( Template.Class != pAnnotation->Desc().Class )
        {
            Host::Out( LEVEL_ERROR, L"'%s' - The Annotation '%s' was expected on a variable of a different class", this->GetVariable()->Desc().Name.c_str(), Name );
            return E_FAIL;
        }

        if( Template.Type != pAnnotation->Desc().Type )
        {
            Host::Out( LEVEL_ERROR, L"'%s' - The Annotation '%s' was expected on a variable of a different type", this->GetVariable()->Desc().Name.c_str(), Name );
            return E_FAIL;
        }

        if( Template.Rows != pAnnotation->Desc().Rows )
        {
            Host::Out( LEVEL_ERROR, L"'%s' - The Annotation '%s' was expected on a variable with %d rows", this->GetVariable()->Desc().Name.c_str(), Name, pAnnotation->Desc().Rows );
            return E_FAIL;
        }

        if( Template.Columns != pAnnotation->Desc().Columns )
        {
            Host::Out( LEVEL_ERROR, L"'%s' - The Annotation '%s' was expected on a variable with %d columns", this->GetVariable()->Desc().Name.c_str(), Name, pAnnotation->Desc().Columns );
            return E_FAIL;
        }

        if( Template.Elements != pAnnotation->Desc().Elements )
        {
            Host::Out( LEVEL_ERROR, L"'%s' - The Annotation '%s' was expected on a variable with %d elements", this->GetVariable()->Desc().Name.c_str(), Name, pAnnotation->Desc().Elements );
            return E_FAIL;
        }

        // Validated, return the value
        Value* pValue = pAnnotation->GetValue();
        if( NULL == pValue )
            return E_FAIL;

        return valueOut.Cast( *pValue );
    }


    // Annotation not found, make sure it wasn't required
    if( Template.IsRequired )
    {
        Host::Out( LEVEL_ERROR, L"'%s' - Missing required annotation '%s'", this->GetVariable()->Desc().Name.c_str(), Name );
        return E_FAIL;
    }

    //return the template-defined default
    if( NULL == Template.pDefaultValue )
    {
        Host::Out( LEVEL_INTERNALERROR, L"'%s' - The template for Sas annotation '%s' is missing a default value", this->GetVariable()->Desc().Name.c_str(), Name );
        return E_FAIL;
    }


    return valueOut.Cast( *(Template.pDefaultValue) );
}


//--------------------------------------------------------------------------------------
void Parameter::OnValueChanged( BindAddressHandle handle, const Value* pNewValue )
{
    HRESULT hr;

    if( NULL == pNewValue )
        return;

    hr = m_Variable->SetValue( *pNewValue );
    if( FAILED(hr) )
    {
        Host::Out( LEVEL_ERROR, L"Failed setting parameter '%s'", GetVariable()->Desc().Name.c_str() );
    }
}
