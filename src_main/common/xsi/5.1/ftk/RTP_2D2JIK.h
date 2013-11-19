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

#ifndef _RTP_2D2IK_H_
#define _RTP_2D2IK_H_

#include <SIRTProcess.h>
#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <SIBCMatrix.h>
#include <FCurve.h>
#include "RTP_BasicProcessesTypes.h"
#include <IKRoot.h>
#include <IKJoint.h>

SI_Error	Register_2D2J_SIIKRoot ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* 2D / 2 joint IK Solver
* 
* This runtime process is a simple 2D/2 joint IK solver. Note that this process can only operate on
* chains that have exactly 2 joints. (1 root, 2 joints and 1 effector). Others types of systems cannot
* be solved with this process.
*
*/
class XSIEXPORT CSIRTP_2D2J_IK : public CSIRTProcess
{
public:
	CSIRTP_2D2J_IK();
	~CSIRTP_2D2J_IK();

	enum
	{
		UNKNOWN,
		POSITIVE,
		NEGATIVE
	};

	static	SLTemplateCallback	m_sNativeCallbacks;

	///////////////////////////////////////////////////////////////////////
	// Methods ( inherited from SIRTProcess )
	///////////////////////////////////////////////////////////////////////

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::HIERARCHICAL;}

	//! The evaluate fonction will apply the 2D/2J solver for the current frame
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

	//! Sets the Chain on which to operate. Pass in a CSLIKRoot* casted as a void pointer
	/*!
	\param	CSLIKRoot*	The ChainRoot on which to operate.
	\return SI_Error success or failure code
	*/	
	virtual SI_Error			SetValues(void *in_pValueStruct);

	//! get public values
	/*!
	\return SI_Error success or failure code
	*/
	virtual SI_Error			GetValues(void *in_pValueStruct);

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

private:

	CSLIKRoot					*m_pIKRoot;
	SI_Void						*m_pUnused;

	CSIBCVector3D				m_vRootScale;
	CSIBCVector3D				m_vRootRotation;
	CSIBCVector3D				m_vRootTranslation;
	SI_Bool						m_bMustInit;

	SI_Int						m_Angle1, m_Angle2;
	
};

#endif
