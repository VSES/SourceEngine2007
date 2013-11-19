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

#ifndef _RTP_ShapeAnimation_H_
#define _RTP_ShapeAnimation_H_

#include <SIRTProcess.h>
#include <SIBaseApplication.h>
#include <Scene.h>
#include <ShapeAnimation.h>
#include <BaseShape.h>
#include <RTP_Shader.h>

#include <RTP_BasicProcessesTypes.h>

SI_Error	RegisterSIShapeAnimation ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* ShapeAnimation runtime process
* 
* This runtime process will deform it's list of associated vertex clusters.
*
*/
class XSIEXPORT CSIRTP_ShapeAnimation : public CSIRTProcess
{
public:
	//! constructor for the CSIRTP_ShapeAnimation class
	/*! 
	\return A pointer to the new CSIRTP_ShapeAnimation
	*/
	CSIRTP_ShapeAnimation();

	//! destructor for the CSIRTP_ShapeAnimation class
	/*! 
	\return nothing
	*/
	~CSIRTP_ShapeAnimation();

	//! Set the CSLModel that is deformed by ShapeAnimation
	/*! 
	\param	CSLModel*	The model deformed by ShapeAnimation
	\return nothing
	*/
	SI_Error	SetDeformedModel ( CSLModel *in_pModel );

	//! Averages the vertex positions of 2 shapes
	/*! 
	\param	CSLBaseShape*		The first shape
	\param	CSLBaseShape*		The second shape
	\param	SLIndexedVector3D*	will contain the result of the weighting (vertices)
	\param	SLIndexedVector3D*	will contain the result of the weighting (normals)
	\param	SI_Float			Weight

	\return nothing
	*/
	SI_Error	AverageShapes ( CSLBaseShape *in_pShape1,
								CSLBaseShape *in_pShape2,
								SLIndexedVector3D *out_pVertexResult,
								SLIndexedVector3D *out_pNormalResult,
								SI_Float in_fWeight );
			
	virtual CSIRTProcessType	GetType() { return CSIRTProcess::DEFORM;};

	static	SLTemplateCallback	m_sNativeCallbacks;

	//! Evaluates the current shape
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

	CSLShapeAnimation*					m_pShapeAnimation;
	CSIRTP_BoundingVolume*				m_pBoundingVolume;
	CSIBCArray<CSIMaterialCluster>	*	m_pClusterList;
	SI_Int								m_iClusterCount;
	SLIndexedVector3D*					m_pAveragedVertexList;
	SLIndexedVector3D*					m_pAveragedNormalList;
	SI_Float							m_fLastEvaluation;
			
};



#endif
