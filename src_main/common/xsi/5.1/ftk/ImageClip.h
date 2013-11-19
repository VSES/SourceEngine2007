/******************************************************************************\
*
* File:          ImageClip.h
* Creation date: January 15, 2002 17:31
* Author:        ClassBuilder
*                XXXX
* Purpose:       Declaration of class 'ImageClip'
*
* Modifications: @INSERT_MODIFICATIONS(* )
* January 23, 2002 10:59 Frederic O'Reilly
*     Added method 'StartTime'
*     Added method 'RepeatType'
*     Added method 'RemoveImage'
*     Added method 'Reference'
*     Added method 'Rate'
*     Added method 'NbImages'
*     Added method 'Images'
*     Added method 'EndTime'
*     Added method 'AddImage'
*     Added method 'CSLImageClip'
*     Added member 'm_pImages'
*     Added member 'm_pRepeatType'
*     Added member 'm_pRate'
*     Added member 'm_pEndTime'
*     Added member 'm_pStartTime'
*     Added member 'm_pReference'
*     Updated inheritance 'CSLTemplate'
*
* Copyright 2002, XXXXX
* All rights are reserved. Reproduction in whole or part is prohibited
* without the written consent of the copyright owner.
*
\******************************************************************************/
#ifndef _IMAGECLIP_H
#define _IMAGECLIP_H

#include "Template.h"

// Forward declaration
class CSLTexture2D;

//! Class not implemented
class CSLImageClip
    : public CSLTemplate
{

//@START_USER2
//@END_USER2

// Members
private:
    CSLTexture2D* m_pReference;
    CSLFloatProxy* m_pStartTime;
    CSLFloatProxy* m_pEndTime;
    CSLFloatProxy* m_pRate;
    CSLIntProxy* m_pRepeatType;
    CSIBCArray<CSLStringProxy *> m_pImages;

protected:

public:

// Methods
private:

protected:

public:
    CSLImageClip(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
    virtual ~CSLImageClip();
    CSLStringProxy* AddImage();
    CSLFloatProxy* EndTime() const;
    CSLStringProxy** Images() const;
    SI_Int* NbImages() const;
    CSLFloatProxy* Rate() const;
    CSLTexture2D* Reference() const;
    SI_Error RemoveImage();
    CSLIntProxy* RepeatType() const;
    CSLFloatProxy* StartTime() const;
};

#endif


#ifdef CB_INLINES
#ifndef _IMAGECLIP_H_INLINES
#define _IMAGECLIP_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
