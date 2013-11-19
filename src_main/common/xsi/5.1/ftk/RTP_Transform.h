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

#ifndef _RTP_TRANSFORM_H_
#define _RTP_TRANSFORM_H_

#include <SIRTProcess.h>
#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <SIBCMatrix.h>
#include <Transform.h>
#include "RTP_BasicProcessesTypes.h"

SI_Error	RegisterSITransform ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* Transform runtime process
* 
* When evaluated, the transform runtime process will update the model's local and global matrix
*
*/
class XSIEXPORT CSIRTP_Transform : public CSIRTProcess
{
public:
	CSIRTP_Transform();
	~CSIRTP_Transform();

	static	SLTemplateCallback	m_sNativeCallbacks;

	///////////////////////////////////////////////////////////////////////
	// Methods ( inherited from SIRTProcess )
	///////////////////////////////////////////////////////////////////////

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::PREHIERARCHICAL;}

	//! The evaluate fonction will compute the local and global matrix for this node
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

	//! get bounding volume structure
	/*!
	\return CSIRTP_BoundingVolume*	A pointer to a structure holding the bounding volume for this Model
	*/

	CSIRTP_BoundingVolume*		GetBoundingVolume() { return &m_sBoundingVolume; };

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

	CSLTransform*				m_pTransform;
	CSIRTP_BoundingVolume		m_sBoundingVolume;
	SI_Void						*m_pUnused;

};

/** 
* BoundingVolume runtime process
* 
* When evaluated, the bounding volume will calculate the bounding volume for it's associated geometry.
* Note that this process is ignored if the model is affected by a DEFORMER such as CSIRTP_Envelope or CSI_RTP_ShapeAnimation.
* In this case, the DEFORMERS will handle the bounding volume calculations.
*
*/
class CSIRTP_BoundingVolumeUpdate : public CSIRTProcess
{
public:
	CSIRTP_BoundingVolumeUpdate();
	~CSIRTP_BoundingVolumeUpdate();

	///////////////////////////////////////////////////////////////////////
	// Methods ( inherited from SIRTProcess )
	///////////////////////////////////////////////////////////////////////

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::POSTHIERARCHICAL;}

	//! The evaluate fonction will compute the bounding volume of its associated mesh
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

	SI_Void	SetTransform		( CSLTransform* in_pTransform ) { m_pTransform = in_pTransform; };
	SI_Void	SetBoundingVolume	( CSIRTP_BoundingVolume* in_pBV ) { m_sBoundingVolume = in_pBV; };

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
	
	CSLTransform				*m_pTransform;
	CSIRTP_BoundingVolume		*m_sBoundingVolume;

	SI_Void						*m_pUnused;

};

#endif
