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

#ifndef _DISPLAYDRIVER_H_
#define _DISPLAYDRIVER_H_

#include <SIBCUtil.h>
#include <SIBCNode.h>
#include <SIBCMatrix.h>
#include <SIBCColor.h>
#include <Light.h>
#include <Fog.h>

enum EMaterialCapability
{
	TRANSPARENCY
};

typedef void				(*Shader_Init)(void *);
typedef void				(*Shader_Exit)(void *);
typedef void				(*Shader_Eval)(int, unsigned char *, void *, void *); 
typedef void				(*Shader_Requirement)(void *, void *, int, int*); 
typedef int					(*GetNbSupportedShadersFunc)();
typedef void				(*GetSupportedShadersFunc)(char **in_pchSupportedShaders);
typedef Shader_Init			(*GetShaderInitFunc)(char *in_pchShaderName);
typedef Shader_Eval			(*GetShaderEvalFunc)(char *in_pchShaderName);
typedef Shader_Exit			(*GetShaderExitFunc)(char *in_pchShaderName);
typedef Shader_Requirement	(*GetShaderReqFunc)(char *in_pchShaderName);


#define RIGHT	0
#define LEFT	1
#define BOTTOM	2
#define TOP		3
#define	FFAR	4
#define FNEAR	5

#define SI_MAX_DRV_NAME_LENGTH 4096

#define DEFAULT_DISPLAY_WIDTH	800
#define DEFAULT_DISPLAY_HEIGHT	600

// Display Driver API Type Used
#define DISPLAYAPI_CUSTOM		0x00000000
#define DISPLAYAPI_OGL			0x00000001
#define DISPLAYAPI_D3D			0x00000002

#define DISPLAYAPI_ISVALID( in_nApiType ) \
	( in_nApiType == DISPLAYAPI_CUSTOM || in_nApiType == DISPLAYAPI_OGL || in_nApiType == DISPLAYAPI_D3D )

/** 
* Display Driver base class
* This is the base class from which all hardware display drivers derive from. A display driver is 
* responsible for the following tasks:
*
* - Initialize and Shutdown the driver
*	- <I> For instance, an OpenGL display driver would choose and set a pixel format, create the rendering context and make it current. </I>
* - Setting the initial drawing states at each frame
*	- <I> Typically, set the background color, clear the screen and zbuffer etc. </I>
* - "Swapping" or blitting the buffer to the screen
* - Updating the various transforms (projection, texture, world etc.)
* - Saving and restoring hardware attributes
*	- <I> This would be glPushAttrib, glPopAttrib </I>
* - Setting the ambient color
* - Setting lights and their parameters
* 
*/
class XSIEXPORT CSIDisplayDriver : public CSIBCNode
{
private:
	int					m_lWidth, m_lHeight;
	SI_Void				*m_pUnused;

protected:
	CSIBCColorf			m_cBackgroundColor;
	CSIBCColorf			m_cAmbientColor;
	
	CSIBCMatrix4x4		m_mCurrentProjectionMatrix;
	CSIBCMatrix4x4		m_mCurrentViewMatrix;
	CSIBCVector4D		m_vFrustrumPlane[6];

	CSLFog				*m_pFog;

	SI_Char				m_szName[SI_MAX_DRV_NAME_LENGTH];

	SI_Int				m_nApiType;

public:
	CSIDisplayDriver();
	virtual ~CSIDisplayDriver();

	//! Initialize is where the hardware and graphics API such as OpenGL or DirectX are initialized.
	/*! 
	\param	void*		Data passed by the application object. For the XSI Viewer running on Windows (CSIViewerApplicationWin32), this points to a HWND
	\return SI_Error	SI_SUCCESS, the driver was initialized, otherwise an error code is returned.
	*/
	virtual SI_Error	Initialize ( void *in_pData, int in_iWidth = DEFAULT_DISPLAY_WIDTH, int in_iHeight = DEFAULT_DISPLAY_WIDTH,
									 void *in_pParam1 = NULL, void *in_pParam2 = NULL, void *in_pParentContext = NULL );

	//! Shutdown the hardware and graphics API, release memory etc. This is called when the Application is destroyed.
	/*! 
	\return SI_Error	SI_SUCCESS, the driver was shutdown successfully, otherwise an error code is returned.
	*/
	virtual SI_Error	Shutdown ();

	//! SetName, sets the name of a specific display driver. This uniquely identifies a display driver and will appear
	//! in the display driver selection list of the Configuration application. 
	/*! 
	\param	SI_Char*	Display Driver Name
	\return SI_Error	Error code.
	*/
	virtual SI_Error	SetName ( SI_Char* in_szName );

	//! Gets the name of the display dirver. 
	/*! 
	\return SI_Char*	Name of the display dirver.
	*/
	virtual SI_Char*	GetName ();

	//! Sets the Api Type used by the display driver.
	/*! 
	\param	SI_Int		Display Driver Api Type
	\return SI_Error	Error code.
	*/
	virtual SI_Error	SetApiType ( SI_Int in_nApiType );

	//! Gets the Api Type used by the display dirver. 
	/*! 
	\return SI_Int		Api Type of the display dirver.
	*/
	virtual SI_Int		GetApiType ();

	//! SetSize is called when the application window is resized (or initialy created). 
	/*! 
	\param	SI_Int		Screen width
	\param	SI_Int		Screen height
	\return SI_Error	Error code.
	*/
	virtual SI_Error	SetSize ( int in_iWidth, int in_iHeight );

	//! GetSize can be called to retrieve the size of the application window. 
	/*! 
	\param	SI_Int&		Screen width
	\param	SI_Int&		Screen height
	\return SI_Error	Error code.
	*/
	virtual SI_Error	GetSize ( int &out_iWidth, int &out_iHeight );

	//! BeginFrame is called at the begining of each frame before the engine is evaluated. This is typically where
	//! the screen and z-buffer gets cleared and where hardware states are set.
	/*! 
	\return SI_Error	Error code.
	*/
	virtual SI_Error	BeginFrame ();

	//! EndFrame is called at the end of each frame after the engine is evaluated. This is typically where
	//! the buffer is swapped to the screen.
	/*! 
	\return SI_Error	Error code.
	*/
	virtual SI_Error	EndFrame();

	//! Sets the new world transform matrix
	/*! 
	\param	CSIBCMatrix4x4&		Matrix that represents the new world transform
	\return SI_Error	Error code.
	*/
	virtual SI_Error	SetTransform ( CSIBCMatrix4x4& in_mNewTransform );

	//! Restores the previous world matrix
	/*! 
	\return SI_Error	Error code.
	*/
	virtual SI_Error	UnSetTransform ();

	//! Sets the new texture transform matrix
	/*! 
	\param	CSIBCMatrix4x4&		Matrix that represents the new texture transform
	\return SI_Error	Error code.
	*/
	virtual	SI_Error	SetTextureTransform ( CSIBCMatrix4x4& );
	
	//! Restores the previous texture matrix
	/*! 
	\return SI_Error	Error code.
	*/
	virtual	SI_Error	UnSetTextureTransform ();

	//! Sets a projection matrix
	/*! 
	\param	CSIBCMatrix4x4&		Matrix that represents the projection matrix
	\return SI_Error	Error code.
	*/
	virtual SI_Error	SetProjection (CSIBCMatrix4x4& in_mNewProjection );

	//! Sets a projection transform
	/*! 
	\param	SI_Float	Near plane
	\param	SI_Float	Far plane
	\param	SI_Float	FOV (in radians)
	\param	SI_Float	Aspect ratio
	\return SI_Error	Error code.
	*/
	virtual SI_Error	SetProjection (	SI_Float in_fNearPlane, SI_Float in_fFarPlane,
										SI_Float in_fFOV, SI_Float in_fAspect);

	//! Sets the view matrix
	/*! 
	\param	CSIBCMatrix4x4&		Matrix that represents the view matrix
	\return SI_Error	Error code.
	*/
	virtual SI_Error	SetView (CSIBCMatrix4x4& in_mNewView );

	//! Saves the hardware attributes or render states
	/*! 
	\return SI_Error	Error code.
	*/
	virtual SI_Error	PushAttributes();

	//! Restores the hardware attributes or render states
	/*! 
	\return SI_Error	Error code.
	*/
	virtual SI_Error	PopAttributes();

	//! Sets the background color
	/*! 
	\param	CSIBCColorf&		CSIBCColorf holding the new background color
	\return SI_Error	Error code.
	*/
	virtual SI_Error		SetBackgroundColor( CSIBCColorf& m_cBackgroundColor);

	//! Gets the background color
	/*! 
	\return CSIBCColorf	The background color.
	*/
	virtual CSIBCColorf&	GetBackgroundColor();

	//! Sets the scene's ambient color
	/*! 
	\param	CSIBCColorf&		CSIBCColorf holding the new ambient color
	\return SI_Error	Error code.
	*/
	virtual SI_Error		SetAmbientColor( CSIBCColorf& m_cAmbientColor);

	//! Gets the ambient color
	/*! 
	\return CSIBCColorf	The ambient color.
	*/
	virtual CSIBCColorf&	GetAmbientColor();

	//! Updates the frustrum planes. Calling this will ensure that the frustrum planes are in synch with the various transform matrices
	/*! 
	\return SI_Error	Error code
	*/

	//! Sets the scene's fog settings
	/*! 
	\param	CSLFof*		The Semantic Layer object describing the fog
	\return SI_Error	Error code.
	*/
	virtual SI_Error		SetFog( CSLFog* in_pFog );


	virtual SI_Error	CalculateFrustrumPlanes();

	//! Test the visibility of a single 3D point.
	/*! 
	\param	CSIBCVector3D&		The coordinate (point) to test
	\return SI_Bool		SI_TRUE if the point is visible, SI_FALSE if it's not.
	\note This tests the visibility against the view frustrum only and does not test for occlusion by other objects in the scene.
	*/
	virtual SI_Bool		PointInFrustrum ( CSIBCVector3D& in_pPoint );
	
	//! Test the visibility of a sphere.
	/*! 
	\param	CSIBCVector3D&		The center of the sphere
	\param	SI_Float			The sphere's radius
	\return SI_Bool				SI_TRUE if the sphere is visible (even partially), SI_FALSE if it is completly outside the view frustrum.
	\note This tests the visibility against the view frustrum only and does not test for occlusion by other objects in the scene.
	*/
	virtual SI_Float	SphereInFrustrum ( CSIBCVector3D& in_pCenter, SI_Float in_fRadius );

	//! Test the visibility of a cube.
	/*! 
	\param	CSIBCVector3D&		The center of the cube
	\param	SI_Float			The cube's extent (XYZ)
	\return SI_Bool				SI_TRUE if the cube is visible (even partially), SI_FALSE if it is completly outside the view frustrum.
	\note This tests the visibility against the view frustrum only and does not test for occlusion by other objects in the scene.
	*/
	virtual SI_Bool		CubeInFrustrum ( CSIBCVector3D& in_pCenter, SI_Float in_fExtent );

	//! Get the hardware-specific device context. 
	/*! 
	\return SI_Void*	Pointer to the device context. See below.

	\note The data returned depends on the API used. For instance, the OpenGL driver returns the handle to the Render Context (HRC) while the Direct3D driver returns a Direct3DDevice object.
	*/ 
	virtual SI_Void	*GetDeviceContext() { return NULL; };

	//! Tells the device driver to set/create a light
	/*! 
	\param	CSLLight*	The semantic layer object that describes the light
	\return SI_Error	Error code
	\sa CSLLight
	*/
	virtual SI_Error	SetLight ( CSLLight * in_pLight );

	virtual SI_Error	ReadPixels(int in_iX, int in_iY, unsigned int in_uWidth, unsigned int in_uHeight, unsigned int in_Format, unsigned int in_Type, void* out_pPixels);

	virtual SI_Error	MakeCurrent();
};


typedef CSIDisplayDriver *(*DISPLAYDRIVER_REGISTERCALLBACK)( );





#endif


