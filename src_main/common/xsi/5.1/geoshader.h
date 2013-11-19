/******************************************************************************
 * Copyright 2005-2006 by mental images GmbH, Fasanenstr. 81, D-10623 Berlin
 * All rights reserved.
 ******************************************************************************
 * Author:	thomas
 * Created:	21.04.2005
 * Purpose:	extended header file for mental ray geometry shaders
 *
 * Note: This file is generated automatically from the mental ray sources, do
 * not edit. Some definitions exist for internal reasons and are subject to
 * change between mental ray versions. See the mental ray user's manual for
 * a definitive description of the shader interface.
 *****************************************************************************/

#ifndef GEOSHADER_H
#define GEOSHADER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RAY2
 #error you must include shader.h before geoshader.h
#endif


/*------------ mi_raylib.h ------------*/

/*begin_matter_geom_h*/
#define miNAMESIZE	80	/* length of function names and other names */
#define miPATHSIZE	1024	/* length of file paths */
/*end_matter_geom_h*/

/*------------ mi_scene.h ------------*/

/*begin_matter_geom_h*/
struct miInstance;			/* make structure known to compiler */
struct miMaterial;			/* make structure known to compiler */
typedef int (*miInh_func)(void **, void *, int, void *, int,
			struct miInstance *, struct miInstance *, miMatrix,
			struct miMaterial *);
/*end_matter_geom_h*/
/*begin_matter_geom_h*/
/*
 * Vertices are stored as lists of miGeoIndex values. The first always
 * references the point-in-space vector. The meaning of the others is
 * determined by this structure. Offsets are relative to the first index
 * for this vertex and can never be zero (because index 0 is the point
 * in space). Zero is therefore used to indicate "none" or "missing".
 * no_derivs is implicitly either 0 or 2; no_derivs2 either 0 or 3.
 */

typedef struct miVertex_content {
	miUchar		sizeof_vertex;	/* size of a vertex in miGeoIndex's */
	miUchar		normal_offset;	/* when 0, not present */
	miUchar		derivs_offset;	/* surf derivs, when 0, not present */
	miUchar		derivs2_offset;	/* surf 2nd derivs, when 0, not pr. */
	miUchar		motion_offset;	/* when 0, not present */
	miUchar		no_motions;	/* number of motion vectors */
	miUchar		texture_offset;	/* when 0, not present */
	miUchar		no_textures;	/* number of textures */
	miUchar		bump_offset;	/* when 0, not present */
	miUchar		no_bumps;	/* number of bumps */
	miUchar		user_offset;	/* when 0, not present */
	miUchar		no_users;	/* number of user vectors */
} miVertex_content;


/*
 * Hair has both per-hair data and per-vertex data, and needs a different
 * structure than miVertex_content because of this. Since hair uses only two
 * structures per hair-object, we relax the space constraints and use shorts.
 * For all fields, examine the counter to see if there are data present, not
 * the offset field. offsets are in miScalar amounts, ie. typically 4 bytes.
 */

typedef struct miHair_content {
	miUshort  sizeof_vertex;  /* size of a vertex in scalars      */
	miUshort  normal_offset;
	miUshort  no_normals;     /* number of normal scalars, 0 or 3 */
	miUshort  motion_offset;
	miUshort  no_motions;     /* number of motion scalars         */
	miUshort  texture_offset;
	miUshort  no_textures;    /* number of texture scalars        */
	miUshort  radius_offset;
	miUshort  no_radii;       /* number of radius scalars, 0 or 1 */
	miUshort  user_offset;
	miUshort  no_users;       /* number of user scalars           */
} miHair_content;


/*
 * Vertices contain references to vectors, which are stored in another
 * list. The list is divided into seven "sections" that permit simple
 * transformation of the box without traversing the vertices: points,
 * normals, surface derivatives, motion, texture, bump basis, user-
 * defined. A section is empty if no vertex needs it. This structure
 * defines the layout of the sections in the vector list.
 * We use miGeoIndex to reference them.
 * (miVector_list is no longer used, the boxes use miGeoVector_list.)
 */

typedef struct miGeoVector_list {
	miGeoIndex	no_vectors;	/* total number of input vectors */
	miGeoIndex	no_points;	/* number of points in space */
	miGeoIndex	no_normals;	/* number of normals */
	miGeoIndex	no_derivs;	/* number of 1st/2nd surface derivs */
	miGeoIndex	no_motions;	/* number of motion vectors */
	miGeoIndex	no_textures;	/* number of texture coordinates */
	miGeoIndex	no_bumps;	/* number of bump basis vectors */
	miGeoIndex	no_users;	/* number of user-defined vectors */
} miGeoVector_list;
/*end_matter_geom_h*/
/*begin_matter_geom_h*/
void *mi_scene_edit	(const miTag	tag);
void mi_scene_edit_end	(const miTag	tag);
/*end_matter_geom_h*/
/*begin_matter_geom_h*/
struct miState;
miBoolean mi_geoshader_tesselate
			(struct miState	*state,
			 miTag		*leaves,
			 miTag		root);
miBoolean mi_geoshader_tesselate_end
			(miTag		leaves);
/*end_matter_geom_h*/

/*------------ mi_scene_db.h ------------*/

						/*begin_matter_geom_h*/
/*****************************************************************************
 **				tree nodes				    **
 *****************************************************************************
 *
 * Element type:    miSCENE_INSTANCE
 * Data type:	    miInstance
 * Sizes:	    int param_size
 * Defaults:	    all nulls, two identity matrices, parameter
 *		    size as given by size argument
 */

enum miGenMotionType {
	miGM_INHERIT = 0,
	miGM_TRANSFORM,
	miGM_OFF
};

typedef struct miTransform {
	miTag		function;		/* function, may be null tag */
	float		time;			/* the last evaluation time */
	miMatrix	global_to_local;	/* transformation in world */
	miMatrix	local_to_global;	/* transformation to world */
	miCBoolean	identity;		/* SCENE: matrices are ident.*/
	miCBoolean	spare[3];		/* SCENE: not used */
	int		id;			/* SCENE: unique transform ID*/
} miTransform;


typedef struct miInstance {
	miTransform	tf;			/* space transformation */
	miMatrix	motion_transform;	/* motion transformation */
	miTag		item;			/* instanced item */
	miTag		material;		/* inherited material or list*/
	miTag		parent; 		/* SCENE: leaf inst. parent */
	miTag		boxes;			/* SCENE: renderable repres. */
	miTag		next;			/* SCENE: leaf instance list */
	miTag		prev;			/* SCENE: leaf instance list */
	int		mtl_array_size; 	/* if mtl array, array size */
	miTag		light_obj_ring; 	/* SCENE: obj-light links */
	miTag		userdata;		/* optional user data blocks */
	miUint		label;			/* optional label */
	miTag		geogroup;		/* SCENE: geomshader group */
	miCBoolean	off;			/* ignore this instance */
	miUint1 	gen_motion;		/* motion information */
	miUint1 	visible;		/* visible ? */
	miUint1 	shadow; 		/* bits: 0=cast, 1=receive */
	miUint1 	reflection;		/* bits: 0=cast, 1=receive */
	miUint1 	refraction;		/* bits: 0=cast, 1=receive */
	miUint1		transparency;		/* bits: 0=cast, 1=receive */
	miUint1 	caustic;		/* caustic bitmap */
	miUint1 	globillum;		/* globillum bitmap */
	miUint1		finalgather;		/* finalgather bitmap */
	char		face;			/* a=all, f=front, b=back */
	miCBoolean	temp_material;		/* SCENE: has inherited mtl */
	miUint1 	select; 		/* selectable when picking? */
	miCBoolean	mtl_override;		/* inst mtl overrides obj mtl*/
	miUshort	history_size;		/* leaf inst: # of hist tags */
	miTag		approx_list;		/* 10 inherited miApprox's: */
						/* v,t,s,c,g, displ v,t,s,c,g*/
	miCBoolean	approx_override;	/* inst approx overr. obj app*/
	miUint1 	hardware;		/* render inst. with hardware */
	miUint1 	shadowmap; 		/* casts shadowmap shadows? */
	char		spare;			/* not used */
	miTag		param_decl;		/* parameter declaration */
	int		param_size;		/* parameter size in bytes */
	char		param[8];		/* arbitrary-sized parameters*/
} miInstance;

/* SWAP: change also scn_instance.c */

#define miLEAFHISTORY(i) ((miTag *)(&(i)->param[(i)->param_size +3&~3]))


/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_GROUP
 * Data type:	    miGroup
 * Sizes:	    int max_kids, int max_connections
 * Defaults:	    all nulls, max_* as given by size arguments.
 */

typedef struct miGroup {
	miGeoScalar	merge;		/* merge tolerance */
	miBoolean	placeholder;	/* is this a demand-loaded group? */
	int		spare0; 	/* not used */
	miBoolean	merge_group;	/* perform merging on members? */
	int		max_kids;	/* number of kids allocated */
	int		no_kids;	/* number of kids actually used */
	int		max_connections;/* number of connections allocated */
	int		no_connections; /* number of connections used */
	miTag		userdata;	/* optional user data blocks */
	miUint		label;		/* optional label */
	int		spare1[3];	/* not used */
					/* ---- if !placeholder */
	miTag		kids[2];	/* kid list (instances) */
					/* ---- if placeholder */
	miTag		groupjob;	/* the real group with geom. */
	miTag		filename;	/* file name to read */
	miScalar	maxdisplace;	/* max return of displ shader*/
	int		spare2; 	/* not used, for padding */
	miVector	bbox_min;	/* bounding box: low corner */
	miVector	bbox_max;	/* bounding box: high corner */
	miVector	bbox_min_m;	/* bbox shift at T==1.0 (low) */
	miVector	bbox_max_m;	/* bbox shift at T==1.0 (hi) */
} miGroup;
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/

#define miMERGE_MIN	0.001		/* minimum value of miGroup.merge */

typedef struct miConnection {
	miTag		instance[2];	/* the two face type object instances*/
	miGeoIndex	face[2];	/* indices of the two miFace's */
	miGeoIndex	curve[2];	/* indices of the two miCurves*/
	miGeoRange	range[2];
} miConnection;
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
#define miGROUP_GET_CONNECTIONS(g) \
	((miConnection *)&(g)->kids[(g)->max_kids + 1 & ~1])
					/*end_matter_geom_h*/
						/*begin_matter_geom_h*/


/*****************************************************************************
 **				lights					    **
 *****************************************************************************
 *
 * Element type:    miSCENE_LIGHT
 * Data type:	    miLight
 * Sizes:	    ---
 * Defaults:	    see documentation
 */

						/*end_matter_geom_h*/
						/*begin_matter_geom_h*/

struct miLight_rectangle {
	miVector	edge_u;
	miVector	edge_v;
};

struct miLight_disc {
	miVector	normal;
	miScalar	radius;
};

struct miLight_sphere {
	miScalar	radius;
};

struct miLight_cylinder {
	miVector	axis;
	float		radius;
};

struct miLight_object {
	miTag		object;
};

struct miLight_shmap {
	miTag		camera;		/* optional camera for shadow maps */
	miScalar	accuracy;	/* detail shadow map min sample dist.*/
	miScalar	filter_u;	/* currently only size 1 supported */
	miScalar	filter_v;	/* currently only size 1 supported */
	miSint2		samples;	/* n*n samples per pixel */
	miUchar		filter;		/* currently only box 'b' supported */
	miUchar		type;		/* detail shadow map: color or alpha */
};


typedef struct miLight {
	enum miLight_type	type;		/* light type */
	enum miLight_area	area;		/* area? */
	miScalar		exponent;	/* global illum. 1/r^(2*exp)*/
	unsigned int		caustic_store_photons;/*caus.photons to store*/
	unsigned int		global_store_photons; /*glob.photons to store*/
	miColor 		energy; 	/* global illum. intensity */
	miTag			shader; 	/* light shader */
	miTag			emitter;	/* photon emitter shader */
	miVector		origin; 	/* opt. origin */
	miVector		direction;	/* opt. normalized direction */
	float			spread; 	/* size of spot? (cos angle) */
	union {
		struct miLight_rectangle    rectangle;
		struct miLight_disc	    disc;
		struct miLight_sphere	    sphere;
		struct miLight_cylinder     cylinder;
		struct miLight_object	    object;
	}			primitive;	/* area primitive */
	short			samples_u;	/* area u samples */
	short			samples_v;	/* area v samples */
	short			low_samples_u;	/* low area u samples */
	short			low_samples_v;	/* low area v samples */
	short			low_level;	/* switch to low at this lvl */
	miUint1			shadowmap_flags;/* indicate shadow map modes */
	miUint1			dirlight_has_org;/* infinite light with org? */

	miBoolean		use_shadow_maps;/* for this light */
	miTag			shadowmap_file; /* the shadow map file */
	int			shadowmap_resolution;  /* resolution */
	float			shadowmap_softness;    /* sample region size */
	int			shadowmap_samples;     /* #samples */
	miBoolean		visible;	/* visible? area lights only */
	miUint			label;		/* light label */
	miTag			userdata;	/* optional user data blocks */
	unsigned int		caustic_emit_photons; /*caus.photons to emit */
	unsigned int		global_emit_photons;  /*glob.photons to emit */
	miTag			hardware;	/* hardware light shader */
	short			shmap_h_min;	/* shadowmap window */
	short			shmap_h_max;	/* should be floats, but we */
	short			shmap_v_min;	/* do not want to increase */
	short			shmap_v_max;	/* struct size */
	float			transparent;	/* experimental shmap feature */
	miScalar		shadowmap_bias;	/* move shmap Z back by bias */
	struct miLight_shmap	shmap;		/* shadowmap data */
	int			spare2[2];	/* not used */
} miLight;

						/*end_matter_geom_h*/
						/*begin_matter_geom_h*/

/*****************************************************************************
 **				materials				    **
 *****************************************************************************
 *
 * Element type:    miSCENE_MATERIAL
 * Data type:	    miMaterial
 * Sizes:	    ---
 * Defaults:	    all nulls
 */

typedef struct miMaterial {
	miBoolean	opaque; 		/* no transparency? */
	miTag		shader; 		/* material shader */
	miTag		displace;		/* opt. displacement shader */
	miTag		shadow; 		/* opt. shadow shader */
	miTag		volume; 		/* opt. volume shader */
	miTag		environment;		/* opt. environment shader */
	miTag		contour;		/* opt. contour shader */
	miTag		photon; 		/* opt. photon RT shader */
	miTag		photonvol;		/* opt. photon volume shader */
	miTag		lightmap;		/* used for lightmaps */
	miTag		hardware;		/* hardware shader */
} miMaterial;
#define miSCENE_MTL_NSHADERS 8


/*****************************************************************************
 **				functions				    **
 *****************************************************************************
 *
 * Element type:    miSCENE_FUNCTION
 * Data type:	    miFunction
 * Sizes:	    int param_size
 * Defaults:	    all nulls
 *
 * The miDecl_fileparm structure is stored as the parameter structure of
 * miFunctions of type miFUNCTION_OUTFILE (i.e. a file output statement).
 * File outputs, like output shaders, are stored in miFunctions because
 * they are easy to manage and easy to chain and attach to a camera.
 */
						/*end_matter_geom_h*/
						/*begin_matter_geom_h*/

typedef struct {			/* parameters[] if miFUNCTION_OUTFILE*/
	miImg_format	format; 	/* format or miIMG_FORMAT_PS */
	miImg_type	type;		/* type or miIMG_TYPE_CONT */
	miUint		one;		/* must have value 1, byte order chk */
	miTag		colorprofile;	/* tag for output color profile */
	char		filename[4];	/* file name, space for null bytes, */
					/* additional 8 float parameters */
					/* are appended after filename, see */
					/* mi_api_output_file_parameter() */
} miDecl_fileparm;
						/*end_matter_geom_h*/
						/*begin_matter_geom_h*/


/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_FUNCTION_DECL
 * Data type:	    miFunction_decl
 * Sizes:	    int decl_size
 * Defaults:	    all nulls, type miFUNCTION_C
 *
 * When adding a new miTYPE_*, it's a very good idea to not use chars and
 * shorts, because we could run into structure padding problems. Keep in
 * mind that pointers may be 64 bits. miTYPE_TEX is a catchall for map values;
 * when the parser finds a map value it doesn't know yet which type of map.
 */

typedef enum {
	miTYPE_BOOLEAN = 0,			/* simple types: used for */
	miTYPE_INTEGER, 			/* returns and parameters */
	miTYPE_SCALAR,
	miTYPE_STRING,
	miTYPE_COLOR,
	miTYPE_VECTOR,
	miTYPE_TRANSFORM,
	miTYPE_SHADER,				/* complex types: used for */
	miTYPE_SCALAR_TEX,			/* parameters only */
	miTYPE_COLOR_TEX,
	miTYPE_VECTOR_TEX,
	miTYPE_LIGHT,
	miTYPE_STRUCT,
	miTYPE_ARRAY,
	miTYPE_TEX,
	miTYPE_MATERIAL,			/* phenomenon types */
	miTYPE_GEOMETRY,
	miTYPE_LIGHTPROFILE,			/* light profiles as args */
	miTYPE_DATA,				/* free-form user data */
	miTYPE_SPECTRUM,			/* light spectrum as args */
	miNTYPES
} miParam_type;

						/* for decl->apply bitmap */
#define miAPPLY_LENS		(1 << miSHADER_LENS)
#define miAPPLY_MATERIAL	(1 << miSHADER_MATERIAL)
#define miAPPLY_LIGHT		(1 << miSHADER_LIGHT)
#define miAPPLY_SHADOW		(1 << miSHADER_SHADOW)
#define miAPPLY_ENVIRONMENT	(1 << miSHADER_ENVIRONMENT)
#define miAPPLY_VOLUME		(1 << miSHADER_VOLUME)
#define miAPPLY_TEXTURE 	(1 << miSHADER_TEXTURE)
#define miAPPLY_PHOTON		(1 << miSHADER_PHOTON)
#define miAPPLY_GEOMETRY	(1 << miSHADER_GEOMETRY)
#define miAPPLY_DISPLACE	(1 << miSHADER_DISPLACE)
#define miAPPLY_PHOTON_EMITTER	(1 << miSHADER_PHOTON_EMITTER)
#define miAPPLY_OUTPUT		(1 << miSHADER_OUTPUT)
#define miAPPLY_LIGHTMAP	(1 << miSHADER_LIGHTMAP)
#define miAPPLY_PHOTONVOL	(1 << miSHADER_PHOTONVOL)
#define miAPPLY_STATE		(1 << miSHADER_STATE)
#define miAPPLY_CONTOUR		(1 << miSHADER_CONTOUR)
#define miAPPLY_OTHER		(1 << miSHADER_OTHER)

typedef struct miPhen_decl {
	int		n_subtags;		/* # of subshader/mtl tags */
	miTag		root;			/* root attachment point */
	miTag		lens;			/* optional lens shaders */
	miTag		output; 		/* optional output shaders */
	miTag		volume; 		/* optional volume shaders */
	miTag		environment;		/* optional environm. shaders*/
	miTag		geometry;		/* optional geometry shaders */
	miTag		contour_store;		/* opt'l contour store func */
	miTag		contour_contrast;	/* opt'l contour contrast f. */
	int		lens_seqnr;		/* opt'l sequence number */
	int		output_seqnr;		/* opt'l sequence number */
	int		volume_seqnr;		/* opt'l sequence number */
	int		environment_seqnr;	/* opt'l sequence number */
	/* Fuzzy booleans (0=dont care, 1=false, 2=true) */
	miCBoolean	scanline;		/* need scanline? */
	miCBoolean	trace;			/* need ray tracing? */
	/* Normal Booleans (these cannot be set explicitly off): */
	miCBoolean	deriv1; 		/* need first derivatives? */
	miCBoolean	deriv2; 		/* need second derivatives? */
	miUchar 	mintextures;		/* not used */
	miUchar 	minbumps;		/* not used */
	miUchar 	volume_level;		/* optional volume level */
	miUchar 	parallel;		/* parallel output shader */
	char		shadow; 		/* 0, 1, 'l' sort, 's' segm */
	char		face;			/* 'f'ront, 'b'ack, 'a'll */
	char		render_space;		/* 'c'amera, 'o'bject, 0 any */
	miCBoolean	cloned; 		/* delete decl when del. shd */
} miPhen_decl;

typedef struct miFunction_decl {
	miPointer	sparep[2];
	enum miFunction_type type;		/* C function or phenomenon */
	miParam_type	ret_type;		/* return type of shader */
	int		declaration_size;	/* size of declaration */
	int		result_size;		/* size of result struct */
	int		version;		/* shader version from .mi */
	miUint		apply;			/* what can it be used for? */
	miPhen_decl	phen;			/* if type==miFUNCTION_PHEN */
	miTag		hardware;		/* obsolete */
	int		spare[1];		/* not used */
	miTag		defaults;		/* default values or 0 */
	char		name[miNAMESIZE];	/* ascii name */
	char		declaration[4]; 	/* declaration string */
} miFunction_decl;
						/*end_matter_geom_h*/
						/*begin_matter_geom_h*/
#define miDECL_SUBTAG(d,i) ((miTag *)&(d)->declaration\
					[(d)->declaration_size + 3&~3])[i]


/*****************************************************************************
 **				geometry: geoboxes			    **
 *****************************************************************************
 *
 * Element type:    miSCENE_BOX
 * Data type:	    miGeoBox
 * Sizes:	    int nvec, int nvert, int npri, miBox_type type,
 *		    miBoolean moving
 * Defaults:	    all nulls except sizes
 */

enum miBox_type { miBOX_TRIANGLES, miBOX_ALGEBRAICS };
enum miTriangle_projection {
	TRI_XY_P = 0, TRI_YZ_P, TRI_ZX_P,
	TRI_XY_N = 4, TRI_YZ_N, TRI_ZX_N,
	TRI_DEG
};

#define miISOLATED_TRI	0x1		/* gap_flags */
#define miIS_STRIP	0x2
#define miSTRIP_BEGIN	0x4
#define miSTRIP_RIGHT	0x8

#define miIS_QUAD	0x10

typedef struct miTriangle {
	miUchar 	proj;		/* RC: miTriangle_projection flag */
	miUchar 	gap_flags;	/* flags for strip output */
	miUchar 	spare[2];	/* not used */
	miGeoIndex	a;		/* vertex index */
	miGeoIndex	b;		/* vertex index */
	miGeoIndex	c;		/* vertex index */
	miTag		material;	/* opt. material */
	miVector	normal; 	/* RC: normal */
	miScalar	d;		/* RC: distance */
} miTriangle;

typedef struct miGeoBox {
	miUint		label;		/* translator: object label */
	miTag		next_box;	/* translator or gap: box list	*/
	enum miBox_type type;		/* type of primitives */
	miBoolean	spare[3];	/* not used */
	miBoolean	mtl_is_label;	/* triangle mtls are labels */
	miUint1		spare2[2];	/* not used */
	miUint1 	select; 	/* selectable when picking? */
	miUint1 	sharp;		/* 0=smooth normals, 255=faceted */
	miCBoolean	spare3[4];	/* not used */
	miTag		userdata;	/* optional user data blocks */
	miTag		facedata;	/* optional data for triangles */
	miBoolean	hardware;	/* content uses hardware shading */
	miTag		intersect;	/* intersection calculation */
	miVertex_content vert_info;	/* size  content of vertices */
	miGeoVector_list vect_info;	/* sections of vector array */
	miGeoIndex	no_vertices;	/* number of vertices */
	miGeoIndex	no_primitives;	/* number of primitives */
	miVector	bbox_min;	/* bounding box: low corner */
	miVector	bbox_max;	/* bounding box: high corner */
	miVector	vectors[1];	/* vectors, vertices, primitives */
} miGeoBox;
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/

#define miBOX_GET_VERTICES(box) 					\
	((miGeoIndex *)((miVector *)((box) + 1) +			\
		 (box)->vect_info.no_vectors-1))

#define miBOX_GET_PRIMITIVES(box)					\
	(void *)(miBOX_GET_VERTICES(box) +				\
		((box)->no_vertices * (box)->vert_info.sizeof_vertex))

#define miBOX_GET_CONTENTS(box,vertices)		\
	((void *)((miGeoIndex *)((vertices) = miBOX_GET_VERTICES(box)) + \
		((box)->no_vertices * (box)->vert_info.sizeof_vertex)))

#define miBOX_PRIMITIVE_SIZE(box)					\
	((box)->type == miBOX_TRIANGLES ?				\
		sizeof(miTriangle) : sizeof(miAlgeb_pri))


/*end_matter_geom_h*/
/*begin_matter_geom_h*/
/*****************************************************************************
 **                             light spectra                               **
 *****************************************************************************
 * Element type:   miSCENE_SPECTRUM
 * Data type:      miSpectrum_data
 * Defaults:       31 equidistant coeffs between 400 and 700 nm, 
 *		   all coeffs set to 0, repres = miSPECTRUM_GRID 
 *		   tristimulus color 0
 */

typedef enum miSpectrum_representation {
	miSPECTRUM_SAMPLES	= 1,	/* spectrum is sampled */
	miSPECTRUM_LINE		= 2,	/* just one spectral line */
	miSPECTRUM_FOURIER	= 3,	/* fourier representation */

	miSPECTRUM_TYPE		= 0x000000ff, /* up to 255 spectr. represent. */
	miSPECTRUM_FORCE	= 0x00000100, /* if set transform file repres.*/
	miSPECTRUM_READ		= 0x00000200, /* set if spec is on disk */
	miSPECTRUM_GRID		= 0x00000400, /* set if samples are equidist */
	miSPECTRUM_FLAGS	= 0x0000ff00, /* flags within repres. */  
	miSPECTRUM_MAGIC	= 0x5eaf0000, /* spectrum magic id */
	miSPECTRUM_MAGIC_MASK   = 0x7fff0000  /* Sun, IBM don't like hi-bit */
} miSpectrum_representation;

typedef struct miSpectrum_data {
	miSpectrum_representation	representation; 
	int				n_coeff;
	int				n_comp;
	miScalar			coeff[1];
} miSpectrum_data;


/*****************************************************************************
 **				color profiles				    **
 *****************************************************************************
 *
 * Element type:    miSCENE_COLORPROFILE
 * Data type:	    miColor_profile
 * DefaultsL:	    all nulls
 */

/* definitions for color representations for rendering and output device */
#define miCPROF_DEFAULT		0x00
#define miCPROF_RGB		0x01	/* generic RGB */
#define miCPROF_NTSC		0x02	/* RGB with Rec.601 color primaries */	
#define miCPROF_HDTV		0x03	/* RGB with Rec.709 color primaries */
#define miCPROF_SHARP		0x04	/* RGB with sharp primaries */
#define miCPROF_CIEXYZ		0x05	/* CIE XYZ */
#define miCPROF_CIEXYY		0x06	/* CIE xyY */
#define miCPROF_CIELUV		0x07	/* CIE L*u*v* */
#define miCPROF_CIELAB		0x08	/* CIE L*a*b* */
#define miCPROF_SPECTRUM	0x09	/* internal spectrum representation  */
#define miCPROF_CUSTOM		0x0a	/* user defined tristimulus values   */
#define miCPROF_LINERGB		0x0b	/* R = 700nm, G = 546nm, B = 436nm   */
#define miCPROF_BOXRGB		0x0c	/* R >= 600nm, B <= 400nm, else G    */
#define miCPROF_SIMPLECMY	0x0d	/* simple cyan, yellow, magenta	     */
#define miCPROF_SIMPLECMYK	0x0e	/* simple cyan,yellow,magenta, black */
#define miCPROF_SPACEMASK	0x3f
#define miCPROF_WHITEPOINT	0x40	/* perform white point correction    */
#define miCPROF_CID_NOT_ENOUGH	0x80	/* one of above color space id's is  */
					/* not enough to perform color space */
					/* from/to internal color space,     */
					/* need entire color profile         */
typedef struct miColor_profile {
	miUint1		space;		/* color space */
	miCBoolean	white_adapt;	/* perform white space adaption */
	miUint2		flags;	
	miTag		cprof_tag;	/* tag of profile load job */
	miTag		filename;	/* profile on disk */
	miScalar	gamma;		/* gamma correction (XYZ->space)   */
	miScalar	gamma_offset;	/* x' = (1+scale)*x^gamma - scale, */
	miScalar	gamma_scale;    /* if x > offset; for x < offset:  */ 
	miScalar	gamma_slope;	/* x' = slope*x                    */
	miColor		white;		/* CIE XYZ white point */
	miScalar	trafo[9];	/* CIE XYZ to color space trafo */	
	miScalar	inv_trafo[9];	/* color space to CIE XYZ trafo */
	miUint		spare[3];
} miColor_profile;


/*****************************************************************************
 **				light profiles				    **
 *****************************************************************************
 *
 * Element type:    miSCENE_LIGHTPROFILE
 * Data type:	    miLight_profile
 * DefaultsL:	    all nulls
 */

typedef enum miLight_profile_basis {
	miLP_BASIS_NONE    = 0,
	miLP_BASIS_HERMITE = 1
} miLight_profile_basis;


/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_LIGHTPROFILE
 * Data type:	    miLight_profile
 * DefaultsL:	    all nulls
 */

typedef enum miLight_profile_std {
	miLP_STD_NONE	   = 0,
	miLP_STD_IES	   = 1,
	miLP_STD_EULUMDAT  = 2
} miLight_profile_std;

typedef struct miLight_profile {
	miTag		profile;	/* in placeholder, points to
					 * real profile job, 0 else */
	miLight_profile_std   format;	/* data file standard */
	miLight_profile_basis base;	/* basis for interpolation */
	int		quality;	/* interpolation quality */
	miTag		filename;	/* orig data file */
	int		n_vert_angles;	/* there are n theta angles */
	int		n_horz_angles;	/* there are n phi angles */
	int		flags;		/* symmetry properties */
	miTag		std_data;	/* format specific data */
	miScalar	val_max;	/* scaling factor for table */
	miScalar	phi0;		/* starting angle */
	miScalar	theta0; 	/* starting angle */
	miScalar	dphi;		/* angle increments */
	miScalar	dtheta; 	/* angle increments */
	miScalar	table[1];
} miLight_profile;



/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_LIGHTPROFILE_STD
 * Data type:	    miIes_light or miEulumdat_light
 * Defaults:	    all nulls (invalid)
 */

/*
 * Version of Ies light profile format
 */

typedef enum miIes_version {
	miIESNA_LM_63_1986 = 0,
	miIESNA_LM_63_1991,
	miIESNA_LM_63_1995	/* to be continued ... */
} miIes_version;


/*
 * Enumeration of IES standard labels
 */

typedef enum miIes_label {
	/* recommended minimum */
	miIES_LABEL_TEST = 0,	/* Test report number/laboratory */
	miIES_LABEL_MANUFAC,	/* Luminaire manufactirer */
	miIES_LABEL_LUMCAT,	/* Luminaire catalog number */
	miIES_LABEL_LUMINAIRE,	/* Luminaire description */
	miIES_LABEL_LAMPCAT,	/* Lamp catalog number */
	miIES_LABEL_LAMP,	/* Lamp description */

	/* rest */
	miIES_LABEL_DATE,	/* Date of report */
	miIES_LABEL_NEARFIELD,	/* Near field photometry D1,D2,D3 */
	miIES_LABEL_BALLAST,	/* Ballast in report */
	miIES_LABEL_BALLASTCAT, /* Ballast catalog number */
	miIES_LABEL_MAINTCAT,	/* Maintaince category */
	miIES_LABEL_FLUSHAREA,	/* Light emitting area, m2, proj @76 degrees*/
	miIES_LABEL_COLORCONSTANT,	/* For CIE glare control */
	miIES_LABEL_OTHER,	/* Other info */
	miIES_LABEL_SEARCH,	/* Search string */

	/* Number of supported fields */
	miIES_LABEL_FIELD_NUMBER	= miIES_LABEL_SEARCH,
	miIES_LABEL_FIELD_DB_NUMBER	= 32	/* reserve for future use */
} miIes_label;

/*
 * Supported photometric types
 */
typedef enum miIes_photometric_type {
	miIES_PHOTOMETRIC_TYPE_C = 1,
	miIES_PHOTOMETRIC_TYPE_B = 2,
	miIES_PHOTOMETRIC_TYPE_A = 3
} miIes_photometric_type;


/*
 * Unit types
 */
typedef enum miIes_unit_type {
	miIES_UNIT_TYPE_FEET	= 1,
	miIES_UNIT_TYPE_METER	= 2
} miIes_unit_type;

/*
 * Tilt types
 */
typedef enum miIes_tilt_type {
	miIES_LAMP_SYMM 	= 0,	/* Tilt plays no role */
	miIES_LAMP_VERT 	= 1,
	miIES_LAMP_HORIZ	= 2,
	miIES_LAMP_TILT 	= 3
} miIes_tilt_type;

/*
 * Tilt structure
 */

typedef struct miIes_tilt {
	miIes_tilt_type type;		/* tilt type */
	miUint		number; 	/* of angles and multipliers*/
	struct {
		miScalar angle; 	/* Increasing, degrees */
		miScalar factor;		/* Myltiplying factors */
	} pairs[1];
} miIes_tilt;

/* pairs are not swapped */


/*
 * Original light profile information as provided in IES file.
 */
typedef struct miIes_light {
	miLight_profile_std std;	/* should be  miLP_STD_IES */
	miIes_version	version;	/* IESNA version */
	miTag		filename;	/* profile file name */
	miTag		labels[miIES_LABEL_FIELD_DB_NUMBER];
					/* IES label strings */
	miTag		tilt_filename;	/* Tilt file name, if any */
	miUint		n_lamps;	/* Number of lamps */
	miScalar	lamp_lumen;	/* Lumens per lamp */
	miScalar	multiplier;
	miUint		n_vert_angles;
	miUint		n_horz_angles;
	miIes_photometric_type	photometric_type;	/* A,B,C */
	miIes_unit_type unit_type;	/* Feet or meters */
	miScalar	sizes[3];	/* Width, length, height */
	miScalar	ballast_factor;
	miScalar	ballast_lamp;	/* 86 and 91 only: ballast per lamp.*/
	miScalar	spare;		/* IESNA "future use" */
	miScalar	input_watt;
	miUint		mr_spare[16];	/* For MR / IESNA future use */
	miIes_tilt	tilt;		/* Tilt of the lamp */
} miIes_light;



/*
 * Photometric types
 */

typedef enum miEulumdat_type {
	miEULUMDAT_POINT_VERTAXISSYMM	= 1,
	miEULUMDAT_LINEAR		= 2,
	miEULUMDAT_POINT		= 3
} miEulumdat_type;


/*
 * Symmetry types
 */

typedef enum miEulumdat_symm {
	miEULUMDAT_SYMM_NONE		= 0,
	miEULUMDAT_SYMM_VERTAXIS	= 1,
	miEULUMDAT_SYMM_0_180		= 2,
	miEULUMDAT_SYMM_90_270		= 3,
	miEULUMDAT_SYMM_QUAD		= 4
} miEulumdat_symm;


/*
 * Lamp set
 */

typedef struct miEulumdat_lampset {
	miUint		n_lamps;
	miTag		lamps_type;
	miScalar	lamps_flux;		/* total, lumens */
	miTag		temperature;
	miTag		rendering_index;
	miScalar	watt;			/* including ballast */
	miUint		spare[4];		/* for future use */
} miEulumdat_lampset;



/*
 * Original light profile information as provided in EULUMDAT file. Note
 * that some data fields are not parsed and passed as miSCENE_STRING tags.
 */

typedef struct miEulumdat_light {
	miLight_profile_std	std;	/* should be  miLP_STD_EULUMDAT */
	miTag		filename;
	miTag		id;		/* company, version, format id etc. */
	miEulumdat_type type;
	miEulumdat_symm symm;
	miUint		n_horz_angles;
	miScalar	horz_step;	/* should be 0 if not constant */
	miUint		n_vert_angles;
	miScalar	vert_step;	/* should be 0 if not constant */
	miTag		report_number;
	miTag		luminaire;	/* luminaire name */
	miTag		luminaire_number;
	miTag		filename_orig;
	miTag		date;
	miScalar	luminaire_sizes[3];/* length/diam,width,height, mm */
	miScalar	lum_area_sizes[6]; /* length/diam,width,
					    * height0,90,180,270 */
	miScalar	dff;	/* downward flux fraction, real (not %) */
	miScalar	lorl;	/* light output ration luminaire,real(not %)*/
	miScalar	factor; /* conversion factor */
	miScalar	tilt;
	miScalar	direct_ratios[10];

	miUint		mr_spare[16];	/* For MR / EULUMDAT future use */

	miUint			n_lampsets;
	miEulumdat_lampset	lampsets[1];
} miEulumdat_light;

/* lampsets are not swapped */


/*****************************************************************************
 **				geometry: line boxes			    **
 *****************************************************************************
 *
 * Element type:    miSCENE_LINEBOX
 * Data type:	    miLinebox
 * Sizes:	    int nvec, int npnt, int nlin
 * Defaults:	    all nulls except sizes
 */

typedef struct miLinebox_info {
	miGeoIndex	no_vectors;	/* number of vectors		*/
	miGeoIndex	vector_offset;	/* offset of vectors, always 0	*/
	miGeoIndex	no_stripes;	/* number of stripes		*/
	miGeoIndex	stripe_offset;	/* offset of stripes in vectors */
	miGeoIndex	no_u_lines;	/* number of lines of isolines in U */
	miGeoIndex	u_line_offset;	/* offset of lines of isolines in U */
	miGeoIndex	no_v_lines;	/* number of lines of isolines in V */
	miGeoIndex	v_line_offset;	/* offset of lines of isolines in V */
	miGeoIndex	no_trims;	/* number of trimming curves	*/
	miGeoIndex	trim_offset;	/* offset of trim curve lines	*/
	miGeoIndex	no_specials;	/* number of special curves	*/
	miGeoIndex	special_offset; /* offset of lines of special curves */
} miLinebox_info;

/*:: Lineboxes do not contain anything else than ints.
     Swapping of lineboxes relies on this fact. ::*/

typedef struct miLinebox {
	miUint		label;		/* translator: object label	*/
	miTag		next_linebox;	/* translator: line box list	*/
	miTag		userdata;	/* optional user data blocks	*/
	int		spare;		/* not used			*/
	miLinebox_info	info;		/* line box info		*/
	miVector	vectors[1];	/* vectors, stripes, lines	*/
} miLinebox;

typedef struct miLine {
	miGeoIndex	no_stripes;
	miGeoIndex	total_no_points;
	miGeoIndex	line_idx;
} miLine;

typedef struct miLine_stripe {
	miGeoIndex	no_points;
	miGeoIndex	stripe[1];
} miLine_stripe;


/*****************************************************************************
 **				geometry: objects			    **
 *****************************************************************************
 *
 * Element type:    miSCENE_OBJECT
 * Data type:	    miObject
 * Sizes:	    ---
 * Defaults:	    see documentation
 */

enum miObject_type {
	miOBJECT_POLYGONS,
	miOBJECT_FACES,
	miOBJECT_BOXES,
	miOBJECT_SPACECURVES,
	miOBJECT_SUBDIVSURFS,
	miOBJECT_ALGEBRAICS,			/* for future use */
	miOBJECT_PLACEHOLDER,			/* read geometry on demand */
	miOBJECT_HAIR
};

typedef struct miPolygon_list {
	miGeoIndex	no_polygons;
	miGeoIndex	no_indices;
	miGeoIndex	no_vertices;
	miGeoVector_list vect_info;		/* contents of vector array */
	miVertex_content vert_info;		/* vertex size	content */
	miTag		polygons;		/* array of miPolygon */
	miTag		indices;		/* array of miGeoIndex */
	miTag		vertices;		/* see vert_info */
	miTag		vectors;		/* array of miGeoVector */
	miApprox	approx; 		/* poly approx technique */
} miPolygon_list;

typedef struct miFace_list {
	miGeoIndex	no_faces;
	miGeoIndex	no_surfaces;
	miGeoIndex	no_curves;
	miGeoIndex	no_specpnts;
	miGeoIndex	no_surf_scalars;
	miGeoIndex	no_curve_scalars;
	miTag		faces;			/* array of miFace */
	miTag		surfaces;		/* array of miSurface */
	miTag		curves; 		/* array of miCurve */
	miTag		specpnts;		/* array of miCurve_point */
	miTag		surf_scalars;		/* array of miGeoScalar */
	miTag		curve_scalars;		/* array of miGeoScalar */
	miTag		basis_list;		/* miBasis_list */
} miFace_list;

typedef struct miSpacecurve_list {
	miGeoIndex	no_spacecurves;
	miGeoIndex	no_curves;
	miGeoIndex	no_specpnts;
	miGeoIndex	no_curve_scalars;
	miTag		spacecurves;		/* array of miSpacecurve */
	miTag		curves; 		/* array of miCurve */
	miTag		specpnts;		/* array of miCurve_point */
	miTag		curve_scalars;		/* array of miGeoScalar */
	miTag		basis_list;		/* miBasis_list */
	miBoolean	pad;			/* not used */
} miSpacecurve_list;

typedef struct miSubdivsurf_list {
	miGeoIndex	no_subdivsurfs;
	miTag		subdivsurfs;
} miSubdivsurf_list;

typedef struct miAlgebraic_list {
	miGeoIndex	not_yet_supported;
} miAlgebraic_list;

typedef struct miPlaceholder_list {
	miTag			object; 	/* the real object with geom.*/
	miTag			filename;	/* file name to read */
	enum miObject_type	type;		/* type of object created */
} miPlaceholder_list;


typedef struct miHair_list {
	miGeoIndex	no_hairs;		/* number of hairs */
	miGeoIndex	no_scalars;		/* number of vectors */
	miHair_content	hair_info;		/* data attached to hairs */
	miHair_content	vert_info;		/* data attached to vertices,*/
	miTag		hairs;			/* 1st hair vec: miInteger[] */
	miTag		scalars;		/* control points: miScalar[]*/
	miInteger	approx; 		/* quality control of approx */
	miTag		material;		/* material for all hairs */
	miScalar	radius; 		/* hair radius, default 1 */
	int		degree; 		/* bezier degree 1..3, def. 1*/
	int		space_max_size; 	/* space subdiv. leaf size */
	int		space_max_depth;	/* space subdiv. tree depth */
} miHair_list;



typedef struct miBox_list {
	miTag		box;			/* miGeoBox */
	miApprox	approx;			/* displacement approx */
} miBox_list;



typedef struct miObject {
	int			spare_0[1];	/* not used		     */
	miUint			volume_id;	/* autovolume identification */
	miTag			finalgather_file; /* fgmap file name, if any */
	enum miObject_type	type;		/* which in union to use     */
	miUint1			visible;	/* object visible?	     */
	miUint1			shadow; 	/* cast a shadow?	     */
	miUint1			reflection;	/* bit0=cast, bit1=receive   */
	miUint1			refraction;	/* bit0=cast, bit1=receive   */
	miUint1			transparency;	/* bit0=cast, bit1=receive   */
	miUint1 		caustic;	/* bit0=cast, bit1=receive   */
	miUint1 		globillum;	/* bit0=cast, bit1=receive   */
	miUint1			finalgather;	/* bit0=cast, bit1=receive   */
	miUint1 		select; 	/* selectable when picking?  */
	char			face;		/* a=all, f=front, b=back    */
	miUint1			spare_1[5];	/* not used */
	miCBoolean		fine;		/* API: has "fine" miApprox  */
	miBoolean		view_dependent; /* miOBJECT_FACES only	     */
	miBoolean		mtl_is_label;	/* poly/surf mtls are labels */
	miScalar		maxdisplace;	/* max return of displ shader*/
	miBoolean		hardware;	/* use hardware shading      */
	miTag			userdata;	/* optional user data blocks */
	miUint			label;		/* optional label	     */
	miVector		bbox_min;	/* bounding box: low corner  */
	miVector		bbox_max;	/* bounding box: high corner */
	miVector		bbox_min_m;	/* bbox shift at T==1.0 (low)*/
	miVector		bbox_max_m;	/* bbox shift at T==1.0 (hi) */
	miTag			functions;	/* SCENE: material req list  */
	unsigned int		n_functions;	/* SCENE: # tags in functions*/
	miCBoolean		mbox_explicit;	/* motion box given by user  */
	miSint1 		min_samples;	/* per-object oversampling,  */
	miSint1 		max_samples;	/* defaults -128 and 127     */
	miUint1 		shadowmap; 	/* casts shadowmap shadows?  */
	miUchar 		spare_3[4];	/* not used */
	miScalar		ray_offset;	/* ray offset for the object */
	miTag			approx_list;	/* list of miApproximations  */
	/* eight-byte aligned union */
	union {
	    miPolygon_list	polygon_list;
	    miFace_list 	face_list;
	    miBox_list		box_list;
	    miSpacecurve_list	spacecurve_list;
	    miSubdivsurf_list	subdivsurf_list;
	    miPlaceholder_list	placeholder_list;
	    miHair_list 	hair_list;
	}			geo;		/* geometry */
} miObject;

/* SWAP: change scn_object.c for Algebraics */

#define miU		0
#define miV		1
#define miUV_DIM	2
#define miMIN		0
#define miMAX		1
#define miMINMAX_DIM	2


/*****************************************************************************
 **				geometry: object splitting
 *****************************************************************************
 *
 * Element type:    ---
 * Data type:	    miSub_object
 * Sizes:	    int n_sub_objects
 * Defaults:	    all nulls
 */

typedef struct miSub_object {
	miVector	bbox_min;	/* sub object bounding box min */
	miVector	bbox_max;	/* sub object bounding box max */
	miVector	bbox_min_m;	/* sub object motion box min */
	miVector	bbox_max_m;	/* sub object motion box max */
	miTag		box;		/* tessellation job tag */
	miUint1		spare1;
	miUint1 	approx_flag;	/* visible,trace,shadow:miApprox_flag*/
	miUint1 	spare2[2];
} miSub_object;



/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_SPLITOBJECT
 * Data type:	    miSplit_object
 * Sizes:	    ---
 * Defaults:	    all nulls
 */

typedef struct miSplit_object {
	miGeoIndex	n_sub_objects;	/* number of sub objects */
	miSub_object	sub_objects[1]; /* array of sub objects  */
} miSplit_object;


/*****************************************************************************
 **				geometry: poly/hair/surf		    **
 *****************************************************************************
 *
 * Element type:    miSCENE_POLYGON
 * Data type:	    miPolygon[]
 * Sizes:	    int no_polys
 * Defaults:	    all nulls
 */

typedef struct miPolygon {
	miGeoIndex	no_loops;	/* 1 + (number of holes) */
	miGeoIndex	no_vertices;	/* total number of vertices + headers*/
	miGeoIndex	vertex_idx;	/* into indices list, for sharing */
	miTag		material;	/* surface properties */
	miCBoolean	convex; 	/* is polygon convex ? */
	miCBoolean	spare[3];	/* not used */
} miPolygon;


/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_FACE
 * Data type:	    miFace[]
 * Sizes:	    int no_faces
 * Defaults:	    all nulls except where otherwise noted
 */

typedef struct miFace {
	miApprox	def_trim_approx;  /* approx techn. for default trims */
	miGeoRange	range[miUV_DIM];  /* min/max for parameter vals*/
	miGeoIndex	no_curves;	  /* total number of curves */
	miGeoIndex	no_surfaces;	  /* total # of surfs: 1 geo. + xtras*/
	miGeoIndex	surface_idx;	  /* surface list index */
	miGeoIndex	curve_idx;	  /* index into the 'curves' list*/
	miTag		material;	  /* optional: material of the face */
	miVertex_content gap_vert_info;   /* For calculating box sizes */
} miFace;


/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_SURFACE
 * Data type:	    miSurface[]
 * Sizes:	    int no_surfaces
 * Defaults:	    all nulls, except where otherwise noted
 */

enum miSurface_type {
	miSURFACE_GEOMETRIC,
	miSURFACE_GEOMOTION,		/* geometric + motion vectors */
	miSURFACE_TEXTURE_2D,		/* texture with/without seam compens */
	miSURFACE_TEXTURE_3D,
	miSURFACE_BUMP, 		/* bump */
	miSURFACE_TEXBUMP_2D,		/* texture + bump */
	miSURFACE_TEXBUMP_3D,
	miSURFACE_NTYPES
};
#define miSURFACE_TEXBUMP miSURFACE_TEXBUMP_2D	/* temporary */

typedef struct miSurface {
	miApprox	approx; 		/* approx techn. for surface */
	miApprox	disp_approx;		/* approx for disp. surface */
	miGeoIndex	no_parms[miUV_DIM];	/* no of parameter values */
	miGeoIndex	no_ctls;		/* no of control points */
	miGeoIndex	no_specpnts;		/* no of special points */
	miGeoIndex	scalar_idx;		/* surf_scalar list index */
	miGeoIndex	specpnt_idx;		/* special points list index */
	miGeoIndex	basis_idx[miUV_DIM];	/* index into basis list */
	enum miSurface_type type;
	miUshort	degree[miUV_DIM];	/* from bases, for GAP */
	miUshort	ctl_dim;		/* control pnt dimension */
	miUshort	spare[3];
} miSurface;

#define miSURFACE_CTLS(S)					\
	((S)->scalar_idx + (S)->no_parms[miU] + (S)->no_parms[miV])


/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_CURVE
 * Data type:	    miCurve
 * Sizes:	    int no_curves
 * Defaults:	    see documentation
 */

enum miCurve_type {
	miCURVE_TRIM,
	miCURVE_HOLE,
	miCURVE_SPECIAL,
	miCURVE_SPACE,
	miCURVE_NTYPES
};

typedef struct miCurve {
	miApprox	approx; 	/* approx techn. and const for curve */
	miBoolean	new_loop;	/* F=concatenate to prev, T=begin new*/
	miUshort	ctl_dim;	/* control point dimension 2,3,or 4 */
					/* 2 and 3 for non-space curves */
					/* 3 and 4 for space curves */
	miUshort	degree; 	/* from basis, for ease in GAP */
	miGeoRange	range;		/* min/max for curve params. */
	miGeoIndex	no_parms;	/* number of curve parameters */
	miGeoIndex	no_ctls;	/* number of control points */
	miGeoIndex	no_specpnts;	/* number of special points */
	miGeoIndex	scalar_idx;	/* index into curve_scalar list */
	miGeoIndex	specpnt_idx;	/* index into special points list */
	miGeoIndex	basis_idx;	/* index into basis list */
	enum miCurve_type type;
	int		pad;		/* not used */
} miCurve;

#define miCURVE_CTLS(C) 	((C)->scalar_idx + (C)->no_parms)


/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_SPACECURVE
 * Data type:	    miSpacecurve[]
 * Sizes:	    int no_curves
 * Defaults:	    all nulls except where otherwise noted
 */

typedef struct miSpacecurve {
	miApprox	def_approx;	/* default approximation */
	miGeoIndex	no_curves;	/* total number of curves */
	miGeoIndex	curve_idx;	/* index into	the 'curves' list*/
	miBoolean	pad;		/* not used */
	miVertex_content gap_vert_info; /* For calculating box sizes */
} miSpacecurve;


/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_CURVPNT
 * Data type:	    miCurve_point[]
 * Sizes:	    int no_points
 * Defaults:	    all nulls
 */

typedef struct miCurve_point {
	miGeoVector	v;		/* xyz point on surface of curve at t*/
	miGeoVector2d	uv;		/* uv value of curve at t */
	miGeoScalar	t;		/* curve parameter value */
	miUshort	flags;		/* GAP: */
	miUshort	spare_flags;	/* not used */
	miCBoolean	is_t;		/* is t value present */
	miCBoolean	is_uv;		/* is uv value present */
	miCBoolean	is_v;		/* is xyz value present */
	miCBoolean	spare;		/* not used */
} miCurve_point;



/*-----------------------------------------------------------------------------
 * Element type:    miSCENE_BASIS_LIST
 * Data type:	    miBasis_list[]
 * Sizes:	    int no_bases, int no_scalars
 * Defaults:	    all nulls
 */

enum miBasis_type {
	miBASIS_NONE = 0,		/* for internal use */
	miBASIS_BEZIER,
	miBASIS_BSPLINE,
	miBASIS_CARDINAL,
	miBASIS_MATRIX,
	miBASIS_TAYLOR,
	miNBASISTYPES
};

typedef struct miBasis {
	enum miBasis_type type;
	miGeoIndex	degree; 	/* undefined for Cardinal */
	miGeoIndex	stepsize;	/* only for miBASIS_MATRIX */
	miGeoIndex	value_idx;	/* index into the basis scalar list */
} miBasis;

typedef struct miBasis_list {
	miGeoIndex	no_bases;
	miGeoIndex	no_scalars;
	miBasis 	bases[1];
} miBasis_list;

/* SWAP: change scn_basis.c */

#define miMAX_BASIS_DEGREE 21
#define miBASIS_LIST_SCALARS(bl) ((miGeoScalar *)&(bl)->bases[(bl)->no_bases])


/*****************************************************************************
 **				books					    **
 *****************************************************************************
 *
 * Element type:    miSCENE_BOOK
 * Data type:	    miBook
 * Initialize:	    linesize, page_size, swap
 * Defaults:	    all nulls except fields to be initialized
 */

#define miSCENE_BOOK_MAXSWAP	16

typedef struct miBook {
	miUint		label;		/* book type defined by application */
	miGeoIndex	no_pages;	/* number of used pages 	    */
	miGeoIndex	first_page_size;/* number of lines on first page    */
	miGeoIndex	line_size;	/* size of elements in bytes	    */
	miGeoIndex	next_free_line; /* index of next free line	    */
	miGeoIndex	last_used_line; /* index of last used line	    */
	char		swap[miSCENE_BOOK_MAXSWAP];
					/* string for change of byte order  */
	miTag		next_book;	/* tag for next book		    */
	miTag		pages[1];	/* array of page tags		    */
} miBook;


void	  *mi_scene_book_get_line	(miBook *, miGeoIndex);
void	  *mi_scene_book_allocate_line	(miBook *, miGeoIndex *);
void	   mi_scene_book_release_line	(miBook *, miGeoIndex);
void	   mi_scene_book_enumerate	(miBook *, void (*)(void*,miGeoIndex,
								void*), void*);
miGeoIndex mi_scene_book_free_blk_start (miBook *);
miGeoIndex mi_scene_book_no_used_lines	(miBook *);
miGeoIndex mi_scene_book_max_lines	(miBook *);
miBoolean  mi_scene_book_line_valid	(miBook *, miGeoIndex);
void	   mi_scene_book_sort_freelist	(miBook *);
miTag	   mi_scene_book_attach 	(miTag, miTag, miGeoIndex);
miTag	   mi_scene_book_detach 	(miTag, miGeoIndex);
void	   mi_scene_book_compactify	(miBook *, miGeoIndex **);

typedef struct miPage {
	char		swap[miSCENE_BOOK_MAXSWAP];
					/* string for change of byte order  */
	miGeoIndex	page_size;	/* number of lines on current page  */
	miGeoIndex	line_size;	/* size of elements in bytes	    */
} miPage;

					/*end_matter_geom_h*/
						/*begin_matter_geom_h*/

/*****************************************************************************
 **				geometry: new boxes			    **
 *****************************************************************************/

/*
 * miVertex_info defines a primitive vertex.
 * The length of a vertex line in number of scalars is given by line_size.
 * The element offsets in number of scalars from the beginning of the line
 * is given by the various offset members.
 *
 * The scalar offsets within a vertex line for all texture and user spaces
 * are stored in the box in an array (dim_offset gives the address of this
 * array), allowing fast access to a certain texture or user data space.
 * The dimension of a certain texture space n can be obtained by subtracting
 * the array entry for space n+1 from the entry for space n.
 * The array first defines offsets for the textures, followed by entries
 * for user data. The macros miBOX_TEX_OFFSET and miBOX_USER_OFFSET are
 * provided for access to the array elements in the box. A certain texture
 * or user data space within a line can be accessed using miVL_TEXTURE_D
 * or miVL_USER_D.
 *
 * The dim_offset array has at least one entry for texture spaces and one
 * for user data spaces, which are identical to no_textures or no_users.
 * If no_textures or no_users > 0, additional entries are present with
 * values >= 3 (defined after position in the vertex line).
 * 
 * no_textures/no_users are the number of texture/user spaces (not the
 * number of scalars). The total number of scalars for all texture spaces
 * can be obtained by dim_offset[n] - dim_offset[0] where n is either
 * no_textures or no_user.
 *
 * The scalar offset for the first user data space begin at entry
 * dim_offset[no_textures+1] (macro miBOX_USER_OFFSET). The array has
 * no_textures+no_users+2 entries.
 *
 * A 3d position is always present (line_size >= 3).
 *
 * Vertex line data is stored in an interleaved array of lines of scalars
 * in the box. There are miBOX_NO_VTXLINES lines in the box.
 * Example:
 * px py pz nx ny nz t1u t1v t2u t2v <-- line 0 vertex scalars
 * px py pz nx ny nz t1u t1v t2u t2v <-- line 1 vertex scalars
 * px py pz nx ny nz t1u t1v t2u t2v
 * 0  1  2  3  4  5  6   7   8   9       scalar offsets within line
 *
 * line_size = 10, no_textures = 2,
 * dim_offset = [6, 8, 10, 0].
 * normal_offset = 3, texture_offset = 6.
 * dimensions for texture spaces are 2, 2.
 * miBOX_TEX_OFFSET(box) points to 6, miBOX_USER_OFFSET(box) to 0.
 * 
 * In case a vertex has different texture/normal/etc vectors at a shared
 * 3d position, multiple lines are generated, i.e. 3d positions and other
 * vectors are duplicated.
 */

typedef struct miVertex_info {
	miUint2		line_size;	/* vertex line size in #scalars */
	miUchar		normal_offset;	/* when 0, not present */
	miUchar		derivs_offset;	/* surf derivs, when 0, not present */
	miUchar		derivs2_offset;	/* surf 2nd derivs, when 0, not pr. */
	miUchar		bump_offset;	/* bump basis vectors if non null */
	miUchar		no_bumps;	/* number of bump vectors */
	miUchar		motion_offset;	/* when 0, not present */
	miUchar		no_motions;	/* number of motion vectors */
	miUchar		texture_offset;	/* when 0, not present */
	miUchar		no_textures;	/* number of textures */
	miUchar		user_offset;	/* when 0, not present */
	miUchar		no_users;	/* number of user vectors */
	miUchar		spare[3];	/* not used */
} miVertex_info;


/*
 * prim_offset is an array of integers defining multiple primitive
 * lists. Each primitive list begins with a primitive type combined
 * with a count into a single 32-bit value: type is stored in bits
 * 28-31, count in bits 0-27. The primitive vertex integers follow,
 * referring to the vertex lines.
 * count specifies the number of vertex indices defined by the
 * primitive list.
 * -> Note that a material cannot change within a strip
 *
 * miBox::no_primlists is the total number of primitive lists
 * contained in the box. each primitive list consists of prims.
 * miBox::no_prims is the total number of primitives (triangles,
 * strip faces, quads etc) contained in all primitive lists.
 *
 * The box may contain user data (pdata_offset), swapped as integers,
 * interpretation is up to the application.
 * primdata_size specifies the number of 32-bit integers which are
 * assigned to each primitive from the pdata_offset array. For
 * example primdata_size=1 means that for subsequent primitives one
 * integer from pdata_offset can be used as user data.
 * pdata_size specifies the size of the whole pdata section. there
 * may be extra space after primlist data, for unique needs.
 *
 * mtl_offset is an array of integers defining materials.
 *
 * Current restrictions:
 * - Only two primlists may be defined, one for region triangles,
 *   a second for border triangles (used for split regions).
 * - Only miSCENE_PRIM_TRI primitives are supported.
 * - There must be one material defined for each triangle.
 *
 * Example: content of a primitive list defining a single
 * triangle. BOX_PRIMS will returns a pointer to the code integer.
 * miBox::no_prims=1.
 * 1 0 1 2
 * ^ ^   ^
 * | |   +- third triangle vertex: line 2
 * | +----- first triangle vertex: referring to line 0
 * +------- miSCENE_PRIMLIST_MAKE_CODE(miSCENE_PRIM_TRI, 3)
 *
 * Regions:
 * no_primlists is the number of primitive lists defining the
 * region (without border). no_border_primlists is the number
 * of primitive lists defining the border. Border primitive
 * lists are defined (in memory layout) behind all region
 * primitive lists.
 * no_prims is the total number of primitives in all region
 * primitive lists. no_border_prims is the total number of
 * primitives in all border primitive lists.
 * Border primitives also have corresponding materials.
 *
 */

/* primitive type contained in a primitive list */
typedef enum {
	miSCENE_PRIM_TRI,
	miSCENE_PRIM_TRI_STRIP,
	miSCENE_PRIM_TRI_FAN,
	miSCENE_PRIM_QUAD,
	miSCENE_PRIM_QUAD_STRIP,
	miSCENE_PRIM_LINE_STRIP
} miScene_primtype;

#define miSCENE_PRIMLIST_TYPE_MASK	0x0fffffff

/* return primitive list type */
#define miSCENE_PRIMLIST_GET_TYPE(code)		\
	(miScene_primtype)((code) >> 28)

/* get number of indices from primitive list code */
#define miSCENE_PRIMLIST_GET_COUNT(code)		\
	((code) & miSCENE_PRIMLIST_TYPE_MASK)

/* construct a primitive list id from primitive type
   and number of indices contained in the list */
#define miSCENE_PRIMLIST_MAKE_CODE(type, count)	\
	((type) << 28 | (count))

typedef struct miBox {
	miCBoolean	mtl_is_label;
	miUchar		sharp;
	miUchar		spare[2];
	miUint		label;		/* will disappear soon, don't use */

	miVector	bbox_min;	/* bounding box: low corner */
	miVector	bbox_max;	/* bounding box: high corner */

	miUint		no_primlists;	/* number of lists of prims,
					   excluding border primlists */
	miUint		no_prims;	/* total number of primitives
					   contained in all primitive lists,
					   excluding border prims */
	miUint		no_border_primlists; /* number of border primlists */
	miUint		no_border_prims;/* number of primitives contained */
					/* in all border primlists */

	miVertex_info	vertex_info;	/* vertex specification */
	miUint		primdata_size;	/* prim data: size per primitive */
	miUint		pdata_size;	/* prim data: total size (there could
					   be extra data after primlist data */
	miTag		topology;	/* adjacency information for all
					   primitives (region+border) */

	miUint		prim_offset;	/* byte offset: primitives */
	miUint		mtl_offset;	/* byte offset: materials */
	miUint		dim_offset;	/* byte offset: tex/usr offset table*/
	miUint		pdata_offset;	/* byte offset: prim data */
	miScalar	data[1];	/* vertex lines begin, more follow */
} miBox;

/* swap: also change scn_nbox.c */

/* ========== section address access ========== */

/* Return address of the first vertex line */
#define miBOX_VERTEX_LINES(box)	\
	(box)->data

/* Return address of a certain vertex line */
#define miBOX_VERTEX_LINE(box, idx)	\
	((box)->data + (idx) * (box)->vertex_info.line_size)

/* Return address of the first primitive list */
#define miBOX_PRIMS(box)		\
	(miUint*)(((char*)(box))+(box)->prim_offset)

/* Return address of the first material tag/integer */
#define miBOX_MATERIALS(box)	\
	(miTag*)(((char*)(box))+(box)->mtl_offset)

/* Return address of the first texture space dim_offset entry */
#define miBOX_TEX_OFFSET(box)	\
	(miUint*)(((char*)(box))+(box)->dim_offset)

/* Return address of the first user space dim_offset entry */
#define miBOX_USER_OFFSET(box)	\
	(miBOX_TEX_OFFSET(box) + ((box)->vertex_info.no_textures+1))

/* Return address of the first 32-bit data for primitives */
#define miBOX_PRIMDATA(box)	\
	(miUint*)(((char*)(box))+(box)->pdata_offset)

/* ========== access within a line  ========== */

/* Return vertex position */
#define miVL_POS(box, line)	\
		(miVector*)(line)

/* Return vertex normal */
#define miVL_NORMAL(box, line)	\
		(miVector*)((line)+(box)->vertex_info.normal_offset)

/* Return partial derivative u vector, v at subsequent address */
#define miVL_DERIVS1(box, line)	\
		(miVector*)((line)+(box)->vertex_info.derivs_offset)

/* Return second derivatives (three vectors in total) */
#define miVL_DERIVS2(box, line)	\
		(miVector*)((line)+(box)->vertex_info.derivs2_offset)

/* Return first bump basis vector */
#define miVL_BUMP(box, line)	\
	(miVector*)((line)+(box)->vertex_info.bump_offset)

/* Return first motion vector */
#define miVL_MOTION(box, line)	\
		(miVector*)((line)+(box)->vertex_info.motion_offset)

/* Return first texture scalar */
#define miVL_TEXTURE(box, line)	\
		(miScalar*)((line)+(box)->vertex_info.texture_offset)

/* Return address of a certain texture space. offs is an entry from
   the dim_offset array, i.e a scalar offset within the line */
#define miVL_TEXTURE_D(box, line, offs)	\
		(miScalar*)((line)+offs)

/* Return first user vertex data */
#define miVL_USER(box, line)	\
		(miScalar*)((line)+(box)->vertex_info.user_offset)

/* Return address of a certain user data space. offs is an entry from
   the dim_offset array, i.e a scalar offset within the line */
#define miVL_USER_D(box, line, offs)	\
		(miScalar*)((line)+offs)

/* ========== number of items ========== */

/* Return number of vertex lines */
#define miBOX_NO_VTXLINES(box)	\
	((box)->vertex_info.line_size == 0 ? 0 :    \
        ((miScalar*)miBOX_PRIMS(box)-(box)->data) / \
	(box)->vertex_info.line_size)

/* Return number of materials defined */
#define miBOX_NO_MATERIALS(box)	\
	(((box)->dim_offset-(box)->mtl_offset)/sizeof(miUint))

/* ========== prim data ========== */

/* Return address of primitive data for a certain primitive with
 * global index "idx" */
#define miBOX_PD(box, idx)	\
	miBOX_PRIMDATA(box)+(idx)*(box)->primdata_size

/* ========== others ========== */

/* Return position entry for line index idx */
#define miBOX_POS(box, idx)		\
	miVL_POS(box, miBOX_VERTEX_LINE(box, idx))

/* Return motion vector 'which' for line index idx */
#define miBOX_MOTION(box, idx, which)	\
	(miVL_MOTION(box, miBOX_VERTEX_LINE(box, idx))+which)

/* Return vertex triple for a certain triangle. Assumes
   that the box defines one sequential list of triangles.
   Skip the primtype/index count entry (+1) */
#define miBOX_TRI(box, idx)		\
	(miBOX_PRIMS(box)+1+(idx)*3)

/* Return material for a certain triangle, same assumptions as
   for miBOX_TRI */
#define miBOX_MATERIAL(box, idx)	\
	(*(miBOX_MATERIALS(box)+(idx)))

/* Return size of entire miBox */
#define miBOX_SIZE(box)			\
	(miUint)((box)->pdata_offset+(box)->pdata_size)

/* ========== border access ========== */

/* Return address of first border prim.
   assumes there is only one region primlist */
#define miBOX_BORDER_PRIMS(box)		\
	(miBOX_PRIMS(box) + 1 +		\
	 miSCENE_PRIMLIST_GET_COUNT(*(miBOX_PRIMS(box))))

/* Return address of first border prim material.
   assumes there is a material for every prim */
#define miBOX_BORDER_MATERIALS(box)	\
	(miBOX_MATERIALS(box) + (box)->no_prims)

/* Return material for a border triangle, same assumptions as
   for miBOX_TRI */
#define miBOX_BORDER_MATERIAL(box, idx)		\
	(*(miBOX_BORDER_MATERIALS(box)+(idx)))
						/*end_matter_geom_h*/

/*------------ mi_api.h ------------*/

/*
 * public API interface. Most of these are used by geometry shaders; some
 * are here for completeness only. This is not the geometry shader interface
 * specification; use only those calls documented in the manual.
 */

/*begin_matter_geom_h*/
/*
 * dynamic ordered lists. Lists can be created, deleted, and appended to.
 */

#define miDLIST_SCALAR		1
#define miDLIST_GEOSCALAR	2
#define miDLIST_DOUBLE		3
#define miDLIST_INTEGER		4
#define miDLIST_POINTER		5
#define miDLIST_VREF		6
#define miDLIST_TAG		7

typedef struct miDlist {
	int		type;		/* one of miDLIST_* */
	int		nb;		/* # of valid array members in dlist */
	int		nb_allocd;	/* max # of slots in dlist (internal)*/
	void		*dlist;		/* data array, 0..nb-1 */
} miDlist;

typedef struct miVref {			/* dlist data type for miDLIST_VREF */
	int		ref;		/* reference to a vertex */
	miGeoScalar	w;		/* homogenous coordinate */
} miVref;


/*
 * vertex flags for subdivision surfaces
 */

typedef enum {
	miAPI_V_SMOOTH=0,
	miAPI_V_CORNER,
	miAPI_V_CONIC,
	miAPI_V_CUSP,
	miAPI_V_DART
} miApi_vertexflags;

/*
 * Performance hints for subdivision surfaces
 */

typedef struct miApi_subdivsurf_options {
	miUint	no_basetris;		/* number of base triangles */
	miUint	no_hiratris;		/* number of hierarchy triangles */
	miUint	no_basequads;		/* number of base quads */
	miUint	no_hiraquads;		/* number of hierarchy quads */
	miUint	no_vertices;		/* number of vertices */
} miApi_subdivsurf_options;

/*
 * Subdivision surface Texture space options
 */

typedef struct miApi_texspace_options {
	miBoolean face;			/* face (linear) interpolation ? */
} miApi_texspace_options;


/*
 * API object and instgroup callbacks
 */

typedef miBoolean (*miApi_object_callback)   (miTag, void *);
typedef miBoolean (*miApi_instgroup_callback)(miTag, void *);

typedef miBoolean (*miApi_object_placeholder_callback) 
                  (miTag, miObject *, void *);

					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/

						/*----------- apidlist.c */
miDlist  *mi_api_dlist_create		(int type);
miBoolean mi_api_dlist_add		(miDlist *dlp, void *elem);
miBoolean mi_api_dlist_delete		(miDlist *dlp);

						/*----------- apisymtab.c */
miBoolean mi_api_delete			(char *);
miBoolean mi_api_delete_tree		(char *);
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miBoolean mi_api_variable_set		(char *, char *);
const char *mi_api_variable_lookup	(char *);
const char *mi_api_tag_lookup		(miTag);
miTag     mi_api_name_lookup		(char *);
miTag     mi_api_decl_lookup		(char *);
miBoolean mi_api_registry_begin		(char *);
miBoolean mi_api_registry_add		(char *, char *);
miBoolean mi_api_registry_end		(void);
const char *mi_api_registry_eval	(char *);
const char *mi_api_registry_lookup	(char *);
miBoolean mi_api_debug			(char *, char *);
miBoolean mi_api_touch			(char *);

						/*----------- apiframe.c */
miBoolean mi_api_incremental		(miBoolean);
miBoolean mi_api_private		(int);
miBoolean mi_api_version_check		(char *, int);
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miRc_options *mi_api_options_begin	(char *);
miTag     mi_api_options_end		(void);
miCamera *mi_api_camera_begin		(char *);
miTag     mi_api_camera_end		(void);
miBoolean mi_api_render			(char *, char *, char *, char *);
miBoolean mi_api_render_params		(miTag*, miTag*, miTag*, miTag*,
						miInh_func*);
void	     mi_api_render_release	(void);
miBoolean mi_api_taglist_reset		(miTag*, miDlist*);

						/*----------- apilight.c */
miLight  *mi_api_light_begin		(char *);
miTag     mi_api_light_end		(void);
miTag     mi_api_light_lookup		(char *);
miBoolean mi_api_light_shmap_camera	(char *);

						/*----------- apimaterial.c */
miMaterial *mi_api_material_begin	(char *);
miTag     mi_api_material_end		(void);
miTag     mi_api_material_lookup	(char *);
miTag     mi_api_taglist		(miDlist *);

						/*----------- apitexture.c */
typedef miBoolean (*miApi_texture_callback)   
     (miTag, struct miImg_image *, void *);
miBoolean mi_api_texpath_config		(char *, char *, char *);
miTag     mi_api_texture_begin		(char *, int, int);
void	  mi_api_texture_end		(void);
void      mi_api_texture_set_filter	(miScalar);
miBoolean mi_api_texture_array_def_begin(int, int, int);
miBoolean mi_api_texture_byte_copy	(int, miUchar *);
miTag     mi_api_texture_array_def_end	(void);
miBoolean mi_api_texture_file_size	(int, int, int, int);
miTag     mi_api_texture_file_def	(char *);
miTag     mi_api_texture_function_def	(miTag);
miTag 	  mi_api_texture_callback_def(miApi_texture_callback, 
				      miApi_texture_callback, void *);
miTag     mi_api_texture_lookup		(char *, int);
void	  mi_api_texture_set_colorprofile(char *);
int	  mi_api_texture_type_identify(char *);

						/*----------- apiinstance.c */
miInstance *mi_api_instance_begin	(char *);
miTag     mi_api_instance_end		(char *, miTag, miTag);
miBoolean mi_api_instance_approx	(miApprox *, miBoolean);
miBoolean mi_api_instance_register	(miTag, char*);

						/*----------- apigroup.c */
miBoolean mi_api_instgroup_file		(char *);
miBoolean mi_api_instgroup_callback	(miApi_instgroup_callback, void *);
miGroup  *mi_api_instgroup_begin	(char *);
miTag     mi_api_instgroup_end		(void);
miBoolean mi_api_instgroup_clear	(void);
miBoolean mi_api_instgroup_additem	(char *);
miBoolean mi_api_instgroup_delitem	(char *);
miBoolean mi_api_instgroup_register	(miTag, char*);

						/*----------- apioutput.c */
miBoolean mi_api_output_file_override	(char *, char *);
miTag     mi_api_output_file_def	(miUint, miUint, char *, char *);
miBoolean mi_api_output_file_parameter	(miTag, miInteger, void *);
miTag     mi_api_output_function_def	(miUint, miUint, miTag);
miBoolean mi_api_output_type_identify	(miUint *, miUint *, char *);
miBoolean mi_api_output_imfdisp_handle	(miTag, miTag);
miBoolean mi_api_output_list		(miTag, miTag);
miBoolean mi_api_output_colorprofile	(char *);
miBoolean mi_api_framebuffer		(miOptions *, int, char *);

						/*----------- apibasis.c */
miBoolean mi_api_basis_list_clear	(void);
miBoolean mi_api_basis_add		(char *, miBoolean, enum miBasis_type,
					 miUshort, miUshort, struct miDlist *);
miGeoIndex mi_api_basis_lookup		(char *, miBoolean *, miUshort *);

						/*----------- apiobject.c */
miObject *mi_api_object_begin		(char *);
miTag     mi_api_object_end		(void);
miBoolean mi_api_object_file		(char *);
miBoolean mi_api_object_placeholder_callback	
                                        (miApi_object_placeholder_callback,
					 miApi_object_placeholder_callback, 
					 void *);
miBoolean mi_api_object_callback	(miApi_object_callback, void *);
miBoolean mi_api_object_matrix		(miMatrix);
miBoolean mi_api_object_group_begin	(double);
miBoolean mi_api_object_group_end	(void);
miBoolean mi_api_object_group_connection
					(char *, char *, miGeoRange *, char *,
					 char *, miGeoRange *);

						/*----------- apivector.c */
miBoolean mi_api_vector_xyz_add		(miVector *);
miBoolean mi_api_geovector_xyz_add	(miGeoVector *);
miBoolean mi_api_vector_lookup		(miGeoVector *, int);

						/*----------- apivertex.c */
miBoolean mi_api_vertex_add		(int);
miBoolean mi_api_vertex_normal_add	(int);
miBoolean mi_api_vertex_deriv_add	(int, int);
miBoolean mi_api_vertex_deriv2_add	(int, int, int);
miBoolean mi_api_vertex_motion_add	(int);
miBoolean mi_api_vertex_tex_add		(int, int, int);
miBoolean mi_api_vertex_user_add	(int);
miBoolean mi_api_vertex_lookup		(miGeoVector *, int);
miBoolean mi_api_vertex_ref_add		(int, double);
miBoolean mi_api_vertex_flags_add	(miApi_vertexflags, int, miScalar);

						/*----------- apipoly.c */
miBoolean mi_api_poly_index_add		(int);
miBoolean mi_api_poly_begin		(int, char *);
miBoolean mi_api_poly_begin_tag		(int, miTag);
miBoolean mi_api_poly_hole_add		(void);
miBoolean mi_api_poly_approx		(miApprox *);
miBoolean mi_api_poly_end		(void);

						/*----------- apisubdiv.c */
miBoolean mi_api_subdivsurf_begin	(char *);
miBoolean mi_api_subdivsurf_begin_x	(char *, miApi_subdivsurf_options*);
miBoolean mi_api_subdivsurf_end		(void);
miBoolean mi_api_subdivsurf_baseface	(void);
miBoolean mi_api_subdivsurf_push	(void);
miBoolean mi_api_subdivsurf_pop		(void);
miBoolean mi_api_subdivsurf_subdivide	(int child);
miBoolean mi_api_subdivsurf_mtl		(int child, char *);
miBoolean mi_api_subdivsurf_mtl_tag	(int child, miTag);
miBoolean mi_api_subdivsurf_detail	(int mask);
miBoolean mi_api_subdivsurf_index	(int);
miBoolean mi_api_subdivsurf_crease	(int child, int mask);
miBoolean mi_api_subdivsurf_trim	(int child, int mask);
miBoolean mi_api_subdivsurf_crease_edge	(miScalar value); 
miBoolean mi_api_subdivsurf_approx	(char *, miApprox *);
miBoolean mi_api_subdivsurf_approx_displace(char *, miApprox *);
miBoolean mi_api_subdivsurf_derivative	(int d, int space);
miBoolean mi_api_subdivsurf_texspace	(miApi_texspace_options *,
					 miUint no_spaces);

						/*----------- apisurface.c */
miBoolean mi_api_surface_begin		(char *, char *);
miBoolean mi_api_surface_begin_tag	(char *, miTag);
miBoolean mi_api_surface_params		(int, char *, miGeoScalar, miGeoScalar,
					 miDlist *, miBoolean);
miBoolean mi_api_surface_curveseg	(miBoolean, enum miCurve_type, char *,
					 miGeoRange *);
miBoolean mi_api_surface_specpnt	(int, int);
miBoolean mi_api_surface_texture_begin	(miBoolean, miBoolean, char*,miDlist*,
					 miBoolean, char*, miDlist*,miBoolean);
miBoolean mi_api_surface_derivative	(int);
miBoolean mi_api_surface_end		(void);
miBoolean mi_api_surface_approx		(char *, miApprox *);
miBoolean mi_api_surface_approx_displace(char *, miApprox *);
miBoolean mi_api_surface_approx_trim	(char *, miApprox *);
miBoolean mi_api_surface_lookup		(char *, miTag *, miGeoIndex *);

						/*----------- apicurve.c */
miBoolean mi_api_curve_begin		(char *, char *, miBoolean);
miBoolean mi_api_curve_specpnt		(int, int);
miBoolean mi_api_curve_end		(miDlist *);
void	 *mi_api_curve_lookup		(char *, enum miCurve_type, miBoolean,
					 miGeoRange *, int *);
miBoolean mi_api_curve_approx		(char *, miApprox *);
miBoolean mi_api_curve_approx_store	(miDlist *, miApprox *);

						/*------ apispacecurve.c */
miBoolean mi_api_spacecurve_begin	(char *);
miBoolean mi_api_spacecurve_curveseg	(miBoolean, char *, miGeoRange *);
miBoolean mi_api_spacecurve_approx	(char *, miApprox *);
miBoolean mi_api_spacecurve_end		(void);

						/*----------- apicode.c */
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/

typedef char *miParameter;
						/*----------- apideclare.c */
miFunction_decl *mi_api_funcdecl_begin	(miParameter *, char *, miParameter *);
miTag     mi_api_funcdecl_end		(void);
miParameter *mi_api_parameter_decl	(miParam_type, char *, int);
miParameter *mi_api_parameter_append	(miParameter *, miParameter *);
miBoolean mi_api_parameter_default	(miParam_type, void *);
miBoolean mi_api_parameter_child	(miParameter *, miParameter *);
miBoolean mi_api_parameter_lookup	(miParam_type *, miBoolean *, int *,
					 miTag, miBoolean, char *);
miBoolean mi_api_parameter_path_lookup	(miTag *, int *, int *, miParam_type *,
					 char *);
miBoolean mi_api_parameter_offset_lookup(miParam_type *, miBoolean *, char
					 **, int *, miTag, miBoolean, int);

					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miBoolean mi_api_function_delete	(miTag *);
miTag     mi_api_function_append	(miTag, miTag);
miTag     mi_api_function_assign	(char *);
miBoolean mi_api_function_call		(char *);
miTag     mi_api_function_call_end	(miTag);
miBoolean mi_api_parameter_name		(char *);
miBoolean mi_api_parameter_value	(miParam_type, void *, int *, int *);
miBoolean mi_api_parameter_shader	(char *);
miBoolean mi_api_parameter_interface	(char *);
miBoolean mi_api_parameter_push		(miBoolean);
miBoolean mi_api_parameter_pop		(void);
miBoolean mi_api_new_array_element	(void);
miBoolean mi_api_shader_add		(char *, miTag);
miBoolean mi_api_shader_call		(miTag, char *, char *);
miBoolean mi_api_shader_call_x		(miColor *, miTag, char *, char *,
					 void*);

						/*----------- apidata.c */
typedef miBoolean (*miApi_data_callback)   (miTag, miUserdata *, void *);

miUserdata *mi_api_data_begin		(char *, int, void *);
miTag     mi_api_data_end		(void);
miTag     mi_api_data_append		(miTag, miTag);
miBoolean mi_api_data_byte_copy		(int, miUchar *);
miTag     mi_api_data_lookup		(char *);
miBoolean mi_api_data_callback		(miApi_data_callback, 
					 miApi_data_callback, void *);

						/*----------- apiphen.c */
miFunction_decl *mi_api_phen_begin	(miParameter *, char *, miParameter *);
miTag     mi_api_phen_end		(void);
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miBoolean mi_api_scope_begin		(char *);
miBoolean mi_api_scope_end		(void);
char	 *mi_api_scope_apply		(char *);
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miHair_list *mi_api_hair_begin		(void);
miBoolean    mi_api_hair_info		(int, char, int);
miScalar    *mi_api_hair_scalars_begin	(int);
miBoolean    mi_api_hair_scalars_end	(int);
miGeoIndex  *mi_api_hair_hairs_begin	(int);
miBoolean    mi_api_hair_hairs_end	(void);
miBoolean    mi_api_hair_hairs_add	(int);
void	     mi_api_hair_end		(void);
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miBoolean    mi_api_lppath_config	(char *, char *, char *);
miLight_profile *mi_api_lightprofile_begin(char *);
miTag        mi_api_lightprofile_end    (void);
miTag        mi_api_lightprofile_lookup (char *);
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miBoolean    mi_api_spectrum_begin	(char *name);
miTag	     mi_api_spectrum_end	(void);
miBoolean    mi_api_spectrum_pair_add	(miScalar, miScalar);
miTag	     mi_api_spectrum_lookup	(char *name);

					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/

struct miColor_profile;
struct miColor_profile *mi_api_colorprofile_begin(char *name);
miTag	     mi_api_colorprofile_end	(void);
miUint1	     mi_api_colorprofile_space	(char *cpname);
void	     mi_api_colorprofile_white	(miColor	*white,
					 int		temperature,
					 miScalar	intensity);
void	     mi_api_colorprofile_gamma  (miScalar	gamma,
					 miScalar	offset,
					 miBoolean	force);
miBoolean    mi_api_colorprofile_custom (miColor_profile *cprof,
					 miMatrix	 m);

					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miTag	     mi_api_pass_save_def	(miUint, miUint, int, char *);
miTag	     mi_api_pass_prep_def	(miUint, miUint, int, char *,
					 char *, miTag);
miTag	     mi_api_pass_merge_def	(miUint, miUint, int, miDlist *,
					 char *, miTag);
miTag	     mi_api_pass_delete_def	(char *);
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miGeoBox     *mi_api_trilist_begin	(miVertex_content*,
					 miUint, miUint, miUint);
miBoolean    mi_api_trilist_approx	(miApprox*);
miBoolean    mi_api_trilist_vectors	(miVector*, miUint);
miBoolean    mi_api_trilist_vertices	(miGeoIndex*, miUint);
miBoolean    mi_api_trilist_triangle_tag (miTag, miGeoIndex*);
miBoolean    mi_api_trilist_triangle	(char*, miGeoIndex*);
miBoolean    mi_api_trilist_triangles	(miGeoIndex*, miUint no_triangles);
miBoolean    mi_api_trilist_end		(void);
					/*end_matter_geom_h*/
					/*begin_matter_geom_h*/
miBox	    *mi_api_primlist_begin	(miVertex_info*, miUint, miUint,
					 miUint, miUint, miUint, miUint);
miBoolean    mi_api_primlist_border	(miUint, miUint);
miBoolean    mi_api_primlist_topology	(miUint*);
miBoolean    mi_api_primlist_dimensions (miUint*, miUint*);
miBoolean    mi_api_primlist_approx	(miApprox*);
miBoolean    mi_api_primlist_end	(void);
					/*end_matter_geom_h*/

/*------------ mi_geo.h ------------*/

/*begin_matter_geom_h*/

#define miGEO_MAX_CDIM 	4	/* Max. no. of control dimensions =
                 	 	   Max. no. of spatial dimensions + 1 
				   (to account for rational surfaces). */
/*end_matter_geom_h*/
/*begin_matter_geom_h*/
typedef miGeoScalar miGeoVector_3d[3];
/*end_matter_geom_h*/
/*begin_matter_geom_h*/

/*  Variable-size array data structure and typedef macro  */

#define array_of(t) struct t ## Ar 					      \
                    {   t 		*ar; 				      \
					/* pointer to the array elements */   \
                        miGeoIndex 	num; 				      \
					/* number of used elements */	      \
                        miGeoIndex 	size; 				      \
					/* maximum number of elements */      \
                        miGeoIndex 	elsize;				      \
					/* size in bytes of single element */ \
                    }

typedef array_of(miGeoScalar)  		miGeoScalarAr;



/*end_matter_geom_h*/

/*------------ mi_geomo.h ------------*/

/*begin_matter_geom_h*/

/* Datastructures for modelling */
/* Typedef for curves which make up spacecurves */
typedef struct miGeomoCurve {
      	miGeoScalarAr		ctl_pnts[miGEO_MAX_CDIM];
	miGeoScalarAr		parm; /* parms for Bez, 
					 knots for bsplines */
       	miUshort		degree;
       	miBoolean		rat;
      	miBasis			basis;
      	miGeoScalar		nu; /* tension parameter */
} miGeomoCurve;

typedef array_of(miGeomoCurve)  		miGeomoCurveAr;

/* Typedef for surfaces, used by the lofting routines */

typedef struct miGeomoSurface {
      	miGeomoCurveAr		v_curves;  /* curves whose param are
      					    * in the v-direction 
      					    */
	miGeoScalarAr		u_parm;    /* knots for bspline */
      	miBasis			u_basis;
      	miBoolean		rat;       /* Is  basis rat or not */
} miGeomoSurface;


#define miGEOMO_MAX_NO_SURFACES	10

/*
 * Prototypes for geomoblend
 */

miBoolean mi_geomo_blend_surfaces
      ( miTag           	*geom_tags,
	miGeoIndex		no_surfaces,
        miGeoIndex		*face_index,
      	miGeoIndex		*trim_index,
        miBoolean   	    	*isoparm,
      	miUshort    	    	*fixed_dir,
      	miGeoScalar 	    	*fixed_parm,
        miGeoScalar 	    	*tension,
	miGeoScalar		*tangent_scale,
        miGeoScalar		tol,
	miGeoIndex		subdiv_depth,
	miGeoIndex		*no_curves,
        miGeomoSurface	    	**surfaces );

/*
 * Prototypes for geomomodel
 */

miBoolean mi_geomo_u_loft
      ( miGeomoCurve            *curves,
        miGeoIndex              no_curves,
        miGeomoSurface          *surface,
        miBoolean               clamped,
        miBoolean               curve_from_surface,
        miGeomoCurve          	*tangents,
        miBoolean		close,
      	miGeoScalar		tolerance,
        miBoolean   	    	autoalign );

void mi_geomo_free_surface
      ( miGeomoSurface            *s );

/*
 * Prototypes for geomospcurve
 */

miBoolean mi_geomo_concatenate_curves
      ( miTag			geom_tag,
        miGeomoCurve            *curve,
        miGeoIndex              scurve_index );

miBoolean mi_geomo_homogenize_curves
      ( miGeomoCurve            *curves,
        miGeoIndex              no_curves );


/*
 * Prototypes for geomoconvert
 */

miBoolean mi_geomo_extract_and_convert_curve
      ( miTag   		geom_tag,
        miGeoIndex		face_index,
      	miGeoIndex		trim_index,
        miGeoScalar		tolerance,
        miGeoScalar		tangent_scale,
      	miGeomoCurve		*c,
      	miGeomoCurve		*tangents );     

void mi_geomo_init_tangent
      ( miGeomoCurve		*tangent );       


/*
 * Prototypes for geomouvloft
 */

miBoolean mi_geomo_uv_loft
      ( miGeomoCurve 		*u_curves,
	miGeoIndex 		no_u_curves,
	miGeomoCurve 		*v_curves,
	miGeoIndex 		no_v_curves,
	miGeomoSurface 		*surface,
	miGeoScalar		tolerance );


/*
 * Prototypes for geomoutils
 */

void mi_geomo_free_curves
      ( miGeomoCurve            *c,
        miGeoIndex              no_curves );

/*end_matter_geom_h*/

/*------------ mi_gapmi.h ------------*/

/*begin_matter_geom_h*/
/*
 * Prototypes for object evaluation
 * leaf_inst must be an miInstance instancing the object
 * used for evaluation.
 */

typedef struct miGeoshaderObjectApprox miGeoshaderObjectApprox;

miGeoshaderObjectApprox *mi_geoshader_approximate_object_begin
      ( miState			*state,
	miTag			leaf_inst );

/* elem_index is either a face or a spacecurve index */

miTag mi_geoshader_approximate_object
      ( miGeoshaderObjectApprox	*gm,
	miGeoIndex		elem_index);

miBoolean mi_geoshader_approximate_object_end(
	miGeoshaderObjectApprox	*gm);

/* 
 * mi_geoshader_eval_surface evaluates a surface at
 * parameter values uv. The 3D position is returned
 * in "pos", the normal vector in "normal" and
 * the derivatives in the dx fields. These values
 * are computed and stored if the pointers in
 * miGeoshaderEvalObj are nonzero.
 * The surface index must be given with "elem_index".
 */

typedef struct {
	miGeoVector	*pos;
	miGeoVector	*normal;
	miGeoVector	*dxdu;
	miGeoVector	*dxdv;
	miGeoVector	*d2xdu2;
	miGeoVector	*d2xdv2;
	miGeoVector	*d2xdudv;
} miGeoshaderEvalObj;

miBoolean mi_geoshader_eval_surface
      ( miGeoshaderEvalObj	*result,
	miGeoshaderObjectApprox	*gm,
	miGeoIndex		elem_index,
	miGeoVector2d		*uv );

/* 
 * mi_geoshader_eval_curve evaluates a curve for
 * parameter value t, returns 3D position in uv
 * if nonzero, the first derivative in dt if
 * nonzero and the second derivatives in dt2
 * if nonzero. The curve index into the list
 * of curves must be given with "curve_index".
 */

typedef struct {
	miGeoVector2d	*uv;
	miGeoVector2d	*dt;
	miGeoVector2d	*dt2;
} miGeoshaderEvalCurve;

miBoolean mi_geoshader_eval_curve
      ( miGeoshaderEvalCurve	*result,
	miGeoshaderObjectApprox *gm,
	miGeoIndex		curve_index,
	miGeoScalar		t );

/*
 * adjacency detection
 *
 * This function searches for neighboring surfaces in the
 * current object using all faces. The adjacency information
 * is returned in "connections", see the scene documentation
 * for details on this data structure.
 * "merge_epsilon" is used when merging trim curves. See
 * raylib documentation for merge group epsilon.
 * The merged curves are returned in "approx_curves". Each
 * face in the current object has a corresponding entry
 * in "approx_curves[face_index]". The number of merged
 * curves is stored in "no_approx_curves[face_index]" where
 * again each face as an integer entry.
 * See the scene documentation for details on "miCurve_point".
 * The merged 3D position is stored in miCurve_point::v, it
 * can be looked up using mi_geoshader_eval_curve and
 * mi_geoshader_eval_surface using miCurve_point::uv.
 * mi_geoshader_release_adjacency should be called to release
 * memory allocated by mi_geoshader_get_adjacency.
 */

typedef struct {
    	enum miCurve_type   type;
    	miBoolean           new_loop;
	miCurve_point	    *curve_points;
	miUint		    no_curve_points;	
} miGeoshaderApproximated;

typedef struct {
	miConnection * connections;
	miUint no_connections;
	miGeoshaderApproximated ** approx_curves;
	miUint * no_approx_curves;
} miGeoshaderObjectAdjacency;

miBoolean mi_geoshader_get_adjacency
      ( miGeoshaderObjectApprox * gm,
        miGeoScalar		merge_epsilon,
	miGeoshaderObjectAdjacency * adj );

void mi_geoshader_release_adjacency
      ( miGeoshaderObjectApprox * gm,
	miGeoshaderObjectAdjacency * adj );
/*end_matter_geom_h*/

/*------------ mi_link.h ------------*/

/*begin_matter_geom_h*/
miFunction_ptr mi_link_lookup(
    const char* name);		/* name of function to be looked up */
/*end_matter_geom_h*/

/*------------ mi_echo.h ------------*/

/*begin_matter_geom_h*/
#define miEO_INST_GROUP	   0x00000001		/* instance of a group */
#define miEO_INST_OBJECT   0x00000002		/* instance of a object/box */
#define miEO_INST_CAMERA   0x00000004		/* instance of a camera */
#define miEO_INST_LIGHT	   0x00000008		/* instance of a light */
#define miEO_INST_FUNCTION 0x00000010		/* instance of a geom shader */
#define miEO_GROUP	   0x00000020		/* group of instances */
#define miEO_OBJECT	   0x00000040		/* geometric object or box */
#define miEO_CAMERA	   0x00000080		/* camera */
#define miEO_LIGHT	   0x00000100		/* light source */
#define miEO_MATERIAL	   0x00000200		/* material */
#define miEO_IMAGE	   0x00000400		/* pixel rectangle */
#define miEO_OPTIONS	   0x00000800		/* options block */
#define miEO_FUNCTION	   0x00001000		/* shader (dont_echo: named) */
#define miEO_FUNCTION_DECL 0x00002000		/* shader declaration */
#define miEO_USERDATA	   0x00004000		/* user data block */

typedef struct {
	miBoolean	prefer_approx_polygons;	/* triangles from polygons */
	miBoolean	prefer_approx_faces;	/* triangles from surfaces */
	miBoolean	ascii_output;		/* non-binary output */
	miBoolean	verbatim_textures;	/* dump textures verbatim ? */
	miBoolean	norendercommand;	/* disable rendercmd echo? */
	miUint		explode_objects;	/* write objects to subfiles */
	miTag		leaf_insts;		/* for prefer_approx_* lookup*/
	miBoolean	nolinkcommand;		/* don't echo link statements*/
	miUint		dont_echo;		/* EO_* bitmap: omit these */
	miUint		dont_recurse;		/* EO_* bitmap: no prereqs */
	miBoolean	incremental;		/* no re-echo of prev frame */
} miEchoOptions;
/*end_matter_geom_h*/
/*begin_matter_geom_h*/
miBoolean mi_geoshader_echo_tag(
	FILE		*fp,			/* file to print to */
	miTag		tag,			/* scene element for echo */
	miEchoOptions	*options);		/* echo options */
/*end_matter_geom_h*/

#ifdef __cplusplus
}
#endif

#endif
