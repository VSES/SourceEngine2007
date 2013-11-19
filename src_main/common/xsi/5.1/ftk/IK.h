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

#ifndef _IK_H
#define _IK_H

#include "Primitive.h"

// Forward declaration
class CSLIKRoot;


//! This is the base class for all IK classes.
class XSIEXPORT CSLIK
    : public CSLPrimitive
{
public:

	/*! Destructor
	*/
    virtual ~CSLIK();

	/*!	Return the root associated with the IK component.
	* \return	CSLIKRoot*	Pointer to an IK root.
	*/
    CSLModel* GetRoot();

	/*! Connects a root model to this IK chain
	* \param in_pNewRoot the root to connect
	* \return CSLModel* the root
	*/
	CSLModel* ConnectRoot(CSLModel* in_pNewRoot);

protected:
    CSLIK(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate, CSLModel* in_pRoot);

private:
    CSLModel* m_pRoot;

	void *m_pReserved;	// reserved for future extension
};

#endif
