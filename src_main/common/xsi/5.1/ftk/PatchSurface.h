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

#ifndef _PATCHSURFACE_H
#define _PATCHSURFACE_H

#include "Geometry.h"

//! Implements a patch surface
class CSLPatchSurface
    : public CSLGeometry
{

//@START_USER2
//@END_USER2

// Members
private:

protected:

public:

// Methods
private:

protected:

public:

	/*! Constructor
	* \param in_pScene	Pointer to the scene containing the Geometry
	* \param in_pModel	Pointer to the model containing the Geometry
	* \param in_pTemplate	Pointer to the CdotXSITemplate containing the Geometry
	* \return an instance of CSLPatchSurface
	*/
	CSLPatchSurface(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate) :
	  CSLGeometry(in_pScene, in_pModel, in_pTemplate) {};

	/*! Destructor
	*/
    virtual ~CSLPatchSurface();
};

#endif


#ifdef CB_INLINES
#ifndef _PATCHSURFACE_H_INLINES
#define _PATCHSURFACE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
