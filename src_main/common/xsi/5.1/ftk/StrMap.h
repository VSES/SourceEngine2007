//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 2001-2002 Avid Technology, Inc. . All rights reserved.
//
//***************************************************************************************

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


#ifndef __STRMAP_H__
#define __STRMAP_H__

// Disable "identifier was truncated to '255' characters in the debug information" warning.
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_XBOX)
#pragma warning( disable : 4786 )
#endif // defined(_WIN32) || defined(_WIN32_WCE) || defined(_XBOX)

#ifdef FTK_KERNEL
#include <map>			// map
#endif // FTK_KERNEL

#include <SIBCUtil.h>	// SI_Char, _SI_STRCMP

/*! String less predicate function object
	This functor can be used in STL algorithms or assiciative containers to sort strings.
		
	Usage example:
	\code
	set<SI_Char*, StrLessPred> l_MySet;
	\endcode
*/
struct StrLessPred
{
	/*!	Determine if in_psz1 is less than in_psz2

		\param in_psz1 First string
		\param in_psz2 Second string
		\return true if in_psz1 is less than in_psz2
	*/
	inline bool operator () ( const SI_Char* in_psz1, const SI_Char* in_psz2 ) const
	{
		return (_SI_STRCMP(in_psz1, in_psz2) < 0);
	}
};

/*! Template specialization of an STL map using SI_Char* as the key type.

  \param ValueType The reference type
	
	Example:	
	\code	
	SI_StrMap<ValueType> l_MyMap;
	\endcode	  
		is equivalent to 
	\code
	map<SI_Char*,ValueType,StrLessPred> l_MyMap;
	\endcode
*/
#ifdef FTK_KERNEL
template <class ValueType>
class SI_StrMap : public std::map<const SI_Char*, ValueType, StrLessPred>
{
};

#endif // FTK_KERNEL
#endif //__STRMAP_H__
