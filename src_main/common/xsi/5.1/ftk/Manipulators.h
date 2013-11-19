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

#ifndef _SI_MANIPULATORS_
#define _SI_MANIPULATORS_

#include <SIBCUtil.h>
#include "SIViewerApplicationWin32.h"

#ifdef __cplusplus
extern "C"
{

XSIEXPORT SI_Error	SI_AcquireInputData
(
	CSIViewerApplicationWin32	*in_pWin32Application, 
	HWND						hWindow,
	UINT						uiMsg,
	WPARAM						wParam,
	LPARAM						lParam
);

XSIEXPORT SI_Error	SI_DrawGrid( CSIViewerApplication *in_pApplication);
XSIEXPORT SI_Error	SI_DrawFPS( CSIViewerApplication *in_pApplication);
XSIEXPORT SI_Error	SI_ManipulateCamera( CSIViewerApplication *in_pApplication);

}
#endif // __cplusplus
#endif // _SI_MANIPULATORS_