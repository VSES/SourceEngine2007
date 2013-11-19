//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#ifndef _BINDADDRESS_H
#define _BINDADDRESS_H
//#include <map>
//#include <vector>
using namespace std;

//---------------------------------------
//Expression ::= Namespace Step+
//Namespace  ::= 'Sas' | Id
//Step       ::= '.' Id ( '[' Index ']' )?
//Id         ::= [a-z_A-Z][a-z_A-Z0-9]*
//Index      ::= Integer | ‘*’
//Integer    ::= [0-9]+	
//---------------------------------------
	
#define WCharToBindAddressHandle( __str ) Sas::BindAddressHelper::StringToHandle( __str )

namespace Sas
{
	typedef int BindAddressHandle;
	
	enum BindAddressType
	{
		BindAddress_Invalid, 
		BindAddress_Singular,
		BindAddress_Multiple
	};

	struct BindAddressInfo
	{
		wstring StrAddress; // Don't modify
		BindAddressType Type;
		BindAddressHandle Handle;
        BindAddressHandle MultipleHandle; // For singular bind addresses, this is the handle of the multiple address ('[*]' syntax). For multiple address, it's -1.
	};

	typedef map<wstring, BindAddressInfo, StringCompareInsensitive> StringInfoMap;
	typedef vector<BindAddressInfo> HandleInfoVector;

	class BindAddressHelper
	{
	public:
		static bool IsWellFormed( const WCHAR* strBindAddress );
		static bool IsSingular( const WCHAR* strBindAddress );
		
		static HRESULT GetInfo( const WCHAR* strBindAddress, BindAddressInfo* pInfo );
		static HRESULT GetInfo( BindAddressHandle handle, BindAddressInfo* pInfo );
		
		static BindAddressHandle StringToHandle( const WCHAR* strBindAddress );
		static const WCHAR* HandleToString( BindAddressHandle handle );

		static void Clear();
		static void ReleaseStaticResources() { Clear(); }

	private:
		static HRESULT PrepareAddress( const WCHAR* strBindAddress, WCHAR* strPreparedAddress, size_t cchPrepared );
        static HRESULT ConvertAddressToMultiple( const WCHAR* strBindAddress, WCHAR* strMultiple, size_t bufferLength );
		static StringInfoMap s_StringInfoMap;
		static HandleInfoVector s_HandleInfoVector;

        /*
        static const wstring _Integer;
        static const wstring _Index;
        static const wstring _Id;
        static const wstring _Step;
        static const wstring _Namespace;
        static const wstring _Expression;
        static const wstring _Pattern;
        */
	};
}

#endif //_BINDADDRESS_H