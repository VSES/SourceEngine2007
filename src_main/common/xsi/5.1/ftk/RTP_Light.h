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

#ifndef _RTP_Light_H_
#define _RTP_Light_H_

#include <SIRTProcess.h>
#include <SIBaseApplication.h>
#include <Scene.h>
#include <Light.h>
#include <RTP_Shader.h>

#include <RTP_BasicProcessesTypes.h>

SI_Error	RegisterSILight ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* Light runtime process
* 
* Upon evaluation, the light runtime process is responsible for setting light parameters in the display driver
*
*/
class XSIEXPORT CSIRTP_Light : public CSIRTProcess
{
public:
	//! constructor for the CSIRTP_Light class
	/*! 
	\return A pointer to the new CSIRTP_Light
	*/
	CSIRTP_Light();

	//! destructor for the CSIRTP_Light class
	/*! 
	\return nothing
	*/
	~CSIRTP_Light();

	//! Sets the CSLLight object referenced by the process
	/*! 
	\param	CSLLight*	The Semantic Layer light object
	\return nothing
	*/
	SI_Void		SetLight				( CSLLight* in_pLight ) { m_pLight = in_pLight; };

	virtual CSIRTProcessType	GetType() { return CSIRTProcess::HIERARCHICAL;};

	static	SLTemplateCallback	m_sNativeCallbacks;

	//! Calls the display driver to set the light parameters
	/*! 
	\return SI_Error			Success or failure
	*/
	virtual SI_Error			Evaluate();

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

	CSLLight							*m_pLight;
	SI_Void								*m_pUnused;
		
};



#endif
