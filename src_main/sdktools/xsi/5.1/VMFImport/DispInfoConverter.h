// DispInfoConverter.h: interface for the CDispInfoConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPINFOCONVERTER_H__B52C93D6_36C4_444E_85D6_EEEF99716793__INCLUDED_)
#define AFX_DISPINFOCONVERTER_H__B52C93D6_36C4_444E_85D6_EEEF99716793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SIBCVector.h>

#include <BaseShape.h>
#include <Material.h>
#include <Mesh.h>
#include "VMFTypes.h"

#include <xsi_ref.h>
#include <xsi_model.h>

typedef struct _tagdispUV
{
	float	u;
	float	v;
} dispUV;

class CDispInfoConverter  
{
public:
	CDispInfoConverter();
	virtual ~CDispInfoConverter();

	void		Convert ( face_t *, dispinfo_t*, int, int );
	XSI::CRef	CreateGeometry ( XSI::Model in_pRootModel);
	void		AssignUVs();
	void		Merge ( CDispInfoConverter* in_pDisp );

	CSIBCVector3D	*m_pNodes;
	CSIBCVector2D	*m_pUVs;
	int*			m_pIndices;

	XSI::X3DObject		l_x3dCube;

	XSI::CLongArray		polygonData;
	CSIBCArray<dispUV>	m_pDispUV;

	int				m_iVertexCount;
	int				m_iNumPoly;

};

#endif // !defined(AFX_DISPINFOCONVERTER_H__B52C93D6_36C4_444E_85D6_EEEF99716793__INCLUDED_)
