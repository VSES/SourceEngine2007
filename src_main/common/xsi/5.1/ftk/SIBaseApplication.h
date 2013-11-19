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

#ifndef _SIBASEAPPLICATION_H_
#define _SIBASEAPPLICATION_H_

#include <SIBCUtil.h>
#include <SIBCNode.h>
#include <Scene.h>
#include <Camera.h>
#include "SIRTProcess.h"
#include "SIDisplayDriver.h"
#include "SIBCNetworkLayer.h"
#include <SIBCPixMap.h>

/**
* realtime shader runtime info
*/
typedef struct tagSI_ShaderRuntimeInfo
{
	CSIBCString				ProgID;	/*!< Shader ProgID. See the SOFTIMAGE|XSI SDK Documentation on Realtime Shaders*/
	Shader_Init				Init;	/*!< Shader Initialization callback*/
	Shader_Eval				Eval;	/*!< Shader Evaluation callback*/
	Shader_Exit				Exit;	/*!< Shader Shutdown callback*/
	Shader_Requirement		Requirement;	/*!< Shader Requirement callback*/
	void *					m_pHandle;		/*!< The handle to the DSO containing this shader (Linux) */
} SI_ShaderRuntimeInfo;


/**
* Standard Game pad information
*/
typedef struct tagSI_GamePadInfo
{
	SI_Float	leftX;	/*!< left analog stick X value */
	SI_Float	leftY;	/*!< left analog stick Y value */

	SI_Float	rightX; /*!< right analog stick X value */
	SI_Float	rightY; /*!< right analog stick Y value */

	// D-PAD
	SI_Float	dpadX; /*!< D-PAD X value */
	SI_Float	dpadY; /*!< D-PAD Y value */

	// Front buttons
	SI_Float	front_right_1; /*!<  Front right button 1*/
	SI_Float	front_right_2; /*!<  Front right button 2*/

	SI_Float	front_left_1; /*!< Front left button 1*/
	SI_Float	front_left_2; /*!< Front left button 2*/

	SI_Float	buttons[8];	/*!< Array of top button states*/

	SI_Float	start;		/*!< start button*/
	SI_Float	select;		/*!< select button*/

} SI_GamePadInfo;


/**
* Warning structure
*/
typedef struct tagSI_Warning
{
	CSIBCString				m_szWarning;	/*!< Error string */
	SI_Int					m_iLevel;		/*!< Error level ( SI_ERROR or SI_WARNING ) */

} SI_Warning;

class XSIEXPORT CSI_ImageClip : public CSIBCPixMap
{
public:

	CSI_ImageClip();
	virtual ~CSI_ImageClip();

	SI_Long	AddRef();
	SI_Long Release();

	void		SetRefName ( SI_Char* in_szName ) { m_szRefName = in_szName; };
	SI_Char*	GetRefName ();
	
private:

	SI_Long			m_lRefCount;
	CSIBCString		m_szRefName;
};

class SI_Time;

/** 
* Base Application base class
* The BaseApplication class is used as base class for all applications that use the Runtime Engine.
*
* It is the provider or container for several features:
*
* <B> Acts as the Scene Container </B>
*
* It contains 3 types of scene objects (CSLScene).
* - 3D Scenes
*	- 3D data gets evaluated and drawn with a perspective projection. All 3D objects rendered by the viewer should be in one (or more) 3D scene.
* - 2D Scenes
*	- The overlay scenes (or 2D scenes) are processes that are evaluated when the engine is using an orthognal projection matrix. This is used when overlays such as HUDs or text need to be rendered on top of the scene
* - A single Master Scene
*	- The MasterScene contains Application specific processes such as mouse, joystick and network controllers.
*
* <B> Acts as the Image/Texture Manager </B>
*
* The BaseApplication class also contains the image list (image cache) used for textures. 
* \sa CSIBaseApplication::GetImageList
* \sa CSIBaseApplication::FindImage
* \sa CSIBaseApplication::AddImage
*
* <B> Provides message logging services </B>
*
* Several functions may be used to log messages to the user
* \sa CSIBaseApplication::Warning
* \sa CSIBaseApplication::GetNbWarnings
* \sa CSIBaseApplication::GetWarning
*
* <B> Acts as a Runtime Process Factory </B>
*
* Using FindRegisteredProcess and GetRegisteredProcessList, the application is able to create Runtime Process (like CSIRTP_FCurve processes etc.) on-the-fly.
* \sa CSIBaseApplication::FindRegisteredProcess
* \sa CSIBaseApplication::GetRegisteredProcessList
*/

class XSIEXPORT CSIBaseApplication : public CSIBCNode
{
public:

	//! constructor for the CSIBaseApplication class
	/*! 
	\return A pointer to the new CSIBaseApplication
	*/
	CSIBaseApplication();

	//! destructor for the CSIBaseApplication class
	/*! 
	\return Nothing
	*/
	virtual ~CSIBaseApplication();

	//! GetMasterScene	returns the master scene object
	/*! 
	\return CSLScene *
	*/
	virtual CSLScene*	GetMasterScene() { return &m_pMasterScene; };


	//! Create3DScene creates a new scene and adds it to the list of 3D scenes
	/*! 
	\return CSLScene *
	*/
	virtual CSLScene*	Create3DScene();

	//! GetNb3DScenes returns the number of scenes in the list of 3D scenes
	/*! 
	\return SI_Int	The number of scenes
	*/
	virtual SI_Int		GetNb3DScenes();

	//! Get3DScene returns the 3D scene specified by the 0-based index passed in
	/*! 
	\param	SI_Int		The 0-based index in the scene list
	\return CSLScene*	the scene or NULL if index was out of range
	*/
	virtual CSLScene*	Get3DScene( SI_Int in_iIndex);

	/*! Removes the last scene created with CSIBaseApplication::Create3DScene.
		This call is made when a scene is created to load a file, but an error
		occurs on loading, and the scene needs to be removed.
	*/
	virtual SI_Void	RemoveLast3DScene( );

	/*! Removes the specified 3D scene in the from the list.
	*/
	virtual SI_Void	Remove3DScene( CSLScene* in_pScene );

	//! RemoveAll3DScenes removes all 3D scenes in the list and frees the memory it occupies
	/*! 
	\return Nothing
	*/
	virtual SI_Void		RemoveAll3DScenes();

	//! Create2DScene creates a new scene and adds it to the list of 2D scenes
	/*! 
	\return CSLScene *
	*/
	virtual CSLScene*	Create2DScene();

	//! GetNb2DScenes returns the number of scenes in the list of 2D scenes
	/*! 
	\return SI_Int	The number of scenes
	*/
	virtual SI_Int		GetNb2DScenes();

	//! Get2DScene returns the 2D scene specified by the 0-based index passed in
	/*! 
	\param	SI_Int		The 0-based index in the scene list
	\return CSLScene*	the scene or NULL if index was out of range
	*/
	virtual CSLScene*	Get2DScene( SI_Int in_iIndex);

	/*! Removes the last scene created with CSIBaseApplication::Create2DScene.
		This call is made when a scene is created to load a file, but an error
		occurs on loading, and the scene needs to be removed.
	*/
	virtual SI_Void	RemoveLast2DScene( );

	//! RemoveAll2DScenes removes all 2D scenes in the list and frees the memory it occupies
	/*! 
	\return Nothing
	*/
	virtual SI_Void		RemoveAll2DScenes();

	//! AddRegisteredProcess Adds a new registered process to the list
	/*! 
	\param	CSIRTProcess*	A pointer to the process 
	\return SI_Error		error code
	*/
	virtual SI_Error AddRegisteredProcess ( CSIRTProcess* in_pNewProcess );

	//! GetRegisteredProcessList get the list of registered processes for runtime linkage
	/*! 
	\return CSIBCArray<CSIRTProcess*>* the list of registered processes
	*/
	virtual CSIBCArray <CSIRTProcess *> *GetRegisteredProcessList();

	//! FindRegisteredProcess finds a registered process by name
	/*! 
	\SI_Char* describing the name of the process to find
	\return CSIRTProcess* NULL if not found, or a pointer to the CSIRTProcess
	*/
	virtual CSIRTProcess				*FindRegisteredProcess(SI_Char	*in_pName);

	//! CreateRegisteredProcess create an instance of a registered process by name
	/*! 
	\param	SI_Char*			describing the name of the process to create
	\param	CSIRTProcess**		Will hold a pointer to the newly created process
	\return CSIRTProcess* NULL if not found, or a pointer to the CSIRTProcess
	*/
	virtual SI_Error					CreateRegisteredProcess(SI_Char	*in_pName, CSIRTProcess **out_pProcess);

	//! RemoveAllRegisteredProcesses removes and clean the list of registered processes
	/*! 
	\return success or failure code
	*/
	virtual SI_Error					RemoveAllRegisteredProcesses();
		
	//! get the display driver handle
	/*! 
	\return A pointer to the display driver handle
	*/ 
	virtual CSIDisplayDriver	*GetDisplayDriver();

	//! get the network handle
	/*! 
	\return A pointer to the network layer handle
	*/ 
	virtual SIBCNetworkLayer	*GetNetworkDriver();

	//! set the display driver handle 
	/*! 
	\param	in_pDisplayDriver being a pointer to a display driver instance
	\return A pointer to the new CSIViewerApplication 
	*/ 
	virtual SI_Void				SetDisplayDriver(CSIDisplayDriver *in_pDisplayDriver);

	// Shortcuts
	//! get the camera
	/*! 
	\return A pointer to the camera
	*/ 
	virtual CSLCamera	*GetCamera();

	//! get the camera manipulator
	/*! 
	\return A pointer to the camera manipulator
	*/ 
	virtual CSIRTProcess *GetCameraManipulator();

	//! get the game pad info
	/*! 
	\return A pointer to the game pad info
	*/ 
	virtual SI_GamePadInfo *GetGamePadInfo();

	//! get the time info
	/*! 
	\return A pointer to the time info
	*/ 
	virtual SI_Time *GetTimeInfo();

	//! get the shaders runtime info
	/*! 
	\return A pointer to the shaders runtime info list
	*/ 
	virtual CSIBCArray <SI_ShaderRuntimeInfo> *GetShaderRuntimeInfo();

	//! finds the shaders runtime info
	/*! 
	\return A pointer to the shaders runtime info 
	*/ 
	virtual SI_ShaderRuntimeInfo *FindShaderRuntimeInfo(SI_Char *in_pProgID);

	//! get the image clip list
	/*! 
	\return A pointer to the image clip list
	*/ 
	virtual CSIBCArray <CSI_ImageClip*> *GetImageList();

	//! finds the image clip
	/*! 
	\return A pointer to the image clip
	*/ 
	virtual CSI_ImageClip *FindImage(SI_Char *in_pImagePath);

	//! adds the image clip
	/*! 
	\return A pointer to the image clip
	*/ 
	virtual CSI_ImageClip *AddImage(SI_Char *in_pImagePath);

	//! adds the image clip with a reference name
	/*!
	\param	SI_Char*	The reference name
	\param	SI_Char*	The path to the image
	\return A pointer to the image clip
	*/ 
	virtual CSI_ImageClip *AddImage(SI_Char* in_szRefName, SI_Char *in_pImagePath);

	//! get the application ID
	/*! 
	\return the application ID
	*/ 
	virtual SI_Int	GetApplicationID();
	

	//! set the application ID
	/*! 
	\int describing the application ID to set
	\return nothing
	*/ 
	virtual SI_Void	SetApplicationID(SI_Int in_lAppID);

	//! set wether the application shows warnings generated by the various parts of the application
	/*! 
	\SI_Bool SI_TRUE to show the warnings
	\return nothing
	*/ 
	virtual SI_Void	SetShowWarnings(SI_Bool	in_bWarnings);

	//! gets the flag that indicates if the application should show warnings
	/*! 
	\return SI_Bool
	*/ 
	virtual SI_Bool	GetShowWarnings();
	
	//! gets the HWND of the main window
	/*! 
	\return SI_Void*
	*/ 
	virtual SI_Void*	GetMainHwnd() { return NULL;}

	//! Generate a new warning
	/*! 
	\SI_Char*	the warning
	\SI_Int*	the warning level ( SI_MESSAGE, SI_WARNING, SI_ERROR )
	\return nothing
	*/ 
	static SI_Void	Warning( SI_Char* in_szWarning, SI_Int in_iLevel );

	//! Gets the number of warning in the list
	/*! 
	\return Number of warnings
	*/ 
	SI_Int	GetNbWarnings();

	//! Gets a warning from the list
	/*! 
	\param	SI_Warning& the warning
	\return SI_Warning*	A pointer to the warning, or null if index is invalid
	*/ 
	SI_Warning*	GetWarning( SI_Int in_iIndex );
	SI_Void		ClearWarning( );

	//! Get Global Dirty Flag
	/*! 
	\return SI_UInt	The global dirty flag
	*/ 
	SI_UInt		GetGlobalDirtyState() { return m_uiGlobalDirtyState; };

	//! Set Global Dirty Flag
	/*!
	\param	SI_UInt	The new dirty state
	\return nothing
	*/ 
	SI_Void		SetGlobalDirtyState( SI_UInt in_uiNewState ) { m_uiGlobalDirtyState = in_uiNewState; };

	bool        IsOtherInstanceRunning() { return m_bOtherInstanceRunning; }

protected:

	CSIDisplayDriver					*m_pDisplayDriver;
	SIBCNetworkLayer					*m_pNetwork;

	// Shortcuts
	CSLCamera							*m_pCamera;
	CSIRTProcess						*m_pCameraManipulator;
	SI_GamePadInfo						m_GamePadInfo;
	SI_Time								*m_Time;						// constructed by platform specific code
	CSIBCArray <SI_ShaderRuntimeInfo>	m_ShaderRuntimeInfo;
	SI_ShaderRuntimeInfo				m_DefaultShaderRuntimeInfo;
	SI_UInt								m_uiGlobalDirtyState;			// Global dirty flag

private:
	
	CSLScene							m_pMasterScene;
	CSIBCArray <CSLScene *>				m_p3DSceneList;
	CSIBCArray <CSLScene *>				m_p2DSceneList;
	CSIBCArray <CSIRTProcess *>			m_pProcessList;
	CSIBCArray <CSI_ImageClip *>		m_pImagePool;
	SI_Int								m_lApplicationID;
	
	static SI_Bool						m_bShowWarnings;
	static CSIBCArray <SI_Warning>		m_pWarnings;

	SI_Void*							m_pUnused;

    bool                                m_bOtherInstanceRunning;
};

#endif

