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

#ifndef _RTP_CONSTRAINT_H_
#define _RTP_CONSTRAINT_H_

#include <SIRTProcess.h>
#include <Scene.h>
#include <SIBCUtil.h>
#include <SIBaseApplication.h>
#include <SIBCMatrix.h>
#include <Transform.h>
#include "RTP_BasicProcessesTypes.h"

SI_Error	RegisterSIConstraint ( CSIBaseApplication* in_pApplication, CSLScene *in_pCurrentScene );

/** 
* Constraint runtime process
* 
* The constraint runtime process handles the evaluation of position, scaling, orientation, direction and interest constraints
*
* <B> Example - A Contraint Runtime Process </B>
\code
SI_Error	CSIRTP_Constraint::Evaluate()
{

	CSLModel**	l_pSources = m_pConstraint->ConstrainingObjects ();
	SI_Int		l_iSourceCount = m_pConstraint->GetConstrainingObjectCount ();

	CSIBCMatrix4x4&	l_mTargetMatrix = m_pConstraint->ParentModel()->Transform ()->GetGlobalMatrix ();

	for (SI_Int i=0;i<l_iSourceCount;i++)
	{
		CSIBCMatrix4x4	l_mSrcGlobalMatrix;
		l_mSrcGlobalMatrix = l_pSources[i]->Transform()->GetGlobalMatrix();

		switch ( m_pConstraint->GetConstraintType() )
		{
			case CSLConstraint::SI_POSITION:
			{
				CSIBCVector3D	l_pPosition;
				l_mSrcGlobalMatrix.GetTranslation( l_pPosition );

				l_mTargetMatrix.SetTranslation ( l_pPosition );
				
				break;
			}

			case CSLConstraint::SI_SCALING:
			{
				CSIBCVector3D	l_pScaling;
				l_mSrcGlobalMatrix.GetScaling( l_pScaling );

				l_mTargetMatrix.SetScaling ( l_pScaling );

				break;
			}

			case CSLConstraint::SI_ORIENTATION:
			{
				CSIBCVector3D	l_pOrientation;
				l_mSrcGlobalMatrix.GetRotation( l_pOrientation );

				l_mTargetMatrix.SetRotation( l_mSrcGlobalMatrix );

				break;
			}

		}
		
	}

	return SI_SUCCESS;
}
\endcode
*/
class XSIEXPORT CSIRTP_Constraint : public CSIRTProcess
{
public:
	CSIRTP_Constraint();
	~CSIRTP_Constraint();

	static	SLTemplateCallback	m_sNativeCallbacks;

	///////////////////////////////////////////////////////////////////////
	// Methods ( inherited from SIRTProcess )
	///////////////////////////////////////////////////////////////////////

	//! Returns the classification of the Constraint process (HIERARCHICAL)
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType() {return CSIRTProcess::HIERARCHICAL;}

	//! The evaluate fonction will calculate the effect of the constraint and will update the node's global matrix
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

	CSLConstraint				*m_pConstraint;
	SI_Void						*m_pUnused;

};

#endif
