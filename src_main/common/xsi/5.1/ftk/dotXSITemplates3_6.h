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

#ifndef	DOTXSI_36_DEFINED
#define	DOTXSI_36_DEFINED
#include "dotXSITemplate.h"

#include "dotXSITemplates3_5.h"

extern	TemplateCreation	RegistrationTable36[];

/*! \addtogroup regfunc_36 Registration functions for dotXSI 3.6
*  @{
*/

/*! XSI_ImageLibrary template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_ImageLibrary template
*/
CdotXSITemplate*	Register_XSI_ImageLibrary();

/*! XSI_NurbsProjection template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_NurbsProjection template
*/
CdotXSITemplate*	Register_XSI_NurbsProjection();

/*! XSI_NurbsTrim template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_NurbsTrim template
*/
CdotXSITemplate*	Register_XSI_NurbsTrim();

/*! XSI_Image template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_Image template
*/
CdotXSITemplate*	Register_XSI_Image();

/*! XSI_ImageData template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_ImageData template
*/
CdotXSITemplate*	Register_XSI_ImageData();

/*! XSI_ImageFX template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_ImageFX template
*/
CdotXSITemplate*	Register_XSI_ImageFX();

/*! SI_LightInfo template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_LightInfo template
*/
CdotXSITemplate*	Register_SI_LightInfo();

/*! XSI_MaterialInfo template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_MaterialInfo template
*/
CdotXSITemplate*	Register_XSI_MaterialInfo();

/*! XSI_CustomParamInfo template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_CustomParamInfo template
*/
CdotXSITemplate*	Register_XSI_CustomParamInfo();

/*! XSI_UserDataList template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_UserDataList template
*/
CdotXSITemplate*	Register_XSI_UserDataList();

/*! XSI_UserData template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_UserData template
*/
CdotXSITemplate*	Register_XSI_UserData();

/*! XSI_FxTree template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_FxTree template
*/
CdotXSITemplate*	Register_XSI_FxTree();

/*! XSI_FxOperator template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_FxOperator template
*/
CdotXSITemplate*	Register_XSI_FxOperator();

/*! XSI_ClusterInfo template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_ClusterInfo template
*/
CdotXSITemplate*	Register_XSI_ClusterInfo();

/*! @} */

#endif
