#ifndef _MTimer
#define _MTimer
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
// CLASS:    MTimer
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MTimer)
//
// The MTimer class works similar to timerX MEL command.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MTimer)

/// This class works similar to timerX MEL command.
/**
  Methods for calculating the time elapsed in executing a particular portion of the code.
*/
class OPENMAYA_EXPORT MTimer  
{

public:
	///
					MTimer();
	///
					MTimer( const MTimer & );
	///
					~MTimer();

	///
					void beginTimer();
	///
					void endTimer();
	///
					double elapsedTime();
	///
	MTimer&			operator =  ( const MTimer& rhs );
	///
	bool			operator == ( const MTimer& rhs ) const;
	///
	bool			operator != ( const MTimer& rhs ) const;
	///
	void			clear();

private:
	double				getCurrentTime();

	double				startTime;
	double				endTime;

};

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MTimer */
