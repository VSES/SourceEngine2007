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

#ifndef _SIRTNODE_
#define _SIRTNODE_

#include "SIBCUtil.h"
#include "SIBCNode.h"
#include "Template.h"
#include "SIRTProcess.h"

#ifdef __cplusplus
extern "C"
{
	XSIEXPORT void SI_RTNode_ReleaseMethod(void *in_pObjectToDelete);
}
#endif

/** 
* Runtime node base class
* The runtime node class contains the list of processes attached to a CSLTemplate. It also maintains the runtime states of the CSLTemplate such as dependencies..
* 
* The runtime node is attached to a CSLTemplate via its user data. The runtime data's user data tag will be identified as "XSIRTD". 
*
\b Example - Retrieving the Runtime node object from a CSLTemplate:
\code
CSIRTNode			*pRTNode		= NULL;
CSIBCUserData		*pUserData	= NULL;

//
// The runtime node is attached to the CSLTemplate as user data and it's tag is "XSIRTD"
//

pUserData = l_pTemplate->FindUserData(_SI_TEXT("XSIRTD"));

if(pUserData != NULL)
{	
	//
	// We've found user data with a "XSIRTD" tag, so cast it to a CSIRTNode*
	//
	
	pRTNode = (CSIRTNode *) pUserData->GetData();

	//
	// Now do something with the runtime node like access it's model processes:
	//

	CSIBCArray<CSIRTProcess*>& l_pModelProcesses = pRTNode->GetModelProcessList ();

}
\endcode
\sa CSIRTProcess
*/

class XSIEXPORT CSIRTNode : public CSIBCNode
{

private:

	SI_UInt						m_lModelDirtyFlag;
	SI_UInt						m_lPrimitiveDirtyFlag;

	CSLTemplate					*m_pOwner;
	CSIBCArray<CSIRTProcess *>	m_ModelProcesses;
	CSIBCArray<CSIRTNode *>		m_Dependencies;
	CSIBCArray<CSIRTProcess *>	m_PrimitiveProcesses;
	void						*m_pUnused;

protected:

public:
		
// Methods
private:

protected:

public:
    
	//*******************************************************************
	// constructor/destructor
	//*******************************************************************

	//! constructor for the CSIRTNode class
	/*! 
	\return A pointer to the new CSIRTNode
	*/
	CSIRTNode();

	//! destructor for the CSIRTNode class
	/*! 
	\return nothing
	*/
	~CSIRTNode();

	//*******************************************************************
	// accessors
	//*******************************************************************

	//! get model dirty flag 
	/*! 
	\return the model dirty flag value
	*/
	SI_UInt						GetModelDirtyFlag();

	//! get primitive dirty flag 
	/*! 
	\return the primitive dirty flag value
	*/
	SI_UInt						GetPrimitiveDirtyFlag();

	//! set the model dirty flag 
	/*! 
	\param	in_lFlag setting the new flag value
	\return nothing
	*/
	SI_Void						SetModelDirtyFlag(SI_UInt in_lFlag);

	//! set the primitive dirty flag 
	/*! 
	\param	in_lFlag setting the new flag value
	\return nothing
	*/
	SI_Void						SetPrimitiveDirtyFlag(SI_UInt in_lFlag);

	//! get the CSLTemplate owner
	/*! 
	\return the owner
	*/
	CSLTemplate					*GetOwner();

	//! set the CSLTemplate owner
	/*! 
	\param	in_pOwner setting the owner of that SIRTNode
	\return nothing
	*/
	SI_Void						SetOwner(CSLTemplate *in_pOwner);

	//! get the list of model processes
	/*! 
	\return CSIBCArray<CSIRTProcess*> containing the list of model processes
	*/
	CSIBCArray<CSIRTProcess*>	&GetModelProcessList();

	//! get the list of primitive processes
	/*! 
	\return CSIBCArray<CSIRTProcess*> containing the list of primitive processes
	*/
	CSIBCArray<CSIRTProcess*>	&GetPrimitiveProcessList();

	//! get the list of dependencies
	/*! 
	\return CSIBCArray<CSIRTNode*> containing the list of dependencies
	*/
	CSIBCArray<CSIRTNode*>		&GetDependencyList();

	//*******************************************************************
	// Methods
	//*******************************************************************
	//! attach itself to an existing template
	/*! 
	\return SI_Error describing if the operation succeeded or failed
	*/
	SI_Error					AttachToTemplate(CSLTemplate	*in_pTemplate);

	//! Sorts the model and primitive runtime lists
	/*! 
	\return SI_Error describing if the operation succeeded or failed
	*/
	SI_Error					SortProcesses();
	
private:
	//! Compare function for the qsort process
	/*!
	\param	CSIRTProcess*		Runtime process to compare
	\param	CSIRTProcess*		Runtime process to compare
	\return SI_Int				Result of the comparison
	*/
	static SI_Int				CompareProc( const void *arg1, const void *arg2 );
	
};

#endif // _SIRTNODE_
