/******************************************************************************
 * Copyright 2005-2006 by mental images GmbH, Fasanenstr. 81, D-10623 Berlin
 * All rights reserved.
 ******************************************************************************
 * Author:	thomas
 * Created:	16.08.2005
 * Purpose:	standard header file for all mental ray shaders
 *
 * Note: This file is generated automatically from the mental ray sources, do
 * not edit. Some definitions exist for internal reasons and are subject to
 * change between mental ray versions. See the mental ray user's manual for
 * a definitive description of the shader interface. Avoid using features not
 * documented in the manual because they may change in future versions.
 *****************************************************************************/

#include <stdio.h>
#include <math.h>

#ifndef SHADER_H
#define SHADER_H

#define RAY2			/* for shaders that share sources with 1.9 */

#ifdef __cplusplus
extern "C" {
#endif


/*------------ mi_raylib.h ------------*/

/*begin_matter_base_h*/
typedef enum {miFALSE=0, miTRUE=1}	miBoolean;
typedef unsigned char			miCBoolean;	/* if space is tight */
typedef int				miInteger;	/* for shader params */
typedef signed char			miSint1;
typedef unsigned char			miUint1;
typedef signed short			miSint2;
typedef unsigned short			miUint2;
typedef signed int			miSint4;
typedef unsigned int			miUint4;
typedef unsigned int			miUint;
typedef unsigned char			miUchar;
typedef unsigned short			miUshort;

#if defined(_WIN64) || (defined(WIN_NT) && defined(BIT64))
typedef unsigned __int64		miUlong;
typedef signed   __int64		miSlong;
#else
typedef unsigned long			miUlong;
typedef signed   long			miSlong;
#endif

typedef signed char			miSchar;
typedef float				miScalar;
typedef double				miGeoScalar;
typedef struct {miScalar u, v;}		miVector2d;
typedef struct {miScalar x, y, z;}	miVector;
typedef struct {miGeoScalar u, v;}	miGeoVector2d;
typedef struct {miGeoScalar x, y, z;}	miGeoVector;
typedef struct {miScalar x, y, z, w;}	miQuaternion;
typedef miScalar			miMatrix[16];
typedef struct {float r, g, b, a;}	miColor;
typedef struct {miScalar min, max;}	miRange;
typedef struct {miGeoScalar min, max;}	miGeoRange;
typedef unsigned short			miIndex;
typedef unsigned int			miGeoIndex;
typedef miBoolean			(*miFunction_ptr)(void);
typedef union {void *p; double d;}	miPointer;	/* 8-byte aligned ptr*/
typedef miUint 				miTag;
typedef miSint4				miThreadID;
typedef miSint4				miHostID;
typedef miSint4				miSessionID;
typedef miSint4				miTransactionID;
typedef miSint4				miWorldID;

struct mi_lock;
typedef union {
	struct	mi_lock *milockptr;
	int	padding[2];
} miLock;
/*end_matter_base_h*/
/*begin_matter_base_h*/
#if defined(_WIN64) || (defined(WIN_NT) && defined(BIT64)) /* int wide enough */
typedef unsigned __int64		miIntptr;	/* for a pointer.  */
#else							/* 64-bit NT will  */
typedef unsigned long			miIntptr;	/* have 32-bit     */
#endif							/* longs...        */

#define miSCALAR_EPSILON		(1.0e-5f)
#define miGEO_SCALAR_EPSILON		(1e-9)
#define miHUGE_SCALAR			(1e36f)	/* don't require <limits.h> */
#define miHUGE_INT			(2147483647)
#define miNULL_INDEX			(0xffff) /* null value for miIndex */
#define miMAX_INDEX			(0xfffe) /* largest valid miIndex */
#define miNULL_GEOINDEX			(0xffffffff) /* null miGeoIndex */
#define miMAX_GEOINDEX			(0xfffffffe) /* largest miGeoIndex */

typedef enum {
	miM_NULL = 0,		/* not a legal module, wildcard value */
	miM_MSG,		/* message passing */
	miM_PAR,		/* message routing and event loops */
	miM_MEM,		/* memory manager */
	miM_DB,			/* database module */
	miM_PHEN,		/* phenomenon database */
	miM_RCRM,		/* rendering core: rasteriser */
	miM_SCENE,		/* common scene operations */
	miM_RC,			/* rendering core */
	miM_GAPMI,		/* geometry approximation (GAP) */
	miM_IMG,		/* image file read/write */
	miM_ECHO,		/* scene echo to .mi file */
	miM_MI,			/* mi file translator */
	miM_RCLM,		/* light mapping module */
	miM_SBS,		/* shape-by-shading */
	miM_RCH,		/* Hair rendering */
	miM_IMATTER,		/* imatter C++ interface (iraylib module) */
	miM_GEOSI,		/* surface / surface intersection */
	miM_TEST,		/* not used in raylib, for test beds only */
	miM_LINK,		/* dynamical linking, function symbol table */
	miM_P2SD,		/* polygonal import */
	miM_DISP,		/* realtime image displayer */
	miM_LPROF,              /* light profile support (ray 3.1) */
	miM_JOB,		/* job manager (ray 3.0) */
	miM_MAIN,		/* all front-ends with main() */
	miM_GEOMI,		/* geometry service library (GEO) */
	miM_EXR,		/* ILM OpenEXR image file library */
	miM_TRANS,		/* TRANS transaction handling */
	miM_INVENTOR,		/* SGI Inventor translator */
	miM_LIB,		/* library module, error messages */
	miM_API,		/* high-level app programming interface */
	miM_SPDL,		/* Obsolete: Softimage SPDL phenom. parser */
	miM_PHCR,		/* shader editor GUI */
	miM_RCB,		/* RC boxing */
	miM_RCC,		/* RC contours */
	miM_RCI,		/* RC intersection */
	miM_RCGI,		/* RC global illumination and caustics */
	miM_RCHW,	        /* RC hardware rendering */
	miM_SIMP,		/* Mesh simplification */
	miM_SURF,		/* subdivision surfaces */
	miM_CAPI,		/* class api module */
	miM_GAPAD,		/* GAP adjacency detection and edge merging */
	miM_GAPCU,		/* GAP curves */
	miM_GAPDI,		/* GAP displacement mapping */
	miM_GAPIS,		/* GAP isoline segments */
	miM_GAPPO,		/* GAP polygons */
	miM_GAPFA,		/* GAP faces */
	miM_GAPTE,		/* GAP tessellation */
	miM_GEOCO,		/* GEO conditioning */
	miM_GEOEV,		/* GEO evaluation */
	miM_GEOMO,		/* GEO modelling */
	miM_GEOST,		/* GEO strips */
	miM_GEOTE,		/* GEO tessellation */
	miM_JPG,		/* JPEG compression/decompression */
	miM_RCFG,		/* Final gathering */
	miNMODULES		/* # of modules (not a module itself) */
} miModule;
/*end_matter_base_h*/
/*begin_matter_base_h*/
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2	0.70710678118654752440
#endif
/*end_matter_base_h*/
/*begin_matter_base_h*/
#define RAY2			/* for shaders that share sources with 1.9 */
#define RAY3			/* for shaders that share sources with 2.x */
#define RAY31                   /* for shaders that share sources with 3.0 */
#define RAY32                   /* for shaders that share sources with 3.1 */
#define RAY33                   /* for shaders that share sources with 3.2 */
#define RAY34                   /* for shaders that share sources with 3.4 */
#ifdef _WIN32
#define DLLIMPORT __declspec(dllexport)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLIMPORT
#define DLLEXPORT
#endif
/*end_matter_base_h*/

/*------------ mi_lib.h ------------*/

/*begin_matter_base_h*/
/*
 * vector operations. MI_SQRT works for ANSI C compilers. If it causes trouble,
 * #define MI_SQRT sqrt before including this file. IBM and HP mess up as usual.
 */

#ifndef MI_SQRT
#ifdef IRIX
#define MI_SQRT sqrtf
#else
#define MI_SQRT sqrt
#endif
#endif

#define mi_vector_neg(r)	((r)->x = -(r)->x,\
				 (r)->y = -(r)->y,\
				 (r)->z = -(r)->z)

#define mi_vector_add(r,a,b)	((r)->x = (a)->x + (b)->x,\
				 (r)->y = (a)->y + (b)->y,\
				 (r)->z = (a)->z + (b)->z)

#define mi_vector_sub(r,a,b)	((r)->x = (a)->x - (b)->x,\
				 (r)->y = (a)->y - (b)->y,\
				 (r)->z = (a)->z - (b)->z)

#define mi_vector_mul(r,f)	((r)->x *= (f),\
				 (r)->y *= (f),\
				 (r)->z *= (f))

#define mi_vector_div(r,d)	do {if ((d) != 0.0f)\
					{register miScalar _i=1.0f/(d);\
						 (r)->x *= _i;\
						 (r)->y *= _i;\
						 (r)->z *= _i; }} while (0)

#define mi_vector_div_d(r,d)	do {if ((d) != 0.0)\
					{register double _i = 1.0/(d);\
						 (r)->x *= _i;\
						 (r)->y *= _i;\
						 (r)->z *= _i; }} while (0)

#define mi_vector_prod(r,a,b)	((r)->x = (a)->y * (b)->z - (a)->z * (b)->y,\
				 (r)->y = (a)->z * (b)->x - (a)->x * (b)->z,\
				 (r)->z = (a)->x * (b)->y - (a)->y * (b)->x)

#define mi_vector_dot(a,b)	((a)->x * (b)->x +\
				 (a)->y * (b)->y +\
				 (a)->z * (b)->z)

#define mi_vector_dot_d(a,b)	((double)(a)->x * (double)(b)->x +\
				 (double)(a)->y * (double)(b)->y +\
				 (double)(a)->z * (double)(b)->z)

#define mi_vector_norm(r)	(miScalar)MI_SQRT((r)->x * (r)->x +\
					(r)->y * (r)->y +\
					(r)->z * (r)->z)

#define mi_vector_norm_d(r)	sqrt((double)(r)->x * (double)(r)->x +\
				     (double)(r)->y * (double)(r)->y +\
				     (double)(r)->z * (double)(r)->z)

#define mi_vector_normalize(r)	do { register miScalar _n = mi_vector_norm(r);\
				     mi_vector_div(r, _n); } while (0)

#define mi_vector_normalize_d(r) do { register double _n= mi_vector_norm_d(r);\
				     mi_vector_div_d(r, _n); } while (0)

#define mi_vector_min(r,a,b)	((a)->x < (b)->x ? ((r)->x = (a)->x) \
						 : ((r)->x = (b)->x),\
				 (a)->y < (b)->y ? ((r)->y = (a)->y) \
						 : ((r)->y = (b)->y),\
				 (a)->z < (b)->z ? ((r)->z = (a)->z) \
						 : ((r)->z = (b)->z))

#define mi_vector_max(r,a,b)	((a)->x > (b)->x ? ((r)->x = (a)->x) \
						 : ((r)->x = (b)->x),\
				 (a)->y > (b)->y ? ((r)->y = (a)->y) \
						 : ((r)->y = (b)->y),\
				 (a)->z > (b)->z ? ((r)->z = (a)->z) \
						 : ((r)->z = (b)->z))

#define mi_vector_det(a,b,c)	((a)->x * ((b)->y * (c)->z - (c)->y * (b)->z)+\
				 (b)->x * ((c)->y * (a)->z - (a)->y * (c)->z)+\
				 (c)->x * ((a)->y * (b)->z - (b)->y * (a)->z))

#define mi_vector_dist(a,b)	MI_SQRT(((a)->x - (b)->x) * ((a)->x - (b)->x)+\
					((a)->y - (b)->y) * ((a)->y - (b)->y)+\
					((a)->z - (b)->z) * ((a)->z - (b)->z))

/*
 * matrix operations (libmatrix.c)
 */

#define	  mi_matrix_copy(t,s)		(void)memcpy((t),(s),sizeof(miMatrix))
#define	  mi_matrix_null(m)		(void)memset((m), 0, sizeof(miMatrix))

void		mi_matrix_ident		(miMatrix);
void		mi_matrix_prod		(miMatrix,
					 miMatrix const,
					 miMatrix const);
miBoolean	mi_matrix_isident	(miMatrix const);
miBoolean	mi_matrix_isnull	(miMatrix const);
float		mi_matrix_rot_det	(miMatrix const);
miBoolean	mi_matrix_invert	(miMatrix, miMatrix const);
void		mi_point_transform	(miVector * const,
					 miVector const * const,
					 miMatrix const);
void		mi_vector_transform	(miVector * const,
					 miVector const * const,
					 miMatrix const);
void		mi_vector_transform_T	(miVector * const,
					 miVector const * const,
					 miMatrix const);
void		mi_matrix_rotate	(miMatrix, const miScalar,
					 const miScalar, const miScalar);
void		mi_matrix_rotate_axis	(miMatrix, miVector const * const,
					 miScalar const);
miBoolean	mi_matrix_solve		(miScalar *, miMatrix const,
					 miScalar const *, int);
/*end_matter_base_h*/
/*begin_matter_base_h*/

miScalar	mi_spline		(miScalar, const int, miScalar *const);
miScalar	mi_random		(void);
miScalar	mi_erandom		(unsigned short seed[3]);
void		mi_srandom		(long newseed);
miScalar	mi_noise_1d		(const miScalar);
miScalar	mi_unoise_1d		(const miScalar);
miScalar	mi_noise_2d		(const miScalar, const miScalar);
miScalar	mi_unoise_2d		(const miScalar, const miScalar);
miScalar	mi_noise_3d		(miVector * const);
miScalar	mi_unoise_3d		(miVector * const);
miScalar	mi_noise_1d_grad	(const miScalar, miScalar * const);
miScalar	mi_unoise_1d_grad	(const miScalar, miScalar * const);
miScalar	mi_noise_2d_grad	(const miScalar, const miScalar,
					 miScalar * const, miScalar * const);
miScalar	mi_unoise_2d_grad	(const miScalar, const miScalar,
					 miScalar * const, miScalar * const);
miScalar	mi_noise_3d_grad	(miVector * const, miVector * const);
miScalar	mi_unoise_3d_grad	(miVector * const, miVector * const);


/*
 * error handler: severity levels for mi_set_verbosity (miERR_*), and
 * sub-levels to turn on individual message types per module (miMSG_*).
 */

/*end_matter_base_h*/
/*begin_matter_base_h*/
void		mi_fatal		(const char * const, ...);
void		mi_error		(const char * const, ...);
void		mi_warning		(const char * const, ...);
void		mi_info			(const char * const, ...);
void		mi_progress		(const char * const, ...);
void		mi_debug		(const char * const, ...);
void		mi_vdebug		(const char * const, ...);
void		mi_nfatal		(const int, const char * const, ...);
void		mi_nerror		(const int, const char * const, ...);
void		mi_nwarning		(const int, const char * const, ...);
char		*mi_strerror		(void);
/*end_matter_base_h*/
/*begin_matter_base_h*/

char *mi_string_substitute(	/* apply dos/unix etc path substns */
	char	*new_name,		/* place to copy mangled path */
	char	*name,			/* path to be mangled */
	long	max_new_name_len);	/* size of new_name[] */
/*end_matter_base_h*/
/*begin_matter_base_h*/
struct miState;
miScalar mi_luminance(struct miState *state, miColor *c);
/*end_matter_base_h*/
/*begin_matter_base_h*/
typedef enum {
	miQ_NONE			= 0,
	miQ_VERSION			= 1,
	miQ_DATE			= 2,
	miQ_INST_FUNCTION		= 3,
	miQ_INST_GLOBAL_TO_LOCAL	= 4,
	miQ_INST_LOCAL_TO_GLOBAL	= 5,
	miQ_INST_IDENTITY		= 6,
	miQ_INST_ITEM			= 7,
	miQ_INST_PLACEHOLDER_ITEM	= 153,
	miQ_INST_PARENT			= 8,
	miQ_INST_HIDE			= 9,
	miQ_INST_VISIBLE		= 90,
	miQ_INST_SHADOW			= 91,
	miQ_INST_TRACE			= 92,
	miQ_INST_CAUSTIC		= 93,
	miQ_INST_GLOBILLUM		= 111,
	miQ_INST_FINALGATHER		= 151,
	miQ_INST_DECL			= 10,
	miQ_INST_PARAM_SIZE		= 11,
	miQ_INST_PARAM			= 12,
	miQ_INST_MATERIAL		= 83,
	miQ_INST_LABEL			= 101,
	miQ_INST_DATA			= 102,
        miQ_INST_AREA			= 125, 
	miQ_GROUP_MERGE_GROUP		= 13,
	miQ_GROUP_NKIDS			= 14,
	miQ_GROUP_KID			= 15,
	miQ_GROUP_LABEL			= 103,
	miQ_GROUP_DATA			= 104,
	miQ_OBJ_TYPE			= 16,
	miQ_OBJ_VISIBLE			= 17,
	miQ_OBJ_TRACE			= 18,
	miQ_OBJ_SHADOW			= 19,
	miQ_OBJ_VIEW_DEPENDENT		= 20,
	miQ_OBJ_CAUSTIC			= 21,
	miQ_OBJ_GLOBILLUM		= 112,
	miQ_OBJ_FINALGATHER		= 150,
	miQ_OBJ_LABEL			= 22,
	miQ_OBJ_DATA			= 100,
	miQ_LIGHT_TYPE			= 23,
	miQ_LIGHT_AREA			= 24,
	miQ_LIGHT_EXPONENT		= 25,
	miQ_LIGHT_CAUSTIC_PHOTONS	= 26,
	miQ_LIGHT_CAUSTIC_PHOTONS_EMIT	= 135,
	miQ_LIGHT_CAUSTIC_PHOTONS_EMITTED	= 144,
	miQ_LIGHT_GLOBAL_PHOTONS	= 27,
	miQ_LIGHT_GLOBAL_PHOTONS_EMIT	= 136,
	miQ_LIGHT_GLOBAL_PHOTONS_EMITTED	= 145,
	miQ_LIGHT_ENERGY		= 28,
	miQ_LIGHT_SHADER		= 29,
	miQ_LIGHT_EMITTER		= 30,
	miQ_LIGHT_ORIGIN		= 31,
	miQ_LIGHT_DIRECTION		= 32,
	miQ_LIGHT_AREA_R_EDGE_U		= 33,
	miQ_LIGHT_AREA_R_EDGE_V		= 34,
	miQ_LIGHT_AREA_D_NORMAL		= 35,
	miQ_LIGHT_AREA_D_RADIUS		= 36,
	miQ_LIGHT_AREA_S_RADIUS		= 37,
	miQ_LIGHT_AREA_C_RADIUS		= 96,
	miQ_LIGHT_AREA_C_AXIS		= 97,
	miQ_LIGHT_AREA_SAMPLES_U	= 38,
	miQ_LIGHT_AREA_SAMPLES_V	= 39,
	miQ_LIGHT_SPREAD		= 40,
	miQ_LIGHT_USE_SHADOWMAP		= 41,
	miQ_LIGHT_LABEL			= 98,
	miQ_LIGHT_DATA			= 99,
	miQ_LIGHT_SAMPLE_COORD		= 147,
	miQ_MTL_OPAQUE			= 42,
	miQ_MTL_SHADER			= 43,
	miQ_MTL_DISPLACE		= 44,
	miQ_MTL_SHADOW			= 45,
	miQ_MTL_VOLUME			= 46,
	miQ_MTL_ENVIRONMENT		= 47,
	miQ_MTL_CONTOUR			= 48,
	miQ_MTL_PHOTON			= 49,
	miQ_MTL_PHOTONVOL		= 50,
	miQ_MTL_LIGHTMAP		= 146,
 	miQ_FUNC_USERPTR		= 51,
	miQ_FUNC_LOCK			= 52,
	miQ_FUNC_TYPE			= 53,
	miQ_FUNC_DECL			= 54,
	miQ_FUNC_NEXT			= 55,
	miQ_FUNC_INDIRECT		= 56,
	miQ_FUNC_PARAM_SIZE		= 57,
	miQ_FUNC_RESULT_SIZE		= 58,
	miQ_FUNC_PARAM			= 59,
	miQ_FUNC_TLS_GET		= 124,
	miQ_FUNC_TLS_SET		= 126,
	miQ_FUNC_TLS_GETALL		= 127,
	miQ_FUNC_CALLTYPE		= 140,
	miQ_FUNC_TAG			= 141,
	miQ_FUNC_LENS_RECALL		= 149,
	miQ_DECL_LOCK			= 60,
	miQ_DECL_TYPE			= 61,
	miQ_DECL_RESULT_SIZE		= 62,
	miQ_DECL_NAME			= 63,
	miQ_DECL_PARAM			= 64,
	miQ_DECL_VERSION		= 89,
	miQ_IMAGE_WIDTH			= 65,
	miQ_IMAGE_HEIGHT		= 66,
	miQ_IMAGE_BITS			= 67,
	miQ_IMAGE_COMP			= 68,
	miQ_IMAGE_FILTER		= 69,
	miQ_IMAGE_DESATURATE		= 85,
	miQ_IMAGE_DITHER		= 86,
	miQ_IMAGE_NOPREMULT		= 87,
	miQ_IMAGE_GAMMA			= 88,
	miQ_IMAGE_COLORCLIP		= 115,
	miQ_NUM_GLOBAL_LIGHTS		= 70,
	miQ_GLOBAL_LIGHTS		= 71,
	miQ_TRANS_INTERNAL_TO_WORLD	= 72,
	miQ_TRANS_INTERNAL_TO_CAMERA	= 73,
	miQ_TRANS_INTERNAL_TO_OBJECT	= 74,
	miQ_TRANS_WORLD_TO_INTERNAL	= 75,
	miQ_TRANS_CAMERA_TO_INTERNAL	= 76,
	miQ_TRANS_OBJECT_TO_INTERNAL	= 77,
	miQ_TRANS_WORLD_TO_CAMERA	= 78,
	miQ_TRANS_WORLD_TO_OBJECT	= 79,
	miQ_TRANS_CAMERA_TO_WORLD	= 80,
	miQ_TRANS_OBJECT_TO_WORLD	= 81,
	miQ_NUM_TEXTURES		= 82,
	miQ_NUM_BUMPS			= 137,
	miQ_GEO_LABEL			= 84,
	miQ_GEO_DATA			= 114,
	miQ_GEO_HAS_DERIVS		= 138,
	miQ_GEO_HAS_DERIVS2		= 139,
	miQ_PRI_BBOX_MIN		= 94,
	miQ_PRI_BBOX_MAX		= 95,
	miQ_DATA_PARAM			= 105,
	miQ_DATA_DECL			= 106,
	miQ_DATA_NEXT			= 107,
	miQ_DATA_LABEL			= 108,
	miQ_DATA_PARAM_SIZE		= 109,
	miQ_DATA_NEEDSWAP		= 110,
	miQ_TILE_PIXELS			= 116,
	miQ_TILE_SAMPLES		= 117,
	miQ_FINALGATHER_STATE		= 142,
	miQ_SCENE_BBOX_ALL		= 118,
	miQ_SCENE_BBOX_SHADOW		= 119,
	miQ_SCENE_BBOX_GLOBILLUM_G	= 120,
	miQ_SCENE_BBOX_GLOBILLUM_R	= 121,
	miQ_SCENE_BBOX_CAUSTIC_G	= 122,
	miQ_SCENE_BBOX_CAUSTIC_R	= 123,
        miQ_LIGHTPROFILE_PHI_MIN        = 128,
        miQ_LIGHTPROFILE_PHI_MAX        = 129,
        miQ_LIGHTPROFILE_COSTHETA_MIN   = 130,
        miQ_LIGHTPROFILE_COSTHETA_MAX   = 131,
        miQ_LIGHTPROFILE_INTENSITY_MAX  = 132,
        miQ_LIGHTPROFILE_PHI_RES        = 133,
        miQ_LIGHTPROFILE_THETA_RES      = 134,
	miQ_PIXEL_SAMPLE		= 143,
	miQ_SCENE_ROOT_GROUP		= 148,
	miQ_TEXTURE_DIM			= 152,
	miNQ				= 154
} miQ_type;

struct miState;
miBoolean	mi_query(const miQ_type, struct miState * const,
		 miUint, void * const, ...);
/*end_matter_base_h*/
					/*begin_matter_base_h*/
#define miSS_LIFETIME_EYERAY	1	/* for mi_shaderstate_set: delete */
#define miSS_LIFETIME_RECT	2	/* if eye ray is done, or sometime */
					/* later when pixel rect is finished */
miBoolean mi_shaderstate_set(
	struct miState	*state,		/* table is anchored in state->ts */
	const char	*key,		/* key to look up */
	void		*value,		/* value to store */
	int		valsize,	/* # of bytes in value */
	int		lifetime);	/* 0=subrays, 1=eye ray, 2=many rays */

void *mi_shaderstate_get(
	struct miState	*state,		/* table is anchored in state->ts */
	const char	*key,		/* key to look up */
	int		*valsize);	/* # of bytes in key (strlen/sizeof) */

void mi_shaderstate_enumerate(
	struct miState	*state,		/* table is anchored in state->ts */
	miBoolean	(*cb)(void *arg, char *key, void *val, int vsz, int l),
	void		*arg);		/* opaque data pointer for cb */
					/*end_matter_base_h*/
					/*begin_matter_base_h*/
typedef enum miKdtree_type {
	miKDTREE_UNKNOWN		= 0,
	miKDTREE_3D,
	miKDTREE_NTYPES 		= miKDTREE_3D
} miKdtree_type;


struct miKdtree;

typedef miBoolean ((miKdtree_insert_callback)
	(void 		*callback_data, /* user callback data */
	 void		*data,		/* element data */
	 miScalar	point[],	/* element location */
	 const void 	*shared_data)); /* data shared for inserting elements*/

typedef miBoolean ((miKdtree_lookup_callback)
	(void 		*callback_data, /* user callback data */
	 const void	*data,		/* element data */
	 const miScalar	point[]));	/* element location */

miTag mi_kdtree_create(
	miKdtree_type	type,		/* access type and dimenstion */
	miUint		datasize);	/* data size of the element */

struct miKdtree *mi_kdtree_edit(
	miTag		tag,			/* kdtree tag */
	const void	*shared_data,		/* data shared by elements */
	miUint		shared_data_size);	/* size of the data */

miBoolean mi_kdtree_add(
	struct miKdtree	*tree,		/* thread handle to the tree */
	const void	*data,		/* element data to store */
	const miScalar	point[]);	/* element location to store */

miBoolean mi_kdtree_unpin(
	struct miKdtree	*tree);		/* thread handle to the tree */

struct miKdtree *mi_kdtree_access(
	miTag			 tag,
	miKdtree_insert_callback *callback, /* modify/reject new elements*/
	void			 *callback_data); /* user callback data */

miUint mi_kdtree_lookup(
	struct miKdtree	*tree,	     /* thread handle to the tree */
	const miScalar	point[],     /* lookup position */
	miUint		max_n,	     /* max. number of elem. to look for */
	miScalar	max_dist,    /* max. distance to look for */
	miScalar	*max_dist_used,	   /* out: dist. to furtherst elem.*/
	miKdtree_lookup_callback *callback,/* callback function*/
	void		*callback_data);   /* user callback data */

miBoolean mi_kdtree_iterate(
	struct miKdtree	*tree,		   /* thread handle to the tree */
	miKdtree_lookup_callback *callback,/* callback function*/
	void		*callback_data);   /* user callback data */

miBoolean mi_kdtree_delete(
	miTag		tag);	/* Unpinned tree tag */
				/*end_matter_base_h*/

/*------------ mi_mem.h ------------*/

/*begin_matter_base_h*/
struct miMemblk;
typedef struct miMemblk miMemblk;
/*end_matter_base_h*/
/*begin_matter_base_h*/
#define mi_mem_blkcreate(s)	mi_mem_int_blkcreate	(__FILE__,__LINE__,s)
#define mi_mem_strdup(s)	mi_mem_int_strdup	(__FILE__,__LINE__,s)
#define mi_mem_release(m)	mi_mem_int_release	(__FILE__,__LINE__,m)
#define mi_mem_allocate(s)	mi_mem_int_allocate	(__FILE__,__LINE__,s)
#define mi_mem_test_allocate(s)	mi_mem_int_test_allocate(__FILE__,__LINE__,s)
#define mi_mem_reallocate(m,s)	mi_mem_int_reallocate	(__FILE__,__LINE__,m,s)
#define mi_mem_test_reallocate(m,s)	mi_mem_int_test_reallocate\
							(__FILE__,__LINE__,m,s)
/*end_matter_base_h*/
/*begin_matter_base_h*/
void		*mi_mem_int_allocate	   (const char * const, const int,
					    const int);
void		*mi_mem_int_test_allocate  (const char * const, const int,
					    const int);
void		*mi_mem_int_reallocate	   (const char * const, const int,
					    void *, const int);
void		*mi_mem_int_test_reallocate(const char * const, const int,
					    void *, const int);
void		 mi_mem_int_release	   (const char * const, const int,
					    void *);
/*end_matter_base_h*/
							/*begin_matter_base_h*/
void		 mi_mem_check		(void);
miUlong		 mi_mem_getsize		(void);
void		 mi_mem_summary		(void);
void		 mi_mem_dump		(const miModule);

char		*mi_mem_int_strdup	(const char * const, const int,
					 const char *);

/*
 * block allocator. note that the current internal block size is 8Kb, and
 * we assume that at least two items fit on a page. hence the largest block
 * you should use the block allocator for is around 4000 bytes. even in this
 * range the efficiency of using a block allocator is noticeably lower than
 * for smaller blocks. it is recommended to use the block allocator only up
 * to a block size of about 1Kb.
 */

miMemblk	*mi_mem_int_blkcreate	(const char * const, const int, int);
void		 mi_mem_blkdelete	(miMemblk *);
void		*mi_mem_blkallocate	(miMemblk *);
void		*mi_mem_blkfallocate	(miMemblk *);
void		 mi_mem_blkrelease	(miMemblk *, void *);
void		 mi_mem_blkfrelease	(miMemblk *, void *);
void		 mi_mem_blkclear	(miMemblk *);
void		 mi_mem_blkenumerate	(miMemblk *, void (*)(void *));
							/*end_matter_base_h*/

/*------------ mi_db.h ------------*/

					/*begin_matter_base_h*/

#define miNULLTAG 0
					/*end_matter_base_h*/
					/*begin_matter_base_h*/
void  *mi_db_access(
			const miTag);	/* tag to be accessed */
int    mi_db_type(
			const miTag);	/* tag to be accessed */
int    mi_db_size(
			const miTag);	/* tag to be accessed */
miBoolean mi_db_isjob(
			const miTag);	/* tag to be accessed */
void  *mi_db_access_type(
			int * const,	/* returned type, really miScene_type*/
			const miTag);	/* tag to be accessed */
void   mi_db_unpin(
			const miTag);	/* tag to be unpinned */
void  *mi_db_create(
			miTag * const,	/* returned new tag */
			const int,	/* miSCENE_* type to store */
			const int);	/* size of item in bytes */
void  *mi_db_realloc(
			const miTag,	/* tag to be resized */
			const int);		/* new size of the db item */
void   mi_db_delete(
			const miTag);	/* tag to be deleted */
miTag  mi_db_copy(
			const miTag);	/* source tag to duplicate */
void   mi_db_dump(
			const miModule);/* dump whose entries, 0=all */
					/*end_matter_base_h*/

/*------------ mi_img.h ------------*/

/*begin_matter_base_h*/
/*
 * supported internal scanline types. These types are used when a file is
 * opened and created; they specify the internal data structure but not the
 * type of the file to be created. If you change this list, also change
 * compatible_type[] in imgcall.c. The use as array indexes is the reason
 * why every enumerator is assigned explicitly, to show that the numbers
 * are important. No real type may exceed 31!
 * miIMG_TYPE_ANY is a wildcard that disables conversion. The USER types
 * are assigned real types in "frame buffer" statements. Defining them here
 * allows storing them in the out_type and out_interp bitmaps in miFunction.
 * No type should have a code >31 because output statements use type bitmaps.
 */

struct miColor_profile;

typedef enum {
	miIMG_TYPE_RGBA		= 0,	/* 4*8	bits, color with alpha */
	miIMG_TYPE_RGBA_16	= 1,	/* 4*16 bits, color with alpha */
	miIMG_TYPE_RGB		= 2,	/* 4*8	bits, color */
	miIMG_TYPE_RGB_16	= 3,	/* 4*16 bits, color */
	miIMG_TYPE_A		= 4,	/* 1*8	bits, alpha */
	miIMG_TYPE_A_16		= 5,	/* 1*16 bits, alpha */
	miIMG_TYPE_S		= 6,	/* 1*8	bits, intensity */
	miIMG_TYPE_S_16		= 7,	/* 1*16 bits, intensity */
	miIMG_TYPE_VTA		= 8,	/* 2*16 bits, bumpmap from alpha */
	miIMG_TYPE_VTS		= 9,	/* 2*16 bits, bumpmap from intensity */
	miIMG_TYPE_Z		= 10,	/* 1*32 bits, Z coordinates (float) */
	miIMG_TYPE_N		= 11,	/* 3*32 bits, normal vectors (float) */
	miIMG_TYPE_M		= 14,	/* 3*32 bits, motion vectors (float) */
	miIMG_TYPE_TAG		= 12,	/* 1*32 bits, material tags (u int) */
	miIMG_TYPE_BIT		= 15,	/* 1*1	bit,  must-render flag map */
	miIMG_TYPE_RGBA_FP	= 13,	/* 4*32 bits, color with alpha, float*/
	miIMG_TYPE_RGB_FP	= 16,	/* 3*32 bits, color, float*/
	miIMG_TYPE_COVERAGE	= 17,	/* 1*32 bits, coverage, float */
	miIMG_TYPE_RGBE		= 18,	/* 4*8  bits, HDR with common expon. */
	miIMG_TYPE_A_FP		= 19,	/* 1*16 bits, alpha */
	miIMG_TYPE_S_FP		= 20,	/* 1*32 bits, intensity */
	miIMG_TYPE_RGBA_H	= 21,	/* 4*16 bits, half float */
	miIMG_TYPE_RGB_H	= 22,	/* 3*16 bits, half float */
	miIMG_NTYPES		= 23,
					/* remainder is out-of-band types */
	miIMG_TYPE_CONT		= 29,	/* API/PHEN: contours */
	miIMG_TYPE_ANY		= 30,	/* img_open: don't convert */
	miIMG_TYPE_USER_FIRST	= 31	/* API/PHEN: user-defined types >= 0 */
} miImg_type;
/* The documented error return */
#define miIMG_TYPE_ERROR miIMG_NTYPES

/*end_matter_base_h*/
/*begin_matter_base_h*/


/*
 * supported file formats. These are used for one purpose only: when creating
 * an image file, they specify the output format. When adding or inserting
 * an entry, add external declarations and entries to all arrays defined as
 * "something[miIMG_NFORMATS]" at the beginning of imgdispatch.c. Make sure
 * the order is correct!
 * Generally, common formats should precede uncommon, and formats difficult
 * to recognize by their headers should come last (qnt, zpic). QNT_PAL must
 * be QNT_NTSC-1, and there may be only two QNT formats (see mi_img_open).
 */

typedef enum {
	miIMG_FORMAT_RLA	= 0,	/* Wavefront RLA */
	miIMG_FORMAT_RLB	= 1,	/* Wavefront RLB */
	miIMG_FORMAT_PIC	= 2,	/* Softimage pic, compressed */
	miIMG_FORMAT_ALIAS	= 3,	/* Alias */
	miIMG_FORMAT_RGB	= 4,	/* SGI RGB, compressed */
	miIMG_FORMAT_CT		= 5,	/* mi Color Texture */
	miIMG_FORMAT_CT_16	= 6,	/* mi Color Texture, 16 bits */
	miIMG_FORMAT_ST		= 7,	/* mi Scalar Texture */
	miIMG_FORMAT_ST_16	= 8,	/* mi Scalar Texture, 16 bits */
	miIMG_FORMAT_VT		= 9,	/* mi vector texture, from alpha */
	miIMG_FORMAT_WT		= 10,	/* mi vector texture, from intensity */
	miIMG_FORMAT_ZT		= 11,	/* mi Z channel, from intersect depth*/
	miIMG_FORMAT_NT		= 12,	/* mi normal vectors */
	miIMG_FORMAT_MT		= 28,	/* mi motion vectors */
	miIMG_FORMAT_TT		= 13,	/* mi tag channel, from material tags*/
	miIMG_FORMAT_BIT	= 29,	/* mi one-bit render-enable flags */
	miIMG_FORMAT_TIFF	= 14,	/* compressed	any TIFF */
	miIMG_FORMAT_TIFF_U	= 15,	/* uncompressed any TIFF */
	miIMG_FORMAT_TIFF_3	= 16,	/* compressed	any 3-component TIFF */
	miIMG_FORMAT_TIFF_U3	= 17,	/* uncompressed any 3-component TIFF */
	miIMG_FORMAT_TIFF_16	= 30,	/* compressed any 16bit TIFF */
	miIMG_FORMAT_TIFF_16_U	= 31,	/* uncompress any 16bit TIFF */
	miIMG_FORMAT_TIFF_16_3	= 32,	/* compressed 3-component 16bit TIFF */
	miIMG_FORMAT_TIFF_16_U3	= 33,	/* uncompress 3-component 16bit TIFF */
	miIMG_FORMAT_CATIA	= 18,	/* Dassault Systemes Catia format */
	miIMG_FORMAT_PPM	= 19,	/* Jef Poskanzer's portable pixmaps */
	miIMG_FORMAT_CT_FP	= 20,	/* mi Color Texture, floating-point */
	miIMG_FORMAT_MAP	= 21,	/* mmap-ed raw texture file */
	miIMG_FORMAT_TARGA	= 22,	/* compressed RGBA Targa */
	miIMG_FORMAT_BMP	= 23,	/* Microsoft BMP 32-bit uncompressed */
	miIMG_FORMAT_QNT_PAL	= 24,	/* Quantel/Abekas, 576x720 */
	miIMG_FORMAT_QNT_NTSC	= 25,	/* Quantel/Abekas, 486x720 */
	miIMG_FORMAT_ZPIC	= 26,	/* Softimage Zpic, uncompr Z coords */
	miIMG_FORMAT_NULL	= 27,	/* dummy file, write nothing at all */
	miIMG_FORMAT_JPEG	= 34,	/* baseline sequential JPEG */
	miIMG_FORMAT_LWI	= 35,	/* Solidworks texture, read only */
	miIMG_FORMAT_PS		= 36,	/* PostScript - write only */
	miIMG_FORMAT_EPS	= 37,	/* encapsulated PostScript */
	miIMG_FORMAT_CT_H	= 38,	/* mi Color Texture, HDR RGBE */
	miIMG_FORMAT_HDR	= 39,	/* Radiance HDR (aka PIC) */
	miIMG_FORMAT_ST_FP	= 40,	/* mi Color Texture, 32 bits */
	miIMG_FORMAT_IFF	= 41,	/* Alias|wavefront IFF compressed */
	miIMG_FORMAT_CUSTOM_0	= 42,	/* user-defined */
	miIMG_FORMAT_CUSTOM_1	= 43,	/* user-defined */
	miIMG_FORMAT_CUSTOM_2	= 44,	/* user-defined */
	miIMG_FORMAT_CUSTOM_3	= 45,	/* user-defined */
	miIMG_FORMAT_CUSTOM_4	= 46,	/* user-defined */
	miIMG_FORMAT_CUSTOM_5	= 47,	/* user-defined */
	miIMG_FORMAT_CUSTOM_6	= 48,	/* user-defined */
	miIMG_FORMAT_CUSTOM_7	= 49,	/* user-defined */
	miIMG_FORMAT_CUSTOM_8	= 50,	/* user-defined */
	miIMG_FORMAT_CUSTOM_9	= 51,	/* user-defined */
	miIMG_FORMAT_CUSTOM_10	= 52,	/* user-defined */
	miIMG_FORMAT_CUSTOM_11	= 53,	/* user-defined */
	miIMG_FORMAT_CUSTOM_12	= 54,	/* user-defined */
	miIMG_FORMAT_CUSTOM_13	= 55,	/* user-defined */
	miIMG_FORMAT_CUSTOM_14	= 56,	/* user-defined */
	miIMG_FORMAT_CUSTOM_15	= 57,	/* user-defined */
	miIMG_FORMAT_CUSTOM_LAST= 57,	/* last user-defined */
	miIMG_FORMAT_PNG	= 58,	/* png */
	miIMG_FORMAT_SHMAP	= 59,   /* internal shadowmap format */
	miIMG_FORMAT_EXR	= 60,   /* ILM OpenEXR */
	miIMG_NFORMATS		= 61
} miImg_format;
/* The documented error return */
#define miIMG_FORMAT_ERROR miIMG_NFORMATS


/*
 * error codes. If you add one, also add a message to errmsg[] in libimg.c.
 */

typedef enum {
	miIMG_ERR_NONE		= 0,	/* no error */
	miIMG_ERR_READ		= 1,	/* can't read, end of file */
	miIMG_ERR_WRITE		= 2,	/* can't write, probably disk full */
	miIMG_ERR_DECODE	= 3,	/* file format error */
	miIMG_ERR_ENCODE	= 4,	/* internal error */
	miIMG_ERR_TYPE		= 5,	/* unrecognized file type */
	miIMG_ERR_SUBTYPE	= 6,	/* file version not supported */
	miIMG_ERR_CREATE	= 7,	/* failed to create file */
	miIMG_ERR_OPEN		= 8,	/* failed to open file */
	miIMG_ERR_CLOSE		= 9,	/* failed to close file */
	miIMG_ERR_ARGS		= 10,	/* incorrect call argument */
	miIMG_ERR_NOTOPEN	= 11,	/* file not open */
	miIMG_ERR_NOTCLOSED	= 12,	/* file still open, can't re-open */
	miIMG_ERR_SIZE		= 13,	/* wrong file size */
	miIMG_NERRS
} miImg_error;


/*
 * file descriptor. The file name is stored to be able to print meaningful
 * error messages. It is an array, not a strdup'ed pointer, because it is
 * not clear who would de-allocate the name otherwise. If the file name is
 * longer than the array, it is truncated. The real file name is stored
 * too, but it can't be relied upon to exist. Gamma is not used and may go
 * away in future versions.
 * Other modules may only use the public fields. The others are reserved
 * by raylib (and in fact not documented in the external manual). The fp
 * field is actually a FILE*, but it's void* here because otherwise every
 * file including img.h would have to include stdio.h as well. It's always
 * cast to FILE* in img*.c.
 */

typedef struct {
	int		c[4];		/* indexed by miIMG_* */
} miImg_line;				/* component scanlines follow here */

typedef struct miImg_file {
	int		null;		/* bomb out if someone forgets & */
					/* --------------- public --------- */
	int		width, height;	/* width and height in pixels */
	int		bits;		/* requested bits per component, 8/16*/
	int		comp;		/* requested components/pixel, 1..4 */
	miBoolean	filter;		/* caller wants filtered lookups */
	miBoolean	topdown;	/* read file top-down */
	int		filesize;	/* if reading, size of file in bytes */
	float		gamma;		/* gamma correction value (read only)*/
	float		aspect;		/* aspect/xres*yres (for .pic output)*/
	float		parms[8];	/* additional undefined parameters, */
					/* [0] is .jpg quality: 0..100
					 *     or .exr compression type:
					 *     0=default, 1=none, 2=piz,
					 *     3=zip, 4=rle, 5=pxr24 */
					/* [1] is .pic field: 1=even, 2=odd */
	                      		/* [2] scaling factor for filters */
					/* [3] whether IFF has extra Z */
	                                /* [4] DOD tag for IFF */
	                                /* [5] typemap for EXR */
	                                /* [6] DPI for IFF, TIF, JPG */
	miImg_type	type;		/* requested miIMG_TYPE_* */
	miImg_format	format;		/* if reading, file format */
	miImg_error	error;		/* one of miIMG_ERR_*, or 0 */
	int		os_error;	/* copy of Unix's errno, or 0 */
					/* --------------- private --------- */
	int		magic;		/* magic number while file is open */
	int		lineno;		/* current line# to be read/written */
	miBoolean	writing;	/* if nz, file is open for writing */
	miCBoolean	swap_map;	/* swap if memory-mapping */
	miUint1		ftype;		/* actual file data type, should     */
					/* be cast to miImg_type. this is    */
					/* set internally in mi_img_create   */
					/* from miImg_file.type              */
	miCBoolean	writable;	/* writable texture (eg. light map) */
	miUint1		pyr_level;	/* open map file, level to rd, 0=std */
	miPointer	fp;		/* open file descriptor, 0 if closed */
	miPointer	real_name;	/* complete path (for unlink/local) */
	miPointer	convline;	/* converted line if type != format */
	miPointer	data;		/* more format-dependent file info*/
					/* or map address for .map files */
	char		name[64];	/* file name (for error messages) */
	miTag		colorprofile;	/* color space of image data */
	miCBoolean	win_error;	/* os_error is GetLastError code */
	miCBoolean	spare2[3];
	int		spare[12];	/* makes sizeof(miImg_file) 256 bytes */
} miImg_file;


/*
 * generic scanline and image struct. They are based on index values that
 * define the beginning of component scanlines relative to the beginning of
 * the struct (including header and all). Both miImg_line and miImg_image
 * are actually allocated larger than their size; the component scanlines
 * directly follow the typedef'ed header. One mi_mem_release suffices for
 * the whole thing including data.
 * <comp> in the access macro stands for one of the miIMG_* constants below.
 * Do not use miIMG_LINEACCESS before using miIMG_ACCESS to load local texs!
 */

#define miIMG_R		0		/* red */
#define miIMG_G		1		/* green */
#define miIMG_B		2		/* blue */
#define miIMG_A		3		/* alpha */
#define miIMG_E		3		/* HDR exponent */
#define miIMG_S		miIMG_A		/* scalar */
#define miIMG_U		0		/* u vector */
#define miIMG_V		1		/* v vector */
#define miIMG_Z		0		/* Z coordinate */
#define miIMG_COVERAGE	0		/* coverage fraction scalar */
#define miIMG_T		0		/* material tag */
#define miIMG_NX	0		/* normal vector X component */
#define miIMG_NY	1		/* normal vector Y component */
#define miIMG_NZ	2		/* normal vector Z component */

/*end_imglib_h*/
#define miIMG_ACCESS(_i, _y,_c)\
        ((_i)->cacheable \
         ? ((miUchar *) mi_img_cache_get((_i), (_i)->c[4*(_y)+(_c)]))\
	 : ((miUchar *)(_i) + (_i)->c[4*(_y)+(_c)]))

/* do not use it with "cacheable" images */
#define miIMG_LINEACCESS(_l,_c)	((miUchar *)(_l)+(_l)->c[(_c)])
/*begin_imglib_h*/

#define miIMG_DIRSIZE	20		/* image filters have max 20 levels */

typedef struct miImg_image {
	miScalar	filter;		/* filtered lookups if > 0 */
	int		dirsize;	/* valid # of filter levels */
	int		dir[miIMG_DIRSIZE]; /* offs from this to other imgs */
	int		width, height;	/* width and height in pixels */
	int		bits;		/* requested bits per comp, 8/16/32 */
	int		comp;		/* requested components/pixel, 1..4 */
	miCBoolean	local;		/* local texture, use image/mmap/path*/
	miCBoolean	writable;	/* writable texture (eg. light map) */
        miCBoolean	cacheable;	/* cache in parts */
	miCBoolean	remap;		/* image is remapped */
	int		type;		/* requested miIMG_TYPE_* */
	miTag		real_name;	/* (local) file name to open */
	miTag		colorprofile;	/* color profile for image data */
	int		c[4];		/*	indexed by miIMG_*; 4*height */
					/*	component indices follow, */
					/*	then component scanlines */
} miImg_image;
							/*end_matter_base_h*/
							/*begin_matter_base_h*/

typedef struct miImg_tonemap {
	miScalar	ref;		/* referene, adjust units */
	miScalar	tolerance;	/* desired accuracy, about 1% */
	miScalar	low_out;	/* min luminance output device */
	miScalar	hi_out;		/* max luminance output device */
	miScalar	low_in;		/* min input lumin. to consider*/
	miScalar	hi_in;		/* max input lumin. to consider*/
	miColor		lum_weights;	/* luminance weights */
	int		n_bins;		/* number of bins in histogram */
	int		x_res;		/* average over x_res*y_res pixels*/
	int		y_res;		
	int		x_lo;		/* restrict to sub-image */
	int		y_lo;		/* lower left corner and */
	int		x_hi;		/* upper right corner */
	int		y_hi;
	miBoolean	linear;	  	/* use simple linear tonemap */
	miBoolean	desaturate;	/* desaturate color components */
	miTag		map;		/* provide your own mapping for */
					/* intensities */
} miImg_tonemap;
							/*end_matter_base_h*/
							/*begin_matter_base_h*/
miBoolean    	mi_img_create(
				miImg_file *const,	/* file descriptor */
				char * const,		/* new file name */
				const miImg_type,	/* data type, RGB etc*/
				const miImg_format,	/* format, RLA etc */
				const int,		/* image width */
				const int);		/* image height */
miBoolean    	mi_img_open(
				miImg_file *const,	/* file descriptor */
				char * const,		/* file name */
				const miImg_type);	/* desired data type */
miBoolean    	mi_img_close(
				miImg_file *const);	/* file descriptor */
miBoolean	mi_img_line_read(
				miImg_file * const,	/* file descriptor */
				miImg_line * const,	/* image line */
				struct miColor_profile *,/* in color profile */
				struct miColor_profile *,/* out color profile */
				struct miColor_profile *,/* white space cprof */
				miBoolean,		/* perform white adapt*/
				miColor *);		/* color buffer */

miBoolean    	mi_img_line_write(
				miImg_file *const,	/* file descriptor */
				miImg_line *const);	/* scanline buffer */

miBoolean    	mi_img_image_read(
				miImg_file *const,	/* file descriptor */
				miImg_image*const);	/* frame buffer */
miBoolean    	mi_img_image_write(
				miImg_file *const,	/* file descriptor */
				miImg_image*const);	/* frame buffer */
miImg_image 	*mi_img_mmap_address(
				miImg_file *);		/* file descriptor */
miBoolean	mi_img_tonemap(
				miImg_image*,		/* target image*/
				miImg_image*,		/* source image*/
				miImg_tonemap*);	/* parameters */

							/*end_matter_base_h*/
							/*begin_matter_base_h*/
miImg_image 	*mi_img_image_dbcreate(
				miImg_file *const,	/* file descriptor */
				miTag * const);		/* returned tag in DB*/
miBoolean      	mi_img_placeholder_dbcreate(
				miImg_file * const,	/* file struct */
				miTag * const,		/* returned tag in DB*/
				miBoolean);		/* local texture? */
miBoolean mi_img_image_shared(
			        miTag,			/* first image tag */
				miTag);			/* second image tag */
                                                        /*begin_imglib_h*/
miImg_image 	*mi_img_mmap_address(
				miImg_file *ifp);	/* file struct */
void		mi_img_image_release(
				miImg_image *img);	/* file struct */

miUchar     	*mi_img_access(
				miImg_image	*,	/* image to access */
				const int,		/* requested scanline*/
				const int);		/* req component 0..3*/
miImg_image	*mi_img_image_alloc(
				miImg_file *const);	/* file descriptor */
miImg_type	mi_img_type_identify(
				char * const);		/* type, "rgb" etc */
miImg_format	mi_img_format_identify(
				char * const);		/* format, "rla" etc */
miImg_type	mi_img_best_type(
				const miImg_format);	/* format to test */
char		*mi_img_type_name(
				const miImg_type);	/* data type to name */
char		*mi_img_format_name(
				const miImg_format);	/* format to name */
void		mi_img_type_query(
				const miImg_type,	/* type to query */
				int *,			/* ret. # of comp */
				int *,			/* ret. # of bits/c */
				miCBoolean *);		/* ret. comp mask.
							 * set to NULL if not
							 * wanted */
							/*end_matter_base_h*/
							/*begin_matter_base_h*/
typedef enum {						/* colorclip modes */
	miIMG_COLORCLIP_RGB,				/* rgb priority */
	miIMG_COLORCLIP_ALPHA,				/* alpha priority */
	miIMG_COLORCLIP_RAW				/* ignore premult */
} miImg_colorclip;
							/*end_matter_base_h*/
							/*begin_matter_base_h*/
void		mi_img_clip_color(
				miImg_image *const,	/* image to clip for */
				miColor *const);	/* color to clip */
void		mi_img_put_color(
				miImg_image *const,	/* image to store in */
				miColor * const,	/* color to store */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_put_scalar(
				miImg_image *const,	/* image to store in */
				const float,		/* scalar to store */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_put_vector(
				miImg_image *const,	/* image to store in */
				miVector * const,	/* vector to store */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_put_depth(
				miImg_image *const,	/* image to store in */
				const float,		/* depth, 0=eye, >=0 */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_put_coverage(
				miImg_image *const,	/* image to store in */
				const float,		/* 0.0f <= c <= 1.0f */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_put_normal(
				miImg_image *const,	/* image to store in */
				miVector * const,	/* normal to store */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_put_label(
				miImg_image *const,	/* image to store in */
				const miUint,		/* label value */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_get_color(
				miImg_image *const,	/* image to store in */
				miColor * const,	/* color to read */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_get_scalar(
				miImg_image *const,	/* image to store in */
				float * const,		/* scalar to read */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_get_vector(
				miImg_image *const,	/* image to store in */
				miVector * const,	/* vector to read */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_get_depth(
				miImg_image *const,	/* image to store in */
				float * const,		/* depth, 0=eye, >=0 */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_get_coverage(
				miImg_image *const,	/* image to store in */
				float * const,		/* 0.0f <= c <= 1.0f */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_get_normal(
				miImg_image *const,	/* image to store in */
				miVector * const,	/* normal to read */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
void		mi_img_get_label(
				miImg_image *const,	/* image to store in */
				miUint * const,		/* returned label */
				const int,		/* X coordinate */
				const int);		/* Y coordinate */
miImg_image	*mi_img_pyramid_get_level(
				miImg_image *,		/* image */
				int);			/* level of pyramid */
							/*end_matter_base_h*/
/*begin_matter_base_h*/
miImg_image	*mi_lightmap_edit(			/* open for writing */
				void **handle,		/* for _edit_end */
				miTag tag);		/* texture tag */
void		mi_lightmap_edit_end(			/* write texture */
				void *handle);		/* from _edit */
/*end_matter_base_h*/
							/*begin_matter_base_h*/
void *mi_img_cache_get(
		const miImg_image	*image,		/* cached image */
		miUint			offset);	/* to image->c */

/*-------------------------< dealing with color profiles >----------------*/

struct miColor_profile;		/* avoid inclusion of mi_scene_db.h */

miUint1 mi_colorprofile_renderspace_id(void);
miUint1 mi_colorprofile_internalspace_id(void);
miUint1 mi_colorprofile_ciexyz_color_id(void);

miBoolean mi_colorprofile_ciexyz_to_internal(
		miColor			*c);		/* color to convert */

miBoolean mi_colorprofile_internal_to_ciexyz(
		miColor			*c);		/* color to convert */

miBoolean mi_colorprofile_internal_to_render(
		miColor			*c);		/* color to convert */

miBoolean mi_colorprofile_render_to_internal(
		miColor			*c);		/* color to convert */

miBoolean mi_colorprofile_ciexyz_to_render(
		miColor			*c);		/* color to convert */

miBoolean mi_colorprofile_render_to_ciexyz(
		miColor			*c);		/* color to convert */

							/*end_matter_base_h*/

/*------------ mi_msg.h ------------*/

						/*begin_matter_base_h*/
/* create and initialize lock*/
#if defined(LOCK_DEBUG)
  miBoolean mi_init_lock_dbg(miLock *, char *, int);
# define mi_init_lock(p) mi_init_lock_dbg(p, __FILE__, __LINE__)
#else
  miBoolean mi_init_lock(miLock *);
#endif

/* destroy, acquire, release and try lock*/

void mi_lock_thread_suspend(miLock);

#if defined(LOCK_DEBUG)
  void mi_delete_lock_dbg(miLock *, char *, int);
  void mi_lock_dbg(miLock, char *, int);
  void mi_unlock_dbg(miLock, char *, int);
  miBoolean mi_trylock_dbg(miLock, char *, int);
# define mi_delete_lock(p) mi_delete_lock_dbg(p, __FILE__, __LINE__)
# define mi_lock(p) mi_lock_dbg(p, __FILE__, __LINE__)
# define mi_unlock(p) mi_unlock_dbg(p, __FILE__, __LINE__)
# define mi_trylock(p) mi_trylock_dbg(p, __FILE__, __LINE__)
#else
  void mi_delete_lock(miLock *); /* destroy and free lock */
  void mi_lock(miLock);          /* acquire lock, blocking */
  void mi_unlock(miLock);        /* release lock */
  miBoolean mi_trylock(miLock);  /* aqcuire lock if available, non-blocking */
#endif
						/*end_matter_base_h*/

/*------------ mi_par.h ------------*/

					/*begin_matter_base_h*/
void mi_abort(void);

int mi_par_aborted(void);
					/*end_matter_base_h*/

/*------------ mi_rc.h ------------*/

struct miState;
void *mi_renderpass_access(		/* for renderpass merge function */
	struct miState	*state,		/* state->pri is sample set array */
	int		pass,		/* access which pass, -1=output */
	int		fb);		/* frame buffer number */

miBoolean mi_renderpass_resolution(
	int		*xl,		/* returned subpixel coordinates of */
	int		*yl,		/* lower left corner of rectangle */
	int		*xs,		/* returned number of subpixels in */
	int		*ys,		/* the rectangle (containing x/y) */
	int		*xres,		/* returned number of subpixels in */
	int		*yres,		/* the entire frame buffer */
	struct miState	*state,		/* state->cache is sample set array */
	int		x,		/* X subpixel coordinate to look up */
	int		y);		/* Y subpixel coordinate to look up */

miBoolean mi_renderpass_sample_get(
	void		*result,	/* copy data here */
	int		resultsize,	/* number of bytes in *result */
	struct miState	*state,		/* state->cache is sample set array */
	int		fb,		/* frame buffer number */
	int		x,		/* X subpixel coordinate */
	int		y);		/* Y subpixel coordinate */

miBoolean mi_renderpass_sample_put(
	void		*result,	/* data to copy */
	int		resultsize,	/* number of bytes in *result */
	struct miState	*state,		/* state->cache is sample set array */
	int		fb,		/* frame buffer number */
	int		x,		/* X subpixel coordinate */
	int		y);		/* Y subpixel coordinate */

miBoolean mi_renderpass_samplerect_black(
	miBoolean	*result,	/* set this to miTRUE if black */
	struct miState	*state,		/* state->cache is sample set array */
	int		x,		/* X subpixel coordinate */
	int		y);		/* Y subpixel coordinate */

miBoolean mi_renderpass_samplerect_flush(
	struct miState	*state,		/* state->cache is sample set array */
	int		pass,		/* 0=write (put), 1=read (get) */
	int		x,		/* X subpixel coordinate in rect */
	int		y);		/* Y subpixel coordinate in rect */
/*
 * Set the falloff start/stop. If the intersecton point is closer than
 * start, result of the intersection is taken. If it lies between start
 * ebd stop, the result is linearly interpoletad from surface shader and
 * environment shader. If intersection point is behind stop, the environment 
 * shader result is taken. Obviously, intersections behind stop may be ignored
 * for optimization.
 */
miBoolean mi_ray_falloff(
	const struct miState	*state,		/* state */
	double			*start,		/* falloff start distance */
	double			*stop);		/* complete falloff */

/*
 * Set the distance to the normal plane under which the intersections
 * would be ignored. The main purpose is to allow rendering of objects
 * with different approximation levels.
 */
miBoolean mi_ray_offset(
	const struct miState	*state,		/* state */
        double			*offset);	/* offset from the surface */


/*------------ mi_api.h ------------*/

					/*begin_matter_base_h*/
miScalar     mi_lightprofile_value	(const void *, miScalar, miScalar,
					 const miVector *, miBoolean);
struct miState;
miScalar     mi_lightprofile_sample	(struct miState *, miTag, miBoolean);
					/*end_matter_base_h*/

/*------------ mi_scene.h ------------*/

/*begin_matter_base_h*/
/*
 * Every DB element managed by SCENE has a unique type and is known by SCENE.
 * mi_db_create should not be used by modules other than SCENE because that
 * makes it hard to properly swap bytes between sane parts of the
 * network and Windows NT.
 * When adding types, change the arrays in scncall1.c!
 */

typedef enum {
	miSCENE_NONE = 0,	/* unknown. Danger, Will Robinson. */
	miSCENE_FUNCTION_DECL,	/*  1:miFunction_decl */
	miSCENE_FUNCTION,	/*  2:miFunction */
	miSCENE_MATERIAL,	/*  3:miMaterial */
	miSCENE_LIGHT,		/*  4:miLight */
	miSCENE_CAMERA,		/*  5:miCamera */
	miSCENE_BASIS_LIST,	/*  6:miBasis_list */
	miSCENE_OBJECT,		/*  7:miObject */
	miSCENE_BOX,		/*  8:miGeoBox */
	miSCENE_LINEBOX,	/*  9:miLinebox */
	miSCENE_INSTANCE,	/* 10:miInstance */
	miSCENE_GROUP,		/* 11:miGroup */
	miSCENE_OPTIONS,	/* 12:miOptions */
	miSCENE_IMAGE,		/* 13:image data type */
	miSCENE_POLYGON,	/* 14:miPolygon [] */
	miSCENE_INDEX,		/* 15:miIndex [], used as indices */
	miSCENE_GEOINDEX,	/* 16:miGeoIndex [], used as indices */
	miSCENE_VERTEX,		/* 17:miIndex [], used as vertices */
	miSCENE_GEOVERTEX,	/* 18:miGeoIndex [], used as vertices */
	miSCENE_VECTOR,		/* 19:miVector [] */
	miSCENE_GEOVECTOR,	/* 20:miGeoVector [] */
	miSCENE_FACE,		/* 21:miFace [] */
	miSCENE_SURFACE,	/* 22:miSurface [] */
	miSCENE_CURVE,		/* 23:miCurve [] */
	miSCENE_CURVPNT,	/* 24:miCurve_point [] */
	miSCENE_SCALAR,		/* 25:miScalar [] */
	miSCENE_GEOSCALAR,	/* 26:miGeoScalar [] */
	miSCENE_ALGEBRAIC,	/* 27:miAlgebraic [] */
	miSCENE_TAG,		/* 28:miTag [] */
	miSCENE_STRING,		/* 29:char [] */
	miSCENE_SPACECURVE,	/* 30:miSpacecurve [] */
	miSCENE_BOOK,		/* 31:miBook */
	miSCENE_PAGE,		/* 32:miPage */
	miSCENE_SUBDIVSURF,	/* 33:miSurf_subsurf */
	miSCENE_USERDATA,	/* 34:miUserdata */
	miSCENE_WORLD,		/* 35:<dummy for worlds> */
	miSCENE_SESSION,	/* 36:miTag[1]: World */
	miSCENE_TRANSACTION,	/* 37:miTag[2]: World,Session */
	miSCENE_SPLIT,		/* 38:miScene_split: for WST-aware DB */
	miSCENE_IMATTER,	/* 39:char []: opaque for imatter/iraylib */
	miSCENE_NBOX,		/* 40: new box. will disappear soon */
	miSCENE_LOCAL,		/* 41:byte[], host-local unswappable data */
	miSCENE_SPLITOBJECT,    /* 42:miSplit_object */
	miSCENE_INDEXRANGE, 	/* 43:miIndex_range */
        miSCENE_LIGHTPROFILE,   /* 44:light profiles*/
        miSCENE_HARDWARE,       /* 45:hardware shaders */
	miSCENE_LIGHTPROFILE_STD,	/* 46: light profile descr. */
        miSCENE_APPROX_LIST,    /* 47:miApprox array*/
        miSCENE_VENDOR,         /* 48:miVendor describes OpenGL hardware */
	miSCENE_FBINFO,		/* 49: frame buffer information */
	miSCENE_COLORPROFILE,	/* 50: support color profiles */
	miSCENE_SPECTRUM,	/* 51: support light spectra */
	miSCENE_BSDF,		/* 52: support bidir. scattering distr. func*/
	miSCENE_FLIST,		/* 53: face list */
	
	miSCENE_NTYPES = 64	/* user defs start with this index */
} miScene_types;
/*end_matter_base_h*/

/*------------ mi_scene_db.h ------------*/

					/*begin_matter_base_h*/
enum miApprox_method {
	miAPPROX_PARAMETRIC,
	miAPPROX_REGULAR,
	miAPPROX_SPATIAL,
	miAPPROX_CURVATURE,
	miAPPROX_LDA,
	miAPPROX_ADJACENCY,		/* only for curves		     */
	miAPPROX_ALGEBRAIC,		/* only for surfaces		     */
	miAPPROX_DEFTRIM,		/* only for miFace def_trim_approx   */
	miAPPROX_REGULAR_PERCENT,	/* new in 3.2, regular param n% m%   */
	miAPPROX_INDIRECT,		/* in which case		     */
					/* cnst[miCNST_VISIBLE] 	     */
					/* cnst[miCNST_SHADOW] ...	     */
					/* are offsets into approx list      */
	miAPPROX_NMETHODS
};

enum miApprox_style {
	miAPPROX_STYLE_NONE = 0,	/* must be 0 for miOptions default */
	miAPPROX_STYLE_GRID,		/* (0 means don't override) */
	miAPPROX_STYLE_TREE,
	miAPPROX_STYLE_DELAUNAY,
	miAPPROX_STYLE_FINE,		/* sub-object/subpixel tessellation */
        miAPPROX_STYLE_FINE_NO_SMOOTHING,/* fine poly displacement without
                                            Hermite interpolation */
        miAPPROX_STYLE_FINE_CONT_DISP,  /* fine poly displacement which is
                                           continuous at unshared vertices */
	miAPPROX_NSTYLES
};

#define miCNST_UPARAM	  0		/* regular/parametric only */
#define miCNST_VPARAM	  1
#define miCNST_LENGTH	  0		/* curvature/spatial only  */
#define miCNST_DISTANCE   1
#define miCNST_ANGLE	  2

enum miCnst_type {
	miCNST_VISIBLE	= 0,		/* for accessing (int)cnst[0] */
	miCNST_TRACE,
	miCNST_SHADOW,
	miCNST_CAUSTIC,
	miCNST_GLOBILLUM,
	miCNST_NTYPES			/* must not be > 7 ! */
};

enum miApprox_flag {
	miAPPROX_FLAG_ANY	= 0,			/* f. all rays (31)  */
	miAPPROX_FLAG_VISIBLE	= 1 << miCNST_VISIBLE,	/* f. 1-gen rays     */
	miAPPROX_FLAG_TRACE	= 1 << miCNST_TRACE,	/* f. trace rays     */
	miAPPROX_FLAG_SHADOW	= 1 << miCNST_SHADOW,	/* f. shadow rays    */
	miAPPROX_FLAG_CAUSTIC	= 1 << miCNST_CAUSTIC,	/* f. caustic rays   */
	miAPPROX_FLAG_GLOBILLUM = 1 << miCNST_GLOBILLUM /* f. globillum rays */
};

typedef struct miApprox {
	miScalar	     cnst[6];	/* indexed with miUPARAM...miANGLE   */
	miUint1 	     sharp;	/* 0=smooth normals, 255=faceted     */
	miUint1 	     flag;	/* vis, trace, shad: miApprox_flag   */
	miCBoolean	     any;	/* stop if any criterion is met      */
	miCBoolean	     view_dep;	/* view dependent ?		     */
	enum miApprox_method method;
	enum miApprox_style  style;
	miUshort	     subdiv[2]; /* recursive subdivision depth	     */
	miGeoIndex	     max;	/* maximum number of triangles	     */
					/* in case of style contour:
					   resolution of sampling grid	     */
	miScalar	     grading;	/* min angle bound for graded meshing*/
} miApprox;

/*end_matter_base_h*/
					/*begin_matter_base_h*/
#define miAPPROX_MAX_SUBDIV 7
#define miAPPROX_NONE(A) memset(&(A), 0, sizeof(miApprox))
/*: ! this does not specify a valid approximation (tree + parametric) */
#define miAPPROX_DEFAULT(A) do {		 \
	(A).style	  = miAPPROX_STYLE_TREE; \
	(A).method	  = miAPPROX_PARAMETRIC; \
	(A).sharp	  = 0;			 \
	(A).flag	  = miAPPROX_FLAG_ANY;	 \
	(A).any 	  = miFALSE;		 \
	(A).view_dep	  = miFALSE;		 \
	(A).cnst[0]	  = 0.0;		 \
	(A).cnst[1]	  = 0.0;		 \
	(A).cnst[2]	  = 0.0;		 \
	(A).cnst[3]	  = 0.0;		 \
	(A).cnst[4]	  = 0.0;		 \
	(A).cnst[5]	  = 0.0;		 \
	(A).subdiv[miMIN] = 0;			 \
	(A).subdiv[miMAX] = 5;			 \
	(A).max 	  = miHUGE_INT; 	 \
	(A).grading	  = 0.0; } while (0)
#define miAPPROX_FINE_DEFAULT(A) do {		 \
	(A).style	  = miAPPROX_STYLE_FINE; \
	(A).method	  = miAPPROX_LDA;	 \
	(A).sharp	  = 0;			 \
	(A).flag	  = (1<<miCNST_NTYPES)-1;\
	(A).any 	  = miFALSE;		 \
	(A).view_dep	  = miTRUE;		 \
	(A).cnst[0]	  = 0.25;		 \
	(A).cnst[1]	  = 0.0;		 \
	(A).cnst[2]	  = 0.0;		 \
	(A).cnst[3]	  = 0.0;		 \
	(A).cnst[4]	  = 0.0;		 \
	(A).cnst[5]	  = 0.0;		 \
	(A).subdiv[miMIN] = 0;			 \
	(A).subdiv[miMAX] = miAPPROX_MAX_SUBDIV; \
	(A).max 	  = miHUGE_INT; 	 \
	(A).grading	  = 0.0; } while (0)
					/*end_matter_base_h*/
						/*begin_matter_base_h*/
#define miRC_IMAGE_RGBA 	0		/* standard RGBA color fb */
#define miRC_IMAGE_Z		1		/* standard depth fb */
#define miRC_IMAGE_N		2		/* standard normal-vector fb */
#define miRC_IMAGE_M		3		/* standard motion-vector fb */
#define miRC_IMAGE_TAG		4		/* standard object label fb */
#define miRC_IMAGE_COVERAGE	5		/* coverage framebuffer */
#define miRC_IMAGE_PPREV	7		/* reserved for pixel preview*/
						/* 8-9 virtual frame buffers */
#define miRC_IMAGE_USER		12		/* first user-defined fb */
#define miRC_CLIP_MIN		0.001		/* min near clipping plane */

#define miRc_options		miOptions	/* backwards compatibility */

/* bit definitions for miOptions.diagnostic_mode */
#define miSCENE_DIAG_SAMPLES	1		/* turn on sample view mode */
#define miSCENE_DIAG_PHOTON	6		/* photon 2 bits  */
#define miSCENE_DIAG_PHOTON_D	2		/* photon density  */
#define miSCENE_DIAG_PHOTON_I	4		/* photon irradiance  */
#define miSCENE_DIAG_GRID	24		/* grid 2 bits */
#define miSCENE_DIAG_GRID_O	8		/* object_space_grid*/
#define miSCENE_DIAG_GRID_W	16		/* world_space_grid*/
#define miSCENE_DIAG_GRID_C	24		/* camera_space_grid*/
#define miSCENE_DIAG_BSP_D	32		/* bsp tree depth */
#define miSCENE_DIAG_BSP_L	64		/* bsp tree leaf size */
#define miSCENE_DIAG_BSP	96		/* bsp 2 bits */
#define miSCENE_DIAG_FG 	128		/* final gather */

/* bit definitions for miOptions.hardware_diagnostic */
#define miSCENE_HWDIAG_WIRE	1		/* hardware mesh wireframe */
#define miSCENE_HWDIAG_SOLID	2		/* hardware mesh solid */
#define miSCENE_HWDIAG_MESH	3		/* hardware mesh: 2 bits */
#define miSCENE_HWDIAG_LIGHTS	4		/* show light sources */
#define miSCENE_HWDIAG_WINDOW	8		/* render in visible window */
#define miSCENE_HWDIAG_DEBUG	16		/* compile fragment shader*/
						/* with -debug */

/* bit definitions for miOptions.motion */
#define miSCENE_MOTION_ON	1		/* motion blur on */
#define miSCENE_MOTION_C_INT	2		/* camera instance interp. */

/* bit definitions for shadowmaps */
#define miSHADOWMAP_MERGE	0x01
#define miSHADOWMAP_TRACE	0x02
#define miSHADOWMAP_CROP	0x04
#define miSHADOWMAP_ONLY	0x08
#define miSHADOWMAP_CAMERA	0x10
#define miSHADOWMAP_DETAIL	0x20
#define miSHADOWMAP_TILE	0x40

/* flag definition for lightmaps */
#define miLIGHTMAP_OFF		0
#define miLIGHTMAP_ON		1
#define miLIGHTMAP_ONLY		2
						/*end_matter_base_h*/
						/*begin_matter_base_h*/

/* bit definitions for hardware rendering */
#define miHW_OFF                0x0             /* not using hw rendering */
#define miHW_ON                 0x1             /* hw rendering on selec obj.*/
#define miHW_ALL                0x2             /* hw rendering on all obj */

/* bit definitions for hardware shading */
#define miHW_DEFAULT            0x0             /* Use the best shd model */
#define miHW_FORCE              0x1             /* force hw, fall back to nxt*/
#define miHW_CG                 0x2             /* Try first Cg */
#define miHW_NATIVE             0x4             /* Try first OGLSL/HLSL */
#define miHW_FAST               0x8             /* Try first(only) OGL/DX */

/*
 * Frame buffer information entry. options define an array of these
 * entries (images_info).
 * swapped as integer vector.
 */

typedef struct miFb_info {
	miImg_type	image_type;		/* image types */
						/* miIMG_TYPE_RGBA, ...ANY...*/
	miUint4		write_image;		/* what to do with it */
						/* bits: 0=sample image */
						/*       1=write  image */
	miBoolean	interp_image;		/* interpolate image? */
} miFb_info;

typedef struct miOptions {
	miBoolean	trace;			/* 2nd generation ray trace? */
	int		scanline;		/* 0=off, 1=on, 'o'=OpenGL, 
						 * 'r'=rapid */
	miUint		motion; 		/* motion blur? : miFALSE */
	miBoolean	shadow_sort;		/* Obsolete! Do not use */
	miBoolean	preview_mode;		/* for future use */
	int		reflection_depth;	/* refl. trace depth : 1 */
	int		refraction_depth;	/* refr. trace depth : 1 */
	int		trace_depth;		/* refl. + refr. depth : 1 */
	int		min_samples;		/* min. sampling level : -2 */
	int		max_samples;		/* max. sampling level : 0 */
	miColor 	contrast;		/* sampling contrast: .1 */
	miColor 	time_contrast;		/* temporal sampling contrast*/
	miTag		contour_contrast;	/* NULLTAG */
	miTag		contour_store;		/* NULLTAG */
	miBoolean	caustic;		/* enable caustics: miFALSE */
	miBoolean	globillum;		/* enable global il.: miFALSE*/
	int		caustic_accuracy;	/* no. caus. photons in est. */
	float		caustic_radius; 	/* maxdist for caus. photons */
	int		globillum_accuracy;	/* no. glob. photons in est. */
	float		globillum_radius;	/* maxdist for glob. photons */
	float		caustic_filter_const;	/* filter const. for caus. */
	float		filter_size_x;		/* filter size in x : 1.0 */
	float		filter_size_y;		/* filter size in y : 1.0 */
	float		jitter; 		/* sample jittering : 0.0 */
	float		shutter;		/* shutter close time: 0 */
	short		grid_res[3];		/* grid resolution */
	short		grid_max_size;		/* max obj. in grid cell */
	short		grid_max_depth; 	/* max grid nesting */
	miSint1 	def_min_samples;	/* min/max samples for objs */
	miSint1 	def_max_samples;	/* without min/max, -128 127 */
	int		space_max_size; 	/* space subdiv. leaf size: 4*/
	int		space_max_depth;	/* space subdiv. depth : 25*/
	float		shutter_delay;		/* shutter open time: 0 */
	int		no_images;		/* # images : 1 */
	miUint		spare1[47];		/* keep ray33 offsets */
	miTag		images_info;		/* frame buffers */

	miTag		finalgather_file;	/* finalgather map file: 0 */
	miCBoolean	space_shadow_separate;	/* space subdiv shadow struct*/
	miUchar 	finalgather_filter;	/* finalgather ray filter */
	char		rapid_collect_rate;	/* rasteriser only: */
						/* samples per pixel per x,y */
						/* default: 4 */
	char		rapid_motion_resample;	/* rasteriser only: # of */
						/* time samples for movng geo*/
	char		use_shadow_maps;	/* 0=off, 1=on, 'o'=opengl*/
						/* bit7: only shmap,no render*/
	miCBoolean	rendering_shadow_maps;	/* Shadow map mode? 0 */
	char		recompute_shadow_maps;	/* 'n'o,'y'es (could be Bool)*/
	char		shadow; 		/* shadow casting? : 1 */
						/* 0 off, 1 on, 'l' sort, */
						/* 's' segments */
	char		caustic_filter; 	/* filter-type */
						/* 'b' box, 'c' cone, */
						/* 'g' gaussian */
	miCBoolean	finalgather_rebuild;	/* rebuild FG ?: on*/
	char		filter; 		/* sample filter type : 'b' */
						/* 'b' box, 't' triangle, */
						/* 'g' gauss */
	char		acceleration;		/* acceleration: 'b'	     */
						/* 'c' ray classification,   */
						/* 'b' bsp, 'g' grid	     */
						/* 'l' large (flushing) bsp  */
	char		face;			/* primitive facing: 'a' */
						/* 'f' front, 'b' back, */
						/* 'a' all */
	char		field;			/* field rendering? 0 */
						/* 0 off, 'e' even, 'o' odd */
	char		smethod;		/* sampling algorithm: 'q' */
	char		render_space;		/* coordinate space 'o' */
						/* 'c' camera space, */
						/* 'o' object space, */
						/* 'm' mixed space */
	miCBoolean	pixel_preview;		/* pixel selective sampling */
	miCBoolean	task_preview;		/* task selective sampling */
	miCBoolean	visible_lights; 	/* any visible area lights? */
	miCBoolean	shadow_map_motion;	/* motion blurred shadowmaps?*/
	int		task_size;		/* image task size : 0 */
	miBoolean	strips; 		/* GAP returns triangles in
						 * strips */
	miTag		photonmap_file; 	/* photon map file name */
	miBoolean	photonmap_rebuild;	/* photon map rebuild ? */
	int		photon_reflection_depth;/* photon refl. depth */
	int		photon_refraction_depth;/* photon refr. depth */
	int		photon_trace_depth;	/* total photon trace depth */
	int		space_max_mem;		/* maximum bsp memory (mb):0 */
	miColor		caustic_scale;		/* caustic multiplier */
	miColor		globillum_scale;	/* globillum multiplier */
	miColor		finalgather_scale;	/* finalgather multiplier */
	miUint		spare2[15];		/* keep ray33 offsets */
	miTag		spectrum_blueprint;	/* spectrum for render space */
	float		lum_efficacy;		/* render space luminous efficacy*/
	miTag		white_cprof;		/* white adapt. col. space */
	miTag		render_cprof;		/* color profile render space*/
	miTag		fb_dir;			/* frame buffer directory */
	miCBoolean	no_lens;		/* disable lens shaders */
	miCBoolean	no_volume;		/* disable volume shaders */
	miCBoolean	no_geometry;		/* disable geometry shaders */
	miCBoolean	no_displace;		/* disable displace shaders */
	miUint1 	no_output;		/* disable output shaders */
	miCBoolean	no_merge;		/* disable surface merging */
	miUint1 	caustic_flag;		/* def. caustic flag for objs*/
	miUint1 	diagnostic_mode;	/* miSCENE_DIAG_* flags */
	int		photonvol_accuracy;	/* no. vol. photons in est. */
	float		photonvol_radius;	/* maxdist for vol. photons */
	miUint1 	globillum_flag; 	/* def. globil. flag for objs*/
	miCBoolean	samplelock;		/* steady samples?: on */
	miCBoolean	autovolume;		/* internal volumes? */
	miCBoolean	finalgather_view;	/* radii in raster pixels ? */
	miCBoolean	no_hair;		/* disable hair rendering */
	miUint1 	n_motion_vectors;	/* global number of motions */
	miCBoolean	no_pass;		/* disable multipass render */
	miCBoolean	photon_autovolume;	/* autovolume for photons */
	miCBoolean	no_predisplace; 	/* disable displ presampling */
	miUint1 	fg_reflection_depth;	/* finalgather refl. depth */
	miUint1 	fg_refraction_depth;	/* finalgather refr. depth */
	miUint1 	fg_trace_depth; 	/* total fg trace depth */
	float		fg_falloff_start;	/* finalgather falloff start */
	float		fg_falloff_stop;	/* finalgather falloff stop */
	miTag		userdata;		/* optional user data blocks */
	miApprox	approx; 		/* approximation overrides if*/
	miApprox	approx_displace;	/* style != APPROX_STYLE_NONE*/
	int		finalgather;		/* 0:off,1:on,'f':fastlookup*/
	int		finalgather_rays;	/* no. rays in final gather */
	float		finalgather_maxradius;	/* maxdist for finalgather */
	float		finalgather_minradius;	/* mindist for finalgather */
	float		diag_photon_density;	/* density for diag */
	float		diag_grid_size; 	/* gridsize for diag */
	miBoolean	desaturate;		/* for IMG: fade to white? */
	miBoolean	dither; 		/* for IMG: LSB dithering? */
	miBoolean	nopremult;		/* for IMG: A < RGB ok? */
	int		colorclip;		/* for IMG: miIMG_COLORCLIP* */
	float		gamma;			/* for IMG: gamma, default 1 */
	miTag		inh_funcdecl;		/* inheritance function */
	miColor 	luminance_weight;	/* weights to calc intensity */
	int		split_obj_no_triangs;	/* max no_triangs for split of
						 * miOBJECT_POLYGONS */
	int		split_obj_no_faces;	/* max no_faces for split of
						 * miOBJECT_FACES */
	miCBoolean	inh_is_traversal;	/* inh_funcdecl is a traver- */
						/* sal, not inheritance, shd */
	miUint1 	hardware;		/* hwrender 0=off 1=on 3=all */
	miUint1 	hardware_diagnostic;	/* flags for diagn. mode */
	miUint1 	shadowmap_flags;	/* flag for shadowmaps */
	float		maxdisplace;		/* override if !=0: default 0*/
	miScalar	shadowmap_bias;		/* move shmap Z back by bias */
	miUint1		hwshader;		/* 1=force */
                                                /* 2=cg 4=oglsl/hlsl 8=fast */
	miCBoolean	photonmap_only;		/* render photon map only */
	miUint1		lightmap;		/* 0=off,1=on,2=only */
	                                        /* 4=use any host (unsafe) */
	miUint1		fg_diffuse_depth;	/* #secondary fg bounces */
	miTag		state_func;		/* allow user to init state */
	float		fg_presamp_density;	/* samples density for precomp.
						 * finalgather points */
	float		rapid_shading_samples;	/* shading samples per pixel */
						/* default: 1.0f */
	miCBoolean	fb_virtual;		/* mmap frame buffers */
	miCBoolean	spare3[3];		/* not used */
	int		spare[10];		/* not used */
} miOptions;
						/*end_matter_base_h*/
						/*begin_matter_base_h*/
typedef struct miCamera {
	miBoolean	orthographic;		/* orthographic rendering? */
	float		focal;			/* focal length */
	float		aperture;		/* aperture */
	float		aspect; 		/* aspect ratio of the image */
	miRange 	clip;			/* min/max clipping */
	int		x_resolution;		/* x resolution */
	int		y_resolution;		/* y resolution */
	struct {int xl, yl, xh, yh;}
			window; 		/* corners of subwindow */
	miTag		volume; 		/* opt. volume shader */
	miTag		environment;		/* opt. environment shader */
	miTag		lens;			/* opt. lens shader */
	miTag		output; 		/* opt. output shaders/files */
	int		frame;			/* current frame number */
	float		frame_time;		/* ... as time in seconds */
	int		frame_field;		/* 0=frame, 1/2=field number */
	float		x_offset;		/* x image offset in pixels */
	float		y_offset;		/* y image offset in pixels */
	miTag		userdata;		/* optional user data blocks */
	miTag		pass;			/* opt. pass function chain */
	float		focus;			/* rasteriser: focus plane */
	float		radius;			/* rasteriser: lens radius */
	miBoolean	pass_mask;		/* passes prevent eye rays */
	int		spare[11];		/* not used */
} miCamera;
						/*end_matter_base_h*/
						/*begin_matter_base_h*/
enum miLight_type {
	miLIGHT_ORIGIN,
	miLIGHT_DIRECTION,
	miLIGHT_SPOT
};

enum miLight_area {
	miLIGHT_NONE = 0,
	miLIGHT_RECTANGLE,
	miLIGHT_DISC,
	miLIGHT_SPHERE,
	miLIGHT_CYLINDER,
	miLIGHT_OBJECT,
	miLIGHT_USER
};
						/*end_matter_base_h*/
						/*begin_matter_base_h*/
enum miFunction_type {
	miFUNCTION_C,				/* regular C/C++ shader */
	miFUNCTION_PHEN,			/* phenomenon interface */
	miFUNCTION_C_REQ,			/* C/C++ shader with reqmnts */
	miFUNCTION_OUTFILE,			/* write an image to a file */
	miFUNCTION_DATA,			/* user data decl, no miFunc */
	miFUNCTION_PASS_SAVE,			/* write pass file */
	miFUNCTION_PASS_PREP,			/* pass file preprocessor */
	miFUNCTION_PASS_MERGE,			/* pass file merge function */
	miFUNCTION_PASS_DELETE, 		/* pass file delete */
	miFUNCTION_HARDWARE			/* hardware rendering shader */
};

typedef struct miFunction {
	miPointer	sparep1[7];
	enum miFunction_type type;		/* C/C++, phenomenon, or file*/
	miUint		out_typemap;		/* if output, IMG_TYPE bitmap*/
	miUint		out_interpmap;		/* if output, interpolate bm */
	miTag		function_decl;		/* declaration if C or PHEN */
	miTag		next_function;		/* next function call in list*/
	miTag		parameter_indirect;	/* get params from this func */
	miTag		interfacephen;		/* parent phen, if any */
	miBoolean	spare0; 		/* not used */
	int		parameter_size; 	/* size of parameter block */
	int		result_size;		/* size of result struct */
	int		ghost_offs;		/* offset to tag ghost, or 0 */
	miTag		pass_read;		/* pass: infile, infile list */
	miTag		pass_write;		/* pass: outfile or 0 */
	miSint1 	pass_maxsamples;	/* pass: max samples or ~0 */
	miCBoolean	spare2[3];		/* not used */
	miCBoolean	no_expl_params; 	/* candidate for indirect par*/
	miCBoolean	cloned; 		/* is this a clone */
	miCBoolean	spare3[5];		/* not used */
	miUchar 	label;			/* sequential # for bitmasks */
	char		parameters[8];		/* parameter block, then */
						/* miTag ghost for phenomena */
} miFunction;
						/*end_matter_base_h*/
						/*begin_matter_base_h*/
typedef struct miUserdata {
	miTag		data_decl;		/* parameter declaration */
	miTag		next_data;		/* next data block in list */
	miUint		label;			/* data label */
	int		parameter_size; 	/* size of parameter block */
	short		one;			/* ==1, for byte order check */
	short		spare1; 		/* not used */
	int		spare2; 		/* not used */
	char		parameters[8];		/* parameter block */
} miUserdata;
						/*end_matter_base_h*/

/*------------ mi_shader.h ------------*/

/*begin_matter_base_h*/
/*
 * Enumeration of all types of rays to be traced
 * DO NOT renumber the flags, or precompiled shaders may stop working!
 * volume shaders can find out from state->type whether this ray came
 * from an object or from a light source. Note that in 3.0, mi_trace_probe
 * used miRAY_NONE and hence ignored non-trace objects. Changed in 3.1.2.
 */


typedef enum miRay_type {
	miRAY_EYE,			/* eye ray */
	miRAY_TRANSPARENT,		/* transparency ray */
	miRAY_REFLECT,			/* reflection ray */
	miRAY_REFRACT,			/* refraction ray */
	miRAY_LIGHT,			/* light ray */
	miRAY_SHADOW,			/* shadow ray */
	miRAY_ENVIRONMENT,		/* ray only into environment/volume */
	miRAY_NONE,			/* other ray */
	miPHOTON_ABSORB,		/* photon is absorbed (RIP) */
	miPHOTON_LIGHT, 		/* photon emitted from a light source*/
	miPHOTON_REFLECT_SPECULAR,	/* specular reflection of a photon */
	miPHOTON_REFLECT_GLOSSY,	/* glossy reflection of a photon */
	miPHOTON_REFLECT_DIFFUSE,	/* diffuse reflection of a photon */
	miPHOTON_TRANSMIT_SPECULAR,	/* specular transmission of a photon */
	miPHOTON_TRANSMIT_GLOSSY,	/* glossy transmission of a photon */
	miPHOTON_TRANSMIT_DIFFUSE,	/* diffuse transmission of a photon */
	miRAY_DISPLACE, 		/* displacement during tesselation */
	miRAY_OUTPUT,			/* output shader */
	miPHOTON_SCATTER_VOLUME,	/* volume scattering of a photon */
	miPHOTON_TRANSPARENT,		/* transparency photon */
	miRAY_FINALGATHER,		/* final gather ray */
	miRAY_LM_VERTEX,		/* light map vertex rendering */
	miRAY_LM_MESH,			/* light map mesh rendering */
	miPHOTON_EMIT_GLOBILLUM,	/* globillum photons (emitters only) */
	miPHOTON_EMIT_CAUSTIC,		/* caustic photons (emitters only) */
	miRAY_PROBE,			/* mi_trace_probe, ignores vis/trace */
	miRAY_HULL,			/* hull ray */
	miPHOTON_HULL,			/* photon hull ray */
	miRAY_NO_TYPES
} miRay_type;

/*end_matter_base_h*/
/*begin_matter_base_h*/
/*
 * Determine if a ray given by miRay_type is an eye or first generation
 * transparency ray (primary), or a reflection or refraction (secondary) ray.
 */

#define miRAY_PRIMARY(r)	((r) == miRAY_EYE)
#define miRAY_SECONDARY(r)	((r) >	miRAY_EYE &&\
				 (r) <	miPHOTON_ABSORB ||\
				 (r) == miRAY_FINALGATHER ||\
				 (r) == miRAY_HULL)

/*
 * Determine if ray is a photon
 */

#define miRAY_PHOTON(r) 	(((r) >= miPHOTON_ABSORB &&\
				 (r) <= miPHOTON_TRANSMIT_DIFFUSE) ||\
				 ((r) >= miPHOTON_SCATTER_VOLUME &&\
				  (r) <= miPHOTON_TRANSPARENT) ||\
				 ((r) >= miPHOTON_EMIT_GLOBILLUM &&\
				  (r) <= miPHOTON_EMIT_CAUSTIC) ||\
				  (r) == miPHOTON_HULL)

#define MAX_TEX 	64	/* max. number of texture spaces */
#define NO_DERIVS	5	/* number of surface derivatives */

/*
 * Virtual frame buffers numbers.  Used exclusively by pass filters.
 * to access sample information that is not written to files.
 */

#define miVFB_FIRST		8	
#define miVFB_SAMPLE_TIME	8	/* time when sample was shot */
#define miVFB_SAMPLE_LIMITS	9	/* per object sample limits */
#define miVFB_TOTAL		9

/*
 * Types of shaders
 */

typedef enum {
	miSHADER_LENS,		/* lens shader */
	miSHADER_MATERIAL,	/* material shader */
	miSHADER_LIGHT, 	/* light shader */
	miSHADER_SHADOW,	/* shadow shader */
	miSHADER_ENVIRONMENT,	/* environment shader */
	miSHADER_VOLUME,	/* volume shader */
	miSHADER_TEXTURE,	/* texture shader */
	miSHADER_PHOTON,	/* forward shader */
	miSHADER_GEOMETRY,	/* geometry shader */
	miSHADER_DISPLACE,	/* displacement shader */
	miSHADER_PHOTON_EMITTER,/* photon emitter */
	miSHADER_OUTPUT,	/* output shader */
	miSHADER_OTHER, 	/* shader not known by RC */
	miSHADER_LIGHTMAP,	/* lightmap shader */
	miSHADER_PHOTONVOL,	/* photon volume shader */
	miSHADER_STATE,    	/* allow user to set-up state */
        miSHADER_CONTOUR,       /* contour shader */
	miSHADER_NO_TYPES
} miShader_type;
/*end_matter_base_h*/
/*begin_matter_base_h*/
/* assure no padding for 8-byte alignment on macosx. */
#if defined(__GNUC__) && (defined(__APPLE_CPP__)     ||  \
                          defined(__APPLE_CC__)      ||  \
                          defined(__MACOS_CLASSIC__))
#define MC_PACK __attribute__((packed))
#else
#define MC_PACK 
#endif

/*
 * State structure
 */

#define MI_STATE_VERSION 2

typedef struct miState {
					/* global */
	int		version;		/* version of state struct,
						 * and shader interface
						 * currently 2 */
	miTag		camera_inst;		/* camera instance */
	struct miCamera *camera;		/* camera */
	struct miOptions *options;		/* options */
	miLock		global_lock;		/* global lock for shaders */
	miUint		qmc_instance;		/* instance of low discrepacny
						 * vector associated to current
						 * ray tree */
#if defined(BIT64) || defined(_WIN64)
	miUint		pad;
#endif
	struct miTs	*ts;			/* thread local storage (ts) */
						/* (internal) */
	float		raster_x;		/* x in raster space */
	float		raster_y;		/* y in raster space */
					/* ray */
	struct miState	*parent;		/* state of parent ray
						 * first eye ray/lens shader:
						 *   NULL
						 * subsequent eye rays/lens
						 * shaders: previous one
						 * reflection, refraction,
						 * dissolve rays: parent ray
						 * light rays: parent ray
						 * shadow rays: light ray */
	miRay_type	type;			/* type of the ray */
	miUchar 	qmc_component;		/* next component of current
						 * instance of low discrepancy
						 * vector to be used */
	miUint1 	scanline;		/* intersect ray by scanline?*/
	miCBoolean	inv_normal;		/* normals are inverted because
						 * ray hit the back side */
	char		face;			/* f)ront, b)ack, a)ll, from
						 * state->options for subrays*/
	void		*spare0;		/* unused */
	void		*cache; 		/* cache holding shadow rays
						 * it's a shadow cache
						 * internal to RC */
	int		reflection_level;	/* reflection depth of the ray
						 * SHOULD BE SHORT
						 */
	int		refraction_level;	/* refraction depth of the ray
						 * SHOULD BE SHORT
						 */
	miVector	org;			/* ray origin
						 * for light, shadow rays the
						 * light position */
	miVector	dir;			/* ray direction
						 * for light, shadow ray
						 * it points
						 * towards the intersection */
	double		dist MC_PACK;		/* length of the ray */
	float		time;			/* time of the ray */
	miScalar	ior;			/* ior of outgoing ray */
	miScalar	ior_in; 		/* ior of incoming ray */
	miTag		material;		/* material of the primitive.
						 * Set to miNULL_TAG if no hit.
						 */
	miTag		volume; 		/* volume shader to be applied
						 */
	miTag		environment;		/* environment shader to be
						 * applied if no hit */
					/* intersection point */
	miTag		refraction_volume;	/* volume shader to apply
						 * to refraction ray */
	miUint		label;			/* label of hit object */
	miTag		instance;		/* hit object instance */
	miTag		light_instance; 	/* light instance */

	void		*pri;			/* box_info of hit box, shaders
						 * can check if  pri== NULL
						 */
	int		pri_idx;		/* index of hit primitive
						 * in box */
	float		bary[4];		/* barycentric coordinates
						 * of hit primitive
						 * triangles: 0 - 2,
						 * imp. patches: 0 - 3 */
	miVector	point;			/* point of intersection */
	miVector	normal; 		/* interpolated normal
						 * pointing to the side of the
						 * ray */
	miVector	normal_geom;		/* geometric normal of pri.
						 * pointing to the side of the
						 * ray */
	miScalar	dot_nd; 		/* dot of normal, dir
						 * for light rays,
						 * it's the dot_nd of the
						 * parent (intersection) normal
						 * and the light ray dir */
	double		shadow_tol MC_PACK;	/* the minimum distance to a
						 * plane perpendicular to the
						 * shading normal for which the
						 * triangle is completely on
						 * the negative side.  */
	miVector	*tex_list;		/* list of texture coordinates
						 */
	miVector	*bump_x_list;		/* list of bump map basis x
						 * (perturbation) vectors */
	miVector	*bump_y_list;		/* list of bump map basis y
						 * (perturbation) vectors */
	miVector	motion; 		/* motion vector or (0,0,0) */
	miVector	*derivs;		/* list of surface derivative
						 * vectors containing dx/du,
						 * dx/dv, d^2x/du^2, d^2x/dv^2,
						 * d^2x/dudv or (0,0,0) */
					/* light shader, shadow shader */
						/* instance is the
						 * light instance, see above */
					/* texture shader */
	miVector	tex;			/* texture coordinates for
						 * lookup */
					/* shader */
	const struct miFunction *shader;	/* current shader */
					/* other */
	struct miState	*child; 		/* child state */
	short		thread; 		/* current thread number */
	miUshort	count;			/* area light sample counter */
					/* user */
	void		*user;			/* user data */
	int		user_size;		/* size of user data */
} miState;

/*end_matter_base_h*/
/*begin_matter_base_h*/
/*
 * state structure passed to old-style output shaders. It is used for
 * output shaders whose declaration does not specify a version number.
 * Do not use this. Output shaders with nonzero version numbers get
 * a miState, not a miOutstate, and work with versioning and init/exit.
 */

#define MI_OUTSTATE_VERSION 2

typedef struct {				/* state for output shaders */
	int		version;		/* version of miOutstate */
	miTag		camera_inst;		/* camera instance */
	struct miCamera *camera;		/* camera */
	struct miOptions *options;		/* options */
	int		xres, yres;		/* image resolution */
	miImg_image	*frame_rgba;		/* RGBA color frame buffer */
	miImg_image	*frame_z;		/* depth channel frame buffer*/
	miImg_image	*frame_n;		/* normal vector frame buffer*/
	miImg_image	*frame_label;		/* label channel frame buffer*/
	miMatrix	camera_to_world;	/* world transformation */
	miMatrix	world_to_camera;	/* inverse world transform */
	miImg_image	*frame_m;		/* motion vector frame buffer*/
} miOutstate;


/*
 * shader type
 */

typedef miBoolean (*miShader)(
		miColor 	*result,	/* result */
		miState 	*state, 	/* state */
		void		*args); 	/* shader args */

typedef miBoolean (*miShaderx)(
		miColor 	*result,	/* result */
		miState 	*state, 	/* state */
		void		*args,		/* shader args */
		void		*arg);		/* arg from parent shader */

typedef miBoolean (*miShader_init)(
		miState 	*state, 	/* state */
		void		*args,		/* shader args or NULL */
		miBoolean	*init_req);	/* instance inits too? */

typedef miBoolean (*miShader_exit)(
		miState 	*state, 	/* state */
		void		*args); 	/* shader args or NULL */

typedef int (*miShader_version)(void);

/* state setup shader */
typedef miBoolean (*miShader_state)(
		miState		*state,
		void		*args);

typedef void (*miContour_Store_Function)(
		void		*info_void,
		int		*info_size,
		miState		*state,
		miColor		*color);

typedef miBoolean (*miContour_Contrast_Function)(
		void		*info1,
		void		*info2,
		int		level,
		miState		*state,
		void		*paras);

struct miContour_endpoint;			/* typedef'd later */

typedef miBoolean (*miContour_Shader)(
		struct miContour_endpoint *epoint,
		void		*info_near,
		void		*info_far,
		miState		*state,
		void		*paras);


/* Typedefs for lightmapping shaders */
typedef struct miRclm_triangle {
		miInteger	a;
		miInteger	b;
		miInteger	c;
		miGeoIndex	pri_idx;
} miRclm_triangle;

typedef struct miRclm_mesh_render {
		void			*pri;
		int			no_triangles;
		miRclm_triangle const	*triangles;
		void const		*vertex_data;
} miRclm_mesh_render;


/*
 * functions
 */

#define mi_eval( s, p ) (!(s)->shader->ghost_offs ? (void*)(p) : \
  (!*((miTag*)((char*)(p)+(s)->shader->ghost_offs)) ? (void*)(p) : \
  mi_phen_eval(s,(void*)(p))))

#define mi_eval_boolean(p)	((miBoolean *)mi_eval(state, (void *)(p)))
#define mi_eval_integer(p)	((miInteger *)mi_eval(state, (void *)(p)))
#define mi_eval_scalar(p)	((miScalar  *)mi_eval(state, (void *)(p)))
#define mi_eval_vector(p)	((miVector  *)mi_eval(state, (void *)(p)))
#define mi_eval_transform(p)	((miScalar  *)mi_eval(state, (void *)(p)))
#define mi_eval_color(p)	((miColor   *)mi_eval(state, (void *)(p)))
#define mi_eval_tag(p)		((miTag     *)mi_eval(state, (void *)(p)))

miBoolean mi_call_shader(
		miColor 	*result,	/* result */
		miShader_type	type,		/* shader type */
		miState 	*state, 	/* state */
		miTag		shader);	/* shader function instance */
miBoolean mi_call_shader_x(
		miColor 	*result,	/* result */
		miShader_type	type,		/* shader type */
		miState 	*state, 	/* state */
		miTag		shader, 	/* shader function instance */
		void		*arg);		/* pass to shader */
void mi_flush_cache(
		miState *state );		/* the famous state */
miBoolean mi_call_material(			/* for smart volume shaders */
		miColor 	*result,	/* material shader result */
		miState 	*state);	/* state with state->material*/
miBoolean mi_call_photon_material(		/* for photon volume shaders */
		miColor 	*energy,	/* energy (surprise!) */
		miState 	*state);	/* state with state->material*/
miBoolean mi_call_shadow_material(		/* for shadow shaders */
		miColor 	*result,	/* result */
		miState 	*state);	/* state with state->material*/
miBoolean mi_call_environment_material(		/* for environment shaders */
		miColor 	*result,	/* result */
		miState 	*state);	/* state with state->material*/
miBoolean mi_call_volume_material(		/* for volume shaders */
		miColor 	*result,	/* result */
		miState 	*state);	/* state with state->material*/
miBoolean mi_call_photonvol_material(		/* for photon volume shaders */
		miColor 	*result,	/* result */
		miState 	*state);	/* state with state->material*/
miBoolean mi_call_contour_material(		/* for contour shaders */
		struct miContour_endpoint* result,  /* contour result */
		void		*info_near,     /* near info */
		void            *info_far,      /* far info */
		miState         *state);        /* state with state->material*/
void *mi_phen_eval(				/* evaluate a shader param */
		miState		*state, 	/* state */
		void		*parm );	/* parameter */
miBoolean mi_lookup_color_texture(
		miColor * const	color,		/* returned color */
		miState * const	state,
		const miTag	tex,		/* texture to look up */
		miVector *const	coord);		/* texture coordinate */
miBoolean mi_lookup_scalar_texture(
		miScalar *const	scalar,		/* returned scalar */
		miState * const	state,
		const miTag	tex,		/* texture to look up */
		miVector *const	coord);		/* texture coordinate */
miBoolean mi_lookup_vector_texture(
		miVector *const	rvector,	/* returned vector */
		miState * const	state,
		const miTag	tex,		/* texture to look up */
		miVector *const	coord);		/* texture coordinate */


/*
 * texture filtering
 */

typedef struct {
		miScalar	eccmax; 	/* max. eccentricity */
		miScalar	max_minor;	/* max. minor axis length */
		miScalar	circle_radius;	/* scale blurring */
		miBoolean	bilinear;	/* bilin. subpixel interp. */
		miScalar	spare[10];	/* not used */
} miTexfilter;

miBoolean mi_lookup_filter_color_texture(
		miColor * const color,		/* returned color */
		miState * const state,
		const miTag	tex,		/* texture to look up */
		const miTexfilter *paras,	/* filter options */
		miMatrix	ST);		/* pixel->texture transform */

miBoolean mi_texture_filter_project(
		miVector	p[3],		/* pixel space points */
		miVector	t[3],		/* texture space points */
		miState *const	state,
		miScalar	disc_r, 	/* projection disc radius */
		miUint		space); 	/* texture space index */

miBoolean mi_texture_filter_transform(
		miMatrix	ST,		/* screen->texture transform */
		miVector	p[3],		/* pixel space points */
		miVector	t[3]);		/* texture space points */

/* info functions */
void	  mi_texture_info(
		const miTag	tag,		/* texture to inquire about */
		int * const	xresp,		/* store width here */
		int * const	yresp,		/* store height here */
		void ** const	paraspp);	/* store ptr to tex paras */
void	  mi_light_info(
		const miTag	tag,		/* light to inquire about */
		miVector *const orgp,		/* store origin here */
		miVector *const dirp,		/* store direction here */
		void ** const	paraspp);	/* store ptr to light paras 0*/
int	  mi_global_lights_info(
		miTag		**tag); 	/* array of light leaf insts */
void	**mi_shader_info(
		miState * const state); 	/* state with shader */
int	  mi_instance_info(
		const miTag	inst_tag,	/* instance tag */
		void ** const	paraspp,	/* store ptr to inst paras */
		void ** const	spare1, 	/* not currently used */
		void ** const	spare2, 	/* not currently used */
		void ** const	spare3);	/* not currently used */

miBoolean mi_inclusive_lightlist(
		int		*n_lights,	/* number of lights */
		miTag		**lights,	/* lightlist (array of tags) */
		miState 	*state);	/* state */
miBoolean mi_exclusive_lightlist(
		int		*n_lights,	/* number of lights */
		miTag		**lights,	/* lightlist (array of tags) */
		miState 	*state);	/* state */

miBoolean mi_tri_vectors(
		miState 	*state,
		int		which,		/* point, normal, motion, tex*/
		int		ntex,		/* if which==t, which texture*/
		miVector	**a,		/* returned vectors */
		miVector	**b,
		miVector	**c);

miBoolean mi_texture_interpolate(
		miState		*state,		/* current state */
		miUint		space,		/* texture space index */
		miScalar	*result);	/* interpolated coordinate */

miBoolean mi_raster_unit(
		miState 	*state, 	/* shader state with camera */
		miVector	*x,		/* ret. X unit basis vector */
		miVector	*y);		/* ret. Y unit basis vector */

miImg_image * mi_output_image_open(
		miState 	*state, 	/* shader state with camera */
		miUint		idx);		/* output image index */

void mi_output_image_close(
		miState 	*state, 	/* shader state with camera */
		miUint		idx);		/* output image index */

/* autovolume functions */
miInteger mi_volume_num_shaders(
		miState 	*state);	/* state */
miInteger mi_volume_cur_shader(
		miState 	*state);	/* state */
miColor *mi_volume_user_color(
		miState 	*state);	/* state */
miTag const *mi_volume_tags(
		miState 	*state);	/* state */
miTag const *mi_volume_instances(
		miState 	*state);	/* state */

/* ray tracing functions */
miBoolean mi_trace_eye(
		miColor 	*result,	/* result */
		miState 	*state, 	/* state */
		miVector	*origin,	/* ray origin */
		miVector	*direction);	/* ray direction */
miBoolean mi_trace_transparent(
		miColor 	*result,	/* result */
		miState 	*state);	/* state */
miBoolean mi_trace_reflection(
		miColor 	*result,	/* result */
		miState 	*state, 	/* state */
		miVector	*direction);	/* ray direction */
miBoolean mi_trace_refraction(
		miColor 	*result,	/* result */
		miState 	*state, 	/* state */
		miVector	*direction);	/* ray direction */
miBoolean mi_trace_environment(
		miColor 	*result,	/* result */
		miState 	*state, 	/* state */
		miVector	*direction);	/* direction */
miBoolean mi_trace_probe(
		miState 	*state, 	/* state */
		miVector const	*direction,	/* direction */
		miVector const	*origin);	/* origin */
miBoolean mi_trace_continue(
		miColor 	*result,	/* result */
		miState 	*state);	/* state */
miBoolean mi_sample_light(
		miColor * const result, 	/* result */
		miVector *const dir,		/* opt. result direction */
		float	* const dot_nl, 	/* opt. result dot of N, L */
		miState * const state,		/* state */
		const miTag	light_inst,	/* light instance */
		int * const	samples);	/* # samples */
miBoolean mi_trace_light(
		miColor 	*result,	/* result */
		miVector	*dir,		/* opt. result direction */
		float		*dot_nl,	/* opt. result dot of N, L */
		miState 	*state, 	/* state */
		miTag		light_inst);	/* light instance */
miBoolean mi_trace_shadow(
		miColor * const result, 	/* result */
		miState * const state); 	/* state */
miBoolean mi_trace_shadow_seg(
		miColor * const result, 	/* result */
		miState * const state); 	/* state */
miBoolean mi_continue_shadow_seg(
		miColor * const result, 	/* result */
		miState * const state); 	/* state */

/* set opacity color. mental ray will use the last value set. note
   that if opacity is not set, alpha is assumed to hold the opacity. */
void mi_opacity_set(
		miState 	*state, 	/* shader state */
		miColor const * const opacity); /* opacity to store */

/* get opacity colour. return miBoolean is miTRUE
   if it was previously set, otherwise miFALSE. */
miBoolean mi_opacity_get(
		miState 	*state, 	/* shader state */
		miColor 	*opacity);	/* where to store opacity */

/* compute reflection / transmission directions */
void mi_reflection_dir(
		miVector	*dir,		/* in direction */
		miState 	*state);	/* state */
miBoolean mi_refraction_dir(
		miVector	*dir,		/* in direction */
		miState 	*state, 	/* state */
		miScalar	ior_in, 	/* in index of refraction */
		miScalar	ior_out);	/* out index of refraction */
void mi_reflection_dir_specular(	/* same as mi_reflection_dir */
		miVector	*dir,		/* in direction */
		miState 	*state);	/* state */
void mi_reflection_dir_glossy(
		miVector	*dir,		/* out direction */
		miState 	*state, 	/* state */
		miScalar	shiny); 	/* shiny coefficient */
void mi_reflection_dir_glossy_x(
		miVector	*dir,		/* out direction */
		miState 	*state, 	/* state */
		miScalar	shiny, 		/* shiny coefficient */
                const  double	sample[2]);	/* 2-dim sample to use */
void mi_reflection_dir_anisglossy(
		miVector	*dir,		/* out direction */
		miState 	*state, 	/* state */
		miVector	*u,		/* u direction */
		miVector	*v,		/* v direction */
		miScalar	shiny_u,	/* shiny coefficient in u dir*/
		miScalar	shiny_v);	/* shiny coefficient in v dir*/
void mi_reflection_dir_anisglossy_x(
		miVector	*dir,		/* out direction */
		miState 	*state, 	/* state */
		const miVector	*u,		/* u direction */
		const miVector	*v,		/* v direction */
		miScalar	shiny_u,	/* shiny coefficient in u dir*/
		miScalar	shiny_v,	/* shiny coefficient in v dir*/
                const  double	sample[2]);	/* 2-dim sample to use */
void mi_reflection_dir_diffuse(
		miVector	*dir,		/* out direction */
		miState 	*state);	/* state */
void mi_reflection_dir_diffuse_x(
		miVector	*dir,		/* out direction */
		miState 	*state,		/* state */
                const  double	sample[2]);	/* 2-dim sample to use */
miBoolean mi_transmission_dir_specular( /*same as mi_refraction_dir*/
		miVector	*dir,		/* in direction */
		miState 	*state, 	/* state */
		miScalar	ior_in, 	/* in index of refraction */
		miScalar	ior_out);	/* out index of refraction */
miBoolean mi_transmission_dir_glossy(
		miVector	*dir,		/* out direction */
		miState 	*state, 	/* state */
		miScalar	ior_in, 	/* in index of refraction */
		miScalar	ior_out,	/* out index of refraction */
		miScalar	shiny); 	/* shiny coefficient */
miBoolean mi_transmission_dir_glossy_x(
		miVector	*dir,		/* out direction */
		miState 	*state, 	/* state */
		miScalar	ior_in, 	/* in index of refraction */
		miScalar	ior_out,	/* out index of refraction */
		miScalar	shiny, 		/* shiny coefficient */
                const  double	sample[2]);	/* 2-dim sample to use */
miBoolean mi_transmission_dir_anisglossy(
		miVector	*dir,		/* out direction */
		miState 	*state, 	/* state */
		miScalar	ior_in, 	/* in index of refraction */
		miScalar	ior_out,	/* out index of refraction */
		miVector	*u,		/* u direction */
		miVector	*v,		/* v direction */
		miScalar	shiny_u,	/* shiny coefficient in u dir*/
		miScalar	shiny_v);	/* shiny coefficient in v dir*/
miBoolean mi_transmission_dir_anisglossy_x(
		miVector	*dir,		/* out direction */
		miState 	*state, 	/* state */
		miScalar	ior_in, 	/* in index of refraction */
		miScalar	ior_out,	/* out index of refraction */
		const miVector	*u,		/* u direction */
		const miVector	*v,		/* v direction */
		miScalar	shiny_u,	/* shiny coefficient in u dir*/
		miScalar	shiny_v,	/* shiny coefficient in v dir*/
		const  double	sample[2]);	/* 2-dim sample to use */
void mi_transmission_dir_diffuse(
		miVector	*dir,		/* out direction */
		miState 	*state);	/* state */
void mi_transmission_dir_diffuse_x(
		miVector	*dir,		/* out direction */
		miState 	*state,		/* state */
		const  double	sample[2]);	/* 2-dim sample to use */
void mi_scattering_dir_diffuse(
		miVector	*dir,		/* result: out direction */
		miState 	*state);
void mi_scattering_dir_directional(
		miVector	*dir,		/* result: out direction */
		miState 	*state,
		miScalar	directionality);/* -1: back ... 1: forward */
miScalar mi_scattering_pathlength(
		miState		*state,
		miScalar	k);

/* choose scatter type */
miRay_type mi_choose_scatter_type(
		miState 	*state,
		miScalar	transp, 	/* refl = 1-transp */
		miColor 	*diffuse,
		miColor 	*glossy,
		miColor 	*specular);
miRay_type mi_choose_simple_scatter_type(
		miState 	*state,
		miColor 	*rdiffuse,
		miColor 	*rspecular,
		miColor 	*tdiffuse,
		miColor 	*tspecular);
int mi_choose_lobe(
		miState 	*state,
		miScalar	r);		/* probability of first lobe */

/* evalution of reflection models */
miScalar mi_phong_specular(
		miScalar	spec_exp,	/* specular exponent */
		miState 	*state, 	/* state */
		miVector	*dir);		/* direction */
miScalar mi_blong_specular(
		miScalar	spec_exp,	/* specular exponent */
		miState 	*state, 	/* state */
		miVector	*dir);		/* direction */
miScalar mi_blinn_specular(
		miVector	*di,		/* incident direction */
		miVector	*dr,		/* direction of reflection */
		miVector	*n,		/* surface normal */
		miScalar	roughness,	/* average microfacet slope */
		miScalar	ior);		/* index of refraction */
miBoolean mi_cooktorr_specular(
		miColor 	*result,	/* resulting color */
		miVector	*di,		/* incident direction */
		miVector	*dr,		/* direction of reflection */
		miVector	*n,		/* surface normal */
		miScalar	roughness,	/* average microfacet slope */
		miColor 	*ior);		/* relative ior for 3 wavelgt*/
void mi_fresnel_specular(
		miScalar	*ns,
		miScalar	*ks,
		miScalar	spec_exp,	/* specular exponent */
		miState 	*state, 	/* state */
		miVector	*dir,		/* direction */
		miScalar	ior_in, 	/* in index of refraction */
		miScalar	ior_out);	/* out index of refraction */
miScalar mi_ward_glossy(
		miVector	*di,		/* incident direction */
		miVector	*dr,		/* direction of reflection */
		miVector	*n,		/* normal */
		miScalar	shiny); 	/* glossyness */
miScalar mi_ward_anisglossy(
		miVector	*di,		/* incident direction */
		miVector	*dr,		/* direction of reflection */
		miVector	*n,		/* normal */
		miVector	*u,		/* u direction */
		miVector	*v,		/* v direction */
		miScalar	shiny_u,	/* glossyness in u direction */
		miScalar	shiny_v);	/* glossyness in u direction */
miScalar mi_fresnel(
		miScalar	n1,
		miScalar	n2,
		miScalar	t1,
		miScalar	t2);
miScalar mi_fresnel_reflection(
		miState 	*state,
		miScalar	ior_in,
		miScalar	ior_out);
miScalar mi_schlick_scatter(
		miVector	*di,		/* incident direction */
		miVector	*dr,		/* direction of reflection */
		miScalar	directionality);/* -1:back, 0:isot., 1:forw. */

/* photon tracing */
miBoolean mi_photon_reflection_specular(
		miColor 	*energy,	/* energy carried by photon */
		miState 	*state,
		miVector	*dir);
miBoolean mi_photon_reflection_glossy(
		miColor 	*energy,	/* energy carried by photon */
		miState 	*state,
		miVector	*dir);
miBoolean mi_photon_reflection_diffuse(
		miColor 	*energy,	/* energy carried by photon */
		miState 	*state,
		miVector	*dir);
miBoolean mi_photon_transmission_specular(
		miColor 	*energy,	/* energy carried by photon */
		miState 	*state,
		miVector	*dir);
miBoolean mi_photon_transmission_glossy(
		miColor 	*energy,	/* energy carried by photon */
		miState 	*state,
		miVector	*dir);
miBoolean mi_photon_transmission_diffuse(
		miColor 	*energy,	/* energy carried by photon */
		miState 	*state,
		miVector	*dir);
miBoolean mi_photon_transparent(
		miColor 	*energy,	/* photon flux */
		miState 	*parent);
miBoolean mi_photon_light(
		miColor 	*energy,
		miState 	*state );
miBoolean mi_photon_volume_scattering(
		miColor 	*energy,	/* energy carried by photon */
		miState 	*state,
		miVector	*dir);

/*
 * user frame buffer access
 */

miBoolean mi_fb_put(
		miState 	*state, 	/* current shader state */
		int		fb,		/* frame buffer number, 0..n */
		void		*data); 	/* sample data to store */
miBoolean mi_fb_get(
		miState 	*state, 	/* current shader state */
		int		fb,		/* frame buffer number, 0..n */
		void		*data); 	/* sample data to store */

/*
 * contour output shader interface
 */

typedef struct miContour_endpoint {
		miVector	point;		/* x, y in screen coords, */
						/* z in camera coordinates */
		miColor 	color;
		float		width;
		miVector	motion;
		miVector	normal;
		miTag		material;
		int		label;
} miContour_endpoint;


miBoolean mi_get_contour_line(
		miContour_endpoint *p1,
		miContour_endpoint *p2);

void mi_add_contour_lines(
		miContour_endpoint p1[],	/* list of first endpoints */
		miContour_endpoint p2[],	/* list of second endpoints */
		int		n);		/* number of contour lines */


/*
 * parallel safe random number generator
 */

double mi_par_random(
		miState		*state);

/*
 * more functions, defined by LIB. They are here because they need miState.
 * All take the state, a result vector pointer, and an input vector pointer.
 */

void mi_point_to_world(
		miState *const, miVector *const, miVector *const);
void mi_point_to_camera(
		miState *const, miVector *const, miVector *const);
void mi_point_to_object(
		miState *const, miVector *const, miVector *const);
void mi_point_to_raster(
		miState *const, miVector *const, miVector *const);
void mi_point_to_light(
		miState *const, miVector *const, miVector *const);
void mi_point_from_world(
		miState *const, miVector *const, miVector *const);
void mi_point_from_camera(
		miState *const, miVector *const, miVector *const);
void mi_point_from_object(
		miState *const, miVector *const, miVector *const);
void mi_point_from_light(
		miState *const, miVector *const, miVector *const);
void mi_vector_to_world(
		miState *const, miVector *const, miVector *const);
void mi_vector_to_camera(
		miState *const, miVector *const, miVector *const);
void mi_vector_to_object(
		miState *const, miVector *const, miVector *const);
void mi_vector_to_light(
		miState *const, miVector *const, miVector *const);
void mi_vector_from_world(
		miState *const, miVector *const, miVector *const);
void mi_vector_from_camera(
		miState *const, miVector *const, miVector *const);
void mi_vector_from_object(
		miState *const, miVector *const, miVector *const);
void mi_vector_from_light(
		miState *const, miVector *const, miVector *const);
void mi_normal_to_world(
		miState *const, miVector *const, miVector *const);
void mi_normal_to_camera(
		miState *const, miVector *const, miVector *const);
void mi_normal_to_object(
		miState *const, miVector *const, miVector *const);
void mi_normal_to_light(
		miState *const, miVector *const, miVector *const);
void mi_normal_from_world(
		miState *const, miVector *const, miVector *const);
void mi_normal_from_camera(
		miState *const, miVector *const, miVector *const);
void mi_normal_from_object(
		miState *const, miVector *const, miVector *const);
void mi_normal_from_light(
		miState *const, miVector *const, miVector *const);

/*
 * geometry shaders
 */

miBoolean mi_geoshader_add_result(
		miTag		*result,	/* shader result */
		const miTag	item);		/* scene item to add */

typedef struct {				/* intersection description */
		double		dist;		/* distance along ray */
		miVector	point;		/* point in render space */
		miVector	normal_geom;	/* geometric normal */
		float		bary[4];	/* barycentric coord. 0..2 */
		void		*pri;		/* internal primitive */
		int		pri_idx;	/* primitive index */
		miTag		instance;	/* hit instance */
} miIntersect;

/*end_matter_base_h*/

/*------------ mi_rcgi.h ------------*/

miBoolean mi_store_volume_photon(
  miColor* energy, 
  miState* state);

miBoolean mi_store_photon(
  miColor* energy, 
  miState* state);

miBoolean mi_compute_irradiance(
  miColor* result, 
  miState* state);

miBoolean mi_compute_irradiance_backside(
  miColor* result, 
  miState* state);

miBoolean mi_compute_directional_irradiance(
  miColor* result, 
  miState* state,
  const miScalar r,
  const miScalar g1,
  const miScalar g2);

miBoolean mi_compute_volume_irradiance(
  miColor* result, 
  miState* state);
/* Type for overwriting global finalgather options */
typedef struct miIrrad_options {
  int		size;			/* size of the structure */
  /* finalgather part */
  int		finalgather_rays;	/* no. rays in final gather */
  miScalar	finalgather_maxradius;	/* maxdist for finalgather */
  miScalar	finalgather_minradius;	/* mindist for finalgather */
  miCBoolean	finalgather_view;	/* radii in raster pixels ? */
  miUchar	finalgather_filter;	/* finalgather ray filter */
  miUchar	padding1[2];		/* padding */

  /* globillum part */
  int		globillum_accuracy;	/* no. GI photons in estimation */
  miScalar	globillum_radius; 	/* maxdist for GI photons */

  
  /* caustics part */
  int		caustic_accuracy;	/* no. caustic photons in estimation */
  miScalar	caustic_radius;		/* maxdist for caustic photons */

  /* this structure may be extended in the future */
} miIrrad_options;

/* 
 * Initialize values from the default options. Call this macro, when overwrite
 * defaults where necessary.
 */
#define miIRRAD_DEFAULT(irrad, state) {                                       \
  (irrad)->size                  = sizeof(miIrrad_options);                 \
  (irrad)->finalgather_rays      = (state)->options->finalgather_rays;      \
  (irrad)->finalgather_maxradius = (state)->options->finalgather_maxradius; \
  (irrad)->finalgather_minradius = (state)->options->finalgather_minradius; \
  (irrad)->finalgather_view      = (state)->options->finalgather_view;      \
  (irrad)->finalgather_filter    = (state)->options->finalgather_filter;    \
  (irrad)->padding1[0]           = 0;                                       \
  (irrad)->padding1[1]           = 0;                                       \
  (irrad)->globillum_accuracy    = (state)->options->globillum_accuracy;    \
  (irrad)->globillum_radius      = (state)->options->globillum_radius;      \
  (irrad)->caustic_accuracy      = (state)->options->caustic_accuracy;      \
  (irrad)->caustic_radius        = (state)->options->caustic_radius;        \
}

/*****************************************************************************
 * This function is a replacement for mi_compute_irradiance and 
 * mi_compute_irradiance_backside. If irrad_options is NULL, the result is 
 * 1/M_PI of mi_compute_irradiance(_backside), side depending on the face 
 * parameter. If it is not NULL, fields in the miIrrad_options overwrite the 
 * global values from miOptions structure.
 */
miBoolean mi_compute_avg_radiance(
  miColor			*result,
  miState			*state,
  miUchar			face,	/* 'f' front, 'b' back */
  struct miIrrad_options	*irrad_options);/* options to overwrite */

/*------------ mi_rcfg.h ------------*/

typedef enum miFinalgather_store_mode {
  miFG_STORE_COMPUTE		= 1,	/* compute FG point */
  miFG_STORE_SET		= 2	/* set FG to given value */
} miFinalgather_store_mode;

miBoolean mi_finalgather_store(
  miColor	*result,
  miState	*state,
  int 		mode);

/*------------ mi_phen.h ------------*/

						/*begin_matter_base_h*/
miTag		mi_phen_clone			(struct miState *, miTag);
void		mi_phen_unclone			(miTag);
						/*end_matter_base_h*/
					/*begin_matter_base_h*/
/* state shaders */
typedef enum miShader_state_op {
        miSHADERSTATE_STATE_INIT,	/* call shader for local init */
        miSHADERSTATE_STATE_EXIT,	/* call shader for local exit */
        miSHADERSTATE_SAMPLE_INIT,	/* shader call before other shaders */
	miSHADERSTATE_SAMPLE_EXIT	/* shader call after other shaders */ 
} miShader_state_op;

typedef struct miShader_state_arg {
	miShader_state_op	op;	/* what the state shader should do */
	miBoolean		shader_return;	/* result prev. shader call*/
} miShader_state_arg;
					/*end_matter_base_h*/

/*------------ mi_smethod.h ------------*/

miBoolean mi_sample(
	double 		*sample, 
	int 		*instance,
	struct miState 	*state, 
	const miUshort 	dimension, 
	miUint 		*n);

/*------------ mi_lprof.h ------------*/

miScalar mi_lightprofile_value(
        const void*     vlprof,         /* opaque pointer to light profile */
        miScalar        phi,            /* horizontal angle */
        miScalar        costheta,       /* cos of vertical angle */
        const miVector* pos,            /* sample position */
        miBoolean       rel);           /* return abs. or relative values */

miScalar mi_lightprofile_sample(
        miState*        state,
        miTag           light_profile,
        miBoolean       rel);

/*------------ mi_spectrum.h ------------*/

						/*begin_matter_base_h*/
struct miSpectrum;
typedef struct miSpectrum miSpectrum;
struct miSpectrum_iterator;
typedef struct miSpectrum_iterator miSpectrum_iterator;
struct miSpectrum_data;
						/*end_matter_base_h*/
						/*begin_matter_base_h*/

const char* 		mi_spectrum_enabled	(void);

struct miSpectrum*	mi_spectrum_blueprint	(void);
struct miSpectrum*	mi_spectrum_create	(const struct 
							miSpectrum_data *sd);
struct miSpectrum*	mi_spectrum_create_inheap (const struct
							miSpectrum_data *sd);
struct miSpectrum*	mi_spectrum_clone	(const struct miSpectrum *s);

void			mi_spectrum_delete	(struct miSpectrum *s);
void			mi_spectrum_delete_fromheap (struct miSpectrum *s);
void			mi_spectrum_clear	(struct miSpectrum *s);

unsigned int 		mi_spectrum_size	(const struct miSpectrum *s);
void			mi_spectrum_assign	(struct miSpectrum *s,
						 const struct miSpectrum* in);
void			mi_spectrum_assign_data (struct miSpectrum *s,
						 const struct miSpectrum_data* sd);
void			mi_spectrum_copy	(struct miSpectrum *s,
						 const struct miSpectrum* in);
void			mi_spectrum_mul		(struct miSpectrum *s,
						 const struct miSpectrum* in);
void			mi_spectrum_add		(struct miSpectrum *s,
						 const struct miSpectrum* in);
void			mi_spectrum_scale	(struct miSpectrum *s,
						 miScalar scale);
void			mi_spectrum_scale_add	(struct miSpectrum  *s,
						 const struct miSpectrum* in,
						 miScalar   scale);
void			mi_spectrum_lerp	(struct miSpectrum *s,
						 const struct miSpectrum *in,
						 miScalar lerp);
void			mi_spectrum_color_add	(struct miSpectrum *s,
						 const miColor	   *c);	
miScalar    		mi_spectrum_ciexyz	(miColor	   *c,
						 const struct miSpectrum *s);

struct miSpectrum_iterator* mi_spectrum_iterator_create(
						struct miSpectrum* s,
						miScalar quality);
void			mi_spectrum_iterator_destroy(
					    struct miSpectrum_iterator* it);

struct miSpectrum*	mi_spectrum_iterator_begin(
					struct miSpectrum_iterator* it);
struct miSpectrum*	mi_spectrum_iterator_next(
					struct miSpectrum_iterator* it);
struct miSpectrum*	mi_spectrum_iterator_end(
					struct miSpectrum_iterator* it);

unsigned int		mi_spectrum_iterator_size(void);

miScalar		mi_spectrum_eval	(const struct miSpectrum* s,
						 miScalar   wavelen);
miScalar		mi_spectrum_value_set	(struct miSpectrum* s,
						 miScalar   wavelen,
						 miScalar   value);
miScalar		mi_spectrum_value_add	(struct miSpectrum* s,
						 miScalar   wavelen,
						 miScalar   value);
void			mi_spectrum_range_get	(const struct miSpectrum* s,
						 miScalar*  wavelen_min,
						 miScalar*  wavelen_max);
miScalar		mi_spectrum_linewidth	(const struct miSpectrum* s,
						 miScalar   wavelen);
miBoolean		mi_spectrum_check	(const struct miSpectrum* s);

						/*end_matter_base_h*/

#ifdef __cplusplus
}
#endif

#endif
