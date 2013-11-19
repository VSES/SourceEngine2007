//***************************************************************************************
//
// File supervisor: Softimage 3D Games & 3D Bridge team
//
// (c) Copyright 1999-2002 Avid Technology, Inc. . All rights reserved.
//
// SIBCUtil.h | Main header file for SIBCUtil implementation
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

//**************************************************************************************
// Defines
//**************************************************************************************
#ifndef __CSIBCUtil_H__
#define __CSIBCUtil_H__


#ifdef __APPLE__
  #include <math.h>
#endif

#include "SIBCMath.h"
#ifdef _WIN32
#ifndef	_XBOX
#include <windows.h>
#include <winuser.h>
#else
#include <xtl.h>
#endif
#endif

//**************************************************************************************
// Includes
//**************************************************************************************
#ifndef _WIN32_WCE
#include <assert.h>
#else
#endif


//**************************************************************************************
// Typedefs
//**************************************************************************************
#ifndef _CUS_DLL
	/*!	\addtogroup	siError_group	FTK error codes	
		Grouping of all FTK error codes. If a value in the eSI_Error enumeration
		does not have a description, it indicates that this value is never returned
		from any function from within the FTK.
		@{ */

	/** Enumeration for error codes, used with SI_Error. Note that most of these error
		codes are unused in the FTK/SL. They are taken as a subset of XSI errors, and
		used for compliance reasons.
	*/
typedef enum {


	SI_SUCCESS		=	0,		//!< Indicates successful operation of the function.

	// general errors //
	SI_ERR_NONE		=	0,		//!< Equivalent to SI_SUCCESS.
	SI_ERR_STATUS_MSG,			//!< Unused
	SI_ERR_MESSAGE_MSG,			//!< Unused
	SI_ERR_WARNING_MSG,			//!< Unused
	SI_ERR_ERROR_MSG,			//!< The function produced a general error. 
	SI_ERR_MODE_MSG,			//!< Unused
	SI_ERR_INTERNAL,			//!< Internal to the FTK. 
	SI_ERR_NOTYETIMPLEMENTED,	//!< The function has not been implemented. 
	SI_ERR_NOTSUPPORTED,		//!< The function is not supported. 

	// argument check //
	SI_ERR_BAD_MDLTYPE,		//!< Unused
	SI_ERR_BAD_CHAPID,		//!< Unused
	SI_ERR_BAD_GROUPID,		//!< Unused
	SI_ERR_BAD_GROUPLIST,	//!< Unused
	SI_ERR_BAD_ELEMENT,		//!< An element was invalid.
	SI_ERR_BAD_DICTIONARY,	//!< Unused
	SI_ERR_BAD_RELID,		//!< Unused
	SI_ERR_BAD_ARGUMENT,	//!< An argument supplied to the function is invalid.
	SI_ERR_BAD_DATABASE,	//!< Unused
	SI_ERR_BAD_RELNDX,		//!< Unused
	SI_ERR_BAD_MSMSH,		//!< Unused
	SI_ERR_BAD_MFACE,		//!< Unused
	SI_ERR_BAD_MPTCH,		//!< Unused
	SI_ERR_BAD_MSPLN,		//!< Unused
	SI_ERR_BAD_PARAM,		//!< Unused
	SI_ERR_BAD_NBPARAM,		//!< Unused
	SI_ERR_BAD_NOPARAM,		//!< Unused
	SI_ERR_BAD_CHP4REL,		//!< Unused
	SI_ERR_BAD_SHAPEID,		//!< Unused
	SI_ERR_CLS_NOMODEL,		//!< Unused
	SI_ERR_LAT_NOMODEL,		//!< Unused

	// relations & rel resources //
	SI_ERR_RELRSRC_CANTOPEN,	//!< Unused
	SI_ERR_RELRSRC_INVGRP,		//!< Unused
	SI_ERR_RELRSRC_INVCHP,		//!< Unused
	SI_ERR_RELPAIR_CANTADD,		//!< Unused
	SI_ERR_RELPAIR_CANTDEL,		//!< Unused
	SI_ERR_REL_CANTDEL,			//!< Unused

	// dictionary //
	SI_ERR_ELEM_NOTFOUND,	//!< The function could not find a required element.
	SI_ERR_NAME_DUPL,		//!< Unused
	SI_ERR_NAME_FORMAT,		//!< Unused

	// file access //
	SI_ERR_DB_NOTFOUND,		//!< Unused
	SI_ERR_DB_INVALID,		//!< Unused
	SI_ERR_DB_CANTCREATE,	//!< Unused
	SI_ERR_DB_CANTDELETE,	//!< Unused
	SI_ERR_DBDELETE,		//!< Unused
	SI_ERR_ACCESS_CANTSET,	//!< Unused
	SI_ERR_SCENE_CANTREAD,	//!< Function encountered an error reading a scene.
	SI_ERR_SCENE_CANTWRITE,	//!< Unused
	SI_ERR_SCENE_CANTDEL,	//!< Unused
	SI_ERR_ELEM_CANTREAD,	//!< Unused
	SI_ERR_ELEM_CANTWRITE,	//!< Unused
	SI_ERR_SAVE_NOTROOT,	//!< Unused
	SI_ERR_NOVERS_FOUND,	//!< Unused

	// Data access and Conversion //
	SI_ERR_NUM_VERTICES,		//!< Unused
	SI_ERR_SHAPE_VERTICES,		//!< Unused
	SI_ERR_TOO_FEW_VERTICES,	//!< Unused
	SI_ERR_TOO_MANY_VERTICES,	//!< Unused
	SI_ERR_NULL_TRANSFO,		//!< Unused
	SI_ERR_NULL_VECTOR,			//!< Unused
	SI_ERR_BAD_COORDSYS,		//!< Unused
	SI_ERR_BAD_VNDX,			//!< Unused
	SI_ERR_CHAPNOTFOUND,		//!< Unused
	SI_ERR_FCVNOTFOUND,			//!< Unused
	SI_ERR_NOT_TRIANGULATED,	//!< Unused

	// Update list and animation //
	SI_ERR_CYCLE_DETECTED,		//!< Unused

	// Dynalink library //
	SI_ERR_DYNALINK_NOT_LOADED,		//!< Unused
	SI_ERR_DYNALINK_NOT_RESOLVED,	//!< Unused
	SI_ERR_DYNALINK_NOT_UNLOADED,	//!< Unused

	// Custom Value //
	SI_ERR_CUSVAL_SYMBOL_NOT_FOUND,	//!< Unused
	SI_ERR_CUSVAL_INCORRECT_TYPE,	//!< Unused
	SI_ERR_CUSVAL_BUFFER_TOO_SMALL,	//!< Unused

	// SAAPHIRE stuff //
	SI_ERR_ALLOC_PROBLEM,		//!< Unused
	SI_ERR_INVALID_SCENE,		//!< Unused
	SI_ERR_ELEM_UNSCENE,		//!< Unused
	SI_ERR_ELEM_TYPE,			//!< Unused
	SI_ERR_ELEM_ROOT,			//!< Unused
	SI_ERR_WRONG_COUNT,			//!< Unused
	SI_ERR_INVALID_LTYPE,		//!< Unused
	SI_ERR_NO_RESET,			//!< Unused
	SI_ERR_INVALID_SELEM,		//!< Unused
	SI_ERR_BAD_MATERIAL,		//!< Unused
	SI_ERR_INVALID_TESSELATION,	//!< Unused
	SI_ERR_NON_TESSELATABLE,	//!< Unused
	SI_ERR_INVALID_MODEL_DATA,	//!< Unused
	SI_ERR_NO_MATERIAL,			//!< Unused
	SI_ERR_NO_TEXTURE,			//!< Unused
	SI_ERR_ELEM_NOT_LEAF_NODE,	//!< Unused
	SI_ERR_ELEM_CANTDEL,		//!< Unused
	SI_ERR_FCURVE_EXISTS,		//!< Unused
	SI_ERR_ELEM_CHNROOT,		//!< Unused
	SI_ERR_ELEM_EFFECTOR,		//!< Unused
	SI_ERR_GOAL_NOT_SET,		//!< Unused
	SI_ERR_USERDATA_EXISTS,		//!< Unused
	SI_ERR_INVALID_INDEX,		//!< Unused

	// Opensyst //
	SI_ERR_CUS_SECOND_SYMBOL,	//!< Unused
	SI_ERR_CUS_NO_SYMBOL_NAME,	//!< Unused

	// DSO's //
	SI_ERR_CUSTOM_FATAL,		//!< Unused

	// DLL's //
	SI_ERR_UNFOUND_GLBSYMB_TAB,	//!< Unused

	// Graphic //
	SI_ERR_STOP,		//!< Unused
	SI_ERR_TXT_NAME,	//!< Unused
	SI_ERR_NO_CAM,		//!< Unused

	// Reason why a transformation is not applicable to a model //
	SI_ERR_TRANSFO_METASYSTEM,		//!< Unused
	SI_ERR_TRANSFO_PATCHDEFORM,		//!< Unused
	SI_ERR_TRANSFO_SPLINEDEFORM,	//!< Unused
	SI_ERR_SCAL_CHAIN,				//!< Unused
	SI_ERR_SCAL_CNSSCL,				//!< Unused
	SI_ERR_ROT_JOINT,				//!< Unused
	SI_ERR_ROT_CHNROOT,				//!< Unused
	SI_ERR_ROT_CNSORI,				//!< Unused
	SI_ERR_TRANS_CHNROOT,			//!< Unused
	SI_ERR_TRANS_JOINT,				//!< Unused
	SI_ERR_TRANS_DYNLEAF,			//!< Unused
	SI_ERR_TRANS_MAINGLOBENV,		//!< Unused
	SI_ERR_TRANS_CONSTRAINT,		//!< Unused

	// Additional SAAPHIRE error codes //
	SI_ERR_SUBELEM_TOTAL_FAILURE,	//!< Unused
	SI_ERR_SOFTIMAGE_TOO_OLD,		//!< Unused
	SI_ERR_SAAPHIRE_TOO_OLD,		//!< Unused
	SI_ERR_SAA_TOO_OLD_FOR_EFFECT,	//!< Unused
	SI_ERR_FILE_EXISTS,				//!< Unused
	SI_ERR_UNRELATED_T2D,			//!< Unused
	SI_ERR_INCOMPATIBLE_T2DMAP,		//!< Unused
	SI_ERR_SHADER_EXISTS,			//!< Unused
	SI_ERR_INCOMPAT_MAP_METHOD,		//!< Unused
	SI_ERR_MODEL_NO_SHAPES,			//!< Unused
	SI_ERR_CLUSTER_NO_KEYS,			//!< Unused

	// DSO's (no status bar message) //
	SI_ERR_CUSTOM_FATAL_NOMSG,		//!< Unused
	SI_ERR_NO_FCV_EDITOR,			//!< Unused

	// Render errors //
	SI_ERR_RENDER_WARNING,			//!< Unused
	SI_ERR_RENDER_ERROR,			//!< Unused

	// physical property Error //
	SI_ERR_MODEL_NO_PHYSPROPS,		//!< Unused

	// dialog errors //
	SI_ERR_EFFECT_NODG,		//!< Unused
	SI_ERR_DG_NOTFOUND,		//!< Unused

	// more render errors //
	SI_ERR_RENDER_VOLUNTARYEXIT,	//!< Unused

	// Audio support //
	SI_ERR_AUDIO_DEVICEINUSE,	//!< Unused
	SI_ERR_AUDIO_INVALIDFORMAT,	//!< Unused
	SI_ERR_AUDIO_INACTIVE,		//!< Unused
	SI_ERR_FILENOTFOUND,		//!< Unused
	SI_ERR_AUDIO_INVALIDTRACK,	//!< Unused
	SI_ERR_FILEPASSEDEOF,		//!< Unused

	// Action Support //
	SI_ERR_NO_ACTION,				//!< Unused
	SI_ERR_MARKER_NOTFOUND,			//!< Unused
	SI_ERR_ACTIONMARKERS_OVERLAP,	//!< Unused

	// More audio errors //
	SI_ERR_AUDIO_FILENOTSET,		//!< Unused
	SI_ERR_AUDIO_FILEALREADYSET,	//!< Unused
	SI_ERR_AUDIO_MIXINGNOTENABLED,	//!< Unused

	// Licensing //
	SI_ERR_LIC_INVALID_HANDLE,	//!< Unused
	SI_ERR_LIC_INVALID_ARGS,	//!< Unused
	SI_ERR_LIC_CHECKOUT,		//!< Unused
	SI_ERR_NUM,					//!< Unused

	//XSI specific
	SI_FILE_NOT_FOUND,			//!< Unused
	SI_NO_AVAILABLE_DRIVER,		//!< Unused
	//Image Lib error
	SI_IMAGE_BAD_FORMAT		//!< Unused

} eSI_Error;

/*!	Error codes are represented by integers. The error code is an enumeration value from
	eSI_Error enumeration */
typedef int SI_Error;	

	/*! @} */

#else

//To be cross compatible with SAAphire
#include <saa.h>

#define SI_FILE_NOT_FOUND (SI_ERR_NUM + 1)
#define SI_NO_AVAILABLE_DRIVER (SI_FILE_NOT_FOUND + 1)
#endif

/*! \addtogroup mathdef_group Mathematical functions & constants.
	@{
*/	

#ifndef TRUE
#define TRUE		1		/*!< TRUE is defined as 1. Used in logical expressions. */
#endif

#ifndef FALSE
#define FALSE		0		/*!< FALSE is defined as 0. Used in logical expressions. */
#endif

#define SI_TRUE TRUE		/*!< SI_TRUE is defined as #TRUE. Used in logical expressions. */
#define SI_FALSE FALSE		/*!< SI_TRUE is defined as #FALSE. Used in logical expressions. */

/*! @}	*/

// Already documented in SIBCMathLib.h
#ifndef NULL
#define NULL 0
#endif

/*! \addtogroup sitype_group Softimage FTK type definitions.
	These defines show the mapping between built-in basic types, and the basic types
	used within the FTK.

	@{
*/

#define SI_Bool		unsigned short		/*!< SI_Bool is defined as 'unsigned short'. */
#define SI_Byte		char				/*!< SI_Byte is defined as 'char'. */
#define SI_UByte	unsigned char		/*!< SI_UByte is defined as 'unsigned char'. */
#define SI_Short	short				/*!< SI_Short is defines as 'short'. */
#define SI_UShort	unsigned short		/*!< SI_UShort is defines as 'unsigned short'. */
#define SI_Int		int					/*!< SI_Int is defines as 'int'. */
#define SI_UInt		unsigned int		/*!< SI_UInt is defines as 'unsigned int'. */
#define SI_Float	float				/*!< SI_Float is defines as 'float'. */
#define SI_Double   double				/*!< SI_Double is defines as 'double'. */
#define SI_Long		int					/*!< SI_Long is defines as 'long'. */
#define SI_Void		void				/*!< SI_Void is defines as 'void'. */
#define SI_ULong	unsigned int		/*!< SI_ULong is defines as 'unsigned long'. */
typedef SI_Float	SI_Matrix[4][4];	/*!< SI_Matrix is defines as a 4x4 array of SI_Float. */
/* @} */	// endgroup

// CustomPset Data Type String. These are not used in the FTK.
#define SI_CUSTOMPSET_PCHAR	"Text"
#define SI_CUSTOMPSET_BOOL	"Boolean"
#define SI_CUSTOMPSET_INT	"Integer"
#define SI_CUSTOMPSET_UBYTE	"Small Integer Number"
#define SI_CUSTOMPSET_DOUBLE	"Floating Point Number"


/*! \addtogroup vptypes_group Variant Parameter types
	These defines represent different types that the value contained within CdotXSIParam objects
	can take on. 

	@{
*/

#define SI_VT_BOOL      0	/*!< Parameter value is of SI_Bool type. */
#define SI_VT_BYTE      1	/*!< Parameter value is of SI_Byte type. */
#define SI_VT_UBYTE     2	/*!< Parameter value is of SI_UByte type. */
#define SI_VT_SHORT     3	/*!< Parameter value is of SI_Short type. */
#define SI_VT_USHORT    4	/*!< Parameter value is of SI_UShort type. */
#define SI_VT_INT       5	/*!< Parameter value is of SI_Int type. */
#define SI_VT_UINT      6	/*!< Parameter value is of SI_UInt type. */
#define SI_VT_FLOAT     7	/*!< Parameter value is of SI_Float type. */
#define SI_VT_DOUBLE    8	/*!< Parameter value is of SI_Double type. */
#define SI_VT_LONG      9	/*!< Parameter value is of SI_Long type. */
#define SI_VT_ULONG     10	/*!< Parameter value is of SI_ULong type. */

#define SI_VT_PBOOL     11	/*!< Parameter value is a pointer to an SI_Bool array. */
#define SI_VT_PBYTE     12	/*!< Parameter value is a pointer to an SI_Byte array. */
#define SI_VT_PUBYTE    13	/*!< Parameter value is a pointer to an SI_UByte array. */
#define SI_VT_PSHORT    14	/*!< Parameter value is a pointer to an SI_Short array. */
#define SI_VT_PUSHORT   15	/*!< Parameter value is a pointer to an SI_UShort array. */
#define SI_VT_PINT      16	/*!< Parameter value is a pointer to an SI_Int array. */
#define SI_VT_PUINT     17	/*!< Parameter value is a pointer to an SI_UInt array. */
#define SI_VT_PFLOAT    18	/*!< Parameter value is a pointer to an SI_Float array. */
#define SI_VT_PDOUBLE   19	/*!< Parameter value is a pointer to an SI_Double array. */
#define SI_VT_PLONG     20	/*!< Parameter value is a pointer to an SI_Long array. */
#define SI_VT_PULONG    21	/*!< Parameter value is a pointer to an SI_ULong array. */

#define SI_VT_PCHAR     22	/*!< Parameter value is a pointer to a char array. */
#define SI_VT_PPCHAR    23	/*!< Parameter value is a pointer to an array of char arrays. */

#define SI_VT_PVOID     24	/*!< Parameter value is a pointer to untyped data. */
/*! @} */	// endgroup.


/*! \addtogroup  vtwktypes_group Variant Parameter Tweak types
	These defines represent the different tweaking controls associated with modifying values
	within CdotXSIParam objects.

	@{
*/

#define SI_TT_NONE		0	/*!< No tweaking control is associated with the parameter. */
#define SI_TT_POSITION	1	/*!< The tweaking control is a position adjuster. */
#define SI_TT_NORMAL	2	/*!< The tweaking control is a default control for the data type. */
#define SI_TT_TEXCOORD	3	/*!< The tweaking control is a texture coordinate adjuster. */
#define SI_TT_COLOR		4	/*!< The tweaking control is a color chooser. */
#define SI_TT_TRANSFO	5	/*!< The tweaking control is a transformation adjuster. */
#define SI_TT_INTEGER	6	/*!< The tweaking control is an integer slider. */
#define SI_TT_FLOAT		7	/*!< The tweaking control is an floating-point slider. */
#define SI_TT_TINYFLOAT	8	/*!< The tweaking control is a tiny-floating-point slider. */
/*! @} */	// endgroup.

/*! \addtogroup	vptypes_group Variant Parameter types
	@{
*/

/*! This structure holds the type, number of elements, and the value of a
	CdotXSIParam object.
*/
struct TinyVariant {
    SI_Int          variantType;	/*!< This is the type of the value contained within this structure. (See @link vptypes_group Variant Parameter types@endlink). */
	SI_Int			tweakType;		/*!< This is the tweaking type for the structure. (See @link vtwktypes_group Variant Parameter Tweak types@endlink). */
	SI_Int			numElems;		/*!< This is used to determine how many elements an array contains (pointer types are arrays). */

	/*! This union contains entries corresponding to every possible variant data type. 
		The method in which the parameter is accessed is dependent on TinyVariant::variantType.
	*/
    union {
        SI_Bool     boolVal;	/*!< Used if TinyVariant::variantType is SI_VT_BOOL */
        SI_Byte     bVal;		/*!< Used if TinyVariant::variantType is SI_VT_BYTE */
        SI_UByte    ubVal;		/*!< Used if TinyVariant::variantType is SI_VT_UBYTE */
        SI_Short    sVal;		/*!< Used if TinyVariant::variantType is SI_VT_SHORT */
        SI_UShort   usVal;		/*!< Used if TinyVariant::variantType is SI_VT_USHORT */
        SI_Int      nVal;		/*!< Used if TinyVariant::variantType is SI_VT_INT */
        SI_UInt     unVal;		/*!< Used if TinyVariant::variantType is SI_VT_UINT */
        SI_Float    fVal;		/*!< Used if TinyVariant::variantType is SI_VT_FLOAT */
        SI_Double   dVal;		/*!< Used if TinyVariant::variantType is SI_VT_DOUBLE */
        SI_Long     lVal;		/*!< Used if TinyVariant::variantType is SI_VT_LONG */
        SI_ULong    ulVal;		/*!< Used if TinyVariant::variantType is SI_VT_ULONG */

        SI_Bool     *p_boolVal;	/*!< Used if TinyVariant::variantType is SI_VT_PBOOL */
        SI_Byte     *p_bVal;	/*!< Used if TinyVariant::variantType is SI_VT_PBYTE */
        SI_UByte    *p_ubVal;	/*!< Used if TinyVariant::variantType is SI_VT_PUBYTE */
        SI_Short    *p_sVal;	/*!< Used if TinyVariant::variantType is SI_VT_PSHORT */
        SI_UShort   *p_usVal;	/*!< Used if TinyVariant::variantType is SI_VT_PUSHORT */
        SI_Int      *p_nVal;	/*!< Used if TinyVariant::variantType is SI_VT_PINT */
        SI_UInt     *p_unVal;	/*!< Used if TinyVariant::variantType is SI_VT_PUINT */
        SI_Float    *p_fVal;	/*!< Used if TinyVariant::variantType is SI_VT_PFLOAT */
        SI_Double   *p_dVal;	/*!< Used if TinyVariant::variantType is SI_VT_PDOUBLE */
        SI_Long     *p_lVal;	/*!< Used if TinyVariant::variantType is SI_VT_PLONG */
        SI_ULong    *p_ulVal;	/*!< Used if TinyVariant::variantType is SI_VT_PULONG */

        // We used char instead of SI_Char because SI_Char is plateform dependant.
        char        *p_cVal;	/*!< Used if TinyVariant::variantType is SI_VT_PCHAR. Note: char is used instead of SI_Char, because SI_Char is platform dependent. */
        char        **pp_cVal;	/*!< Used if TinyVariant::variantType is SI_VT_PPCHAR. Note: char is used instead of SI_Char, because SI_Char is platform dependent. */

        SI_Void     *p_voidVal;	/*!< Used if TinyVariant::variantType is SI_VT_VOID. */
    };
};

/*! \typedef SI_TinyVariant
	SI_TinyVariant is an alias for the TinyVariant structure used within CdotXSIParam objects.
*/
typedef TinyVariant SI_TinyVariant;

/*! @} */

//**************************************************************************************
// Macros
//**************************************************************************************
///////////////////////////////////////////// Unix Defines ///////////////////////////////
//
#if defined(unix)

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifndef __APPLE__
#include <malloc.h>
#else
#include <sys/malloc.h>
#endif

#ifdef __XSI_MAINLIB_
#define XSIEXPORT 
#define XSITEMPLATEEXP
#endif

#ifdef __XSI_PLUGIN_
#define XSIEXPORT 
#define XSITEMPLATEEXP
#endif

#ifdef __XSI_APPLICATION_
#ifdef XSI_STATIC_LINK
#define XSIEXPORT
#define XSITEMPLATEEXP
#else
#define XSIEXPORT 
#define XSITEMPLATEEXP
#endif
#endif

#define XSICOREEXPORT

#define _SI_INITCORE
#define _SI_EXITCORE

// Memory stuff
#define _SI_MSIZE( x )          0
#define _SI_MEMCPY( x, y, z )   memcpy( x, y, z )

// character string format
#define SI_Char			char
#define _SI_STRCMP		strcmp
#define _SI_STRNCMP		strncmp
#define _SI_STRICMP		strcasecmp
#define _SI_STRDUP		si_strdup
#define _SI_BSTRDUP     si_bstrdup
#define _SI_STRCPY		strcpy
#define _SI_STRNCPY		strncpy
#define _SI_STRCAT		strcat
#define _SI_SPRINTF		sprintf
#define _SI_STRLEN		strlen
#define _SI_TEXT(quote)	quote
#define _SI_PRINTF		CSIBCUtil::printf
#define _SI_RAND()		rand()
#define _SI_STRTOK		strtok
#define _SI_STRSTR		strstr
#define _SI_STRCHR		strchr
#define _SI_STRRCHR		strrchr
#define _SI_SPLITPATH	si_splitpath
#define _SI_ATOI		atoi
#define _SI_ATOF		atof

#ifndef __APPLE__
//Math Stuff
#define _SI_SQRT		sqrt
#define _SI_SQRTF		sqrtf
#define _SI_ASIN		asin
#define _SI_ASINF		asinf
#define _SI_ACOS		acos
#define _SI_ACOSF		acosf
#define _SI_FLOOR		floor
#define _SI_FLOORF		floorf
#define _SI_COS			cos
#define _SI_COSF		cosf
#define _SI_SIN			sin
#define _SI_SINF		sinf
#define _SI_TAN			tan
#define _SI_TANF		tanf
#define _SI_ATANF		atanf
#define _SI_ATAN		atan
#else
//Math Stuff
#define _SI_SQRT                sqrt
#define _SI_SQRTF               sqrt
#define _SI_ASIN                asin
#define _SI_ASINF               asin
#define _SI_ACOS                acos
#define _SI_ACOSF               acos
#define _SI_FLOOR               floor
#define _SI_FLOORF              floor
#define _SI_COS                 cos
#define _SI_COSF                cos
#define _SI_SIN                 sin
#define _SI_SINF                sin
#define _SI_TAN                 tan
#define _SI_TANF                tan
#define _SI_ATANF               atan
#define _SI_ATAN                atan
#endif


//File stuff
#define __SI_FILE		FILE *
#define __SI_FILE_MODE	SI_Char *
#define __SI_FILE_NULL	(NULL)
#define __SI_FILE_READ_TEXT	"r"
#define __SI_FILE_READ_BINARY "rb"
#define __SI_FILE_WRITE_TEXT	"w"
#define __SI_FILE_WRITE_BINARY "wb"
#define __SI_FILE_SET SEEK_SET
#define __SI_FILE_CUR SEEK_CUR
#define __SI_FILE_END SEEK_END
#define __SI_IS_FILE_MODE( a, b ) !_SI_STRCMP ( a, b )
#define __SI_FOPEN( name, mode )		fopen( name, mode )
#define __SI_FCLOSE( x )		fclose( x )
#define __SI_FREAD( a, b, c, d ) fread( a, b, c, d )
#define __SI_FWRITE( a, b, c, d ) fwrite( a, b, c, d )
#define __SI_FPRINTF( a, b ) fprintf( a, b )
#define __SI_FSEEK( ptr, count, type ) fseek( ptr, count, type )
#define __SI_FTELL( ptr ) ftell( ptr )
#define __SI_FGETC( fp ) ::fgetc(fp)
#define __SI_FEOF( x )   feof(x)
#define __SI_FERROR( x ) (ferror(x))
#define __SI_FFLUSH( x ) (fflush(x))
#define __SI_UNGETC( ch, fp )  (ungetc(ch,fp))

//Assertion
#ifdef _DEBUG
#define _SI_ASSERT(x) assert(x)	
#else
//void XSIEXPORT si_exit(SI_Char *, SI_Char *, const int );
//#define _SI_ASSERT(exp) if (!(exp)) si_exit( #exp, __FILE__, __LINE__ );
#define _SI_ASSERT(exp)
#endif

//Loadable module 
#define _SI_LIBHANDLE int

// Local Memory Allocators 
XSICOREEXPORT void AllocateLocalMemory(SI_Char* &, SI_Long);
XSICOREEXPORT void AllocateLocalMemory(SI_Char** &, SI_Long);

#endif

//
///////////////////////////////////////////// End Unix Defines ///////////////////////////////
//
//
///////////////////////////////////////////// Win32 Defines ///////////////////////////////
//

#if ((defined _WIN32) || (defined WIN32)) && !(defined _WIN32_WCE) && !(defined _XBOX)

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>

#ifdef __XSI_MAINLIB_
#define XSIEXPORT _declspec(dllexport)
#define XSITEMPLATEEXP
#endif

#ifdef __XSI_PLUGIN_
#define XSIEXPORT _declspec(dllexport)
#define XSITEMPLATEEXP
#endif

#ifdef __XSI_APPLICATION_
#ifdef XSI_STATIC_LINK
#define XSIEXPORT
#define XSITEMPLATEEXP
#else
#define XSIEXPORT _declspec(dllimport)
#define XSITEMPLATEEXP extern
#endif
#endif

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

#define _SI_INITCORE
#define _SI_EXITCORE

// Memory stuff
#define _SI_MSIZE( x )          _msize( x )
#define _SI_MEMCPY( x, y, z )   memcpy( x, y, z )

// character string format
#define SI_Char			char
#define _SI_STRCMP		strcmp
#define _SI_STRNCMP		strncmp
#define _SI_STRICMP		stricmp
#define _SI_STRDUP		si_strdup
#define _SI_BSTRDUP     si_bstrdup
#define _SI_STRCPY		strcpy
#define _SI_STRNCPY		strncpy
#define _SI_STRCAT		strcat
#define _SI_SPRINTF		sprintf
#define _SI_STRLEN		strlen
#define _SI_TEXT(quote)	quote
#define _SI_PRINTF		CSIBCUtil::printf
#define _SI_RAND()		rand()
#define _SI_STRTOK		strtok
#define _SI_STRSTR		strstr
#define _SI_STRCHR		strchr
#define _SI_STRRCHR		strrchr
#define _SI_SPLITPATH	_splitpath
#define _SI_ATOI		atoi
#define _SI_ATOF		atof


//Math Stuff
#define _SI_SQRT		sqrt
#define _SI_SQRTF		sqrtf
#define _SI_ASIN		asin
#define _SI_ASINF		asinf
#define _SI_ACOS		acos
#define _SI_ACOSF		acosf
#define _SI_FLOOR		floor
#define _SI_FLOORF		floorf
#define _SI_COS			cos
#define _SI_COSF		cosf
#define _SI_SIN			sin
#define _SI_SINF		sinf
#define _SI_TAN			tan
#define _SI_TANF		tanf
#define _SI_ATANF		atanf
#define _SI_ATAN		atan

//File stuff
#define __SI_FILE		FILE *
#define __SI_FILE_MODE	SI_Char *
#define __SI_FILE_NULL	(NULL)
#define __SI_FILE_READ_TEXT	"r"
#define __SI_FILE_READ_BINARY "rb"
#define __SI_FILE_WRITE_TEXT	"w"
#define __SI_FILE_WRITE_BINARY "wb"
#define __SI_FILE_SET SEEK_SET
#define __SI_FILE_CUR SEEK_CUR
#define __SI_FILE_END SEEK_END
#define __SI_IS_FILE_MODE( a, b ) !_SI_STRCMP ( a, b )
#define __SI_FOPEN( name, mode )		fopen( name, mode )
#define __SI_FCLOSE( x )		fclose( x )
#define __SI_FREAD( a, b, c, d ) fread( a, b, c, d )
#define __SI_FWRITE( a, b, c, d ) fwrite( a, b, c, d )
#define __SI_FPRINTF( a, b ) fprintf( a, b )
#define __SI_FSEEK( ptr, count, type ) fseek( ptr, count, type )
#define __SI_FTELL( ptr ) ftell( ptr )
#define __SI_FGETC( fp ) ::fgetc(fp)
#define __SI_FEOF( x )   feof( x )
#define __SI_FERROR( x ) (ferror(x))
#define __SI_FFLUSH( x ) (fflush(x))
#define __SI_UNGETC( ch, fp )  (ungetc(ch,fp))


//Assertion
#ifdef _DEBUG
#define _SI_ASSERT(x) assert(x)	
#else
//void XSIEXPORT si_exit(SI_Char *, SI_Char *, const int );
//#define _SI_ASSERT(exp) if (!(exp)) si_exit( #exp, __FILE__, __LINE__ );
#define _SI_ASSERT(exp)
#endif

//Loadable module
#define SI_INVALID_LIB_HANDLE NULL
#define _SI_LIBHANDLE	HMODULE
#define _SI_LOADMODULE  LoadLibrary
#define _SI_UNLOADMODULE( x )  FreeLibrary( x )
#define _SI_LOADFUNCTION( HAND, NAME ) GetProcAddress( HAND, NAME )

// Local Memory Allocators 
XSICOREEXPORT void AllocateLocalMemory(SI_Char* &, SI_Long);
XSICOREEXPORT void AllocateLocalMemory(SI_Char** &, SI_Long);

#endif

//
///////////////////////////////////////////// End Win32 Defines ///////////////////////////////
//

//
///////////////////////////////////////////// Windows CE Defines ///////////////////////////////
//
#ifdef _WIN32_WCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//WINDOWS CE
#define _SI_ASSERT(x) 
#include <tchar.h>
#include <winbase.h>

// Memory stuff
#define _SI_MSIZE( x )          0
#define _SI_MEMCPY( x, y, z )   0

// character string format
#define SI_Char			wchar_t
#define _SI_STRCMP		wcscmp
#define _SI_STRNCMP		wcsncmp
#define _SI_STRICMP		wcsicmp
#define _SI_STRCPY		lstrcpy
#define _SI_STRNCPY		wsncpy
#define _SI_STRDUP		si_strdup
#define _SI_BSTRDUP     si_bstrdup
#define _SI_STRCAT		wcscat
#define _SI_SPRINTF		swprintf
#define _SI_FPRINTF     fprintf
#define _SI_STRLEN		wcslen
#define _SI_TEXT(quote)	__TEXT(quote)
#define _SI_ATOI		_wtoi

#define _SI_PRINTF		NKDbgPrintfW
#define _SI_RAND()		rand()
#define _SI_STRTOK		wcstok
#define _SI_STRSTR		wcsstr
#define _SI_STRCHR		wcschr
#define _SI_STRRCHR		wcsrchr
#define _SI_SPLITPATH	_wsplitpath

extern	SI_Char			*_SI_ENDCHAR;
#define _SI_ATOF(x)		wcstod(x, &_SI_ENDCHAR)

#define _SI_SQRT		sqrt
#define _SI_SQRTF		(SI_Float)sqrt
#define _SI_ASIN		asin
#define _SI_ASINF		(SI_Float)asin
#define _SI_ACOS		acos
#define _SI_ACOSF		(SI_Float)acos
#define _SI_FLOOR		floor
#define _SI_FLOORF		(SI_Float)floor
#define _SI_COS			cos
#define _SI_COSF		(SI_Float)cos
#define _SI_SIN			sin
#define _SI_SINF		(SI_Float)sin
#define _SI_TAN			tan
#define _SI_TANF		(SI_Float)tan

// Some PPSH helper functions.
extern "C" 
{
	int U_ropen(const WCHAR *, UINT);
	int U_rread(int, BYTE *, int);
	int U_rclose(int);
}


#define __SI_FILE				int
#define __SI_FILE_MODE			int
#define __SI_FILE_NULL			0
#define __SI_FILE_READ_TEXT		0x00010002
#define __SI_FILE_READ_BINARY	0x00010002
#define __SI_FOPEN( name, mode )	U_ropen( name, mode) 
#define __SI_FCLOSE( x )			U_rclose( x )
#define __SI_FREAD(a,b,c,d)		U_rread( d, (BYTE *) a, b * c)
#define __SI_FEOF( x )           0
#define __SI_IS_FILE_MODE( a, b ) a == b

#endif

//
///////////////////////////////////////////// End Windows CE Defines ///////////////////////////////
//

//
///////////////////////////////////////////// XBOX Defines ///////////////////////////////
//
#ifdef	_XBOX
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>


#ifdef __XSI_MAINLIB_
#define XSIEXPORT		_declspec(dllexport)
#endif

#ifdef __XSI_PLUGIN_
#define XSIEXPORT _declspec(dllexport)
#define XSITEMPLATEEXP
#endif

#ifdef __XSI_APPLICATION_
#ifdef XSI_STATIC_LINK
#define XSIEXPORT
#undef XSICOREEXPORT
#define XSICOREEXPORT
#define XSITEMPLATEEXP
#else
#define XSIEXPORT _declspec(dllimport)
#define XSICOREEXPORT	_declspec(dllimport)
#define XSITEMPLATEEXP extern
#endif
#endif

#define _SI_INITCORE
#define _SI_EXITCORE

// Memory stuff
#define _SI_MSIZE( x )          _msize( x )
#define _SI_MEMCPY( x, y, z )   memcpy( x, y, z )

// character string format
#define SI_Char			char
#define _SI_STRCMP		strcmp
#define _SI_STRNCMP		strncmp
#define _SI_STRICMP		stricmp
#define _SI_STRDUP		si_strdup
#define _SI_BSTRDUP     si_bstrdup
#define _SI_STRCPY		strcpy
#define _SI_STRNCPY		strncpy
#define _SI_STRCAT		strcat
#define _SI_SPRINTF		sprintf
#define _SI_STRLEN		strlen
#define _SI_TEXT(quote)	quote
#define _SI_PRINTF		CSIBCUtil::printf
#define _SI_RAND()		rand()
#define _SI_STRTOK		strtok
#define _SI_STRSTR		strstr
#define _SI_STRCHR		strchr
#define _SI_STRRCHR		strrchr
#define _SI_SPLITPATH	_splitpath
#define _SI_ATOI		atoi
#define _SI_ATOF		atof

//Math Stuff
#define _SI_SQRT		sqrt
#define _SI_SQRTF		sqrtf
#define _SI_ASIN		asin
#define _SI_ASINF		asinf
#define _SI_ACOS		acos
#define _SI_ACOSF		acosf
#define _SI_FLOOR		floor
#define _SI_FLOORF		floorf
#define _SI_COS			cos
#define _SI_COSF		cosf
#define _SI_SIN			sin
#define _SI_SINF		sinf
#define _SI_TAN			tan
#define _SI_TANF		tanf
#define _SI_ATANF		atanf
#define _SI_ATAN		atan

typedef FILE* (*_XBOX_FOPEN_FNC)	( char * filename, char* mode );
typedef int (*_XBOX_FCLOSE_FNC) ( FILE* fdesc );
typedef int (*_XBOX_FREAD_FNC)	( void *result, int size, int count, FILE* );
typedef int (*_XBOX_FWRITE_FNC) ( void *data, int size, int count, FILE* );
typedef int (*_XBOX_FSEEK_FNC)	( FILE*, long count, int mode );
typedef int (*_XBOX_FTELL_FNC)	( FILE* );
typedef int (*_XBOX_FEOF_FNC)	( FILE* );
typedef int (*_XBOX_FGETC_FNC)	( FILE* );


FILE*	XBOXFileOpen ( char *filename , char* mode );
int	XBOXFileClose ( FILE* );
int XBOXFileRead ( void *result, int size, int count, FILE* );
int XBOXFileWrite ( void *data, int size, int count, FILE* );
int XBOXFileSeek ( FILE*, long count, int mode );
int XBOXFileTell ( FILE* );
int XBOXFileEOF ( FILE* fdesc );
int XBOXFileGETC ( FILE* fdesc );

//File stuff

#define __SI_FILE		FILE *
#define __SI_FILE_MODE	SI_Char *
#define __SI_FILE_NULL	(NULL)
#define __SI_FILE_READ_TEXT	"r"
#define __SI_FILE_READ_BINARY "rb"
#define __SI_FILE_WRITE_TEXT	"w"
#define __SI_FILE_WRITE_BINARY "wb"
#define __SI_FILE_SET SEEK_SET
#define __SI_FILE_CUR SEEK_CUR
#define __SI_FILE_END SEEK_END
#define __SI_IS_FILE_MODE( a, b ) !_SI_STRCMP ( a, b )
#define __SI_FOPEN( name, mode )	XBOXFileOpen( name, mode )
#define __SI_FCLOSE( x )		XBOXFileClose( x )
#define __SI_FREAD( a, b, c, d ) 	XBOXFileRead( a, b, c, d )
#define __SI_FWRITE( a, b, c, d ) 	XBOXFileWrite( a, b, c, d )
#define __SI_FPRINTF( a, b ) (0)
#define __SI_FSEEK( ptr, count, type ) XBOXFileSeek( ptr, count, type )
#define __SI_FTELL( ptr ) XBOXFileTell( ptr )
#define __SI_FGETC( fp ) XBOXFileGETC(fp)
#define __SI_FEOF( x )   XBOXFileEOF( x )
#define __SI_FERROR( x ) (0)
#define __SI_FFLUSH( x ) (0)

XSICOREEXPORT SI_Char si_ungetc( SI_Char ch, __SI_FILE x );

#define __SI_UNGETC( ch, fp )  (0)

//Assertion
#ifdef _DEBUG
#define _SI_ASSERT(x) assert(x)	
#else
//void XSIEXPORT si_exit(SI_Char *, SI_Char *, const int );
//#define _SI_ASSERT(exp) if (!(exp)) si_exit( #exp, __FILE__, __LINE__ );
#define _SI_ASSERT(exp)
#endif

//Loadable module
#define SI_INVALID_LIB_HANDLE NULL
#define _SI_LIBHANDLE	HMODULE
#define _SI_LOADMODULE  LoadLibrary
#define _SI_UNLOADMODULE( x )  FreeLibrary( x )
#define _SI_LOADFUNCTION( HAND, NAME ) GetProcAddress( HAND, NAME )

// Local Memory Allocators 
XSICOREEXPORT void AllocateLocalMemory(SI_Char* &, SI_Long);
XSICOREEXPORT void AllocateLocalMemory(SI_Char** &, SI_Long);
#endif


//
///////////////////////////////////////////// Playstation2 Defines ///////////////////////////////
//
#if defined( _PSX2 ) || defined( __MWERKS__ )

#include <eekernel.h> 
#include <sifdev.h>
#include <eeregs.h>
#include <eetypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SIPS2Comm.h>

//#include "SIPS2DisplayDriverSwitch.h"

//#include <sys/types.h>
//#include <fcntl.h>

#ifdef __XSI_MAINLIB_
#define XSIEXPORT 
#define XSITEMPLATEEXP
#endif

#ifdef __XSI_PLUGIN_
#define XSIEXPORT 
#define XSITEMPLATEEXP
#endif

#ifdef __XSI_APPLICATION_
#ifdef XSI_STATIC_LINK
#define XSIEXPORT
#define XSITEMPLATEEXP
#else
#define XSIEXPORT 
#define XSITEMPLATEEXP
#endif
#endif

#ifndef	XSICOREEXPORT
#define	XSICOREEXPORT
#endif

#ifndef	XSIEXPORT
#define XSIEXPORT
#endif

#ifndef min
# define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
# define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#ifndef	_PSX2_FILEIO_NATIVE
#define	_PSX2_FILEIO_NATIVE
#endif

//predefinition
#ifdef __cplusplus
extern "C"
{
#endif
	int open( const char *, int );
	int close( int );
//	off_t lseek( int fildes, off_t offset, int whence );
	char *strcat( char *dest, const char *src );
	int sprintf( char *str, const char *, ...);
	int printf( const char *, ... );
	int rand(void);
	int read( int , void*, int);
	int atoi( const char * );
	double atof( const char * );
	char * si_strchr( char *, int );

#ifdef __cplusplus
}
#endif


// Memory stuff
#define _SI_MSIZE( x )          0
#define _SI_MEMCPY( x, y, z )   memcpy( x, y, z )

// character string format
#define SI_Char			char
#define _SI_STRCMP		strcmp
#define _SI_STRNCMP		strncmp
#define _SI_STRICMP		stricmp
#define _SI_STRDUP		si_strdup
#define _SI_BSTRDUP     si_bstrdup
#define _SI_STRCPY		strcpy
#define _SI_STRNCPY		strncpy
#define _SI_STRCAT		strcat
#define _SI_SPRINTF		sprintf
#define _SI_STRLEN		strlen
#define _SI_TEXT(quote)	quote
#define _SI_PRINTF		printf
#define _SI_ATOI		atoi
#define _SI_ATOF		atof

// fprintf is not available and not needed on the PS2
//#define _SI_FPRINTF     

#define _SI_RAND()		rand()
#define _SI_STRTOK		strtok
#define _SI_STRSTR		strstr
#define _SI_STRCHR		si_strchr // MJD - Found a bug in strchr on GSCube
#define _SI_STRRCHR		strrchr



//Math Stuff
#define _SI_SQRT		sqrt
#define _SI_SQRTF		sqrtf
#define _SI_ASIN		asin
#define _SI_ASINF		asinf
#define _SI_ACOS		acos
#define _SI_ACOSF		acosf
#define _SI_FLOOR		floor
#define _SI_FLOORF		floorf
#define _SI_TAN			tan
#define _SI_TANF		tanf
#define _SI_ATANF		atanf

#ifdef _INLINETRIG_
#define _SI_COS(x)		SI_CosineTable[((int)(x) * 10000) % 62831]
#define _SI_COSF(x)		SI_CosineTable[((int)(x) * 10000) % 62831]
#define _SI_SIN(x)		SI_SineTable[((int)(x) * 10000) % 62831]
#define _SI_SINF(x)		SI_SineTable[((int)(x) * 10000) % 62831]
#else
#define _SI_COS			CSIBCUtil::cos
#define _SI_COSF		CSIBCUtil::cos
#define _SI_SIN			CSIBCUtil::sin
#define _SI_SINF		CSIBCUtil::sin
#endif







// use only if files come from a DOS/Windows system (which is the case)
#ifndef __APPLE__
#define LITTLE_ENDIAN
#endif

//File stuff
#define __SI_FILE				int
#define __SI_FILE_MODE			int
#define __SI_FILE_NULL			(int)(-1) 
#define __SI_FILE_READ_TEXT	SCE_RDONLY
#define __SI_FILE_READ_BINARY SCE_RDONLY 
#define __SI_FILE_WRITE_TEXT	SCE_WRONLY | SCE_TRUNC | SCE_CREAT
#define __SI_FILE_WRITE_BINARY SCE_WRONLY | SCE_TRUNC | SCE_CREAT
#define __SI_FILE_SET SCE_SEEK_SET
#define __SI_FILE_CUR SCE_SEEK_CUR
#define __SI_FILE_END SCE_SEEK_END
#define __SI_IS_FILE_MODE( a, b ) ((a)==(b))
#define __SI_FOPEN( NAME, MODE ) PS2FileOpen( NAME, MODE )
#define __SI_FCLOSE( x ) PS2FileClose( x )
#define __SI_FREAD( buf, size, count, fp ) PS2FileRead( fp, buf, size, count )
#define __SI_FWRITE( buf, size, count, fp ) PS2FileWrite( fp, buf, size, count )
// fprintf is not available and not needed on the PS2
#define __SI_FSEEK( ptr, count, type ) PS2FileSeek( ptr, count, type )
#define __SI_FTELL( ptr ) PS2FileTell( ptr )
#define __SI_FGETC( fp ) PS2FileGetC( fp )
#define __SI_FEOF( x )   PS2FileEOF( x )

#define _SI_INITCORE SIPS2CommInit();
#define _SI_EXITCORE SIPS2CommClose();

#ifdef _DEBUG
#define _SI_ASSERT(x) assert(x)	
#else
void XSIEXPORT si_exit(SI_Char *, SI_Char *, const int );
//#define _SI_ASSERT(exp) if (!(exp)) si_exit( #exp, __FILE__, __LINE__ );
#define _SI_ASSERT(exp)
#endif


//Loadable module
#define SI_INVALID_LIB_HANDLE -1
#define _SI_LIBHANDLE	int

#endif
//
///////////////////////////////////////////// End Playstation2 Defines ///////////////////////////////
//

// Already documented in SIBCMath.h
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

/*! \addtogroup ftktutil_group FTK utility macros and defines.
	@{
*/

/*! Prints error message \p x. Print location is platform dependent. */
#define _ERRMSG(x) _SI_PRINTF(_SI_TEXT("ERROR : %s\n"),x);	

/*! Prints message \p x. Print location is platform dependent. */
#define _MSG(x)    _SI_PRINTF(_SI_TEXT("MSG   : %s\n"),x);

/*! Creates a new instance of \p y and assigns it to \p x, making sure it is not #NULL. */
#define _SI_NEW(x,y) {	x = new y; _SI_ASSERT( x != NULL ); }

/*! Deletes \p ptr, if it is not #NULL, and then assigns \p ptr to #NULL. */
#define _SI_DELETE( ptr ) {	if ( ptr != NULL ) { delete ptr; ptr = NULL; }  }

/*! Deletes the array \p ptr, if it is not #NULL, and then assigns \p ptr to #NULL. */
#define _SI_DELETE_ARRAY( ptr ) {if ( ptr != NULL ) { delete [] ptr; ptr = NULL; }  }

/*!	If \p result is #SI_SUCCESS, then calls the function \p x, and assigns the
	the return to \p result. If the result is not #SI_SUCCESS, prints out the
	error message \p y. \p result must be a defined varaible.
*/
#define _SI_CALL(x,y) { if (result==SI_SUCCESS)	{ result = x;if(result != SI_SUCCESS){_ERRMSG(y);}}}

#define _SI_CHKRESULT( RESULT, MESSAGE, ACTION ){if (RESULT != SI_SUCCESS){	_ERRMSG( MESSAGE );	ACTION;	}	}

XSICOREEXPORT SI_Char *  si_strdup( const SI_Char * );
XSICOREEXPORT SI_Char *  si_bstrdup( const SI_Byte * );
XSICOREEXPORT SI_Error si_bstringtostring( SI_Char *, const SI_Byte * );
XSICOREEXPORT SI_Void si_splitpath(const SI_Char *, SI_Char *, SI_Char *, SI_Char *, SI_Char *);

#define _SI_STRDUPL(D,S) { D = (char *) alloca(strlen(S)); strcpy(D,S); }

#ifdef __cplusplus //If included by C-files this causes an error

#ifdef _INLINETRIG_
extern	float	SI_SineTable[62831];
extern	float	SI_CosineTable[62831];
#endif

//**************************************************************************************
// CSIBCUtil | Utility functions.
//**************************************************************************************

//! Class used for utility functions.
/*! Used for logging utilities, and quick sine and cosine evaluations.
	This is the class containing all things that do not necessarily belong anywhere else.
*/

class XSICOREEXPORT CSIBCUtil
{
public:

	/*! Default Constructor.
		Constructs a utility object.
	*/
	CSIBCUtil();
	~CSIBCUtil();

	/*! Opens the logfile given by \p in_pFilename
		\param	in_pFilename	Filename of the logfile to open.
		\return	SI_Error	Error code indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The logfile was successfully opened.
		</ul>
		\sa CSIBCUtil::Close
		\sa CSIBCUtil::SetToStderr
	*/
	static	SI_Error	Open(SI_Char *in_pFilename);

	/*!	Closes the currently opened logfile.
		\return	SI_Error	Error code indicating the success or failure of this command.

		<ul>Possible returns:
			<li>SI_SUCCESS - The file was closed successfully.
			<li>SI_ERR_ERROR_MSG - An error occurred while closing the file.
		</ul>
		\sa CSIBCUtil::Open
	*/
	static	SI_Error	Close();
	
	/*! Sets the logfile output to goto stderr.
		\sa CSIBCUtil::Open
	*/
	static	void		SetToStderr();

	/*!	Prints a formatted string to the opened logfile. The format of this
		parameter is equivalent to the stdio printf function.
		\param	str	Formatting string.
		param	...	Values for entries in the formatting string.
		\sa CSIBCUtil::Open
	*/
	static	void		printf(SI_Char * str,...);
	
	// Fast lookup table for trigonometric functions

	/*! Initializes the lookup tables for sine and cosine evaluations.
		\sa CSIBCUtil::cos
		\sa CSIBCUtil::sin
	*/
	static	void		InitializeTrigTables();

	/*!	Returns the cosine of \p x.
		\param	x	The angle (in radians) to get the cosine value of.
		\return	float	The cosine of \p x.
		\sa CSIBCUtil::sin
		\sa CSIBCUtil::InitializeTrigTables
	*/
	static	float		cos(float x);

	/*!	Returns the sine of \p x.
		\param	x	The angle (in radians) to get the sine value of.
		\return	float	The sine of \p x.
		\sa CSIBCUtil::cos
		\sa CSIBCUtil::InitializeTrigTables
	*/
	static	float		sin(float x);
	
protected:	
	static SI_Char m_Filename[256];

private:
	SI_Void				*m_pUnused;

};

#endif


//**************************************************************************************
// Inline code
//**************************************************************************************


//***************************************************************************************
// Author   : Games S.W.A.T. Team
// Date     :
//
// Calculates the cosine of an angle.
// float | x | Angle. 
// Result of the cosine operation.
//***************************************************************************************
inline float CSIBCUtil::cos(float x)
{
#ifdef _INLINETRIG_
	int		index = (int) ( x * 10000.0f);
		
	if (index < 0)
		index += 628310000;
	
	return SI_CosineTable[index % 62831];
#else
	return _SI_COSF(x);
#endif
}

//***************************************************************************************
// Author   : Games S.W.A.T. Team
// Date     :
//
// Calculates the sine of an angle.
// float | x | Angle.
// Result of the sin operation.
//***************************************************************************************
inline float CSIBCUtil::sin(float x)
{
#ifdef _INLINETRIG_
	int		index = (int) ( x * 10000.0f);
	
	if (index < 0)
		index += 628310000;
		
	return SI_SineTable[index % 62831];
#else
	return _SI_SINF(x);
#endif
}

//**************************************************************************************
// _SI_FILE macro definitions
//
//
//**************************************************************************************

// SIFILE things

// No need to document this class, it is only used as a private member of SIFilePtr.
class SIFileInterface
{
public:
	virtual ~SIFileInterface()
	{
	}
	
	// FUNCTIONS
	virtual int close() = 0;

	virtual size_t read( void *pbuffer, size_t size, size_t count ) = 0;

	virtual size_t write( const void *buffer, size_t size, size_t count ) = 0;

	virtual int printf( const char *format ) = 0;

	virtual int seek( long offset, int origin ) = 0;

	virtual long tell() = 0;

	virtual int fgetc() = 0;

	virtual int eof() = 0;

	virtual int error() = 0;

	virtual int flush() = 0;

	virtual int siungetc( int ch ) = 0;

	// OPERATORS
	virtual operator __SI_FILE() = 0;

	//virtual operator bool() const = 0;

	// you must define:  operator=( __SI_FILE const& other )
};
class SIFile: public SIFileInterface
{
private:
	__SI_FILE fp;

public:
	enum openMode
	{
		Read_Text = 0,
		Read_Binary,
		Write_Text,
		Write_Binary,
	};

	enum seekDir
	{
#ifdef __SI_FILE_SET
		sdSet = __SI_FILE_SET,
#endif
#ifdef __SI_FILE_CUR
		sdCur = __SI_FILE_CUR,
#endif
#ifdef __SI_FILE_END
		sdEnd = __SI_FILE_END,
#endif
	};

	// CONSTRUCTORS

	SIFile(): fp( __SI_FILE_NULL ) {}

	SIFile( __SI_FILE file ): fp( file ) {}

	SIFile( const char *name, int mode )		// open
	{
#ifdef __SI_FOPEN
		static __SI_FILE_MODE const mt[] =
		{
#ifdef __SI_FILE_READ_TEXT
		__SI_FILE_READ_TEXT, 
#else
	#error you need to define this
#endif
#ifdef __SI_FILE_READ_BINARY
		__SI_FILE_READ_BINARY, 
#else
	#error you need to define this
#endif
#ifdef __SI_FILE_WRITE_TEXT
		__SI_FILE_WRITE_TEXT, 
#else
	#error you need to define this
#endif
#ifdef __SI_FILE_WRITE_BINARY
		__SI_FILE_WRITE_BINARY,
#else
	#error you need to define this
#endif
		};
		fp = __SI_FOPEN( (char*)name, mt[mode] );
#else
	#error you need to define this
#endif
	}

	// FUNCTIONS
	int close()
	{
#ifdef __SI_FCLOSE
		int		temp = __SI_FCLOSE( fp );
		fp = __SI_FILE_NULL;						// extra safety
		return temp;
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	size_t read( void *buffer, size_t size, size_t count )
	{
#ifdef __SI_FREAD
		return __SI_FREAD( buffer, size, count, fp );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	size_t write( const void *buffer, size_t size, size_t count )
	{
#ifdef __SI_FWRITE
		return __SI_FWRITE( (void*)buffer, size, count, fp );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	int printf( const char *format )
	{
#ifdef __SI_FPRINTF
		return __SI_FPRINTF( fp, format );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	int seek( long offset, int origin )
	{
#ifdef __SI_FSEEK
		return __SI_FSEEK( fp, offset, origin );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	long tell()
	{
#ifdef __SI_FTELL
		return __SI_FTELL( fp );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	int fgetc()
	{
#ifdef __SI_FGETC
		return __SI_FGETC( fp );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	int eof()
	{
#ifdef __SI_FEOF
		return __SI_FEOF( fp );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	int error()
	{
#ifdef __SI_FERROR
		return __SI_FERROR( fp );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}
	
	int flush()
	{
#ifdef __SI_FFLUSH
		return __SI_FFLUSH( fp );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}

	int siungetc( int ch )
	{
#ifdef __SI_UNGETC
		return __SI_UNGETC( ch, fp );
#else
		_SI_ASSERT( false );
		return 0;
#endif
	}
	
	// OPERATORS
	operator __SI_FILE()
	{
		return fp;
	}

	/*operator bool() const
	{
		return fp == __SI_FILE_NULL;
	}*/

	SIFile operator=( __SI_FILE const& other )
	{
		fp = other;
		return *this;
	}

	SIFile operator=( SIFile const& other )
	{
		fp = other.fp;
		return *this;
	}
};

//***************************************************************************************
//
// Author  : Neil Girdhar / Softimage Games Team
// Date    : July 8th 2002
//
// FILE STUFF
//
//***************************************************************************************

// BUFFERED FILE

#if defined( _XBOX ) || defined( _PSX2 )
class BufferedFile: public SIFileInterface
{
private:
	char				*buffer;
	int					goodBytes;			// bytes of the buffer that are good
	int					position;			// position of next read in buffer
	unsigned int		bufferSize;
	SIFile				fp;
	/*
	our position in the underlying stream will always be goodBytes.
	to synchronise the surface stream, we would seek (position-goodBytes) (a negative number) bytes forward.
	*/

public:
	// CONSTRUCTORS
	BufferedFile(): buffer( NULL ), goodBytes(0), position(0), bufferSize( 0 ), fp()
	{
	}

	BufferedFile( __SI_FILE fpIn ): buffer( NULL ), goodBytes(0), position(0), bufferSize( 0 ), fp( fpIn )
	{
	}

	BufferedFile( const char *name, int mode, unsigned int bs ): buffer( new char[bs] ), goodBytes(0), position(0), bufferSize( bs ), fp()
	{
		fp = SIFile( name, mode );
	}

	virtual ~BufferedFile()
	{
		if( buffer!=NULL ) delete [] buffer;
	}
	
	// BUFFERING FUNCTIONS
	void clearBuffer()
	{
		goodBytes = 0;
	}

	void reseekBuffer()
	{
		int			seekdistance = position-goodBytes;		// see note above
		if( seekdistance==0 ) return;			// already seeked
		fp.seek( seekdistance, SIFile::sdCur );	// seek forward to where the user thinks we are  (room for optimization)
		position = 0;							// set our position
		clearBuffer();							// clear the buffer after reseek
	}

	unsigned int fillBuffer()
	{
		reseekBuffer();
		goodBytes = fp.read( buffer, 1, bufferSize );
#ifdef	_PSX2
		if (goodBytes < 0)
			goodBytes = 0;
#endif
		position = 0;
		return  goodBytes;
	}

	unsigned int remainingBytesInBuffer()
	{
		return goodBytes-position;
	}

	// FUNCTIONS
	int close()
	{
		clearBuffer();
		return fp.close();
	}

	size_t read( void *pbuffer, size_t size, size_t count )
	{
		if( size*count<=bufferSize )		// only cache for small reads
		//if( false )
		{
			char		*dest = (char*)pbuffer;
			int			copiedWords = 0;

			while( count>0 )
			{
				if( remainingBytesInBuffer()<size ) 
				{ 
					if( fillBuffer()<size )
					{
						break;	// could not read at least one word/record
					}
				}

				// buffer is not empty, so copy out good bytes
				{
					int			copyWords = min( remainingBytesInBuffer()/size, count );	// words to copy
					int			copyBytes = copyWords * size;					// bytes to copy
				
					_SI_MEMCPY( dest, buffer+position, copyBytes );
					
					dest += copyBytes;
					position += copyBytes;
					
					copiedWords += copyWords;
					count -= copyWords;
				}
			}
		
			return copiedWords;
		}
		else
		{
			reseekBuffer();
			clearBuffer();
			return fp.read( pbuffer, size, count );
		}
	}

	size_t write( const void *buffer, size_t size, size_t count )
	{
		reseekBuffer();
		clearBuffer();
		return fp.write( buffer, size, count );
	}

	int printf( const char *format )
	{
		reseekBuffer();
		clearBuffer();
		return fp.printf( format );
	}

	int seek( long offset, int origin )
	{
		if( origin==SIFile::sdCur )
		{
			offset+=position-goodBytes;				// collect position and offset together
		}
		position = 0;						// after the seek, we will be at position 0
		clearBuffer();						// buffer is invalid after seek; there is room for optimization here, but it shouldn't matter in general use
		return fp.seek( offset, origin );	// recurse
	}

	long tell()
	{
		return fp.tell() + position;		// user thinks we are position bytes ahead
	}

	int fgetc()
	{
		char retval;
		if( read( &retval, 1, 1 )==0 )
		{
			return fp.fgetc();				// same EOF/error characteristics
		}
		return retval;
	}

	int eof()
	{
		if( goodBytes > position ) return 0;
		reseekBuffer();
		return fp.eof();
	}

	int error()
	{
		reseekBuffer();
		return fp.error();
	}

	int flush()
	{
		// no need to reseek or clear buffer
		return fp.flush();
	}

	int siungetc( int ch )
	{
		reseekBuffer();
		clearBuffer();
		return fp.siungetc( ch );
	}

	// OPERATORS
	operator SIFile()
	{
		reseekBuffer();
		clearBuffer();
		return fp;
	}

	operator __SI_FILE()
	{
		reseekBuffer();
		clearBuffer();
		return fp;
	}

	/*operator bool() const
	{
		return (bool)fp;
	}*/
};
#endif

class SIFilePtr
{
private:
	SIFileInterface		*fp;

public:
	SIFilePtr()
	{
		fp = NULL;
	}

	SIFilePtr( __SI_FILE file )
	{
		this->operator=( file );
	}


	SIFilePtr( const char *name, int mode )
	{
#if	(defined _XBOX) || (defined _PSX2)
		fp = (SIFileInterface*)new BufferedFile( name, mode, 2048 );		// bufferring
#elif (defined unix) \
		|| ((defined _WIN32) || (defined WIN32)) && !(defined _WIN32_WCE) && !(defined _XBOX) \
		|| (defined _WIN32_WCE)
		fp = (SIFileInterface*)new SIFile( name, mode );					// no bufferring
#else
		// unknown platform
		fp = (SIFileInterface*)new SIFile( name, mode );					// no bufferring
#endif
	}

	~SIFilePtr()
	{
	}
	
	// FUNCTIONS
	int close()
	{
		int retval = fp->close();
		delete fp;
		return retval;
	}

	size_t read( void *pbuffer, size_t size, size_t count )
	{
		return fp->read( pbuffer, size, count );
	}

	size_t write( const void *buffer, size_t size, size_t count )
	{
		return fp->write( buffer, size, count );
	}

	int printf( const char *format )
	{
		return fp->printf( format );
	}

	int seek( long offset, int origin )
	{
		return fp->seek( offset, origin );	// recurse
	}

	long tell()
	{
		return fp->tell();
	}

	int fgetc()
	{
		return fp->fgetc();
	}

	int eof()
	{
		return fp->eof();
	}

	int error()
	{
		return fp->error();
	}

	int flush()
	{
		return fp->flush();
	}

	int siungetc( int ch )
	{
		return fp->siungetc( ch );
	}

	// OPERATORS

	operator __SI_FILE()
	{
		if( fp==NULL ) return __SI_FILE_NULL;
		return (__SI_FILE)(*fp);
	}

	/*operator bool() const
	{
		if( fp==NULL ) return false;
		return fp->operator bool();
	}*/

	SIFilePtr operator=( __SI_FILE const& other )
	{
		if( other == __SI_FILE_NULL )
		{
			fp = NULL;
		}
		else
		{
			fp = (SIFileInterface*)new SIFile( other );
		}
		return *this;
	}

	SIFilePtr operator=( SIFilePtr &other )
	{
		fp = other.fp;
		return *this;
	}
};

typedef SIFilePtr _SI_FILE;

XSICOREEXPORT SI_Char si_getc( _SI_FILE x );
XSICOREEXPORT SI_Char si_ungetc( SI_Char ch, _SI_FILE x );


// FILE MACROS
#define _SI_FILE_MODE	SIFile::openMode
#define _SI_FILE_NULL	__SI_FILE_NULL
#define _SI_FILE_READ_TEXT	(SIFile::Read_Text)
#define _SI_FILE_READ_BINARY (SIFile::Read_Binary)
#define _SI_FILE_WRITE_TEXT	(SIFile::Write_Text)
#define _SI_FILE_WRITE_BINARY (SIFile::Write_Binary)
#define _SI_FILE_SET (SIFile::sdSet)
#define _SI_FILE_CUR (SIFile::sdCur)
#define _SI_FILE_END (SIFile::sdEnd)
#define _SI_IS_FILE_MODE( a, b ) ((a)==(b))
#define _SI_FOPEN( name, mode )		(_SI_FILE( name, mode ))
#define _SI_FCLOSE( x )		((x).close())
#define _SI_FREAD( a, b, c, d ) ((d).read( a, b, c ))
#define _SI_FWRITE( a, b, c, d ) ((d).write( a, b, c ))
#define _SI_FPRINTF( a, b ) ((a).printf( b ))
#define _SI_FSEEK( ptr, count, type ) ((ptr).seek( count, type ))
#define _SI_FTELL( ptr ) ((ptr).tell())
#define _SI_FGETC( fp ) ((fp).fgetc())
#define _SI_FEOF( x ) ((x).eof())
#define _SI_FERROR( x ) ((x).error())
#define _SI_FFLUSH( fp ) ((fp).flush())
#define _SI_UNGETC( fp ) ((fp).siungetc())

//**************************************************************************************
// WARNING CALLBACK FUNCTION
//
//
//**************************************************************************************

typedef SI_Void (*SI_WARNING_CALLBACK_FNC)	( SI_Char*, SI_Int );
#define SI_MESSAGE	0
#define SI_WARNING	1
#define SI_ERROR	2

//
// Allocation
//

XSICOREEXPORT void	*	FTK_calloc( size_t, size_t );
XSICOREEXPORT void		FTK_free( void * );
XSICOREEXPORT void		FTK_delete( void * );


#endif // CSIBCUtil
