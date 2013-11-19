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

#ifndef _RTP_Envelope_H_
#define _RTP_Envelope_H_

#include <SIRTProcess.h>
#include <SIBaseApplication.h>
#include <Scene.h>
#include <Envelope.h>
#include <RTP_Shader.h>

#include <RTP_BasicProcessesTypes.h>

SI_Error	RegisterSIEnvelope ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

class CSIRTP_Bone
{
	public:
		CSIRTP_Bone() {m_pBoneNode = NULL;}
		~CSIRTP_Bone() {;}

		CSLModel				*m_pBoneNode;
		CSIBCMatrix4x4			m_InvertedBasePose;
		CSIBCMatrix4x4			m_Current;
		CSIBCArray< SI_Float >	m_Weights;				// Weights for each vertex, 0.0 ->1.0
		CSIBCArray< SI_Float >	m_FirstWeights;				// Weights for each vertex, 0.0 ->1.0
		CSIBCArray< SI_Int >	m_Indices;				// Weights for each vertex, 0.0 ->1.0
		CSIBCArray< SI_Int >	m_WeightedIndices;		// Weights for each vertex, 0.0 ->1.0
		CSIBCArray< SI_Int >	m_FirstWeightedIndices;		// Weights for each vertex, 0.0 ->1.0
};
/** 
* Class that evaluates an envelope 
* 
* When the SI_EnvelopeList template is found, an instance of this class is created. When it is evaluated, it 
* will deform the vertices according to their bone/weight information and update the bounding volume
*
*/
class CSIRTP_Envelope : public CSIRTProcess
{
public:
	//! constructor for the CSIRTP_Envelope class
	/*! 
	\return A pointer to the new CSIRTP_Envelope
	*/
	CSIRTP_Envelope();

	//! destructor for the CSIRTP_Envelope class
	/*! 
	\return nothing
	*/
	~CSIRTP_Envelope();

	//! Returns the classification of the Envelope process (DEFORM)
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() { return CSIRTProcess::DEFORM;};

	static	SLTemplateCallback	m_sNativeCallbacks;

	//! The evaluate fonction will calculate the effect of the envelope and will update the object's bounding volume
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

	//! Get the model affected by the envelope operator
	/*! 
	\return CSLModel*	The deformed model
	*/
	CSLModel*					GetEnvelope();

	//! Set the model affected by the envelope operator
	/*! 
	\param	CSLModel*	The model that will be deformed
	\return SI_Error	Error code
	*/
	SI_Void						SetEnvelope( CSLModel* in_pEnv );

	//! Add an CSLEnvelope object
	/*! 
	\param	CSLEnvelope*	The SemanticLayer object describing a bone and it's weights
	\return Nothing
	*/
	SI_Void						AddEnvelope ( CSLEnvelope* );

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

	CSIBCArray<CSIMaterialCluster>	*	m_pClusterList;
	SI_Int								m_iClusterCount;
	CSIBCArray<CSIRTP_Bone*>			m_EnvelopeList;
	CSLModel*							m_pEnvelope;
	SI_Void								*m_pUnused;

	CSIBCVector3D						*m_pSourceVertex;
	CSIBCVector3D						*m_pDestVertex;

	CSIRTP_BoundingVolume				*m_pBoundingVolume;
	SI_Int								m_iNumVertex;
		
};



#endif
