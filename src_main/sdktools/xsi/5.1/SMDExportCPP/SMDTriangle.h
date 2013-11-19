// SMDTriangle.h: interface for the SMDTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMDTRIANGLE_H__CCE08CF8_AAD3_4BDD_8FC4_57875E88A5F2__INCLUDED_)
#define AFX_SMDTRIANGLE_H__CCE08CF8_AAD3_4BDD_8FC4_57875E88A5F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "SIBCArray.h"
#include "SIBCVector.h"

#include <xsi_x3dobject.h>
#include <xsi_geometry.h>

#include "SMDNode.h"


/** 
* SMDWeight structure
*
* Holds the information for a single weight
*
*/
typedef	struct	_SMDWeight
{
	SI_Int		m_iBoneID;	//* Bone weighted
	SI_Float	m_fWeight;	//* Normalized weight
} SMDWeight;

/** 
* SMDVertex class
*
* Holds the information for a single weighted vertex.
*
*/
class SMDVertex
{
public:
	SMDVertex();
	~SMDVertex();

	/*!	Sets the vertex index
	\param	SI_Int	vertex index
	\return	nothing
	*/
	void		SetIndex( SI_Int in_iIndex ) { m_iIndex = in_iIndex; };

	/*!	Gets the vertex index
	\return	SI_Int	vertex index
	*/
	SI_Int		GetIndex() { return m_iIndex; };

	/*!	Adds a single weight to this vertex
	\param	SI_Int		Bone ID
	\param	SI_Float	Normalized weight
	\return	nothing
	*/
	void		AddWeight ( SI_Int in_iBoneID, SI_Float in_fWeight );

	/*!	Gets a single weight of this vertex
	\param	SI_Int		vertex ID
	\return	SMDWeight*	Pointer to a SMDWeight structure
	*/
	SMDWeight*	GetWeight ( SI_Int in_iVertexID );

	/*!	Returns the number of weights for this vertex
	\return	SI_Int	Weight count
	*/
	SI_Int		GetNumWeights () { return m_lWeights.GetUsed();};

private:
	SI_Int					m_iIndex;
	CSIBCArray<SMDWeight>	m_lWeights;

};

/** 
* SMDEnvelope class
*
* Holds the information for a weighted model
*
*/
class SMDEnvelope
{
public:
	SMDEnvelope();
	~SMDEnvelope();

	/*!	Sets the model
	\param	CSLModel*	Semantic layer Model object
	\return	nothing
	*/
	void		SetModel ( XSI::X3DObject in_pModel );

	/*!	Gets the model
	\return	CSLModel*	Semantic layer Model object
	*/
	XSI::X3DObject	GetModel () { return m_pModel; };

	/*!	Adds a single weighted vertex to a specific bone
	\param	SI_Int		Vertex index
	\param	SI_Int		Bone ID
	\param	SI_Float	Normalized weight
	\return	nothing
	*/
	void		AddWeight ( SI_Int in_iVertexID, SI_Int in_iBoneID, SI_Float in_fWeight );
	
	/*!	Writes the lists of weighted vertex to a file
	\param	FILE*	File pointer
	\return	nothing
	*/

	SI_Error	Write ( FILE* l_fptr, int rigid, SMDNodeList* in_pNodeList);

	void		ComputeVertexNormals ( CSIBCArray < CSIBCVector3D >& in_pVertexNormals, XSI::Geometry in_pGeo);

	void		ConvertToModelSMD ();

private:

	XSI::X3DObject				m_pModel;
	CSIBCArray<SMDVertex*>	m_pVertexList;
};

////////////////////////////////////////////////////////////////////////////////
// SMDTriangle
////////////////////////////////////////////////////////////////////////////////
class SMDTriangle  
{
public:
	SMDTriangle();
	virtual ~SMDTriangle();

	SI_Error	AddEnvelope ( XSI::X3DObject );
	SI_Error	AddWeight	( XSI::X3DObject, SI_Int, SI_Int, SI_Float );

	SI_Error	Write ( FILE* l_fptr );

	void		ConvertToModelSMD ();
	void		ConvertToRigid() { rigid = 1; };

	void		SetNodeList ( SMDNodeList* in_pNodeList ) { m_pNodeList = in_pNodeList; };

private:

	CSIBCArray<SMDEnvelope*>	m_lEnvelopeList;
	int			rigid;
	SMDNodeList*				m_pNodeList;

};

#endif // !defined(AFX_SMDTRIANGLE_H__CCE08CF8_AAD3_4BDD_8FC4_57875E88A5F2__INCLUDED_)
