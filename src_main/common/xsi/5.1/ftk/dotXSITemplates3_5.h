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

#ifndef	DOTXSI_35_DEFINED
#define	DOTXSI_35_DEFINED
#include "dotXSITemplate.h"

#include "dotXSITemplates3_0.h"

extern	TemplateCreation	RegistrationTable35[];


/*! \addtogroup regfunc_35 Registration functions for dotXSI 3.5
*  @{
*/


/*! SI_Shape template registration function for dotXSI 3.5
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_Shape template 
	when using dotXSI 3.5
*/
CdotXSITemplate*	Register_SI_Shape35();

/*! SI_PolygonList template registration function for dotXSI 3.5
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_PolygonList template
	when using dotXSI 3.5
*/
CdotXSITemplate*	Register_SI_PolygonList35();

/*! SI_TriangleList template registration function for dotXSI 3.5
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_TriangleList template
	when using dotXSI 3.5
*/
CdotXSITemplate*	Register_SI_TriangleList35();

/*! SI_TriStrip template registration function for dotXSI 3.5
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the SI_TriStrip template
	when using dotXSI 3.5
*/
CdotXSITemplate*	Register_SI_TriStrip35();

/*! XSI_Material template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_Material template
*/
CdotXSITemplate*	Register_XSI_Material();

/*! XSI_Shader template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_Shader template
*/
CdotXSITemplate*	Register_XSI_Shader();

/*! XSI_ShaderInstanceData template registration function
* \return CdotXSITemplate* an instance of CdotXSITemplate that supports the XSI_ShaderInstanceData template
*/
CdotXSITemplate*	Register_XSI_ShaderInstanceData();

/*! @} */
#endif
