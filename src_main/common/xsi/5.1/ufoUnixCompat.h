
/*******************************************************************************
 * "ufoUnixCompat.h"
 *
 * some function defines that are common under unix but not available under 
 * windows
 * 
 * Copyright  (c) Copyright 2002 Softimage/Avid Corporation
 *
 * -----------------------------------------------------------------------------
 * Media Illusion UFO (User Function Object) SDK
 * adapted for Softimage|XSI by Luc-Eric Rousseau (lucer), nov 2002
 ******************************************************************************/

#ifndef	__ufoUnixCompat
#define	__ufoUnixCompat


#if defined(_WIN32 ) && !defined( _NoUfoCompat )
	
	#include <stdlib.h>	// for rand
	#include <float.h>// for isnan

	#ifndef rint 
		#define	rint(x)	(floor(	x +	0.5	))
	#endif
	
	#ifndef M_PI 
		#define	M_PI 3.14159265358979323846
	#endif

	#ifndef drand48
		#define drand48()		( rand() / (double)RAND_MAX )
		#define srand48( seed )	( srand( seed ) )
	#endif


#endif

#ifndef random
	#define	random						unix_random
#endif

#ifndef srandom
	#define	srandom						unix_srandom
#endif

#ifndef initstate
	#define	initstate					unix_initstate
#endif

#if 0
#ifndef setstate
	#define	setstate					unix_setstate
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32)
	#ifdef UFOAPI_IMPL
		#define UNIXAPI		__declspec( dllexport ) 
	#else
		#define UNIXAPI		__declspec( dllimport ) 
	#endif
#else
	#define UNIXAPI
#endif

UNIXAPI	int  unix_random(void);
UNIXAPI	void unix_srandom(unsigned seed);
UNIXAPI	char *unix_initstate(unsigned int seed, char * state, size_t n );
UNIXAPI	char *unix_setstate(const char * state );

#ifdef __cplusplus
}
#endif



#endif /* __ufoUnixCompat */


/*******************************************************************************
 * End of file ufoUnixCompat.h
 ******************************************************************************/
