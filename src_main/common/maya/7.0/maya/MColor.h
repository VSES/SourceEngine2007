#ifndef _MColor
#define _MColor
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
// CLASS:    MColor
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MColor)
//
// This class provides a type used to get/set values of color attributes in
// dependency nodes.  It represents colors as an RGB triple.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MIOStreamFwd.h>

// *****************************************************************************

// CLASS DECLARATION (MColor)

/// A color math class
/**
    This class is used to store values of color-related dependency graph node
    attributes.
*/
#if defined(_WIN32)
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MColor  
{
public:
	///
	enum MColorType {
		///
		kRGB,
		///
		kHSV,
		///
		kCMY,
		///
		kCMYK
	};

	///
					MColor();
	///
					MColor( const MColor&);
	///
					MColor( float rr, float gg, float bb=0.0, float aa=1.0 );
	///
					MColor( const float[3] );

	///
					MColor( MColorType colorModel,
							unsigned char, unsigned char, unsigned char,
							unsigned char alpha=255U );
	///
					MColor( MColorType colorModel,
							unsigned short, unsigned short, unsigned short,
							unsigned short alpha=65535U );
	///
					MColor( MColorType colorModel, float, float, float,
							float alpha=1.0 );
	///
					MColor( MColorType colorModel, double, double, double,
							double alpha=1.0 );

	///
					~MColor();
	///
 	MColor&		    operator= ( const MColor& src );
	///
 	float&      	operator()( unsigned i );
	///
 	float   		operator()( unsigned i ) const;
	///
 	float&      	operator[]( unsigned i );
	///
	float			operator[]( unsigned i )const;
	///
 	MColor&   		operator/=( float scalar );
	///
 	MColor     	    operator/( float scalar ) const;
	///
 	MColor& 		operator*=( float scalar );
	///
 	MColor   		operator*( float scalar ) const;
	///
 	friend OPENMAYA_EXPORT MColor	operator*( float, const MColor&);
	///
 	MColor   		operator+( const MColor& other) const;
	///
	MColor&		    operator+=( const MColor& other );
	///
 	MColor   		operator-() const;
	///
 	MColor   		operator-( const MColor& other ) const;
	///
 	MColor          operator*( const MColor& other ) const;
	///
	MColor&         operator*=( const MColor& other );
	///
 	bool          	operator!=( const MColor& other ) const;
	///
 	bool           	operator==( const MColor& other ) const;
	///
	bool			get( float[3] ) const;
	///
	bool			get( MColorType colorModel, float&, float&, float& ) const;
	///
	bool			get( MColorType colorModel, float&, float&, float&, float& alpha ) const;
	///	
	bool			set( MColorType colorModel, float, float, float, float alpha = 1.0);

	///
	friend OPENMAYA_EXPORT IOS_REF(ostream)& operator<<(IOS_REF(ostream)& os, const MColor& c);

    /// the red component of the color
	float r;
    /// the green component of the color
	float g;
    /// the blue component of the color
	float b;
	/// the alpha component of the color
	float a;


protected:
// No protected members

private:
// No private members

};

#if defined(_WIN32)
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MColor */
