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

#ifndef _SIRTPROCESS_
#define _SIRTPROCESS_

#include "SIBCUtil.h"
#include "SIBCNode.h"

class SI_ModelTime;

/** 
* Runtime process base class
* Runtime processes are attached to CSLTemplate objects via the runtime node (CSIRTNode). They are managed and executed by the runtime engine (CSIRuntimeEngine)
* They are classified into several types so that runtime engines can manage them without having explicit knowledge of their class definition. The classification is as follows:
* 
* <B> Model processes </B> 
*
* <I> These processes must be performed on all runtime nodes stored in the execution list of the runtime engine. </I>
*
* - PreController 	process is executed before any controller processes
* - Controller		updates a single value like animation fcurves or input devices (and eventually expressions)
* - PreDynamics 		process is executed before any dynamics processes
* - Dynamics 			updates several objects at the same time (rigid body dynamics, IK, RTK motion)
* - PreHierarchical 	process is executed before any hierarchical processes
* - Hierarchical 		updates things that are hierarchy dependant 
* - Post-Hierarchical	Process is executed after all hierarchical processes
* 
* <B> Primitive processes </B> 
*
* <I> These processes are performed on all runtime nodes that pass the visibility tests </I>
*
* - PreVisibility	process is excuted before any object visibility processes
* - Visibility 		updates object visibility 
* - PreDeform			process is executed before any deform processes
* - Deforms 			updates objects deformation (shape animation, envelopes)
* - PreRender			process is executed before any rendering processes
* - Render 			sets display settings or render an object
* - PostRender		process is executed after any rendering processes
* 
* \sa CSIRTNode
* \sa CSIRuntimeEngine
*/

class XSIEXPORT CSIRTProcess : public CSIBCNode
{

private:
	void				*m_pUnused;

protected:

public:
	
	/*! Runtime process classification
	*/
	enum CSIRTProcessType
	{
		UNKNOWN,			/*!< Unknown process type */
		PRECONTROLLER,		/*!< PreController */
		CONTROLLER,			/*!< Controller (like an FCurve or expression) */
		PREDYNAMICS,		/*!< PreDynamics */
		DYNAMICS,			/*!< Dynamics (for instance IK solving, rigid body dynamics)*/
		PREHIERARCHICAL,	/*!< PreHierarchical (before children are evaluated)*/
		HIERARCHICAL,		/*!< Hierarchical */
		POSTHIERARCHICAL,	/*!< PostHierarchical (after children have been evaluated)*/
		PREVISIBILITY,		/*!< PreVisibility (before visibility culling is done)*/
		VISIBILITY,			/*!< Visibility (determines the object's visibility)*/
		PREDEFORM,			/*!< PreDeform (before defomers are evaluated)*/
		DEFORM,				/*!< Deformer process (like an envelope or ShapeAnimation)*/
		PRERENDER,			/*!< Before the rendering process occurs*/
		RENDER,				/*!< Render process*/
		ZSORTRENDER,		/*!< ZSort render process*/
		POSTRENDER			/*!< After the rendering process*/
	};
	
	static SI_Void*	m_pApplication;

// Methods
private:

protected:

public:
    
	//*******************************************************************
	// constructor/destructor
	//*******************************************************************

	//! constructor for the CSIRTProcess class
	/*! 
	\return A pointer to the new CSIRTProcess
	*/
	CSIRTProcess();

	//! destructor for the CSIRTProcess class
	/*! 
	\return nothing
	*/
	~CSIRTProcess();

	/*! Class factory for runtime linked runtime processes.
	*
	* This is used when an external entity wants to create a process of this 
	* type without having to link at compile time. The mechanism is simple.
	* the external entity uses the class factory to create a new CSIRTProcess
	* and sets the initialization parameters in the user data "SIRTINIT".
	* The external entity then calls CSIRTProcess::Init to initialize the data
	* extracted from the user data
	*
	\param	CSIRTProcess**	Will hold a pointer to the newly created process
	\return a new CSIRTProcess
	*/
	virtual SI_Error			CreateRTProcess( CSIRTProcess** out_pProcess );

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

	//*******************************************************************
	// accessors
	//*******************************************************************

	//! destructor for the CSIRTProcess class
	/*! 
	\return the classification of the runtime process
	*/
	virtual CSIRTProcessType	GetType();

	//*******************************************************************
	// methods
	//*******************************************************************

	//! evaluation function
	/*! 
	\return the success or failure of the evaluation
	*/
	virtual SI_Error			Evaluate();

	//! evaluation function
	/*! 
	\return the success or failure of the evaluation
	*/
	static SI_Void*				GetBaseApplication();

	//! evalutation function
	/*!
	\return the current frame for evaluation
	*/
	SI_Float					GetFrame();

	//! Get dirty state 
	/*! 
	\return SI_TRUE the process is dirty, FALSE, it has been evaluated
	*/
	SI_UInt						GetDirtyState();

	//! Set dirty state
	/*! 
	\param	Sets the dirty state for this process
	\return Nothing
	*/
	SI_Void						SetDirtyState( SI_UInt in_uiState );

	//! Set Activeness
	/*! 
	\param	SI_Bool	SI_TRUE for Active, SI_FALSE for inactive
	\return Nothing
	*/
	SI_Void						SetActive( SI_Bool in_bActive ) { m_bActive = in_bActive; };

	//! Get Activeness
	/*! 
	\param	nothing
	\return SI_Bool	SI_TRUE process is active, SI_FALSE it is inactive
	*/
	SI_Bool						GetActive() { return m_bActive; };
	
private:

	SI_UInt						m_uiDirty;
	SI_Bool						m_bActive;

};

#endif // _SIRTPROCESS_
