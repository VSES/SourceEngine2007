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

#ifndef _XSIVIEWERDISPLAYMANAGER_H_
#define _XSIVIEWERDISPLAYMANAGER_H_


#include <SIDisplayDriver.h>


/**
* Defines
*/
#define SI_DISPLAY_DRIVER_DIR	".\\drivers\\"

/**
* Display Driver information struct
*/
struct CSIDriverInfo 
{
	SI_Char		m_pchName[SI_MAX_DRV_NAME_LENGTH];
	SI_Char		m_pchDllName[SI_MAX_DRV_NAME_LENGTH];
	SI_Int		m_nApiType;
};

/** 
* Display Driver Manager utility
* \sa XSIViewerDisplayManager 
*/
class XSIEXPORT XSIViewerDisplayManager
{
public:
	XSIViewerDisplayManager();
	~XSIViewerDisplayManager();

	//! Looks for display drivers and fills the m_DriverInfoArray
	/*!
	\param	SI_Char*	The application root directory/path ex.: "c:\\XSIViewer\\"
	\return SI_Error	success or failure code
	*/
	virtual	SI_Error	ScanDrivers ( SI_Char* in_pchPath );


	CSIDriverInfo* GetDriverInfoByName( SI_Char* in_pchDriverName );

	CSIBCArray <CSIDriverInfo> m_DriverInfoArray;
};

#endif // _XSIVIEWERDISPLAYMANAGER_H_
