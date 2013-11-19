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

#ifndef _XSIVIEWERRENDERINGTARGET_
#define _XSIVIEWERRENDERINGTARGET_

#include "SIBCUtil.h"
#include "SIDisplayDriver.h"
#include <Scene.h>
#include "SIRuntimeEngine.h"


class CSIViewerApplication;

class XSIEXPORT XSIViewerRenderingTarget
{
public:
	XSIViewerRenderingTarget( CSIViewerApplication* in_pApp );
	virtual ~XSIViewerRenderingTarget();

	virtual SI_Error Initialize(SI_Char* in_szDriverName, SI_Void *in_pAppData,
		SI_Int in_iWidth = 800, SI_Int in_iHeight = 600, SI_Void* in_pParam1 = NULL, SI_Void* in_pParam2 = NULL, void *in_pParentContext = NULL);

	CSIDisplayDriver* GetDisplayDriver() { return m_pDisplayDriver; }
	void SetDisplayDriver(CSIDisplayDriver* in_pDisplayDriver) { m_pDisplayDriver = in_pDisplayDriver; }

	void SetDisplayDriverID( SI_Int in_iDisplayDriverID ) { m_iDisplayDriverID = in_iDisplayDriverID; }
	int GetDisplayDriverID() { return m_iDisplayDriverID; }

	virtual SI_Error MakeCurrent();

	CSIViewerApplication* GetApp() { return m_pApp; }

	CSIRuntimeEngine*	Get3DEngine() { return &m_3DEngine; }

private:
	CSIDisplayDriver*		m_pDisplayDriver;
	SI_Int					m_iDisplayDriverID;
	CSIViewerApplication*	m_pApp;

	CSIRuntimeEngine		m_3DEngine;
};

#endif // _XSIVIEWERRENDERINGTARGET_
