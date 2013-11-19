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
/******************************************************************************\
*
* File:          SceneInfo.h
* Creation date: January 28, 2002 18:16
* Author:        Frederic O'Reilly
* Purpose:       Declaration of class 'SceneInfo'
*
* Copyright 2002, Softimage
* All rights are reserved. Reproduction in whole or part is prohibited
* without the written consent of the copyright owner.
*
\******************************************************************************/
#ifndef _SCENEINFO_H
#define _SCENEINFO_H

#include "Template.h"

/** 
* CSLSceneInfo Class
* 
* Describes the SceneInformation template.
*/
class XSIEXPORT CSLSceneInfo
	: public CSLTemplate
{
// Methods
public:

	//! Enumeration specifying the timing type
	typedef enum ETimingType
	{
		SI_SECONDS,	//*< Timing is in seconds */
		SI_FRAMES	//*< Timing is in frames */
	};
	
	//! Constructor
	/**
		\param in_pScene Pointer to the scene
		\param in_pModel Pointer to the model
		\param in_pTemplate Pointer to the parent template
		\param in_DefaultETimingType Timing type to use if an invalid type is loaded
	*/
    CSLSceneInfo
	( 
		CSLScene* in_pScene,
		CSLModel* in_pModel,
		CdotXSITemplate *in_pTemplate,
		ETimingType in_DefaultETimingType = SI_FRAMES
	);

	//! Destructor
    virtual ~CSLSceneInfo();

	//! Set end time of the scene (Time type specified by ETimingType)
	/*!
		\param in_nEnd New end frame to use
	*/
	void SetEnd( SI_Float in_nEnd );
	
	//! Get end time of the scene (Time type specified by ETimingType)
	/*!
		\return SI_Int End frame
	*/
	SI_Float GetEnd();

	//! Set frames per second
	/*!
		\param in_fFrameRate New frame rate to use
	*/
    void SetFrameRate( SI_Float in_fFrameRate );

	//! Get frames per second
	/*!
		\return SI_Float Current frame rate
	*/
	SI_Float GetFrameRate();

	//! Set Start time of the scene (Time type specified by ETimingType)
	/*!
		\param in_nStart New start frame to use
	*/
	void SetStart( SI_Float in_nStart );

	//! Get Start time of the scene (Time type specified by ETimingType)
	/*!
		\return The current start frame
	*/
	SI_Float GetStart();

	//! Set the timing type
	/*!
		\param in_Type New timing type to use
	*/
    void SetTimingType( ETimingType in_Type ) { m_TimingType = in_Type; };

	//! Get the timing type
	/*!
		\return Current timing type
	*/
    ETimingType GetTimingType();

    virtual CSLTemplate::ETemplateType Type();
    virtual CSLAnimatableType* ParameterFromName(SI_Char *in_szName);

// Members
private:
    CSLStrEnumProxy<ETimingType, SI_FRAMES> m_TimingType;
    CSLFloatProxy m_nStart;
    CSLFloatProxy m_nEnd;
    CSLFloatProxy m_fFrameRate;

	// This member is here to allow future improvement of the class.
	SI_Void *m_pReserved;
};

#endif

