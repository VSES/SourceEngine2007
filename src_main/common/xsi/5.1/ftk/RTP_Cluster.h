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

#ifndef _RTP_Cluster_H_
#define _RTP_Cluster_H_

#include <SIRTProcess.h>
#include <SIBaseApplication.h>
#include <Scene.h>
#include <Cluster.h>
#include <RTP_Shader.h>
#include <RTP_BasicProcessesTypes.h>
#include <SIBCArray.h>

SI_Error	RegisterSICluster ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

typedef struct	_ClusterRef
{
	S3DVertexArrayVertex	*m_vPosition;
	SI_Int					m_iRef;
} ClusterRef;

/** 
* Cluster runtime process
* 
* The cluster runtime process handles the evaluation of Clusters.
*
*/
class XSIEXPORT CSIRTP_Cluster : public CSIRTProcess
{
public:
	//! constructor for the CSIRTP_Cluster class
	/*! 
	\return A pointer to the new CSIRTP_Cluster
	*/
	CSIRTP_Cluster();

	//! destructor for the CSIRTP_Cluster class
	/*! 
	\return nothing
	*/
	~CSIRTP_Cluster();

	SI_Void	SetCluster ( CSLCluster *in_pCluster );

	virtual CSIRTProcessType	GetType() { return CSIRTProcess::DEFORM;};

	static	SLTemplateCallback	m_sNativeCallbacks;

	//! The evaluate fonction will transform the vertex cluster according to their center and will update the bounding volume information
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

	/**! class factory for runtime linked runtime processes
	* This is used when an external entity wants to create a process of this 
	* type without having to link at compile time. The mechanism is simple.
	* the external entity uses the class factory to create a new CSIRTProcess
	* and sets the initialization parameters in the user data "SIRTINIT".
	* The external entity then calls CSIRTProcess::Init to initialize the data
	* extracted from the user data
	*
	\param CSIRTProcess**	A pointer to a CSIRTProcess pointer
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess**out_pProcess );

private:
	CSLCluster*							m_pCluster;
	CSLModel*							m_pClusterCenter;
	CSIBCVector3D*						m_pSourceVertex;
	CSIBCVector3D*						m_pDestVertex;
	CSIBCVector3D*						m_pFinalVertex;
	
	CSIBCArray<ClusterRef>				m_pClusterRefs;
	SI_Bool								m_bMustInit;

	CSIBCArray<CSIMaterialCluster>	*	m_pClusterList;
	SI_Int								m_iClusterCount;

	CSIRTP_BoundingVolume*				m_pBoundingVolume;

	SI_Void								*m_pUnused;

		
};



#endif
