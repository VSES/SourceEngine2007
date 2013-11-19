//*****************************************************************************
//
// File supervisor: Marc Poirier
// Date: 5/18/1999
// (c) Copyright 2003 Softimage co.
//
// @doc
//
// @module c3dexportsmdvalidate.cpp | key validation functions for MOD (Valve EXP)
// <C3DCreateModelTask> class.
//
//*****************************************************************************

#include "stdafx.h"
#ifdef SGI_COMPILER
#pragma hdrstop
#endif

#include <shellapi.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "md5.h"
#include <SIBCString.h>

#include "expvalidate.h"
#include "ApplicationInfo.h"
#include "resource.h"

extern "C" HINSTANCE& GethmodelsInstance(void);
//#define min(a,b) (((a)<(b)) ? (a) : (b))

static	CSIBCString   g_TheProduct;
static	CSIBCString	g_TheWeb;
static	CSIBCString	g_TheWelcome;
static	CSIBCString	g_TheTitle;

#define DS_REGKEY_COEXISTENCE               TEXT( "CoExistence" )
#define DS_REGVALUE_APPNAME                 TEXT( "AppName" )
#define DS_REGVALUE_APPVERSION              TEXT( "AppVersion" )
#define DS_REGVALUE_SICOREVERSION           TEXT( "SiCoreVersion" )
#define DS_REGVALUE_STORMVERSION            TEXT( "StormVersion" )
#define DS_REGVALUE_APP_DLLPATH             TEXT( "AppDllPath" )
#define DS_REGVALUE_SICORE_DLLPATH          TEXT( "SicorePath" )
#define DS_REGVALUE_STORM_DLLPATH           TEXT( "StormPath" )
#define DS_REGVALUE_APP_PATHENVVAR          TEXT( "DllPathEnvVar" )
#define DS_REGVALUE_APP_CLSID				TEXT( "AppCLSID" )

//
// NEW for application coexistence
//
#define DS_REGKEY_NEWROOT                  TEXT( "Software\\Softimage" )
#define DS_REGKEY_SICORE                   TEXT( "SOFTIMAGE|SICORE Engine" ) 
#define DS_REGKEY_COMSERVERLOCATIONS	   TEXT( "Software\\Softimage\\ComServerLocations" )
#define DS_REGKEY_APPLICATION			   TEXT( "SOFTIMAGE Application" ) 
#define DS_REGKEY_OBJMDL				   TEXT( "Object Model")


HRESULT DSRegGetAppVersionInfo( LPTSTR	out_pstrAppName, 
						         LPTSTR	out_pstrVersion, 
						         LPTSTR	out_pstrSiCoreVersion, 
						         UINT	in_uiMaxCharNum );

HRESULT DSConstructVersionRegKey( LPCTSTR in_szPartKey, 
                                   LPTSTR out_szAppKey,
                                   LPTSTR out_szSicoreKey,
						           UINT	in_uiMaxCharNum );

HRESULT _DSRegGetAppDllPath( LPTSTR out_szAppDllPath, UINT in_uiMaxCharNum );
HRESULT _DSRegGetSicoreDllPath( LPTSTR out_szSicoreDllPath, UINT in_uiMaxCharNum );

extern HINSTANCE	___gThis;
//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
// @parm time_t | time_t input
// @parm LPFILETIME | converted time (output)
//
//
//*****************************************************************************
static void UnixTimeToFileTime(time_t in_t, LPFILETIME pft)
{
	// Note that LONGLONG is a 64-bit value
	LONGLONG ll;

	ll = Int32x32To64(in_t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = (DWORD) (ll >> 32);
} 


//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
// @parm unsigned char* | input buffer
//
// @devnote convert all value in the to into the range ['A','Z']
//*****************************************************************************
static void ConvertToReadable(unsigned char* io_ucBuffer, unsigned int in_uiBufferLen)
{
	unsigned int i;

	// Bring all value in the range ['A', 'Z']
	for(i = 0; i < in_uiBufferLen; i++)	{
		// Alreay in the range
		if ((io_ucBuffer[i] >= (unsigned char) 'A') && (io_ucBuffer[i] <= (unsigned char) 'Z'))
			continue;

		// Just need to remove the upcase bit
		if ((io_ucBuffer[i] >= (unsigned char) 'a') && (io_ucBuffer[i] <= (unsigned char) 'z')) {
			io_ucBuffer[i] &= 0xDF;
			continue;
		}

		// Bring in the right range
		io_ucBuffer[i] = (unsigned char) ((unsigned char) 'A' + (io_ucBuffer[i] % 26));
	}
	io_ucBuffer[in_uiBufferLen-1] = 0;
}

//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
// @parm unsigned int | Days to stamp
// @parm unsigned char* | buffer to stamp to
//
// @devnote Stamp the days converted to the range ['A', 'P'] in the buffer
//*****************************************************************************
static void StampDays(unsigned int in_NbDays, unsigned char* io_ucBuffer)
{
	unsigned int l_iMod = *io_ucBuffer % 3;
	unsigned char l_ucDays[5];

	// By precaution only keep
	in_NbDays = min(in_NbDays, 0xFFFF);

	// Format on 4 char
	sprintf((char*)l_ucDays, "%04x", in_NbDays);

	// Stamp the 4 char in the hash
	for(unsigned int i=0; i < 4; i++) {
		unsigned int l_uiIndex = 3 + (i*4) - l_iMod;

		if (l_ucDays[i] <= '9') {
			io_ucBuffer[l_uiIndex] = (unsigned char) ((unsigned char) 'A' + (l_ucDays[i] - '0'));
		} else {
			io_ucBuffer[l_uiIndex] = (unsigned char) ((l_ucDays[i] + 10) & 0xDF);

		}
	}
}

//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
// @parm unsigned char* | buffer to extract from
//
// @devnote return the days converted from to the range ['A', 'P']
//*****************************************************************************
static unsigned int ExtractDays(unsigned char* in_ucHash)
{
	unsigned int l_iMod = *in_ucHash % 3;
	unsigned char l_ucDays[5];
	unsigned int l_uiNbDays = 0;

	// Extract the 4 char from the hash
	for(unsigned int i=0; i < 4; i++) {
		unsigned int l_uiIndex = 3 + (i*4) - l_iMod;

		if (in_ucHash[l_uiIndex] > 'J') {
			l_ucDays[i] = (unsigned char)  (in_ucHash[l_uiIndex] - 10);
		} else {
			l_ucDays[i] = (unsigned char) (in_ucHash[l_uiIndex] - 'A' + '0');
		}
	}
	// Convert it
	sscanf((char*)l_ucDays, "%04x", &l_uiNbDays);
	return l_uiNbDays;
}

//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
// @parm unsigned int | days number
// @parm unsigned char* | email
// @parm unsigned char* | output buffer
//
// @devnote Generate a hash key base on input
//*****************************************************************************
static void GenerateHash(unsigned int in_uiNbDays, unsigned char* in_ucEmail, unsigned char* out_ucHash,
						 unsigned int in_uiHashLen, unsigned char * in_pProduct)
{
	MD5_CTX l_md5Context;
	unsigned char l_ucBuffer[1024];

	// Format the string to be hashed
	sprintf((char*)l_ucBuffer, "%x %s", in_uiNbDays, in_ucEmail );

	// Calculate MD5
	MD5Init(&l_md5Context);
	MD5Update(&l_md5Context, l_ucBuffer, strlen((char *)l_ucBuffer));
	MD5Final(out_ucHash, &l_md5Context);

	// Convert the result so all chars are in the ['A', 'Z'] range
	ConvertToReadable(out_ucHash, in_uiHashLen);


	// Stamp the days in the generated hash
	StampDays(in_uiNbDays, out_ucHash);
}

//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
// @parm unsigned char* | input email
// @parm unsigned char* | input hash key
//
// @devnote Validate the key using input + current time
//*****************************************************************************
static bool ValidateHash1(unsigned char* in_ucEmail, unsigned char* in_ucHash, unsigned char * in_pProduct )
{
	unsigned char l_ucMD5[17];
	unsigned int l_uiNbDays = time(NULL) / (60 * 60 * 24);
	bool l_bRes1 = false, l_bRes2 = false;

	// Generate the hash base on this days and email
	memset(l_ucMD5, 0, sizeof(l_ucMD5));
	GenerateHash(l_uiNbDays, in_ucEmail, l_ucMD5, sizeof(l_ucMD5), in_pProduct);
	l_bRes1 = strcmp((char*)l_ucMD5, (char*)in_ucHash) ? false : true;

	// Generate the hash base on the previous days and email
	l_uiNbDays -= 1;
	memset(l_ucMD5, 0, sizeof(l_ucMD5));
	GenerateHash(l_uiNbDays, in_ucEmail, l_ucMD5, sizeof(l_ucMD5), in_pProduct);
	l_bRes2 = strcmp((char*)l_ucMD5, (char*)in_ucHash) ? false : true;

	// Only one should match
	return l_bRes1 ^ l_bRes2;
}

//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
//
// @devnote Validate the key using input from the registry
//*****************************************************************************
static bool ValidateHash2( TCHAR * in_pKey )
{
	TCHAR l_szAppKey[_MAX_PATH+1];
	unsigned char l_ucMD5[17];
	unsigned int l_uiNbDays;
	char l_strEmail[1024];
	char l_strHash[1024];
	char l_strKey[1024];
	char l_szEmail[1024];
	char l_szHash[17];
	HKEY l_hKey = NULL;
	FILETIME l_ftWrite;
	FILETIME l_ftHashGenerated;
	DWORD	l_dwSize = 0;
	bool l_bRes = false;

	// 
	// Get the data from the registry
	//

	if (S_OK != DSConstructVersionRegKey(in_pKey , NULL, l_szAppKey, sizeof(l_szAppKey)/sizeof(TCHAR)))
		goto Cleanup;

	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, l_szAppKey, 0, KEY_ALL_ACCESS, &l_hKey))
		goto Cleanup;

	if (ERROR_SUCCESS != RegQueryInfoKey(l_hKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &l_ftWrite))
		goto Cleanup;

	l_dwSize = sizeof(l_szEmail)*sizeof(TCHAR);
	if (ERROR_SUCCESS != RegQueryValueEx(l_hKey, _T("Email"), NULL, NULL, (BYTE*) l_szEmail, &l_dwSize))
		goto Cleanup;

	l_dwSize = sizeof(l_szHash)*sizeof(TCHAR);
	if (ERROR_SUCCESS != RegQueryValueEx(l_hKey, _T("Key"), NULL, NULL, (BYTE*) l_szHash, &l_dwSize))
		goto Cleanup;

	//
	// Validate checksum by itself
	//
	strcpy (l_strEmail, l_szEmail);
	strcpy (l_strHash, l_szHash);
	strcpy (l_strKey, in_pKey );
	l_uiNbDays = ExtractDays((unsigned char*)l_strHash);
	memset(l_ucMD5, 0, sizeof(l_ucMD5));
	GenerateHash(l_uiNbDays, (unsigned char*)l_strEmail, l_ucMD5, sizeof(l_ucMD5),  (unsigned char*)l_strKey);
	l_bRes = strcmp((char*)l_ucMD5, (char*)l_strHash) ? false : true;

	//
	// Check last modification date of the registry key
	//
	if (l_bRes) {
		ULARGE_INTEGER l_liWrite, l_liHashGenerated, l_liRes;

		l_liWrite.LowPart = l_ftWrite.dwLowDateTime;
		l_liWrite.HighPart = l_ftWrite.dwHighDateTime;
		UnixTimeToFileTime(l_uiNbDays * 60 * 60 * 24, &l_ftHashGenerated);
		l_liHashGenerated.LowPart = l_ftHashGenerated.dwLowDateTime;
		l_liHashGenerated.HighPart = l_ftHashGenerated.dwHighDateTime;
		l_liRes.QuadPart = (l_liWrite.QuadPart - l_liHashGenerated.QuadPart) / 10000000;

		// Less then 2 days
		if (l_liRes.QuadPart < 172800) {
			l_bRes = true;
		}
	}

Cleanup:
	if (l_hKey) RegCloseKey(l_hKey);

	return l_bRes;
}

//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
// @parm unsigned char* | input email
// @parm unsigned char* | input hash key
//
// @devnote Persist email & hash key in the registry
//*****************************************************************************
static bool SaveEmailHash(unsigned char* in_ucEmail, unsigned char* in_ucHash, TCHAR * in_pKey)
{
	TCHAR	l_szAppKey[_MAX_PATH+1];
	HKEY	l_hKey = NULL;
	DWORD	l_dwDisp;
	bool	l_bRes = false;

	if (S_OK != DSConstructVersionRegKey(in_pKey, NULL, l_szAppKey, sizeof(l_szAppKey)/sizeof(TCHAR)))
		goto Cleanup;

	// Create a coexistence key for MODKey
	if (ERROR_SUCCESS != RegCreateKeyEx (HKEY_LOCAL_MACHINE, l_szAppKey, 0, NULL, REG_OPTION_NON_VOLATILE, 
										 KEY_ALL_ACCESS, NULL, &l_hKey, &l_dwDisp) )
		goto Cleanup;

	// Persist Email
	if (ERROR_SUCCESS != RegSetValueEx(l_hKey, _T("Email"), 0, REG_SZ, (CONST BYTE *) in_ucEmail, (strlen((char*) in_ucEmail)+1) * sizeof(TCHAR)))
		goto Cleanup;

	// Persist Key
	if (ERROR_SUCCESS != RegSetValueEx(l_hKey, _T("Key"), 0, REG_SZ, (CONST BYTE *) in_ucHash, (strlen((char*) in_ucHash)+1) * sizeof(TCHAR)))
		goto Cleanup;

	l_bRes = true;

Cleanup:
	if (l_hKey) RegCloseKey(l_hKey);

	return l_bRes;
}


//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
// @parm HWND | windown handle
// @parm UINT | message
// @parm WPARAM | wParam
// @parm LPARAM | lParam
//
// @devnote Dialog Proc
//*****************************************************************************
static int CALLBACK ValidateKeyProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg) {
		case WM_INITDIALOG: 
		{
			HWND l_welcome = GetDlgItem( hwnd, IDC_WELCOME );
			SetWindowText( l_welcome, (LPCTSTR) g_TheWelcome.GetText() );
	//		HWND l_title = GetDlgItem( hwnd, IDD_MOD_REGISTRATION );
			SetWindowText( hwnd, (LPCTSTR) g_TheTitle.GetText() );
			return TRUE;
		}
		case WM_COMMAND:
		{
			char l_szEmail[1024];
			char l_szKey[17];

			CSIBCString	l_finalString;
			switch (wParam) {
				case IDOK:
					// User press OK
					memset(l_szEmail, 0, sizeof(l_szEmail));
					memset(l_szKey, 0, sizeof(l_szKey));

					// Get data from the fields
					GetDlgItemText(hwnd, IDC_EMAIL, l_szEmail, sizeof(l_szEmail) / sizeof(TCHAR));
					GetDlgItemText(hwnd, IDC_KEY, l_szKey, sizeof(l_szKey) / sizeof(TCHAR));
					l_szEmail[sizeof(l_szEmail)/sizeof(TCHAR)-1] = 0;
					l_szKey[sizeof(l_szKey)/sizeof(TCHAR)-1] = 0;
					
					if (!ValidateHash1((unsigned char*)l_szEmail, (unsigned char*) l_szKey,
						(unsigned char *) (LPCSTR) g_TheProduct.GetText()))
					{
						MessageBox(hwnd, _T("Invalid key"), _T("Error"), MB_OK);
					} 
					else 
					{
						// Persist the info in the registry
						if (!SaveEmailHash((unsigned char*) l_szEmail, (unsigned char*) l_szKey, (LPTSTR) (LPCTSTR) g_TheProduct.GetText()))
						{
							MessageBox(hwnd, _T("Can't save key"), _T("Error"), MB_OK);
							EndDialog(hwnd, 0);
							return TRUE;
						}
						// Validation is good
						EndDialog(hwnd, 1);
						return TRUE;
					}
					
					return TRUE;
				case IDCANCEL: 
					EndDialog(hwnd, 0);
					return TRUE;
				case IDREGISTER:

#ifndef	_DEBUG
					l_finalString = "http://www.softimage.com/products/mod/v4/register/";
#else
					l_finalString = "http://bamboo/products/mod/v4/register/";
#endif

					// call up the browser
#ifdef unix
					system( l_finalString );
#else
//					int ret = WinExec( l_finalString.GetText(), SW_SHOWMAXIMIZED );

					ShellExecute( NULL, NULL, l_finalString.GetText(), NULL, NULL, SW_SHOWMAXIMIZED );

#endif


//					DSOpenWebBrowser( l_finalString );
					return TRUE;
			}
		}
	}
	return FALSE;
}

//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
//
// @devnote Helper to validate email & hash key from the registry
//*****************************************************************************
static HMODULE g_hModule ;
bool ValidateRegistrationCurrentKey( TCHAR * in_pKey )
{
	bool l_bRet = false;
	g_hModule = LoadLibrary(_T("suminfo.dll"));
	l_bRet = ValidateHash2( in_pKey );
	FreeLibrary(g_hModule);
	g_hModule = NULL;
	return l_bRet;
}

//*****************************************************************************
// Date:	Octobre 2003
// Author:	Marc Porier
//
//
// @devnote Helper to validate email & hash key from user input
//*****************************************************************************
bool ValidateRegistrationNewKey( TCHAR * in_pKey, TCHAR * in_pTheWeb, TCHAR * in_pTheWelcome, TCHAR * in_pTheTitle )
{
	bool l_bRet = false;

	g_hModule = LoadLibrary(_T("suminfo.dll"));
	g_TheProduct = in_pKey;
	g_TheWeb = in_pTheWeb;
	g_TheWelcome = in_pTheWelcome;
	g_TheTitle = in_pTheTitle;
	if (DialogBoxParam(___gThis, MAKEINTRESOURCE(IDD_MOD_REGISTRATION), GetActiveWindow(), ValidateKeyProc, NULL) > 0)
	{
		l_bRet = true;
	}
	FreeLibrary(g_hModule);
	g_hModule = NULL;
	return l_bRet;
}


HRESULT DSRegGetAppVersionInfo( LPTSTR	out_pstrAppName, 
						         LPTSTR	out_pstrVersion, 
						         LPTSTR	out_pstrSiCoreVersion, 
						         UINT	in_uiMaxCharNum )
{

    TCHAR tcFileName[_MAX_PATH+1];
    DWORD dwSize = _MAX_PATH;

    tcFileName[0] = 0;
    //////////////////////////////////////////////////////////////
    // Get module file name.  The module file name is the registry
    // key that contains product name, version, and SiCore version.
    //////////////////////////////////////////////////////////////
    if (GetModuleFileName(g_hModule, tcFileName, dwSize) == 0)
    {
        return E_FAIL;
    }

    TCHAR* pBackSlash = 0;
    // Change occurences of back slashes or slashes to pipes
    for (pBackSlash = tcFileName; *pBackSlash != 0; pBackSlash++)
    {
        if (*pBackSlash == _T('\\'))
            *pBackSlash = _T('|');
        else if (*pBackSlash == _T('/'))
            *pBackSlash = _T('|');
    }

    // remove filename
    pBackSlash = _tcsrchr(tcFileName, _T('|'));
    if (pBackSlash)
        *pBackSlash = 0;

    //////////////////////
    // Construct Key
    //////////////////////
    TCHAR   tcSubKey[_MAX_PATH];
    _stprintf(tcSubKey, _T("%s\\%s\\%s"), DS_REGKEY_NEWROOT, DS_REGKEY_COEXISTENCE,
                        tcFileName);

    /////////////////
	// Open Key
	/////////////////
    HKEY	hKey ;
	DWORD	dwValueType ;
	DWORD	dwValueSize = in_uiMaxCharNum*sizeof( TCHAR ) ;

    if ( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, tcSubKey, NULL, 
						 KEY_EXECUTE|KEY_READ, &hKey ) != ERROR_SUCCESS )
    {
        return E_FAIL;
    }
	
    //////////////////////
	// Get the values 
	//////////////////////
    if( out_pstrAppName != NULL )
    {
		if ( ::RegQueryValueEx(	hKey, DS_REGVALUE_APPNAME, NULL,
								&dwValueType, (LPBYTE)out_pstrAppName, 
								&dwValueSize ) != ERROR_SUCCESS )
		{
			::RegCloseKey( hKey ) ;
			return( E_FAIL ) ;
		}
    }
	
    if ( out_pstrVersion != NULL )
    {
		dwValueSize = in_uiMaxCharNum*sizeof( TCHAR ) ;
		if ( ::RegQueryValueEx(	hKey, DS_REGVALUE_APPVERSION, NULL,
								&dwValueType, (LPBYTE)out_pstrVersion, 
								&dwValueSize ) != ERROR_SUCCESS )
		{
			::RegCloseKey( hKey ) ;
			return( E_FAIL ) ;
		}
    }
	
    if ( out_pstrSiCoreVersion != NULL )
    {
		dwValueSize = in_uiMaxCharNum*sizeof( TCHAR ) ;
        if ( ::RegQueryValueEx(	hKey, DS_REGVALUE_SICOREVERSION, NULL,
								&dwValueType, (LPBYTE)out_pstrSiCoreVersion, 
								&dwValueSize ) != ERROR_SUCCESS )
		{
			::RegCloseKey( hKey ) ;
			return( E_FAIL ) ;
		}
    }

    ::RegCloseKey( hKey ) ;
	return( S_OK ) ;
}

HRESULT DSConstructVersionRegKey( LPCTSTR in_szPartKey, 
                                   LPTSTR out_szAppKey,
                                   LPTSTR out_szSicoreKey,
						           UINT	in_uiMaxCharNum )
{
    //
    // get app name
    //
    TCHAR tszAppName[_MAX_FNAME];
    HRESULT hr = ::DSRegGetAppVersionInfo( tszAppName, NULL, 
                                           NULL, _MAX_FNAME);
    if (FAILED(hr))
    {
        return(hr);
    }

    //
    // get app dll path
    //
    TCHAR tszAppDllPath[_MAX_FNAME];
    hr = _DSRegGetAppDllPath( tszAppDllPath, _MAX_FNAME);
    if (FAILED(hr))
    {
        return(hr);
    }

	//
    // get Sicore dll path
    //
    TCHAR tszSicoreDllPath[_MAX_FNAME];
    hr = _DSRegGetSicoreDllPath( tszSicoreDllPath, _MAX_FNAME);
    if (FAILED(hr))
    {
        return(hr);
    }

    // construct app key        
    if (out_szAppKey != NULL)
        _sntprintf(out_szAppKey, in_uiMaxCharNum, _T("%s\\%s\\%s%s%s"), DS_REGKEY_NEWROOT, tszAppName,
				   tszAppDllPath, in_szPartKey ? _T("\\") : _T(""), in_szPartKey ? in_szPartKey : _T(""));

    // construct sicore key
    if (out_szSicoreKey != NULL)
        _sntprintf(out_szSicoreKey, in_uiMaxCharNum,_T("%s\\%s\\%s%s%s"), DS_REGKEY_NEWROOT, DS_REGKEY_SICORE,
                   tszSicoreDllPath, in_szPartKey ? _T("\\") : _T(""), in_szPartKey ? in_szPartKey : _T(""));
    
    return (S_OK);
}

HRESULT _DSRegGetAppDllPath( LPTSTR out_szAppDllPath, UINT in_uiMaxCharNum )
{

    TCHAR tcFileName[_MAX_PATH+1];
    DWORD dwSize = _MAX_PATH;

    // initialization and validation
    tcFileName[0] = 0;
    if( out_szAppDllPath == NULL )
        return E_POINTER;

    if (GetModuleFileName(g_hModule, tcFileName, dwSize) == 0)
    {
        return E_FAIL;
    }

    TCHAR* pBackSlash = 0;
    // Change occurences of back slashes or slashes to pipes
    for (pBackSlash = tcFileName; *pBackSlash != 0; pBackSlash++)
    {
        if (*pBackSlash == _T('\\'))
            *pBackSlash = _T('|');
        else if (*pBackSlash == _T('/'))
            *pBackSlash = _T('|');
    }

    // remove filename
    pBackSlash = _tcsrchr(tcFileName, _T('|'));
    if (pBackSlash)
        *pBackSlash = 0;

    //////////////////////
    // Construct Key
    //////////////////////
    TCHAR   tcSubKey[_MAX_PATH];
    _stprintf(tcSubKey, _T("%s\\%s\\%s"), DS_REGKEY_NEWROOT, DS_REGKEY_COEXISTENCE,
                        tcFileName);

    /////////////////
	// Open Key
	/////////////////
    HKEY	hKey ;
	DWORD	dwValueType ;
	DWORD	dwValueSize = in_uiMaxCharNum*sizeof( TCHAR ) ;

    if ( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, tcSubKey, NULL, 
						 KEY_EXECUTE|KEY_READ, &hKey ) != ERROR_SUCCESS )
    {
        return E_FAIL;
    }
	
    //////////////////////
	// Get the values 
	//////////////////////
	if ( ::RegQueryValueEx(	hKey, DS_REGVALUE_APP_DLLPATH, NULL,
							&dwValueType, (LPBYTE)out_szAppDllPath, 
							&dwValueSize ) != ERROR_SUCCESS )
	{
		::RegCloseKey( hKey ) ;
		return( E_FAIL ) ;
	}
	

    ::RegCloseKey( hKey ) ;
	return( S_OK ) ;
}

HRESULT _DSRegGetSicoreDllPath( LPTSTR out_szSicoreDllPath, UINT in_uiMaxCharNum )
{

    TCHAR tcFileName[_MAX_PATH+1];
    DWORD dwSize = _MAX_PATH;

    // initialization and validation
    tcFileName[0] = 0;
    if( out_szSicoreDllPath == NULL )
        return E_POINTER;

    if (GetModuleFileName(g_hModule, tcFileName, dwSize) == 0)
    {
        return E_FAIL;
    }

    TCHAR* pBackSlash = 0;
    // Change occurences of back slashes or slashes to pipes
    for (pBackSlash = tcFileName; *pBackSlash != 0; pBackSlash++)
    {
        if (*pBackSlash == _T('\\'))
            *pBackSlash = _T('|');
        else if (*pBackSlash == _T('/'))
            *pBackSlash = _T('|');
    }

    // remove filename
    pBackSlash = _tcsrchr(tcFileName, _T('|'));
    if (pBackSlash)
        *pBackSlash = 0;

    //////////////////////
    // Construct Key
    //////////////////////
    TCHAR   tcSubKey[_MAX_PATH];
    _stprintf(tcSubKey, _T("%s\\%s\\%s"), DS_REGKEY_NEWROOT, DS_REGKEY_COEXISTENCE,
                        tcFileName);

    /////////////////
	// Open Key
	/////////////////
    HKEY	hKey ;
	DWORD	dwValueType ;
	DWORD	dwValueSize = in_uiMaxCharNum*sizeof( TCHAR ) ;

    if ( ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, tcSubKey, NULL, 
						 KEY_EXECUTE|KEY_READ, &hKey ) != ERROR_SUCCESS )
    {
        return E_FAIL;
    }
	
    //////////////////////
	// Get the values 
	//////////////////////
	if ( ::RegQueryValueEx(	hKey, DS_REGVALUE_SICORE_DLLPATH, NULL,
							&dwValueType, (LPBYTE)out_szSicoreDllPath, 
							&dwValueSize ) != ERROR_SUCCESS )
	{
		::RegCloseKey( hKey ) ;
		return( E_FAIL ) ;
	}
	

    ::RegCloseKey( hKey ) ;
	return( S_OK ) ;
}