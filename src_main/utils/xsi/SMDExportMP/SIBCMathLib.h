//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCMathLib.h | Main header file for MathLib implementation
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

#if (_MSC_VER > 1000) || defined(SGI_COMPILER)
#pragma once
#endif

//******************************************************************************
// Defines
//******************************************************************************

#ifndef _SIBCMATHLIB_H_
#define _SIBCMATHLIB_H_

//******************************************************************************
// Includes
//******************************************************************************
#include <math.h>		// for DBL_MAX
#include <float.h>
#include "SIBCUtil.h"


/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	

/*! Used to identify a given main axis. */
typedef enum tagE3DAXISTYPE
{
   X_AXIS = 0,	/*!< Main axis if the x-axis. */
   Y_AXIS = 1,	/*!< Main axis if the y-axis. */
   Z_AXIS = 2	/*!< Main axis if the z-axis. */
} E3DAxisType;

/*! Helper mask to pack which coordinates are valid, used, not used, etc. */

enum {	
	NO_COORD_AXIS	= 0x00,											/*!< No coordinates. */
	X_COORD_AXIS	= 0x01,											/*!< X-axis coordinates */
	Y_COORD_AXIS	= 0x02,											/*!< Y-axis coordinates */
	Z_COORD_AXIS	= 0x04,											/*!< Z-axis coordinates */
	ALL_COORD_AXES	= (X_COORD_AXIS | Y_COORD_AXIS | Z_COORD_AXIS)	/*!< All coordinates */
} ;

typedef double DOUBLE;				/*!< DOUBLE is an equivalent to double */

const DOUBLE PICO_EPS	= 1.0e-12;	/*!< The value used to compare floating-point numbers within pico-units (1E-12). */
const DOUBLE NANO_EPS	= 1.0e-09;	/*!< The value used to compare floating-point numbers within nano-units (1E-9). */
const DOUBLE HEXA_EPS	= 1.0e-06;	/*!< The value used to compare floating-point numbers within micro-units (1E-6). */
const DOUBLE MILLI_EPS	= 1.0e-03;	/*!< The value used to compare floating-point numbers within milli-units (1E-3). */
const DOUBLE SI_HUGE	= DBL_MAX;	/*!< The maximum representable double precision floating-point number (taken from <float.h>). */
/*! @} */

#define M_PI  	3.14159265358979323846	// already documented in SIBCMath.h
#define M_E		2.7182818284590452354	// already documented in SIBCMath.h

/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	
#define M_2PI	         ( 2 * M_PI )	/*!< The value of 2*'pi' */
#define M_Inv_PI         ( 1.0 / M_PI )	/*!< The value of 1/'pi' (equivalent to #M_1_PI). */
#define M_Inv_2PI        ( 0.5 / M_PI ) /*!< The value of 1/(2*'pi'). */

/*! Converts an angle in degrees to a cooresponding angle in radians.
	\param	deg	The angle in degrees.
	\return	The angle in radians.
	\sa #RAD_TO_DEG
*/
#define DEG_TO_RAD( deg ) ((deg)*M_PI/180.0)

/*! Converts an angle in radians to a cooresponding angle in degrees.
	\param	rad The angle in radians.
	\return	The angle in degrees.
	\sa #DEG_TO_RAD
*/
#define RAD_TO_DEG( rad ) ((rad)*180.0*M_Inv_PI)
/*! @} */

// Old defines from S35MthConsts.h (these are not documented).
#define SI_DEGREES_PER_RAD	(180.0/M_PI)
#define SI_RAD_PER_DEGREES	(M_PI/180.0)
#define _SI_RAD2DEG( x )	( (x) * SI_DEGREES_PER_RAD )
#define _SI_DEG2RAD( x )	( (x) * SI_RAD_PER_DEGREES )

/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	
#ifndef NULL
/*! NULL is defined as zero. */
#define NULL 0	
#endif
/*! @} */

class CSIBCVector2Dd;
class CSIBCVector3Dd;
class CSIBCVector4Dd;
class CSIBCVectorwd;
class CSIBCMatrix33d;
class CSIBCMatrix44d;
class CSIBCMatrixMNd;
class CSIBCRotMatd;
class CSIBCXfoMatd;
class C3DQuat;
class CSIBCRotationd;
class CSIBCTransfod;

#include "SIBCMatrix33d.h"
#include "SIBCRotMatd.h"
#include "SIBCMatrix44d.h"
#include "SIBCXfoMatd.h"
#include "SIBCVector2Dd.h"
#include "SIBCVector3Dd.h"
#include "SIBCVector4Dd.h"
#include "SIBCVectorwd.h"
#include "SIBCQuaterniond.h"
#include "SIBCRotationd.h"
#include "SIBCTransfod.h"
#include "SIBCMatrixMNd.h"

#endif
