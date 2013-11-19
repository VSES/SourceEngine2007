//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2005 Avid Technology, Inc. . All rights reserved.
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

#ifndef _XSIVERTEXLIST_H
#define _XSIVERTEXLIST_H

#include "XSISubComponentList.h"


//!Describes a list of vertex for dotXSI 5.0+
/**
* The CSLXSIVertexList class describes a list of vertecies.
* The vertecies in this list can have N number of attibutes like postion, etc.
*
* To find out, you may use the parent class (CSLXSISubComponentList) functionality
* to get the generic attributes index.
*
*/
class XSIEXPORT CSLXSIVertexList
	: public CSLXSISubComponentList
{
public:
	/*! Constructor	
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Refered template
	* \return an instance of CSLXSIVertexList
	*/
	CSLXSIVertexList(CSLScene *in_pScene, CSLModel *in_pModel, CdotXSITemplate *in_pTemplate);

	/*! Destructor
	*/
	virtual ~CSLXSIVertexList();

	/*! Returns the type of this template (XSI_VERTEX_LIST)
		\return CSLTemplate::ETemplateType The type of this template (XSI_VERTEX_LIST)
	*/
	virtual ETemplateType			Type() { return XSI_VERTEX_LIST; }

	/*! Commit the current template information
		\return SI_Error Whether or not the commit was successful
	*/
	SI_Error Synchronize();

private:

	void *m_pReserved;	// reserved for future extension
};

#endif
