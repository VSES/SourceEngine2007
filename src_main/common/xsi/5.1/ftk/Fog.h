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

#ifndef _FOG_H
#define _FOG_H

#include "Template.h"

//! Describes the fog (depth fading) in a scene.
class XSIEXPORT CSLFog
	: public CSLTemplate
{
public:
	/** 
	* This enum list the possible type for the fog.
	*/
	enum EFogType
	{
		SI_VERTEX,	//!< per vertex fog
		SI_PIXEL,	//!< per pixel fog
	};

	/** 
	* This enum list the possible interpolation type for the fog.
	*/
	enum EFogInterpolationType
	{
		SI_LINEAR,				//!< Fog linear interpolation
		SI_EXPONENTIAL,			//!< Fog Exponential interpolation
	};

	/*! Constructor
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Parent Template
	* \return an instance of CSLFog
	*/
	CSLFog(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLFog();

	/*! Return the type of the fog.
	* \return EFogType	Type of fog.
	*/
	EFogType				GetFogType() { return m_Type; }

	/*! Change the type of the fog.
	* \param	in_Type	New fog type.
	* \return SI_Void.
	*/
	SI_Void 				SetFogType(EFogType in_Type) { m_Type = in_Type; }

	/*! Return the type of interpolation of the fog.
	* \return EFogInterpolationType	Type of fog interpolation.
	*/
	EFogInterpolationType	GetFogInterpolationType() { return m_InterpolationType; }

	/*! Change the type of interpolation of the fog.
	* \param	in_Type	New fog interpolation type.
	* \return SI_Void.
	*/
	SI_Void 				SetFogInterpolationType(EFogInterpolationType in_Type) { m_InterpolationType = in_Type; }

	/*!	Returns the color of the fog.
	* \return	CSIBCColorf	Fog color.
	*/
	CSIBCColorf 			GetColor() { return m_Color; }

	/*!	Changes the color of the fog.
	* \param	in_rColor	New fog color.
	* \return	SI_Void.
	*/
	SI_Void 				SetColor(CSIBCColorf& in_rColor) { m_Color = in_rColor; }

	/*!	Returns the distance at which fog starts.
	* \return	SI_Float	Start distance.
	*/
	SI_Float				GetNear() { return m_Near; }

	/*!	Changes the distance at which fog starts.
	* \param	in_fValue	New start distance.
	* \return	SI_Void.
	*/
	SI_Void 				SetNear(SI_Float in_fValue) { m_Near = in_fValue; }

	/*!	Returns the distance at which fog stops.
	* \return	SI_Float	Stop distance.
	*/
	SI_Float				GetFar() { return m_Far; }

	/*!	Changes the distance at which fog stops.
	* \param	in_fValue	New stop distance.
	* \return	SI_Void.
	*/
	SI_Void 				SetFar(SI_Float in_fValue) { m_Far = in_fValue; }

	/*! Gets a handle on one of this fog's parameters by using its name
	* \param in_szName the name of the parameter to get the handle from
	* \return CSLAnimatableType* the handle to the parameter
	*/
	virtual CSLAnimatableType*	ParameterFromName(SI_Char *in_szName);

	/*! Gets a handle on one of this fog's parameters by using its name and type
	* \param in_szParameterName the name of the parameter to get the handle from
	* \param in_Type the type of the parameter to get the handle from
	* \return CSLAnimatableType* the handle to the parameter
	*/ 	
	virtual CSLAnimatableType*	ParameterFromType(EFCurveType in_Type, SI_Char *in_szParameterName);

	/*! Returns the type of this template
	* \return ETemplateType The type of this template 
	*/
	virtual ETemplateType		Type() { return SI_FOG; }

private:
	CSLEnumProxy<EFogType, SI_FOG>						m_Type;
	CSLEnumProxy<EFogInterpolationType, SI_EXPONENTIAL> m_InterpolationType;

	CSLColorRGBProxy	m_Color;
	CSLFloatProxy		m_Near;
	CSLFloatProxy		m_Far;
};

#endif
