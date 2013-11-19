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

#ifndef _RTP_FCURVE_H_
#define _RTP_FCURVE_H_

#include <SIRTProcess.h>
#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <SIBCMatrix.h>
#include <FCurve.h>
#include "RTP_BasicProcessesTypes.h"

SI_Error	RegisterSIFCurve ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* FCurve runtime process
* 
* The fcurve runtime process handles the evaluation of its associated CSLFcurve at the current frame
*
*/
class XSIEXPORT CSIRTP_FCurve : public CSIRTProcess
{
public:
	CSIRTP_FCurve();
	~CSIRTP_FCurve();

	static	SLTemplateCallback	m_sNativeCallbacks;

	///////////////////////////////////////////////////////////////////////
	// Methods ( inherited from SIRTProcess )
	///////////////////////////////////////////////////////////////////////

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::CONTROLLER;}

	//! The evaluate fonction will evaluate it's associated CSLBasicFCurve object
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

	//! Set the CSLFCurve that is associated with this process. Pass a CSLFCurve* casted as a void*
	/*!
	\param	CSLFCurve*	The CSLFcurve associated with this process
	\return success or failure code
	*/	
	virtual SI_Error			SetValues(void *in_pValueStruct);

	//! get public values
	/*!
	\return success or failure code
	*/
	virtual SI_Error			GetValues(void *in_pValueStruct);

	//! Set the CSLFCurve that is associated with this process.
	/*!
	\param	CSLFCurve*	The CSLFcurve associated with this process
	\return success or failure code
	*/	
	void	SetFCurve ( CSLFCurve* );

	CSIBCString					temp;

	/**! class factory for runtime linked runtime processes
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

	//! Get the associated FCurve object
	/*!
	\return CSLFCurve*	A pointer to the FCurve object affected by this process
	*/
	CSLFCurve*					GetFCurve() { return m_pFCurve; };
private:

	
	CSLFCurve*					m_pFCurve;
	SI_Void						*m_pUnused;

};

#endif
