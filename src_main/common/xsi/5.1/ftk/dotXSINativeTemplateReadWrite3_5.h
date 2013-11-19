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


#ifndef	DOTXSIRW_35_DEFINED
#define	DOTXSIRW_35_DEFINED

#include "XSIParser.h"
#include "dotXSIDefines.h"

/*! \addtogroup rwfunc_35 Non generic read/write functions for dotXSI 3.5
*  @{
*/

/*! reading callback for SI_Shape in dotXSI 3.5
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadShape35(			CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for SI_Shape in dotXSI 3.5
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WriteShape35(			CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

/*! reading callback for SI_PolygonList in dotXSI 3.5
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadPolygonList35(	CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for SI_PolygonList in dotXSI 3.5
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WritePolygonList35(	CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

/*! reading callback for SI_TriangleList in dotXSI 3.5
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadTriangleList35(	CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for SI_TriangleList in dotXSI 3.5
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WriteTriangleList35(	CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

/*! reading callback for SI_TriStrip in dotXSI 3.5
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadTriStrip35(		CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for SI_TriStrip in dotXSI 3.5
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WriteTriStrip35(		CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

/*! reading callback for XSI_Material 
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadXSI_Material(		CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for XSI_Material
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WriteXSI_Material(	CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

/*! reading callback for XSI_Shader
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadXSI_Shader(		CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for XSI_Shader
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WriteXSI_Shader(		CXSIParser			*i_pParser, 
										CdotXSITemplate		*i_pCurrentTemplate, 
										CdotXSITemplate		*i_pNewTemplate, 
										SI_Int				i_nLevel );

/*! reading callback for XSI_ShaderInstanceData
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_ReadXSI_ShaderInstanceData(	CXSIParser			*i_pParser, 
												CdotXSITemplate		*i_pCurrentTemplate, 
												CdotXSITemplate		*i_pNewTemplate );

/*! writing callback for XSI_ShaderInstanceData
* \param i_pParser a pointer to the parser that calls this function
* \param i_pCurrentTemplate a pointer to the current template
* \param i_pNewTemplate a pointer to the new template
* \param i_nLevel the level of recursion (used to write tabs in ascii format)
* \return SI_Error 
* \retval SI_SUCESS the read has been successful
* \retval SI_ERR_ERROR_MSG the read has failed
*/
SI_Error dotXSICB_WriteXSI_ShaderInstanceData(	CXSIParser			*i_pParser, 
												CdotXSITemplate		*i_pCurrentTemplate, 
												CdotXSITemplate		*i_pNewTemplate, 
												SI_Int				i_nLevel );

/*! @} */
#endif //DOTXSIRW_35_DEFINED
