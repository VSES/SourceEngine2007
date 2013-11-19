//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCMath.h | Main header file for Math implementation
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

//***************************************************************************************
// Defines
//***************************************************************************************
#ifndef __CSIBC_MATH_H__
#define __CSIBC_MATH_H__


#ifndef XSICOREEXPORT
# if ((defined _WIN32) || (defined WIN32)) && !(defined _WIN32_WCE) && !(defined _XBOX)
#  ifdef XSICORE_IMPL
#   define XSICOREEXPORT __declspec(dllexport)
#  elif defined(XSICORE_STATIC)
#   define XSICOREEXPORT
#  else
#   define XSICOREEXPORT __declspec(dllimport)
#  endif
# else
#  define XSICOREEXPORT
# endif
#endif

//***************************************************************************************
// Includes
//***************************************************************************************
#include <math.h>
#include <float.h>

/****************************************/
/*         GLOBAL CONSTANTS             */
/****************************************/

/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	

#define SI_DODECA_EPS	1.0e-12			/*!< Twelve-digit precision epsilon value. Used for floating-point comparisons. */
#define SI_FLOAT_MIN	FLT_MIN			/*!< Minimum representable floating point value. (Taken from <float.h>) */
#define SI_FLOAT_MAX	FLT_MAX			/*!< Maximum representable floating point value. (Taken from <float.h>) */
#define SI_TWO_PI   	( 2 * M_PI )	/*!< The value of 2*pi. */
#define SI_QUADRA_EPS 	0.0001			/*!< Four-digit precision epsilon value. Used for floating-point comparisons. */
#define SI_PROJ_EPS1	0.0002			/*!< Safe volume epsilon (outer sphere) */
#define SI_PROJ_EPS2	0.001			/*!< Safe volume epsilon (inner sphere) */
#define SI_EPSILON      0.0001			/*!< Floating point comparison value with zero. (Equivalent to #SI_QUADRA_EPS). */
#define SI_PENTA_EPS	1.0e-05			/*!< Five-digit precision epsilon value. Used for floating-pont comparisons. */
#define SI_HEXA_EPS  	1.0e-6			/*!< Six-digit precision epsilon value. Used for floating-pont comparisons. */
/*! @} */

// The follow are not used, and should be removed in future versions.
#define SI_LENGTH_EPS  	0.01
#define SI_VOLUME_EPS	5.0e-3
#define SI_PREC			1e-6
#define SI_IK_EPS2      0.0008
#define SI_IK_EPS1		0.00001
#define SI_PROT_EPS     0.05

/****************************************/
/*         GLOBAL CONSTANTS             */
/****************************************/
/* Some useful constants - These Match SGI's constants*/
/*  As defined in <math.h>  - these aren't defined in */
/*  NT's <math.h>									  */
/******************************************************/

#ifndef	M_E

/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	

#define M_E			2.7182818284590452354	/*!< Value of 'e'. 'e' is defined as lim(x->inf)[(1 + 1/x)^x]. */
#define M_LOG2E		1.4426950408889634074	/*!< The base 2 logarithm of 'e'. */
#define M_LOG10E	0.43429448190325182765	/*!< The base 10 logarithm of 'e'. */
#define M_LN2		0.69314718055994530942	/*!< The natural logarithm (ln) of 2. */
#define M_PI_4		0.78539816339744830962	/*!< The value of 'pi'/4. */
#define M_1_PI		0.31830988618379067154	/*!< The value of 1/'pi'. */
#define M_2_PI		0.63661977236758134308	/*!< The value of 2/'pi'. */
#define M_SQRT1_2	0.70710678118654752440	/*!< The square-root of 1/2. */
/*! @} */

#endif

/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	
#define M_LN10		2.30258509299404568402	/*!< The natural logarithm (ln) of 10. */
/*! @} */

#ifndef M_PI
/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	
#define M_PI		3.14159265358979323846	/*!< The value of 'pi'. */
/*! @} */
#endif

/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	
#define M_PI_2		1.57079632679489661923	/*!< The value of 'pi'/2. */
#define M_2_SQRTPI	1.12837916709551257390	/*!< The value of 2/sqrt('pi'). */
#define M_SQRT2		1.41421356237309504880	/*!< The value of sqrt(2). */
#define MAXINT		INT_MAX					/*!< The maximum representable integer. */
/*! @} */

/****************************************/
/*          LOCAL CONSTANTS             */
/****************************************/

// These are used internally by InvertParamCubic, and thus do not need documentation.
#define _2PI	(2.0 * M_PI)
#define _1_2PI	(0.5 * M_1_PI)
#define INVERTPARAMCUBIC_TOL		1.0e-09
#define INVERTPARAMCUBIC_SMALLERTOL 1.0e-20
#define INVERTPARAMCUBIC_MAXIT		100


/****************************************/
/*      LOCAL FUNCTION PROTOTYPES       */
/****************************************/

/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/

/*! Returns difference between two angles (between 0 and 'pi')
	\param	angle1	The first angle (in radians).
	\param	angle2	The second angle (in radians).
	\return	float	The angle between the two angles (in radians).
*/
XSICOREEXPORT float angleDistance ( double angle1, double angle2 );	// 

/*! Returns the angle based on the value of the sine and cosine values given (between -'pi' and 'pi).
	\param	sinus	The sine value of the angle.
	\param	cosinus	The cosine value of the angle.
	\return	float	The value of the angle (in radians).
*/
XSICOREEXPORT float arcSinCos ( double sinus, double cosinus );

/*! Returns the inverse of the 3rd-order Bernstein polynomial, at value \p Param.
	\param	Param	Value to at which the inverse is taken.
	\param	x0		0th order Berstein polynomial coefficient.
	\param	x1		1st order Berstein polynomial coefficient.
	\param	x2		2nd order Berstein polynomial coefficient.
	\param	x3		3rd order Berstein polynomial coefficient.
	\return	float	Inverse value of the Berstein polynomial at value \p Param.
*/
XSICOREEXPORT float InvertParamCubic ( float Param, float x0, float x1, float x2, float x3 );

/*! Limits a value to a specified range.
	\param	x	The value to limit.
	\param	a	The lower limit of the range.
	\param	b	The upper limit of the range.
*/
#define _SI_LIMIT( x, a, b ) if (x < a) x = a; if (x > b) x = b;
/*! @} */

#endif // 
