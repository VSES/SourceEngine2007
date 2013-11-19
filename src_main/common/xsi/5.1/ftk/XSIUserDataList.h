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

#ifndef _XSIUSERDATALIST_H
#define _XSIUSERDATALIST_H

#include "Template.h"

class CSLUserData;
class CSLCluster;

/** 
* Class CSLUserDataList
*
* Defines a collection of XSI user-defined data
* \note This template was introduced in dotXSI Version 3.6
* \sa CSLUserData
* \sa CSLCluster
*/

class XSIEXPORT CSLUserDataList
    : public CSLTemplate
{
public:

	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
	CSLUserDataList(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
	virtual ~CSLUserDataList();

	/*! Gets the type of this template
		\return ETemplateType The type of this template: XSI_USER_DATA_LIST
	*/
	ETemplateType Type(){ return XSI_USER_DATA_LIST; }

	/*! Commits semantic layer data
		\return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

	///////////////////////////////////////////////////////////////////////////
	// User data functionality ///////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Adds a user data to the user data list
		\return CSLUserData Pointer to the newly added user data
	*/
	CSLUserData* AddUserData();

	/*! Removes a user data from the user data list
		\param in_nIndex Index of the user data to remove
		\return SI_Error Whether or not the user data could be removed
	*/
	SI_Error RemoveUserData( SI_Int in_nIndex );

	/*! Removes a user data from the user data list
		\param in_pToRemove Pointer to the user data to remove
		\return SI_Error Whether or not the user data could be removed
	*/
	SI_Error RemoveUserData( CSLUserData* in_pToRemove );

	/*! Removes all connections points from the user data list
		\return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearUserDatas();

	/*! Adds a exisiting user data to the user data list
		\param in_pToConnect Pointer to the user data to add
		\return The added user data or null if it already exists in the list
	*/
	CSLUserData* ConnectUserData( CSLUserData* in_pToConnect );

	/*! Gets a pointer to the internally-stored UserData pointer list
		\return CSLUserData Pointer to the list
	*/
	CSLUserData** GetUserDataList();

	/*! Gets the total number of user datas
		\return SI_Int Number of user datas
	*/
	SI_Int GetUserDataCount();

	/*! Searches for a given UserData
		\param in_szUserDataName Name of the user data to look for
		\return CSLUserData Pointer to the user data or null if it canot be found
	*/
	CSLUserData* FindUserData( SI_Char* in_szUserDataName );

private:
    CSIBCArray<CSLUserData*> m_UserDatas;

	SI_Void *m_pReserved;
};

#endif
