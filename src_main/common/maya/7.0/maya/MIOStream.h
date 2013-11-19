#ifndef _MIOStream
#define _MIOStream
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
// CLASS:    MIOStream
//
// *****************************************************************************

///
/**
	The MIOStream.h header file was created to centralize the including of the
	iostream header.  You can define REQUIRE_IOSTREAM to
	allow "iostream" to be included rather than "iostream.h".  
	This option is provided as a convenience for external developers.
*/

#include <maya/MIOStreamFwd.h>

#if defined(REQUIRE_IOSTREAM) || defined (AW_NEW_IOSTREAMS) || (_MSC_VER >= 1300)
	#include <iostream>

	using std::cout;
	using std::cin;
	using std::cerr;
	using std::clog;

	using std::endl;
	using std::flush;
	using std::ws;

	using std::streampos;

	using std::iostream;
	using std::ostream;
	using std::istream;
	using std::ios;
	

#else
	#include <iostream.h>
#endif

#endif // _MIOStream
