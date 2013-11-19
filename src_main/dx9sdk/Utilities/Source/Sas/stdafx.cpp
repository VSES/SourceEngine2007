//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"

void WCharToWString( const WCHAR* source, std::wstring& dest )
{
	try
	{
		dest = source;
	}
	catch( ... )
	{}
}

void CharToString( const CHAR* source, std::string& dest )
{
	try
	{
		dest = source;
	}
	catch( ... )
	{}
}
	
void CharToWString( const CHAR* source, std::wstring& dest )
{
	try
	{
		size_t size = 0;
		StringCchLengthA(source, STRSAFE_MAX_CCH, &size);
		size++;

		WCHAR* tmp = (WCHAR*) alloca( size * sizeof(WCHAR) );
		StringCchPrintfW(tmp, size, L"%S", source);

		dest = tmp;
	}
	catch( ... )
	{}
}

void WCharToString( const WCHAR* source, std::string& dest )
{
	try
	{
		size_t size = 0;
		StringCchLengthW(source, STRSAFE_MAX_CCH, &size);
		size++;

		CHAR* tmp = (CHAR*) alloca( size * sizeof(CHAR) );
		StringCchPrintfA(tmp, size, "%S", source);

		dest = tmp;
	}
	catch( ... )
	{}
}
