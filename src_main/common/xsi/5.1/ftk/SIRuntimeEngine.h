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

#ifndef _SIRUNTIMEENGINE_H_
#define _SIRUNTIMEENGINE_H_

#include <SIBCUtil.h>
#include <SIBCArray.h>

#include <Scene.h>
#include <Model.h>
#include <MaterialLibrary.h>

#include "SIRTNode.h"

/** 
* Runtime Engine class
* The Runtime Engine class is used to hold the scenes and evaluates all Runtime Nodes attached to it and their dependencies.
* 
* \b Usage
*
* Add a new CSLScene using the AddScene method. Then, evaluate the whole scene graph by calling EvaluatePass with these
* enums (the order is important)
*
* EvaluatePass ( MODEL );
* This will cause all Model Runtime Processes (FCurves, Hierarchical etc. ) to be evaluated. 
*
* EvaluatePass ( PRIMITIVE );
* Evaluates all Primitive Runtime Processes (Visibility, Deforms, Render etc.)
*	
* EvaluatePass ( MATERIAL );
* Loops thru the list of materials and send the triangles collected during the primitive pass to the video card
*
* \sa CSLScene
* \sa CSIRTNode
* \sa CSIRTProcess
*/

class XSIEXPORT CSIRuntimeEngine 
{
public:

	/** 
	* Enum for possible types of passes
	*/
	enum EEvaluationPass
	{
		/// Evaluatation of the Model Runtime Nodes
		MODEL,		
		/// Evaluatation of the Primitive Runtime Nodes
		PRIMITIVE,
		/// Evaluatation of the Material (Render) Runtime Nodes
		MATERIAL
	};

	//*******************************************************************
	// constructor/destructor
	//*******************************************************************

	
	/*! constructor for the CSIRuntimeEngine class
	\return A pointer to the new CSIRuntimeEngine
	*/
	CSIRuntimeEngine();

	/*! destructor for the CSIRuntimeEngine class
	\return nothing
	*/
	virtual ~CSIRuntimeEngine();

	/*! Resets the engine to it's initial state and free all memory it's using
	\return nothing
	*/
	void		ResetEngine();

	//*******************************************************************
	// Scene Management
	//*******************************************************************

	/*! Adds a scene to the Runtime Engine.
	\param in_pScene A pointer to a CSLScene
	\return SI_Error describing if the operation succeeded or failed
	*/
	SI_Error	AddScene		( CSLScene *in_pScene );

	/*! Removes a scene from the Runtime Engine.
	\param in_pScene A pointer to a CSLScene
	\return SI_Error describing if the operation succeeded or failed
	*/
	SI_Error	RemoveScene		( CSLScene *in_pScene );

	/*! Removes all scenes.
	* This is similar to the ResetEngine except that the Runtime Engine states aren't touched.
	\return SI_Error describing if the operation succeeded or failed
	*/
	SI_Error	RemoveAllScenes ();

	/*! Get the number of scenes in the engine.
	\return SI_Int The number of scenes in the Runtime Engine
	*/
	SI_Int		GetNbScenes		();

	/*! Get a particular scene (by index) from the Runtime Engine.
	\param	SI_Int		Zero based index of the scene you are requesting
	\return CSLScene*	A pointer to the scene or NULL if the index is out of range.
	*/
	CSLScene*	GetScene		( SI_Int in_iIndex );

	//! Finds and returns the index of the scene in the list of associated scenes
	/*! 
	\return CSLScene*	The scene we're looking for
	*/
	virtual SI_Int		GetSceneIndex( CSLScene* );


	//*******************************************************************
	// runtime node info
	//*******************************************************************

	CSIBCArray<CSIRTNode *>	&ModelRTNodes();
	CSIBCArray<CSIRTNode *>	&OrderedModelRTNodes();
	CSIBCArray<CSIRTNode *>	&OpaqueRenderingRTNodes();
	CSIBCArray<CSIRTNode *> &NonOpaqueRenderingRTNode();

	//*******************************************************************
	// Evaluation
	//*******************************************************************

	/*! Evaluates a single pass.
	\param	EEvaluationPass		The pass to evaluate (see above)
	\return SI_Error describing if the operation succeeded or failed
	*/
	SI_Error	EvaluatePass	( EEvaluationPass in_ePass );






private:

	SI_Error				EvaluateModelRTNodes		();
	SI_Error				EvaluatePrimitiveRTNodes	();

	/*!
	*\todo Add transparent material sort once the material RT process has been created
	*/
	SI_Error				EvaluateMaterialRTNodes		();

	/*!
	\todo Add Scene Model Runtime Models here! like Ambience and Fog
	*/
	SI_Error				TraverseScene ( CSLScene * );	// Traverse a scene and populate the RTNode lists
	SI_Error				TraverseModel ( CSLModel * );	// Traverse a model and populate the RTNode lists
	SI_Error				TraverseMaterialLibrary ( CSLMaterialLibrary* ); // Traverse a materia library and populate the Material RTNode list
public:
	/*! Add a single Runtime node to the runtime engine.
	\param	CSIRTNode*		The new Runtime node
	\return SI_Error		Error code
	\sa CSIRTNode
	*/
	SI_Error				AddModelRTNode		( CSIRTNode* );	// Add a new Model Runtime Node
	/*! Synchronizes the system with what has been removed.
	\return SI_Error		Error code
	*/
	SI_Error				Synchronize	( );
private:

	SI_Error				EvaluateModelNode ( CSIRTNode * );	// Evaluates a single node

	CSIBCArray<CSLScene *>	m_vSceneList;					// Scene list array

	CSIBCArray<CSIRTNode *>	m_pModelRTNodes;				// Model Runtime Node list
	CSIBCArray<CSIRTNode *>	m_pModelRTNodesOptimized;		// Optimized Model Runtime Node list

	CSIBCArray<CSIRTNode *>	m_pOpaqueMaterialRTNodes;		// Opaque Material Runtime Node list
	CSIBCArray<CSIRTNode *>	m_pNonOpaqueMaterialRTNodes;	// Non-opaque Material Runtime Node list

	SI_Bool					m_bDependenciesAreClean;		// SI_TRUE When dependencies have been evaluated
															// and m_pModelRTNodesOptimized is filled

	SI_UInt					m_uiGlobalDirtyState;			// Global dirty flag

};

#endif

