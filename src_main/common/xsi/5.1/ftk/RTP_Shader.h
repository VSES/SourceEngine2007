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

#ifndef _RTP_SHADER_H_
#define _RTP_SHADER_H_

#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <RTP_BasicProcessesTypes.h>

SI_Error	RegisterXSIShader ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );
CSLTemplate	*XSI_Shader_RTPCallback(CdotXSITemplate *in_pTemplate, SLContext *in_pContext);
SI_Error	XSI_Shader_RTPPostLoadCallback(SLContext *in_pContext);

/** 
* Shader display runtime process
* 
* Responsible for calling a single realtime shader node.
*
*/
class XSIEXPORT CSIRTP_ShaderDisplay : public CSIRTProcess
{
public:
	//! constructor for the CSIRTP_ShaderDisplay class
	/*! 
	\return A pointer to the new CSIRTP_ShaderDisplay
	*/
	CSIRTP_ShaderDisplay();

	//! destructor for the CSIRTP_ShaderDisplay class
	/*! 
	\return nothing
	*/
	~CSIRTP_ShaderDisplay();

	static	SLTemplateCallback	m_sNativeCallbacks;

	//! Evaluates the shader
	/*! 
	\return Error code
	*/
	virtual SI_Error			Evaluate();

	//! get the type of process
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::RENDER;}

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

	//! convert shader param to PPG data format
	/*!
	\return success or failure code
	*/
	virtual SI_Error			ConvertShaderParam(CSIBCArray<SI_ULong*> &textureDomains);

	//! get requirements
	/*!
	\return success or failure code
	*/
	virtual SI_Int				GetRequirements(SI_Int in_lRequirement);

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
	CSIRTP_ShaderDisplayData	m_PublicData;
	CSIBCArray<SI_ULong>		m_PPGData;
	SI_Void						*m_pUnused;

	CSIBCArray<TinyVariant*>	m_PPGVolatileData;
	CSIBCArray<SI_ULong>		m_PPGVolatileDataIndices;		// indices into m_PPGData
};



#endif // _RTP_SHADER_H_
