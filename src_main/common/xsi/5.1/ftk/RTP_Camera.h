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

#ifndef _RTP_CAMERA_
#define _RTP_CAMERA_

#include <Scene.h>
#include <SIBCUtil.h>
#include <SIRTProcess.h>
#include <SIBaseApplication.h>
#include <SIDisplayDriver.h>
#include <Camera.h>
#include "RTP_BasicProcessesTypes.h"

SI_Error		RegisterSICamera ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );
CSLTemplate		*SICameraRTPCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
SI_Error		SICameraPostRTPCallback(CSLTemplate *in_pTemplate, SLContext *in_pContext);

/** 
* Camera display runtime process
* 
* The camera display runtime process takes the current camera parameters, computes the projection matrix based on the projection type and sets it to the display driver
*
*/
class XSIEXPORT CSIRTP_CameraDisplay : public CSIRTProcess
{

public:

	//*******************************************************************
	// constructor/destructor
	//*******************************************************************

	//! constructor for the CSIRTP_CameraDisplay class
	/*! 
	\return A pointer to the new CSIRTP_CameraDisplay
	*/
	CSIRTP_CameraDisplay();

	//! destructor for the CSIRTP_CameraDisplay class
	/*! 
	\return nothing
	*/
	~CSIRTP_CameraDisplay();


	/*! class factory for runtime linked runtime processes
	* This is used when an external entity wants to create a process of this 
	* type without having to link at compile time. The mechanism is simple.
	* the external entity uses the class factory to create a new CSIRTProcess
	* and sets the initialization parameters in the user data "SIRTINIT".
	* The external entity then calls CSIRTProcess::Init to initialize the data
	* extracted from the user data
	*
	\param	CSIRTProcess**	A pointer to a CSIRTProcess pointer 
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess**out_pProcess );

	//*******************************************************************
	// accessors
	//*******************************************************************

	//! get the type of process
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::POSTHIERARCHICAL;}

	//! get the projection type
	/*! 
	\return the projection type
	*/
	virtual SI_CameraProjectionType	GetProjectionType();

	//! set public values
	/*!
	\return success or failure code
	*/	
	virtual SI_Error			SetValues(void *in_pValueStruct);

	//! get public values
	/*!
	\return success or failure code
	*/
	virtual SI_Error			GetValues(void *in_pValueStruct);

	//*******************************************************************
	// methods
	//*******************************************************************

	//! initialization function
	/*! 
	\param CSIDisplayDriver* describing the display driver to set the projection and view matrix to
	\param CSLCamera* describing the camera to get the information from
	\return SI_Error	the success or failure of the initialization
	*/
	virtual SI_Error			Initialize
								(
									CSIDisplayDriver	*in_pDisplayDriver,
									CSLCamera			*in_pCamera
								);


	//! evaluation function
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();



private:

	CSIRTP_CameraDisplayData	m_PublicData;
	SI_Void						*m_pUnused;

	SI_Error					ComputeOrthoMatrix();
	SI_Error					ComputePerspMatrix();
	SI_Error					ComputeViewMatrix();

};


/** 
* Camera manipulate runtime process
* 
* The camera manipulate runtime process moves the camera according to the user's manipulation
*
*/
class XSIEXPORT CSIRTP_CameraManipulate : public CSIRTProcess
{
public:

	//*******************************************************************
	// constructor/destructor
	//*******************************************************************

	//! constructor for the CSIRTP_CameraDisplay class
	/*! 
	\return A pointer to the new CSIRTP_CameraDisplay
	*/
	CSIRTP_CameraManipulate();

	//! destructor for the CSIRTP_CameraDisplay class
	/*! 
	\return nothing
	*/
	~CSIRTP_CameraManipulate();


	/** class factory for runtime linked runtime processes
	* This is used when an external entity wants to create a process of this 
	* type without having to link at compile time. The mechanism is simple.
	* the external entity uses the class factory to create a new CSIRTProcess
	* and sets the initialization parameters in the user data "SIRTINIT".
	* The external entity then calls CSIRTProcess::Init to initialize the data
	* extracted from the user data
	*
	\param	CSIRTProcess**	A pointer to a CSIRTProcess pointer 
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess**out_pProcess );


	//*******************************************************************
	// accessors
	//*******************************************************************

	//! get the type of process
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::PREHIERARCHICAL;}

	//! set public values
	/*!
	\return success or failure code
	*/	
	virtual SI_Error			SetValues(void *in_pValueStruct);

	//! get public values
	/*!
	\return success or failure code
	*/
	virtual SI_Error			GetValues(void *in_pValueStruct);

	//! get linked camera manipulator
	/*!
	\return linked camera manipulators
	*/
	virtual CSIRTP_CameraManipulate *GetLinkedCameraManipulator();

	//! Set linked camera manipulator
	/*!
	\param	CSIRTP_CameraManipulate*	Pointer to the linked camera manipulator
	\return nothing
	*/
	virtual SI_Void SetLinkedCameraManipulator(CSIRTP_CameraManipulate *in_pManipulator);

	//! get target camera manipulator
	/*!
	\return target camera manipulators
	*/
	virtual CSIRTP_CameraManipulate *GetTargetCameraManipulator();

	//! Set target camera manipulator
	/*!
	\param	CSIRTP_CameraManipulate*	Pointer to the target camera manipulator
	\return nothing
	*/
	virtual SI_Void SetTargetCameraManipulator(CSIRTP_CameraManipulate *in_pManipulator);

	//*******************************************************************
	// methods
	//*******************************************************************

	//! evaluation function
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

private:
	CSIRTP_CameraManipulateData				m_PublicData;
	SI_Int									m_lMode;
	SI_Void									*m_pUnused;

	SI_Error								Orbit(SI_Float dx, SI_Float dy);
	SI_Error								Zoom(SI_Float dx, SI_Float dy);
	SI_Error								Pan(SI_Float dx, SI_Float dy);
	SI_Error								Dolly(SI_Float dx, SI_Float dy);
	SI_Error								Strafe(SI_Float dx, SI_Float dy);
	SI_Error								Forward(SI_Float dx, SI_Float dy);
	SI_Error								Look(SI_Float dx, SI_Float dy);
	SI_Error								FrameAll();
	SI_Error								SelectObject();
};

/** 
* Camera copy runtime process
* 
* The camera copy runtime process copies source camera properties to a destination camera, this is used to update the master camera from a camera that exists in one of the dotXSI scenes.
*
*/
class XSIEXPORT CSIRTP_CameraCopy : public CSIRTProcess
{
public:

	//*******************************************************************
	// constructor/destructor
	//*******************************************************************

	//! constructor for the CSIRTP_CameraCopy class
	/*! 
	\return A pointer to the new CSIRTP_CameraCopy
	*/
	CSIRTP_CameraCopy();

	//! destructor for the CSIRTP_CameraCopy class
	/*! 
	\return nothing
	*/
	~CSIRTP_CameraCopy();


	/**! class factory for runtime linked runtime processes
	* This is used when an external entity wants to create a process of this 
	* type without having to link at compile time. The mechanism is simple.
	* the external entity uses the class factory to create a new CSIRTProcess
	* and sets the initialization parameters in the user data "SIRTINIT".
	* The external entity then calls CSIRTProcess::Init to initialize the data
	* extracted from the user data
	*
	\param	CSIRTProcess**	A pointer to a CSIRTProcess pointer 
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess**out_pProcess );


	//*******************************************************************
	// accessors
	//*******************************************************************

	//! get the type of process
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::HIERARCHICAL;}

	//! set public values
	/*!
	\return success or failure code
	*/	
	virtual SI_Error			SetValues(void *in_pValueStruct);

	//! get public values
	/*!
	\return success or failure code
	*/
	virtual SI_Error			GetValues(void *in_pValueStruct);

	//! get native callbacks
	/*!
	\return the list of native callbacks
	*/
	static SLTemplateCallback	*GetNativeCallbacks();

	//*******************************************************************
	// methods
	//*******************************************************************

	//! evaluation function
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

private:
	CSIRTP_CameraCopyData		m_PublicData;
	static SLTemplateCallback	m_sNativeCallbacks;
	SI_Void						*m_pUnused;
};

#endif // _RTP_CAMERA_
