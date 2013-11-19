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

#ifndef _VARIANTPARAMETER_H
#define _VARIANTPARAMETER_H

#include "Template.h"

class CSLXSICustomParamInfo;

/** 
* Class CSLVariantParameter
*
* Class representing variant parameters
*
* \sa CSLCustomPSet
*/
class XSIEXPORT CSLVariantParameter : public CSLAnimatableType
{
public:
	/*! Constructor
		\param in_pTemplate Parent Template
		\parm in_nValueIndex Index
	*/
	CSLVariantParameter
	( 
		CdotXSITemplate *in_pTemplate, 
		SI_Int in_nValueIndex 
	);
	
	//! Destructor
	virtual ~CSLVariantParameter();
	
	/*!
		\return CSLAnimatableType::EElementType The type stored in the variant
	*/
	CSLAnimatableType::EElementType ValueType();

	/*! Gets the name of the parameter
		\return SI_Char String representing the name of the parameter
	*/
	SI_Char* GetName();

	/*! Sets the name of the parameter
		\param in_szName New name for the parameter
	*/
	SI_Void SetName( SI_Char* in_szName );
	
	/*! Gets the stored value
		\return SI_TinyVariant Pointer to the stored value
	*/
	SI_TinyVariant* GetValue();

	/*! Sets the stored value
		\return in_pNewVal Pointer to the new value to store
	*/
	SI_Void SetValue( SI_TinyVariant* in_pNewVal );

	/*! Returns the animatable type type (SI_VARIANT_TYPE)
		\return CSLVariantParameter::EElementType Type of this element (SI_VARIANT_TYPE)
	*/
	CSLVariantParameter::EElementType Type(){ return CSLVariantParameter::SI_VARIANT_TYPE; }

	/*! Gets custom parameter information
		\retval NULL
		\note A CSLVariantParameter cannot contain custom parameter 
		information.
	*/
	virtual CSLXSICustomParamInfo* CustomParamInfo(){ return (CSLXSICustomParamInfo *)NULL; }

	virtual SI_Float GetFloatValue();
	virtual SI_Void SetFloatValue(SI_Float in_fValue);

private:
	CdotXSIParam *m_pParam;
};


/** 
* Class CSLCustomParameter
*
* Defines parameters in custom presets
*
* \sa CSLCustomPSet
* \note this class was introduced in dotXSI version 3.6
*/
class XSIEXPORT CSLCustomParameter :
	public CSLVariantParameter
{
public:
	/*! Constructor
		\param in_pTemplate Parent Template
		\parm in_nValueIndex Index
	*/
	CSLCustomParameter
	( 
		CdotXSITemplate *in_pTemplate, 
		SI_Int in_nValueIndex 
	);
	
	//! Destructor
	virtual ~CSLCustomParameter();

	///////////////////////////////////////////////////////////////////////////
	// Custom parameter information
	///////////////////////////////////////////////////////////////////////////

	/*! Gets custom parameter information
		\return CSLXSICustomParamInfo Pointer to the custom parameter information or null if there's none
	*/
	CSLXSICustomParamInfo* CustomParamInfo();

	/*! Connects custom parameter information
		\param in_pCustomParamInfo Pointer to the custom parameter information or null if there's none
	*/
	CSLXSICustomParamInfo* ConnectCustomParamInfo( CSLXSICustomParamInfo *in_pCustomParamInfo );

private:
	CSLXSICustomParamInfo *m_pCustomParamInfo;
};

#endif
