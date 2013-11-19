//*****************************************************************************
/*!
   \file xsi_time.h
   \brief CTime and CTimeArray class declarations.

   © Copyright 1998-2002 Avid Technology, Inc. and its licensors. All rights
   reserved. This file contains confidential and proprietary information of
   Avid Technology, Inc., and is subject to the terms of the SOFTIMAGE|XSI
   end user license agreement (or EULA).
*/
//*****************************************************************************

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

#ifndef __XSITIME_H__
#define __XSITIME_H__


#include "sicppsdk.h"
#include "xsi_string.h"

namespace XSI {


//*****************************************************************************
/*! \class CTime xsi_time.h
	\brief The CTime class is a utility class for manipulating time.

	Creating a time object using the default constructor can be used to get the
	current time. The CTime class can also be used to convert from one time format to another.

  	\eg
	\code
		using namespace XSI;

		// Set time to frame 30 using the default frame rate
		CTime time( 30 );

		// Get time in seconds
		double secs = time.GetTime( CTime::Seconds );

	\endcode

 */
//*****************************************************************************

class SICPPSDKDECL CTime
{
public:
	//! The time format defines the number of frames per second.
	enum Format
	{
		NTSC,		/*!< NTSC time format (29.97fps) */
		PAL,		/*!< PAL time format (25fps) */
		FILM,		/*!< FILM time format (24fps) */
		CUSTOM		/*!< User defined time format. */
	};

	//! Time units are used to determine how to interpret the time.
	enum Unit
	{
		Frames,		 /*!< time in frames */
		Hours,		 /*!< time in hours or units of 3600 seconds  */
		Minutes,	 /*!< time in minutes or units of 60 seconds  */
		Seconds,	 /*!< time in seconds */
		Milliseconds /*!< time in milliseconds or units of 1/1000 of a second */
	};

	/*! Constructs a time object with the current time and current time format specified by
	the user preferences for the default frame rate in the Time preferences.
	*/
	CTime();

	/*! Constructs a time object with a specified time value in the time format specified by
	the user preferences for the default frame rate in the Time preferences.
	\param in_time time value
	*/
	CTime(double in_time);

	/*! Constructs a time object with a specified time value and time format.
	\param in_time time value
	\param in_format time format
	\param in_customrate frames per seconds for custom frame rate format
	*/
	CTime(double in_time, Format in_format, double in_customrate = 1.0);

	/*! Copy constructor for time object.
	\param in_time time to copy
	*/
	CTime(const CTime& in_time);

	/*! Destructor
	*/
	~CTime();

	/*! Returns the time in the specified units. By default time is returned in frames.
	\return The time in the specified units.
	*/
	double GetTime(Unit in_units = CTime::Frames ) const;

	/*! Sets the time in the specified units.
	\param in_time time
	\param units units
	*/
	void PutTime(double in_time, CTime::Unit units = CTime::Frames );

	/*! Returns the time format
	\return Time format as one of the CTime::Format enum values.
	*/
	CTime::Format GetFormat(void) const;

	/*! Returns the frame rate in seconds for the given time format
	\return The frame rate in seconds as a double.
	*/
	double GetFrameRate(void) const;

	/*! Sets the time format
	\param in_format in_format
	\param in_customrate frames per seconds for custom frame rate format
	*/
	void PutFormat(CTime::Format in_format, double in_customrate = 1.0 );

	/*! Returns the time as a string using SMPTE format. SMPTE, which stands for the Society of Motion Picture
	and Television Engineers, is a method of displaying time. There are two ways that SMPTE can be displayed,
	non-drop or drop frame:

	\par SMPTE non-drop
		Represented by the display; hours, minutes, seconds and frame number, or \c 0:00:00:00.

	\par SMPTE drop frame
		Represented by the display; hours, minutes, seconds and frame number, or \c 0:00:00;00.

	Custom frame rate formats are returned as a string representation of the time as a double precision floating
	point number to 0 decimal places.
	\return The time as a string (SMPTE).
	*/
	CString GetAsText(void) const;

	/*! Sets the time from a string formated using the SMPTE format. SMPTE, which stands for the Society of Motion
	Picture and Television Engineers, is a method of displaying time. There are two ways that SMPTE can be displayed,
	non-drop or drop frame:

	\par SMPTE non-drop
		Represented by the display; hours, minutes, seconds and frame number, or \c 0:00:00:00.

	\par SMPTE drop frame
		Represented by the display; hours, minutes, seconds and frame number, or \c 0:00:00;00.

	If the time is specified using a custom format then the time is expected as a double precision floating point
	number to 0 decimal places.
	\param in_str Text representation of the time
	*/
	void SetAsText(const CString& in_str);

	/*! Assignment operator (sets the CTime object with a given time).
	\param in_time new time
	\return The CTime object.
	*/
	CTime& operator= ( const CTime& in_time );

	/*! Assignment operator (sets the CTime object with a given time specified in frames).
	\param in_time new time in seconds.
	\return The CTime object.
	*/
	CTime& operator= ( double in_time );

	/*! Equality operator (comparison to another CTime object).
	\param rhs time object
	\return bool
	*/
	bool  operator== ( const CTime& rhs ) const;

	/*! Equality operator (comparison to a CTime::Time value).
	\param rhs time Time
	\return bool
	*/
	bool  operator== ( const double rhs ) const;

	/*! Inequality operator (comparison to another CTime object).
	\param rhs time object
	\return bool
	*/
	bool  operator!= ( const CTime& rhs ) const;

	/*! Inequality operator (comparison to a CTime::Time value).
	\param rhs time Time
	\return bool
	*/
	bool  operator!= ( const double rhs ) const;

	/*! Cast operator (returns the time in frames).
	\return The time in frames as a double.
	*/
    operator double() const ;

private:
	void* m_pvoid;
};

/*! Equality operator (takes a CTime::Format on the left side and a CTime object on the right side).
\param lhs time format
\param rhs time object
\return bool
*/
SICPPSDKDECL bool  operator== ( const double lhs, const CTime& rhs);

/*! Inequality operator (takes a CTime::Format on the left side and a CTime object on the right side).
\param lhs time format
\param rhs time object
\return bool
*/
SICPPSDKDECL bool  operator!= ( const double lhs, const CTime& rhs);

//*************************************************************************
/*! \class CTimeArray xsi_time.h
	\brief The CTimeArray is an uni-dimensional array of CTime.

	The array is zero-based, which means that the first item index is 0 and the larger index
	is one less that the number of elements contained in the array.

	\sa CTime, FCurve::SetKeys
*/
//*************************************************************************
class SICPPSDKDECL CTimeArray
{
public:
	/*! Constructs a CTimeArray and optionally initializes the array to a known size.
	\param in_size Size of array, defaults to 0.
	*/
	CTimeArray( LONG in_size=0);

	/*! Constructs a CTimeArray object from another CTimeArray object.
	\param in_array constant CTimeArray object.
	*/
	CTimeArray(const CTimeArray& in_array);

	/*! Default destructor. */
	~CTimeArray();

	/*! Assignment operator.
	\param in_array constant class object.
	\return A reference to this array.
	 */
	CTimeArray& operator=( const CTimeArray& in_array );

	/*! Returns the number of items in this CTimeArray
	\return The number of items in the array.
	*/
	LONG GetCount() const;

	/*! Adds a CTime at the end of this array's.
	\param in_item New item to be added at the end of the array.
	\return CStatus::OK success
	*/
	CStatus Add( const CTime& in_item );

	/*! Erases all elements contained in the array.
	\return CStatus::OK success
	*/
	CStatus Clear();

	/*! Reallocates memory for the array, preserves its contents if new new size is larger than existing size.
	\param in_size New size of the array.
	\return CStatus::OK success
	\return CStatus::InvalidArgument in_size < 0
	*/
	CStatus Resize( LONG in_size);

	/*! Accessor to elements at a given index. This function can only be called by constant objects, the
	returned value is read-only.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero and
		smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A read-only reference to the indexed item.
	*/
	const CTime& operator[]( LONG in_index ) const;

	/*! Accessor to elements at a given index.
	\param in_index index in this zero-based array.  The index must be greater or equal to zero and
		smaller than the number of element in the array, otherwise the results are unpredictable.
	\return A reference to the indexed item.
	*/
	CTime& operator[]( LONG in_index );

	/*! Equality operator.
	\param in_array CTimeArray to compare with.
	\return true if in_array contains the same number of elements as this array and if all members
		of in_array are equal to the corresponding one contained in this array.
	*/
	bool operator==(const CTimeArray& in_array) const;

	/*! Inequality operator.
	\param in_array
	\return true if one member of in_array is different from the corresponding members in this
		array or if the arrays are not of the same size.
	*/
	bool operator!=(const CTimeArray& in_array) const;

private:

	void* m_ptr;
};


};

#endif // __XSITIME_H__
