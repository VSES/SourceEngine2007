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

#ifndef _XSIVIEWERRENDERINGTARGETMANAGER_
#define _XSIVIEWERRENDERINGTARGETMANAGER_

#include "XSIViewerRenderingTarget.h"

class XSIEXPORT XSIViewerRenderingTargetManager
{
public:
	XSIViewerRenderingTargetManager();
	~XSIViewerRenderingTargetManager();

	void Add( XSIViewerRenderingTarget* in_pRenderingTarget );
	void Remove( XSIViewerRenderingTarget* in_pRenderingTarget );
	SI_Error MakeCurrent( XSIViewerRenderingTarget* in_pRenderingTarget );

	int GetIndex( XSIViewerRenderingTarget* in_pRenderingTarget );
	XSIViewerRenderingTarget* GetCurrent();
	int GetCount();
	XSIViewerRenderingTarget* Get( int in_iIndex );

private:

	XSIViewerRenderingTarget* m_pCurrentRenderingTarget;

	CSIBCArray <XSIViewerRenderingTarget*> m_XSIViewerRenderingTargetArray;
};

#endif // _XSIVIEWERRENDERINGTARGETMANAGER_
