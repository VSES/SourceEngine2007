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

#ifndef _SHAPEANIMATION_H
#define _SHAPEANIMATION_H

#include "Template.h"

class CSLBaseShape;
class CSLFCurve;

/**
* Describes a mesh, surface or curve shape animation
	\sa CSLAction
*/
class XSIEXPORT CSLShapeAnimation
	: public CSLTemplate
{
public:
	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate referred template
		\param in_Type Default interpolation type
	*/
	CSLShapeAnimation(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, EFCurveInterpolationType in_Type);
	virtual ~CSLShapeAnimation();

	/*! Add a new shape at the end of the animation.
		\return CSLBaseShape*	Pointer to the newly created shape.
	*/
	CSLBaseShape*				AddShape();

	/*! Insert a new shape into the animation.
		\param	SI_Int	Index of insertion of the new shape.
		\return Pointer to the newly created shape.
		\note Animation is inserted at the end of the list if the index is out
		of bounds
	*/
	CSLBaseShape*				InsertShape(SI_Int in_nIndex);

	/*! Return the number of shapes in the animation.
		\return SI_Int	Number of shapes.
	*/
	SI_Int						GetShapeCount();

	/*!	Return an array of all shapes in the animation.
		\return	Pointer to the array of pointers to shape objects.
	*/
	CSLBaseShape**				Shapes();

	/*! Remove a single shape from the animation.
		\param	SI_Int	Index of the shape to remove.
		\retval SI_SUCCESS The animation was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error					RemoveShape(SI_Int in_nIndex);

	/*!	Remove a single shape from the animation.
		\param	CSLBaseShape*	Pointer to the shape to remove.
		\retval SI_SUCCESS The animation was removed
		\retval SI_ERR_BAD_ARGUMENT Shape was not found
	*/
	SI_Error					RemoveShape(CSLBaseShape* in_pObject);

	/*!	Remove all shapes from the shape animation.
		\return	SI_SUCCESS
	*/
	SI_Error					ClearShapes();

	/*!	Return the fcurve containing the animation of the shape.
		\return	CSLFCurve*	Pointer to a fcurve object.
	*/
	CSLFCurve*					Animation();

	// Connection functions;
	CSLFCurve*					ConnectAnimation(CSLFCurve *in_pNewAnimation);
	CSLBaseShape*				ConnectShape(CSLBaseShape* in_pNewShape, SI_Int in_nIndex);

	virtual SI_Error Fix();
    virtual SI_Error Synchronize();

	/*! Returns the type of this template
		\retval CSLTemplate::SI_SHAPE_ANIMATION
	*/
    virtual ETemplateType Type() { return SI_SHAPE_ANIMATION; }

protected:
	CSLBaseShape*				CreateShape();

private:
	CSIBCArray<CSLBaseShape *>	m_Shapes;
	CSLFCurve*					m_pAnimation;
	CSLStrEnumProxy<EFCurveInterpolationType, SI_CUBIC>		m_InterpolationType;

	void *m_pReserved;	// reserved for future extension
};

#endif
