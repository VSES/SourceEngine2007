//*****************************************************************************
/*!
   \file xsi_status.h
   \brief CStatus class declaration.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSISTATUS_H__
#define __XSISTATUS_H__

#include "sicppsdk.h"
#include "xsi_string.h"


namespace XSI {


//*****************************************************************************
/*! \class CStatus xsi_status.h
	\brief Encapsulates status data types for error handling purpose. The class also
	exposes methods to query and set the error code.

	The error codes use the same values as the standard HRESULT error codes on Windows.
 */
//*****************************************************************************

class SICPPSDKDECL CStatus
{
public:

	//! Error code enumerator.
	enum Code
	{
		Undefined = 0xFFFFFFFF,			/*!< Undefined */
		OK = 0,							/*!< OK */
		False = 1,						/*!< False.  Operator is considered to have Succeeded, but with no result.*/
		Fail = 0x80004005L,				/*!< Fail */
		InvalidArgument = 0x80070057L,	/*!< InvalidArgument */
		OutOfMemory = 0x8007000EL,		/*!< OutOfMemory */
		AccessDenied = 0x80070005L,		/*!< AccessDenied */
		Unexpected = 0x8000FFFFL,		/*!< Unexpected */
		NotImpl = 0x80004001L,			/*!< NotImpl */
		MemberNotFound = 0x80020003L	/*!< MemberNotFound */
	} ;

	/*! Constructs a status object from a given error code.
	\param in_code Error code
	*/
	CStatus(Code in_code=OK);

	CStatus(LONG in_code);

	/*! Returns the underlying error code.
	\return Error code
	*/
	Code GetCode() const;

	/*! Returns true if the status is either OK or False
	\return true if OK or False, false otherwise.
	*/
	bool Succeeded() const;

	/*! Sets the error code type
	\param in_code Error code
	*/
	void PutCode(Code in_code);

	/*! Gets the status code description.
	\return Error description string
	*/
	CString GetDescription() const;

	/*! Sets the CStatus object with a given error code.
	\param in_code Error code
	\return CStatus object.
	*/
	CStatus & operator = ( Code in_code );

	/*! The equality operator that takes another CStatus object.
	\param rhs status object
	\return bool
	*/
	bool  operator== ( const CStatus & rhs ) const;

	/*! The equality operator that takes a CStatus::Code.
	\param rhs status code
	\return bool
	*/
	bool  operator== ( Code rhs ) const;

	/*! The inequality operator that takes a CStatus object.
	\param rhs status object
	\return bool
	*/
	bool  operator!= ( const CStatus & rhs ) const;

	/*! The inequality operator that takes a CStatus::Code.
	\param rhs status code
	\return bool
	*/
	bool  operator!= ( Code rhs ) const;

	/*! Assert the current status code is OK or False.
	\param in_strText Optional message logs if the function doesn't assert.
	\return bool
	*/
	void AssertSucceeded( const CString& in_strText = CString()) const;

private:
	Code m_code;
};

/*! The equality operator that takes a Code
 * on the left side and a CStatus object on the right side.
\param lhs status code
\param rhs status object
\return bool
*/
SICPPSDKDECL bool  operator== ( CStatus::Code lhs, const CStatus & rhs);

/*! The inequality operator that takes a Code
 * on the left side and a CStatus object on the right side.
\param lhs status code
\param rhs status object
\return bool
*/
SICPPSDKDECL bool  operator!= ( CStatus::Code lhs, const CStatus & rhs);


//*****************************************************************************
/*! \class CScriptErrorDescriptor xsi_status.h
	\brief This class extends the CStatus object with additional error details 
	if a Script Error has occured.	

	\since v5.0
	\sa Application::ExecuteScriptCode
 */
//*****************************************************************************
class SICPPSDKDECL CScriptErrorDescriptor : public CStatus
{
public:
	/*! Constructs a CScriptErrorDescriptor
	*/		
	CScriptErrorDescriptor(CStatus::Code in_code=OK);
	CScriptErrorDescriptor(LONG in_code);

	/*! Return an string containing the error message that describes the script error.
	It will return an empty string if no script error occurred.
	\return CString An error string
	*/
	CString GetDescription() const ; 

	/*! Get the line number within the script that caused the script error.
	It will return 0 if no script error occurred.
	\return ULONG Line number, where the first line of a script is line zero
	*/
	ULONG GetErrorLineNumber() const ; 

	void PutDescription(const CString & in_strDescription);
	void PutErrorLineNumber(ULONG in_ulLineNumber);
	~CScriptErrorDescriptor();
	CScriptErrorDescriptor( const CScriptErrorDescriptor & ) ;
	CScriptErrorDescriptor& operator=( const CScriptErrorDescriptor & ) ; 

private:
	void * m_pData ;	
} ;


};



#endif // __XSISTATUS_H__
