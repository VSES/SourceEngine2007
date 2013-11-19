// BrushConverter.cpp: implementation of the CBrushConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BrushConverter.h"
#include "VMFUtils.h"

vec3_t vec3_origin = {0,0,0};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBrushConverter::CBrushConverter()
{

}

CBrushConverter::~CBrushConverter()
{
	for (int f=0;f<m_pFaceList.GetUsed();f++)
	{
		if ( m_pFaceList[f].dispinfo != NULL )
		{
			dispinfo_t *l_pdispinfo = m_pFaceList[f].dispinfo;

			delete [] l_pdispinfo->normals;
			delete [] l_pdispinfo->distances;
			delete [] l_pdispinfo->offsets;
			delete [] l_pdispinfo->offset_normals;
			delete [] l_pdispinfo->alphas;
			delete l_pdispinfo;

		}

		if ( m_pFaceList[f].w )
		{
			free ( m_pFaceList[f].w );
		}
	}

	m_pFaceList.DisposeData();

}

SI_Error	CBrushConverter::AddFace	( face_t& in_pFace )
{
	m_pFaceList.Extend(1);
	m_pFaceList[m_pFaceList.GetUsed()-1] = in_pFace;

	return SI_SUCCESS;
}

face_t*		CBrushConverter::GetFace		( SI_Int in_iIndex )
{

	return &m_pFaceList[in_iIndex];

}

SI_Int		CBrushConverter::GetNbFace	()
{

	return m_pFaceList.GetUsed();

}

SI_Error	CBrushConverter::Convert ( bool* out_bDisp )
{
	int				i,j, k;
	float			v;
	face_t			*f;
	winding_t		*w;
	plane_t			plane;
	vec3_t			t1, t2, t3;
	SI_Bool			*useplane;

	useplane = new SI_Bool [  GetNbFace() ] ;


	bmins[0] = bmins[1] = bmins[2] = 99999;
	bmaxs[0] = bmaxs[1] = bmaxs[2] = -99999;
	m_bInvalid = SI_FALSE;
	
//	[self freeWindings];

	
	for (i=0 ; i<GetNbFace() ; i++)
	{
		f = &m_pFaceList[i];
		
		// calc a plane from the points
		for (j=0 ; j<3 ; j++)
		{
			t1[j] = f->planepts[0][j] - f->planepts[1][j];
			t2[j] = f->planepts[2][j] - f->planepts[1][j];
			t3[j] = f->planepts[1][j];
		}
		
		CrossProduct(t1,t2, f->plane.normal);
		if (VectorCompare (f->plane.normal, vec3_origin))
		{
			useplane[i] = SI_FALSE;
			break;
		}
		VectorNormalize (f->plane.normal);
		f->plane.dist = DotProduct (t3, f->plane.normal);
		
	// if the plane duplicates another plane, ignore it
	// (assume it is a brush being edited that will be fixed)
		useplane[i] = SI_TRUE;
		for (j=0 ; j< i ; j++)
		{
			face_t*	l_pDup =  &m_pFaceList[j];

			if ( f->plane.normal[0] == l_pDup->plane.normal[0]
			&& f->plane.normal[1] == l_pDup->plane.normal[1]
			&& f->plane.normal[2] == l_pDup->plane.normal[2]
			&& f->plane.dist == l_pDup->plane.dist )
			{
				useplane[i] = SI_FALSE;
				break;
			}
		}
	
	}
	
	for (i=0 ; i<GetNbFace() ; i++)
	{
		if (!useplane[i])
			continue;			// duplicate plane
			
		f = &m_pFaceList[i];

		w = BasePolyForPlane (f);
		for (j=0 ; j<GetNbFace() && w ; j++)
		{
			if (j == i)
				continue;

			face_t*	l_pFace =  &m_pFaceList[j];

		// flip the plane, because we want to keep the back side
			VectorSubtract (vec3_origin, l_pFace->plane.normal, plane.normal);
			plane.dist = -l_pFace->plane.dist;
			
			w = ClipWinding (w, &plane);
		}
		f->w = w;
		if (w)
		{
			CheckFace (f);
			for (j=0 ; j<w->numpoints ; j++)
			{
				for (k=0 ; k<3 ; k++)
				{
					v = w->points[j][k];
					//if (fabs(v - (int)(v)) < FP_EPSILON)
					//	v = w->points[j][k] = (int)(v);
					if (v < bmins[k])
						bmins[k] = v;
					if (v > bmaxs[k])
						bmaxs[k] = v;
				}
			}
		} else {
			int a = 10;
		}
	}	
	
	if (bmins[0] == 99999)
	{
		m_bInvalid = SI_TRUE;
		VectorCopy (vec3_origin, bmins);
		VectorCopy (vec3_origin, bmaxs);
		return SI_ERR_ERROR_MSG;
	}
	

	*out_bDisp = false;

	for (int r=0;r<GetNbFace();r++)
	{

		if ( m_pFaceList[r].dispinfo != NULL )
			*out_bDisp = true;

		RegisterFace ( &m_pFaceList[r] );
	}

	delete [] useplane;

	return SI_SUCCESS;
}


void CBrushConverter::CreateCluster	( char *in_szClusterName )
{
	m_pClusterList.Extend(1);
	BrushCluster*	l_pNewCluster = &m_pClusterList[m_pClusterList.GetUsed()-1];

	l_pNewCluster->m_szClusterName = in_szClusterName;

}

BrushCluster* CBrushConverter::GetClusterByName( char *in_szClusterName )
{

	for (int c=0;c<m_pClusterList.GetUsed();c++)
	{
		BrushCluster*	l_pNewCluster = &m_pClusterList[c];

		if ( !strcmp ( l_pNewCluster->m_szClusterName.GetText(), in_szClusterName ) )
		{
			return l_pNewCluster;
		}
	}

	return NULL;

}

void	CBrushConverter::RegisterFace ( face_t* in_pFace )
{
	BrushCluster*	l_pBrushCluster = NULL;

	//
	// Get the cluster in which this face will end up
	//

	l_pBrushCluster = GetClusterByName ( in_pFace->texturefile );

	if ( l_pBrushCluster == NULL )
	{
		//
		// The cluster doesnt exist, so create it here
		//

		CreateCluster ( in_pFace->texturefile  );

		l_pBrushCluster = GetClusterByName ( in_pFace->texturefile );

	}

	l_pBrushCluster->m_pFaceList.Extend(1);
	l_pBrushCluster->m_pFaceList[ l_pBrushCluster->m_pFaceList.GetUsed()-1] = in_pFace;
}
