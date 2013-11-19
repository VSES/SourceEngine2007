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

#ifndef _XSINURBSPROJECTIONLIST_H
#define _XSINURBSPROJECTIONLIST_H

#include "Template.h"

class CSLCurveList;

/** 
* Class CSLNurbsProjection
*
* Describes a Nurbs Projection List
* \note This template was introduced in dotXSI Version 3.6
* \sa CSLCurveList
*/
class XSIEXPORT CSLNurbsProjection
    : public CSLTemplate
{
public:
	//! Define projection types
	enum EProjectionType
	{
		SI_UV,		/*!< UV Space */
		SI_WORLD	/*!< World Space */
	};

	/*! Commits semantic layer data
		\return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

	SI_Error Fix();

    /*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
    CSLNurbsProjection(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
	virtual ~CSLNurbsProjection();

	/*! Gets the type of this template
		\return ETemplateType The type of this template: XSI_NURBS_TRIM_LIST
	*/
	ETemplateType Type(){ return XSI_NURBS_PROJECTION; }

	/*! Gets the projection type
		\return EProjectionType The type of projection
	*/
	EProjectionType GetProjectionType();

	/*! Sets the projection type
		\param in_Type The type of projection
	*/
	SI_Void SetProjectionType( EProjectionType in_Type );

	///////////////////////////////////////////////////////////////////////////
	// Projection Functionality /////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*!	Return the curve list describing this projection.
		\return	CSLCurveList*	Pointer to a curve list object.
	*/
	CSLCurveList* CurveList();

	CSLCurveList* ConnectCurveList( CSLCurveList* in_pNewCurveList );

private:
    CSLCurveList* m_pProjCurveList;
    CSLEnumProxy<EProjectionType, SI_WORLD> m_Projection;

	SI_Void *m_pReserved;
};

#endif
