
#ifndef _MDistance
#define _MDistance
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
// CLASS:    MDistance
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDistance)
//
//	The MDistance class provides a fundamental type for the Maya API to hold
//	and manipulate linear data.  All API methods that require or return
//	linear information do so through variables of this type.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MDistance)

/// Manipulate Linear Data
/**
  Methods for setting and retreiving linear data in various unit systems 
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MDistance  
{
public:
	/// Available Unit Systems
	enum Unit
	{
		///
		kInvalid,
		/// Inches
		kInches,
		/// Feet
		kFeet,
		/// Yards
		kYards,
		/// Miles
		kMiles,
		/// Millimeters
		kMillimeters,
		/// Centimeters
		kCentimeters,
		/// Kilometers
		kKilometers,
		/// Meters
		kMeters,
		///
		kLast
	};
	///
					MDistance();
	///
					MDistance( double value, Unit unitSystem = kCentimeters );
	///
					MDistance( const MDistance& src );
	///
					~MDistance();  
	///
	MDistance&		operator=( const MDistance& src ); 
	///
	Unit			unit() const;
	///
	double			value() const;
	///
	MStatus			setUnit( Unit newUnit );
	///
	MStatus			setValue( double newValue );
	///
	double			as( Unit newUnit, MStatus *ReturnStatus = NULL ) const;
	///
	double			asInches() const;
	///
	double			asFeet() const;  
	///
	double			asYards() const;  
	///
	double			asMiles() const;  
	///
	double			asMillimeters() const;
	///
	double			asCentimeters() const;  
	///
	double			asKilometers() const;  
	///
	double			asMeters()  const;  
	///
	static Unit		uiUnit();
	///
	static MStatus	setUIUnit( Unit newUnit );
	///
	static Unit		internalUnit();
	///
	static double	internalToUI( double internalValue );
	///
	static double	uiToInternal( double uiValue );
	///OBSOLETE Changing the internal units is generally not a good idea and using this method may often produce unexpected results.
	static MStatus  setInternalUnit( Unit internalUnit );

protected:
	double			val;
	Unit			valUnit;

private:

	static const char* className();

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDistance */
