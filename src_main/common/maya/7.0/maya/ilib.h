#ifndef __INCLUDE_ILIB_H__
#define __INCLUDE_ILIB_H__

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
**									     
**  Image library 
**
**  Name    : ilib.h		header file for use with the image library
**  Author  : A.Hervas
**  Version : Beta 1.01, Wed May 18th 1994
**
** ==========================================================================
** Copyright (C) 1995 - 2005 Alias Systems Corp. and/or its licensors.  All 
** rights reserved.
** 
** The coded instructions, statements, computer programs, and/or related 
** material (collectively the "Data") in these files contain unpublished 
** information proprietary to Alias Systems Corp. ("Alias") and/or its 
** licensors, which is protected by Canadian and US federal copyright law and 
** by international treaties.
** 
** The Data may not be disclosed or distributed to third parties or be copied 
** or duplicated, in whole or in part, without the prior written consent of 
** Alias.
** 
** THE DATA IS PROVIDED "AS IS". ALIAS HEREBY DISCLAIMS ALL WARRANTIES RELATING 
** TO THE DATA, INCLUDING, WITHOUT LIMITATION, ANY AND ALL EXPRESS OR IMPLIED 
** WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND/OR FITNESS FOR A 
** PARTICULAR PURPOSE. IN NO EVENT SHALL ALIAS BE LIABLE FOR ANY DAMAGES 
** WHATSOEVER, WHETHER DIRECT, INDIRECT, SPECIAL, OR PUNITIVE, WHETHER IN AN 
** ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, OR IN EQUITY, 
** ARISING OUT OF ACCESS TO, USE OF, OR RELIANCE UPON THE DATA.
** ==========================================================================
**
*****************************************************************************/

#include "flib.h"

#ifndef IMAGE_EXPORT /* for fcheck and irix */
#define IMAGE_EXPORT
#endif
/*
	NAME
	    ILintro - Introduction to the image library

	DESCRIPTION
		
		The IO image library is part of the flib library. 
		A set of routines allows to read and write images in a 
		structured file. 

	FILE FORMAT
		The format of an image file is very flexible. 
		Constraints on the number and relative position of the different 
		blocks are minimal and often purely of a logical nature 
		(for example, "the header must come before the pixel blocks" 
		rather than more static constraints such as "the header begins 
		at offset 124"). 

		An image file being first and foremost a file 
		(as far as flib is concerned) the user is free to insert extra 
		blocks. A minimal image is composed of a FOR4 group 
		(aligned to a word) of CIMG type containing, in the following order: 

	    - a BMHD header (bitmap header) 
	    - a FOR4 group of type TBMP (tiled bitmap)

		Pixel information is contained in the TBMP group, which can be 
		quickly skipped if necessary.

		In its minimal version, the TBMP group contains pixel related 
		data blocks, in some order. For example, for a picture divided 
		in four tiles we have: 


	    FOR4 <taille> CIMG
	      BMHD 24 ... definition of size, maps, etc...
	      FOR4 <taille2> TBMP
	        RGBA <ttile1> ... pixels of tile 1 ...
	        RGBA <ttile2> ... pixels of tile 2 ...
	        RGBA <ttile3> ... pixels of tile 3 ...
	        RGBA <ttile4> ... pixels of tile 4 ...

		The header is defined by structure ILheader. 
		RGBA blocks have the following structure: 

  	    [x1, y1, x2, y2] : tile coordinates (2 bytes each)
	    [pixels] : encoded according to compression mode.

	    If the image has a z-buffer, it is described by ZBUF blocks with the 
		same structure as the RGBA blocks, RLE encoded.
		NOTE: See TrenderFrame::writeToFile() for more information on how the
		depth is encoded by Maya.
		
		An IFF field typically also contains end fields.
		Calls to FLIB after image loading can be used to check for and 
		retrieve this data. ILIB ignores these fields. The following four 
		fields provide information about the image: 

			HIST: string data giving the Maya command line from which this 
				  image was created. 

			VERS: string data giving the Maya cut information. 

			CLPZ: depth map specific field giving the clipping planes used. 
				  This information is stored as two float values.

			ESXY: eye x-y ratios. This is a depth map specific field used to 
			      compute the xy eye coordinates from the normalized pixel 
				  coordinates stored as two float values.

		NOTE: 
			The ILIB library does not support these end fields.

		[claforte Oct 12 2001 NOTE: Why the heck wasn't that included as part
		of the header at the beginning of the file? We have to read all of 
		the file to get that information, that's stupid and inefficient.]


	TYPICAL EXAMPLE
		Maya produces IFF files whose RGBA and ZBUF tiles are interlaced.
		Note that fcheck doesn't currently translate tags from big-endian
		to little-endian machine. The end result is that on the Intel
		architecture, the tag names are inverted. (ie: FUBZ instead of ZBUF)
		Here is an example:

			C:\TEMP> fcheck -acv hasDepth.iff

			File : hasDepth.iff
			4ROF   125860 GMIC
			.REVF        4
			.DHBT       32     320x240 Color RLE, 8 bits with Mask, ZBuf, 8 tiles
			.HTUA       19     rjones on PC-RJONES
			.ETAD       24     Thu Oct 11 12:55:13 2001
			.4ROF   125524 PMBT
			..ABGR    14488     x1:  160  y1:    0  x2:  239  y2:  119
			..FUBZ    14720     x1:  160  y1:    0  x2:  239  y2:  119
			..ABGR      620     x1:  240  y1:    0  x2:  319  y2:  119
			..FUBZ      608     x1:  240  y1:    0  x2:  319  y2:  119
			..ABGR    16153     x1:  160  y1:  120  x2:  239  y2:  239
			..FUBZ    14760     x1:  160  y1:  120  x2:  239  y2:  239
			..ABGR      618     x1:  240  y1:  120  x2:  319  y2:  239
			..FUBZ      608     x1:  240  y1:  120  x2:  319  y2:  239
			..ABGR      680     x1:    0  y1:    0  x2:   79  y2:  119
			..FUBZ      608     x1:    0  y1:    0  x2:   79  y2:  119
			..ABGR    15485     x1:   80  y1:    0  x2:  159  y2:  119
			..FUBZ    14764     x1:   80  y1:    0  x2:  159  y2:  119
			..ABGR      648     x1:    0  y1:  120  x2:   79  y2:  239
			..FUBZ      608     x1:    0  y1:  120  x2:   79  y2:  239
			..ABGR    15319     x1:   80  y1:  120  x2:  159  y2:  239
			..FUBZ    14695     x1:   80  y1:  120  x2:  159  y2:  239
			.ZPLC        8
			.YXSE        8
			.TSIH      138     mayaBatch -render -n 1 -proj default -A 
							   pc-rjones.tor.aw.sgi.com:7835 
							   -p hasDepth 
							   E:/rjones/maya/.../hasDepth.mb
			.SREV       21     Maya cut 200105302318


	FUNCTIONS
	    Some functions allow the reading and writing of images in line 
		to line mode without worrying about tile management. Images can also 
		be automatically zoomed and/or corrected (by lookup) during read 
		(correction on compressed data being significantly more efficient). 
		Finally, an automatic conversion system makes it possible to read images 
		stored under other formats. 

		For more details on routines from the image library, see the man pages as well 
		as includes and examples provided with the library. 

*/

typedef	int (*ILgfunc)(void *, const void **, const float **);

typedef struct _ILheader
{
    unsigned int	width;
    unsigned int	height;
    unsigned short	prnum;
    unsigned short	prden;
    unsigned int	flags;
    unsigned short	bytes;
    unsigned short	tiles;
    unsigned int	compress;
    int		orgx;
    int		orgy;

} ILheader;

typedef struct _ILpage
{
    FLnode	node;
    unsigned int	fpos;
    unsigned int	size;
    unsigned short	coords[4];

} ILpage;

typedef struct _ILfkern
{
    uint	n;
    int		off;
    int *	w;
    
} ILfkern;

typedef struct _ILfint
{
    FLnode	node;
    int		use;
    float	rad, min, max;
    float *	tab;
    
} ILfint;

typedef struct _ILzoom
{
    FLnode	node;	
    FLmkey	memory;
    int		updated;
    uint	w,h;
    uint	zw,zh;
    int		extend;
    float	xblur;
    float	yblur;
    float	xoff;
    float	yoff;
    uint	lw,clw;
    int		degen;
    ILfint *	xshape;
    ILfint *	yshape;
    ILfint *	s_xshape;
    ILfint *	s_yshape;
    uint *	cbuf;
    float *	zbuf;
    int *	xmap;
    int		curay;
    uint	y,ay,ny;
    ILfkern*	xfilt;
    ILfkern*	yfilt;
    uint	nrows;
    int **	fcrows;
    float **	fzrows;
    int *	acrow;
    float *	azrow;
    void *	id;
    ILgfunc	gfunc;
    uint	iavail;
    int *	iptr;
    int		gback;
    float	xlm,ybm;
    float	xrm,ytm;
    
} ILzoom;

typedef struct _ILcdef
{
    uint	    width;
    uint	    height;
    uint	    depth;
    uint	    mask;
    uint	    ignore;
    uint	    channels;
    uint	    stride;
    uint	    btrunc;
    uint	    size;
    const unsigned char **  lookups;
    
} ILcdef;

typedef	int (*ILcfunc)(void *, const void *, const ILcdef *);

typedef struct _ILimage
{
    FLfile		fp;

    int			version;
    ILheader		bmhd;	    /*	bitmap header		    */
    unsigned int		ictl;	    /*	image control field	    */
    
    int			twidth;	    /*	tiling			    */
    int			theight;
    
    FLfunc		auxfunc;    /*	callback for aux blocks	    */
    FLfunc		usrfunc;    /*	callback for user blocks    */
    
    unsigned int		bitmask;    /*	mask of active channels	    */
    int			channels;   /*	nb of color channels	    */
    int			pwidth;	    /*	pixel width in bytes	    */
    int			lwidth;	    /*	line width in bytes	    */

	float   	dpi;		/* dots per inch		*/
    
    char *		line;	    /*	line buffer in line mode    */
    float *		zline;	    /*	z-line in line mode	    */
    int			waterline;  /*	nb of lines in write buffer */
    int			startline;  /*	# of the first line in wbuf */
   
    FLlist		pages;	    /*	page buffer		    */
    int			pbound;	    /*	page boundary		    */
    int			pheight;    /*	page height		    */
    
    void *		color;	    /*	color buffer for line mode  */
    float *		zbuffer;    /*	z-buffer for line mode	    */
    
    int			samples;    /*	# of samples in page buffer */
    int			zsamples;   /*	same for Z page buffer	    */
    
    const unsigned char **	lookups;
    ILcfunc		cfunc;
    ILzoom *		zoom;
    
    FLlist		fields;
    int			headerTellPos;
    
} ILimage;

#define	IL_DefaultVersion   FLmakeversion(1,0)
#define	IL_Version	    FLmakeversion(1,1)

/*	builtin compressors	    */

#define	ILC_None	0
#define	ILC_RLE		1
#define	ILC_QRL		2
#define	ILC_QR4		3

#define	ILC_Builtin	31
#define	ILC_User	32
#define	ILC_Max		63

#define	ILC_Default	ILC_RLE

/*	Bitmap header's flags	    */

#define	ILH_RGB		0x00000001
#define	ILH_Alpha	0x00000002
#define	ILH_Zbuffer	0x00000004
#define	ILH_Abuffer	0x00000008
#define	ILH_Black	0x00000010
#define	ILH_Type	0x0000001f

#define	ILH_RGBA	(ILH_RGB | ILH_Alpha)
#define	ILH_RGBAZ	(ILH_RGB | ILH_Alpha | ILH_Zbuffer)

/*	Reserved for mapping	    */

#define	ILH_Reserved	0xffff0000
#define	ILH_LBuffer	0x00020000
#define	ILH_RBump	0x00100000
#define	ILH_GBump	0x00200000
#define	ILH_BBump	0x00400000
#define	ILH_ABump	0x00800000
#define	ILH_TxFlags	0x00f20000

/*	Control flags		    */

#define	ILF_HdrOK	0x00000001	/*  Header read or written	*/
#define	ILF_PageIO	0x00000002	/*  set when writing TBMP	*/
#define	ILF_LineIO	0x00000004	/*  set in line mode		*/
#define	ILF_LBLoaded	0x00000008	/*  long buffer loaded (maps)	*/
#define	ILF_Finish	0x00000010	/*  TBMP written and closed	*/
#define	ILF_Forced	0x00000020	/*  switches auto-tiling off	*/
#define	ILF_Extended	0x00000080	/*  16 extension of 8 bits img	*/
#define	ILF_Reading	0x00001000	/*  r or r+ mode		*/
#define	ILF_SysInit	0x000007ff

/*  User modifiable control flags   */

#define	ILF_User	0xffff0000
#define	ILF_Updown	0x00010000	/*  line are r/w from top	*/
#define	ILF_Vertical	0x00020000	/*  not yet implemented		*/
#define	ILF_Pack	0x00040000	/*  do not expand to RGBA	*/
#define	ILF_Full	0x00100000	/*  r/w in 16 bits format	*/
#define	ILF_NoMask	0x00200000	/*  Ignore mask when reading	*/ 

/*	Status of pages in cache    */

#define ILP_Loaded	0x00000001
#define ILP_Expanded	0x00000002
#define ILP_Zpage	0x00000004

/*	Indexes of corners	    */

#define	IL_TX1		0
#define	IL_TY1		1
#define	IL_TX2		2
#define	IL_TY2		3

/*	flags for 16 bit	    */

#define ILE_None	0
#define ILE_In		1
#define ILE_Out		2
#define ILE_InOut	3

/*	flags for ILconfig	    */

#define ICFG_NoCvt	0x00000001
#define ICFG_PipeCvt	0x00000002
#define ICFG_FifoCpr	0x00000004
#define ICFG_NoFCache	0x00000008

/*	Field types and qualifiers  */

#define	IL_FByte	0x01
#define	IL_FHalf	0x02
#define	IL_FWord	0x04
#define	IL_FFloat	0x04
#define	IL_FDouble	0x08

#define	IL_FType	FLmakeid(0, 0, 0, 0x0f)

#define	IL_FVolatile	0x00
#define	IL_FPermanent	0x10

#define	IL_FQual	FLMakeid(0, 0, 0, 0xf0)

/*	some actually used fields   */

#define	IL_FFormat	FLmakeid('F', 'M', 'T', IL_FByte)

/*
**	function prototypes
**
*****************************************************************************/

extern	IMAGE_EXPORT	const unsigned char	ildeflut[256];

extern	IMAGE_EXPORT ILimage *	ILopen(const char *, const char *);
extern	IMAGE_EXPORT	ILimage *	ILreopen(const char *, const char *, ILimage *);
extern	IMAGE_EXPORT int		ILclose(ILimage *);
extern	IMAGE_EXPORT	int		ILnext(ILimage *);

extern	IMAGE_EXPORT void		ILinfilter(const char *);
extern	IMAGE_EXPORT	void		ILoutfilter(const char *);

extern	IMAGE_EXPORT	int		ILrange(const char *, float *, char *, float *);

extern	IMAGE_EXPORT	ILheader *	ILgetheader(ILimage *);

extern	IMAGE_EXPORT int		ILgetsize(ILimage *, int *, int *);
extern	IMAGE_EXPORT int		ILgetorigin(ILimage *, int *, int *);
extern	IMAGE_EXPORT	int		ILgetratio(ILimage *, int *, int *);
extern	IMAGE_EXPORT int		ILgettype(ILimage *);
extern	IMAGE_EXPORT int		ILgetbpp(ILimage *);
extern	IMAGE_EXPORT	int		ILgetcompress(ILimage *);
extern	IMAGE_EXPORT	int		ILgetpages(ILimage *);

extern	IMAGE_EXPORT int		ILsetsize(ILimage *, int, int);
extern	IMAGE_EXPORT int		ILsetorigin(ILimage *, int, int);
extern	IMAGE_EXPORT int		ILsettype(ILimage *, int);
extern	IMAGE_EXPORT	int		ILsetratio(ILimage *, int, int);
extern	IMAGE_EXPORT int		ILsetbpp(ILimage *, int);
extern	IMAGE_EXPORT	int		ILsetcompress(ILimage *, int);
extern	IMAGE_EXPORT	int		ILsetpages(ILimage *, int);
extern	IMAGE_EXPORT	int		ILsetpagesize(ILimage *, int, int);
extern	IMAGE_EXPORT	int		ILsetlookups(ILimage *, const unsigned char **);

extern	IMAGE_EXPORT int		ILgetline(ILimage *, const void **, const float **);
extern	IMAGE_EXPORT int		ILreadline(ILimage *, void *, float *);

extern	IMAGE_EXPORT	int		ILsetzoom(ILimage *, const char *, int, int,
				float, float, float, float);
extern	IMAGE_EXPORT	void		ILdefaultzoom(const char *);

extern	IMAGE_EXPORT	ILzoom *	ILnewzoom(const char *);
extern	IMAGE_EXPORT	void		ILsetzoomblur(ILzoom *, float, float);
extern	IMAGE_EXPORT	void		ILsetzoomoffset(ILzoom *, float, float);
extern	IMAGE_EXPORT	void		ILsetzoommargin(ILzoom *, float, float, float, float);
extern	IMAGE_EXPORT	void		ILdelzoom(ILzoom *);
extern	IMAGE_EXPORT	int		ILbindzoom(ILzoom *, void *, ILgfunc, int, int, int,
				int, int);
extern	IMAGE_EXPORT	int		ILreadzoom(ILzoom *, void *, float *);
extern	IMAGE_EXPORT	int		ILgetzoom(ILzoom *, const void **, const float **);
extern  IMAGE_EXPORT int     ILputheader(ILimage *img);

extern	IMAGE_EXPORT int		ILwriteline(ILimage *, const void *, const float *);
extern	IMAGE_EXPORT	void *		ILbgnline(ILimage *, float **);
extern	IMAGE_EXPORT	int		ILendline(ILimage *);

extern	IMAGE_EXPORT	int		ILwritetile(ILimage *, const void *, const float *,
			    const int *, int);
extern	IMAGE_EXPORT	int		ILendtile(ILimage *);

extern	IMAGE_EXPORT void		ILconfig(int, int);
extern	IMAGE_EXPORT int		ILctrl(ILimage *, int, int);

extern	IMAGE_EXPORT	void		ILaddcompressor(int, const char *, ILcfunc, ILcfunc);
extern	IMAGE_EXPORT	void		ILfindcompressor(int, char *, ILcfunc *, ILcfunc *);

extern	IMAGE_EXPORT	int		ILpack(void *, const void *, const ILcdef *);
extern	IMAGE_EXPORT	int		ILpackrle(void *, const void *, const ILcdef *);
extern	IMAGE_EXPORT	int		ILpackqrl(void *, const void *, const ILcdef *);
extern	IMAGE_EXPORT	int		ILpackqr4(void *, const void *, const ILcdef *);
extern	IMAGE_EXPORT int		ILunpack(void *, const void *, const ILcdef *);
extern	IMAGE_EXPORT int		ILunpackrle(void *, const void *, const ILcdef *);
extern	IMAGE_EXPORT int		ILunpackqrl(void *, const void *, const ILcdef *);
extern	IMAGE_EXPORT	int		ILunpackqr4(void *, const void *, const ILcdef *);

extern	IMAGE_EXPORT void		ILfill(uint *, int, int, int, int, int);

extern	IMAGE_EXPORT	int		ILloadbm(ILimage *, void *, float *);
extern	IMAGE_EXPORT	int		ILload(ILimage *, void *, float *);
extern	IMAGE_EXPORT	int		ILsave(ILimage *, const void *, const float *, int);

extern	IMAGE_EXPORT	int		ILgetfield(ILimage *, FLid, const void **);
extern	IMAGE_EXPORT	int		ILsetfield(ILimage *, FLid, uint size, const void *);
extern	IMAGE_EXPORT int		ILcopyfields(ILimage *dest, ILimage *src);

/*
**  Private functions
*/

extern	IMAGE_EXPORT	void		ILinitio(ILimage *);
extern	IMAGE_EXPORT	void		ILinitfields(ILimage *);
extern	IMAGE_EXPORT	int		ILreadfields(ILimage *);
extern	IMAGE_EXPORT	int		ILwritefields(ILimage *);
extern	IMAGE_EXPORT	void *		ILgetpath(void);
extern	IMAGE_EXPORT	int		ILrebindzoom(ILzoom *);

/*
**  temporary
*/

extern	IMAGE_EXPORT	int		ILqblur(uint *, const uint *, int, int, float);

/*
**  under development
*/

extern	IMAGE_EXPORT	int		ILdfft1x(float *, const float *, int);
extern	IMAGE_EXPORT	int		ILifft1x(float *, const float *, int);
extern	IMAGE_EXPORT	int		ILdfft1f(float *, const float *, int);
extern	IMAGE_EXPORT	int		ILifft1f(float *, const float *, int);
extern	IMAGE_EXPORT	void		ILqft(float *, float *, int);

extern	IMAGE_EXPORT	int		ILcload(ILimage *, void *, int, int, int, int, 
			    float, float, int);

#if (BYTE_ORDER == LITTLE_ENDIAN)
extern	IMAGE_EXPORT  void	ConvertABGRtoBGRA(void *dest, const ILcdef *cd);
extern	IMAGE_EXPORT  void	ConvertBGRAtoABGR(void *dest, const ILcdef *cd);
#endif 
#ifdef __cplusplus
}
#endif
#endif

