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

//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __CSIILJPGFileDriver_H__
#define __CSIILJPGFileDriver_H__

//***************************************************************************************
// Includes
//***************************************************************************************
#include <SIBCPixMap.h>


//***************************************************************************************
// Typedefs
//***************************************************************************************

//! JPG file loader class
class XSIEXPORT CSIILJPGFileDriver : public CSIBCPixMapDriver
{
public:
	/*! Constructor
	* \return an instance of CSIILJPGFileDriver
	*/
	CSIILJPGFileDriver();

	/*! Destructor
	*/
	virtual ~CSIILJPGFileDriver();
	
	/*! tells whether the extension passed in the string is supported by this driver
	* \param in_Filename file name to check 
	* \return SI_Bool
	* \retval TRUE if this file is supported
	* \retval FALSE if this file is not supported
	*/
	virtual SI_Bool Supported( CSIBCString &in_Filename );					
															
	/*! Loads a bitmap file into a CSIBCPixMap object
	* \param in_filename bitmap file name
	* \param in_PixMap CSIBCPixMap object to load the file into
	* \return SI_Error
	* \retval SI_SUCCESS loading has been succesful
	* \retval SI_ERR_ERROR_MSG loading has failed
	* \retval SI_FILE_NOT_FOUND the file has not been found
	*/
	virtual SI_Error Load( CSIBCString &in_filename, CSIBCPixMap &in_PixMap );		

	/*! returns the global JPG loader instance
	* \return CSIILJPGFileDriver*
	*/
	static CSIILJPGFileDriver *Driver();
private:

	SI_Error ReadPixels(_SI_FILE *filePtr, CSIBCPixMap &);
	
	// shielding from any lib dependencies
	void	*jpgInfo;
};




#endif // CSIILJPGFileDriver
