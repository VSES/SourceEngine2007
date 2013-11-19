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

#ifndef _SIVIEWERAPPLICATION_
#define _SIVIEWERAPPLICATION_

#include "SIBCUtil.h"
#include "SIBCNode.h"
#include "SIDisplayDriver.h"
#include "SIBaseApplication.h"
#include <Camera.h>
#include <RTP_BasicProcessesTypes.h>
#include "XSIViewerOptions.h"
#include "XSIViewerRenderingTargetManager.h"


class CSIViewerApplication;
typedef SI_Error (*VIEWER_EXEC_CALLBACK)( CSIViewerApplication *in_pApplication);
typedef SI_Error (*PLUGIN_INITIALIZATION_CALLBACK) ( void *in_pData );
typedef SI_Error (*PLUGIN_REGISTRATION_CALLBACK) (CSIBaseApplication *in_pApplication, CSLScene *in_pCurrentScene);
typedef SI_Error (*PLUGIN_CLEANUP_CALLBACK) ();



#include "SIRuntimeEngine.h"
	
/** 
* Base class for the platform independant viewer application
* The base application class will implement the platform independent application framework. It is essentially a base class that implements the most common and useful functions:
* 
* -	Load/Save a .XSI file
*	-	The base application will be able to load any 3.0, 3.5 or 3.6 dotXSI file
*	-	The base application will be able to either replace or merge dotXSI files
* -	Set the update frame rate (clock speed)
*	-	Play all frames
*	-	Specific frame rate (industry standard)
*	-	User defined frame rate
* -	Scene playback control 
*	-	Forward
*	-	Backward
*	-	Reset
*	-	Loop
*	-	Play
*	-	Stop
*	-	Set frame (start, end, current)
* -	Runtime control 
*	-	Start
*	-	Stop
*	-	Pause
*	-	Reset
* -	Camera manipulation 
*	-	Softimage
*		-	Orbit
*		-	Zoom
*		-	Pan
*		-	Dolly
*		-	Frame all
*		-	Select object
*	-	FPS
*		-	Strafe
*		-	Forward/backward
*		-	Camera look
*		-	Select object
*	-	Simple mode
*		-	LMB + mouse = orient
*		-	RMB + mouse = pan
*		-	MMB + mouse = zoom
* -	High level control
*	-	Get/Set values
*	-	Selection
*	-	Application automation (platform independent)
*	-	Remote control
* 
* The application has two modes:
* 
* Standalone mode: the application has its own control
* Slave mode: the application is remote controlled by another application
* 
*/

class XSIEXPORT CSIViewerApplication : public CSIBaseApplication
{
public:

	/** 
	* Execution callbacks types
	* Those are called when the application reaches a certain point in its execution cycle. 
	*
	* Execution callbacks are:
	* 
	* - PRE_BEGIN_FRAME
	* - PRE_EVALUATE_MODEL
	* - PRE_EVALUATE_PRIMITIVE
	* - PRE_EVALUATE_RENDER
	* - PRE_ORTHO
	* - PRE_END_FRAME
	* - POST_END_FRAME
	* 
	* The execution flow is as follows:
	* 
	* -	Execute PRE_BEGIN_FRAME callbacks
	* -	Prepare the display driver to start a frame
	* -	Execute POST_BEGIN_FRAME/PRE_EVALUATE_MODEL callbacks 
	* -	Evaluate the runtime nodes in the model execution list
	* -	Execute POST_EVALUATE_MODEL/PRE_EVALUATE_PRE_PRIMITIVE callbacks 
	* -	Evaluate the runtime nodes in the primitive execution list
	* -	Execute POST_EVALUATE_PRIMITIVE/PRE_EVALUATE_RENDER callbacks 
	* -	Evaluate the material runtime nodes in the material library list
	* -	Execute POST_EVALUATE_RENDER/PRE_END_ORTHO callbacks
	* -	Evaluate the 2D ortho scene content
	* -	Execute the POST_ORTHO/PRE_END_FRAME callbacks
	* -	Prepare the display driver to end a frame
	* - Execute POST_END_FRAME callbacks
	* 
	*/
	enum	CSIExecCallbackType
	{
		SI_PRE_BEGIN_FRAME,				/*!< Occurs before BeginFrame */
		SI_PRE_EVALUATE_MODEL,			/*!< Occurs before model processes are evaluated */
		SI_PRE_EVALUATE_PRIMITIVE,		/*!< Occurs before primitive processes are evaluated */
		SI_PRE_EVALUATE_RENDER,			/*!< Occurs before render processes are evaluated */
		SI_PRE_ORTHO,					/*!< Occurs before orthogonal drawing is evaluated*/
		SI_PRE_END_FRAME,				/*!< Occurs before EndFrame (swapbuffer)*/
		SI_POST_END_FRAME				/*!< Occurs after EndFrame*/
	};
	
// Methods
public:
    
	//***********************************************************************
	// Constructor/destructor
	//***********************************************************************
	//! constructor for CSIViewerApplication
	/*! 
	\return A pointer to the new CSIViewerApplication 
	*/ 
	CSIViewerApplication();

	//! destructor for CSIViewerApplication
	/*! 
	\return nothing
	*/ 
	~CSIViewerApplication();

	//***********************************************************************
	// Accessors
	//***********************************************************************

	//! get a specific callback list 
	/*! 
	\param	in_pType being the type of callback list we need to return
	\return A list of callbacks
	*/ 
	virtual CSIBCArray<VIEWER_EXEC_CALLBACK>&			GetExecutionCallbackList(CSIViewerApplication::CSIExecCallbackType	in_pType);

	//! get the plugin registration callback list
	/*! 
	\return A list of plugin registration callbacks
	*/ 
	virtual CSIBCArray<PLUGIN_REGISTRATION_CALLBACK>*	GetPluginRegistrationCallbackList();

	//! get the 2D engine
	/*! 
	\return A pointer to the 2D engine
	*/ 
	virtual CSIRuntimeEngine	*Get2DEngine();

	//***********************************************************************
	// Methods
	//***********************************************************************

	//! call each registered plugin callback
	/*! 
	\param CSLScene* describing the scene that the callback will initialize
	\return success or failure code
	*/ 
	virtual SI_Error			RegisterPluginCallbacks(CSLScene *in_pNewScene);

	//! Core initialization of the viewer application in non stand alone mode
	/*! 
	\return success or failure code
	*/ 
	virtual SI_Error			InitializeCore(SI_Void *in_pAppData) { return SI_SUCCESS; }

	//! Initialize the viewer application in stand alone mode
	/*! 
	\param	in_pAddData describing the platorm dependant data
	\return success or failure code
	*/ 
	virtual SI_Error			Initialize(SI_Void *in_pAppData);

	//! runs viewer application
	/*! 
	\return success or failure code
	*/ 
	virtual SI_Error			Run();

	//! shuts down the viewer application
	/*! 
	\return success or failure code
	*/ 
	virtual SI_Error			Shutdown();

	//! Cleans up all scenes and starts with the master and ortho scenes
	/*!
	\return success or failure code
	*/ 
	virtual SI_Error			New();
	
	//! Loads a dotXSI file and adds it to the scene database
	/*! 
	\param	SI_Char*	The full path of the dotXSI file to add to the scene 
	\return success or failure code
	*/ 
	virtual SI_Error			Merge( SI_Char* l_szFilename, SI_READ_CALLBACK_FNC in_pFunc );


	//! Loads a dotXSI file and sets it to the scene database
	/*! 
	\param	SI_Char*	The full path of the dotXSI file to add to the scene 
	\param	SI_READ_CALLBACK_FNC	The read callback function
	\return success or failure code
	*/ 
	virtual SI_Error			Load( SI_Char* l_szFilename, SI_READ_CALLBACK_FNC in_pFunc );

	//! Get the viewer options
	/*!
	\return CXSIViewerOptions*	The XSI viewer option object
	*/
	virtual CXSIViewerOptions*	GetViewerOptions() { return &m_pOptions; };

	//! Set an integer value from a viewer config file token
	/*!
	\param  CSIBCString	Token
	\param	SI_Int&		Value
	\return SI_Error	Success or failure
	*/
	virtual SI_Error	SetViewerOptionInt(CSIBCString in_szToken, SI_Int in_iValue );

	//! Set string value from a viewer config file token
	/*!
	\param  CSIBCString		Token
	\param	CSIBCString&	Value
	\return SI_Error	Success or failure
	*/
	virtual SI_Error	SetViewerOptionString(CSIBCString in_szToken, CSIBCString in_szValue );

	//! Get an integer value from a viewer config file token
	/*!
	\param  CSIBCString	Token
	\param	SI_Int&		Value
	\return SI_Error	Success or failure
	*/
	virtual SI_Error	GetViewerOptionInt(CSIBCString in_szToken, SI_Int& out_iValue );

	//! Get string value from a viewer config file token
	/*!
	\param  CSIBCString		Token
	\param	CSIBCString&	Value
	\return SI_Error	Success or failure
	*/
	virtual SI_Error	GetViewerOptionString(CSIBCString in_szToken, CSIBCString& out_szValue );

	//! Add an integer value to the viewer config file
	/*!
	\param  CSIBCString	Token
	\param	SI_Int&		Value
	\return SI_Error	Success or failure
	*/
	virtual SI_Error	AddViewerOptionInt(CSIBCString in_szToken, SI_Int in_iValue );

	//! Add a string value to a viewer config file 
	/*!
	\param  CSIBCString		Token
	\param	CSIBCString&	Value
	\return SI_Error	Success or failure
	*/
	virtual SI_Error	AddViewerOptionString(CSIBCString in_szToken, CSIBCString in_szValue );

	//! Set the active camera 
	/*!
	\param	CSLCamera*	Pointer to a CSLCamera object
	\return CSLCamera*	Previous active camera
	*/ 

	//! Set the active camera 
	/*!
	\param	CSLCamera*	Pointer to a CSLCamera object
	\return CSLCamera*	Previous active camera
	*/ 

	CSLCamera*	SetActiveCamera( CSLCamera* in_pCamera );

	
	//! Get the active camera 
	/*!
	\param	None
	\return CSLCamera*	Active camera (or null for default scene camera)
	*/ 

	CSLCamera*	GetActiveCamera() { return m_pActiveCamera; };

	SI_Error	RemoveModelFromScene ( CSLModel* in_pModel );

	XSIViewerRenderingTargetManager* GetRenderingTargetManager() { return &m_RenderingTargetManager; }

private:

	SI_Void		FindAllCameras	(CSLModel* in_pModel, CSIBCArray<CSLCamera*>& in_pCameraList );

	CSIBCArray<VIEWER_EXEC_CALLBACK>			m_ExecCallbacks[7];
	CSIBCArray<PLUGIN_REGISTRATION_CALLBACK>	m_PluginsRegistrationCallbacks;	
	CSIRuntimeEngine							*m_p2DEngine;

	SI_Void										*m_pUnused;

	CXSIViewerOptions							m_pOptions;

	CSLCamera									*m_pActiveCamera;

	XSIViewerRenderingTargetManager				m_RenderingTargetManager;


};

XSIEXPORT SI_Error SI_DrawSortedCluster( CSIViewerApplication *in_pApplication);




#endif // _SIVIEWERAPPLICATION_

