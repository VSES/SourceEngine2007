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

#ifndef _COORDINATESYSTEM_H
#define _COORDINATESYSTEM_H

#include "Template.h"


//! Specifies the coordinate system in which transformations are expressed.
class XSIEXPORT CSLCoordinateSystem
	: public CSLTemplate
{
public:
	/** 
	* This enum list the possible hand rotation for the coordinate system.
	*/
	enum EHandRotation
	{
		SI_LEFT_HANDED,		//!<Left handed coordinate system
		SI_RIGHT_HANDED,	//!<Right handed coordinate system
	};

	/** 
	* This enum list the possible U axis orientation for the coordinate system.
	*/
	enum EUOrientation
	{
		SI_RIGHT_U,			//!<Positive U goes right
		SI_LEFT_U,			//!<Positive U goes left
	};

	/** 
	* This enum list the possible V axis orientation for the coordinate system.
	*/
	enum EVOrientation
	{
		SI_DOWN_V,			//!<Positive V goes down
		SI_UP_V,			//!<Positive V goes up
	};

	/** 
	* This enum list the possible X, Y & Z axis orientation for the coordinate system.
	*/
	enum EAxisOrientation
	{
		SI_RIGHT,			//!<Positive X goes right
		SI_LEFT,			//!<Positive X goes left
		SI_UP,				//!<Positive Y goes up
		SI_DOWN,			//!<Positive Y goes down
		SI_IN,				//!<Positive Z goes in
		SI_OUT,				//!<Positive Z goes out
	};

	/** 
	* This enum list all coordinate system style the class can recognize.
	*/
	enum ECoordinateSystemStyle
	{
		SI_SOFTIMAGE_3D,
		SI_D3D,
		SI_PLAYSTATION,
		SI_NINTENDO,
	};

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the coordinate system
	* \param in_pModel	Pointer to the model containing the coordinate system
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the coordinate system
	* \return an instance of CSLCoordinateSystem
	*/
	CSLCoordinateSystem(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLCoordinateSystem();

	/*! Return the hand rotation of the coordinate system.
	* \return EHandRotation	Hand rotation.
	*/
	EHandRotation			GetHandRotation() { return m_HandRotation; }

	/*! Change the hand rotation of the coordinate system.
	* \param	in_Type	New hand rotation.
	* \return SI_Void
	*/
	SI_Void					SetHandRotation(EHandRotation in_Type) { m_HandRotation = in_Type; }

	/*! Return the orientation of the U axis for texture coordinates.
	* \return EUOrientation	Orientation of the U axis.
	*/
	EUOrientation			GetUAxisOrientation() { return m_UAxis; }

	/*! Change the orientation of the U axis for texture coordinates.
	* \param	in_Type	New orientation of the U axis.
	* \return SI_Void
	*/
	SI_Void					SetUAxisOrientation(EUOrientation in_Type) { m_UAxis = in_Type; }

	/*! Return the orientation of the V axis for texture coordinates.
	* \return EVOrientation	Orientation of the V axis.
	*/
	EVOrientation			GetVAxisOrientation() { return m_VAxis; }

	/*! Change the orientation of the V axis for texture coordinates.
	* \param	in_Type	New orientation of the V axis.
	* \return SI_Void
	*/
	SI_Void					SetVAxisOrientation(EVOrientation in_Type) { m_VAxis = in_Type; }

	/*! Return the orientation of the X axis in the coordinate system.
	* \return EAxisOrientation	Orientation of the X axis.
	*/
	EAxisOrientation		GetXAxisOrientation() { return m_XAxis; }

	/*! Change the orientation of the X axis in the coordinate system.
	* \param	in_Type	New orientation of the X axis.
	* \return SI_Void
	*/
	SI_Void					SetXAxisOrientation(EAxisOrientation in_Type) { m_XAxis = in_Type; }

	/*! Return the orientation of the Y axis in the coordinate system.
	* \return EAxisOrientation	Orientation of the Y axis.
	*/
	EAxisOrientation		GetYAxisOrientation() { return m_YAxis; }

	/*! Change the orientation of the Y axis in the coordinate system.
	* \param	in_Type	New orientation of the Y axis.
	* \return SI_Void
	*/
	SI_Void					SetYAxisOrientation(EAxisOrientation in_Type) { m_YAxis = in_Type; }

	/*! Return the orientation of the Z axis in the coordinate system.
	* \return EAxisOrientation	Orientation of the Z axis.
	*/
	EAxisOrientation		GetZAxisOrientation() { return m_ZAxis; }

	/*! Change the orientation of the Z axis in the coordinate system.
	* \param	in_Type	New orientation of the Z axis.
	* \return SI_Void
	*/
	SI_Void					SetZAxisOrientation(EAxisOrientation in_Type) { m_ZAxis = in_Type; }

	/*! Change the coordinate system to reflect a certain 3D environment.
	* \param ECoordinateSystemStyle	New coordinate system style.
	* \return SI_Void
	*/
	SI_Void					SetCoordinateSystemStyle(ECoordinateSystemStyle in_Style);

	/*! Get the template type
	* \return ETemplateType the template type of the coordinate system
	*/
	virtual ETemplateType	Type() { return SI_COORDINATE_SYSTEM; }

private:
	CSLEnumProxy<EHandRotation, SI_RIGHT_HANDED>	m_HandRotation;
	CSLEnumProxy<EUOrientation, SI_LEFT>			m_UAxis;
	CSLEnumProxy<EVOrientation, SI_UP>				m_VAxis;
	CSLEnumProxy<EAxisOrientation, SI_OUT>			m_XAxis;
	CSLEnumProxy<EAxisOrientation, SI_OUT>			m_YAxis;
	CSLEnumProxy<EAxisOrientation, SI_OUT>			m_ZAxis;
};

#endif
