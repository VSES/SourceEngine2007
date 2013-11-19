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

#ifndef _OPENGLDRIVER_H_
#define _OPENGLDRIVER_H_


#if !defined(UNIX) || defined(MAINWIN)
#   include <windows.h>
#   include <gl/gl.h>
#else
#   include <GL/gl.h>
#   include <GL/glx.h>
#   include <gtk/gtk.h>
#   include <gdk/gdkx.h>
#endif

#include <SIBCUtil.h>
#include <SIDisplayDriver.h>

#if !defined(MAINWIN)
#ifdef UNIX

/*! This is the structure that ::Initialize expects.
*  */
typedef struct tagOpenGLDriverInitialize
{
          Display *    m_xdDisplay;
            GtkWidget *  m_gtkWidget;

} OpenGLDriverInitialize;

#endif /* UNIX */
#endif /* MAINWIN */

/** 
* OpenGL Display Driver
* \sa CSIDisplayDriver 
*/

#ifndef SIOPENGLDRIVEREXPORT
# ifdef XSI_STATIC_LINK
#  define SIOPENGLDRIVEREXPORT
# else
#  ifdef SIOPENGLDRIVEREXPORT_IMPL
#   define SIOPENGLDRIVEREXPORT __declspec(dllexport)
#  else
#   define SIOPENGLDRIVEREXPORT __declspec(dllimport)
#  endif
# endif
#endif

class SIOPENGLDRIVEREXPORT CSIOpenGLDriver : public CSIDisplayDriver
{
public:
	CSIOpenGLDriver();
	~CSIOpenGLDriver();

	//
	// Initialization
	//

	virtual	SI_Error	Initialize ( void *in_pData, int in_iWidth = DEFAULT_DISPLAY_WIDTH, int in_iHeight = DEFAULT_DISPLAY_WIDTH,
									 void *in_pParam1 = NULL, void *in_pParam2 = NULL, void *in_pParentContext = NULL);
	virtual	SI_Error	Shutdown ();

	//
	// Viewport
	//
	
	virtual	SI_Error	SetSize ( int, int ) ;

	//
	// Frame
	//

	virtual	SI_Error	BeginFrame ();
	virtual	SI_Error	EndFrame();

	//
	// Transforms
	//

	virtual	SI_Error	SetTransform ( CSIBCMatrix4x4& );
	virtual	SI_Error	UnSetTransform ();

	virtual	SI_Error	SetTextureTransform ( CSIBCMatrix4x4& );
	virtual	SI_Error	UnSetTextureTransform ();

	virtual	SI_Error	SetProjection (CSIBCMatrix4x4& );
	virtual SI_Error	SetProjection (	SI_Float in_fNearPlane, SI_Float in_fFarPlane,
										SI_Float in_fFOV, SI_Float in_fAspect);
	virtual SI_Error	SetView (CSIBCMatrix4x4& in_mNewView );

	virtual	SI_Error	PushAttributes();
	virtual	SI_Error	PopAttributes();

	SI_Error	SetBackgroundColor	( CSIBCColorf& m_cBackgroundColor );
	
	SI_Error	SetAmbientColor		( CSIBCColorf& m_cAmbientColor);
	
	virtual SI_Void	*GetDeviceContext() 
    { 
#if !defined(UNIX) || defined(MAINWIN)
            return (SI_Void *)m_hRC;
#else
            return (SI_Void *)m_glxDC;
#endif
    }

	//
	// Lighting
	//

	virtual SI_Error	SetLight ( CSLLight * in_pLight );

	virtual SI_Error	MakeCurrent();

private:

#if !defined(UNIX) || defined(MAINWIN)
	HDC			m_hDC;
	HGLRC		m_hRC;
	SI_Void		*m_pUnused;
#else
    Display *       m_xdDisplay;
    GtkWidget *     m_gtkRenderWidget;
    GdkVisual *     m_gdkVisual;
    GLXContext      m_glxDC;

    SI_Int          m_nErrorBase;
    SI_Int          m_nEventBase;
#endif

	SI_Int		m_iCurrentLight;
	SI_Int		m_iMaxLights;

};

#ifdef __cplusplus
extern "C"
{
	XSIEXPORT CSIDisplayDriver	*RegisterDisplayDriver();


}
#endif // __cplusplus


#endif
