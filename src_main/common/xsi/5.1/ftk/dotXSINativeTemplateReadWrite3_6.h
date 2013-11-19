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

#ifndef	DOTXSIRW_36_DEFINED
#define	DOTXSIRW_36_DEFINED

#include "XSIParser.h"
#include "dotXSIDefines.h"
#include "dotXSINativeTemplateReadWrite3_5.h"


/*! reading callback for XSI_UserData
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadUserData(			CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for XSI_UserData
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WriteUserData(		CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

/*! reading callback for XSI_ImageData
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadImageData(		CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for XSI_ImageData
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WriteImageData(		CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

SI_Error dotXSICB_ReadCustomParamInfo(	CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

SI_Error dotXSICB_WriteCustomParamInfo(	CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

#endif //DOTXSIRW_36_DEFINED
