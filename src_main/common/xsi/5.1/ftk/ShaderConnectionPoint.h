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
* File:          ShaderConenctionPoint.h
* Creation date: January 23, 2002 10:59
* Author:        ClassBuilder
*                XXXX
* Purpose:       Declaration of class 'CSLShaderConenctionPoint'
*
* Modifications: @INSERT_MODIFICATIONS(* )
* January 23, 2002 10:59 Frederic O'Reilly
*     Added method 'DestructorInclude'
*     Added method 'ConstructorInclude'
*     Added method '~CSLShaderConenctionPoint'
*     Added inheritance 'CSLConnectionPoint'
*     Added member 'm_pType'
*
* Copyright 2002, XXXXX
* All rights are reserved. Reproduction in whole or part is prohibited
* without the written consent of the copyright owner.
*
\******************************************************************************/
#ifndef _SHADERCONENCTIONPOINT_H
#define _SHADERCONENCTIONPOINT_H

#include "ConnectionPoint.h"

/** 
* CSLShaderConnectionPoint Class
* 
* Class representing shader connection points
*/
class XSIEXPORT CSLShaderConnectionPoint
    : public CSLConnectionPoint
{
public:
	/*! Constructor
		\param in_pTemplate Parent Template
		\parm in_nValueIndex Index
	*/	
	CSLShaderConnectionPoint
	( 
		CdotXSITemplate *in_pTemplate, 
		SI_Int in_nValueIndex 
	);

	//! Destructor
    virtual ~CSLShaderConnectionPoint();

	//! Type of connection point
	enum EConnectionType
	{
		SI_SHADER,	/*!< Shader connection */
		SI_IMAGE,	/*!< Image connection */
		SI_NONE,	/*!< No connection */
	};

	/*! Gets the type of the connection point
		\return EType The type of connection
	*/
	EConnectionType GetConnectionType();

	/*! Sets the name of the source image
		\param in_szName Name of the source image
		\return SI_Error This function fails if the connection type is different than SI_IMAGE
	*/
	SI_Error SetImage( SI_Char* in_szName );

	/*! Gets the name of the source image
		\return SI_Char Name of the source image or null if there's no source image
	*/
	SI_Char* GetImage();

	virtual SI_Error Synchronize();

private:
	CSLStringProxy m_ConnectionType;
	CSIBCString m_ImageName;
};

#endif
