// SMDVertex.h: interface for the SMDVertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMDVERTEX_H__5F7044AE_4ECC_44D8_B3CE_E4D0DD6463E3__INCLUDED_)
#define AFX_SMDVERTEX_H__5F7044AE_4ECC_44D8_B3CE_E4D0DD6463E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <xsi_point.h>
#include <SIBCArray.h>

typedef struct tagWeight
{
	int		m_iBoneID;
	float	m_fWeight;
} Weight;

class SMDVertex  
{
public:
	SMDVertex();
	virtual ~SMDVertex();

	XSI::MATH::CVector3	m_vPosition;
	XSI::MATH::CVector3	m_vNormal;
	XSI::MATH::CVector3	m_vUV;

	CSIBCArray<Weight>	m_vWeights;
};

#endif // !defined(AFX_SMDVERTEX_H__5F7044AE_4ECC_44D8_B3CE_E4D0DD6463E3__INCLUDED_)
