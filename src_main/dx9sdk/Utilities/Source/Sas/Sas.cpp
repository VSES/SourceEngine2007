//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Sas.h"
#pragma warning(disable: 4995)
#include <shlwapi.h>
#include <shlobj.h>
#pragma warning(default: 4995)
#include <malloc.h>

using namespace Sas;


//--------------------------------------------------------------------------------------
void Sas::ReleaseStaticResources()
{
    Host::GetRootTable()->Clear();
	BindAddressHelper::ReleaseStaticResources();
    Parameter::ReleaseStaticResources();
}

Sas::SourceInfo::SourceInfo()
{
    m_Type = SOURCEINFO_NONE;
    m_ResourceModule = NULL;
}

Sas::SourceInfo::SourceInfo(const WCHAR* fullPath)
{
    Set(fullPath);
}

Sas::SourceInfo::SourceInfo(const WCHAR* resource, const WCHAR* type, HMODULE hModule)
{
    Set(resource, type, hModule);
}

void Sas::SourceInfo::Set(const WCHAR* fullPath)
{
	try
	{
		HRESULT hr = S_OK;

		*this = SourceInfo();

		m_Type = SOURCEINFO_FILE;

		m_Name = fullPath;

		WCHAR dirPath[MAX_PATH];
		StringCchCopy( dirPath, MAX_PATH, fullPath );
		PathRemoveFileSpec( dirPath );
		m_Directory = dirPath;
	    
		m_ResourceModule = NULL;


		m_Description = L"File:";
		m_Description += m_Name;
	}
	catch( ... )
	{}
}

void Sas::SourceInfo::Set(const WCHAR* strResource, const WCHAR* strType, HMODULE hModule)
{
	try
	{
		HRESULT hr = S_OK;
		*this = SourceInfo();

		m_Type = SOURCEINFO_RESOURCE;

		m_ResourceModule = hModule;
		hr= StringFromResource(m_Name, strResource);
		if( FAILED(hr) )
		{
			Sas::Host::Out( LEVEL_ERROR, L"Error consuming resource name" );
			return;
		}

		hr= StringFromResource(m_ResourceType, strType);
		if( FAILED(hr) )
		{
			Sas::Host::Out( LEVEL_ERROR, L"Error consuming resource type" );
			return;
		}

		WCHAR dirPath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, dirPath);
		m_Directory = dirPath;

		m_Description = L"Resource:";
		m_Description += m_Name;
	}
	catch( ... )
	{}
}


Sas::SourceInfo::SourceInfo(const Sas::SourceInfo& sf)
{
    *this = sf;
}

Sas::SourceInfo& Sas::SourceInfo::operator=(const Sas::SourceInfo& sf)
{
	try
	{
		m_Type              =sf.m_Type;   
		m_Name              =sf.m_Name;       
		m_Directory         =sf.m_Directory;   
		m_ResourceType      =sf.m_ResourceType;
		m_ResourceModule    =sf.m_ResourceModule;
		m_Description       =sf.m_Description;       
	}
	catch( ... )
	{}

	return *this;
}



HRESULT Sas::SourceInfo::StringFromResource(std::wstring& str, const WCHAR* resNameOrType)
{
    HRESULT hr = S_OK;

	try
	{
		if( IS_INTRESOURCE(resNameOrType) )
		{
			WCHAR resStr[64] = L"";
			hr = StringCchPrintf(
								resStr, 
								64,
								L"#%d", 
								PtrToLong(resNameOrType) );
			str= resStr;
		}
		else
			str = resNameOrType;
	}
	catch( ... )
	{
		hr = E_FAIL;
	}

    return hr;
}
