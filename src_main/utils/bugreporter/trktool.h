//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// $Header:   Y:/archives/dv/Tracker/trkar/trktool.h_v   1.73   27 May 1999 13:13:44   michaelg  $
//
// Copyright 1992-1999 Merant, Inc.  All rights reserved.
//
// PVCS Tracker Toolkit API
//
// $Tabs:5 9$
// *******************************************************************

#ifndef _TRKTOOL_H_
#define _TRKTOOL_H_ 1

// *********************************************************
// Data Types and Defines
// *********************************************************

// Define an interface version code which is used to detect
// out of date DLLs.  (see TrkHandleAlloc)
//
#define TRK_VERSION_ID	400005

// Define values specific to 32 bit or 16 bit target environments.
//
#if defined(WIN32)	// Windows NT or Windows 95 (32-bit)

#ifndef FAR
#define FAR
#endif

#ifndef TRK_DECL
#define TRK_DECL		__stdcall
#endif

#ifndef EXPORT32
#define	EXPORT32		__declspec(dllexport)
#endif

#ifndef LOADDS
#define	LOADDS
#endif

#else				// Windows 3.1 (16-bit)

#ifndef FAR
#define FAR				_far
#endif

#ifndef TRK_DECL
#define TRK_DECL		_pascal
#endif

#ifndef EXPORT32
#define	EXPORT32
#endif

#ifndef LOADDS
#define	LOADDS			_loadds
#endif

#endif //!WIN32

// Define the type used to declare all API functions.
//
#ifndef TRKAPI_DEFINED
#define TRKAPI_DEFINED
#define TRKAPI			EXPORT32 unsigned long FAR TRK_DECL
#endif

// Define the primitive data types.
//
typedef char FAR*		TRK_STR;
typedef const char FAR*	TRK_CSTR;
typedef unsigned long	TRK_UINT;
typedef TRK_UINT		TRK_BOOL;
typedef TRK_UINT		TRK_CHAR;
typedef TRK_UINT		TRK_VERSION_ID_TYPE;
typedef TRK_UINT		TRK_DBMS_LOGIN_MODE;
typedef TRK_UINT		TRK_RECORD_TYPE;
typedef TRK_UINT		TRK_FIELD_TYPE;
typedef TRK_UINT		TRK_TRANSACTION_ID;
typedef TRK_UINT		TRK_TIME;
typedef TRK_UINT		TRK_ACCESS_MODE;
typedef TRK_UINT		TRK_FILE_STORAGE_MODE;
typedef TRK_UINT		TRK_LIST_LINK_ORDER;

#define DECLARE_TRK_HANDLE(h)	struct _##h { int unused; };\
								typedef const struct _##h FAR* h

#ifndef TRK_HANDLE_DEFINED
#define TRK_HANDLE_DEFINED
DECLARE_TRK_HANDLE(TRK_HANDLE);
#endif

#ifndef TRK_RECORD_HANDLE_DEFINED
#define TRK_RECORD_HANDLE_DEFINED
DECLARE_TRK_HANDLE(TRK_RECORD_HANDLE);
#endif

#ifndef TRK_NOTE_HANDLE_DEFINED
#define TRK_NOTE_HANDLE_DEFINED
DECLARE_TRK_HANDLE(TRK_NOTE_HANDLE);
#endif

#ifndef TRK_ATTFILE_HANDLE_DEFINED
#define TRK_ATTFILE_HANDLE_DEFINED
DECLARE_TRK_HANDLE(TRK_ATTFILE_HANDLE);
#endif

#ifndef TRK_ASSOC_HANDLE_DEFINED
#define TRK_ASSOC_HANDLE_DEFINED
DECLARE_TRK_HANDLE(TRK_ASSOC_HANDLE);
#endif

#ifndef TRK_EXPORT_HANDLE_DEFINED
#define TRK_EXPORT_HANDLE_DEFINED
DECLARE_TRK_HANDLE(TRK_EXPORT_HANDLE);
#endif

#ifndef TRK_IMPORT_HANDLE_DEFINED
#define TRK_IMPORT_HANDLE_DEFINED
DECLARE_TRK_HANDLE(TRK_IMPORT_HANDLE);
#endif

// Define the upper limit on the size of string fields.
//
#define TRK_MAX_STRING	(255)

// *********************************************************
// Error Codes
// *********************************************************

enum _TrkError {
	TRK_SUCCESS						=  0,
	TRK_E_VERSION_MISMATCH			=  1,
	TRK_E_OUT_OF_MEMORY				=  2,
	TRK_E_BAD_HANDLE				=  3,
	TRK_E_BAD_INPUT_POINTER			=  4,
	TRK_E_BAD_INPUT_VALUE			=  5,
	TRK_E_DATA_TRUNCATED			=  6,
	TRK_E_NO_MORE_DATA				=  7,
	TRK_E_LIST_NOT_INITIALIZED		=  8,
	TRK_E_END_OF_LIST				=  9,
	TRK_E_NOT_LOGGED_IN				= 10,
	TRK_E_SERVER_NOT_PREPARED		= 11,
	TRK_E_BAD_DATABASE_VERSION		= 12,
	TRK_E_UNABLE_TO_CONNECT			= 13,
	TRK_E_UNABLE_TO_DISCONNECT		= 14,
	TRK_E_UNABLE_TO_START_TIMER		= 15,
	TRK_E_NO_DATA_SOURCES			= 16,
	TRK_E_NO_PROJECTS				= 17,
	TRK_E_WRITE_FAILED				= 18,
	TRK_E_PERMISSION_DENIED			= 19,
	TRK_E_SET_FIELD_DENIED			= 20,
	TRK_E_ITEM_NOT_FOUND			= 21,
	TRK_E_CANNOT_ACCESS_DATABASE	= 22,
	TRK_E_CANNOT_ACCESS_QUERY		= 23,
	TRK_E_CANNOT_ACCESS_INTRAY		= 24,
	TRK_E_CANNOT_OPEN_FILE			= 25,
	TRK_E_INVALID_DBMS_TYPE			= 26,
	TRK_E_INVALID_RECORD_TYPE		= 27,
	TRK_E_INVALID_FIELD				= 28,
	TRK_E_INVALID_CHOICE			= 29,
	TRK_E_INVALID_USER				= 30,
	TRK_E_INVALID_SUBMITTER			= 31,
	TRK_E_INVALID_OWNER				= 32,
	TRK_E_INVALID_DATE				= 33,
	TRK_E_INVALID_STORED_QUERY		= 34,
	TRK_E_INVALID_MODE				= 35,
	TRK_E_INVALID_MESSAGE			= 36,
	TRK_E_VALUE_OUT_OF_RANGE		= 37,
	TRK_E_WRONG_FIELD_TYPE			= 38,
	TRK_E_NO_CURRENT_RECORD			= 39,
	TRK_E_NO_CURRENT_NOTE			= 40,
	TRK_E_NO_CURRENT_ATTACHED_FILE	= 41,
	TRK_E_NO_CURRENT_ASSOCIATION	= 42,
	TRK_E_NO_RECORD_BEGIN			= 43,
	TRK_E_NO_MODULE					= 44,
	TRK_E_USER_CANCELLED			= 45,
	TRK_E_SEMAPHORE_TIMEOUT			= 46,
	TRK_E_SEMAPHORE_ERROR			= 47,
	TRK_E_INVALID_SERVER_NAME		= 48,
	TRK_E_NOT_LICENSED				= 49,
	TRK_NUMBER_OF_ERROR_CODES = TRK_E_NOT_LICENSED,
	//
	// Export/Import error codes follow:
	TRKEXP_ERROR_CODE_BASE = 10000,
	TRKEXP_E_EXPORT_WRONG_VERSION = TRKEXP_ERROR_CODE_BASE,
	TRKEXP_E_EXPORTSET_NOT_INIT		= 10001,
	TRKEXP_E_NO_EXPSET_NAME			= 10002,
	TRKEXP_E_BAD_EXPSET_NAME		= 10003,
	TRKEXP_E_EXPSET_FAIL_CREATE		= 10004,
	TRKEXP_E_IMPORTMAP_NOT_INIT		= 10005,
	TRKEXP_E_NO_IMPMAP_NAME			= 10006,
	TRKEXP_E_BAD_IMPMAP_NAME		= 10007,
	TRKEXP_E_IMPMAP_FAIL_CREATE		= 10008,
	TRKEXP_E_IMP_VALIDATE_FAIL		= 10009,
	TRKEXP_E_USER_NOEXIST			= 10010,
	TRKEXP_E_USER_ADD				= 10011,
	TRKEXP_E_IMPORT_NOT_INIT		= 10012,
	TRKEXP_E_BAD_EMBEDDED_QUOTE_ARG	= 10013,
	TRKEXP_E_BAD_DATEFORMAT_ARG		= 10014,
	TRKEXP_E_BAD_TIMEFORMAT_ARG		= 10015,
	TRKEXP_E_BAD_CHOICE_OPTION_ARG	= 10016,
	TRKEXP_E_BAD_USER_OPTION_ARG	= 10017,
	TRKEXP_E_BAD_NUMBER_OPTION_ARG	= 10018,
	TRKEXP_E_BAD_DATE_OPTION_ARG	= 10019,
	TRKEXP_E_ALL_NOTES_SELECTED		= 10020,
	TRKEXP_E_READ_EXPORTHDR			= 10021,
	TRKEXP_E_WRITE_EXPORTHDR		= 10022,
	TRKEXP_E_READ_RECORDHDR			= 10023,
	TRKEXP_E_WRITE_RECORDHDR		= 10024,
	TRKEXP_E_WRITE_FIELD			= 10025,
	TRKEXP_E_OPEN_FILE				= 10026,
	TRKEXP_E_READ_FIELD				= 10027,
	TRKEXP_E_READ_FIELD_WRONG_TYPE	= 10028,
	TRKEXP_E_BAD_ITEM_TYPE			= 10029,
	TRKEXP_E_READ_FROM_DB			= 10030,
	TRKEXP_E_WRITE_TO_DB			= 10031,
	TRKEXP_E_BAD_DATE				= 10032,
	TRKEXP_E_BAD_CHOICE				= 10033,
	TRKEXP_E_BAD_NUMBER				= 10034,
	TRKEXP_E_OPEN_ERRORLOG			= 10035,
	TRKEXP_E_BAD_ERRORLOG_PATH		= 10036,
	TRKEXP_E_LOGGING_ERROR			= 10037,
	TRKEXP_E_IMPORT_PERMISSION		= 10038,
	TRKEXP_E_EXPORT_PERMISSION		= 10039,
	TRKEXP_E_NEW_USER_PERMISSION	= 10040,
	TRKEXP_E_CLOSE_ERRORLOG			= 10041,
	TRKEXP_E_NEWCHOICE_SYSFLD		= 10042,
	TRKEXP_E_EXTRA_FIELDS			= 10043,
	TRKEXP_E_USER_ALREADY_IN_GROUP	= 10044,
	TRKEXP_NUMBER_OF_ERROR_CODES = TRKEXP_E_EXTRA_FIELDS,
	//
	// Internal error codes follow:
	// (Clients of the DLL should never see these.)
	TRK_INTERNAL_ERROR_CODE_BASE = 20000,
	TRK_E_INTERNAL_ERROR = TRK_INTERNAL_ERROR_CODE_BASE };

// *********************************************************
// Tracker Toolkit API prototypes
// *********************************************************

#ifndef TRK_DEBUG
#ifdef __cplusplus
extern "C" {
#endif
#endif

// TrkHandleAlloc must be called before all other Tracker API
// functions.  TrkHandleFree closes files and releases all
// associated memory.  Pass the defined constant TRK_VERSION_ID
// for the version ID.

TRKAPI TrkHandleAlloc(
	TRK_VERSION_ID_TYPE			trkVersionID,			// Input
	TRK_HANDLE FAR*				pTrkHandle);			// Output

TRKAPI TrkHandleFree(
	TRK_HANDLE FAR*				pTrkHandle);			// Input/Output

// *********************************************************

// TrkProjectLogin requires a valid trkHandle, obtained using
// TrkHandleAlloc.  DBMSLoginMode should be set to one of the three
// values listed below.  All other arguments may optionally be NULL,
// indicating that the value should be read from the current .INI
// file (see also TrkSetIniFile).  Updated values will be written
// to the current .INI file upon successful login.
//
// Successful login is prerequisite to all Tracker API functions
// except where noted otherwise.

enum _TrkDBMSLoginMode {
	TRK_USE_INI_FILE_DBMS_LOGIN		= 0,
	TRK_USE_SPECIFIED_DBMS_LOGIN	= 1,
	TRK_USE_DEFAULT_DBMS_LOGIN		= 2 };

TRKAPI TrkProjectLogin(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					userName,				// Input (see above)
	TRK_CSTR					password,				// Input (see above)
	TRK_CSTR					projectName,			// Input (see above)
	TRK_CSTR					DBMSType,				// Input (see above)
	TRK_CSTR					DBMSName,				// Input (see above)
	TRK_CSTR					DBMSUserName,			// Input (see above)
	TRK_CSTR					DBMSPassword,			// Input (see above)
	TRK_DBMS_LOGIN_MODE			DBMSLoginMode);			// Input (see above)

TRKAPI TrkProjectLogout(
	TRK_HANDLE					trkHandle);				// Input

// *********************************************************

// A default .INI file will be used if not otherwise specified.
//
// Either of these two functions may be called before login.

TRKAPI TrkSetIniFile(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					filename);				// Input

TRKAPI TrkGetIniFile(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						filename);				// Output

// *********************************************************

// Provide a general mechanism for storing (ID,value) pairs.
// Attributes with ID values of less than 1000 are used by
// Tracker.  Attributes with ID values of 1000 and greater
// may be set and retrieved by clients for any purpose.
//
// Either of these two functions may be called before login.

enum _TrkAttributeId {
	TRK_TRKTOOL_ATTRIBUTE_ID_BASE = 0,
	TRK_CANCEL_INTRAY			= 1,
	TRK_CANCEL_QUERY			= 2,
	TRK_CANCEL_IMPORT			= 3,
	TRK_NO_KEEP_ALIVE			= 4,
	TRK_NO_TIMER				= 5,
	TRK_NO_RECORD_CACHE			= 6,
	TRK_CONCURRENT_DB_TIMEOUT	= 7,
	//
	// (Clients of the DLL are free to use values at or
	// above this threshhold.)
	TRK_USER_ATTRIBUTE_ID_BASE = 1000 };

TRKAPI TrkSetNumericAttribute(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					attributeId,			// Input
	TRK_UINT					value);					// Input

TRKAPI TrkGetNumericAttribute(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					attributeId,			// Input
	TRK_UINT FAR*				pValue);				// Output

// *********************************************************

// Provide an enumeration of all Projects in the current DB.
// TrkInitProjectList initializes an in-memory list of
// project names, which are accessed sequentially using
// TrkGetNextProject.
//
// Example:
//		...
//		rc = TrkInitProjectList(h, NULL, NULL, NULL, NULL);
//		while (TrkGetNextProject(h, sizeof(buf), buf) == TRK_SUCCESS)
//			...
//
// These two functions may be called before login.  However, the
// interpretation of the arguments differs depending on which mode
// of operation is used:
//
// If called before login, TrkInitProjectList reads all of its
// input arguments, where they will be interpreted exactly as in
// TrkProjectLogin.  This mode of operation may be useful, for
// example, in preparing a login dialog.
//
// If called after a successful login, the values for all
// arguments, other than trkHandle, are ignored.

TRKAPI TrkInitProjectList(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					DBMSType,				// Input (see above)
	TRK_CSTR					DBMSName,				// Input (see above)
	TRK_CSTR					DBMSUserName,			// Input (see above)
	TRK_CSTR					DBMSPassword,			// Input (see above)
	TRK_DBMS_LOGIN_MODE			DBMSLoginMode);			// Input (see above)

TRKAPI TrkGetNextProject(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						projectName);			// Output

// *********************************************************

// Provide an enumeration of all DBMS Types known to Tracker.
//
// May be called before login.

TRKAPI TrkInitDBMSTypeList(
	TRK_HANDLE					trkHandle);				// Input

TRKAPI TrkGetNextDBMSType(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						DBMSType);				// Output

// *********************************************************

// Provide access to current login information

TRKAPI TrkGetLoginUserName(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						userName);				// Output

TRKAPI TrkGetLoginProjectName(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						projectName);			// Output

TRKAPI TrkGetLoginDBMSType(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						DBMSType);				// Output

TRKAPI TrkGetLoginDBMSName(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						DBMSName);				// Output

// *********************************************************

// Provide an enumeration of all Record Types (SCR, Time)
// defined for the current project.

TRKAPI TrkInitRecordTypeList(
	TRK_HANDLE					trkHandle);				// Input

TRKAPI TrkGetNextRecordType(
	TRK_HANDLE					trkHandle,				// Input
	TRK_RECORD_TYPE FAR*		pRecordType);			// Output

TRKAPI TrkGetRecordTypeName(
	TRK_HANDLE					trkHandle,				// Input
	TRK_RECORD_TYPE				recordType,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						recordTypeName);		// Output

// *********************************************************

// Provide an enumeration of all Fields defined in the
// current project.

enum _TrkFieldType {
	TRK_FIELD_TYPE_NONE			= 0,
	TRK_FIELD_TYPE_CHOICE		= 1,
	TRK_FIELD_TYPE_STRING		= 2,
	TRK_FIELD_TYPE_NUMBER		= 3,
	TRK_FIELD_TYPE_DATE			= 4,
	TRK_FIELD_TYPE_SUBMITTER	= 5,
	TRK_FIELD_TYPE_OWNER		= 6,
	TRK_FIELD_TYPE_USER			= 7,
	TRK_FIELD_TYPE_ELAPSED_TIME = 8 };

TRKAPI TrkInitFieldList(
	TRK_HANDLE					trkHandle,				// Input
	TRK_RECORD_TYPE				recordType);			// Input

TRKAPI TrkGetNextField(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						fieldName,				// Output
	TRK_FIELD_TYPE FAR*			pFieldType);			// Output (optional)

// Provide general information about Fields.

TRKAPI TrkGetFieldType(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					fieldName,				// Input
	TRK_RECORD_TYPE				recordType,				// Input
	TRK_FIELD_TYPE FAR*			pFieldType);			// Output

TRKAPI TrkGetFieldMaxDataLength(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					fieldName,				// Input
	TRK_RECORD_TYPE				recordType,				// Input
	TRK_UINT FAR*				pMaxDataLength);		// Output

TRKAPI TrkGetFieldDefaultStringValue(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					fieldName,				// Input
	TRK_RECORD_TYPE				recordType,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						defaultValue);			// Output

TRKAPI TrkGetFieldDefaultNumericValue(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					fieldName,				// Input
	TRK_RECORD_TYPE				recordType,				// Input
	TRK_UINT FAR*				pDefaultValue);			// Output

TRKAPI TrkGetFieldRange(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					fieldName,				// Input
	TRK_RECORD_TYPE				recordType,				// Input
	TRK_UINT FAR*				pMinValue,				// Output
	TRK_UINT FAR*				pMaxValue);				// Output

// *********************************************************

// Provide an enumeration of all stored Queries defined in the
// current project.

TRKAPI TrkInitQueryNameList(
	TRK_HANDLE					trkHandle);				// Input

TRKAPI TrkGetNextQueryName(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						queryName,				// Output
	TRK_RECORD_TYPE FAR*		pRecordType);			// Output (optional)

TRKAPI TrkGetQueryRecordType(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					queryName,				// Input
	TRK_RECORD_TYPE FAR*		pRecordType);			// Output

// *********************************************************

// Provide an enumeration of all Users defined in the
// current project.

TRKAPI TrkInitUserList(
	TRK_HANDLE					trkHandle);				// Input

TRKAPI TrkGetNextUser(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						userName);				// Output

// Provide general information about Users.

TRKAPI TrkGetUserFullName(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					userName,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						fullName);				// Output

TRKAPI TrkGetUserEmail(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					userName,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						emailAddress);			// Output

// *********************************************************

// Provide an enumeration of all values for the specified
// Choice Field.

TRKAPI TrkInitChoiceList(
	TRK_HANDLE					trkHandle,				// Input
	TRK_CSTR					fieldName,				// Input
	TRK_RECORD_TYPE				recordType);			// Input

TRKAPI TrkGetNextChoice(
	TRK_HANDLE					trkHandle,				// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						choiceName);			// Output

// *********************************************************

// Provide a Transaction ID representing the most recent
// change to the current project.
//
// A Transaction ID represents the exact date and time when
// a change is made in a DB.  TrkGetProjectTransactionID
// provides a Transaction ID which represents the most
// recent update to any record of the specified type in
// the DB.  Note that separate Transaction ID domains exist
// for each record type.

TRKAPI TrkGetProjectTransactionID(
	TRK_HANDLE					trkHandle,				// Input
	TRK_RECORD_TYPE				recordType,				// Input
	TRK_TRANSACTION_ID FAR*		pTransactionID);		// Output

// *********************************************************

// Tracker Record Handles are used in place of Tracker API
// Handles in those functions below which operate on Record
// data.  TrkRecordHandleAlloc must be called first to create
// a record handle.  TrkRecordHandleFree releases all memory
// associated with that handle.
//
// If desired, multiple Record Handles may be used concurrently.

TRKAPI TrkRecordHandleAlloc(
	TRK_HANDLE					trkHandle,				// Input
	TRK_RECORD_HANDLE FAR*		pTrkRecordHandle);		// Output

TRKAPI TrkRecordHandleFree(
	TRK_RECORD_HANDLE FAR*		pTrkRecordHandle);		// Input/Output

// *********************************************************

// Provide access to sets of records from the DB.  Two functions
// TrkQueryInitRecordList and TrkInTrayInitRecordList create
// in-memory sets of records, which are accessed sequentially
// using TrkGetNextRecord.  TrkGetNextRecord can be thought of
// as making a record 'current' in that subsequent calls to the
// various record access functions will operate on data for that
// particular record.
//
// TrkQueryInitRecordList and TrkInTrayInitRecordList each take
// an optional input Transaction ID.  If provided, only records
// which were modified after the time of the specified
// Transaction ID will be included in the output set.  In
// addition, a Transaction ID is returned, which represents the
// time of the most recent update to the DB.

TRKAPI TrkQueryInitRecordList(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_CSTR					queryName,				// Input
	TRK_TRANSACTION_ID			transactionID,			// Input (optional)
	TRK_TRANSACTION_ID FAR*		pNewTransactionID);		// Output (optional)

TRKAPI TrkInTrayInitRecordList(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_RECORD_TYPE				recordType,				// (currently unused)
	TRK_TRANSACTION_ID			transactionID,			// Input (optional)
	TRK_TRANSACTION_ID FAR*		pNewTransactionID);		// Output (optional)

TRKAPI TrkGetNextRecord(
	TRK_RECORD_HANDLE			trkRecordHandle);		// Input

// *********************************************************

// TrkQueryGetSingleRecord provides access to an individual
// Record given its ID number and Type.  The record will be
// made 'current' in the same sense as TrkGetNextRecord.

TRKAPI TrkGetSingleRecord(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_UINT					recordId,				// Input
	TRK_RECORD_TYPE				recordType);			// Input

// *********************************************************

// Submit a new record to the DB.  TrkNewRecordBegin should
// be called first to clear the current in-memory record,
// i.e. to set all data to their default values.
//
// After all fields, etc. have been set to their desired
// values, TrkNewRecordCommit is called to actually add the
// record to the DB.  The transaction may be cancelled any
// time before the commit with TrkRecordCancelTransaction.

TRKAPI TrkNewRecordBegin(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_RECORD_TYPE				recordType);			// Input

TRKAPI TrkNewRecordCommit(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_TRANSACTION_ID FAR*		pNewTransactionID);		// Output

TRKAPI TrkRecordCancelTransaction(
	TRK_RECORD_HANDLE			trkRecordHandle);		// Input

// *********************************************************

// Update a record in the DB.  TrkUpdateRecordBegin populates
// all fields of the current Record with data from the DB.
// TrkUpdateRecordBegin also locks the record in the DB to
// prevent concurrent access.
//
// After the desired changes to fields, etc. have been made,
// TrkUpdateRecordCommit is called to update the record in the
// DB.  The transaction may be cancelled any time before the
// commit with TrkRecordCancelTransaction.

TRKAPI TrkUpdateRecordBegin(
	TRK_RECORD_HANDLE			trkRecordHandle);		// Input

TRKAPI TrkUpdateRecordCommit(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_TRANSACTION_ID FAR*		pNewTransactionID);		// Output

// *********************************************************

// Remove a record from the DB.

TRKAPI TrkDeleteRecord(
	TRK_RECORD_HANDLE			trkRecordHandle);		// Input

// *********************************************************

// Provide miscellaneous information on the current Record.

TRKAPI TrkGetRecordTransactionID(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_TRANSACTION_ID FAR*		pSubmitTransactionID,	// Output (optional)
	TRK_TRANSACTION_ID FAR*		pUpdateTransactionID);	// Output (optional)

TRKAPI TrkGetRecordRecordType(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_RECORD_TYPE FAR*		pRecordType);			// Output

// *********************************************************

// Get or set data fields in a record.
//
// "Set" functions only modify the in-memory copy of the data.

TRKAPI TrkGetNumericFieldValue(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_CSTR					fieldName,				// Input
	TRK_UINT FAR*				pFieldValue);			// Output

TRKAPI TrkGetStringFieldValue(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_CSTR					fieldName,				// Input
	TRK_UINT					fieldValueBufferSize,	// Input
	TRK_STR						fieldValue);			// Output

TRKAPI TrkSetNumericFieldValue(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_CSTR					fieldName,				// Input
	TRK_UINT					fieldValue);			// Input

TRKAPI TrkSetStringFieldValue(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_CSTR					fieldName,				// Input
	TRK_CSTR					fieldValue);			// Input

// *********************************************************

// Provide a Transaction ID representing the most recent
// change to a particular Field in the current record.

TRKAPI TrkGetFieldTransactionID(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_CSTR					fieldName,				// Input
	TRK_TRANSACTION_ID FAR*		pTransactionID);		// Output

// *********************************************************

// Provide permission information on a Record which is about
// to be Submitted to or Updated in the DB.
//
// Note that permissions depend on the state of the record
// in the DB.  For example, a field may be modifiable upon Submit
// but not Update, and vice-versa.

enum _TrkFieldAccessMode {
	TRK_READ_ONLY = 0,
	TRK_READ_WRITE = 2 };

TRKAPI TrkGetFieldAccessRights(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_CSTR					fieldName,				// Input
	TRK_ACCESS_MODE FAR*		pAccessMode);			// Output

// *********************************************************

// Get or set data in a record's Description.

// TrkGetDescriptionData and TrkSetDescriptionData may be
// called multiple times to read data which exceeds the size
// of the caller's buffer.

TRKAPI TrkGetDescriptionDataLength(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_UINT FAR*				pDataBufferSize);		// Output

TRKAPI TrkGetDescriptionData(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_UINT					maxBufferSize,			// Input
	TRK_STR						data,					// Output
	TRK_UINT FAR*				pDataRemaining);		// Output

TRKAPI TrkSetDescriptionData(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_UINT					currentBufferSize,		// Input
	TRK_CSTR					data,					// Input
	TRK_UINT					dataRemaining);			// Input

// Provide permission information.

TRKAPI TrkGetDescriptionAccessRights(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_ACCESS_MODE FAR*		pAccessMode);			// Output

// Provide a Transaction ID representing the most recent
// change to the description.

TRKAPI TrkGetDescriptionTransactionID(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_TRANSACTION_ID FAR*		pTransactionID);		// Output

// *********************************************************

// Tracker Note Handles are used in place of Tracker Record
// Handles in those functions below which operate on Note
// data.  TrkNoteHandleAlloc must be called first to create
// a Note Handle.  TrkNoteHandleFree releases all memory
// associated with that handle.
//
// If desired, multiple Note Handles may be used concurrently.

TRKAPI TrkNoteHandleAlloc(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_NOTE_HANDLE FAR*		pTrkNoteHandle);		// Output

TRKAPI TrkNoteHandleFree(
	TRK_NOTE_HANDLE FAR*		pTrkNoteHandle);		// Input/Output

// *********************************************************

// TrkInitNoteList initializes an in-memory list of entries which
// are accessed sequentially.  TrkGetNextNote is used to read
// the current Note (including the first, if present).
// TrkGetNextNote can be thought of as making a note 'current'
// in that subsequent calls to the various note access functions
// will operate on data for that particular Note.  TrkAddNewNote
// is used to insert a new Note in the list at the current
// position, (making the new note current).  A new Note may be
// appended to the list by adding it after the last Note.

TRKAPI TrkInitNoteList(
	TRK_NOTE_HANDLE				trkNoteHandle);			// Input

TRKAPI TrkGetNextNote(
	TRK_NOTE_HANDLE				trkNoteHandle);			// Input

TRKAPI TrkAddNewNote(
	TRK_NOTE_HANDLE				trkNoteHandle);			// Input

TRKAPI TrkDeleteNote(
	TRK_NOTE_HANDLE				trkNoteHandle);			// Input

// *********************************************************

// Get or set data in a Note.

// TrkGetNoteData and TrkSetNoteData may be called multiple
// times to read data which exceeds the size of the caller's
// buffer.

// "Set" functions only modify the in-memory copy of the data.

TRKAPI TrkGetNoteTitle(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						noteTitle);				// Output

TRKAPI TrkSetNoteTitle(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_CSTR					noteTitle);				// Input


TRKAPI TrkGetNoteAuthor(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						authorName);			// Output

TRKAPI TrkSetNoteAuthor(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_CSTR					authorName);			// Input


TRKAPI TrkGetNoteCreateTime(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_TIME FAR*				pCreateTime);			// Output

TRKAPI TrkSetNoteCreateTime(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_TIME					createTime);			// Input


TRKAPI TrkGetNoteModifyTime(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_TIME FAR*				pModifyTime);			// Output

TRKAPI TrkSetNoteModifyTime(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_TIME					modifyTime);			// Input


TRKAPI TrkGetNoteDataLength(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_UINT FAR*				pDataBufferSize);		// Output


TRKAPI TrkGetNoteData(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_UINT					maxBufferSize,			// Input
	TRK_STR						noteData,				// Output
	TRK_UINT FAR*				pDataRemaining);		// Output

TRKAPI TrkSetNoteData(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_UINT					currentBufferSize,		// Input
	TRK_CSTR					noteData,				// Input
	TRK_UINT					dataRemaining);			// Input

// *********************************************************

// Provide a Transaction ID representing the most recent
// change to a particular Note in the current record.

TRKAPI TrkGetNoteTransactionID(
	TRK_NOTE_HANDLE				trkNoteHandle,			// Input
	TRK_TRANSACTION_ID FAR*		pTransactionID);		// Output

// *********************************************************

// Tracker Attached File Handles are used in place of Tracker Record
// Handles in those functions below which operate on Attached File
// data.  TrkAttachedFileHandleAlloc must be called first to create
// an Attached File Handle.  TrkAttachedFileHandleFree releases all
// memory associated with that handle.
//
// If desired, multiple Attached File Handles may be used concurrently.

TRKAPI TrkAttachedFileHandleAlloc(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_ATTFILE_HANDLE FAR*		pTrkAttFileHandle);		// Output

TRKAPI TrkAttachedFileHandleFree(
	TRK_ATTFILE_HANDLE FAR*		pTrkAttFileHandle);		// Input/Output

// *********************************************************

// TrkInitAttachedFileList initializes an in-memory list of the Files
// which are attached to the current record, and are accessed
// sequentially.  TrkGetNextAttachedFile is used to make each
// successive Attached File current.  Subsequent calls to the various
// AttFile access functions will operate on data for the current
// Attached File.  TrkAddNewAttachedFile is used to insert
// a new AttachedFile in the list at the current position, (making
// the new Attached File current).  A new Attached File may be
// appended to the list by adding it after the last Attached File.

TRKAPI TrkInitAttachedFileList(
	TRK_ATTFILE_HANDLE			trkAttFileHandle);		// Input

TRKAPI TrkGetNextAttachedFile(
	TRK_ATTFILE_HANDLE			trkAttFileHandle);		// Input

enum _TrkFileStorageMode {
	TRK_FILE_BINARY	= 0,
	TRK_FILE_ASCII	= 1 };

TRKAPI TrkAddNewAttachedFile(
	TRK_ATTFILE_HANDLE			trkAttFileHandle,		// Input
	TRK_CSTR					filename,				// Input
	TRK_FILE_STORAGE_MODE		storageMode);			// Input

TRKAPI TrkDeleteAttachedFile(
	TRK_ATTFILE_HANDLE			trkAttFileHandle);		// Input

// *********************************************************

// Extract or get information about an Attached File.  The function
// TrkExtractAttachedFile copies the file contents out of the
// database and into the caller-specified file name.  If the file
// already exists, it is overwritten.

TRKAPI TrkGetAttachedFileName(
	TRK_ATTFILE_HANDLE			trkAttFileHandle,		// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						filename);				// Output

TRKAPI TrkGetAttachedFileTime(
	TRK_ATTFILE_HANDLE			trkAttFileHandle,		// Input
	TRK_TIME FAR*				pTimestamp);			// Output

TRKAPI TrkGetAttachedFileStorageMode(
	TRK_ATTFILE_HANDLE			trkAttFileHandle,		// Input
	TRK_FILE_STORAGE_MODE FAR*	pStorageMode);			// Output

TRKAPI TrkExtractAttachedFile(
	TRK_ATTFILE_HANDLE			trkAttFileHandle,		// Input
	TRK_CSTR					filename);				// Input

// *********************************************************

// Provide a Transaction ID representing when a particular
// Attached File was added to the current record.

TRKAPI TrkGetAttachedFileTransactionID(
	TRK_ATTFILE_HANDLE			trkAttFileHandle,		// Input
	TRK_TRANSACTION_ID FAR*		pTransactionID);		// Output

// *********************************************************

// Tracker Association Handles are used in place of Tracker
// Record Handles in those functions below which operate on
// Module Association data.  TrkAssociationHandleAlloc must
// be called first to create an Association Handle.
// TrkAssociationHandleFree releases all memory associated with
// that handle.
//
// If desired, multiple Association Handles may be used
// concurrently.

TRKAPI TrkAssociationHandleAlloc(
	TRK_RECORD_HANDLE			trkRecordHandle,		// Input
	TRK_ASSOC_HANDLE FAR*		pTrkAssociationHandle);	// Output

TRKAPI TrkAssociationHandleFree(
	TRK_ASSOC_HANDLE FAR*		pTrkAssociationHandle);	// Input/Output

// *********************************************************

// TrkInitAssociationList initializes an in-memory list of the
// Modules which are associated with the current record, and are
// accessed sequentially.  TrkGetNextAssociation is used to make
// each successive Association current.  Subsequent calls to the
// various Association access functions will operate on data for
// the current Association.  TrkAddNewAssociation is used to
// insert a new Association in the list at the current position,
// (making the new Association current).  A new Association may
// be appended to the list by adding it after the last
// Association.

TRKAPI TrkInitAssociationList(
	TRK_ASSOC_HANDLE			trkAssociationHandle);	// Input

TRKAPI TrkGetNextAssociation(
	TRK_ASSOC_HANDLE			trkAssociationHandle);	// Input

TRKAPI TrkAddNewAssociation(
	TRK_ASSOC_HANDLE			trkAssociationHandle);	// Input

TRKAPI TrkDeleteAssociation(
	TRK_ASSOC_HANDLE			trkAssociationHandle);	// Input

// *********************************************************

// Get or set data in an Association.

// TrkGetAssociationText and TrkSetAssociationText may be
// called multiple times to read data which exceeds the size
// of the caller's buffer.

// "Set" functions only modify the in-memory copy of the data.

TRKAPI TrkGetAssociationModuleName(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						moduleName);			// Output

TRKAPI TrkSetAssociationModuleName(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_CSTR					moduleName);			// Input


TRKAPI TrkGetAssociationUser(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						userName);				// Output

TRKAPI TrkSetAssociationUser(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_CSTR					userName);				// Input


TRKAPI TrkGetAssociationRevisionFound(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						revisionFound);			// Output

TRKAPI TrkSetAssociationRevisionFound(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_CSTR					revisionFound);			// Input


TRKAPI TrkGetAssociationRevisionFixed(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_UINT					bufferSize,				// Input
	TRK_STR						revisionFixed);			// Output

TRKAPI TrkSetAssociationRevisionFixed(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_CSTR					revisionFixed);			// Input


TRKAPI TrkGetAssociationTimeFound(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_TIME FAR*				pTimeFound);			// Output

TRKAPI TrkSetAssociationTimeFound(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_TIME					timeFound);				// Input


TRKAPI TrkGetAssociationTimeFixed(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_TIME FAR*				pTimeFixed);			// Output

TRKAPI TrkSetAssociationTimeFixed(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_TIME					timeFixed);				// Input


TRKAPI TrkGetAssociationTextLength(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_UINT FAR*				pDataBufferSize);		// Output


TRKAPI TrkGetAssociationText(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_UINT					maxBufferSize,			// Input
	TRK_STR						description,			// Output
	TRK_UINT FAR*				pDataRemaining);		// Output

TRKAPI TrkSetAssociationText(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_UINT					currentBufferSize,		// Input
	TRK_CSTR					description,			// Input
	TRK_UINT					dataRemaining);			// Input

// *********************************************************

// Provide a Transaction ID representing when a particular
// Association was added to the current record.

TRKAPI TrkGetAssociationTransactionID(
	TRK_ASSOC_HANDLE			trkAssociationHandle,	// Input
	TRK_TRANSACTION_ID FAR*		pTransactionID);		// Output

// *********************************************************
// *********************************************************
// Export / Import
// *********************************************************
// *********************************************************

// *********************************************************
// The Tracker Export Handle is used in place of Tracker
// Handles in those functions below which perform export
// operations.
//
// TrkExportHandleAlloc must be called first to create
// an Export Handle.  TrkExportHandleFree releases all
// memory associated with that handle.
//
// If desired, multiple Export Handles may be used
// concurrently.
// *********************************************************

TRKAPI TrkExportHandleAlloc(
	TRK_HANDLE					trkHandle,				// Input
	TRK_EXPORT_HANDLE FAR*		pTrkExportHandle);		// Output

TRKAPI TrkExportHandleFree(
	TRK_EXPORT_HANDLE FAR*		pTrkExportHandle);		// Input/Output

// *********************************************************
// Provide higher level functions which export records to a
// file using a fixed format.
//
// The TrkExportRecordsBegin function is called first to set
// the filename, export set, delimiter character, separator
// character, the type of quote embedding to use (either \" or
// ""), as well as the format for date and time fields.  If the
// "trackerFormat" argument is set to TRK_EXPORT_TRACKER_FORMAT,
// then the remaining arguments should be set to zero and will 
// be ignored.  The "trackerFormat" argument must be set to 
// TRK_EXPORT_TRACKER_FORMAT if the export file will be 
// imported back into Tracker.
//
// The TrkExportOneRecord function exports one record.  The
// TrkExportRecordsClose function must be called to complete
// the export operation.
// *********************************************************

typedef TRK_UINT		TRK_EXPORT_FORMAT;
typedef TRK_UINT		TRK_EMB_QUOTE;
typedef TRK_UINT		TRK_DATE_FORMAT;
typedef TRK_UINT		TRK_TIME_FORMAT;

enum _TrkExportFormat {
	TRK_EXPORT_FORMAT_SIMPLE	= 0,
	TRK_EXPORT_FORMAT_TRACKER	= 1,
	TRK_EXPORT_FORMAT_PDIFF		= 2,
	TRK_EXPORT_FORMAT_XML       = 3 };

enum _TrkEmbeddedQuote {
	TRK_DOUBLE_QUOTE		= 1,
	TRK_BACKSLASH_QUOTE		= 2 };

enum _TrkDateFormat {
	TRK_CONTROL_PANEL_DATE	= 1,
	TRK_DBASE_FORMAT		= 2 };

enum _TrkTimeFormat {
	TRK_CONTROL_PANEL_TIME	= 1,
	TRK_24HOUR				= 2,
	TRK_24HOUR_LEADING_ZERO	= 3 };

TRKAPI TrkExportRecordsBegin(
	TRK_EXPORT_HANDLE			trkExportHandle,		// Input
	TRK_RECORD_TYPE				recordType,				// Input
	TRK_CSTR					fileToWriteTo,			// Input
	TRK_BOOL					trackerFormat,			// Input
	TRK_CHAR					delimiterCharacter,		// Input (optional)
	TRK_CHAR					separatorCharacter,		// Input (optional)
	TRK_EMB_QUOTE				embeddedQuoteType,		// Input (optional)
	TRK_DATE_FORMAT				dateFormat,				// Input (optional)
	TRK_TIME_FORMAT				timeFormat);			// Input (optional)

TRKAPI TrkExportOneRecord(
	TRK_EXPORT_HANDLE			trkExportHandle,		// Input
	TRK_RECORD_HANDLE			trkRecordHandle);		// Input

TRKAPI TrkExportRecordsClose(
	TRK_EXPORT_HANDLE			trkExportHandle);		// Input

// *********************************************************
// The Tracker Import Handle is used in place of Tracker
// Handles in those functions below which perform import
// operations.
//
// TrkImportHandleAlloc must be called first to create an
// Import Handle.  TrkImportHandleFree releases all memory
// associated with that handle.
//
// If desired, multiple Import Handles may be used
// concurrently.
// *********************************************************

TRKAPI TrkImportHandleAlloc(
	TRK_HANDLE					trkHandle,				// Input
	TRK_IMPORT_HANDLE FAR*		pTrkImportHandle);		// Output

TRKAPI TrkImportHandleFree(
	TRK_IMPORT_HANDLE FAR*		pTrkImportHandle);		// Input/Output

// *********************************************************
// Provide higher level functions to import records from a
// file.  Assumes the file was written using the same format as
// the "TrkExport" functions above and that "trackerFormat" was
// set to TRUE in the TrkExportRecordsBegin function.
//
// TrkImportInit must be called first.  The filename containing
// the import data is specified in this function call.  In
// addition, arguments are given to indicate which actions to
// take when a bad date, choice, user, or out of range number
// is encountered. If these functions are not called, the
// default behavior is to fail import.  If the
// TRK_SET_TO_SPECIFIED date option is used, the "defaultDate"
// argument must be provided (in Control Panel format - default
// format is MM/DD/YY).
//
// TrkImportNewRecords performs the actual import. Failure to
// set "trackerFormat" to TRUE on export will result in a
// failure to import.  The fields being imported will be
// automatically mapped by type and name.  Fields that don't
// match any fields in the import project will be discarded.
// All notes will be imported regardless of note title.
// *********************************************************

typedef TRK_UINT		TRK_DATE_OPTION;
typedef TRK_UINT		TRK_CHOICE_OPTION;
typedef TRK_UINT		TRK_USER_OPTION;
typedef TRK_UINT		TRK_NUMBER_OPTION;

enum _TrkDateOption {
	TRK_FAIL_DATE			= 0,
	TRK_SET_CURRENT			= 1,
	TRK_SET_TO_SPECIFIED	= 2 };

enum _TrkChoiceOption {
	TRK_FAIL_CHOICE			= 0,
	TRK_DEFAULT_CHOICE		= 1,
	TRK_NEW_CHOICE			= 2 };

enum _TrkUserOption {
	TRK_FAIL_USER			= 0,
	TRK_ADD_USER			= 1,
	TRK_ADD_USER_WITH_GROUP	= 2,
	TRK_DEFAULT_USER		= 3 };

enum _TrkNumberOption {
	TRK_FAIL_NUMBER			= 0,
	TRK_DEFAULT_NUMBER		= 1 };

TRKAPI TrkImportInit(
	TRK_IMPORT_HANDLE			trkImportHandle,		// Input
	TRK_CSTR					fileToReadFrom,			// Input
	TRK_BOOL					trackerFormat,			// Input
	TRK_RECORD_TYPE				recordType,				// Input (optional)
	TRK_CHAR					delimiterCharacter,		// Input (optional)
	TRK_CHAR					separatorCharacter,		// Input (optional)
	TRK_DATE_FORMAT				dateFormat,				// Input (optional)
	TRK_TIME_FORMAT				timeFormat);			// Input (optional)

TRKAPI TrkImportSetOptions(
	TRK_IMPORT_HANDLE			trkImportHandle,		// Input
	TRK_CHOICE_OPTION			choiceOption,			// Input (optional)
	TRK_USER_OPTION				userOption,				// Input (optional)
	TRK_NUMBER_OPTION			numberOption,			// Input (optional)
	TRK_DATE_OPTION				dateOption,				// Input (optional)
	TRK_CSTR					defaultDate);			// Input (optional)

TRKAPI TrkImportNewRecords(
	TRK_IMPORT_HANDLE			trkImportHandle,		// Input
	TRK_CSTR					errorLogFile);			// Input (optional)

// *********************************************************
// *********************************************************
// Callbacks
// *********************************************************
// *********************************************************

// Use of callbacks is optional.

// Define the list of messages for which callbacks will be issued.

enum _TrkCallbackMessage {
	TRK_MSG_API_TRACE				=  1,
	TRK_MSG_API_EXIT				=  2,
	TRK_MSG_ODBC_ERROR				=  3,
	TRK_MSG_INVALID_FIELD_VALUE		=  4,
	TRK_MSG_DATA_TRUNCATED			=  5,
	TRK_MSG_FORCE_LOGOUT			=  6,
	TRK_MSG_IMPORT_ERROR			=  7,
	TRK_MSG_INTRAY_PROGRESS			=  8,
	TRK_MSG_QUERY_PROGRESS			=  9,
	TRK_MSG_IMPORT_PROGRESS			= 10,
	TRK_LAST_CALLBACK_MSG };	// (not a message; marks end of list)

enum _TrkCallbackReturnCode {
	TRK_MSG_NOT_HANDLED		= 0,
	TRK_MSG_HANDLED			= 1 };

// Define the prototype for Callback functions.
//
// Arguments:
//		The message argument 'msg' will be one value from the
//			list of messages defined above.
//		The values for 'param1', 'param2' and 'strParam' will vary
//			depending on the message.
//		The 'userData' value is the same value that was specified
//			by the client when the callback was registered.  See
//			also 'TrkRegisterCallback'.
//
// The return value should be set within the client's callback
// handler code to be either:
//		TRK_MSG_NOT_HANDLED to continue processing remaining
//			callbacks (from the list of registered functions), or
//		TRK_MSG_HANDLED to bypass further callback processing.
//
// Note the use of '_loadds' in the callback declaration.  16-bit
// clients must use this or its equivalent (such as listing the
// function as an EXPORT in the .DEF file) to ensure that the Data
// Segment is changed to the client app's value upon entry into
// the callback.

typedef TRKAPI LOADDS TRKCALLBACK(
	TRK_UINT msg,
	TRK_UINT param1,
	TRK_UINT param2,
	TRK_CSTR strParam,
	TRK_UINT userData);

typedef TRKCALLBACK FAR *PTRKCALLBACK;

// *********************************************************

// Callback registration.
//
// The linkOrder parameter should be set to either TRK_LIST_ADD_HEAD
// or TRK_LIST_ADD_TAIL to indicate whether the callback should be
// called first or last.
//
// The userData value will be supplied back to the client as one
// of the arguments in the callback.
//
// This function may be called before login.

enum _TrkLinkOrder {
	TRK_LIST_ADD_HEAD	= 0,
	TRK_LIST_ADD_TAIL	= 1 };

TRKAPI TrkRegisterCallback(
	TRK_HANDLE					trkHandle,				// Input
	PTRKCALLBACK				pCallbackFunction,		// Input
	TRK_LIST_LINK_ORDER			linkOrder,				// Input
	TRK_UINT					userData);				// Input (optional)

// *********************************************************

// Remove a function from the list of callbacks.
//
// This function may be called before login.

TRKAPI TrkUnregisterCallback(
	TRK_HANDLE					trkHandle,				// Input
	PTRKCALLBACK				pCallbackFunction);		// Input

// *********************************************************

#ifndef TRK_DEBUG
#ifdef __cplusplus
}
#endif
#endif

#endif // _TRKTOOL_H_

// *******************************************************************
