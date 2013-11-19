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

#ifndef _MIXER_H
#define _MIXER_H

#include "Template.h"

// Forward declaration
class CSLAction;
class CSLTrack;

/*! The animation mixer provides high-level control over animation.  The 
	mixer contains a database of short animations called action sources 
	(CSLAction).  An action source can be instantiated in the form of an
	action clip (CSLActionClip) in tracks (CSLTrack) to compose complex 
	animation.  You can create an animation mixer with the 
	CSLModel::CreateMixer method.

	\note This template is not supported by SI3D.
	
	\sa CSLAction
	\sa CSLActionClip
	\sa CSLModel
	\sa CSLTrack
*/
class XSIEXPORT CSLMixer
    : public CSLTemplate
{
public:
	/*! Constructor
		\param in_pScene	Pointer to the scene containing the mixer
		\param in_pModel	Pointer to the model containing the mixer
		\param in_pTemplate	Pointer to the CdotXSITemplate containing the mixer
	*/
    CSLMixer(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLMixer();
    
	SI_Error Synchronize();

	/*! Returns the type of this template
		\retval CSLTemplate::XSI_MIXER
	*/
	CSLTemplate::ETemplateType Type(){ return CSLTemplate::XSI_MIXER; }

	/*! Determines if the animation mixer is active or not
		\return SI_Bool Value of the flag
	*/
    SI_Bool GetActive();

	/*! Toggles the animation mixer evaluation
		\param in_bNew Value of the flag
	*/
	SI_Void SetActive( SI_Bool in_bNew );

	/*! Determines if inter-clip interpolation is enabled or not
		\return SI_Bool Value of the flag
	*/
    SI_Bool GetAutoTransition();

	/*! Toggles inter-clip interpolation
		\param in_bNew Value of the flag
	*/
	SI_Void SetAutoTransition( SI_Bool in_bNew );

	/*! Gets the Maintain Continuity Flag
		\retval True Maintain Continuity is enabled
		\retval False Maintain Continuity is disabled
		\note When you select Quaternion for Rotation Mixing, this option 
		generates more continuous rotation curves, especially for operations 
		such as plotting/freezing rotation parameters driven by the animation 
		mixer.
	*/
    SI_Bool GetMaintainContinuity();

	/*! Sets the Maintain Continuity Flag
		\param in_bNew True to enable, false to disable
		\note When you select Quaternion for Rotation Mixing, this option 
		generates more continuous rotation curves, especially for operations 
		such as plotting/freezing rotation parameters driven by the animation 
		mixer.
		\warning This flag is ignored unless QuaternionMixing is enabled.
		\sa CSLMixer::SetQuaternionMixing
	*/
	SI_Void SetMaintainContinuity( SI_Bool in_bNew );

	/*! Gets how the mix is calculated
		\retval True Mixes are a weighted averages. The results are mixes that 
		fall in-between the values of the separate clips.
		\retval False mixes are additive. The values of the separate clips are 
		added on top of each other.
	*/
    SI_Bool GetNormalize();

	/*! Sets how the mix is calculated
		\param in_bNew Value of the flag
		\note When set to \b True, mixes are a weighted averages. The results 
		are mixes that fall in-between the values of the separate clips.  When
		set to \b False, mixes are additive. The values of the separate clips 
		are added on top of each other.
	*/
	SI_Void SetNormalize( SI_Bool in_bNew );

	/*! Gets the Quaternion Mixing Flag
		\retval True Rotation values are mixed according to their Quaternion values
		\retval False Rotation values are mixed according to their Euler values
		\note Quaternions usually result in smoother rotations; however, if you are 
		mixing two or more rotations and using Quaternion, there may be some 
		unexpected flips in the animation. Using Euler will probably solve this 
		problem.
	*/
    SI_Bool GetQuaternionMixing();

	/*! Sets the Quaternion Mixing Flag
		\param in_bNew Value of the flag
		\note Controls whether rotation values are mixed according to their Quaternion 
		values (\b True) or their Euler values (\b False). Quaternions usually result 
		in smoother rotations; however, if you are mixing two or more rotations and 
		using Quaternion, there may be some unexpected flips in the animation. 
		Using Euler will probably solve this problem.
	*/
	SI_Void SetQuaternionMixing( SI_Bool in_bNew );

	/*! Gets the Remove Spins Flag
		\return SI_Bool Value of the flag
	*/
    SI_Bool GetRemoveSpins();

	/*! Sets the Remove Spins Flag
		\param in_bNew Value of the flag
		\note When set to \b True, this option removes extra spinning that may occur in a rotation so that 
		the object's rotation values are only between -180 and 180. For example, you 
		can sometimes get rotation values with extra "spins," such as 
		[-591, 1015, 15]. This option gives you the same rotation but without the 
		spins.
		\warning This flag is ignored unless Quaternion Mixing is disabled.
		\sa CSLMixer::SetQuaternionMixing
	*/
	SI_Void SetRemoveSpins( SI_Bool in_bNew );

	// Actions functionality //////////////////////////////////////////////////

	/*! Removes an action
		\param in_nIndex Index of the CSLAction to remove
		\retval SI_SUCCESS The element was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
    SI_Error RemoveAction( SI_Int in_nIndex );

	/*! Removes an action
		\param in_pToRemove Pointer to the action to remove
		\retval SI_SUCCESS The element was removed
		\retval SI_ERR_BAD_ARGUMENT The element could not be found
	*/
	SI_Error RemoveAction( CSLAction *in_pToRemove );

	/*! Gets the list of all actions
		\return Pointer to the list of pointer of all actions
	*/
	CSLAction** GetActionList();

	/*! Get the total number of actions
		\return SI_Int The number of actions
	*/
	SI_Int GetActionCount();

	/*! Adds a new action
		\return CSLAction Pointer to the newly added action
	*/
	CSLAction* AddAction();

	/*! Removes all actions
		\return SI_Error Whether or not removal was successful
	*/
	SI_Error ClearActions();

	/*! Adds a preallocated action to the action list
		\param CSLAction Pointer to the action to add
		\return CSLAction The newly added action or null if it was already added
	*/
	CSLAction* ConnectAction(CSLAction* in_pNewAction);

	// Tracks functionality //////////////////////////////////////////////////
	
	/*! Removes a track
		\param in_nIndex Index of the track to remove
		\retval SI_SUCCESS The element was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error RemoveTrack( SI_Int in_nIndex );

	/*! Removes a track
		\param in_pToRemove Pointer to the track to remove
		\retval SI_SUCCESS The element was removed
		\retval SI_ERR_BAD_ARGUMENT The element could not be found
	*/
	SI_Error RemoveTrack( CSLTrack *io_pToRemove );

	/*! Gets the list of all tracks
		\return Pointer to the list of pointer of all tracks
	*/
    CSLTrack** GetTrackList();

	/*! Get the total number of tracks
		\return SI_Int The number of tracks
	*/
	SI_Int GetTrackCount();

	/*! Adds a new track
		\return CSLTrack Pointer to the newly added track
	*/
	CSLTrack* AddTrack();

	/*! Removes all Tracks
		\return SI_Error Whether or not removal was successful
	*/
	SI_Error ClearTracks();

	/*! Adds a preallocated Track to the Track list
		\param CSLTrack Pointer to the Track to add
		\return CSLTrack The newly added Track or null if it was already added
	*/
	CSLTrack* ConnectTrack(CSLTrack* in_pNewTrack);
	
private:
    CSLBoolProxy m_AutoTransition;
    CSLBoolProxy m_Active;
    CSLBoolProxy m_Normalize;
    CSLBoolProxy m_QuaternionMixing;
    CSLBoolProxy m_RemoveSpins;
    CSLBoolProxy m_MaintainContinuity;
    CSIBCArray<CSLAction *> m_Actions;
    CSIBCArray<CSLTrack *> m_Tracks;

	SI_Void *m_pReserved;
};

#endif // _MIXER_H
