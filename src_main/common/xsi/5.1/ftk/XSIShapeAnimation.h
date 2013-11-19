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

#ifndef _XSISHAPEANIMATION_H
#define _XSISHAPEANIMATION_H

#include "Template.h"

class CSLXSIShape;
class CSLFCurve;

/**
* Describes a mesh shape animation for dotXSI 5.0+.
	\sa CSLAction
*/
class XSIEXPORT CSLXSIShapeAnimation
	: public CSLTemplate
{
public:
	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate referred template
		\param in_Type Default interpolation type
	*/
	CSLXSIShapeAnimation(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, EFCurveInterpolationType in_Type);
	virtual ~CSLXSIShapeAnimation();

	/*! Add a new shape at the end of the animation.
		\return CSLXSIShape*	Pointer to the newly created shape.
	*/
	CSLXSIShape*				AddXSIShape();

	/*! Insert a new shape into the animation.
		\param	SI_Int	Index of insertion of the new shape.
		\return Pointer to the newly created shape.
		\note Animation is inserted at the end of the list if the index is out
		of bounds
	*/
	CSLXSIShape*				InsertXSIShape(SI_Int in_nIndex);

	/*! Return the number of shapes in the animation.
		\return SI_Int	Number of shapes.
	*/
	SI_Int						GetXSIShapeCount();

	/*!	Return an array of all shapes in the animation.
		\return	Pointer to the array of pointers to shape objects.
	*/
	CSLXSIShape**				XSIShapes();

	/*! Remove a single shape from the animation.
		\param	SI_Int	Index of the shape to remove.
		\retval SI_SUCCESS The animation was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error					RemoveXSIShape(SI_Int in_nIndex);

	/*!	Remove a single shape from the animation.
		\param	CSLXSIShape*	Pointer to the shape to remove.
		\retval SI_SUCCESS The animation was removed
		\retval SI_ERR_BAD_ARGUMENT Shape was not found
	*/
	SI_Error					RemoveXSIShape(CSLXSIShape* in_pObject);

	/*!	Remove all shapes from the shape animation.
		\return	SI_SUCCESS
	*/
	SI_Error					ClearXSIShapes();

	/*!	Return the fcurve containing the animation of the shape.
		\return	CSLFCurve*	Pointer to a fcurve object.
	*/
	CSLFCurve*					Animation();

	/*!	Verify if we have an animation fcurve, if not, it creates the template of the fcurve,
		creates the semantic layer object of the fcurve and connect it to the template.

		\return	CSLFCurve*	Pointer to a fcurve containing the animation of the shape (new or existing one).
	*/
	CSLFCurve*					AddAnimation();

	// Connection functions;
	CSLFCurve*					ConnectAnimation(CSLFCurve *in_pNewAnimation);
	CSLXSIShape*				ConnectXSIShape(CSLXSIShape* in_pNewShape, SI_Int in_nIndex);

	virtual SI_Error Fix();
    virtual SI_Error Synchronize();

	/*! Returns the type of this template
		\retval CSLTemplate::XSI_SHAPE_ANIMATION
	*/
    virtual ETemplateType Type() { return XSI_SHAPE_ANIMATION; }

protected:
	CSLXSIShape*				CreateXSIShape();

private:
	CSIBCArray<CSLXSIShape *>	m_XSIShapes;
	CSLFCurve*					m_pAnimation;
	CSLStrEnumProxy<EFCurveInterpolationType, SI_CUBIC>		m_InterpolationType;

	void *m_pReserved;	// reserved for future extension
};

#endif
