// VMFTemplate.h: interface for the VMFTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VMFTEMPLATE_H__DF38E90D_5396_417B_9E3B_2A43D2E5EA8A__INCLUDED_)
#define AFX_VMFTEMPLATE_H__DF38E90D_5396_417B_9E3B_2A43D2E5EA8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SIBCArray.h>
#include <SIBCString.h>

#include <Model.h>
#include "BrushConverter.h"

#include <SIILTGAFileDriver.h>

#include <Scene.h>
#include <Material.h>

#include <xsi_model.h>
#include <xsi_ref.h>
#include <xsi_string.h>
#include <xsi_value.h>
#include <xsi_status.h>
#include <xsi_comapihandler.h>
#include <xsi_x3dobject.h>

#include "C3DVectorCompressor.h"
#include "DispInfoConverter.h"

typedef struct _tagMatGroup
{
	XSI::CRefArray	m_Objects;
	CSIBCString		m_szMaterialName;
} MatGroup;

typedef struct _tagvmfUV
{
	float	u;
	float	v;
} vmfUV;

typedef struct _TGAMapEntryTAG
{
	CSIBCString			m_szFilename;
	float				w;
	float				h;
	
} TGAMapEntry;

class VMFTemplate  
{
public:
	VMFTemplate();
	virtual ~VMFTemplate();

	void			AddProperty ( CSIBCString& in_szName, CSIBCString& in_szValue );
	void			GetProperty ( SI_Int in_iIdx, CSIBCString& out_szName, CSIBCString& out_szValue );
	int				GetNbProperty ();
	
	void			AddChild ( VMFTemplate* in_pChild, VMFTemplate* in_pParent );
	VMFTemplate*	GetChild ( SI_Int in_iIdx );
	int				GetNbChild ();

	void			SetName( SI_Char* in_szName ) { m_szName = in_szName; };
	
	SI_Char*		GetName() { return m_szName.GetText(); };

	void			Read ( SI_Char* seps );

	CSLModel*		Convert( XSI::CComAPIHandler, XSI::CValue, XSI::Model in_pModel, CSLModel *, int init = 0  );

	void				SetParent( VMFTemplate * in_pParent ) { m_pParent = in_pParent; };
	VMFTemplate*		GetParent() { return m_pParent;};
	
	CBrushConverter*	GetBrush() { return &m_pBrush; };

	void				CreateGeometry( XSI::Model in_pRootModel, CSLModel *in_pModel, bool disp = false);
	void				CreateDispinfo( XSI::Model in_pRootModel, CSLModel *in_pModel);
	void				AssignUVs();

	void				SetMeshModel ( CSLModel* in_pModel ) { m_pMeshModel = in_pModel; };
	CSLModel*			GetMeshModel () { return m_pMeshModel;};

	CSLBaseMaterial*	GetMaterialFromTextureFile ( CSLScene* in_pScene, char *in_szFilename );

	void				GetImageDimensions ( char * in_szFilename, float& w, float& h );

	void				AddMeshToGroup ( XSI::CRef in_cRef, char *in_szMaterial );
	void				CreateGroups ( XSI::Model in_pRoot );

	CSIBCArray < CSIBCString >	m_szDirtyClusterMaterials;

private:

	void				GetUVFromPosition ( CSIBCVector3D in_vPosition, float& u, float& v);
	void				SetXSIValue ( XSI::CString in_szString , long in_lValue );

	CSIBCString						m_szName;
	CSIBCArray< CSIBCString >		m_pPropertyList;
	CSIBCArray< CSIBCString >		m_pValueList;
	CSIBCArray< vmfUV		>		m_pUVs;
	XSI::CLongArray					polygonData;

	CSIBCArray<CDispInfoConverter*>	dispinfo_convert;

	CSIBCArray< VMFTemplate* >		m_pChildren;

	static CSIBCArray<MatGroup>			m_pGroups;

	VMFTemplate*					m_pParent;

	CBrushConverter					m_pBrush;

	CSLModel*						m_pMeshModel;

	XSI::X3DObject					l_x3dCube;

	static	CSIBCArray< struct _TGAMapEntryTAG >	m_gTGAMap;

	static	int						m_iTemplateCount;

	int								m_iPolygonCount;

	static CSIBCArray<CSIBCVector3D>	l_Vertices;
	static CSIBCArray<CSIBCVector3D>	l_Normals;
	static CSIBCArray<CSIBCVector2D>	l_UVs;


};

#endif // !defined(AFX_VMFTEMPLATE_H__DF38E90D_5396_417B_9E3B_2A43D2E5EA8A__INCLUDED_)
