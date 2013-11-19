//----------------------------------------------------------------------------
// File: DXErr.cpp
//
// Desc: The DXErr sample allows users to enter a numberical HRESULT and get back
//       the string match its define.  For example, entering 0x8878000a will
//       return DSERR_ALLOCATED.
//
// Copyright (c) Microsoft Corp. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include <windows.h>
#include <math.h>
#include <basetsd.h>
#include <dxerr.h>
#include <tchar.h>
#include "resource.h"

#include <strsafe.h>





//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
VOID OnInitDialog( HWND hDlg );
VOID LookupValue( HWND hDlg );
BOOL g_bUseHex = TRUE;




//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------

// Older winerror.h may not have all of these, so include them here 
// so this app can build on older Platform SDKs
#define FACILITY_WINDOWS_CE              24
#define FACILITY_WINDOWS                 8
#define FACILITY_URT                     19
#define FACILITY_UMI                     22
#define FACILITY_SXS                     23
#define FACILITY_STORAGE                 3
#define FACILITY_STATE_MANAGEMENT        34
#define FACILITY_SSPI                    9
#define FACILITY_SCARD                   16
#define FACILITY_SETUPAPI                15
#define FACILITY_SECURITY                9
#define FACILITY_RPC                     1
#define FACILITY_WIN32                   7
#define FACILITY_CONTROL                 10
#define FACILITY_NULL                    0
#define FACILITY_MSMQ                    14
#define FACILITY_MEDIASERVER             13
#define FACILITY_INTERNET                12
#define FACILITY_ITF                     4
#define FACILITY_HTTP                    25
#define FACILITY_DPLAY                   21
#define FACILITY_DISPATCH                2
#define FACILITY_CONFIGURATION           33
#define FACILITY_COMPLUS                 17
#define FACILITY_CERT                    11
#define FACILITY_BACKGROUNDCOPY          32
#define FACILITY_ACS                     20
#define FACILITY_AAF                     18

#define FACILITY_D3DX                    0x877
#define FACILITY_D3D                     0x876




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point for the application.  Since we use a simple dialog for 
//       user interaction we don't need to pump messages.
//-----------------------------------------------------------------------------
INT APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, 
                      INT nCmdShow )
{
    // Display the main dialog box.
    DialogBox( hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc );

    return TRUE;
}




//-----------------------------------------------------------------------------
// Name: MainDlgProc()
// Desc: Handles dialog messages
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg ) 
    {
        case WM_INITDIALOG:
            OnInitDialog( hDlg );
            break;

        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
                case IDC_LOOKUP:
                    LookupValue( hDlg );
                    break;

                case IDC_HEX:
                    g_bUseHex = TRUE;
                    break;

                case IDC_DECIMAL:
                    g_bUseHex = FALSE;
                    break;

                case IDCANCEL:
                    EndDialog( hDlg, IDCANCEL );
                    break;

                default:
                    return FALSE; // Didn't handle message
            }
            break;

        case WM_DESTROY:
            break; 

        default:
            return FALSE; // Didn't handle message
    }

    return TRUE; // Handled message
}




//-----------------------------------------------------------------------------
// Name: OnInitDialog()
// Desc: Initializes the dialogs (sets up UI controls, etc.)
//-----------------------------------------------------------------------------
VOID OnInitDialog( HWND hDlg )
{
    // Load the icon
#ifdef _WIN64
    HINSTANCE hInst = (HINSTANCE) GetWindowLongPtr( hDlg, GWLP_HINSTANCE );
#else
    HINSTANCE hInst = (HINSTANCE) GetWindowLong( hDlg, GWL_HINSTANCE );
#endif
    HICON hIcon = LoadIcon( hInst, MAKEINTRESOURCE( IDR_MAINFRAME ) );

    // Set the icon for this dialog.
    SendMessage( hDlg, WM_SETICON, ICON_BIG,   (LPARAM) hIcon );  // Set big icon
    SendMessage( hDlg, WM_SETICON, ICON_SMALL, (LPARAM) hIcon );  // Set small icon

    CheckDlgButton( hDlg, IDC_HEX, BST_CHECKED );

    SendMessage( hDlg, EM_LIMITTEXT, 20, 0 );  
}



//-----------------------------------------------------------------------------
// Name: LookupValue()
// Desc: 
//-----------------------------------------------------------------------------
VOID LookupValue( HWND hDlg )
{
    HRESULT hrErr = 0;
    TCHAR strValue[MAX_PATH];
    const TCHAR* strHRESULT;
    const TCHAR* strDescription;
    TCHAR strHRESULTCopy[MAX_PATH*2];
    int nIndex;
    int nPower = 0;
    int nPowerDec = 1;
    int nDigit = 0;
    GetDlgItemText( hDlg, IDC_VALUE, strValue, MAX_PATH );

    StringCchLength(strValue, MAX_PATH, (size_t *)&nIndex);
    nIndex--;

    // skip whitespace
    while( nIndex >= 0 )
    {
    	if( strValue[nIndex] != ' ' && 
            strValue[nIndex] != 'L' )
    	    break;
    	    
        nIndex--;
    }

    bool bFoundDigit = false;
    while( nIndex >= 0 )
    {
        // Convert to uppercase
        if( strValue[nIndex] >= 'a' && strValue[nIndex] <= 'z' )
            strValue[nIndex] += 'A' - 'a';

        if( g_bUseHex && strValue[nIndex] >= 'A' && strValue[nIndex] <= 'F' )
            nDigit = strValue[nIndex] - 'A' + 10;
        else if( strValue[nIndex] >= '0' && strValue[nIndex] <= '9' )
            nDigit = strValue[nIndex] - '0';
        else if( strValue[nIndex] == '-' )
        {
            hrErr = -hrErr;
            break;
        }
        else
        {
            // break if we've found a number, but don't break otherwise
            // This will skip any random letters in the front & end of the string
            if( bFoundDigit )
            {
                break;
            }
            else
            {
                nIndex--;
                continue;
            }
        }

        bFoundDigit = true;
        if( g_bUseHex )
            hrErr += ( nDigit << (nPower*4) );
        else
            hrErr += ( nDigit * nPowerDec );

        nPowerDec *= 10;
        nIndex--;
        nPower++;
    }

    if( hrErr == 0 && !bFoundDigit )
    {
        SetDlgItemText( hDlg, IDC_MESSAGE, TEXT("Invalid input") );
    }
    else
    {
        TCHAR strInternals[100];
        TCHAR strFacility[100];
        
        // Use dxerr.lib to lookup HRESULT.
        strHRESULT = DXGetErrorString( hrErr );
        StringCchPrintf( strHRESULTCopy, MAX_PATH*2, TEXT("HRESULT: 0x%0.8x (%lu)"), 
                    hrErr, hrErr );
        StringCchCat( strHRESULTCopy, MAX_PATH*2, TEXT("\r\nName: ") );
        StringCchCat( strHRESULTCopy, MAX_PATH*2, strHRESULT );

        strDescription = DXGetErrorDescription( hrErr );

        TCHAR* strTemp;
        while( ( strTemp = _tcschr( strHRESULTCopy, '&') ) != '\0' )
        {
            strTemp[0] = '\r';
            strTemp[1] = '\n';
        }

        if( lstrlen(strDescription) > 0 )
        {
            StringCchCat( strHRESULTCopy, MAX_PATH*2, TEXT("\r\nDescription: ") );
            StringCchCat( strHRESULTCopy, MAX_PATH*2, strDescription );
        }

        StringCchPrintf( strInternals, 100, TEXT("\r\nSeverity code: %s"), 
                    ( HRESULT_SEVERITY(hrErr) == 1 ) ? TEXT("Failed") : TEXT("Success") );
        StringCchCat( strHRESULTCopy, MAX_PATH*2, strInternals );

        int nFacility = HRESULT_FACILITY(hrErr);
        switch( nFacility )
        {
            case FACILITY_WINDOWS_CE:  StringCchCopy( strFacility, 100, TEXT("FACILITY_WINDOWS_CE") ); break;
            case FACILITY_WINDOWS: StringCchCopy( strFacility, 100, TEXT("FACILITY_WINDOWS") ); break;
            case FACILITY_URT: StringCchCopy( strFacility, 100, TEXT("FACILITY_URT") ); break;
            case FACILITY_UMI: StringCchCopy( strFacility, 100, TEXT("FACILITY_UMI") ); break;
            case FACILITY_SXS: StringCchCopy( strFacility, 100, TEXT("FACILITY_SXS") ); break;
            case FACILITY_STORAGE: StringCchCopy( strFacility, 100, TEXT("FACILITY_STORAGE") ); break;
            case FACILITY_STATE_MANAGEMENT: StringCchCopy( strFacility, 100, TEXT("FACILITY_STATE_MANAGEMENT") ); break;
//            case FACILITY_SSPI: StringCchCopy( strFacility, 100, TEXT("FACILITY_SSPI") ); break;
            case FACILITY_SCARD: StringCchCopy( strFacility, 100, TEXT("FACILITY_SCARD") ); break;
            case FACILITY_SETUPAPI: StringCchCopy( strFacility, 100, TEXT("FACILITY_SETUPAPI") ); break;
            case FACILITY_SECURITY: StringCchCopy( strFacility, 100, TEXT("FACILITY_SECURITY or FACILITY_SSPI") ); break;
            case FACILITY_RPC: StringCchCopy( strFacility, 100, TEXT("FACILITY_RPC") ); break;
            case FACILITY_WIN32: StringCchCopy( strFacility, 100, TEXT("FACILITY_WIN32") ); break;
            case FACILITY_CONTROL: StringCchCopy( strFacility, 100, TEXT("FACILITY_CONTROL") ); break;
            case FACILITY_NULL: StringCchCopy( strFacility, 100, TEXT("FACILITY_NULL") ); break;
            case FACILITY_MSMQ: StringCchCopy( strFacility, 100, TEXT("FACILITY_MSMQ") ); break;
            case FACILITY_MEDIASERVER: StringCchCopy( strFacility, 100, TEXT("FACILITY_MEDIASERVER") ); break;
            case FACILITY_INTERNET: StringCchCopy( strFacility, 100, TEXT("FACILITY_INTERNET") ); break;
            case FACILITY_ITF: StringCchCopy( strFacility, 100, TEXT("FACILITY_ITF") ); break;
            case FACILITY_HTTP: StringCchCopy( strFacility, 100, TEXT("FACILITY_HTTP") ); break;
            case FACILITY_DPLAY: StringCchCopy( strFacility, 100, TEXT("FACILITY_DPLAY") ); break;
            case FACILITY_DISPATCH: StringCchCopy( strFacility, 100, TEXT("FACILITY_DISPATCH") ); break;
            case FACILITY_CONFIGURATION: StringCchCopy( strFacility, 100, TEXT("FACILITY_CONFIGURATION") ); break;
            case FACILITY_COMPLUS: StringCchCopy( strFacility, 100, TEXT("FACILITY_COMPLUS") ); break;
            case FACILITY_CERT: StringCchCopy( strFacility, 100, TEXT("FACILITY_CERT") ); break;
            case FACILITY_BACKGROUNDCOPY: StringCchCopy( strFacility, 100, TEXT("FACILITY_BACKGROUNDCOPY") ); break;
            case FACILITY_ACS: StringCchCopy( strFacility, 100, TEXT("FACILITY_ACS") ); break;
            case FACILITY_AAF: StringCchCopy( strFacility, 100, TEXT("FACILITY_AAF") ); break;
            case FACILITY_D3DX: StringCchCopy( strFacility, 100, TEXT("FACILITY_D3DX") ); break;
            case FACILITY_D3D: StringCchCopy( strFacility, 100, TEXT("FACILITY_D3D") ); break;
            default: StringCchCopy( strFacility, 100, TEXT("Unknown") ); break;
        }
        StringCchPrintf( strInternals, 100, TEXT("\r\nFacility Code: %s (%d)"), 
                    strFacility, HRESULT_FACILITY(hrErr) );
        StringCchCat( strHRESULTCopy, MAX_PATH*2, strInternals );

        StringCchPrintf( strInternals, 100, TEXT("\r\nError Code: 0x%0.4x (%lu)"), 
                    HRESULT_CODE(hrErr), HRESULT_CODE(hrErr) );
        StringCchCat( strHRESULTCopy, MAX_PATH*2, strInternals );

        SetDlgItemText( hDlg, IDC_MESSAGE, strHRESULTCopy );
    }

    return;
}
