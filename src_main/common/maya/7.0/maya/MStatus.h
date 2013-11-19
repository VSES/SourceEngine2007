#ifndef _MStatus
#define _MStatus

//
//-
// ==========================================================================
// Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
// rights reserved.
// 
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Alias Systems Corp. ("Alias") and/or its 
// licensors, which is protected by Canadian and US federal copyright law and 
// by international treaties.
// 
// The Data may not be disclosed or distributed to third parties or be copied 
// or duplicated, in whole or in part, without the prior written consent of 
// Alias.
// 
// THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
// TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
// WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
// PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
// WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
// ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
// ==========================================================================
//+
//
// CLASS: MStatus
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MStatus)
//
// This class facilitates API level error handling.  It encapsulates the status
// code and the internal MAYA error code as return by API functions.   The 
// user can query, set, print the error code along with the description.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#if defined(REQUIRE_IOSTREAM)
	// External usage
#include <maya/MIOStream.h>
#else
	// Internal usage
#include <maya/MIOStreamFwd.h>
#endif

// *****************************************************************************

// DECLARATIONS

class MString;

// *****************************************************************************

// CLASS DECLARATION (MStatus)

/// Manipulate Maya Status codes
/**
  Methods for passing status codes between user code and Maya
*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MStatus
{

public:

	/// Available Status Codes
	enum MStatusCode
	{
		/// The operation was successful
		kSuccess = 0,
		/// The operation failed
		kFailure,
		/// The operation failed due to insufficient memory
		kInsufficientMemory,
		/// An invalid parameter was provided
		kInvalidParameter,
		/// Application is not licensed for the attempted operation
		kLicenseFailure,
		/// Returned by MPxNode::compute for unrecognised plugs
		kUnknownParameter,
		/// Not currently used
		kNotImplemented,
		/// Not currently used
		kNotFound,
		/// Not currently used
		kEndOfFile
	};

	/// 
						MStatus();
	/// 
						MStatus( MStatusCode );
	/// 
						MStatus( const MStatus& );

	///
	MStatus&			operator=( const MStatus& rhs);
	/// 
	bool				operator==( const MStatus& rhs ) const;
	/// 
	bool				operator==( const MStatusCode rhs ) const;
	/// 
	bool				operator!=( const MStatus& rhs ) const;
	/// 
	bool				operator!=( const MStatusCode rhs ) const;
	/// 
	operator			bool() const;
	/// 
	bool				error() const;
	///
	void				clear();
	/// 
	MStatusCode	        statusCode() const;
	/// 
	MString				errorString() const;
	/// 
	void				perror( const char * ) const;
	/// 
	void				perror( const MString& ) const;
	/// Internal use only
	void				set( bool status,
							 unsigned char statusCode,
							 unsigned char internalStatusCode);

	void				setSuccess();

	/// 
	friend OPENMAYA_EXPORT IOS_REF(ostream)& operator<<( IOS_REF(ostream)&, MStatus&);
	/// 
	friend OPENMAYA_EXPORT inline bool operator==( const MStatus::MStatusCode,
												   const MStatus& );
	/// 
	friend OPENMAYA_EXPORT inline bool operator!=( const MStatus::MStatusCode,
												   const MStatus& );


protected:
// No protected members

private:
	unsigned char		fStatusCode; 
	unsigned char		fInternalStatusCode; 
	bool				fStatus;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

typedef MStatus MS;


// Convenience macros for checking the return status from API methods.
//

// Output an error message and returns the value 'retVal' if status
// is not kSuccess.
//
#define CHECK_MSTATUS_AND_RETURN(x, retVal)				\
{ 														\
	MStatus _maya_status = (x);							\
	if ( MStatus::kSuccess != _maya_status ) 			\
	{													\
		cerr << "\nAPI error detected in " << __FILE__ 	\
			 <<	" at line "	<< __LINE__ << endl;		\
		_maya_status.perror ( "" );						\
		return (retVal);								\
	}													\
}

// Same as above, execpt that the return value is the status itself.
//
#define CHECK_MSTATUS_AND_RETURN_IT(x)					\
CHECK_MSTATUS_AND_RETURN((x), (x))

// Output an error message if status is not kSuccess.
//
#define CHECK_MSTATUS(x)								\
{														\
	MStatus _maya_status = (x);							\
	if ( MStatus::kSuccess != _maya_status ) 			\
	{													\
		cerr << "\nAPI error detected in " << __FILE__	\
			 <<	" at line "	<< __LINE__ << endl;		\
		_maya_status.perror ( "" ); 					\
	}													\
}

//
#define MFAIL(x)	( MStatus::kSuccess != (x) )


inline bool MStatus::operator==( const MStatus& rhs ) const
{
	return ( fStatusCode == rhs.fStatusCode );
}

inline bool MStatus::operator==( const MStatus::MStatusCode rhs ) const
{
	return ( fStatusCode == rhs );
}

inline bool MStatus::operator!=( const MStatus& rhs ) const
{
	return ( fStatusCode != rhs.fStatusCode );
}

inline bool MStatus::operator!=( const MStatus::MStatusCode rhs ) const
{
	return ( fStatusCode != rhs );
}

inline MStatus::operator bool() const
{ 
	return fStatus;
} 

inline bool MStatus::error() const
{
	return !fStatus;
}

inline MStatus::MStatusCode MStatus::statusCode() const
{
	return ( MStatus::MStatusCode ) fStatusCode;
}

inline void MStatus::set( bool status,
				 unsigned char theStatusCode,
				 unsigned char internalStatusCode)
{
	fStatus = status;
	fStatusCode = theStatusCode; 
	fInternalStatusCode = internalStatusCode; 
}

inline void MStatus::setSuccess()
{
	fStatus = true;
	fStatusCode = MS::kSuccess; 
	fInternalStatusCode = 0xff; 
}

inline bool operator==( const MStatus::MStatusCode code, 
						const MStatus& status ) 
{
	return ( status.fStatusCode == code );
}

inline bool operator!=( const MStatus::MStatusCode code, 
						const MStatus& status )
{
	return ( status.fStatusCode != code );
}

#endif /* __cplusplus */
#endif /* _MStatus */
