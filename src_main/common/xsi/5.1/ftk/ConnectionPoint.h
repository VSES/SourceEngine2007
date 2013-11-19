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

#ifndef _CONNECTIONPOINT_H
#define _CONNECTIONPOINT_H

#include "Template.h"
#include "Scene.h"
#include <dotXSIParam.h>	// CdotXSIParam

// Forward declaration
class CSLXSIShader;

//! Class representing connection points
class XSIEXPORT CSLConnectionPoint
{
	friend class CSLScene;
public:
	/*! Constructor
	* \param in_pTemplate Parent Template
	* \param in_nValueIndex Index
	* \return an instance of CSLConnectionPoint
	*/
	CSLConnectionPoint
	( 
		CdotXSITemplate *in_pTemplate, 
		SI_Int in_nValueIndex 
	);
	
	/*! Destructor
	*/
    virtual ~CSLConnectionPoint();

	/*! Set the name of the connection point
	* \param in_szName Name of the connection point
	* \return SI_Void
	*/
	SI_Void SetName( SI_Char *in_szName );

	/*! Gets the name of the connection point
	* \return SI_Char Name of the connection point
	*/
	SI_Char* GetName();

	/*! Set the shader of the connection point
	* \param in_pShader Pointer to the shader to connect
	* \return SI_Void
	*/
	SI_Void SetShader( CSLXSIShader *in_pShader );

	/*! Get the shader of the connection point
	* \return Pointer to the shader
	*/
	CSLXSIShader* GetShader();

	/*! Synchronizes this connection point instance with its IO layer representation
	* \return SI_Error
	* \retval SI_SUCCESS this function always returns SI_SUCCESS
	*/
	virtual SI_Error Synchronize();

protected:
	CdotXSIParam *m_pParam;

	CSLXSIShader *m_pShader;
};

#endif
