// SMDImporter.h: interface for the SMDImporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMDIMPORTER_H__4FB30E24_16B2_4CC8_AEDF_D2BD03230DFE__INCLUDED_)
#define AFX_SMDIMPORTER_H__4FB30E24_16B2_4CC8_AEDF_D2BD03230DFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SMDNode.h"
#include "SMDTriangle.h"
#include "C3DVectorCompressor.h"

typedef struct tagVTAVertex
{
	int	m_iIndex;
	XSI::MATH::CVector3 m_vPosition;
	XSI::MATH::CVector3 m_vNormal;
} VTAVertex;

typedef struct tagVTAClip
{
	int						m_iFrame;
	CSIBCArray<VTAVertex>	m_pVertices;

} VTAClip;

class SMDImporter  
{
public:
	SMDImporter();
	virtual ~SMDImporter();

	bool	Import ( char *in_szFilename );

		XSI::X3DObject				m_pMesh;

private:
	bool	Parse ( FILE *f );
	bool	ImportNodes ();
	bool	ImportSkeleton ();
	bool	ImportTriangles();
	bool	ImportVertexAnimation();

	int		NextTokenAsInteger ();
	float	NextTokenAsFloat ();

	void	DetectModel();
	void	CreateHierarchy();
	void	CreateChainFromThisNode( SMDNode*,XSI::X3DObject in_Parent, int from);
	void	RecurseCreateNull ( XSI::X3DObject in_pParent, SMDNode* in_pNode );
	void	CreateGeometry();
	void	WeightGeometry();
	void	CreateShapeClips();

	void	CreateAnimationSource();
	XSI::X3DObject GetNodeByID(int ID );

	CSIBCArray<SMDNode*>		m_pNodes;
	CSIBCArray<SMDTriangle*>	m_pTriangles;
	CSIBCArray<VTAClip>			m_pVTA;

	int		m_iSMDType;
	XSI::Model					m_pModel;


	SMDNode *					m_pMeshNode;
	C3DVectorCompressor			compress;

	XSI::MATH::CVector3 GetGlobalPosition ( SMDNode* in_pNode );

	char m_szActionName[1024];
	char m_szDirectory[1024];

	CSIBCArray<long>			m_lVertexMap;
};

#endif // !defined(AFX_SMDIMPORTER_H__4FB30E24_16B2_4CC8_AEDF_D2BD03230DFE__INCLUDED_)
