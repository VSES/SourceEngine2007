
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
 * mocapserial.h - Definitions for handling serial lines for motion
 *		 capture servers for Maya
 */

#if !defined(__CAPSERIAL_H__)
#define __CAPSERIAL_H__ 1

#ifndef _WIN32
#include <termios.h>
#else
#define termios _DCB
#endif

/*
 * Enumerated types
 */

typedef enum CapParity
{
  CAP_PARITY_EVEN,
  CAP_PARITY_ODD,
  CAP_PARITY_NONE,
  _CAP_PARITY_LAST
} CapParity;

typedef enum CapSerialMode
{
  CAP_SERIAL_ASCII,
  CAP_SERIAL_BINARY,
  _CAP_MODE_LAST
} CapSerialMode;

/*
 * Figure out if we should use prototypes or not
 */

#if !defined(_PROTO)
#if defined(PROTOTYPES) || defined(FUNCPROTO) || defined(__STDC__) || defined(__EXTENSIONS__) || defined(__cplusplus)
#define _PROTO(x)	x
#else
#define _PROTO(x)	()
#endif
#endif // !defined(_PROTO)

/*
 * Capture library serial routines function declarations/prototypes
 */

/*
 * Open a serial port and return a file descriptor
 */
int	CapSerialOpen _PROTO((char *tty, int baud, int data, CapParity parity,
			  int stop, CapSerialMode mode,
			  struct termios *save_attr));

/*
 * Reset a serial port to its default attributes
 */
int	CapSerialReset _PROTO((int tty_fd, struct termios *attr));

/*
 * Read n bytes (even from a serial line)
 */
int	CapSerialRead _PROTO((int tty_fd, void *buf, size_t nbytes));

/*
 * Read n bytes (even from a serial line) with a possible timeout
 */
int	CapSerialReadTimeout _PROTO((int tty_fd, void *buf,
                                        size_t nbytes, int msec));

/*
 * Write n bytes (even from a serial line)
 */
int	CapSerialWrite _PROTO((int tty_fd, void *buf, size_t nbytes));

#endif /* __CAPSERIAL_H__ */
