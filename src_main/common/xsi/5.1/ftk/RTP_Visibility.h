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

#ifndef _RTP_VISIBILITY_H_
#define _RTP_VISIBILITY_H_

#include <SIRTProcess.h>
#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <SIBCMatrix.h>
#include <Transform.h>
#include <Visibility.h>
#include <RTP_BasicProcessesTypes.h>

SI_Error	RegisterSIVisibility ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* Visibility runtime process
* 
* When evaluated, the visibility runtime process will check if the associated bounding volume is
* in the camera frustrum (using a SphereInFrustrum algorithm.) If the bounding volume is not visible by the
* camera, the ::Evaluate fonction will return an error, causing the cancel of further evaluation for this model.
* This means that if a model is currently behind the camera or outside the view frustrum, all other primitive
* process that come after the visibility test will not be evaluated (such as CSIRTP_Envelope or CSIRTP_Mesh).
*
*/
class XSIEXPORT CSIRTP_Visibility : public CSIRTProcess
{
public:
	CSIRTP_Visibility();
	~CSIRTP_Visibility();

	static	SLTemplateCallback	m_sNativeCallbacks;

	///////////////////////////////////////////////////////////////////////
	// Methods ( inherited from SIRTProcess )
	///////////////////////////////////////////////////////////////////////

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::VISIBILITY;}

	//! The evaluate fonction will figure out if the associated model is visible
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

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

	/** class factory for runtime linked runtime processes
	* This is used when an external entity wants to create a process of this 
	* type without having to link at compile time. The mechanism is simple.
	* the external entity uses the class factory to create a new CSIRTProcess
	* and sets the initialization parameters in the user data "SIRTINIT".
	* The external entity then calls CSIRTProcess::Init to initialize the data
	* extracted from the user data
	*
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess**out_pProcess );

private:

	CSLVisibility*				m_pVisibility;
	CSIRTP_BoundingVolume*		m_pBoundingVolume;
	SI_Void						*m_pUnused;

	
};
#endif
