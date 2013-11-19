#ifndef _FACTORYDISPLAYPUBLICDATA_
#define _FACTORYDISPLAYPUBLICDATA_

/////////////////////////////////////////////////////////////////////////////////////
// ENUMS
/////////////////////////////////////////////////////////////////////////////////////

// Shouldn't do this but ...
#ifdef _needE3DVIEWPOINT
typedef enum tagE3DVIEWPOINT
{
	SI3D_VIEWPOINT_USER = 0,
	SI3D_VIEWPOINT_TOP,
	SI3D_VIEWPOINT_FRONT,
	SI3D_VIEWPOINT_RIGHT,
	SI3D_VIEWPOINT_MAX
} E3DVIEWPOINT;
#endif // _needE3DVIEWPOINT

/////////////////////////////////////////////////////////////////////////////////////
// context states
/////////////////////////////////////////////////////////////////////////////////////

typedef struct tagXSI_ViewData
{
	// colors
	float		BackgroundColor[4];
	float		ViewportColor[4];

	// sizes
	LONG		Width;
	LONG		Height;

	// viewing gizmos
	int			DrawAxis;
	int			DrawRulers;
	int			DrawGrid;
	int			DrawSafeGuide;

	// view Data
	UINT		CameraType;
	UINT		CameraIndex;
	UINT		CameraProjType;

	// View ID
	UINT_PTR	ID;
	TCHAR		ViewName[1024];


} XSI_ViewData;

typedef struct tagXSI_CameraDisplay
{
	// fast playback
	LONG		FastPlaybackMode;
	LONG		FastPlaybackWireframe;

} XSI_CameraDisplay;

typedef struct tagXSI_OpenGLData
{
	// primary context
	HDC			hdc;
} XSI_OpenGLData;

typedef struct tagXSI_HWRenderingData
{
	bool				Activate;
	int					Width;
	int					Height;
	unsigned char*		Buffer;
	unsigned char*		BigBuffer;
	wchar_t				RenderEngineName[1024];
} XSI_HWRenderingData;


#endif//_FACTORYDISPLAYPUBLICDATA_