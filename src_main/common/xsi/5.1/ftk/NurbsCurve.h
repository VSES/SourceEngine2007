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

#ifndef _NURBSCURVE_H
#define _NURBSCURVE_H

#include "Geometry.h"

#ifndef _SL_FLOAT_ARRAY_PROXY_EXPORT_
#define _SL_FLOAT_ARRAY_PROXY_EXPORT_

//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SI_Float, SI_Float, 1>;
//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<CSIBCVector4D, SI_Float, 4>;

#endif // _SL_FLOAT_ARRAY_PROXY_EXPORT_


//! Describes a single NURBS curve.
class XSIEXPORT CSLNurbsCurve
	: public CSLGeometry
{
public:
	/**
	* Proxy class that allows modifying a list of SI_Float like an array.
	*/
	typedef CSLArrayProxy<SI_Float, SI_Float, 1>		CSLFloatArray;

	/**
	* Proxy class that allows modifying a list of CSIBCVector4D objects like an array.
	*/
	typedef CSLArrayProxy<CSIBCVector4D, SI_Float, 4> 	CSLVector4DArray;

	/** 
	* The EParametrizationType enum contains possible types for knot parametrization
	*/
	enum EParametrizationType
	{
		SI_UNIFORM,			//!< Uniform parametrization
		SI_NON_UNIFORM,		//!< Non-uniform parametrization
		SI_CHORD_LENGTH,	//!< Chord length parametrization
		SI_CENTRIPETAL,		//!< Centripetal parametrization
	};

	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLNurbsCurve
	*/
	CSLNurbsCurve(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLNurbsCurve();

	/*! Return the degree of the curve.
	* \return SI_Int	Degree value.
	*/
	SI_Int					GetDegree();

	/*! Change the degree the curve.
	* \param	SI_Int	New degree value.
	* \return SI_Void
	*/
	SI_Void					SetDegree(SI_Int in_nValue);

	/*! Return the closed flag of the curve.
	* \return SI_Bool Closed flag.
	*/
	SI_Bool					GetClosedFlag();

	/*! Change the closed flag of the curve.
	* \param	SI_Bool New closed flag.
	* \return SI_Void
	*/
	SI_Void					SetClosedFlag(SI_Bool in_bValue);

	/*! Return the parametrization type of the curve.
	* \return EParametrizationType	Parametrization type.
	*/
	EParametrizationType	GetParametrization();

	/*! Change the parametrization type of the curve.
	* \param	EParametrizationType	New parametrization type.
	* \return SI_Void
	*/
	SI_Void					SetParametrization(EParametrizationType in_Type);

	/*! Return the number of knots of the curve.
	* \return SI_Int	Number of knots.
	*/
	SI_Int					GetKnotCount();

	/*! Return the knot values array proxy.
	* \return CSLFloatArray* Pointer to an array proxy of knots values.
	*/
	CSLFloatArray*			GetKnotList();

	/*! Returns an array of SI_Float containing the knot values.
	* \return SI_Float*		Array of knots values.
	*/
	SI_Float*				GetKnotListPtr();

	/*! Return the number of control points of the curve.
	* \return SI_Int	Number of control points.
	*/
	SI_Int					GetControlPointCount();

	/*! Return the control point values array proxy.
	* \return CSLVector4DArray*	Pointer to an array proxy of control point values.
	*/
	CSLVector4DArray*		GetControlPointList();

	/*! Returns an array of SI_Float containing the control point values.
	* \return SI_Float*		Array of control point values.
	*/
	CSIBCVector4D*			GetControlPointListPtr();

	/*! Synchronizes this NURBS curve instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the NURBS curve 
	*/
    virtual ETemplateType	Type() { return SI_NURBS_CURVE; }

private:
	CSLIntProxy				m_Degree;
	CSLBoolProxy			m_Closed;
	CSLEnumProxy<EParametrizationType, SI_CENTRIPETAL>	m_Parametrization;
	CSLFloatArray			m_Knots;
	CSLVector4DArray		m_ControlPoints;

	void *m_pReserved;	// reserved for future extension
};

#endif

