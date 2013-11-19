/*******************************************************************************
 * "ufoTypes.h"
 * 
 * 
 * Copyright (c) 3 Space Software Ltd. 1994, 1995.
 *           (c) Copyright 1996 - 2002 Softimage/Avid Corporation
 *
 * -----------------------------------------------------------------------------
 * Media Illusion UFO (User Function Object) SDK
 * adapted for Softimage|XSI by Luc-Eric Rousseau (lucer), nov 2002
 ******************************************************************************/


#ifndef __ufoTypes
#define __ufoTypes

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * lucer : 
 * Under Windows, we can provide automatic exporting of symbols using the 
 * declspec keyword as token before the function definition.
 * We stick to cdecl calling convention, so as to not introduce
 * different name decoration under unix/win32
******************************************************************************/


#if !defined(unix)
    #ifdef UFOAPI_IMPL
        #define UFOAPI      __declspec( dllexport ) 
    #else
        #define UFOAPI      __declspec( dllimport ) 
    #endif

    #define UFOEXPORT   __declspec( dllexport ) 
#else
    #define UFOAPI
    #define UFOEXPORT   
#endif


/*******************************************************************************
 * ufoCompComb: 
 *      Used in definition of process input or output. It is used to indicate
 *      what type of raster is primarily expected on that input/output by the
 *      process.
 *      
 *      (NB: it does not control the type that is actually provided.)
 ******************************************************************************/
typedef enum
{
    ufoAnyCompComb          = 3, 
    ufoRGBCompComb          = 0, 
    ufoRGBACompComb         = 2, 
    ufoACompComb            = 1,
    ufoRGBOptionalACompComb = 4

} ufoCompComb;


typedef enum
{
    ufoAnyPixelType     = 0, 
    ufoAny8PixelType    = 10, 
    ufoAny16PixelType   = 20, 
    ufoAnyFPixelType    = 30,
    
    ufoRGB8PixelType    = 1, 
    ufoA8PixelType      = 2, 
    ufoRGBA8PixelType   = 3, 
    
    ufoRGB16PixelType   = 11, 
    ufoA16PixelType     = 12, 
    ufoRGBA16PixelType  = 13,
    
    ufoRGBFPixelType    = 21, 
    ufoAFPixelType      = 22, 
    ufoRGBAFPixelType   = 23,
    
	ufoRGB8PackedPixelType	= 41,	// for conversion only - RGB (not BGR), 24 bit

    ufoUndefinedPixelType = 9999

} ufoPixelType;


typedef enum
{
    ufoUndefinedEffect  = 0,
    ufoTransitionEffect = 1
} ufoEffectType;
   
typedef enum
{
    ufoBooleanParam     = 0, 
    ufoEnumParam        = 1, 
    ufoIntegerParam     = 2, 
    ufoFloatParam       = 3, 
    ufoNormalisedParam  = 4, 
    ufoAngleParam       = 5, 
    ufoColourRGBParam   = 6, 
    ufoColourCMYParam   = 7, 
    ufoColourHSVParam   = 8, 
    ufoColourHLSParam   = 9, 
    ufoRectangleParam   = 10, 
    ufoPos3dParam       = 11, 
    ufoScale3dParam     = 12, 
    ufoPos2dParam       = 13, 
    ufoScale2dParam     = 14, 
    ufoStringParam      = 15,
    ufoShapeListParam   = 16,

} ufoParamType;


typedef enum
{
    ufoDefaultChannelIndex          = 0,
    
    ufoColourRGBChannelIndexR       = 0, 
    ufoColourRGBChannelIndexG       = 1, 
    ufoColourRGBChannelIndexB       = 2,
     
    ufoColourCMYChannelIndexC       = 0, 
    ufoColourCMYChannelIndexM       = 1, 
    ufoColourCMYChannelIndexB       = 2,
     
    ufoColourHLSChannelIndexH       = 0, 
    ufoColourHLSChannelIndexL       = 1, 
    ufoColourHLSChannelIndexS       = 2,
     
    ufoColourHSVChannelIndexH       = 0, 
    ufoColourHSVChannelIndexS       = 1, 
    ufoColourHSVChannelIndexV       = 2,
     
    ufoPos3dChannelX                = 0, 
    ufoPos3dChannelY                = 1, 
    ufoPos3dChannelZ                = 2,
     
    ufoScale3dChannelX              = 0, 
    ufoScale3dChannelY              = 1, 
    ufoScale3dChannelZ              = 2,
     
    ufoRectangleChannelIndexXmin    = 0, 
    ufoRectangleChannelIndexYmin    = 1, 
    ufoRectangleChannelIndexXmax    = 2, 
    ufoRectangleChannelIndexYmax    = 3, 
    
    ufoPos2dChannelX                = 0, 
    ufoPos2dChannelY                = 1,
    
    ufoScale2dChannelX              = 0, 
    ufoScale2dChannelY              = 1 

    
} ufoChannelIndex;


typedef enum
{
    ufoLimitsClampNone              = 0, 
    ufoLimitsClampMin               = 1, 
    ufoLimitsClampMax               = 2, 
    ufoLimitsClampBoth              = 3
    
} ufoLimitsClamp;


#define     ufoCurrentTime

typedef enum
{
    ufoEdgeNoClamp      = 0, 
    ufoEdgeClamp        = 1
    
} ufoEdgeMode;


typedef enum
{
    ufoPassHorizontal, 
    ufoPassVertical 

} ufoPassMode;


typedef struct
{
    int         x1;
    int         y1;
    int         x2;
    int         y2;
} ufoRectangle;
      
        
typedef enum
{
    ufoPreviewFrame     = 0,
    ufoFullFrame        = 1,
    ufoPreviewSequence  = 2,
    ufoFullSequence     = 3

} ufoRenderMode;


typedef enum
{
    ufoEditorDrag, 
    ufoEditorRelease, 
    ufoEditorForce
} ufoEditorEventType;


typedef enum
{
    ufoInput  = 0, 
    ufoOutput = 1, 
    ufoMask   = 2
    
} ufoViewType;


/*definition for basic ufo mp fork function*/
typedef void (*ufoMPForkFunc)();                

typedef enum
{
    ufoLButtonFlag  = 0x0001,
    ufoRButtonFlag  = 0x0002,
    ufoShiftFlag    = 0x0004,
    ufoControlFlag  = 0x0008,
    ufoMButtonFlag  = 0x0010
    
} ufoMouseFlags;


typedef struct 
{
    float       x, y;           // position
    unsigned    flags;          // combination of ufoMouseFlags
    float       pressure;       // pen tablet pressure, between 0 and 1

    float       x_drag_delta;   // distance since the mouse down, when the user dragging the mouse
    float       y_drag_delta;
} ufoMouseEvent;


#ifdef __cplusplus
}
#endif

#endif


/*******************************************************************************
 * End of file ufoTypes.h
 ******************************************************************************/
