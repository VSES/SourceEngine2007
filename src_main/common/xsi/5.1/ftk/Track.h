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

#ifndef _TRACK_H
#define _TRACK_H

#include "Template.h"
class CSLActionClip;

/*! Track are containers of action clips (CSLActionClip). They can be added
	to an animation mixer using the CSLMixer::AddTrack method.

	\note CSLTrack is not supported by SI3D

	\sa CSLAction
	\sa CSLActionClip
	\sa CSLMixer
*/
class XSIEXPORT CSLTrack
    : public CSLTemplate
{
public:
	/*! Constructor
		\param in_pScene	Pointer to the scene containing the CSLTrack
		\param in_pModel	Pointer to the model containing the CSLTrack
		\param in_pTemplate	Pointer to the CdotXSITemplate defining the CSLTrack data
	*/
    CSLTrack(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLTrack();

	SI_Error Synchronize();

	/*! Returns the type of this template
		\return CSLTemplate::ETemplateType The type of this template: XSI_TRACK
	*/
	CSLTemplate::ETemplateType Type(){ return XSI_TRACK; }

	/*! Gets the value of the mute flag
		\retval True The track contributes to the animation.
		\retval False The track \b does \b not contribute to the animation.

	*/
    SI_Bool GetMute();

	/*! Sets the value of the mute flag
		\return in_bNew New value
		\note When set to \b False, the track contributes to the animation.
	*/
	SI_Void SetMute( SI_Bool in_bNew );

	/*! Gets the value of the ripple flag
		\retval True A moved clip will push the next or previous clips along 
		the track if necessary so that clips do not overlap on the same track. 
		\retval False Clips cannot be moved past the position of the next or 
		previous clip.
		\note Adding new tracks will always ripple existing clips if necessary.
	*/
	SI_Bool GetRipple();

	/*! Sets the value of the ripple flag
		\param in_bNew New value
		\note When set to \b True, moved clips will push the next or previous 
		clips along the track if necessary so that clips do not overlap on the
		same track. 
		\note A \b False value can be used so clips cannot be moved past the 
		position of the next or previous clip.
	*/
	SI_Void SetRipple( SI_Bool in_bNew );

	/*! Gets the value of the solo flag
		\retval True Only this track contributes to the animation.
		\retval False Other tracks might contribute to the animation. 
	*/
    SI_Bool GetSolo();

	/*! Sets the value of the solo flag
		\param in_bNew New value for the flat
		\note Set this flag to \b True if you want this track to be the sole contributor to the animation.
	*/
	SI_Void SetSolo( SI_Bool in_bNew );


	// action clip functionality //////////////////////////////////////////////
	
	/*! Removes an action clip
		\param in_nIndex Index of the action clip to remove
		\retval SI_SUCCESS The element was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error RemoveActionClip( SI_Int in_nIndex );

	/*! Removes an action clip
		\param in_pToRemove Pointer to the action clip to remove
		\retval SI_SUCCESS The element was removed
		\retval SI_ERR_BAD_ARGUMENT The element could not be found
	*/
	SI_Error RemoveActionClip( CSLActionClip *io_pToRemove );

	/*! Gets the list of all action clips
		\return CSLActionClip Pointer to the list of pointer of all action clips
	*/
	CSLActionClip** GetActionClipList();

	/*! Get the total number of action clips
		\return SI_Int The number of action clips
	*/
	SI_Int GetActionClipCount();

	/*! Adds a new ActionClip
		\return CSLActionClip Pointer to the newly added action clip
	*/
	CSLActionClip* AddActionClip();

	/*! Removes all action clips
		\return SI_SUCCESS
	*/
	SI_Error ClearActionClips();

	/*! Adds a preallocated ActionClip to the action clip list
		\param CSLActionClip Pointer to the action clip to add
		\return CSLActionClip The newly added action clip or null if it was already added
	*/
	CSLActionClip* ConnectActionClip(CSLActionClip* in_pNewActionClip);

private:
    CSLBoolProxy m_Ripple;
    CSLBoolProxy m_Mute;
    CSLBoolProxy m_Solo;
    CSIBCArray<CSLActionClip *> m_ActionClips;

	SI_Void *m_pReserved;

};

#endif
