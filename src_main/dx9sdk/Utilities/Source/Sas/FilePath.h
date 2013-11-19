//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once

namespace Sas
{
    class IFilePath
    {
    public:
        virtual const WCHAR* GetFilePath() const = 0;
        virtual const WCHAR* GetDirectoryPath() const = 0;
    };

	enum SOURCEINFO_TYPE
	{
		SOURCEINFO_NONE = 0,
		SOURCEINFO_FILE,
		SOURCEINFO_RESOURCE,
	};

    class SourceInfo : public IFilePath
    {
	protected:
        SOURCEINFO_TYPE m_Type;
        std::wstring m_Description;
        std::wstring m_Name;
        std::wstring m_Directory;
        std::wstring m_ResourceType;
        HMODULE m_ResourceModule;

	public:
        SourceInfo();
        SourceInfo(const SourceInfo& sf);
        SourceInfo& operator=(const SourceInfo& sf);

        SourceInfo(const WCHAR* fullPath);
        SourceInfo(const WCHAR* resource, const WCHAR* type, HMODULE hModule);

        void Set(const WCHAR* fullPath);
        void Set(const WCHAR* resource, const WCHAR* type, HMODULE hModule);

		SOURCEINFO_TYPE GetSourceType() const { return m_Type; }

        const WCHAR* GetResourceName() const { return m_Type != SOURCEINFO_RESOURCE ? NULL : m_Name.c_str(); }
        const WCHAR* GetResourceType() const { return m_Type != SOURCEINFO_RESOURCE ? NULL : m_ResourceType.c_str(); }
		HMODULE GetResourceModule() const { return m_Type != SOURCEINFO_RESOURCE ? NULL : m_ResourceModule; }

        const WCHAR* GetFilePath() const { return m_Type != SOURCEINFO_FILE ? NULL : m_Name.c_str(); }
        const WCHAR* GetDirectoryPath() const { return m_Type != SOURCEINFO_FILE ? NULL : m_Directory.c_str(); }

		const WCHAR* GetDescription() const { return m_Type == SOURCEINFO_NONE ? NULL : m_Description.c_str(); }

		HRESULT StringFromResource(std::wstring& str, const WCHAR* resNameOrType);
    };

}


