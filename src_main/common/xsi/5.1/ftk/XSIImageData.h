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

#ifndef _XSIIMAGEDATA_H
#define _XSIIMAGEDATA_H

#include "Template.h"
#include "XSIImage.h"

#ifndef _SL_INT_ARRAY_PROXY_EXPORT_
#define _SL_INT_ARRAY_PROXY_EXPORT_

#endif 

/** 
* Class CSLImageData
*
* Defines raw pixel information of an image.
* \note This template was introduced in dotXSI Version 3.6
*/
class XSIEXPORT CSLImageData
    : public CSLTemplate
{
public:

	/*! Proxy class that allows modifying a list of SI_Int like an array.
	*/
	typedef CSLArrayProxy<SI_Int, SI_Int, 1> CSLIntArray;

	//! Defines the various compression types
	enum ECompressionType
	{
		SI_UNCOMPRESSED = 0,	/*!< No compression */
	};

	/*! Constructor
		\param in_pScene Parent scene
		\param in_pModel Parent model
		\param in_pTemplate Refered template
	*/
	CSLImageData(CSLScene* in_pScene, CSLModel *in_pModel, CdotXSITemplate* in_pTemplate);
	
	//! Destructor
	virtual ~CSLImageData();

	/*! Commits Semantic Layer data
		\return  SI_Error Whether or not commit was a success
	*/
	SI_Error Synchronize();

	/*! Gets the type of this template
		\return ETemplateType The type of this template: XSI_IMAGE_DATA
	*/
	ETemplateType Type(){ return XSI_IMAGE_DATA; }

	/*! Sets the raw color information of the image
		\param in_Data Array containing the raw color information
	*/
	SI_Void SetImageData( CSIBCArray<SI_Int> &in_Data );

	/*! Gets the size of the image data color information
		\return SI_Int size of the image data color information
	*/
	SI_Int GetImageDataCount();

	/*! Get the color information array
		\return CSLImageDataArray Array of color information
	*/
	CSLIntArray* GetImageDataList();

	/*! Gets a pointer to the raw color information of the image
		\return SI_Int Pointer to the array containing the color information
	*/
	SI_Int* GetImageDataListPtr();

	/*! Gets the compression type of the image
		\return ECompressionType The type of compression
	*/
	ECompressionType GetCompressionType();

	/*! Sets the compression type of the image
		\param in_Type The type of compression
	*/
	SI_Void SetCompressionType( ECompressionType in_Type );

private:
    CSLEnumProxy<ECompressionType, SI_UNCOMPRESSED> m_CompressionType;
    CSLIntArray m_ImageData;

	SI_Void *m_pReserved;
};

#endif
