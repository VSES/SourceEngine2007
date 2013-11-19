// SMDNode.h: interface for the SMDNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMDNODE_H__7105D459_763E_4AD5_A15F_8B5AF1B0A24C__INCLUDED_)
#define AFX_SMDNODE_H__7105D459_763E_4AD5_A15F_8B5AF1B0A24C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SIBCUtil.h"
#include "SIBCString.h"
#include "SIBCArray.h"
#include "SIBCMatrix.h"

#include <stdio.h>
#include <xsi_x3dobject.h>
#include <xsi_kinematics.h>
#include <xsi_string.h>

void W2AHelper( LPSTR out_sz, LPCWSTR in_wcs, int in_cch = -1);

/** 
* SMDNode class
*
* This class holds the information about a single bone (node) that affects an envelope.
* It holds it's name, it's unique ID and it's parent ID.
*
*/
class SMDNode  
{
public:
	SMDNode();
	virtual ~SMDNode();

	/*!	Returns the Node's unique ID.
	\return	SI_Int	Node's ID
	*/
	SI_Int		GetID()			{ return m_iNodeID; };

	/*!	Returns the Node's parent ID.
	\return	SI_Int	Node's parent ID
	*/
	SI_Int		GetParentID()	{ return m_iNodeParentID; };

	/*!	Returns the Node's name.
	\return	SI_Char*	Node's name
	*/
	SI_Char*	GetName()		{ return m_szNodeName.GetText(); };

	/*!	Returns the Node's model
	\return	CSLModel* The node's model
	*/
	XSI::X3DObject 	GetModel()		{ return m_pModel; };
	
	/*!	Sets the Node's ID
	\param	SI_Int	The Node's ID
	*/
	void		SetID( SI_Int in_iID )			{ m_iNodeID = in_iID; };

	/*!	Sets the Node's parent ID
	\param	SI_Int	The Node's parent ID
	*/
	void		SetParentID( SI_Int in_iID )	{ m_iNodeParentID = in_iID; };

	/*!	Sets the Node's name
	\param	SI_Char*	The Node's name
	*/
	void		SetName( SI_Char* in_szName )	{ m_szNodeName = in_szName; };
	
	/*!	Sets the Node's Semantic Layer Model object
	\param	CSLModel*	The model object
	*/
	void		SetModel( XSI::X3DObject in_pModel ) { m_pModel = in_pModel; };

	/*!	Sets the Node's matrix
	\param	CSIBCMatrix4x4	Node matrix
	*/
	void		SetMatrix( CSIBCMatrix4x4 in_mMatrix ) { m_mMatrix = in_mMatrix; };

	/*!	Gets the Node's matrix
	\return	CSIBCMatrix4x4	Node matrix
	*/
	CSIBCMatrix4x4 GetMatrix() { return m_mMatrix ; };
	
	/*!	Sets the Node's Hierarchical Compensation matrix
	\param	CSIBCMatrix4x4	HC matrix
	*/
	void		SetHierarchicalCompensation( XSI::Kinematics in_mMatrix );

	/*!	Sets the Node's Second Hierarchical Compensation matrix
	\param	CSIBCMatrix4x4	HC matrix
	*/
	void		SetHierarchicalCompensation2( XSI::Kinematics in_mMatrix );
	

	/*!	Gets the Node's Hierarchical Compensation matrix
	\return		CSIBCMatrix4x4&
	*/
	XSI::Kinematics		GetHierarchicalCompensation() { return m_mHierarchyCompensation; }; 

	/*!	Gets the Node's Second Hierarchical Compensation matrix
	\return		CSIBCMatrix4x4&
	*/
	XSI::Kinematics		GetHierarchicalCompensation2() { return m_mHierarchyCompensation2; }; 
	

	/*!	Write the node's information to a stream
	\param	FILE*	The stream to fprintf to.
	*/
	SI_Error	Write( FILE *l_fptr  );

private:
	XSI::X3DObject		m_pModel;
	CSIBCString			m_szNodeName;
	SI_Int				m_iNodeID;
	SI_Int				m_iNodeParentID;
	CSIBCMatrix4x4		m_mMatrix;
	XSI::Kinematics		m_mHierarchyCompensation;
	XSI::Kinematics		m_mHierarchyCompensation2;
};

/** 
* SMDNodeList class
*
* This class holds a list of SMDNode objects.
*
*/
class SMDNodeList
{
public:
	SMDNodeList();
	~SMDNodeList();

	/*!	Add a single node to the list (parent by ID)
	\param	char*	Node name
	\param	int		Node parent ID
	\return	nothing
	*/
	void AddNode ( XSI::CString , int in_iParentID, XSI::X3DObject in_pModel, XSI::Kinematics	l_pHierarchyCompensation, XSI::Kinematics l_pHierarchyCompensation2);
	void AddNodeS ( char*  , int in_iParentID, XSI::X3DObject in_pModel, XSI::Kinematics	l_pHierarchyCompensation, XSI::Kinematics l_pHierarchyCompensation2);

	/*!	Add a single node to the list (parent by name)
	\param	char*	Node name
	\param	char*	Parent name
	\return	nothing
	*/
	void AddNode ( XSI::CString in_szName, XSI::CString  in_szParentName,  XSI::Kinematics	l_pHierarchyCompensation, XSI::Kinematics l_pHierarchyCompensation2);
	void AddNode ( char *in_szName, char *in_szParentName,  XSI::Kinematics	l_pHierarchyCompensation, XSI::Kinematics l_pHierarchyCompensation2);

	/*!	Loops through the list of nodes and calls SMDNode::Write
	\param	FILE*	The stream to fprintf to.
	*/
	void Write   ( FILE* in_fptr );

	/*!	Returns a single node by name
	\param	SI_Char*	Node name
	\return SMDNode*	Node or NULL if not found
	*/
	SMDNode*	GetByName ( XSI::CString in_szName );
	SMDNode*	GetByName ( char *in_szName );

	/*!	Returns a single node by ID
	\param	SI_Int		Node's ID
	\return SMDNode*	Node or NULL if not found
	*/
	SMDNode*	GetByID ( SI_Int in_iIndex );

	/*!	Returns a single node by its index in the list
	\param	SI_Int		Node's index
	\return SMDNode*	Node or NULL if not found
	*/
	SMDNode*	GetByIndex ( SI_Int in_iIndex );

	/*!	Returns the number of nodes in the list
	\return int		Number of nodes
	*/
	int			GetNbNodes () { return m_pNodeList.GetUsed(); };


	/*!	By definition a ModelSMD file is a file with a single bone located at the 
		center of the world to which every vertex is assigned. When called, this 
		function will erase it's node list and replace it with this single bone.
	\return nothing
	*/

	void		ConvertToModelSMD ();

private:

	CSIBCArray<SMDNode*>	m_pNodeList;

};

	
#endif // !defined(AFX_SMDNODE_H__7105D459_763E_4AD5_A15F_8B5AF1B0A24C__INCLUDED_)
