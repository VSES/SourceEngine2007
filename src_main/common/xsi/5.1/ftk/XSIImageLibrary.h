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

#ifndef _XSIIMAGELIBRARY_H
#define _XSIIMAGELIBRARY_H

#include "Template.h"

class CSLImage;

/** 
* Class CSLImageLibrary
*
* Defines a collection of images
* \note This template was introduced in dotXSI Version 3.6
*/
class XSIEXPORT CSLImageLibrary
    : public CSLTemplate
{
public:
	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
	CSLImageLibrary(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);

	//! Destructor
	virtual ~CSLImageLibrary();

	/*! Gets the type of this template
		\return ETemplateType The type of this template: XSI_IMAGE_LIBRARY
	*/
	ETemplateType Type(){ return XSI_IMAGE_LIBRARY; }

	/*! Commits semantic layer data
		\return SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

    virtual CSLAnimatableType*		ParameterFromName(SI_Char *in_szName);

	///////////////////////////////////////////////////////////////////////////
	// Image Functionality /////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	/*! Adds an Image to the Image list
		\return CSLImage Pointer to the newly added Image
	*/
	CSLImage* AddImage();

	/*! Removes an Image from the Image list
		\param in_nIndex Index of the Image to remove
		\return SI_Error Whether or not the Image could be removed
	*/
	SI_Error RemoveImage( SI_Int in_nIndex );

	/*! Removes an Image from the Image list
		\param in_pToRemove Pointer to the Image to remove
		\return SI_Error Whether or not the Image could be removed
	*/
	SI_Error RemoveImage( CSLImage* in_pToRemove );

	/*! Removes all connections points from the Image list
		\return SI_Error Whether or not the list could be cleared
	*/
	SI_Error ClearImages();

	/*! Adds an exisiting Image to the Image list
		\param in_pToConnect Pointer to the Image to add
		\return The added Image or null if it already exists in the list
	*/
	CSLImage* ConnectImage( CSLImage* in_pToConnect );

	/*! Gets a pointer to the internally-stored Image pointer list
		\return CSLImage Pointer to the list
	*/
	CSLImage** GetImageList();

	/*! Gets the total number of Images 
		\return SI_Int Number of Images
	*/
	SI_Int GetImageCount();

	/*! Searches for a given Image
		\param in_szImageName Name of the Image to look for
		\return CSLImage Pointer to the Image or null if it cannot be found
	*/
	CSLImage* FindImage( SI_Char* in_szImageName );

private:
    CSIBCArray<CSLImage*> m_Images;

	SI_Void *m_pReserved;
};

#endif
