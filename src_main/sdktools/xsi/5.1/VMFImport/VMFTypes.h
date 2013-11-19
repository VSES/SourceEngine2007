#ifndef _MAPTYPES_H_
#define _MAPTYPES_H_

#define		MAX_FACES				16
#define		MAX_POINTS_ON_WINDING	64

typedef float	vec5_t[5];
typedef float	vec3_t[3];

typedef struct
{
	int		numpoints;
	vec5_t	points[1024];			// variable sized
	int		uvidx[1024];

} winding_t;

typedef struct
{
	vec3_t		normal;
	float		dist;
} plane_t;

typedef struct
{
	char	texture[16];
	float	rotate;
	float	shift[2];
	float	scale[2];
} texturedef_t;

typedef struct
{
	float	power;
	vec3_t	startposition;
	float	elevation;
	float	subdiv;
	vec3_t	*normals;
	float	*distances;
	vec3_t	*offsets;
	vec3_t	*offset_normals;
	float	*alphas;

} dispinfo_t;

typedef struct
{
// implicit rep
	vec3_t			planepts[3];
	texturedef_t	texture;

// cached rep
	plane_t			plane;
	void			*qtexture;
	float			light;		// 0 - 1.0
	winding_t		*w;

	vec3_t			uaxis;
	vec3_t			vaxis;
	float			ushift;
	float			vshift;
	float			uscale;
	float			vscale;
	
	char			texturefile[256];
	
	dispinfo_t		*dispinfo;
} face_t;

#define	ON_EPSILON		0.1
#define	FP_EPSILON		0.01
#define	VECTOR_EPSILON	0.0001

#define	SIDE_FRONT		0
#define	SIDE_BACK		1
#define	SIDE_ON			2

/*
winding_t *ClipWinding (winding_t *in, plane_t *split);
winding_t *CopyWinding (winding_t *w);
winding_t *NewWinding (int points);


@interface SetBrush : Object
{
	BOOL		regioned;		// not active
	BOOL		selected;

	BOOL		invalid;		// not a proper polyhedron

	id			parent;			// the entity this brush is in
	vec3_t		bmins, bmaxs;
	vec3_t		entitycolor;
	int			numfaces;
	face_t		faces[MAX_FACES];
}

- initOwner: own mins:(float *)mins maxs:(float *)maxs texture:(texturedef_t *)tex;
- initFromTokens: own;
- setMins:(float *)mins maxs:(float *)maxs;

- parent;
- setParent: (id)p;

- setEntityColor: (vec3_t)color;

- calcWindings;

- writeToFILE: (FILE *)f region: (BOOL)reg;

- (BOOL)selected;
- (BOOL)regioned;
- setSelected: (BOOL)s;
- setRegioned: (BOOL)s;

- getMins: (vec3_t)mins maxs: (vec3_t)maxs;

- (BOOL)containsPoint: (vec3_t)pt;

- freeWindings;
- removeIfInvalid;

extern	vec3_t	region_min, region_max;
- newRegion;

- (texturedef_t *)texturedef;
- (texturedef_t *)texturedefForFace: (int)f;
- setTexturedef: (texturedef_t *)tex;
- setTexturedef: (texturedef_t *)tex forFace:(int)f;

- XYDrawSelf;
- ZDrawSelf;
- CameraDrawSelf;
- XYRenderSelf;
- CameraRenderSelf;

- hitByRay: (vec3_t)p1 : (vec3_t) p2 : (float *)time : (int *)face;

//
// single brush actions
//
extern	int		numcontrolpoints;
extern	float	*controlpoints[MAX_FACES*3];
- getZdragface: (vec3_t)dragpoint;
- getXYdragface: (vec3_t)dragpoint;
- getXYShearPoints: (vec3_t)dragpoint;

- addFace: (face_t *)f;

//
// multiple brush actions
//
- carveByClipper;

extern	vec3_t	sb_translate;
- translate;

extern	id		carve_in, carve_out;
- select;
- deselect;
- remove;
- flushTextures;

extern	vec3_t	sb_mins, sb_maxs;
- addToBBox;

extern	vec3_t	sel_x, sel_y, sel_z;
extern	vec3_t	sel_org;
- transform;

- flipNormals;

- carve;
- setCarveVars;

extern	id	sb_newowner;
- moveToEntity;

- takeCurrentTexture;

extern	vec3_t	select_min, select_max;
- selectPartial;
- selectComplete;
- regionPartial;
- regionComplete;

extern	float	sb_floor_dir, sb_floor_dist;
- feetToFloor;

- (int) getNumBrushFaces;
- (face_t *)getBrushFace: (int)which;


*/

inline void DSA2WHelper( LPWSTR out_wcs, LPCSTR in_sz, int in_cch = -1 )
{
	if ( out_wcs != NULL && 0 != in_cch )
	{
		out_wcs[0] = L'\0' ;

		if ( in_sz != NULL )
		{
			// Right now, we can't really test the return value because mbstowcs() is
			// mapped to strcpy() on the Irix o32 build. When this build is not done
			// anymore, the != 0 test should be updated.

			size_t l_iLen = 0;
			l_iLen = ::mbstowcs( out_wcs, in_sz, ( in_cch < 0 ) ? ::strlen(in_sz) + 1 : (size_t) in_cch ) ;
			assert( (int)l_iLen != -1 );

        		if ( in_cch > 0 ) { out_wcs[ in_cch - 1 ] = L'\0'; }
		}
	}
}

#define DSA2W(out_wcs, in_sz) \
	if (NULL == (LPCSTR)(in_sz)) \
		*(out_wcs) = NULL; \
	else \
	{ \
		*(out_wcs) = (LPWSTR)alloca((strlen((in_sz)) + 1) * sizeof(WCHAR)); \
		DSA2WHelper(*(out_wcs), (in_sz)); }\
	

#endif
