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
#ifndef _LIGHTINFO_H
#define _LIGHTINFO_H

#include "Template.h"

//! Provides additional information regarding a CSLLight
class XSIEXPORT CSLLightInfo
    : public CSLTemplate
{
public:

	//! Defines the various fallof modes
	enum EFallofMode
	{
		SI_LINEAR,		/*!< Linear */
		SI_EXPONENT,	/*!< Use exponent type */
	};
	/*! Constructor
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLLightInfo
	*/
    CSLLightInfo(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*
	*/
    virtual ~CSLLightInfo();

	/*! Gets the type of this template
	* \return ETemplateType The type of this template: SI_LIGHT_INFO
	*/
	ETemplateType Type(){ return SI_LIGHT_INFO; }

    virtual CSLAnimatableType*		ParameterFromName(SI_Char *in_szName);

    virtual CSLAnimatableType*		ParameterFromType(EFCurveType in_Type, SI_Char *in_szName);

	/*! Gets the fallof active state
	* \return SI_Bool Whether or not fallof is active
	*/
	SI_Bool GetFallofActive();

	/*! Sets the fallof active state
	* \param in_bActive Whether or not fallof is active
	* \return SI_Void
	*/
	SI_Void SetFallofActive( SI_Bool in_bActive );

	/*! Gets the fallof mode
	* \return EFallofMode The fallof mode
	*/
	EFallofMode GetFallofMode();

	/*! Sets the fallof mode
	* \param in_Mode The fallof mode
	* \return SI_Void
	*/
	SI_Void SetFallofMode( EFallofMode in_Mode );

	/*! Gets the start frame
	* \return SI_Float the start frame
	*/
	SI_Float GetStart();
	
	/*! Sets the start frame
	* \return in_fStart the start frame
	*/
	SI_Void SetStart( SI_Float in_fStart);

	/*! Gets the end frame
	* \return SI_Float the end frame
	*/
	SI_Float GetEnd();

	/*! Sets the end frame
	* \return in_fEnd the end frame
	*/
	SI_Void SetEnd( SI_Float in_fEnd);

	/*! Gets the shadow active state
	* \return SI_Bool Whether or not shadow is active
	*/
	SI_Bool GetShadowActive();

	/*! Sets the shadow active state
	* \param in_bActive Whether or not shadow is active
	* \return SI_Void
	*/
	SI_Void SetShadowActive( SI_Bool in_bActive );

	/*! Gets the Umbra value
	* \return SI_Float The umbra value
	*/
	SI_Float GetUmbra();

	/*! Sets the Umbra value
	* \param in_fUmbra The umbra value
	*/
	SI_Void SetUmbra( SI_Float in_fUmbra = 0.75F);

	/*! Gets the light as energy active state
	* \return SI_Bool Whether or not light as energy is active
	* \note Only values beetween 0 and 1 are valid
	*/
	SI_Bool GetLightAsEnergy();

	/*! Sets the light as energy active state
	* \param in_bActive Whether or not light as energy is active
	* \return SI_Void
	*/
	SI_Void SetLightAsEnergy( SI_Bool in_bActive );

	/*! Gets the photon factor value
	* \return SI_Float The photon factor value
	*/
	SI_Float GetPhotonFactor();

	/*! Sets the photon factor value
	* \param in_fPhotonFactor The photon factor value
	* \return SI_Void
	* \note Only values beetween 0 and 100000 are valid
	*/
	SI_Void SetPhotonFactor( SI_Float in_fPhotonFactor = 0.75F);

	/*! Gets the Intensity
	* \return SI_Float The Intensity
	*/
	SI_Float GetIntensity();
	/*! Sets the Intensity
	* \param in_fIntensity The intensity factor
	* \return SI_Void
	* \note Only values beetween 0 and 1.00000 are valid
	*/
	SI_Void SetIntensity( SI_Float in_fIntensity );


private:
    CSLBoolProxy m_FallofActive;
    CSLEnumProxy<EFallofMode, SI_EXPONENT> m_FallofMode;
    CSLFloatProxy m_Start;
    CSLFloatProxy m_End;
    CSLBoolProxy m_ShadowsActive;
    CSLFloatProxy m_Umbra;
    CSLBoolProxy m_LightAsEnergy;
    CSLFloatProxy m_PhotonFactor;
	CSLFloatProxy m_Intensity;

	SI_Void* m_pReserved;
};

#endif
