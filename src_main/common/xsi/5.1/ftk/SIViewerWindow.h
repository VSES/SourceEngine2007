/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

#ifndef _SIWINDOWVIEWER_H_
#define _SIWINDOWVIEWER_H_

#include <windows.h>
#include <SIBCUtil.h>

SI_Error	InitializeViewerFramework
(
	HINSTANCE	hInstance, 
	HINSTANCE	hPrevInstance, 
	LPTSTR		lpCmdLine,
	int			nCmdShow
);

SI_Error	ShutdownViewerFramework ();

int APIENTRY MainWindowLoop	
(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow
);

LRESULT CALLBACK ViewerWindowProc
(
    HWND    hWindow,
    UINT    uiMsg,
    WPARAM  wParam,
    LPARAM  lParam
);


#endif
