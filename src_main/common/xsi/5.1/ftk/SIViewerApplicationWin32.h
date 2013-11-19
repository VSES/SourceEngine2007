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

#ifndef _SIVIEWERAPPLICATIONWIN32_
#define _SIVIEWERAPPLICATIONWIN32_

#include <windows.h>
#include <commctrl.h>
#include <dinput.h>
#include "SIviewerApplication.h"

#define XSIVIEWER_CUSTOM_MESSAGE_BASE	100

/**
* Win32 Context Structure
*/
typedef struct tagWin32Context
{
	HINSTANCE			m_hInstance;		/*!< XSI Viewer Instance*/
	HINSTANCE			m_hPrevInstance;	/*!< XSI Viewer Previous Instance*/
	LPSTR				m_lpCmdLine;		/*!< Command line*/
	int					m_nCmdShow;			/*!< Window Show Mode*/
	HWND				m_hParentWnd;		/*!< Handle of the XSI Viewer parent window*/
	HWND				m_hWnd;				/*!< Handle of the XSI Viewer window*/
	HWND				m_hLogWindow;		/*!< Handle of the message log window*/
	HWND				m_hExplorerWindow;	/*!< Handle of the explorer window*/
	DLGPROC				m_pEditProc;		/*!< Message callback for the parameter edit widget used in the explorer*/

	HMENU				m_hMenu;			/*!< Handle of the main XSI Viewer menu*/
	HACCEL				m_hAccel;			/*!< Handle of the keyboard accelerators*/
	bool				m_bHasFocus;		/*!< XSIViewer keyboard focus state*/


	LPDIRECTINPUT		m_lpdi;				/*!< DirectInput Interface*/
	LPDIRECTINPUTDEVICE	m_lpdiMouse;		/*!< Mouse device interface*/
	bool				m_bAcquired;		/*!< Mouse acquired state*/

	// Application data
	bool				m_bShowGrid;		/*!< Show Grid state*/
	bool				m_bShowLog;			/*!< Show Message Log state*/
	bool				m_bShowPlayback;	/*!< Show Playback Information state*/
	bool				m_bShowInfo;		/*!< Show Information state (like FPS etc.)*/
	bool				m_bShowPanel;		/*!< Show Panel State*/
	bool				m_bShowExplorer;	/*!< Show Explorer state*/
	bool				m_bEditParam;	
	bool				m_bStandAlone;		/*!< Run in Stand Alone mode or Embeded*/

	bool				m_bAppReady;

	CSLScene			*m_pCurrentScene;

	SI_Void				*m_pUnused;
} Win32Context;

class CSIViewerApplicationWin32;
typedef SI_Error (*SI_EventCallback)
(
	CSIViewerApplicationWin32	*in_pWin32Application, 
	HWND						hWindow,
	UINT						uiMsg,
	WPARAM						wParam,
	LPARAM						lParam
);

#ifdef _XSI_VIEWER_DEMO
#define VIEWER_APPNAME	"SOFTIMAGE® | XSI® Viewer v3.6.2"
#else
#define VIEWER_APPNAME	"SOFTIMAGE® | XSI® Viewer Professional v3.6.2"
#endif

/** 
* Class for the Win32 viewer application.
*
* The CSIViewerApplicationWin32 class is a Win32 version of the CSIViewerApplication class.
* It's job is to create and manage the Win32 version of the XSI Viewer, like creating windows and dialogs, 
* and everything else that is Win32 specific.
* 
* In addition, the Win32 application will also: 
* 
* -	initialize the OGL or Direct3D display driver with the window context
* -	Implement DirectInput devices capabilities and MIDI device capability
* -	Implement Remote control (IPC, networking) via the high level control layer
* -	Add drag and drop of XSI files
* - Owns the XSI Viewer Explorer
* 
* \sa CSIViewerApplication
*/
class XSIEXPORT CSIViewerApplicationWin32 : public CSIViewerApplication
{
// Methods
private:
	Win32Context						m_WinContext;	
	CSIBCArray<SI_EventCallback>		m_pEventCallbacks;
	SI_Void								*m_pUnused;

protected:

public:
    
	//***********************************************************************
	// Constructor/destructor
	//***********************************************************************
	//! constructor for CSIViewerApplicationWin32
	/*! 
	\return A pointer to the new CSIViewerApplicationWin32
	*/ 
	CSIViewerApplicationWin32();

	//! destructor for CSIViewerApplicationWin32
	/*! 
	\return nothing
	*/ 
	~CSIViewerApplicationWin32();

	//***********************************************************************
	// Methods (CSIViewerApplication overloads)
	//***********************************************************************

	//! Core initialization of the viewer application in non stand alone mode
	/*! 
	\param	SI_Void*			describing the platorm dependant data
	\return SI_error			success or failure code
	*/ 
	virtual SI_Error			InitializeCore(SI_Void *in_pAppData);

	//! Initialize the viewer application in stand alone
	/*! 
	\param	SI_Void*			describing the platorm dependant data
	\return SI_error			success or failure code
	*/ 
	virtual SI_Error			Initialize(SI_Void *in_pAppData);

	//! runs viewer application
	/*! 
	\return SI_error			success or failure code
	*/ 
	virtual SI_Error			Run();

	//! shuts down the viewer application
	/*! 
	\return SI_error			success or failure code
	*/ 
	virtual SI_Error			Shutdown();

	//! Loads a dotXSI file and sets it to the scene database
	/*! 
	\param SI_Char*				The full path of the dotXSI file you wish to load
	\return SI_error			success or failure code
	*/ 
	virtual SI_Error			Load( SI_Char* l_szFilename );

	//! Loads a dotXSI file and adds it to the scene database
	/*! 
	\param SI_Char*				The full path of the dotXSI file you wish to load
	\return SI_error			success or failure code
	*/ 
	virtual SI_Error			Merge( SI_Char* l_szFilename );

	virtual SI_Error			SaveLog();
	virtual SI_Error			ClearLog();

	virtual SI_Error			Save( SI_Char* l_szFilename );


	//***********************************************************************
	// Accessors
	//***********************************************************************
	//! Get the win32 context
	/*! 
	\return Win32Context*		pointer to the win32 context
	*/ 
	Win32Context				*GetWin32Context();

	//! Get the event callback list
	/*! 
	\return CSIBCArray<SI_EventCallback>*	pointer to the event callback array
	*/ 	
	CSIBCArray<SI_EventCallback>		*GetEventCallbackList();

	//***********************************************************************
	// Methods plugin management (dynamic)
	//***********************************************************************

	//! Initialize the viewer application display driver
	//! The display driver name must be relative to the .\drivers directory
	/*! 
	\param	SI_Char*			describing the platorm dependant data
	\return SI_Error			success or failure code
	*/ 
	virtual SI_Error			InitializeDisplayDriver(SI_Char	*in_pchDriverName);

	//! Initialize the viewer application plugins
	/*! 
	\param	SI_Char*			describing the directory to scan
	\param	SI_Bool				recurse down directory hierarchy
	\return SI_Error			success or failure code
	*/ 	
	virtual SI_Error			InitPlugins(SI_Char *in_pchDirectory, SI_Bool in_bRecursive);

	//! Clean up and free memory used by the viewer application plugins
	/*! 
	\param	SI_Char*			describing the directory to scan
	\return SI_Error			success or failure code
	*/ 	
	virtual SI_Error			CleanPlugins ( SI_Char* in_pchDirectory );

	//! Initialize the viewer application shaders
	/*! 
	\param	SI_Char*			describing the directory to scan
	\param	SI_Bool				recurse down directory hierarchy
	\return SI_Error			success or failure code
	*/ 	
	virtual SI_Error			InitializeShaders(SI_Char *in_pchDirectory, SI_Bool in_bRecursive);
	
	//***********************************************************************
	// Methods 
	//***********************************************************************

	//! Show/Hide the log window
	/*! 
	\param	none
	\return nothing
	*/ 	
	virtual SI_Void		ShowLog();

	//! Show/Hide the explorer window
	/*! 
	\param	none
	\return nothing
	*/ 	

	virtual SI_Void		ShowExplorer();


	//! rescan the explorer window
	/*! 
	\param	none
	\return nothing
	*/ 	

	virtual SI_Void		RescanExplorer();

	//! rescan the explorer window
	/*! 
	\param	CdotXSITemplate*	The template to add to the explorer view
	\param  HWND				widget hwnd
	\param  HTREEITEM			the parent item in the tree view
	\return Nothing
	*/ 	

	virtual SI_Void		ScanTemplate(CdotXSITemplate *in_pTemplate, HWND widget, HTREEITEM hParent);

	//! Set/unset fullscreen mode
	/*! 
	\param	in_bFullscreen	TRUE = fullscreen, FALSE = windowed
	\param	in_iWidth		width of new resolution (no effect for windowed mode)
	\param	in_iHeight		height of new resolution (no effect for windowed mode)
	\return SI_Bool			SI_TRUE = Success, SI_FALSE if something went wrong (check log for details)
	*/ 	
	virtual SI_Bool		SetFullscreen(SI_Bool in_bFullscreen, SI_Int in_iWidth, SI_Int in_iHeight);


	virtual SI_Void*	GetMainHwnd() {return (SI_Void*) GetWin32Context()->m_hWnd;}

	//! Creates a new message ID that can be used by plugins to identify custom Windows messages or menu items
	/*! 
	\return SI_UInt			Unique message ID
	*/ 	
	virtual SI_UInt		NewUniqueMessageID ();

	//! Returns the handle of the plugin menu. Typically used by plugins to attach submenus
	/*! 
	\return HMENU		A Win32 menu handle to the plugin menu
	*/ 	
	virtual HMENU		GetPluginMenu ();


	static SI_Void ReadProgress(CXSIParser *in_pParser);

private:

	// dotXSI data
	static SI_Long		m_lFileLength;
	
	SI_Bool				m_bPluginsAreClean;
	SI_UInt				m_iCurrentMessageID;
	

};



#endif //_SIVIEWERAPPLICATIONWIN32_	