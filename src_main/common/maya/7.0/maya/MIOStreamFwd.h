#ifndef _MIOStreamFwd
#define _MIOStreamFwd
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
// CLASS:    MIOStreamFwd
//
// *****************************************************************************

///
/**
	The MIOStreamFwd.h header file was created to centralize the
	including of the iosfwd header.  You can define REQUIRE_IOSTREAM
	to allow "iostream" to be included rather than "iostream.h".
	iosfwd is a much smaller header than iostream, so speeds compile
	times for cases where you only need to predeclare these
	types. This option is provided as a convenience for external
	developers.
*/

#if defined(REQUIRE_IOSTREAM) || defined (AW_NEW_IOSTREAMS)
	# include <iosfwd>

	using std::streampos;

	using std::iostream;
	using std::ostream;
	using std::istream;
	using std::ios;
	#define IOS_REF(iosref) std::iosref
	

#else
	// For non-template implementation of the old, compatibility
	// iostreams. "ostream" is a class and can just be forward
	// declared.
	class ostream;
	class istream;
	class iostream;
	class ostringstream;
	class istringstream;
	class ofstream;
	class ifstream;
	class fstream;
	#define IOS_REF(iosref) iosref
#endif

#endif // _MIOStreamFwd
