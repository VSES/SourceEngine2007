//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2005 Avid Technology, Inc. . All rights reserved.
//
//***************************************************************************************

/****************************************************************************************
THIS CODE IS PUBLISHED AS A SAMPLE ONLY AND IS PROVIDED "AS IS".
IN NO EVENT SHALL SOFTIMAGE, AVID TECHNOLOGY, INC. AND/OR THEIR RESPECTIVE
SUPPLIERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS CODE .

COPYRIGHT NOTICE. Copyright © 1999-2002 Avid Technology Inc. . All rights reserved. 

SOFTIMAGE is a registered trademark of Avid Technology Inc. or its subsidiaries 
or divisions. Windows NT is a registered trademark of Microsoft Corp. All other
trademarks contained herein are the property of their respective owners. 
****************************************************************************************/

#ifndef _XSICLUSTERINFO_H
#define _XSICLUSTERINFO_H

#include "Template.h"

/** 
* CSLSceneInfo Class
* 
* Describes the SceneInformation template.
*/
class XSIEXPORT CSLXSIClusterInfo
	: public CSLTemplate
{
// Methods
public:

	//! Defines the various types of clusters
	enum EClusterType
	{
		SI_POLYGON,
		SI_FACE,
		SI_SURFACE_CURVE,
		SI_KNOT_CURVE_U,
		SI_KNOT_CURVE_V,
		SI_BOUNDARY,
		SI_VERTEX,
		SI_EDGE,
		SI_POLYGON_NODE,
		SI_SAMPLED_POINT,
		SI_KNOT,
		SI_TRIM_CURVE,
		SI_ISO_LINE_U,
		SI_ISO_LINE_V,
		SI_ISO_POINT,
		SI_ISO_LINE,
		SI_SUB_SURFACE,
		SI_SUB_CURVE,
	};

	//! Constructor
	/**
		\param in_pScene Pointer to the scene
		\param in_pModel Pointer to the model
		\param in_pTemplate Pointer to the parent template
		\param in_DefaultEClusterType Cluster type to use
	*/
    CSLXSIClusterInfo
	( 
		CSLScene* in_pScene,
		CSLModel* in_pModel,
		CdotXSITemplate *in_pTemplate,
		EClusterType in_DefaultEClusterType = SI_VERTEX
	);

	//! Destructor
    virtual ~CSLXSIClusterInfo();

	//! Set the cluster type
	/*!
		\param in_Type New cluster type
	*/
    void SetClusterType( EClusterType in_Type ) { m_ClusterType = in_Type; };

	//! Get the cluster type
	/*!
		\return Current cluster type
	*/
    EClusterType GetClusterType();

    virtual CSLTemplate::ETemplateType Type();

	virtual SI_Error Synchronize();

	// Members
private:
    CSLStrEnumProxy<EClusterType, SI_SUB_CURVE> m_ClusterType;

	// This member is here to allow future improvement of the class.
	SI_Void *m_pReserved;
};

#endif

