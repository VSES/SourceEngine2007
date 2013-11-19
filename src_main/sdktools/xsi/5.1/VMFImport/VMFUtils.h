#ifndef _MAPUTILS_H_
#define _MAPUTILS_H_

#define	BOGUS_RANGE	18000
#include <SIBCUtil.h>
#include "VMFTypes.h"

winding_t *	BasePolyForPlane (face_t *f);
winding_t *	ClipWinding (winding_t *in, plane_t *split);
void		CheckFace (face_t *f);
winding_t *	NewWinding (int points);
float		TextureAxisFromPlane(plane_t *pln, float *xv, float *yv);

void		CrossProduct	(vec3_t v1, vec3_t v2, vec3_t cross);
SI_Bool		VectorCompare	(vec3_t v1, vec3_t v2);
void		VectorNormalize (vec3_t v);
void		VectorMA (vec3_t va, double scale, vec3_t vb, vec3_t vc);
void		VectorScale (vec3_t v, SI_Float scale, vec3_t out);
double		VectorLength(vec3_t v);

int GetAxisAlignIndex(vec3_t in_vNormal);
void GetUV(vec3_t in_vNormal, 
		   vec3_t  in_vVertex, 
		   float &s, 
		   float &t, 
		   float in_fAngle, 
		   float in_fOffsetX, 
		   float in_fOffsetY, 
		   float in_fScaleX,
		   float in_fScaleY
		   );

#define DotProduct(x,y) (x[0]*y[0]+x[1]*y[1]+x[2]*y[2])
#define VectorSubtract(a,b,c) {c[0]=a[0]-b[0];c[1]=a[1]-b[1];c[2]=a[2]-b[2];}
#define VectorAdd(a,b,c) {c[0]=a[0]+b[0];c[1]=a[1]+b[1];c[2]=a[2]+b[2];}
#define VectorCopy(a,b) {b[0]=a[0];b[1]=a[1];b[2]=a[2];}

// float to float
#define ROUND(X) ( (X) > 0.0f ? \
	( (float) ( (int) ( (X) + 0.5f) ) ) : \
	( (float) ( (int) ( (X) - 0.5f) ) ) )

#define ROUND0(X, I) ( ( (X) * (I) ) > 0.0f ? \
	( ( (float) ( (int) ( (X) * (I) + 0.5f) ) ) / (I) ): \
	( ( (float) ( (int) ( (X) * (I) - 0.5f) ) ) / (I) ) )
#define ROUND1(X) (ROUND0((X), 10.0f))
#define ROUND2(X) (ROUND0((X), 100.0f))
#define ROUND3(X) (ROUND0((X), 1000.0f))
#define ROUND4(X) (ROUND0((X), 10000.0f))
#define ROUND5(X) (ROUND0((X), 100000.0f))
#define ROUND6(X) (ROUND0((X), 1000000.0f))

// float to int
#define ROUNDI(X) ((X) > 0.0f ? \
	(((int) ((X) + 0.5f))) : \
	(((int) ((X) - 0.5f))))

#ifndef PI
#define PI (3.1415926535897932385f)
#endif
#define RADIAN (PI / 180.0f)
#define RAD2DEG(X) ((X) / RADIAN)
#define DEG2RAD(X) ((X) * RADIAN)

#define MAX_LEVEL_SIZE_IN_UNITS		16384


#endif
