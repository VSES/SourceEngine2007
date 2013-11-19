#ifndef _VMFMAPS_H_
#define _VMFMAPS_H_

#include <SIBCUtil.h>
#include "VMFTemplate.h"
#include <Model.h>
#include <xsi_model.h>

typedef CSLModel* (*VMFMap)( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );

typedef struct _Map
{
	SI_Char *m_szName;
	VMFMap	m_pCallback;

} VMFMapEntry;

VMFMap	GetVMFMap		( SI_Char * );
void	RegisterMaps	();

//
// Maps
//
CSLModel* GenericMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* SolidMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* SideMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* DispinfoMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* NormalsMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* DistancesMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* OffsetsMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* OffsetNormalsMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* AlphasMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );
CSLModel* EntityMap( VMFTemplate* in_pTemplate, CSLModel* in_pParent, XSI::Model in_pModel );

#endif
