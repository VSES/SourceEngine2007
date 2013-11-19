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


#ifndef _ACTIONCLIP_H
#define _ACTIONCLIP_H

#include "Template.h"

// Forward declaration
class CSLTimeControl;
class CSLAction;

/*! An action clip is an instance of an action source (CSLAction) that can be 
	used in an animation mixer (CSLMixer) track (CSLTrack).  You can create an
	action clip with the CSLTrack::AddActionClip method.

	\note CSLActionClip is not supported by SI3D.

	\sa CSLAction
	\sa CSLMixer
	\sa CSLTimeControl
	\sa CSLTrack
*/
class XSIEXPORT CSLActionClip
    : public CSLTemplate
{
public:

	/*! Constructor
		\param in_pScene	Pointer to the scene containing the ActionClip
		\param in_pModel	Pointer to the model containing the ActionClip
		\param in_pTemplate	Pointer to the CdotXSITemplate defining the ActionClip
	*/
    CSLActionClip(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
    virtual ~CSLActionClip();

	SI_Error Synchronize();

	/*! Gets the type of this template
		\return CSLTemplate::XSI_ACTIONCLIP
	*/
	CSLTemplate::ETemplateType Type(){ return CSLTemplate::XSI_ACTIONCLIP; }

	/*! Gets the active flag that determines whether the clip contributes to the animation or not.
		\retval true The clip contributes to the animation
		\retval false The clip does \b not contribute to the animation
	*/
    SI_Bool GetActive();

	/*! Sets whether the clip contributes to the animation or not
		\param in_bew true to contribute, false otherwise
	*/
	SI_Void SetActive( SI_Bool in_bNew );

    /*! Gets the length of the clip
		\return SI_Float Duration
	*/
	SI_Float GetDuration();

	/*! Sets the the length of the clip
		\param in_fNew New Duration
	*/
	SI_Void SetDuration( SI_Float in_fNew );

	/*! Gets the fill action flag
		\retval true The clip is a fill action
		\retval false The clip is \b not a fill action
		\note Fill actions determine parameter values only when there is no 
		non-fill action driving particular parameters. They can be used to 
		set parameters to reference values between action clips. Fill 
		actions are not mixed with other clips.
	*/
    SI_Bool GetFillAction();

	/*! Sets the fill action flag
		\param in_bew True to enable fill action, false otherwise
		\note Fill actions determine parameter values only when there is no 
		non-fill action driving particular parameters. They can be used to 
		set parameters to reference values between action clips. Fill 
		actions are not mixed with other clips.
	*/
	SI_Void SetFillAction( SI_Bool in_bNew );

	/*! Gets the priority of the fill action
		\return SI_Int Fill priority value
		\note If two fill actions drive the same parameter at a given frame,
		the one with the higher priority is used.
	*/
    SI_Int GetFillPriority();

	/*! Sets the priority of the fill action
		\param in_nNew New fill priority value
		\note If two fill actions drive the same parameter at a given frame,
		the one with the higher priority is used.
	*/
	SI_Void SetFillPriority( SI_Int in_nNew );

	/*! Gets the start time of the clip
		\return SI_Float Start time
	*/
	SI_Float GetStartTime();

	/*! Sets the start time of the clip
		\param in_fNew New start time
	*/
	SI_Void SetStartTime( SI_Float in_fNew );

	/*! Gets the relative weight of the action clip when it is mixed with other clips that affect the same parameters or clusters at the same frames.
		\return SI_Float Weight
	*/
	SI_Float GetWeight();

	/*! Sets the relative weight of the action clip when it is mixed with other clips that affect the same parameters or clusters at the same frames.
		\return in_fNew New weight
	*/
	SI_Void SetWeight( SI_Float in_fNew );

	// action functionality ///////////////////////////////////////////////////
	
	/*! Gets the action source
		\retval Pointer to the action source
		\retval NULL No action source was set, this should never happen unless 
		it was ovverriden by mistake by CSLActionClip::SetReference.
	*/
	CSLAction* GetReference();

	/*! Sets the action source
		\param in_pAction Pointer to the new Referred action
		\note The internal action source pointer will be replaced by the one
		provided.  This is perfectly normal because there can/will be other 
		CSLActionClips instanciating it.
	*/
	SI_Void SetReference( CSLAction* in_pAction );

	SI_Char* GetReferenceXSIPath();
	SI_Void SetReferenceXSIPath( SI_Char* in_szPath );

	// time control functionality /////////////////////////////////////////////
	
	/*! Add a new time control
		\return CSLTimeControl Pointer to the newly allocated time control
	*/
	CSLTimeControl* AddTimeControl();

	/*! Add a previously allocated time control
		\param in_pTimeControl Pointer to the time control to add
		\return CSLTimeControl Pointer to added time control or null if already present
	*/
	CSLTimeControl* ConnectTimeControl( CSLTimeControl *in_pTimeControl );

	/*! Removes a specific time control
		\param SI_Index Index of the control to remove
		\retval SI_SUCCESS The element was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error RemoveTimeControl( SI_Int in_nIndex );

	/*! Removes a specific time control
		\param in_pTimeControl Pointer of the control to remove
		\retval SI_SUCCESS The element was removed
		\retval SI_ERR_BAD_ARGUMENT Could not find the element to remove
	*/
	SI_Error RemoveTimeControl( CSLTimeControl *in_pTimeControl );

	/*! Gets the list of all time controls
		\return CSLTimeControl Pointer to the list of time control pointers
	*/
	CSLTimeControl** GetTimeControlList();

	/*! Gets the time control count
		\return SI_Int Number of time control in the list
	*/
	SI_Int GetTimeControlCount();

	/*! Frees all time controls
		\retval SI_SUCCESS
	*/
	SI_Error ClearTimeControls();

private:
    CSLFloatProxy m_StartTime;
    CSLFloatProxy m_Duration;
    CSLBoolProxy m_Active;
    CSLFloatProxy m_Weight;
    CSLBoolProxy m_FillAction;
    CSLIntProxy m_FillPriority;
	CSLStringProxy m_ReferencePath;
	CSLAction* m_pReference;
    CSIBCArray<CSLTimeControl *> m_TimeControls;
	SI_Void *m_pReserved;
};

#endif
