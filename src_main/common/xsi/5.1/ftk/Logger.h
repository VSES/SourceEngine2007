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
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <SIBCUtil.h>

///////////////////////////////////////////////////////////////////////////////
// Forward declaration
///////////////////////////////////////////////////////////////////////////////
class CSLLogger;
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! Callback prototype for SL logger
/**
	\param in_pLogger Pointer to the logger responsible for the message, not necessarely used
	\param in_pData Data to be passed to the callback, not necessarely used
	\return SI_Bool Whether or not the callback was successful
*/
typedef SI_Bool (*SLLoggerCallBack)( CSLLogger* in_pLogger, SI_Int in_nErrorType,  SI_Void* in_pData);
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// default CSLLogger callbacks
///////////////////////////////////////////////////////////////////////////////
XSIEXPORT SI_Bool StdOnInit( CSLLogger *in_pLogger, SI_Int, SI_Void* in_pData );			// writes "Start" to stdout
XSIEXPORT SI_Bool StdOnFatal( CSLLogger *in_pLogger, SI_Int, SI_Void* in_pData );		// writes "Fatal Error: " in_pData to stderr
XSIEXPORT SI_Bool StdOnError( CSLLogger *in_pLogger, SI_Int, SI_Void* in_pData );		// writes "Error: " in_pData to stderr
XSIEXPORT SI_Bool StdOnWarning( CSLLogger *in_pLogger, SI_Int, SI_Void* in_pData );		// writes "Warning: " in_pData to stderr
XSIEXPORT SI_Bool StdOnVerbose( CSLLogger *in_pLogger, SI_Int, SI_Void* in_pData );		// writes "Info: " in_pData to stdout
XSIEXPORT SI_Bool StdOnTerm( CSLLogger *in_pLogger, SI_Int, SI_Void* in_pData );			// writes "End" to stdout
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//! Simple message logging class 
class XSIEXPORT CSLLogger
{
public:

	//! Logs SL-related information
	/**
		\param OnInitFunc Callback used to initialize logging
		\param OnFatalFunc Callback that reports fatal errors
		\param OnErrorFunc Callback that reports errors
		\param OnWarningFunc Callback that reports warnings
		\param OnVerboseFunc Callback that reports general information
		\param OnTermFunc Callback used to oninitialize logging
		\param in_pUserData Pointer to user-supplied data (won't be freed by destructor)
	*/
	CSLLogger
	( 
		SLLoggerCallBack OnInitFunc = StdOnInit,			
		SLLoggerCallBack OnFatalFunc = StdOnFatal,
		SLLoggerCallBack OnErrorFunc = StdOnError,
		SLLoggerCallBack OnWarningFunc = StdOnWarning,
		SLLoggerCallBack OnVerboseFunc = StdOnVerbose,
		SLLoggerCallBack OnTermFunc = StdOnTerm,
		SI_Void* in_pUserData = 0
	);

	//! Defines the various types of messages that can be logged
	enum ELogType
	{
		SI_LOG_INIT = 0,	/*!< Logging ininitalization callback */
		SI_LOG_FATAL,		/*!< No hope for recovery */
		SI_LOG_ERROR,		/*!< User needs to correct something before going any further */
		SI_LOG_WARNING,	/*!< Some problems where found and have been corrected automatically */
		SI_LOG_VERBOSE,	/*!< Informative message */
		SI_LOG_TERM		/*!< Logging termination callback */
	};

	//! Log a message
	/**
		\note This function dispatches the message to the appropriate callback
		\param in_Type	Type of message being sent
		\param in_pData	Data that will be used by the callback
	*/
	SI_Bool Log( ELogType in_Type, SI_Void* in_pData );

	//! Get the internally stored user defined data
	SI_Void* GetData();

	//! Set the internally stored user defined data
	/**
		\param in_pData New user defined data to store
		\return SI_Void* Former user defined data
	*/
	SI_Void* SetData( SI_Void* in_pData );

	//! Replace a callback with a new one
	/**
		\param in_Callback Type of callback to replace
		\param in_NewCallback The new callback to use
		\return SLLoggerCallBack Pointer to the former callback
	*/
	SLLoggerCallBack SetCallBack( ELogType in_Callback, SLLoggerCallBack in_NewCallback );
	
	//! Get the current callback
	/**
		\param in_Callback Type of the callback to retrieve
		\return SLLoggerCallBack The callback
	*/
	SLLoggerCallBack GetCallBack( ELogType in_Callback );


private:
	SI_Void* m_pData;	// pointer to user-defined data

	SLLoggerCallBack m_pCallBacks[SI_LOG_TERM+1];
};
///////////////////////////////////////////////////////////////////////////////

#endif //__LOGGER_H__
