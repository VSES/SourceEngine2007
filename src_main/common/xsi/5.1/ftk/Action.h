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


#ifndef _ACTION_H
#define _ACTION_H

#include "Template.h"

class CSLActionFCurve;
class CSLStaticValue;
class CSLShapeAnimation;
class CSLXSIShapeAnimation;

/*! Actions, also known as action sources, are reusable animation segments. 
	They can be created with the CSLMixer::AddAction method and instanciated
	in the mixer's tracks (CSLTrack) as action clips (CSLActionClip).

	\note Actions are not supported by SI3D

	\sa CSLActionClip
	\sa CSLActionFCurve
	\sa CSLMixer
	\sa CSLShapeAnimation
	\sa CSLStaticValue
*/
class XSIEXPORT CSLAction
    : public CSLTemplate
{
public:
	/*! Constructor
		\param in_pScene	Pointer to the scene containing the CSLAction
		\param in_pModel	Pointer to the model containing the CSLAction
		\param in_pTemplate	Pointer to the CdotXSITemplate defining the CSLAction data
	*/
    CSLAction(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
    virtual ~CSLAction();

	//! Enumeration of all possible action types
	enum EActionType
	{
		SI_AT_FCURVE,			/*!< Fcurve action source item */
		SI_AT_STATICVALUE,		/*!< Static value source  */
		SI_AT_EXPRESSION,		/*!< Expression source */
		SI_AT_CLUSTERKEY,		/*!< Cluster key source */
		SI_AT_CONSTRAINT,		/*!< Constraint source */
		SI_AT_COMPOUND,			/*!< Compound Action item source. */
		SI_AT_SHAPECOMPOUND,	/*!< Shape compound Action item source. */
		SI_AT_NONE,				/*!< No flagged source */
	};

	SI_Error Synchronize();

	/*! Returns the type of this template
		\return CSLTemplate::XSI_ACTION
	*/
	CSLTemplate::ETemplateType Type(){ return CSLTemplate::XSI_ACTION; }

	/*! Gets the action type
		\return The action type
	*/
    EActionType GetActionType();

	/*! Sets the action type
		\param in_Type The action type
	*/
	SI_Void  SetActionType( EActionType in_Type );

	/*! Gets the end time
		\return The end time
	*/
	SI_Float GetEndTime();

	/*! Sets the end time of the action
		\param in_fTime The end time
	*/
	SI_Void SetEndTime( SI_Float in_fTime );

	/*! Gets the start time of the action
		\return The start time
	*/
	SI_Float GetStartTime();

	/*! Sets the start time of the action
		\param in_fTime The new start time
	*/
	SI_Void SetStartTime( SI_Float in_fTime );

	// Shape animations functionality //////////////////////////////////////////////////
	
	/*! Removes a shape animation
		\param in_nIndex Index of the shape animation to remove
		\retval SI_SUCCESS The shape animation was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error RemoveShapeAnimation( SI_Int in_nIndex );

	/*! Removes a shape animation
		\param in_pToRemove Pointer to the shape animation to remove
		\retval SI_SUCCESS The shape animation was removed
		\retval SI_ERR_BAD_ARGUMENT Could not find the shape animation
	*/
	SI_Error RemoveShapeAnimation( CSLShapeAnimation *io_pToRemove );

	/*! Gets the list of all shape animations
		\return Pointer to the list of pointer of all shape animations
	*/
	CSLShapeAnimation** GetShapeAnimationList();

	/*! Get the total number of shape animations
		\return The number of shape animations
	*/
	SI_Int GetShapeAnimationCount();

	/*! Adds a new shape animation
		\param in_Type Interpolation type to use
		\return Pointer to the newly added shape animation
	*/
	CSLShapeAnimation* AddShapeAnimation( EFCurveInterpolationType in_Type );

	/*! Removes all shape animations
		\retval SI_SUCCESS
	*/
	SI_Error ClearShapeAnimations();

	/*! Adds a preallocated shape animation to the shape animation list
		\param CSLShapeAnimation Pointer to the shape animation to add
		\return The newly added shape animation or null if it was already added
	*/
	CSLShapeAnimation* ConnectShapeAnimation(CSLShapeAnimation* in_pNewShapeAnimation);
	
	// XSI Shape animations functionality //////////////////////////////////////////////////
	
	/*! Removes a xsi shape animation
		\param in_nIndex Index of the xsi shape animation to remove
		\retval SI_SUCCESS The xsi shape animation was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error RemoveXSIShapeAnimation( SI_Int in_nIndex );

	/*! Removes a xsi shape animation
		\param in_pToRemove Pointer to the xsi shape animation to remove
		\retval SI_SUCCESS The xsi shape animation was removed
		\retval SI_ERR_BAD_ARGUMENT Could not find the xsi shape animation
	*/
	SI_Error RemoveXSIShapeAnimation( CSLXSIShapeAnimation *io_pToRemove );

	/*! Gets the list of all xsi shape animations
		\return Pointer to the list of pointer of all xsi shape animations
	*/
	CSLXSIShapeAnimation** GetXSIShapeAnimationList();

	/*! Get the total number of xsi shape animations
		\return The number of xsi shape animations
	*/
	SI_Int GetXSIShapeAnimationCount();

	/*! Adds a new xsi shape animation
		\param in_Type Interpolation type to use
		\return Pointer to the newly added xsi shape animation
	*/
	CSLXSIShapeAnimation* AddXSIShapeAnimation( EFCurveInterpolationType in_Type );

	/*! Removes all xsi shape animations
		\retval SI_SUCCESS
	*/
	SI_Error ClearXSIShapeAnimations();

	/*! Adds a preallocated xsi shape animation to the xsi shape animation list
		\param CSLXSIShapeAnimation Pointer to the xsi shape animation to add
		\return The newly added xsi shape animation or null if it was already added
	*/
	CSLXSIShapeAnimation* ConnectXSIShapeAnimation(CSLXSIShapeAnimation* in_pNewXSIShapeAnimation);

	// Animation functionality //////////////////////////////////////////////////
	
	/*! Removes an animation
		\param in_nIndex Index of the animation to remove
		\retval SI_SUCCESS The animation was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error RemoveAnimation( SI_Int in_nIndex );

	/*! Removes an animation
		\param in_pToRemove Pointer to the animation to remove
		\retval SI_SUCCESS The animation was removed
		\retval SI_ERR_BAD_ARGUMENT Animation was not found
	*/
	SI_Error RemoveAnimation( CSLActionFCurve *io_pToRemove );

	/*! Gets the list of all animations
		\return Pointer to the list of pointer of all animations
	*/
	CSLActionFCurve** GetAnimationList();

	/*! Get the total number of Animations
		\return The number of Animations
	*/
	SI_Int GetAnimationCount();

	/*! Adds a new animation with default interpolation ( constant interpolation )
		\return Pointer to the newly added Animation
	*/
	CSLActionFCurve* AddAnimation();

	/*! Adds a new animation
		\param in_InterpolationType FCurve interpolation type
		\return Pointer to the newly added Animation
	*/
	CSLActionFCurve* AddAnimation( EFCurveInterpolationType in_InterpolationType );

	/*! Removes all animations
		\retval SI_SUCCESS
	*/
	SI_Error ClearAnimations();

	/*! Adds a preallocated animation to the animation list
		\param CSLActionFCurve Pointer to the animation to add
		\return The newly added Animation or null if it was already added
	*/
	CSLActionFCurve* ConnectAnimation(CSLActionFCurve* in_pNewAnimation);
	
	// Static Values functionality //////////////////////////////////////////////////
	
	/*! Removes a static value
		\param in_nIndex Index of the static value to remove
		\retval SI_SUCCESS The static value was removed
		\retval SI_ERR_BAD_ARGUMENT Invalid index
	*/
	SI_Error RemoveStaticValue( SI_Int in_nIndex );

	/*! Removes a static value
		\param in_pToRemove Pointer to the static value to remove
		\retval SI_SUCCESS The static value was removed
		\retval SI_ERR_BAD_ARGUMENT static value was not found
	*/
	SI_Error RemoveStaticValue( CSLStaticValue *io_pToRemove );

	/*! Gets the list of all static values
		\return Pointer to the list of pointer of all static values
	*/
	CSLStaticValue** GetStaticValueList();

	/*! Get the total number of static values
		\return The number of static values
	*/
	SI_Int GetStaticValueCount();

	/*! Adds a new static value
		\return Pointer to the newly added static value
	*/
	CSLStaticValue* AddStaticValue();

	/*! Removes all static values
		\retval SI_SUCCESS
	*/
	SI_Error ClearStaticValues();

	/*! Adds a preallocated static value to the static value list
		\param CSLStaticValue Pointer to the static value to add
		\return The newly added static value or null if it was already added
	*/
	CSLStaticValue* ConnectStaticValue(CSLStaticValue* in_pNewStaticValue);

private:
    CSLFloatProxy m_StartTime;
    CSLFloatProxy m_EndTime;
    CSLEnumProxy<EActionType, SI_AT_NONE> m_ActionType;
    CSIBCArray<CSLActionFCurve*> m_Animations;
    CSIBCArray<CSLStaticValue*> m_StaticValues;
    CSIBCArray<CSLShapeAnimation*> m_ShapeAnimations;
    CSIBCArray<CSLXSIShapeAnimation*> m_XSIShapeAnimations;
};
#endif
