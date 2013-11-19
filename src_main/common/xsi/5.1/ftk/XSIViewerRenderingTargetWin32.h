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

#ifndef _XSIVIEWERRENDERINGTARGETWIN32_
#define _XSIVIEWERRENDERINGTARGETWIN32_

#include "XSIViewerRenderingTarget.h"
#include "SIViewerApplicationWin32.h"

#define RENDERINGTARGET_NAME "SOFTIMAGE® | XSI® Viewer Rendering Target"


LRESULT XSIEXPORT CALLBACK RenderingTargetWindowProc
(
	HWND    hWindow,
	UINT    uiMsg,
	WPARAM  wParam,
	LPARAM  lParam
);

class XSIEXPORT XSIViewerRenderingTargetWin32 : public XSIViewerRenderingTarget
{
public:
	XSIViewerRenderingTargetWin32(CSIViewerApplication* in_pApp);
	virtual ~XSIViewerRenderingTargetWin32();

	virtual SI_Error Initialize	(
		SI_Char* in_szDriverName,
		SI_Void *in_pAppData,
		SI_Int in_iWidth = DEFAULT_DISPLAY_WIDTH,
		SI_Int in_iHeight = DEFAULT_DISPLAY_HEIGHT,
		SI_Void* in_pParam1 = NULL,
		SI_Void* in_pParam2 = NULL,
		void *in_pParentContext = NULL,
		SI_Char* in_pWndClsName = RENDERINGTARGET_NAME
	);

	Win32Context* GetWin32Context() { return &m_WinContext; }

	virtual SI_Error MakeCurrent();

private:
	static BOOL m_bWndClsRegistered;
	Win32Context m_WinContext;
};

#endif // _XSIVIEWERRENDERINGTARGETWIN32_
