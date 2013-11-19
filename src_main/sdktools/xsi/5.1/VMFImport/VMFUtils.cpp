#include "stdafx.h"
#include "VMFUtils.h"

extern vec3_t vec3_origin;
float  lightaxis[3] = {1.0f, 0.6f, 0.75f};

//////////////////////////////////////////////////////////////////
// The following is code inspired by John Carmack's QuakeEd app.
// It was ported from Objective C and modified to fit
// my architecture.
// ref. http://www.gamers.org/dEngine/quake/QuakeEd/QuakeEd.html
//////////////////////////////////////////////////////////////////

void CrossProduct (vec3_t v1, vec3_t v2, vec3_t cross)
{
	cross[0] = v1[1]*v2[2] - v1[2]*v2[1];
	cross[1] = v1[2]*v2[0] - v1[0]*v2[2];
	cross[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

SI_Bool VectorCompare (vec3_t v1, vec3_t v2)
{
	int		i;
	
	for (i=0 ; i<3 ; i++)
		if (v1[i] != v2[i])
			return SI_FALSE;
			
	return SI_TRUE;
}

void VectorNormalize (vec3_t v)
{
	int		i;
	float	length;
	
	length = 0;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrtf (length);

	for (i=0 ; i< 3 ; i++)
		v[i] /= length;	
}

void VectorMA (vec3_t va, double scale, vec3_t vb, vec3_t vc)
{
	vc[0] = va[0] + scale*vb[0];
	vc[1] = va[1] + scale*vb[1];
	vc[2] = va[2] + scale*vb[2];
}

void VectorScale (vec3_t v, SI_Float scale, vec3_t out)
{
	out[0] = v[0] * scale;
	out[1] = v[1] * scale;
	out[2] = v[2] * scale;
}

double VectorLength(vec3_t v)
{
	int		i;
	double	length;
	
	length = 0;
	for (i=0 ; i< 3 ; i++)
		length += v[i]*v[i];
	length = sqrt (length);		// FIXME

	return length;
}


winding_t *BasePolyForPlane (face_t *f)
{
	
	int		i, x;
	float	max, v;
	vec3_t	org, vright, vup;
	vec3_t	xaxis, yaxis;
	winding_t	*w;
	texturedef_t	*td;
	plane_t		*p;
	float	ang, sinv, cosv;
	float	s, t, ns, nt;
	
	p = &f->plane;
	
// find the major axis

	max = -BOGUS_RANGE;
	x = -1;
	for (i=0 ; i<3; i++)
	{
		v = (float)fabs(p->normal[i]);
		if (v > max)
		{
			x = i;
			max = v;
		}
	}
	if (x==-1)
		OutputDebugString ("BasePolyForPlane: no axis found");
		
	VectorCopy (vec3_origin, vup);	
	switch (x)
	{
	case 0:
	case 1:
		vup[2] = 1;
		break;		
	case 2:
		vup[0] = 1;
		break;		
	}

	v = DotProduct (vup, p->normal);
	VectorMA (vup, -v, p->normal, vup);
	VectorNormalize (vup);
		
	VectorScale (p->normal, p->dist, org);
	
	CrossProduct (vup, p->normal, vright);
	
	VectorScale (vup, MAX_LEVEL_SIZE_IN_UNITS, vup);
	VectorScale (vright, MAX_LEVEL_SIZE_IN_UNITS, vright);

// project a really big	axis aligned box onto the plane
	w = NewWinding (4);
	w->numpoints = 4;
	
	VectorSubtract (org, vright, w->points[0]);
	VectorAdd (w->points[0], vup, w->points[0]);
	
	VectorAdd (org, vright, w->points[1]);
	VectorAdd (w->points[1], vup, w->points[1]);
	
	VectorAdd (org, vright, w->points[2]);
	VectorSubtract (w->points[2], vup, w->points[2]);
	
	VectorSubtract (org, vright, w->points[3]);
	VectorSubtract (w->points[3], vup, w->points[3]);
	
// set texture values
	f->light = TextureAxisFromPlane(&f->plane, xaxis, yaxis);
	td = &f->texture;
	
// rotate axis
	ang = td->rotate / 180 * M_PI;
	sinv = sinf(ang);
	cosv = cosf(ang);
	
	if (!td->scale[0])
		td->scale[0] = 1;
	if (!td->scale[1])
		td->scale[1] = 1;

	for (i=0 ; i<4 ; i++)
	{
		s = DotProduct (w->points[i], xaxis);
		t = DotProduct (w->points[i], yaxis);

		ns = cosv * s - sinv * t;
		nt = sinv * s +  cosv * t;

		w->points[i][3] = ns/td->scale[0] + td->shift[0];
		w->points[i][4] = nt/td->scale[1] + td->shift[1];
	}
	
	return w;	
}


winding_t *ClipWinding (winding_t *in, plane_t *split)
{
	
	float	dists[MAX_POINTS_ON_WINDING];
	int		sides[MAX_POINTS_ON_WINDING];
	int		counts[3];
	float	dot;
	int		i, j;
	float	*p1, *p2, *mid;
	winding_t	*neww;
	int		maxpts;
	
	counts[0] = counts[1] = counts[2] = 0;

// determine sides for each point
	for (i=0 ; i<in->numpoints ; i++)
	{
		dot = DotProduct (in->points[i], split->normal);
		dot -= split->dist;
		dists[i] = dot;
		if (dot > ON_EPSILON)
			sides[i] = SIDE_FRONT;
		else if (dot < -ON_EPSILON)
			sides[i] = SIDE_BACK;
		else
		{
			sides[i] = SIDE_ON;
		}
		counts[sides[i]]++;
	}
	sides[i] = sides[0];
	dists[i] = dists[0];
	
	if (!counts[0] && !counts[1])
		return in;
		
	if (!counts[0])
	{
		free (in);
		return 0;
		//return in;
	}
	if (!counts[1])
		return in;
	
	maxpts = in->numpoints+4;	// can't use counts[0]+2 because
								// of fp grouping errors
	neww = NewWinding (maxpts);
		
	for (i=0 ; i<in->numpoints ; i++)
	{
		p1 = in->points[i];
		
		mid = neww->points[neww->numpoints];

		if (sides[i] == SIDE_FRONT || sides[i] == SIDE_ON)
		{
			VectorCopy (p1, mid);
			mid[3] = p1[3];
			mid[4] = p1[4];
			neww->numpoints++;
			if (sides[i] == SIDE_ON)
				continue;
			mid = neww->points[neww->numpoints];
		}
		
		if (sides[i+1] == SIDE_ON || sides[i+1] == sides[i])
			continue;
			
	// generate a split point
		if (i == in->numpoints - 1)
			p2 = in->points[0];
		else
			p2 = p1 + 5;
		
		neww->numpoints++;
		
		dot = dists[i] / (dists[i]-dists[i+1]);
		for (j=0 ; j<3 ; j++)
		{	// avoid round off error when possible
			if (split->normal[j] == 1)
				mid[j] = split->dist;
			else if (split->normal[j] == -1)
				mid[j] = -split->dist;
			mid[j] = p1[j] + dot*(p2[j]-p1[j]);
		}
		mid[3] = p1[3] + dot*(p2[3]-p1[3]);
		mid[4] = p1[4] + dot*(p2[4]-p1[4]);
	}

	if (neww->numpoints > maxpts)
		OutputDebugString ("ClipWinding: points exceeded estimate");

// free the original winding
	free (in);

	return neww;
}

void CheckFace (face_t *f)
{
	
	int		i, j;
	float	*p1, *p2;
	float	d, edgedist;
	vec3_t	dir, edgenormal;
	winding_t	*w;
	
	w = f->w;
	if (!w)
		OutputDebugString ("CheckFace: no winding");
		
	if (w->numpoints < 3)
		OutputDebugString ("CheckFace greater that 3 points");
	
	for (i=0 ; i<w->numpoints ; i++)
	{
		p1 = w->points[i];

		for (j=0 ; j<3 ; j++)
			if (p1[j] > BOGUS_RANGE || p1[j] < -BOGUS_RANGE)
				OutputDebugString ("CheckFace: BUGUS_RANGE");

		j = i+1 == w->numpoints ? 0 : i+1;
		
	// check the point is on the face plane
		d = DotProduct (p1, f->plane.normal) - f->plane.dist;
		if (d < -ON_EPSILON || d > ON_EPSILON)
			OutputDebugString ("CheckFace: point off plane");
	
	// check the edge isn't degenerate
		p2 = w->points[j];
		VectorSubtract (p2, p1, dir);
		
		if (VectorLength (dir) < ON_EPSILON)
			OutputDebugString ("CheckFace: degenerate edge");
			
		CrossProduct (f->plane.normal, dir, edgenormal);
		VectorNormalize (edgenormal);
		edgedist = DotProduct (p1, edgenormal);
		edgedist += ON_EPSILON;
		
	// all other points must be on front side
		for (j=0 ; j<w->numpoints ; j++)
		{
			if (j == i)
				continue;
			d = DotProduct (w->points[j], edgenormal);
			if (d > edgedist)
				OutputDebugString ("CheckFace: non-convex");
		}
	}
	
}

winding_t *NewWinding (int points)
{
	winding_t	*w;
	int			size;
	
	if (points > MAX_POINTS_ON_WINDING)
		OutputDebugString ("NewWinding error");
	
	//size = (int)((winding_t *)0)->points[points];
	size = sizeof(winding_t) * 2;
	w = (winding_t*)malloc (size);

	
	memset (w, 0, size);
	
	return w;
}


#if 0
vec3_t	baseaxis[18] =
{
{0,0,1}, {1,0,0}, {0,-1,0},			// floor
{0,0,-1}, {1,0,0}, {0,-1,0},		// ceiling
{1,0,0}, {0,1,0}, {0,0,-1},			// west wall
{-1,0,0}, {0,1,0}, {0,0,-1},		// east wall
{0,1,0}, {1,0,0}, {0,0,-1},			// south wall
{0,-1,0}, {1,0,0}, {0,0,-1}			// north wall
};
#else
vec3_t	baseaxis[18] =
{
{0,0,1}, {1,0,0}, {0,-1,0},			// floor
{0,0,-1}, {1,0,0}, {0,1,0},			// ceiling
{1,0,0}, {0,1,0}, {0,0,-1},			// west wall
{-1,0,0}, {0,-1,0}, {0,0,-1},		// east wall
{0,1,0}, {-1,0,0}, {0,0,-1},		// south wall
{0,-1,0}, {1,0,0}, {0,0,-1}			// north wall
};
#endif


float TextureAxisFromPlane(plane_t *pln, float *xv, float *yv)
{
	int		bestaxis;
	float	dot,best;
	int		i;
	
	best = 0;
	bestaxis = 0;
	
	for (i=0 ; i<6 ; i++)
	{
		dot = DotProduct (pln->normal, baseaxis[i*3]);
		if (dot > best)
		{
			best = dot;
			bestaxis = i;
		}
	}
	
	VectorCopy (baseaxis[bestaxis*3+1], xv);
	VectorCopy (baseaxis[bestaxis*3+2], yv);
	
	return lightaxis[bestaxis>>1];
}

int GetAxisAlignIndex(vec3_t in_vNormal) 
{
	float z, zMax = 0.0f;
	int axisAlign;
	
	// Round the norm vector due to floating precision errors.
	// Check East & West.
	z = ROUND4(in_vNormal[0]);
	if (z > zMax) {
		zMax = z;
		axisAlign = 4;
	}
	z = ROUND4(-in_vNormal[0]);
	if (z > zMax) {
		zMax = z;
		axisAlign = 5;
	}
	
	// Check North and South
	z = ROUND4(in_vNormal[1]);
	if (z > zMax) {
		zMax = z;
		axisAlign = 2;
	}
	z = ROUND4(-in_vNormal[1]);
	if (z > zMax) {
		zMax = z;
		axisAlign = 3;
	}
	
	// Check Up and Down
	z = ROUND4(in_vNormal[2]);
	if (z > zMax) {
		zMax = z;
		axisAlign = 0;
	}
	z = ROUND4(-in_vNormal[2]);
	if (z > zMax) {
		zMax = z;
		axisAlign = 1;
	}
	
	return axisAlign;
}

void GetUV(vec3_t in_vNormal, 
		   vec3_t  in_vVertex, 
		   float &s, 
		   float &t, 
		   float in_fAngle, 
		   float in_fOffsetX, 
		   float in_fOffsetY, 
		   float in_fScaleX,
		   float in_fScaleY
		   )
{
	int align = GetAxisAlignIndex(in_vNormal);
	
	if (align < 2) {  // Up & Down.
		// Map to the XY plane.
		s = in_vVertex[0];
		t = in_vVertex[1];
	}
	else if (align < 4) { // North & South
		// Map to the XZ plane.
		s = in_vVertex[0];
		t = in_vVertex[2];
	}
	else { // East & West.
		// Map to YZ plane
		s = in_vVertex[1];
		t = in_vVertex[2];
	}
	
	// Rotate it.
	float rcos = (float) cos(DEG2RAD(-in_fAngle));
	float rsin = (float) sin(DEG2RAD(-in_fAngle));
	float x = s * rcos - t * rsin;
	float y = s * rsin + t * rcos;
	s = x;
	t = y;
	
	// Scale it.
	if (in_fScaleX != 0.0f)
		s /= in_fScaleX;
	if (in_fScaleY != 0.0f)
		t /= in_fScaleY;
	
	// Offset
	s += in_fOffsetX;
	t -= in_fOffsetY;
	
	// Flip y coord.
	t = -t;
}
