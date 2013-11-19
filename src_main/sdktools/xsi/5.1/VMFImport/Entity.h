// Entity.h: interface for the CEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENTITY_H__8D60CB7C_114F_4092_94DF_0C12905985B6__INCLUDED_)
#define AFX_ENTITY_H__8D60CB7C_114F_4092_94DF_0C12905985B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SIBCUtil.h>
#include <SIBCArray.h>
#include <SIBCString.h>
#include <Model.h>

#include "VMFTypes.h"
#include "BrushConverter.h"

class CEntity  
{
public:
	CEntity();
	virtual ~CEntity();

	SI_Error	AddProperty ( CSIBCString&, CSIBCString& );
	
	SI_Error	BeginBrush();
	SI_Error	EndBrush();

	SI_Error	AddFace ( face_t& );

	SI_Error	Convert		();

	SI_Error	FillModel	( CSLModel * );

	SI_Char*	GetWadFile	();

private:
	CBrushConverter*				m_pCurrentBrush;
	CSIBCArray< CBrushConverter* >	m_pBrushList;
	CSIBCArray< CSIBCString >		m_pPropertyList;
	CSIBCArray< CSIBCString >		m_pValueList;
};

#endif // !defined(AFX_ENTITY_H__8D60CB7C_114F_4092_94DF_0C12905985B6__INCLUDED_)
