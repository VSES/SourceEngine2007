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

#ifndef _XSIGEOMETRY_H
#define _XSIGEOMETRY_H

#include "Primitive.h"

// Forward declaration
class CSLXSIShapeAnimation;

//! Base class representing XSIGeometry, handles xsi shape animation functionality for dotXSI 5.0+.
class XSIEXPORT CSLXSIGeometry
	: public CSLPrimitive
{
public:

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the Geometry
	* \param in_pModel	Pointer to the model containing the Geometry
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the Geometry
	* \return an instance of CSLGeometry
	*/
	CSLXSIGeometry(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLXSIGeometry();

	/*! Create a new shape animation for the geometry.
	* \param	EFCurveInterpolationType	Type of interpolation for the new shape animation.
	* \return CSLXSIShapeAnimation*	Pointer to the newly created shape animation.
	*/
	CSLXSIShapeAnimation* CreateXSIShapeAnimation(EFCurveInterpolationType in_Type);

	/*! Return the shape animation of the geometry.
	* \return CSLXSIShapeAnimation*	Pointer to the shape animation.
	*/
	CSLXSIShapeAnimation* XSIShapeAnimation();

	/*! Destroy an existing shape animation on the geometry.
	* \return SI_Error	Error code.
	*/
	SI_Error DestroyXSIShapeAnimation();

	/*! Connects a shape animation
	* \param CSLXSIShapeAnimation* the shape animation to connect
	* \return The connected shape animation
	*/
	CSLXSIShapeAnimation* ConnectXSIShapeAnimation(CSLXSIShapeAnimation* in_pNewXSIShapeAnim);

	/*! Synchronizes this geometry instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Gets a handle on one of this geometry's parameters by using its name
	* \param in_szName the name of the parameter to get the handle from
	* \return CSLAnimatableType* the handle to the parameter
	*/
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

private:
	CSLXSIShapeAnimation* m_pXSIShapeAnimation;

	void *m_pReserved;	// reserved for future extension
};

#endif
