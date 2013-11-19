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

#ifndef _FCURVE_H
#define _FCURVE_H

#include "BaseFCurve.h"
#include "SL_Enum.h"

extern XSIEXPORT CEnumDictionary<CSLTemplate::EFCurveType, CSLTemplate::SI_INTENSITY> g_FCurveTypeDictionary;

XSIEXPORT	CEnumDictionary<CSLTemplate::EFCurveType, CSLTemplate::SI_INTENSITY>* GetFCurveEnumDictionary();

//! FCurve class
class XSIEXPORT CSLFCurve
    : public CSLBaseFCurve
{
public:
	typedef CSLStrEnumProxy<EFCurveType, SI_INTENSITY> EFCurveTypeProxy;

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the fcurve
	* \param in_pModel	Pointer to the model containing the fcurve
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the fcurve
	* \param in_Type	Type of fcurve
	* \param in_InterpolationType	Interpolation type for this fcurve
	* \return an instance of CSLFCurve
	*/
    CSLFCurve
	(
		CSLScene* in_pScene,
		CSLModel *in_pModel,
		CdotXSITemplate* in_pTemplate, 
		EFCurveType in_Type,
		EFCurveInterpolationType in_InterpolationType
	);

	/*! Destructor
	*/
    virtual ~CSLFCurve();

	/*! Get the model to which this fcurve belongs
	* \return CSLModel* the model to which this fcurve belongs
	*/
    CSLModel* Reference();

	/*! Get the type of fcurve
	* \return EFCurveType type of this fcurve
	*/
    EFCurveType GetFCurveType();

	/*! Get the fcurve type as a string value
	* \return SI_Char* the type of the fcurve in formatted in a string
	*/
	const SI_Char* GetFCurveTypeAsString();

	/*! Synchronizes this fcurve instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
    virtual SI_Error Synchronize();

	/*! Get the template type
	* \return ETemplateType the template type of the fcurve
	*/
    virtual ETemplateType Type();
	
private:
   EFCurveType m_FCurveType;

	void *m_pReserved;	// reserved for future extension
};

#endif
