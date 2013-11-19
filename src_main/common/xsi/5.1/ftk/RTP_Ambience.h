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

#ifndef _RTP_Ambience_H_
#define _RTP_Ambience_H_

#include <SIRTProcess.h>
#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <Ambience.h>
#include <RTP_BasicProcessesTypes.h>

SI_Error	RegisterSIAmbience ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* This Runtime Process handles the SI_Ambience template in a dotXSI. When this template is found by the viewer, a CSIRTP_Ambience 
* process is created. When evaluated, this process will set the ambient light in the Display Driver.
* 
* \sa CSIAmbience
* \sa CSIDisplayDriver::SetAmbientColor
*/
class XSIEXPORT CSIRTP_Ambience : public CSIRTProcess
{
public:
	CSIRTP_Ambience();
	~CSIRTP_Ambience();

	static	SLTemplateCallback	m_sNativeCallbacks;

	///////////////////////////////////////////////////////////////////////
	// Methods ( inherited from SIRTProcess )
	///////////////////////////////////////////////////////////////////////

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	\note The CSIRTP_Ambience process is a PreController process simply because we want to set the ambient light before we do anything else.
	\sa CSIRTProcess::CSIRTProcessType
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::PRECONTROLLER;}

	//! The evaluate fonction will call the display driver to set the current ambient light
	/*! 
	\return the success or failure of the evaluation
	*
	*<B> Example 1 - How the CSIRTP_Ambience process works </B>
\code
SI_Error	CSIRTP_Ambience::Evaluate()
{
	//
	// Get a pointer to the base application (this pointer is stored as a static member under CSIRTProcess for fast access.
	//

	CSIBaseApplication* l_pBaseApp = (CSIBaseApplication *)GetBaseApplication();

	//
	// Get the display driver
	//

	CSIDisplayDriver*	l_pDisplayDriver = l_pBaseApp->GetDisplayDriver ();

	_SI_ASSERT ( l_pDisplayDriver != NULL );

	//
	// Set the ambient color. The m_pAmbience member was set when the CSIRTP_Ambience was first created. It is
	// simple pointing to the Semantic Layer object wrapping the dotXSI SI_Ambience template.
	//

	l_pDisplayDriver->SetAmbientColor ( m_pAmbience->GetColor () );

	return SI_SUCCESS;

}
\endcode
	*/
	virtual SI_Error			Evaluate();

	//! Sets the CSLAmbience* on which to operate
	/*!
	\param CSLAmbience* Pointer to the CSLAmbience that describes the ambient light
	\return success or failure code
	*/	
	virtual SI_Error			SetValues(void *in_pValueStruct);

	//! get public values
	/*!
	\return success or failure code
	*/
	virtual SI_Error			GetValues(void *in_pValueStruct);

	/*! Class factory for runtime linked runtime processes.
	*
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

	CSLAmbience*				m_pAmbience;
	SI_Void						*m_pUnused;

	
};
#endif
