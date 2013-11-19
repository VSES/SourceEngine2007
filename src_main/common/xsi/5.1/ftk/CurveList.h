//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
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

#ifndef _CURVELIST_H
#define _CURVELIST_H

#include "Primitive.h"

// Forward declaration
class CSLNurbsCurve;

//! Describes a list of NURBS curve.
class XSIEXPORT CSLCurveList
	: public CSLPrimitive
{
public:

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLCurveList
	*/
	CSLCurveList(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLCurveList();

	/*!	Add a new curve to the list.
	* \return	CSLNurbsCurve*	Pointer to the newly created curve.
	*/
	CSLNurbsCurve* AddCurve();

	/*!	Return the number of curves in the list.
	* \return	SI_Int	Number of curves.
	*/
	SI_Int GetCurveCount();

	/*!	Return an array of all curves in the list.
	* \return	CSLNurbsCurve**	Array of pointers to curve objects.
	*/
	CSLNurbsCurve** Curves();

	/*!	Remove a single curve from the list.
	* \param	in_nIndex	Index of the curve to remove.
	* \return	SI_Error	Error code.
	*/
	SI_Error RemoveCurve(SI_Int in_nIndex);
	
	/*!	Remove a single curve from the list.
	* \param	in_pCurve	Pointer to the curve to remove.
	* \return	SI_Error	Error code.
	*/
	SI_Error RemoveCurve(CSLNurbsCurve* in_pCurve);

	/*!	Remove all curves from the list.
	* \return	SI_Error	Error code.
	*/
	SI_Error ClearCurves();

	/*! Connects a NURBS curve to this curve list
	* \param in_pNewCurve the new NURBS curve to connect
	* \return CSLNurbsCurve* the connected NURBS curve
	*/
	CSLNurbsCurve* ConnectCurve(CSLNurbsCurve* in_pNewCurve);

	/*! Synchronizes this curve list instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the curve list
	*/
    virtual ETemplateType Type();

private:
	CSIBCArray<CSLNurbsCurve *> m_Curves;

	void *m_pReserved;	// reserved for future extension
};

#endif
