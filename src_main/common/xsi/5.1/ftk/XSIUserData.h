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

#ifndef _XSIUSERDATA_H
#define _XSIUSERDATA_H

#include "Template.h"

#ifndef _SL_INT_ARRAY_PROXY_EXPORT_
#define _SL_INT_ARRAY_PROXY_EXPORT_

//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SI_Int, SI_Int, 1>;

#endif // _SL_INT_ARRAY_PROXY_EXPORT_

#ifndef _SL_UBYTE_ARRAY_PROXY_EXPORT_
#define _SL_UBYTE_ARRAY_PROXY_EXPORT_

//XSITEMPLATEEXP template class XSIEXPORT CSLArrayProxy<SI_UByte, SI_UByte, 1>;

#endif // _SL_UBYTE_ARRAY_PROXY_EXPORT_

class CSLUserDataList;

/** 
* Class CSLUserData
*
* Defines XSI user-defined data
* \note This template was introduced in dotXSI Version 3.6
* \sa CSLUserDataList
*/
class XSIEXPORT CSLUserData
    : public CSLTemplate
{
public:

	//! Unsigned Byte array proxy class that works like an array of SI_UByte
	typedef CSLArrayProxy<SI_UByte, SI_UByte, 1> CSLUByteArray;

	//! Int array proxy class that works like an array of SI_Int
	typedef CSLArrayProxy<SI_Int, SI_Int, 1> CSLIntArray;

	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
	CSLUserData(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
	virtual ~CSLUserData();

	/*! Gets the type of this template
		\return ETemplateType The type of this template: XSI_USER_DATA
	*/
	ETemplateType Type(){ return XSI_USER_DATA; }

	/*! Commits semantic layer data
		\return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

	///////////////////////////////////////////////////////////////////////////
	// User data //////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	/*! Gets pointer the user data array
		\return SI_Int Pointer to the array containing the user data
	*/
	SI_UByte* GetUserDataListPtr();

	/*! Gets the user data array
		\return CSLUByteArray Array of user data
	*/
	CSLUByteArray* GetUserDataList();

	/*! Gets the size of the user data array
		\return SI_Int size of the user data
	*/
	SI_Int GetUserDataCount();

	///////////////////////////////////////////////////////////////////////////
	// Sub element access /////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Gets a pointer to the sub element array
		\return SI_Int Pointer to the array containing indices for all affected vertices/edges/polygons
	*/
	SI_Int* GetSubElementListPtr();

	/*! Gets the sub element array
		\return CSLIntArray Pointer to the array containing indices for all affected vertices/edges/polygons
	*/
	CSLIntArray* GetSubElementList();

	/*! Gets the number of sub elements
		\return SI_Int Number of sub elements indices
	*/
	SI_Int GetSubElementCount();
	
private:
	CSLIntProxy m_DataSize;
    CSLUByteArray m_UserData;
    CSLIntArray m_SubElements;

	SI_Void* m_pReserved;
};

/** 
* Class CSLUserDataBlob
*
* Defines XSI user-defined data
* \note This template was introduced in dotXSI Version 5.0
* \sa CSLUserDataBlob
*/
class XSIEXPORT CSLUserDataBlob
    : public CSLTemplate
{
public:

	//! Unsigned Byte array proxy class that works like an array of SI_UByte
	typedef CSLArrayProxy<SI_UByte, SI_UByte, 1> CSLUByteArray;

	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
	CSLUserDataBlob(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
	virtual ~CSLUserDataBlob();

	/*! Gets the type of this template
		\return ETemplateType The type of this template: XSI_USER_DATA_BLOB
	*/
	ETemplateType Type(){ return XSI_USER_DATA_BLOB; }

	/*! Commits semantic layer data
		\return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

	///////////////////////////////////////////////////////////////////////////
	// User data //////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	
	/*! Gets pointer the user data array
		\return SI_UByte Pointer to the array containing the user data
	*/
	SI_UByte* GetUserDataPtr();

	/*! Gets the user data array
		\return CSLUByteArray Array of user data
	*/
	CSLUByteArray* GetUserData();

	/*! Gets the size of the user data array
		\return SI_Int size of the user data
	*/
	SI_Int GetUserDataSize();

	
private:
	CSLIntProxy		m_DataSize;
    CSLUByteArray	m_UserData;

	SI_Void* m_pReserved;
};


#endif
