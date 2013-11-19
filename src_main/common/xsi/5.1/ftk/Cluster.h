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
#ifndef _CLUSTER_H
#define _CLUSTER_H

#include "Template.h"
#include "XSIClusterInfo.h"

#ifndef _SL_INT_ARRAY_PROXY_EXPORT_
#define _SL_INT_ARRAY_PROXY_EXPORT_

//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SI_Int, SI_Int, 1>;

#endif // _SL_INT_ARRAY_PROXY_EXPORT_

// Forward declaration
class CSLWeightingType;
class CLSModel;
class CSLBaseShape;
class CSLUserDataList;


//! Describes a cluster.
class XSIEXPORT CSLCluster
	: public CSLTemplate
{
public:
	/**
	* Proxy class that allows modifying a list of SI_Int like an array.
	*/
	typedef CSLArrayProxy<SI_Int, SI_Int, 1> CSLIntArray;

	/** 
	* The EClusterWeightingType enum contains possible weighting types for a cluster
	*/
	enum EClusterWeightingType
	{
		SI_AVERAGE,		/*!< The cluster uses an averaging function to determine weights */
		SI_ADDITIVE,	/*!< The cluster uses an additive function to determine weights */
	};

	/*! Synchronizes this cluster instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the cluster
	*/
	virtual ETemplateType Type() { return SI_CLUSTER; }
	
	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the Cluster
	* \param in_pModel	Pointer to the model containing the Cluster
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the Cluster
	* \return an instance of CSLCluster
	*/
	CSLCluster(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLCluster();

	/*!	Return the model possessing this cluster.
	* \return	CSLModel*	Pointer to the model that has this cluster
	*/
	CSLModel* GetReference();

	/*!	Return the center of the cluster.
	* \return	CSLModel*	Pointer to the cluster center model.
	*/
	CSLModel* GetCenter();

	/*!	Change the center of the cluster.
	* \param	CSLModel*	New cluster center object.
	* \return	SI_Void
	*/
	SI_Void SetCenter(CSLModel* in_pNewCenter);

	/*!	Create a new shape for the cluster.
	* \return	CSLBaseShape*	Newly creates shape object.
	*/
	CSLBaseShape* CreateShape();

	/*!	Return the shape of the cluster.
	* \return	CSLBaseShape*	Pointer to a shape object.
	*/
	CSLBaseShape* Shape();

	/*!	Destroy the shape of the cluster.
	* \return	SI_Error	Error code.
	*/
	SI_Error DestroyShape();

	/*! Return the number of vertex indices.
	* \return SI_Int	Number of vertex index.
	*/
	SI_Int GetVertexIndicesCount();

	/*! Return the vertex indices in an array proxy.
	* \return CSLIntArray*	Pointer to an array proxy of vertex indices.
	*/
	CSLIntArray* GetVertexIndicesList();

	/*!	Returns an array of SI_Int containing the vertex indices.
	* \return SI_Int*	Array of vertex indices.
	*/
	SI_Int* GetVertexIndicesListPtr();

	/*!	Return the type of weighting for the cluster.
	* \return	EClusterWeightingType	Type of weighting.
	*/
	EClusterWeightingType GetWeightingType();

	/*!	Change the type of weighting of the cluster.
	* \param	EClusterWeightingType	New weighting type.
	* \return	SI_Void
	*/
	SI_Void SetWeightingType(EClusterWeightingType in_Type);

	// Connection functions.
	/*!	Connects a shape to that cluster
	* \param	in_pNewShape	New shape to connect
	* \return	CSLBaseShape*	a pointer to the  connected shape
	*/
	CSLBaseShape* ConnectShape(CSLBaseShape* in_pNewShape);

	///////////////////////////////////////////////////////////////////////////
	// UserDataList Functionality /////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Adds an user data list to the user data lists
	* \return CSLUserDataList Pointer to the newly added UserDataList
	*/
	CSLUserDataList* AddUserDataList();

	/*! Removes an user data list from the user data lists
	* \param in_nIndex Index of the UserDataList to remove
	* \return SI_Error Whether or not the user data list could be removed
	*/
	SI_Error RemoveUserDataList( SI_Int in_nIndex );

	/*! Removes an user data list from the user data lists
	* \param in_pToRemove Pointer to the user data list to remove
	* \return SI_Error Whether or not the user data list could be removed
	*/
	SI_Error RemoveUserDataList( CSLUserDataList* in_pToRemove );

	/*! Removes all connections points from the user data lists
	* \return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearUserDataLists();

	/*! Adds an exisiting user data list to the user data lists
	* \param in_pToConnect Pointer to the user data list to add
	* \return The added user data list or NULL if it already exists in the list
	*/
	CSLUserDataList* ConnectUserDataList( CSLUserDataList* in_pToConnect );

	/*! Gets a pointer to the internally-stored user data list pointer list
	* \return CSLUserDataList Pointer to the list
	*/
	CSLUserDataList** GetUserDataLists();

	/*! Gets the total number of user data lists 
	* \return SI_Int Number of user data lists
	*/
	SI_Int GetUserDataListCount();

	/*! Searches for a given user data list
	* \param in_szUserDataListName Name of the user data list to look for
	* \return CSLUserDataList Pointer to the user data list or null if it cannot be found
	*/
	CSLUserDataList* FindUserDataList( SI_Char* in_szUserDataListName );

	CSLXSIClusterInfo* GetClusterInfo();

	CSLXSIClusterInfo* ConnectClusterInfo(CSLXSIClusterInfo* in_pClusterInfo);

	virtual SI_Error Fix();

private:
	CSLStrEnumProxy<EClusterWeightingType, SI_ADDITIVE> m_WeightingType;
	
/* 
	We might activate this once the cluster type are set to strings.  It is also possible
	that we will need to add a separate implementation of the whole class in 3.6.  If we do
	use the line below, the constructor will have to be updated as well.
	
	 CSLStrEnumProxy<EClusterType, SI_SUB_CURVE> m_ClusterType;
*/

	CSLModel* m_pClusterCenter;
	CSLBaseShape* m_pShape;
	CSLIntArray m_VertexIndices;
	CSIBCArray<CSLUserDataList*> m_UserDataLists;

	void *m_pReserved;	// reserved for future extension
};

#endif
