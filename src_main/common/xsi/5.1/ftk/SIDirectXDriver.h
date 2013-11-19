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

#ifndef _DIRECTXDRIVER_H_
#define _DIRECTXDRIVER_H_


#include <windows.h>
#include <d3d9.h>
#include <D3dx9math.h>

#include <SIBCUtil.h>
#include <SIDisplayDriver.h>


#include	<LIST>
typedef		std::list<IDirect3DStateBlock9*>		SI_DXAttribStack;
typedef		SI_DXAttribStack::iterator	SI_DXAttribStackIter;

#ifndef RTSHADER_TYPES_H
typedef struct tagSID3DContext
{
	LPDIRECT3DDEVICE9		m_pDevice;
	LONG					m_lFlushTextures;
    D3DPRESENT_PARAMETERS	m_d3dpp;
} SID3DContext;
#endif RTSHADER_TYPES_H

//typedef std::list< DWORD >	SIDXAttribStack;
/** 
* DirectX Display Driver
* \sa CSIDisplayDriver 
*/

#ifndef SIDIRECTXDRIVEREXPORT
# ifdef XSI_STATIC_LINK
#  define SIDIRECTXDRIVEREXPORT
# else
#  ifdef SIDIRECTXDRIVEREXPORT_IMPL
#   define SIDIRECTXDRIVEREXPORT __declspec(dllexport)
#  else
#   define SIDIRECTXDRIVEREXPORT __declspec(dllimport)
#  endif
# endif
#endif

class SIDIRECTXDRIVEREXPORT CSIDirectXDriver : public CSIDisplayDriver
{
public:
	CSIDirectXDriver();
	~CSIDirectXDriver();

	virtual	SI_Error	Initialize ( void *in_pData, int in_iWidth = DEFAULT_DISPLAY_WIDTH, int in_iHeight = DEFAULT_DISPLAY_WIDTH,
									 void *in_pParam1 = NULL, void *in_pParam2 = NULL, void *in_pParentContext = NULL );
	virtual SI_Error	Shutdown ();

	
	virtual SI_Error	SetSize ( int in_iWidth, int in_iHeight );

	virtual SI_Error	BeginFrame ();
	virtual SI_Error	EndFrame();


	virtual SI_Error	SetTransform ( CSIBCMatrix4x4& );
	virtual SI_Error	UnSetTransform ();
	virtual SI_Error	SetTextureTransform ( CSIBCMatrix4x4& );
	virtual SI_Error	UnSetTextureTransform ();

	virtual SI_Error	SetProjection (CSIBCMatrix4x4& );
	virtual SI_Error	SetProjection (	SI_Float in_fNearPlane, SI_Float in_fFarPlane,
										SI_Float in_fFOV, SI_Float in_fAspect);
	virtual SI_Error	SetView(CSIBCMatrix4x4& in_mView );

	virtual SI_Error	PushAttributes();
	virtual SI_Error	PopAttributes();

	virtual SI_Error	SetAmbientColor( CSIBCColorf& m_cAmbientColor );
	
	virtual SI_Void	*GetDeviceContext() { return (SI_Void *)&m_DeviceContext; };

	virtual SI_Error	SetLight ( CSLLight * in_pLight );

private:

	LPDIRECT3D9             m_pD3D;
	SID3DContext			m_DeviceContext;
	D3DXMATRIX				m_mPreviousMatrix;
	D3DXMATRIX				m_mPreviousTextureMatrix;
	
	SI_DXAttribStack		m_sAttribStack;

	SI_Void					*m_pUnused;
	SI_Int					m_iCurrentLight;
	SI_Int					m_iMaxLights;

private:

	SI_Error				ClearAttribStack();
	

};

#ifdef __cplusplus
extern "C"
{
	XSIEXPORT CSIDisplayDriver	*RegisterDisplayDriver();


}
#endif // __cplusplus


#endif
