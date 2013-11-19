// BrushConverter.h: interface for the CBrushConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRUSHCONVERTER_H__5EF22907_9137_4744_9333_A627CC030DFD__INCLUDED_)
#define AFX_BRUSHCONVERTER_H__5EF22907_9137_4744_9333_A627CC030DFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SIBCUtil.h>
#include <SIBCArray.h>
#include <SIBCString.h>
#include "VMFTypes.h"

typedef struct _ClusterTAG
{
	CSIBCString	m_szClusterName;
	CSIBCArray< face_t* >	m_pFaceList;
} BrushCluster;

class CBrushConverter  
{
public:
	CBrushConverter();
	virtual ~CBrushConverter();

	SI_Error	AddFace		( face_t& );
	face_t*		GetFace		( SI_Int in_iIndex );
	SI_Int		GetNbFace	();

	SI_Error	Convert		( bool* out_bDisp );

	void				CreateCluster	( char *in_szClusterName );
	BrushCluster*		GetClusterByName( char *in_szClusterName );
	void				RegisterFace ( face_t* in_pFace );

	int					GetClusterCount () { return m_pClusterList.GetUsed(); };
	BrushCluster*		GetCluster( int in_iIndex ) { return &m_pClusterList[in_iIndex]; };

private:

	CSIBCArray< face_t >	m_pFaceList;
	CSIBCArray< BrushCluster >	m_pClusterList;

	vec3_t					bmins;
	vec3_t					bmaxs;

	SI_Bool					m_bInvalid;
};

#endif // !defined(AFX_BRUSHCONVERTER_H__5EF22907_9137_4744_9333_A627CC030DFD__INCLUDED_)
