// MapParser.h: interface for the CMapParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPPARSER_H__5F494664_92EF_4AA9_90CF_70621B749159__INCLUDED_)
#define AFX_MAPPARSER_H__5F494664_92EF_4AA9_90CF_70621B749159__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SIBCUtil.h>
#include <SIBCArray.h>

#include "VMFTemplate.h"
#include "VMFTypes.h"
#include "Entity.h"

#define BEGIN_SECTION		'{'
#define END_SECTION			'}'
#define BEGIN_INFO			'"'
#define BEGIN_BRUSH_LINE	'('

class CMapParser  
{
public:
	CMapParser();
	virtual ~CMapParser();

	SI_Error Read						( SI_Char* in_szFilename );
	
	SI_Error ConvertToSemanticLayer		();
	SI_Error CleanMaterials();
	SI_Error CleanXSIMaterials();
	


private:

	SI_Error CleanMaterial( VMFTemplate* );
	SI_Error CleanModelMaterial( CSLModel * );

	SI_Error BuildEntity	( _SI_FILE, CEntity* );
	SI_Error ReadProperty	( _SI_FILE, CSIBCString&, CSIBCString& );
	SI_Error ReadBrush		( _SI_FILE, CEntity* );

	SI_Int	 GetNextInteger ( _SI_FILE );
	SI_Float GetNextFloat	 ( _SI_FILE );
	SI_Error GetNextString	( _SI_FILE, SI_Char * );

	SI_Error ConvertEntityList	();

private:

	CSIBCArray< CEntity* >		m_pEntityList;
	CSIBCArray< VMFTemplate* >	m_pTemplateList;
	CSIBCArray< CSIBCString >	m_pMaterialList;
	CSIBCString					m_szPath;
	
};

#endif // !defined(AFX_MAPPARSER_H__5F494664_92EF_4AA9_90CF_70621B749159__INCLUDED_)
