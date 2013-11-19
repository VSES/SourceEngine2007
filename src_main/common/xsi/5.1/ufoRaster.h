/*******************************************************************************
 * Media Illusion "ufoRaster.h"
 * 
 * 
 * Copyright (c) 3 Space Software Ltd. 1994, 1995.
 *               Avid Technology Inc.  1996.
 * 
 ******************************************************************************/


#ifndef __ufoRaster
#define __ufoRaster

#include "ufoTypes.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef void * ufoRaster;


/* pixel component constants storage constants */

#define     ufoComp8Min         0               /* min value for 8 bit pixel component */
#define     ufoComp8Max         255             /* max value for 8 bit pixel component */
#define     ufoComp8Bits        8               /* actual number of bits from byte used */
    
#define     ufoComp16Min        0               /* min value for 16 bit pixel component */
#define     ufoComp16Max        16383           /* max value for 16 bit pixel component */
#define     ufoComp16Bits       14              /* actual number of bits from short used */
    
#define     ufoCompFloatMin     0.0             /* min value for float pixel component */
#define     ufoCompFloatMax     1.0             /* max value for float pixel component */


/* pixel structure definitions (for reference or use) */

typedef struct 
{
    unsigned char       alpha_unused_;          /* don't use */
    unsigned char       blue_;
    unsigned char       green_;
    unsigned char       red_;
    
} ufoPixelRGB8;         /* use when ufoPixelType is ufoPixelRGB8Type */

typedef struct 
{
    unsigned char       alpha_;
    
} ufoPixelA8;           /* use when ufoPixelType is ufoPixelA8Type */

typedef struct 
{
    unsigned char       alpha_;
    unsigned char       blue_;
    unsigned char       green_;
    unsigned char       red_;
    
} ufoPixelRGBA8;        /* use when ufoPixelType is ufoPixelRGBA8Type */

typedef struct 
{
    unsigned short      blue_;
    unsigned short      green_;
    unsigned short      red_;
    
} ufoPixelRGB16;        /* use when ufoPixelType is ufoPixelRGB16Type */

typedef struct 
{
    unsigned short      alpha_;
    
} ufoPixelA16;          /* use when ufoPixelType is ufoPixelA16Type */

typedef struct 
{
    unsigned short      alpha_;
    unsigned short      blue_;
    unsigned short      green_;
    unsigned short      red_;
    
} ufoPixelRGBA16;       /* use when ufoPixelType is ufoPixelRGBA16Type */

typedef struct 
{
    float               blue_;
    float               green_;
    float               red_;
    
} ufoPixelRGBF;         /* use when ufoPixelType is ufoPixelRGBFType */

typedef struct 
{
    float               alpha_;
    
} ufoPixelAF;           /* use when ufoPixelType is ufoPixelAFType */

typedef struct 
{
    float               alpha_;
    float               blue_;
    float               green_;
    float               red_;
    
} ufoPixelRGBAF;        /* use when ufoPixelType is ufoPixelRGBAFType */


/*******************************************************************************
 * ufoRasterGetLimits():
 *    Inquires limits of raster.
 *    This should be used to check the bounds of a raster where required.
 ******************************************************************************/
UFOAPI
void ufoRasterGetLimits(
    ufoRaster           raster_instance, 
    
    int                 *x1, 
    int                 *y1, 
    int                 *x2, 
    int                 *y2);
    
/*******************************************************************************
 * ufoRasterGetWidth():
 * ufoRasterGetHeight():
 *    Inquire width and height of raster, respectively.
 ******************************************************************************/
UFOAPI
int ufoRasterGetWidth(
    ufoRaster           raster_instance);

UFOAPI
int ufoRasterGetHeight(
    ufoRaster           raster_instance);


/*******************************************************************************
 * ufoRasterClipLimits():
 *    Checks rectangle against raster limits and clips if partially outside.
 *    Returns 1 if rectangle is inside or clipped to raster limits.
 *    Returns 0 if rectangle is outside of raster limits.
 * 
 * ufoRasterIsInside():
 *    Checks (x, y) point is inside raster limits.
 *    Returns 1 if point is inside limits.
 *    Returns 0 if point is outside limits.
 ******************************************************************************/

UFOAPI
int ufoRasterClipLimits(
    ufoRaster           raster_instance, 
    
    int                 *x1, 
    int                 *y1, 
    int                 *x2, 
    int                 *y2);

UFOAPI
int ufoRasterIsInside(
    ufoRaster           raster_instance, 
    
    int                 x, 
    int                 y);
    
    
/*******************************************************************************
 * ufoRasterGetPixelType():
 *    Inquire the pixel type of the raster.
 ******************************************************************************/
UFOAPI
ufoPixelType ufoRasterGetPixelType(
    ufoRaster           raster_instance);


/*******************************************************************************
 * == new in XSI
 * ufoPixelTypeGetAlphaVariation():
 * ufoPixelTypeGetRGBAVariation():
 * ufoPixelTypeGetRGBVariation():
 *    Returns the Alpha, RGBA or RGB the pixel format that is 
 *    in the same bit depth as the input pixel type
 ******************************************************************************/
UFOAPI
ufoPixelType ufoPixelTypeGetAlphaVariation(
    ufoPixelType        pixel_type);

UFOAPI
ufoPixelType ufoPixelTypeGetRGBVariation(
    ufoPixelType        pixel_type);

UFOAPI
ufoPixelType ufoPixelTypeGetRGBAVariation(
    ufoPixelType        pixel_type);

/*******************************************************************************
 * == new in XSI
 * ufoPixelTypeGetPixelSize():
 *    Returns the size of one pixel for the given pixel type
 *    RGB8 and RGBA8 are both 4 bytes, for other types it is 
 *    the component size multipled by the number of channels
 ******************************************************************************/

UFOAPI
int ufoPixelTypeGetPixelSize(
    ufoPixelType        pixel_type);


/******************************************************************************
 * ufoRasterGetPixelAddress():
 *    Inquire the pixel address of the raster buffer at x, y.
 *    No bounds check is performed on x and y.
 ******************************************************************************/
UFOAPI
void* ufoRasterGetPixelAddress(
    ufoRaster           raster_instance, 
    
    int                 x, 
    int                 y);


/****************************************************************************************
 * ufoRasterGetBufferAddress():
 *    Inquire the base pixel address of the raster buffer.
 ****************************************************************************************/
UFOAPI
void* ufoRasterGetBufferAddress(
    ufoRaster           raster_instance);


/*******************************************************************************
 * ufoRasterGetBufferSize():
 * ufoRasterGetPixelSize():
 * ufoRasterGetComponentSize():
 *    Inquire the buffer size, the pixel size and component size of the raster, 
 *    respectively.
 ******************************************************************************/
UFOAPI
int ufoRasterGetBufferSize(
    ufoRaster           raster_instance);

UFOAPI
int ufoRasterGetPixelSize(
    ufoRaster           raster_instance);

UFOAPI
int ufoRasterGetComponentSize(
    ufoRaster           raster_instance);
    
    
/*******************************************************************************
 * ufoRasterGetPixelCol():
 * ufoRasterGetPixelAlpha():
 * ufoRasterGetPixelColAlpha():
 *    Inquire the RGB, A or RGBA values for a raster, respectively.
 *    Values are in the range (0..1).
 *    
 *    ufoRasterGetPixelCol() 
 *                  called on a RGB raster will return 
 *                              r = R
 *                              g = G
 *                              b = B
 *                  called on a A raster will return 
 *                              r = A
 *                              g = A
 *                              b = A
 *                  called on a RGBA raster will return 
 *                              r = R
 *                              g = G
 *                              b = B
 * 
 *    ufoRasterGetPixelAlpha() 
 *                  called on a RGB raster will return 
 *                              a = (R+G+B)/3
 *                  called on a A raster will return 
 *                              a = A
 *                  called on a RGBA raster will return 
 *                              a = A
 *    
 *    ufoRasterGetPixelColAlpha() 
 *                  called on a RGB raster will return 
 *                              r = R
 *                              g = G
 *                              b = B
 *                              a = 1
 *                  called on a A raster will return 
 *                              r = A
 *                              g = A
 *                              b = A
 *                              a = A
 *                  called on a RGBA raster will return 
 *                              r = R
 *                              g = G
 *                              b = B
 *                              a = A
 ******************************************************************************/
UFOAPI
void ufoRasterGetPixelCol(
    ufoRaster           raster_instance, 
    
    int                 x, 
    int                 y,
    
    double*             r, 
    double*             g, 
    double*             b);
    
UFOAPI
void ufoRasterGetPixelAlpha(
    ufoRaster           raster_instance, 
    
    int                 x, 
    int                 y,
    
    double*             a);
    
UFOAPI
void ufoRasterGetPixelColAlpha(
    ufoRaster           raster_instance, 
    
    int                 x, 
    int                 y,
    
    double*             r, 
    double*             g, 
    double*             b, 
    double*             a);
    
    
/*******************************************************************************
 * ufoRasterSetPixelCol():
 * ufoRasterSetPixelAlpha():
 * ufoRasterSetPixelColAlpha():
 *    Set the RGB, A or RGBA values for a raster, respectively.
 *    Values should be in the range (0..1).
 *    
 *    ufoRasterSetPixelCol() 
 *                  called on a RGB raster will return 
 *                              R = r
 *                              G = g
 *                              B = b
 *                  called on a A raster will return 
 *                              A = (r+g+b)/3
 *                  called on a RGBA raster will return 
 *                              R = r
 *                              G = g
 *                              B = b
 *                              A = 1
 * 
 *    ufoRasterSetPixelAlpha() 
 *                  called on a RGB raster will return 
 *                              R = a
 *                              G = a
 *                              B = a
 *                  called on a A raster will return 
 *                              A = a
 *                  called on a RGBA raster will return 
 *                              R = a
 *                              G = a
 *                              B = a
 *                              A = a
 * 
 *    ufoRasterSetPixelColAlpha() 
 *                  called on a RGB raster will return 
 *                              R = r
 *                              G = g
 *                              B = b
 *                  called on a A raster will return 
 *                              A = a
 *                  called on a RGBA raster will return 
 *                              R = r
 *                              G = g
 *                              B = b
 *                              A = a
 ******************************************************************************/
UFOAPI
void ufoRasterSetPixelCol(
    ufoRaster           raster_instance, 
    
    int                 x, 
    int                 y,
    
    double              r, 
    double              g, 
    double              b);
    
UFOAPI
void ufoRasterSetPixelAlpha(
    ufoRaster           raster_instance, 
    
    int                 x, 
    int                 y,
    
    double              a);
    
UFOAPI
void ufoRasterSetPixelColAlpha(
    ufoRaster           raster_instance, 
    
    int                 x, 
    int                 y,
    
    double              r, 
    double              g, 
    double              b, 
    double              a);

/*******************************************************************************
 * == new in XSI
 * ufoRasterClearPart():
 *      clears the raster to a given r,g,b,a value
 ******************************************************************************/

UFOAPI
void ufoRasterClearPart(
    ufoRaster           raster_instance, 
    double              r, 
    double              g, 
    double              b, 
    double              a,
    int                 x1, 
    int                 y1, 
    int                 x2, 
    int                 y2);

/*******************************************************************************
 * ufoRasterCopy():
 *      Copy from one raster to another. 
 *      Handles pixel conversion and intersection cropping.
 * ufoRasterCopyPart():
 *      Copy from one raster to another within a given area. 
 *      Handles pixel conversion and intersection cropping.
 ******************************************************************************/
UFOAPI
void ufoRasterCopy(
    ufoRaster           raster_instance_from, 
    ufoRaster           raster_instance_to);
   
UFOAPI
void ufoRasterCopyPart(
    ufoRaster           raster_instance_from, 
    ufoRaster           raster_instance_to, 
    int                 x1, 
    int                 y1, 
    int                 x2, 
    int                 y2);

/****************************************************************************************
 * ufoRasterCopyPart_AlphaOnly():
 *		Copy alpha only from one raster to another within a given area.
 *		Handles	pixel conversion and intersection cropping.
 * ufoRasterCopyPart_RGBOnly():
 *		Copy RGB only from one raster to another within a given area.
 *		Handles	pixel conversion and intersection cropping.
 ****************************************************************************************/
UFOAPI
void ufoRasterCopyPart_AlphaOnly(
	ufoRaster			raster_instance_from, 
	ufoRaster			raster_instance_to,	
	int					x1,	
	int					y1,	
	int					x2,	
	int					y2);

UFOAPI
void ufoRasterCopyPart_RGBOnly(
	ufoRaster			raster_instance_from, 
	ufoRaster			raster_instance_to,	
	int					x1,	
	int					y1,	
	int					x2,	
	int					y2);

/*******************************************************************************
 * == new in XSI
 * ufoRasterGetScanline():
 *      Get a scanline from a raster of any type into the given buffer
 *      handles all required image conversions
 *      the pixel at x1 is put in written at the begining of the buffer
 * ufoRasterPutScanline():
 *      Puts a scanline buffer to a raster of any type 
 *      handles all required image conversions
 ******************************************************************************/

UFOAPI
void ufoRasterGetScanline(
    ufoRaster           raster_instance_from, 
	int					y,
    int                 x1, 
    int                 x2,
    void			*	buffer, 
	ufoPixelType		buffer_pixel_type );

UFOAPI
void ufoRasterPutScanline(
    ufoRaster           raster_instance_to, 
	int					y,
    int                 x1, 
    int                 x2,
    const void		*	buffer, 
	ufoPixelType		buffer_pixel_type );

/*******************************************************************************
 * ufoRasterAlloc():
 *    Allocate raster of the given size and return new instance.
 * ufoRasterAllocCopy():
 *    allocate raster of the same size and type as existing raster and
 *    (optionally) copy the data.
 * ufoRasterAllocCopyPart():
 *    allocate raster of the same type as existing raster and of a defined
 *    size and (optionally) copy the relative data
 * ufoRasterFree():
 *    free previously allocated raster (using one of the above alloction
 *    functions). 
 * ufoRasterSetTitle():
 * ufoRasterGetTitle():
 * == new in XSI.  sets a title on a newly allocated raster so that it can 
 *    be easily identified with memory tracking tools.
 ******************************************************************************/

UFOAPI
ufoRaster ufoRasterAlloc(
    ufoPixelType        pixel_type, 
    int                 x1, 
    int                 y1, 
    int                 x2, 
    int                 y2);
   
UFOAPI
ufoRaster ufoRasterAllocCopy(
    ufoRaster           raster_instance, 
    int                 do_copy_data);
     
 
UFOAPI
ufoRaster ufoRasterAllocCopyPart(
    ufoRaster           raster_instance, 
    int                 x1, 
    int                 y1, 
    int                 x2, 
    int                 y2, 
    int                 do_copy_data);
   
UFOAPI
void ufoRasterFree(
    ufoRaster           raster_instance); 

UFOAPI
void ufoRasterSetTitle(
    ufoRaster           raster_instance,
	const char *        title ); 

UFOAPI
const char * ufoRasterGetTitle(
    ufoRaster           raster_instance ); 


/*******************************************************************************
 * ufoRasterConvertToSoftimageZValue():
 * used to convert a set of pixels to Softimage floating point Z values, 
 * using the inverse of the map used to fit Softimage .Zpic data into [0,1]
 * during file import.
 * 
 * N.B. result should point to a buffer large enough to hold n_pix floating
 * point values
 ******************************************************************************/

UFOAPI
void ufoRasterConvertToSoftimageZValue(
    ufoRaster           raster_instance,
    int                 x, 
    int                 y,
    int                 n_pix,
    float               *result);


#ifdef __cplusplus
}
#endif

#endif


/*******************************************************************************
 * End of file ufoRaster.h
 ******************************************************************************/
