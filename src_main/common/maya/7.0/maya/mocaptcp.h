/*-
 ==========================================================================
Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
rights reserved.

The coded instructions, statements, computer programs, and/or related 
material (collectively the "Data") in these files contain unpublished 
information proprietary to Alias Systems Corp. ("Alias") and/or its 
licensors, which is protected by Canadian and US federal copyright law and 
by international treaties.

The Data may not be disclosed or distributed to third parties or be copied 
or duplicated, in whole or in part, without the prior written consent of 
Alias.

THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
 ==========================================================================
*/

/*
 * captcp.h -	Definitions for handling serial lines for motion
 *		capture servers for Maya
 */

#if !defined(__CAPTCP_H__)
#define __CAPTCP_H__ 1

/*
 * Figure out if we should use prototypes or not
 */

#if !defined(_PROTO)
#if defined(PROTOTYPES) || defined(FUNCPROTO) || defined(__STDC__) || defined(__EXTENSIONS__) || defined(__cplusplus)
#define _PROTO(x)	x
#else
#define _PROTO(x)	()
#endif
#endif //(_PROTO)

/*
 * Capture library TCP/IP routines function declarations/prototypes
 */

/*
 * Open a connection to a socket on a host and tcp port.
 */
int CapTcpOpen _PROTO((char *name));

#endif /* __CAPTCP_H__ */
