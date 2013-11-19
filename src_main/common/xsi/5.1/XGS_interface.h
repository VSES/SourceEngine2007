#ifndef _XGS_INTERFACE_H_
#define _XGS_INTERFACE_H_

#include <GL/gl.h>
#ifdef linux
#undef GL_VERSION_1_2
#undef __glext_h_
#undef GL_ARB_multitexture
#endif
#include <gl/glext.h>

#ifdef _WIN32
#include <gl/wglext.h>
#endif

#include <xsi_graphicsequencer.h>
#include <XSI_rtshaders.h>

#ifdef XSISDK
#define XGSPASSFLAG				XSI::siXGSPassFlag
#define XGSDISPLAYCALLBACK		XSI::siDisplayCallback
#define XGSUNDEFINED			XSI::siUndefined
#define XGSINSERTAFTER			XSI::siInsertAfter
#else
#define XGSPASSFLAG				int
#define XGSDISPLAYCALLBACK		int
#define XGSUNDEFINED			0
#define XGSINSERTAFTER			1
#endif

#ifdef XSISDK
#define XGSINTSTATE				XSI::siXGSInteger
#define XGSFLOATSTATE			XSI::siXGSFloat
#define XGSHANDLESTATE			XSI::siXGSHandle
#define XGSVECTOR4STATE			XSI::siXGSVector4
#define XGSMATRIX4STATE			XSI::siXGSMatrix4
#else
#define XGSINTSTATE				1
#define XGSFLOATSTATE			2
#define XGSHANDLESTATE			3
#define XGSVECTOR4STATE			4
#define XGSMATRIX4STATE			5
#endif

#ifndef XSISDK

// these are used when compiling a plugin that does not
// require the entire XSI SDK library
namespace XSI 
{
typedef   
enum siDisplayCallback
{
	siUndefined	= 0,				/*!< Undefined. */
	siPreBeginFrame	= 1,			/*!< called just before BeginFrame. */
	siBeginFrame	= 2,			/*!< called when a new frame begins. */
	siPostBeginFrame	= 3,		/*!< called after the frame has begun. */
	siPrePass	= 4,				/*!< called before each pass. */
	siPass	= 5,					/*!< XGS display pass callback. */
	siPostPass	= 6,				/*!< called after each pass. */
	siPreEndFrame	= 7,			/*!< called just before the frame ends. */
	siEndFrame	= 8,				/*!< called when the frame ends. */
	siPostEndFrame	= 9,			/*!< called after the frame has ended. */
	siRenderFrame	= 10			/*!< called after MR renders a frame. */
}	siDisplayCallback;

typedef   
enum siXGSPassFlag
{	
	siInsertBefore	= 0,			/*!< Insert before a callback. */
	siInsertAfter	= 1,			/*!< Insert after a callback. */
	siInsertBeforeAll	= 2,		/*!< Insert before all callbacks. */
	siInsertAfterAll	= 3			/*!< Insert after all callbacks. */
}	siXGSPassFlag;

typedef
enum siXGSRenderFlag
{
	siRenderDefault			= 0x00000000,	/*!< Render normally. */
	siRenderUnsorted		= 0x00000001,	/*!< Render unsorted objects. */
	siRenderSorted			= 0x00000002,	/*!< Render sorted objects. Use default sorting mechanism */
	siRenderSortedNear		= 0x00000004,	/*!< Render objects sorted by nearest point in bounding box. */
	siRenderSortedCenter	= 0x00000008,	/*!< Render objects sorted by center of bounding box. */
	siRenderSortedFar		= 0x0000000c,	/*!< Render objects sorted by farthest point of bounding box. */
	siRenderSortFront		= 0x00000010,	/*!< Sorted object are rendered from front to back (instead of the default back to front) */
}	siXGSRenderFlag;


}

#define ISIPluginHost	void
#define LPI3DOBJECT		void*
#define LPI3DLIST		void*
#endif

//! XGS WGL extensions structure
/*! The is the structure for keeping WGL extension support status
*/

typedef struct tagXGSExtensionsWGL
{
	int	 version;
	int  size;

	bool _WGL_ARB_buffer_region ;
	bool _WGL_ARB_extensions_string ;
	bool _WGL_ARB_pixel_format ;
	bool _WGL_ARB_make_current_read ;
	bool _WGL_ARB_pbuffer ;
	bool _WGL_ARB_render_texture ;
	bool _WGL_EXT_display_color_table ;
	bool _WGL_EXT_extensions_string ;
	bool _WGL_EXT_make_current_read ;
	bool _WGL_EXT_pixel_format ;
	bool _WGL_EXT_pbuffer ;
	bool _WGL_EXT_swap_control ;
	bool _WGL_EXT_depth_float ;
	bool _WGL_EXT_multisample ;
	bool _WGL_OML_sync_control ;
	bool _WGL_I3D_digital_video_control ;
	bool _WGL_I3D_gamma ;
	bool _WGL_I3D_genlock ;
	bool _WGL_I3D_image_buffer ;
	bool _WGL_I3D_swap_frame_lock ;
	bool _WGL_I3D_swap_frame_usage ;
	bool _WGL_NV_render_depth_texture ;
	bool _WGL_NV_render_texture_rectangle ;
	bool _WGL_ATI_pixel_format_float ;

} XGSExtensionsWGL;

//! XGS WGL extensions functions
/*! The is the structure for keeping WGL extension functions
*/

typedef struct tagXGSWGLExtensionsFuncs
{																				
	int	 version;
	int  size;	

	PFNWGLCREATEBUFFERREGIONARBPROC							wglCreateBufferRegionARB;							
	PFNWGLDELETEBUFFERREGIONARBPROC							wglDeleteBufferRegionARB;							
	PFNWGLSAVEBUFFERREGIONARBPROC							wglSaveBufferRegionARB;							
	PFNWGLRESTOREBUFFERREGIONARBPROC						wglRestoreBufferRegionARB;								

	PFNWGLGETEXTENSIONSSTRINGARBPROC						wglGetExtensionsStringARB;

	PFNWGLGETPIXELFORMATATTRIBIVARBPROC						wglGetPixelFormatAttribivARB;								
	PFNWGLGETPIXELFORMATATTRIBFVARBPROC						wglGetPixelFormatAttribfvARB;								
	PFNWGLCHOOSEPIXELFORMATARBPROC							wglChoosePixelFormatARB;							

	PFNWGLMAKECONTEXTCURRENTARBPROC							wglMakeContextCurrentARB;
	PFNWGLGETCURRENTREADDCARBPROC							wglGetCurrentReadDCARB;

	PFNWGLCREATEPBUFFERARBPROC								wglCreatePbufferARB;						
	PFNWGLGETPBUFFERDCARBPROC								wglGetPbufferDCARB;						
	PFNWGLRELEASEPBUFFERDCARBPROC							wglReleasePbufferDCARB;							
	PFNWGLDESTROYPBUFFERARBPROC								wglDestroyPbufferARB;						
	PFNWGLQUERYPBUFFERARBPROC								wglQueryPbufferARB;						

	PFNWGLBINDTEXIMAGEARBPROC								wglBindTexImageARB;						
	PFNWGLRELEASETEXIMAGEARBPROC							wglReleaseTexImageARB;							
	PFNWGLSETPBUFFERATTRIBARBPROC							wglSetPbufferAttribARB;							

	PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC					wglCreateDisplayColorTableEXT;									
	PFNWGLLOADDISPLAYCOLORTABLEEXTPROC						wglLoadDisplayColorTableEXT;								
	PFNWGLBINDDISPLAYCOLORTABLEEXTPROC						wglBindDisplayColorTableEXT;								
	PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC					wglDestroyDisplayColorTableEXT;									

	PFNWGLGETEXTENSIONSSTRINGEXTPROC						wglGetExtensionsStringEXT;

	PFNWGLMAKECONTEXTCURRENTEXTPROC							wglMakeContextCurrentEXT;
	PFNWGLGETCURRENTREADDCEXTPROC							wglGetCurrentReadDCEXT;

	PFNWGLCREATEPBUFFEREXTPROC								wglCreatePbufferEXT;						
	PFNWGLGETPBUFFERDCEXTPROC								wglGetPbufferDCEXT;						
	PFNWGLRELEASEPBUFFERDCEXTPROC							wglReleasePbufferDCEXT;							
	PFNWGLDESTROYPBUFFEREXTPROC								wglDestroyPbufferEXT;						
	PFNWGLQUERYPBUFFEREXTPROC								wglQueryPbufferEXT;						

	PFNWGLGETPIXELFORMATATTRIBIVEXTPROC						wglGetPixelFormatAttribivEXT;								
	PFNWGLGETPIXELFORMATATTRIBFVEXTPROC						wglGetPixelFormatAttribfvEXT;								
	PFNWGLCHOOSEPIXELFORMATEXTPROC							wglChoosePixelFormatEXT;							

	PFNWGLSWAPINTERVALEXTPROC								wglSwapIntervalEXT;						
	PFNWGLGETSWAPINTERVALEXTPROC							wglGetSwapIntervalEXT;							

	PFNWGLALLOCATEMEMORYNVPROC								wglAllocateMemoryNV;
	PFNWGLFREEMEMORYNVPROC									wglFreeMemoryNV;

	PFNWGLGETSYNCVALUESOMLPROC								wglGetSyncValuesOML;						
	PFNWGLGETMSCRATEOMLPROC									wglGetMscRateOML;					
	PFNWGLSWAPBUFFERSMSCOMLPROC								wglSwapBuffersMscOML;						
	PFNWGLSWAPLAYERBUFFERSMSCOMLPROC						wglSwapLayerBuffersMscOML;								
	PFNWGLWAITFORMSCOMLPROC									wglWaitForMscOML;					
	PFNWGLWAITFORSBCOMLPROC									wglWaitForSbcOML;					

	PFNWGLGETDIGITALVIDEOPARAMETERSI3DPROC					wglGetDigitalVideoParametersI3D;									
	PFNWGLSETDIGITALVIDEOPARAMETERSI3DPROC					wglSetDigitalVideoParametersI3D;									

	PFNWGLGETGAMMATABLEPARAMETERSI3DPROC					wglGetGammaTableParametersI3D;									
	PFNWGLSETGAMMATABLEPARAMETERSI3DPROC					wglSetGammaTableParametersI3D;									
	PFNWGLGETGAMMATABLEI3DPROC								wglGetGammaTableI3D;						
	PFNWGLSETGAMMATABLEI3DPROC								wglSetGammaTableI3D;						

	PFNWGLENABLEGENLOCKI3DPROC								wglEnableGenlockI3D;						
	PFNWGLDISABLEGENLOCKI3DPROC								wglDisableGenlockI3D;						
	PFNWGLISENABLEDGENLOCKI3DPROC							wglIsEnabledGenlockI3D;							
	PFNWGLGENLOCKSOURCEI3DPROC								wglGenlockSourceI3D;						
	PFNWGLGETGENLOCKSOURCEI3DPROC							wglGetGenlockSourceI3D;							
	PFNWGLGENLOCKSOURCEEDGEI3DPROC							wglGenlockSourceEdgeI3D;							
	PFNWGLGETGENLOCKSOURCEEDGEI3DPROC						wglGetGenlockSourceEdgeI3D;								
	PFNWGLGENLOCKSAMPLERATEI3DPROC							wglGenlockSampleRateI3D;							
	PFNWGLGETGENLOCKSAMPLERATEI3DPROC						wglGetGenlockSampleRateI3D;								
	PFNWGLGENLOCKSOURCEDELAYI3DPROC							wglGenlockSourceDelayI3D;							
	PFNWGLGETGENLOCKSOURCEDELAYI3DPROC						wglGetGenlockSourceDelayI3D;								
	PFNWGLQUERYGENLOCKMAXSOURCEDELAYI3DPROC					wglQueryGenlockMaxSourceDelayI3D;									

	PFNWGLCREATEIMAGEBUFFERI3DPROC							wglCreateImageBufferI3D;							
	PFNWGLDESTROYIMAGEBUFFERI3DPROC							wglDestroyImageBufferI3D;							
	PFNWGLASSOCIATEIMAGEBUFFEREVENTSI3DPROC					wglAssociateImageBufferEventsI3D;									
	PFNWGLRELEASEIMAGEBUFFEREVENTSI3DPROC					wglReleaseImageBufferEventsI3D;									

	PFNWGLENABLEFRAMELOCKI3DPROC							wglEnableFrameLockI3D;							
	PFNWGLDISABLEFRAMELOCKI3DPROC							wglDisableFrameLockI3D;							
	PFNWGLISENABLEDFRAMELOCKI3DPROC							wglIsEnabledFrameLockI3D;							
	PFNWGLQUERYFRAMELOCKMASTERI3DPROC						wglQueryFrameLockMasterI3D;								

	PFNWGLGETFRAMEUSAGEI3DPROC								wglGetFrameUsageI3D;						
	PFNWGLBEGINFRAMETRACKINGI3DPROC							wglBeginFrameTrackingI3D;							
	PFNWGLENDFRAMETRACKINGI3DPROC							wglEndFrameTrackingI3D;							
	PFNWGLQUERYFRAMETRACKINGI3DPROC							wglQueryFrameTrackingI3D;			
	
} XGSWGLExtensionsFuncs;


/////////////////////////////////////////////////////////////////////////////
//
// @interface IXGSOGLRenderTarget | Graphic Sequencer OpenGL render target.
//
// @base public | IUnknown
//
/////////////////////////////////////////////////////////////////////////////

#undef  INTERFACE
#define INTERFACE IXGSOGLRenderTarget

DECLARE_INTERFACE_( IXGSOGLRenderTarget, IUnknown )
{
	/////////////////////////////////////////////////////////////////////////////
	// IUnknown methods
	/////////////////////////////////////////////////////////////////////////////
	
	STDMETHOD( QueryInterface )( THIS_ REFIID, LPVOID FAR * ) PURE;
	STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
	STDMETHOD_( ULONG, Release )( THIS ) PURE;

	/////////////////////////////////////////////////////////////////////////////
	// Interface Specific Methods
	/////////////////////////////////////////////////////////////////////////////
	STDMETHOD( GetDC) (THIS_ HDC *out_hdc) PURE;
	STDMETHOD( GetOpenGLContext) (THIS_ HGLRC *out_hglrc) PURE;
	STDMETHOD( GetName ) (THIS_ TCHAR **out_pName) PURE;
} ;

/////////////////////////////////////////////////////////////////////////////
//
// @interface IXGSDisplayContextManager | Graphic Sequencer Context Manager.
//
// @base public | IUnknown
//
/////////////////////////////////////////////////////////////////////////////

#undef  INTERFACE
#define INTERFACE IXGSDisplayContextManager

DECLARE_INTERFACE_( IXGSDisplayContextManager, IUnknown )
{
	/////////////////////////////////////////////////////////////////////////////
	// IUnknown methods
	/////////////////////////////////////////////////////////////////////////////
	
	STDMETHOD( QueryInterface )( THIS_ REFIID, LPVOID FAR * ) PURE;
	STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
	STDMETHOD_( ULONG, Release )( THIS ) PURE;

	/////////////////////////////////////////////////////////////////////////////
	// Interface Specific Methods
	/////////////////////////////////////////////////////////////////////////////

	// context management
	STDMETHOD( CreateContext )( THIS_ ULONG in_pID, TCHAR *in_pRenderTargetID ) PURE ;
	STDMETHOD( SetCurrentContext )( THIS_ ULONG in_pID ) PURE ;
	STDMETHOD( FreeContext )( THIS_ ULONG in_pID ) PURE ;
	STDMETHOD( UpdateContextList )( THIS ) PURE ;
	STDMETHOD( RefreshContextList )( THIS ) PURE ;
	STDMETHOD( SetContextViewMode )( THIS_ ULONG in_pID, unsigned int in_uiViewMode, TCHAR *in_szViewModeName ) PURE;
	STDMETHOD( SetContextRenderTarget ) ( THIS_ ULONG in_contextID, TCHAR *in_rtID ) PURE;
	STDMETHOD( SetViewportMapping ) ( THIS_ ULONG, ULONG ) PURE;
	STDMETHOD( IsValidDisplayMode ) ( THIS_ TCHAR* ) PURE;
		
	// callback management
	STDMETHOD( ExecuteCallbacks )( THIS_ ULONG in_pID, XSI::siDisplayCallback in_eFrom, XSI::siDisplayCallback in_eTo ) PURE ;
	STDMETHOD( ExecutePasses	)( THIS_ ULONG in_pID ) PURE ;

	// render target management
	STDMETHOD( CreateRenderTarget )( THIS_ unsigned int in_lType, TCHAR *in_pID, void *in_pFormat, HWND in_hWnd) PURE ;
	STDMETHOD( GetRenderTargetFromContext )( THIS_ ULONG in_pID, IXGSOGLRenderTarget **out_pRenderTarget) PURE;
	STDMETHOD( GetRenderTargetByName )( THIS_ TCHAR *in_pID, IXGSOGLRenderTarget **out_pRenderTarget) PURE;

	// states management (Generic)
	STDMETHOD( GetNbStates )( THIS_ int in_lType, int *out_pNbStates) PURE;
	STDMETHOD( DeleteState )( THIS_ int in_lType, TCHAR *in_pID ) PURE;
	STDMETHOD( CreateState )( THIS_ int in_lType, TCHAR *in_pID, LPVOID in_pValuePtr, LPVOID *out_pStateHandle) PURE;
	STDMETHOD( GetStateHandle )( THIS_ int in_lType, TCHAR *in_pID, LPVOID *out_pStateHandle) PURE;
	STDMETHOD( GetStateValue )( THIS_ int in_lType, LPVOID in_lStateHandle, LPVOID out_pStateValue) PURE;
	STDMETHOD( SetStateValue )( THIS_ int in_lType, LPVOID in_lStateHandle, LPVOID in_pStateValue) PURE;

	// notifications
	STDMETHOD( NotifyHostUnloading )( THIS_ ISIPluginHost* ) PURE ;
	
	// data extraction for realtime shaders
	STDMETHOD( ExtractLightInfo )( THIS ) PURE ;
	STDMETHOD( ExtractCameraInfo )( THIS_ LPI3DOBJECT ) PURE ;

	// rendering
	STDMETHOD( RenderSceneUsingMode )( THIS_ int, int) PURE ;
	STDMETHOD( RenderSceneUsingMaterial) ( THIS_ TCHAR*, int) PURE ;

	STDMETHOD( RenderListUsingMode )( THIS_ LPI3DLIST, int ) PURE ;
	STDMETHOD( RenderListUsingMaterial )( THIS_ LPI3DLIST, TCHAR* ) PURE ;

	// capabilities and extensions
	STDMETHOD( GetOpenGLExtensions )		( THIS_ XGSExtensionsGL* ) PURE ;
	STDMETHOD( GetOpenGLExtensionsFuncs )	( THIS_ XGSGLExtensionsFuncs* ) PURE ;
	STDMETHOD( GetWGLExtensions )			( THIS_ XGSExtensionsWGL* ) PURE ;
	STDMETHOD( GetWGLExtensionsFuncs )		( THIS_ XGSWGLExtensionsFuncs* ) PURE ;

	// scene data access
	STDMETHOD( GetSceneData )				( THIS_ XSI_RTS_SceneData** ) PURE;

	// plugin host
	STDMETHOD( GetGraphicSequencer )		( THIS_ XSI::CGraphicSequencer** ) PURE;

	STDMETHOD( GetSceneDirtyFlag )			( THIS_ bool* ) PURE;
	
	STDMETHOD( RenderNotification )			( THIS_ ULONG in_pID, XSI::siDisplayCallback notif ) PURE;
	STDMETHOD( SetPixelFormatAttributes )	( THIS_ int* ) PURE;
	STDMETHOD( GetPixelFormatAttributes )	( THIS_ int** ) PURE;

	STDMETHOD( NotifyHostLoading )( THIS_ ISIPluginHost* ) PURE ;

	STDMETHOD( SetSceneDirtyFlag )			( THIS_ bool ) PURE;
	
	STDMETHOD( GetContextFlags )			( THIS_ LONG* ) PURE;
	STDMETHOD( SetContextFlags )			( THIS_ LONG ) PURE;
	
} ;




#endif //_XGS_INTERFACE_H_
