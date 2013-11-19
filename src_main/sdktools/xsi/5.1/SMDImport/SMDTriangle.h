// SMDTriangle.h: interface for the SMDTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMDTRIANGLE_H__E9707268_B1C7_4523_B91E_D61A4445513A__INCLUDED_)
#define AFX_SMDTRIANGLE_H__E9707268_B1C7_4523_B91E_D61A4445513A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SIBCArray.h>
#include "SMDVertex.h"

class SMDTriangle  
{
public:
	SMDTriangle();
	virtual ~SMDTriangle();

	char m_szTexture[MAX_PATH];
	CSIBCArray<SMDVertex>	m_pVertex;

};

#endif // !defined(AFX_SMDTRIANGLE_H__E9707268_B1C7_4523_B91E_D61A4445513A__INCLUDED_)
