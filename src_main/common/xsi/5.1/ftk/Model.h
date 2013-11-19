//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
//
//***************************************************************************************

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

#ifndef _MODEL_H
#define _MODEL_H

#include "Template.h"
#include "ConstrainableType.h"

// Forward declaration
class CSLVisibility;
class CSLCluster;
class CSLTransform;
class CSLGlobalMaterial;
class CSLPrimitive;
class CSLMixer;
class CSLEnvelope;
class CSLFXTree;
class CSLXSITransform;
class CSLXSIBasePose;

class CSLModelExtension_5_0
{
public:
	CSLModelExtension_5_0() {m_pXSITransform = NULL; m_pXSIBasePose = NULL;};
	~CSLModelExtension_5_0() {};

	CSLXSITransform* m_pXSITransform;
	CSLXSIBasePose* m_pXSIBasePose;
	void *m_pReserved;					// reserved for future expansion
};

//! Handles the Model template
class XSIEXPORT CSLModel
    : public CSLTemplate
	, public CSLConstrainableType
{
public:
	/*! Constructor
	* \param in_pScene Parent Scene
	* \param in_pModel Parent Model
	* \param in_pTemplate Parent Template
	* \return an instance of CSLModel
	*/
    CSLModel(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
    virtual ~CSLModel();

	/*! Updates the template with information from this instance
	* \return SI_Error Whether or not the template could be updated
	*/
    virtual SI_Error Synchronize();

	/*! Returns the type of this template: SI_Model
	* \return SI_Model
	*/
    virtual ETemplateType Type();

	/*! Gets a handle on one of this model's parameters by using its name
	* \param in_szName the name of the parameter to get the handle from
	* \return CSLAnimatableType* the handle to the parameter
	*/
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

	/*! Fixes all problems in the model to ensure it is valid
	* \return SI_Error Whether or not the model could be fixed
	*/
	virtual SI_Error Fix();

	/*! Add a cluster to the cluster list
	* \return CSLCluster Pointer to the newly added cluster
	*/
    CSLCluster* AddCluster();

	/*! Add a constraint to the constraint list
	* \return CSLConstraint Pointer to the newly added constraint
	*/
	CSLConstraint* AddConstraint(CSLConstraint::EConstraintType in_ConstraintType);
	
	/*! Removes a cluster from the cluster list
	* \param in_nIndex Index of the cluster to remove
	* \return SI_Error Whether or not the cluster could be removed
	*/
	SI_Error RemoveCluster( SI_Int in_nIndex );

	/*! Removes a cluster from the cluster list
	* \param in_pToRemove Pointer of the cluster to remove
	* \return SI_Error Whether or not the cluster could be removed
	*/
	SI_Error RemoveCluster( CSLCluster *in_pToRemove );

	/*! Gets the total number of clusters in the cluster list
	* \return Number of clusters
	*/
	SI_Int GetClusterCount();

	/*! Gets the total number of envelopes in the envelope list
	* \return Number of envelopes
	*/
	SI_Int GetEnvelopeCount();

	/*! Gets a pointer to the internally-stored cluster pointer list
	* \return CSLCluster Pointer to the list
	*/
	CSLCluster** GetClusterList();

	/*! Gets a pointer to the internally-stored envelope pointer list
	* \return CSLEnvelope Pointer to the list
	*/
	CSLEnvelope** GetEnvelopeList();

	/*! Removes all clusters from the internally-stored list and deallocate them
	* \return SI_Error Whether or not removal was a success
	*/
	SI_Error ClearClusters();

	/*! Removes the reference to the specified envelope
	* \param in_pToUnconnect Pointer to the envelope whose reference is to be removed
	* \return SI_Error Whether or not the removal was a success
	*/
	SI_Error UnconnectEnvelope( CSLEnvelope* in_pToUnconnect );

	/*! Adds a reference to an existing cluster
	* \param in_pNewCluster Pointer to the new object to refer to
	* \return Pointer to the new reference
	*/
	CSLCluster* ConnectCluster( CSLCluster* in_pNewCluster );

	/*! Adds a reference to an existing envelope
	* \param in_pNewCluster Pointer to the new object to refer to
	* \return Pointer to the new reference
	*/
	CSLEnvelope* ConnectEnvelope( CSLEnvelope* in_pNewEnvelope );
	
	/*! Returns the base position
	* \return CSLTransform Pointer to the base position
	*/
    CSLTransform* GetBasePose();

	/*! Allocates and connects a new base position to the model
	* \return CSLTransform Pointer to the newly allocated base position
	*/
    CSLTransform* CreateBasePose();

	/*! Deallocates the base position
	* \return SI_Error Whether or not the base position was removed
	*/
    SI_Error DestroyBasePose();

	/*! Returns the global material
	* \return CSLGlobalMaterial Pointer to the global material
	*/
    CSLGlobalMaterial* GlobalMaterial();

	/*! Adds a new GlobalMaterial to this model
	* \return Success or Error
	*/
    SI_Error AddGlobalMaterial();

	/*! Returns the transform
	* \return CSLTransform Pointer to the transform
	*/
	CSLTransform* Transform();

	/*! Returns visibility
	* \CSLVisibility Pointer to the visibility
	*/
    CSLVisibility* Visibility();
    
	/*! Returns the parent of the model
	* \return CSLModel Pointer to the parent
	*/
	CSLModel* Parent();

	/*! Returns the primitive of the model
	* \return CSLPrimitive Pointer to the primitive
	*/
    CSLPrimitive* Primitive();

	/*! Connects a mixer to the model
	* \param in_pNewMixer Mixer to connect
	* \return CSLMixer Pointer to the newly connected mixer
	*/
	CSLMixer* ConnectMixer(CSLMixer *in_pNewMixer);

	/*! Allocates and connects a new mixer to the model
	* \return CSLMixer Pointer to the newly allocated mixer
	*/
	CSLMixer* CreateMixer();

	/*! Deallocates the mixer
	* \return SI_Error Whether or not the mixer was removed
	*/
	SI_Error DestroyMixer();

	/*! Returns the mixer
	* \return CSLMixer Pointer to the mixer
	*/
	CSLMixer* Mixer();

	/*! Connects to an existing model
	* \param in_pNewModel Pointer to the model to connect
	* \return CSLModel Pointer to the newly connected model
	*/
	CSLModel* ConnectModel(CSLModel *in_pNewModel);

	/*! Connects to an existing visibility
	* \param in_pNewVisibility Pointer to the visibility to connect
	* \return CSLVisibility Pointer to the newly connected visibility
	*/
	CSLVisibility* ConnectVisibility(CSLVisibility *in_pNewVisibility);

	/*! Connects to an existing transform
	* \param in_pNewTransform Pointer to the transform to connect
	* \return CSLTransform Pointer to the newly connected transform
	*/
	CSLTransform* ConnectTransform(CSLTransform *in_pNewTransform);

	/*! Connects to an existing base position
	* \param in_pNewBasePose Pointer to the base pose to connect
	* \return CSLTransform Pointer to the newly connected base pose
	*/
	CSLTransform* ConnectBasePose(CSLTransform *in_pNewBasePose);

	/*! Connects to an existing primitive
	* \param in_pNewPrimitive Pointer to the primitive to connect
	* \return CSLPrimitive Pointer to the newly connected primitive
	*/
	CSLPrimitive* ConnectPrimitive(CSLPrimitive *in_pNewPrimitive);

	/*! Connects to an existing global material
	* \param in_pNewGlobalMaterial Pointer to the global material to connect
	* \return CSLGlobalMaterial Pointer to the newly connected global material
	*/
	CSLGlobalMaterial* ConnectGlobalMaterial(CSLGlobalMaterial* in_pNewGlobalMaterial);
	
	// Primitive and children manipulation.
	
	/*! Returns the primitive type of this model
	* \return ETemplateType Type of the primitive
	*/
	ETemplateType GetPrimitiveType();

	/*! Adds a camera to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddCamera();

	/*! Adds a curve list to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddCurveList();

	/*! Adds a directional light to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddDirectionalLight();

	/*! Adds an IK Chain to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddIKChain(SI_Int in_nNbJoints);

	/*! Adds an infinite light to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddInfiniteLight();

	/*! Adds an instance to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddInstance();

	/*! Adds a mesh to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddMesh();

	/*! Adds a xsi mesh to the children list
	* \return Pointer to the new children
	*/
	CSLModel* AddXSIMesh();

	/*! Adds a model to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddModel();

	/*! Adds a null object to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddNull();

	/*! Adds a patch to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddPatch();

	/*! Adds a point light to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddPointLight();

	/*! Adds a spot light to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddSpotLight();

	/*! Adds a surface mesh to the children list
	* \return Pointer to the new children
	*/
    CSLModel* AddSurfaceMesh();

	/*! Gets the children of the model
	* \return CSLModel List of the children
	*/
    CSLModel** GetChildrenList();

	/*! Deallocates all the children of the model
	* \return SI_Error Whether or not clearance was a success
	*/
    SI_Error ClearChildren();

	/*! Gets the number of children
	* \return SI_Int Number of children
	*/
    SI_Int GetChildrenCount();

	/*! Removes a child
	* \param in_pChild Pointer to the child to remove
	* \return SI_Error Whether or not removal was a success
	*/
    SI_Error RemoveChild(CSLModel* in_pChild);

	/*! Removes a child
	* \param in_nIndex Index of the child to remove
	* \return SI_Error Whether or not removal was a success
	*/
    SI_Error RemoveChild(SI_Int in_nIndex);

	/*! Sets a new parent for the model
	* \param in_pNewParent The new parent
	* \return SI_Error Whether or not reparenting was a success
	*/
	SI_Error Reparent(CSLModel *in_pNewParent);

	/*! Adds an FX tree to the FX tree list
	* \return CSLFXTree Pointer to the newly added FX tree
	*/
	CSLFXTree* AddFXTree();

	/*! Removes an FX tree from the FX tree list
	* \param in_nIndex Index of the FX tree to remove
	* \return SI_Error Whether or not the FX tree could be removed
	*/
	SI_Error RemoveFXTree( SI_Int in_nIndex );

	/*! Removes an FX tree from the FX tree list
	* \param in_pToRemove Pointer to the FX tree to remove
	* \return SI_Error Whether or not the FX tree could be removed
	*/
	SI_Error RemoveFXTree( CSLFXTree* in_pToRemove );

	/*! Removes all connections points from the FX tree list
	* \return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearFXTrees();

	/*! Adds an exisiting FX tree to the FX tree list
	* \param in_pToConnect Pointer to the FX tree to add
	* \return The added FX tree or null if it already exists in the list
	*/
	CSLFXTree* ConnectFXTree( CSLFXTree* in_pToConnect );

	/*! Gets a pointer to the internally-stored FX tree pointer list
	* \return CSLFXTree Pointer to the list
	*/
	CSLFXTree** GetFXTreeList();

	/*! Gets the total number of FXTrees 
	* \return SI_Int Number of FXTrees
	*/
	SI_Int GetFXTreeCount();

	/*! Searches for a given FX tree
	* \param in_szFXTreeName Name of the FX tree to look for
	* \return CSLFXTree Pointer to the FX tree or null if it cannot be found
	*/
	CSLFXTree* FindFXTree( SI_Char* in_szFXTreeName );

	/*! Returns the XSI transform
	* \return CSLXSITransform Pointer to the transform
	*/
	CSLXSITransform* XSITransform();

	/*! Connects to an existing XSI transform
	* \param in_pNewTransform Pointer to the transform to connect
	* \return CSLXSITransform Pointer to the newly connected transform
	*/
	CSLXSITransform* ConnectXSITransform(CSLXSITransform *in_pNewTransform);

	/*! Returns the XSI base pose
	* \return CSLXSITransform Pointer to the base pose
	*/
	CSLXSIBasePose* GetXSIBasePose();

	/*! Connects to an existing XSI base pose
	* \param in_pNewBasePose Pointer to the basepose to connect
	* \return CSLXSIBasePose Pointer to the newly connected basepose
	*/
	CSLXSIBasePose* ConnectXSIBasePose(CSLXSIBasePose *in_pNewBasePose);

	/*! Allocates and connects a new XSI base position to the model
	* \return CSLXSIBasePose Pointer to the newly allocated base position
	*/
    CSLXSIBasePose* CreateXSIBasePose();

	/*! Deallocates the XSI base pose
	* \return SI_Error Whether or not the base pose was removed
	*/
    SI_Error DestroyXSIBasePose();

	/*!	Return the name (string) of a specified parameter.
		\param	in_pParameter	The parameter
		\return	SI_Char*	Name of the parameter.
	*/
    virtual const SI_Char* GetParameterName(CSLAnimatableType* in_pParameter);

protected:
	SI_Error UnconnectModel(CSLModel *in_pModel);

private:
	CSLModel* CreateLightTemplate(CdotXSITemplate **out_ppLightTemplate);
	SI_Bool FindSpecificChildrenRecursively(CSLModel *in_pSource, CSLModel *in_pTarget);

    CSLVisibility* m_pVisibility;
    CSIBCArray<CSLCluster*> m_Clusters;
    CSLMixer* m_pMixer;
	CSIBCArray<CSLEnvelope*> m_Envelopes;
    CSLTransform* m_pLocalTransform;
    CSLTransform* m_pBasepose;
    CSLGlobalMaterial* m_pGlobalMaterial;
    CSLPrimitive* m_pPrimitive;
    CSIBCArray<CSLModel*> m_Children;
	CSIBCArray<CSLFXTree*> m_FXTrees;
	CSLModelExtension_5_0 *m_pExtension;	
};

#endif
