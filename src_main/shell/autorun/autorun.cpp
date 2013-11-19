//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// Autorun.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "..\\netgame\\dibapi.h"
#include "..\\netgame\\util.h"
#include "Autorun.h"
#include "AutorunDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern void COM_InitFilesystem (void);
extern void COM_ShutdownFileSystem(void);
//extern pack_t COM_LoadPackFile(char*);
CAutorunApp *g_pApp = NULL;

void Con_Printf(char *fmt, ...)
{
}
void __cdecl L_Con_Printf(char *,...)
{
}
/////////////////////////////////////////////////////////////////////////////
// CAutorunApp

BEGIN_MESSAGE_MAP(CAutorunApp, CWinApp)
	//{{AFX_MSG_MAP(CAutorunApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutorunApp construction

CAutorunApp::CAutorunApp()
:bButtonsLoaded(true)
{
	bHLInstalled = false;
	bSetupExeExists = false;


	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAutorunApp object

CAutorunApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAutorunApp initialization

BOOL CAutorunApp::InitInstance()
{
	g_pApp = this;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Limit to one instance only...
	m_pMutex = new CMutex(FALSE, "HALFLIFEAUTORUN");
	if (!m_pMutex || !m_pMutex->Lock(0))
		return 0;		//application already running

	//now make sure game isn't running
	if (FindWindow("HalfLifeLauncher",NULL))
		return 0;		//if the game is running, don't pop up autorun.



	CheckOS();			//Get Info on the OS
	CheckRegistry();	//get info from registry, such as if HL has been installed, and DX,etc.

	CheckDirectories();	//determine if a) DX installer exists, and b) setup.exe exists
	
	/////				
	//NOTE: DX stuff removed, as DX install was moved back into the Setup
	/////

#ifdef _OEM
	strGameName.LoadString(IDS_AUTORUN_OEM_GAME_NAME);
#else
	strGameName.LoadString(IDS_AUTORUN_GAME_NAME);
#endif

		
	char szFileName[MAX_PATH];
	//sprintf(szFileName, "%s%s.bmp", LAUNCHER_GFX, STR_BACKGROUND_DIB_NAME);
	sprintf(szFileName, "%s.bmp", STR_BACKGROUND_DIB_NAME);
	hDib = ReadDIBFile(szFileName);

	CAutorunDlg dlg;
	
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

CAutorunApp::~CAutorunApp()
{
	COM_ShutdownFileSystem();
}

#define PERMUTE1(a,b) (a > b ? (a) : (b >> 1))
#define PERMUTE2(a,b) (a > b ? (a) : (b))
#define PERMUTE3(a,b) (a | b)
#define PERMUTE4(a,b) (a > b ? (a) : (b >> 2))                      // Disabled button.

void CAutorunApp::NewMerge(
	CRect& r, 
	CDC *pSrcDC, 
	CDC *pDestDC, 
	CBitmap *pSrcBitmap, 
	CBitmap *pDestBitmap,
	int nPermutation /* = 1*/,
	int nAdjust /* = 0*/)
{
	BITMAP BmpObject[2];	
	CDC *pDC[2];
	CBitmap *pBm[2];

	// 0 = Dest 1 = SRC
	pDC[0] = pDestDC;
	pDC[1] = pSrcDC;
	pBm[0] = pDestBitmap;
	pBm[1] = pSrcBitmap;

	int i;
	BITMAPINFO BmInfo[2];
	LPBITMAPINFOHEADER pBmHeader[2];
	int w, h;
	int nResult;
	long lSize[2];
	int realW[2];

	unsigned char *pBitData[2];
	unsigned char *pOutData;
	
	for (i = 0; i < 2; i++)
	{
		pBm[i]->GetObject(sizeof(BITMAP),&BmpObject[i]);

		
		w = BmpObject[i].bmWidth;
		h = BmpObject[i].bmHeight;

		memset(&BmInfo[i], 0, sizeof(BITMAPINFO));

		pBmHeader[i] = (LPBITMAPINFOHEADER)&BmInfo[i];
		pBmHeader[i]->biSize   = sizeof(BITMAPINFOHEADER);
		pBmHeader[i]->biWidth  = w;
		pBmHeader[i]->biHeight = h;
		pBmHeader[i]->biPlanes = 1;
		pBmHeader[i]->biBitCount = 24; // WANT RGB
		pBmHeader[i]->biCompression = BI_RGB;
		pBmHeader[i]->biSizeImage = 0; // RGB

		lSize[i] = ((BmInfo[i].bmiHeader.biWidth*3 + 3)&~3) * h;
		lSize[i] += sizeof(BITMAPINFOHEADER); // Necessary?

		realW[i] = (BmInfo[i].bmiHeader.biWidth*3 + 3)&~3;
		
		pBitData[i] = new unsigned char[lSize[i]];
		memset(pBitData[i], 0, lSize[i]);
			
		nResult = ::GetDIBits(
			pDC[i]->GetSafeHdc(),				// handle of device context 
			(HBITMAP)pBm[i]->GetSafeHandle(),	// handle of bitmap 
			0,									// first scan line to set in destination bitmap  
			h,									// # of scan lines to retrieve
			(LPVOID)pBitData[i],				// address of array for bitmap bits 
			(LPBITMAPINFO)&BmInfo[i],			// address of structure with bitmap data 
			DIB_RGB_COLORS 						// RGB or palette index 
		);
	}

	if (lSize[0] != lSize[1])
	{
		delete[] pBitData[0];
		delete[] pBitData[1];
		return;
	}

	pOutData = new unsigned char[lSize[0]];
	memset(pOutData, 0, lSize[0]);

	int x, y;

	unsigned char in0, in1, out;
	unsigned char *pOut, *pIn[2];

//#define PERMUTE(a,b) (a | b)
if (nPermutation == 1)
{
	for (y = 0; y < h; y++)
	{
		pOut = &pOutData[y * realW[0]];
		pIn[0] = &pBitData[0][y*realW[0]];
		pIn[1] = &pBitData[1][y*realW[1]];

		for (x = 0; x < w; x++)
		{
			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE1(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;

			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE1(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;

			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE1(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;
		}
	}
}
else if (nPermutation == 2)
{
	unsigned char *pOrigIn[2];
	for (y = 0; y < h; y++)
	{
		pOut = &pOutData[y * realW[0]];
		pOrigIn[0] = &pBitData[0][y*realW[0]];
		pOrigIn[1] = &pBitData[1][y*realW[1]];

		pIn[0] = pOrigIn[0]; //  + (3*nAdjust % realW[0]);
		pIn[1] = pOrigIn[1] + (3*nAdjust % realW[1]);
		for (x = 0; x < w; x++)
		{
			//if ((pIn[0] - pOrigIn[0]) >= realW[0])
			//pIn[0] -= realW[0];
			
			if ((pIn[1] - pOrigIn[1]) >= realW[1])
				pIn[1] -= realW[1];

			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE2(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;

			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE2(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;

			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE2(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;
		}
	}
}
else if (nPermutation == 4)
{
	unsigned char *pOrigIn[2];
	for (y = 0; y < h; y++)
	{
		pOut = &pOutData[y * realW[0]];
		pOrigIn[0] = &pBitData[0][y*realW[0]];
		pOrigIn[1] = &pBitData[1][y*realW[1]];

		pIn[0] = pOrigIn[0]; //  + (3*nAdjust % realW[0]);
		pIn[1] = pOrigIn[1] + (3*nAdjust % realW[1]);
		for (x = 0; x < w; x++)
		{
			//if ((pIn[0] - pOrigIn[0]) >= realW[0])
			//pIn[0] -= realW[0];
			
			if ((pIn[1] - pOrigIn[1]) >= realW[1])
				pIn[1] -= realW[1];

			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE4(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;

			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE4(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;

			in0 = *pIn[0];
			in1 = *pIn[1];

			out = PERMUTE4(in0,in1);

			*pOut = out;

			pOut++;
			pIn[0]++;
			pIn[1]++;
		}
	}
}
	/* Make sure to use the stretching mode best for color pictures */
	::SetStretchBltMode(pDestDC->GetSafeHdc(), COLORONCOLOR);

	nResult = ::SetDIBitsToDevice(pDestDC->GetSafeHdc(),
		0,0,
		w,h,
		0,0,
		0,h,
		pOutData,
		(LPBITMAPINFO)&BmInfo[0],     // lpBitsInfo
		DIB_RGB_COLORS);            // wUsage
		
	delete[] pBitData[0];
	delete[] pBitData[1];
	delete[] pOutData;
}

void CAutorunApp::CopyFromBackground(CDC *pDC, CRect& rcDC, CRect& rcArea)
{
	if (!hDib)
		return;

	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDib);

	PaintDIB(
		pDC->GetSafeHdc(),
		&rcDC,
		hDib,
		&rcArea,
		NULL);

	::GlobalUnlock((HGLOBAL) hDib);
}

void CAutorunApp::DrawBackground(CDC *pDC, CRect& r, CRect *srcRect /* = NULL*/)
{
	if (!hDib)
		return;

	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDib);
	CRect rcDib;
	if (!srcRect)
	{
		rcDib.top = 0;
		rcDib.left = 0;
		rcDib.bottom = DIBHeight(lpDIB);
		rcDib.right  = DIBWidth(lpDIB);
	}
	else
	{
		rcDib = *srcRect;
	}

	PaintDIB(
		pDC->GetSafeHdc(),
		&r,
		hDib,
		&rcDib,
		NULL);
	

	::GlobalUnlock((HGLOBAL) hDib);
}

void CAutorunApp::CheckRegistry()
{
	//default settings:
	bHLInstalled=false;
	
	
	// Determine if we have already installed Half-Life.
	HKEY hkSettings;
	DWORD fInstalled=0;
	DWORD fInstalledSize=sizeof(fInstalled);
	DWORD fInstallPathSize=256;
	LONG lResult;
	
	lResult=RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_AUTORUN_KEY,0,KEY_ALL_ACCESS,&hkSettings);
	if (lResult==ERROR_SUCCESS)
	{
		lResult=RegQueryValueEx(hkSettings,REG_INSTALLED_VALUE,NULL,NULL,(LPBYTE)&fInstalled,&fInstalledSize);
		RegCloseKey(hkSettings);
	}

	if (lResult==ERROR_SUCCESS)
	{
		if (fInstalled)
		{
			szInstallPath[0]='\0';
			lResult=RegOpenKeyEx(HKEY_LOCAL_MACHINE,REG_HALFLIFE_KEY,0,KEY_ALL_ACCESS,&hkSettings);
			if (lResult==ERROR_SUCCESS)
			{
				lResult=RegQueryValueEx(hkSettings,REG_INSTALLPATH_VALUE,NULL,NULL,(LPBYTE)szInstallPath,&fInstallPathSize);
				RegCloseKey(hkSettings);
			}
		}
		if (lResult==ERROR_SUCCESS)
		{
			bHLInstalled=fInstalled;
		}
	}
	else
		bHLInstalled=false;
	
	return;
}

void CAutorunApp::CheckOS()
{
	OSVERSIONINFO	vinfo;
	vinfo.dwOSVersionInfoSize = sizeof(vinfo);

	//default to win95
	bNTServicePack3=false;
	OSVersion=4;
	Platform=VER_PLATFORM_WIN32_WINDOWS;

	if (!GetVersionEx (&vinfo))
	{
		//AfxMessageBox ("Couldn't get OS info");
		return;
	}

	bNTServicePack3=!stricmp(vinfo.szCSDVersion,"Service Pack 3");
	OSVersion=vinfo.dwMajorVersion;
	Platform=vinfo.dwPlatformId;

}

void CAutorunApp::CheckDirectories()
{
	FILE* f=NULL;

	CString strSetupExe;
	strSetupExe.LoadString(IDS_AUTORUN_SETUP_EXE_NAME);
	
	f=fopen(strSetupExe,"rb");
	if (!f)
	{
		bSetupExeExists=false;
#ifdef _DEBUG		
		AfxMessageBox("Can't find " + strSetupExe + ", install buttons will not be shown.");
#endif
	}
	else
	{
		bSetupExeExists=true;	
		fclose(f);
	}

}


int CAutorunApp::ExitInstance()
{
	if (m_pMutex)
	{
		m_pMutex->Unlock();
		delete m_pMutex;
	}
	return 0;
}
