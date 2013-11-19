// SMDKey.h: interface for the SMDKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMDKEY_H__DDBC814E_272C_4867_8687_E1716ADFE090__INCLUDED_)
#define AFX_SMDKEY_H__DDBC814E_272C_4867_8687_E1716ADFE090__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <xsi_point.h>
class SMDKey  
{
public:
	SMDKey();
	virtual ~SMDKey();

	float				m_fTime;
	XSI::MATH::CVector3	m_vPosition;
	XSI::MATH::CVector3	m_vRotation;
};

#endif // !defined(AFX_SMDKEY_H__DDBC814E_272C_4867_8687_E1716ADFE090__INCLUDED_)
