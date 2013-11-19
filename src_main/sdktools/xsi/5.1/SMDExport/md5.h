/******************************************************************************

	File supervisor: jfclouti
	Date: 2000/04/19

	(c) Copyright 1996 - 2000 Avid Corporation

	@doc	EXPORTED | PROMOTED | SDKUTILS_API

	@module SDKUTILS md5.h |
			This file contains the APIs for interacting with the MD5
			algorithm.

			The MD5 Algorithm produces a small amount of data based on a hash of some large input data.
			It is very unlikely for two different input datas to produce the same value.
			
			An example use: remember the MD5 results on a file.  When you want to test if the file has changed
			at all run the MD5 algorithm again and compare the results.  Because you only have to remember
			the MD5 structure you have saved a lot of disk space or memory.
	
			The MD5 algorithm can also be used to generated GUIDs based on a string.
			For any two input strings the chance that they will produce the same GUID is extremely small.


	@head3 MD5 Functions: |
	@index func | MD5 & SDKUTILS_API

******************************************************************************/

#ifndef MD5_H
#define MD5_H

#include <sdkutils.h>
#include <SIBCString.h>
/*---------------------------------------------------------------------------
	GLOBAL.H
  ---------------------------------------------------------------------------*/

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

#ifdef __cplusplus
#define PROTOTYPES 1
#else
#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif
#endif

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
  returns an empty list.
 */
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif


/*---------------------------------------------------------------------------
	MD5.H
  ---------------------------------------------------------------------------*/


/* MD5 context. */
typedef struct
{
	UINT4 state[4];                                   /* state (ABCD) */
	UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;



// Recommended way to generate a UNIQUE but predicatable GUID from 
// a string.  
//
// The Namespace is a GUID that you use to SEED the result
// This prevents any possible collision with other users of the MD5
// algorithm.  For each particular namespace you would always provide the
// same GUID as the second argument.
//
// Normally you should send "false" to the third argument so that
// the md5 is run on a lowercase version of the string.  It depends on
// the semantics of your namespace.
//
// REVIEW: The algorithm works on a simple chars because wchar_t has a
// different size on linux and the results would be different.  IF necessary
// to support Unicode this functionality could be added in a safe fashion.

void MD5_GuidFromStringInNamespace
(
	const char* in_szString, 
	const GUID& in_namespace, 
	bool in_bCaseSensitive, 
	GUID& out_rguid
);

// Another variation, taking a string instead of a guid for the
// namespace.  It is expected that normally this string would
// contain a GUID.

void MD5_GuidFromStringInNamespace
(
	const char* in_szString, 
	const char* in_namespace, 
	bool in_bCaseSensitive, 
	GUID& out_rguid
);


// Generate a GUID from a string, with no namespace support.
// The results vary according to the case of the string
// It is recommended to use MD5_GuidFromStringInNamespace instead
void MD5_String(const char* in_szString, GUID& out_rguid);


// Low level API
void MD5Init PROTO_LIST ((MD5_CTX *));
void MD5Update PROTO_LIST ((MD5_CTX *, unsigned char *, unsigned int));
void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));
void MD5_Buffer(const void* in_pBuffer, unsigned long in_ulNbBytes, GUID& out_rguid);


#endif /* MD5_H */