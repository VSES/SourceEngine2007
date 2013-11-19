/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE 
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE . 
 
COPYRIGHT NOTICE. Copyright © 1999 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

#ifndef _RTP_INSTANCE_H_
#define _RTP_INSTANCE_H_

#include <SIRTProcess.h>
#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <SIBCMatrix.h>
#include <Transform.h>
#include <Instance.h>
#include <Model.h>
#include "RTP_BasicProcessesTypes.h"

SI_Error	RegisterSIInstance ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );
XSIEXPORT	CSLModel *CreateSIInstance( CSLScene *in_pCurrentScene, CSLModel *in_pModel, CSLModel *in_pParentModel);

/**
* Structure holding information about a per-instance material clusters
*/
typedef struct tagMaterialInstanceCluster : public CSIMaterialCluster
{
	CSIBCMatrix4x4*			m_pInstanceGlobalMatrix;	/*!< This instance's global matrix*/
	
} CSIInstanceMaterialCluster;

/**
* Structure holding local information for a single instance
*/
typedef struct tagInstanceLocalData
{
	CSLModel								*pMaster;		/*!< The model which is instanced*/
	CSIBCMatrix4x4							LocalMatrix;	/*!< This instance's local matrix*/
	CSIBCMatrix4x4							GlobalMatrix;	/*!< This instance's global matrix*/
	CSIBCVector3D							scale;			/*!< This instance's scale (internal)*/
	CSIBCVector3D							rotation;		/*!< This instance's rotation (internal)*/
	CSIBCVector3D							translation;	/*!< This instance's translation (internal)*/
	CSIRTNode								*pCurrentNode;	/*!< This instance's node*/
	CSIRTNode								*pCurrentOriginalNode; /*!< The instanced model runtime node*/
	CSIBCArray<CSIInstanceMaterialCluster>	ClusterLists;			/*!< A list of material clusters for this instance (actually it's a copy of clusters from the original model. The instance needs this to update the material*/


} InstanceLocalData;

/** 
* Instance runtime process
* 
* The instance runtime process handles the evaluation of a single instance of a CSLModel
*
*/
class XSIEXPORT CSIRTP_Instance : public CSIRTProcess
{
public:
	CSIRTP_Instance();
	~CSIRTP_Instance();

	static	SLTemplateCallback	m_sNativeCallbacks;

	///////////////////////////////////////////////////////////////////////
	// Methods ( inherited from SIRTProcess )
	///////////////////////////////////////////////////////////////////////

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::POSTHIERARCHICAL;}

	//! The evaluate fonction will evaluate the instance 
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();
	virtual SI_Error			EvaluateRecursively(CSLModel	*in_pModel);
	virtual SI_Error			RestoreMatrices(CSLModel *in_pModel);

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

	//! initialize from a top node
	/*!
	\return success of failure code
	*/

	virtual SI_Error			InitializeFromTopNode(CSLModel *in_pTopNode, CSLModel *in_pParentModel);

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

	CSIRTP_SI_InstanceData		m_PublicData;
	SI_Void						*m_pUnused;

};

#endif
