//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// Uninstall.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Uninstall.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CUninstallApp



BEGIN_MESSAGE_MAP(CUninstallApp, CWinApp)
	//{{AFX_MSG_MAP(CUninstallApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUninstallApp construction

CUninstallApp::CUninstallApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance


}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUninstallApp object

CUninstallApp theApp;

BOOL CUninstallApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
#ifdef _OEM
	m_szGameName = "Half-Life: Day One";
#pragma message ("OEM")
	m_szSimpleGameName = "Half-Life Day One";
#else
	m_szGameName = "Half-Life";
#pragma message ("Not OEM")
	m_szSimpleGameName = "Half-Life";
#endif

	return CWinApp::InitInstance();
}


void CUninstallApp::DoPreUninstallStuff()
{
	//just show an informative dialog box in this step.
	CString SaveGameMsg;
	SaveGameMsg.LoadString(IDS_SAVEMSG);
	CString SaveGameTitle;
	SaveGameTitle.LoadString(IDS_SAVETITLE);
	MessageBox(NULL,SaveGameMsg,SaveGameTitle,MB_OK);

	
}

void CUninstallApp::DoPostUninstallStuff()
{
#ifdef _DEBUG
	AfxMessageBox("starting post uninstall process");
#endif
	DelURLValue();		//clean up the URL key we used when we installed the game, (sometimes it doesn't get wiped).
	DelHLKeyContentsInCurrentUser();	 //remove all the stuff that the game put there.
	DelHLKeyContentsInLocalMachine(); //clean up after InstallShield. Make sure everything gets del'd correctly
	DelValveKeysIfEmpty();	//if HL was the only Valve software on the machine, remove the Valve Key
#ifdef _DEBUG
	AfxMessageBox("finished post uninstall process");
#endif
}


#define REG_VALVE_KEY "SOFTWARE\\Valve"
#define REG_HALFLIFE_KEY ((REG_VALVE_KEY "\\") + m_szSimpleGameName)




void CUninstallApp::DelHLKeyContentsInLocalMachine()
{
	//this func is mainly just a clean-up style function
	//after HL is uninstalled, this just wipes all the HL 
	//related stuff in HKEY_LOCAL_MACHINE
	//InstallShield should wipe it, but it's kind of flaky
	//as to whether or not it will actually do it sometimes.
	
	HKEY hkwipe;
	LONG lResult;
	lResult=RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_HALFLIFE_KEY,0,KEY_ALL_ACCESS,&hkwipe);
	if (lResult==ERROR_SUCCESS)
	{
#ifdef _DEBUG
		AfxMessageBox("starting recursive del on HKCU");		
#endif
		RecursiveRegDelKey(hkwipe);
		RegCloseKey(hkwipe);
	}
#ifdef _DEBUG
	else 
	{

		AfxMessageBox(CString("failed trying open: HKLM ")+CString(REG_HALFLIFE_KEY));
	}
#endif

	lResult=RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_VALVE_KEY,0,KEY_ALL_ACCESS,&hkwipe);
	if (lResult==ERROR_SUCCESS)
	{
		RegDeleteKey(hkwipe,m_szSimpleGameName);
		RegCloseKey(hkwipe);
	}
#ifdef _DEBUG
	else 
	{
		AfxMessageBox(CString("failed trying open: HKLM ")+CString(REG_VALVE_KEY));
	}
#endif
}


void CUninstallApp::RecursiveRegDelKey(HKEY hKey)
{
	char keyname[256]={0};
	DWORD namesize=256;

	//base case: no subkeys when RegEnumKeyEx returns error on index 0
	LONG lResult=RegEnumKeyEx(hKey,0,keyname,&namesize,NULL,NULL,NULL,NULL);
	if (lResult!=ERROR_SUCCESS)
	{
#ifdef _DEBUG
		AfxMessageBox("RecursiveRegDelKey found no keys");
#endif
		return;
	}

	DWORD i=1;
	do
	{
		HKEY subkey;
		LONG lResult2;
		LONG lDelResult;
		lResult2=RegOpenKeyEx(hKey,keyname,0,KEY_ALL_ACCESS,&subkey);
		
		if (lResult2==ERROR_SUCCESS)
		{
#ifdef _DEBUG
			AfxMessageBox("RRDK going to " + CString(keyname));
#endif
			RecursiveRegDelKey(subkey);

			RegCloseKey(subkey);
			lDelResult=RegDeleteKey(hKey,keyname);
			if (lDelResult!=ERROR_SUCCESS)
			{
#ifdef _DEBUG
				AfxMessageBox(CString("could not delete ") + CString(keyname));
#endif
			}
			namesize=256;
			//use 0 in the next function call because when you delete one, the rest shift down!
			lResult=RegEnumKeyEx(hKey,0,keyname,&namesize,NULL,NULL,NULL,NULL);
		}

		else 
		{
#ifdef _DEBUG
			AfxMessageBox("failed trying open: " + CString(keyname));
#endif
			break;
		}

	} while (lResult!=ERROR_NO_MORE_ITEMS);
	
}

void CUninstallApp::DelHLKeyContentsInCurrentUser()
{
	LONG lResult;
	//note, in HKEY_CURRENT_USER Half-Life Day One is never used. Only Half-Life.
	HKEY hkwipe;
	
	
	lResult = RegOpenKeyEx(HKEY_CURRENT_USER,CString(REG_VALVE_KEY) + "\\Half-Life",0,KEY_ALL_ACCESS,&hkwipe);
	if (lResult==ERROR_SUCCESS)
	{
#ifdef _DEBUG
		AfxMessageBox("starting recursive del on HKCU");
#endif
		RecursiveRegDelKey(hkwipe);
		RegCloseKey(hkwipe);
	}
#ifdef _DEBUG
	else 
	{

		AfxMessageBox(CString("failed trying open: HKCU ")+CString(REG_VALVE_KEY) + "\\Half-Life" );
	}
#endif

	lResult=RegOpenKeyEx(HKEY_CURRENT_USER,REG_VALVE_KEY,0,KEY_ALL_ACCESS,&hkwipe);
	if (lResult==ERROR_SUCCESS)
	{
		lResult=RegDeleteKey(hkwipe,"Half-Life");
#ifdef _DEBUG
		if (lResult!=ERROR_SUCCESS)
		{
			AfxMessageBox("failed trying DELETE: HKCU " REG_VALVE_KEY "\\Half-Life");
		}
#endif

		RegCloseKey(hkwipe);
	}
#ifdef _DEBUG
	else 
	{
		AfxMessageBox("failed trying open: HKCU " REG_VALVE_KEY );
	}
#endif
}


void CUninstallApp::DelURLValue()
{
	HKEY hkwipe;
	LONG lResult;
	
	lResult=RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_VALVE_KEY,0,KEY_ALL_ACCESS,&hkwipe);
	if (lResult==ERROR_SUCCESS)
	{
		lResult=RegDeleteValue(hkwipe,"URL");
		RegCloseKey(hkwipe);
	}
#ifdef _DEBUG
	else
	{
		AfxMessageBox("couldn't open URL value to delete");
	}
#endif	
}

void CUninstallApp::DeleteSelf()
{
	//useless for now. :I
	//supposed to delete the uninstall dll
}

void CUninstallApp::DelValveKeysIfEmpty()
{
	LONG lResult;
	HKEY hKey;
	
	char keyname[256]={0};
	DWORD namesize=256;
	
	//do HKEY_LOCAL_MACHINE first
	lResult=RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_VALVE_KEY,0,KEY_ALL_ACCESS,&hKey);
	if (lResult==ERROR_SUCCESS)
	{
		lResult=RegEnumKeyEx(hKey,0,keyname,&namesize,NULL,NULL,NULL,NULL);
		if (lResult==ERROR_NO_MORE_ITEMS)
		{
			RegCloseKey(hKey);
			lResult=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE",0,KEY_ALL_ACCESS,&hKey);
			if (lResult==ERROR_SUCCESS)
			{
				RegDeleteKey(hKey,"Valve");
				RegCloseKey(hKey);
			}
#ifdef _DEBUG
			else 
			{
				AfxMessageBox("Couldn't open HKLM\\Software");
			}
#endif
		}
#ifdef _DEBUG
		else 
		{
			AfxMessageBox("found something in HKLM\\software\\valve, not deleting");
		}
#endif
	}
#ifdef _DEBUG
	else 
	{
		AfxMessageBox(CString("failed trying open: HKLM ")+CString(REG_VALVE_KEY));
	}
#endif

	//now do HKEY_CURRENT_USER
	lResult=RegOpenKeyEx(HKEY_CURRENT_USER,REG_VALVE_KEY,0,KEY_ALL_ACCESS,&hKey);
	if (lResult==ERROR_SUCCESS)
	{
		lResult=RegEnumKeyEx(hKey,0,keyname,&namesize,NULL,NULL,NULL,NULL);
		if (lResult==ERROR_NO_MORE_ITEMS)
		{
			RegCloseKey(hKey);
			lResult=RegOpenKeyEx(HKEY_CURRENT_USER,"SOFTWARE",0,KEY_ALL_ACCESS,&hKey);
			if (lResult==ERROR_SUCCESS)
			{
				RegDeleteKey(hKey,"Valve");
				RegCloseKey(hKey);
			}
#ifdef _DEBUG
			else 
			{
				AfxMessageBox("Couldn't open HKCU\\Software");
			}
#endif
		}
#ifdef _DEBUG
		else 
		{
			AfxMessageBox("found something in HKCU\\software\\valve, not deleting");
		}
#endif
	}
#ifdef _DEBUG
	else 
	{
		AfxMessageBox(CString("failed trying open: HKCU ")+CString(REG_VALVE_KEY));
	}
#endif
}