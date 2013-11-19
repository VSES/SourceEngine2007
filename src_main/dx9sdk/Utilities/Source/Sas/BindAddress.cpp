//--------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "stdafx.h"
#include "BindAddress.h"
using namespace std;
using namespace Sas;


//--------------------------------------------------------------------------------------
// Static members
//--------------------------------------------------------------------------------------
StringInfoMap BindAddressHelper::s_StringInfoMap;
HandleInfoVector BindAddressHelper::s_HandleInfoVector;

//--------------------------------------------------------------------------------------
// Expression ::= Namespace Step+
// Namespace  ::= 'Sas' | Id
// Step       ::= '.' Id ( '[' Index ']' )?
// Id         ::= [a-z_A-Z][a-z_A-Z0-9]*
// Index      ::= Integer | ‘*’
// Integer    ::= [0-9]+	
//--------------------------------------------------------------------------------------
/*
const wstring BindAddressHelper::_Integer = L"([0-9]+)";
const wstring BindAddressHelper::_Index = L"(" + _Integer + L"|\\052)";
const wstring BindAddressHelper::_Id = L"([a-z_A-Z][a-z_A-Z0-9]*)";
const wstring BindAddressHelper::_Step = L"(\\." + _Id + L"(\\[" + _Index + L"\\])?)";
const wstring BindAddressHelper::_Namespace = L"('Sas'|" + _Id + L")";
const wstring BindAddressHelper::_Expression = L"(" + _Namespace + _Step + L"+)";
const wstring BindAddressHelper::_Pattern = L"^" + _Expression + L"$";
*/


//--------------------------------------------------------------------------------------
// String -> BindAddressInfo 
//--------------------------------------------------------------------------------------
HRESULT BindAddressHelper::GetInfo( const WCHAR* strBindAddress, BindAddressInfo* pInfo )
{
	if( pInfo == NULL )
		return E_INVALIDARG;

	ZeroMemory( pInfo, sizeof(BindAddressInfo) );
	pInfo->Handle = -1;
    pInfo->MultipleHandle = -1;

	// Remove extra whitespace and convert to lowercase
	WCHAR strPreparedAddress[SAS_STRINGSIZE] = {0};
	PrepareAddress( strBindAddress, strPreparedAddress, SAS_STRINGSIZE );

	// If it's a well-formed address...
	if( !IsWellFormed(strPreparedAddress) )
		return E_FAIL;

	// see if it's found in the current list...
	StringInfoMap::iterator it = s_StringInfoMap.find( strPreparedAddress );
	if( it != s_StringInfoMap.end() )
	{
		*pInfo = (*it).second;
		return S_OK;
	}

	// if not, create a new entry

    // As an extra safety check, set aside handle 0 as invalid
    if( s_HandleInfoVector.size() == 0 )
    {
        BindAddressInfo invalidInfo;
        ZeroMemory( &invalidInfo, sizeof(BindAddressInfo) );
        invalidInfo.StrAddress = L"(INVALID)";
        invalidInfo.Handle = -1;
        invalidInfo.MultipleHandle = -1;
        s_HandleInfoVector.push_back( invalidInfo );
    }

    // Create a new entry
	BindAddressInfo info;
	ZeroMemory( &info, sizeof(BindAddressInfo) );

	info.StrAddress = strPreparedAddress;

	// Duplicate the address string
	info.Type = IsSingular(strPreparedAddress) ? BindAddress_Singular : BindAddress_Multiple;
    info.Handle = -1;
    info.MultipleHandle = -1;
	
    // Replace [<number>] with [*] to determine the multiple equivalent
    WCHAR strMultipleAddress[SAS_STRINGSIZE] = {0};
    ConvertAddressToMultiple( strPreparedAddress, strMultipleAddress, SAS_STRINGSIZE );

    // If the strings are different, then there is a multiple equivalent
    if( 0 != wcscmp( strPreparedAddress, strMultipleAddress ) )
        info.MultipleHandle = StringToHandle( strMultipleAddress );

    // The handle is the index into the HandleInfoVector list
    info.Handle = (int)s_HandleInfoVector.size();
	s_StringInfoMap[info.StrAddress] = info;
	s_HandleInfoVector.push_back( info );

	*pInfo = info;
	return S_OK;
}


//--------------------------------------------------------------------------------------
// BindAddressHandle > BindAddressInfo
//--------------------------------------------------------------------------------------
HRESULT BindAddressHelper::GetInfo( BindAddressHandle handle, BindAddressInfo* pInfo )
{
	if( pInfo == NULL )
		return E_INVALIDARG;

	ZeroMemory( pInfo, sizeof(BindAddressInfo) );
	pInfo->Handle = -1;
    pInfo->MultipleHandle = -1;

	if( handle < 0 || handle > (int)s_HandleInfoVector.size() )
		return E_FAIL;

	*pInfo = s_HandleInfoVector[handle];
	return S_OK;
}


//--------------------------------------------------------------------------------------
BindAddressHandle BindAddressHelper::StringToHandle( const WCHAR* strBindAddress )
{
	HRESULT hr;

	BindAddressInfo info;;
	hr = GetInfo( strBindAddress, &info );
	if( FAILED(hr) )
		return -1;
	 
	return info.Handle;
}


//--------------------------------------------------------------------------------------
const WCHAR* BindAddressHelper::HandleToString( BindAddressHandle handle )
{
	HRESULT hr;

	BindAddressInfo info;;
	hr = GetInfo( handle, &info );
	if( FAILED(hr) )
		return NULL;

	return info.StrAddress.c_str();
}


//--------------------------------------------------------------------------------------
// Clear all mappings and release string memory
//--------------------------------------------------------------------------------------
void BindAddressHelper::Clear()
{
	s_StringInfoMap.clear();
	s_HandleInfoVector.clear();
}


//--------------------------------------------------------------------------------------
// Verify the input string matches this pattern:
//
// Expression ::= Namespace Step+
// Namespace  ::= 'Sas' | Id
// Step       ::= '.' Id ( '[' Index ']' )?
// Id         ::= [a-z_A-Z][a-z_A-Z0-9]*
// Index      ::= Integer | ‘*’
// Integer    ::= [0-9]+	
//--------------------------------------------------------------------------------------
bool BindAddressHelper::IsWellFormed( const WCHAR* strBindAddress )
{
    //debug
    return true;

    /*
    WCHAR address[SAS_STRINGSIZE] = {0};
    PrepareAddress( strBindAddress, address, SAS_STRINGSIZE );

    regex::match_results results;
    regex::rpattern pattern( _Pattern );

    regex::match_results::backref_type br = pattern.match( address, results );
    return br.matched;
    */
}

//--------------------------------------------------------------------------------------
bool BindAddressHelper::IsSingular( const WCHAR* strBindAddress )
{
	return ( NULL == wcsstr(strBindAddress, L"[*]") );
}


//--------------------------------------------------------------------------------------
HRESULT BindAddressHelper::PrepareAddress( const WCHAR* str, WCHAR* strPrepared, size_t bufferLength )
{
	if( str == NULL || strPrepared == NULL || bufferLength <= 0 )
		return E_INVALIDARG;
	
	WCHAR* pStart = (WCHAR*)str;
	WCHAR* pEnd = (WCHAR*)str + wcslen(str)-1;
	

	// Trim off leading whitespace
	while( pStart <= pEnd )
	{
		// Break when a non-whitespace character is found
		if( !iswspace(*pStart) )
			break;
			
		pStart++;
	}

	// Trim off trailing whitespace
	while( pEnd >= pStart )
	{
		// Break when a non-whitespace character is found
		if( !iswspace(*pEnd) )
			break;
			
		pEnd--;
	}

	// If no non-whitespace characters are found, return an empty string
	if( pStart > pEnd )
	{
		*strPrepared = 0;
		return S_OK;
	}

	// Copy the string in such a way that extra whitespace is left out
	StringCchCopyN( strPrepared, bufferLength, pStart, (pEnd - pStart) +1 );
    
	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT BindAddressHelper::ConvertAddressToMultiple( const WCHAR* str, WCHAR* strMultiple, size_t bufferLength )
{
	if( str == NULL || strMultiple == NULL || bufferLength <= 0 )
		return E_INVALIDARG;
	
    // The output buffer should be at least the size of the input string (there's no chance
    // of a multiple address being larger than a singular)
    if( bufferLength <= wcslen(str) )
        return E_INVALIDARG;

    ZeroMemory( strMultiple, sizeof(WCHAR) * bufferLength );

	WCHAR* pSrc = (WCHAR*)str;
    WCHAR* pDest = (WCHAR*)strMultiple;
    bool copying = true;

    while( *pSrc )
    {
        if( copying )
        {
            *pDest++ = *pSrc;

            // Switch to skipping if you find a '['
            if( *pSrc == L'[' )
            {
                // Substitute the '*' character for anything you find until the closing bracket
                *pDest++ = L'*';
                copying = false;
            }
        }
        else
        {
            // If you find the closing bracket, start copying again
            if( *pSrc == L']' )
            {
                *pDest++ = *pSrc;
                copying = true;
            }
        }
        
        // Increment the source pointer
        pSrc++;
    }
    
    // Null terminate
    *pDest = 0;
	return S_OK;
}
