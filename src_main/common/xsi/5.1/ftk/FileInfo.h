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

#ifndef _FILEINFO_H
#define _FILEINFO_H

#include "Template.h"

//! Used to set some information about the user and software that generated the scene.

class XSIEXPORT CSLFileInfo
    : public CSLTemplate
{
public:
	/*! Constructor
	* \param in_pScene Parent scene
	* \param in_pModel Parent model
	* \param in_pTemplate Parent Template
	* \return an instance of CSLFileInfo
	*/
    CSLFileInfo(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	/*! Destructor
	*/
    virtual ~CSLFileInfo();

	/*! Returns the originating software	
	* \return Originating software or null if not available
	*/ 
    char* GetOriginator() ;

	/*! Sets the originating software
	* \param in_szOriginator Originating software
	* \return void
	*/ 
	void SetOriginator( char* in_szOriginator );

	/*! Returns the name of the project
	* \return Name of the project or null if not available
	*/ 
    char* GetProjectName() ;

	/*! Sets the name of the project
	* \param in_szProjectName Name of the project
	* \return void
	*/ 
    void SetProjectName( char* in_szProjectName );

	/*! Returns the user name.
	* \return Name of the user or null if not available
	*/ 
    char* GetUsername() ;

	/*! Sets the user name.
	* \param in_szUserName Name of the user
	* \return void
	*/ 
    void SetUsername( char* in_szUserName );

	/*! Returns the save date time.
	* \return Save date time
	*/ 
    char* GetSaveDateTime() ;

	/*! Sets the save date time.
	* \param in_szSaveDateTime Save date time
	* \return void
	*/ 
    void SetSaveDateTime( char* in_szSaveDateTime );

	/*! Returns the type of this template (FILE_INFO)
	* \return ETemplateType The type of this template (FILE_INFO)
	*/
	ETemplateType Type();

	/*! Update template with current information
	* \return SI_Error Specify if we could update the information or not
	*/
	SI_Error Synchronize();

private:
    CSLStringProxy m_ProjectName;
    CSLStringProxy m_UserName;
    CSLStringProxy m_Originator;
    CSLStringProxy m_SaveDateTime;

	// reserved for future use
	SI_Void *m_pReserved;
};

#endif

