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


#ifndef _TIMECONTROL_H
#define _TIMECONTROL_H

#include "Template.h"
#include "Extrapolation.h"

//! Stores the clipping and offset parameters of an action clip (CSLActionClip)
/*! You can add a CSLTimeControl with the CSLActionClip::AddTimeControl 
	method.
	\note CSLTimeControl is not supported by SI3D.

	\sa CSLActionClip
	\sa CSLExtrapolation
*/
class XSIEXPORT CSLTimeControl
    : public CSLTemplate
{
public:

	//! Specifies where extrapolation occures while processing an CSLActionClip
	enum EExtrapolationPos
	{
		SI_BEFORE,	//!< Extrapolation performed \b before the CSLActionClip
		SI_AFTER	//!< Extrapolation performed \b after the CSLActionClip
	};

	/*! Constructor
		\param in_pScene	Pointer to the scene containing the CSLTimeControl
		\param in_pModel	Pointer to the model containing the CSLTimeControl
		\param in_pTemplate	Pointer to the CdotXSITemplate containing the CSLTimeControl data
	*/
    CSLTimeControl(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLTimeControl();

	/*! Gets the type of this template
		\retval CSLTemplate::XSI_TIMECONTROL
	*/
	CSLTemplate::ETemplateType Type(){ return CSLTemplate::XSI_TIMECONTROL; }
	SI_Error Synchronize();

	/*! Gets the first frame of the source that is used by the clip.
		\return Frame number
	*/
	SI_Float GetIn();

	/*! Sets the first frame of the source that is used by the clip.
		\param in_fNew Frame number
	*/
	SI_Void SetIn( SI_Float in_fNew );
    
	/*! Gets the last frame of the source that is used by the clip.
		\return Frame number
	*/
	SI_Float GetOut();

	/*! Sets the last frame of the source that is used by the clip.
		\param in_fNew Frame number
	*/
	SI_Void SetOut( SI_Float in_fNew );
    
	/*! Gets the scaling of the clip in time.
		\return SI_Float The scale value
	*/
	SI_Float GetScale();

	/*! Sets The scaling of the clip in time.
		\param in_fNew Scale factor
		\note Increasing this value speeds up the relative time of the clip, decreasing the duration.
	*/
	SI_Void SetScale( SI_Float in_fNew );

	/*! Gets the frame where the clip starts in the local time of the CSLActionClip.
		\return SI_Float The start offset
	*/
	SI_Float GetStartOffset();

	/*! Sets he frame where the clip starts in the local time of the CSLActionClip.
		\param in_fNew The new start offset value
	*/
	SI_Void SetStartOffset( SI_Float in_fNew );

	// extrapolation functionality ////////////////////////////////////////////

	/*! Creates a new extrapolation and connect it
		\param in_Pos Specifies where the extrapolation must be performed
		\param in_Type Type of extrapolation to create
		\retval CSLExtrapolation Pointer to the newly created extrapolation.
		\retval NULL An extrapolation at \p in_Pos already exists.
	*/
    CSLExtrapolation* CreateExtrapolation( EExtrapolationPos in_Pos, CSLExtrapolation::EExtrapolationType in_Type = CSLExtrapolation::SI_NO_CONTRIBUTION );
	
	/*! Gets one of the extrapolation
		\param in_Pos Specifies the extrapolation position to get
		\retval Pointer to the requested extrapolation
		\retval NULL There is no extrapolation at the requested position
	*/
	CSLExtrapolation* GetExtrapolation( EExtrapolationPos in_Pos );

	/*! Sets one of the extrapolation position
		\param in_Pos The extrapolation position to set.
		\param in_pExtrapolation The extrapolation to put at the requested position
		\warning This method overwrites any existing extrapolation and might cause
		leaks.  You either have to free previously allocated extrapolation or use
		the non-destructive CSLTimeControl::ConnectExtrapolation method instead.
		\sa CSLTimeControl::ConnectExtrapolation
	*/
	SI_Void SetExtrapolation( EExtrapolationPos in_Pos, CSLExtrapolation *in_pExtrapolation );
	
	/*! Sets one of the extrapolation values if not already set
		\param in_Pos Specifies the extrapolation position to set
		\param in_pExtrapolation Extrapolation to connect
		\sa CSLTimeControl::SetExtrapolation
	*/
	CSLExtrapolation* ConnectExtrapolation( EExtrapolationPos in_Pos, CSLExtrapolation* in_pExtrapolation );

private:
    CSLFloatProxy m_In;
    CSLFloatProxy m_Out;
    CSLFloatProxy m_StartOffset;
    CSLFloatProxy m_Scale;
    CSLExtrapolation* m_pExtrapolationBefore;
    CSLExtrapolation* m_pExtrapolationAfter;
};

#endif
