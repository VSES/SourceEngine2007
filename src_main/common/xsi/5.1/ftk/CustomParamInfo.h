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

#ifndef _CUSTOMPARAMINFO_H
#define _CUSTOMPARAMINFO_H

#include "Template.h"
#include "VariantParameter.h"

class CSLCustomPSet;

//! Provides additional information concerning a specific parameter in a CSLCustomPSet.
/*!
	The CSLXSICustomParamInfo class enhances the usability of the CSLCustonPSet 
	class by providing minimum and	maximum range values of a parameter.  It can
	also be used to define the capabilities of the parameter.  You can create a 
	custom parameter information with the CSLCustomPSet::CreateCustomParamInfo 
	method.

	\introduced 3.6
	
	\sa CSLCustomPSet
	\sa CSLXSICustomParamInfo::ECapabilitiesFlag
*/
class XSIEXPORT CSLXSICustomParamInfo
    : public CSLTemplate
{
	friend CSLXSICustomParamInfo* CSLCustomParameter::ConnectCustomParamInfo(CSLXSICustomParamInfo*);
public:
	/*! Definition of all possible parameter capabilies flags.
		
		You can use a combination of flags using the bitwise 'or' operator

		\b Example:
		\code
		CSLXSICustomParamInfo::SI_ANIMATABLE | CSLXSICustomParamInfo::SI_PERSISTABLE;
		\endcode
	*/
	typedef enum
	{
		SI_ANIMATABLE			= 1,	/*!< Animatable */
		SI_READ_ONLY			= 2,	/*!< Cannot be written to */
		SI_PERSISTABLE			= 4,	/*!< Is saved with its parameter set */
		SI_NOT_INSPECTABLE		= 8,	/*!< Does not show up in the UI */
		SI_SILENT				= 16,	/*!< Does not notify if its value is changed */
		SI_NOT_PSET_PERSISTABLE	= 128,	/*!< Will not be saved in presets */
		SI_TEXTURABLE			= 256,	/*!< Parameter support connection to rendering node */
	} ECapabilitiesFlag;

	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate referred template
		\param in_pParameter Parameter whose definition is enhanced
	*/
    CSLXSICustomParamInfo(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, CSLVariantParameter *in_pParameter = 0);
    virtual ~CSLXSICustomParamInfo();

	SI_Error Synchronize();

	/*! Returns the type of this template
		\return CSLTemplate::XSI_CUSTOM_PARAM_INFO
	*/
	ETemplateType Type(){ return XSI_CUSTOM_PARAM_INFO; }

	/*! Gets the minimum value
		\param out_Value The minimum value
		\retval SI_SUCCESS Got the minimum value
		\retval SI_ERR_BAD_ARGUMENT The variant type of \p out_Value does not match the type of the minimum value.
	*/
	SI_Error GetMinValue( SI_TinyVariant &out_Value );

	/*! Gets the minimum value
		\return CSLVariantProxy The minimum value of the parameter
	*/
	CSLVariantProxy& GetMinValue();

	/*! Sets the minimum value
		\param in_Value The minimum value
		\retval SI_SUCCESS The value was set
		\retval SI_ERR_BAD_ARGUMENT The variant type of \p in_Value does not match the type of the minimum value.
	*/
	SI_Error SetMinValue( const CSLVariantProxy &in_Value );

	/*! Sets the minimum value
		\param in_Value The minimum value
		\retval SI_SUCCESS The value was set
		\retval SI_ERR_BAD_ARGUMENT The variant type of \p in_Value does not match the type of the minimum value.
	*/
	SI_Error SetMinValue( const SI_TinyVariant &in_Value );
	
	/*! Gets the maximum value
		\param out_Value The maximum value of the parameter
		\retval SI_SUCCESS Got the maximum value
		\retval SI_ERR_BAD_ARGUMENT The variant type of \p out_Value does not match the type of the minimum value.
	*/
	SI_Error GetMaxValue( SI_TinyVariant &out_Value );

	/*! Gets the maximum value
		\return CSLVariantProxy The maximum value of the parameter
	*/
	CSLVariantProxy& GetMaxValue();
	
	/*! Sets the maximum value
		\param in_Value The maximum value
		\retval SI_SUCCESS The value was set
		\retval SI_ERR_BAD_ARGUMENT The variant type of \p in_Value does not match the type of the minimum value.
	*/
	SI_Error SetMaxValue( const CSLVariantProxy &in_Value );

	/*! Sets the maximum value
		\param in_Value The maximum value
		\retval SI_SUCCESS The value was set
		\retval SI_ERR_BAD_ARGUMENT The variant type of \p in_Value does not match the type of the minimum value.
	*/
	SI_Error SetMaxValue( const SI_TinyVariant &in_Value );

	/*! Gets the Capabilities flag value
		\return SI_ULong Capabilities flags

		You can test for specific capabilities with the bitwise
		'and' operator.

		\b Example:
		\code
		SI_ULong l_uCapabilities = l_pCustomParamInfo->GetCapabilities();
		// if the paramater is animatable, do something
		if ( CSLXSICustomParamInfo::SI_ANIMATABLE & l_uCapabilities )
		{
			// Do something
		}
		\endcode
	*/
	SI_ULong GetCapabilities();

	/*! Sets the Capabilities flag value.
		\param in_ulValue  New capabilities flags
		
		Please note that in_ulValue must be a combination of valid values
		coming from the CSLXSICustomParamInfo::ECapabilitiesFlag enum.
		You can combine values with the bitwise 'or' operator.

		\b Example: 
		\code
		l_pCustomParamInfo->SetCapabilities
		( 
			(SI_ULong)CSLXSICustomParamInfo::SI_ANIMATABLE | 
			(SI_ULong)CSLXSICustomParamInfo::SI_PERSISTABLE 
		);
		\endcode
	*/
	SI_Void SetCapabilities( SI_ULong in_ulValue );

private:
	void SetParameter( CSLVariantParameter *in_pParam ){ m_pParameter = in_pParam; }
    CSLVariantProxy m_MinValue;
    CSLVariantProxy m_MaxValue;
    CSLULongProxy m_Capabilities;
    
	CSLVariantParameter *m_pParameter;

	SI_Void *m_pReserved;
};

#endif
