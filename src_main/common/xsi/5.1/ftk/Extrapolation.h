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

#ifndef _EXTRAPOLATION_H
#define _EXTRAPOLATION_H

#include "Template.h"

/*! Determine how a an action clip (CSLActionClip) contributes to the 
	animation outside of the frames on which it is defined.  You can create 
	an instance of this class with the CSLTimeControl::CreateExtrapolation
	method.

	\note CSLExtrapolation is not supported by SI3D

	\sa CSLActionClip
	\sa CSLTimeControl
*/
class XSIEXPORT CSLExtrapolation
    : public CSLTemplate
{
public:

	/*! Defines the type of extrapolation to perform
	*/
	enum EExtrapolationType
	{
		SI_NO_CONTRIBUTION,		//!< Results in no extrapolation. Use this to delete an extrapolation.
		SI_HOLD,				//!< Results in the first or last values being held for the number of frames specified by CSLExtrapolation::GetValue.
		SI_CYCLE,				//!< Repeats the clip for the number of times specified by CSLExtrapolation::GetValue.
		SI_BOUNCE,				//!< Repeats the clip forward, then backward, and so on for the number of times specified by CSLExtrapolation::GetValue. 
	};

	/*! Constructor
		\param in_pScene The scene containing this template
		\param in_pModel The parent model containing this template
		\param in_pTemplate The actual template the defines the extrapolation type
		\param in_Type Default extrapolation type to use
	*/
	CSLExtrapolation
	(
		CSLScene* in_pScene, 
		CSLModel* in_pModel, 
		CdotXSITemplate* in_pTemplate, 
		EExtrapolationType in_Type = CSLExtrapolation::SI_NO_CONTRIBUTION
	);

    virtual ~CSLExtrapolation();

	/*! Returns the type of this template
		\retval CSLTemplate::XSI_EXTRAPOLATION
	*/
	CSLTemplate::ETemplateType Type(){ return CSLTemplate::XSI_EXTRAPOLATION; }

	/*! Gets the extrapolation type
		\return Type of extrapolation
	*/
	EExtrapolationType GetExtrapolationType();

	/*! Sets the extrapolation type
		\return in_Type New type of extrapolation
	*/
	SI_Void SetExtrapolationType( CSLExtrapolation::EExtrapolationType in_Type );

	/*! Gets the extrapolation value
		\return Extrapolation value
		\note The meaning of the value changes according to the extrapolation type
		\sa CSLExtrapolation::EExtrapolationType
	*/

	SI_Float GetValue();

	/*! Sets the extrapolation value
		\return in_fValue New extrapolation value
		\note The meaning of the value changes according to the extrapolation type.
		\sa CSLExtrapolation::EExtrapolationType
	*/
	SI_Void SetValue( SI_Float in_fValue );

private:
    CSLStrEnumProxy<EExtrapolationType, SI_BOUNCE> m_ExtrapolationType;
    CSLFloatProxy m_Value;

	SI_Void* m_pReserved;
};

#endif
